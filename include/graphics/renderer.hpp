#pragma once
#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <glm/gtc/matrix_transform.hpp>
#include <optional>
#include "graphics/input.hpp"
#include "graphics/logic_interface.hpp"

struct GLFWwindow;
class Shader;
class Board;
class Tile;
class Object3D;
class Object2D;
class Camera;
class Scene;

// Board Y top: 0.25

class Renderer {
    private:
        static std::shared_ptr<Shader> getDefaultShader();

        std::unique_ptr<Scene> scene;

        int width, height;
        const char* title;

        GLFWwindow* window;
        Input input;

        double lastFrameTime = 0.0;
        double deltaTime = 0.0;
        void updateDeltaTime();

        void set3Dmode(bool is3D);
        
        void initDefaultShader();

        static void onResize(GLFWwindow* win, int w, int h);

        void renderObject(Object3D& obj, const Camera& cam);
        void renderObject(Object2D& obj);
        void renderScene();

        // Ray-casts the cursor onto the board's top face. Returns the world-space
        // hit point, or nullopt when the cursor is not over the board. Converting
        // that point to a board coordinate is LogicInterface's job (worldToBoard).
        std::optional<glm::vec3> mouseToWorld(const BoardMetrics& metrics) const;

        // Per-frame board pointer handling: updates the hover cue, and on a
        // left-click routes the move into the game logic via LogicInterface.
        void handleBoardInteraction(LogicInterface& interface);

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

        GLFWwindow* getWindow() const { return window; }

        const Input& getInput() const { return input; }

        void mainLoop(std::unique_ptr<LogicInterface> interface);
        void render();
        std::unique_ptr<Scene> setScene(std::unique_ptr<Scene> newScene);

};
