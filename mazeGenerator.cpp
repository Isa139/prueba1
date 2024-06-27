#include "MazeGenerator.h"
#include "constants.h"
#include "Cell.h"
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
            maze[i][j] = Cell(j, i); // Asignar coordenadas correctas
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
    // Connect each cell with its neighbors
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (i > 0) maze[i][j].setUp(&maze[i - 1][j]);
            if (i < rows - 1) maze[i][j].setDown(&maze[i + 1][j]);
            if (j > 0) maze[i][j].setLeft(&maze[i][j - 1]);
            if (j < cols - 1) maze[i][j].setRight(&maze[i][j + 1]);
        }
    }

    // Initialize random number generator
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Place powers randomly in the maze based on POWER_SPAWN_RATE
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (static_cast<double>(std::rand()) / RAND_MAX < POWER_SPAWN_RATE) {
                Cell::Power power = static_cast<Cell::Power>(std::rand() % 3 + 1); // Adjust range for new powers
                maze[i][j].placePower(power);
            }
        }
    }

    // Place portals randomly in the maze based on PORTAL_SPAWN_RATE
    std::vector<Cell*> portalCandidates;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (static_cast<double>(std::rand()) / RAND_MAX < PORTAL_SPAWN_RATE) {
                portalCandidates.push_back(&maze[i][j]);
            }
        }
    }

    // Shuffle portal candidates to randomize portal connections
    std::shuffle(portalCandidates.begin(), portalCandidates.end(), std::mt19937(std::random_device()()));

    // Pair up portal candidates and establish bidirectional portals
    for (size_t i = 0; i + 1 < portalCandidates.size(); i += 2) {
        portalCandidates[i]->placePortal(portalCandidates[i + 1]);
        portalCandidates[i + 1]->placePortal(portalCandidates[i]);
    }

// Use selected maze generation algorithm (DFS or BFS based on compile-time flag)
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
    if (current->getUp() == next) {
        current->removeWall(0);
        next->removeWall(2);
    } else if (current->getDown() == next) {
        current->removeWall(2);
        next->removeWall(0);
    } else if (current->getLeft() == next) {
        current->removeWall(3);
        next->removeWall(1);
    } else if (current->getRight() == next) {
        current->removeWall(1);
        next->removeWall(3);
    }
}

Cell* MazeGenerator::getNeighbor(Cell* cell, int direction) const {
    switch (direction) {
    case 0: return cell->getUp();
    case 1: return cell->getRight();
    case 2: return cell->getDown();
    case 3: return cell->getLeft();
    default: return nullptr;
    }
}
