#version 330 core

uniform sampler2D buffer_texture;
in vec2 texCoord;

out vec4 FragColor;

void main() {
    FragColor = texture2D(buffer_texture, texCoord);
}

