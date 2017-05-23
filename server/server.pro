#-------------------------------------------------
#
# Project created by QtCreator 2017-05-21T09:28:03
#
#-------------------------------------------------

QT       += core gui\
            network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = server
TEMPLATE = app


SOURCES += main.cpp\
        mainwidget.cpp \
    multiinputdialog.cpp \
    character.cpp \
    characterfac.cpp \
    processmanager.cpp \
    logicworker.cpp \
    client.cpp \
    gameprocess.cpp

HEADERS  += mainwidget.h \
    multiinputdialog.h \
    character.h \
    characterfac.h \
    processmanager.h \
    logicworker.h \
    client.h \
    gameprocess.h
