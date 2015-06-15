// Stingray Software Extension Classes
// 
// Stingray Software Extension Classes
// Copyright (C) 1995 Stingray Software Inc.
// All Rights Reserved
// 
// This source code is only intended as a supplement to the
// Stingray Extension Class product.
// See the SEC help files for detaild information
// regarding using SEC classes.
// 
//  Author:			Dean Hallman
//  Description:	Declarations for SECDialogBar
//  Created:		May 1996
//

#ifdef WIN32

#ifndef __SBARDLG_H__
#define __SBARDLG_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

// CDockBar is an private, undocumented MFC class that we subclass.
// Hence, the necessity to include afxpriv.h
#ifndef __AFXPRIV_H__
#include "afxpriv.h"
#endif

#ifndef __SBARCORE_H__
#include "toolkit\sbarcore.h"
#endif

//
// SEC Extension DLL
// Initialize declaration context
//

#ifdef _SECDLL
	#undef AFXAPP_DATA
	#define AFXAPP_DATA AFXAPI_DATA
	#undef AFX_DATA
	#define AFX_DATA    SEC_DATAEXT
#endif //_SECDLL

//////////////////////////////////////////////////////////////////////
// SECCDialogBar

/////////////////////////////
// AutoDuck tag block block for SECDialogBar 


// adinsert AutoDuck insertion point for SECDialogBar 
//@doc SECDialogBar 
//@class SECDialogBar is an interface equivalent replacement for CDialogBar and serves as the 
// base class for all of your dialog bars.  
//@comm 
// SECDialogBar does nothing more than rederive from <c SECControlBar>, so that all member 
// variables and implementation details exist to facilitate sizing while docked, etc.  No member 
// variables or functions are introduced by this class.  (NOTE: All dialog bars formally derived 
// from CDialogBar must be  rederived from SECDialogBar.  No CDialogBars are allowed when using 
// Objective Toolkit’s docking window enhancements as they lack the member variables required to 
// perform the sizing calculations.)
//
// See the VIZ sample in the \SAMPLES\TOOLKIT\STANDARD\DOCKING\VIZ directory for a demonstration of this class.
//@xref <c SECControlBar> <c SECToolbar>
//@base public | SECControlBar
class SECDialogBar : public SECControlBar
{
	DECLARE_SERIAL(SECDialogBar)

// Construction
public:
	//@cmember
	/* Constructs an SECDialogBar object.*/

	SECDialogBar();


	/*BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, UINT nID,
		DWORD dwStyle, DWORD dwExStyle, const RECT& rect, CWnd* pParentWnd = NULL,
		CCreateContext* pContext = NULL);*/


	//@cmember
	/* Creates an instance of SECDialogBar. */
	BOOL Create(CWnd* pParentWnd, UINT nIDTemplate,UINT nStyle = WS_CHILD | WS_VISIBLE | CBRS_BOTTOM,DWORD dwExStyle = CBRS_EX_STDCONTEXTMENU, UINT nID = 1)
		{ return Create(pParentWnd, MAKEINTRESOURCE(nIDTemplate), nStyle, dwExStyle, nID); };

	//@cmember
	/* Creates an instance of SECDialogBar.*/
	BOOL Create(CWnd* pParentWnd, LPCTSTR lpszTemplateName,UINT nStyle = WS_CHILD | WS_VISIBLE | CBRS_BOTTOM,DWORD dwExStyle = CBRS_EX_STDCONTEXTMENU, UINT nID = 1);
	virtual void GetBarInfo(SECControlBarInfo* pInfo);

// Implementation
public:
	virtual ~SECDialogBar();
	CSize m_sizeDefault;
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

protected:
	int m_nResourceID;

#ifndef _AFX_NO_OCC_SUPPORT
	// data and functions necessary for OLE control containment
	_AFX_OCC_DIALOG_INFO* m_pOccDialogInfo;
	LPCTSTR m_lpszTemplateName;
	virtual BOOL SetOccDialogInfo(_AFX_OCC_DIALOG_INFO* pOccDialogInfo);
	afx_msg LRESULT HandleInitDialog(WPARAM, LPARAM);
#endif

// Generated message map functions
protected:
    //{{AFX_MSG(SECDialogBar)
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif // __SBARDLG_H__

#endif // WIN32

