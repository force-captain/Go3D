#pragma once

#include <glm/geometric.hpp>
#include <glm/glm.hpp>

class Ray {
    private:
        glm::vec3 origin;
        glm::vec3 direction;
    public:
        Ray() = default;
        Ray(const glm::vec3& o, const glm::vec3& dir) 
            : origin(o), direction(glm::normalize(dir)) {}

        Ray transform(const glm::mat4& mat) const;
        glm::vec3 at(float t) const { return origin + direction * t; }
};
