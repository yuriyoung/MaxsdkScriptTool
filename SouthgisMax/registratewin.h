#ifndef REGISTRATEWIN_H
#define REGISTRATEWIN_H
#include "Win32Window.h"

#include "qwin/qmfcapp.h"
#include "qwin/qwinhost.h"
#include "qwin/qwinwidget.h"

class RegWidget;

class RegistrateWin : public QObject
{
    Q_OBJECT
public:
    RegistrateWin( HWND hParent = 0, QObject *parent = 0);
    ~RegistrateWin();

    void show();

    bool isRegistered();
    bool isExpiration();

public slots:
    void close();

private:
    Win32Window *m_win32window;
    RegWidget   *m_regWidget;
};

#endif // REGISTRATEWIN_H
