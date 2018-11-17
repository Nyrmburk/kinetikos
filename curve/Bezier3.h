#ifndef BEZIER3_H
#define BEZIER3_H

#include "../matrix/vec3.h"
#include "../matrix/MatSerialize.h"
#include "ApproximateCurve.h"
#include "../helper/Serializable.h"

class Bezier3 : public ApproximateCurve<Vec3> {
public:
    struct Bezier3Node : Serializable {
        Vec3 point;
        Vec3 handle;

        void serialize(DataView* writer) {
            serializev3(writer, &point);
            serializev3(writer, &handle);
        }

        void deserialize(DataView* reader) {
            deserializev3(reader, &point);
            deserializev3(reader, &handle);
        }
    };

    Bezier3Node* start;
    Bezier3Node* end;

    void valueAt(float t, Vec3* result) {
        float one_minus_t = (1 - t);
        float one_minus_t_2 = one_minus_t * one_minus_t;

        float t_2 = t * t;

        float a = one_minus_t_2 * one_minus_t;
        float b = 3 * one_minus_t_2 * t;
        float c = 3 * one_minus_t * t_2;
        float d = t_2 * t;

        Vec3 temp = {0, 0, 0};
        setv3(result, &temp); // zero out result

        // multiply start * a
        multiplyv3s(&start->point, a, &temp);
        addv3(&temp, result, result);

        // multiply start handle * b
        // handle is relative to start point
        addv3(&start->point, &start->handle, &temp);
        multiplyv3s(&temp, b, &temp);
        addv3(&temp, result, result);

        // multply end handle * c
        // handle is relative to end pont
        addv3(&end->point, &end->handle, &temp);
        multiplyv3s(&temp, c, &temp);
        addv3(&temp, result, result);

        multiplyv3s(&end->point, d, &temp);
        addv3(&temp, result, result);
    }

    float distance(const Vec3* a, const Vec3* b) {
        distancev3(a, b);
    }

    void serialize(DataView* writer) {
        writer->writeSerial(start);
        writer->writeSerial(end);
    }

    void deserialize(DataView* reader) {
        reader->readSerial(start);
        reader->readSerial(end);
    }
};

#endif /* BEZIER3_H */
