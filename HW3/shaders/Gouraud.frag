#version 330 core

uniform sampler2D texture;

in vec3 ambient;
in vec3 diffuse;
in vec3 specular;
in vec2 texCoord;

out vec4 color;

void main()
{
    vec4 object_color = texture2D(texture, texCoord);
    vec3 t = ambient * vec3(object_color) + diffuse * vec3(object_color) + specular;
    color = vec4(t, 1.0f);
}

