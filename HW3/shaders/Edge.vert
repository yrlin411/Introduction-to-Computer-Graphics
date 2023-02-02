#version 330 core

// TODO:
// Implement Edge effect

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec3 Normal;
out vec3 Position;

void main()
{
    gl_Position = Projection * View * Model * vec4(position, 1.0);
    Normal = mat3(transpose(inverse(Model))) * normal;
    Position = vec3(Model * vec4(position, 1.0));
}