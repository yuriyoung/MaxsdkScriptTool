#include "dockablewindow.h"

DockableWindow::DockableWindow( HWND hwndCuiFrame )
	: m_hwndCuiFrame(hwndCuiFrame), m_widget(NULL), m_frame(NULL) 
{
	m_frame = ::GetICUIFrame(m_hwndCuiFrame);
	m_frame->InstallMsgHandler(this);
}

DockableWindow::~DockableWindow()
{
	::ReleaseICUIFrame(m_frame);
	delete m_widget;
}

void DockableWindow::ResizeFrameToContent()
{
	RECT r = {0};
	if (m_widget == NULL) return;
	GetWindowRect(m_hwndCuiFrame, &r);
    MoveWindow( m_hwndCuiFrame, r.left, r.top, m_widget->width()+9, m_widget->height()+20, TRUE );
}

void DockableWindow::ResizeContentToFrame()
{
	if (m_widget == NULL) return;
    RECT r = {0};
//	GetWindowRect(m_hwndCuiFrame, &r);
    GetClientRect(m_hwndCuiFrame, &r);
    int width = r.right - r.left;
    int height = r.bottom - r.top;
    m_widget->resize(width, height);
}

int DockableWindow::GetWidth( int sizeType, int orient )
{
	switch (sizeType)
	{
		case CUI_MIN_SIZE: return 50;
		case CUI_MAX_SIZE: return 10000;
	}
	return m_widget->width(); 
}

int DockableWindow::GetHeight( int sizeType, int orient )
{
	switch (sizeType)
	{
		case CUI_MIN_SIZE: return 50;
		case CUI_MAX_SIZE: return 10000;
	}
	return m_widget->height(); 
}

DockableWindow* DockableWindow::Create( MCHAR* name, DockFlags pos /*= All*/, DWORD initialPos /*= 0*/, bool isDockedInitially /*= false*/, bool resizable /*= true*/, bool visible /*= true*/ )
{
    HWND hMax = ::GetCOREInterface()->GetMAXHWnd();
    HWND h = ::CreateCUIFrameWindow(hMax, name, 0, 0, 0, 0);
    if (!h) return NULL;
    DockableWindow* r = new DockableWindow(h);
	ICUIFrame* f = r->GetICUIFrame();
    DWORD flags = pos | CUI_FLOATABLE | CUI_VERT_DOCK;
    if (resizable) flags |= CUI_SM_HANDLES;
        f->SetPosType(flags &~CUI_HORIZ_DOCK );
	if (isDockedInitially) r->Dock(initialPos);
		f->Hide(visible ? FALSE : TRUE);

	return r;
}

int DockableWindow::ProcessMessage( UINT message, WPARAM wParam, LPARAM lParam )
{
	switch (message)
	{
		case CUI_POSDATA_MSG:
		{
			CUIPosData **cpd = (CUIPosData **)lParam;
			cpd[0] = this;
			return TRUE; 
		}
		case WM_SIZING: 
			ResizeContentToFrame();
			return FALSE;
	}
	return FALSE;
}

bool DockableWindow::HasWidget()
{
	return GetWidget() != NULL;
}

QWidget* DockableWindow::GetWidget()
{
	return m_widget;
}

void DockableWindow::SetWidget( QWidget* w )
{
	delete(this->m_widget);
	this->m_widget = w;
	if (w == NULL) return;
		w->move(0, 0);
	ResizeFrameToContent();
	w->show();
}

void DockableWindow::Dock( DWORD location )
{
	if (location & Top) GetCUIFrameMgr()->DockCUIWindow(m_hwndCuiFrame, Top);
	else if (location & Bottom) GetCUIFrameMgr()->DockCUIWindow(m_hwndCuiFrame, Bottom);
	else if (location & Left) GetCUIFrameMgr()->DockCUIWindow(m_hwndCuiFrame, Left);
	else if (location & Right) GetCUIFrameMgr()->DockCUIWindow(m_hwndCuiFrame, Right);
    else if (location & Floating) GetCUIFrameMgr()->DockCUIWindow(m_hwndCuiFrame, Floating);

    ResizeContentToFrame();
    GetCUIFrameMgr()->RecalcLayout(TRUE);
}

void DockableWindow::Float()
{
	GetCUIFrameMgr()->FloatCUIWindow(m_hwndCuiFrame);
    ResizeFrameToContent();
    GetCUIFrameMgr()->RecalcLayout(TRUE);
}

HWND DockableWindow::GetHWND()
{
	return m_hwndCuiFrame;
}

ICUIFrame* DockableWindow::GetICUIFrame()
{
    return m_frame;
}

void DockableWindow::centerInMax()
{
    if (m_widget == NULL) return;
    // ¾ÓÖÐÏÔÊ¾
    HWND hMax = ::GetCOREInterface()->GetMAXHWnd();
    RECT rcParent;
    ::GetClientRect(hMax, &rcParent);
    POINT ptParentInScreen;
    ptParentInScreen.x = rcParent.left;
    ptParentInScreen.y = rcParent.top;
    ::ClientToScreen(hMax, (LPPOINT)&ptParentInScreen);
    QRect rect;
    rect = m_widget->geometry();
    int x = ptParentInScreen.x + (rcParent.right - rcParent.left - (rect.right() - rect.left())) / 2;
    int y = ptParentInScreen.y + (rcParent.bottom - rcParent.top - (rect.bottom() - rect.top())) / 2;

    MoveWindow(m_hwndCuiFrame, x, y, m_widget->width(), m_widget->height(), TRUE);
}

void DockableWindow::show()
{
    if( m_frame )
        m_frame->Hide(false);
    else if( m_hwndCuiFrame )
        ShowWindow(m_hwndCuiFrame, SW_SHOW);
}

void DockableWindow::hide()
{
    if( m_frame )
        m_frame->Hide(true);
    else if( m_hwndCuiFrame )
        ShowWindow(m_hwndCuiFrame, SW_HIDE);
}
