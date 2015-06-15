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
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Compatibility/Common/Events/MouseListener.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

#ifndef __COMPAT_MOUSELISTENER_H__
#define __COMPAT_MOUSELISTENER_H__

#include "Foundation\Compatibility\Common\CmnGuids.h"

#ifndef __COMPAT_EVENTLISTENER_H__
#include "Foundation\Compatibility\Common\Events\EventListener.h"
#endif

namespace rw
{

///////////////////////////////////////////////////////////////////////////
//@doc IMouseListener
//
//@class IMouseListener | Interface to handling mouse events. Mouse event
// listeners map various mouse events onto the virtual member functions for
// handling.
//
//@mfunc BOOL | IMouseListener | OnLButtonDown | Handle left mouse button down events.
//@rdesc TRUE if event is handled; otherwise FALSE.
//@parm UINT | nFlags | Flags indicating if certain virtual keys were pressed
// when the mouse event occurred.
//@parm POINT | pt | Point at which the mouse event occurred.
//
//@mfunc BOOL | IMouseListener | OnLButtonUp | Handle left mouse button up events.
//@rdesc TRUE if event is handled; otherwise FALSE.
//@parm UINT | nFlags | Flags indicating if certain virtual keys were pressed
// when the mouse event occurred.
//@parm POINT | pt | Point at which the mouse event occurred.
//
//@mfunc BOOL | IMouseListener | OnLButtonDblClk | Handle left mouse button double click events.
//@rdesc TRUE if event is handled; otherwise FALSE.
//@parm UINT | nFlags | Flags indicating if certain virtual keys were pressed
// when the mouse event occurred.
//@parm POINT | pt | Point at which the mouse event occurred.
//
//@mfunc BOOL | IMouseListener | OnRButtonDown | Handle right mouse button down events.
//@rdesc TRUE if event is handled; otherwise FALSE.
//@parm UINT | nFlags | Flags indicating if certain virtual keys were pressed
// when the mouse event occurred.
//@parm POINT | pt | Point at which the mouse event occurred.
//
//@mfunc BOOL | IMouseListener | OnRButtonUp | Handle right mouse button up events.
//@rdesc TRUE if event is handled; otherwise FALSE.
//@parm UINT | nFlags | Flags indicating if certain virtual keys were pressed
// when the mouse event occurred.
//@parm POINT | pt | Point at which the mouse event occurred.
//
//@mfunc BOOL | IMouseListener | OnRButtonDblClk | Handle right mouse button double click events.
//@rdesc TRUE if event is handled; otherwise FALSE.
//@parm UINT | nFlags | Flags indicating if certain virtual keys were pressed
// when the mouse event occurred.
//@parm POINT | pt | Point at which the mouse event occurred.
//
//@mfunc BOOL | IMouseListener | OnMouseMove | Handle mouse move events.
//@rdesc TRUE if event is handled; otherwise FALSE.
//@parm UINT | nFlags | Flags indicating if certain virtual keys were pressed
// when the mouse event occurred.
//@parm POINT | pt | Point at which the mouse event occurred.
//
//@base public | IEventListener
//
class __declspec(uuid("47E1CE39-D500-11d2-8CAB-0010A4F36466"))
				IMouseListener : public IEventListener
{
public:
	//@cmember
	/* Handle left mouse button down events. */
	virtual BOOL OnLButtonDown(UINT nFlags, POINT pt) = 0;
	//@cmember
	/* Handle left mouse button up events. */
	virtual BOOL OnLButtonUp(UINT nFlags, POINT pt) = 0;
	//@cmember
	/* Handle left mouse button double click events. */
	virtual BOOL OnLButtonDblClk(UINT nFlags, POINT pt) = 0;
	//@cmember
	/* Handle right mouse button down events. */
	virtual BOOL OnRButtonDown(UINT nFlags, POINT pt) = 0;
	//@cmember
	/* Handle right mouse button up events. */
	virtual BOOL OnRButtonUp(UINT nFlags, POINT pt) = 0;
	//@cmember
	/* Handle right mouse button double click events. */
	virtual BOOL OnRButtonDblClk(UINT nFlags, POINT pt) = 0;
	//@cmember
	/* Handle mouse move events. */
	virtual BOOL OnMouseMove(UINT nFlags, POINT pt) = 0;
};

///////////////////////////////////////////////////////////////////////////
//@doc MouseAdapter
//
//@class MouseAdapter | The mouse adapter class supplies a default
// implementation of the <c IMouseListener> interface. The
// <mf MouseAdapter::HandleEvent> function deciphers the event and maps
// it to a member function for handling. This class supplies a default
// implementation for all handler messages in the IMouseListener interface
// that always return FALSE. Mouse listeners can be derived from this class
// that override and implement only the handlers they are interested in.
//
//@base public | EventListener_T
//
class MouseAdapter : public EventListener_T<IMouseListener, &IID_IMouseListener>
{
// Operations
public:

	//@cmember
	/* Handle left mouse button down events. */
	virtual BOOL OnLButtonDown(UINT nFlags, POINT pt);

	//@cmember
	/* Handle left mouse button up events. */
	virtual BOOL OnLButtonUp(UINT nFlags, POINT pt);

	//@cmember
	/* Handle left mouse button double click events. */
	virtual BOOL OnLButtonDblClk(UINT nFlags, POINT pt);

	//@cmember
	/* Handle right mouse button down events. */
	virtual BOOL OnRButtonDown(UINT nFlags, POINT pt);

	//@cmember
	/* Handle right mouse button up events. */
	virtual BOOL OnRButtonUp(UINT nFlags, POINT pt);

	//@cmember
	/* Handle right mouse button double click events. */
	virtual BOOL OnRButtonDblClk(UINT nFlags, POINT pt);

	//@cmember
	/* Handle mouse move events. */
	virtual BOOL OnMouseMove(UINT nFlags, POINT pt);
};

};  // end namespace rw

#endif // __COMPAT_MOUSELISTENER_H__
