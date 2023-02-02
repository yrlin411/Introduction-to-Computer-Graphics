#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_texcoord;

uniform mat4 M, V, P, normalM;
 
out VS_OUT {
    vec3 normal;
    vec2 texCoord;
} vs_out;

void main() {
    gl_Position = P * V * M * vec4(in_position, 1.0);
    vs_out.normal = normalize((normalM * vec4(in_normal, 1.0)).xyz);
    vs_out.texCoord = in_texcoord;
}