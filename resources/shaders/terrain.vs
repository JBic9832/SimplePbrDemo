#version 330 core

layout(location = 0) in vec3 aPos;

out vec3 VertPos;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform vec4 uWaterClipPlane;

void main() {
    VertPos = aPos;

    vec4 fragPos = uModel * vec4(aPos, 1.0);
    gl_ClipDistance[0] = dot(fragPos, uWaterClipPlane);

    gl_Position = uProjection * uView * fragPos;
}