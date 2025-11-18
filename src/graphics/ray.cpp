#include "graphics/ray.hpp"


Ray Ray::transform(const glm::mat4& mat) const {
    glm::vec4 newO = mat * glm::vec4(origin, 1.0f);
    glm::vec4 newD = mat * glm::vec4(direction, 1.0f);
    return Ray(glm::vec3(newO), glm::vec3(newD));
}
