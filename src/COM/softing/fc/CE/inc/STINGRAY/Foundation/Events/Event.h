/////////////////////////////////////////////////////////////////////////////
// Event.h : Interface to event classes.
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
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Events/Event.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

#ifndef __EVENT_H__
#define __EVENT_H__

#pragma once

#include <foundation\CppIface.h>

namespace stingray {
namespace foundation {

///////////////////////////////////////////////////////////////////////////
//@doc IEvent
//
//@class IEvent | Base interface for event objects.
//
//@base public | IRefCount
//@base public | IQueryGuid
//
class __declspec(uuid("83C0B4B9-BFBD-466b-8082-B9B086E00756"))
					IEvent : public IRefCount, public IQueryGuid
{
public:
	//@cmember,mfunc
	// Dispatches the event to the given event listener.
	//@@rdesc Returns true if event dispatched successfully, otherwise false.
	//@@parm Pointer to listener object to receive the event.
	//@@end
	/* Dispatches the event to the given event listener*/
	virtual bool Dispatch(IQueryGuid* pIListener) = 0;
};


};	// namespace stingray::foundation
};  // namespace stingray

#endif  // #ifndef __EVENT_H__
