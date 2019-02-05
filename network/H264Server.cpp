#include "H264Server.h"

#include <cstring>
#include <iostream>
#include  <iomanip>

#define NAL_SEPARATOR 0x00000001
#define NAL_SIZE 4

using namespace std;
using namespace uWS;

H264Server::H264Server() {
    camera = popen(
        "raspivid -t 0 -o - -w 640 -h 480 -fps 25 -pf baseline -ih -fl", "r");

    if (!camera) {
        cout << "could not open camera. livestream server exiting." << endl;
        exit(EXIT_FAILURE);
    }

    int fd = camera->_fileno;
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

H264Server::~H264Server() {
    pclose(camera);
}

int split_nal();

void H264Server::run() {

    hub.onConnection([this](WebSocket<SERVER> *ws, HttpRequest req) {
        cout << "new livestream client" << endl;
        const char *initMessage = "{\"action\":\"init\",\"width\":640,\"height\":480}";
        int initLength = strlen(initMessage);

        ws->send(initMessage, initLength, OpCode::TEXT);
    });

    hub.onMessage([](WebSocket<SERVER> *ws, char *message, size_t length, OpCode opCode) {
        char temp[length + 1];
        memcpy(temp, message, length);
        temp[length] = '\0';

        cout << "livestream client: " << temp << endl;
    });

    // set up the timer loop
    uS::Timer *timer = new uS::Timer(hub.getLoop());
    timer->setData(this);
    timer->start([](uS::Timer *timer) {
        H264Server* _this = (H264Server*) timer->getData();
        vector<char>& buffer = _this->buffer;

        ssize_t bytesRead = 0;
        do {
            buffer.reserve(_this->index + BLOCK_SIZE);

            bytesRead = read(_this->camera->_fileno, &buffer.data()[_this->index], BLOCK_SIZE);
            if (ferror(_this->camera)) {
                cout << "camera error. livestream server exiting" << endl;
                exit(EXIT_FAILURE);
            }

            size_t end = _this->index + bytesRead;
            for (int i = 0; i < bytesRead; i++) {
                _this->history = (_this->history << 8) | buffer[_this->index++];
                if (_this->history == NAL_SEPARATOR) {
                    _this->index -= NAL_SIZE;

                    // buffer is ready. sending.
                    _this->hub.getDefaultGroup<uWS::SERVER>()
                            .broadcast(_this->buffer.data(), _this->index, OpCode::BINARY);

                    // move leftover data back to beginning
                    int leftover = end - _this->index;
                    memcpy(buffer.data(), &buffer.data()[_this->index], leftover);
                    _this->index = leftover;
                    break;
                }
            }
        } while (bytesRead > 0);
    }, 0, 1);

    if (hub.listen(8081)) {
        hub.run();
    }
}
