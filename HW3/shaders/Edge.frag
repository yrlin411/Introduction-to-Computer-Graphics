#version 330 core

uniform vec3 cameraPosition;

in vec3 Normal;
in vec3 Position;

out vec4 color;

void main()
{
    float edge = dot(normalize(cameraPosition - Position), normalize(Normal));
    if (edge < 0.1) color = 0.9 * vec4(1.0, 0.0, 0.0, 1.0);
    else if (edge < 0.15) color = 0.7 * vec4(1.0, 0.0, 0.0, 1.0);
    else if (edge < 0.2) color = 0.5 * vec4(1.0, 0.0, 0.0, 1.0);
}