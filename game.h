#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Cell.h"

class Game {
public:
    Game(Player* player1, Player* player2, Cell* treasureCell);

    void nextTurn();
    Player* getCurrentPlayer();
    bool checkTreasureFound() const;
    void switchTurn();

private:
    Player* player1;
    Player* player2;
    Player* currentPlayer;
    Cell* treasureCell;
};

#endif // GAME_H
