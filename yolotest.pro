#-------------------------------------------------
#
# Project created by QtCreator 2019-09-05T10:39:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = yolotest
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS OPENCV NOMINMAX GPU CUDNN

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

INCLUDEPATH += $$_PRO_FILE_PWD_/include \

LIBS += -L$$_PRO_FILE_PWD_/lib \
     -lopencv_world340 \
     -lyolo_cpp_dll \

SOURCES += \
        canvas.cpp \
        main.cpp \
        dialog.cpp \
        utils.cpp \
        yolodetector.cpp

HEADERS += \
        canvas.h \
        dialog.h \
        global.h \
        yolodetector.h

FORMS += \
        dialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
