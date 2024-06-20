#include "MazeWidget.h"
#include <QPainter>
#include <QKeyEvent>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <QMessageBox>

#define CELL_SIZE 30
#define ROWS 9
#define COLS 9
#define MAX_WINDOW_WIDTH 800
#define MAX_WINDOW_HEIGHT 600

MazeWidget::MazeWidget(QWidget *parent)
    : QWidget(parent), mazeGenerator(ROWS, COLS),
    player1(mazeGenerator.getNode(0, 0), 1),
    player2(mazeGenerator.getNode(0, COLS - 1), 2),
    treasureCell(nullptr),
    game(&player1, &player2, nullptr) {

    if (CELL_SIZE < 0 || ROWS < 0 || COLS < 0) {
        throw std::invalid_argument("Error de Render: Valores inválidos para CELL_SIZE, ROWS o COLS.");
    }

    int totalWidth = CELL_SIZE * COLS;
    int totalHeight = CELL_SIZE * ROWS;
    if (totalWidth > MAX_WINDOW_WIDTH || totalHeight > MAX_WINDOW_HEIGHT) {
        throw std::invalid_argument("Error de Render: Tamaño del laberinto excede el tamaño máximo de la ventana.");
    }

    setFixedSize(totalWidth, totalHeight); // Corrected this line to set both width and height
    placeTreasure();
}

void MazeWidget::placeTreasure() {
    srand(static_cast<unsigned int>(time(nullptr)));

    int treasureX, treasureY;
    do {
        treasureX = rand() % COLS;
        treasureY = rand() % ROWS;
    } while ((treasureX == 0 && treasureY == 0) ||
             (treasureX == COLS - 1 && treasureY == 0) ||
             !isStrategicPosition(treasureX, treasureY));

    treasureCell = mazeGenerator.getNode(treasureY, treasureX);
    treasureCell->placeTreasure();

    game = Game(&player1, &player2, treasureCell);
}

bool MazeWidget::isStrategicPosition(int x, int y) {
    int player1Distance = abs(player1.getCurrentCell()->getX() - x) + abs(player1.getCurrentCell()->getY() - y);
    int player2Distance = abs(player2.getCurrentCell()->getX() - x) + abs(player2.getCurrentCell()->getY() - y);
    return player1Distance > 3 && player2Distance > 3 && abs(player1Distance - player2Distance) <= 2;
}

void MazeWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawMaze(painter);
    drawTreasure(painter);
    drawPowers(painter);
    drawPortals(painter);
    drawPlayer(painter, player1, Qt::red);
    drawPlayer(painter, player2, Qt::blue);
    drawCurrentPlayer(painter);

    QWidget::paintEvent(event);
}

void MazeWidget::drawMaze(QPainter &painter) {
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            Cell* cell = mazeGenerator.getNode(i, j);
            if (cell->hasWall(0)) {
                painter.drawLine(j * CELL_SIZE, i * CELL_SIZE, (j + 1) * CELL_SIZE, i * CELL_SIZE);
            }
            if (cell->hasWall(2)) {
                painter.drawLine(j * CELL_SIZE, (i + 1) * CELL_SIZE, (j + 1) * CELL_SIZE, (i + 1) * CELL_SIZE);
            }
            if (cell->hasWall(3)) {
                painter.drawLine(j * CELL_SIZE, i * CELL_SIZE, j * CELL_SIZE, (i + 1) * CELL_SIZE);
            }
            if (cell->hasWall(1)) {
                painter.drawLine((j + 1) * CELL_SIZE, i * CELL_SIZE, (j + 1) * CELL_SIZE, (i + 1) * CELL_SIZE);
            }
        }
    }
}

void MazeWidget::drawPowers(QPainter &painter) {
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            Cell* cell = mazeGenerator.getNode(i, j);
            Cell::Power power = cell->getPower();
            if (power != Cell::None) {
                // Adjust color and shape based on power
                QColor color;
                switch (power) {
                case Cell::JumpWalls:
                    color = Qt::green;
                    break;
                case Cell::ControlOtherPlayer:
                    color = Qt::red;
                    break;
                case Cell::DoubleTurn:
                    color = Qt::blue;
                    break;
                default:
                    color = Qt::gray; // Default color
                    break;
                }
                int x = cell->getX() * CELL_SIZE + CELL_SIZE / 4;
                int y = cell->getY() * CELL_SIZE + CELL_SIZE / 4;
                painter.setBrush(QBrush(color));
                painter.drawEllipse(x, y, CELL_SIZE / 2, CELL_SIZE / 2);
                painter.setBrush(Qt::NoBrush); // Reset brush
            }
        }
    }
}

