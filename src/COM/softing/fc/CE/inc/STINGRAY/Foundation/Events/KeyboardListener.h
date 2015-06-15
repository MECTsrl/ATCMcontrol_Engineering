/////////////////////////////////////////////////////////////////////////////
// KeyboardListener.h : Declares keyboard listener interface and adapter class.
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
// Author:       Steve Danielson
// Description:  Framework independent event model.
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Events/KeyboardListener.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

#pragma once

#ifndef __KEYBOARDLISTENER_H__
#define __KEYBOARDLISTENER_H__

#include <foundation\SflGuids.h>
#include <foundation\Events/EventListener.h>

namespace stingray {
namespace foundation {

class __declspec(uuid("C7E4D0B6-4F42-4a42-8E63-C2CC1C529424"))
				IKeyboardListener : public IEventListener
{
public:
	virtual bool OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) = 0;
	virtual bool OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) = 0;
	virtual bool OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) = 0;
	virtual bool OnDeadChar(UINT nChar, UINT nRepCnt, UINT nFlags) = 0;
	virtual bool OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) = 0;
	virtual bool OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) = 0;
	virtual bool OnSysChar(UINT nChar, UINT nRepCnt, UINT nFlags) = 0;
	virtual bool OnSysDeadChar(UINT nChar, UINT nRepCnt, UINT nFlags) = 0;
};

class CKeyboardAdapter : public CEventListenerBase<IKeyboardListener>
{
// Operations
public:
	virtual bool OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual bool OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual bool OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual bool OnDeadChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual bool OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual bool OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual bool OnSysChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual bool OnSysDeadChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};

};	// namespace stingray::foundation
};  // namespace stingray

#endif  // #ifndef __KEYBOARDLISTENER_H__
