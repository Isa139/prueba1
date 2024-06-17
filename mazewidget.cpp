#include "MazeWidget.h"
#include <QPainter>
#include <QPaintEvent>
#include <stdexcept> // Para usar std::invalid_argument

#define CELL_SIZE 30
#define ROWS 13
#define COLS 13
#define MAX_WINDOW_WIDTH 800
#define MAX_WINDOW_HEIGHT 600

MazeWidget::MazeWidget(QWidget *parent)
    : QWidget(parent), mazeGenerator(ROWS, COLS), player1(mazeGenerator.getNode(0, 0), 1), player2(mazeGenerator.getNode(0, 0), 2) {
    // Verificar que CELL_SIZE, ROWS y COLS sean valores válidos
    if (CELL_SIZE <= 0) {
        throw std::invalid_argument("Error de Render: CELL_SIZE debe ser mayor que 0.");
    }
    if (ROWS <= 0) {
        throw std::invalid_argument("Error de Render: ROWS debe ser mayor que 0.");
    }
    if (COLS <= 0) {
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

    // Dibujar los jugadores
    drawPlayer(painter, player1, Qt::red);
    drawPlayer(painter, player2, Qt::blue);

    QWidget::paintEvent(event);  // Llamada al método base para manejar otros eventos de pintura
}

void MazeWidget::keyPressEvent(QKeyEvent *event) {
    int key = event->key();
    if (key == Qt::Key_W || key == Qt::Key_S || key == Qt::Key_A || key == Qt::Key_D ||
        key == Qt::Key_Up || key == Qt::Key_Down || key == Qt::Key_Left || key == Qt::Key_Right) {
        if (key == Qt::Key_W || key == Qt::Key_S || key == Qt::Key_A || key == Qt::Key_D) {
            updatePlayerPosition(player1, key);
        } else {
            updatePlayerPosition(player2, key);
        }
        update();
    }
}

void MazeWidget::drawPlayer(QPainter &painter, const Player &player, QColor color) {
    int x = player.getCurrentCell()->getX() * CELL_SIZE + CELL_SIZE / 4;
    int y = player.getCurrentCell()->getY() * CELL_SIZE + CELL_SIZE / 4;
    painter.setBrush(QBrush(color));
    painter.drawRect(x, y, CELL_SIZE / 2, CELL_SIZE / 2);
}

void MazeWidget::updatePlayerPosition(Player &player, int key) {
    switch (key) {
    case Qt::Key_W: player.moveUp(); break;
    case Qt::Key_S: player.moveDown(); break;
    case Qt::Key_A: player.moveLeft(); break;
    case Qt::Key_D: player.moveRight(); break;
    case Qt::Key_Up: player.moveUp(); break;
    case Qt::Key_Down: player.moveDown(); break;
    case Qt::Key_Left: player.moveLeft(); break;
    case Qt::Key_Right: player.moveRight(); break;
    }
}
