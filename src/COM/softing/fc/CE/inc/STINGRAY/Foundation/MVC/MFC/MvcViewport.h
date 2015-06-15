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
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/MVC/MFC/MvcViewport.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

//@doc MvcViewport

#pragma once

#ifndef __MFC_MVCVIEWPORT_H__
#define __MFC_MVCVIEWPORT_H__

// Includes
#include <Foundation\MVC\MFC\MvcIfcs.h>
#include <Foundation\MVC\MFC\MvcLogicalPart.h>

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

#include <Foundation\MVC\MFC\MvcModel.h>
#include <Foundation\MVC\MFC\MvcController.h>
#include <Foundation\Events\EventFactory.h>
#include <Foundation\Events\EventRouter.h>
#include <Foundation\MVC\MvcViewport.h>

namespace stingray {
namespace foundation {

///////////////////////////////////////////////////////////////////////////
//@doc MvcViewport
//
//@class This class is derived from an instantiation of the <c CMvcViewport>
// template class. It implements an MFC-specific, windowless viewport and
// maintains a pointer to the CWnd it belongs to. It instantiates the
// CMvcViewport template using <t MvcLogicalPart>, <t MvcModel>, and
// <t MvcController> as the parameters. It implements the MFC message
// handling functions <mf MvcViewport::OnWndMsg> and <mf MvcViewport::OnCmdMsg>
// so that it can integrated with the MFC message map architecture.
//
//@base public | CMvcViewport
//

class MvcViewport : public CMvcViewport<MvcLogicalPart,MvcModel,MvcController>
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
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	//@cmember
	/* The command message is being delegated to this component for handling*/
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

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
	virtual stingray::foundation::CEventFactory* GetEventFactory();

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

#endif // #ifndef __MFC_MVCVIEWPORT_H__
