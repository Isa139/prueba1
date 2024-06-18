#include "Game.h"

Game::Game(Player* player1, Player* player2, Cell* treasureCell)
    : player1(player1), player2(player2), treasureCell(treasureCell), currentPlayer(player1) {}

void Game::nextTurn() {
    currentPlayer = (currentPlayer == player1) ? player2 : player1;
}

Player* Game::getCurrentPlayer() {
    return currentPlayer;
}

bool Game::checkTreasureFound() const {
    return currentPlayer->getCurrentCell() == treasureCell;
}
