#ifndef __RENDERER_TEXT_H__
#define __RENDERER_TEXT_H__

#include <cstdint>
#include <string>

#include "Common/Shared.h"
#include "Renderer/Program.h"

namespace Renderer {

class Text {
    GLuint nulTexture;
    GLuint blockTexture;
    std::array<GLuint, 256> fontTextures;

    Program program;

    void loadPixelBitmaps();
    static GLuint CreateTexture(std::array<char, 256> pixels);

    GLuint VBO;
    GLuint VAO;
    GLuint EBO;

    void createGLObjects();
public:
    Text(Program &program);
    Text(const std::string &vertex_shader_filename, const std::string &fragment_shader_filename);
    Text(const std::string &shader_filename);

    void draw(const std::string &str, const Common::Colour &colour, const MatrixF &screen);


    ~Text();
};

}; // namespace Renderer

#endif //__RENDERER_TEXT_H__
