#version 330 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D bloomTexture;

vec3 aces(vec3 x) {
    const float a = 2.51;
    const float b = 0.03;
    const float c = 2.43;
    const float d = 0.59;
    const float e = 0.14;
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

void main() {
    vec4 hdrColor = texture(screenTexture, TexCoords);
    vec3 mapped = aces(hdrColor.rgb);
    vec4 bloomColor = texture(bloomTexture, TexCoords);

    FragColor = mix(vec4(mapped, 1.0), bloomColor, 0.4);
}