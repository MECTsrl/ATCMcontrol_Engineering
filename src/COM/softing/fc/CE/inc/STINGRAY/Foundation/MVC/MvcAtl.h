/////////////////////////////////////////////////////////////////////////////
// MvcAtl.h : SFL Model-View-Controller architecture 
//
// Stingray Software Foundation Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Jeff Boenig
// Description:  SFL MVC: ATL bridge
//

#pragma once

#ifndef __MVCATL_H__
#define __MVCATL_H__

#include <Foundation\Mvc\MvcComponent.h>
#include <Foundation\Events\EventRouterMap.h>
#include <Foundation\Events\EventFactory.h>
#include <Foundation\StructuredGraphics\VisualHostImpl.h>
#include <Foundation\Graphics\GCCore.h>

namespace stingray {
namespace foundation {

/////////////////////////////////////////////////////////////////////
//@doc CMvcAtlWndViewport
//
//@class CMvcAtlWndViewport | Mixes a window class with an MVC
// viewport and handles WM_SIZE messages by setting the viewport size.
//
//@tcarg class | _Viewport | Viewport class to derive from
//@tcarg class | _Base | ATL window class to derive from
//
//@base public | _Base
//@base public | CEventRouterMapWrapper
//@base public | IVisualWindow

template <typename _Viewport, typename _Base>
class CMvcAtlWndViewport : 
	public _Base,
    public CEventRouterMapWrapper<_Viewport>,
	public IVisualWindow
{
public:
	typedef CMvcAtlWndViewport<_Viewport, _Base> _thisClass;
	typedef _Base _windowBase;
	typedef CEventRouterMapWrapper<_Viewport> _viewportBase;

	CMvcAtlWndViewport(bool bHandlePaintMessage = true):
		m_bHandlePaint(bHandlePaintMessage)
	{}

protected:
	bool m_bHandlePaint;

public:
	// Message handling

	BEGIN_MSG_MAP(_thisClass)
		MESSAGE_HANDLER_DELEGATE(WM_CREATE, OnCreate)
		MESSAGE_HANDLER_DELEGATE(WM_SIZE, OnSize)
		MESSAGE_HANDLER_DELEGATE(WM_MOVE, OnMove)
        MESSAGE_HANDLER(WM_PAINT, OnPaint)
		CHAIN_MSG_MAP(_viewportBase)
	END_MSG_MAP()

	//@cmember,mfunc
	// Creates the viewport given a parent window and a rectangle.
	//@@syntax BOOL Create(HWND hWndParent, LPRECT rc)
	//@@syntax BOOL Create(HWND hWndParent, RECT& rcPos, LPCTSTR lpszCaption = 0, DWORD dwStyle = 0, DWORD dwExStyle = 0,	UINT nId = 0, LPVOID lpCreateParam = 0)
	//@@rdesc TRUE if successful, otherwise FALSE
	//@@parm Handle of parent window
	//@@parm Bounds of viewport to create
	//@@end
	/* Creates the viewport*/
	virtual BOOL Create(HWND hWndParent, LPRECT rc)
	{
		return Create(hWndParent, *rc);
	}

	BOOL Create(HWND hWndParent, RECT& rcPos, LPCTSTR lpszCaption = 0,
		DWORD dwStyle = 0, DWORD dwExStyle = 0,	UINT nId = 0, LPVOID lpCreateParam = 0)
	{
		HWND hwndThis = _windowBase::Create(hWndParent, rcPos, lpszCaption, dwStyle, dwExStyle, nId, lpCreateParam);
		return (hwndThis != 0);
	}

	//@cmember,mfunc
	// Return the window handle for the viewport.
	//@@rdesc Window handle
	//@@end
	/* Return the window handle*/
	virtual HWND GetWindowHandle()
	{
		return m_hWnd;
	}

	//@cmember,mfunc
	// Prepare device context for rendering.
	//@@rdesc void
	//@@parm CDC* | pDC | Pointer to a device context.
	//@@end
	/* Prepare device context for rendering*/
	virtual void OnPrepareDC(CDC* pDC)
	{
		// This overrides the base class OnPrepareDC and prevents
		// setting the viewport origin.  The viewport origin for
		// a windowed viewport should be 0,0.
		pDC;
	}

	//@cmember,mfunc
	// Cleanup device context after rendering.
	//@@rdesc void
	//@@parm CDC* | pDC | Pointer to a device context.
	//@@end
	/* Cleanup device context after rendering*/
	virtual void OnCleanupDC(CDC* pDC)
	{
		pDC;
	}

	//@cmember,mfunc
	// Set the width and height of the viewport.
	//@@syntax virtual CSize SetSize(int cx, int cy)
	//@@syntax CSize SetSize(const CSize& sz)
	//@@rdesc Previous size
	//@@parm Width to set viewport
	//@@parm Height to set viewport
	//@@comm This method is overridden to adjust the size of the window.
	//@@end
	/* Set the width and height of the viewport*/
	virtual CSize SetSize(int cx, int cy)
	{
		CRect rcBounds = GetBounds();
		rcBounds.right = rcBounds.left + cx;
		rcBounds.bottom = rcBounds.top + cy;

		BOOL bMoved;
		
		bMoved = _windowBase::MoveWindow(rcBounds.left,
		                                     rcBounds.top,
											 rcBounds.Width(),
											 rcBounds.Height(),
											 TRUE);

		return _Viewport::SetSize(cx,cy);
	}

	//@cmember
	/* Set the width and height of an object. */
	CSize SetSize(const CSize& sz)
	{
		return SetSize(sz.cx, sz.cy);
	}

	//@cmember
	/* Set the origin of an object. */
	virtual CPoint SetOrigin(int x, int y)
	{
		CRect rcBounds = GetBounds();
		rcBounds.left = x;
		rcBounds.top = y;

		BOOL bMoved;
		
		bMoved = _windowBase::MoveWindow(rcBounds.left,
		                                     rcBounds.top,
											 rcBounds.Width(),
											 rcBounds.Height(),
											 TRUE);

		return _Viewport::SetOrigin(x,y);
	}

	//@cmember
	/* Move the origin of an object by a given X,Y offset. */
	virtual CPoint MoveOrigin(int xOff,int yOff)
	{
		CRect rcBounds = GetBounds();

		BOOL bMoved;
		
		bMoved = _windowBase::MoveWindow(rcBounds.left + xOff,
		                                     rcBounds.top + yOff,
		                                     rcBounds.Width(),
		                                     rcBounds.Height(),
		                                     TRUE);

		return _Viewport::MoveOrigin(xOff,yOff);
	}

	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL&)
	{
		CreateController();
		return 0;
	}

