#include "graphics/logic_interface.hpp"
#include "board.hpp"
#include "graphics/objects/object3d.hpp"
#include "graphics/scene.hpp"
#include "tile.hpp"
#include "types.hpp"
#include <cmath>
#include <memory>
#include <glm/glm.hpp>
#include <stdexcept>
#include "graphics/objects/shader.hpp"

// Texture layout: each board PNG is 240x560; the cube's top face is the top
// 240x240 region. The playing grid is inset by a uniform 12px buffer on every
// side (active grid area 216x216), so margin_pct = 12 / 240 for all sizes.

// Stone drop-in animation tuning.
static constexpr float FALL_DURATION = 0.15f;       // seconds, start to landing
static constexpr float FALL_HEIGHT_FACTOR = 2.0f;   // drop = stone_size * this


LogicInterface::LogicInterface(Scene& scene, Board& board, size_t board_size, std::shared_ptr<Shader> shader) : scene(scene), board_size(board_size), board(board), stones(board_size, std::vector<uint64_t>(board_size, EMPTY)) {
    float margin = 12.0f / 240.0f;
    switch(board_size) {
        case 9:
            metrics = {.stone_size = 0.25f, .init_height = 0.325f};
            break;
        case 13:
            metrics = {.stone_size = 0.175f, .init_height = 0.3125f};
            break;
        case 19:
            metrics = {.stone_size = 0.1125f, .init_height = 0.3f};
            break;
        default:
            throw std::invalid_argument("Incorrect board size");
    }
    metrics.board_top_y = 0.25f;
    metrics.board_size = board_size;
    metrics.margin_pct = margin;
    // Board cube is scaled to 6.0 wide in main.cpp, so its top face spans [-3, 3].
    metrics.board_half_world = 3.0f;

    metrics.square_size = (6.0f * (1 - 2 * margin)) / float(board_size - 1);
    fallHeight = metrics.stone_size * FALL_HEIGHT_FACTOR;

    stoneMesh = std::make_shared<Mesh>("assets/stone.obj");
    stoneMat_W = std::make_shared<Material>(glm::vec3(0.5f), 0, shader);
    stoneMat_B = std::make_shared<Material>(glm::vec3(0.1f), 0, shader);

    // Hover cue: a wireframe stone, hidden until the cursor is over an empty
    // intersection. The Scene owns it permanently; keep a non-owning pointer.
    std::unique_ptr<Object3D> cue = createStone(glm::ivec2(0, 0), Colour::BLACK);
    cue->setWireframe(true);
    cue->setVisible(false);
    hoverCue = cue.get();
    scene.addObject(std::move(cue));
}

// Maps a board coordinate to the stone's world-space position (inverse of
// worldToBoard). x -> world x, y -> world z, with the grid centred on the origin.
glm::vec3 LogicInterface::getPosition(glm::ivec2 pos) {
    int centre = board_size / 2;
    return glm::vec3(
            (pos.x - centre) * metrics.square_size,
            metrics.init_height,
            (pos.y - centre) * metrics.square_size);
}

// Builds a stone object of the given colour, scaled and positioned for the
// intersection at `pos`. Caller owns the result (e.g. hands it to the Scene).
std::unique_ptr<Object3D> LogicInterface::createStone(glm::ivec2 pos, Colour colour) {
    std::unique_ptr<Object3D> stone = std::make_unique<Object3D>(stoneMesh);
    stone->setMaterial(colour == Colour::WHITE ? stoneMat_W : stoneMat_B);
    stone->setScale(glm::vec3(metrics.stone_size));
    stone->translate(getPosition(pos));
    return stone;
}

bool LogicInterface::boardChanged() const {
    return last_state == 0 || board.getLastState() != last_state;
}

