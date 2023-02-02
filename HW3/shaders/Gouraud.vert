#version 330 core

// TODO:
// Implement Gouraud shading

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;

uniform vec3 Ka, Kd, Ks, La, Ld, Ls, lightPosition, cameraPosition;
uniform float alpha;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec2 texCoord;
out vec3 ambient;
out vec3 diffuse;
out vec3 specular;

void main()
{
    gl_Position = Projection * View * Model * vec4(position, 1.0);
    texCoord = texcoord;
    vec3 Normal = mat3(transpose(inverse(Model))) * normal;
    vec3 Position = vec3(Model * vec4(position, 1.0));

    ambient = La * Ka;
    float dotLN = 0.0;
    if (dot(normalize(lightPosition - Position), normalize(Normal)) > 0)
        dotLN = dot((normalize(lightPosition - Position)), normalize(Normal));
    diffuse = Ld * Kd * dotLN;
    float dotVR = 0.0;
    if (dot(normalize(cameraPosition - Position), normalize(reflect(-(normalize(lightPosition - Position)), normalize(Normal)))) > 0)
        dotVR = dot(normalize(cameraPosition - Position), normalize(reflect(-(normalize(lightPosition - Position)), normalize(Normal))));
    specular = Ls * Ks * pow(dotVR, alpha);
}