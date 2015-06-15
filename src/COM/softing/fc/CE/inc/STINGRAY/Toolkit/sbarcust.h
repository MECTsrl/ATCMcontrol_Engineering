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
//
//  Author:			John B. Williston
//  Description:	Definition of SECCustomStatusBar
//  Created:		July 1996
//

#ifndef __SBARCUST_H__
#define __SBARCUST_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#ifdef WIN32

#ifndef __AFXCMN_H__
#include "afxcmn.h"
#endif

#ifndef __SBARSTAT_H__
#include "toolkit\sbarstat.h"
#endif

#ifndef __SECPROGRESSCTRL_H__
#include "toolkit\progress.h"
#endif

#ifndef __SDOCKCNT_H__
#include "toolkit\sdockcnt.h"
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

//@doc
//@topic PANEINFOEX Overview |
// The SECCustomStatusBar adds several new configurable attributes to your frame window’s 
// status bar.  <t PANEINFOEX> is a structure which aggregates all attributes of one pane of a custom 
// status bar.  All attributes introduced by SECCustomStatusBar and even those defined by its 
// SECStatusBar base class are included.  By aggregating all attributes into one data structure, 
// it becomes much more convenient to set and/or retrieve pane attributes.  For this reason,  
// both <mf SECCustomStatusBar::SetPaneInfoEx> and <mf SECCustomStatusBar::GetPaneInfoEx> expect 
// a <t PANEINFOEX> type argument.
//
// See the STATBAR sample in the \SAMPLES\TOOLKIT\STANDARD\UIEXT\STATBAR directory for a demonstration 
// of how <t PANEINFOEX> is used by <c SECCustomStatusBar> to provide custom status bar handling.
//@ex
// The following code fragment shows how flags must be set for each attribute you want a 
// status bar pane to have: |
// // Configure the second pane to display the light bitmap and use the gripping hand cursor.
// 
// PANEINFOEX pex;
// pex.iIndex = 2;
// pex.hCursor = AfxGetApp()->LoadCursor(IDC_GRIPPING_HAND);
// pex.pBitmap = m_pBitmap;
// pex.iFlags = SBP_CURSOR | SBP_BITMAP;
// m_wndStatusBar.SetPaneInfoEx(&pex);
//@devnote SBP_TEXT, SBP_BITMAP, and SBP_STYLE are mutually exclusive.
//@xref <c SECCustomStatusBar> <t PANEINFOEX>
//@xref <l Custom status bar styles> 
//@xref <l Custom status bar "window-pane" styles>
//@xref <mf SECCustomStatusBar::SetPaneInfoEx>
//@xref <mf SECCustomStatusBar::GetPaneInfoEx>
//@end

//@doc
//@topic Custom status bar styles | Here are the <l PANEINFOEX> styles supported by <c SECCustomStatusBar>
//@flag   SBP_ID | Set the id of the pane to the uiID member of PANEINFOEX.
const int SBP_ID			= 0x1;
//@flag   SBP_STYLE | Set the style of the pane to that of the uiStyle member of PANEINFO.
const int SBP_STYLE			= 0x2;
//@flag   SBP_WIDTH | Use the cxWidth member of PANEINFO for the width.
const int SBP_WIDTH			= 0x4;
//@flag   SBP_TEXT | Use the text set in the strText member of PANEINFO for the pane.
const int SBP_TEXT			= 0x8;
//@flag   SBP_TEXT_ALIGN | Use the iTextAlignment member of PANEINFO for text alignment (Text alignment
// flags like TA_LEFT, etc., are defined in WINGDI.H).
const int SBP_TEXT_ALIGN	= 0x10;
//@flag   SBP_FOREGROUND | Use the COLORREF set in the crTextForeground member of PANEINFOEX for 
// the foreground color.
const int SBP_FOREGROUND	= 0x20;
//@flag   SBP_BACKGROUND | Use the COLORREF set in the crTextBackground member of PANEINFOEX for
// the background color.
const int SBP_BACKGROUND	= 0x40;
//@flag   SBP_BITMAP | Display the CBitmap pointed to by the pBitmap member of PANEINFOEX in the pane.
const int SBP_BITMAP		= 0x80;
//@flag   SBP_CURSOR | Display the cursor specified by the hCursor member of PANEINFOEX when the 
// mouse pointer is over the pane.
const int SBP_CURSOR		= 0x100;
const int SBP_ALL			= 0xFFFFFFFF;
//@xref <c SECCustomStatusBar> 
//@xref <t PANEINFOEX> 
//@xref <l PANEINFOEX Overview>
//@end