void LogicInterface::sync3DBoard() {
    last_state = board.getLastState();

    for(size_t i = 0; i < board_size; i++) {
        for(size_t j = 0; j < board_size; j++) {
            // Remove stone
            if (board(i, j)->isEmpty() && stones[i][j] != EMPTY) {
                scene.removeObject(stones[i][j]);
                stones[i][j] = EMPTY;
            } else if (!board(i, j)->isEmpty() && stones[i][j] == EMPTY) {
                std::unique_ptr<Object3D> stone = createStone(glm::ivec2(int(i), int(j)), board(i, j)->getColour());
                // Start the stone fallHeight above its resting spot; the drop-in
                // animation (advanceFallingStones) lowers it into place.
                float targetY = stone->getPosition().y;
                stone->translate(glm::vec3(0.0f, fallHeight, 0.0f));
                uint64_t id = scene.addObject(std::move(stone));
                stones[i][j] = id;
                fallingStones.push_back({id, targetY, 0.0f});
            }
        }
    }
}

// Per-frame hook: reconciles the 3D scene with the logical board (for changes
// from a path other than place()) and advances the stone drop-in animation.
void LogicInterface::update(float deltaTime) {
    if (boardChanged()) {
        sync3DBoard();
    }
    advanceFallingStones(deltaTime);
}

// Advances the stone drop-in animation. Each falling stone eases from its start
// height down to its resting Y, accelerating (p*p) to mimic gravity.
void LogicInterface::advanceFallingStones(float deltaTime) {
    for (auto it = fallingStones.begin(); it != fallingStones.end(); ) {
        Object3D* stone = dynamic_cast<Object3D*>(scene.getObject(it->id));
        if (stone == nullptr) {
            // The stone was removed (e.g. captured) before it finished falling.
            it = fallingStones.erase(it);
            continue;
        }

        it->elapsed += deltaTime;
        float p = it->elapsed / FALL_DURATION;
        glm::vec3 pos = stone->getPosition();

        if (p >= 1.0f) {
            pos.y = it->targetY;
            stone->setPosition(pos);
            it = fallingStones.erase(it);
        } else {
            pos.y = it->targetY + fallHeight * (1.0f - p * p);
            stone->setPosition(pos);
            ++it;
        }
    }
}

// Primary entry point from the graphical layer into the game logic. Verifies it
// is `colour`'s turn and that the move is legal (delegating to Board), applies
// it, and on success syncs the 3D scene to the new board state.
MoveResult LogicInterface::place(glm::ivec2 pos, Colour colour) {
    if (colour != board.getCurrentTurn()) {
        return MoveResult::INVALID;
    }

    MoveResult result = board.attemptMove(pos.x, pos.y);
    if (result == MoveResult::OK) {
        sync3DBoard();
    }
    return result;
}

Colour LogicInterface::getCurrentTurn() const {
    return board.getCurrentTurn();
}

void LogicInterface::showHover(glm::vec3 world) {
    auto [col, row] = worldToBoard(world);

    // Only cue empty intersections — a stone can't be played on an occupied one.
    if (!board(col, row)->isEmpty()) {
        hideHover();
        return;
    }

    hoverCue->setPosition(getPosition(glm::ivec2(col, row)));
    hoverCue->setMaterial(board.getCurrentTurn() == Colour::WHITE ? stoneMat_W
                                                                  : stoneMat_B);
    hoverCue->setVisible(true);
}

void LogicInterface::hideHover() {
    hoverCue->setVisible(false);
}

// Inverse of getPosition(): maps a world-space point on the board plane back to
// the nearest board coordinate, clamped to the grid. This is the single place
// the renderer routes cursor picks through (see Renderer::mouseToWorld).
std::pair<int, int> LogicInterface::worldToBoard(glm::vec3 world) {
    int centre = board_size / 2;
    int bx = int(std::lround(world.x / metrics.square_size)) + centre;
    int by = int(std::lround(world.z / metrics.square_size)) + centre;

    return std::pair(
            glm::clamp(bx, 0, (int)board_size - 1),
            glm::clamp(by, 0, (int)board_size - 1));
}
