/////////////////////////////////////////////////////////////////////////////
// EventRouter.h : Interface to event routers.
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
// Authors:      Jeff Boenig
// Description:  Framework independent event model.
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Compatibility/Common/Events/EventRouter.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

#ifndef __COMPAT_EVENTROUTER_H__
#define __COMPAT_EVENTROUTER_H__

#include "Foundation\Compatibility\Common\CppIface.h"

#ifndef __COMPAT_EVENT_H__
#include "Foundation\Compatibility\Common\Events\Event.h"
#endif

#ifndef __COMPAT_EVENTLISTENER_H__
#include "Foundation\Compatibility\Common\Events\EventListener.h"
#endif

namespace rw
{

///////////////////////////////////////////////////////////////////////////
// IEventRouter

//@doc IEventRouter
//@mfunc BOOL | IEventRouter | RouteEvent | Routes event objects to event listeners.
//@rdesc TRUE if at least one listener handled the event.
//@parm IEvent* | pIEvent | Pointer to event object.

//@doc IEventRouter
//@mfunc BOOL | IEventRouter | AddListener | Add an event listener to the router.
//@rdesc TRUE if successful, otherwise FALSE.
//@parm IEventListener* | pIListener | Pointer to event listener to add.

//@doc IEventRouter
//@mfunc BOOL | IEventRouter | RemoveListener | Remove an event listener from the router.
//@rdesc TRUE if successful, otherwise FALSE.
//@parm IEventListener* | pIListener | Pointer to event listener to remove.

//@class IEventRouter | Interface for routing events to event listeners.
// The RouteEvent method takes a pointer to an event and returns TRUE if
// the event was handled by one or more listeners or FALSE if the event
// was not handled.

//@base public | IUnknown

class __declspec(uuid("47E1CE36-D500-11d2-8CAB-0010A4F36466"))
				IEventRouter : public IUnknown
{
public:
	//@cmember
	/* Routes event objects to event listeners. */
	virtual BOOL RouteEvent(IEvent* pIEvent) const = 0;
	//@cmember
	/* Add an event listener to the router. */
	virtual BOOL AddListener(IEventListener* pIListener) = 0;
	//@cmember
	/* Remove an event listener from the router. */
	virtual BOOL RemoveListener(IEventListener* pIListener) = 0;
};

};  // end namespace rw

#endif // __COMPAT_EVENTROUTER_H__
