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
//  Description:	Declarations for Worksheet, Workbook, and WorkbookClient
//

#ifndef __SECWB_H__
#define __SECWB_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#ifndef __SWINMDI_H__
#include "toolkit\swinmdi.h"
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

// User-defined message sent to the SECWorkbook class
#define SECWB_WM_ACTIVATIONCHANGED  (WM_USER + 1101)

// User-defined message sent to the SECWorkbookClient class
#define SECWB_WM_USINGMENUBARS		(WM_USER + 1102)

class SECWorkbook;

/////////////////////////////
// AutoDuck tag block block for SECWorksheet 
// adinsert AutoDuck insertion point for SECWorksheet 
//@doc SECWorksheet 
//@class The SECWorksheet class is an MDI child window that can viewed as an individual worksheet in an SECWorkbook when
// in workbook mode.  To use SECWorksheet, derive your MDI child window classes from SECWorksheet rather than 
// SECMDIChildWnd.  There are no public operations or member variables defined by SECWorksheet.
//@comm See the WORKBPAD sample in the \SAMPLES\TOOLKIT\STANDARD\MDI\WORKBPAD directory for a demonstration of this class.
//@xref<c SECWorkbook>
//@BASE public|SECMDIChildWnd
class SECWorksheet : public SECMDIChildWnd
{
    friend class SECWorkbook;
    DECLARE_DYNCREATE(SECWorksheet);
    
// Construction
public:
    SECWorksheet();

// Implementation
protected:
    HICON	m_hIcon;	// Icon for this window
    int		m_nPosition;	// Position of tab within array
    BOOL	m_bSelected;    // True if the tab is selected but
				//   the frame isn't yet active

    SECWorkbook* GetWorkbook();
    virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext = NULL);
	virtual void OnUpdateFrameMenu( BOOL bActive, CWnd* pActiveWnd, HMENU hMenuAlt );
	virtual void OnUpdateFrameTitle( BOOL bAddToTitle );
    
public:
    virtual ~SECWorksheet();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
    //{{AFX_MSG(SECWorksheet)
    afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP();
};

#ifndef _DEBUG  // debug version in secwb.cpp
inline SECWorkbook* SECWorksheet::GetWorkbook()
   { return (SECWorkbook*)GetMDIFrame(); };
#endif

/////////////////////////////
// AutoDuck tag block block for SECWorkbookClient 
// adinsert AutoDuck insertion point for SECWorkbookClient 
//@doc SECWorkbookClient 
//@class Utility class used in the implementation of SECWorkbook. This class is derived from CWnd.
//@xref<c SECWorkbook>
//@BASE public|CWnd
class SECWorkbookClient : public CWnd
{
    DECLARE_DYNCREATE(SECWorkbookClient);
	
// Construction
public:
    SECWorkbookClient();
    
// Operations
public:
    void SetMargins(int left, int right, int top, int bottom);
    void GetMargins(int& left, int& right, int& top, int& bottom);
    
// Implementation
protected:
    CRect m_margins;	// Space between frame border and MDI Client
    virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType);
    SECWorkbook* GetWorkbook();

public:
    virtual ~SECWorkbookClient();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif
    
// Generated message map functions
protected:
    //{{AFX_MSG(SECWorkbookClient)
	afx_msg LRESULT OnMDISetMenu(WPARAM wParam, LPARAM lParam);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP();
};

#ifndef _DEBUG  // debug version in secwb.cpp
inline SECWorkbook* SECWorkbookClient::GetWorkbook()
   { return (SECWorkbook*)GetParent(); };
#endif

/////////////////////////////
// AutoDuck tag block block for SECWorkbook 


