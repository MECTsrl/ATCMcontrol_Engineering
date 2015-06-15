/////////////////////////////////////////////////////////////////////////////
// CScrollEvent.h : Interface for mouse events.
//
// Stingray Software Extension Classes
// Copyright (C) 1999 Stingray Software Inc,
// All rights reserved.
//
// This source code is only intended as a supplement to
// the Objective Toolkit Class Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding
// the Objective Toolkit product.
//
// Authors:      Jeff Boenig
// Description:  Framework independent event model.
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Events/ScrollEvent.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

#pragma once

#ifndef __SCROLLEVENT_H__
#define __SCROLLEVENT_H__

#include <foundation\SflGuids.h>
#include <foundation\Events\WinEvent.h>

namespace stingray {
namespace foundation {

///////////////////////////////////////////////////////////////////////////
//@doc IScrollEvent | Interface for mouse events.
//
//@class IScrollEvent | Interface to scroll events. This interface derives
// from IWinEvent.
//
//@base public | IWinEvent
//
class __declspec(uuid("930EAE9B-9FBB-4610-9C1D-1DD2C6A532CD"))
					IScrollEvent : public IWinEvent
{
public:
	//@cmember
	/* The scroll code identifies the type of scrolling (line, page, etc.). */
	virtual int GetScrollCode() = 0;

	//@cmember
	/* Return the scroll position. */
	virtual short GetScrollPosition() = 0;

	//@cmember
	/* Return the scroll bar window handle. */
	virtual HWND GetScrollBarHandle() = 0;
};

///////////////////////////////////////////////////////////////////////////
//@doc CScrollEvent
//
//@class CScrollEvent |
//
//@base public | CWinEventBase
//
class CScrollEvent : public CWinEventBase<IScrollEvent>
{
// Constructor/destructor
public:

	//@cmember
	/* Construct a scroll event from a message ID, WPARAM, and LPARAM. */
	CScrollEvent(const UINT nMessageID, const WPARAM wParam = 0, const LPARAM lParam = 0L);

// Operations
public:

	//@cmember
	/* Dispatches the event to the given event listener. */
	virtual bool Dispatch(IQueryGuid* pIListener);

	//@cmember
	/* The scroll code identifies the type of scrolling (line, page, etc.). */
	virtual int GetScrollCode();

	//@cmember
	/* Return the scroll position. */
	virtual short GetScrollPosition();

	//@cmember
	/* Return the scroll bar window handle. */
	virtual HWND GetScrollBarHandle();
};

};	// namespace stingray::foundation
};  // namespace stingray

#endif  // #ifndef __SCROLLEVENT_H__
