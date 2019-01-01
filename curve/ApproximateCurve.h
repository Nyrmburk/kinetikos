#ifndef APPROXIMATE_CURVE_H
#define APPROXIMATE_CURVE_H

#include "Curve.h"

template<typename T>
class ApproximateCurve : public Curve<T> {
public:
    ApproximateCurve(size_t segments = 10) {
        this->segmentCount = segments;
        lengthLookup = new float[segments];
    }

    ~ApproximateCurve() {
        delete[] lengthLookup;
    }

    void approximate() {
        float step = 1.0f / segmentCount;
        float t = 0;
        T temp1;
        T* previous = &temp1;
        T temp2;
        T* next = &temp2;
        this->valueAt(t, previous);
        float totalLength = 0;

        // populate length lookup table
        for (size_t i = 0; i < segmentCount; i++) {
            t += step;
            this->valueAt(t, next);
            totalLength += distance(previous, next);
            lengthLookup[i] = totalLength;

            // swap the pointers
            T* temp = previous;
            previous = next;
            next = temp;
        }

        // normalize the length from [0, length] to [0, 1]
        totalLength = 1.0f / totalLength; // pre-divide
        for (size_t i = 0; i < segmentCount; i++) {
            lengthLookup[i] *= totalLength;
        }
    }

    void linearValueAt(float t, T* value) {
        float previous = 0;
        float next = 0;
        float step = 1.0f / segmentCount;
        for (size_t i = 0; i < segmentCount; i++) {
            previous = next;
            next = lengthLookup[i];
            if (next > t) {
                // map t between [0, 1](linear) to [previous, next]
                t = (t - previous) / (next - previous);

                // map t from [previous, next] to [0, 1](nonlinear)
                t = (i + t) * step;
                break;
            }
        }
        this->valueAt(t, value);
    }

    float length() {
        return lengthLookup[segmentCount - 1]; // end value
    }

    virtual float distance(const T* a, const T* b) = 0;

    size_t segmentCount = 10;
    float* lengthLookup;
};

#endif /* APPROXIMATE_CURVE_H */
