/////////////////////////////////////////////////////////////////////////////
// WindowListener.h : Declares mouse listener interface and adapter class.
//
// Stingray Software Extension Classes
// Copyright (C) 1997 Stingray Software Inc,
// All rights reserved.
//
// This source code is only intended as a supplement to
// the Objective Toolkit Class Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding
// the Objective Toolkit product.
//
// Author:       Jeff Boenig and Bob Powell
// Description:  Framework independent event model.
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Events/WindowListener.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

#pragma once

#ifndef __WINDOWLISTENER_H__
#define __WINDOWLISTENER_H__

#include <foundation\SflGuids.h>
#include <foundation\Events\EventListener.h>

namespace stingray {
namespace foundation {

///////////////////////////////////////////////////////////////////////////
//@doc IWindowListener
//
//@class IWindowListener | Interface to handling mouse events. Window event
// listeners map various mouse events onto the virtual member functions for
// handling.
//
//
//@base public | IEventListener
//
class __declspec(uuid("A67C846D-0A0A-4b5e-8DC0-3DA18454F582"))
				IWindowListener : public IEventListener
{
public:
	//@cmember
	/* Handles window create events. */
	virtual bool OnCreate(LPCREATESTRUCT lpCreateStruct) = 0;
	//@cmember
	/* Handles window destroy events. */
	virtual bool OnDestroy() = 0;
	//@cmember
	/* Handles window move events. */
	virtual bool OnMove(int x, int y) = 0;
	//@cmember
	/* Handles window size events. */
	virtual bool OnSize(UINT nFlag, int cx, int cy) = 0;
	//@cmember
	/* Handles window erase background events. */
	virtual bool OnEraseBkgnd(HDC hDC) = 0;
	//@cmember
	/* Handles window paint events. */
	virtual bool OnPaint(HDC hDC) = 0;
	//@cmember
	/* Handles window position changing events. */
	virtual bool OnWindowPosChanging(LPWINDOWPOS lpWindowPos) = 0;
	//@cmember
	/* Handles window position changed events. */
	virtual bool OnWindowPosChanged(LPWINDOWPOS lpWindowPos) = 0;
	//@cmember
	/* Handles window timer events. */
	virtual bool OnTimer(UINT nIDTimer) = 0;
};

///////////////////////////////////////////////////////////////////////////
//@doc CWindowAdapter
//
//@class CWindowAdapter | The mouse adapter class supplies a default
// implementation of the <c IWindowListener> interface. The
// <mf CWindowAdapter::HandleEvent> function deciphers the event and maps
// it to a member function for handling. This class supplies a default
// implementation for all handler messages in the IWindowListener interface
// that always return FALSE. Window listeners can be derived from this class
// that override and implement only the handlers they are interested in.
//
//@base public | CEventListenerBase
//
class CWindowAdapter : public CEventListenerBase<IWindowListener>
{
// Operations
public:
	//@cmember
	/* Handles window create events. */
	virtual bool OnCreate(LPCREATESTRUCT lpCreateStruct);
	//@cmember
	/* Handles window destroy events. */
	virtual bool OnDestroy();
	//@cmember
	/* Handles window move events. */
	virtual bool OnMove(int x, int y);
	//@cmember
	/* Handles window size events. */
	virtual bool OnSize(UINT nFlag, int cx, int cy);
	//@cmember
	/* Handles window erase background events. */
	virtual bool OnEraseBkgnd(HDC hDC);
	//@cmember
	/* Handles window paint events. */
	virtual bool OnPaint(HDC hDC);
	//@cmember
	/* Handles window position changing events. */
	virtual bool OnWindowPosChanging(LPWINDOWPOS lpWindowPos);
	//@cmember
	/* Handles window position changed events. */
	virtual bool OnWindowPosChanged(LPWINDOWPOS lpWindowPos);
	//@cmember
	/* Handles window timer events. */
	virtual bool OnTimer(UINT nIDTimer);
};

};	// namespace stingray::foundation
};  // namespace stingray

#endif  // #ifndef __WINDOWLISTENER_H__
