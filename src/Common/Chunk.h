#ifndef __COMMON_CHUNK_H__
#define __COMMON_CHUNK_H__

#include <memory>
#include <array>

#include "Common/Shared.h"

#include "external/PerlinNoise.hpp"

#define BLOCKS_LEN 32

namespace Common {
    enum class Block {
        Empty = 0,
        Rock,
        Sand,
        Dirt,
        Grass,
        Water,
        Ice,
    };

    enum class Neighbour {
        Bottom = 0,
        Top,
        Left,
        Right,
        Front,
        Back
    };

    inline Neighbour operator!(Neighbour n) {
        switch (n) {
            case Neighbour::Bottom:
                return Neighbour::Top;
            case Neighbour::Top:
                return Neighbour::Bottom;
            case Neighbour::Left:
                return Neighbour::Right;
            case Neighbour::Right:
                return Neighbour::Left;
            case Neighbour::Front:
                return Neighbour::Back;
            case Neighbour::Back:
                return Neighbour::Front;
            default:
                return n;
        }
    }

    class Mesh;
    class Chunk {
        std::array<Block, BLOCKS_LEN * BLOCKS_LEN * BLOCKS_LEN> blocks;
        std::array<std::shared_ptr<Chunk>, 6> neighbours;

        std::unique_ptr<Mesh> mesh;

        uint32_t calculateHeight(const siv::PerlinNoise &perlin, int32_t x, int32_t z);
    public:
        Chunk();

        Block block(const uint32_t x, const uint32_t y, const uint32_t z) const;

        void set(const uint32_t x, const uint32_t y, const uint32_t z, Block block);

        void fill(Block block);
        void fillLayer(const uint32_t y, Block block);

        bool empty() const;
        bool full() const;

        std::shared_ptr<Chunk> neighbour(Neighbour index) const {
            return neighbours[(int)index];
        }

        void fill(uint64_t seed, int32_t x_offset, int32_t y_offset, int32_t z_offset);

        static void Join(std::shared_ptr<Chunk> a, Neighbour relationship, std::shared_ptr<Chunk> b);

        const std::vector<uint32_t> &visible_faces(int face_index) const;
        void buildMesh();

        ~Chunk();
    };
}; // namespace Common

#endif //__COMMON_CHUNK_H__
