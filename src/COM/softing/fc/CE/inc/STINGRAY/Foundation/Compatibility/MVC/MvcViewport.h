/////////////////////////////////////////////////////////////////////////////
// MvcViewport.h : Declaration of MvcViewport
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
// Description:  Declaration of MvcViewport interface.
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Compatibility/MVC/MvcViewport.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

//@doc MvcViewport

#ifndef __COMPAT_MVCVIEWPORT_H__
#define __COMPAT_MVCVIEWPORT_H__

// Includes

#ifndef __COMPAT_MVCLOGICALPART_H__
#include "Foundation\Compatibility\MVC\MvcLogicalPart.h"
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

#ifndef __COMPAT_MVCTEMPLATE_H__
#include "Foundation\Compatibility\MVC\MvcTemplate.h"
#endif

#ifndef __COMPAT_MVCMODEL_H__
#include "Foundation\Compatibility\MVC\MvcModel.h"
#endif

#ifndef __COMPAT_MVCCONTROLLER_H__
#include "Foundation\Compatibility\MVC\MvcController.h"
#endif

#ifndef __COMPAT_EVENTFACTORY_H__
#include "Foundation\Compatibility\Common\Events\EventFactory.h"
#endif


///////////////////////////////////////////////////////////////////////////
//
//  MvcViewport

//@doc MvcViewport
//@mfunc  void | MvcViewport | LineUp | Scroll one line up. Default implementation does nothing.
//@xref <c MvcViewport>

//@doc MvcViewport
//@mfunc  void | MvcViewport | LineDown | Scroll one line down. Default implementation does nothing.
//@xref <c MvcViewport>

//@doc MvcViewport
//@mfunc  void | MvcViewport | PageUp | Scroll one page up. Default implementation does nothing.
//@xref <c MvcViewport>

//@doc MvcViewport
//@mfunc  void | MvcViewport | PageDown | Scroll one page down. Default implementation does nothing.
//@xref <c MvcViewport>

//@doc MvcViewport
//@mdata MvcController* | MvcViewport | m_pCtlr  | A pointer to this viewport's controller
//@xref <c MvcViewport>

//@doc MvcViewport
//@mdata MvcModel* | MvcViewport | m_pModel | The model this visual part dependents upon
//@xref <c MvcViewport>


//
//@class
// A viewport is a logical part that depends upon a model and
// has a controller.  An MvcViewport is conceptually similar to MFC's
// CView class, but its most important difference is it isn't a
// window (or CWnd derivative).  In MFC, a CView uses inheritance
// to define the presentation logic to display a model.  MVC uses
// aggregation, thereby affording greater flexibility.  In effect,
// any CWnd-derived class can simply aggregate a viewport and
// delegate the rendering to it.
//
// MvcViewport can be mixed into any class.  So, if you want to define
// a CListCtrl viewport, you could multiply derive a class from
// CListCtrl and MvcViewport.  This would yield a list control that
// bridges into the MVC architecture nicely.
//
//@base public | MvcViewport_T
//@base public | MvcLogicalPart
//@base public | IMvcMsgHandler
//
class MvcViewport : public MvcViewport_T<MvcLogicalPart,MvcModel,MvcController>, public IMvcMsgHandler
{
public:

	// DC
	// 
	//A visual part device context
	//
	class DC : public CDC
	{
	// Constructors
	public:
		// Constructor takes a visual part and auto-prepare parameter
		DC(MvcViewport* pViewport, BOOL bAutoPrepare = TRUE);

		virtual ~DC();

	// Attributes
	protected:
		HWND m_hWnd;
	};

// Constructor(s) & destructor
public:
	MvcViewport(MvcVisualPart* pContainer = NULL);
	virtual ~MvcViewport();

	//@cmember
	/* Creates the viewport and optional controller*/
	virtual BOOL Create(CWnd* pWnd, MvcVisualPart* pContainer);

// Attributes
protected:

	CWnd* m_pWnd;

// Operations
public:

	virtual CWnd* GetWnd() const;

	virtual BOOL ContainsDP(const CPoint& pt);

	// Control Delegation

	//@cmember
	/* A windows message is being delegated to this component for handling*/
	BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	//@cmember
	/* The command message is being delegated to this component for handling*/
	BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// Subject / Observer Members

	//@cmember
	/* The function reacts to notifications of change on the subject it is observing*/
	virtual void OnUpdate(IMvcSubject* pSubject, IMvcMessage* pMsg);

	// Invalidation Members

	//@cmember
	/* Invalidate the specified rectangle within this visual part*/
	virtual void InvalidateRect(const CRect& rcLog, BOOL bErase = TRUE, BOOL bRepairNow = FALSE);

	//@cmember
	/* Returns the current invalid region as a rectangle.*/
	virtual BOOL GetUpdateRect(LPRECT lpRect, BOOL bErase = FALSE);

	//@cmember
	/* Convert logical units to device units*/
	virtual void LPtoDP( LPPOINT lpPoints, int nCount = 1 ) const;
	virtual void LPtoDP( LPRECT lpRect ) const;
	virtual void LPtoDP( LPSIZE lpSize ) const;

	//@cmember
	/* Convert device units to logical units*/
	virtual void DPtoLP( LPPOINT lpPoints, int nCount = 1 ) const;
	virtual void DPtoLP( LPRECT lpRect ) const;
	virtual void DPtoLP( LPSIZE lpSize ) const;

	//@cmember
	/* Renders the component to a device context*/
	virtual void Draw(CDC* pDC);

	//@cmember
	/* Erases the viewport contents. */
	virtual void Erase(CDC* pDC);

	////////////// Logical Size Accessors //////////////

	//@cmember
	/* Returns the width and height of the component in logical units*/
	virtual CSize GetLogSize() const;

	//@cmember
	/* Returns a pointer to the event factory for this viewport. */
	virtual EventFactory* GetEventFactory();

// Implementation
protected:

	virtual BOOL OnCreate();
};

////////////
//
// Inline functions
//

//@doc MvcViewport
//@mfunc Returns the current invalid region as a rectangle.
//@rdesc Specifies the status of the update region. The value is nonzero if the
// update region is not empty; otherwise 0.
//@parm LPRECT | lpRect | Points to a CRect object or RECT structure that is to
// receive the client coordinates of the update that encloses the update region.
//@parm BOOL | bErase | Specifies whether the background in the update region is
// to be erased.
inline BOOL MvcViewport::GetUpdateRect(LPRECT lpRect, BOOL bErase)
{
	return GetWnd()->GetUpdateRect(lpRect, bErase);
}

///////////////////////////////////////////////////////////////////////////


//
// MVC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif  // __COMPAT_MVCVIEWPORT_H__

