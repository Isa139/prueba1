QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# Include the Qt testlib module
QT += testlib

# Source files for your project
SOURCES += \
    cell.cpp \
    game.cpp \
    main.cpp \
    mainwindow.cpp \
    mazeGenerator.cpp \
    mazewidget.cpp \
    player.cpp \
    tests/test_main.cpp \
    tests/tst_cell.cpp \
    tests/tst_game.cpp \
    tests/tst_mazegenerator.cpp \
    tests/tst_player.cpp

# Header files for your project
HEADERS += \
    cell.h \
    constants.h \
    game.h \
    mainwindow.h \
    mazeGenerator.h \
    mazewidget.h \
    player.h

# UI forms for your project
FORMS += \
    mainwindow.ui

# Path to Google Test
GTEST_DIR = $$PWD/libs/googletest

# Include paths for Google Test
INCLUDEPATH += $$GTEST_DIR/include \
               $$GTEST_DIR

# Source files for Google Test
SOURCES += \
    $$GTEST_DIR/src/gtest-all.cc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
