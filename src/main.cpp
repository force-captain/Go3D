#include "graphics/light.hpp"
#include "graphics/objects/object2d.hpp"
#include "graphics/objects/object3d.hpp"
#include "graphics/camera.hpp"
#include "graphics/renderer.hpp"
#include "graphics/scene.hpp"
#include <GLFW/glfw3.h>
#include <glm/trigonometric.hpp>
#include <memory>

int main() {
    Renderer renderer(800, 600, "Go3D");
    
    if (!renderer.init()) return -1;

    auto scene = std::make_unique<Scene>();

    auto cubeshad = std::make_shared<Shader>();
    cubeshad->load("assets/shaders/cube.vert", "assets/shaders/cube.frag");
    Material cubemat{glm::vec3(1.0f, 0.0f, 0.0f), 0, cubeshad};

    std::unique_ptr<Object3D> cube = std::make_unique<Object3D>(Mesh::getCube());

    cube->setMaterial(cubemat);
    
    cube->setScale(glm::vec3(4.0f, 0.5f, 4.0f));

    scene->addObject(std::move(cube));

    auto light = std::make_unique<Light>(LightType::Point, 1.0f, glm::vec3(1.0f), glm::vec3(-1.0f));

    scene->addLight(std::move(light));

    std::unique_ptr<Camera> cam = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 0.0f), 10.0f, 0.0f, 0.0f, 75.0f, 800.0f / 600.0f);

    cam->rotate(glm::radians(20.0f), glm::radians(35.0f));

    Camera* rcam = cam.get();
    
    scene->setCamera(std::move(cam));

    renderer.setScene(std::move(scene));


    while(!renderer.shouldClose()) {
        renderer.clear();
        renderer.renderScene();
        renderer.update();
    }

    return 0;
}