// adinsert AutoDuck insertion point for SECWorkbook 
//@doc SECWorkbook 
//@class The SECWorkbook is a Windows multiple document interface (MDI) frame window that adds the 
// capability to view the MDI child windows as worksheets in a workbook.  
//@comm
// Tool tips are now present for workbook tab text that is partially or fully obscured.  This is 
// new with release 5.0 of Objective Toolkit.
//
// SECWorkbook inherits most of its functionality from SECMDIFrameWnd with the workbook viewing 
// mode being an extension. This workbook view capability provides an alternative, tab-based user 
// interface for managing multiple documents.  An SECWorkbook-based application can operate in either 
// normal or workbook mode, and the user can switch between these modes at runtime.  In normal mode, 
// the application appears and functions exactly like an MDI application.  In workbook mode, the MDI 
// child windows are presented as a collection of worksheets with cascaded tabs for navigation (i.e., 
// selecting a tab activates the associated MDI child window).
//
// When the workbook mode is switched on, the MDICLIENT window is resized to provide room for tabs 
// to be drawn.  One tab is drawn for each MDI child window.  In the description of the class’s 
// operations given below, it is important to note that a worksheet and an MDI child window are one 
// and the same, since <c SECWorksheet> is derived from <c SECMDIChildWnd>.
//
// NOTE: Workbook mode can be in effect without maximizing the MDI child windows.
//
// To create a workbook-based application, derive the CMainFrame class (or your equivalent class) 
// from SECWorkbook instead of CMDIFrameWnd.  In addition, add a "Workbook" item on the "View" menu, 
// and map its command and update handlers to <mf SECWorkbook::OnViewWorkbook> and 
// <mf SECWorkbook::OnUpdateViewWorkbook>, respectively.
//
// For example, your CMainFrame class message map would have the following entries:
//
//	BEGIN_MESSAGE_MAP(CMainFrame, SECWorkbook)<nl>
//		//{{AFX_MSG_MAP(CMainFrame)<nl>
//			ON_WM_CREATE()<nl>
//			ON_COMMAND(ID_VIEW_WORKBOOK, OnViewWorkbook)<nl>
//		//}}AFX_MSG_MAP<nl>
//	END_MESSAGE_MAP()
//
// Adding "workbook" handlers to your pulldown menus is optional.  The user can just hardcode the 
// workbook to be always on.
// 
// See the WORKBPAD sample in the \SAMPLES\TOOLKIT\STANDARD\MDI\WORKBPAD directory for a demonstration of 
// this class.
//@xref<c SECWorksheet>
//@BASE public|SECMDIFrameWnd
class SECWorkbook : public SECMDIFrameWnd
{
    friend class SECWorksheet;
    DECLARE_DYNCREATE(SECWorkbook);

// Construction
public:
	
    SECWorkbook();

// Attributes
public:
	virtual void GetFullWorkbookRect(CRect& rectFullWorkbook);

    CObArray m_worksheets;		// List of worksheets
    BOOL	m_bWorkbookMode;	// Are we in workbook mode?
    BOOL	m_bShowIcons;		// Show document icons on the tabs?
    CFont	m_fontTab;		// The font to use for tab labels
    int		m_nScrollOffset;	// Amount to displace tab offset by
    SECWorkbookClient* m_pWBClient;	// The MDI client
    CPen	m_penWhite, m_penBlack, m_penShadow, m_penFace;
    int		m_cxTab;		// Width of tab 
    int		m_cyTab;		// Height of tab
    int		m_cxFold;		// Width of fold in the tab
	int		m_cxActive;		// Extra tab width when active
	int		m_cyActive;		// Extra tab height when active
	int		m_nMaxTabWidth;
    CRect	m_rectClientLast;	// Redraw hint   

	// Set to TRUE for the workbook to paint properly as an OLE
	// container. If not using OLE containment, it is recommended
	// you not reset to maintain less flicker.
	BOOL	m_bSupportOleContainer;

protected:

	int m_nActiveWorksheet;

// Overridables
protected:
	virtual BOOL OnNewWorksheetSelected(SECWorksheet* pSheet);
	virtual CMDIChildWnd* CreateFloatingMDIChild(DWORD dwStyle, CControlBar* pBar = NULL);

// Implementation
protected:

    void GetTabRgn(SECWorksheet* pSheet, CRgn& rgn);
    void InvalidateTab(SECWorksheet* pSheet, BOOL bInvalidAfter = FALSE);
    void InvalidateAllTabs();
    int recalcTabWidth();
	inline int GetIconWidth() { return m_cyTab-8; }

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(SECWorkbook)
public:
    virtual BOOL CreateClient(LPCREATESTRUCT lpCreateStruct, CMenu* pWindowMenu);
    virtual void RecalcLayout(BOOL bNotify = TRUE);
    //}}AFX_VIRTUAL

#ifdef WIN32
	// tooltip support
protected:
	CToolTipCtrl* m_pToolTips;
public:
	virtual int OnToolHitTest(CPoint point,TOOLINFO* pTI) const;
#endif 

public:
    ~SECWorkbook();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Overridables

protected:

