#version 430

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 texcoord;

uniform mat4 M, V, P;

out VS_OUT {
    vec2 uv;
    vec4 normal;
    vec4 worldPos;
} vs_out;

void main() {
    vs_out.uv = texcoord;
    vs_out.normal = M * vec4(in_normal, 0.0);
    vs_out.worldPos = M * vec4(in_position, 1.0);
    gl_Position = P * V * vs_out.worldPos;
}