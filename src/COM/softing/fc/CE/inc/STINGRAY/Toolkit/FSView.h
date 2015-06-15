// FSView.h : header file
// 
// Stingray Software Extension Classes
// Copyright (C) 1999 Stingray Software Inc.
// All Rights Reserved
// 
// This source code is only intended as a supplement to
// the Objective Toolkit Class Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding
// the Objective Toolkit product.
//
//  Author:			Prakash Surendra
//  Description:	Declaration of SECFullScreenView
//

#ifndef __SECFULLSCREEN_H__
#define __SECFULLSCREEN_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#ifndef __AFXPRIV_H__
#include <afxpriv.h>
#endif // __AFXPRIV_H__

#ifndef __AFXTEMPL_H__
#include <afxtempl.h>
#endif // __AFXTEMPL_H__

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

// Private messages, Constants & Macros

/////////////////////////////////////////////////////////////////////////////
#define SEC_IDR_FSVIEWER		0xE123
#define SEC_ID_FSTOOLBAR		0xE124

#define SEC_FSHIDEMENU			(WM_USER+0x0301)
#define FSM_SETMODE				(WM_USER+0x0302)

//Timer for keeping track of OT Menu bars
#define TIMER_MENUUPDATE		0xE001

//FS styles
#define SEC_FS_DEFAULT			0				//Use default toolbar icon and no menus
#define SEC_FS_DROPDOWNMENU		0x0001			//Enable drop-down menu display
#define SEC_FS_TEXTTOOLBAR		0x0002			//Use text-only tool bar
#define SEC_FS_TASKBARHIDE		0x0004			//Forcibly hide the Windows task bar
#define SEC_FS_NOCUSTTOOLBAR	0x0008			//Default to SECToolBar

//Default Toolbar Styles
#if _MFC_VER >= 0x0600
#define FS_DEFAULT_TBARSTYLES	CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_GRIPPER|CBRS_BORDER_3D|CBRS_SIZE_DYNAMIC
#else
#define FS_DEFAULT_TBARSTYLES	CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC
#endif

//callback for setting the FS toolbar styles
typedef void (CALLBACK *PFNFSSETSTYLES)(UINT& dwStyle, UINT& dwExStyle);

//Helper macro for determining the mainframe type
#define SEC_FS_SECFRAME(pFrmWnd)	( pFrmWnd->IsKindOf(RUNTIME_CLASS(SECFrameWnd)) || pFrmWnd->IsKindOf(RUNTIME_CLASS(SECMDIFrameWnd)) )

//Helper structure used for setting the FullScreen control bars
typedef struct tagSECFSBarState
{
	CControlBar* pCtrlBar;
	BOOL bFSModeOnly;
} SECFSBarState;
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// SECFSMDIClientListener 
class SECFSMainFrmListener;
class SECFSMDIClientListener : public stingray::foundation::SECWndListener
{
//Nested Child frame listener
protected:
	class SECFSChildFrmListener : public stingray::foundation::SECWndListener
	{
	friend class SECFSMDIClientListener;
	// Construction
	public:
		SECFSChildFrmListener();
		virtual ~SECFSChildFrmListener();

	protected:
		SECFSMainFrmListener* m_pMainFrmLstnr;

		void SetMainFrmRef(SECFSMainFrmListener* pMainFrmLstnr) { m_pMainFrmLstnr = pMainFrmLstnr; };
		//{{AFX_MSG(SECFSChildFrmListener)
		afx_msg LRESULT OnWMGetMinMaxInfo(WPARAM wPar, LPARAM lPar);
		afx_msg LRESULT OnWMSysCommand(WPARAM wPar, LPARAM lPar);
		afx_msg LRESULT OnWMSize(WPARAM wPar, LPARAM lPar);
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()
	};


// SECFSMDIClientListener Construction
public:
	SECFSMDIClientListener();
	virtual ~SECFSMDIClientListener();

//Operations
	virtual BOOL PlugInTo( CWnd* pWnd, SECFSMainFrmListener* pMainFrmLstnr);
	virtual void UnPlug();

protected:
	SECFSChildFrmListener m_wndMDIChildLstnr;
	SECFSMainFrmListener* m_pMainFrmLstnr;

protected:
	virtual BOOL IsWindowProc(HWND hWndListen);

