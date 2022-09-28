#-------------------------------------------------
#
# Project created by QtCreator 2018-12-17T09:50:47
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = StrategyGame
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

CONFIG += c++11

LIBS += -lprotobuf

INCLUDEPATH += src/include

SOURCES += \
        src/main.cpp \
        src/game.cpp \
        src/gamefield.cpp \
        src/markfield.cpp \
        src/networkclient.cpp \
        src/networkhost.cpp \
        src/networkmanager.cpp \
        src/ntwrkCmd.pb.cc \
        src/playerlist.cpp \
        src/solefield.cpp \
        src/unit.cpp

HEADERS += \
    src/include/game.h \
    src/include/gamefield.h \
    src/include/markfield.h \
    src/include/networkclient.h \
    src/include/networkhost.h \
    src/include/networkmanager.h \
    src/include/ntwrkCmd.pb.h \
    src/include/playerlist.h \
    src/include/solefield.h \
    src/include/unit.h

copydata.commands = $(COPY_DIR) $$PWD/misc/maps/ $$OUT_PWD
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    misc/res.qrc

DISTFILES += \
    misc/img/archer.png \
    misc/img/archer21.png \
    misc/img/castle.png \
    misc/img/castle1.png \
    misc/img/castle2.png \
    misc/img/catapult.png \
    misc/img/coin.png \
    misc/img/dragon.png \
    misc/img/dragon2.png \
    misc/img/elemental.png \
    misc/img/forest1.png \
    misc/img/forest2.png \
    misc/img/grass00.png \
    misc/img/grass03.png \
    misc/img/grass04.png \
    misc/img/grass27.png \
    misc/img/grass_26.png \
    misc/img/green.png \
    misc/img/knight.png \
    misc/img/mage.png \
    misc/img/militia.png \
    misc/img/red.png \
    misc/maps/rand.map \
    misc/ntwrkCmd.proto
