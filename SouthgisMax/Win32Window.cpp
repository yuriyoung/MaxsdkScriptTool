#include "Win32Window.h"

Win32Window::Win32Window(HWND hParent):
	m_hParent(hParent),
	m_hWnd(NULL),
	m_hMenu(NULL),
	m_hInstance(NULL),
    m_hIcon(NULL),
    m_widget(NULL)
{
}

Win32Window::~Win32Window()
{
	if(m_hWnd)
	{
		destroy();
	}

    delete m_widget;
}

HWND Win32Window::createEx( HINSTANCE hInst,DWORD dwExStyle,LPCTSTR lpText,DWORD dwStyle,LPRECT lprcWnd,HWND hParent,int nMenuID,int nIconID )
{
	if(m_hWnd)
	{
		return m_hWnd;
	}

	m_hInstance = hInst;
	int x,y,nWidth,nHeight;

	if(lprcWnd == NULL)
	{
		x=y=nWidth=nHeight=CW_USEDEFAULT;
	}
	else
	{
		x=lprcWnd->left;
		y=lprcWnd->top;
		nWidth=lprcWnd->right-lprcWnd->left;
		nHeight=lprcWnd->bottom-lprcWnd->top;
	}

	if(nMenuID > 0)
	{
		m_hMenu=LoadMenu(m_hInstance,MAKEINTRESOURCE(nMenuID));
	}
	if(nIconID > 0)
	{
		m_hIcon=LoadIcon(m_hInstance,MAKEINTRESOURCE(nIconID));
	}
    CheckClass("Win32Window");
    m_hWnd=CreateWindowEx(dwExStyle, "Win32Window",lpText,dwStyle,x,y,nWidth,nHeight,hParent,m_hMenu,
							m_hInstance,this);

	return m_hWnd;
}

HWND Win32Window::create( LPCTSTR windowName, DWORD dwStyle,int x,int y,int nWidth,int nHeight,HWND hParent )
{
    RECT rc={x, y, x+nWidth, y+nHeight};
    return createEx(NULL, 0, windowName, dwStyle, &rc, hParent, 0, 0);
}

void Win32Window::create( LPCTSTR windowName)
{
    m_windowName = windowName;
    m_hWnd = create( windowName, WS_CAPTION|WS_BORDER| /*WS_MAXIMIZEBOX|*/ WS_MINIMIZEBOX
                | WS_SYSMENU, 0, 0, 460, 340, m_hParent);
}

void Win32Window::destroy()
{
	if(m_hWnd)
    {
        delete m_widget;
		DestroyWindow(m_hWnd);
		m_hWnd=NULL;
	}
}

HWND Win32Window::getWnd()
{
    return m_hWnd;
}

bool Win32Window::hasWidget()
{
    return getWidget() != NULL;
}

QWidget *Win32Window::getWidget()
{
    return m_widget;
}

void Win32Window::setWidget(QWidget *w)
{
    delete(this->m_widget);
    this->m_widget = w;
    if (w == NULL) return;
        w->move(0, 0);
        w->show();
}

void Win32Window::resizeFrameToContent()
{
    RECT r = {0};
    if (m_widget == NULL) return;
    GetWindowRect(m_hWnd, &r);
    MoveWindow( m_hWnd, r.left, r.top, m_widget->width(), m_widget->height(), TRUE );
}

void Win32Window::resizeContentToFrame()
{
    if (m_widget == NULL) return;
    RECT r = {0};
    GetWindowRect(m_hWnd, &r);
//    GetClientRect(m_hWnd, &r);
    int width = r.right - r.left;
    int height = r.bottom - r.top;
    m_widget->resize(width, height);
}

