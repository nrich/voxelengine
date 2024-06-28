#include "Common/Mesh.h"

#include <iostream>

using namespace Common;

Mesh::Mesh(const Chunk *chunk) {
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

    for (int oy = 0, dy = 0; oy < BLOCKS_LEN; oy+=8, dy++) {
        for (int oz = 0, dz = 0; oz < BLOCKS_LEN; oz+=8, dz++) {
            for (int ox = 0, dx = 0; ox < BLOCKS_LEN; ox+=8, dx++) {
                int coarse = 0;

                for (int iy = 0; iy < 8; iy++) {
                    for (int iz = 0; iz < 8; iz++) {
                        for (int ix = 0; ix < 8; ix++) {
                            int x = ox + ix;
                            int y = oy + iy;
                            int z = oz + iz;

                            if (chunk->block(x, y, z) != Common::Block::Empty)
                                coarse++;
                        }
                    }
                }

                coarseOcclusion[dy*4*4 + dz*4 + dx] = Occluder(Dot3(ox, oy, oz), Dot3(ox+8, oy+8, oz+8), coarse, 8*8*8);
            }
        }
    }

    for (int oy = 0, dy = 0; oy < BLOCKS_LEN; oy+=4, dy++) {
        for (int oz = 0, dz = 0; oz < BLOCKS_LEN; oz+=4, dz++) {
            for (int ox = 0, dx = 0; ox < BLOCKS_LEN; ox+=4, dx++) {
                int fine = 0;

                for (int iy = 0; iy < 4; iy++) {
                    for (int iz = 0; iz < 4; iz++) {
                        for (int ix = 0; ix < 4; ix++) {
                            int x = ox + ix;
                            int y = oy + iy;
                            int z = oz + iz;

                            if (chunk->block(x, y, z) != Common::Block::Empty)
                                fine++;
                        }
                    }
                }

                fineOcclusion[dy*8*8 + dz*8 + dx] = Occluder(Dot3(ox, oy, oz), Dot3(ox+4, oy+4, oz+4), fine, 4*4*4);
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
}

Mesh::~Mesh() {
}

