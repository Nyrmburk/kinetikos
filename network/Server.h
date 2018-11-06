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

#include <functional>

class Server {
public:
    Server();
    virtual ~Server();
    
    void run(std::function<void(int)> step, int millis);
private:
};

/*
class ResponseCallback {
private:
//    UWS* ws;
    Writerator *writer;
public: 
    void send(uint16_t controlCode, Serializable* s) {
        writer->writeU16(controlCode);
        s->serialize(&writer);
        //ws->send(writer.data(), writer.getSize(), OpCode.binary); 
    }
};

struct User {
    // int ringbuffer index
    ResponseCallback responseCallback;
};
*/
#endif /* SERVER_H */

