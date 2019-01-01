#ifndef WORKSPACE_H
#define WORKSPACE_H

#include "Leg.h"
#include "../helper/Serializable.h"
#include "../helper/Math.h"
#include "../matrix/vec3.h"

class Workspace : public Serializable {
public:
    Workspace(int precision = 10) {
        this->precision = precision;
        this->points = new Vec3[totalElements()];
    }

    size_t elementsPerSlice() {
        // 4 sides of workspace slice
        return precision * 4;
    }

    size_t slices() {
         // n+1 vertical slices
        return precision + 1;
    }

    size_t totalElements() {
         return elementsPerSlice() * slices();
    }

    int getPrecision() {
        return precision;
    }

    Vec3* getPoints() {
        return points;
    }

    void generate(Leg& leg) {
        Mat4 orientation;
        identitym4(&orientation);
        Joints joints;
        int index = 0;

        for (int slice = 0; slice <= precision; slice++) {
            joints.femur = mapRange(slice, 0, precision, 0, 1);
            // left up ^
            for (int i = 0; i < precision; i++) {
                joints.tibia = mapRange(i, 0, precision, 0, 1);
                leg.solveForward(&orientation, &joints, &points[index++]);
            }
            joints.tibia = 1;

            // top right >
            for (int i = 0; i < precision; i++) {
                joints.coxa = mapRange(i, 0, precision, 0, 1);
                leg.solveForward(&orientation, &joints, &points[index++]);
            }
            joints.coxa = 1;

            // right down v
            for (int i = 0; i < precision; i++) {
                joints.tibia = mapRange(i, 0, precision, 1, 0);
                leg.solveForward(&orientation, &joints, &points[index++]);
            }
            joints.tibia = 0;

            // bottom left <
            for (int i = 0; i < precision; i++) {
                joints.coxa = mapRange(i, 0, precision, 1, 0);
                leg.solveForward(&orientation, &joints, &points[index++]);
            }
            joints.coxa = 0;
        }
    }

    void serialize(DataView* data) {
        data->write32(precision);
        size_t elements = totalElements();
        for (int i = 0; i < elements; i++) {
            serializev3(data, &points[i]);
        }
    }

    void deserialize(DataView* data) {
        precision = data->read32();
        size_t elements = totalElements();
        for (int i = 0; i < elements; i++) {
            deserializev3(data, &points[i]);
        }
    }
private:
    int precision;
    Vec3* points;
};

#endif /* WORKSPACE_H */

