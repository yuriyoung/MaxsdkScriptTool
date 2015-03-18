#-------------------------------------------------
#
# Project created by QtCreator 2013-07-04T10:49:05
#
#-------------------------------------------------

#QT       += core gui qaxserver qaxcontainer
QT += core gui
#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SouthgisMax2009
TEMPLATE = lib

win32: LIBS += -L$$PWD/../../../../../3DsMaxSDK/Max2009SDK/maxsdk/lib
INCLUDEPATH += $$PWD/../../../../../3DsMaxSDK/Max2009SDK/maxsdk/include
DEPENDPATH += $$PWD/../../../../../3DsMaxSDK/Max2009SDK/maxsdk/include

SOURCES += main.cpp\
    main_menu.cpp \
    dynmenu_callback.cpp \
    registratewin.cpp \
    qwin/qwinwidget.cpp \
    qwin/qwinhost.cpp \
    qwin/qmfcapp.cpp \
    Win32Window.cpp \
    regwidget.cpp

HEADERS  += \
    main_menu.h \
    constants.h \
    dynmenu_callback.h \
    registratewin.h \
    qwin/qwinwidget.h \
    qwin/qwinhost.h \
    qwin/qmfcapp.h \
    Win32Window.h \
    ASProtect/asprotect.h \
    ASProtect/aspr_api.h \
    regwidget.h

LIBS += -luser32 \
    -lole32 \
    -loleaut32 \
    -lgdi32 \
    -lodbc32 \
    -lodbccp32 \
    -lcomctl32 \
    -lbmm \
    -lcore \
    -lmaxutil \
    -lmaxscrpt \
    -lgup \
    -lparamblk2

DEPENDPATH += windows.h
DEFINES -= UNICODE
win32:QMAKE_CXXFLAGS += -wd4100 -wd4819 -wd4099

OTHER_FILES += \
    cp.bat

FORMS += \
    regwidget.ui

win32: LIBS += -L$$PWD/ASProtect/ -laspr_ide

INCLUDEPATH += $$PWD/ASProtect
DEPENDPATH += $$PWD/ASProtect
