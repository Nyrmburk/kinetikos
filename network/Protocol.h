#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <cstdint>
#include <map>
#include "../helper/Serializable.h"

typedef Serializable* (*Procedure)(DataView* it);
typedef std::map<uint16_t, Procedure> ProcedureMap;

class Packet : public Serializable {
public:
    uint16_t controlCode = 0;

    void serialize(DataView* writer) {
        writer->writeU16(controlCode);
    }

    void deserialize(DataView* reader) {
        controlCode = reader->readU16();
    }
};

class Protocol {
public:
    Protocol();
    ProcedureMap procedureMap;

    void addProcedure(uint16_t controlCode, Procedure procedure) {
        procedureMap.insert(std::make_pair(controlCode, procedure));
    }

    int handleMessage(
            const char* dataIn, const size_t sizeIn,
            char* dataOut, const size_t sizeOut) const {

        // get control code
        Packet packet;
        DataView reader(const_cast<char*>(dataIn), sizeIn);
        packet.deserialize(&reader);

        // get which procedure to run
        Procedure p = procedureMap.at(packet.controlCode);

        // execute the procedure and get its response
        Serializable* response = p(&reader);

        if (!response) {
            return 0;
        }

        // write the response out and return size of bytes written
        DataView writer(dataOut, sizeOut);
        packet.serialize(&writer);
        response->serialize(&writer);
        return writer.getSize();
    }
};

#endif /* PROTOCOL_H */
