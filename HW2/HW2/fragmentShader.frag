#version 330 core
// TO DO:
// Implement fragment shader

uniform sampler2D texture;
uniform vec3 K;

in vec2 texCoord;
in vec3 Normal;

out vec4 color;

void main()
{
    color = texture2D(texture, texCoord);
    if(K.y == 1.0)
        if(color.r + color.g + color.b < 0.8) 
            color = vec4(0.2, 0.0, 0.0, 1.0);
    if(K.z == 1.0){
        if(color.r + color.g + color.b < 0.8)
            color = vec4(0.0, 0.0, 0.0, 0.0);
        else if(color.r + color.g + color.b >= 0.8 && color.r + color.g + color.b < 1.59995)
            color = vec4(1.0, 1.0, 1.0, 1.0); 
    }
    if(K.y == 0.0 && K.z == 0.0)
        color = texture2D(texture, texCoord);
}