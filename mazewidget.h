#ifndef MAZE_WIDGET_H
#define MAZE_WIDGET_H

#include <QWidget>
#include <QKeyEvent>
#include "MazeGenerator.h"
#include "Player.h"
#include "Game.h"

class MazeWidget : public QWidget {
    Q_OBJECT

public:
    MazeWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    void drawPlayer(QPainter &painter, const Player &player, QColor color);
    void drawTreasure(QPainter &painter);
    bool updatePlayerPosition(Player *player, int key);
    void placeTreasure();
    bool isStrategicPosition(int x, int y);
    void play(int key);  // Método para manejar los turnos

    static const int mazeWidth = 13;
    static const int mazeHeight = 13;
    static const int cellSize = 30;

    MazeGenerator mazeGenerator;
    Player player1;
    Player player2;
    Cell* treasureCell;
    Game game;  // Nuevo atributo para manejar el juego
};

#endif // MAZE_WIDGET_H
