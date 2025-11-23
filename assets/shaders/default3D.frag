#version 330 core

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform sampler2D diffuseTexture;
uniform bool hasTexture;
uniform vec3 objectColour;
uniform vec3 viewPos;

struct Light {
    int type;
    vec3 position;
    vec3 direction;
    vec3 color;
    float intensity;
    float constant;
    float linear;
    float quadratic;
};

uniform Light lights[16];
uniform int numLights;

void main() {
    vec3 texColour = hasTexture ? texture(diffuseTexture, TexCoords).rgb : vec3(1.0);
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = vec3(0.0);

    for(int i = 0; i < numLights; i++) {
        vec3 lightDir;
        if (lights[i].type == 0) {
            lightDir = normalize(-lights[i].direction);
        } else {
            lightDir = normalize(lights[i].position - FragPos);
        }

        // Diffuse
        float diff = max(dot(norm, lightDir), 0.0);

        // Specular
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

        // Attenuation
        float attenuation = 1.0;
        if (lights[i].type == 1) {
            float distance = length(lights[i].position - FragPos);
            attenuation = 1.0 / (lights[i].constant + lights[i].linear * distance + lights[i].quadratic * distance * distance);
        }

        vec3 ambient = 0.1 * lights[i].color;
        vec3 diffuse = diff * lights[i].color;
        vec3 specular = spec * lights[i].color;

        result += (ambient + diffuse + specular) * attenuation * lights[i].intensity;
    }

    FragColor = vec4(result * texColour * objectColour, 1.0);
}
