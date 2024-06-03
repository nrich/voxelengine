#ifndef __RENDERER_COLOURBUFFER_H__
#define __RENDERER_COLOURBUFFER_H__

#include <cstdint>
#include <string>

#include "Common/Shared.h"
#include "Common/Colour.h"
#include "Renderer/Program.h"

namespace Renderer {

class ColourBuffer {
    Program program;

    GLuint VBO;
    GLuint VAO;
    GLuint EBO;

    void createGLObjects();
public:
    ColourBuffer(Program &program);
    ColourBuffer(const std::string &vertex_shader_filename, const std::string &fragment_shader_filename);
    ColourBuffer(const std::string &shader_filename);

    void draw(const std::vector<uint32_t> buffer, int buffer_width, int buffer_height, const std::vector<Common::Colour> &colours, const MatrixF &screen);

    ~ColourBuffer();
};

}; // namespace Renderer

#endif //__RENDERER_COLOURBUFFER_H__
