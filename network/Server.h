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
#include <chrono>

#include <uWS/uWS.h>

#include "RobotProtocol.h"

using namespace uWS;

class Server : public RobotProtocol<WebSocket<SERVER>*> {
public:
    Server(Robot& robot): RobotProtocol<WebSocket<SERVER>*>(robot) {};

    void send(WebSocket<SERVER>*& remote, DataView& data) {
        size_t bytesWritten = data.getIndex();
        remote->send(data.getBuffer(), bytesWritten, OpCode::BINARY);
        totalBytesWritten += bytesWritten;
    }
    
    void run() {
        h.onMessage([&](WebSocket<SERVER> *ws, char *message, size_t length, OpCode opCode) {
            totalBytesRead += length;
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

        std::string response = "Hello!";
        h.onHttpRequest([&](HttpResponse *res, HttpRequest req, char *data, size_t length,
                            size_t remainingBytes) {
            res->end(response.data(), response.length());
        });

        if (h.listen(8080)) {
            h.run();
        }
    }

    void addTimer(std::function<void(long, int)> step, int millis) {
        // struct for the user data in the time
        struct TimerData {
            std::function<void(long, int)> step;
            int millis;
        };

        TimerData* data = new TimerData();
        data->step = step;
        data->millis = millis;

        // set up the timer loop
        uS::Timer *timer = new uS::Timer(h.getLoop());
        timer->setData(data);
        timer->start([](uS::Timer *timer) {
            TimerData* data = (TimerData*) timer->getData();
            auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(timer->loop->timepoint);
            auto epoch = now_ms.time_since_epoch();
            long now = epoch.count();
            data->step(now, data->millis);
        }, 0, millis);
    }

    unsigned long long getTotalBytesRead() {
        return totalBytesRead;
    }

    unsigned long long getTotalBytesWritten() {
        return totalBytesWritten;
    }
private:
    Hub h;
    unsigned long long totalBytesRead = 0;
    unsigned long long totalBytesWritten = 0;
};

#endif /* SERVER_H */

