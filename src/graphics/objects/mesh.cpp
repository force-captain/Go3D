#include "graphics/objects/mesh.hpp"
#include <glm/common.hpp>
#include <glm/glm.hpp>
#include <memory>

Mesh::Mesh(const std::vector<Vertex>& verts, const std::vector<unsigned int>& inds)
    : vertices(verts), indices(inds) {
    setupMesh();
    boundingBox.min = glm::vec3(FLT_MAX);
    boundingBox.max = glm::vec3(-FLT_MAX);
    for(const auto& v : vertices) {
        boundingBox.min = glm::min(boundingBox.min, v.position);
        boundingBox.max = glm::max(boundingBox.max, v.position);
    }
}

Mesh::~Mesh() {
    if (VAO) glDeleteVertexArrays(1, &VAO);
    if (VBO) glDeleteBuffers(1, &VBO);
    if (EBO) glDeleteBuffers(1, &EBO);
}


void Mesh::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    if (!indices.empty()) {
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    }

    // Vertices
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

    // Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    // Textures
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    // Colour
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, colour));

    glBindVertexArray(0);
}


std::vector<Vertex> cubeVertices = {
    // Positions          // Normals           // Colour         // texCoords
    {{-0.5f,-0.5f,-0.5f}, {0.0f, 0.0f,-1.0f},  {1.0f,1.0f,1.0f}, {0.0f,0.0f}},
    {{ 0.5f,-0.5f,-0.5f}, {0.0f, 0.0f,-1.0f},  {1.0f,1.0f,1.0f}, {1.0f,0.0f}},
    {{ 0.5f, 0.5f,-0.5f}, {0.0f, 0.0f,-1.0f},  {1.0f,1.0f,1.0f}, {1.0f,1.0f}},
    {{-0.5f, 0.5f,-0.5f}, {0.0f, 0.0f,-1.0f},  {1.0f,1.0f,1.0f}, {0.0f,1.0f}},

    {{-0.5f,-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f},  {1.0f,1.0f,1.0f}, {0.0f,0.0f}},
    {{ 0.5f,-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f},  {1.0f,1.0f,1.0f}, {1.0f,0.0f}},
    {{ 0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f},  {1.0f,1.0f,1.0f}, {1.0f,1.0f}},
    {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f},  {1.0f,1.0f,1.0f}, {0.0f,1.0f}},

    {{-0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f,1.0f,1.0f}, {1.0f,0.0f}},
    {{-0.5f, 0.5f,-0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f,1.0f,1.0f}, {1.0f,1.0f}},
    {{-0.5f,-0.5f,-0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f,1.0f,1.0f}, {0.0f,1.0f}},
    {{-0.5f,-0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f,1.0f,1.0f}, {0.0f,0.0f}},

    {{ 0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f},  {1.0f,1.0f,1.0f}, {1.0f,0.0f}},
    {{ 0.5f, 0.5f,-0.5f}, {1.0f, 0.0f, 0.0f},  {1.0f,1.0f,1.0f}, {1.0f,1.0f}},
    {{ 0.5f,-0.5f,-0.5f}, {1.0f, 0.0f, 0.0f},  {1.0f,1.0f,1.0f}, {0.0f,1.0f}},
    {{ 0.5f,-0.5f, 0.5f}, {1.0f, 0.0f, 0.0f},  {1.0f,1.0f,1.0f}, {0.0f,0.0f}},

    {{-0.5f,-0.5f,-0.5f}, {0.0f,-1.0f, 0.0f},  {1.0f,1.0f,1.0f}, {0.0f,1.0f}},
    {{ 0.5f,-0.5f,-0.5f}, {0.0f,-1.0f, 0.0f},  {1.0f,1.0f,1.0f}, {1.0f,1.0f}},
    {{ 0.5f,-0.5f, 0.5f}, {0.0f,-1.0f, 0.0f},  {1.0f,1.0f,1.0f}, {1.0f,0.0f}},
    {{-0.5f,-0.5f, 0.5f}, {0.0f,-1.0f, 0.0f},  {1.0f,1.0f,1.0f}, {0.0f,0.0f}},

    {{-0.5f, 0.5f,-0.5f}, {0.0f, 1.0f, 0.0f},  {1.0f,1.0f,1.0f}, {0.0f,1.0f}},
    {{ 0.5f, 0.5f,-0.5f}, {0.0f, 1.0f, 0.0f},  {1.0f,1.0f,1.0f}, {1.0f,1.0f}},
    {{ 0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f},  {1.0f,1.0f,1.0f}, {1.0f,0.0f}},
    {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f},  {1.0f,1.0f,1.0f}, {0.0f,0.0f}},
};

std::vector<unsigned int> cubeIndices = {
    0,1,2,    2,3,0,        // Back
    4,5,6,    6,7,4,        // Front
    8,9,10,   10,11,8,      // Left
    12,13,14, 14,15,12,     // Right
    16,17,18, 18,19,16,     // Bottom
    20,21,22, 22,23,20      // Top
};

std::vector<Vertex> quadVertices = {
    {{-0.5f,-0.5f,0.0f}, {0,0,1}, {1,1,1}, {0,0}},
    {{ 0.5f,-0.5f,0.0f}, {0,0,1}, {1,1,1}, {1,0}},
    {{ 0.5f, 0.5f,0.0f}, {0,0,1}, {1,1,1}, {1,1}},
    {{-0.5f, 0.5f,0.0f}, {0,0,1}, {1,1,1}, {0,1}}
};

std::vector<unsigned int> quadIndices = {
    0,1,2, 2,3,0
};

std::shared_ptr<Mesh> Mesh::getCube() {
    static std::shared_ptr<Mesh> cube = std::make_shared<Mesh>(cubeVertices, cubeIndices);
    return cube;
}

std::shared_ptr<Mesh> Mesh::getQuad() {
    static std::shared_ptr<Mesh> quad = std::make_shared<Mesh>(quadVertices, quadIndices);
    return quad;
}
