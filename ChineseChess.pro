#-------------------------------------------------
#
# Project created by QtCreator 2020-06-29T15:54:22
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChineseChess
TEMPLATE = app


SOURCES += main.cpp\
    mainscene.cpp \
    buttonctl.cpp \
    chessboard.cpp \
    chesspieces.cpp \
    chessstep.cpp \
    gamescene.cpp \
    chessthink.cpp

HEADERS  += \
    common.h \
    mainscene.h \
    buttonctl.h \
    chesspieces.h \
    chessboard.h \
    chessstep.h \
    gamescene.h \
    chessthink.h

RESOURCES += \
    res.qrc

CONFIG += resources_big
