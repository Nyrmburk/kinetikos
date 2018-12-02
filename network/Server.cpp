#include <iostream>
#include <uWS/uWS.h>
#include "Server.h"
#include "RobotProtocol.h"

using namespace uWS;

// check out serveEventSource
// because I need to have some sort of simulation data transmitted to the client on an interval
// this is an example on that feature
// it's got a stupid comment on it saying it's not even a test, but a major example >:(
void Server::run(std::function<void(int)> step, int millis) {

    Hub h;
    std::string response = "Hello!";

    Protocol& p = this->protocol;
    
    std::vector<char> buffer(1024 * 1024);

    // struct for the user data in the simulation step
    struct timerData {
        std::function<void(int)> step;
        int millis;
    } data;
    data.step = step;
    data.millis = millis;

    // set up the timer loop for simulation steps
    uS::Timer *timer = new uS::Timer(h.getLoop());
    timer->setData(&data);
    timer->start([](uS::Timer *timer) {
        timerData* data = (timerData*) timer->getData();
        data->step(data->millis);
    }, 0, millis);

    h.onMessage([&p, &buffer](WebSocket<SERVER> *ws, char *message, size_t length, OpCode opCode) {
        size_t readBytes = 0;
        
        try {
            readBytes = p.handleMessage(message, length, buffer.data(), (size_t)buffer.size());
        } catch (std::out_of_range e) {
            cout << "client overran their buffer" << endl;
            ws->close();
        }

        cout << "wrote out " << readBytes << " bytes." << endl;

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