//@doc
//@topic Custom status bar "window-pane" styles | The following styles are supported by <c SECCustomStatusBar>
//@flag SEC_STATBAR_NOSIZE_CX | Do not automatically size the pane width.
#define SEC_STATBAR_NOSIZE_CX		0x0001
//@flag SEC_STATBAR_NOSIZE_CX | Do not automatically size the pane height.
#define SEC_STATBAR_NOSIZE_CY		0x0002
//@flag SEC_STATBAR_SUNKEN_BORDER | Provide a sunken border for the pane.
#define SEC_STATBAR_SUNKEN_BORDER	0x0004
//@xref <c SECCustomStatusBar>
//@xref <mf SECCustomStatusBar::RegisterWndToPane> 
//@xref <mf SECCustomStatusBar::GetRegisteredWndSizeFlags>
//@xref <mf SECCustomStatusBar::ResizeAllRegWnd>
//@end

//@struct PANEINFOEX | PANEINFOEX struct for <c SECCustomStatusBar>
typedef struct tagPaneInfoEx
{
	// Basic information supported by SECStatusBar::SetPaneInfo.

	int iIndex;		//@field The pane index (supported by <mf SECStatusBar::SetPaneInfo>).
	UINT uiID;		//@field The pane id (supported by <mf SECStatusBar::SetPaneInfo>).
	UINT uiStyle;	//@field The pane style (supported by <mf SECStatusBar::SetPaneInfo>).
	int cxWidth;	//@field The pane width (supported by <mf SECStatusBar::SetPaneInfo>).

	// Extra information supported via SECStatusBar::SetPaneText.

	CString strText; //@field The pane text (supported by <mf SECStatusBar::SetPaneText>).

	// Custom pane handling provided by SECCustomStatusBar.

	COLORREF crTextForeground;	//@field Text foreground color.
	COLORREF crTextBackground;	//@field Text background color.
	CBitmap* pBitmap;			//@field Bitmap pointer.
	HCURSOR hCursor;			//@field Cursor handle.
	int iTextAlign;				//@field Text alignment (see WINGDI.H for text alignment flags).
	int iFlags;					//@field <l Custom status bar styles>.

	tagPaneInfoEx()
	{
		iIndex = 0;
		uiID = 0;
		uiStyle = 0;
		cxWidth = 0;
		crTextForeground = 0;
		crTextBackground = 0;
		pBitmap = NULL;
		hCursor = 0;
		iTextAlign = TA_LEFT;
		iFlags = 0;
	}

	tagPaneInfoEx& operator=(const tagPaneInfoEx& rhs)
	{
		if (&rhs != this)
		{
			iIndex = rhs.iIndex;
			uiID = rhs.uiID;
			uiStyle = rhs.uiStyle;
			cxWidth = rhs.cxWidth;
			strText = rhs.strText;
			crTextForeground = rhs.crTextForeground;
			crTextBackground = rhs.crTextBackground;
			pBitmap = rhs.pBitmap;
			iTextAlign = rhs.iTextAlign;
			iFlags = rhs.iFlags;
		}

		return *this;
	}

} PANEINFOEX;


/////////////////////////////
// AutoDuck tag block block for class SECCustomStatusBar

