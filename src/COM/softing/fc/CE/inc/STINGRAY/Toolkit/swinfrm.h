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
//  Description:	Declarations for SECFrameWnd
//  Created:		May 1996
//
//	Modified:		Feb 1997 (Kenn Scribner)
//	Description:	Added italicized text to inactive windows
//					Fixed MDI-derived class behavior (parent
//					and children window captions are activated/
//					de-activated properly)
//
//

#ifdef WIN32

#ifndef __SWINFRM_H__
#define __SWINFRM_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#ifndef __SBARDOCK_H__
#include "toolkit\sbardock.h"
#endif

#ifndef __SECWSMGREX_H__
#include "toolkit\swsmgrex.h"
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

/////////////////////////////////////////////////////////////////////////////
// SECFrameWnd frame

// Forward Declarations
class CDockBar;
class SECDockState;
class SECControlBar;
class SECControlBarManager;
class stingray::foundation::SECWndPlugIn;
class SECBmpMenuPlugIn;
class SECMenuBar;

/////////////////////////////
// AutoDuck tag block block for SECFrameWnd 

//@doc SECFrameWnd
//@mfunc BOOL  | SECFrameWnd | GetActiveState | GetActiveState returns true if this is the 
// active window.
//@xref<c SECFrameWnd>


//@doc SECFrameWnd
//@mdata SECControlBarManager*  | SECFrameWnd | m_pControlBarManager | A control bar manager 
// should be created in the constructor if use of a control bar manager class is desired. This
// should be created on the heap, and must be destroyed in the destructor for the frame.


//@doc SECFrameWnd
//@mdata UINT  | SECFrameWnd | m_uiTextAlign | DrawText alignment flags for caption

//@doc SECFrameWnd
//@mdata BOOL  | SECFrameWnd | m_bHandleCaption | If custom caption drawing is enabled, this will be the caption handle. 

//@doc SECFrameWnd
//@mdata BOOL  | SECFrameWnd | m_bActive | Activation status (WM_ACTIVATE status)



//@doc SECFrameWnd
//@mfunc void  | SECFrameWnd | SetMenuBar | Sets the menubar for the frame.
//@parm SECMenuBar*  | pMenuBar | A pointer to a SECMenuBar Object.



// adinsert AutoDuck insertion point for SECFrameWnd 
//@doc SECFrameWnd 
//@class The SECFrameWnd class is a replacement for MFC’s CFrameWnd class.  
//@comm
// The creation of a 
// CFrameWnd replacement is necessary to support the extended control bar architecture and 
// the gradient caption feature.  Building on  this architecture, your application can create 
// docking windows with capabilities comparable to those seen in the Microsoft Developer 
// Studio.  You can also give your applications the gradient caption similar to the Microsoft 
// Word title bar.  Control bars can be resized while docked, diagonally resized while 
// floating and more. The SECFrameWnd class derives from CFrameWnd and adds the 
// implementation details and public operations required to support these enhancements.
//
// Since SECFrameWnd inherits the CFrameWnd interface, all occurrences of CFrameWnd in 
// your application code can simply be replaced with SECFrameWnd.  For detailed 
// instructions on adding enhanced docking window support to your application, 
// refer to The Extended Control Bar Architecture section of the Objective Toolkit 
// User’s Guide.
//
// The SECFrameWnd and <c SECMDIFrameWnd> classes serve similar purposes  they add enhanced 
// docking window support to your application.  If your application is SDI-based, you’ll 
// need to change the base class of your application’s main frame window from CFrameWnd 
// to SECFrameWnd.  On the other hand, if your application is MDI-based, you’ll need to 
// change the base class of your application’s main frame window from CMDIFrameWnd to 
// <c SECMDIFrameWnd>.
//@base public | CFrameWnd
//@xref <c SECMDIFrameWnd>
class SECFrameWnd : public CFrameWnd
{
    DECLARE_DYNCREATE(SECFrameWnd)

protected:
	//@access Construction
	//@cmember
	/* Constructor for the SECFrameWnd class.*/
    SECFrameWnd();

// Attributes
public:
	//@access Attributes
	//@cmember
	/* Returns the activation state for the window.*/
	BOOL GetActiveState() { return m_bActive; }

	//@access Operations
public:
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

