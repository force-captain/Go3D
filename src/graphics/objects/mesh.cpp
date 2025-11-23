#include "graphics/objects/mesh.hpp"
#include <algorithm>
#include <fstream>
#include <glm/common.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include "graphics/objects/shader.hpp"

Mesh::Mesh(const std::vector<Vertex>& verts, const std::vector<unsigned int>& inds)
    : vertices(verts), indices(inds) {
    setupMesh();
    initBox();
}

Mesh::Mesh(const std::string& path) {
    auto [verts, inds] = loadFile(path);
    vertices = verts;
    indices = inds;
    setupMesh();
    initBox();
}

void Mesh::initBox() {
    boundingBox.min = glm::vec3(FLT_MAX);
    boundingBox.max = glm::vec3(-FLT_MAX);
    for (const auto& v : vertices) {
        boundingBox.min = glm::min(boundingBox.min, v.position);
        boundingBox.max = glm::max(boundingBox.max, v.position);
    }
}

std::pair<std::vector<Vertex>, std::vector<unsigned int>> Mesh::loadFile(const std::string& path) {
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> UVs;

    std::vector<Vertex> verts;
    std::vector<unsigned int> inds;

    std::ifstream file(path);
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") {
            glm::vec3 pos;
            iss >> pos.x >> pos.y >> pos.z;
            positions.push_back(pos);
        } else if (prefix == "vn") {
            glm::vec3 norm;
            iss >> norm.x >> norm.y >> norm.z;
            normals.push_back(norm);
        } else if (prefix == "vt") {
            glm::vec2 uv;
            iss >> uv.x >> uv.y;
            UVs.push_back(uv);
        } else if (prefix == "f") {
            std::string vert;
            for (int i = 0; i < 3; i++) {
                iss >> vert;
                std::replace(vert.begin(), vert.end(), '/', ' ');
                std::istringstream viss(vert);
                int vIdx, vtIdx, vnIdx;
                viss >> vIdx >> vtIdx >> vnIdx;
                Vertex v;
                v.position = positions[vIdx - 1];
                v.normal = normals[vnIdx - 1];
                v.texCoords = UVs[vtIdx - 1];
                verts.push_back(v);
                inds.push_back(verts.size() - 1);
            }
        }
    }

    return { verts, inds };
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

    glBindVertexArray(0);
}


// 6.0 x 0.5 x 6.0
// Top/bottom 1x1
// Sides 1x12

float v0 = 0.0f;
float v1 = 16.0f / 448.0f;
float v2 = 2 * v1;
float v3 = 3 * v1;
float v4 = 4 * v1;
float v5 = v4 + (192.0f / 448.0f);
float v6 = 1.0f;

// Top: v5-v6
// Bottom: v4-v5



std::vector<Vertex> cubeVertices = {
    // Positions          // Normals          // texCoords
    {{-0.5f,-0.5f,-0.5f}, {0.0f, 0.0f,-1.0f}, {0.0f,v0}},
    {{ 0.5f,-0.5f,-0.5f}, {0.0f, 0.0f,-1.0f}, {1.0f,v0}},
    {{ 0.5f, 0.5f,-0.5f}, {0.0f, 0.0f,-1.0f}, {1.0f,v1}},
    {{-0.5f, 0.5f,-0.5f}, {0.0f, 0.0f,-1.0f}, {0.0f,v1}},

    {{-0.5f,-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f,v1}},
    {{ 0.5f,-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f,v1}},
    {{ 0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f,v2}},
    {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f,v2}},

    {{-0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f},{1.0f,v2}},
    {{-0.5f, 0.5f,-0.5f}, {-1.0f, 0.0f, 0.0f},{1.0f,v3}},
    {{-0.5f,-0.5f,-0.5f}, {-1.0f, 0.0f, 0.0f},{0.0f,v3}},
    {{-0.5f,-0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f},{0.0f,v2}},

    {{ 0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f,v3}},
    {{ 0.5f, 0.5f,-0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f,v4}},
    {{ 0.5f,-0.5f,-0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f,v4}},
    {{ 0.5f,-0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f,v3}},

    {{-0.5f,-0.5f,-0.5f}, {0.0f,-1.0f, 0.0f}, {0.0f,v4}},
    {{ 0.5f,-0.5f,-0.5f}, {0.0f,-1.0f, 0.0f}, {1.0f,v4}},
    {{ 0.5f,-0.5f, 0.5f}, {0.0f,-1.0f, 0.0f}, {1.0f,v5}},
    {{-0.5f,-0.5f, 0.5f}, {0.0f,-1.0f, 0.0f}, {0.0f,v5}},

    {{-0.5f, 0.5f,-0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f,v5}},
    {{ 0.5f, 0.5f,-0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f,v5}},
    {{ 0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f,v6}},
    {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f,v6}},
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
    {{-0.5f,-0.5f,0.0f}, {0,0,1}, {0,0}},
    {{ 0.5f,-0.5f,0.0f}, {0,0,1}, {1,0}},
    {{ 0.5f, 0.5f,0.0f}, {0,0,1}, {1,1}},
    {{-0.5f, 0.5f,0.0f}, {0,0,1}, {0,1}}
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

void Mesh::loadTexture(const std::string& path, Material& mat) const {
    stbi_set_flip_vertically_on_load(true);
    int width, height, channels;
    
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    if (!data) {
        std::cerr << "Failed to load texture at path: " << path << "\n";
        return;
    }

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    GLenum format = (channels == 1) ? GL_RED : (channels == 3) ? GL_RGB : GL_RGBA;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);

    mat.textureID = tex;
}
