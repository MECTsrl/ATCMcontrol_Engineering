/////////////////////////////////////////////////////////////////////////////
// MvcModel.h : SFL Model-View-Controller architecture 
//
// Stingray Software Foundation Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Jeff Boenig
// Description:  SFL MVC: Model classes
//

#pragma once

#ifndef __MVCMODEL_H__
#define __MVCMODEL_H__

#include <Foundation\Patterns\SubjectObserver.h>

namespace stingray {
namespace foundation {

///////////////////////////////////////////////////////////////////////////
//
//@doc CMvcModel
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
//@base public | ISubject
//
class CMvcModel : public ISubject
{
// Constructors/destructor
public:
	//@cmember,mfunc Construct a model
	//@@end
	/* Construct a model*/
	CMvcModel()
	{
	}

	//@cmember,mfunc Destroys the model
	//@@comm
	// Before we destroy the model, we must be sure no object is still
	// observing it.  Otherwise, the observer will be left with a dangling
	// reference.  If the observer collection has any members, then the
	// observer of this subject hasn't unobserved properly.
	//
	// Notice, this assumes the observe has some way of knowing when the
	// subject is being destroyed so that it can unobserve it first.  If
	// this isn't true for your case, you'll have to handle it specially,
	// so we can avoid extra code size and performance hit in the base
	// classes.
	//@@end
	/* Destroys the model*/
	virtual ~CMvcModel()
	{
		while (m_observers.size() > 0)
		{
			IObserver* pObserver = m_observers.back();
			pObserver->Release();
			m_observers.pop_back();
		}
	}

// Attributes
protected:
	//@cmember
	/* Array of observers*/
	ObserverVector m_observers;

// Operations
public:
	BEGIN_GUID_MAP(CMvcModel)
		GUID_ENTRY(ISubject)
		GUID_ENTRY(IQueryGuid)
		GUID_ENTRY(IRefCount)
	END_GUID_MAP

	//@cmember,mfunc
	// Add a reference to this object.
	//@@rdesc New reference count value.
	//@@comm This is a no-op implementation. Derived classes must
	// implement reference counting themselves.
	//@@end
	/* Add a reference to this object*/
	ULONG STDMETHODCALLTYPE AddRef()
	{
		return 1;
	}

	//@cmember,mfunc
	// Release a reference to this object.
	//@@rdesc New reference count value.
	//@@comm This is a no-op implementation. Derived classes must
	// implement reference counting themselves.
	//@@end
	/* Release a reference to this object*/
	ULONG STDMETHODCALLTYPE Release()
	{
		return 1;
	}

	//@cmember,mfunc Adds an observer to the list of dependents
	//@@rdesc void
	//@@parm IObserver* | pObserver | A pointer to an object that is dependent on (observing) this object
	//@@comm
	// The function adds any object derived from IObserver to our list of
	// dependents.  When a change occurs in the state of this subject, all
	// observers will be sent a notification describing the nature of the
	// change.
	//@@xref <c ISubject>, <mf CMvcModel::RemoveObserver>
	//@@end
	/* Adds an observer to the list of dependents*/
	virtual void AddObserver(IObserver* pObserver)
	{
		if (pObserver != NULL) {
			pObserver->AddRef();
			m_observers.push_back(pObserver);
		}
	}

	//@cmember,mfunc Removes an observer from the list of dependents
	//@@rdesc void
	//@@parm IObserver* | pObserver | The dependent to be removed from the list of dependents
	//@@comm
	//  This function removes the specified observer from this subject's
	//  list of dependent objects.  Following this call, the observer will
	//  no longer receive notifications of change on this subject.
	//@@xref <c ISubject>, <mf CMvcModel::AddObserver>
	//@@end
	/* Removes an observer from the list of dependents*/
	virtual void RemoveObserver(IObserver* pObserver)
	{
		ObserverVector::iterator itObserver = m_observers.begin();
		while (itObserver != m_observers.end()) {
			if (pObserver == *itObserver) {
				(*itObserver)->Release();
				itObserver = m_observers.erase(itObserver);
			}
			else
			{
				itObserver++;
			}
		}
	}

	//@cmember,mfunc Broadcast a notification of change to all observers
	//@@rdesc void
	//@@parmopt IObserver* | pSender | NULL | The observer which triggered the update
	//@@parmopt IMessage* | pMsgA | NULL | message describing what change has occurred
	//@@comm
	//  When a subject changes state, it must call UpdateAllObservers() passing
	//  a record describing the nature of the change.  This function will then
	//  proceed to broadcast this notification to all observers.  A word on
	//  desired usage: The UpdateAllObservers can be called from observers or
	//  clients of the subject directly.  However, it is better practice to have
	//  the observers and clients call operations on the subject which produce
	//  the desired change and notification as a side effect.  In other words,
	//  the subject perform all the calls to UpdateAllObservers itself where
	//  possible.
	//@@xref <c ISubject>, <mf CMvcModel::AddObserver>
	//@@end
	/* Broadcast a notification of change to all observers*/
	virtual void UpdateAllObservers(IObserver* pSender = NULL, IMessage* pMsg = NULL)
	{
		ObserverVector::iterator itObserver;

		for (itObserver = m_observers.begin(); itObserver != m_observers.end(); itObserver++) {
			if ((*itObserver) != pSender) {
				(*itObserver)->OnUpdate(this, pMsg);
			}
		}
	}

	//@cmember,mfunc Tests whether this model has been modified since its creation
	//@@rdesc TRUE if the model has been modified; otherwise FALSE
	//@@comm This function is designed to be overridden. The default implementation
	// simply returns FALSE.
	//@@end
	/* Tests whether the model has been modified*/
	virtual BOOL IsModified() const
	{
		return false;
	}

	//@cmember,mfunc Reset the state of the model to its initial state
	//@@comm The default implementation of this function does nothing. Override
	// this function to provide the desired reset behavior.
	//@@end
	/* Reset the state of the model to its initial state*/
	virtual void Reset()
	{
	}
};

};	// namespace stingray::foundation
};  // namespace stingray

#endif  // #ifndef __MVCMODEL_H__
