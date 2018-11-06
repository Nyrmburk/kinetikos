#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include <exception>
#include <stdexcept>
#include <cstdlib>
#include <endian.h>

#define size8 1
#define size16 2
#define size32 4
#define size64 8

class Serializable;

class DataView {
public:
    DataView(char* data, const size_t size) :
            size{size},
            data{data} {}

    size_t getIndex() {
        return index;
    }

    void setIndex(size_t index) {
        this->index = index;
    }

    void rewind() {
        this->index = 0;
    }

    size_t getSize() {
        return size;
    }

    // 8 bit
    void writeU8(uint8_t value);
    void write8(int8_t value);
    void writeChar(char value);
    void writeU8s(const uint8_t* array, size_t count);
    void write8s(const int8_t* array, size_t count);
    void writeChars(const char* array, size_t count);

    // 16 bit
    void writeU16(uint16_t value);
    void write16(int16_t value);
    void writeU16s(const uint16_t* array, size_t count);
    void write16s(const int16_t* array, size_t count);

    // 32 bit
    void writeU32(uint32_t value);
    void write32(int32_t value);
    void writeFloat(float value);
    void writeU32s(const uint32_t* array, size_t count);
    void write32s(const int32_t* array, size_t count);
    void writeFloats(const float* array, size_t count);

    // 64 bit
    void writeU64(uint64_t value);
    void write64(int64_t value);
    void writeDouble(double value);
    void writeU64s(const uint64_t* array, size_t count);
    void write64s(const int64_t* array, size_t count);
    void writeDoubles(const double* array, size_t count);

    // Serializable
    void writeSerial(Serializable* s);

    // 8 bit
    uint8_t readU8();
    int8_t read8();
    char readChar();
    void readU8s(uint8_t* array, size_t count);
    void read8s(int8_t* array, size_t count);
    void readChars(char* array, size_t count);

    // 16 bit
    uint16_t readU16();
    int16_t read16();
    void readU16s(uint16_t* array, size_t count);
    void read16s(int16_t* array, size_t count);

    // 32 bit
    uint32_t readU32();
    int32_t read32();
    float readFloat();
    void readU32s(uint32_t* array, size_t count);
    void read32s(int32_t* array, size_t count);
    void readFloats(float* array, size_t count);

    // 64 bit
    uint64_t readU64();
    int64_t read64();
    double readDouble();
    void readU64s(uint64_t* array, size_t count);
    void read64s(int64_t* array, size_t count);
    void readDoubles(double* array, size_t count);

    // Serializable
    void readSerial(Serializable* s);

private:
    char* data;
    const size_t size;
    size_t index = 0;

    void range(size_t count) {
        if (index + count > size) {
            throw std::out_of_range(
                    "Error accessing " + std::to_string(count) +
                    " at " + std::to_string(index) +
                    " out of " + std::to_string(size) +
                    " bytes.");
        }
    }

    // no bounds checked writes
    void writeRaw8(uint8_t value);
    void writeRaw16(uint16_t value);
    void writeRaw32(uint32_t value);
    void writeRaw64(uint64_t value);

    // no bounds checked reads
    uint8_t readRaw8();
    uint16_t readRaw16();
    uint32_t readRaw32();
    uint64_t readRaw64();
};

class Serializable {
public:
    virtual void serialize(DataView* writer) = 0;
    virtual void deserialize(DataView* reader) = 0;
};

#endif /* SERIALIZABLE_H */
