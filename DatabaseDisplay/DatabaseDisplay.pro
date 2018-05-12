#-------------------------------------------------
#
# Project created by QtCreator 2017-12-21T09:27:43
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DatabaseDisplay
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    applicationdata.cpp \
    settingface.cpp \
    loginface.cpp \
    connectdbfailed.cpp \
    sqlface.cpp \
    lineinputatsetting.cpp \
    multi_key_map.cpp \
    vicefunction.cpp \
    sqleditframe.cpp \
    vicefunctionitem.cpp \
    viceresult.cpp \
    versioninstructions.cpp

HEADERS += \
        mainwindow.h \
    applicationdata.h \
    settingface.h \
    loginface.h \
    connectdbfailed.h \
    sqlface.h \
    lineinputatsetting.h \
    multi_key_map.h \
    vicefunction.h \
    sqleditframe.h \
    vicefunctionitem.h \
    viceresult.h \
    versioninstructions.h

FORMS += \
        mainwindow.ui \
    settingface.ui \
    loginface.ui \
    connectdbfailed.ui \
    sqlface.ui \
    lineinputatsetting.ui \
    vicefunction.ui \
    sqleditframe.ui \
    vicefunctionitem.ui \
    viceresult.ui \
    versioninstructions.ui

RESOURCES += \
    resource.qrc

QT += axcontainer