// adinsert AutoDuck insertion point for class SECCustomStatusBar : public SECStatusBar
//@doc SECCustomStatusBar
//@class The SECCustomStatusBar class implements a Windows status bar that is more 
// functional and configurable than MFC’s CStatusBar.  
//
//@comm Notice that SECCustomStatusBar 
// inherits from <c SECStatusBar>, which is an interface equivalent replacement for 
// CStatusBar and serves as the base class for your application’s status bar.  
// SECCustomStatusBar inherits all the functionality of a normal MFC status bar and 
// adds a host of new features.  With SECCustomStatusBar, you can configure the fonts 
// used in status bar panes, the text alignment, and foreground and background colors.  
// SECCustomStatusBar also adds a progress indicator which can be programmatically shown 
// in place of the status bar panes and later hidden when there is no progress to illustrate.
//
// With CStatusBar, panes can only contain text.  SECCustomStatusBar adds the ability 
// to embed bitmaps in status bar panes.  Moreover, SECCustomStatusBar allows custom 
// cursor bitmaps to be assigned to individual status bar panes.  When the cursor is 
// inside the pane, the cursor takes the form of the specified bitmap.  Outside of the 
// pane, the cursor is its normal shape.  Lastly, SECCustomStatusBar adds member functions 
// that facilitate the processing of mouse events inside a status bar pane.
//
// To make use of the SECCustomStatusBar class inside your frame windows, change the 
// type of your status bar from CStatusBar (or <c SECStatusBar>) to SECCustomStatusBar.  
//
// Assuming you want to perform some status bar customizations, the next step is to call 
// the necessary SECCustomStatusBar member functions (i.e., <mf SECCustomStatusBar::SetPaneInfoEx>) 
// from your overridden CFrameWnd::OnCreate member function.
//
// See the STATBAR sample in the \SAMPLES\TOOLKIT\UIEXT\STATBAR directory for a demonstration of 
// how to use the SECCustomStatusBar class.
//@devnote Any status bar formally derived from CStatusBar must be rederived from SECStatusBar.  
// No CStatusBars are allowed when using Objective Toolkit’s docking window enhancements, 
// as they lack the expected member variables.)
//@xref <c SECStatusBar> 
//@xref <l PANEINFOEX Overview>
//@base public | SECStatusBar
class SECCustomStatusBar : public SECStatusBar
{
	DECLARE_DYNAMIC( SECCustomStatusBar )
public:

	//@cmember,menum
	/* Pane "Fit" Types */
	enum FitType {
		FitWndToPane,	//@@emem Fit the window to the pane.
		FitPaneToWnd,	//@@emem Fit the pane to the window.
		NoFit			//@@emem Do not try to fit either the window or the pane.
		};

protected:


	typedef struct tagWCPane
	{
		COLORREF crFore, crBack;	// foreground and background text colors
		CBitmap* pBitmap;			// pointer to bitmap to display
		HCURSOR hCursor;			// used for setting the cursor
		int iTextAlign;				// text alignment flags
		int iFlags;					// flags indicating which data to use

		tagWCPane()
		{
			crFore = crBack = 0;
			pBitmap = NULL;
			hCursor = 0;
			iTextAlign = TA_LEFT;
			iFlags = 0;
		}

	} WCPane;

public:

	//@access Creation/Initialization

	//@cmember
	/* Constructs a SECCustomStatusBar object.*/
	SECCustomStatusBar();

public:
	// Attributes

	BOOL IsProgressControlActive() const		
		{ return m_pProgressCtrl != NULL; }
	
public:

	//@access Operations

	//@cmember
	/* Maps a CWnd-derived control to a specific status bar pane for automatic sizing and visibility manipulations.*/
	void RegisterWndToPane(int nPane,CWnd* pWnd,FitType fit=FitWndToPane,UINT nSizeFlags=0);

	//@cmember
	/* Retrieves the CWnd-derived control from the given pane.*/
	CWnd* GetRegisteredWnd(int nPane);

	//@cmember
	/* Retrieves the size flags for the given pane.*/
	UINT GetRegisteredWndSizeFlags(int nPane);

	// Custom pane information API.
	
