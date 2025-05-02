#version 330 core

in vec3 VertPos;
out vec4 FragColor;

void main() {
    FragColor = vec4(vec3(VertPos.y), 1.0);
    FragColor = clamp(FragColor, 0.0, 1.0);
}