QT       += core gui widgets

TARGET = Astar
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    graphscene.cpp \
    edge.cpp \
    startdialog.cpp \
    vertex.cpp

HEADERS  += mainwindow.h \
    graphscene.h \
    edge.h \
    startdialog.h \
    vertex.h

FORMS    += mainwindow.ui \
    startdialog.ui

RESOURCES += res.qrc

RC_FILE = res.rc
