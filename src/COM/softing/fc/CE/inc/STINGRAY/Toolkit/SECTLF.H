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
//  Description:	Declarations for SECToplevelFrame
//

#ifndef __SECTLF_H__
#define __SECTLF_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#ifndef __SWINFRM_H__
#include "toolkit\swinfrm.h"
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


// adinsert AutoDuck insertion point for SECToplevelFrame 
//@doc SECToplevelFrame 
//@class The SECToplevelFrame class provides the functionality of a Windows toplevel window.  
//@comm
// A toplevel window is a
// window that has no parent and floats freely on the desktop.  Toplevel windows receive their own 
// entry in the Windows 95 task bar (or Alt-Tab in Windows 3.1) and do not iconify in response to 
// iconifying any other window.  Each toplevel window can include its own menu bar, tool bar and 
// client window.  One application can open any number of toplevel windows.
//
// An SECToplevelFrame can be used in one of two ways.  First, you can define a toplevel window that
// embeds some arbitrary client window as its child.  In this case, the SECToplevelFrame is only 
// providing you with the capability to float the window on the desktop.  Secondly, SECToplevelFrame 
// can be combined with the CMultiDocTemplate to provide an alternative to MDI.  This usage produces 
// an application that behaves identically to MDI except that the MDI child windows have effectively 
// been moved out of the MDI frame onto the desktop.
//
// To convert an application from MDI to a Multiple Toplevel Window Interface, modify your 
// application’s InitInstance member as follows:
//
//  AddDocTemplate(new CMultiDocTemplate(IDR_MYTYPE,<nl>
//		RUNTIME_CLASS(MyDoc),<nl>
//		RUNTIME_CLASS(MyToplevelFrame),<nl>
//		RUNTIME_CLASS(MyView)));
//
// In addition, remove the instantiation of CMainFrame (or your equivalent class) which is normally 
// done in OnInitInstance().  Because an SECToplevelFrame is only a slight variant of the SECFrameWnd 
// class, you can use and derive from it just as you would any other SECFrameWnd derived class.
//
// For example, to have a tool bar and status bar created within an SECToplevelFrame derived class, 
// override the OnCreate() member and instantiate all desired control bars there (as is
// normally done).
//
// See the SCRIBBLE sample in the \SAMPLES\TOOLKIT\STANDARD\MDI\MT_SCRIB directory for a demonstration 
// of this class.
//@BASE public|SECFrameWnd
class SECToplevelFrame : public SECFrameWnd
{  
    DECLARE_DYNCREATE(SECToplevelFrame);

// Construction
protected: // create from serialization only
    SECToplevelFrame();	

// Attributes:
protected:
    static CObList s_tlfList;	

// Operations
public:
	// These functions required as a workaround to properly
	// import/export static member data s_tlfList across DLL boundary
	void GetCopyTLFList(CObList& rList);
	void SetTLFList(CObList& rNewList);
	static void GetCopyTLFListStatic(CObList& rList);
	static void SetTLFListStatic(CObList& rNewList);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SECToplevelFrame)
public:
    virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~SECToplevelFrame();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
    //{{AFX_MSG(SECToplevelFrame)
    afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
    afx_msg void OnWindowNew();
    afx_msg void OnClose();
	afx_msg void OnNcDestroy();
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

#endif //__SECTLF_H__

/////////////////////////////////////////////////////////////////////////////
