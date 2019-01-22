#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <map>
#include <vector>

#include "../helper/Serializable.h"
#include "../mapping/JoystickPlan.h"

#define ROOM_ALL 0

template <typename T>
class Protocol {
public:
    Protocol(size_t size = 1024 * 1024): buffer(size) {}

    enum Opcode {
        opPublish = 10,
        opSubscribe,
        opUnsubscribe,
    };

    virtual void handle(T& remote, uint16_t opcode, DataView& data) = 0;

    void handleMessage(T& remote, const char* dataIn, const size_t sizeIn) {
        DataView data(const_cast<char*>(dataIn), sizeIn);
        uint16_t opcode = data.readU16();
 
        uint16_t roomId = 0;
        switch(opcode) {
            case opPublish:
                roomId = data.readU16();
                publish(roomId, data);
                break;
            case opSubscribe:
                roomId = data.readU16();
                subscribe(roomId, remote);
                break;
            case opUnsubscribe:
                roomId = data.readU16();
                unsubscribe(roomId, remote);
                break;
            default:
                return handle(remote, opcode, data);
        }
    }

    virtual void send(T& remote, DataView& data) = 0;
    void send(T& remote, uint16_t opcode, std::function<void(DataView&)> write) {
        // use internal buffer
        DataView data(buffer.data(), buffer.size());
        data.writeU16(opcode);
        write(data);
        send(remote, data);
    }

    void send(T& remote, uint16_t opcode, Serializable* s) {
        send(remote, opcode, [s](DataView& out){s->serialize(&out);});
    }

    virtual void setUserData(T& remote, void* data) = 0;
    virtual void* getUserData(T& remote) = 0;

    virtual void onConnect(T& remote) {
        subscribe(ROOM_ALL, remote);
    }

    virtual void onDisconnect(T& remote) {
        unsubscribeAll(remote);
    }

    void publish(uint16_t roomId, DataView& data) {
        vector<T>& subscribers = rooms[roomId];

        for (size_t i = 0; i < subscribers.size(); i++) {
            send(subscribers[i], data);
        }
    }

    void publish(uint16_t roomId, uint16_t opcode, std::function<void(DataView&)> write) {
        DataView data(buffer.data(), buffer.size());
        data.writeU16(opcode);
        write(data);
        publish(roomId, data);
    }

    void publish(uint16_t roomId, uint16_t opcode, Serializable* s) {
        publish(roomId, opcode, [s](DataView& out){s->serialize(&out);});
    }

    void subscribe(uint16_t roomId, T& subscriber) {
        vector<T>& subscribers = rooms[roomId];
        subscribers.push_back(subscriber);
        cout << "subscribed to room id #" << roomId << endl;
    }

    void unsubscribe(uint16_t roomId, T& subscriber) {
        cout << "unsubscribed from room id #" << roomId << endl;
        vector<T>& subscribers = rooms[roomId];
        subscribers.erase(
                std::remove(subscribers.begin(), subscribers.end(), subscriber),
                subscribers.end());
    }

    void unsubscribeAll(T& subscriber) {
        for (auto it = rooms.begin(); it != rooms.end(); it++) {
            cout << "unsubscribed from room id #" << it->first << endl;
            vector<T>& subscribers = it->second;
            subscribers.erase(
                    std::remove(subscribers.begin(), subscribers.end(), subscriber),
                    subscribers.end());
        }
    }
private:
    map<uint16_t, vector<T>> rooms;
    vector<char> buffer;
};
#endif /* PROTOCOL_H */
