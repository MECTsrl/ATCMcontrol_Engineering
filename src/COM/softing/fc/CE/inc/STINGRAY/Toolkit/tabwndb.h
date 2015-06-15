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
//  Description:	SECTabWndBase declaration
//

#ifndef __TABWNDB_H__
#define __TABWNDB_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#ifndef __TABCTRLB_H__
#include "toolkit\tabctrlb.h"
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

// Tab Control Window ID
#define SEC_IDW_TABCTRL                 0xEA30
#define SEC_IDW_TABCLIENT_FIRST         0xEB00  // 256 tabs max
#define SEC_IDW_TABCLIENT_LAST          0xEBff
#define SEC_IDW_HORZ_SCROLLBAR          0xEC00  // Horz scrollbar
#define SEC_IDW_VERT_SCROLLBAR          0xECff  // Vert scrollbar


// Default tab styles
#define SEC_WS_DEFAULT_TABCLIENT	(WS_CHILD | WS_VISIBLE)

/////////////////////////////
// AutoDuck tag block block for SECTabWndBase 



//@doc SECTabWndBase
//@mfunc const SECTabControlBase*  | SECTabWndBase | GetTabControl | Call this function to obtaina a pointer to the
// SECTabControlBase object.
//@rdesc Pointer to the SECTabControlBase object.
//@xref<c SECTabWndBase>
//@xref<c SECTabControlBase>










// adinsert AutoDuck insertion point for SECTabWndBase 
//@doc SECTabWndBase 
//@class The SECTabWndBase class defines the interface of a tabbed window.  
//@comm
// A tabbed window is a window containing
// multiple embedded child windows (or pages) and a row of tabs.  When a tab is activated (via a mouse press),
// SECTabWndBase shows and activates the CWnd-derived window associated with the tab.
//
// What is the relationship between SECTabControl and SECTabWndBase?  As stated previously, an SECbWndBase contains a list of pages and a row of 
// tabs. The latter is implemented as a single instance of tab control of type SECTabControl. The single tab control
// object manages and displays an array of tabs.  This tab control is sized and positioned by its SECTabWndBase parent
// such that no overlap exists between the pages and the tab control.  So, an SECTabWndBase contains an instance of an 
// SECTabControl (and zero or more pages).  This organization allows the SECTabControl to be used in a different context.
//
// There are currently two implementations of the SECTabWndBase interface: SECTabWnd and SEC3DTabWnd.  SECTabWnd 
// implements a two-dimensional look and SEC3DTabWnd implements a three-dimensional look and feel with optional icon.  
// The two-dimensional tabbed window is more space efficient, while the three-dimensional tabbed window gives a more
// elegant look and simpler usage model.
//
// NOTE: You cannot instantiate SECTabWndBase directly as it is an abstract 
// base class.  Instead, you must instance one of SEC3DTabWnd or SEC3DTabWnd.
//
// The TABDEMO sample in the \SAMPLES\TOOLKIT\STANDARD\TABS\TABDEMO directory demonstrates the SECTabWndBase class.
//@xref<c SECTabControlBase> <c SECTabWnd> <c SEC3DTabWnd> <c SECWorkbook> 
//@BASE public | CWnd
class SECTabWndBase : public CWnd
{
    DECLARE_DYNAMIC(SECTabWndBase)
    
// Construction
public:
	SECTabWndBase();
    virtual ~SECTabWndBase();

	virtual BOOL Create(LPCTSTR lpszClassName,
		LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect,
		CWnd* pParentWnd, UINT nID,
		CCreateContext* pContext = NULL);

    virtual BOOL Create(CWnd* pParentWnd, DWORD dwStyle = WS_CHILD | WS_VISIBLE |
    		WS_HSCROLL | WS_VSCROLL, UINT nID = AFX_IDW_PANE_FIRST);

protected:    
    CWnd* CreateView(CRuntimeClass* pViewClass,
			 CCreateContext* pContext = NULL, UINT nID = -1);
    virtual BOOL CreateScrollBarCtrl(DWORD dwStyle, UINT nID);
     
	virtual BOOL CreateTabCtrl(DWORD dwStyle, UINT nID);

public:
	
// Operations
	//@Access Public Members
	//@Access Operations
   
	//@cmember
	/* Inserts a new tab with the given label before the currently active tab.*/
	virtual SECTab* InsertTab(CWnd* pWnd,
							  int nIndex,
							  LPCTSTR lpszLabel);

	//@cmember
	/* Inserts a new tab with the given label before the currently active tab.*/
    virtual SECTab* InsertTab(CRuntimeClass* pViewClass,
							  int nIndex,
							  LPCTSTR lpszLabel,
							  CCreateContext* pContext = NULL,
							  UINT nID = -1);

	//@cmember
	/* Appends a new tab to the end of the existing array of tabs.*/
    virtual SECTab* AddTab(CWnd* pWnd,
						   LPCTSTR lpszLabel);

