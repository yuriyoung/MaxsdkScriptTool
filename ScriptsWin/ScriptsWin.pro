#-------------------------------------------------
#
# Project created by QtCreator 2013-07-08T08:59:39
#
#-------------------------------------------------

QT       += core gui qaxserver qaxcontainer sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ScriptsWin
TEMPLATE = lib
TRANSLATIONS += scriptswin_zh_CN.ts scriptswin_zh_CN.qm
#CONFIG += staticlib

SOURCES += main.cpp\
    qwin/qwinwidget.cpp \
    qwin/qwinhost.cpp \
    qwin/qmfcapp.cpp \
    scriptgup.cpp \
    plugin.cpp \
    dockablewindow.cpp \
    table_widget.cpp \
    scripts_widget.cpp \
    sqlreader.cpp \
    sqlwriter.cpp \
    preferences.cpp \
    propertydlg.cpp \
    addscriptdlg.cpp

HEADERS  += \
    qwin/qwinwidget.h \
    qwin/qwinhost.h \
    qwin/qmfcapp.h \
    scriptgup.h \
    plugin.h \
    dockablewindow.h \
    constants.h \
    table_widget.h \
    scripts_widget.h \
    sqlreader.h \
    sqlwriter.h \
    preferences.h \
    propertydlg.h \
    addscriptdlg.h \

FORMS    += \
    scriptswidget.ui \
    propertydlg.ui \
    addscriptdlg.ui

#
# 3ds max lib
#
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

win32: LIBS += -L$$PWD/../../../../../3DsMaxSDK/Max9SDK/maxsdk/lib
INCLUDEPATH += $$PWD/../../../../../3DsMaxSDK/Max9SDK/maxsdk/include
DEPENDPATH += $$PWD/../../../../../3DsMaxSDK/Max9SDK/maxsdk/include

OTHER_FILES += cp.bat \
            resource.rc
RC_FILE = resource.rc

RESOURCES += \
    res.qrc
