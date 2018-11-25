#include "RobotProtocol.h"
#include <iostream>

void RobotProtocol::echo(DataView& in, DataView& out) {
    std::cout << "echo: ";
    while (in.getIndex() < in.getSize()) {
        char character = in.readChar();
        std::cout << character;
        out.writeChar(character);
    }
    std::cout << endl;
}

