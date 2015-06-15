//////////////////////////////////////////////////////////////////////
// ot_atlShortcutBar.h: interface for the ATL ShortcutBar components.
//
// Stingray Software Extension Classes
// Copyright © 1999 Stingray Software Inc,
// All rights reserved.
//
// Author:      Prakash Surendra
// Description:  SECATLShortcutBarWnd and SECATLShortcutBarHosted implementations
//

#ifndef __ATLSHORTCUTBAR_H__
#define __ATLSHORTCUTBAR_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if (_ATL_VER < 0x0300)		
	#error The SECATLShortcutBarWnd and SECATLShortcutBarHosted classes require ATL version 3.0 or higher. Stopping Compilation!
#endif // _ATL_VER < 0x0300

#define hAppInstance	_Module.GetModuleInstance()
#define hResInstance	_Module.GetResourceInstance()

#include "foundation\SflDef.h"
#include "Foundation\SflRes.h"
#include "foundation\graphics\gccore.h"
#include "Foundation\String\SflString.h"
#include "Foundation\events\event.h"
#include "Foundation\events\eventfactory.h"
#include "Foundation\events\eventroutermap.h"
#include "toolkit\ShortcutBar.h"

namespace stingray {

using namespace foundation;

/////////////////////////////////////////////////////////////////////////////
// SECATLShortcutBarWnd

/////////////////////////////
// AutoDuck tag block block for SECATLShortcutBarWnd

//@doc SECATLShortcutBarWnd
//@mfunc BOOL  | SECATLShortcutBarWnd | Create | Creates the shortcutbar control and initializes it with the styles set in dwStyle.
//@parm HWND | hWndParent | The parent window of the shortcutbar.
//@parm RECT& | rcPos | The initial position of the window.
//@parm DWORD | dwStyle | Specifies the initial styles for the shortcutbar.
//@parm UINT | nID | The ID of the child window.
//@rdesc If the creation is successful, TRUE is returned. Else FALSE.
//@xref<mf SECATLShortcutBarWnd::Create>
//@xref<c SECATLShortcutBarWnd>

// adinsert AutoDuck insertion point for SECATLShortcutBarWnd
//@doc SECATLShortcutBarWnd
//@class The SECATLShortcutBarWnd class implements a windowed version of the Shortcutbar for use with the ATL framework. SECATLShortcutBarWnd 
// derives from SECShortcutBarComp and thus inherits the native shortcutbar API exposed by that class. Additionally, SECATLShortcutBarWnd 
// implements only the Create() function for constructing the windowed control. The rest of the API is fully documented under SECShortcutBarComp. 
//@comm To use SECATLShortcutBarWnd/SECATLShortcutBarWnd, include the 'ot_atlshortcutbar.h' header in your ATL application.
// Please refer to the ATLSHORTCUTBAR sample for a demonstration of SECATLShortcutBarWnd.
//@base public | SECShortcutBarComp
//@base public | CWindowImpl<lt>SECATLShortcutBarWnd<gt>
//@base public | CEventRouterMap<lt>SECATLShortcutBarWnd<gt>
//@base public | IVisualWindow
//@xref <c SECShortcutBarComp>
//@xref <c SECATLShortcutBarWnd>
class SECATLShortcutBarWnd : public SECShortcutBarComp, public CEventRouterMap<SECATLShortcutBarWnd>,
						public CWindowImpl<SECATLShortcutBarWnd>, public IVisualWindow
{
typedef CEventRouterMap<SECATLShortcutBarWnd> routerBase;
typedef CWindowImpl<SECATLShortcutBarWnd>	   ClassWindowBase;

	BEGIN_MSG_MAP(SECATLShortcutBarWnd)
        CHAIN_MSG_MAP(routerBase)
    END_MSG_MAP()

public:
	// Constructors/Destructor
	SECATLShortcutBarWnd()	{};		
	virtual ~SECATLShortcutBarWnd()	{};

	// IQueryGuid
	virtual bool QueryGuid(REFGUID guid, void **ppvObj)
	{
		if( guid == __uuidof(IVisualWindow) )
		{
			*ppvObj = static_cast<IVisualWindow*>(this);
			return true;
		}
		return false;
	}

	// IVisualWindow implementation
	virtual HWND GetWindowHandle()
	{
		return m_hWnd;
	}

	//@cmember
	/* Creates the windowed shortcutbar control.*/
	virtual BOOL Create( HWND hWndParent, RECT& rcPos, DWORD dwStyle = WS_CHILD|WS_VISIBLE|SEC_TABBAR_BARCURSOR, UINT nID = 0 )
	{
		DWORD dwBarStyle = dwStyle & SEC_TABBAR_STYLES;
		dwStyle &= ~SEC_TABBAR_STYLES;
		SetHostImpl(static_cast<IVisualWindow*>(this));
		if( !ClassWindowBase::Create(hWndParent,rcPos,NULL,dwStyle,WS_EX_CLIENTEDGE,nID) )
		{
			ATLTRACE(_T("Failed to create the ShortcutBar Window.\n"));
			return FALSE;
		}
		CRect rcClient;
		GetClientRect(&rcClient);
		if( !SECShortcutBarComp::Create(rcClient) )
			return FALSE;
		SetBarStyle(dwBarStyle);
		// The ShortcutBar's parent window is made the default notification target
		SetNotifyWnd(::GetParent(m_hWnd));
		
		return TRUE;
	}
};

/////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////
// SECATLShortcutBarHosted:

/////////////////////////////
// AutoDuck tag block block for SECATLShortcutBarHosted

//@doc SECATLShortcutBarHosted
//@mfunc BOOL  | SECATLShortcutBarHosted | Create | Creates the shortcutbar component and initializes it with the host implementation
// (IVisualWindow*) provided by the parent window.
//@parm IVisualWindow* | pHost | An IVisualWindow interface pointer. The host window is expected to implement this SFL interface.
//@parm RECT& | rcPos | The initial position/size of the control with respect to it's parent.
//@parm DWORD | dwStyle | Specifies the initial styles for the shortcutbar.
//@rdesc If the creation is successful, TRUE is returned. Else FALSE.
//@xref<mf SECATLShortcutBarHosted::Create>
//@xref<c SECATLShortcutBarHosted>

// adinsert AutoDuck insertion point for SECATLShortcutBarHosted
//@doc SECATLShortcutBarHosted
//@class The SECATLShortcutBarHosted class implements a non-windowed version of the Shortcutbar for use with ATL. SECATLShortcutBarHosted 
// derives from SECShortcutBarComp and thus inherits the native shortcutbar API exposed by that class. Additionally, SECATLShortcutBarHosted 
// implements only the Create() function for constructing the control and providing it with a host interface pointer. The rest of the API 
// is fully documented under SECShortcutBarComp. 
//@comm To use SECATLShortcutBarWnd/SECATLShortcutBarWnd, include the 'ot_atlshortcutbar.h' header in your ATL application.
// Please refer to the ATLSHORTCUTBAR sample for a demonstration of SECATLShortcutBarHosted.
//@base public | SECShortcutBarComp
//@base public | CEventRouterMap<lt>SECATLShortcutBarHosted<gt>
//@xref <c SECShortcutBarComp>
//@xref <c SECATLShortcutBarWnd>
class SECATLShortcutBarHosted : public SECShortcutBarComp, public CEventRouterMap<SECATLShortcutBarHosted> 
{
typedef CEventRouterMap<SECATLShortcutBarHosted> routerBase;

