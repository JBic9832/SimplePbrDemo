#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;

out vec4 clipSpace;
out vec2 TexCoord;
out float visibility;
out vec3 toCameraVector;
out vec3 fromLightVector;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform vec3 cameraPosition;
uniform vec3 lightPosition;

const float density = 0.001;
const float gradient = 1.5;

const float tiling = 6;

void main() {
    vec4 worldPos = uModel * vec4(aPos, 1.0);

    TexCoord = aTexCoords * tiling;
    clipSpace = uProjection * uView * worldPos;
    gl_Position = clipSpace;
    toCameraVector = cameraPosition - worldPos.xyz;

    vec4 positionRelativeToCamera = uView * worldPos;
    float distance = length(positionRelativeToCamera.xyz);
    visibility = exp(-pow((distance * density), gradient));
    fromLightVector = worldPos.xyz - lightPosition;
}