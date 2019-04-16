TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lpthread
INCLUDEPATH += ./include

SOURCES += \
    src/sysdata.cpp \
    src/main.cpp \
    src/wrap.cpp \
    src/buff.cpp \
    src/connection.cpp

HEADERS += \
    include/sysdata.h \
    include/wrap.h \
    include/buff.h \
    include/connection.h
