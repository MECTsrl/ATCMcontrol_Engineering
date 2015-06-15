/////////////////////////////////////////////////////////////////////////////
// MouseListener.h : Declares mouse listener interface and adapter class.
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
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Events/MouseListener.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

#pragma once

#ifndef __MOUSELISTENER_H__
#define __MOUSELISTENER_H__

#include <foundation\SflGuids.h>
#include <foundation\Events\EventListener.h>

namespace stingray {
namespace foundation {

///////////////////////////////////////////////////////////////////////////
//@doc IMouseListener
//
//@class IMouseListener | Interface to handling mouse events. Mouse event
// listeners map various mouse events onto the virtual member functions for
// handling.
//
//@mfunc bool | IMouseListener | OnLButtonDown | Handle left mouse button down events.
//@rdesc TRUE if event is handled; otherwise FALSE.
//@parm UINT | nFlags | Flags indicating if certain virtual keys were pressed
// when the mouse event occurred.
//@parm POINT | pt | Point at which the mouse event occurred.
//
//@mfunc bool | IMouseListener | OnLButtonUp | Handle left mouse button up events.
//@rdesc TRUE if event is handled; otherwise FALSE.
//@parm UINT | nFlags | Flags indicating if certain virtual keys were pressed
// when the mouse event occurred.
//@parm POINT | pt | Point at which the mouse event occurred.
//
//@mfunc bool | IMouseListener | OnLButtonDblClk | Handle left mouse button double click events.
//@rdesc TRUE if event is handled; otherwise FALSE.
//@parm UINT | nFlags | Flags indicating if certain virtual keys were pressed
// when the mouse event occurred.
//@parm POINT | pt | Point at which the mouse event occurred.
//
//@mfunc bool | IMouseListener | OnRButtonDown | Handle right mouse button down events.
//@rdesc TRUE if event is handled; otherwise FALSE.
//@parm UINT | nFlags | Flags indicating if certain virtual keys were pressed
// when the mouse event occurred.
//@parm POINT | pt | Point at which the mouse event occurred.
//
//@mfunc bool | IMouseListener | OnRButtonUp | Handle right mouse button up events.
//@rdesc TRUE if event is handled; otherwise FALSE.
//@parm UINT | nFlags | Flags indicating if certain virtual keys were pressed
// when the mouse event occurred.
//@parm POINT | pt | Point at which the mouse event occurred.
//
//@mfunc bool | IMouseListener | OnRButtonDblClk | Handle right mouse button double click events.
//@rdesc TRUE if event is handled; otherwise FALSE.
//@parm UINT | nFlags | Flags indicating if certain virtual keys were pressed
// when the mouse event occurred.
//@parm POINT | pt | Point at which the mouse event occurred.
//
//@mfunc bool | IMouseListener | OnMouseMove | Handle mouse move events.
//@rdesc TRUE if event is handled; otherwise FALSE.
//@parm UINT | nFlags | Flags indicating if certain virtual keys were pressed
// when the mouse event occurred.
//@parm POINT | pt | Point at which the mouse event occurred.
//
//@base public | IEventListener
//
class __declspec(uuid("F6D393EC-87E9-474e-AD7F-81802A500DE9"))
				IMouseListener : public IEventListener
{
public:
	//@cmember
	/* Handle left mouse button down events. */
	virtual bool OnLButtonDown(UINT nFlags, POINT pt) = 0;
	//@cmember
	/* Handle left mouse button up events. */
	virtual bool OnLButtonUp(UINT nFlags, POINT pt) = 0;
	//@cmember
	/* Handle left mouse button double click events. */
	virtual bool OnLButtonDblClk(UINT nFlags, POINT pt) = 0;
	//@cmember
	/* Handle right mouse button down events. */
	virtual bool OnRButtonDown(UINT nFlags, POINT pt) = 0;
	//@cmember
	/* Handle right mouse button up events. */
	virtual bool OnRButtonUp(UINT nFlags, POINT pt) = 0;
	//@cmember
	/* Handle right mouse button double click events. */
	virtual bool OnRButtonDblClk(UINT nFlags, POINT pt) = 0;
	//@cmember
	/* Handle mouse move events. */
	virtual bool OnMouseMove(UINT nFlags, POINT pt) = 0;
	//@cmember
	/* Handles mouse set cursor events. */
	virtual bool OnSetCursor( HWND hWnd, UINT nHitTest, UINT message ) = 0;
};

///////////////////////////////////////////////////////////////////////////
//@doc CMouseAdapter
//
//@class CMouseAdapter | The mouse adapter class supplies a default
// implementation of the <c IMouseListener> interface. The
// <mf CMouseAdapter::HandleEvent> function deciphers the event and maps
// it to a member function for handling. This class supplies a default
// implementation for all handler messages in the IMouseListener interface
// that always return FALSE. Mouse listeners can be derived from this class
// that override and implement only the handlers they are interested in.
//
//@base public | CEventListenerBase
//
class CMouseAdapter : public CEventListenerBase<IMouseListener>
{
// Operations
public:

	//@cmember
	/* Handle left mouse button down events. */
	virtual bool OnLButtonDown(UINT nFlags, POINT pt);

	//@cmember
	/* Handle left mouse button up events. */
	virtual bool OnLButtonUp(UINT nFlags, POINT pt);

	//@cmember
	/* Handle left mouse button double click events. */
	virtual bool OnLButtonDblClk(UINT nFlags, POINT pt);

	//@cmember
	/* Handle right mouse button down events. */
	virtual bool OnRButtonDown(UINT nFlags, POINT pt);

	//@cmember
	/* Handle right mouse button up events. */
	virtual bool OnRButtonUp(UINT nFlags, POINT pt);

	//@cmember
	/* Handle right mouse button double click events. */
	virtual bool OnRButtonDblClk(UINT nFlags, POINT pt);

	//@cmember
	/* Handle mouse move events. */
	virtual bool OnMouseMove(UINT nFlags, POINT pt);

	//@cmember
	/* Handles mouse set cursor events. */
	virtual bool OnSetCursor( HWND hWnd, UINT nHitTest, UINT message );
};

};	// namespace stingray::foundation
};  // namespace stingray

#endif  // #ifndef __MOUSELISTENER_H__
