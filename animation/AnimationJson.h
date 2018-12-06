#ifndef ANIMATION_JSON_H
#define ANIMATION_JSON_H

#include <string>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <iostream>
#include <fstream>

#include "../curve/Bezier3.h"
#include "../matrix/vec3.h"
#include "../matrix/mat4.h"
#include "RobotClip.h"

using namespace std;
using namespace rapidjson;

class AnimationJson {
public:
    AnimationJson(string path) {
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

    RobotClip& getAnimation(const char* name) {
        Value& docClip = doc[name];

        // clip length
        float length = docClip["length"].GetFloat();

        // clip loop style / iterations
        Value& docLoop = docClip["loop"];
        if (docLoop.IsString()) {
            string loop = docLoop.GetString();
        } else if (docLoop.IsInt()) {
            int loop = docLoop.GetInt();
        }

        // body channel
        Mat4Channel& bodyChannel = getBodyChannel(docClip["body"]);

        // feet channels
        Value& docFeet = docClip["feet"];
        Bezier3Channel* footChannels = new Bezier3Channel[docFeet.Size()];
        for (SizeType i = 0; i < docFeet.Size(); i++) {
            footChannels[i] = getFootChannel(docFeet[i]);
        }

        return *new RobotClip(length, bodyChannel, docFeet.Size(), footChannels);
    }

    Mat4Channel& getBodyChannel(Value& docBody) {
        Mat4Channel& channel = *new Mat4Channel;
        for (SizeType i = 0; i < docBody.Size(); i++) {
            Tween<Mat4> tween = getMat4Tween(docBody[i]);
            channel.insertTween(tween);
        }
        return channel;
    }

    Bezier3Channel& getFootChannel(Value& docFoot) {
        Bezier3Channel& channel = *new Bezier3Channel();
        for (SizeType i = 0; i < docFoot.Size(); i++) {
            Tween<Bezier3::Node>& tween = getBezier3Tween(docFoot[i]);
            channel.insertTween(tween);
        }
        return channel;
    }

    Tween<void*>& getBaseTween(Value& docTween) {
        float time = docTween["time"].GetFloat();
        string ease = docTween["ease"].GetString();
        easeFunc func = easeNone;

        if (ease == "none") {
            func = easeNone;
        } else if (ease == "linear") {
            func = easeLinear;
        } else if (ease == "in") {
            func = easeQuadraticIn;
        } else if (ease == "out") {
            func = easeQuadraticOut;
        } else if (ease == "inout") {
            func = easeQuadraticInOut;
        }
        
        return *new Tween<void*>(nullptr, time, func);
    }

    Tween<Bezier3::Node>& getBezier3Tween(Value& docTween) {
        Tween<void*>& base = getBaseTween(docTween);

        Bezier3::Node& node = *new Bezier3::Node();
        node.point = new Vec3;
        node.handle = new Vec3;
        getVec3(docTween["point"], *node.point);
        getVec3(docTween["handle"], *node.handle);
        
        return *new Tween<Bezier3::Node>(node, base.getTime(), base.getFunction());
    }

    Tween<Mat4>& getMat4Tween(Value& docTween) {
        Tween<void*>& base = getBaseTween(docTween);
        
        Mat4* orientation = new Mat4;
        getMat4(docTween["orientation"], *orientation);
        return *new Tween<Mat4>(*orientation, base.getTime(), base.getFunction());
    }

    void getVec3(Value& docVec, Vec3& vec) {
        vec.x = docVec[0].GetFloat();
        vec.y = docVec[1].GetFloat();
        vec.z = docVec[2].GetFloat();
    }

    void getMat4(Value& docMat, Mat4& mat) {
        for (int i = 0; i < TOTAL_SIZE_M4; i++) {
            mat.m[i] = docMat[i].GetFloat();
        }
        transposem4(&mat, &mat);
    }

private:
    Document doc;
};

#endif /* ANIMATION_JSON_H */
