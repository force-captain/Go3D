#pragma once

#include <cstdint>
#include <glm/fwd.hpp>
#include <unordered_map>
#include <vector>
#include <memory>

class Object;
class Camera;
class Light;
class Board;

class Scene {
    private:
        std::unordered_map<uint64_t, std::unique_ptr<Object>> objects;
        std::unique_ptr<Camera> camera;
        std::vector<std::unique_ptr<Light>> lights;
        uint64_t nextID = 1;

    public:
        Scene() = default;
        ~Scene() = default;

        uint64_t addObject(std::unique_ptr<Object> obj);

        bool removeObject(uint64_t id);

        void addLight(std::unique_ptr<Light> light);

        void setCamera(std::unique_ptr<Camera>);
        void pointCamera(glm::vec3 target);
        void rotateCamera(float dYaw, float dPitch);

        const Camera& getCamera() const { return *camera; }

        Object* getObject(uint64_t id);

        const std::unordered_map<uint64_t, std::unique_ptr<Object>>& getObjects() const { return objects; }
        const std::vector<std::unique_ptr<Light>>& getLights() const { return lights; }
};
