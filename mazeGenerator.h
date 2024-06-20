#ifndef MAZEGENERATOR_H
#define MAZEGENERATOR_H

#include "Cell.h"
#include <stack>
#include <queue>
#include <vector>

#define USE_DFS  // Cambia a #define USE_BFS para usar BFS
#define EXTRA_EDGE_PROB 0.1  // Probabilidad de eliminar una arista aleatoria

class MazeGenerator {
public:
    MazeGenerator(int rows, int cols);
    ~MazeGenerator();

    void createMaze();
    Cell* getNode(int row, int col) const;
    int getRows() const { return rows; }
    int getCols() const { return cols; }

private:
    void generateMazeDFS();
    void generateMazeBFS();
    void removeWalls(Cell* current, Cell* next);
    Cell* getNeighbor(Cell* cell, int direction) const;
    void removeRandomEdge();

    int rows;
    int cols;
    Cell** maze;
};

#endif // MAZEGENERATOR_H
