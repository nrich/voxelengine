#include "Renderer/Voxel.h"

#include <iostream>

using namespace Renderer;

static const GLfloat face_vertex_buffer_data[VOXEL_COUNT][12] = {
    {   // VOXEL_BOTTOM
        0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
    },
    {   // VOXEL_TOP
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f,
    },
    {   // VOXEL_LEFT
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f,
    },
    {   // VOXEL_RIGHT
        1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
    },
    {   // VOXEL_FRONT
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f,

    },
    {   // VOXEL_BACK
        0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
    },
};

#define MAX_VOXEL_INSTANCES 32*32*32

Voxel::Face::Face() : VAO(0), IBO(0) {
}

Voxel::Face::~Face() {
    glDeleteBuffers(1, &IBO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

Voxel::Voxel() {
    for (size_t i = 0; i < faces.size(); i++) {
        glGenVertexArrays(1, &faces[i].VAO);

        glGenBuffers(1, &faces[i].VBO);
        glBindBuffer(GL_ARRAY_BUFFER, faces[i].VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(face_vertex_buffer_data[i]), face_vertex_buffer_data[i], GL_DYNAMIC_DRAW);

        glGenBuffers(1, &faces[i].IBO);
        glBindBuffer(GL_ARRAY_BUFFER, faces[i].IBO);
        glBufferData(GL_ARRAY_BUFFER, MAX_VOXEL_INSTANCES * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
    }
}

void Voxel::drawFaces(int index, std::vector<uint32_t> instance_buffer) {
    glBindBuffer(GL_ARRAY_BUFFER, faces[index].IBO);
    glBufferData(GL_ARRAY_BUFFER, MAX_VOXEL_INSTANCES * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, instance_buffer.size() * sizeof(GLuint), (const GLuint *)instance_buffer.data());

    glBindVertexArray(faces[index].VAO);

    glBindBuffer(GL_ARRAY_BUFFER, faces[index].VBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void*)0
    );

    glBindBuffer(GL_ARRAY_BUFFER, faces[index].IBO);
    glEnableVertexAttribArray(1);
    glVertexAttribIPointer(
        1,
        1,
        GL_UNSIGNED_INT,
        0,
        (void*)0
    );

    glVertexAttribDivisor(0, 0);
    glVertexAttribDivisor(1, 1);

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, instance_buffer.size());

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

uint32_t Voxel::Encode(int x, int y, int z, int colour_index, int add_width, int add_height) {
    return (x & 31) | ((y & 31) << 5) | ((z & 31) << 10) | ((colour_index & 127) << 15) | ((add_width & 31) << 22) | ((add_height & 31) << 27) ;
}

uint32_t Voxel::Encode(const Dot3 &d, int colour_index, int add_width, int add_height) {
    return Voxel::Encode(d.X(), d.Y(), d.X(), colour_index, add_width, add_height);
}

Voxel::~Voxel() {
}
