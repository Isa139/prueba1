#include "Cell.h"

Cell::Cell(int x, int y) : x(x), y(y), visited(false), treasure(false) {
    walls[0] = walls[1] = walls[2] = walls[3] = true;
    up = down = left = right = nullptr;
}

bool Cell::hasWall(int direction) const {
    return walls[direction];
}

void Cell::removeWall(int direction) {
    walls[direction] = false;
}

void Cell::visit() {
    visited = true;
}

bool Cell::isVisited() const {
    return visited;
}

void Cell::placeTreasure() {
    treasure = true;
}

bool Cell::hasTreasure() const {
    return treasure;
}

int Cell::getX() const {
    return x;
}

int Cell::getY() const {
    return y;
}
