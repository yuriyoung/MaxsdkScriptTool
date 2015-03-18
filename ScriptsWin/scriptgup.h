#ifndef SCRIPTGUP_H
#define SCRIPTGUP_H

#include <Max.h>
#include <iparamb2.h>
#include <iparamm2.h>
#include <guplib.h>
#include <notify.h>

class ScriptGup : public GUP
{
public:
    ScriptGup();
    ~ScriptGup();
    // GUP Methods
    DWORD	Start( );
    void	Stop( );
    void    DeleteThis(){ delete this;}
//    static void scriptNotifyHandler(void *param, NotifyInfo *info);
};

#endif // SCRIPTGUP_H
