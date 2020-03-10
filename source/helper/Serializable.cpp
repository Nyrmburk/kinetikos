#include "Serializable.h"

// Write

// 8 bit
void DataView::writeU8(uint8_t value) {
    range(size8);
    writeRaw8(value);
}

void DataView::write8(int8_t value) {
    writeU8((uint8_t) value);
}

void DataView::writeChar(char value) {
    writeU8((uint8_t) value);
}

void DataView::writeU8s(const uint8_t* array, size_t count) {
    range(size8 * count);
    for (size_t i = 0; i < count; i++) {
        writeRaw8(array[i]);
    }
}

void DataView::write8s(const int8_t* array, size_t count) {
    writeU8s(reinterpret_cast<const uint8_t*>(array), count);
}

void DataView::writeChars(const char* array, size_t count) {
    writeU8s(reinterpret_cast<const uint8_t*>(array), count);
}

// 16 bit
void DataView::writeU16(uint16_t value) {
    range(size16);
    writeRaw16(value);
}

void DataView::write16(int16_t value) {
    writeU16((uint16_t) value);
}

void DataView::writeU16s(const uint16_t* array, size_t count) {
    range(size16 * count);
    for (size_t i = 0; i < count; i++) {
        writeRaw16(array[i]);
    }
}

void DataView::write16s(const int16_t* array, size_t count) {
    writeU16s(reinterpret_cast<const uint16_t*>(array), count);
}

// 32 bit
void DataView::writeU32(uint32_t value) {
    range(size32);
    writeRaw32(value);
}

void DataView::write32(int32_t value) {
    writeU32((uint32_t) value);
}

void DataView::writeFloat(float value) {
    writeU32(reinterpret_cast<uint32_t &>(value));
}

void DataView::writeU32s(const uint32_t* array, size_t count) {
    range(size32 * count);
    for (size_t i = 0; i < count; i++) {
        writeRaw32(array[i]);
    }
}

void DataView::write32s(const int32_t* array, size_t count) {
    writeU32s(reinterpret_cast<const uint32_t*>(array), count);
}

void DataView::writeFloats(const float* array, size_t count) {
    writeU32s(reinterpret_cast<const uint32_t*>(array), count);
}

// 64 bit
void DataView::writeU64(uint64_t value) {
    range(size64);
    writeRaw64(value);
}

void DataView::write64(int64_t value) {
    writeU64((const uint64_t) value);
}

void DataView::writeDouble(double value) {
    writeU64(reinterpret_cast<uint64_t &>(value));
}

void DataView::writeU64s(const uint64_t* array, size_t count) {
    range(size64 * count);
    for (size_t i = 0; i < count; i++) {
        writeRaw64(array[i]);
    }
}

void DataView::write64s(const int64_t* array, size_t count) {
    writeU64s(reinterpret_cast<const uint64_t*>(array), count);
}

void DataView::writeDoubles(const double* array, size_t count) {
    writeU64s(reinterpret_cast<const uint64_t*>(array), count);
}

// Serializable
void DataView::writeSerial(Serializable* s) {
    s->serialize(this);
}

// raw
void DataView::writeRaw8(uint8_t value) {
    data[index] = value;
    index += size8;
}

void DataView::writeRaw16(uint16_t value) {
    uint16_t* temp = reinterpret_cast<uint16_t*>(&data[index]);
    *temp = htobe16(value);
    index += size16;
}

void DataView::writeRaw32(uint32_t value) {
    uint32_t* temp = reinterpret_cast<uint32_t*>(&data[index]);
    *temp = htobe32(value);
    index += size32;
}

void DataView::writeRaw64(uint64_t value) {
    uint64_t* temp = reinterpret_cast<uint64_t*>(&data[index]);
    *temp = htobe64(value);
    index += size64;
}

// Read

// 8 bit
uint8_t DataView::readU8() {
    range(size8);
    return readRaw8();
}

int8_t DataView::read8() {
    return (int8_t) readU8();
}

char DataView::readChar() {
    return (char) readU8();
}

void DataView::readU8s(uint8_t* array, size_t count) {
    range(size8 * count);
    for (size_t i = 0; i < count; i++) {
        array[i] = readRaw8();
    }
}

void DataView::read8s(int8_t* array, size_t count) {
    readU8s(reinterpret_cast<uint8_t*>(array), count);
}

void DataView::readChars(char* array, size_t count) {
    readU8s(reinterpret_cast<uint8_t*>(array), count);
}

// 16 bit
uint16_t DataView::readU16() {
    range(size16);
    return readRaw16();
}

int16_t DataView::read16() {
    return (int16_t) readU16();
}

void DataView::readU16s(uint16_t* array, size_t count) {
    range(size16 * count);
    for (size_t i = 0; i < count; i++) {
        array[i] = readRaw16();
    }
}

void DataView::read16s(int16_t* array, size_t count) {
    readU16s(reinterpret_cast<uint16_t*>(array), count);
}

// 32 bit
uint32_t DataView::readU32() {
    range(size32);
    return readRaw32();
}

int32_t DataView::read32() {
    return (int32_t) readU32();
}

float DataView::readFloat() {
    uint32_t value = readU32();
    return reinterpret_cast<float &>(value);
}

void DataView::readU32s(uint32_t* array, size_t count) {
    range(size32 * count);
    for (size_t i = 0; i < count; i++) {
        array[i] = readRaw32();
    }
}

void DataView::read32s(int32_t* array, size_t count) {
    readU32s(reinterpret_cast<uint32_t*>(array), count);
}

void DataView::readFloats(float* array, size_t count) {
    readU32s(reinterpret_cast<uint32_t*>(array), count);
}

// 64 bit
uint64_t DataView::readU64() {
    range(size64);
    return readRaw64();
}

int64_t DataView::read64() {
    return (int64_t) readU64();
}

double DataView::readDouble() {
    uint64_t value = readU64();
    return reinterpret_cast<double &>(value);
}

void DataView::readU64s(uint64_t* array, size_t count) {
    range(size64 * count);
    for (size_t i = 0; i < count; i++) {
        array[i] = readRaw64();
    }
}

void DataView::read64s(int64_t* array, size_t count) {
    readU64s(reinterpret_cast<uint64_t*>(array), count);
}

void DataView::readDoubles(double* array, size_t count) {
    readU64s(reinterpret_cast<uint64_t*>(array), count);
}

// Serializable
void DataView::readSerial(Serializable* s) {
    s->deserialize(this);
}

// raw
uint8_t DataView::readRaw8() {
    uint8_t value = (uint8_t) data[index];
    index += size8;
    return value;
}

uint16_t DataView::readRaw16() {
    uint16_t value = reinterpret_cast<uint16_t &>(data[index]);
    index += size16;
    return be16toh(value);
}

uint32_t DataView::readRaw32() {
    uint32_t value = reinterpret_cast<uint32_t &>(data[index]);
    index += size32;
    return be32toh(value);
}

uint64_t DataView::readRaw64() {
    uint64_t value = reinterpret_cast<const uint64_t &>(data[index]);
    index += size64;
    return be64toh(value);
}
