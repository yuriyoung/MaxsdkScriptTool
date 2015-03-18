#include "Plugin.h"
#include "constants.h"
#include "dockablewindow.h"
#include "scripts_widget.h"

#include "qwin/qmfcapp.h"
#include "qwin/qwinwidget.h"

#include <QtCore>
#include <QtGui>
#include <QHBoxLayout>
#include <QPushButton>
#include <QGridLayout>
#include <QWidget>

using namespace Plugin;

ScriptsWidget           *Plugin::g_scriptsWidget = 0;
DockableWindow			*Plugin::g_dockableWindow = 0;


void Plugin::PluginStart()
{
    g_dockableWindow = DockableWindow::Create( IDC_WIN_TITEL );
    HWND hwnd = g_dockableWindow->GetHWND();
    QWinWidget *qWin=new QWinWidget(hwnd);
    qWin->resize(220,680);
    qWin->setContentsMargins(0, 0, 0, 0);
	QGridLayout *gridlayout = new QGridLayout;
	gridlayout->setContentsMargins(0, 0, 0, 0);
	gridlayout->setSpacing(0);
    g_scriptsWidget = new ScriptsWidget( qWin );
    gridlayout->addWidget(g_scriptsWidget);
	qWin->setLayout(gridlayout);
    g_dockableWindow->SetWidget(qWin);
    g_dockableWindow->centerInMax();
    g_dockableWindow->ResizeFrameToContent();
//    g_dockableWindow->Dock(DockableWindow::Left | DockableWindow::Right);
//    g_dockableWindow->ResizeContentToFrame();
}

void Plugin::PluginEnd()
{
    delete g_scriptsWidget;
    delete g_dockableWindow;
}


QString Plugin::pluginPath()
{
    QString pluginpath = QDir::toNativeSeparators( QDir::currentPath() + "/plugins" );

    return pluginpath;
}

QString Plugin::dataPath()
{
    QString datapath = QDir::toNativeSeparators( QDir::currentPath() + "/plugins/data" );

    return datapath;
}
