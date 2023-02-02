#version 330 core

// TODO:
// Implement Phong shading

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec2 texCoord;
out vec3 Normal;
out vec3 Position;

void main()
{
    gl_Position = Projection * View * Model * vec4(position, 1.0);
    texCoord = texcoord;
    Normal = mat3(transpose(inverse(Model))) * normal;
    Position = vec3(Model * vec4(position, 1.0));
}