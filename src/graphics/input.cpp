#include "graphics/input.hpp"
#include <GLFW/glfw3.h>

void Input::update(GLFWwindow* window) {
    // Keyboard
    for (int i = 0; i < MAX_KEYS; i++) {
        keys[i] = glfwGetKey(window, i) == GLFW_PRESS;
    }

    // Mouse 
    for (int i = 0; i < MAX_BUTTONS; i++) {
        bool pressed = glfwGetMouseButton(window, i) == GLFW_PRESS;
        buttons[i] = pressed;

        dragDX[i] = pressed ? mouseX - lastMouseX : 0;
        dragDY[i] = pressed ? mouseY - lastMouseY : 0;
    }

    lastMouseX = mouseX;
    lastMouseY = mouseY;

    glfwGetCursorPos(window, &mouseX, &mouseY);
}

double Input::getDragX(int button) const {
    return (button < 0 || button >= MAX_BUTTONS) ? 0
        : dragDX[button];
}

double Input::getDragY(int button) const {
    return (button < 0 || button >= MAX_BUTTONS) ? 0
        : dragDY[button];
}

bool Input::isKeyPressed(int key) const {
    return (key < 0 || key >= MAX_KEYS) ? false
        : keys[key];
}

bool Input::isButtonPressed(int button) const {
    return (button < 0 || button >= MAX_BUTTONS) ? false
        : buttons[button];
}
