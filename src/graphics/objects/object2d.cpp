#include "graphics/objects/object2d.hpp"
#include "graphics/objects/mesh.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>


Object2D::Object2D(float x, float y, float width, float height) : position(x, y), rotation(0.0f), scale(width, height) {
    mesh = Mesh::getQuad();
    initBounds();
}

glm::mat4 Object2D::getModelMatrix() const {
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::rotate(model, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(scale, 1.0f));
    return model;
}

void Object2D::update(float deltaTime){
    onUpdate(deltaTime);
}

bool Object2D::contains(double mx, double my) const {
    glm::vec4 worldPoint(mx, my, 0.0f, 1.0f);
    glm::mat4 invModel = glm::inverse(getModelMatrix());
    glm::vec4 local = invModel * worldPoint;

    return (local.x >= bounds.min.x && local.x <= bounds.max.x &&
            local.y >= bounds.min.y && local.y <= bounds.max.y);
}
