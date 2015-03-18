#pragma once
#ifndef PLUGIN_H
#define PLUGIN_H

#include <QString>

class ScriptsWidget;
class DockableWindow;

namespace Plugin
{
    extern ScriptsWidget        *g_scriptsWidget;
    extern DockableWindow		*g_dockableWindow;

	void PluginStart();
	void PluginEnd();

    QString pluginPath();
    QString dataPath();
}

#endif

