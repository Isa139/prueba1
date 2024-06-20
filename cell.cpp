#include "Cell.h"

Cell::Cell() : x(0), y(0), visited(false), treasure(false), power(None), portalDestination(nullptr), portalUsed(false) {
    walls[0] = walls[1] = walls[2] = walls[3] = true;
    up = down = left = right = nullptr;
}

Cell::Cell(int x, int y) : x(x), y(y), visited(false), treasure(false), power(None), portalDestination(nullptr), portalUsed(false) {
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

void Cell::placePower(Power power) {
    this->power = power;
}

Cell::Power Cell::getPower() const {
    return power;
}

// Métodos de acceso para los vecinos
Cell* Cell::getUp() const {
    return up;
}

Cell* Cell::getDown() const {
    return down;
}

Cell* Cell::getLeft() const {
    return left;
}

Cell* Cell::getRight() const {
    return right;
}

// Métodos para establecer vecinos
void Cell::setUp(Cell* up) {
    this->up = up;
}

void Cell::setDown(Cell* down) {
    this->down = down;
}

void Cell::setLeft(Cell* left) {
    this->left = left;
}

void Cell::setRight(Cell* right) {
    this->right = right;
}

// Métodos para portales
void Cell::placePortal(Cell* destination) {
    portalDestination = destination;
    portalUsed = false;
}

Cell* Cell::getPortalDestination() const {
    return portalDestination;
}

bool Cell::hasPortal() const {
    return portalDestination != nullptr && !portalUsed;
}

void Cell::usePortal() {
    portalUsed = true;
}

bool Cell::isPortalUsed() const {
    return portalUsed;
}
