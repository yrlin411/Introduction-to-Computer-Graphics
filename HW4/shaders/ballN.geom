#version 330 core

layout(triangles) in;
layout(points, max_vertices = 100) out;

in VS_OUT {
    vec3 normal;
    vec2 texCoord;
} gs_in[];

uniform mat4 P;
uniform float magic2;
uniform int crazy;

out vec4 color;

void main()
{
    if (crazy == 1) {
        color = vec4(1.0, 0.7, 0.8, 1.0);
        gl_Position = (gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position)/3;
        EmitVertex();
        EndPrimitive();
    }
    if (crazy == 2) {
        color = vec4(1.0, 0.7, 0.8, 1.0);
        vec3 point = (gs_in[0].normal + gs_in[1].normal + gs_in[2].normal)/3;
        gl_Position = ((gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position)/3) + vec4(point, 0.0) * (magic2 + 0.5 * 0.1 * magic2 * magic2);
        EmitVertex();
        EndPrimitive();
    }
}