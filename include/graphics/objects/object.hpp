#pragma once

#include <glm/fwd.hpp>

class Object {
    public:
        virtual ~Object() = default;

        virtual glm::mat4 getModelMatrix() const = 0;

        virtual void update(float deltaTime);
};
