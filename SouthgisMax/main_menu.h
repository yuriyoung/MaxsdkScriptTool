#pragma once

//3dsmaxsdk preinclude 
#define STRING2(x) #x
#define STRING(x) STRING2(x)
#define TODO(x) __FILE__ "(" STRING(__LINE__) "): TODO: "x

#include <Max.h>
#include <istdplug.h>
#include <iparamb2.h>
#include <iparamm2.h>
//SIMPLE TYPE
#include <actiontable.h>
#include <notify.h>
#include <guplib.h>

#include "constants.h"
#include "dynmenu_callback.h"

#define YMAINMENU_CLASS_ID				Class_ID(0x40327edf, 0xa44487f4)

extern TCHAR *GetString(int id);
extern HINSTANCE hInstance;

class YMainMenu : public GUP
{
public:
    YMainMenu(void);
    ~YMainMenu(void);
    friend class DynMenuCallback;
public:
	IMenuItem*		m_iMenuItem;
	static HWND		hParams;

	// GUP Methods
	DWORD		Start			( );
	void		Stop			( );
	DWORD_PTR	Control			( DWORD parameter );

	// Loading/Saving
	IOResult Save(ISave *isave);
	IOResult Load(ILoad *iload);	

public:
    DynMenuCallback m_dmmCallback;

public:
    bool initYMainMenu();
	bool uninitManiMenu();

protected:
	static void	PostCreateUICallback(void* param, NotifyInfo* info);

};

static YMainMenu *ymainMenu;

class YMainMenuClassDesc : public ClassDesc2
{
public:
	virtual int IsPublic() 							{ return TRUE; }
    virtual void* Create(BOOL /*loading = FALSE*/) 	{ return new YMainMenu(); }
	virtual const TCHAR *	ClassName() 			{ return IDS_CLASS_NAME; }
	virtual SClass_ID SuperClassID() 				{ return GUP_CLASS_ID; }
    virtual Class_ID ClassID() 						{ return YMAINMENU_CLASS_ID; }
	virtual const TCHAR* Category() 				{ return IDS_CATEGORY; }

    virtual const TCHAR* InternalName() 			{ return IDS_CLASS_NAME; }	// returns fixed parsable name (scripter-visible name)
	virtual HINSTANCE HInstance() 					{ return hInstance; }					// returns owning module handle


};
