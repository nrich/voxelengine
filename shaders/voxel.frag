#version 420 core

in vec4 colour;
in vec3 normal;
out vec4 FragColor;

void main(){
    FragColor = vec4(colour.x, colour.y, colour.z, colour.w);
}

