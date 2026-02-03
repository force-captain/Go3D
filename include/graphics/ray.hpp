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

        const glm::vec3& getOrigin() const { return origin; }
        const glm::vec3& getDirection() const { return direction; }

        glm::vec3 at(float t) const { return origin + direction * t; }
        
        bool intersects(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, float t);
        bool intersectsAABB(const glm::vec3& min, const glm::vec3& max, float* tNear = nullptr, float* tFar = nullptr);
};
