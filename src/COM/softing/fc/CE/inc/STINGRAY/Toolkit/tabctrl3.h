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
//  Author:       Dean Hallman
//  Description:  Declarations for SEC3DTab and SEC3DTabControl
//  Created:      July 20, 1996
//
//	Modified:	  Feb. 1997 (Kim Starks)
//	Description:  Added tab disable feature			
//	

#ifndef __TABCTRL3_H__
#define __TABCTRL3_H__

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


// Array indexes for GDI objects
#define SEC3DTAB_INACTIVE	0
#define SEC3DTAB_ACTIVE		1

/////////////////////////////////////////////////////////////////////////////
// SEC3DTab

class SEC3DTab : public SECTab
{
    DECLARE_DYNAMIC(SEC3DTab);

public:
    SEC3DTab();

// Attributes

public:
	CRect m_rect;
	HICON m_hIcon;
	BOOL m_bEnabled;
	CString m_csLong;	//Retrieves a copy of the tab label.
	BOOL m_bToolTip;

// Implementation
protected:
    ~SEC3DTab();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif
};


/////////////////////////////////////////////////////////////////////////////
// SEC3DTabControl window

/////////////////////////////
// AutoDuck tag block block for SEC3DTabControl 


//@doc SEC3DTabControl
//@mfunc BOOL  | SEC3DTabControl | SetFontActiveTab | Sets an active tab's current font to the specified font.  If bRedraw
// is TRUE, the tab will also be redrawn.
//
//The CFont object passed by pointer will be copied to duplicate storage, so 
// the caller need not hang onto the pointer.
//@parm CFont*  | pFont | Specifies the new font.
//@parm BOOL | bRedraw | If TRUE, redraw the SEC3DTabControl object.
//@xref<mf SEC3DTabControl::GetFontActiveTab>
//@xref<c SEC3DTabControl>

//@doc SEC3DTabControl
//@mfunc BOOL  | SEC3DTabControl | SetFontInactiveTab | Sets an inactive tab's current font to the specified font.  If 
// bRedraw is TRUE, the tab will also be redrawn.
//
//The CFont object passed by pointer will be copied to duplicate
// storage, so the caller need not hang onto the pointer.
//@parm CFont*  | pFont | Specifies the new font.
//@parm BOOL | bRedraw | If TRUE, redraw the SEC3DTabControl object.
//@xref<mf SEC3DTabControl::GetFontInactiveTab>
//@xref<c SEC3DTabControl>

//@doc SEC3DTabControl
//@mfunc CFont*  | SEC3DTabControl | GetFontActiveTab | Gets the current font of an active tab.  The CFont pointer passed 
// back is temporary and should not be stored for later use.
//@rdesc A pointer to a CFont that contains the current font.
//@xref<mf SEC3DTabControl::SetFontActiveTab>
//@xref<c SEC3DTabControl>


//@doc SEC3DTabControl
//@mfunc CFont*  | SEC3DTabControl | GetFontInactiveTab | Gets the current font of an inactive tab.  The CFont pointer 
// passed back is temporary and should not be stored for later use.
//@rdesc A pointer to a CFont that contains the current font.
//@xref<mf SEC3DTabControl::SetFontInactiveTab>
//@xref<c SEC3DTabControl>


// adinsert AutoDuck insertion point for SEC3DTabControl 
//@doc SEC3DTabControl 
//@class The SEC3DTabControl class implements a tab control with a three-dimensional look and feel (similar to Developer
// Studio’s Project Workspace window).  The 3-D tabs can be positioned on the top, bottom, left or right.  The SEC3DTabControl
// inherits its interface from SECTabControlBase and adds the implementation details that define its appearance.
//@comm
// The top, left and right tab position styles are supported only by the 3-D tabs and these do not apply to the 2-D tabs.
//
// Create an SEC3DTabControl in two steps. First, call the constructor SEC3DTabControl to construct the SEC3DTabControl
// object, then call the Create function (inherited from SECTabControlBase) to create the tab control’s window and attach
// it to the SEC3DTabControl object.
//
// If you create an SEC3DTabControl object within a dialog box (through a dialog
// resource), the SEC3DTabControl object is automatically destroyed when the user closes the dialog box.
//
// If you create an SEC3DTabControl object within a window, you may also need to destroy it.  An SEC3DTabControl object created 
// on the stack within a window is automatically destroyed.  If you create the SEC3DTabControl object on the heap by using
// the new function, you must call delete on the object to destroy it when the tab control window is destroyed.
//
// To handle tab control notification messages sent by an SEC3DTabControl object to its parent (usually a class derived from CWnd),
// add a message-map entry and message-handler member function to the parent class for each message.
//
// Each message-map entry takes the following form:
//
// 		ON_MESSAGE( <lt>tab control message id<gt>, memberFxn )
//
// where memberFxn is the name of the parent member function you have written to handle the notification and <lt>tab
// control message id<gt> is one of the following:
//
// 	Tab Control Message				Send To Parent When...<nl>
// 	TCM_TABSEL				A tab is selected.<nl>
// 	TCM_TABSELCLR				The tab selection is cleared.<nl>
// 	TCM_TABDBLCLK				Mouse double-clicked on a tab.
//
// The parent's function prototype is as follows:
//
// 		afx_msg LRESULT memberFxn(WPARAM wParam, LPARAM lParam);
//
// See the TABDEMO sample in the \SAMPLES\TOOLKIT\STANDARD\TABS\TABDEMO directory for a demonstration of SEC3DTabControl.
//@xref<c SECTabControlBase> <c SEC3DTabWnd>
//@BASE public | SECTabControlBase
class SEC3DTabControl : public SECTabControlBase
{
    DECLARE_DYNAMIC(SEC3DTabControl);


// Construction
public:
    
