#pragma once
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 colour;
    glm::vec2 texCoords;
};

struct BoundingBox {
    glm::vec3 min;
    glm::vec3 max;
};

class Mesh {
    private:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        BoundingBox boundingBox;
        GLuint VAO = 0;
        GLuint VBO = 0;
        GLuint EBO = 0;

        void setupMesh();

    public:
        Mesh(const std::vector<Vertex>& verts, const std::vector<unsigned int>& inds);
        ~Mesh();

        static std::shared_ptr<Mesh> getCube();
        static std::shared_ptr<Mesh> getQuad();

        size_t indexCount() const { return indices.size(); }
        size_t vertexCount() const { return vertices.size(); }

        GLuint getVAO() const { return VAO; }
        GLuint getVBO() const { return VBO; }
        GLuint getEBO() const { return EBO; }
        BoundingBox getBoundingBox() const { return boundingBox; }
};
