#include "graphics/renderer.hpp"
#include <GLFW/glfw3.h>

int main() {
    Renderer renderer(800, 600, "Go3D");
    
    if (!renderer.init()) return -1;

    bool showMenu = true;

    while(!renderer.shouldClose()) {
        renderer.clear();

        renderer.swapBuffers();
        glfwPollEvents();
    }

    renderer.cleanup();
    return 0;
}
