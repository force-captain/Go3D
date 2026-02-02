#pragma once

#include "types.hpp"
#include "glm/glm.hpp"
#include <unordered_set>

class Group;
class Board;

class Tile {
    private:
        int x;
        int y;
        int board_size;
        Colour colour;
        Group* group;
        std::unordered_set<Tile*> neighbours;

    public:
        Tile(int x, int y) : x(x), y(y), colour(Colour::EMPTY), group(nullptr) {}
        ~Tile() = default;

        bool isEmpty() const { return colour == Colour::EMPTY; }

        void setNeighbours(Board* b);

        void setGroup(Group& g);
        void clearGroup();
        std::unordered_set<Tile*> getNeighbours() const { return neighbours; }

        Colour getColour() const { return colour; }
        Group* getGroup() const  { return group;  } 

        glm::vec3 getRenderPos() const;
};
