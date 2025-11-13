#pragma once

#include <memory>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Mesh;

struct Material {
    glm::vec3 colour = glm::vec3(1.0f);
    GLuint textureID = 0;
};

class Object3D {
    private:
        std::shared_ptr<Mesh> mesh;

        glm::vec3 position;
        glm::vec3 scale = glm::vec3(1.0f);
        glm::vec3 rotation;
        Material mat;

    public:
        Object3D(Mesh* meshPtr, glm::vec3 pos = {0, 0, 0});

        void setMaterial(const Material& mat);
        void draw(GLuint shaderProgram, glm::mat4& proj, glm::mat4& view) const;

        glm::mat4 getModelMatrix() const;
};
