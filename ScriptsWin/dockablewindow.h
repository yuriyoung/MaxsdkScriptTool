#pragma once
#include <windows.h>
#include <max.h>
#include <maxapi.h>
#include <custcont.h>
#include <QWidget>

class DockableWindow : public CUIPosData, CUIFrameMsgHandler
{
public:
    DockableWindow(HWND hwndCuiFrame);
    virtual ~DockableWindow();

	HWND		m_hwndCuiFrame;
	ICUIFrame	*m_frame;
	QWidget		*m_widget;
	CUIPosData	m_posData;

	void ResizeFrameToContent();
	void ResizeContentToFrame();
	virtual int GetWidth(int sizeType, int orient);
	virtual int GetHeight(int sizeType, int orient);

	enum DockFlags
	{
		Top = 1 << 0,
		Bottom = 1 << 1,
		Left = 1 << 2,
		Right = 1 << 3,
		Horizontal = Left | Right,
		Vertical = Top | Bottom,
		All = Horizontal | Vertical,
        Floating = 1<<4
	};

    static DockableWindow* Create(MCHAR* name, DockFlags pos = All, DWORD initialPos = 0,
								bool isDockedInitially = false, bool resizable = true, bool visible = true);
	virtual int ProcessMessage (UINT message, WPARAM wParam, LPARAM lParam);
	bool HasWidget();
	QWidget* GetWidget();
	void SetWidget(QWidget* w);
	void Dock(DWORD location);
	void Float();
	HWND GetHWND();
	ICUIFrame* GetICUIFrame();
    void centerInMax();

    void show();
    void hide();
};
