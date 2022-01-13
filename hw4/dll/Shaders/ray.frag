#version 430

uniform vec3 WorldLightPos;
uniform sampler2D texture;

in vec2 uv;
in vec4 normal;
in vec4 worldPos;

out vec4 color;

void main() {
	vec4 N = normalize(normal);
	vec4 L = normalize(vec4(WorldLightPos, 1.0) - worldPos);

    float intensity = 1.0;
    float level = dot(N, L);
    if (level > 0.75)
        intensity = 1.0;
    else if (level > 0.3)
        intensity = 0.9;
    else
        intensity = 0.8;

    vec4 albedo = texture2D(texture, uv);
	color = albedo * vec4(intensity, intensity, intensity, 1.0);
} 