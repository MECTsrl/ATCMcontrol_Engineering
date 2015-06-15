/////////////////////////////////////////////////////////////////////////////
// MvcController.h : SFL Model-View-Controller architecture 
//
// Stingray Software Foundation Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Jeff Boenig
// Description:  SFL MVC: Controller class
//

#pragma once

#ifndef __MVCCONTROLLER_H__
#define __MVCCONTROLLER_H__

#pragma warning (disable : 4786)

#include <foundation\Events\EventListener.h>
#include <foundation\Events\EventRouter.h>


namespace stingray {
namespace foundation {

///////////////////////////////////////////////////////////////////////////
// CMvcController

//@doc CMvcController
//@mfunc | CMvcController | CMvcController | Controller constructor.
//@comm Sets the model and viewport pointers to NULL.

//@doc CMvcController
//@mfunc | CMvcController | ~CMvcController | Controller destructor.
//@comm Removes all event listeners.

//@doc CMvcController
//@mfunc HRESULT | CMvcController | QueryInterface | Retrieve a pointer to
// an interface supported by this object.
//@parm REFIID | riid | Identifier of the interface being requested. 
//@parm void ** | ppvObj | Address of pointer variable that receives the
// interface pointer requested in riid.
//@rdesc S_OK if the interface is supported, E_NOINTERFACE if not.

//@doc CMvcController
//@mfunc ULONG | CMvcController | AddRef | Add a reference to this object.
//@rdesc New reference count value.
//@comm The CMvcController template does not implement reference counting.
// This function does nothing and returns 0. Derived classes can implement
// reference counting by overriding AddRef and Release.

//@doc CMvcController
//@mfunc ULONG | CMvcController | Release | Release a reference to this object.
//@rdesc New reference count value.
//@comm The CMvcController template does not implement reference counting.
// This function does nothing and returns 0. Derived classes can implement
// reference counting by overriding AddRef and Release.

//@doc CMvcController
//@mfunc void | CMvcController | SetModel | Assign a model to the controller.
//@rdesc void
//@parm ModelClass* | pModel | Pointer to model to assign to the controller.

//@doc CMvcController
//@mfunc ModelClass* | CMvcController | GetModel | Get a pointer to the model
// associated with this controller.
//@rdesc Pointer to the model associated with this controller.

//@doc CMvcController
//@mfunc void | CMvcController | SetViewport | Assign a viewport to the controller
//@parm ViewportClass* | pViewport | Pointer to the viewport to associate with this controller.

//@doc CMvcController
//@mfunc ViewportClass* | CMvcController | GetViewport | Get a pointer to the viewport
// associated with this controller
//@rdesc Pointer to the viewport associated with this controller.

//@doc CMvcController
//@mfunc bool | CMvcController | RouteEvent | Routes event objects to event listeners.
//@rdesc TRUE if event handled by at least one listener; otherwise FALSE.
//@parm IEvent* | pIEventPointer | Pointer to event object to route.
//@comm This function iterates through the collection of event listeners and calls
// <mf IEventListener::HandleEvent> for each event listener object. The function
// returns TRUE if at least one event listener handles the event. All event listeners
// get a chance to handle the event, regardless of whether it has already been
// handled by a previous event listener.

//@doc CMvcController
//@mfunc bool | CMvcController | AddListener | Add an event listener to the controller.
//@parm IEventListener* | pIListener | Points to an event listener to be added to the viewport
//@rdesc Returns TRUE if the listener was successfully added.  If the listener pointer
// is NULL, returns FALSE.

//@doc CMvcController
//@mfunc bool | CMvcController | RemoveListener | Removes an event listener from the controller.
//@parm IEventListener* | pIListener | Points to an event listener to be added to the viewport
//@rdesc Returns TRUE if the listener was successfully removed, else returns FALSE.

//@doc CMvcController
//@mdata ListenerVector | CMvcController | m_listeners | The array of event listeners for
// this controller.

//@doc CMvcController
//@mdata _Model* | CMvcController | m_pModel | Pointer to the model associated with 
// this controller.

//@doc CMvcController
//@mdata ViewportClass* | CMvcController | m_pViewport | Pointer to viewport associated with 
// this controller.

//@class CMvcController | This template is used to define controller
// classes that interact with a given type of model and viewport. The
// model class argument defines the type of model that this controller
// interacts with. The viewport class argument defines the type of
// viewport that this controller interacts with.
//
// This class inherits the <c IMvcController_T> template interface, and
// passes the model and viewport class arguments to that template.
//
// Controllers support the <c IEventListener> interface and also provide
// a mechanism for aggregating event listeners. The
// <mf CMvcController::AddListener> and <mf CMvcController::RemoveListener>
// methods allow event listener objects to be added to the controller.
// The controller then gives all of its listeners a chance to handle
// events that it receives.
//
//@tcarg class | _Model | Type of model accessed by the controller.
//@tcarg class | _Viewport | Type of viewport accessed by the controller.
//
//@base public | IEventRouter

template<typename _Model, typename _Viewport>
class CMvcController : public IEventRouter
{
public:
	typedef _Model ModelClass;
	typedef _Viewport ViewportClass;

public:

// Constructor/destructor
	//@cmember
	/* Controller constructor. */
	CMvcController()
	{
		m_pModel = NULL;
		m_pViewport = NULL;
	}

