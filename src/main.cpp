#include "graphics/objects/object2d.hpp"
#include "graphics/objects/object3d.hpp"
#include "graphics/camera.hpp"
#include "graphics/renderer.hpp"
#include "graphics/scene.hpp"
#include <GLFW/glfw3.h>
#include <memory>

int main() {
    Renderer renderer(800, 600, "Go3D");
    
    if (!renderer.init()) return -1;

    bool showMenu = true;

    auto scene = std::make_unique<Scene>(true);

    std::unique_ptr<Object> rect = std::make_unique<Object2D>(100.0f, 100.0f, 200.0f, 400.0f);
    
    Material mat{glm::vec3(1.0f, 0.0f, 1.0f), 0, nullptr};
    rect->setMaterial(mat);

    Shader cubeshad;
    cubeshad.load("assets/shaders/cube.vert", "assets/shaders/cube.frag");
    Material cubemat{glm::vec3(1.0f, 0.0f, 0.0f), 0, std::make_shared<Shader>(cubeshad)};

    std::unique_ptr<Object> cube = std::make_unique<Object3D>(Mesh::getCube());

    scene->addObject(std::move(rect));

    std::unique_ptr<Camera> cam = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 0.0f), 10.0f, 0.0f, 0.0f, 75.0f, 1.0f);
    
    scene->setCamera(std::move(cam));

    renderer.setScene(std::move(scene));


    while(!renderer.shouldClose()) {
        renderer.clear();
        renderer.renderScene();

        renderer.update();
        glfwPollEvents();
    }

    renderer.cleanup();
    return 0;
}
