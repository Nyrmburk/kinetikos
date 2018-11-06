#include <uWS/uWS.h>
#include "H264Server.h"
#include "Protocol.h"

using namespace uWS;

H264Server::H264Server() {
}

H264Server::~H264Server() {
}

void H264Server::run() {

    Hub h;
    std::string response = "Hello!"; 
    Protocol p;
    
    std::vector<char> buffer;

    h.onMessage([&p, &buffer](WebSocket<SERVER> *ws, char *message, size_t length, OpCode opCode) {
        size_t readBytes = 0;
        
        try {
            readBytes = p.handleMessage(message, length, buffer.data(), (size_t)buffer.size());
        } catch (std::out_of_range e) {
            ws->close();
        }

        ws->send(buffer.data(), readBytes, opCode); 
    });

    h.onHttpRequest([&](HttpResponse *res, HttpRequest req, char *data, size_t length,
                        size_t remainingBytes) {
        res->end(response.data(), response.length());
    });

    if (h.listen(8080)) {
        h.run();
    }
}