	//@cmember
	/* Returns the attributes of the specified pane.*/
	void GetPaneInfoEx(PANEINFOEX* pex) const;
	
	//@cmember
	/* Sets the attributes of the specified pane.*/
	void SetPaneInfoEx(const PANEINFOEX* pex, BOOL bFit = TRUE);

	//@cmember
	/* Increments the progress indicator.*/
	void StepProgress();
	
	//@cmember
	/* Shows amount of progress on the indicator.*/
	void SetProgress(int iPos);

	//@cmember
	/* Sets the step size.*/
	void SetStepProgress(int nStep);

	// Resets horizontal pane sizing for non-stretchy panes; very
	// useful when changing fonts or using bitmaps.
	//@cmember
	/* Resets horizontal pane sizing for non-stretchy panes.*/
	void FitPanesToContent();

	// Useful for processing WM_NOTIFY messages to further
	// customize control response.

	int GetIndexFromPoint(const CPoint& pt) const
		{ return PaneIndexFromPoint(pt); }

public:
	virtual ~SECCustomStatusBar();

	//@access Overridable
public:
	
	// Progress control API.
	//@cmember
	/* Initializes and shows the progress indicator.*/
	virtual BOOL InitializeProgressControl(LPCTSTR pcszMsg = NULL, int iLow = 0, 
								   int iHigh = 100, BOOL bResetPos = TRUE,
								   DWORD dwExStyle=SEC_EX_PROGRESS_DEFAULTS, int nStep = 10);

	//@cmember
	/* Deletes the progress indicator and restores the status bar to its original content.*/
	virtual void UninitializeProgressControl();
	
protected:
	
	// "Registered window" manipulation
	//@cmember
	/* Sets the visibility of the registered windows.*/
	virtual void SetVisibleAllRegWnd(BOOL bShow=TRUE);
	
	//@cmember
	/* Resizes all registered windows based on current attributes.*/
	virtual void ResizeAllRegWnd();
	
	// access Overrides
public:

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SECCustomStatusBar)
	//}}AFX_VIRTUAL

	// 
	BOOL SetIndicators(const UINT* lpIDArray, int nIDCount);
	
	// 
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	
	// 
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);

protected:
	
	// Generated message map functions

	//{{AFX_MSG(SECCustomStatusBar)

	// 
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

	//}}AFX_MSG

	// 
	afx_msg LRESULT OnSizeParent(WPARAM wParam, LPARAM lParam);
	
	// 
	afx_msg LRESULT OnSetFont(WPARAM wParam, LPARAM lParam);
	
	// 
	afx_msg LRESULT OnSetMinHeight(WPARAM wParam, LPARAM);
	
	// 
	afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM);

	// access Implementation

protected:

	// 
	BOOL SimpleRect(CDC* pDC, LPRECT p, COLORREF c);
	
	// 
	CSize GetPaneTextExtent(int iIndex, const CString& str);
	
	// 
	void AllocatePaneInfo();
	
	// 
	void ForcePaneOwnerdraw();
	
	// 
	void ForceOnSizeParent();
	
	// 
	void FreePaneInfo()
		{ delete m_pPanes; }
	
	// 
	void GetPaneRect(int iIndex, LPRECT p) const;
	
	// 
	WCPane* PaneFromPoint(const CPoint& pt) const;
	
	// 
	int PaneIndexFromPoint(const CPoint& pt) const;

	// 
	BOOL ResetIndicators();
	
protected:

	// access Protected data members

	// The progress control associated with this status bar.
	SECProgressCtrl* m_pProgressCtrl;
	
	// Saved pane info (restored when progress control is removed).
	PANEINFOEX* m_pFirstPaneData;
	
	// Pointer to the panes.
	WCPane* m_pPanes;
	
	// List of registered windows.
	CPtrArray m_arrRegisterWnd;
	
	// Array of registered windows size flags.
	CUIntArray m_arrRegisterWndSizeFlags;

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

#endif // WIN32

#endif //__SBARCUST_H__

/////////////////////////////////////////////////////////////////////////////

