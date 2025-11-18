#include "graphics/objects/object.hpp"

BoundingBox Object::getWorldBoundingBox() const {
    glm::mat4 model = getModelMatrix();

    glm::vec3 corners[8] = {
        {bounds.min.x, bounds.min.y, bounds.min.z},
        {bounds.min.x, bounds.min.y, bounds.max.z},
        {bounds.min.x, bounds.max.y, bounds.min.z},
        {bounds.min.x, bounds.max.y, bounds.max.z},
        {bounds.max.x, bounds.min.y, bounds.min.z},
        {bounds.max.x, bounds.min.y, bounds.max.z},
        {bounds.max.x, bounds.max.y, bounds.min.z},
        {bounds.max.x, bounds.max.y, bounds.max.z}
    };

    BoundingBox result;
    result.min = glm::vec3(FLT_MAX);
    result.max = glm::vec3(-FLT_MAX);

    for (const auto& c : corners) {
        glm::vec3 wc = glm::vec3(model * glm::vec4(c, 1.0f));
        result.min = glm::min(result.min, wc);
        result.max = glm::max(result.max, wc);
    }
    return result;
}

void Object::initBounds() {
    bounds = mesh->getBoundingBox();
}
