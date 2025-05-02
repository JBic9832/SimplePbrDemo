#version 330 core

out vec4 FragColor;

in vec4 clipSpace;
in vec2 TexCoord;
in vec3 toCameraVector;
in vec3 fromLightVector;
in float visibility;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D dudvMap;
uniform sampler2D normalMap;
uniform sampler2D refractionDepth;
uniform float waveStrength;
uniform vec3 lightColor;

uniform float moveFactor;

vec3 fogColor = vec3(0.9, 0.9, 0.9);
const float shineDamper = 20.0;
const float reflectivity = 0.6;

void main() {
    vec2 ndc = (clipSpace.xy / clipSpace.w) / 2.0 + 0.5;
    vec2 refractTexCoords = ndc;
    vec2 reflectTexCoords = vec2(ndc.x, -ndc.y);

    float near = 0.1;
    float far = 1000.0;
    float depth = texture(refractionDepth, refractTexCoords).r;
    float floorDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));

    depth = gl_FragCoord.z;
    float waterDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));

    float waterDepth = floorDistance - waterDistance;

    vec2 distortedTexCoords = texture(dudvMap, vec2(TexCoord.x + moveFactor, TexCoord.y)).rg * 0.1;
    distortedTexCoords = TexCoord + vec2(distortedTexCoords.x, distortedTexCoords.y + moveFactor);
    vec2 total = (texture(dudvMap, distortedTexCoords).rg * 2.0 - 1.0);
    total *= waveStrength;

    refractTexCoords += total;
    reflectTexCoords += total;

    reflectTexCoords.x = clamp(reflectTexCoords.x, 0.001, 0.999);
    reflectTexCoords.y = clamp(reflectTexCoords.y, -0.999, -0.001);
    refractTexCoords = clamp(refractTexCoords, 0.001, 0.999);

    vec4 reflectColor = texture(reflectionTexture, reflectTexCoords);
    vec4 refractColor = texture(refractionTexture, refractTexCoords);

    vec3 viewVector = normalize(toCameraVector);
    float refractiveFactor = dot(viewVector, vec3(0.0, 1.0, 0.0));

    vec4 normalMapColor = texture(normalMap, distortedTexCoords);
    vec3 normal = vec3(normalMapColor.r * 2.0 - 1.0, normalMapColor.b, normalMapColor.g * 2.0 - 1.0);
    normal = normalize(normal);

    vec3 reflectedLight = reflect(normalize(fromLightVector), normal);
    float specular = max(dot(reflectedLight, viewVector), 0.0);
    specular = pow(specular, shineDamper);
    vec3 specularHighlight = lightColor * specular * reflectivity;

    FragColor = mix(reflectColor, refractColor, refractiveFactor);
    FragColor = mix(FragColor, vec4(0.0, 0.073, 0.122, 1.0), 0.15) + vec4(specularHighlight, 0.0);
    //Fog
    //FragColor = mix(vec4(fogColor, 1.0), FragColor, visibility);
}