	//@cmember
	/* Appends a new tab to the end of the existing array of tabs.*/
	virtual SECTab* AddTab(CRuntimeClass* pViewClass,
						   LPCTSTR lpszLabel,
						   CCreateContext* pContext = NULL,
						   UINT nID = -1);

	//@cmember
	/* Specifies the icon for a given tab.*/
	virtual void SetTabIcon(int nIndex, HICON hIcon);

	//@cmember
	/* Specifies the icon for a given tab.*/
	virtual void SetTabIcon(int nIndex, UINT nIDIcon,
							int cx=SEC_TAB_DEFICON_CX, 
							int cy=SEC_TAB_DEFICON_CY);

	//@cmember
	/* Specifies the icon for a given tab.*/
	virtual void SetTabIcon(int nIndex, LPCTSTR lpszResourceName,
							int cx=SEC_TAB_DEFICON_CX, 
							int cy=SEC_TAB_DEFICON_CY);

	virtual void SetScrollStyle(int nIndex, DWORD dwStyle);

	//@cmember
	/* Removes the tab at the specifed index.*/
    virtual void RemoveTab(CWnd* pWnd);

	//@cmember
	/* Removes the tab at the specifed index.*/
    virtual void RemoveTab(int nIndex);

	//@cmember
	/* Gives a new name to the tab at the specified index.*/
    virtual void RenameTab(CWnd* pWnd, LPCTSTR lpszLabel);

	//@cmember
	/* Gives a new name to the tab at the specified index.*/
    virtual void RenameTab(int nIndex, LPCTSTR lpszLabel);

	//@cmember
	/* Causes the specified tab to become the active tab.*/
    virtual BOOL ActivateTab(CWnd* pWnd, int nIndex);

	//@cmember
	/* Causes the specified tab to become the active tab.*/
    virtual BOOL ActivateTab(CWnd* pWnd);

	//@cmember
	/* Causes the specified tab to become the active tab.*/
    virtual BOOL ActivateTab(int nIndex);

	//@cmember
	/* All tabs are marked as unselected.*/
    virtual void ClearSelection();

	//@cmember
	/* The specified tab is scrolled into view.*/
    virtual void ScrollToTab(CWnd* pWnd); 
	
	//@cmember
	/* The specified tab is scrolled into view.*/
    virtual void ScrollToTab(int nIndex);

	virtual void OnReActivateTab();

	// Set the target for tab notifications. Useful when nesting tabs.
	virtual void SetNotifyWnd(CWnd* pNotifyWnd);
	virtual CWnd* GetNotifyWnd() { return m_pNotifyWnd; }

// Queries
	//@Access Queries
	//@cmember
	/* Returns the number of tabs*/
    int GetTabCount();

	//@cmember
	/* Returns information about the tab with the supplied index.*/
    BOOL GetTabInfo(int nIndex, LPCTSTR& lpszLabel, BOOL& bSelected, CWnd*& pWnd, void*& pExtra);

	//@cmember
	/* Returns the index of the tab which is associated with the specified window.*/
    BOOL FindTab(const CWnd* const pWnd, int& nTab) const;

	//@cmember
	/* If any tab is active, return its associated window.*/
    BOOL GetActiveTab(CWnd*& pWnd);

	//@cmember
	/* If any tab is active, return its index.*/
    BOOL GetActiveTab(int& nIndex);

	//@cmember
	/* Appends a tab with the specified label to the row of existing tabs.*/
    BOOL TabExists(CWnd* pClient);

	//@cmember
	/* Appends a tab with the specified label to the row of existing tabs.*/
    BOOL TabExists(int nTab);

	//@cmember
	/* Returns a pointer to the SECTabControlBase object associated with the tab window.*/
	const SECTabControlBase* GetTabControl() { return m_pTabCtrl; }

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(SECTabWndBase)
    //}}AFX_VIRTUAL
    
// Implementation
protected:
    SECTabControlBase *m_pTabCtrl;
    CWnd* m_pActiveWnd;
	CWnd* m_pWndCreating;
	CWnd* m_pNotifyWnd;

    virtual void GetInsideRect(CRect& rect) const;
    virtual void RecalcLayout();
    
    // Generated message map functions
protected:
    //{{AFX_MSG(SECTabWndBase)
    afx_msg void OnDestroy();
	afx_msg void OnChildActivate( );
    afx_msg LRESULT OnTabSelect(WPARAM wParam, LPARAM lParam);
    //}}AFX_MSG
	afx_msg LRESULT OnTabReActivate( WPARAM wParam, LPARAM lParam );
    DECLARE_MESSAGE_MAP();
};

// C Function
SECTabWndBase* GetParentTabWnd(const CWnd* const pWnd);

//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif // __TABWNDB_H__
 

