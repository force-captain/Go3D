#include "graphics/objects/mesh.hpp"
#include <glm/common.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <memory>
#include <stdexcept>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include "graphics/objects/shader.hpp"

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

Mesh::Mesh(const std::string& path) : Mesh([&]() {
        auto [verts, inds] = loadFile(path);
        return Mesh(verts, inds);
        }()) {}


std::pair<std::vector<Vertex>, std::vector<unsigned int>> Mesh::loadFile(const std::string& path) {
    std::vector<Vertex> verts;
    std::vector<unsigned int> inds;

    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(path,
            aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

    if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
        throw std::runtime_error(importer.GetErrorString());

    aiMesh* mesh = scene->mMeshes[0];

    for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex v;
        v.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        v.normal = mesh->HasNormals() ? glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z) : glm::vec3(0.0f);
        v.colour = mesh->HasVertexColors(0) ? glm::vec3(mesh->mColors[0][i].r, mesh->mColors[0][i].g, mesh->mColors[0][i].b) : glm::vec3(1.0f);
        v.texCoords = mesh->HasTextureCoords(0) ? glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y) : glm::vec2(0.0f);
        verts.push_back(v);
    }

    for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++) {
            inds.push_back(face.mIndices[j]);
        }
    }

    return {verts, inds};
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
    // Positions          // Normals           // Colour         // texCoords
    {{-0.5f,-0.5f,-0.5f}, {0.0f, 0.0f,-1.0f},  {1.0f,1.0f,1.0f}, {0.0f,v0}},
    {{ 0.5f,-0.5f,-0.5f}, {0.0f, 0.0f,-1.0f},  {1.0f,1.0f,1.0f}, {1.0f,v0}},
    {{ 0.5f, 0.5f,-0.5f}, {0.0f, 0.0f,-1.0f},  {1.0f,1.0f,1.0f}, {1.0f,v1}},
    {{-0.5f, 0.5f,-0.5f}, {0.0f, 0.0f,-1.0f},  {1.0f,1.0f,1.0f}, {0.0f,v1}},

    {{-0.5f,-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f},  {1.0f,1.0f,1.0f}, {0.0f,v1}},
    {{ 0.5f,-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f},  {1.0f,1.0f,1.0f}, {1.0f,v1}},
    {{ 0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f},  {1.0f,1.0f,1.0f}, {1.0f,v2}},
    {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f},  {1.0f,1.0f,1.0f}, {0.0f,v2}},

    {{-0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f,1.0f,1.0f}, {1.0f,v2}},
    {{-0.5f, 0.5f,-0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f,1.0f,1.0f}, {1.0f,v3}},
    {{-0.5f,-0.5f,-0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f,1.0f,1.0f}, {0.0f,v3}},
    {{-0.5f,-0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f,1.0f,1.0f}, {0.0f,v2}},

    {{ 0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f},  {1.0f,1.0f,1.0f}, {1.0f,v3}},
    {{ 0.5f, 0.5f,-0.5f}, {1.0f, 0.0f, 0.0f},  {1.0f,1.0f,1.0f}, {1.0f,v4}},
    {{ 0.5f,-0.5f,-0.5f}, {1.0f, 0.0f, 0.0f},  {1.0f,1.0f,1.0f}, {0.0f,v4}},
    {{ 0.5f,-0.5f, 0.5f}, {1.0f, 0.0f, 0.0f},  {1.0f,1.0f,1.0f}, {0.0f,v3}},

    {{-0.5f,-0.5f,-0.5f}, {0.0f,-1.0f, 0.0f},  {1.0f,1.0f,1.0f}, {0.0f,v4}},
    {{ 0.5f,-0.5f,-0.5f}, {0.0f,-1.0f, 0.0f},  {1.0f,1.0f,1.0f}, {1.0f,v4}},
    {{ 0.5f,-0.5f, 0.5f}, {0.0f,-1.0f, 0.0f},  {1.0f,1.0f,1.0f}, {1.0f,v5}},
    {{-0.5f,-0.5f, 0.5f}, {0.0f,-1.0f, 0.0f},  {1.0f,1.0f,1.0f}, {0.0f,v5}},

    {{-0.5f, 0.5f,-0.5f}, {0.0f, 1.0f, 0.0f},  {1.0f,1.0f,1.0f}, {0.0f,v5}},
    {{ 0.5f, 0.5f,-0.5f}, {0.0f, 1.0f, 0.0f},  {1.0f,1.0f,1.0f}, {1.0f,v5}},
    {{ 0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f},  {1.0f,1.0f,1.0f}, {1.0f,v6}},
    {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f},  {1.0f,1.0f,1.0f}, {0.0f,v6}},
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
