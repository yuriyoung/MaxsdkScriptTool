#include "main_menu.h"
#include "qwin/qmfcapp.h"
#include "qwin/qwinwidget.h"

#include <QTextCodec>

extern ClassDesc2* GetYMainMenuDesc();

HINSTANCE hInstance;
static bool ownApplication = FALSE;

BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD dwReason, LPVOID /*lpvReserved*/ )
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
//        hInstance = hinstDLL;
//        DisableThreadLibraryCalls(hInstance);
        QMfcApp::pluginInstance(hinstDLL);
    }

    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);

    return TRUE;
}

extern "C" __declspec(dllexport) const TCHAR* LibDescription()
{
    return IDS_LIBDESCRIPTION;
}

extern "C" __declspec(dllexport) int LibNumberClasses()
{
    return 1;
}

extern "C" __declspec(dllexport) ClassDesc* LibClassDesc(int i)
{
    switch(i)
    {
        case 0: return GetYMainMenuDesc();
        default: return 0;
    }
}

extern "C" __declspec(dllexport) ULONG LibVersion()
{
    return VERSION_3DSMAX;
}

extern "C" __declspec(dllexport)int LibInitialize(void)
{
    return TRUE; // TODO: Perform initialization here.
}

extern "C" __declspec(dllexport) int LibShutdown(void)
{
    return TRUE;// TODO: Perform un-initialization here.
}

TCHAR *GetString(int id)
{
    static TCHAR buf[256];

    if (hInstance)
        return LoadString(hInstance, id, buf, sizeof(buf)) ? buf : NULL;
    return NULL;
}
