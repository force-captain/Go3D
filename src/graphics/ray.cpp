#include "graphics/ray.hpp"
#include <cmath>


Ray Ray::transform(const glm::mat4& mat) const {
    glm::vec4 newO = mat * glm::vec4(origin, 1.0f);
    glm::vec4 newD = mat * glm::vec4(direction, 1.0f);
    return Ray(glm::vec3(newO), glm::vec3(newD));
}

bool Ray::intersects(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, float t) {
    const float EPSILON = 1e-8f;
    glm::vec3 e1 = v1 - v0;
    glm::vec3 e2 = v2 - v0;

    glm::vec3 h = glm::cross(direction, e2);

    // Parallel check
    float a = glm::dot(e1, h);
    if (std::fabs(a) < EPSILON) return false;

    float f = 1.0f / a;
    glm::vec3 s = origin - v0;

    float u = f * glm::dot(s, h);
    if (u < 0.0f || u > 1.0f) return false;

    glm::vec3 q = glm::cross(s, e1);
    float v = f * glm::dot(direction, q);
    if (v < 0.0f || u + v > 1.0f) return false;
    
    t = f * glm::dot(e2, q);
    return t > EPSILON;
}
