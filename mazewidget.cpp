#include "MazeWidget.h"
#include <QPainter>
#include <QKeyEvent>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <QMessageBox>
#include "constants.h"

//constructor
MazeWidget::MazeWidget(QWidget *parent)
    : QWidget(parent), mazeGenerator(ROWS, COLS),
    player1(mazeGenerator.getNode(0, 0), 1),//row 0, column 0, player1
    player2(mazeGenerator.getNode(0, COLS - 1), 2), //row 0, last column, player 2
    treasureCell(nullptr),
    game(&player1, &player2, nullptr) {

    if (CELL_SIZE < 0 || ROWS < 0 || COLS < 0) {
        throw std::invalid_argument("Rendering error: invalid CELL_SIZE, ROWS or COLS values.");
    }

    int totalWidth = CELL_SIZE * COLS;
    int totalHeight = CELL_SIZE * ROWS;

    if (totalWidth > MAX_WINDOW_WIDTH || totalHeight > MAX_WINDOW_HEIGHT) {
        throw std::invalid_argument("Rendering error: Maze Size exceeds maximun window height.");
    }

    setFixedSize(totalWidth, totalHeight);
    placeTreasure();
}

void MazeWidget::placeTreasure() {
    srand(static_cast<unsigned int>(time(nullptr))); // Seed

    int treasureX = -1;
    int treasureY = -1;
    bool found = false;

    while (!found) {
        treasureX = rand() % COLS;
        treasureY = rand() % ROWS;

        Cell* potentialCell = mazeGenerator.getNode(treasureY, treasureX);

        if (isStrategicPosition(treasureX, treasureY) &&
            !(treasureX == 0 && treasureY == 0) &&
            !(treasureX == COLS - 1 && treasureY == 0) &&
            !potentialCell->hasPortal() &&
            !potentialCell->getPower()) {
            found = true;
        }
    }

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
    displayText(painter);

    QWidget::paintEvent(event);
}

void MazeWidget::drawMaze(QPainter &painter) {
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {

            Cell* cell = mazeGenerator.getNode(i, j);

            //+1 is added to i and j indexes to in order to get the coordinates of the final position of the cell
            if (cell->hasWall(UP)) {
                painter.drawLine(j * CELL_SIZE, i * CELL_SIZE, (j + 1) * CELL_SIZE, i * CELL_SIZE);
            }
            if (cell->hasWall(DOWN)) {
                painter.drawLine(j * CELL_SIZE, (i + 1) * CELL_SIZE, (j + 1) * CELL_SIZE, (i + 1) * CELL_SIZE);
            }
            if (cell->hasWall(LEFT)) {
                painter.drawLine(j * CELL_SIZE, i * CELL_SIZE, j * CELL_SIZE, (i + 1) * CELL_SIZE);
            }
            if (cell->hasWall(RIGHT)) {
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

            if ((power != Cell::None)) {

                QColor color;

                    if (power == Cell::JumpWalls) {
                        color = Qt::green;
                    } else if (power == Cell::ControlOtherPlayer) {
                        color = Qt::red;
                    } else if (power == Cell::DoubleTurn) {
                        color = Qt::blue;
                    }

                    //the cell size is divided by 4 so the power is drawn in the center rather than the border of the cell
                    int x = cell->getX() * CELL_SIZE + CELL_SIZE/4;
                    int y = cell->getY() * CELL_SIZE + CELL_SIZE/4;

                    painter.setBrush(QBrush(color));
                    //the cell size is divided by 2 so the power identifier is drawn in the middle of the circle
                    painter.drawEllipse(x, y, CELL_SIZE / 2, CELL_SIZE / 2);
                    painter.drawText(x, y, CELL_SIZE / 2, CELL_SIZE / 2, Qt::AlignCenter, QString::number(static_cast<int>(power)));

            }

        }
    }
}

//all the draw methods contain constants divided by 2 and 4 for the same reason as explained above

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
    Player* otherPlayer = game.getOtherPlayer();

    if (currentPlayer->canControlOtherPlayer()) {

        translateKeyForOtherPlayer(key, otherPlayer);
        currentPlayer->useControlOtherPlayer();

        update();
        return;
    }

    if ((currentPlayer == &player1 && (key == Qt::Key_W || key == Qt::Key_A || key == Qt::Key_S || key == Qt::Key_D)) || (currentPlayer == &player2 && (key == Qt::Key_Up || key == Qt::Key_Left || key == Qt::Key_Down || key == Qt::Key_Right))) {
        if (play(key, currentPlayer)) {
            if (!currentPlayer->hasDoubleTurn()) {
                game.nextTurn();
            } else {
                currentPlayer->useDoubleTurn();
            }
        }
    }

    update();
}

