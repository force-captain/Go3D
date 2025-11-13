#pragma once

#include "graphics/objects/object.hpp"
#include <GL/glew.h>
#include <memory>
#include <glm/glm.hpp>

class Mesh;

class Object3D : public Object {
    private:
        std::shared_ptr<Mesh> mesh;

        glm::vec3 position;
        glm::vec3 scale = glm::vec3(1.0f);
        glm::vec3 rotation = glm::vec3(0.0f);

    public:
        Object3D(std::shared_ptr<Mesh> meshPtr, glm::vec3 pos = {0, 0, 0});

        std::shared_ptr<Mesh> getMesh() const { return mesh; }

        glm::mat4 getModelMatrix() const;
};
