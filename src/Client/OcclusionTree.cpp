#include "Client/OcclusionTree.h"
#include <algorithm>
#include <iostream>
#include <bitset>

using namespace Client;

static const int Z_BUFFER_WIDTH = 160*1;
static const int Z_BUFFER_HEIGHT = 90*1;

static std::pair<Dot, Dot> FindSmallestQuad(const Dot &p0, const Dot &p1, const Dot &p2, const Dot &p3) {
    std::vector<int> x_list = {p0.X(), p1.X(), p2.X(), p3.X()};
    std::vector<int> y_list = {p0.Y(), p1.Y(), p2.Y(), p3.Y()};

    std::sort(x_list.begin(), x_list.end());
    std::sort(y_list.begin(), y_list.end());

    Dot min(x_list[1], y_list[1]);
    Dot max(x_list[2], y_list[2]);

    return std::make_pair(min, max);
}

OcclusionTree::OcclusionTree() {

}

static void draw_quad_on_layer(const Dot &min, const Dot &max, std::bitset<Z_BUFFER_WIDTH * Z_BUFFER_HEIGHT> &layer) {
    for (int y = min.Y(); y < max.Y(); y++) {
        for (int x = min.X(); x < max.X(); x++) {
            layer[y * Z_BUFFER_WIDTH + x] = 1;
        }
    }
}

/*
bool OcclusionTree::isVisible(const OcclusionTree::Node &node) const {
    for (auto const &[z, nodelist]: nodes) {
        if (z >= node.z)
            continue;

        if (z < node.z) {
            std::bitset<Z_BUFFER_WIDTH * Z_BUFFER_HEIGHT> node_layer = {0};
            std::bitset<Z_BUFFER_WIDTH * Z_BUFFER_HEIGHT> n_layer = {0};

            draw_quad_on_layer(node.min, node.max, node_layer);

            for (auto const &n : nodelist) {
                draw_quad_on_layer(n.min, n.max, n_layer);
            }

            auto compare = (node_layer ^ n_layer) & node_layer;

            if (compare.none())
                return false;
        }
    }

    return true;
}


void OcclusionTree::occluder(const OcclusionTree::Node &node) {
    std::bitset<Z_BUFFER_WIDTH * Z_BUFFER_HEIGHT> main_layer = {0};

    for (auto &[z, nodelist]: nodes) {
        if (z <= node.z) {
            for (auto const &n : nodelist) {
                draw_quad_on_layer(n.min, n.max, main_layer);
            }
        } else {
            nodelist.erase(
                std::remove_if(
                    nodelist.begin(), 
                    nodelist.end(),
                    [main_layer](const OcclusionTree::Node &n) {
                        std::bitset<Z_BUFFER_WIDTH * Z_BUFFER_HEIGHT> node_layer = {0};
                        draw_quad_on_layer(n.min, n.max, node_layer);

                        auto compare = (node_layer ^ main_layer) & node_layer;

                        return compare.none();
                    }
                ), 
                nodelist.end()
            ); 

        }
    }
}
*/

bool OcclusionTree::addQuad(const Dot &p0, const float z0, const Dot &p1, const float z1, const Dot &p2, const float z2, const Dot &p3, const float z3, uint32_t packed_voxel, int face_index, int chunk_index) {
    const int z = (int)((z0 + z1 + z2 + z3) / 4);
    //std::array<int, 4> z_list = {(int)z0, (int)z1, (int)z2, (int)z3};
    //const int z = *std::min_element(z_list.begin(), z_list.end());

    auto smallest_quad = FindSmallestQuad(p0, p1, p2, p3);

    OcclusionTree::Node node(smallest_quad.first, smallest_quad.second, z, packed_voxel, face_index, chunk_index);

    auto it = nodes.find(z);
    if (it != nodes.end()) {
        it->second.push_back(node);
    } else {
        nodes[z] = {node};
    }

    return true;
}

std::array<std::vector<uint32_t>, Renderer::VOXEL_COUNT> OcclusionTree::visibleFaces(int chunk_index) const {
    std::array<std::vector<uint32_t>, Renderer::VOXEL_COUNT> faces;

    std::bitset<Z_BUFFER_WIDTH * Z_BUFFER_HEIGHT> main_layer = {0};

    for (auto &[z, nodelist]: nodes) {
        int visible = 0;

        for (auto const &node : nodelist) {
            if (z > 25 && chunk_index == node.chunkIndex) {
                faces[node.faceIndex].push_back(node.packedVoxel);
                continue;
            }

            std::bitset<Z_BUFFER_WIDTH * Z_BUFFER_HEIGHT> node_layer = {0};
            draw_quad_on_layer(node.min, node.max, node_layer);

            auto compare = (node_layer ^ main_layer) & node_layer;

            if (compare.any() && chunk_index == node.chunkIndex) {
                faces[node.faceIndex].push_back(node.packedVoxel);
                visible++;
            }
        }

        for (auto const &node : nodelist) {
            if (z > 25)
                continue;
            draw_quad_on_layer(node.min, node.max, main_layer);
        }

        float covered = (float)main_layer.count()/(float)main_layer.size();
        if (covered > 0.60)
            break;
//        std::cerr << z << " " << nodelist.size() << " " << visible << " " << covered << "\n";
    }

    return faces;
}

OcclusionTree::~OcclusionTree() {

}

