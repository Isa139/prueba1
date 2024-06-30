#include "Game.h"

//game constructor
Game::Game(Player* player1, Player* player2, Cell* treasureCell)
    : player1(player1), player2(player2), treasureCell(treasureCell), currentPlayer(player1), extraTurn(false), controlEnemy(false), tempControl(false) {}

Player* Game::getCurrentPlayer() const {
    return currentPlayer;
}

Player* Game::getOtherPlayer() const {
    if (currentPlayer == player1) {
        return player2;
    } else {
        return player1;
    }
}

//handles next turn (and power ups)
void Game::nextTurn() {
    if (extraTurn) {
        extraTurn = false; //handles a doubleTurn scenario
    } else if (controlEnemy) {
        tempControl = true;
        controlEnemy = false; //handles a controlEnemy scenario
    } else if (tempControl) {
        tempControl = false;
        currentPlayer = getOtherPlayer();
    } else {
        currentPlayer = getOtherPlayer();
    }
}

bool Game::checkTreasureFound() const {
    return currentPlayer->getCurrentCell() == treasureCell; //if the current cell is a treasure cell, the game will end
}

bool Game::isExtraTurn() const {
    return extraTurn;
}

