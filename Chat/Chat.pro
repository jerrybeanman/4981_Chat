#-------------------------------------------------
#
# Project created by QtCreator 2016-03-02T21:44:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Chat
TEMPLATE = app

SOURCES += main.cpp\
        ui.cpp \
    Client.cpp \
    network_thread.cpp

QMAKE_CXXFLAGS += -std=c++11

HEADERS  += ui.h \
    Client.h \
    network_thread.h

FORMS    += ui.ui

RESOURCES += qdarkstyle/style.qrc
