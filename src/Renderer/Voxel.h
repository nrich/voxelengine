#ifndef __RENDERER_VOXEL_H__
#define __RENDERER_VOXEL_H__

#include <cstdint>
#include <string>
#include <vector>
#include <array>

#include "Common/Shared.h"
#include "Math/Point3.h"
#include "Math/Matrix.h"

namespace Renderer {

enum {
    VOXEL_BOTTOM = 0,
    VOXEL_TOP,
    VOXEL_LEFT,
    VOXEL_RIGHT,
    VOXEL_FRONT,
    VOXEL_BACK,
    VOXEL_COUNT
};

class Voxel {
    struct Face {
        GLuint VAO;
        GLuint VBO;
        GLuint IBO;

        Face();
        ~Face(); 
    };

    std::array<Face, VOXEL_COUNT> faces;
public:
    Voxel();
    /*
    GLuint operator()() const {
        return EBO;
    }
    */

    void drawFaces(int index, std::vector<uint32_t> instance_buffer);

    static uint32_t Encode(int x, int y, int z, int colour_index, int add_width=0, int add_height=0);
    static uint32_t Encode(const Dot3 &d, int colour_index, int add_width=0, int add_height=0);
    static Dot3 Decode(uint32_t packed_voxel);

    ~Voxel();
}; 

}; // namespace Renderer

#endif //__RENDERER_VOXEL_H__
