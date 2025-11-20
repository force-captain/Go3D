#pragma once

class GLFWwindow;

class Input {
    private:
        static const int MAX_KEYS = 1024;
        bool keys[MAX_KEYS] = {false};

        static const int MAX_BUTTONS = 8;
        bool buttons[MAX_BUTTONS] = {false};

        double dragDX[MAX_BUTTONS] = {0};
        double dragDY[MAX_BUTTONS] = {0};

        double mouseX = 0.0;
        double mouseY = 0.0;

        double lastMouseX = 0.0;
        double lastMouseY = 0.0;
    public:
        void update(GLFWwindow* window);
        bool isKeyPressed(int key) const;

        bool isButtonPressed(int button) const;
        double getMouseX() const { return mouseX; }
        double getMouseY() const { return mouseY; }

        double getDragX(int button) const;
        double getDragY(int button) const;
};
