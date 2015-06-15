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
//  Description:	Declarations for SECControlBar
//  Created:		May 1996
//
//	Modified:	 	March 1997 (Mark Isham)
//	Description: 	Revamped gripper support
//	
//
//	Modified:	 	June 1998 (Brad King)
//	Description: 	SECReBar added for VC6.0

#ifdef WIN32

#ifndef __SBARCORE_H__
#define __SBARCORE_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
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


/////////////////////////////////////////////////////////////////////////////
// SECControlBar

// Forward Declarations
class SECControlBarInfo;
class SECControlBarManager;

// Constants
#define CX_BORDER   1
#define CY_BORDER   1

// User-defined messages
#define WM_EXTENDCONTEXTMENU		0x38ff	// replaces	(WM_USER+800) to avoid
											// conflict with help subsystem messages.
// Extended Styles
#define CBRS_EX_STDCONTEXTMENU		0x0001L  // Standard context menu items
#define CBRS_EX_STRETCH_ON_SIZE		0x0002L  // Control bar is stretched on resize
#define CBRS_EX_UNIDIRECTIONAL		0x0004L  // Bar can be sized in one direction
                                             //    at a time (i.e. toolbar)
#define CBRS_EX_DRAWBORDERS			0x0008L  // Draw a border around the bar
#define CBRS_EX_BORDERSPACE			0x0010L  // Leave border space for ease of dragging
#define CBRS_EX_ALLOW_MDI_FLOAT		0x0020L  // Control bar can be re-parented
											 //    by an MDI Child window
#define CBRS_EX_SIZE_TO_FIT			0x0040L  // Size the (single) child to fit
#define CBRS_EX_FULL_ROW			0x0080L	 // control bar will always occupy entire row

// Not yet implemented
//#define CBRS_EX_DISALLOW_FLOAT		0x0100L  // Control bar can only be docked


// You probably just want to set the CBRS_EX_COOL style in your app,
// but you can also play around with these styles to customize
// Note: As of OT 5.0, gripper requires coolboders, and close reqs gripper.
#define CBRS_EX_COOLBORDERS			0x0100L  // floating buttons, no border
#define CBRS_EX_GRIPPER				0x0200L	 // draw dragging "gripper"
#define CBRS_EX_GRIPPER_CLOSE		0x0400L	 // draw close button on gripper
#define CBRS_EX_GRIPPER_EXPAND		0x0800L	 // draw expand/contract button
#define CBRS_EX_TRANSPARENT			0x1000L	 // toolbar buttons drawn transparently
#define CBRS_EX_DISALLOW_FLOAT		0x2000L	 // Control bar can only be docked // not implemented
#define CBRS_EX_MENU_STYLE			0X4000L  // Do not become large unless a large object
											 // is dropped on bar.
#define CBRS_EX_COOL				CBRS_EX_COOLBORDERS | CBRS_EX_GRIPPER | \
									CBRS_EX_GRIPPER_CLOSE | CBRS_EX_GRIPPER_EXPAND

////////////////////////////////////////////////////////////////////////
// SECGripperInfo
//
class SECGripperInfo : public CObject {
public:
	SECGripperInfo();
	~SECGripperInfo();

	// override these for your own custom gripper
	virtual int GetWidth();
	virtual int GetHeight();

public:
	// horizontal spacing
	int m_cxPad1;
	int m_cxWidth1;
	int m_cxPad2;
	int m_cxWidth2;
	int m_cxPad3;

	// vertical spacing
	int m_cyPad1;
	int m_cyWidth1;
	int m_cyPad2;
	int m_cyWidth2;
	int m_cyPad3;

	// padding between border and client area for all non-gripper sides
	// when gripper style enabled
	int m_nGripperOffSidePadding;

	// Tooltip text 
	CString m_strCloseTipText;
	CString m_strExpandTipText;
	CString m_strContractTipText;
};


/////////////////////////////////////////////////////////////////////////
// SECControlBar
//

/////////////////////////////
// AutoDuck tag block block for SECControlBar 

//@doc SECControlBar
//@mdata static BOOL  | SECControlBar | m_bOptimizedRedrawEnabled | This member controls the
// delayed redraw methods used to control flicker on redraw. It is sometimes necessary to 
// turn off this functionality
//@xref <mf SECControlBar::SetOptimizedRedrawEnabled>
 
//@doc SECControlBar
//@mdata CSize  | SECControlBar | m_szDockHorz | The  size of the bar when docked horizontally.

