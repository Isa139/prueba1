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
    void activateJumpWalls(); // Método para activar el poder de saltar muros
    void consumeJumpWalls(); // Método para consumir el poder de saltar muros después de usarlo

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
    bool jumpWallsActive; // Estado para saber si el poder está activo
    bool controlOtherPlayer;
    bool doubleTurn;
};

#endif // PLAYER_H