	//Access Public Members
	//@Access Constructors
	//@cmember
	/* Constructs an SEC3DTabControl object.*/
    SEC3DTabControl();
    ~SEC3DTabControl();

// Overrides
	//@Access Overridables
	//@cmember
	/* Initializes the tab.*/
    virtual void Initialize(DWORD dwStyle);

	//@cmember
	/* Creates a new tab.*/
	virtual SECTab* CreateNewTab() const;

	//@cmember
	/* Recalculates the tab layout.*/
	virtual void RecalcLayout();

	//@cmember
	/* Initializes the tab font.*/
	virtual void InitializeFonts(DWORD dwStyle);

	//@cmember
	/* Call when the tab is being activated or deactivated.*/
	virtual void OnActivateTab(int nTab);

	//@cmember
	/* Destroys all fonts, pens, and brushes used for tab control drawing.*/
	virtual void DestroyGDIObjects();

	//Added to facilitate direct tab control creation
	virtual SECTab* AddTab(LPCTSTR lpszLabel,
							  CObject* pClient,
							  HMENU hMenu,
							  void* pExtra);
	
	//Virtual overridden to allow tab resizing upon insertion of new tab
	virtual SECTab* InsertTab(int nIndex,
								 LPCTSTR lpszLabel,
								 CObject* pClient,
								 HMENU hMenu,
								 void* pExtra);

	//override needed to update the m_csLong member of SEC3DTab
	void RenameTab(int nIndex, LPCTSTR lpszLabel);
	
    //Overridden to prevent multiple invocation of OnToolHitTest.
	virtual BOOL PreTranslateMessage(MSG * pMsg);

#if !defined(_WIN32_WCE_NO_TOOLTIPS)
    //@cmember
	/* Determines whether a point is in the bounding rectangle of the specified tool.  If the point is in the */
	// rectangle, it retrieves information about the tool.  Win32 only.
	virtual int OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
#endif

// Operations

	// Font support
	//@Access Operations
	//@cmember
	/* Sets an active tab's currrent font.*/
	BOOL SetFontActiveTab(CFont* pFont,BOOL bRedraw=TRUE)
		{ return SetFontTab(pFont,SEC3DTAB_ACTIVE,bRedraw); }

	//@cmember
	/* Sets an inactive tab's current font.*/
	BOOL SetFontInactiveTab(CFont* pFont,BOOL bRedraw=TRUE)
		{ return SetFontTab(pFont,SEC3DTAB_INACTIVE,bRedraw); }

	//@cmember
	/* Gets the current font of an active tab.*/
	CFont* GetFontActiveTab() 				{ return &m_fonts[SEC3DTAB_ACTIVE]; }

	//@cmember
	/* Gets the current font of an inactive tab.*/
	CFont* GetFontInactiveTab() 			{ return &m_fonts[SEC3DTAB_INACTIVE]; }

	//Called for dynamic sizing
	 void Rename3DTab(int nTab, LPCTSTR lpszLabel);

	// Set Tab Icon
	// Default size set to 16x16 to avoid downward scaling. If not found,
	// 32x32 alternative will be loaded, then scaled to fit 16x16.
	//@cmember
	/* Sets the tab icon.*/
	virtual void SetTabIcon(int nIndex, HICON hIcon);

	//@cmember
	/* Sets the tab icon.*/
	virtual void SetTabIcon(int nIndex, UINT nIDIcon,
							int cx=SEC_TAB_DEFICON_CX, int cy=SEC_TAB_DEFICON_CY);

