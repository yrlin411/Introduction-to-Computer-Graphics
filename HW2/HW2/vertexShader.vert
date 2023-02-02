#version 330 core
// TO DO:
// Implement vertex shader
// note: remember to set gl_Position

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoord;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 K;

out vec2 texCoord;
out vec3 Normal;

void main()
{
    vec4 v = vec4(position, 1.0);
    if(K.x == 1.0)
        v.z = 0.0;
    if(K.x == 0.0)
        v = vec4(position, 1.0);
    gl_Position = Projection * View * Model * v;
    texCoord = texcoord;
    Normal = normal;
}