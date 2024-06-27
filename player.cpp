#include "Player.h"

Player::Player(Cell* startCell, int id)
    : currentCell(startCell), id(id), jumpWalls(false), jumpWallsActive(false), controlOtherPlayer(false), doubleTurn(false) {}

void Player::moveUp() {
    if (currentCell->getUp() && (!currentCell->hasWall(0) || jumpWallsActive)) {
        currentCell = currentCell->getUp();
        if (jumpWallsActive && currentCell->hasWall(0)) consumeJumpWalls(); // Desactiva el poder después de usarlo
    }
}

void Player::moveDown() {
    if (currentCell->getDown() && (!currentCell->hasWall(2) || jumpWallsActive)) {
        currentCell = currentCell->getDown();
        if (jumpWallsActive && currentCell->hasWall(2)) consumeJumpWalls(); // Desactiva el poder después de usarlo
    }
}

void Player::moveLeft() {
    if (currentCell->getLeft() && (!currentCell->hasWall(3) || jumpWallsActive)) {
        currentCell = currentCell->getLeft();
        if (jumpWallsActive && currentCell->hasWall(3)) consumeJumpWalls(); // Desactiva el poder después de usarlo
    }
}

void Player::moveRight() {
    if (currentCell->getRight() && (!currentCell->hasWall(1) || jumpWallsActive)) {
        currentCell = currentCell->getRight();
        if (jumpWallsActive && currentCell->hasWall(1)) consumeJumpWalls(); // Desactiva el poder después de usarlo
    }
}

Cell* Player::getCurrentCell() const {
    return currentCell;
}

void Player::setCurrentCell(Cell* cell) {
    currentCell = cell;
}

int Player::getId() const {
    return id;
}

int Player::getX() const {
    return currentCell ? currentCell->getX() : -1;
}

int Player::getY() const {
    return currentCell ? currentCell->getY() : -1;
}

bool Player::canJumpWalls() const {
    return jumpWalls;
}

void Player::setCanJumpWalls(bool value) {
    jumpWalls = value;
}

void Player::activateJumpWalls() {
    if (jumpWalls) {
        jumpWallsActive = true;
    }
}

void Player::consumeJumpWalls() {
    jumpWallsActive = false;
    jumpWalls = false;
}

bool Player::canControlOtherPlayer() const {
    return controlOtherPlayer;
}

void Player::setControlOtherPlayer(bool value) {
    controlOtherPlayer = value;
}

void Player::useControlOtherPlayer() {
    controlOtherPlayer = false;
}

bool Player::hasDoubleTurn() const {
    return doubleTurn;
}

void Player::setDoubleTurn(bool value) {
    doubleTurn = value;
}

void Player::useDoubleTurn() {
    doubleTurn = false;
}
