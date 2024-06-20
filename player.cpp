#include "player.h"

Player::Player(Cell* startCell, int id) : currentCell(startCell), playerId(id), winner(false), jumpWalls(false) {}

void Player::moveUp() {
    if (currentCell->getUp()) {
        currentCell = currentCell->getUp();
    }
}

void Player::moveDown() {
    if (currentCell->getDown()) {
        currentCell = currentCell->getDown();
    }
}

void Player::moveLeft() {
    if (currentCell->getLeft()) {
        currentCell = currentCell->getLeft();
    }
}

void Player::moveRight() {
    if (currentCell->getRight()) {
        currentCell = currentCell->getRight();
    }
}

Cell* Player::getCurrentCell() const {
    return currentCell;
}

int Player::getId() const {
    return playerId;
}

bool Player::isWinner() const {
    return winner;
}

void Player::setWinner(bool isWinner) {
    winner = isWinner;
}

void Player::collectPower(Cell::Power power) {
    switch (power) {
    case Cell::JumpWalls:
        jumpWalls = true;
        break;
    case Cell::ControlOtherPlayer:
        // Implement ControlOtherPlayer logic
        break;
    case Cell::DoubleTurn:
        // Implement DoubleTurn logic
        break;
    default:
        break;
    }
}

bool Player::canJumpWalls() const {
    return jumpWalls;
}

void Player::setCurrentCell(Cell* cell) {
    currentCell = cell;
}
