/////////////////////////////////////////////////////////////////////////////
// MvcTemplate.h : MVC template classes.
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
// Description:  MVC template classes.
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Compatibility/MVC/MvcTemplate.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

#ifndef __COMPAT_MVCTEMPLATE_H__
#define __COMPAT_MVCTEMPLATE_H__

#pragma warning (disable : 4786)

#ifndef __COMPAT_MVCIFCS_H__
#include "Foundation\Compatibility\MVC\MvcIfcs.h"
#endif

#ifndef __COMPAT_EVENTROUTER_H__
#include "Foundation\Compatibility\Common\Events\EventRouter.h"
#endif

#ifndef __COMPAT_MOUSEEVENT_H__
#include "Foundation\Compatibility\Common\Events\MouseEvent.h"
#endif

using namespace rw;

///////////////////////////////////////////////////////////////////////////
// MvcViewport_T

//@doc MvcViewport_T
//@mfunc | MvcViewport_T | MvcViewport_T | Viewport constructor.
//@comm Sets the model and controller pointers to NULL.

//@doc MvcViewport_T
//@mfunc | MvcViewport_T | ~MvcViewport_T | Viewport destructor.
//@comm Removes the viewport from the model as an observer. If the
// m_bAutoDelCtlr flag is set, then the controller is deleted.

//@doc MvcViewport_T
//@mfunc HRESULT | MvcViewport_T | QueryInterface | Retrieve a pointer to
// an interface supported by this object.
//@parm REFIID | riid | Identifier of the interface being requested. 
//@parm void ** | ppvObj | Address of pointer variable that receives the
// interface pointer requested in riid.
//@rdesc S_OK if the interface is supported, E_NOINTERFACE if not.

//@doc MvcViewport_T
//@mfunc ULONG | MvcViewport_T | AddRef | Add a reference to this object.
//@rdesc New reference count value.
//@comm The MvcViewport_T template does not implement reference counting.
// This function does nothing and returns 0. Derived classes can implement
// reference counting by overriding AddRef and Release.

//@doc MvcViewport_T
//@mfunc ULONG | MvcViewport_T | Release | Release a reference to this object.
//@rdesc New reference count value.
//@comm The MvcViewport_T template does not implement reference counting.
// This function does nothing and returns 0. Derived classes can implement
// reference counting by overriding AddRef and Release.

//@doc MvcViewport_T
//@mfunc void | MvcViewport_T | OnUpdate | The function reacts to notifications of
// change on the subject it is observing.
//@rdesc void
//@parm ISubject* | pSubject | The subject sending the update.
//@parm IMessage* | pMsg | The message indicating what change has occurred.
//@comm The default implementation does nothing. Override this function to provide your
// application specific update code.

//@doc MvcViewport_T
//@mfunc void | MvcViewport_T | Draw_ | Draw the viewport to a device context.
//@rdesc void
//@parm IGraphicsContext* | pIGC | Pointer to drawing context.
//@comm The base class implementation does nothing. Override this function to
// render the viewport to a device context. NOTE: The name of this function can
// be decorated with an underscore in order to avoid conflicts with exist code.
// If the macro _DECORATE_VISUAL_H_NAMES is defined, then the decorated name
// name is used. If it is not defined, then the underscore can be dropped.

//@doc MvcViewport_T
//@mfunc void | MvcViewport_T | SetModel | Assign a model to the viewport.
//@rdesc void
//@parm model_t* | pModel | Pointer to model to assign to the viewport.
//@comm Adds the viewport as an observer to the model. Also, pass the
// model to the controller by calling <mf MvcController_T::SetModel>.

//@doc MvcViewport_T
//@mfunc model_t* | MvcViewport_T | GetModel | Gets a pointer to the model associated with this viewport.
//@rdesc A pointer to the associated model.

