#include "board.h"
#include "types.h"
#include "group.h"
#include "tile.h"
#include "states.h"
#include <algorithm>

Board::Board(int size)
    : size(size) { 
    for (int r = 0; r < size; r++) {
        for (int c = 0; c < size; c++) {
            tiles.push_back(Tile(this, r, c));
        }
    }
}

Tile* Board::operator()(int row, int col) {
    if (row < 0 || row >= size) return nullptr;
    if (col < 0 || col >= size) return nullptr;
    return &tiles[row * size + col];
}

Group* Board::addGroup(std::unique_ptr<Group> g) {
    Group* gptr = g.get();
    groups.insert(std::move(g));
    return gptr;
}

void Board::removeGroup(Group* g) {
    auto it = std::find_if(
            groups.begin(), groups.end(),
            [&](const std::unique_ptr<Group>& ptr){ return ptr.get() == g; });


    if (it != groups.end())
        groups.erase(it);
}

Group* Board::initiateMove(Colour colour, Tile* t) {
    if (!t->isEmpty()) return nullptr;

    std::unique_ptr<Group> g = std::unique_ptr<Group>(new Group(*t, colour));
    return addGroup(std::move(g));
}

bool Board::checkKo(Colour c) {
    uint64_t state = getHash(this, c);
    if (previousStates.count(state) > 0) return true;
    previousStates.insert(state);
    return false;
}

void Board::commitMove(Group* newGroup, Tile* t) {
    // Clear captured groups
    for(const auto& g : groups) {
        if (g->isCaptured()) {
            for (Tile* t : g->getPoints()) {
                t->clearGroup();
            }
            removeGroup(g.get());
        }
    }

    // Merge neighbours
    std::unordered_set<Tile*> newLiberties;

    for (Tile* n : t->getNeighbours()) {
        Group* g = n->getGroup();
        if (g == nullptr) {
            newLiberties.insert(n);
            continue;
        }
        if (g == newGroup) continue;
        if (g->getColour() != newGroup->getColour()) continue;

        newGroup->merge(*g);
        removeGroup(g);
    }

    for (Tile* t : newLiberties) {
        newGroup->addLiberty(t);
    }
}


MoveResult Board::attemptMove(Colour colour, int row, int col) {
    Tile* t = (*this)(row, col);
    
    // Get new group
    Group* newGroup = initiateMove(colour, t);

    if (newGroup == nullptr) return MoveResult::INVALID;

    // Check captures
    std::unordered_set<Group*> neighbourOpponents;
    std::unordered_set<Group*> neighbourFriendlies;

    int current_liberties = 0;
    
    for (Tile* n : t->getNeighbours()) {
        Group* ng = n->getGroup();

        if (ng == nullptr || ng->getColour() == Colour::EMPTY) {
            current_liberties++;
            continue;
        }
        
        ng->removeLiberty(t);
            
        if (ng->getColour() != colour) {
            neighbourOpponents.insert(ng);
            if (ng->getLibertyCount() == 0) {
                ng->markForCapture(true);
                current_liberties++;
            }
        } else {
            neighbourFriendlies.insert(ng);
            current_liberties += ng->getLibertyCount();
        }
    }

    if (current_liberties == 0 || checkKo(colour)) {
        for(Group* ng : neighbourOpponents) {
            ng->markForCapture(false);
            ng->addLiberty(t);
        }

        for (Group* ng : neighbourFriendlies) {
            ng->addLiberty(t);
        }

        t->clearGroup();
        removeGroup(newGroup);

        if (current_liberties == 0) return MoveResult::SUICIDE;
        else return MoveResult::KO;
    }

    commitMove(newGroup, t);

    return MoveResult::OK;
}
