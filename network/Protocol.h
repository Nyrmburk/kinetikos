#include <cstdint>
#include <map>
#include "../robot/Body.h"
//#include "../robot/RobotState.h"
#include "../helper/Serializable.h"

typedef Serializable* (*Procedure)(Readerator* it);
typedef std::map<uint16_t, Procedure> ProcedureMap;

class Packet : public Serializable {
public:
    uint16_t controlCode = 0;

    void doSerialize(Writerator* writer) {
        writer->writeU16(controlCode);
    }

    void doDeserialize(Readerator* reader) {
        controlCode = reader->readU16();
    }
};

class Protocol {
public:
    Protocol();
    ProcedureMap procedureMap;

    void insert(uint16_t controlCode, Procedure procedure) {
        procedureMap.insert(std::make_pair(controlCode, procedure));
    }

    void insert(uint16_t controlCodeBase, Procedure* procedures) {
        uint16_t controlCode = controlCodeBase;
        for(size_t i = 0; procedures[i] != nullptr; i++) {
            insert(controlCode++, procedures[i]);
        }
    }

    int handleMessage(
            const char* dataIn, const size_t sizeIn,
            char* dataOut, const size_t sizeDataOut) const {

        // get control code
        Packet packet;
        Readerator reader(dataIn, sizeIn);
        packet.doDeserialize(&reader);

        // get which procedure to run
        Procedure p = procedureMap.at(packet.controlCode);

        // execute the procedure and get its response
        Serializable* s = p(&reader);

        if (!s) {
            return 0;
        }

        // write the response out and return size of bytes written
        return s->serialize(dataOut, sizeDataOut);
    }
};