//@doc MvcViewport_T
//@mfunc BOOL | MvcViewport_T | CreateController | Create the default controller for this viewport.
//@rdesc FALSE if an error occurs; otherwise TRUE.
//@comm The default implementation does nothing and returns TRUE. Derived
// classes override this function in order to create a default controller.

//@doc MvcViewport_T
//@mfunc void | MvcViewport_T | SetController | Set the controller for this viewport.
//@rdesc void
//@parm ctlr_t* | pController | Pointer to controller to assign to the viewport.
//@parmopt const BOOL | bAutoDelCtlr | FALSE | Flag indicating whether the viewport
// should destroy the controller in its destructor.

//@doc MvcViewport_T
//@mfunc ctlr_t* | MvcViewport_T | GetController | Get a pointer to the controller for this viewport.
//@rdesc Pointer to controller for this viewport.

//@doc MvcViewport_T
//@mfunc BOOL | MvcViewport_T | RouteEvent | Routes event objects to event listeners.
//@rdesc TRUE if event handled by at least one listener; otherwise FALSE.
//@parm IEvent* | pIEventPointer | Pointer to event object to route.
//@comm If the event is a <c MouseEvent>, the viewport verifies that it occurred
// within its bounds. Mouse events that occur outside of the viewport bounds
// are ignored. If a controller is connected to the viewport, the viewport then
// passes the event to the controller by calling <mf MvcController_T::HandleEvent>
// on the controller.

//@doc MvcViewport_T
//@mfunc BOOL | MvcViewport_T | AddListener | Add an event listener to the viewport.
//@parm IEventListener* | pIListener | Points to an event listener to be added to the viewport
//@rdesc Always returns FALSE.
//@comm This implementation does nothing because all events are routed to the controller.

//@doc MvcViewport_T
//@mfunc BOOL | MvcViewport_T | RemoveListener | Removes an event listener from the viewport.
//@parm IEventListener* | pIListener | Points to an event listener to be removed from the viewport
//@rdesc Always returns FALSE.
//@comm This implementation does nothing because all events are routed to the controller.

//@doc MvcViewport_T
//@mdata model_t* | MvcViewport_T | m_pModel | Pointer to the model this
// viewport observes and renders.

//@doc MvcViewport_T
//@mdata ctlr_t* | MvcViewport_T | m_pCtlr | Pointer to the controller this
// viewport sends events to.

//@doc MvcViewport_T
//@mdata BOOL | MvcViewport_T | m_bAutoDelCtlr | Flag to indicate if controller
// should be deleted when the viewport is deleted

//@class MvcViewport_T | This template is used to define viewport classes
// that interact with a given type of model and controller. The base class
// template argument is usually a class that supports the <c IVisual>
// interface. The model class template argument defines the type of model
// that the viewport will observe and render, and is usually strongly-typed.
// The controller class template argument defines the type of controller
// that the viewport interacts with. The association from the viewport to
// the controller is usually a weakly-typed one. Generally speaking, the
// viewport only needs to route events to the controller.
//
// This class inherits the <c IMvcViewport_T> template interface, passing
// it the model class and controller class arguments.
//
// This class also inherits the <c IEventRouter> interface, so that it
// can route events to event listeners. Events are always routed to the
// controller. Derived viewport classes can override the <mf MvcViewport_T::RouteEvent>
// method and implement custom routing.
//
//@tcarg class | visual_t | Base class for the viewport.
//@tcarg class | model_t | Base class for models to be used with the viewport.
//@tcarg class | ctlr_t | Base class for controllers to be used with the viewport.
//
//@base public | IMvcViewport_T
//@base public | IEventRouter
//
template<class visual_t, class model_t, class ctlr_t>
class MvcViewport_T : public visual_t, public IMvcViewport_T<model_t,ctlr_t>, public IEventRouter
{
// Constructor/destructor
public:

	//@cmember
	/* Viewport constructor. */
	MvcViewport_T()
	{
		m_bAutoDelCtlr = FALSE;
		m_pModel = NULL;
		m_pCtlr = NULL;
	}

