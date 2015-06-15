// 
// Stingray Software Extension Classes
// Copyright (C) 1995 Stingray Software Inc.
// All Rights Reserved
// 
// This source code is only intended as a supplement to the
// Stingray Extension Class product.
// See the SEC help files for detailed information
// regarding using SEC classes.
// 
//  Author:			Dean Hallman
//  Description:	Declarations for SECFDIChildWnd
//

#ifndef __FDICHILD_H__
#define __FDICHILD_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
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

/////////////////////////////
// AutoDuck tag block block for SECFDIChildWnd 


// adinsert AutoDuck insertion point for SECFDIChildWnd 
//@doc SECFDIChildWnd 
//@class Class SECFDIChildWnd derives from SECFrameWnd and provides the implementation for the FDI child window. The 
// SECFDIChildWnd class is the base class for all FDI child windows.  If you are converting an MDI application to FDI,
// rederive your MDI child windows from SECFDIChildWnd.  
//@comm See the FDI sample in the \SAMPLES\TOOLKIT\STANDARD\MDI directory for a demonstration of this class.
//@xref<c SECFDIFrameWnd>
//@BASE public|SECFrameWnd
class SECFDIChildWnd : public SECFrameWnd
{
    DECLARE_DYNCREATE(SECFDIChildWnd);


protected:
	static int s_xInit, s_yInit;
	HMENU m_hMenu;
	
// Construction

public:
	//@Access Constructors/Destructors
	//@cmember
	/* Class constructor*/
    SECFDIChildWnd();

	
    virtual ~SECFDIChildWnd();
    
	//@Access Initializations
	//@cmember
	/* Creates the Windows frame window object. */
	BOOL Create(LPCTSTR lpszClassName,
				LPCTSTR lpszWindowName,
				DWORD dwStyle = WS_OVERLAPPEDWINDOW,
				const RECT& rect = rectDefault,
				CWnd* pParentWnd = NULL,        // != NULL for popups
				LPCTSTR lpszMenuName = NULL,
				DWORD dwExStyle = 0,
				CCreateContext* pContext = NULL);

				
// Attributes


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(SECFDIChildWnd)
	public:
    //@Access Overrideables
	//@cmember
	/* Loads the Windows object along with the resources and associates it with the SECFDIChildWnd object*/
		virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	//@cmember
	/* Sets the calling window as the active window and brings it to the top of the Z-order*/
		virtual void ActivateFrame(int nCmdShow = -1);
		virtual BOOL DestroyWindow();
	protected:
	//@cmember
	/* Override to specify initial size of the window*/
		virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

public:
	
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
    //{{AFX_MSG(SECFDIChildWnd)
    afx_msg void OnWindowNew();
	afx_msg void OnClose();
	afx_msg void OnParentNotify(UINT message, LPARAM lParam);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg BOOL OnNcActivate(BOOL bActive);
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP();
};

//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif // __FDICHILD_H__

