#-------------------------------------------------
#
# Project created by QtCreator 2015-08-28T21:30:35
#
#-------------------------------------------------

QT       += core gui
QT       += xml

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ThraciaMapConverter-QT
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    base64/base64.cpp \
    ThraciaMap/snesmapchange.cpp \
    ThraciaMap/thraciamap.cpp \
    TMXReader/mapchange.cpp \
    TMXReader/tmxreader.cpp

HEADERS  += mainwindow.h \
    base64/base64.h \
    ThraciaMap/snesmapchange.h \
    ThraciaMap/thraciamap.h \
    TMXReader/mapchange.h \
    TMXReader/tmxreader.h

win32:INCLUDEPATH += $$[QT_INSTALL_HEADERS]/QtZlib

unix:LIBS += -lz

FORMS    += mainwindow.ui
