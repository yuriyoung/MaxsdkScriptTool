#include "dynmenu_callback.h"
#include "constants.h"

#include "registratewin.h"

#include <maxscrpt/maxscrpt.h>
#include <time.h>
#include <imenuman.h> 
#include <imenus.h>

#include <QMessageBox>
#include <QDir>
#include <QGridLayout>

DynMenuCallback::DynMenuCallback(void)
{
}

DynMenuCallback::~DynMenuCallback(void)
{
    closeScriptsWindow();
}

void DynMenuCallback::MenuItemSelected( int itemId /*= 0 */ )
{
    HWND hMax = ::GetCOREInterface()->GetMAXHWnd();

	switch (itemId)
	{
	case IDC_YMENU_PLUGIN:		//插件集
		{
            showScriptsWindow();
		};
		break;
	case IDC_YMENU_CREATER:	//模型创造器
		{

		};
		break;
	case IDC_YMENU_MODIFIER://场景资源修改器
		{
			
		};
		break;
	case IDC_YMENU_EXPORTER:	//场景导出器
		{
			MessageBox( hMax, "导出工具", "导出", 0 );
		};
		break;

	case IDC_YMENU_IMPORTER:	//导入器
		{
			MessageBox( hMax, "导入工具", "导入", 0 );
		};
		break;
	case IDC_YMENU_TOOLBAR:	//工具栏
		{
			MessageBox( hMax, "显示工具栏", "工具栏", 0 );
		};
		break;
	case IDC_YMENU_SETTING:	//设置
		{
			MessageBox( hMax, "显示设置对话框", "设置", 0 );
		};
		break;
	case IDC_YMENU_HELP:	//帮助主题
		{
			MessageBox( hMax, "帮助主题", "帮助", 0 );
		};
		break;
    case IDC_YMENU_REG:	// 注册
        {
            RegistrateWin* regWin = new RegistrateWin(hMax);
            regWin->show();
        };
        break;
	case IDC_YMENU_ABOUT:	//关于我们
		{
			MessageBox( hMax, "关于我们", "关于", 0 );
		};
		break;

	default:break;
    }
}

bool DynMenuCallback::showScriptsWindow()
{
    HWND hMax = ::GetCOREInterface()->GetMAXHWnd();
    RegistrateWin* regWin = new RegistrateWin(hMax);

    if( regWin->isExpiration() )
    {
        // 已到期
        if( !regWin->isRegistered() )
        {
            MessageBox( hMax, "你使用的版本是未注册副本，请注册后使用。", "提示", 0 );
            regWin->show();
            return false;
        }
    }

    if( m_scriptsDll.isLoaded() )
    {
        dllScripts lib = (dllScripts)m_scriptsDll.resolve("showScriptsWin");
        if ( lib )
        {
            lib(hMax, CODE);
            return true;
        }
    }

    QString pluginpath = QDir::toNativeSeparators( QDir::currentPath() + "/plugins/SouthgisMax" );
    m_scriptsDll.setFileName( pluginpath + "/ScriptsWin.dll" );
    if(m_scriptsDll.load())
    {
        dllScripts lib = (dllScripts)m_scriptsDll.resolve("showScriptsWin");
        if ( lib )
        {
            lib(hMax, CODE);
            return true;
        }
        else
        {
            MessageBox( hMax, "resolve function failed.", "error", 0 );
        }
    }
    else
    {
        MessageBox( hMax, pluginpath.toLocal8Bit().data(), "library file dosen't exists", 0 );
    }
    return false;
}

bool DynMenuCallback::closeScriptsWindow()
{
    if( m_scriptsDll.isLoaded() )
    {
        HWND hMax = ::GetCOREInterface()->GetMAXHWnd();
        dllScripts lib = (dllScripts)m_scriptsDll.resolve("closeScriptsWin");
        if ( lib )
        {
            lib(hMax, CODE);
            return true;
        }
    }
    m_scriptsDll.unload();
    return false;
}