	//{{AFX_MSG(SECFSMDIClientListener)
	afx_msg LRESULT OnMDISetMenu(WPARAM wPar, LPARAM lPar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// SECFSMainFrmListener 
class SECFSMainFrmListener : public stingray::foundation::SECWndListener
{
friend class SECFullScreenView;
friend class SECFSMDIClientListener;
friend class SECFSMDIClientListener::SECFSChildFrmListener;
// Construction
public:
	SECFSMainFrmListener();
	virtual ~SECFSMainFrmListener();

protected:
	SECFullScreenView* m_pFSViewer;
	BOOL m_bMenuing;
	BOOL m_bMenuFlag;
	BOOL m_bMenuBar;
	HWND m_hWndActiveChild;
	BOOL m_bSDI;
	BOOL m_bAllowClientMsg;

	HMENU m_hMenuCached;

	void DisableFrameRecalcLayout(BOOL bVal);

	virtual BOOL PlugInTo( CWnd* pWnd, SECFullScreenView* pViewer);	

	void SetActiveChild(HWND hWndChild)
	{
		ASSERT(::IsWindow(hWndChild));
		m_hWndActiveChild = hWndChild;		
	};

	void HideMenuDisp();
	BOOL RestoreMaxMenuState(CFrameWnd* pFrmWnd);
	void SetMenuNULL()	{ m_hMenuCached = NULL; };

	//{{AFX_MSG(SECFSMainFrmListener)
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnNcMouseMove( UINT nHitTest, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnToggleFullScreen();
	afx_msg LRESULT OnExitMenuLoop(WPARAM wPar, LPARAM lPar);
	afx_msg void OnSysCommand( UINT nID, LONG lParam );
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg LRESULT OnFSHideMenu(WPARAM wPar, LPARAM lPar);
	afx_msg void OnTimer( UINT nIDEvent );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// SECFSToolBar 
template<class TBarBase>
class SECFSToolBar : public TBarBase
{
//Construction/Destruction
public:
	SECFSToolBar() {};
	virtual ~SECFSToolBar()	{};	

// Overrides
	virtual void OnUpdateCmdUI( CFrameWnd* /*pTarget*/, BOOL /*bDisableIfNoHndler*/ )
	{
		CFrameWnd* pParent = (CFrameWnd*)GetOwner();
		TBarBase::OnUpdateCmdUI(pParent, FALSE);
	}; 
};
/////////////////////////////////////////////////////////////////////////////


// SECFullScreenView declaration

/////////////////////////////
// AutoDuck tag block block for SECFullScreenView 


//@doc SECFullScreenView
//@mfunc BOOL  | SECFullScreenView | GetFSMode | Returns TRUE if the Full Screen mode is currently set. Else FALSE.
//@xref<mf SECFullScreenView::SetFSMode>
//@xref<c SECFullScreenView>

//@doc SECFullScreenView
//@mfunc void  | SECFullScreenView | SetFSStyle | Sets the Full Screen View styles.
//@parm DWORD | dwStyle | Full Screen View styles.
//@xref<mf SECFullScreenView::GetFSStyle>
//@xref<c SECFullScreenView>

//@doc SECFullScreenView
//@mfunc DWORD  | SECFullScreenView | GetFSStyle | Returns the current Full Screen View styles.
//@xref<mf SECFullScreenView::SetFSStyle>
//@xref<c SECFullScreenView>


// adinsert AutoDuck insertion point for SECFullScreenView 
//@doc SECFullScreenView
//@class The SECFullScreenView class is a plug-in type component that can be added to existing MDI/SDI based application and provides
// a 'full screen view' of your client window that extends across the entire desktop. When the full screen mode is set, SECFullScreenView 
// provides a dockable toolbar that can be used to revert to the normal viewing mode. The \<ESCAPE\> keyboard accelerator can also be used
// to exit the full screen mode. Similar to Microsoft Word, the class also provide a drop-down menu when the cursor is hovered over the 
// top portion of the window in the full screen mode.
//@comm
// SECFullScreenView supports the following styles:
//
// SEC_FS_TEXTTOOLBAR - The full screen toolbar is a text-only toolbar. 
//
// SEC_FS_DROPDOWNMENU - Displays main window menu in the full screen mode.
//
// SEC_FS_TASKBARHIDE - Forcibly hides the Windows task bar when full screen mode is triggered. 
//
// SEC_FS_NOCUSTTOOLBAR - Setting this style creates the toolbar as type SECToolBar. Default is SECCustomToolBar.
//
// See the SCREENVIEW sample in the \SAMPLES\TOOLKIT\STANDARD\UIEXT\SCREENVIEW directory for a demonstration of SECFullScreenView.

// Note: SECFullScreenView requires C++ RTTI to be enabled for your project. From within the VC++ IDE, you can select this option
// through the C/C++ tab in the 'Project - Settings' dialog.

//@BASE public | CWnd
class SECFullScreenView : public CWnd
{
public:

	SECFullScreenView();
	virtual ~SECFullScreenView();

// Attributes
protected:
	CControlBar* m_pWndFSTBar;

protected:
	BOOL m_bFSMode;
	DWORD m_dwFSStyle;

	CRect m_rcMainWnd;

	UINT m_nFSTBarResID;
	CBitmap* m_pBitmap;
	CString m_strTBarText;
	//Toolbar setstyles callback
	PFNFSSETSTYLES m_lpfnSetStyles;

	HACCEL m_hMainFrmAccel;
	HACCEL m_hFSAccelTemp;

	//FS controlbar state
	CMap<CControlBar*, CControlBar*, BOOL, BOOL> m_mapBarToState;
	CMap<CControlBar*, CControlBar*, BOOL, BOOL> m_mapBarHidden;

	SECFSMDIClientListener m_PaneLstnr;
	SECFSMainFrmListener m_MainFrmLstnr;

	SECDockState* m_pDockStateMenuBar;

// Operations
public:
	//@Access Public Members
	//@cmember
	/* Invokes the Full Screen mode.*/
	BOOL SetFSMode(DWORD dwStyle=SEC_FS_DEFAULT);

	//@cmember
	/* Invokes the Full Screen mode with a text toolbar.*/
	BOOL SetFSMode(DWORD dwStyle, LPCTSTR lpszTBTxt); 

	//@cmember
	/* Invokes the Full Screen mode with the specified toolbar bitmap.*/
	BOOL SetFSMode(DWORD dwStyle, UINT nResBitmapID);

	//@cmember
	/* Exits the Full Screen mode.*/
	void CloseFSMode();

	//@cmember
	/* Returns TRUE if full screen mode is set.*/
	BOOL GetFSMode() { return m_bFSMode;};

	//@cmember
	/*Call this member to set control bars that have to be displayed in the full screen mode.*/
	BOOL SetBarStateArray(SECFSBarState* pBarArray, int nCount);

	//@cmember
	/*Specifies a callback function for changing the default toolbar styles*/
	void SetFSToolBarStylesCB(PFNFSSETSTYLES lpfnSetStyles);

	//@cmember
	/*Returns the current full screen styles.*/
	DWORD GetFSStyle()	{ return m_dwFSStyle; };

	//@cmember
	/*Sets the full screen view styles.*/
	void SetFSStyle(DWORD dwStyle)	{ m_dwFSStyle = dwStyle; };

protected:
	void SetTaskBarState(BOOL bVal);
	void SetControlBarListState(BOOL bVal);
	void ResetMainWndState();

	void SetAccelTable();
	void RestoreAccelTable();

	void GetMenuBarState(CFrameWnd* pFrameWnd);
	void SetMenuBarState(CFrameWnd* pFrameWnd);

	BOOL TextToBmp(CBitmap* pBmp, CDC* pDC);

	//Function template used for CToolBar/SECToolBar instantiation
	template<class TBarType> BOOL SetFSStdToolBar(TBarType* pToolBar, CFrameWnd* pMainFrmWnd, UINT* pArr, CSize szBmp);

//Overrides	
	virtual int SetFSToolBar(CFrameWnd* pMainFrmWnd);
	virtual void CloseDefFSToolBar();

	//@Access Overridables
	//@cmember
	/*Override for displaying any custom windows in the full screen mode.*/
	virtual void PreFullScreenMode();

	//@cmember
	/*Override for removing custom windows that are local to the full screen mode.*/
	virtual void PostFullScreenMode();

	// Generated message map functions
protected:
	//{{AFX_MSG(SECFullScreenView)
	afx_msg BOOL OnTTNNeedText(UINT id, NMHDR * pTTTStruct, LRESULT * pResult );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//
// SEC Extension DLL
// Reset declaration context
//
#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR
//

#endif //__SECFULLSCREEN_H__

