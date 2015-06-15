/////////////////////////////////////////////////////////////////////////////
// class.h : Declaration of SECClass
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
// Author:       Don
// Description:  SECMenuBar frame window support
//

//@doc SECMenuBar
//@mfunc UINT  | SECMenuBar | GetCurMenuID | Returns the current menu ID

//adinsert

#ifndef __TMENUFRM_H__
#define __TMENUFRM_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#ifdef WIN32

#ifndef __TBTNMENU_H__
#include "toolkit\tbtnmenu.h"
#endif

#ifndef __PLUGIN_H__
#include "Foundation\patterns\MFC\plugin.h"
#endif

#ifndef __TBMPMENU_H__
#include "toolkit\TBmpMenu.h"
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

struct SECMenuBarBtnInfo : public SECBtnInfo
{
	UINT       m_nPos;
	SECStdBtn* m_pBtn;		// Pointer to button
};

// Maps bit flags to menu resources. Used for identifying which buttons
// are associated with which views.
typedef CMap<UINT, UINT, DWORD, DWORD> SECMapMenuIDToBit;

//@doc SECMenuBar 
//@class SECMenuBar replaces your normal menu with a dockable bar that has the look and feel of the Visual Studio and Office 97 menus.  The menu bar buttons are represented by a simple button class that contains the HMENU to display when clicked.  SECMenuBar uses SECCustomToolBar as a base class.  So not only do you get the cool flat look, you also gain complete customization of the menu bar when used in conjuntion with SECToolBarManager.  To integrate this class into your own existing aplication that uses the SECToolBarManager takes as little as three lines of code.  See VIZ for an example of an application that was converted to use the new menu bar classes.  The button map that is used in customizable toolbars to denote combo boxes, two-part buttons, and custom buttons can also be utilized to dynamically swap menus based on the currently active view.
//@base public | SECCustomToolBar
class SECMenuBar : public SECCustomToolBar
{
	DECLARE_DYNAMIC(SECMenuBar)

public:
	//@cmember
	/* Constructor*/
	SECMenuBar();
	
	//@cmember
	/* Destructor*/
	virtual ~SECMenuBar();

	//@cmember
	/* Creates the SECMenuBar window*/
	virtual BOOL CreateEx(DWORD dwExStyle, CWnd* pParentWnd, DWORD dwStyle = 
				WS_VISIBLE | WS_CHILD | CBRS_TOP, UINT nID = AFX_IDW_CONTROLBAR_LAST, 
				LPCTSTR lpszTitle = NULL);

	virtual int  IsAccelerated(TCHAR c) const;

	void InvokeSysMenu();

	void DropDownMenuHeader();

	void NextMenu(BOOL m_bRight);
	//@cmember
	/* Loads the menu specified*/
	BOOL LoadMenu(UINT nIDResource);
	//@cmember
	/* Switches to the menu specified*/
	BOOL SwitchMenu(UINT nIDResource);
	//@cmember
	/* Enables the bit flag*/
	virtual void EnableBitFlag(DWORD dwBit, BOOL bUpdate = FALSE );
	//@cmember
	/* Notifies you when the style is changing*/
	virtual void OnBarStyleChange(DWORD dwOldStyle, DWORD dwNewStyle);
	virtual DWORD RecalcDelayShow(AFX_SIZEPARENTPARAMS* lpLayout);

	//@cmember
	/* Returns font used for the menu*/
	virtual HFONT GetMenuFont() const;
	//@cmember
	/* Resets the menu*/
	virtual void ResetMenus(BOOL bNoUpdate = FALSE);

	//@cmember
	/* Defines the menu resources to use.*/
	virtual BOOL SetMenuInfo(int nCount, UINT nIDMenu, ...);

	// Add a button to the menubar
	virtual void AddButton(int nIndex, int nID, BOOL bSeparator = FALSE, 
						   BOOL bNoUpdate = FALSE);

	//@cmember
	/* Returns the current menu ID*/
	UINT GetCurMenuID() const { return m_nIDMenu; }
	
	// Prevent the menubar from ever being seen as a non-cool style. This function will 
	// be changed later when non-cool look functionality is defined for the menu bar.
	virtual inline BOOL CoolLookEnabled() const { return TRUE; }

	//Set Full Screen Display
	void SetMenuBarFSMode(BOOL bSet) { m_bFSView = bSet; };

