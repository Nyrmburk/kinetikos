#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <cstdint>
#include <map>
#include <functional>
#include "../helper/Serializable.h"

class Protocol;
typedef Serializable* (Procedure)(DataView* it);

class Protocol {
public:
    int handleMessage(
            const char* dataIn, const size_t sizeIn,
            char* dataOut, const size_t sizeOut) {

        DataView in(const_cast<char*>(dataIn), sizeIn);
        DataView out(dataOut, sizeOut);

        // get control code
        uint16_t controlCode = in.readU16();
        out.writeU16(controlCode);

        // handle message in implementation
        handle(controlCode, in, out);

        // check if data has been written
        if (out.getIndex() == size16) {
            return 0; // no data written so returning early
        }

        return out.getIndex();
    }

protected:
    // easy handler for implementation
    virtual void handle(uint16_t controlCode, DataView& in, DataView& out) = 0;
};

#endif /* PROTOCOL_H */