	//@cmember
	/* Enables docking for the frame. */
    void EnableDocking(DWORD dwDockStyle, DWORD dwDockStyleEx = 0);
	//@cmember
	/* Create a new dockbar*/
	virtual CDockBar* CreateNewDockBar();

	//@cmember
	/* Enables or disables the control bar context list mode.*/
	void EnableContextListMode(BOOL bEnable = TRUE);

	//@cmember
	/* Remove a control bar from it's dockbar and float it, or move a floating bar to*/
	// another location.
	virtual void FloatControlBar(CControlBar* pBar, CPoint point, 
		DWORD dwStyle = CBRS_ALIGN_TOP);
	//@cmember
	/* Dock a control bar with extended information. */
	virtual void DockControlBarEx(CControlBar* pBar, 
		UINT nDockBarID = 0,int nCol = 0, int nRow = 0, 
		float fPctWidth = (float)1.0, int nHeight = 150);

	void DockControlBar(CControlBar* pBar, 
		UINT nDockBarID = 0,LPCRECT lpRect = NULL);
	void DockControlBar(CControlBar* pBar, CDockBar* pDockBar,
		LPCRECT lpRect = NULL);
	void ReDockControlBar(CControlBar* pBar, CDockBar* pDockBar,
		LPCRECT lpRect = NULL);
	void ShowControlBar(CControlBar* pBar, BOOL bShow, BOOL bDelay);
	SECControlBar* CtrlBarFromID(const UINT nID);

	// saving and loading control bar state
	virtual void LoadBarState(LPCTSTR lpszProfileName);
	virtual void SaveBarState(LPCTSTR lpszProfileName) const;
	virtual void SetDockState(SECDockState& state);
	virtual void GetDockState(SECDockState& state) const;

	// Control Bar Manager
	virtual SECControlBarManager* GetControlBarManager() const;
	virtual void SetControlBarManager(SECControlBarManager*);

	// General custom caption members

	BOOL IsCustomCaptionEnabled() const
		{ return m_bHandleCaption; }
	BOOL EnableCustomCaption(BOOL bEnable, BOOL bRedraw = TRUE);
	void ForceCaptionRedraw(BOOL bImmediate);
	void ForceCaptionRedraw();

	//@cmember
	/* Creates the caption text font for the application name */
	virtual void CreateCaptionAppFont(CFont& font);

	//@cmember
	/* Creates the caption text font for the document name */
	virtual void CreateCaptionDocFont(CFont& font);

	//@cmember
	/* Draws the text on the caption bar */
	virtual void DrawCaptionText();

	// Caption text control members

	enum AlignCaption { acLeft, acCenter, acRight };
	AlignCaption GetCaptionTextAlign() const
		{ return (AlignCaption)m_uiTextAlign; }
	void SetCaptionTextAlign(AlignCaption ac, BOOL bRedraw = TRUE);

	// Enables support for bitmap menus
	void EnableBmpMenus(BOOL bEnable = TRUE, BOOL bManaged = TRUE);

	// Adds bitmaps for the bitmap menu support
	BOOL AddBmpMenuToolBarResource(LPCTSTR lpszStdBmpName);
	BOOL AddBmpMenuToolBarResource(UINT nIDStdBmp);
	BOOL AddBmpMenuBitmapResource(LPCTSTR lpszStdBmpName,
								  const UINT* lpIDArray, UINT nIDCount);
	BOOL AddBmpMenuBitmapResource(UINT nIDstdBmpName,
								  const UINT* lpIDArray, UINT nIDCount);
	BOOL AddBmpMenuBitmap(HBITMAP hBmpSmall, const UINT* lpIDArray, UINT nIDCount);

	// Returns the SECBmpMenuPlugIn object
	SECBmpMenuPlugIn* GetBmpMenuPlugIn() const;
	//@cmember
	/* Switches the menu on the menubar when running with a cool look menubar.*/
	void SwapMenu(UINT nID);

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(SECFrameWnd)
 	virtual void OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState);
   //}}AFX_VIRTUAL

// Message handler member functions
protected:
	afx_msg void OnSysColorChange();

// Implementation
protected:
	// Destructor for the SECFrameWnd
    virtual ~SECFrameWnd();
	static const DWORD dwSECDockBarMap[4][2];
	void GetWindowsVersion();
	BOOL m_bIsWin95orAbove;
	BOOL m_bDidClose;

	// Returns the HMENU for the given id.
	virtual HMENU MenuFromResource(UINT nID);

