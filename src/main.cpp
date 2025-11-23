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

    // Shaders
    auto shader3 = std::make_shared<Shader>();
    shader3->load("assets/shaders/default3D.vert", "assets/shaders/default3D.frag");

    auto shader2 = std::make_shared<Shader>();
    shader2->load("assets/shaders/default2D.vert", "assets/shaders/default2D.frag");

    // Board
    auto board_mat = std::make_shared<Material>(glm::vec3(1.0f, 1.0f, 1.0f), 0, shader3);
    std::unique_ptr<Object3D> boardObj = std::make_unique<Object3D>(Mesh::getCube());

    boardObj->setMaterial(board_mat);
    boardObj->getMesh()->loadTexture("assets/board13.png", *board_mat);
    boardObj->setScale(glm::vec3(6.0f, 0.5f, 6.0f));

    scene->addObject(std::move(boardObj));

    // Stone
    auto stonemesh = std::make_shared<Mesh>("assets/stone.obj");
    auto stonemat_w = std::make_shared<Material>(glm::vec3(0.5f), 0, shader3);
    std::unique_ptr<Object3D> stone_w = std::make_unique<Object3D>(stonemesh);

    stone_w->setMaterial(stonemat_w);
    stone_w->setScale(glm::vec3(0.5f));
    stone_w->translate(glm::vec3(0.0f, 1.0f, 0.0f));

    scene->addObject(std::move(stone_w));

    // Lights
    auto light = std::make_unique<Light>(LightType::Point, 1.0f, glm::vec3(0.0f, -2.0f, 0.0f), glm::vec3(-1.0f));
    auto light2 = std::make_unique<Light>(LightType::Point, 1.0f, glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(-1.0f));

    scene->addLight(std::move(light));
    scene->addLight(std::move(light2));


    // UI
    std::unique_ptr<Object2D> sizeButton = std::make_unique<Object2D>(0.0f, 0.0f, 300.0f, 100.0f);
    auto ui_mat = std::make_shared<Material>(glm::vec3(1.0f), 0, shader2);

    sizeButton->setMaterial(ui_mat);

    //scene->addObject(std::move(sizeButton));

    std::unique_ptr<Camera> cam = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 0.0f), 10.0f, 0.0f, 0.0f, 45.0f, 800.0f / 600.0f);

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
