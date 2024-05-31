#ifndef __COMMON_MESH_H__
#define __COMMON_MESH_H__

#include <memory>
#include <vector>

#include "Common/Chunk.h"
#include "Renderer/Voxel.h"

namespace Common {
    class Mesh {
        std::array<std::vector<uint32_t>, Renderer::VOXEL_COUNT> faces_list;
    public:
        Mesh(const Chunk *chunk);

        const std::vector<uint32_t> &faces(int face_index) const {
            return faces_list[face_index];
        }

        ~Mesh();
    };
}; // namespace Common

#endif //__COMMON_MESH_H__
