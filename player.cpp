#include "Player.h"
#include "constants.h"

//player constructor
Player::Player(Cell* startCell, int id)
    : currentCell(startCell), id(id), jumpWalls(false), jumpWallsActive(false), controlOtherPlayer(false), doubleTurn(false) {}

void Player::moveUp() {
    if (currentCell->getUp() && (!currentCell->hasWall(UP) || jumpWallsActive)) {
        if (currentCell->hasWall(UP) && jumpWallsActive) {
            consumeJumpWalls();
        }
        currentCell = currentCell->getUp();
    }
}

void Player::moveDown() {
    if (currentCell->getDown() && (!currentCell->hasWall(DOWN) || jumpWallsActive)) {
        if (currentCell->hasWall(DOWN) && jumpWallsActive) {
            consumeJumpWalls();
        }
        currentCell = currentCell->getDown();
    }
}

void Player::moveLeft() {
    if (currentCell->getLeft() && (!currentCell->hasWall(LEFT) || jumpWallsActive)) {
        if (currentCell->hasWall(LEFT) && jumpWallsActive) {
            consumeJumpWalls();
        }
        currentCell = currentCell->getLeft();
    }
}

void Player::moveRight() {
    if (currentCell->getRight() && (!currentCell->hasWall(RIGHT) || jumpWallsActive)) {
        if (currentCell->hasWall(RIGHT) && jumpWallsActive) {
            consumeJumpWalls();
        }
        currentCell = currentCell->getRight();
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
    if (currentCell) {
        return currentCell->getX();
    } else {
        return -1;
    }
}

int Player::getY() const {
    if (currentCell) {
        return currentCell->getY();
    } else {
        return -1;
    }
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