//@doc SECControlBar
//@mdata CPoint  | SECControlBar | m_ptDockHorz | The upper left point of the bar when docked horizontally. 

//@doc SECControlBar
//@mdata CSize  | SECControlBar | m_szDockVert | The size of the bar when docked vertically.

//@doc SECControlBar
//@mdata CSize  | SECControlBar | m_szFloat | The size of the bar when floating.

//@doc SECControlBar
//@mdata DWORD  | SECControlBar | m_dwMRUDockingState | The most recent docking state of the bar.

//@doc SECControlBar
//@mdata float  | SECControlBar | m_fPctWidth | The percentage of the width (or height if docked vertically)
// that the bar wishes to occupy if the the bar is undocked. 

//@doc SECControlBar
//@mdata float  | SECControlBar | m_fDockedPctWidth | The percentage of the width (or height if docked vertically)
// that the bar wishes to occupy if the the bar is docked. This percentage may not be allowed
// if all of the docked bars request percentages adding up to more than one hundred percent. 

//@doc SECControlBar
//@mdata DWORD | SECControlBar | m_dwExStyle | Extended style bit flags
//
// CBRS_EX_STDCONTEXTMENU   Control bar is given the standard context menu (i.e., Allow docking, and Show/Hide control bar menu items).<nl>
// CBRS_EX_STRETCH_ON_SIZE   When the control bar is resized, all child windows are stretched and repositioned so as to preserve the proportions of the control bar.  If you require another form of resize handling, be sure to omit this extended style from the Create call and override SECControlBar::OnSize.<nl>
// CBRS_EX_DRAWBORDERS   Draw a border around the bar.<nl>
// CBRS_EX_BORDERSPACE   Leave border space for ease of dragging.<nl>
// CBRS_EX_ALLOW_MDI_FLOAT   Control bar can be re-parented by an MDI child window.<nl>
// CBRS_EX_SIZE_TO_FIT   Size the (single) child to fit.<nl>
// CBRS_EX_UNIDIRECTIONAL   The control bar can be sized in one dimension at a time (no diagonal sizing allowed).  In addition, a change in height dictates a new width and vice versa.  A toolbar is an example of a unidirectional control bar.<nl>
// CBRS_EX_COOLBORDERS   Floating buttons, no border.<nl>
// CBRS_EX_GRIPPER   Draw the dragging gripper.<nl>
// CBRS_EX_GRIPPER_CLOSE   Draw the close button on gripper.<nl>
// CBRS_EX_GRIPPER_EXPAND   Expand/contract control bar button.<nl>
// CBRS_EX_COOL = CBRS_EX_COOLBORDERS or CBRS_EX_GRIPPER or CBRS_EX_GRIPPER_CLOSE or CBRS_EX_GRIPPER_EXPAND   Control bar will have the “cool look” – a flat, painted look similar to the control bars seen in Microsoft Developer Studio.  These extended style options allow you to customize the “cool look” for your application.  By default, customizable toolbars are CBRS_EX_COOLBORDERS or CBRS_EX_GRIPPER; all other control bars are CBRS_EX_COOL.  NOTE:  As of Objective Toolkit 5.0, gripper requires coolborders, and close requires gripper.  Also, the gripper drawing code has been virtualized, so you can easily plug in your own gripper (or modify the existing gripper) with just one or two overrides.<nl>

//@doc SECControlBar
//@mfunc virtual void  | SECControlBar | OnBarBeginDock | Virtual function that can be 
// overridden to handle special requirements before a bar is docked. The default 
// implementation just calls OnBarDock().

//@doc SECControlBar
//@mfunc virtual void  | SECControlBar | OnBarEndDock | Virtual function that can be 
// overridden to handle special requirements after a bar is docked. The default 
// implementation does nothing and returns.

//@doc SECControlBar
//@mfunc virtual void  | SECControlBar | OnBarBeginFloat | Virtual function that can be 
// overridden to handle special requirements before a bar is floated. The default 
// implementation just calls OnBarFloat().

//@doc SECControlBar
//@mfunc virtual void  | SECControlBar | OnBarEndFloat | Virtual function that can be 
// overridden to handle special requirements after a bar is floated. The default 
// implementation does nothing and returns.

//@doc SECControlBar
//@mfunc virtual void  | SECControlBar | OnBarBeginMDIFloat | Virtual function that can be 
// overridden to handle special requirements before a bar is floated in a MDI window. The default 
// implementation just calls OnBarMDIFloat().

