#version 330 core

uniform sampler2D texture;
uniform vec3 Ka, Kd, Ks, La, Ld, Ls, lightPosition, cameraPosition;
uniform float alpha;

in vec2 texCoord;
in vec3 Normal;
in vec3 Position;

out vec4 color;

void main()
{
    vec4 object_color = texture2D(texture, texCoord);

    vec3 ambient = La * Ka * vec3(object_color);
    float dotLN = 0.0;
    if (dot(normalize(lightPosition - Position), normalize(Normal)) > 0)
        dotLN = dot((normalize(lightPosition - Position)), normalize(Normal));
    vec3 diffuse = Ld * Kd * vec3(object_color) * dotLN;
    float dotVR = 0.0;
    if (dot(normalize(cameraPosition - Position), normalize(reflect(-(normalize(lightPosition - Position)), normalize(Normal)))) > 0)
        dotVR = dot(normalize(cameraPosition - Position), normalize(reflect(-(normalize(lightPosition - Position)), normalize(Normal))));
    vec3 specular = Ls * Ks * pow(dotVR, alpha);

    color = vec4((ambient + diffuse + specular), 1.0f);
}