bool Win32Window::show()
{
	if (m_hWnd == NULL) return FALSE;

	// 居中显示
	RECT rcParent;
    ::GetClientRect(m_hParent, &rcParent);
	POINT ptParentInScreen;
	ptParentInScreen.x = rcParent.left;
	ptParentInScreen.y = rcParent.top;
    ::ClientToScreen(m_hParent, (LPPOINT)&ptParentInScreen);
    QRect rect;
    rect = m_widget->geometry();
    int x = ptParentInScreen.x + (rcParent.right - rcParent.left - (rect.right() - rect.left())) / 2;
    int y = ptParentInScreen.y + (rcParent.bottom - rcParent.top - (rect.bottom() - rect.top())) / 2;
    MoveWindow(m_hWnd, x, y, m_widget->width(), m_widget->height(), TRUE);

    //UpdateWindow(m_hWnd);
	
    BOOL ok = ShowWindow(m_hWnd, SW_SHOW);
    return ok;
}

bool Win32Window::hide()
{
    if (m_hWnd == NULL) return FALSE;

    return ShowWindow(m_hWnd, SW_HIDE);
}

BOOL Win32Window::PreProcessMsg( HWND hWnd,UINT nMsg,WPARAM wParam,LPARAM lParam,LRESULT *pResult )
{
	//TODO: your process code here

	 return true;
}

LRESULT Win32Window::OnCreate( HWND hWnd )
{
// 	m_hMenu=LoadMenu(this->m_hInstance,MAKEINTRESOURCE(IDR_TRAYICON));
// 	m_hIcon=LoadIcon(this->m_hInstance,MAKEINTRESOURCE(IDI_MAIN));

	return 0;
}

BOOL Win32Window::OnDestroy()
{
	destroy();
	//PostQuitMessage(0); // 关闭父窗口(退出程序)

	return true;
}

BOOL Win32Window::OnCommand( int nCmdID )
{
	//WM_COMMAND process hander
	switch( nCmdID )
	{
	case IDCANCEL:
		PostQuitMessage( 0 );
		break;
	}

	return true;
}

BOOL Win32Window::OnClose()
{
	::SendMessage(this->m_hWnd, WM_DESTROY, 0, 0);
	return true;
}

BOOL Win32Window::CheckClass( LPCTSTR lpClassName )
{
	WNDCLASS ws={0};
	if(!GetClassInfo(m_hInstance,lpClassName,&ws))
	{
		ws.hIcon=m_hIcon;
		ws.hbrBackground=(HBRUSH)COLOR_APPWORKSPACE;
		ws.lpfnWndProc= WndProc;
		ws.lpszClassName=lpClassName;
		return RegisterClass(&ws);
	}
	return true;
}

LRESULT CALLBACK Win32Window::WndProc( HWND hWnd,UINT nMsg,WPARAM wParam,LPARAM lParam )
{
	LRESULT lResult=0;
	BOOL bCallDefault=true;

    Win32Window *pWnd=(Win32Window *)GetProp(hWnd, "Win32Window");
	if(pWnd==NULL)
	{
		if(nMsg!=WM_CREATE)
		{
			return DefWindowProc(hWnd,nMsg,wParam,lParam);
		}
		else
		{
			LPCREATESTRUCT pSt = (LPCREATESTRUCT)lParam;
			pWnd = (Win32Window *)pSt->lpCreateParams;
            SetProp(hWnd, "Win32Window",pWnd);
		}
	}

	if(!pWnd->PreProcessMsg(hWnd,nMsg,wParam,lParam,&lResult))
	{
		return lResult;
	}

	switch(nMsg)
	{
	case WM_CREATE:
		{
			pWnd->m_hWnd=hWnd;
			lResult=pWnd->OnCreate(hWnd);
			bCallDefault=false;
			break;
		}
	case WM_DESTROY:
		bCallDefault = pWnd->OnDestroy();
		break;
	case WM_COMMAND:
		bCallDefault = pWnd->OnCommand(LOWORD(wParam));
		break;
	case WM_MOUSEMOVE:
		break;
	case WM_CLOSE:
		pWnd->OnClose();
		bCallDefault = false;
		break;
    case WM_SIZING:
        pWnd->resizeContentToFrame();
	default:
		break;
	}

	if(bCallDefault)
	{
		return DefWindowProc(hWnd,nMsg,wParam,lParam);
	}

	return lResult;
}
