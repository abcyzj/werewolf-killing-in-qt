#-------------------------------------------------
#
# Project created by QtCreator 2017-05-21T09:28:03
#
#-------------------------------------------------

QT       += core gui\
            network\
            testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = server
TEMPLATE = app


SOURCES += main.cpp\
        mainwidget.cpp \
    multiinputdialog.cpp \
    client.cpp \
    character.cpp \
    characterfac.cpp \
    process.cpp \
    processmanager.cpp \
    logicworker.cpp

HEADERS  += mainwidget.h \
    multiinputdialog.h \
    client.h \
    character.h \
    characterfac.h \
    process.h \
    processmanager.h \
    logicworker.h
