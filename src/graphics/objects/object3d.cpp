#include "graphics/objects/object3d.hpp"
#include "graphics/objects/mesh.hpp"
#include "graphics/ray.hpp"
#include <glm/detail/qualifier.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

Object3D::Object3D(std::shared_ptr<Mesh> meshPtr, glm::vec3 pos) {
    mesh = std::move(meshPtr);
    position = pos;
    initBounds();
}

glm::mat4 Object3D::getModelMatrix() const {
    glm::mat4 model = glm::mat4(1.0f);

    // Translate
    model = glm::translate(model, position);

    // Rotate
    glm::quat q = glm::quat(rotation);
    model *= glm::mat4_cast(q);

    // Scale
    model = glm::scale(model, scale);

    return model;
}

bool Object3D::contains(Ray& ray) const {
    glm::mat4 invMod = glm::inverse(getModelMatrix());

    Ray localRay = ray.transform(invMod);

    float tMin, tMax;
}

void Object3D::update(float deltaTime) {
    onUpdate(deltaTime);
}