	//@cmember
	/* Controller destructor. */
	virtual ~CMvcController()
	{
		while (m_listeners.size() > 0)
		{
			IEventListener* pIListener = m_listeners.back();
			pIListener->Release();
			m_listeners.pop_back();
		}
	}

// Attributes
protected:

	typedef CMvcController<_Model, _Viewport> _thisClass;

	//@cmember
	/* Array of event listeners. */
	ListenerVector m_listeners;

	//@cmember
	/* Pointer to model associated with this controller. */
	ModelClass* m_pModel;

	//@cmember
	/* Pointer to viewport associated with this controller. */
	ViewportClass* m_pViewport;

// Operations
public:

	//@cmember
	/* Retrieve a pointer to an interface supported by this object. */
	virtual bool QueryGuid(REFGUID guid, void **ppvObj)
	{
		*ppvObj = NULL;

		if (guid == __uuidof(IEventRouter))
			*ppvObj = static_cast<IEventRouter *>(this);

		return (*ppvObj != NULL);
	}

	//@cmember
	/* Add a reference to this object. */
	ULONG STDMETHODCALLTYPE AddRef()
	{
		return 1;
	}

	//@cmember
	/* Release a reference to this object. */
	ULONG STDMETHODCALLTYPE Release()
	{
		return 1;
	}

	//@cmember
	/* Assign a model to the controller. */
	virtual void SetModel(ModelClass* pModel)
	{
		m_pModel = pModel;
	}

	//@cmember
	/* Get a pointer to the model associated with this controller. */
	virtual ModelClass* GetModel() const
	{
		return m_pModel;
	}

	//@cmember
	/* Assign a viewport to the controller. */
	virtual void SetViewport(ViewportClass* pViewport)
	{
		m_pViewport = pViewport;
	}

	//@cmember
	/* Get a pointer to the viewport associated with this controller. */
	virtual ViewportClass* GetViewport() const
	{
		return m_pViewport;
	}

	//@cmember
	/* Routes event objects to event listeners. */
	virtual bool RouteEvent(IEvent* pIEvent)
	{
		int nHandledCount = 0;

		if (pIEvent != NULL)
		{
			ListenerVector::const_iterator itListener;

			//
			// Give each event listener a chance to handle the event.
			//
			for (itListener = m_listeners.begin(); itListener != m_listeners.end(); itListener++)
			{
				if ((*itListener)->HandleEvent(pIEvent))
				{
					nHandledCount++;
				}
			}
		}

		return (nHandledCount > 0);
	}

	//@cmember
	/* Add an event listener to the controller. */
	virtual bool AddListener(IEventListener* pIListener)
	{
		if (pIListener != NULL)
		{
			pIListener->AddRef();
			m_listeners.push_back(pIListener);
			return true;
		}
		return false;
	}

	//@cmember
	/* Remove an event listener from the controller. */
	virtual bool RemoveListener(IEventListener* pIListener)
	{
		ListenerVector::iterator itListener = m_listeners.begin();
		while (itListener != m_listeners.end())
		{
			if (pIListener == *itListener)
			{
				(*itListener)->Release();
				m_listeners.erase(itListener);
				return true;
			}
		}
		return false;
	}
};

};	// namespace stingray::foundation
};  // namespace stingray

#endif  // #ifndef __MVCCONTROLLER_H__
