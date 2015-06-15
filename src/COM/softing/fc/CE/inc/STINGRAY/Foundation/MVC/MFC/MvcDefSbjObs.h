/////////////////////////////////////////////////////////////////////////////
// MvcDefSbjObs.h : Declaration of MvcDefSubject, MvcDefObserver
//
// Stingray Software Extension Classes
// Copyright (C) 1998 Stingray Software Inc,
// All rights reserved.
//
// This source code is only intended as a supplement to
// the Objective Toolkit Class Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding
// the Objective Toolkit product.
//
// Author:       Mark Isham
// Description:  Declaration of MvcDefSubject, MvcDefObserver
//

//@doc MvcDefSbjObs

#pragma once

#ifndef __MFC_MVCDEFSBJOBS_H__
#define __MFC_MVCDEFSBJOBS_H__

// Includes

//
// MVC Extension DLL
// Initialize declaration context
//
#ifdef _SFLDLL
	#undef AFXAPP_DATA
	#define AFXAPP_DATA AFXAPI_DATA
	#undef AFX_DATA
	#define AFX_DATA    SFL_DATAEXT
#endif //_SFLDLL

//
// Note, to use the diagnostics below, define SEC_MVC_SUBJECT_OBSERVER_DIAG 
// in the OT stdafx.h, then rebuild the library
//


#ifdef SEC_MVC_SUBJECT_OBSERVER_DIAG
class MvcSubjectObserverDiagnostic;
#endif

#include <Foundation\MVC\MFC\MvcIfcs.h>

namespace stingray {
namespace foundation {

///////////////////////////////////////////////////////////////////////////
//
// MvcDefSubject
//
//@class 
// Default implementation of IMvcSubject interface
//@base public | IMvcSubject
//
class MvcDefSubject : public IMvcSubject
{
public:
	typedef CCArray_T<IMvcObserver*, IMvcObserver*> ObserverSet;

// Construction/Destruction
public:
	MvcDefSubject();
	virtual ~MvcDefSubject();

// Operations

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
	/* Determine if the observer passed in is currently configured for this subject */
	virtual BOOL HasObserver(IMvcObserver* pObserver);

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
	ObserverSet m_setPendingAddObservers;
	ObserverSet m_setPendingRemoveObservers;
	BOOL m_bUpdateInProgress;

// Subject-observer diagnostics
#ifdef SEC_MVC_SUBJECT_OBSERVER_DIAG
public:
	static MvcSubjectObserverDiagnostic* GetSbjObsDiag();
	static void SetReportUpdateViolations(bool bReport);
	static int GetObserverCount(ObserverSet& setObservers,IMvcObserver* pObserver);
protected:
	static MvcSubjectObserverDiagnostic* m_pSbjObsDiag;
	static int m_nSbjObsDiagRefCount;
	static bool sm_bReportUpdateViolations;
#endif

};

///////////////////////////////////////////////////////////////////////////
//
// MvcDefObserver

//@doc MvcDefObserver
//@mfunc  void | MvcDefObserver | OnUpdate | The function reacts to
// notifications of change on the subject it is observing.
//@rdesc void
//@parm IMvcSubject* | pSubject | Pointer to subject that was updated.
//@parm IMvcMessage* | pMsg | Pointer to message that describes the update.

//@class 
// Default implementation of IMvcObserver interface
//@base public | IMvcObserver
//
class MvcDefObserver : public IMvcObserver
{
public:
	MvcDefObserver();
	virtual ~MvcDefObserver();

	//@cmember
	/* The function reacts to notifications of change on the subject it is observing*/
	virtual void OnUpdate(IMvcSubject* pSubject, IMvcMessage* pMsg)=0;
	
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
};

///////////////////////////////////////////////////////////////////////////
//
// MvcDefMsg
//

//
class MvcDefMsg : public IMvcMessage
{
public:
	MvcDefMsg();
	virtual ~MvcDefMsg();

	virtual unsigned int GetTypeID() const = 0;
	virtual void Sprint(CString& strCmd) = 0;
	
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
};

///////////////////////////////////////////////////////////////////////////
//
// MvcSubjectObserverDiagnostic
//

#ifdef SEC_MVC_SUBJECT_OBSERVER_DIAG	// diagnostics only!

// Diagnostic aid in sniffing out dangling subject-observer linkages,
// for use with MvcDefSubject, MvcDefObserver
class MvcSubjectObserverDiagnostic {
public:
	MvcSubjectObserverDiagnostic();
	virtual ~MvcSubjectObserverDiagnostic();

	// Error-Reporting options, defaults to "trace"
	enum ErrorLevel { err_trace, err_assert, err_msgbox };
	static void SetErrorLevel(ErrorLevel level);

	// Call these functions whenever a new subject is constructed/destructed
	virtual BOOL RegisterNewSubject(MvcDefSubject* pSubject);
	virtual BOOL DeregisterSubject(MvcDefSubject* pSubject);
	
	// Call this function to verify a particular observer is no longer
	// referenced by any of the registered subjects. Useful for sniffing
	// out bogus dangling observer references.
	virtual BOOL VerifyDetachedObserver(IMvcObserver* pObserver);

	// Log an error message for our subject-observer diagnostics
	virtual void ReportError(LPCTSTR lpszErr);

protected:
	CTypedPtrList<CPtrList,MvcDefSubject*> m_listSubjects;
	static ErrorLevel m_errLevel;
};

#endif // SEC_MVC_SUBJECT_OBSERVER_DIAG

///////////////////////////////////////////////////////////////////////////

};  // namespace stingray::foundation
};  // namespace stingray

//
// MVC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif  // #ifndef __MFC_MVCDEFSBJOBS_H__
