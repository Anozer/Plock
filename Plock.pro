#-------------------------------------------------
#
# Project created by QtCreator 2014-02-14T19:50:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Plock
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    options.cpp \
    setpassword.cpp \
    setfolder.cpp

HEADERS  += mainwindow.h \
    constants.h \
    options.h \
    setpassword.h \
    setfolder.h

FORMS    += mainwindow.ui \
    options.ui \
    setpassword.ui \
    setfolder.ui

CONFIG(release, debug|release) {
    #This is for release build
    DEFINES += QT_NO_DEBUG_OUTPUT
} else {
    #This is for debug build
}
