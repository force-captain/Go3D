#version 330 core

layout(location = 0) in vec3 aPos;      // Vertex position
layout(location = 1) in vec3 aNormal;   // Vertex normal (optional, for lighting)

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;   // World-space position for lighting
out vec3 Normal;    // Transformed normal

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}

