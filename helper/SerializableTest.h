#include <iostream>
#include <string>
#include "Serializable.h"

class Doggo : public Serializable {
public:
    int legs = 4;
    std::string sound = "woof";
    float height = 4.3;
    double maxSpeed = 9.9999;

    void doSerialize(Writerator* it) {
        it->write32(legs);
        it->writeU8(sound.size() + 1);
        it->writeChars(sound.c_str(), sound.size() + 1);
        it->writeFloat(height);
        it->writeDouble(maxSpeed);
    }

    void doDeserialize(Readerator* it) {
        
        legs =  it->read32();
        uint8_t stringSize = it->readU8();
        char bork[stringSize];
        it->readChars(bork, stringSize);
        sound = std::string(bork, stringSize);
        height = it->readFloat();
        maxSpeed = it->readDouble();
    }

    void print() {
        std::cout << legs << " legs" << std::endl;
        std::cout << "says " << sound << std::endl;
        std::cout << height << " hands tall" << std::endl;
        std::cout << "zooms at " << maxSpeed << " m/s" << std::endl;
    }
};

class Spaceship : public Serializable {
public:
    float thrust = 9000.1;
    Doggo passenger;

    Spaceship() {
        passenger.sound = "bark!";
    }

    void doSerialize(Writerator* it) {
        it->writeFloat(thrust);
        it->writeSerial(&passenger);
    }
    
    void doDeserialize(Readerator* it) {
        thrust = it->readFloat();
        std::cout << thrust << std::endl;
        it->readSerial(&passenger);
    }
};
