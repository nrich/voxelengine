#include "Renderer/ColourBuffer.h"
#include "Common/Vertex.h"

using namespace Renderer;

ColourBuffer::ColourBuffer(Program &program) : program(std::move(program)) {
    createGLObjects();
}

ColourBuffer::ColourBuffer(const std::string &vertex_shader_filename, const std::string &fragment_shader_filename) : program(vertex_shader_filename, fragment_shader_filename) {
    createGLObjects();
}

ColourBuffer::ColourBuffer(const std::string &shader_filename) : program(shader_filename) {
    createGLObjects();
}

void ColourBuffer::createGLObjects() {
    std::array<Vertex, 4> vertices = {
        Vertex(Vec3F(0.0f, 0.0f, 0.0f), Vec2F(0,1)),
        Vertex(Vec3F(1.0f, 0.0f, 0.0f), Vec2F(1,1)),
        Vertex(Vec3F(1.0f, 1.0f, 0.0f), Vec2F(1,0)),
        Vertex(Vec3F(0.0f, 1.0f, 0.0f), Vec2F(0,0)),
    };

    GLuint indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)24);
    glEnableVertexAttribArray(1);
}

void ColourBuffer::draw(const std::vector<uint32_t> buffer, int buffer_width, int buffer_height, const std::vector<Common::Colour> &colours, const MatrixF &screen) {
    GLuint buffer_texture;
    glGenTextures(1, &buffer_texture);
    glBindTexture(GL_TEXTURE_2D, buffer_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    std::vector<uint8_t> pixels;
    pixels.resize(buffer.size() * 4);
    for (size_t i = 0, z = 0; i < buffer.size(); i++, z+=4) {
        auto colour = buffer[i] == 0 ? Common::Colour::Black : colours[buffer[i]-1];

        pixels[z+0] = colour.R();
        pixels[z+1] = colour.G();
        pixels[z+2] = colour.B();
        pixels[z+3] = colour.A();
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, buffer_width, buffer_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);

    program.use();
    glBindVertexArray(VAO);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    program("screen", screen);
    program("buffer_texture", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, buffer_texture);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glDeleteTextures(1, &buffer_texture);
}

ColourBuffer::~ColourBuffer() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
