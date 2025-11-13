#pragma once

#include "graphics/objects/object.hpp"
#include <GL/glew.h>
#include <memory>
#include <glm/glm.hpp>

class Shader;
class Mesh;

struct Material {
    glm::vec3 colour = glm::vec3(1.0f);
    GLuint textureID = 0;
    std::shared_ptr<Shader> shader;
};

class Object3D : public Object {
    private:
        std::shared_ptr<Mesh> mesh;

        glm::vec3 position;
        glm::vec3 scale = glm::vec3(1.0f);
        glm::vec3 rotation;
        Material mat;

    public:
        Object3D(std::shared_ptr<Mesh> meshPtr, glm::vec3 pos = {0, 0, 0});

        void setMaterial(const Material& mat);

        const Material& getMaterial() const { return mat; }
        std::shared_ptr<Mesh> getMesh() const { return mesh; }

        glm::mat4 getModelMatrix() const;

        const glm::
};
