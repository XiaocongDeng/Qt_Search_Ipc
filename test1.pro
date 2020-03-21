#-------------------------------------------------
#
# Project created by QtCreator 2020-03-21T10:32:18
#
#-------------------------------------------------

QT       += core gui
QT          += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cjsonsrc/jsonmap.cpp \
    cjsonsrc/cJSON.c

HEADERS  += mainwindow.h \
    include/cJSON.h \
    include/common.hpp \
    include/jsonmap.hpp

FORMS    += mainwindow.ui
