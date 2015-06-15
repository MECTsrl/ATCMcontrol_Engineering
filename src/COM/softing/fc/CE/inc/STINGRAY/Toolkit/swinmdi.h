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
//  Description:	Declarations for SECMDIFrameWnd	/ SECMDIChildWnd / SECControlBarMDIChildWnd
//  Created:		May 1996
//
#ifdef WIN32

#ifndef __SWINMDI_H__
#define __SWINMDI_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#ifndef __AFXPRIV_H__
#include <afxpriv.h>
#endif

#ifndef __SWINFRM_H__
#include "toolkit\swinfrm.h"
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

// Forward Declarations
class SECControlBarMDIChildWnd;
class SECControlBarInfo;
class SECControlBarManager;
class SECControlBar;
class SECMenuBar;
class stingray::foundation::SECWndPlugIn;
class SECBmpMenuPlugIn;

/////////////////////////////////////////////////////////////////////////////
// SECMDIFrameWnd

/////////////////////////////
// AutoDuck tag block block for SECMDIFrameWnd 


//@doc SECMDIFrameWnd
//@mfunc void | SECMDIFrameWnd | EnableOleContainmentMode | Sets CBRS_HIDE_INPLACE for all control bars. Call when used with Ole Containers.
//@xref <c SECMDIFrameWnd>

// adinsert AutoDuck insertion point for SECMDIFrameWnd 
//@doc SECMDIFrameWnd 
//@class The SECMDIFrameWnd class is an interface equivalent for CMDIFrameWnd that adds support for 
// a variety of new features.  SECMDIFrameWnd supports enhanced docking window capabilities and a gradient 
// caption (similar to the Microsoft Word caption).  Future releases will add more features to this 
// class.
//@comm
// The enhanced docking window capabilities enable control bars to be resized while docked, 
// converted to MDI child windows, diagonally resized while floating and more.  These enhancments 
// give your application docking window capabilities comparable to those seen in the Microsoft 
// Developer Studio. The SECMDIFrameWnd class derives from CMDIFrameWnd and adds the implementation 
// details and public operations required to support the above mentioned enhancements to MDI.  
// Since SECMDIFrameWnd inherits the CMDIFrameWnd interface, all occurrences of CMDIFrameWnd in 
// your application code can simply be replaced with SECMDIFrameWnd.  Refer to the Objective 
// Toolkit User’s Guide for more information about using the enhanced docking window and gradient 
// caption features.
// 
// To create a useful SECMDIFrameWnd frame window for your application, derive a class (typically, 
// your CMainFrame class) from SECMDIFrameWnd.  Add member variables to the derived class to store 
// data specific to your application.  Implement message-handler member functions and a message map 
// in the derived class to specify what happens when messages are directed to the window.  Note, 
// if you are converting an existing MDI application to Objective Toolkit’s SECMDIFrameWnd, 
// simply change the base class of your main frame (i.e., CMainFrame), from CMDIFrameWnd to 
// SECMDIFrameWnd.
// 
// You can construct an SECMDIFrameWnd by calling the Create or LoadFrame member function of 
// CFrameWnd.
// 
// Before you call Create or LoadFrame, you must construct the frame window object on the heap 
// using the C++ new operator. Before calling Create you can also register a window class with 
// the AfxRegisterWndClass global function to set the icon and class styles for the frame.
//
// Use the Create member function to pass the frame’s creation parameters as immediate arguments.
// 
// LoadFrame requires fewer arguments than Create, and instead retrieves most of its default values 
// from resources, including the frame’s caption, icon, accelerator table, and menu.  To be 
// accessed by LoadFrame, all these resources must have the same resource ID (for example, 
// IDR_MAINFRAME).
// 
// Though SECMDIFrameWnd is derived from CMDIFrameWnd, a frame window class derived from 
// SECMDIFrameWnd need not be declared with DECLARE_DYNCREATE.
//
// The SECMDIFrameWnd class inherits much of its default implementation from CMDIFrameWnd.  
// For a detailed list of these features, refer to the CMDIFrameWnd class description. 
// The SECMDIFrameWnd class has the following additional features:
//
// ·	When docked inside an SECMDIFrameWnd window, a control bar can be resized via splitter bars positioned 
// along the control bar’s edge.<nl>
// ·	A control bar can be converted to an MDI child window and floated inside the MDI client area.<nl>
// ·	The context menu associated with any control bar can be edited by the SECMDIFrameWnd object prior to 
// the menu’s display.
//
// Do not use the C++ delete operator to destroy a frame window. Use CWnd::DestroyWindow instead.  
// The CFrameWnd implementation of PostNcDestroy will delete the C++ object when the window is 
// destroyed. When the user closes the frame window, the default OnClose handler will call 
// DestroyWindow.
// 
// For more information on SECMDIFrameWnd, see The Extended Control Bar Architecture section of 
// the Objective Toolkit User’s Guide.
//
// See the \SAMPLES\TOOLKIT\STANDARD\DOCKING\VIZ and \SAMPLES\TOOLKIT\STANDARD\UIEXT\GRADFRM samples, which demonstrate the capabilities of this class.
//@base public | CMDIFrameWnd
class SECMDIFrameWnd : public CMDIFrameWnd
{
    DECLARE_DYNCREATE(SECMDIFrameWnd)

protected:
    SECMDIFrameWnd(); 

// Attributes
protected:
	CRect m_prevLayout;
	CRuntimeClass* m_pDockBarClass;
	CRuntimeClass* m_pFloatingMDIChildClass;
	static const DWORD dwSECDockBarMap[4][2];
	CPoint m_ptDefaultMDIPos;
	SECFrameWnd* m_pActiveDockableFrame;
	SECControlBarManager* m_pControlBarManager;
	BOOL m_bLockNotifyActiveView;	// prevents NotifyActiveView() recursion loop

