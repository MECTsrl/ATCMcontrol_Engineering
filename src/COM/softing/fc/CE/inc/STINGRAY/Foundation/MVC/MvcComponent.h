/////////////////////////////////////////////////////////////////////////////
// MvcComponent.h : SFL Model-View-Controller architecture 
//
// Stingray Software Foundation Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Jeff Boenig
// Description:  Templates for creating MVC based components
//

#pragma once

#ifndef __MVCCOMPONENT_H__
#define __MVCCOMPONENT_H__

#include <foundation\RefCount.h>
#include <foundation\GuidCast.h>

namespace stingray {
namespace foundation {

//////////////////////////////////////////////////////////////////////
//@doc CMvcComponent
//
//@class CMvcComponent | This class combines a model, viewport, and
// controller into a single object. It takes the type of model,
// viewport, and controller as template parameters and aggregates
// the three objects into a single object. This is very useful for
// for creating MVC-based components. NOTE: The <mf CMvcComponent::QueryGuid>
// function queries all three objects, so an MVC component can be cast
// to any interface supported by either the model, viewport, or controller.
//
//@tcarg class | _Model | Type of model
//@tcarg class | _Viewport | Type of viewport
//@tcarg class | _Controller | Type of controller
//
//@base public | IQueryGuid
//@base public | IRefCount

template <typename _Model, typename _Viewport, typename _Controller>
class CMvcComponent : public IQueryGuid, public IRefCount
{
// Constructors/destructor
public:

	typedef _Model ModelClass;
	typedef _Viewport ViewportClass;
	typedef _Controller ControllerClass;

	//@cmember,mfunc
	// Construct an MVC component object.
	//@@end
	/* Construct an MVC component object*/
	CMvcComponent() : m_pMdl(NULL), m_pVp(NULL), m_pCtlr(NULL)
	{
		m_ulRefCount = 0;
	}

	virtual ~CMvcComponent()
	{
		if (m_pMdl)
			m_pMdl->Release();
		if (m_pVp)
			m_pVp->Release();
		if (m_pCtlr)
			m_pCtlr->Release();
	}

// Attributes
protected:
	//@cmember
	/* Reference count*/
	ULONG m_ulRefCount;
	//@cmember
	/* Pointer to model object*/
	ModelClass* m_pMdl;
	//@cmember
	/* Pointer to viewport object*/
	ViewportClass* m_pVp;
	//@cmember
	/* Pointer to controller object*/
	ControllerClass* m_pCtlr;

// Operations
public:

	//@cmember,mfunc
	// Creates the viewport given a parent window and a rectangle.
	//@@rdesc TRUE if successful, otherwise FALSE
	//@@parm Handle of parent window
	//@@parm Bounds of viewport to create
	//@@end
	/* Creates the viewport*/
	virtual bool Create(HWND hWndParent, LPRECT rc)
	{
		ModelClass* pMdl = m_pMdl;
		ViewportClass* pVp = m_pVp;
		ControllerClass* pCtlr = m_pCtlr;

		if (pMdl == NULL)
		{
			pMdl = new ModelClass;
		}

		if (pVp == NULL)
		{
			pVp = new ViewportClass;
		}

		if (pCtlr == NULL)
		{
			pCtlr = new ControllerClass;
		}

		pVp->Create(hWndParent, rc);
		pVp->SetOrigin(rc->left, rc->top);
		pVp->SetSize(rc->right - rc->left, rc->bottom - rc->top);

		SetModel(pMdl);
		SetViewport(pVp);
		SetController(pCtlr);

		return true;
	}

	//@cmember,mfunc
	// Assign the viewport object to the component.
	//@@rdesc void
	//@@parm Pointer to viewport to assign to component.
	//@@end
	/* Assign the viewport object to the component*/
	void SetViewport(ViewportClass* pViewport)
	{
		if (pViewport != NULL)
		{
			pViewport->AddRef();
		}

		if (m_pVp != NULL)
		{
			m_pVp->Release();

			if (m_pMdl != NULL)
			{
				m_pMdl->RemoveObserver(m_pVp);
			}
		}

		m_pVp = pViewport;

		if (m_pCtlr != NULL)
		{
			m_pCtlr->SetViewport(m_pVp);
		}

		if (m_pMdl != NULL && m_pVp != NULL)
		{
			m_pVp->SetModel(m_pMdl);
		}
	}

