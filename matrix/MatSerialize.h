#ifndef MAT_SERIALIZE_H
#define MAT_SERIALIZE_H

#include "../helper/Serializable.h"
#include "vec2.h"
#include "vec3.h"
#include "mat4.h"

void serializev2(DataView* writer, const Vec2* vec);
void deserializev2(DataView* reader, Vec2* vec);

void serializev3(DataView* writer, const Vec3* vec);
void deserializev3(DataView* reader, Vec3* vec);

void serializem4(DataView* writer, const Mat4* mat);
void serializem4(DataView* reader, Mat4* mat);

#endif /* MAT_SERIALIZE_H */
