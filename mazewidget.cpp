// MazeWidget.cpp
#include "MazeWidget.h"
#include <QPainter>
#include <QKeyEvent>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <QMessageBox>
#include "constants.h"

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

    setFixedSize(totalWidth, totalHeight);
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
                painter.drawText(x, y, CELL_SIZE / 2, CELL_SIZE / 2, Qt::AlignCenter, QString::number(static_cast<int>(power)));
            }
        }
    }
}

void MazeWidget::drawPortals(QPainter &painter) {
    painter.setBrush(QBrush(Qt::magenta));
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            Cell* cell = mazeGenerator.getNode(i, j);
            if (cell->hasPortal()) {
                int cx = cell->getX() * CELL_SIZE + CELL_SIZE / 2;
                int cy = cell->getY() * CELL_SIZE + CELL_SIZE / 2;
                QPolygon diamond;
                diamond << QPoint(cx, cy - CELL_SIZE / 4)
                        << QPoint(cx + CELL_SIZE / 4, cy)
                        << QPoint(cx, cy + CELL_SIZE / 4)
                        << QPoint(cx - CELL_SIZE / 4, cy);
                painter.drawPolygon(diamond);
            }
        }
    }
}

void MazeWidget::drawPlayer(QPainter &painter, const Player &player, QColor color) {
    int x = player.getCurrentCell()->getX() * CELL_SIZE + CELL_SIZE / 4;
    int y = player.getCurrentCell()->getY() * CELL_SIZE + CELL_SIZE / 4;
    painter.setBrush(QBrush(color));
    painter.drawRect(x, y, CELL_SIZE / 2, CELL_SIZE / 2);
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
    Player* currentPlayer = game.getCurrentPlayer();

    if ((currentPlayer == &player1 && (key == Qt::Key_W || key == Qt::Key_A || key == Qt::Key_S || key == Qt::Key_D)) ||
        (currentPlayer == &player2 && (key == Qt::Key_Up || key == Qt::Key_Left || key == Qt::Key_Down || key == Qt::Key_Right))) {
        play(key);
        update();
    }
}


void MazeWidget::drawCurrentPlayer(QPainter &painter) {
    Player* currentPlayer = game.getCurrentPlayer();
    QString playerName = (currentPlayer == &player1) ? "Player 1" : "Player 2";
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 16));
    painter.drawText(rect(), Qt::AlignTop | Qt::AlignHCenter, "Turn: " + playerName);
}

bool MazeWidget::updatePlayerPosition(Player *player, int key) {
    Cell* currentCell = player->getCurrentCell();
    Cell* nextCell = nullptr;

    switch (key) {
    case Qt::Key_W:
    case Qt::Key_Up:
        if (currentCell->getUp() && (!currentCell->hasWall(0) || player->canJumpWalls())) {
            nextCell = currentCell->getUp();
            player->moveUp();
            if (player->canJumpWalls() && currentCell->hasWall(0)) {
                player->consumeJumpWalls();
            }
        }
        break;
    case Qt::Key_S:
    case Qt::Key_Down:
        if (currentCell->getDown() && (!currentCell->hasWall(2) || player->canJumpWalls())) {
            nextCell = currentCell->getDown();
            player->moveDown();
            if (player->canJumpWalls() && currentCell->hasWall(2)) {
                player->consumeJumpWalls();
            }
        }
        break;
    case Qt::Key_A:
    case Qt::Key_Left:
        if (currentCell->getLeft() && (!currentCell->hasWall(3) || player->canJumpWalls())) {
            nextCell = currentCell->getLeft();
            player->moveLeft();
            if (player->canJumpWalls() && currentCell->hasWall(3)) {
                player->consumeJumpWalls();
            }
        }
        break;
    case Qt::Key_D:
    case Qt::Key_Right:
        if (currentCell->getRight() && (!currentCell->hasWall(1) || player->canJumpWalls())) {
            nextCell = currentCell->getRight();
            player->moveRight();
            if (player->canJumpWalls() && currentCell->hasWall(1)) {
                player->consumeJumpWalls();
            }
        }
        break;
    }

    if (nextCell) {
        // Check for power in the next cell
        Cell::Power power = nextCell->getPower();
        if (power != Cell::None) {
            handlePower(player, nextCell, power);
        }

        // Handle portal movement
        if (nextCell->hasPortal()) {
            Cell* portalDestination = nextCell->getPortalDestination();
            nextCell->usePortal();
            if (portalDestination) {
                portalDestination->usePortal();
                nextCell = portalDestination;
                player->setCurrentCell(nextCell);
            }
        } else {
            player->setCurrentCell(nextCell);
        }

        return true; // Move was successful
    }

    return false; // Move was not possible
}

void MazeWidget::handlePower(Player *player, Cell *cell, Cell::Power power) {
    switch (power) {
    case Cell::JumpWalls:
        QMessageBox::information(this, "Power Activated", "You can now jump over walls!");
        player->setCanJumpWalls(true);
        break;
    case Cell::ControlOtherPlayer:
        QMessageBox::information(this, "Power Activated", "You can now control another player!");
        player->setControlOtherPlayer(true);
        break;
    case Cell::DoubleTurn:
        QMessageBox::information(this, "Power Activated", "You get an extra turn!");
        player->setDoubleTurn(true);
        break;
    default:
        break;
    }

    // Clear the power from the cell after it's been used
    clearPower(cell);
}

void MazeWidget::play(int key) {
    Player* currentPlayer = game.getCurrentPlayer();

    // Attempt to update player position based on key input
    if (updatePlayerPosition(currentPlayer, key)) {
        // Check if the player has found the treasure
        if (game.checkTreasureFound()) {
            QString message = QString("Player %1 has found the treasure!").arg(currentPlayer->getId());
            QMessageBox msgBox;
            msgBox.setText(message);
            msgBox.setInformativeText("Do you want to end the game or restart?");
            msgBox.setStandardButtons(QMessageBox::Close | QMessageBox::Retry);
            msgBox.setDefaultButton(QMessageBox::Retry);
            int ret = msgBox.exec();

            switch (ret) {
            case QMessageBox::Close:
                // End the game
                close();
                break;
            case QMessageBox::Retry:
                // Restart the game
                placeTreasure();
                player1.setCurrentCell(mazeGenerator.getNode(0, 0));
                player2.setCurrentCell(mazeGenerator.getNode(0, COLS - 1));
                update();
                break;
            default:
                // should never be reached
                break;
            }
        }

        // Proceed to the next turn if no extra turn or control enemy is set
        if (currentPlayer->hasDoubleTurn()) {
            currentPlayer->useDoubleTurn(); // Consume el doble turno
        } else {
            game.nextTurn();
        }

        update();
    }
}

void MazeWidget::clearPower(Cell* cell) {
    cell->placePower(Cell::None);
}
