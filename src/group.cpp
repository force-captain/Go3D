#include "group.hpp"

Group::Group(Tile& init, Colour colour) 
    : colour(colour), captured(false) {
        points.insert(&init);
}


void Group::addLiberty(Tile* t) {
    if (t) 
        liberties.insert(t);
}

void Group::removeLiberty(Tile* t) {
    liberties.erase(t);
}

void Group::addPoint(Tile* t) {
    if (t) 
        points.insert(t);
}

void Group::removePoint(Tile* t) {
    points.erase(t);
}

void Group::merge(Group& other) {
    if (this == &other) return;

    // Merge lists
    liberties.insert(other.liberties.begin(), other.liberties.end());
    points.insert(other.points.begin(), other.points.end());

    // Update tiles
    for (Tile* t : points) {
        liberties.erase(t);
        t->setGroup(*this);
    }
}
