#version 330 core

uniform vec4 textColour;
uniform sampler2D charTexture;
in vec2 texCoord;

out vec4 FragColor;

void main() {
    FragColor = texture2D(charTexture, texCoord) * textColour;
}

