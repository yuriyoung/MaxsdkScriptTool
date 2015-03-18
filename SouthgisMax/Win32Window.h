#pragma once
#include <qt_windows.h>
#include <QWidget>

class Win32Window
{
public:
	Win32Window(HWND hParent);
	virtual ~Win32Window();

    void create( LPCTSTR windowName);
	void destroy();
    bool show();
    bool hide();

    HWND getWnd();
    bool hasWidget();
    QWidget *getWidget();
    void setWidget( QWidget* w);

    void resizeFrameToContent();
    void resizeContentToFrame();

private:
	HWND createEx(HINSTANCE hInst,DWORD dwExStyle,LPCTSTR lpText,DWORD dwStyle,LPRECT lprcWnd,HWND hParent,int nMenuID,int nIconID);
    HWND create( LPCTSTR windowName,DWORD dwStyle,int x,int y,int nWidth,int nHeight,HWND hParent);

protected:
	virtual BOOL PreProcessMsg(HWND hWnd,UINT nMsg,WPARAM wParam,LPARAM lParam,LRESULT *pResult);
	virtual LRESULT OnCreate(HWND hWnd);
	virtual BOOL OnDestroy();
	virtual BOOL OnCommand(int nCmdID);
	virtual BOOL OnClose();

protected:
	BOOL CheckClass(LPCTSTR lpClassName);
	static LRESULT CALLBACK WndProc(HWND hWnd,UINT nMsg,WPARAM wParam,LPARAM lParam);

public:
    HWND        m_hWnd;
    HWND        m_hParent;
    QWidget*    m_widget;

private:
    HICON       m_hIcon;
    HMENU       m_hMenu;
    HINSTANCE   m_hInstance;
    LPCTSTR     m_windowName;
};

