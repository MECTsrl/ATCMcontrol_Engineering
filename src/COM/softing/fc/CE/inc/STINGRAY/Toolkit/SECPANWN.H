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
//  Author:		Scot Wingo
//  Description:	Declaration of the panning overview window
//  Created:		Tue Jun 27 15:13:35 1995
//
//   CHANGELOG
//
//      MSW    9/30/95   Code Review
//
//
//
#ifndef __SECPANWN_H__
#define __SECPANWN_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
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

class SECPanView;
/////////////////////////////////////////////////////////////////////////////
// SECPanWnd window

/////////////////////////////
// AutoDuck tag block block for SECPanWnd 







// adinsert AutoDuck insertion point for SECPanWnd 
//@doc SECPanWnd 
//@class SECPanWnd is an overview window that shows the user a miniaturization of the entire view provided 
// by <c SECPanView>.
//@comm
// The user can use the overview window to pan the visible portion of the view and also to see where the 
// visible area lies in relation to other portions of the view.  The overview window is especially 
// helpful at large zoom levels when it is possible to lose track of the current location in a complex 
// view.
//
// The currently visible region of the view is outlined by a dotted rectangle.  The user can move the 
// dotted rectangle to change the currently visible area.  If the user updates the view or scrolls, 
// SECPanWnd is automatically updated to  reflect the changes.
//
// The CLOUD sample in the \SAMPLES\TOOLKIT\STANDARD\VIEWS\CLOUD directory demonstrates SECPanWnd.
//@xref<c SECPanView>
//@BASE public | CWnd
class SECPanWnd : public CWnd
{
    // We're friends with the pan view. 
    friend class SECPanView;
    
// Construction
public:
	//@Access Public Members
	//Constructors
	//@cmember
	/* Constructs an SECPanWnd overview window object.*/
    SECPanWnd(SECPanView *);

// Attributes
public:
	//@Access Attributes
	//@cmember
	/* Creates the overview window and specifies its initial position and size.*/
    virtual BOOL Create(CRect rectCreate, LPCTSTR lpszTitle = NULL);

	//@cmember
	/* Implements the overview window’s style settings at creation.*/
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    
// Operations
public:
	//@Access Operations
	//@cmember
	/* Sets the rectangle representing the overview window.*/
    void SetPanRect(CRect rectNewPan);

	//@cmember
	/* Draws the rectangle representing the overview window.*/
    void DrawPanRect(CDC * pDC);

	void InvalidateOverviewWnd(CRect * rectUpdate  = NULL );

	//@cmember
	/* Updates the overview window when the user scrolls or updates the panning view.*/
    void OnUpdate(CView * pSender, LPARAM lHint, CObject * pHint);
    
    
// Overridables
protected:
    
// Implementation
public:
    virtual ~SECPanWnd();
#ifdef _DEBUG
	void Dump(CDumpContext  &dc);
	void AssertValid();
#endif //_DEBUG
    SECPanView * m_pPanView;        //Back pointer to view
    CRect        m_rectLogPan;      //The current viewport
    CSize        m_szLogDrag;       //The size of the rect being dragged
    BOOL         m_bCaptured;       //Is the mosue captured
    HCURSOR      m_hOverviewCursor; //Cursor for dragging the overview rect
	BOOL		 m_bClipVerified;	//Has the success of ClipOverviewCursor() been verified
 
    //Impementation specific helper functions for overview wnd.
    void ClipOverviewCursor();
    void OverviewDPtoLP(CPoint *);
    
    // Generated message map functions
protected:
    //{{AFX_MSG(SECPanWnd)
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnClose();
	//}}AFX_MSG
    afx_msg void OnPaint();
    
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


#endif // __SECPANWN_H__




