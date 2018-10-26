#include "Serializable.h"

// Writerator
Writerator::Writerator(char* data, const size_t size) : Iterator(data, size) {
}

// 8 bit
void Writerator::writeU8(uint8_t value) {
    range(size8);
    raw8(value);
}

void Writerator::write8(int8_t value) {
    writeU8((uint8_t) value);
}

void Writerator::writeChar(char value) {
    writeU8((uint8_t) value);
}

void Writerator::writeU8s(const uint8_t* array, size_t count) {
    range(size8 * count);
    for (size_t i = 0; i < count; i++) {
        raw8(array[i]);
    }
}

void Writerator::write8s(const int8_t* array, size_t count) {
    writeU8s(reinterpret_cast<const uint8_t*>(array), count);
}

void Writerator::writeChars(const char* array, size_t count) {
    writeU8s(reinterpret_cast<const uint8_t*>(array), count);
}

// 16 bit
void Writerator::writeU16(uint16_t value) {
    range(size16);
    raw16(value);
}

void Writerator::write16(int16_t value) {
    writeU16((uint16_t) value);
}

void Writerator::writeU16s(const uint16_t* array, size_t count) {
    range(size16 * count);
    for (size_t i = 0; i < count; i++) {
        raw16(array[i]);
    }
}

void Writerator::write16s(const int16_t* array, size_t count) {
    writeU16s(reinterpret_cast<const uint16_t*>(array), count);
}

// 32 bit
void Writerator::writeU32(uint32_t value) {
    range(size32);
    raw32(value);
}

void Writerator::write32(int32_t value) {
    writeU32((uint32_t) value);
}

void Writerator::writeFloat(float value) {
    writeU32(reinterpret_cast<uint32_t &>(value));
}

void Writerator::writeU32s(const uint32_t* array, size_t count) {
    range(size32 * count);
    for (size_t i = 0; i < count; i++) {
        raw32(array[i]);
    }
}

void Writerator::write32s(const int32_t* array, size_t count) {
    writeU32s(reinterpret_cast<const uint32_t*>(array), count);
}

void Writerator::writeFloats(const float* array, size_t count) {
    writeU32s(reinterpret_cast<const uint32_t*>(array), count);
}

// 64 bit
void Writerator::writeU64(uint64_t value) {
    range(size64);
    raw64(value);
}

void Writerator::write64(int64_t value) {
    writeU64((const uint64_t) value);
}

void Writerator::writeDouble(double value) {
    writeU64(reinterpret_cast<uint64_t &>(value));
}

void Writerator::writeU64s(const uint64_t* array, size_t count) {
    range(size64 * count);
    for (size_t i = 0; i < count; i++) {
        raw64(array[i]);
    }
}

void Writerator::write64s(const int64_t* array, size_t count) {
    writeU64s(reinterpret_cast<const uint64_t*>(array), count);
}

void Writerator::writeDoubles(const double* array, size_t count) {
    writeU64s(reinterpret_cast<const uint64_t*>(array), count);
}

// Serializable
void Writerator::writeSerial(Serializable* s) {
    s->doSerialize(this);
}

// raw
void Writerator::raw8(uint8_t value) {
    data[index] = value;
    index += size8;
}

void Writerator::raw16(uint16_t value) {
    uint16_t* temp = reinterpret_cast<uint16_t*>(&data[index]);
    *temp = htobe16(value);
    index += size16;
}

void Writerator::raw32(uint32_t value) {
    uint32_t* temp = reinterpret_cast<uint32_t*>(&data[index]);
    *temp = htobe32(value);
    index += size32;
}

void Writerator::raw64(uint64_t value) {
    uint64_t* temp = reinterpret_cast<uint64_t*>(&data[index]);
    *temp = htobe64(value);
    index += size64;
}

// Readerator
Readerator::Readerator(const char* data, const size_t size) : Iterator(const_cast<char*>(data), size) {
}

// 8 bit
uint8_t Readerator::readU8() {
    range(size8);
    return raw8();
}

int8_t Readerator::read8() {
    return (int8_t) readU8();
}

char Readerator::readChar() {
    return (char) readU8();
}

void Readerator::readU8s(uint8_t* array, size_t count) {
    range(size8 * count);
    for (size_t i = 0; i < count; i++) {
        array[i] = raw8();
    }
}

void Readerator::read8s(int8_t* array, size_t count) {
    readU8s(reinterpret_cast<uint8_t*>(array), count);
}

void Readerator::readChars(char* array, size_t count) {
    readU8s(reinterpret_cast<uint8_t*>(array), count);
}

// 16 bit
uint16_t Readerator::readU16() {
    range(size16);
    return raw16();
}

int16_t Readerator::read16() {
    return (int16_t) readU16();
}

void Readerator::readU16s(uint16_t* array, size_t count) {
    range(size16 * count);
    for (size_t i = 0; i < count; i++) {
        array[i] = raw16();
    }
}

void Readerator::read16s(int16_t* array, size_t count) {
    readU16s(reinterpret_cast<uint16_t*>(array), count);
}

// 32 bit
uint32_t Readerator::readU32() {
    range(size32);
    return raw32();
}

int32_t Readerator::read32() {
    return (int32_t) readU32();
}

float Readerator::readFloat() {
    uint32_t value = readU32();
    return reinterpret_cast<float &>(value);
}

void Readerator::readU32s(uint32_t* array, size_t count) {
    range(size32 * count);
    for (size_t i = 0; i < count; i++) {
        array[i] = raw32();
    }
}

void Readerator::read32s(int32_t* array, size_t count) {
    readU32s(reinterpret_cast<uint32_t*>(array), count);
}

void Readerator::readFloats(float* array, size_t count) {
    readU32s(reinterpret_cast<uint32_t*>(array), count);
}

// 64 bit
uint64_t Readerator::readU64() {
    range(size64);
    return raw64();
}

int64_t Readerator::read64() {
    return (int64_t) readU64();
}

double Readerator::readDouble() {
    uint64_t value = readU64();
    return reinterpret_cast<double &>(value);
}

void Readerator::readU64s(uint64_t* array, size_t count) {
    range(size64 * count);
    for (size_t i = 0; i < count; i++) {
        array[i] = raw64();
    }
}

void Readerator::read64s(int64_t* array, size_t count) {
    readU64s(reinterpret_cast<uint64_t*>(array), count);
}

void Readerator::readDoubles(double* array, size_t count) {
    readU64s(reinterpret_cast<uint64_t*>(array), count);
}

// Serializable
void Readerator::readSerial(Serializable* s) {
    s->doDeserialize(this);
}

// raw
uint8_t Readerator::raw8() {
    uint8_t value = (uint8_t) data[index];
    index += size8;
    return value;
}

uint16_t Readerator::raw16() {
    uint16_t value = reinterpret_cast<uint16_t &>(data[index]);
    index += size16;
    return be16toh(value);
}

uint32_t Readerator::raw32() {
    uint32_t value = reinterpret_cast<uint32_t &>(data[index]);
    index += size32;
    return be32toh(value);
}

uint64_t Readerator::raw64() {
    uint64_t value = reinterpret_cast<const uint64_t &>(data[index]);
    index += size64;
    return be64toh(value);
}
