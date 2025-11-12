#pragma once
#include "../board.h"
#include <GLFW/glfw3.h>

class Renderer {
    private:
        unsigned int shaderPogram;
        unsigned int vao, vbo;

        int width, height;

        GLFWwindow* window;
        void initShaders();
        void initBuffers();
    public:
        Renderer(int width, int height);
        ~Renderer();

        void clear();
        void drawBoard(const Board& board);
        void drawPieces(const Board& board);
        void drawSelection(const Tile& t, Colour c);
        void swapBuffers();
        bool shouldClose() const;
        void cleanup();
};
