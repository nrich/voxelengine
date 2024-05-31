#version 330 core

layout(location = 0) in vec3 base_pos;
layout(location = 1) in int voxel;

uniform mat4 mvp;
uniform vec4[127] colours;
uniform int face_type;
uniform vec3 world_offset;


/*
    VOXEL_BOTTOM = 0
    VOXEL_TOP = 1
    VOXEL_LEFT = 2
    VOXEL_RIGHT = 3
    VOXEL_FRONT = 4
    VOXEL_BACK = 5
*/

out vec4 colour;
out vec3 normal;

vec3 NORMALS[6];

void main(){
    NORMALS[0] = vec3( 0.0,  1.0,  0.0);   // Y+
    NORMALS[1] = vec3( 0.0, -1.0,  0.0);   // Y-
    NORMALS[2] = vec3( 1.0,  0.0,  0.0);   // X+
    NORMALS[3] = vec3(-1.0,  0.0,  0.0);   // X-
    NORMALS[4] = vec3( 0.0,  0.0,  1.0);   // Z+
    NORMALS[5] = vec3( 0.0,  0.0, -1.0);   // Z-

    int xpos = voxel & 31; 
    int ypos = (voxel >> 5) & 31; 
    int zpos = (voxel >> 10) & 31; 
    int cind = (voxel >> 15) & 127;
    int add_width = (voxel >> 22) & 31;
    int add_height = (voxel >> 27) & 31;

    vec3 pos = base_pos;

/*
    if (face_type == 0 || face_type == 1) {
        if (pos.x == 1) {
            pos.x += add_width;
        }

        if (pos.z == 1) {
            pos.z += add_height;
        }
    } else if (face_type == 2 || face_type == 3) {
        if (pos.z == 1) {
            pos.z += add_width;
        }

        if (pos.y == 1) {
            pos.y += add_height;
        }
    } else if (face_type == 4 || face_type == 5) {
        if (pos.x == 1) {
            pos.x += add_width;
        }

        if (pos.y == 1) {
            pos.y += add_height;
        }
    }
*/

    colour = colours[cind] * (1.0 - (face_type * 0.025));
    normal = NORMALS[face_type];
    gl_Position = mvp * vec4(pos + vec3(xpos, ypos, zpos) + world_offset, 1.0);
}

