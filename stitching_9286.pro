#-------------------------------------------------
#
# Project created by QtCreator 2017-04-05T14:33:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = stitching
TEMPLATE = app

CONFIG += C++11

android{

message("android");

 LIBS += $$PWD/libg2d.so
}

SOURCES += main.cpp \
    mainwindow.cpp \
    g2d_yuyv_bgra.cpp \
    looptable.cpp \
    v4l2_work_fourin.cpp \
    g2d_thread.cpp \
    caleloopuptabledata.cpp \
    initcamerathread.cpp \
    public.cpp

HEADERS  += mainwindow.h \
    g2d_yuyv_bgra.h \
    g2d.h \
    loopuptable.h \
    v4l2_work_fourin.h \
    g2d_thread.h \
    caleloopuptabledata.h \
    initcamerathread.h \
    public.h

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_EXTRA_LIBS = \
        $$PWD/libg2d.so
}

RESOURCES += \
    resource.qrc