	// Gradient Caption Attributes
	UINT m_uiTextAlign;		// DrawText alignment flags for caption
	BOOL m_bActive;			// Track Activation State
	BOOL m_bHandleCaption;	// is custom caption drawing enabled?

// Operations
public:
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif
    void EnableDocking(DWORD dwDockStyle, DWORD dwDockStyleEx = 0);
	virtual CDockBar* CreateNewDockBar();

	//@cmember
	/* Enables or disables the control bar context list mode.*/
	void EnableContextListMode(BOOL bEnable = TRUE);

	//@cmember
	/* Similar to DockControlBar but gives greater control over how and where the control bar will */
	// dock. 
	virtual void DockControlBarEx(CControlBar* pBar, UINT nDockBarID = 0,int nCol = 0, int nRow = 0, float fPctWidth = (float)1.0, int nHeight = 150);
    virtual void DockControlBarEx(CControlBar* pBar, const SECDockPos & rDockPos, UINT nDockBarID = 0,const SECDockSize & rDockSize = SECDockSize());
	virtual void FloatControlBar(CControlBar* pBar, CPoint point, DWORD dwStyle = CBRS_ALIGN_TOP);
	void DockControlBar(CControlBar* pBar, UINT nDockBarID = 0,
		LPCRECT lpRect = NULL);
	//@cmember
	/* Converts the control bar back to a normal docking or floating control bar.*/
	void ReDockControlBar(CControlBar* pBar, CDockBar* pDockBar,
		LPCRECT lpRect = NULL);
	void ShowControlBar(CControlBar* pBar, BOOL bShow, BOOL bDelay);
	//@cmember
	/* Converts the control bar to an MDI child window. */
	virtual void FloatControlBarInMDIChild(CControlBar* pBar, CPoint point, DWORD dwStyle = CBRS_ALIGN_TOP);
	void ReFloatControlBar(CControlBar* pBar, CPoint point, DWORD dwStyle = CBRS_ALIGN_TOP);
	virtual CMDIChildWnd* CreateFloatingMDIChild(DWORD dwStyle, CControlBar* pBar = NULL);

