#-------------------------------------------------
#
# Project created by QtCreator 2017-05-20T10:20:42
#
#-------------------------------------------------

QT       += core gui\
            network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client
TEMPLATE = app


SOURCES += main.cpp\
    welcome.cpp \
    connect.cpp \
    mainwindow.cpp \
    play.cpp \
    inputbox.cpp

HEADERS  += mainwindow.h \
    welcome.h \
    connect.h \
    play.h \
    inputbox.h