	BEGIN_MSG_MAP(SECATLShortcutBarHosted)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
        CHAIN_MSG_MAP(routerBase)
    END_MSG_MAP()

public:
	SECATLShortcutBarHosted()	{}
	virtual ~SECATLShortcutBarHosted()	{}
	
	//@cmember
	/* Creates and initializes the non-windowed shortcutbar.*/
	virtual BOOL Create( IVisualWindow* pHost, RECT& rcPos, DWORD dwStyle = SEC_TABBAR_BARCURSOR)
	{
		SetHostImpl(pHost);		
		if( !SECShortcutBarComp::Create(rcPos) )
			return FALSE;
		SetBarStyle(dwStyle);
		// The IHost implementor is the default notification target for the Shortcut Bar
		SetNotifyWnd(pHost->GetWindowHandle());
		
		return TRUE;
	}

	CSize GetSize() const { return m_pViewport->GetSize(); }
	CSize SetSize(int cx, int cy) { return m_pViewport->SetSize(cx, cy); }
	CRect GetBounds() const { return m_pViewport->GetBounds(); }
	CPoint GetOrigin() const { return m_pViewport->GetOrigin();	}
	CPoint SetOrigin(int x, int y) { return m_pViewport->SetOrigin(x,y); }
	CPoint MoveOrigin(int xOff,int yOff) { return m_pViewport->MoveOrigin(xOff, yOff); }

// Message Handlers
	// A do-nothing handler for WM_SIZE, as all bound-rect changes should be made only through SECATLShortcutBarHosted's API.
	LRESULT OnSize(UINT, WPARAM, LPARAM, BOOL& bHandled)
	{
		bHandled = TRUE;
		return 0L;
	}

};

}; //namespace stingray

/////////////////////////////////////////////////////////////////////////////


#endif //__ATLSHORTCUTBAR_H__