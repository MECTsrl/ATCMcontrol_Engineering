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
//  Description:  Declarations for SECTabControl
//

#ifndef __TABCTRL_H__
#define __TABCTRL_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#ifndef __TABCTRLB_H__
#include "toolkit\tabctrlb.h"
#endif //__TABCTRLB_H__

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
// SECTabControl window

// Tab Control Styles
#define TCS_LEFTRIGHTSCROLL	0x0001
#define TCS_FULLSCROLL		0x0002

// Tab Control Button Masks
#define TCBM_NONE		0x00
#define TCBM_FIRST		0x01
#define TCBM_LEFT		0x02
#define TCBM_RIGHT		0x04
#define TCBM_LAST		0x08

// Array indexes for GDI objects
#define SECTAB_UNSELECTED		0
#define SECTAB_SELECTED			1
#define SECTAB_ACTIVE			2

/////////////////////////////
// AutoDuck tag block block for SECTabControl 


//@doc SECTabControl
//@mfunc BOOL  | SECTabControl | SetFontSelectedTab | Sets a selected tab’s current font to the specified font.  If
// bRedraw is TRUE, the tab will also be redrawn. The CFont object passed by pointer will be copied to duplicate storage,
// so the caller need not hang onto the pointer.
//@rdesc BOOL indicating outcome.
//@parm CFont*  | pFont | Specifies the new font.
//@parm BOOL | bRedraw | If TRUE, redraw the SECTabControl object
//@xref<mf SECTabControl::GetFontSelectedTab>
//@xref<c SECTabControl>


//@doc SECTabControl
//@mfunc BOOL  | SECTabControl | SetFontUnselectedTab | Sets an unselected tab’s current font to the specified font.  If 
// bRedraw is TRUE, the tab will also be redrawn. The CFont object passed by pointer will be copied to duplicate storage, 
// so the caller need not hang onto the pointer.
//@rdesc BOOL indicating outcome.
//@parm CFont*  | pFont | Specifies the new font.
//@parm BOOL | bRedraw | If TRUE, redraw the SECTabControl object.
//@xref<mf SECTabControl::GetFontUnselectedTab>
//@xref<c SECTabControl>



//@doc SECTabControl
//@mfunc BOOL  | SECTabControl | SetFontActiveTab | Sets an active tab’s current font to the specified font.  If bRedraw
// is TRUE, the tab will also be redrawn. The CFont object passed by pointer will be copied to duplicate storage, so the
// caller need not hang onto the pointer.
//@parm CFont*  | pFont | Specifies the new font.
//@parm BOOL |bRedraw | If TRUE, redraw the SECTabControl object.
//@xref<mf SECTabControl::GetFontActiveTab>
//@xref<c SECTabControl>

//@doc SECTabControl
//@mfunc CFont*  | SECTabControl | GetFontSelectedTab | Gets the current font of a selected tab.  The CFont pointer passed
// back is temporary and should not be stored for later use.
//@rdesc A pointer to a CFont that contains the current font.
//@xref<mf SECTabControl::SetFontSelectedTab>
//@xref<c SECTabControl>


//@doc SECTabControl
//@mfunc CFont*  | SECTabControl | GetFontUnselectedTab | Gets the current font of an unselected tab.  The CFont pointer
// passed back is temporary and should not be stored for later use.
//@rdesc A pointer to a CFont that contains the current font.
//@xref<mf SECTabControl::SetFontUnSelectedTab>
//@xref<c SECTabControl>



//@doc SECTabControl
//@mfunc CFont*  | SECTabControl | GetFontActiveTab | Gets the current font of an active tab.  The CFont pointer passed 
// back is temporary and should not be stored for later use.
//@rdesc A pointer to a CFont that contains the current font.
//@xref<mf SECTabControl::SetFontActiveTab>
//@xref<c SECTabControl>


//@doc SECTabControl
//@mfunc void  | SECTabControl | SetMultiSelect | Call this function to enable/disable multi-selection of tabs.
//@parm  BOOL | bMultiSelect | Pass TRUE to enable multi-selection and FALSE to disable. 


//@doc SECTabControl
//@mfunc BOOL  | SECTabControl | GetMultiSelect | Returns a BOOL indicating whether multi-selection is enabled or not.
//rdesc TRUE indicates multi-selection enabled.



// adinsert AutoDuck insertion point for SECTabControl 
//@doc SECTabControl 
//@class The SECTabControl class implements a tab control with a two-dimensional (Excel-like) 
// look and feel.  The SECTabControl inherits its interface from SECTabControlBase and adds the 
// implementation details that define its appearance.
//@comm
// Create an SECTabControl in two steps.  First, call the constructor SECTabControl to construct the
// SECTabControl object, then call the <mf SECTabControl::Create> member function to create the tab 
// control’s window and attach it to the SECTabControl object.
//
// If you create an SECTabControl object within a dialog box (through a dialog resource), the
// SECTabControl object is automatically destroyed when the user closes the dialog box.
//
// If you create an SECTabControl object within a window, you may also need to destroy it.  
// A SECTabControl object created on the stack within a window is automatically destroyed. If you 
// create the SECTabControl object on the heap by using the new function, you must call delete
// the object to destroy it when the tab control window is destroyed.
//
// To handle tab control notification messages sent by an SECTabControl object to its parent 
// (usually a class derived from CWnd), add a message-map entry and message-handler member function 
// to the parent class for each message.
//
// Each message-map entry takes the following form:
//
//		ON_MESSAGE( <lt>tab control message id<gt>, memberFxn )
//
// where memberFxn is the name of the parent member function you have written to handle the 
// notification and <lt>tab control message id<gt> is one of the following:
//
//	Tab Control Message		Send To Parent When...<nl>
//	TCM_TABSEL				A tab is selected.<nl>
//	TCM_TABSELCLR			The tab selection is cleared.<nl>
//	TCM_TABDBLCLK			Mouse double-clicked on a tab.
//
//The parent's function prototype is as follows:
//
//		afx_msg LRESULT memberFxn(WPARAM wParam, LPARAM lParam);<nl>
//
//See the TABDEMO sample in the \SAMPLES\TOOLKIT\STANDARD\TABS\TABDEMO directory for a demonstration 
// of SECTabControl.
//@xref<c SECTabControlBase> <c SEC3DTabControl> <c SECTab> <c SECTabWnd> 
//@BASE public | SECTabControlBase
class SECTabControl : public SECTabControlBase
{
    DECLARE_DYNAMIC(SECTabControl);

// Construction
public:
	//@Access Public Members
	//@Access Constructors
	//@cmember
	/* Constructs an SECTabControl object.*/
    SECTabControl();
    ~SECTabControl();

public:

// Operations
    //@Access Operations 
	//@cmember
	/* Scrolls tabs so that index passed in is now visible.*/
    void ScrollToTab(int nTab);

