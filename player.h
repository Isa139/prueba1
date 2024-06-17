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
    int getId() const;

private:
    Cell* currentCell;
    int id;
};

#endif // PLAYER_H
