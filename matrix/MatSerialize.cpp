#include "MatSerialize.h"

void serializev2(DataView* writer, Vec2* vec) {
    writer->writeFloat(vec->x);
    writer->writeFloat(vec->y);
}

void deserializev2(DataView* reader, Vec2* vec) {
    vec->x = reader->readFloat();
    vec->y = reader->readFloat();
}

void serializev3(DataView* writer, Vec3* vec) {
    writer->writeFloat(vec->x);
    writer->writeFloat(vec->y);
    writer->writeFloat(vec->z);
}

void deserializev3(DataView* reader, Vec3* vec) {
    vec->x = reader->readFloat();
    vec->y = reader->readFloat();
    vec->z = reader->readFloat();
}

void serializem4(DataView* writer, Mat4* mat) {
    writer->writeFloats(mat->m, TOTAL_SIZE_M4);
}

void deserializem4(DataView* reader, Mat4* mat) {
    reader->readFloats(mat->m, TOTAL_SIZE_M4);
}