void MazeWidget::drawPortals(QPainter &painter) {
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            Cell* cell = mazeGenerator.getNode(i, j);
            if (cell->hasPortal()) {
                int x = cell->getX() * CELL_SIZE + CELL_SIZE / 4;
                int y = cell->getY() * CELL_SIZE + CELL_SIZE / 4;
                painter.setBrush(QBrush(Qt::magenta));
                painter.drawRect(x, y, CELL_SIZE / 2, CELL_SIZE / 2);
                painter.setBrush(Qt::NoBrush); // Reset brush
            }
        }
    }
}

void MazeWidget::drawPlayer(QPainter &painter, const Player &player, QColor color) {
    Cell* cell = player.getCurrentCell();
    int x = cell->getX() * CELL_SIZE;
    int y = cell->getY() * CELL_SIZE;

    painter.setBrush(QBrush(color));
    painter.drawRect(x + 5, y + 5, CELL_SIZE - 10, CELL_SIZE - 10);
}

void MazeWidget::drawCurrentPlayer(QPainter &painter) {
    Player* currentPlayer = game.getCurrentPlayer();
    Cell* cell = currentPlayer->getCurrentCell();
    int x = cell->getX() * CELL_SIZE;
    int y = cell->getY() * CELL_SIZE;

    painter.setPen(QPen(Qt::black, 3));
    painter.drawRect(x + 2, y + 2, CELL_SIZE - 4, CELL_SIZE - 4);
}

void MazeWidget::drawTreasure(QPainter &painter) {
    if (treasureCell) {
        int x = treasureCell->getX() * CELL_SIZE + CELL_SIZE / 4;
        int y = treasureCell->getY() * CELL_SIZE + CELL_SIZE / 4;
        painter.setBrush(QBrush(Qt::yellow));
        painter.drawEllipse(x, y, CELL_SIZE / 2, CELL_SIZE / 2);
    }
}

void MazeWidget::keyPressEvent(QKeyEvent *event) {
    int key = event->key();
    play(key);
    update();
}

bool MazeWidget::updatePlayerPosition(Player* player, int key) {
    Cell* currentCell = player->getCurrentCell();
    Cell* nextCell = nullptr;

    switch (key) {
    case Qt::Key_W:
    case Qt::Key_Up:
        if (currentCell->getUp() && (!currentCell->hasWall(0) || player->canJumpWalls())) {
            nextCell = currentCell->getUp();
            player->moveUp();
        }
        break;
    case Qt::Key_S:
    case Qt::Key_Down:
        if (currentCell->getDown() && (!currentCell->hasWall(2) || player->canJumpWalls())) {
            nextCell = currentCell->getDown();
            player->moveDown();
        }
        break;
    case Qt::Key_A:
    case Qt::Key_Left:
        if (currentCell->getLeft() && (!currentCell->hasWall(3) || player->canJumpWalls())) {
            nextCell = currentCell->getLeft();
            player->moveLeft();
        }
        break;
    case Qt::Key_D:
    case Qt::Key_Right:
        if (currentCell->getRight() && (!currentCell->hasWall(1) || player->canJumpWalls())) {
            nextCell = currentCell->getRight();
            player->moveRight();
        }
        break;
    }

    if (nextCell) {
        if (nextCell->hasPortal()) {
            int targetX = rand() % COLS;
            int targetY = rand() % ROWS;
            Cell* portalTarget = mazeGenerator.getNode(targetY, targetX);
            player->setCurrentCell(portalTarget);
        } else {
            player->setCurrentCell(nextCell);
        }

        Cell::Power power = nextCell->getPower();
        if (power != Cell::None) {
            player->collectPower(power);
            nextCell->removePower();
        }

        if (nextCell == treasureCell) {
            player->setWinner(true);
            QMessageBox::information(this, "Game Over", QString("Player %1 wins!").arg(player->getId()));
        }

        return true;
    }

    return false;
}

void MazeWidget::play(int key) {
    Player* currentPlayer = game.getCurrentPlayer();
    if (updatePlayerPosition(currentPlayer, key)) {
        game.switchTurn();
    }
}
