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
    
    h.onMessage([](WebSocket<SERVER> *ws, char *message, size_t length, OpCode opCode) {
    });

    h.onHttpRequest([&](HttpResponse *res, HttpRequest req, char *data, size_t length,
                        size_t remainingBytes) {
    });

    if (h.listen(8080)) {
        h.run();
    }
}