void MazeWidget::translateKeyForOtherPlayer(int key, Player* otherPlayer) {
    int translatedKey = key;

        if (key == Qt::Key_W) {
            translatedKey = Qt::Key_Up;
        } else if (key == Qt::Key_A) {
            translatedKey = Qt::Key_Left;
        } else if (key == Qt::Key_S) {
            translatedKey = Qt::Key_Down;
        } else if (key == Qt::Key_D) {
            translatedKey = Qt::Key_Right;
        } else if (key == Qt::Key_Up) {
            translatedKey = Qt::Key_W;
        } else if (key == Qt::Key_Left) {
            translatedKey = Qt::Key_A;
        } else if (key == Qt::Key_Down) {
            translatedKey = Qt::Key_S;
        } else if (key == Qt::Key_Right) {
            translatedKey = Qt::Key_D;
        }

    play(translatedKey, otherPlayer);
}

void MazeWidget::displayText(QPainter &painter) {
    Player* currentPlayer = game.getCurrentPlayer();
    QString playerName = (currentPlayer == &player1) ? "Player 1" : "Player 2";
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 16)); //font size
    painter.drawText(rect(), Qt::AlignTop | Qt::AlignHCenter, "Turn: " + playerName);
}

bool MazeWidget::updatePlayerPosition(Player *player, int key) {
    Cell* currentCell = player->getCurrentCell();
    Cell* nextCell = nullptr;

    if (key == Qt::Key_W || key == Qt::Key_Up) {
        if (currentCell->getUp() && (!currentCell->hasWall(UP) || player->canJumpWalls())) {
            nextCell = currentCell->getUp();
            player->moveUp();

            if (player->canJumpWalls() && currentCell->hasWall(UP)) {
                player->consumeJumpWalls();
            }
        }
    } else if (key == Qt::Key_S || key == Qt::Key_Down) {
        if (currentCell->getDown() && (!currentCell->hasWall(DOWN) || player->canJumpWalls())) {
            nextCell = currentCell->getDown();
            player->moveDown();

            if (player->canJumpWalls() && currentCell->hasWall(DOWN)) {
                player->consumeJumpWalls();
            }
        }
    } else if (key == Qt::Key_A || key == Qt::Key_Left) {
        if (currentCell->getLeft() && (!currentCell->hasWall(LEFT) || player->canJumpWalls())) {
            nextCell = currentCell->getLeft();
            player->moveLeft();

            if (player->canJumpWalls() && currentCell->hasWall(LEFT)) {
                player->consumeJumpWalls();
            }
        }
    } else if (key == Qt::Key_D || key == Qt::Key_Right) {
        if (currentCell->getRight() && (!currentCell->hasWall(RIGHT) || player->canJumpWalls())) {
            nextCell = currentCell->getRight();
            player->moveRight();

            if (player->canJumpWalls() && currentCell->hasWall(RIGHT)) {
                player->consumeJumpWalls();
            }
        }
    }


    if (nextCell) {
        Cell::Power power = nextCell->getPower();
        if (power != Cell::None) {
            handlePower(player, nextCell, power);
        }


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

        return true;
    }

    return false;
}

void MazeWidget::handlePower(Player *player, Cell *cell, Cell::Power power) {

    if (power == Cell::JumpWalls) {
        QMessageBox::information(this, "Power Activated", "You can now jump over walls!");
        player->setCanJumpWalls(true);

    } else if (power == Cell::ControlOtherPlayer) {
        QMessageBox::information(this, "Power Activated", "You can now control another player!");
        player->setControlOtherPlayer(true);

    } else if (power == Cell::DoubleTurn) {
        QMessageBox::information(this, "Power Activated", "You get an extra turn!");
        player->setDoubleTurn(true);
    }

    clearPower(cell);
}

bool MazeWidget::play(int key, Player* player) {
//handles the overall functioning of the game. If the cell the player chose contains the treasure, the game ends. Otherwise, returns true for a valid move and updates board
    if (updatePlayerPosition(player, key)) {
        if (game.checkTreasureFound()) {
            QString message = QString("Player %1 has found the treasure!").arg(player->getId());
            QMessageBox msgBox;
            msgBox.setText(message);
            msgBox.setInformativeText("Do you want to end the game or restart?");
            msgBox.setStandardButtons(QMessageBox::Close | QMessageBox::Retry);
            msgBox.setDefaultButton(QMessageBox::Retry);
            int ret = msgBox.exec();
            if (ret == QMessageBox::Close) {
                close();
            } else if (ret == QMessageBox::Retry) {
                placeTreasure();
                player1.setCurrentCell(mazeGenerator.getNode(0, 0));
                player2.setCurrentCell(mazeGenerator.getNode(0, COLS - 1));
                update();
            }

        }
        return true;
    }
    return false;
}

void MazeWidget::clearPower(Cell* cell) {
    cell->placePower(Cell::None);
}
