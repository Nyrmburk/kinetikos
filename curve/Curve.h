#ifndef CURVE_H
#define CURVE_H

template<typename T>
class Curve : public Serializable {
public:
    virtual void valueAt(float t, T* value) = 0;
    virtual void linearValueAt(float t, T* value) = 0;
    virtual float length() = 0;
};

#endif /* CURVE_H */
