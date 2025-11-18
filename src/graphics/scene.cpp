#include "graphics/scene.hpp"
#include "graphics/camera.hpp"
#include "graphics/light.hpp"
#include "graphics/objects/object.hpp"

Scene::Scene(bool is3D) : is3D(is3D) {}

Scene::~Scene() {

}

uint64_t Scene::addObject(std::unique_ptr<Object> obj) {
    uint64_t id = nextID++;
    objects[id] = std::move(obj);
    return id;
}

bool Scene::removeObject(uint64_t id) {
    return objects.erase(id) > 0;
}

void Scene::setCamera(std::unique_ptr<Camera> cam) {
    camera = std::move(cam);
}

void Scene::pointCamera(glm::vec3 target) {
    camera->lookAt(target);
}

void Scene::rotateCamera(float dYaw, float dPitch) {
    camera->rotate(dYaw, dPitch);
}

Object* Scene::getObject(uint64_t id) {
    auto it = objects.find(id);
    return it != objects.end() ? it->second.get() : nullptr;
}