	//@cmember
	/* Viewport destructor. */
	virtual ~MvcViewport_T()
	{
		if (m_pModel != NULL)
		{
			m_pModel->RemoveObserver(this);
		}
		if (m_pCtlr != NULL && m_bAutoDelCtlr)
		{
			delete m_pCtlr;
		}
	}

// Attributes
protected:

	//@cmember
	/* Pointer to the model. */
	model_t* m_pModel;

	//@cmember
	/* Pointer to the controller. */
	ctlr_t* m_pCtlr;

	//@cmember
	/* Flag to indicate if controller should be deleted when the viewport is deleted. */
	BOOL m_bAutoDelCtlr;

// Operations
public:

	//@cmember
	/* Retrieve a pointer to an interface supported by this object. */
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid,void **ppvObj)
	{
		HRESULT hr = E_NOINTERFACE;
		*ppvObj = NULL;

		if (riid == IID_IEventRouter)
		{
			*ppvObj = static_cast<IEventRouter *>(this);
			this->AddRef();
			hr = S_OK;
		}
		else if (riid == IID_IUnknown)
		{
			*ppvObj = static_cast<IEventRouter *>(this);
			this->AddRef();
			hr = S_OK;
		}
		else
		{
			hr = visual_t::QueryInterface(riid, ppvObj);
		}

		return hr;
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
	/* The function reacts to notifications of change on the subject it is observing. */
	virtual void OnUpdate(ISubject* pSubject, rw::IMessage* pMsg)
	{
		pSubject;
		pMsg;
	}

	//@cmember
	/* Draw the viewport to a device context. */
	virtual void Draw_(IGraphicsContext* pIGC)
	{
		pIGC;
	}

	//@cmember
	/* Assign a model to the viewport. */
	virtual void SetModel(model_t* pModel)
	{
		m_pModel = pModel;
		if (m_pModel != NULL)
			m_pModel->AddObserver(this);
		if (m_pCtlr != NULL)
			m_pCtlr->SetModel(m_pModel);
	}

	//@cmember
	/* Get a pointer to the model associated with this viewport. */
	virtual model_t* GetModel() const
	{
		return m_pModel;
	}

	//@cmember
	/* Create the default controller for this viewport. */
	virtual BOOL CreateController()
	{
		return TRUE;
	}

	//@cmember
	/* Set the controller for this viewport. */
	virtual void SetController(ctlr_t* pController, const BOOL bAutoDelCtlr = FALSE)
	{
		m_bAutoDelCtlr = bAutoDelCtlr;
		m_pCtlr = pController;
	}

	//@cmember
	/* Get a pointer to the controller for this viewport. */
	virtual ctlr_t* GetController()
	{
		return m_pCtlr;
	}

	//@cmember
	/* Routes event objects to event listeners. */
	virtual BOOL RouteEvent(IEvent* pIEvent) const
	{
		BOOL bRes = FALSE;

		IMouseEvent* pIMouseEvent;

		if (pIEvent->QueryInterface(IID_IMouseEvent, (void**) &pIMouseEvent) == S_OK)
		{
			//
			// If this event is a mouse event, then verify that it occurred
			// within the viewport bounds. If not, return FALSE to caller.
			//
			POINT pt;
			pIMouseEvent->GetPoint(pt);
			pIMouseEvent->Release();

			if (::PtInRect(&m_rc, pt) != TRUE)
			{
				return FALSE;
			}
		}

		if (m_pCtlr != NULL)
		{
			//
			// Pass event to the controller.
			//
			bRes = m_pCtlr->RouteEvent(pIEvent);
		}

		return bRes;
	}

	//@cmember
	/* Add an event listener to the router. */
	virtual BOOL AddListener(IEventListener* pIListener)
	{
		pIListener;
		return FALSE;
	}

	//@cmember
	/* Remove an event listener from the router. */
	virtual BOOL RemoveListener(IEventListener* pIListener)
	{
		pIListener;
		return FALSE;
	}

};

