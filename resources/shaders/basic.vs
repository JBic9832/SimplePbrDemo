#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out vec3 Normal;
out vec3 WorldPos;
out vec2 TexCoords;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform mat3 uNormalMatrix;
uniform vec4 uWaterClipPlane;

void main() {
    WorldPos = vec3(uModel * vec4(aPos, 1.0));
    TexCoords = aTexCoord;
    Normal = uNormalMatrix * aNormal;

    vec4 fragPos = uModel * vec4(aPos, 1.0);
    gl_ClipDistance[0] = dot(fragPos, uWaterClipPlane);

    gl_Position = uProjection * uView * vec4(WorldPos, 1.0);
}