#include "scripts_widget.h"
#include "qwin/qmfcapp.h"
#include "qwin/qwinwidget.h"

#include <windows.h>
#include <QTextCodec>
#include <QDir>
#include <QMessageBox>

BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD dwReason, LPVOID /*lpvReserved*/ )
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        QMfcApp::pluginInstance(hinstDLL);
    }
    QMfcApp::addLibraryPath("./plugins");
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);

    return TRUE;
}