	//@cmember,mfunc
	// Assign the controller object to the component.
	//@@rdesc void
	//@@parm Pointer to controller to assign to component.
	//@@end
	/* Assign the controller object to the component*/
	virtual void SetController(ControllerClass* pController)
	{
		if (pController != NULL)
		{
			pController->AddRef();
		}

		if (m_pCtlr != NULL)
		{
			m_pCtlr->Release();
		}

		m_pCtlr = pController;

		if (m_pCtlr != NULL)
		{
			if (m_pMdl != NULL)
			{
				m_pCtlr->SetModel(m_pMdl);
			}

			if (m_pVp != NULL)
			{
				m_pVp->SetController(m_pCtlr);
				m_pCtlr->SetViewport(m_pVp);
			}
		}
	}

	//@cmember,mfunc
	// Assign the model object to the component.
	//@@rdesc void
	//@@parm Pointer to model to assign to component.
	//@@end
	/* Assign the model object to the component*/
	virtual void SetModel(ModelClass* pModel)
	{
		if (m_pMdl != NULL)
		{
			if (m_pVp != NULL)
			{
				m_pMdl->RemoveObserver(m_pVp);
			}
		}

		m_pMdl = pModel;

		if (m_pMdl != NULL)
		{
			if (m_pVp != NULL)
			{
				m_pMdl->AddObserver(m_pVp);
			}

			if (m_pCtlr != NULL)
			{
				m_pCtlr->SetModel(m_pMdl);
			}
		}
	}

	//@cmember,mfunc
	// Cast the component to a viewport.
	//@@rdesc Reference to viewport
	//@@end
	/* Cast the component to a viewport*/
	operator ViewportClass&()
	{
		return *m_pVp;
	}

	//@cmember,mfunc
	// Cast the component to a model.
	//@@rdesc Reference to model
	//@@end
	/* Cast the component to a model*/
	operator ModelClass&()
	{
		return *m_pMdl;
	}

	//@cmember,mfunc
	// Cast the component to a controller.
	//@@rdesc Reference to controller
	//@@end
	/* Cast the component to a controller*/
	operator ControllerClass&()
	{
		return *m_pCtlr;
	}

	//@cmember,mfunc Cast object to a pointer type given a GUID.
	//@@rdesc true if object is successfully cast to the new data type, otherwise false.
	//@@parm REFGUID | guid | Input GUID that determines the data type to cast to.
	//@@parm void ** | ppvObj | Output parameter to receive the result of the cast.
	//@@comm
	// This implementation of QueryGuid delegates to the model, viewport, and
	// controller objects. This means that the component can be cast to any type
	// support by any of the three aggregrate objects.
	//@@end
	/* Cast object to a pointer type given a GUID*/
	virtual bool QueryGuid(REFGUID guid, void **ppvObj)
	{
		bool bResult = false;
		*ppvObj = NULL;

		if (m_pVp != NULL)
		{
			bResult = m_pVp->QueryGuid(guid, ppvObj);
		}

		if (!bResult && m_pMdl != NULL)
		{
			bResult = m_pMdl->QueryGuid(guid, ppvObj);
		}

		if (!bResult && m_pCtlr != NULL)
		{
			bResult = m_pCtlr->QueryGuid(guid, ppvObj);
		}

		return bResult;
	}

	//@cmember,mfunc
	// Add a reference to this object.
	//@@rdesc New reference count value.
	//@@end
	/* Add a reference to this object*/
	ULONG STDMETHODCALLTYPE AddRef()
	{
		return ++m_ulRefCount;
	}

	//@cmember,mfunc
	// Release a reference to this object.
	//@@rdesc New reference count value.
	//@@end
	/* Release a reference to this object*/
	ULONG STDMETHODCALLTYPE Release()
	{
		ULONG ulRefCount = --m_ulRefCount;
		if (m_ulRefCount == 0)
			delete this;
		return ulRefCount;
	}
};

};	// namespace stingray::foundation
};  // namespace stingray

#endif  // #ifndef __MVCCOMPONENT_H__
