#ifndef PLAYER_H
#define PLAYER_H

#include "cell.h"

class Player {
public:
    Player(Cell* startCell, int id);
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    Cell* getCurrentCell() const;
    int getId() const;
    bool isWinner() const;
    void setWinner(bool isWinner);
    void collectPower(Cell::Power power);
    bool canJumpWalls() const;
    void setCurrentCell(Cell* cell);

private:
    Cell* currentCell;
    int playerId;
    bool winner;
    bool jumpWalls; // New member variable
};

#endif // PLAYER_H
