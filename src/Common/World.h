#ifndef __COMMON_WORLD_H__
#define __COMMON_WORLD_H__

#include "Common/Shared.h"
#include "Math/Point3.h"
#include "Common/Chunk.h"

#include <vector>

namespace Common {
    class World {

        std::shared_ptr<Chunk> root;
        uint64_t seed;

        void expand(std::shared_ptr<Chunk> chunk, const Dot3 &centre);
    public:
        World(uint64_t seed);

        std::shared_ptr<Chunk> chunk(int32_t x, int32_t y, int32_t z);
        std::shared_ptr<Chunk> chunk(const Dot3 &index);
        std::shared_ptr<Chunk> chunk(const Vec3F &world_pos);

        const std::shared_ptr<Chunk> chunk(int32_t x, int32_t y, int32_t z) const;
        const std::shared_ptr<Chunk> chunk(const Dot3 &index) const;
        const std::shared_ptr<Chunk> chunk(const Vec3F &world_pos) const;

        Dot3 calculateChunkIndex(const Vec3F &world_point) const;

        uint64_t Seed() const {
            return seed;
        }

        ~World();
    };
}; // namespace Common

#endif //__COMMON_WORLD_H__
