#include "registratewin.h"
#include "regwidget.h"

#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>

RegistrateWin::RegistrateWin(HWND hParent, QObject *parent):
    QObject(parent)
{
    m_win32window = new Win32Window( hParent );
    m_win32window->create( "зЂВс" );
    HWND hwnd = m_win32window->getWnd();
    QWinWidget* winWid = new QWinWidget( hwnd );
    winWid->resize(400, 300);
    winWid->setContentsMargins(0, 0, 0, 0);
    winWid->setStyleSheet("background-color: rgb(0, 85, 127);"
                          "color: rgb(245, 245, 245);");

    m_regWidget = new RegWidget(winWid);
    QGridLayout *gridlayout = new QGridLayout;
    gridlayout->setContentsMargins(0, 0, 10, 30);
    gridlayout->setSpacing(0);
    gridlayout->addWidget( m_regWidget );
    winWid->setLayout( gridlayout );
    m_win32window->setWidget(winWid);
    //m_win32window->resizeFrameToContent();

    QObject::connect( m_regWidget, SIGNAL(onClose()), this, SLOT(close()) );
}

RegistrateWin::~RegistrateWin()
{
    m_regWidget->deleteLater();
}

void RegistrateWin::show()
{
    m_win32window->show();
}

bool RegistrateWin::isRegistered()
{
    bool k = m_regWidget->isRegistered();

    return k;
}

bool RegistrateWin::isExpiration()
{
    bool ok = m_regWidget->isExpiration();

    return ok;
}

void RegistrateWin::close()
{
    m_win32window->destroy();
}
