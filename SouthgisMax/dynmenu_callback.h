#pragma once
//#include "actiontable.h"
#include <Max.h>
#include <stdmat.h>
#include <imtl.h>
#include "constants.h"

#include <QLibrary>

class MainWidget;
class RegistrateWin;

typedef void (*dllScripts)(HWND, const QString&);

class DynMenuCallback : public DynamicMenuCallback
{
public:
    DynMenuCallback(void);
    ~DynMenuCallback(void);
	void virtual MenuItemSelected( int itemId = 0 );

	enum DynamicMenuFlags { kDisabled = 1 << 0, kChecked = 1 << 1, kSeparator = 1 << 2 }; 

    bool showScriptsWindow();
    bool closeScriptsWindow();

private:
    QLibrary        m_scriptsDll;
};