	LRESULT OnMove(UINT, WPARAM, LPARAM /*lParam*/, BOOL&)
	{
//		int xPos = LOWORD(lParam);
//		int yPos = HIWORD(lParam);
//		_Viewport::SetOrigin(xPos, yPos);
		return 0;
	}

	LRESULT OnSize(UINT, WPARAM, LPARAM lParam, BOOL&)
	{
		int nWidth = LOWORD(lParam);
		int nHeight = HIWORD(lParam);
		_Viewport::SetSize(nWidth, nHeight);
		return 0;
	}

	LRESULT OnPaint(UINT, WPARAM, LPARAM, BOOL& bHandled)
    {
        // Paint the window
		if (m_bHandlePaint) {
#if !defined(_WTL_VER)
			CPaintGraphicsContext dcPaint(*this);
#else
			PAINTSTRUCT ps;
			CDC dcPaint(::BeginPaint(*this, &ps));
#endif

			OnPrepareDC(&dcPaint);
			Draw(&dcPaint);
			OnCleanupDC(&dcPaint);
#if defined(_WTL_VER)
			::EndPaint(*this, &ps);
#endif
		}
		else {
			// Do not swallow the message, this viewport may be based
			// in an existing control.
			bHandled = FALSE;
		}
		return 0;
    }

	BEGIN_GUID_MAP(_thisClass)
		GUID_CHAIN_ENTRY(_Viewport)
		GUID_ENTRY(IVisualWindow)
	END_GUID_MAP
};

//////////////////////////////////////////////////////////////////////
//@doc CMvcAtlComponent
//
//@class CMvcAtlComponent | This class combines a model, viewport,
// and controller into a single object and mixes in the ATL CMessageMap
// class so that components can be added to ATL message maps.
//
//@base public | CMvcComponent
//@base public | CMessageMap

template <typename _Model, typename _Viewport, typename _Controller, typename _EventFactory = CEventFactory>
class CMvcAtlComponent : 
	public CMvcComponent<_Model, _Viewport, _Controller>,
	public CMessageMap
{
public:
	virtual BOOL ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID = 0)
	{
		// Get rid of warnings
		hWnd;
		uMsg;
		wParam;
		lParam;
		lResult;
		dwMsgMapID;

		BOOL bHandled = FALSE;
		static _EventFactory eventFactory;

		IEventRouter* pIRouter = guid_cast<IEventRouter*>(this);

		if (pIRouter != NULL)
		{
			IEvent* pIEvent = eventFactory.CreateWindowsEvent(uMsg, wParam, lParam);
			if (pIEvent != NULL)
			{
				bHandled = pIRouter->RouteEvent(pIEvent);
				pIEvent->Release();
			}
		}

		return bHandled;
	}
};

};	// namespace stingray::foundation
};  // namespace stingray

#endif  // #ifndef __MVCATL_H__