	// override DelayShow required for Ole Containment to work properly (MDI case)
	virtual void DelayShow(BOOL bShow);
	
	// for the DelayShow setting. Required for Ole Containment to work properly (MDI case)
	void SetIgnoreDelayShow(BOOL b);
	BOOL GetIgnoreDelayShow();

protected:
	//overrides necessary to accomodate menubar display during FullScreen Mode
	virtual CSize CalcLayout(int nLength, DWORD dwMode);
	virtual CSize CalcDynamicLayout(int nLength, DWORD dwMode);

// Implementation
public:
	SECMapMenuIDToBit m_mapMenuToBit;
	CStringArray      m_arrMenuTitles;

protected:
	BOOL              m_bRemoveSysMenu;
	HFONT             m_hMenuFont;
	DWORD			  m_dwBitFlag;			// Current menu bits;
	UINT              m_nIDMenu;			// Current menu ID
	BOOL			  m_bFSView;			// Flag to enable full screen display
	BOOL			  m_bIgnoreDelayShow;	// specifies whether DelayShow is to be ignored
											// This flag is checked only when bShow in DelayShow is FALSE

	// Loads the menu bar buttons from the given button map.
	BOOL LoadBtnsFromBtnMap(const SECBtnMapEntry* pMap);

	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnSettingChange(UINT wParam, LONG lParam);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

/////////////////////////////////////////////////////////////////////////////

class SECBtnCaption : public SECStdBtn
{
	DECLARE_BUTTON(SECBtnCaption);

public:
	SECBtnCaption();
	virtual ~SECBtnCaption() {}

	virtual BOOL BtnPressDown(CPoint point);
	virtual void BtnPressMouseMove(CPoint point);
	virtual void BtnPressCancel();
	virtual UINT BtnPressUp(CPoint point);
	virtual int  OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	virtual BOOL CanDrag() const { return FALSE; }

	CMDIChildWnd* GetMDIChild() const;

// Implementation
public:
	DWORD m_dwMDIChildStyle;

protected:
	int   m_cxCapBtn;
	int   m_cyCapBtn;
	int   m_nTracking;

	int HitTest(CPoint pt) const;

	// Called when toolbar bitmap has changed ... buttons should now adjust
	// their size.
	virtual void AdjustSize();

	// Draw face of button
	virtual void DrawFace(SECBtnDrawData& data, BOOL bForce, int& x, int& y, 
						  int& nWidth, int& nHeight, int nImgWidth = -1);
};

/////////////////////////////////////////////////////////////////////////////

class SECCapMenuBtn : public SECTBMenuBtn
{
	DECLARE_BUTTON(SECCapMenuBtn);

public:
	SECCapMenuBtn();
	virtual ~SECCapMenuBtn();

	virtual BOOL CanDrag() const { return FALSE; }
	virtual void ShowMenu();

	CMDIChildWnd* GetMDIChild() const;

// Implementation
public:
	HICON m_hIcon;

protected:
	// Called when toolbar bitmap has changed ... buttons should now adjust
	// their size.
	virtual void AdjustSize();

	// Draw face of button
	virtual void DrawFace(SECBtnDrawData& data, BOOL bForce, int& x, int& y, 
						  int& nWidth, int& nHeight, int nImgWidth = -1);
};

/////////////////////////////////////////////////////////////////////////////

//@doc SECMDIMenuBar 
//@class SECMDIMenuBar is a SECMenuBar derivative that handles the system menu and caption buttons when an MDI child window is maximized.  All other features are the same as the SECMenuBar class.  See the three MDI samples in the MenuBar project for demonstrations.
//@base public | SECMenuBar
class SECMDIMenuBar : public SECMenuBar
{
	DECLARE_DYNAMIC(SECMDIMenuBar);

public:
	SECMDIMenuBar(); 
	virtual ~SECMDIMenuBar();

	virtual BOOL SetButtons(const UINT* lpIDArray, int nIDCount);
	virtual BOOL OnZoomChange(BOOL bDisable = FALSE);
	virtual void EnableBitFlag(DWORD dwBit);
	virtual int GetStdBtnHeight() const;

