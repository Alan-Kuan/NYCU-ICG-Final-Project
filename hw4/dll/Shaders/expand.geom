#version 430 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform mat4 M, V, P;
uniform float ratio;

in VS_OUT {
    vec2 uv;
    vec4 normal;
    vec4 worldPos;
} gs_in[];

out vec2 uv;
out vec4 normal;
out vec4 worldPos;

void main() {
    for (int i = 0; i < 3; i++) {
        uv = gs_in[i].uv;
        normal = gs_in[i].normal;
        worldPos = gs_in[i].worldPos + gs_in[i].normal * ratio;
        gl_Position = P * V * worldPos;
        EmitVertex();
    }
    EndPrimitive();
}