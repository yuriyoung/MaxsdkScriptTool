#include "Plugin.h"
#include "constants.h"
#include "dockablewindow.h"
#include "scripts_widget.h"
#include "preferences.h"
#include "propertydlg.h"

#include "qwin/qmfcapp.h"
#include "qwin/qwinwidget.h"

#include <QtCore>
#include <QtGui>
#include <QHBoxLayout>
#include <QPushButton>
#include <QGridLayout>
#include <QWidget>
#include <QStringList>
#include <QTranslator>
#include <QSettings>

static const qint64 ONE_KB = 1024.0;
static const qint64 ONE_MB = 1024*1024.0;

using namespace Plugin;

bool                     Plugin::g_exsits = false;
QTranslator             *Plugin::g_qtTrans = 0;
QTranslator             *Plugin::g_appTrans = 0;
ScriptsWidget           *Plugin::g_scriptsWidget = 0;
DockableWindow			*Plugin::g_dockableWindow = 0;
Preferences             *Plugin::g_preferences = 0;
QWinWidget              *Plugin::g_qWin = 0;
QSettings               *Plugin::g_settings = 0;

void Plugin::PluginStart()
{
    QString homePath =  QApplication::applicationDirPath();
    if (homePath.isEmpty())
    {
        g_settings = new QSettings(QSettings::IniFormat, QSettings::UserScope,
                                 QString(COMPANY), QString(PROGRAM));
    }
    else
    {
        QString fileName = rootPath() + "/ScriptsWin.ini";
        g_settings = new QSettings(fileName, QSettings::IniFormat);
    }

    g_qtTrans = new QTranslator;
    g_appTrans = new QTranslator;
    g_qtTrans->load(":/qt_zh_CN.qm");
    QMfcApp::installTranslator(g_qtTrans);
    g_appTrans->load( ":/scriptswin_zh_CN.qm");
    QMfcApp::installTranslator(g_appTrans);

    g_preferences = new Preferences;
    g_dockableWindow = DockableWindow::Create( IDC_WIN_TITEL );
    HWND hwnd = g_dockableWindow->GetHWND();
    g_qWin = new QWinWidget(hwnd);
    g_qWin->setObjectName("qwin");
    g_qWin->resize(200,680);
    g_qWin->setContentsMargins(0, 0, 0, 0);

    QGridLayout *gridlayout = new QGridLayout;
    gridlayout->setContentsMargins(0, 0, 0, 10);
	gridlayout->setSpacing(0);
    g_scriptsWidget = new ScriptsWidget(g_qWin);
    gridlayout->addWidget(g_scriptsWidget);
    g_qWin->setLayout(gridlayout);

    g_dockableWindow->SetWidget(g_qWin);
    g_dockableWindow->centerInMax();

    if(g_preferences->m_dockLocation == 2)
        g_scriptsWidget->dockLeft();
    else if(g_preferences->m_dockLocation == 3)
        g_scriptsWidget->dockRight();
    else
        g_dockableWindow->ResizeFrameToContent();

    setTheme();
    Plugin::g_exsits = true;
}

void Plugin::PluginEnd()
{
    delete g_appTrans;
    delete g_qtTrans;

    delete g_preferences;
    delete g_settings;

    g_scriptsWidget->deleteLater();
    g_qWin->deleteLater();

    HWND hwnd = g_dockableWindow->GetHWND();
    ::SendMessage(hwnd, WM_DESTROY, NULL, NULL);
}

QString Plugin::rootPath()
{
    const char *rootPath = ::GetCOREInterface()->GetDir(APP_MAX_SYS_ROOT_DIR);
    QString root = QString(rootPath);
    QString pluginpath = root + "/plugins/SouthgisMax";

    QDir dir(pluginpath);
    if( !dir.exists() )
    {
        dir.mkdir(pluginpath);
    }

    return pluginpath;
}

QString Plugin::dataPath()
{
    const char *rootPath = ::GetCOREInterface()->GetDir(APP_MAX_SYS_ROOT_DIR);
    QString root = QString(rootPath);
    QString datapath = root + "/plugins/SouthgisMax/data";

    QDir dir(datapath);
    if( !dir.exists() )
    {
        dir.mkdir(datapath);
    }

    return datapath;
}

QString Plugin::formatSize(qint64 size)
{
    QString str = QString("%1 B").arg(size);
    int base = 1024;
    qint64 mult = 1;
    for (int i = 0; i < 4; i++)
    {
        qint64 div = size / mult;
        if (div < base)
        {
            double val = (double)size / mult;
            switch (i)
            {
                case 1: str = QString("%1Kb").arg(val, 0, 'f', 1); break;
                case 2: str = QString("%1Mb").arg(val, 0, 'f', 1); break;
                case 3: str = QString("%1Gb").arg(val, 0, 'f', 1); break;
                default: break;
            }
            break;
        }
        mult *= base;
    }
    return str;
}

extern "C" __declspec(dllexport) const bool showScriptsWin(HWND hParent, const QString &code)
{
    if( !hParent || code != CODE)
        return false;

    if( Plugin::g_exsits )
        Plugin::g_dockableWindow->show();
    else
        Plugin::PluginStart();

    return true;
}

extern "C" __declspec(dllexport) const bool closeScriptsWin(HWND /*hParent*/, const QString &/*code*/)
{
    Plugin::PluginEnd();

    return true;
}

void Plugin::setTheme()
{
    int num = g_preferences->themeNumber();
    QString qssFile = g_preferences->themeFiles().at(num);

    QFile qss(qssFile);
    if( !qss.open(QFile::ReadOnly) )
    {
        QMessageBox::information(g_scriptsWidget, IDC_WIN_TITEL,
                                QObject::tr("Set theme failed: Theme file"
                                "is missing or read error: %1").arg(qss.fileName()), QMessageBox::Ok);
        return;
    }
    QByteArray buff = qss.readAll();
    g_scriptsWidget->setStyleSheet(buff);

    qss.close();

    if( num == 0 )
    {
        Plugin::g_qWin->setStyleSheet("background-color: rgb(42, 42, 42);"
                                        "color: rgb(255, 255, 255);");
    }
    else if( num == 1 )
    {
        Plugin::g_qWin->setStyleSheet("background-color: rgb(197, 197, 197);"
                                        "color: rgb(0, 0, 0);");
    }
    else if( num == 2 )
    {
        Plugin::g_qWin->setStyleSheet("background-color: rgb(0, 85, 127);"
                                        "color: rgb(255, 255, 255);");
    }
    else if( num == 3 )
    {
        Plugin::g_qWin->setStyleSheet("background-color: rgb(0, 59, 0);"
                                        "color: rgb(255, 255, 255);");
    }
    else if( num == 4 )
    {
        Plugin::g_qWin->setStyleSheet("background-color: rgb(180, 90, 147);"
                                        "color: rgb(255, 255, 255);");
    }
    else if( num == 5 )
    {
        Plugin::g_qWin->setStyleSheet("background-color: rgb(170, 170, 127);"
                                        "color: rgb(255, 255, 255);");
    }
}
