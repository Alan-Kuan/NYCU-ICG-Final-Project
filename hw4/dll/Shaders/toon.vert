#version 430

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 texcoord;

uniform mat4 M, V, P;

out vec2 uv;
out vec4 normal;
out vec4 worldPos;

void main() {
  uv = texcoord;
  normal = M * vec4(in_normal, 0.0);
  worldPos = M * vec4(in_position, 1.0);
  gl_Position = P * V * worldPos;
}