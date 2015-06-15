/////////////////////////////////////////////////////////////////////////////
// MvcModel.h : Declaration of MvcModel
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
// Author:       Dean Hallman
// Description:  Declaration of MvcModel interface.
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Compatibility/MVC/MvcModel.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

//@doc MvcModel

#ifndef __COMPAT_MVCMODEL_H__
#define __COMPAT_MVCMODEL_H__

// Includes
#ifndef __COMPAT_MVCIFCS_H__
#include <Foundation\Compatibility\MVC\MvcIfcs.h>
#endif

//
// MVC Extension DLL
// Initialize declaration context
//
#ifdef _MVCDLL
	#undef AFXAPP_DATA
	#define AFXAPP_DATA AFXAPI_DATA
	#undef AFX_DATA
	#define AFX_DATA    MVC_DATAEXT
#endif //_MVCDLL

///////////////////////////////////////////////////////////////////////////
//
// MvcModel
//
//@class
// A model is a kind of subject that serves as a base class for your
// application-specific models.  There are generally two types
// of models: system-models and presentation-models.  This distinction is
// purely conceptual.  System-models are models that simulate or model some
// real world process or object.  The system-model, in this case, is somewhat
// like a proxy or stand-in for the real thing.  An presentation model is
// a model that represents application or presentation data and functionality.  
//
//@base public | IMvcModel
//

class MvcModel : public IMvcModel
{
public:
	typedef CCArray_T<IMvcObserver*, IMvcObserver*> ObserverSet;

// Construction/Destruction
public:
	MvcModel();
	virtual ~MvcModel();

	// IMvcModel interface

	//@cmember
	/* Tests whether the model has been modified*/
	virtual BOOL IsModified() const;

	// ISubject interface

	//@cmember
	/* Adds a observer to the list of dependents on this object.  When a change in state occurs, all observers are notified.*/
	virtual void AddObserver(IMvcObserver* pObserver);

	//@cmember
	/* The dependent identified by the argument is removed from this objects list of dependents*/
	virtual void RemoveObserver(IMvcObserver* pObserver);

	//@cmember
	/* All objects dependent on this object are sent a notification indicating what aspect of this object has changed*/
	virtual void UpdateAllObservers(IMvcObserver* pObserver = NULL, IMvcMessage* pMsg = NULL);

	//@cmember
	/* Reset the state of the model to its initial state*/
	virtual void Reset();

	/////////
	// IUnknown interface

	//@cmember
	/* Retrieve a pointer to an interface supported by this object. */
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void ** ppvObject);

	//@cmember
	/* Add a reference to this object. */
	virtual ULONG STDMETHODCALLTYPE AddRef();

	//@cmember
	/* Release a reference to this object. */
	virtual ULONG STDMETHODCALLTYPE Release();

// Attributes
protected:
	ObserverSet m_setObservers;
};

///////////////////////////////////////////////////////////////////////////

//
// MVC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif  // __COMPAT_MVCMODEL_H__

