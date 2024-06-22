#include "Common/Mesh.h"

#include <iostream>

using namespace Common;

Mesh::Mesh(const Chunk *chunk) : occluder(std::nullopt), visible(std::nullopt) {
    for (int y = 0; y < BLOCKS_LEN; y++) {
        for (int z = 0; z < BLOCKS_LEN; z++) {
            for (int x = 0; x < BLOCKS_LEN; x++) {
                auto block = chunk->block(x, y, z);

                if (block == Common::Block::Empty)
                    continue;

                for (int face_index = 0; face_index < Renderer::VOXEL_COUNT; face_index++) { 
                    int nx = x;
                    int ny = y;
                    int nz = z;

                    switch (face_index) {
                        case Renderer::VOXEL_BOTTOM:
                            ny -= 1;
                            break;
                        case Renderer::VOXEL_TOP:
                            ny += 1;
                            break;
                        case Renderer::VOXEL_LEFT:
                            nx -= 1;
                            break;
                        case Renderer::VOXEL_RIGHT:
                            nx += 1;
                            break;
                        case Renderer::VOXEL_FRONT:
                            nz -= 1;
                            break;
                        case Renderer::VOXEL_BACK:
                            nz += 1;
                            break;
                        default:
                            break;
                    }

                    if (chunk->block(nx, ny, nz) != Common::Block::Empty)
                        continue;

                    facesList[face_index].push_back(Renderer::Voxel::Encode(x, y, z, (int)block - 1));
                }
            }
        }
    }

    for (int face_index = 0; face_index < Renderer::VOXEL_COUNT; face_index++) {
        std::sort(facesList[face_index].begin(), facesList[face_index].end(), [face_index](const uint32_t a, const uint32_t b) {
            Dot3 a_pos = Renderer::Voxel::Decode(a);
            Dot3 b_pos = Renderer::Voxel::Decode(b);

            switch (face_index) {
                case Renderer::VOXEL_BOTTOM:
                    if (a_pos.Y() == b_pos.Y()) {
                        if (a_pos.Z() == b_pos.Z())
                            return a_pos.X() < b_pos.X();
                        else
                            return a_pos.Z() < b_pos.Z();
                    }
                    return a_pos.Y() < b_pos.Y();
                    break;
                case Renderer::VOXEL_TOP:
                    if (a_pos.Y() == b_pos.Y()) {
                        if (a_pos.Z() == b_pos.Z())
                            return a_pos.X() < b_pos.X();
                        else
                            return a_pos.Z() < b_pos.Z();
                    }
                    return a_pos.Y() > b_pos.Y();
                    break;
                case Renderer::VOXEL_LEFT:
                    if (a_pos.X() == b_pos.X()) {
                        if (a_pos.Z() == b_pos.Z())
                            return a_pos.Y() < b_pos.Y();
                        else
                            return a_pos.Z() < b_pos.Z();
                    }
                    return a_pos.X() < b_pos.X();
                    break;
                case Renderer::VOXEL_RIGHT:
                    if (a_pos.X() == b_pos.X()) {
                        if (a_pos.Z() == b_pos.Z())
                            return a_pos.Y() < b_pos.Y();
                        else
                            return a_pos.Z() < b_pos.Z();
                    }
                    return a_pos.X() > b_pos.X();
                    break;
                case Renderer::VOXEL_FRONT:
                    if (a_pos.Z() == b_pos.Z()) {
                        if (a_pos.X() == b_pos.X())
                            return a_pos.Y() < b_pos.Y();
                        else
                            return a_pos.X() < b_pos.X();
                    }
                    return a_pos.Z() < b_pos.Z();
                    break;
                case Renderer::VOXEL_BACK:
                    if (a_pos.Z() == b_pos.Z()) {
                        if (a_pos.X() == b_pos.X())
                            return a_pos.Y() < b_pos.Y();
                        else
                            return a_pos.X() < b_pos.X();
                    }
                    return a_pos.Z() > b_pos.Z();
                    break;
                default:
                    break;
            }
        });
    }

    if (!facesList[Renderer::VOXEL_TOP].size() || !facesList[Renderer::VOXEL_BOTTOM].size())
        return;

    if (!facesList[Renderer::VOXEL_LEFT].size() || !facesList[Renderer::VOXEL_RIGHT].size())
        return;

    if (!facesList[Renderer::VOXEL_FRONT].size() || !facesList[Renderer::VOXEL_BACK].size())
        return;

    auto top_face = Renderer::Voxel::Decode(*facesList[Renderer::VOXEL_TOP].begin());
    auto bottom_face = Renderer::Voxel::Decode(*facesList[Renderer::VOXEL_BOTTOM].begin());
    auto left_face = Renderer::Voxel::Decode(facesList[Renderer::VOXEL_LEFT].back());
    auto right_face = Renderer::Voxel::Decode(facesList[Renderer::VOXEL_RIGHT].back());
    auto front_face = Renderer::Voxel::Decode(facesList[Renderer::VOXEL_FRONT].back());
    auto back_face = Renderer::Voxel::Decode(facesList[Renderer::VOXEL_BACK].back());

    //std::cerr<<top_face.toString() << " " << bottom_face.toString() << "\n";

    occluder = std::optional<std::pair<Dot3, Dot3>>(std::make_pair<Dot3, Dot3>(Dot3(left_face.X(), bottom_face.Y(), front_face.Z()), Dot3(right_face.X(), top_face.Y(), back_face.Z())));
}

Mesh::~Mesh() {

}

