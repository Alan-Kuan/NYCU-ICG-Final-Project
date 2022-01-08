#version 430

uniform vec3 WorldLightPos;
uniform vec3 WorldCamPos;
uniform sampler2D texture;
uniform bool edge_glow;

in vec2 uv;
in vec4 normal;
in vec4 worldPos;

out vec4 color;

void main() {
	vec4 N = normalize(normal);
	vec4 L = normalize(vec4(WorldLightPos, 1.0) - worldPos);
    vec4 V = normalize(vec4(WorldCamPos, 1.0) - worldPos);

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

    float edge_level = dot(N, V);
    if (edge_glow && 0.0 <= edge_level && edge_level <= 0.4)
        color += vec4(1.0, 1.0, 1.0, 1.0) * pow(0.9, 5);
} 