//@doc SECControlBar
//@mfunc virtual void  | SECControlBar | OnBarEndMDIFloat | Virtual function that can be 
// overridden to handle special requirements before a bar is floated. The default 
// implementation does nothing and returns.

//@doc SECControlBar
//@mfunc virtual BOOL  | SECControlBar | OnGripperClose | Gripper button callback. 
// Return FALSE to abort closing.

//@doc SECControlBar
//@mfunc virtual BOOL  | SECControlBar | OnGripperExpand | Gripper button callback. 
// Return FALSE to abort expansion.

//@doc SECControlBar
//@mdata CRect  | SECControlBar | m_rcBorderSpace | The rect containging the bar border.

//@doc SECControlBar
//@mdata SECControlBarManager*  | SECControlBar | m_pManager | A pointer to the control bar
// manager object for this bar.

//@doc SECControlBar
//@mdata SECGripperInfo  | SECControlBar | m_GripperInfo | The class which manages the gripper.

//@doc SECControlBar
//@mdata CRect  | SECControlBar | m_rcGripperCloseButton | The rect for the gripper close
// button.

//@doc SECControlBar
//@mdata CRect  | SECControlBar | m_rcGripperExpandButton | The rect for the gripper expand
// button


// adinsert AutoDuck insertion point for SECControlBar 
//@doc SECControlBar 
//@class SECControlBar replaces CControlBar as the base class for all your control 
// bars.  
//@comm
// SECControlBar adds support for sizing while docked, automatic stretching 
// when resized and a default context menu with facilities for adding and removing 
// menu items. (NOTE: All control bars formally derived from CControlBar must be 
// rederived from SECControlBar.  No CControlBars are allowed when using Objective 
// Toolkit’s docking window enhancements, as they lack the member variables 
// required to perform the sizing calculations.)
//
// See the sample VIZ in the \SAMPLES\TOOLKIT\STANDARD\DOCKING\VIZ directory for a demonstration of 
// this class.
//@base public | CControlBar
//@xref <c SECToolBar> <c SECCustomToolBar>
class SECControlBar : public CControlBar
{
	DECLARE_SERIAL(SECControlBar)

	//@access Construction

public:

	//@cmember
	/* Constructs an SECControlBar object.*/
    SECControlBar();

	//@cmember
	/* Creates a control bar and attaches it to an SECControlBar object.*/
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, UINT nID,
		DWORD dwStyle, DWORD dwExStyle, const RECT& rect, CWnd* pParentWnd = NULL,
		CCreateContext* pContext = NULL);
	
	//@cmember
	/* Creates a control bar and attaches it to an SECControlBar object.*/
	virtual BOOL Create(CWnd* pParentWnd, LPCTSTR lpszWindowName, DWORD dwStyle,
		DWORD dwExStyle, UINT nID, CCreateContext* pContext = NULL);

public:

	//@cmember
	/* Retrieves attributes associated with control bar’s inside rectangle.*/
	virtual void GetInsideRect(CRect& rectInside) const;
	
	//@cmember
	/* Determines whether control bar has been re-parented by an MDI child window.*/
	BOOL IsMDIChild() const;

	// Force redraw for a child window (required for ActiveX controls)
	void SetForcedChildWndRedraw(HWND hWndChildRedraw) { m_hWndChildRedraw=hWndChildRedraw; }


	//@access Attributes
public:

	//@cmember
	/* TRUE if optimized redraw is in effect.*/
	static BOOL m_bOptimizedRedrawEnabled; 
	//@cmember
	/* Cross-dll accessor to get m_bOptimizedRedrawEnabled state.*/
	static BOOL GetOptimizeRedrawEnabled();					
	//@cmember
	/* Cross-dll accessor to set m_bOptimizedRedrawEnabled state.*/
	static void SetOptimizedRedrawEnabled(BOOL bOptimize);
	//@cmember
	/* Access function to get the position of a bar.*/
	BOOL GetBarSizePos(int& nRow,int& nCol);
	//@cmember
	/* Access function to get the position of a bar with the ID.*/
	BOOL GetBarSizePos(int& nRow,int& nCol,int& nDockbarID);
	//@cmember
	/* Access function to get the position of a bar with the ID, height, and percentage width.*/
	BOOL GetBarSizePos(int& nRow,int& nCol,int& nDockbarID,float& fPctWidth,int& nHeight);

	//@access Public data members

	//@cmember
	/* Dimensions when docked horizontally.*/
	CSize m_szDockHorz; 
	//@cmember
	/* Location when docked horizontally.*/
	CPoint m_ptDockHorz;
	//@cmember
	/* Dimensions when docked vertically.*/
	CSize m_szDockVert; 
	//@cmember
	/* Dimensions when floating.*/
	CSize m_szFloat;    
	//@cmember
	/* Remember docking state when “Allow Docking” unchecked.*/
	DWORD m_dwMRUDockingState; 
	//@cmember
	/* The percentage of the dock bar’s width this control bar occupies.*/
	float m_fPctWidth;
	//@cmember
	/* The percentage of the dock bar’s width this control bar occupies.*/
	float m_fDockedPctWidth;			
	//@cmember
	/* Extended style bits.*/
	DWORD m_dwExStyle; 

