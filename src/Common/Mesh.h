#ifndef __COMMON_MESH_H__
#define __COMMON_MESH_H__

#include <memory>
#include <vector>
#include <bitset>

#include "Common/Chunk.h"
#include "Common/Occluder.h"
#include "Renderer/Voxel.h"

namespace Common {
    class Mesh {
        std::array<std::vector<uint32_t>, Renderer::VOXEL_COUNT> facesList;

        std::array<Occluder, 4*4*4> coarseOcclusion;
        std::array<Occluder, 8*8*8> fineOcclusion;
    public:
        Mesh(const Chunk *chunk);

        const std::vector<uint32_t> &faces(int face_index) const {
            return facesList[face_index];
        }

        const Occluder fine(int x, int y, int z) const {
            return fineOcclusion[y*4*4 + z*4 + x];
        }

        const Occluder coarse(int x, int y, int z) const {
            return fineOcclusion[y*8*8 + z*8 + x];
        }

        ~Mesh();
    };
}; // namespace Common

#endif //__COMMON_MESH_H__
