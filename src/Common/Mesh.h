#ifndef __COMMON_MESH_H__
#define __COMMON_MESH_H__

#include <memory>
#include <vector>
#include <optional>

#include "Common/Chunk.h"
#include "Renderer/Voxel.h"

namespace Common {
    class Mesh {
        std::array<std::vector<uint32_t>, Renderer::VOXEL_COUNT> facesList;
        std::optional<std::pair<Dot3, Dot3>> occluder;
        std::optional<std::pair<Dot3, Dot3>> visible;
    public:
        Mesh(const Chunk *chunk);

        const std::vector<uint32_t> &faces(int face_index) const {
            return facesList[face_index];
        }

        std::optional<std::pair<Dot3, Dot3>> Occluder() const {
            return occluder;
        }

        std::optional<std::pair<Dot3, Dot3>> Visible() const {
            return visible;
        }

        ~Mesh();
    };
}; // namespace Common

#endif //__COMMON_MESH_H__
