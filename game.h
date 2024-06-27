#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Cell.h"

class Game {
public:
    Game(Player* player1, Player* player2, Cell* treasureCell);

    Player* getCurrentPlayer() const;
    Player* getOtherPlayer() const;
    void nextTurn();

    bool checkTreasureFound() const;

    bool isExtraTurn() const;
    void setExtraTurn(bool value);

    bool isControlEnemy() const;
    void setControlEnemy(bool value);

private:
    Player* player1;
    Player* player2;
    Cell* treasureCell;
    Player* currentPlayer;
    bool extraTurn;
    bool controlEnemy;
    bool tempControl;
};

#endif // GAME_H
