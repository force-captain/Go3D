#include "utils/renderer.h"
#include "board.h"
#include "glad/glad.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <algorithm>

Renderer::Renderer(int w, int h) : width(w), height(h) {
    if (!glfwInit()) {
        std::cerr << "Failed to init GLFW\n";
        std::exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "Go", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent((GLFWwindow*)window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to init GLAD\n";
        std::exit(EXIT_FAILURE);
    }

    glViewport(0, 0, w, h);

    initShaders();
    initBuffers();
}

void Renderer::initShaders() {

}

void Renderer::initBuffers() {

}

void Renderer::clear() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::drawBoard(const Board& board) {
    int fbWidth, fbHeight;

    glfwGetFramebufferSize((GLFWwindow*)window, &fbWidth, &fbHeight);

    int boardPixels = std::min(fbWidth, fbHeight);
    glViewport(0, 0, boardPixels, boardPixels);

}

bool Renderer::shouldClose() const {
    return glfwWindowShouldClose(window);
}

void Renderer::swapBuffers() {
    glfwSwapBuffers(window);
    glfwPollEvents();
}

