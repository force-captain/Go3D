#include "graphics/renderer.hpp"
#include "graphics/objects/object3d.hpp"
#include "graphics/objects/object2d.hpp"
#include "graphics/objects/mesh.hpp"
#include "graphics/camera.hpp"
#include "graphics/light.hpp"
#include "graphics/scene.hpp"
#include "graphics/objects/shader.hpp"
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_double4x4.hpp>
#include <glm/trigonometric.hpp>
#include <memory>
#include <iostream>

static const float CAMERA_SPEED = 20.0f;


void Renderer::onResize(GLFWwindow* win, int w, int h) {
    Renderer* ren = static_cast<Renderer*>(glfwGetWindowUserPointer(win));
    ren->width = w;
    ren->height = h;
    glViewport(0, 0, w, h);
}

bool Renderer::init() {
    if (!glfwInit()) {
        std::cerr << "Failed to init GLFW\n";
        return false;
    }

    // OpenGL 3.3 Core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return false;
    }

    glfwSetWindowUserPointer(window, this);

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, onResize);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to init GLEW\n";
        return false;
    }

    // Depth testing
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);

    glViewport(0, 0, width, height);

    return true;
}

void Renderer::clear(const glm::vec4& colour) {
    glClearColor(colour.r, colour.g, colour.b, colour.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Renderer::shouldClose() const {
    return glfwWindowShouldClose(window);
}

void Renderer::updateDeltaTime() {
    double currentTime = glfwGetTime();
    deltaTime = currentTime - lastFrameTime;
    lastFrameTime = currentTime;
}

void Renderer::update() {
    updateDeltaTime();
    glfwPollEvents();
    input.update(window);
    glfwSwapBuffers(window);

    if (input.isKeyPressed(GLFW_KEY_RIGHT)) 
        scene->rotateCamera(glm::radians(deltaTime * CAMERA_SPEED), 0.0f);
    if (input.isKeyPressed(GLFW_KEY_LEFT)) 
        scene->rotateCamera(glm::radians(deltaTime * -CAMERA_SPEED), 0.0f);
    if (input.isKeyPressed(GLFW_KEY_UP)) 
        scene->rotateCamera(0.0f, glm::radians(deltaTime * CAMERA_SPEED));
    if (input.isKeyPressed(GLFW_KEY_DOWN))
        scene->rotateCamera(0.0f, glm::radians(deltaTime * -CAMERA_SPEED));

}

void Renderer::cleanup() {
    glfwDestroyWindow(window);
    glfwTerminate();
    window = nullptr;
}

// Drawing

void Renderer::renderObject(Object2D& obj) {
    glm::mat4 projection = glm::ortho(0.0f, float(width), 0.0f, float(height), -1.0f, 1.0f);
    
    const Material& mat = obj.getMaterial();
    auto shader = mat.shader;

    if (!shader) {
        shader = getDefaultShader();
    }

    shader->use();
    glm::mat4 mvp = projection * obj.getModelMatrix();
    shader->setMat4("u_MVP", mvp);
    shader->setVec3("u_Color", mat.colour);

    glBindVertexArray(obj.getMesh()->getVAO());
    glDrawElements(GL_TRIANGLES, obj.getMesh()->indexCount(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Renderer::renderObject(Object3D& obj, const Camera& cam) {
    glm::mat4 view = cam.getViewMatrix();
    glm::mat4 proj = cam.getProjectionMatrix();

    auto shader = obj.getMaterial().shader;
    if (!shader) return;

    shader->use();
    
    // Lighting
    const auto& lights = scene->getLights();
    shader->setInt("numLights", lights.size());

    for(size_t i = 0; i < lights.size(); i++) {
        const Light* light = lights[i].get();
        std::string base = "lights["+std::to_string(i)+"]";

        shader->setInt(base + ".type", light->getType());
        shader->setVec3(base + ".position", light->getPos());
        shader->setVec3(base + ".direction", light->getDir());
        shader->setVec3(base + ".color", light->getClr());

        shader->setFloat(base + ".intensity", light->getIntensity());
        shader->setFloat(base + ".constant", light->getConstant());
        shader->setFloat(base + ".linear", light->getLinear());
        shader->setFloat(base + ".quadratic", light->getQuadratic());
    }

    // Transformation
    shader->setMat4("model", obj.getModelMatrix());
    shader->setMat4("view", view);
    shader->setMat4("projection", proj);
    shader->setVec3("objectColour", obj.getMaterial().colour);

    // Texture
    if (obj.getMaterial().textureID != 0) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, obj.getMaterial().textureID);
        shader->setInt("diffuseTexture", 0);
    }

    // Drawing
    glBindVertexArray(obj.getMesh()->getVAO());
    if (obj.getMesh()->getEBO() != 0) 
        glDrawElements(GL_TRIANGLES, obj.getMesh()->indexCount(), GL_UNSIGNED_INT, 0);
    else 
        glDrawArrays(GL_TRIANGLES, 0, obj.getMesh()->vertexCount());
    glBindVertexArray(0);
}


void Renderer::renderScene() {
    for (const auto& [id, obj]: scene->getObjects()) {
        auto shader = obj->getMaterial().shader;

        if (Object3D* o3_ptr = dynamic_cast<Object3D*>(obj.get())) 
            renderObject(*o3_ptr, scene->getCamera());
        else {
            Object2D* o2_ptr = static_cast<Object2D*>(obj.get());
            renderObject(*o2_ptr);
        }
    }
}

std::shared_ptr<Shader> Renderer::getDefaultShader() {
    static std::shared_ptr<Shader> defaultShader = []() { 
        auto shader = std::make_shared<Shader>();
        shader->load("assets/shaders/default2D.vert", "assets/shaders/default2D.frag");
        return shader; }();

    return defaultShader;
}

std::unique_ptr<Scene> Renderer::setScene(std::unique_ptr<Scene> newScene) {
    std::unique_ptr<Scene> oldScene = std::move(scene);
    scene = std::move(newScene);
    return oldScene;
}
