#include "tile.hpp"
#include "group.hpp"
#include "board.hpp"
#include <unordered_set>

void Tile::setNeighbours(Board* b) {
    for(int i = -1; i <= 1; i += 2) {
          Tile* v = (*b)(x, y + i);
          Tile* h = (*b)(x + i, y);
          if (v != nullptr) neighbours.insert(v);
          if (h != nullptr) neighbours.insert(h);
    }
}

void Tile::setGroup(Group& g) {
    group = &g;
    colour = g.getColour();
}

void Tile::clearGroup() {
    group = nullptr;
    colour = Colour::EMPTY;
}

glm::vec3 Tile::getRenderPos() const {
    float square_size = board_size == 9 ? 0.6f :
        board_size == 13 ? 0.4f : 0.275f;
    glm::vec3 pos = glm::vec3(square_size * ((float)board_size / -2), 0.3f, square_size * ((float)board_size / -2));
    pos.x += x * square_size;
    pos.y += y * square_size;
    return pos;
}
