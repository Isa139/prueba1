#ifndef CELL_H
#define CELL_H

class Cell {
public:
    Cell(int x = 0, int y = 0);

    bool hasWall(int direction) const;
    void removeWall(int direction);
    void visit();
    bool isVisited() const;

    void placeTreasure();
    bool hasTreasure() const;

    Cell* up;
    Cell* down;
    Cell* left;
    Cell* right;

    int getX() const;
    int getY() const;

private:
    int x, y;
    bool visited;
    bool walls[4]; // Top, Right, Bottom, Left
    bool treasure; // Indicates if the cell contains a treasure
};

#endif // CELL_H
