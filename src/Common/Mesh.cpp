#include "Common/Mesh.h"

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

                    faces_list[face_index].push_back(Renderer::Voxel::Encode(x, y, z, (int)block - 1));
                }
            }
        }
    }
}

Mesh::~Mesh() {

}
