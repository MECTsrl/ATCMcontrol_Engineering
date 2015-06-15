/////////////////////////////////////////////////////////////////////////////
// CMouseEvent.h : Interface for mouse events.
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
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Events/MouseEvent.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

#pragma once

#ifndef __MOUSEEVENT_H__
#define __MOUSEEVENT_H__

#include <foundation\SflGuids.h>
#include <foundation\Events\WinEvent.h>

namespace stingray {
namespace foundation {

///////////////////////////////////////////////////////////////////////////
//@doc IMouseEvent | Interface for mouse events.
//
//@class IMouseEvent | Interface to mouse events. This interface derives
// from IWinEvent.
//
//@base public | IWinEvent
//
class __declspec(uuid("0F5F5DE3-B582-4539-9145-E749250F2FDA"))
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
	virtual bool TestFlags(const UINT nMask) = 0;
};

///////////////////////////////////////////////////////////////////////////
//@doc CMouseEvent
//
//@class CMouseEvent | This class encapsulates a Windows mouse event. The
// LONG parameter of the Windows message contains the point at which the
// mouse event occurred. The function <mf CMouseEvent::GetPoint> decodes
// the LONG parameter and returns it as a POINT. The WORD parameter
// contains flags indicating if certain virtual keys are down. The
// <mf CMouseEvent::TestFlags> method can be used to test the flags with
// the following masks -
//
//     MK_CONTROL   Set if the CTRL key is down.
//     MK_LBUTTON   Set if the left mouse button is down.
//     MK_MBUTTON   Set if the middle mouse button is down.
//     MK_RBUTTON   Set if the right mouse button is down.
//     MK_SHIFT     Set if the SHIFT key is down. 
//
//
//@base public | CWinEventBase
//
class CMouseEvent : public CWinEventBase<IMouseEvent>
{
// Constructor/destructor
public:

	//@cmember
	/* Construct a mouse event from a message ID, WPARAM, and LPARAM. */
	CMouseEvent(const UINT nMessageID, const WPARAM wParam = 0, const LPARAM lParam = 0L);

// Operations
public:

	//@cmember
	/* Dispatches the event to the given event listener. */
	virtual bool Dispatch(IQueryGuid* pIListener);

	//@cmember
	/* Return the point at which the mouse event occurred. */
	virtual void GetPoint(POINT& pt);

	//@cmember
	/* Set the point at which the mouse event occurred. */
	virtual void SetPoint(const POINT& pt);

	//@cmember
	/* Test to see if certain virtual keys were pressed during the mouse event. */
	virtual bool TestFlags(const UINT nMask);
};



///////////////////////////////////////////////////////////////////////////
//@doc IMouseSetCursorEvent | Interface for mouse set cursor events.
//
//@class IMouseSetCursorEvent | Interface for mouse set cursor events. This interface derives
// from IWinEvent.
//
//@base public | IWinEvent
//
class __declspec(uuid("4C53E132-7D9F-4ce6-AB04-0A79A741F1E1"))
					IMouseSetCursorEvent : public IWinEvent
{
public:
	//@cmember
	/* Return the window that contains the cursor. */
	virtual HWND GetCursorWindow() const = 0;

	//@cmember
	/* Return the hit-test area code. */
	virtual UINT GetHitTestCode() const = 0;

	//@cmember
	/* Return the mouse message number. */
	virtual UINT GetMouseMessage() const = 0;
};

///////////////////////////////////////////////////////////////////////////
//@doc CMouseSetCursorEvent
//
//@class CMouseSetCursorEvent | This class encapsulates a set cursor message.
//
//@base public | CWinEventBase
//
class CMouseSetCursorEvent : public CWinEventBase<IMouseSetCursorEvent>
{
// Constructor/destructor
public:

	//@cmember
	/* Construct a set cursor event from a message ID, WPARAM, and LPARAM. */
	CMouseSetCursorEvent(const UINT nMessageID, const WPARAM wParam = 0, const LPARAM lParam = 0L);

// Operations
public:
	//@cmember
	/* Dispatches the event to the given event listener. */
	virtual bool Dispatch(IQueryGuid* pIListener);

	//@cmember
	/* Return the window that contains the cursor. */
	virtual HWND GetCursorWindow() const;

	//@cmember
	/* Return the hit-test area code. */
	virtual UINT GetHitTestCode() const;

	//@cmember
	/* Return the mouse message number. */
	virtual UINT GetMouseMessage() const;
};

};	// namespace stingray::foundation
};  // namespace stingray

#endif  // #ifndef __MOUSEEVENT_H__
