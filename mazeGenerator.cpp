#include "MazeGenerator.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <random>
#include <stack>
#include <vector>
#include <stdexcept>

MazeGenerator::MazeGenerator(int rows, int cols) : rows(rows), cols(cols) {
    maze = new Cell*[rows];
    for (int i = 0; i < rows; ++i) {
        maze[i] = new Cell[cols];
        for (int j = 0; j < cols; ++j) {
            maze[i][j] = Cell(j, i); // Assign correct coordinates
        }
    }
    createMaze();
}

MazeGenerator::~MazeGenerator() {
    for (int i = 0; i < rows; ++i) {
        delete[] maze[i];
    }
    delete[] maze;
}

Cell* MazeGenerator::getNode(int row, int col) const {
    if (row >= 0 && row < rows && col >= 0 && col < cols) {
        return &maze[row][col];
    }
    return nullptr;
}

void MazeGenerator::createMaze() {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (i > 0) maze[i][j].up = &maze[i - 1][j];
            if (i < rows - 1) maze[i][j].down = &maze[i + 1][j];
            if (j > 0) maze[i][j].left = &maze[i][j - 1];
            if (j < cols - 1) maze[i][j].right = &maze[i][j + 1];
        }
    }
#ifdef USE_DFS
    generateMazeDFS();
#elif defined(USE_BFS)
    generateMazeBFS();
#endif
}

void MazeGenerator::generateMazeDFS() {
    std::stack<Cell*> stack;
    Cell* startCell = &maze[0][0];
    startCell->visit();
    stack.push(startCell);

    std::srand(std::time(nullptr));
    std::random_device rd;
    std::mt19937 g(rd());

    while (!stack.empty()) {
        Cell* current = stack.top();
        stack.pop();

        std::vector<int> directions = {0, 1, 2, 3};
        std::shuffle(directions.begin(), directions.end(), g);

        for (int direction : directions) {
            Cell* next = getNeighbor(current, direction);
            if (next && !next->isVisited()) {
                next->visit();
                removeWalls(current, next);
                stack.push(current);
                stack.push(next);
                break;
            }
        }
    }
}

// BFS
void MazeGenerator::generateMazeBFS() {
    std::queue<Cell*> queue;
    Cell* startCell = &maze[0][0];
    startCell->visit();
    queue.push(startCell);

    std::srand(std::time(nullptr));

    while (!queue.empty()) {
        Cell* current = queue.front();
        queue.pop();

        std::vector<int> directions = {0, 1, 2, 3};
        std::shuffle(directions.begin(), directions.end(), std::default_random_engine(std::rand()));

        for (int direction : directions) {
            Cell* next = getNeighbor(current, direction);
            if (next && !next->isVisited()) {
                next->visit();
                removeWalls(current, next);
                queue.push(next);
            }
        }
    }
}


void MazeGenerator::removeWalls(Cell* current, Cell* next) {
    if (current->up == next) {
        current->removeWall(0);
        next->removeWall(2);
    } else if (current->down == next) {
        current->removeWall(2);
        next->removeWall(0);
    } else if (current->left == next) {
        current->removeWall(3);
        next->removeWall(1);
    } else if (current->right == next) {
        current->removeWall(1);
        next->removeWall(3);
    }
}

Cell* MazeGenerator::getNeighbor(Cell* cell, int direction) const {
    switch (direction) {
    case 0: return cell->up;
    case 1: return cell->right;
    case 2: return cell->down;
    case 3: return cell->left;
    default: return nullptr;
    }
}