protected:
	//@access Protected data members

	//@cmember
	/* White space around bar used for dragging.*/
	CRect m_rcBorderSpace; 			
	//@cmember
	/* Control bar manager.*/
	SECControlBarManager* m_pManager;	
	//@cmember
	/* Gripper information.*/
	SECGripperInfo m_GripperInfo;
	//@cmember
	/* Gripper close button.*/
	CRect m_rcGripperCloseButton;		
	// Left button is depressed on gripper close
	BOOL  m_bClickingGripperClose;
	//@cmember
	/* Expand button*/
	CRect m_rcGripperExpandButton;		
	// left button is depressed on gripper expand
	BOOL  m_bClickingGripperExpand;	
	// Enable expansion button.
	BOOL  m_bGripperExpandEnabled;
	BOOL  m_bGripperExpandExpanding;
	BOOL  m_bGripperExpandHorz;

protected:
	// The following structure and array are used for layout. They are not documented as there
	// may be changes in this structure. Use these with caution.
	struct LayoutInfo {
		HWND m_hWnd;
		CRect m_rect;
	};

	CPtrArray* m_pArrLayoutInfo;

	HWND m_hWndChildRedraw;

	//@access Operations
public:

	//@cmember
	/* Enables control bar docking.*/
    void EnableDocking(DWORD dwDockStyle);
	//@cmember
	/* This member is called by EnableDocking() to instantiate a derivative of SECDockContext*/
    virtual SECDockContext * NewDockContext(void);
	DWORD GetExBarStyle() const
		{ return m_dwExStyle; };
	//@cmember
	/* Set the extended bar style.*/
	virtual void SetExBarStyle(DWORD dwExStyle,BOOL bAutoUpdate=FALSE);
	//@cmember
	/* Called to give control bars derived from SECControlBar a chance to modify */
	// the default context menu.
	virtual void ModifyBarStyleEx(DWORD dwRemove,DWORD dwAdd,BOOL bAutoUpdate=FALSE);

	//@cmember
	/* Static utility function to derive a unique controlbar id*/
	static UINT GetUniqueBarID(CFrameWnd* pMainWnd,UINT nBaseID=0x100);

	//@cmember
	/* Static utility function to verify all existing controlbars have unique ids.*/
	static BOOL VerifyUniqueBarIds(CFrameWnd* pFrameWnd);

	//@cmember
	/* Static utility function to verify a specific controlbar id is unique*/
	static BOOL VerifyUniqueSpecificBarID(CFrameWnd* pFrameWnd,UINT nBarID);
	
	// access Overrides
	// Determines whether a point is in the bounding rectangle of the specified tool.  If the point is in the rectangle, it retrieves information about the tool.
	virtual int OnToolHitTest(CPoint point,TOOLINFO* pTI) const;

	// ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(SECControlBar)
    //}}AFX_VIRTUAL

	//@access Overridable
