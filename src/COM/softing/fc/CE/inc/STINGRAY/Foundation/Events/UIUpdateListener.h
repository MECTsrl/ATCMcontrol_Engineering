/////////////////////////////////////////////////////////////////////////////
// UIUpdateListener.h : Declares command listener interface and adapter class.
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
// Author:       Jeff Boenig and Bob Powell and George Shepherd
// Description:  Framework independent event model.
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Events/UIUpdateListener.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

#pragma once

#ifndef __UIUPDATELISTENER_H__
#define __UIUPDATELISTENER_H__

#include <Foundation\SflGuids.h>
#include <Foundation\Events\EventListener.h>
#include <Foundation\Events\UIUpdateElement.h>

namespace stingray {
namespace foundation {

///////////////////////////////////////////////////////////////////////////
//@doc IUIUpdateListener
//
//@class IUIUpdateListener | Interface to handling UIUpdate events. UIUpdate
// event listeners map UIUpdate and UIUpdate query events onto virtual member
// functions.
//
//@base public | IEventListener
//
class __declspec(uuid("E3533293-D410-4502-9AB3-61C1CCA58572"))
				IUIUpdateListener : public IEventListener
{
public:
	//@cmember
	/* Called when a UIUpdate event is received by the UIUpdate listener. */
	virtual bool OnUIUpdate(IUIUpdateElement* pUIUpdateElement, UINT nCommandID) = 0;
};

///////////////////////////////////////////////////////////////////////////
//@doc CUIUpdateAdapter
//
//@class CUIUpdateAdapter | The UIUpdate adapter class supplies a default
// implementation of the <c IUIUpdateListener> interface. The
// <mf CUIUpdateAdapter::HandleEvent> function deciphers the event and maps
// it to a member function for handling. This class supplies a default
// implementation for all handler messages in the IUIUpdateListener
// interface that always return FALSE. UIUpdate listeners can be derived
// from this class that override and implement only the handlers they are
// interested in.
//
//@base public | CEventListenerBase
//
class CUIUpdateAdapter : public CEventListenerBase<IUIUpdateListener>
{
// Operations
public:
	//@cmember
	/* Called when a UIUpdate event is received by the UIUpdate listener. */
	virtual bool OnUIUpdate(IUIUpdateElement* pUIUpdateElement, UINT nCommandID);
};

};	// namespace stingray::foundation
};  // namespace stingray

#endif  // #ifndef __UIUPDATELISTENER_H__
