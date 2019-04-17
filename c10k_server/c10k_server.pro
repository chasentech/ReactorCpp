TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lpthread
INCLUDEPATH += ./include

SOURCES += \
    src/main.cpp \
    src/wrap.cpp \
    src/myepollser.cpp \
    src/queuedata.cpp \
    src/mythread.cpp \
    src/savedata.cpp \
    src/sysdata.cpp \
    src/buff.cpp \
    src/threadpool.cpp

HEADERS += \
    include/wrap.h \
    include/myepollser.h \
    include/queuedata.h \
    include/mythread.h \
    include/savedata.h \
    include/sysdata.h \
    include/buff.h \
    include/threadpool.h