public:
	virtual void RecalcLayout(BOOL bNotify = TRUE);
	virtual void OnUpdateFrameMenu(BOOL bActive, CWnd* pActivateWnd,
		HMENU hMenuAlt);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,
		AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual void OnEnterIdle(UINT nWhy, CWnd* pWho);

    // Generated message map functions
    //{{AFX_MSG(SECFrameWnd)
	afx_msg void OnNcPaint();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg BOOL OnNcActivate(BOOL bActive);
	//}}AFX_MSG
	afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM lParam);
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnEnable(BOOL bEnable);

	afx_msg void OnContextHelp();

	// Default handlers for the workspace functions to assure the menu
	// items are properly enabled (plugin will not enable menus, but
	// will accept commands)
	DECLARE_WSMGREX_DEFAULT_HANDLERS()

    DECLARE_MESSAGE_MAP()

	///////////////////////////////////////////////////////////////////////
	//                     Menu Bar Information							 //
	///////////////////////////////////////////////////////////////////////
public:
	//@cmember
	/* Returns TRUE if a menubar is present, FALSE if CMenu.*/
	BOOL HasMenuBar();
	//@cmember
	/* Gets a pointer to the menu bar.*/
	SECMenuBar* GetMenuBar();
	//@cmember
	/* Set the MenuBar for this frame.*/
	void SetMenuBar(SECMenuBar* pMenuBar) { m_pMenuBar=pMenuBar; }
	//@cmember
	/* Returns a pointer to the currently active menu.*/
	CMenu* GetMenu() const;

	// returns approp. default HMENU, accounting for menubar state
	virtual HMENU GetDefaultMenu();
	
	virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam );
	virtual BOOL PreTranslateMessage( MSG* pMsg );
	virtual BOOL LoadFrame(UINT nIDResource, 
						   DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE,
						   CWnd* pParentWnd = NULL,
						   CCreateContext* pContext = NULL);

	// Integration of the extended workspace manager
public:
	
	//@cmember
	/* Initialize the extended workspace manager. */
	virtual SECWorkspaceManagerEx* InitWorkspaceMgrEx(const CString& strAppKey,
										BOOL bRegistryMode=FALSE,	
										CRuntimeClass* pWSClass=
										RUNTIME_CLASS(SECWorkspaceManagerEx),
                                        BOOL bSectionKey=FALSE);

	// Get a pointer to the frame's workspace mgr.
	SECWorkspaceManagerEx* GetWorkspaceMgrEx() { return m_pWorkspaceManagerEx; }

	// Accessor the dynamic controlbar allocation list
	CPtrList* GetListAllocControlBars() { return &m_listAllocControlBars; }

private:
	// Cache the return value of InitWorkspaceMgrEx to use in a derived class.
	SECWorkspaceManagerEx* m_pWorkspaceManagerEx;

protected:
	//@access Protected data members
	CRect m_prevLayout;

	// Gradient Caption Attributes
	//@cmember
	/* DrawText alignment flags for caption*/
	UINT m_uiTextAlign;	
	//@cmember
	/* If custom caption drawing is enabled, this will be the caption handle. */
	BOOL m_bHandleCaption;	
	//@cmember
	/* Activation status (WM_ACTIVATE status)*/
	BOOL m_bActive;	

	//@cmember
	/* Pointer to the ControlBarManager*/
	SECControlBarManager* m_pControlBarManager;


protected:
	stingray::foundation::SECWndPlugIn * m_pMenuBarFeature;
	BOOL           m_bBmpMenu;
	BOOL           m_bBmpMenuManaged;
	SECMenuBar * m_pMenuBar;
	HMENU m_hMenuFrame;
	UINT m_nIDMenuResource;
	CPtrList m_listAllocControlBars;

	afx_msg int OnCreate( LPCREATESTRUCT lpcs );
	afx_msg void OnClose();
	afx_msg LRESULT OnSecGetMenu(WPARAM wParam, LPARAM lParam);

	BOOL ProcessHelpMsg(MSG& msg, DWORD* pContext);
};

//
// SEC Extension DLL
// Reset declaration context
//
#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif // __SWINFRM_H__

#else // WIN32                       

#define SECFrameWnd CFrameWnd

#endif // WIN32

