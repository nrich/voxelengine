#ifndef __COMMON_VERTEX_H__
#define __COMMON_VERTEX_H__

#include <cstdint>

#include "Math/Point2.h"
#include "Math/Point3.h"

namespace Common {
    struct Vertex {
        Vec3F vert;
        Vec3F normal;
        Vec2F tex;
        int32_t index;

        Vertex(const Vec3F &vert, const Vec2F &tex, const uint32_t index=0) : vert(vert), normal(Vec3F(0, 0, 0)), tex(tex), index(index) {
        }

        Vertex(const Vec3F &vert, const Vec3F &normal, const Vec2F &tex, const int32_t index=0) : vert(vert), normal(normal), tex(tex), index(index) {
        }
    };
}; // namespace Common

typedef Common::Vertex Vertex;

#endif //__COMMON_VERTEX_H__

