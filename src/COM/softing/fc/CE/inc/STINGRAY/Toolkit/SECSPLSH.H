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
//  Author:		Scot Wingo
//  Description:	SECSplashWnd (Splash Window) declaration
//  Created:		Mon Jun 29 21:18:15 1995
//
//  CHANGELOG: 
//    MSW     9/30/95   Code Review, repaired minor bugs.
//	  SAD     10/5/97	32 bit Implementation merged

#ifndef __SECSPLSH_H__
#define __SECSPLSH_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#ifndef __SECBIB_H__
#include "Foundation\Image\MFC\secdib.h"
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

#ifndef WIN32
// 16 bit Implementation of SECSplashWnd

#define IDT_SPLASHTIMER 1245


class  SECSplashWnd : public CWnd
{
// Construction
public:
    SECSplashWnd(UINT nNewBitmapID, UINT nNewDuration = 2500);
    
// Attributes
public:
    virtual BOOL Create(CWnd* pParentWnd = NULL, 
		                DWORD dwStyle = WS_POPUP|WS_VISIBLE, 
						DWORD dwExStyle = WS_EX_TOPMOST);
	
// Operations
public:

// Implementation
public:
    virtual ~SECSplashWnd();
    virtual BOOL PreTranslateMessage(MSG * pMsg);

protected:
	BOOL CreateDIBPalette (LPBITMAPINFO lpbmi, LPINT lpiNumColors, CPalette *pPal);


protected:
    CBitmap m_bmSplash;
    int     m_nWndWidth;
    int     m_nWndHeight;
    UINT    m_nBitmapID;
    UINT    m_nSplashTimer;
    UINT    m_nSplashDuration;
	CPalette m_Pal; 
    
// Message map functions
    afx_msg void OnPaint();
    afx_msg void OnTimer(UINT nTimerID);
    
    DECLARE_MESSAGE_MAP()
};
#else // End Win16 SECSplashWnd

//////////////////////////////////////////////////
// 32 bit Implementation of SECSplashWnd
// SAD 10/5/97

// SECSplashWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// SECSplashWnd window

/////////////////////////////
// AutoDuck tag block block for SECSplashWnd 








// adinsert AutoDuck insertion point for SECSplashWnd 
//@doc SECSplashWnd 
//@class The SECSplashWnd class is derived from the CWnd class. It gives a ready to use
// splash window for your applications. SECSplashWnd implements a self initializing and 
// destroying splash window.  To use the splash window, the developer creates the bitmap
// and then creates an instance of SECSplashWnd.  The splash window self destructs after a 
// specified duration or if/when the user presses the mouse button on the splash window.
//@comm The SPLASH51 sample in the \SAMPLES\TOOLKIT\STANDARD\UIEXT\SPLASH51 directory demonstrates how to create a
// splash window. 
//@ex Splash Window | The following example from that sample shows a typical usage of SECSplashWnd.
//Create the Splash Window first
//SECSplashWnd * pSplashWnd = new SECSplashWnd(IDB_SPLASHWND,3500);
//pSplashWnd->Create();
//pSplashWnd->ShowWindow(SW_SHOW);
//pSplashWnd->UpdateWindow();
//ASSERT(pSplashWnd != NULL);
//@BASE public|CWnd
class SECSplashWnd : public CWnd
{
// Construction

//@access Public Members
public:
	//@cmember
	/* The SECSplashWnd constructor creates an instance of the SECSplashWnd object.*/
	SECSplashWnd(UINT nNewBitmapID, 
			     UINT nNewDuration = 2500,
			     BOOL bWaitForTimer = FALSE,
			     BOOL bAlwaysOnTop = FALSE);

	// Called after the new SECSplashWnd is instantiated. Recommended to be called
	// with default parameters.
	//@cmember
	/* This function creates the actual Splash Screen object.*/
	virtual BOOL Create(CWnd* pParentWnd = NULL, 
						DWORD dwStyle = WS_POPUP|WS_VISIBLE, 
						DWORD dwExStyle = NULL);

	// Call before you call Create if desired
	//@cmember
	/* This function can be called to add a taskbar entry for the Splash Screen.*/
	virtual void SetTaskbarTitle(CString strText);
	//@cmember
	/* This function may be called to dismiss the Splash Screen.*/
	virtual void Dismiss();
	// When m_bWaitForTimer is true, the timer does not start until this function is called
	// Sets new duration for timer
	//@cmember
	/* This function is called to enable/set the Splash Screen timer.*/
	virtual BOOL EnableTimer(UINT nNewDuration); 
	//@cmember
	/* Called to enable the Splash Screen timer.*/
	virtual BOOL EnableTimer();	
	// Uses original timer duration
	//@cmember
	/* The Splash Screen parent may be disabled. Default: Parent enabled.*/
	virtual void DisableParent(BOOL bDisable = TRUE);
	//@cmember
	/* The splash screent is set with the WS_EX_TOPMOST style.*/
	virtual void SetAlwaysOnTop(BOOL bAlwaysOnTop = TRUE);
	//@cmember
	/* Sets whether the user may be allowed to dismiss the Splash Screen.*/
	virtual void AllowUserDismiss(BOOL bAllow = FALSE);

// Attributes
protected:
	HWND	m_hWndHiddenParentWnd;
	CWnd*	m_pOwnerParentWnd;
	BOOL	m_bAllowDismiss;
	BOOL	m_bDisableParent;
	UINT	m_nSplashTimer;
	BOOL	m_bAlwaysOnTop;
	int		m_nWndHeight;
	int		m_nWndWidth;
	BOOL	m_bWaitForTimer;
	UINT	m_nSplashDuration;
	UINT	m_nBitmapID;
	BOOL	m_bWantTaskbarEntry;
	CString	m_strTaskbarTitle;
	stingray::foundation::SECDib	m_dib;
	CPalette*	m_pPalette;

	CPalette* SetPalette(CPalette* pPalette);

// Operations
public:
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SECSplashWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

protected:
	// Called after the screen is rendered in OnPaint
	virtual void PostSplashDraw(CDC* pDC, CRect* pRect);

// Implementation
public:
	virtual ~SECSplashWnd();

protected:

	// Generated message map functions
	//{{AFX_MSG(SECSplashWnd)
	afx_msg void OnPaint();
	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	afx_msg BOOL OnQueryNewPalette();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif // End 32 bit Implementation of SECSplashWnd

//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif //__SECSPLSH_H__