public:
	// SECControlBar Destructor
    virtual ~SECControlBar();
	// Return the size of the bar when it is not dockable
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	//@cmember
	/* Return the size of a dockable bar*/
	virtual CSize CalcDynamicLayout(int nLength, DWORD dwMode);
	virtual void DoPaint(CDC* pDC);
	virtual void DrawBorders(CDC* pDC, CRect& rect);
	void EraseNonClient();
	// Get information about the bar.
	virtual void GetBarInfo(SECControlBarInfo* pInfo);
	// Set information about the bar.
	void SetBarInfo(SECControlBarInfo* pInfo, CFrameWnd* pFrameWnd);
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	virtual void CalcInsideRect(CRect& rect, BOOL bHorz, BOOL bVert = FALSE) const;
	inline SECControlBarManager* GetManager() const;
	inline void SetManager(SECControlBarManager*);

	//@cmember
	/* Virtual function that can be overridden to handle special requirements before a bar is docked.*/
	virtual void OnBarBeginDock(){ OnBarDock(); }
	// Virtual function that can be overridden to handle special requirements when a bar is docked.
	virtual void OnBarDock();
	//@cmember
	/* Virtual function that can be overridden to handle special requirements after a bar is docked.*/
	virtual void OnBarEndDock(){ }


	//@cmember
	/* Virtual function that can be overridden to handle special requirements before a bar is floated.*/
	virtual void OnBarBeginFloat(){ OnBarFloat(); }
	// Virtual function that can be overridden to handle special requirements when a bar is floated.
	virtual void OnBarFloat();
	//@cmember
	/* Virtual function that can be overridden to handle special requirements after a bar is floated.*/
	virtual void OnBarEndFloat(){ }

	//@cmember
	/* Virtual function that can be overridden to handle special requirements before a bar is floated as a MDI child window.*/
	virtual void OnBarBeginMDIFloat(){ OnBarMDIFloat(); }
	// Virtual function that can be overridden to handle special requirements when a bar is floated as a MDI child window.
	virtual void OnBarMDIFloat();
	//@cmember
	/* Virtual function that can be overridden to handle special requirements after a bar is floated as a MDI child window.*/
	virtual void OnBarEndMDIFloat(){ }

	virtual void GetBarInfoEx(SECControlBarInfo* pInfo);
	virtual void SetBarInfoEx(SECControlBarInfo* pInfo, CFrameWnd* pFrameWnd);

protected:

	//@access Implementation

	void InitLayoutInfo();
	void DeleteLayoutInfo();
	// Called to give control bars derived from SECControlBar a chance to modify the default context menu.
	virtual void OnExtendContextMenu(CMenu* pMenu);

	// Gripper button callbacks. Return FALSE to abort operation
	//@cmember
	/* Gripper button callback. Return FALSE to abort closing.*/
	virtual BOOL OnGripperClose()  { return TRUE; }
	//@cmember
	/* Gripper button callbacks. Return FALSE to abort expansion.*/
	virtual BOOL OnGripperExpand() { return TRUE; }


	//
	// gripper drawing functions
	//
	// override *both* of these if you want to draw your own custom gripper
	virtual void AdjustInsideRectForGripper(CRect& rect,BOOL bHorz);
	virtual void DrawGripper(CDC* pDC, CRect& rect);

	// utility functions to draw the gripper "close" button
	virtual void DrawGripperCloseButton(CDC* pDC, CRect& rect,BOOL bHorz);
	virtual void DrawGripperCloseButtonDepressed(CDC* pDC);
	virtual void DrawGripperCloseButtonRaised(CDC* pDC);

	// utility functions to draw the gripper "expand" button
	virtual void DrawGripperExpandButton(CDC* pDC, CRect& rect,BOOL bHorz);
	virtual void DrawGripperExpandButtonDepressed(CDC* pDC);
	virtual void DrawGripperExpandButtonRaised(CDC* pDC);
	virtual void SetGripperExpandButtonState(BOOL bHorz);

// Command Handlers
protected:

	afx_msg void OnHide();
	afx_msg void OnToggleAllowDocking();
	afx_msg void OnFloatAsMDIChild();

// Generated message map functions
public: // public functions so other helper classes can call these handlers directly
    //{{AFX_MSG(SECControlBar)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint pt );
	afx_msg void OnLButtonUp(UINT nFlags, CPoint pt );
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint pt);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// inline functions

inline SECControlBarManager* SECControlBar::GetManager() const
{
	return m_pManager;
}

inline void SECControlBar::SetManager(SECControlBarManager* pManager)
{
	m_pManager = pManager;
}

#if _MFC_VER >= 0x0600
/////////////////////////////////////////////////////////////////////////////
// SECReBar

//@doc SECReBar 
//@class this is a very thin class that enables SECControlBars to be contained in
// a Rebar common control
//@base public | CReBar
class SECReBar : public CReBar
{
	DECLARE_DYNAMIC(SECReBar)

// Construction
public:
	SECReBar(){}
	//@cmember
	/* Calculates the layout of contained SECControlBars*/
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);

};
#endif	//_MFC_VER >= 0x0600

//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif // __SBARCORE_H__

#else // WIN32

#define SECControlBar CControlBar

#endif // WIN32

