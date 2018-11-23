#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <iostream>
#include <fstream>

using namespace std;
using namespace rapidjson;

class Config {
public:
    Config(string path) {
        ifstream in(path, ios::in|ios::binary|ios::ate);
        if (!in)
            return;
        streampos size = in.tellg();
        char* json = new char[size];
        in.seekg(0, ios::beg);
        in.read(json, size);
        in.close();

        doc.Parse(json);
    }

    void getBody(Body* body) {
        Value& docBody = doc["body"];
        Value& docLegs = docBody["legs"];

        body->legsCount = docLegs.Size();
        body->legs = new Leg[body->legsCount];

        for (SizeType i = 0; i < body->legsCount; i++) {
            getLeg(docLegs[i], body->legs[i]);
        }
    }

    void getLeg(Value& docLeg, Leg& leg) {
        Value& docOffset = docLeg["offset"];
        leg.offset.x = docOffset[0].GetDouble();
        leg.offset.y = docOffset[1].GetDouble();
        leg.offset.z = docOffset[2].GetDouble();

        getBone(docLeg["coxa"], leg.coxa);
        getBone(docLeg["femur"], leg.femur);
        getBone(docLeg["tibia"], leg.tibia);
    }

    void getBone(Value& docBone, Bone& bone) {
        bone.length = docBone["length"].GetDouble();
        bone.angle = docBone["angle"].GetDouble();
        bone.range = docBone["range"].GetDouble();
    }

    void getTrim(Value& docTrim, short trim[]) {
        for (SizeType i = 0; i < docTrim.Size(); i++) {
            trim[i] = (short) docTrim[i].GetInt();
        }
    }

    void getGait(Value& docGait, void* gait) {
        return;
    }

private:
    Document doc;
};

#endif /* CONFIG_H */
