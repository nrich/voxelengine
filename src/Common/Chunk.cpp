#include "Common/Chunk.h"
#include "Common/Mesh.h"

#include <algorithm>
#include <iostream>

using namespace Common;

Chunk::Chunk() {
    std::fill(blocks.begin(), blocks.end(), Block::Empty);
}

Block Chunk::block(const uint32_t x, const uint32_t y, const uint32_t z) const {
    if (x < 0 || y < 0 || z < 0)
        return Block::Empty;

    if (x >= BLOCKS_LEN || y >= BLOCKS_LEN || z >= BLOCKS_LEN)
        return Block::Empty;

    return blocks[(y*BLOCKS_LEN*BLOCKS_LEN) + (z*BLOCKS_LEN) + x];
}

void Chunk::set(const uint32_t x, const uint32_t y, const uint32_t z, Block block) {
    blocks[(y*BLOCKS_LEN*BLOCKS_LEN) + (z*BLOCKS_LEN) + x] = block;
}

void Chunk::fill(Block block) {
    std::fill(blocks.begin(), blocks.end(), block);
}

void Chunk::fillLayer(const uint32_t y, Block block) {
    std::fill(blocks.begin() + (y*BLOCKS_LEN*BLOCKS_LEN), blocks.begin()+((y+1)*BLOCKS_LEN*BLOCKS_LEN), block);
}

void Chunk::Join(std::shared_ptr<Chunk> a, Neighbour relationship, std::shared_ptr<Chunk> b) {
    a->neighbours[(int)relationship] = b;
    b->neighbours[(int)!relationship] = a;
}

bool Chunk::empty() const {
    int sum = 0;

     for (auto block : blocks)
        sum += (int)block;

    return sum == 0;
}

const std::vector<uint32_t> &Chunk::visible_faces(int face_index) const {
    if (!mesh) {
        static std::vector<uint32_t> empty_faces = {};
        return empty_faces;
    }
    return mesh->faces(face_index);
}

void Chunk::buildMesh() {
    if (mesh)
        mesh.release();

    mesh = std::make_unique<Mesh>(this);
}

uint32_t Chunk::calculateHeight(const siv::PerlinNoise &perlin, int32_t x, int32_t z) {
    const float noise = perlin.octave2D_01(x * 0.025, z * 0.025, 4);

    return (uint32_t)(noise * ((BLOCKS_LEN>>1) + (BLOCKS_LEN>>2)));
}

void Chunk::fill(uint64_t seed, int32_t x_offset, int32_t y_offset, int32_t z_offset) {
    const siv::PerlinNoise perlin{ seed };

    for (int z = 0; z < BLOCKS_LEN; z++) {
        for (int x = 0; x < BLOCKS_LEN; x++) {
            uint32_t y = calculateHeight(perlin, x + x_offset, z + z_offset);

            for (size_t iy = 0; iy <= y; iy++) {
                if (iy > 16) {
                    set(x, iy, z, Block::Ice);
                } else {
                    set(x, iy, z, Block::Rock);
                }
            }

            if (y == 7) {
                set(x, y, z, Block::Sand);
            }

            if (y == 8) {
                set(x, y, z, Block::Grass);
            }

            for (size_t iy = y; iy <= 6; iy++) {
                set(x, iy, z, Block::Water);
            }
        }
    }
}

Chunk::~Chunk() {

}