	//@cmember
	/* Sets the tab icon.*/
	virtual void SetTabIcon(int nIndex, LPCTSTR lpszResourceName,
							int cx=SEC_TAB_DEFICON_CX, int cy=SEC_TAB_DEFICON_CY);

	//@cmember
	/* Invalidates tab for repainting.*/
	virtual void InvalidateTab(int nTab);

	//@cmember
	/* Enables or disables the tab.*/
	virtual void EnableTab(int nIndex, BOOL bEnable=TRUE);

	//@cmember
	/* Specifies whether the tab is enabled or disabled.*/
	virtual BOOL IsTabEnabled(int nIndex);

   //@cmember
   /* Enables/Disables the tab sizing feature.*/ 
	void EnableTabSizing(BOOL bVal = TRUE);

#ifdef WIN32
	//@cmember
	/* Modifies the style. Win32 only*/
	virtual BOOL ModifyStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0);
#endif

	int GetLeftOffset() { return m_nLeftOffset; }
	int GetTopOffset(){ return m_nTopOffset; }
	int GetDefTabHeight() { return m_nDefTabHeight; }
	int GetDefTabWidth() { return m_nDefTabWidth; }
	void SetDefTabHeight(int nHeight) { m_nDefTabHeight=nHeight; }
	void SetDefTabWidth(int nWidth) { m_nDefTabWidth=nWidth; }
	void SetLeftOffset(int nOffset) { m_nLeftOffset=nOffset; }
	void SetTopOffset(int nOffset) { m_nTopOffset=nOffset; }

//Protected Members used for dynamic resizing of tabs
protected:
	BOOL m_bSizing;
	int m_nDefTabHeight,m_nDefTabWidth,m_nLeftOffset,m_nTopOffset;

#ifdef _AFXDLL
	BOOL m_bModuleDiff;
#endif //_AFXDLL

	//Forwards mouse messages to CWnd::FilterToolTipMessage.
	void RelayTabToolTipEvent(const MSG* pMsg);	

protected:
	//helper members for sizing
	virtual void SizeTabs(int nTot, int nTIcon, int nDimParam);
	void CalcLabelWidth(int& nTotBuff, int& nTotalIconBuff, BOOL bLong = FALSE);
	int GetTabBig();

// Implementation
protected:
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

	// GDI Objects
    CFont m_fonts[2];
    CPen m_pens[2];
    CPen m_penShadow;
    CPen m_penWhite;
//    CBrush m_brushes[2];
    COLORREF m_rgbText[2];
	int m_bShowTabLabels;	// show labels since tabs all tabs fit

	virtual BOOL SetFontTab(CFont* pFont,int nTabType=SEC3DTAB_ACTIVE,BOOL bRedraw=TRUE);

public:
// Queries
	//@Access Queries
	//@cmember
	/* Indicates whether a given point is within the tab rectangle.*/
	BOOL TabHit(int nTab, CPoint& point) const;

	//@cmember
	/* Returns a reference to the specified tab object.*/
    SEC3DTab& GetTab(int nTab);

	//@cmember
	/* Returns a pointer to the specified tab object.*/
	SEC3DTab* GetTabPtr(int nTab) const;

	//@cmember
	/* Retrieves the tab rectangle.*/
	virtual void GetTabRect(int nTab, CRect &tabRect) const;

	//@cmember
	/* Retrieves the width of the tab label.*/
	int GetLabelWidth(int nTab, BOOL bLong=FALSE);

protected:
// Layout
	virtual void SetTabPos(int nTab, int x, int y, DWORD dwStyle);

// Drawing
	virtual void DrawTab(CDC* pDC, int nTab);
    virtual void rgbFill(CDC* pDC,int x,int y,int cx,int cy, COLORREF rgb);
	virtual void DrawTabBottom(CDC* pDC, SEC3DTab* pTab, DWORD dwStyle);
	virtual void DrawTabTop(CDC* pDC, SEC3DTab* pTab, DWORD dwStyle);
	virtual void DrawTabLeft(CDC* pDC, SEC3DTab* pTab, DWORD dwStyle);
	virtual void DrawTabRight(CDC* pDC, SEC3DTab* pTab, DWORD dwStyle);
	virtual void DrawInterior(CDC* pDC, SEC3DTab* pTab, DWORD dwStyle);
	virtual void DrawDisabled(CDC* pDC, int nWidth, int nHeight);

    // Generated message map functions
protected:
    //{{AFX_MSG(SEC3DTabControl)
    afx_msg void OnDestroy();
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSysColorChange();
	afx_msg void OnCaptureChanged( CWnd* pWnd );
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

#endif // __TABCTRL3_H__
/////////////////////////////////////////////////////////////////////////////

