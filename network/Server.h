/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Server.h
 * Author: nyrmburk
 *
 * Created on May 12, 2018, 9:39 PM
 */

#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <functional>

#include <uWS/uWS.h>

#include "RobotProtocol.h"

using namespace uWS;

class Server : public RobotProtocol<WebSocket<SERVER>*> {
public:
    Server(Robot& robot): RobotProtocol<WebSocket<SERVER>*>(robot) {};

    void send(WebSocket<SERVER>*& remote, DataView& data) {
        size_t bytesWritten = data.getIndex();
        if (bytesWritten > 100) {
            std::cout << bytesWritten << " bytes written" << std::endl;
        }
        remote->send(data.getBuffer(), bytesWritten, OpCode::BINARY);
    }
    
    void run(std::function<void(int)> step, int millis) {
        Hub h;
        std::string response = "Hello!";

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

        h.onMessage([&](WebSocket<SERVER> *ws, char *message, size_t length, OpCode opCode) {
            try {
                handleMessage(ws, message, length);
            } catch (std::out_of_range e) {
                cout << "client overran their buffer" << endl;
                ws->close();
            }
        });

        h.onConnection([this](WebSocket<SERVER> *ws, HttpRequest req) {
            onConnect(ws);
        });

        h.onDisconnection([this](WebSocket<SERVER> *ws, int code, char* message, size_t length) {
            onDisconnect(ws);
        });

        h.onHttpRequest([&](HttpResponse *res, HttpRequest req, char *data, size_t length,
                            size_t remainingBytes) {
            res->end(response.data(), response.length());
        });

        if (h.listen(8080)) {
            h.run();
        }
    }
};

#endif /* SERVER_H */

