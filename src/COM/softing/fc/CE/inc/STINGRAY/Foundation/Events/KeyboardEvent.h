/////////////////////////////////////////////////////////////////////////////
// KeyboardEvent.h : Interface for keyboard events.
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
// Authors:      Steve Danielson
// Description:  Framework independent event model.
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Events/KeyboardEvent.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

#pragma once

#ifndef __KEYBOARDEVENT_H__
#define __KEYBOARDEVENT_H__

#include <foundation\SflGuids.h>
#include <foundation\Events\WinEvent.h>

namespace stingray {
namespace foundation {

class __declspec(uuid("3C741331-9C59-4966-B509-B4A0A2FD2250"))
					IKeyboardEvent : public IWinEvent
{
public:
	// Basic keyboard params
	virtual UINT GetChar() const = 0;
	virtual UINT GetRepCount() const = 0;
	virtual UINT GetFlags() const = 0;

	// Extra flags (HIWORD of lParam)
	virtual UINT GetScanCode() const = 0;
	virtual bool GetExtendedKey() const = 0;
	virtual bool GetContextCode() const = 0;
	virtual bool GetPrevKeyState() const = 0;
	virtual bool GetTransitionState() const = 0;
};

class CKeyboardEvent : public CWinEventBase<IKeyboardEvent>
{
// Constructor/destructor
public:

	//@cmember
	/* Construct a keyboard event from a message ID, WPARAM, and LPARAM. */
	CKeyboardEvent(const UINT nMessageID, const WPARAM wParam = 0, const LPARAM lParam = 0L);

// Operations
public:

	//@cmember
	/* Dispatches the event to the given event listener. */
	virtual bool Dispatch(IQueryGuid* pIListener);

	// Basic keyboard params
	virtual UINT GetChar() const;
	virtual UINT GetRepCount() const;
	virtual UINT GetFlags() const;

	// Extra flags (HIWORD of lParam)
	virtual UINT GetScanCode() const;
	virtual bool GetExtendedKey() const;
	virtual bool GetContextCode() const;
	virtual bool GetPrevKeyState() const;
	virtual bool GetTransitionState() const;
};

};	// namespace stingray::foundation
};  // namespace stingray

#endif  // #ifndef __KEYBOARDEVENT_H__
