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


//maze constructor
MazeGenerator::MazeGenerator(int rows, int cols) : rows(rows), cols(cols) {
    maze = new Cell*[rows];
    for (int i = 0; i < rows; ++i) {
        maze[i] = new Cell[cols];
        for (int j = 0; j < cols; ++j) {
            maze[i][j] = Cell(j, i);
        }
    }
    createMaze();
}

//maze destructor
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
            if (i > 0) maze[i][j].setUp(&maze[i - 1][j]);
            if (i < rows - 1) maze[i][j].setDown(&maze[i + 1][j]);
            if (j > 0) maze[i][j].setLeft(&maze[i][j - 1]);
            if (j < cols - 1) maze[i][j].setRight(&maze[i][j + 1]);
        }
    }

    std::srand(static_cast<unsigned int>(std::time(nullptr)));


    const int MAX_CELLS = ROWS * COLS;
    Cell* portalCandidates[MAX_CELLS];
    int candidateCount = 0;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if ((i!=0 && j!=0)||(i!=0 && j!=cols-1)) {
                if (static_cast<double>(std::rand()) / RAND_MAX < PORTAL_SPAWN_RATE) {
                    if (candidateCount < MAX_CELLS) {
                        portalCandidates[candidateCount++] = &maze[i][j];
                    }
                }
            }
        }
        if (candidateCount >= MAX_CELLS) {
            break;
        }
    }


    std::srand(static_cast<unsigned int>(std::time(nullptr)));


    for (int i = candidateCount - 1; i > 0; --i) {
        int j = std::rand() % (i + 1);
        std::swap(portalCandidates[i], portalCandidates[j]);
    }


    for (int i = 0; i + 1 < candidateCount; i += 2) {
        portalCandidates[i]->placePortal(portalCandidates[i + 1]);
        portalCandidates[i + 1]->placePortal(portalCandidates[i]);
    }

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            Cell* cell = getNode(i, j);

            if ((i!=0 && j!=0)||(i!=0 && j!=cols-1)) {
                if (!(cell->hasPortal())) {
                    if (static_cast<double>(std::rand()) / RAND_MAX < POWER_SPAWN_RATE) {
                        Cell::Power power = static_cast<Cell::Power>(std::rand() % POWER_TYPES + 1);
                        maze[i][j].placePower(power);
                    }
                }
            }
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
    //double wall removal
    if (current->getUp() == next) {
        current->removeWall(UP);
        next->removeWall(DOWN);
    } else if (current->getDown() == next) {
        current->removeWall(DOWN);
        next->removeWall(UP);
    } else if (current->getLeft() == next) {
        current->removeWall(LEFT);
        next->removeWall(RIGHT);
    } else if (current->getRight() == next) {
        current->removeWall(RIGHT);
        next->removeWall(LEFT);
    }
}

Cell* MazeGenerator::getNeighbor(Cell* cell, int direction) const {
    if (direction == 0) {
        return cell->getUp();
    } else if (direction == 1) {
        return cell->getRight();
    } else if (direction == 2) {
        return cell->getDown();
    } else if (direction == 3) {
        return cell->getLeft();
    } else {
        return nullptr;
    }

}
