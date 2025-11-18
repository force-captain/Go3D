#pragma once

#include <glm/fwd.hpp>
#include "graphics/objects/shader.hpp"

class Object {
    private:
        Material mat;
    public:
        virtual ~Object() = default;

        virtual glm::mat4 getModelMatrix() const = 0;

        virtual void update(float deltaTime) = 0;
        
        void setMaterial(const Material& newMat) {mat = newMat; }
        const Material& getMaterial() const { return mat; }
};
