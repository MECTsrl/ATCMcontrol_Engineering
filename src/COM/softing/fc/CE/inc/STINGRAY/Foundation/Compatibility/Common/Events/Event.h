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
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Compatibility/Common/Events/Event.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

#ifndef __COMPAT_EVENT_H__
#define __COMPAT_EVENT_H__

#include "Foundation\Compatibility\Common\CppIface.h"

namespace rw
{

///////////////////////////////////////////////////////////////////////////
//@doc IEvent
//
//@class IEvent | Base interface to event objects.
//
//@base public | IUnknown
//
class __declspec(uuid("47E1CE31-D500-11d2-8CAB-0010A4F36466"))
					IEvent : public IUnknown
{
public:
	//@cmember
	/* Dispatches the event to the given event listener. */
	virtual BOOL Dispatch(IUnknown* pIListener) = 0;
};

};  // end namespace rw

#endif  // __COMPAT_EVENT_H__
