#pragma once
#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <utility>
#include "types.h"

struct GLFWwindow;
class Board;
class Tile;
class Object3D;

class Renderer {
    private:
        int width, height;
        const char* title;

        GLFWwindow* window;

        glm::mat4 projection;
        glm::mat4 view;
        glm::mat4 model;

        Vector3 camEye, camCenter, camUp;

        GLuint shaderProgram;
        GLuint boardVAO, boardVBO;
        GLuint stoneVAO, stoneVBO;

        void setupLighting();
        static void onResize(GLFWwindow* win, int w, int h);

        bool loadShaders(const char* vertPath, const char* fragPath);
        void setMVP(const glm::mat4& model);
    public:
        Renderer(int width, int height, const char* title);

        bool init();

        void clear();
        void swapBuffers();
        bool shouldClose() const;
        void cleanup();

        GLFWwindow* getWindow() const;

        void setCamera(Vector3 eye, Vector3 center, Vector3 up);

        // Drawing
        /*
        void drawBoard(const Board& board);
        void drawPieces(const Board& board);
        void drawSelection(const Tile& t, Colour c);
        */
        void drawObject(const Object3D& obj);

        std::pair<int, int> screenToBoard(double x, double y);

        void updateProjection(float fov, float aspect, float nearPlane, float farPlane);
};
