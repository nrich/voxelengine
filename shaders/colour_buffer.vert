#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex_coord;

uniform mat4 screen;

out vec2 texCoord;

void main() {
    gl_Position = screen * vec4(pos.x, pos.y, 0, 1.0);
    texCoord = tex_coord;
}

