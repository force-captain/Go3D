#pragma once
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 colour;
    glm::vec2 texCoords;
};

class Mesh {
    private:
        void setupMesh();
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        GLuint VAO, VBO, EBO;
        void draw() const;
    public:
        Mesh(const std::vector<Vertex>& verts, const std::vector<unsigned int>& inds);
        ~Mesh();

        friend class Object3D;
};
