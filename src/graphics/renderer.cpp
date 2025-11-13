#include "graphics/renderer.h"
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_double4x4.hpp>
#include <iostream>
#include "types.h"

Renderer::Renderer(int w, int h, const char* t) : width(w), height(h), title(t), window(nullptr) {
    camEye = Vector3(w / 2.0f, (float)h, w / 2.0f);
    camCenter = Vector3(w / 2.0f, 0.0f, w / 2.0f);
    camUp = Vector3(0.0f, 1.0f, 0.0f );
}

void Renderer::onResize(GLFWwindow* win, int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, w / (float)h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

bool Renderer::init() {
    if (!glfwInit()) {
        std::cerr << "Failed to init GLFW\n";
        return false;
    }


    printf("Using GLFW backend: %d\n", glfwGetPlatform());
    // OpenGL 3.3 Core
    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to init GLEW\n";
        return false;
    }

    // Depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    setupLighting();

    glShadeModel(GL_SMOOTH);

    // init viewport
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    glViewport(0, 0, fbWidth, fbHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, fbWidth / (float)fbHeight, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camEye.x, camEye.y, camEye.z,
              camCenter.x, camCenter.y, camCenter.z,
              camUp.x, camUp.y, camUp.z);
    
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* win, int w, int h) {
            onResize(win, w, h);
    });

    return true;
}

void Renderer::setupLighting() {
    GLfloat lightPos[] = { 10.0f, 15.0f, 10.0f, 1.0f };
    GLfloat ambient [] = { 0.3f , 0.3f , 0.3f , 1.0f };
    GLfloat diffuse [] = { 0.8f , 0.8f , 0.8f , 1.0f };
    GLfloat specular[] = { 0.9f , 0.9f , 0.9f , 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
}

void Renderer::clear() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}


bool Renderer::shouldClose() const {
    return glfwWindowShouldClose(window);
}

void Renderer::swapBuffers() {
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Renderer::cleanup() {
    glfwDestroyWindow(window);
    glfwTerminate();
}
