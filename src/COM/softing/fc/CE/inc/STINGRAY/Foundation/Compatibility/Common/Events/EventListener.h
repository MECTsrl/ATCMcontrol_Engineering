/////////////////////////////////////////////////////////////////////////////
// EventListener.h : Declares a template for Event Listeners
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
// Author:       Jeff Boenig and Bob Powell
// Description:  Framework independent event model.
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Compatibility/Common/Events/EventListener.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

#ifndef __COMPAT_EVENTLISTENER_H__
#define __COMPAT_EVENTLISTENER_H__

#include "Foundation\Compatibility\Common\CppIface.h"

#ifndef __COMPAT_EVENT_H__
#include "Foundation\Compatibility\Common\Events\Event.h"
#endif

#ifndef __COMPAT_SECCOLLECT_H__
#include "Foundation\Compatibility\Common\Patterns\Collect.h"
#endif

namespace rw
{

///////////////////////////////////////////////////////////////////////////
//@doc IEventListener
//
//@class IEventListener | Interface for receiving and handling events. The
// HandleEvent method takes a pointer to an event and returns TRUE if the
// event is handled.
//
//@base public | IUnknown
//
class __declspec(uuid("47E1CE38-D500-11d2-8CAB-0010A4F36466"))
				IEventListener : public IUnknown
{
public:
	//@cmember
	/* Receive an event and attempt to handle it. */
	virtual BOOL HandleEvent(IEvent* pIEvent) = 0;
};

///////////////////////////////////////////////////////////////////////////
//@doc EventListener_T
//
//@class EventListener_T | This template class is provided as a convenient
// way to implement event listener interfaces in concrete classes. This
// template implements <mf EventListener_T::QueryInterface> and
// <mf EventListener_T::HandleEvent>. The event listener interface is one
// of the two template parameters, and is used as the base class in the
// template declaration. The interface passed in is assumed to be derived
// from <c IEventListener>. The other template parameter is a pointer to
// the interface ID for the event listener interface.
//
//@tcarg class | interfacebase_t | Event listener interface to implement.
//@tcarg const IID * | piid | Pointer to interface ID of event listener interface.
//
//@mfunc HRESULT | EventListener_T | QueryInterface | Retrieve a pointer to
// an interface supported by this object.
//@rdesc S_OK if the interface is supported, E_NOINTERFACE if not.
//@parm REFIID | riid | Identifier of the interface being requested. 
//@parm void ** | ppvObj | Address of pointer variable that receives the
// interface pointer requested in riid.
//@comm Checks for the interface ID passed in the template parameter
// list. Also checks for IEventListener and IUnknown.
//
//@mfunc ULONG | EventListener_T | AddRef | Add a reference to this object.
//@rdesc New reference count value.
//@comm The EventListener_T template does not implement reference counting.
// This function does nothing and returns 0. Derived classes can implement
// reference counting by overriding AddRef and Release.
//
//@mfunc ULONG | EventListener_T | Release | Release a reference to this object.
//@rdesc New reference count value.
//@comm The EventListener_T template does not implement reference counting.
// This function does nothing and returns 0. Derived classes can implement
// reference counting by overriding AddRef and Release.
//
//@mfunc BOOL | EventListener_T | HandleEvent | Receive an event and attempt
// to handle it.
//@rdesc TRUE if event is handled; otherwise FALSE.
//@parm IEvent* | pIEvent | Pointer to event object.
//@comm This function calls the function <mf IEvent::Dispatch> on
// the event passed in. The listener passes a pointer to itself to
// the event's dispatch message.
//
template<class interfacebase_t, const IID *piid>
class EventListener_T : public interfacebase_t
{
public:

	//@cmember
	/* Retrieve a pointer to an interface supported by this object. */
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid,void **ppvObj)
	{
		*ppvObj = NULL;

		if (riid == *piid)
		{
			*ppvObj = static_cast<interfacebase_t *>(this);
		}
		else if (riid == IID_IEventListener)
		{
			*ppvObj = static_cast<IEventListener *>(this);
		}
		else if (riid == IID_IUnknown)
		{
			*ppvObj = static_cast<IUnknown *>(this);
		}

		if (*ppvObj != NULL)
		{
			this->AddRef();
			return S_OK;
		}

		return E_NOINTERFACE;
	}

	//@cmember
	/* Add a reference to this object. */
	virtual ULONG STDMETHODCALLTYPE AddRef()
	{
		return 1;
	}

	//@cmember
	/* Release a reference to this object. */
	virtual ULONG STDMETHODCALLTYPE Release()
	{
		return 1;
	}

	//@cmember
	/* Receive an event and attempt to handle it. */
	virtual BOOL HandleEvent(IEvent* pIEvent)
	{
		BOOL bHandled = FALSE;

		if (pIEvent != NULL)
		{
			bHandled = pIEvent->Dispatch(this);
		}

		return bHandled;
	}
};

///////////////////////////////////////////////////////////////////////////
//@type ListenerArray | Array of event listeners.
//
typedef CCArray_T<IEventListener*, IEventListener*> ListenerArray;

///////////////////////////////////////////////////////////////////////////
//@type ListenerIterator | Iterator for event listener collections.
//
typedef Iterator_T<IEventListener*> ListenerIterator;

};  // end namespace rw

#endif  // __COMPAT_EVENTLISTENER_H__
