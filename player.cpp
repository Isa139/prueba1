#include "Player.h"

Player::Player(Cell* startCell, int id) : currentCell(startCell), id(id) {}

void Player::moveUp() {
    if (currentCell->up && !currentCell->hasWall(0)) {
        currentCell = currentCell->up;
    }
}

void Player::moveDown() {
    if (currentCell->down && !currentCell->hasWall(2)) {
        currentCell = currentCell->down;
    }
}

void Player::moveLeft() {
    if (currentCell->left && !currentCell->hasWall(3)) {
        currentCell = currentCell->left;
    }
}

void Player::moveRight() {
    if (currentCell->right && !currentCell->hasWall(1)) {
        currentCell = currentCell->right;
    }
}

Cell* Player::getCurrentCell() const {
    return currentCell;
}

int Player::getId() const {
    return id;
}
