#ifndef CELL_H
#define CELL_H

class Cell {
public:
    enum Power {
        None = 0,
        JumpWalls,
        ControlOtherPlayer,
        DoubleTurn
    };
    Cell();
    Cell(int x, int y);

    bool hasWall(int direction) const;
    void removeWall(int direction);

    void visit();
    bool isVisited() const;

    void placeTreasure();
    bool hasTreasure() const;

    int getX() const;
    int getY() const;

    void placePower(Power power);
    Power getPower() const;

    Cell* getUp() const;
    Cell* getDown() const;
    Cell* getLeft() const;
    Cell* getRight() const;

    void setUp(Cell* up);
    void setDown(Cell* down);
    void setLeft(Cell* left);
    void setRight(Cell* right);

    void placePortal(Cell* destination);
    Cell* getPortalDestination() const;
    bool hasPortal() const;
    void usePortal();
    bool isPortalUsed() const;

private:
    int x, y;
    bool visited;
    bool walls[4]; // { up, right, down, left }
    bool treasure;
    Power power;

    Cell* up;
    Cell* down;
    Cell* left;
    Cell* right;

    Cell* portalDestination;
    bool portalUsed;
};

#endif // CELL_H
