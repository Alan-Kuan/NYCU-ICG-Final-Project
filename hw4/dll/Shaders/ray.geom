#version 430 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 6) out;

uniform mat4 M, V, P;
uniform vec3 WorldTailPos;

in VS_OUT {
    vec2 uv;
    vec4 normal;
    vec4 worldPos;
} gs_in[];

out vec2 uv;
out vec4 normal;
out vec4 worldPos;

void main() {
    vec4 G;
    for (int i = 0; i < 3; i++)
        G += gs_in[i].worldPos;
    G /= 3;

    for (int i = 0; i < 3; i++) { 
        uv = gs_in[i].uv;
        normal = gs_in[i].normal;
        worldPos = gs_in[i].worldPos;
        gl_Position = P * V * worldPos;
        EmitVertex();
    }
    EndPrimitive();

    for (int i = 0; i < 3; i++) { 
        vec4 N = normalize(gs_in[i].normal);
        vec4 T = normalize(vec4(WorldTailPos, 1.0) - gs_in[i].worldPos);

        float ratio = dot(N, T) * 0.5;
        if (ratio < 0) ratio = -0.05;

        uv = vec2(0.9, 0.9);  // red part of the texture
        normal = gs_in[i].normal;
        worldPos = gs_in[i].worldPos + T * ratio;
        worldPos += (G - gs_in[i].worldPos) * 0.2;
        gl_Position = P * V * worldPos;
        EmitVertex();
    }
    EndPrimitive();
}