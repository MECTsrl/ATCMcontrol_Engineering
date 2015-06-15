/////////////////////////////////////////////////////////////////////////////
// MvcViewport.h : SFL Model-View-Controller architecture 
//
// Stingray Software Foundation Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Jeff Boenig
// Description:  SFL MVC: Viewport classes
//

#pragma once

#ifndef __MVCVIEWPORT_H__
#define __MVCVIEWPORT_H__

#ifndef _SFL_NO_PRAGMA_WARNINGS
	#if 1200 <= _MSC_VER
		#pragma warning (push)
	#endif
	#pragma warning (disable : 4786)
	#pragma warning (disable : 4097)
#endif

#include <Foundation\Patterns\SubjectObserver.h>
#include <Foundation\StructuredGraphics\Visual.h>
#include <Foundation\StructuredGraphics\VisualWindow.h>
#include <Foundation\StructuredGraphics\LogCoordinates.h>
#include <Foundation\StructuredGraphics\Zoom.h>
#include <Foundation\Events\EventRouter.h>
#include <Foundation\Events\EventFactory.h>
#include <Foundation\Events\MouseEvent.h>

namespace stingray {
namespace foundation {

///////////////////////////////////////////////////////////////////////////
// CMvcViewport

//@doc CMvcViewport
//@mfunc | CMvcViewport | CMvcViewport | Viewport constructor.
//@comm Sets the model and controller pointers to NULL.

//@doc CMvcViewport
//@mfunc | CMvcViewport | ~CMvcViewport | Viewport destructor.
//@comm Removes the viewport from the model as an observer. If the
// m_bAutoDelCtlr flag is set, then the controller is deleted.

//@doc CMvcViewport
//@mfunc HRESULT | CMvcViewport | QueryInterface | Retrieve a pointer to
// an interface supported by this object.
//@parm REFIID | riid | Identifier of the interface being requested. 
//@parm void ** | ppvObj | Address of pointer variable that receives the
// interface pointer requested in riid.
//@rdesc S_OK if the interface is supported, E_NOINTERFACE if not.

//@doc CMvcViewport
//@mfunc ULONG | CMvcViewport | AddRef | Add a reference to this object.
//@rdesc New reference count value.
//@comm The CMvcViewport template does not implement reference counting.
// This function does nothing and returns 0. Derived classes can implement
// reference counting by overriding AddRef and Release.

//@doc CMvcViewport
//@mfunc ULONG | CMvcViewport | Release | Release a reference to this object.
//@rdesc New reference count value.
//@comm The CMvcViewport template does not implement reference counting.
// This function does nothing and returns 0. Derived classes can implement
// reference counting by overriding AddRef and Release.

//@doc CMvcViewport
//@mfunc void | CMvcViewport | OnUpdate | The function reacts to notifications of
// change on the subject it is observing.
//@rdesc void
//@parm ISubject* | pSubject | The subject sending the update.
//@parm IMessage* | pMsg | The message indicating what change has occurred.
//@comm The default implementation does nothing. Override this function to provide your
// application specific update code.

//@doc CMvcViewport
//@mfunc void | CMvcViewport | Draw | Draw the viewport to a device context.
//@rdesc void
//@parm CDC* | pDC | Pointer to a device context.
//@comm The base class implementation does nothing. Override this function to
// render the viewport to a device context.

//@doc CMvcViewport
//@mfunc void | CMvcViewport | SetModel | Assign a model to the viewport.
//@rdesc void
//@parm ModelClass* | pModel | Pointer to model to assign to the viewport.
//@comm Adds the viewport as an observer to the model. Also, pass the
// model to the controller by calling <mf CMvcController::SetModel>.

//@doc CMvcViewport
//@mfunc ModelClass* | CMvcViewport | GetModel | Gets a pointer to the model associated with this viewport.
//@rdesc A pointer to the associated model.

//@doc CMvcViewport
//@mfunc BOOL | CMvcViewport | CreateController | Create the default controller for this viewport.
//@rdesc FALSE if an error occurs; otherwise TRUE.
//@comm The default implementation does nothing and returns TRUE. Derived
// classes override this function in order to create a default controller.

//@doc CMvcViewport
//@mfunc void | CMvcViewport | SetController | Set the controller for this viewport.
//@rdesc void
//@parm ControllerClass* | pController | Pointer to controller to assign to the viewport.
//@parmopt const BOOL | bAutoDelCtlr | FALSE | Flag indicating whether the viewport
// should destroy the controller in its destructor.

//@doc CMvcViewport
//@mfunc ControllerClass* | CMvcViewport | GetController | Get a pointer to the controller for this viewport.
//@rdesc Pointer to controller for this viewport.

//@doc CMvcViewport
//@mfunc BOOL | CMvcViewport | RouteEvent | Routes event objects to event listeners.
//@rdesc TRUE if event handled by at least one listener; otherwise FALSE.
//@parm IEvent* | pIEventPointer | Pointer to event object to route.
//@comm If the event is a <c MouseEvent>, the viewport verifies that it occurred
// within its bounds. Mouse events that occur outside of the viewport bounds
// are ignored. If a controller is connected to the viewport, the viewport then
// passes the event to the controller by calling <mf MvcController_T::HandleEvent>
// on the controller.

//@doc CMvcViewport
//@mdata ModelClass* | CMvcViewport | m_pModel | Pointer to the model this
// viewport observes and renders.

//@doc CMvcViewport
//@mdata ControllerClass* | CMvcViewport | m_pCtlr | Pointer to the controller this
// viewport sends events to.

//@doc CMvcViewport
//@mdata BOOL | CMvcViewport | m_bAutoDelCtlr | Flag to indicate if controller
// should be deleted when the viewport is deleted

//@class CMvcViewport | This template is used to define viewport classes
// that interact with a given type of model and controller. The base class
// template argument is usually a class that supports the <c IVisual>
// interface. The model class template argument defines the type of model
// that the viewport will observe and render, and is usually strongly-typed.
// The controller class template argument defines the type of controller
// that the viewport interacts with. The association from the viewport to
// the controller is usually a weakly-typed one. Generally speaking, the
// viewport only needs to route events to the controller.
//
// This class inherits the <c IMvcViewport> template interface, passing
// it the model class and controller class arguments.
//
// This class also inherits the <c IEventRouter> interface, so that it
// can route events to event listeners. Events are always routed to the
// controller. Derived viewport classes can override the <mf CMvcViewport::RouteEvent>
// method and implement custom routing.
//
//@tcarg class | _Visual | Base class for the viewport.
//@tcarg class | _Model | Base class for models to be used with the viewport.
//@tcarg class | _Ctlr | Base class for controllers to be used with the viewport.
//
//@base public | IMvcViewport
//@base public | IEventRouterImpl
//
template<typename _Visual, typename _Model, typename _Ctlr>
class CMvcViewport : public _Visual,
                     public IObserver,
                     public IEventRouterImpl
{
// Embedded types
public:
	typedef CMvcViewport<_Visual, _Model, _Ctlr> ThisClass;
	typedef _Visual VisualClass;
	typedef _Model ModelClass;
	typedef _Ctlr ControllerClass;

	// Embedded DC class
	class DC : public CDC
	{
	// Constructors
	public:
		DC(ThisClass* pViewport, BOOL bAutoPrepare = TRUE)
		{
			ASSERT(pViewport != NULL);
			IVisualWindow* pVisualWnd = guid_cast<IVisualWindow*>(pViewport);
			ASSERT(pVisualWnd != NULL);
			m_hWnd = pVisualWnd->GetWindowHandle();
			ASSERT(m_hWnd);
			Attach(::GetDC(m_hWnd));
			if (bAutoPrepare == TRUE)
				pViewport->OnPrepareDC(this);
		}

		virtual ~DC()
		{
			ASSERT(m_hDC != NULL);
			::ReleaseDC(m_hWnd, Detach());
		}

	// Attributes
	protected:
		HWND m_hWnd;
	};

// Constructor/destructor
public:

	//@cmember
	/* Viewport constructor. */
	CMvcViewport():
		m_bAutoDelCtlr(false), 
		m_pModel(NULL),
		m_pCtlr(NULL)
	{
	}

	//@cmember
	/* Viewport destructor. */
	virtual ~CMvcViewport()
	{
		if (m_pModel != NULL) {
			m_pModel->RemoveObserver(this);
		}
		if (m_pCtlr != NULL && m_bAutoDelCtlr) {
			delete m_pCtlr;
		}
	}

// Attributes
protected:

	//@cmember
	/* Pointer to the model. */
	ModelClass* m_pModel;

	//@cmember
	/* Pointer to the controller. */
	ControllerClass* m_pCtlr;

	//@cmember
	/* Flag to indicate if controller should be deleted when the viewport is deleted. */
	bool m_bAutoDelCtlr;

// Operations
public:

	//@cmember,mfunc
	// Creates the viewport given a parent window and a rectangle.
	//@@rdesc TRUE if successful, otherwise FALSE
	//@@parm Handle of parent window
	//@@parm Bounds of viewport to create
	//@@end
	/* Creates the viewport*/
	virtual BOOL Create(HWND hWndParent, LPRECT rc)
	{
		hWndParent;
		SetOrigin(rc->left, rc->top);
		SetSize(rc->right - rc->left, rc->bottom - rc->top);
		return TRUE;
	}

	//@cmember
	/* Get a pointer to the model associated with this viewport. */
	virtual ModelClass* GetModel() const
	{
		return m_pModel;
	}

	//@cmember
	/* Assign a model to the viewport. */
	virtual void SetModel(ModelClass* pModel)
	{
		m_pModel = pModel;
		if (m_pModel != NULL) {
			m_pModel->AddObserver(this);
		}
		if (m_pCtlr != NULL) {
			m_pCtlr->SetModel(m_pModel);
		}
	}

	//@cmember
	/* Create the default controller for this viewport. */
	virtual BOOL CreateController()
	{
		return TRUE;
	}

	//@cmember
	/* Set the controller for this viewport. */
	virtual void SetController(ControllerClass* pController, const bool bAutoDelCtlr = false)
	{
		m_bAutoDelCtlr = bAutoDelCtlr;
		m_pCtlr = pController;
	}

	//@cmember
	/* Get a pointer to the controller for this viewport. */
	virtual ControllerClass* GetController()
	{
		return m_pCtlr;
	}

// Interface implementations
public:

	BEGIN_GUID_MAP(ThisClass)
		GUID_CHAIN_ENTRY(_Visual)
		GUID_ENTRY(IEventRouter)
		GUID_ENTRY(IObserver)
	END_GUID_MAP

	/////////////////////////////////////////////////////////////////////
	// IRefCount

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

	/////////////////////////////////////////////////////////////////////
	// IObserver

	//@cmember
	/* The function reacts to notifications of change on the subject it is observing. */
	virtual void OnUpdate(ISubject* pSubject, IMessage* pMsg)
	{
		pSubject;
		pMsg;
	}

	//@cmember
	/* Prepare graphics context for rendering. */
	virtual void OnPrepareDC(CDC* pDC)
	{
		// Set the viewport origin
		pDC->SetViewportOrg(GetOrigin());

		ILogCoordinates* pLog = guid_cast<ILogCoordinates*>(this);
		if (pLog != NULL)
		{
			// Set window origin using logical origin
			pDC->SetWindowOrg(pLog->GetLogOrigin());
			// Set mapping mode
			pDC->SetMapMode(pLog->GetMapMode());
			// Set window and viewport extents
			CSize szExtents(pLog->GetExtents());
			CSize szLogExtents(pLog->GetLogExtents());
			pDC->SetWindowExt(szLogExtents.cx, szLogExtents.cy);
			pDC->SetViewportExt(szExtents.cx, szExtents.cy);
		}
		else
		{
			// Default is MM_TEXT
			pDC->SetMapMode(MM_TEXT);
		}

		IZoom* pZoom = guid_cast<IZoom*>(this);
		if (pZoom != NULL)
		{
			// Set mapping mode to MM_ANISOTROPIC so that extents can
			// be scaled to account for zooming. Changing to MM_ANISOTROPIC
			// preserves previously set extents.
			pDC->SetMapMode(MM_ANISOTROPIC);

			// Get current extents
#if !defined(_WTL_VER) 
			CSize szWndExt(pDC->GetWindowExt());
			CSize szVpExt(pDC->GetViewportExt());
#else
			CSize szWndExt;
			CSize szVpExt;
			pDC->GetWindowExt(&szWndExt);
			pDC->GetViewportExt(&szVpExt);
#endif
			// Scale the extents to account for zooming
			pZoom->ZoomExtents(szWndExt, szVpExt);

			// Update the extents in the DC
			pDC->SetWindowExt(szWndExt.cx, szWndExt.cy);
			pDC->SetViewportExt(szVpExt.cx, szVpExt.cy);
		}
	}

	virtual void OnCleanupDC(CDC* pDC)
	{
		pDC;
		// Do nothing. Derived classes may override this,
		// it is called after Draw is called
	}

	/////////////////////////////////////////////////////////////////////
	// IEventRouter

	//@cmember
	/* Routes event objects to event listeners. */
	virtual bool RouteEvent(IEvent* pIEvent)
	{
		bool bRes = false;

		IMouseEvent* pIMouseEvent = guid_cast<IMouseEvent*>(pIEvent);

		if (pIMouseEvent != NULL) {
			pIMouseEvent->AddRef();

			//
			// If this event is a mouse event, then verify that it occurred
			// within the viewport bounds. If not, return FALSE to caller.
			//
			POINT pt;
			pIMouseEvent->GetPoint(pt);
			pIMouseEvent->Release();

			ILogCoordinates* pLog = guid_cast<ILogCoordinates*>(this);
			if (pLog != NULL)
			{
				pLog->DPtoLP(&pt,1);
				CRect rcLog(pLog->GetLogOrigin(), pLog->GetLogSize());
				rcLog.NormalizeRect();
				if (!rcLog.PtInRect(pt)) {
					return false;
				}
			}
		}

		if (m_pCtlr != NULL) {
			//
			// Pass event to the controller.
			//
			bRes = m_pCtlr->RouteEvent(pIEvent);
		}

		if (!bRes) {
			//
			// Not handled by the controller. Give event listeners
			// a chance to handle the event.
			//
			bRes = IEventRouterImpl::RouteEvent(pIEvent);
		}

		return bRes;
	}

	/////////////////////////////////////////////////////////////////////
	// IVisualHost

	virtual void InvalidateRect(const CRect& rect, bool bErase = true)
	{
		IVisualWindow* pIWnd = guid_cast<IVisualWindow*>(this);
		if (pIWnd != NULL)
		{
			CRect rc(rect);
			::InvalidateRect(pIWnd->GetWindowHandle(), &rc, (bErase ? TRUE : FALSE) );
		}
	}

	virtual void ValidateRect(const CRect& rect)
	{
		IVisualWindow* pIWnd = guid_cast<IVisualWindow*>(this);
		if (pIWnd != NULL)
		{
			CRect rc(rect);
			::ValidateRect(pIWnd->GetWindowHandle(), &rc);
		}
	}

	/////////////////////////////////////////////////////////////////////
	// ILogCoordinates - Override LPtoDP and DPtoLP functions and
	// implement using a device context.

	virtual void LPtoDP(LPPOINT lpPoints, int nCount) const
	{
		ThisClass::DC dc(const_cast<ThisClass*>(this), TRUE);
		dc.LPtoDP(lpPoints, nCount);
	}

	virtual void LPtoDP(LPRECT lpRect) const
	{
		ThisClass::DC dc(const_cast<ThisClass*>(this), TRUE);
		dc.LPtoDP(lpRect);
	}

	virtual void LPtoDP(LPSIZE lpSize) const
	{
		ThisClass::DC dc(const_cast<ThisClass*>(this), TRUE);
		dc.LPtoDP(lpSize);
	}

	virtual void DPtoLP(LPPOINT lpPoints, int nCount) const
	{
		ThisClass::DC dc(const_cast<ThisClass*>(this), TRUE);
		dc.DPtoLP(lpPoints, nCount);
	}

	virtual void DPtoLP(LPRECT lpRect) const
	{
		ThisClass::DC dc(const_cast<ThisClass*>(this), TRUE);
		dc.DPtoLP(lpRect);
	}

	virtual void DPtoLP(LPSIZE lpSize) const
	{
		ThisClass::DC dc(const_cast<ThisClass*>(this), TRUE);
		dc.DPtoLP(lpSize);
	}
};

///////////////////////////////////////////////////////////////////////////
//@doc CMvcWindowlessViewport
//
//@class CMvcWindowlessViewport | This is a wrapper class that implements
// a given viewport as windowless. It mixes in the <c IVisualWindow>
// interface and implements it by returning the parent window handle.
//
//@tcarg class | _Base | Base viewport class to wrap as a windowless viewport.
//
//@base public | _Base
//@base public | IVisualWindow

template <typename _Base>
class CMvcWindowlessViewport : public _Base, public IVisualWindow
{
public:
	CMvcWindowlessViewport() :
		m_hWnd(NULL)
	{
	}

protected:
	//@cmember
	/* Parent window handle. */
	HWND m_hWnd;

public:
	BEGIN_GUID_MAP(CMvcWindowlessViewport<_Base>)
		GUID_CHAIN_ENTRY(_Base)
		GUID_ENTRY(IVisualWindow)
	END_GUID_MAP

	//@cmember,mfunc
	// Creates the viewport given a parent window and a rectangle.
	//@@rdesc TRUE if successful, otherwise FALSE
	//@@parm Handle of parent window
	//@@parm Bounds of viewport to create
	//@@comm Calls the base class create and stores the parent window
	// handle.
	//@@end
	/* Creates the viewport*/
	virtual BOOL Create(HWND hWndParent, LPRECT rc)
	{
		m_hWnd = hWndParent;
		return _Base::Create(hWndParent, rc);
	}

	//@cmember,mfunc
	// Return the window handle for the viewport.
	//@@rdesc Window handle
	//@@comm Returns the parent window handle
	//@@end
	/* Return the window handle*/
	virtual HWND GetWindowHandle()
	{
		return m_hWnd;
	}
};

};	// namespace stingray::foundation
};  // namespace stingray

#ifndef _SFL_NO_PRAGMA_WARNINGS
	#if 1200 <= _MSC_VER
		#pragma warning (pop)
	#endif
#endif

#endif  // #ifndef __MVCVIEWPORT_H__
