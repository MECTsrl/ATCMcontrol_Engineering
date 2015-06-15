////////////////////////////////////////////////////////////////////////////////
// TabBarController.h: interface for the SECTabBarController class.
//
// Stingray Software Extension Classes
// Copyright © 1999 Stingray Software Inc,
// All rights reserved.
//
// Authors:      Prakash Surendra and Jay Gopalakrishnan
// Description:  ShortcutBar - MVCController implementation
//

#ifndef __TABBAR_CNTRLR_H__
#define __TABBAR_CNTRLR_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Foundation\Mvc\MvcController.h"
#include "Foundation\events\mouselistener.h"
#include "Foundation\events\keyboardlistener.h"
#include "Foundation\events\windowlistener.h"

#include "BarViewport.h"

namespace stingray {

// User-defined message for extending the ShortcutBar's context menu
#define WM_TABBAR_CNTXTMENU	(WM_APP+0x0101) 

// Forward declarations.
class SECTabBarPresModel;
class SECBarViewport;

class SECTabBarController  : public CMvcController<SECTabBarPresModel,SECBarViewport>, public CMouseAdapter, 
						public CWindowAdapter, public CKeyboardAdapter
{
public:
	SECTabBarController()
	{
		AddListener(static_cast<IWindowListener*>(this));
		AddListener(static_cast<IMouseListener*>(this));
		AddListener(static_cast<IKeyboardListener*>(this));

		m_nHitIndex = -1;
		m_pCurrEvent = NULL;
	}

	virtual ~SECTabBarController()	{}

protected:
// Message handlers
	virtual bool OnEraseBkgnd(HDC hDC)
	{
		if(hDC != NULL)
		{
			CGraphicsContext CGC;
			CGC.Attach(hDC);
			GetViewport()->EraseBkgndVP(&CGC);
			CGC.Detach();
		}
		else
			GetViewport()->EraseBkgndVP(NULL);
		return false;
	}

	virtual bool OnPaint(HDC hDC)
	{
		if(hDC != NULL)
		{
			CGraphicsContext CGC;
			CGC.Attach(hDC);
			GetViewport()->Draw(&CGC);
			CGC.Detach();
		}
		else
			GetViewport()->Draw(NULL);
		return false;
	}

	virtual bool OnSize(UINT nFlag, int cx, int cy)
	{
		GetViewport()->	SetSize(cx, cy);
		return false;
	}

	virtual bool OnLButtonDown(UINT nFlags, POINT pt)
	{
		UINT nHitTestCode;
		int nHitIndex;
		GetViewport()->HitTest(CHitTest::WMLBtnDown, pt, nHitTestCode, nHitIndex);

		if(nHitTestCode == CHitTest::htItem)
			m_nHitIndex = nHitIndex;
		else if(nHitTestCode == CHitTest::htClient)
		{
			SECTabBarBase* pTabBar = GetModel()->GetTabBarAt(nHitIndex);
			if(pTabBar->IsVisualComp())
				RouteEventtoClient(pTabBar);
		}
		return true;
	}

	virtual bool OnLButtonUp(UINT nFlags, POINT pt)
	{
		UINT nHitTestCode;
		int nHitIndex;
		GetViewport()->HitTest(CHitTest::WMLBtnUp, pt, nHitTestCode, nHitIndex);

		if(nHitTestCode == CHitTest::htItem && nHitIndex == m_nHitIndex)
			GetModel()->ActivateTabBar(m_nHitIndex);
		else if(nHitTestCode == CHitTest::htClient)
		{
			SECTabBarBase* pTabBar = GetModel()->GetTabBarAt(nHitIndex);
			if(pTabBar->IsVisualComp())
				RouteEventtoClient(pTabBar);
		}

		m_nHitIndex = -1;

		return true;
	}

	virtual bool OnRButtonUp(UINT nFlags, POINT pt)
	{
		if(!(GetModel()->GetTabBarStyle() & SEC_TABBAR_CNTXTMENU))
		return false;

		UINT nHitTestCode;
		int nHitIndex;
		GetViewport()->HitTest(CHitTest::WMRBtnUp, pt, nHitTestCode, nHitIndex);

		if(nHitTestCode == CHitTest::htClient || nHitTestCode == CHitTest::htItem)
		{
			HMENU hMenu = CreatePopupMenu();
			assert(hMenu != NULL);
			// If the handler returns true then display the popup menu. Else simply destroy the menu resource.
			if( ::SendMessage(GetModel()->GetNotifyWnd(), WM_TABBAR_CNTXTMENU, (WPARAM)nHitIndex, (LPARAM)&hMenu) )
			{
				::ClientToScreen(GetViewport()->GetHostImpl()->GetWindowHandle(), &pt);
				::TrackPopupMenu(hMenu, TPM_RIGHTBUTTON|TPM_LEFTALIGN, pt.x, pt.y, 0, GetModel()->GetNotifyWnd(), 0);
			}
			::DestroyMenu(hMenu);
		}

		return true;
	}

	virtual bool OnMouseMove(UINT nFlags, POINT pt)
	{
		UINT nHitTestCode;
		int nHitIndex;
		GetViewport()->HitTest(CHitTest::WMMouseMove, pt, nHitTestCode, nHitIndex);

		if(nHitTestCode == CHitTest::htClient)
		{
			SECTabBarBase* pTabBar = GetModel()->GetTabBarAt(nHitIndex);
			if(pTabBar->IsVisualComp())
				RouteEventtoClient(pTabBar);
		}

		return true;
	}

	virtual bool OnTimer(UINT nIDEvent)
	{
		GetViewport()->OnViewportTimer(nIDEvent);
		return true;
	}

	virtual bool OnSetCursor( HWND hWnd, UINT nHitTest, UINT message )
	{
		GetViewport()->OnViewportSetCursor(hWnd, nHitTest, message);
		return true;
	}	

public:
	// Ideally this temporary event cache should not be needed at this level. The current event should be cached and 
	// available from somewhere higher up in the events model. Till then...
	virtual bool RouteEvent(IEvent* pIEvent)
	{
		if(pIEvent != NULL)	
			m_pCurrEvent = pIEvent;

		return CMvcController<SECTabBarPresModel,SECBarViewport>::RouteEvent(pIEvent);
	}

protected:
	// MvcViewports that subscribe to the WTL events model will implement the IEventRouter interface.
	// Based on this premise, if this visual component is an MVC viewport, then route the current event
	// through the viewport
	void RouteEventtoClient(SECTabBarBase* pTabBar)
	{
		IEventRouter* pEventRouter;
		if(pTabBar->QueryGuid(__uuidof(IEventRouter), (void**)&pEventRouter))
			pEventRouter->RouteEvent(m_pCurrEvent);	
	}

protected:
	int m_nHitIndex;
	IEvent* m_pCurrEvent;	// Temporary cache for the current event.
};

};	// namespace stingray

#endif //__TABBAR_CNTRLR_H__

