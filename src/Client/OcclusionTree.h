#ifndef __CLIENT_OCCLUSIONTREE_H__
#define __CLIENT_OCCLUSIONTREE_H__

#include <map>
#include <vector>
#include <bitset>

#include "Common/Shared.h"
#include "Math/Point2.h"
#include "Math/Point3.h"
#include "Renderer/Voxel.h"
#include "Common/Chunk.h"

#define Z_BUFFER_WIDTH  (160*2)
#define Z_BUFFER_HEIGHT (90*2)

namespace Client {
    typedef std::bitset<Z_BUFFER_WIDTH * Z_BUFFER_HEIGHT> Layer; 

    class OcclusionTree {
        struct Node {
            Dot min;
            Dot max;
            int z;
            Dot3 chunkIndex;

            Node(const Dot &min, const Dot &max, int z, Dot3 chunk_index) : min(min), max(max), z(z), chunkIndex(chunk_index) {
            }

            Node(const Node &other) : min(other.min), max(other.max), z(other.z), chunkIndex(other.chunkIndex) {
            }
        };


        std::map<int, std::vector<Node>> nodes;

        Layer layer;
    public:

        OcclusionTree();
        bool addQuad(const Dot &p0, const float z0, const Dot &p1, const float z1, const Dot &p2, const float z2, const Dot &p3, const float z3, const Dot3 &chunk_index);
        void drawQuad(const Dot &p0, const Dot &p1, const Dot &p2, const Dot &p3);
        std::vector<Dot3> visibleChunks();
        ~OcclusionTree();
    };
};

#endif //__CLIENT_OCCLUSIONTREE_H__
