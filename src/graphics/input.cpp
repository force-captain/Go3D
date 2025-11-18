#include "graphics/input.hpp"
#include <GLFW/glfw3.h>

void Input::update(GLFWwindow* window) {
    // Keyboard
    for (int i = 0; i < MAX_KEYS; i++) {
        keys[i] = glfwGetKey(window, i) == GLFW_PRESS;
    }

    // Mouse 
    for (int i = 0; i < MAX_BUTTONS; i++) {
        buttons[i] = glfwGetMouseButton(window, i) == GLFW_PRESS;
    }
    glfwGetCursorPos(window, &mouseX, &mouseY);
}

bool Input::isKeyPressed(int key) const {
    return (key < 0 || key >= MAX_KEYS) ? false
        : keys[key];
}

bool Input::isButtonPressed(int button) const {
    return (button < 0 || button >= MAX_BUTTONS) ? false
        : buttons[button];
}
