#include "MazeWidget.h"
#include <QPainter>
#include <QPaintEvent>
#include <stdexcept> // Para usar std::invalid_argument
#include <iostream>  // Para imprimir en la consola
#include <cstdlib>   // Para srand() y rand()
#include <ctime>     // Para time()

#define CELL_SIZE 30
#define ROWS 13
#define COLS 13
#define MAX_WINDOW_WIDTH 800
#define MAX_WINDOW_HEIGHT 600

MazeWidget::MazeWidget(QWidget *parent)
    : QWidget(parent), mazeGenerator(ROWS, COLS),
    player1(mazeGenerator.getNode(0, 0), 1),
    player2(mazeGenerator.getNode(0, COLS - 1), 2),
    treasureCell(nullptr),
    game(&player1, &player2, nullptr) { // Inicializa el juego con los jugadores y el tesoro como nullptr

    // Verificar que CELL_SIZE, ROWS y COLS sean valores válidos
    if (CELL_SIZE < 0) {
        throw std::invalid_argument("Error de Render: CELL_SIZE debe ser mayor que 0.");
    }
    if (ROWS < 0) {
        throw std::invalid_argument("Error de Render: ROWS debe ser mayor que 0.");
    }
    if (COLS < 0) {
        throw std::invalid_argument("Error de Render: COLS debe ser mayor que 0.");
    }

    // Verificar que el tamaño del laberinto no exceda el tamaño de la ventana
    int totalWidth = CELL_SIZE * COLS;
    int totalHeight = CELL_SIZE * ROWS;
    if (totalWidth > MAX_WINDOW_WIDTH) {
        throw std::invalid_argument("Error de Render: El ancho total del laberinto excede el tamaño máximo de la ventana.");
    }
    if (totalHeight > MAX_WINDOW_HEIGHT) {
        throw std::invalid_argument("Error de Render: El alto total del laberinto excede el tamaño máximo de la ventana.");
    }

    // Tamaño de la ventana basado en el tamaño de celda y número de celdas
    setFixedSize(totalWidth, totalHeight);

    // Generar el laberinto
    mazeGenerator.createMaze();

    // Colocar el tesoro en una celda aleatoria
    placeTreasure();
}

void MazeWidget::placeTreasure() {
    srand(static_cast<unsigned int>(time(nullptr))); // Inicializa el generador de números aleatorios

    int treasureX, treasureY;
    do {
        treasureX = rand() % COLS;
        treasureY = rand() % ROWS;
    } while ((treasureX == 0 && treasureY == 0) ||
             (treasureX == COLS - 1 && treasureY == 0) ||
             !isStrategicPosition(treasureX, treasureY));

    treasureCell = mazeGenerator.getNode(treasureY, treasureX);
    treasureCell->placeTreasure();

    // Actualizar la referencia del tesoro en el juego
    game = Game(&player1, &player2, treasureCell);
}

bool MazeWidget::isStrategicPosition(int x, int y) {
    int player1Distance = abs(player1.getCurrentCell()->getX() - x) + abs(player1.getCurrentCell()->getY() - y);
    int player2Distance = abs(player2.getCurrentCell()->getX() - x) + abs(player2.getCurrentCell()->getY() - y);

    // Consider the position strategic if the treasure is not too close to any player and somewhat equidistant
    return player1Distance > 3 && player2Distance > 3 && abs(player1Distance - player2Distance) <= 2;
}

void MazeWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            Cell* cell = mazeGenerator.getNode(i, j);

            // Dibujar las paredes solo si existen
            if (cell->hasWall(0)) {
                painter.drawLine(j * CELL_SIZE, i * CELL_SIZE, (j + 1) * CELL_SIZE, i * CELL_SIZE); // Línea superior
            }
            if (cell->hasWall(2)) {
                painter.drawLine(j * CELL_SIZE, (i + 1) * CELL_SIZE, (j + 1) * CELL_SIZE, (i + 1) * CELL_SIZE); // Línea inferior
            }
            if (cell->hasWall(3)) {
                painter.drawLine(j * CELL_SIZE, i * CELL_SIZE, j * CELL_SIZE, (i + 1) * CELL_SIZE); // Línea izquierda
            }
            if (cell->hasWall(1)) {
                painter.drawLine((j + 1) * CELL_SIZE, i * CELL_SIZE, (j + 1) * CELL_SIZE, (i + 1) * CELL_SIZE); // Línea derecha
            }
        }
    }

    // Dibujar el tesoro
    drawTreasure(painter);

    // Dibujar los jugadores
    drawPlayer(painter, player1, Qt::red);
    drawPlayer(painter, player2, Qt::blue);

    QWidget::paintEvent(event);  // Llamada al método base para manejar otros eventos de pintura
}

void MazeWidget::keyPressEvent(QKeyEvent *event) {
    int key = event->key();
    Player* currentPlayer = game.getCurrentPlayer();

    if ((currentPlayer == &player1 && (key == Qt::Key_W || key == Qt::Key_A || key == Qt::Key_S || key == Qt::Key_D)) ||
        (currentPlayer == &player2 && (key == Qt::Key_Up || key == Qt::Key_Left || key == Qt::Key_Down || key == Qt::Key_Right))) {
        play(key);  // Llamar al método play para manejar el turno
        update();
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

bool MazeWidget::updatePlayerPosition(Player *player, int key) {
    Cell* currentCell = player->getCurrentCell();
    switch (key) {
    case Qt::Key_W: if (currentCell->up && !currentCell->hasWall(0)) { player->moveUp(); return true; } break;
    case Qt::Key_S: if (currentCell->down && !currentCell->hasWall(2)) { player->moveDown(); return true; } break;
    case Qt::Key_A: if (currentCell->left && !currentCell->hasWall(3)) { player->moveLeft(); return true; } break;
    case Qt::Key_D: if (currentCell->right && !currentCell->hasWall(1)) { player->moveRight(); return true; } break;
    case Qt::Key_Up: if (currentCell->up && !currentCell->hasWall(0)) { player->moveUp(); return true; } break;
    case Qt::Key_Down: if (currentCell->down && !currentCell->hasWall(2)) { player->moveDown(); return true; } break;
    case Qt::Key_Left: if (currentCell->left && !currentCell->hasWall(3)) { player->moveLeft(); return true; } break;
    case Qt::Key_Right: if (currentCell->right && !currentCell->hasWall(1)) { player->moveRight(); return true; } break;
    }
    return false;
}

void MazeWidget::play(int key) {
    Player* currentPlayer = game.getCurrentPlayer();
    if (updatePlayerPosition(currentPlayer, key)) {
        // Check if current player has found the treasure
        if (game.checkTreasureFound()) {
            std::cout << "Player " << currentPlayer->getId() << " has found the treasure!" << std::endl;
            treasureCell = nullptr; // Treasure found, remove it
            // Display a message or handle treasure found logic here
        }
        game.nextTurn();  // Cambiar al siguiente turno solo si hubo un movimiento
    }
}
