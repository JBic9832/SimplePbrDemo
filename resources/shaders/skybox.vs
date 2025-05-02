#version 330 core

layout(location = 0) in vec3 inPos;

out vec3 TexCoords;

uniform mat4 uProjection;
uniform mat4 uView;

void main() {
    TexCoords = inPos;
    gl_Position = uProjection * uView * vec4(inPos, 1.0);
}