	virtual void ActivateDockableFrame(SECFrameWnd* pFrameWnd, BOOL bForce = FALSE);
     virtual void DeactivateDockableFrame(BOOL bAutoActivateSibling = TRUE);
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);

	// Active frame (for frames within frames -- MDI)
	virtual CFrameWnd* GetActiveFrame();

	// saving and loading control bar state
	virtual void LoadBarState(LPCTSTR lpszProfileName);
	virtual void SaveBarState(LPCTSTR lpszProfileName) const;
	virtual void SetDockState(SECDockState& state);
	virtual void GetDockState(SECDockState& state) const;

	// Control Bar Manager
	virtual SECControlBarManager* GetControlBarManager() const;
	virtual void SetControlBarManager(SECControlBarManager*);
	SECControlBar* CtrlBarFromID(const UINT nID);

	// General custom caption members

	// Returns true if the gradient caption rendering is enabled or false if not.
	BOOL IsCustomCaptionEnabled() const
		{ return m_bHandleCaption; }
	//@cmember
	/* Enables or disables gradient caption rendering.*/
	BOOL EnableCustomCaption(BOOL bEnable, BOOL bRedraw = TRUE);
	//@cmember
	/* Forces a redraw of the caption bar.*/
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
	// Returns the alignment of the caption text relative to the caption bar.
	AlignCaption GetCaptionTextAlign() const
		{ return (AlignCaption)m_uiTextAlign; }
	//@cmember
	/* Modifies the alignment of the caption text relative to the caption bar.*/
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

	//@cmember
	/* Loads additional menus that are not created by the document templates.*/
	BOOL LoadAdditionalMenus(UINT nCount, UINT nIDMenu, ...);


	//@cmember
	/* Sets CBRS_HIDE_INPLACE for all control bars. Call when used with Ole Containers.*/
	void EnableOleContainmentMode();


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(SECMDIFrameWnd)
	virtual void OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState);
    //}}AFX_VIRTUAL

// Message handler member functions
protected:
	afx_msg void OnSysColorChange();
	//@cmember
	/* Traps WM_EXTENDCONTEXTMENU message sent by the SECControlBar class after it creates a context menu.  Trap this message if you wish to customize the default context menu.*/
    afx_msg LRESULT OnExtendContextMenu(WPARAM wParam, LPARAM lParam);

	//////////////////////////////////////////////////////////////////
	//					Menu Bar Enhancments						//
	//////////////////////////////////////////////////////////////////
public:
	BOOL HasMenuBar();
	SECMenuBar* GetMenuBar();
	void SetMenuBar(SECMenuBar* pMenuBar) { m_pMenuBar=pMenuBar; }

	CMenu* GetMenu() const;

	// returns approp. default HMENU, accounting for menubar state
	virtual HMENU GetDefaultMenu();

	//Special Message Handler...
	afx_msg int OnCreate( LPCREATESTRUCT lpcs );
	afx_msg void OnClose();

	//Special Virtuals
	virtual void ActivateMenu(UINT nIDMenuInfo);

	virtual BOOL LoadFrame(UINT nIDResource, 
						   DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE,
						   CWnd* pParentWnd = NULL,
						   CCreateContext* pContext = NULL);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

	// Called when the zoom state of a window changes
	virtual void OnMDIZoomChange();

	// Updates the window menu
	virtual void UpdateWindowMenu();
	
	// Needed PlugIn Overrides
	virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam );
	virtual BOOL    PreTranslateMessage( MSG* pMsg );

	// Accessor the dynamic controlbar allocation list
	CPtrList* GetListAllocControlBars() { return &m_listAllocControlBars; }

	// for Ole container support with menu bars
	virtual BOOL NegotiateBorderSpace(UINT nBorderCmd, LPRECT lpRectBorder);

	// Implementation public variable.
	BOOL m_bDoingMDIFloat;

	afx_msg void OnContextHelp();


protected:
	SECMenuBar * m_pMenuBar;
	stingray::foundation::SECWndPlugIn * m_pMenuBarFeature;
	BOOL           m_bBmpMenu;
	BOOL           m_bBmpMenuManaged;
	HMENU m_hMenuFrame;
	CMap<UINT, UINT, HMENU, HMENU> m_mapExtraMenus;	// Additional menu handles
	UINT m_nIDCurMenuRsrc;							// ID of current frame window menu resource
	UINT m_nIDMenuResource;
	CPtrList m_listAllocControlBars;

	// suppress frame menu updates if using a menubar
	virtual BOOL SuppressUpdateMenuForMenubar() { return (BOOL)m_pMenuBar; }

	BOOL ProcessHelpMsg(MSG& msg, DWORD* pContext);

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
private:
	// Cache the return value of InitWorkspaceMgrEx to use in a derived class.
	SECWorkspaceManagerEx* m_pWorkspaceManagerEx;

// Implementation
protected:
	void GetWindowsVersion();
	BOOL m_bIsWin95orAbove;

    virtual ~SECMDIFrameWnd();


    virtual void NotifyActiveView (UINT nState, CWnd* pWndOther, BOOL bMinimized);

	// Returns the HMENU for the given id.
	virtual HMENU MenuFromResource(UINT nID);

	// Class local NotifyActiveView function
    class CViewWithSECMDIFrameFriendship : public CView 
	 	{ friend class SECMDIFrameWnd; }; 

