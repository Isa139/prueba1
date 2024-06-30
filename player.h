#ifndef PLAYER_H
#define PLAYER_H

#include "Cell.h"

class Player {
public:
    Player(Cell* startCell, int id);

    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();

    Cell* getCurrentCell() const;
    void setCurrentCell(Cell* cell);

    int getId() const;
    int getX() const;
    int getY() const;

    bool canJumpWalls() const;
    void setCanJumpWalls(bool value);
    void activateJumpWalls();
    void consumeJumpWalls();

    bool canControlOtherPlayer() const;
    void setControlOtherPlayer(bool value);
    void useControlOtherPlayer();

    bool hasDoubleTurn() const;
    void setDoubleTurn(bool value);
    void useDoubleTurn();

private:
    Cell* currentCell;
    int id;
    bool jumpWalls;
    bool jumpWallsActive;
    bool controlOtherPlayer;
    bool doubleTurn;
};

#endif // PLAYER_H
