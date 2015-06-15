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
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Events/EventQueue.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

#pragma once

#ifndef __EVENTQUEUE_H__
#define __EVENTQUEUE_H__

#include <unknwn.h>

#include <foundation\Events\Event.h>
#include <Foundation\Events\EventRouter.h>

namespace stingray {
namespace foundation {

/////////////////////////////////////////////////////////////////////////////
//@doc IEventQueue
//
//@class IEventQueue | Interface to an event queue. Events are placed in
// the queue using the <mf IEventQueue::QueueEvent> method. Each event is
// assigned a priority so that the event queue can dispatch events according
// to their priority. Events are dispatched to event routers using
// the <mf IEventQueue::DispatchEvent> method.
//
//@mfunc bool | IEventQueue | QueueEvent | Place an event in the queue and
// assign it a priority.
//@rdesc TRUE if the event is successfully added to the queue; otherwise FALSE.
//@parm IEvent* | pIEvent | Pointer to event object.
//@parm UINT | nPriority | Priority that the queue should assign to the event.
//
//@mfunc bool | IEventQueue | DispatchEvent | Remove next event from the queue
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
class __declspec(uuid("D89E6C95-069E-4312-9933-60259A571F41"))
				IEventQueue : public IUnknown
{
public:
	//@cmember
	/* Place an event in the queue and assign it a priority. */
	virtual bool QueueEvent(IEvent* pIEvent, const UINT nPriority) = 0;
	//@cmember
	/* Remove next event from the queue and send it to an event router. */
	virtual bool DispatchEvent(IEventRouter* pIEventRouter) = 0;
	//@cmember
	/* Remove all events from the queue without dispatching them. */
	virtual void ClearAllEvents() = 0;
};

/////////////////////////////////////////////////////////////////////////////
//@doc CEventQueueEntry
//
//@class CEventQueueEntry | Encapsulates an event entry in an event queue. It
// wraps a pointer to <c IEvent> and takes care of reference counting. In
// addition, it stores a priority value for the event queue entry. Comparison
// operators are overloaded and implemented by comparing the priority value.
// This allows event queue entries to be used with standard collection
// classes and easily sorted.
//
class CEventQueueEntry
{
public:

	//@cmember
	/* Construct an event queue entry from an event pointer and priority. */
	CEventQueueEntry(IEvent* pIEvent = NULL, const UINT nPriority = 0)
	{
		m_pIEvent = pIEvent;
		if (m_pIEvent != NULL)
		{
			m_pIEvent->AddRef();
		}
		m_nPriority = nPriority;
	}

	//@cmember
	/* Release the event pointer. */
	virtual ~CEventQueueEntry()
	{
		if (m_pIEvent != NULL)
		{
			m_pIEvent->Release();
		}
	}

	//@cmember
	/* Copy constructor. */
	CEventQueueEntry(const CEventQueueEntry& src)
	{
		*this = src;
	}

	//@cmember
	/* Assign an event queue entry to this object. */
	CEventQueueEntry& operator=(const CEventQueueEntry& src)
	{
		m_pIEvent = src.m_pIEvent;
		if (m_pIEvent != NULL)
		{
			m_pIEvent->AddRef();
		}
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
	bool operator>(const CEventQueueEntry& entry) const
	{
		return (m_nPriority > entry.m_nPriority);
	}

	//@cmember
	/* Less than operator. */
	bool operator<(const CEventQueueEntry& entry) const
	{
		return (m_nPriority < entry.m_nPriority);
	}

	//@cmember
	/* Compare for equivalence. */
	bool operator==(const CEventQueueEntry& entry) const
	{
		return (m_nPriority == entry.m_nPriority);
	}

	//@cmember
	/* Greater than or equal operator. */
	bool operator>=(const CEventQueueEntry& entry) const
	{
		return (m_nPriority >= entry.m_nPriority);
	}

	//@cmember
	/* Less than or equal operator. */
	bool operator<=(const CEventQueueEntry& entry) const
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

};	// namespace stingray::foundation
};  // namespace stingray

#endif  // #ifndef __EVENTQUEUE_H__