	//@Access Protected Members
	//@Access Overrideables
	//@cmember
	/* Adds a new worksheet to the workbook*/
    virtual void AddSheet(SECWorksheet* pSheet);
	//@cmember
	/* Removes the specified worksheet from the workbook*/
    virtual void RemoveSheet(SECWorksheet* pSheet);
	virtual BOOL IsFullTabTextVisible(SECWorksheet* pSheet);
    virtual void GetTabPts(SECWorksheet* pSheet, CPoint*& pts, int& count);
	//@cmember
	/* Gets the label associated with the tab*/
    virtual const TCHAR* GetTabLabel(SECWorksheet* pSheet) const;
	//Gets the bounding rectangle of the tab's text
	virtual BOOL GetTextRect(CPoint* pts,int ciIconWidth,CRect& rectText);
	//@cmember
	/* Draws a blank tab at the position specified*/
    virtual void OnDrawTab(CDC* pDC, SECWorksheet* pSheet);
	//Draws a border around the tab
	virtual void OnDrawBorder(CDC* pDC);
	//@cmember
	/* Renders the icon and tab label on top of the blank tab drawn by OnDrawTab()*/
    virtual void OnDrawTabIconAndLabel(CDC* pDC, SECWorksheet* pSheet);

// Operations

public:

    //@Access Public Members
	//@Access Operations
	//@cmember
	/* Toggles between Workbook and Normal modes*/
    void SetWorkbookMode(BOOL bEnabled = TRUE);
	//@cmember
	/* Sets display of icons*/
    void SetShowIcons(BOOL bShowIcons = TRUE);
	//@cmember
	/* Returns the icon to be drawn on the tab*/
    virtual HICON GetTabIcon(SECWorksheet* pSheet) const;
	//@cmember
	/* Sets an icon to be drawn on the tab*/
	virtual BOOL SetTabIcon(SECWorksheet* pSheet,HICON hIcon,BOOL bRedraw=TRUE);

// Queries

    //@Access Queries
	//@cmember
	/* Looks up the specified worksheet in the workbook*/
    BOOL LookupSheet(SECWorksheet* pSheet, int& nIndex);
	//@cmember
	/* Returns a pointer to the worksheet specified in the index */
    SECWorksheet* GetWorksheet(int nSheet) const;
	//@cmember
	/* Returns a count of the total number of worksheets*/
    int GetSheetCount() const;

// Generated message map functions
protected:
    //{{AFX_MSG(SECWorkbook)
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnViewWorkbook();
    afx_msg void OnUpdateViewWorkbook(CCmdUI* pCmdUI);
    afx_msg void OnOptionsToggleIcons();
    afx_msg void OnUpdateOptionsToggleIcons(CCmdUI* pCmdUI);
    afx_msg void OnDestroy();
	afx_msg LRESULT OnActivationChanged(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP();
};

/////////////////////////////////////////////////////////////////////////////
// SECControlBarWorksheet

class SECControlBarWorksheet : public SECWorksheet
{
	DECLARE_DYNCREATE(SECControlBarWorksheet)

// Constructors
protected:
	SECControlBarWorksheet();           // protected constructor used by dynamic creation

public:
#ifdef WIN32
	SECDockBar	m_wndDockBar;		
#endif

// Attributes
public:
	
// Overrides
	void RecalcLayout(BOOL bNotify = TRUE);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SECControlBarWorksheet)
	protected:
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext = NULL);
	virtual ~SECControlBarWorksheet();

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);

	// Generated message map functions
	//{{AFX_MSG(SECControlBarWorksheet)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

// For backward compatibility...
#define SECDDIWorksheet SECWorksheet
#define SECDDIWorkbookClient SECWorkbookClient
#define SECDDIWorkbook SECWorkbook

//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif // __SECWB_H__