	// Load/Save the toolbar configuration
	virtual void GetBarInfoEx(SECControlBarInfo* pInfo);
	virtual void SetBarInfoEx(SECControlBarInfo* pInfo, CFrameWnd* pFrameWnd);

	virtual void ResetMenus(BOOL bNoUpdate = FALSE);
	virtual BOOL MenuMouseEvent(MSG* pMsg);

protected:
	SECBtnCaption m_capBtn;
	SECCapMenuBtn m_sysBtn;
	UINT m_nLBtnInit;

	void AddCaptionBtns();
	virtual void RemoveBtns();

	// Removes and re-adds the caption buttons
	void RemoveCapBtns(BOOL& bRemoved1, BOOL& bRemoved2);
	void ReAddCapBtns(BOOL bRemoved1, BOOL bRemoved2);

	// Handles a button drop
	virtual BOOL DropButton(CRect& dropRect, SECStdBtn* pDragBtn, BOOL bAdd, 
							BOOL bNoUpdate = FALSE);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

/////////////////////////////////////////////////////////////////////////////

class SECMenuBarMessages
{
public:
	SECMenuBarMessages();
	virtual ~SECMenuBarMessages();

	BOOL HasMenuBar() { return (m_pMenuBar != NULL) ? TRUE : FALSE; }
	SECMenuBar* GetMenuBar() { return m_pMenuBar; }
	
	virtual void AwaitMenuSelect(BOOL bRight);
	virtual void AbortMenu();
	virtual void ActivateMenu(UINT nIDMenuInfo);
	virtual void DropDownMenuHeader();
	virtual void ProcessMenuHeaderMouseMove(CPoint& pt);
	virtual void IgnoreNextKeyUp();
	virtual BOOL ProcessSysMenuNcEvent(MSG* pMsg);

protected:
	SECMenuBar* m_pMenuBar;
	BOOL        m_bHiliteFirst;
	BOOL        m_bSeenSelect;
	BOOL        m_bRight;
	BOOL			m_bIgnoreNextKeyUp;

	virtual CWnd* GetWnd() = 0;
	virtual void DisplayMenu(SECCustomToolBar* lpTB, HMENU hMenu, LPSECNMTOOLBAR lpnmTB);
	virtual void DisplaySysMenu(SECCustomToolBar* lpTB, HMENU hMenu, LPSECNMTOOLBAR lpnmTB);
	virtual void InstallHook(BOOL bInstall);
	virtual void InstallHeaderHook(BOOL bInstall);
	virtual BOOL MFPreTranslateMessage(MSG* pMsg);
	virtual BOOL MFOnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu);
	virtual LONG MFOnMenuBarProd(UINT, LONG);
	virtual BOOL MFOnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL MFOnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	virtual BOOL MFOnSysCommand(UINT nID, LPARAM lParam);
	virtual BOOL MFOnNcActivate(BOOL bActive);
};

/////////////////////////////////////////////////////////////////////////////

class SECMenuBarFeature : public SECBmpMenuPlugIn, public SECMenuBarMessages
{
	DECLARE_DYNAMIC( SECMenuBarFeature )
public:
	SECMenuBarFeature();
	virtual ~SECMenuBarFeature();

	void AttachMenuBarTo( SECMenuBar* pMenuBar );

	virtual BOOL PreTranslateMessage( MSG* pMsg );
	virtual BOOL OnNotify( WPARAM wParam, LPARAM lParam, LRESULT* pResult );
	
	BOOL IsSystemMenu();

protected:
	virtual CWnd* GetWnd(){ return this; }

	afx_msg void OnMenuSelect( UINT nItemID, UINT nFlags, HMENU hSysMenu );
	afx_msg LONG OnMenuBarProd( UINT, LONG );
	afx_msg void OnInitMenuPopup( CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu );
	afx_msg void OnSysCommand( UINT nID, LPARAM lParam );
	afx_msg BOOL OnNcActivate(BOOL bActive);

	DECLARE_MESSAGE_MAP()
};

//Helper class for allowing access to SECMenuBarFeature's protected members
class SECFriendMenuBarFeature : public SECMenuBarFeature
{
	friend LRESULT CALLBACK SECKeyboardHeaderHookProc(int nCode, WPARAM wParam, LPARAM lParam); 
};

//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

/////////////////////////////////////////////////////////////////////////////

#endif	// WIN32

#endif	// __TMENUFRM_H__

