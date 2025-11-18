#pragma once
#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <glm/gtc/matrix_transform.hpp>

struct GLFWwindow;
class Shader;
class Board;
class Tile;
class Object3D;
class Object2D;
class Camera;
class Scene;

class Renderer {
    private:
        static std::shared_ptr<Shader> getDefaultShader();

        int width, height;
        const char* title;

        GLFWwindow* window;

        double lastFrameTime = 0.0;
        double deltaTime = 0.0;
        void updateDeltaTime();
        
        void initDefaultShader();

        static void onResize(GLFWwindow* win, int w, int h);

        void renderObject(const Object3D& obj, const Camera& cam);
        void renderObject(const Object2D& obj);

    public:
        Renderer(int width, int height, const char* title)
            : width(width), height(height), title(title), window(nullptr) {}
        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;
        ~Renderer() { cleanup(); }

        void update();
        bool init();
        void clear(const glm::vec4& colour = glm::vec4(0,0,0,1));
        bool shouldClose() const;
        void cleanup();

        GLFWwindow* getWindow() const;

        void renderScene(const Scene& scene);
};