public:
	void DockControlBar(CControlBar* pBar, CDockBar* pDockBar,
		LPCRECT lpRect = NULL);
	virtual void RecalcLayout(BOOL bNotify = TRUE);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,
		AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual void OnUpdateFrameMenu(HMENU hMenuAlt);


protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnEnterIdle(UINT nWhy, CWnd* pWho);

	// Generated message map functions
    //{{AFX_MSG(SECMDIFrameWnd)
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnNcPaint();
	afx_msg void OnActivateApp(BOOL bActive, HTASK hTask);
    afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	//}}AFX_MSG
	afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCommandHelp(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSecGetMenu(WPARAM wParam, LPARAM lParam);
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnEnable(BOOL bEnable);


	// Default handlers for the workspace functions to assure the menu
	// items are properly enabled (plugin will not enable menus, but
	// will accept commands)
	DECLARE_WSMGREX_DEFAULT_HANDLERS()

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// SECMDIChildWnd

/////////////////////////////
// AutoDuck tag block block for SECMDIChildWnd 
// adinsert AutoDuck insertion point for SECMDIChildWnd 
//@doc SECMDIChildWnd 
//@class The SECMDIChildWnd class derives from CMDIChildWnd and adds the implementation details 
// required to support the enhanced docking window capabilities.  
//@comm
// To make use of this class, just re-derive your CMDIChildWnd-based class from SECMDIChildWnd 
// instead.  This class adds no new members to those inherited from CMDIChildWnd.
//
// See the SCRIBBLE sample in the \SAMPLES\TOOLKIT\STANDARD\MDI\MT_SCRIB directory for a 
// demonstration of this class.
//@xref <c SECMDIFrameWnd> <c SECWorksheet>
//@base public | CMDIChildWnd
class SECMDIChildWnd : public CMDIChildWnd
{
    DECLARE_DYNCREATE(SECMDIChildWnd)

protected:
    SECMDIChildWnd(); 

	//////////////////////////////////////////////////////////////////
	//					Menu Bar Enhancments						//
	//////////////////////////////////////////////////////////////////
public:
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,CWnd* pParentWnd, CCreateContext* pContext);
	virtual void RefreshFrameMenu(CWnd* pActiveWnd);		// smart refresh of current frame window menu

	UINT GetMenuRsrc() { return m_nIDMenuRsrc; }

	//@cmember
	/* Switches the menu on the menubar when running with a cool look menubar.*/
	void SwapMenu(UINT nID);

	class CViewWithSECMDIChildFriendship : public CView 
	 	{ friend class SECMDIChildWnd; }; 

protected:
	UINT m_nIDMenuRsrc;
	BOOL m_bUsingMenuBars;
	HMENU m_hUpdateFrameMenu;

	void DetermineMenuToUse( CMultiDocTemplate* pTemplate );
	virtual void OnUpdateFrameMenu(BOOL bActive, CWnd* pActivateWnd,
		HMENU hMenuAlt);

	afx_msg void OnSize( UINT nType, int cx, int cy );
	afx_msg void OnMDIActivate( BOOL bActive, CWnd* pActivateWnd, CWnd* pDeactivateWnd );

// Operations
public:
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(SECMDIChildWnd)
    //}}AFX_VIRTUAL

// Message handler member functions
protected:

// Implementation
protected:
    virtual ~SECMDIChildWnd();


	// Generated message map functions
    //{{AFX_MSG(SECMDIChildWnd)
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnChildActivate();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// SECControlBarMDIChildWnd

class SECControlBarMDIChildWnd : public SECMDIChildWnd
{
	DECLARE_DYNCREATE(SECControlBarMDIChildWnd)

// Constructors
protected:
	SECControlBarMDIChildWnd();           // protected constructor used by dynamic creation

public:
	SECDockBar	m_wndDockBar;		

// Attributes
public:

// Overrides
	void RecalcLayout(BOOL bNotify = TRUE);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SECControlBarMDIChildWnd)
	protected:
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~SECControlBarMDIChildWnd();

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);

	// Generated message map functions
	//{{AFX_MSG(SECControlBarMDIChildWnd)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
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

#endif // __SWINMDI_H__

#else // WIN32                       

#define SECMDIFrameWnd CMDIFrameWnd
#define SECMDIChildWnd CMDIChildWnd

#endif // WIN32