///////////////////////////////////////////////////////////////////////////
// MvcController_T

//@doc MvcController_T
//@mfunc | MvcController_T | MvcController_T | Controller constructor.
//@comm Sets the model and viewport pointers to NULL.

//@doc MvcController_T
//@mfunc | MvcController_T | ~MvcController_T | Controller destructor.
//@comm Removes all event listeners.

//@doc MvcController_T
//@mfunc HRESULT | MvcController_T | QueryInterface | Retrieve a pointer to
// an interface supported by this object.
//@parm REFIID | riid | Identifier of the interface being requested. 
//@parm void ** | ppvObj | Address of pointer variable that receives the
// interface pointer requested in riid.
//@rdesc S_OK if the interface is supported, E_NOINTERFACE if not.

//@doc MvcController_T
//@mfunc ULONG | MvcController_T | AddRef | Add a reference to this object.
//@rdesc New reference count value.
//@comm The MvcController_T template does not implement reference counting.
// This function does nothing and returns 0. Derived classes can implement
// reference counting by overriding AddRef and Release.

//@doc MvcController_T
//@mfunc ULONG | MvcController_T | Release | Release a reference to this object.
//@rdesc New reference count value.
//@comm The MvcController_T template does not implement reference counting.
// This function does nothing and returns 0. Derived classes can implement
// reference counting by overriding AddRef and Release.

//@doc MvcController_T
//@mfunc void | MvcController_T | SetModel | Assign a model to the controller.
//@rdesc void
//@parm model_t* | pModel | Pointer to model to assign to the controller.

//@doc MvcController_T
//@mfunc model_t* | MvcController_T | GetModel | Get a pointer to the model
// associated with this controller.
//@rdesc Pointer to the model associated with this controller.

//@doc MvcController_T
//@mfunc void | MvcController_T | SetViewport | Assign a viewport to the controller
//@parm viewport_t* | pViewport | Pointer to the viewport to associate with this controller.

//@doc MvcController_T
//@mfunc viewport_t* | MvcController_T | GetViewport | Get a pointer to the viewport
// associated with this controller
//@rdesc Pointer to the viewport associated with this controller.

//@doc MvcController_T
//@mfunc BOOL | MvcController_T | RouteEvent | Routes event objects to event listeners.
//@rdesc TRUE if event handled by at least one listener; otherwise FALSE.
//@parm IEvent* | pIEventPointer | Pointer to event object to route.
//@comm This function iterates through the collection of event listeners and calls
// <mf IEventListener::HandleEvent> for each event listener object. The function
// returns TRUE if at least one event listener handles the event. All event listeners
// get a chance to handle the event, regardless of whether it has already been
// handled by a previous event listener.

//@doc MvcController_T
//@mfunc BOOL | MvcController_T | AddListener | Add an event listener to the controller.
//@parm IEventListener* | pIListener | Points to an event listener to be added to the viewport
//@rdesc Returns TRUE if the listener was successfully added.  If the listener pointer
// is NULL, returns FALSE.

//@doc MvcController_T
//@mfunc BOOL | MvcController_T | RemoveListener | Removes an event listener from the controller.
//@parm IEventListener* | pIListener | Points to an event listener to be added to the viewport
//@rdesc Returns TRUE if the listener was successfully removed, else returns FALSE.

//@doc MvcController_T
//@mdata ListenerArray | MvcController_T | m_listeners | The array of event listeners for
// this controller.

//@doc MvcController_T
//@mdata model_t* | MvcController_T | m_pModel | Pointer to the model associated with 
// this controller.

//@doc MvcController_T
//@mdata viewport_t* | MvcController_T | m_pViewport | Pointer to viewport associated with 
// this controller.

