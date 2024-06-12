#ifndef __CLIENT_OCCLUSIONTREE_H__
#define __CLIENT_OCCLUSIONTREE_H__

#include <map>
#include <vector>

#include "Common/Shared.h"
#include "Math/Point2.h"
#include "Math/Point3.h"
#include "Renderer/Voxel.h"

namespace Client {
    class OcclusionTree {
        struct Node {
            Dot min;
            Dot max;
            int z;
            uint32_t packedVoxel;
            int faceIndex;
            int chunkIndex;

            Node(const Dot &min, const Dot &max, int z, uint32_t packed_voxel, int face_index, int chunk_index) : min(min), max(max), z(z), packedVoxel(packed_voxel), faceIndex(face_index), chunkIndex(chunk_index) {
            }

            Node(const Node &other) : min(other.min), max(other.max), z(other.z), packedVoxel(other.packedVoxel), faceIndex(other.faceIndex), chunkIndex(other.chunkIndex) {
            }
        };

        std::map<int, std::vector<Node>> nodes;
    public:
        OcclusionTree();
        bool addQuad(const Dot &p0, const float z0, const Dot &p1, const float z1, const Dot &p2, const float z2, const Dot &p3, const float z3, uint32_t packed_voxel, int face_index, int chunk_index);
        std::array<std::vector<uint32_t>, Renderer::VOXEL_COUNT> visibleFaces(int chunk_index) const; 
        ~OcclusionTree();
    };
};

#endif //__CLIENT_OCCLUSIONTREE_H__
