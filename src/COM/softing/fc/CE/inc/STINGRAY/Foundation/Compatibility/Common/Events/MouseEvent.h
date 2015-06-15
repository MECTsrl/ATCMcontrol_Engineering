/////////////////////////////////////////////////////////////////////////////
// MouseEvent.h : Interface for mouse events.
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
// Authors:      Jeff Boenig and Bob Powell
// Description:  Framework independent event model.
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Compatibility/Common/Events/MouseEvent.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

#ifndef __COMPAT_MOUSEEVENT_H__
#define __COMPAT_MOUSEEVENT_H__

#include "Foundation\Compatibility\Common\CmnGuids.h"

#ifndef __COMPAT_WINEVENT_H__
#include "Foundation\Compatibility\Common\Events\WinEvent.h"
#endif

namespace rw
{

///////////////////////////////////////////////////////////////////////////
//@doc IMouseEvent | Interface for mouse events.
//
//@class IMouseEvent | Interface to mouse events. This interface derives
// from IWinEvent.
//
//@base public | IWinEvent
//
class __declspec(uuid("47E1CE33-D500-11d2-8CAB-0010A4F36466"))
					IMouseEvent : public IWinEvent
{
public:
	//@cmember
	/* Return the point at which the mouse event occurred. */
	virtual void GetPoint(POINT& pt) = 0;

	//@cmember
	/* Set the point at which the mouse event occurred. */
	virtual void SetPoint(const POINT& pt) = 0;

	//@cmember
	/* Test to see if certain virtual keys were pressed during the mouse event. */
	virtual BOOL TestFlags(const UINT nMask) = 0;
};

///////////////////////////////////////////////////////////////////////////
//@doc MouseEvent
//
//@class MouseEvent | This class encapsulates a Windows mouse event. The
// LONG parameter of the Windows message contains the point at which the
// mouse event occurred. The function <mf MouseEvent::GetPoint> decodes
// the LONG parameter and returns it as a POINT. The WORD parameter
// contains flags indicating if certain virtual keys are down. The
// <mf MouseEvent::TestFlags> method can be used to test the flags with
// the following masks -
//
//     MK_CONTROL   Set if the CTRL key is down.
//     MK_LBUTTON   Set if the left mouse button is down.
//     MK_MBUTTON   Set if the middle mouse button is down.
//     MK_RBUTTON   Set if the right mouse button is down.
//     MK_SHIFT     Set if the SHIFT key is down. 
//
//
//@base public | WinEvent_T
//
class MouseEvent : public WinEvent_T<IMouseEvent, &IID_IMouseEvent>
{
// Constructor/destructor
public:

	//@cmember
	/* Construct a mouse event from a message ID, WPARAM, and LPARAM. */
	MouseEvent(const UINT nMessageID, const WPARAM wParam = 0, const LPARAM lParam = 0L);

// Operations
public:

	//@cmember
	/* Dispatches the event to the given event listener. */
	virtual BOOL Dispatch(IUnknown* pIListener);

	//@cmember
	/* Return the point at which the mouse event occurred. */
	virtual void GetPoint(POINT& pt);

	//@cmember
	/* Set the point at which the mouse event occurred. */
	virtual void SetPoint(const POINT& pt);

	//@cmember
	/* Test to see if certain virtual keys were pressed during the mouse event. */
	virtual BOOL TestFlags(const UINT nMask);
};

};  // end namespace rw

#endif // __COMPAT_MOUSEEVENT_H__