	// Font support
	

	//@cmember
	/* Sets a selected tab’s current font.*/
	BOOL SetFontSelectedTab(CFont* pFont,BOOL bRedraw=TRUE)
		{ return SetFontTab(pFont,SECTAB_SELECTED,bRedraw); }
	
	//@cmember
	/* Sets an unselected tab’s current font.*/
	BOOL SetFontUnselectedTab(CFont* pFont,BOOL bRedraw=TRUE)
		{ return SetFontTab(pFont,SECTAB_UNSELECTED,bRedraw); }
	
	//@cmember
	/* Sets an active tab’s currrent font.*/
	BOOL SetFontActiveTab(CFont* pFont,BOOL bRedraw=TRUE)
		{return SetFontTab(pFont,SECTAB_ACTIVE,bRedraw); }
	
	//@cmember
	/* Gets the current font of a selected tab.*/
	CFont* GetFontSelectedTab()
		{ return &m_fonts[SECTAB_SELECTED]; }
	
	//@cmember
	/* Gets the current font of an unselected tab.*/
	CFont* GetFontUnselectedTab()
		{ return &m_fonts[SECTAB_UNSELECTED]; }
	
	//@cmember
	/* Gets the current font of an active tab.*/
	CFont* GetFontActiveTab()
		{ return &m_fonts[SECTAB_ACTIVE]; }

	//@cmember
	/* Toggles the enable multi-selection parameter of the tab control.*/
	void SetMultiSelect( BOOL bMultiSelect )
		{ m_bMultiSelect = bMultiSelect; }

	//@cmember
	/* Returns value indicating whether multi-selection is enabled or not.*/
	BOOL GetMultiSelect(){ return m_bMultiSelect; }


// Overridables
	//@Access Overridable
	//@cmember
	/* Initializes tab control with the style specified.*/
    virtual void Initialize(DWORD dwStyle);

	//@cmember
	/* Draws specified tab by reference to a CRgn region with clipping as determined by a pointer to a CDC.*/
    virtual void DrawTab(CDC *pDC, int nTab, CRgn &tabRgn);

	//@cmember
	/* Invalidates specified tab for redrawing.*/
	virtual void InvalidateTab(int nTab);

	//@cmember
	/* Recalculates tab control’s layout.*/
	virtual void RecalcLayout();

   //Override needed to update 3D tab specific data members.
	virtual SECTab* InsertTab(int nIndex,
							  LPCTSTR lpszLabel,
							  CObject* pClient,
							  HMENU hMenu,
							  void* pExtra);

// Queries
	//@Access Queries
	//@cmember
	/* Determines if point passed in resides inside tab at index nTab.*/
	virtual BOOL TabHit(int nTab, CPoint& point) const;

// Implementation
protected:
	virtual BOOL SetFontTab(CFont* pFont,int nTabType=SECTAB_ACTIVE,BOOL bRedraw=TRUE);

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    int		m_nScrollOffset;
    int		m_nHeight;
	BOOL	m_bMultiSelect;

    // GDI Objects
    CFont	m_fonts[3];
    CPen	m_pens[3];
    CPen	m_penShadow;
    CBrush	m_brushes[3];
    COLORREF	m_rgbText[3];
        
    // Button attributes
    int		m_nButtons, m_nButtonPressed, m_nDesensedButtons;
    CSize	m_sizeBtn;
    
    void rgbFill(CDC* pDC,int x,int y,int cx,int cy, COLORREF rgb);
    void DrawButton(CDC *pDC, int btn);
    void GetTabRgn(int nTab, CRgn &tabRgn) const;
    void GetTabsRect(CRect& tabArea) const;
    void GetButtonsRect(CRect& btnArea);
    void GetButtonRect(int btn, CRect& rect);
    void RecalcButtonSensitivity();
    void OnButtonPress(int nButtonCode);
	int GetLabelWidth(int nTab);

	// Constant metrical values
	const int m_nTabSpacing;
	const int m_nTabFold;
	const int m_nScrollIncr;
	const int m_nLabelPadding;
	const int m_nNextTabShowing;
    
    // Generated message map functions
protected:
    //{{AFX_MSG(SECTabControl)
    afx_msg void OnDestroy();
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSysColorChange();
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

#endif // __TABCTRL_H__
/////////////////////////////////////////////////////////////////////////////

