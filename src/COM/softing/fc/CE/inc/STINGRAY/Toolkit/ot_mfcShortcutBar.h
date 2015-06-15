//////////////////////////////////////////////////////////////////////
// ot_mfcShortcutBar.h: interface for the MFC ShortcutBar components.
//
// Stingray Software Extension Classes
// Copyright © 1999 Stingray Software Inc,
// All rights reserved.
//
// Author:      Prakash Surendra
// Description:  SECMFCShortcutBarWnd and SECMFCShortcutBarHosted implementations
//

#ifndef __MFCSHORTCUTBAR_H__
#define __MFCSHORTCUTBAR_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ***********************************************************
// *
// *  OT optimized include file
// *
// ***********************************************************
#ifndef __SECOPT_H__
    #include "toolkit\secopt.h"
#endif

#define hAppInstance	AfxGetInstanceHandle()
#define hResInstance	AfxGetResourceHandle()

#include "Foundation\SflDef.h"
#include "Foundation\SflRes.h"
#include "Foundation\events\event.h"
#include "Foundation\events\eventfactory.h"
#include "Foundation\events\mfceventrouter.h"
#include "ShortcutBar.h"

namespace stingray	{

using namespace foundation;

/////////////////////////////////////////////////////////////////////////////
// SECMFCShortcutBarWnd

/////////////////////////////
// AutoDuck tag block block for SECMFCShortcutBarWnd

//@doc SECMFCShortcutBarWnd
//@mfunc BOOL  | SECMFCShortcutBarWnd | Create | Creates the shortcutbar control and initializes it with the styles set in dwStyle.
//@parm CWnd* | pWndParent | The parent window of the shortcutbar.
//@parm RECT& | rcPos | The initial position of the window.
//@parm DWORD | dwStyle | Specifies the initial styles for the shortcutbar.
//@parm UINT | nID | The ID of the child window.
//@rdesc If the creation is successful, TRUE is returned. Else FALSE.
//@xref<mf SECMFCShortcutBarHosted::Create>
//@xref<c SECMFCShortcutBarWnd>

// adinsert AutoDuck insertion point for SECMFCShortcutBarWnd
//@doc SECMFCShortcutBarWnd
//@class The SECMFCShortcutBarWnd class implements a windowed version of the Shortcutbar for use with MFC. SECMFCShortcutBarWnd 
// derives from SECShortcutBarComp and thus inherits the native shortcutbar API exposed by that class. Additionally, SECMFCShortcutBarWnd 
// implements only the Create() function for constructing the windowed control. The rest of the API is fully documented under SECShortcutBarComp. 
//@comm To use SECMFCShortcutBarWnd/SECMFCShortcutBarHosted, include the 'ot_mfcshortcutbar.h' header in your MFC application.
// Please refer to the SHORTCUTBAR sample for a demonstration of SECMFCShortcutBarWnd.
//@base public | SECShortcutBarComp
//@base public | CMFCEventRouter<lt>SECMFCShortcutBarWnd, CWnd<gt>
//@base public | IVisualWindow
//@xref <c SECShortcutBarComp>
//@xref <c SECMFCShortcutBarHosted>
class SECMFCShortcutBarWnd : public SECShortcutBarComp, public CMFCEventRouter<SECMFCShortcutBarWnd, CWnd>,
						  public IVisualWindow
{
typedef CMFCEventRouter<SECMFCShortcutBarWnd, CWnd> routerBase;

public:
	// Constructors/Destructor
	SECMFCShortcutBarWnd()	{};		
	virtual ~SECMFCShortcutBarWnd()
	{
		::DestroyWindow(m_hWnd);
		Detach();
	}

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
		return this->m_hWnd;
	}

	//@cmember
	/* Creates a windowed shortcutbar.*/
	virtual BOOL Create( CWnd* pWndParent, RECT& rcPos, DWORD dwStyle = WS_CHILD|WS_VISIBLE|SEC_TABBAR_BARCURSOR, UINT nID = 0 )
	{
		DWORD dwBarStyle = dwStyle & SEC_TABBAR_STYLES;
		dwStyle &= ~SEC_TABBAR_STYLES;
		SetHostImpl(static_cast<IVisualWindow*>(this));
		if( !CWnd::Create(NULL, NULL, dwStyle, rcPos, pWndParent, nID) )
		{
			TRACE(_T("Failed to create the ShortcutBar Window.\n"));
			return FALSE;
		}
		ModifyStyleEx(0, WS_EX_CLIENTEDGE);
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
// SECMFCShortcutBarHosted

/////////////////////////////
// AutoDuck tag block block for SECMFCShortcutBarHosted

//@doc SECMFCShortcutBarHosted
//@mfunc BOOL  | SECMFCShortcutBarHosted | Create | Creates the shortcutbar component and initializes it with the host implementation
// (IVisualWindow*) provided by the parent window.
//@parm IVisualWindow* | pHost | An IVisualWindow interface pointer. The host window is expected to implement this SFL interface.
//@parm RECT& | rcPos | The initial position/size of the control with respect to it's parent.
//@parm DWORD | dwStyle | Specifies the initial styles for the shortcutbar.
//@rdesc If the creation is successful, TRUE is returned. Else FALSE.
//@xref<mf SECMFCShortcutBarWnd::Create>
//@xref<c SECMFCShortcutBarHosted>

// adinsert AutoDuck insertion point for SECMFCShortcutBarHosted
//@doc SECMFCShortcutBarHosted
//@class The SECMFCShortcutBarHosted class implements a non-windowed version of the Shortcutbar for use with MFC. SECMFCShortcutBarHosted 
// derives from SECShortcutBarComp and thus inherits the native shortcutbar API exposed by that class. Additionally, SECMFCShortcutBarHosted 
// implements only the Create() function for constructing the control and providing it with a host interface pointer. The rest of the API 
// is fully documented under SECShortcutBarComp. 
//@comm To use SECMFCShortcutBarHosted/SECMFCShortcutBarHosted, include the 'ot_mfcshortcutbar.h' header in your MFC application.
// Please refer to the SHORTCUTBAR sample for a demonstration of SECMFCShortcutBarHosted.
//@base public | SECShortcutBarComp
//@base public | CMFCEventRouter<lt>SECMFCShortcutBarHosted<gt>
//@xref <c SECShortcutBarComp>
//@xref <c SECMFCShortcutBarWnd>
class SECMFCShortcutBarHosted : public SECShortcutBarComp, public CMFCEventRouter<SECMFCShortcutBarHosted> 
{
typedef CMFCEventRouter<SECMFCShortcutBarHosted> routerBase;

public:
	SECMFCShortcutBarHosted()	{}
	virtual ~SECMFCShortcutBarHosted()	{}
	
	//@cmember
	/* Creates and initializes the non-windowed shortcutbar.*/
	virtual BOOL Create( IVisualWindow* pHost, RECT& rcPos, DWORD dwStyle = SEC_TABBAR_BARCURSOR )
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

	// Message map overrides
	BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		// Ignore WM_SIZE messages as all bound-rect changes are made only through SECMFCShortcutBarHosted's API.
		if(message != WM_SIZE)
			return routerBase::OnWndMsg(message, wParam, lParam, pResult);
		return FALSE;
	}	
};

};	//namespace stingray

/////////////////////////////////////////////////////////////////////////////

#endif //__MFCSHORTCUTBAR_H__