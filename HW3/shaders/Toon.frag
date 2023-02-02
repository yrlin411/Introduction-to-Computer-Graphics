#version 330 core

uniform sampler2D texture;
uniform vec3 lightPosition;
uniform vec3 Kd;

in vec2 texCoord;
in vec3 Normal;
in vec3 Position;

out vec4 color;

void main()
{
    vec4 object_color = texture2D(texture, texCoord);

    // calculate angle between light and normal vector
    float cos = dot(normalize(lightPosition - Position), normalize(Normal));
    // decide threshold with cos (bigger cos smaller angle)
    float intensity;
    if (cos > 0.9) intensity = 1;
    else if (cos > 0.8) intensity = 0.8;
    else if (cos > 0.2) intensity = 0.6;
    else if (cos > 0.1) intensity = 0.4;
    else intensity = 0.2;

    color = vec4((Kd * vec3(object_color) * intensity), 1.0f);
}