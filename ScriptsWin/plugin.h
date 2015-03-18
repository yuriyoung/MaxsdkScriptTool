#pragma once
#ifndef PLUGIN_H
#define PLUGIN_H

#ifdef Q_WS_WIN
#define SCRIPTS_EXPORT __declspec(dllexport)
#else
#define SCRIPTS_EXPORT
#endif

#include <QString>
#include <QVector>

class QWinWidget;
class ScriptsWidget;
class DockableWindow;
class Preferences;
class PropertyDlg;
class QTranslator;
class QSettings;

namespace Plugin
{
    extern QTranslator          *g_qtTrans;
    extern QTranslator          *g_appTrans;

    extern QWinWidget           *g_qWin;
    extern ScriptsWidget        *g_scriptsWidget;
    extern DockableWindow		*g_dockableWindow;
    extern Preferences          *g_preferences;
    extern QSettings            *g_settings;

    extern bool                  g_exsits;
	void PluginStart();
	void PluginEnd();

    void setTheme();

    QString rootPath();
    QString dataPath();
    QString formatSize(qint64 size);
}

#endif