//@class MvcController_T | This template is used to define controller
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
// <mf MvcController_T::AddListener> and <mf MvcController_T::RemoveListener>
// methods allow event listener objects to be added to the controller.
// The controller then gives all of its listeners a chance to handle
// events that it receives.
//
//@tcarg class | model_t | Type of model accessed by the controller.
//@tcarg class | viewport_t | Type of viewport accessed by the controller.
//
//@base public | IMvcController_T
template<class model_t, class viewport_t>
class MvcController_T : public IMvcController_T<model_t, viewport_t>
{
// Constructor/destructor
public:

	//@cmember
	/* Controller constructor. */
	MvcController_T()
	{
		m_pModel = NULL;
		m_pViewport = NULL;
	}

	//@cmember
	/* Controller destructor. */
	virtual ~MvcController_T()
	{
		for (int nIdx = m_listeners.GetSize() - 1; nIdx > 0; nIdx--)
		{
			IEventListener* pIListener = m_listeners.GetAt(nIdx);
			pIListener->Release();
			m_listeners.RemoveAt(nIdx);
		}
	}

// Attributes
protected:

	//@cmember
	/* Array of event listeners. */
	ListenerArray m_listeners;

	//@cmember
	/* Pointer to model associated with this controller. */
	model_t* m_pModel;

	//@cmember
	/* Pointer to viewport associated with this controller. */
	viewport_t* m_pViewport;

// Operations
public:

	//@cmember
	/* Retrieve a pointer to an interface supported by this object. */
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid,void **ppvObj)
	{
		void *rvP = NULL;

		if (riid == IID_IEventRouter)
			rvP = static_cast<IEventRouter *>(this);
		else if (riid == IID_IUnknown)
			rvP = static_cast<IUnknown *>(this);

		if (rvP != NULL)
			AddRef();

		*ppvObj = rvP;

		return (rvP == NULL ? E_NOINTERFACE : S_OK);
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
	virtual void SetModel(model_t* pModel)
	{
		m_pModel = pModel;
	}

	//@cmember
	/* Get a pointer to the model associated with this controller. */
	virtual model_t* GetModel() const
	{
		return m_pModel;
	}

	//@cmember
	/* Assign a viewport to the controller. */
	virtual void SetViewport(viewport_t* pViewport)
	{
		m_pViewport = pViewport;
	}

	//@cmember
	/* Get a pointer to the viewport associated with this controller. */
	virtual viewport_t* GetViewport() const
	{
		return m_pViewport;
	}

	//@cmember
	/* Routes event objects to event listeners. */
	virtual BOOL RouteEvent(IEvent* pIEvent) const
	{
		int nHandledCount = 0;

		if (pIEvent != NULL)
		{
			MvcController_T<model_t,viewport_t>* pncThis = const_cast<MvcController_T<model_t,viewport_t>*>(this);
			ListenerIterator itListener(&pncThis->m_listeners);
			IEventListener* pIListener;

			//
			// Give each event listener a chance to handle the event.
			//
			for (pIListener = itListener.GetFirst(); pIListener != NULL; pIListener = itListener.GetNext())
			{
				if (pIListener->HandleEvent(pIEvent))
				{
					nHandledCount++;
				}
			}
		}

		return (nHandledCount > 0);
	}

	//@cmember
	/* Add an event listener to the controller. */
	virtual BOOL AddListener(IEventListener* pIListener)
	{
		if (pIListener != NULL)
		{
			pIListener->AddRef();
			m_listeners.Add(pIListener);
			return TRUE;
		}
		return FALSE;
	}

	//@cmember
	/* Remove an event listener from the controller. */
	virtual BOOL RemoveListener(IEventListener* pIListener)
	{
		int nIdx = m_listeners.FindIndex(pIListener);
		if (nIdx >= 0)
		{
			IEventListener* pIListener = m_listeners.GetAt(nIdx);
			pIListener->Release();
			m_listeners.RemoveAt(nIdx);
			return TRUE;
		}
		return FALSE;
	}
};

#endif  // __COMPAT_MVCTEMPLATE_H__

