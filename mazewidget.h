#ifndef MAZEWIDGET_H
#define MAZEWIDGET_H

#include <QWidget>
#include "MazeGenerator.h"
#include "Player.h"
#include "Game.h"

class MazeWidget : public QWidget {
    Q_OBJECT

public:
    explicit MazeWidget(QWidget *parent = nullptr);
    void handlePower(Player* player, Cell* cell, Cell::Power power);

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    MazeGenerator mazeGenerator;
    Player player1;
    Player player2;
    Cell* treasureCell;
    Game game;

    void drawMaze(QPainter &painter);
    void drawPowers(QPainter &painter);
    void drawPortals(QPainter &painter);
    void drawPlayer(QPainter &painter, const Player &player, QColor color);
    void drawTreasure(QPainter &painter);
    void drawCurrentPlayer(QPainter &painter);

    bool updatePlayerPosition(Player *player, int key);
    void play(int key);
    void placeTreasure();
    bool isStrategicPosition(int x, int y);
    void clearPower(Cell* cell);
};

#endif // MAZEWIDGET_H
