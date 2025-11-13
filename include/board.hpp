#pragma once

#include <cstdint>
#include <memory>
#include <unordered_set>
#include <vector>
#include "types.hpp"

class Group;
class Tile;

class Board {
    private:
        int size;
        std::unordered_set<std::unique_ptr<Group>> groups;
        std::vector<Tile> tiles;
        std::unordered_set<uint64_t> previousStates;

        Group* initiateMove(Colour colour, Tile* t);
        void commitMove(Group* g, Tile* t);
        bool checkKo(Colour c);
    public:
        Board(int size);
        ~Board();

        Tile* operator()(int row, int col);
        int getSize() const { return size; }

        MoveResult attemptMove(Colour, int row, int col);

        Group* addGroup(std::unique_ptr<Group> g);
        void removeGroup(Group* g);
};
