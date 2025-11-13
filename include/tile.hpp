#pragma once

#include "types.hpp"
#include <unordered_set>

class Group;
class Board;

class Tile {
    private:
        int x;
        int y;
        Colour colour;
        Group* group;
        std::unordered_set<Tile*> neighbours;

    public:
        Tile(Board* b, int x, int y);
        ~Tile() = default;

        bool isEmpty() const { return colour == Colour::EMPTY; }

        void setGroup(Group& g);
        void clearGroup();
        std::unordered_set<Tile*> getNeighbours() const { return neighbours; }

        Colour getColour() const { return colour; }
        Group* getGroup() const  { return group;  } 
};
