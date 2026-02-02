#include "graphics/logic_interface.hpp"
#include "board.hpp"
#include "graphics/objects/object3d.hpp"
#include "graphics/scene.hpp"
#include "tile.hpp"
#include "types.hpp"
#include <memory>
#include <glm/glm.hpp>
#include <stdexcept>
#include "graphics/objects/shader.hpp"

LogicInterface::LogicInterface(Scene& scene, Board& board, size_t board_size, std::shared_ptr<Shader> shader) : scene(scene), board_size(board_size), board(board), stones(board_size, std::vector<uint64_t>(board_size, EMPTY)) {
    switch(board_size) {
        case 9:
            metrics = {.square_size = 0.6f, .stone_size = 0.25f, .init_height = 0.325f};
            break;
        case 13:
            metrics = {.square_size = 0.4f, .stone_size = 0.175f, .init_height = 0.3125f};
            break;
        case 19:
            metrics = {.square_size = 0.275f, .stone_size = 0.1125f, .init_height = 0.3f};
            break;
        default:
            throw std::invalid_argument("Incorrect board size");
    }
    stoneMesh = std::make_shared<Mesh>("assets/stone.obj");
    stoneMat_W = std::make_shared<Material>(glm::vec3(0.5f), 0, shader);
    stoneMat_B = std::make_shared<Material>(glm::vec3(0.1f), 0, shader);
}

glm::vec3 LogicInterface::getPosition(int x, int y) {
    int centre = board_size / 2;
    return glm::vec3(
            (x - centre) * metrics.square_size,
            metrics.init_height,
            (y - centre) * metrics.square_size);
}

std::unique_ptr<Object3D> LogicInterface::createStone(Colour clr, int x, int y) {
    std::unique_ptr<Object3D> stone = std::make_unique<Object3D>(stoneMesh);
    stone->setMaterial(clr == Colour::WHITE ? stoneMat_W : stoneMat_B);
    stone->setScale(glm::vec3(metrics.stone_size));
    stone->translate(getPosition(x, y));
    return stone;
}


void LogicInterface::update() { 
    if (last_state != 0 && board.getLastState() == last_state) return;

    last_state = board.getLastState();

    for(size_t i = 0; i < board_size; i++) {
        for(size_t j = 0; j < board_size; j++) {
            // Remove stone
            if (board(i, j)->isEmpty() && stones[i][j] != EMPTY) {
                scene.removeObject(stones[i][j]);
                stones[i][j] = EMPTY;
            } else if (!board(i, j)->isEmpty() && stones[i][j] == EMPTY) {
                std::unique_ptr<Object3D> stone = createStone(board(i, j)->getColour(), i, j);
                stones[i][j] = scene.addObject(std::move(stone));
            }
        }
    }
}
