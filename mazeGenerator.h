#ifndef MAZEGENERATOR_H
#define MAZEGENERATOR_H

#include "Cell.h"
#include <stack>
#include <queue>
#include <vector>
#include "constants.h"

class MazeGenerator {
public:
    MazeGenerator(int rows, int cols);
    ~MazeGenerator();

    void createMaze();
    Cell* getNode(int row, int col) const;
    int getRows() const { return rows; }
    int getCols() const { return cols; }
    void generateMazeDFS();
    void generateMazeBFS(int startRow, int startCol);


private:
    void connectCells();
    void placePortals();
    void placePowers();
    void removeWalls(Cell* current, Cell* next);
    Cell* getNeighbor(Cell* cell, int direction) const;
    void removeRandomEdge();

    int rows;
    int cols;
    Cell** maze;
};

#endif // MAZEGENERATOR_H
