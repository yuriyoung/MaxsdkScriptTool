
#include "main_menu.h"
#include <imenuman.h> 
#include <imenus.h> 

ClassDesc2* GetYMainMenuDesc()
{ 
    static YMainMenuClassDesc ymainMenuDesc;
    return &ymainMenuDesc;
}

YMainMenu::YMainMenu(void)
{
	m_iMenuItem = NULL;
}

YMainMenu::~YMainMenu(void)
{
    // MessageBox( 0, "test", "test", 0 );
    // 这里并没有被执行到.
    // 在3dsmax shutdown时,插件没能即时释放自己,就被3dsmax给中止了插件
	if (m_iMenuItem)
	{
		delete m_iMenuItem;
		m_iMenuItem = NULL;
	}
}

DWORD YMainMenu::Start()
{
	//QMfcApp::enterModalLoop();
	RegisterNotification(PostCreateUICallback, this, NOTIFY_SYSTEM_STARTUP);
    RegisterNotification(PostCreateUICallback, this, NOTIFY_SYSTEM_SHUTDOWN);
    // 不能使用NOTIFY_SYSTEM_SHUTDOWN2,否则会导致菜单卸载失败,二次打开3dsmax出现UI混乱而崩溃.

    return GUPRESULT_KEEP;
}

void YMainMenu::Stop()
{
	UnRegisterNotification(PostCreateUICallback, this, NOTIFY_SYSTEM_STARTUP);
    UnRegisterNotification(PostCreateUICallback, this, NOTIFY_SYSTEM_SHUTDOWN);
}

DWORD_PTR YMainMenu::Control( DWORD parameter )
{

	return 0;
}

IOResult YMainMenu::Save( ISave *isave )
{

	return IO_OK;
}

IOResult YMainMenu::Load( ILoad *iload )
{

	return IO_OK;
}

bool YMainMenu::initYMainMenu()
{
	Interface *core = GetCOREInterface();
	IMenuManager* menuMan = core->GetMenuManager();
    IMenu* YMainMenuBar = menuMan->GetMainMenuBar();

	bool exist = false;
    for ( int index = 0; index < YMainMenuBar->NumItems(); ++index )
	{
        if ( YMainMenuBar->GetItem( index )->GetTitle() == CStr( MENU_TITLE_MAIN ) )
		{
			exist = true;
			break;
		}
	}
	if ( !exist )
	{
		DynamicMenu dynamicMenu( &m_dmmCallback );
		dynamicMenu.BeginSubMenu( MENU_TITLE_MAIN );
		dynamicMenu.AddItem( 0, IDC_YMENU_PLUGIN, MENU_TITLE_PLUGIN );
        dynamicMenu.AddItem( m_dmmCallback.kDisabled, IDC_YMENU_CREATER, MENU_TITLE_CREATER );
        dynamicMenu.AddItem( m_dmmCallback.kDisabled, IDC_YMENU_SEPARATOR, "" );
        dynamicMenu.AddItem( m_dmmCallback.kDisabled, IDC_YMENU_MODIFIER, MENU_TITLE_MODIFIER);
        dynamicMenu.AddItem( m_dmmCallback.kDisabled, IDC_YMENU_EXPORTER, MENU_TITLE_EXPORTER );
        dynamicMenu.AddItem( m_dmmCallback.kDisabled, IDC_YMENU_IMPORTER, MENU_TITLE_IMPORTER );
		dynamicMenu.AddItem( m_dmmCallback.kSeparator, IDC_YMENU_SEPARATOR, "" );
        dynamicMenu.AddItem( m_dmmCallback.kDisabled, IDC_YMENU_TOOLBAR, MENU_TITLE_TOOLBAR );
        dynamicMenu.AddItem( m_dmmCallback.kDisabled, IDC_YMENU_SETTING, MENU_TITLE_SETTING );
		dynamicMenu.AddItem( m_dmmCallback.kSeparator, IDC_YMENU_SEPARATOR, "" );

        dynamicMenu.AddItem( 0, IDC_YMENU_REG, MENU_TITLE_REG );
		dynamicMenu.BeginSubMenu( MENU_TITLE_HELPER );
		dynamicMenu.AddItem( 0, IDC_YMENU_HELP, MENU_TITLE_HELP );
		dynamicMenu.AddItem( 0, IDC_YMENU_ABOUT, MENU_TITLE_ABOUT );
		dynamicMenu.EndSubMenu();

		dynamicMenu.EndSubMenu();

		m_iMenuItem = dynamicMenu.GetMenu()->GetItem( 0 );
		m_iMenuItem->EnableActions(TRUE);
        YMainMenuBar->AddItem( m_iMenuItem, YMainMenuBar->NumItems()-1);
		menuMan->UpdateMenuBar();

		exist = true;
		return true;
	}

	return false;
}

bool YMainMenu::uninitManiMenu()
{
    if( !m_dmmCallback.closeScriptsWindow() )
    {
        //MessageBox( 0, "close Window error", "error", 0 );
    }

	Interface *core = GetCOREInterface();
	IMenuManager* menuMan = core->GetMenuManager();
    IMenu* YMainMenuBar = menuMan->GetMainMenuBar();

    for ( int index = 0; index < YMainMenuBar->NumItems(); ++index )
	{
        CStr title = YMainMenuBar->GetItem( index )->GetTitle();
		if ( title == CStr(MENU_TITLE_MAIN) )
		{
            YMainMenuBar->RemoveItem(index);
			menuMan->UpdateMenuBar();

			return true;	
		}
	}


	return false;
}

void YMainMenu::PostCreateUICallback( void* param, NotifyInfo* info )
{
    YMainMenu * pThis = (YMainMenu*) param;
	DbgAssert(pThis != NULL);

	switch ( info->intcode )
	{
	case NOTIFY_SYSTEM_STARTUP:
		{
            pThis->initYMainMenu();
		};
		break;
    case NOTIFY_SYSTEM_SHUTDOWN:   // 在3dsmax关闭前通知插件释放自己的资源
		{
            if ( !pThis->uninitManiMenu() )
            {
                MessageBox( 0, "uninstall menu failed.", "uninstall", 0 );
            }
		};
		break;
	default:
		return;
	}
}
