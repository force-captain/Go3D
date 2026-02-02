#pragma once
#include "graphics/objects/object3d.hpp"
#include "types.hpp"
#include <glm/fwd.hpp>
#include <memory>
#include <vector>
#include <stdint.h>

struct BoardMetrics {
    float square_size;
    float stone_size;
    float init_height;
};

class Scene;
class Board;
class Shader;

class LogicInterface {
    public:
        static constexpr uint64_t EMPTY = 0;

        LogicInterface(Scene& scene, Board& board, size_t board_size, std::shared_ptr<Shader> shader);

        void update();

    private:
        uint64_t last_state = 0;
        std::shared_ptr<Mesh> stoneMesh;
        std::shared_ptr<Material> stoneMat_W;
        std::shared_ptr<Material> stoneMat_B;

        glm::vec3 getPosition(int x, int y);

        Scene& scene;
        Board& board;
        size_t board_size;
        BoardMetrics metrics;
        std::vector<std::vector<uint64_t>> stones;
        std::unique_ptr<Object3D> createStone(Colour clr, int x, int y);

};
