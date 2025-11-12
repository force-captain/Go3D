#include "tile.h"
#include "group.h"
#include "board.h"
#include <unordered_set>

Tile::Tile(Board* b, int x, int y) : x(x), y(y) {
    for(int r = -1; r <= 1; r += 2) {
        for(int c = -1; c <= 1; c += 2) {
            Tile* n = (*b)(x + r, x + c);
            if (n != nullptr) neighbours.insert(n);
        }
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
