#include <QApplication>
#include "MazeWidget.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MazeWidget w;
    w.show();

    return app.exec();
}
