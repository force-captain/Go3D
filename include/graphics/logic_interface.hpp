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
    float board_top_y;
    float board_half_world;
    float margin_pct;
    int board_size;
};

class Scene;
class Board;
class Shader;

class LogicInterface {
    public:
        static constexpr uint64_t EMPTY = 0;

        LogicInterface(Scene& scene, Board& board, size_t board_size, std::shared_ptr<Shader> shader);

        void update(float deltaTime);

        // Primary entry point from the graphical layer into the game logic:
        // validates the move, applies it to the logical board, and syncs the
        // 3D scene to the result. Returns the rules verdict.
        MoveResult place(glm::ivec2 pos, Colour colour);

        std::pair<int, int> worldToBoard(glm::vec3 world);

        // Whose turn it is, per the logical board (the single source of truth).
        Colour getCurrentTurn() const;

        // Moves/recolours the hover cue to the intersection under `world`, or
        // hides it (cursor off-board, or the intersection is already occupied).
        void showHover(glm::vec3 world);
        void hideHover();

        const BoardMetrics& getMetrics() const { return metrics; }

    private:
        uint64_t last_state = 0;
        std::shared_ptr<Mesh> stoneMesh;
        std::shared_ptr<Material> stoneMat_W;
        std::shared_ptr<Material> stoneMat_B;

        // Wireframe ghost-stone shown under the cursor. Owned by the Scene; this
        // is a non-owning pointer (the cue is never removed).
        Object3D* hoverCue = nullptr;

        // A stone playing the drop-in animation: it falls from fallHeight above
        // its resting Y down to targetY over FALL_DURATION seconds.
        struct FallingStone {
            uint64_t id;     // Scene object id (looked up each frame, nullptr-safe)
            float targetY;   // resting world Y
            float elapsed;   // seconds since the stone was placed
        };
        std::vector<FallingStone> fallingStones;
        float fallHeight = 0.0f;

        void sync3DBoard();
        void advanceFallingStones(float deltaTime);
        bool boardChanged() const;

        glm::vec3 getPosition(glm::ivec2 pos);

        Scene& scene;
        Board& board;
        size_t board_size;
        BoardMetrics metrics;
        std::vector<std::vector<uint64_t>> stones;
        std::unique_ptr<Object3D> createStone(glm::ivec2 pos, Colour colour);

};
