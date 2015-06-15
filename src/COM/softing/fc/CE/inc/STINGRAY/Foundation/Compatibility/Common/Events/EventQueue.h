/////////////////////////////////////////////////////////////////////////////
// EventQueue.h : Interface to event queue classes.
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
// Author:       Jeff Boenig
// Description:  Framework independent event model.
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Compatibility/Common/Events/EventQueue.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

#ifndef __COMPAT_EVENTQUEUE_H__
#define __COMPAT_EVENTQUEUE_H__

#include <unknwn.h>

#ifndef __COMPAT_EVENT_H__
#include "Foundation\Compatibility\Common\Events\Event.h"
#endif

namespace rw
{

/////////////////////////////////////////////////////////////////////////////
//@doc IEventQueue
//
//@class IEventQueue | Interface to an event queue. Events are placed in
// the queue using the <mf IEventQueue::QueueEvent> method. Each event is
// assigned a priority so that the event queue can dispatch events according
// to their priority. Events are dispatched to event routers using
// the <mf IEventQueue::DispatchEvent> method.
//
//@mfunc BOOL | IEventQueue | QueueEvent | Place an event in the queue and
// assign it a priority.
//@rdesc TRUE if the event is successfully added to the queue; otherwise FALSE.
//@parm IEvent* | pIEvent | Pointer to event object.
//@parm UINT | nPriority | Priority that the queue should assign to the event.
//
//@mfunc BOOL | IEventQueue | DispatchEvent | Remove next event from the queue
// and send it to an event router
//@rdesc TRUE if an event was dispatched; otherwise FALSE.
//@parm IEventRouter* | pIEventRouter | Pointer to event router to receive
// event.
//@comm The queue determines the next event to be dispatched and pulls it
// from the queue. It then sends it to the event router using the
// <mf IEventRouter::RouteEvent> method. The next event in the queue
// is usually determined by a combination of chronological order and
// priority. This function removes the event from the queue.
//
//@mfunc void | IEventQueue | ClearAllEvents | Remove all events from the
// queue without dispatching them
//@rdesc void
//@comm This function clears out the queue without dispatching any of
// the events.
//
//@base public | IUnknown
//
class __declspec(uuid("47E1CE37-D500-11d2-8CAB-0010A4F36466"))
				IEventQueue : public IUnknown
{
public:
	//@cmember
	/* Place an event in the queue and assign it a priority. */
	virtual BOOL QueueEvent(IEvent* pIEvent, const UINT nPriority) = 0;
	//@cmember
	/* Remove next event from the queue and send it to an event router. */
	virtual BOOL DispatchEvent(IEventRouter* pIEventRouter) = 0;
	//@cmember
	/* Remove all events from the queue without dispatching them. */
	virtual void ClearAllEvents() = 0;
};

/////////////////////////////////////////////////////////////////////////////
//@doc EventQueueEntry
//
//@class EventQueueEntry | Encapsulates an event entry in an event queue. It
// wraps a pointer to <c IEvent> and takes care of reference counting. In
// addition, it stores a priority value for the event queue entry. Comparison
// operators are overloaded and implemented by comparing the priority value.
// This allows event queue entries to be used with standard collection
// classes and easily sorted.
//
class EventQueueEntry
{
public:

	//@cmember
	/* Construct an event queue entry from an event pointer and priority. */
	EventQueueEntry(IEvent* pIEvent = NULL, const UINT nPriority = 0)
	{
		m_pIEvent = pIEvent;
		if (m_pIEvent != NULL)
			m_pIEvent->AddRef();
		m_nPriority = nPriority;
	}

	//@cmember
	/* Release the event pointer. */
	virtual ~EventQueueEntry()
	{
		if (m_pIEvent != NULL)
			m_pIEvent->Release();
	}

	//@cmember
	/* Copy constructor. */
	EventQueueEntry(const EventQueueEntry& src)
	{
		*this = src;
	}

	//@cmember
	/* Assign an event queue entry to this object. */
	EventQueueEntry& operator=(const EventQueueEntry& src)
	{
		m_pIEvent = src.m_pIEvent;
		if (m_pIEvent != NULL)
			m_pIEvent->AddRef();
		m_nPriority = src.m_nPriority;
		return *this;
	}

	//@cmember
	/* Cast the event queue entry to an IEvent pointer. */
	operator IEvent*() const
	{
		return m_pIEvent;
	}

	//@cmember
	/* Greater than operator. */
	bool operator>(const EventQueueEntry& entry) const
	{
		return (m_nPriority > entry.m_nPriority);
	}

	//@cmember
	/* Less than operator. */
	bool operator<(const EventQueueEntry& entry) const
	{
		return (m_nPriority < entry.m_nPriority);
	}

	//@cmember
	/* Compare for equivalence. */
	bool operator==(const EventQueueEntry& entry) const
	{
		return (m_nPriority == entry.m_nPriority);
	}

	//@cmember
	/* Greater than or equal operator. */
	bool operator>=(const EventQueueEntry& entry) const
	{
		return (m_nPriority >= entry.m_nPriority);
	}

	//@cmember
	/* Less than or equal operator. */
	bool operator<=(const EventQueueEntry& entry) const
	{
		return (m_nPriority <= entry.m_nPriority);
	}

protected:
	//@cmember
	/* Pointer to event that this entry contains. */
	IEvent* m_pIEvent;
	//@cmember
	/* Priority for this event queue entry. */
	UINT m_nPriority;
};

};  // end namespace rw

#endif  // __COMPAT_EVENTQUEUE_H__
