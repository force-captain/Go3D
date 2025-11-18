#pragma once

#include "graphics/objects/object.hpp"
#include <glm/glm.hpp>
#include <memory>

class Mesh;

class Object2D : public Object {
    private:
        static std::shared_ptr<Mesh> createQuadMesh();
        std::shared_ptr<Mesh> mesh;
        glm::vec2 position;
        glm::vec2 scale;
        float rotation;

    public:
        Object2D(float x, float y, float width, float height);
        glm::mat4 getModelMatrix() const override;
        void update(float deltaTime) override;
        std::shared_ptr<Mesh> getMesh() { return mesh;}
};
