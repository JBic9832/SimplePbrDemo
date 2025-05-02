#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform vec3 uLightColor;
uniform vec3 uLightPos;
uniform vec3 uCameraPos;
uniform Material uMaterial;
uniform samplerCube uSkyboxTexture;

void main() {
    vec3 ambient = uLightColor * uMaterial.ambient;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(uLightPos - FragPos);

    vec3 viewDir = normalize(uCameraPos - FragPos);
    vec3 viewReflection = reflect(-viewDir, norm);
    vec3 reflectedColor = texture(uSkyboxTexture, viewReflection).rgb;

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = (diff * uMaterial.diffuse) * uLightColor;

    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128 * uMaterial.shininess);
    vec3 specular = (uMaterial.specular * spec) * uLightColor;

    vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);
}