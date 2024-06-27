#include "Game.h"

Game::Game(Player* player1, Player* player2, Cell* treasureCell)
    : player1(player1), player2(player2), treasureCell(treasureCell), currentPlayer(player1), extraTurn(false), controlEnemy(false), tempControl(false) {}

Player* Game::getCurrentPlayer() const {
    return currentPlayer;
}

Player* Game::getOtherPlayer() const {
    return (currentPlayer == player1) ? player2 : player1;
}

void Game::nextTurn() {
    if (extraTurn) {
        extraTurn = false;
    } else if (controlEnemy) {
        tempControl = !tempControl;
        controlEnemy = false;
        currentPlayer = getOtherPlayer();
    } else if (tempControl) {
        tempControl = false;
        currentPlayer = getOtherPlayer();
    } else {
        currentPlayer = (currentPlayer == player1) ? player2 : player1;
    }
}

bool Game::checkTreasureFound() const {
    return currentPlayer->getCurrentCell() == treasureCell;
}

bool Game::isExtraTurn() const {
    return extraTurn;
}

void Game::setExtraTurn(bool value) {
    extraTurn = value;
}

bool Game::isControlEnemy() const {
    return controlEnemy;
}

void Game::setControlEnemy(bool value) {
    controlEnemy = value;
}
