#include <cstdint>
#include <glm/fwd.hpp>
#include <unordered_map>
#include <vector>
#include <memory>

class Object2D;
class Object3D;
class Camera;
class Light;

class Scene {
    private:
        std::unordered_map<uint64_t, std::unique_ptr<Object2D>> objects2D;
        std::unordered_map<uint64_t, std::unique_ptr<Object3D>> objects3D;
        std::unique_ptr<Camera> camera;
        std::vector<std::unique_ptr<Light>> lights;
    public:
        Object2D& addObject(std::unique_ptr<Object2D> o2d);
        Object3D& addObject(std::unique_ptr<Object3D> o3d);

        void removeObject(Object2D& o2d);
        void removeObject(Object3D& o3d);

        void setCamera(Camera& cam);
        void translateCamera(glm::vec3 pos);
        void pointCamera(glm::vec3 target);
};
