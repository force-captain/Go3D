#pragma once

#include "tile.h"
#include "types.h"
#include <unordered_set>

class Group {
    private:
        std::unordered_set<Tile*> points;
        std::unordered_set<Tile*> liberties;
        Colour colour;
        bool captured;

    public:
        // Lifecycle
        Group(Tile& init_point, Colour colour);
        ~Group() = default;
        
        // Getters
        int getLibertyCount() const { return liberties.size(); }
        int getSize() const { return points.size(); }

        Colour getColour() const { return colour; }

        const std::unordered_set<Tile*>& getPoints() const { return points; }

        // Modifiers
        void addLiberty(Tile* t);
        void removeLiberty(Tile* t);

        void addPoint(Tile* t);
        void removePoint(Tile* t);

        void markForCapture(bool isCaptured) { captured = isCaptured; }
        bool isCaptured() const { return captured; }

        void merge(Group& other);
};
