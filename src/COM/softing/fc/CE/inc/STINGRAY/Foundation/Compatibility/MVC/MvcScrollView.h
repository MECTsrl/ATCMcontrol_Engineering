/////////////////////////////////////////////////////////////////////////////
// MvcScrollView.h : Declaration of MvcScrollView_T
//
// Stingray Software Extension Classes
// Copyright (C) 1998 Stingray Software Inc,
// All rights reserved.
//
// This source code is only intended as a supplement to
// the Objective Toolkit Class Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding
// the Objective Toolkit product.
//
// Author:       Jeff Boenig and Brad King
// Description:  Declaration and implementation of MvcScrollView_T class.
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Compatibility/MVC/MvcScrollView.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

//@doc MvcScrollView_T

#ifndef __COMPAT_MVCSCROLLVIEW_H__
#define __COMPAT_MVCSCROLLVIEW_H__

// Header Includes
#ifndef __COMPAT_MVCWRAPPER_H__
#include "Foundation\Compatibility\MVC\MvcWrapper.h"
#endif

// MvcScrollView.h : header file
//
//
// MVC Extension DLL
// Initialize declaration context
//
#ifdef _MVCDLL
	#undef AFXAPP_DATA
	#define AFXAPP_DATA AFXAPI_DATA
	#undef AFX_DATA
	#define AFX_DATA    MVC_DATAEXT
#endif //_MVCDLL

/////////////////////////////////////////////////////////////////////////////
// MvcScrollView_T

//@doc MvcScrollView_T
//@class This is a wrapper class that derives from your viewport and
// CScrollView. The resulting class can be used like a CScrollView, except all
// of the drawing will automaticallt be delegated to the viewport class.
//
// See the MvcForm sample for a demonstration of this class.
//
//@tcarg class | base_t | The class of the viewport to derive from
//@base public | CScrollView
template<class base_t>
class MvcScrollView_T : public CScrollView, public MvcWrapper_T<base_t>
{
// Construction/Destruction
public:
	/* Constructor */
	MvcScrollView_T();

	virtual BOOL Create(LPCTSTR lpszClassName,
	                    LPCTSTR lpszWindowName,
	                    DWORD dwStyle,
	                    const RECT& rect,
	                    CWnd* pParentWnd,
	                    UINT nID,
	                    CCreateContext* pContext = NULL);

// Attributes
protected:
	CSize m_szLineScroll;
	BOOL m_bUpdateScrollBars;

// Operations
public:
	//@cmember
	/* Casts this object to the base viewport class*/
	base_t* GetViewport();

	//@cmember
	/* Sets up default viewport initialization */
	virtual void OnInitialUpdate();
	//@cmember
	/* Forwards commands to the embedded viewport*/
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//@cmember
	/* Forwards messages to the embedded viewport*/
	virtual BOOL OnWndMsg( UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult );
	//@cmember
	/* Prepares the device context for drawing*/
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	//@cmember
	/* Called by MFC framework to scroll the view. */
	virtual BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll = TRUE);

protected:
	//@cmember
	/* Draws the viewport*/
	virtual void OnDraw(CDC* pDC);

public:
	// Overrides which may change scroll bar size and position
	virtual CPoint SetOrigin(int x, int y);
	virtual CPoint SetLogOrigin(int x, int y);
	virtual CSize SetExtents(int cx, int cy);
	virtual CSize SetLogExtents(int cx, int cy);
	virtual CSize SetSize(int cx, int cy);
	virtual CSize SetLogSize(int cx, int cy);
	virtual CSize SetLogScaling(float fScaleWidth,
	                            float fScaleHeight);
	virtual void SetVirtualSize(int cx,int cy);
	virtual void SetVirtualOrigin(int x,int y);

// Implementation
protected:
	//@cmember
	/* Scrolls the embedded viewport in response to WM_xSCROLL or WM_SIZE messages*/
	virtual void DoScrollViewport(CPoint ptScrollPos);
	virtual void UpdateScrollBarPos();
	virtual void UpdateScrollBarSize();
};

template<class base_t>
MvcScrollView_T<base_t>::MvcScrollView_T()
{
	m_szLineScroll = CSize(10,10);
	m_bUpdateScrollBars = FALSE;
}

template<class base_t>
BOOL MvcScrollView_T<base_t>::Create(LPCTSTR lpszClassName,
                                     LPCTSTR lpszWindowName,
                                     DWORD dwStyle,
                                     const RECT& rect,
                                     CWnd* pParentWnd,
                                     UINT nID,
                                     CCreateContext* pContext)
{
	SetContainer(NULL);

	BOOL bSuccess = CScrollView::Create(lpszClassName,
	                                    lpszWindowName,
	                                    dwStyle,
	                                    rect,
	                                    pParentWnd,
	                                    nID,
	                                    pContext);


	if (bSuccess)
	{
		bSuccess = base_t::Create(this, NULL);

		if (bSuccess && m_pCtlr != NULL)
		{
			// Make sure model is set on controller, because model
			// may have been assigned to this viewport before
			// the controller was created.
			m_pCtlr->SetModel(m_pModel);
		}
	}

	return bSuccess;
}

//@doc MvcScrollView_T
//@mfunc Casts this object to the base viewport class
//@rdesc Returns "this" instance cast to the base viewport class
template<class base_t>
base_t* MvcScrollView_T<base_t>::GetViewport()
{
	return (base_t*) this;
}

//@doc MvcScrollView_T
//@mfunc Sets up default viewport initialization
//@rdesc void
template<class base_t>
void MvcScrollView_T<base_t>::OnInitialUpdate()
{
	// construct the viewport in your derived class constructor
	// and delete it in the destructor
	CScrollView::OnInitialUpdate();

	// viewport initialization
	CRect r;
	GetClientRect(&r);
	MvcWrapper_T<base_t>::SetSize(r.Size());
	MvcWrapper_T<base_t>::OnInitialUpdate();

	UpdateScrollBarPos();
	UpdateScrollBarSize();
}

//@doc MvcScrollView_T
//@mfunc Forwards commands to the embedded viewport
//@rdesc TRUE if message was handled, otherwise FALSE.
//@parm UINT | nID | Contains the command ID.
//@parm int | nCode | Identifies the command notification code.
//@parm void* | pExtra | Used according to the value of nCode.
//@parm AFX_CMDHANDLERINFO* | pHandlerInfo | If not NULL, OnCmdMsg fills in the pTarget and pmf members of the pHandlerInfo structure instead of dispatching the command. Typically, this parameter should be NULL.
template<class base_t>
BOOL MvcScrollView_T<base_t>::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	BOOL bHandled = base_t::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);

	if(!bHandled)
		bHandled = CScrollView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);

	return bHandled;
}

//@doc MvcScrollView_T
//@mfunc Forwards messages to the embedded viewport
//@rdesc TRUE if message was handled, otherwise FALSE.
//@parm UINT | message | The windows message being handled
//@parm WPARAM | wParam | Specifies additional message-dependent information.
//@parm LPARAM | lParam | Specifies additional message-dependent information.
//@parm LRESULT* | pResult | The return value
//@comm
// This handler also watches for scrolling and sizing messages to call DoScrollViewport.
//@xref <mf MvcScrollView_T::DoScrollViewport>
template<class base_t>
BOOL MvcScrollView_T<base_t>::OnWndMsg( UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult )
{
	BOOL bHandled = base_t::OnWndMsg(message, wParam, lParam, pResult);
	if (!bHandled)
		bHandled = CScrollView::OnWndMsg(message, wParam, lParam, pResult);

	// handle messages here that could cause scrolling
	switch(message)
	{
	case WM_SIZE:
		{
			// resize the logical rect of the viewport
			int nWidth = LOWORD(lParam);  // width of client area 
			int nHeight = HIWORD(lParam); // height of client area 
			MvcWrapper_T<base_t>::SetSize(nWidth, nHeight);

			// viewport scrolling
			int nMapMode;
			SIZE sizeTotal;
			SIZE sizePage;
			SIZE sizeLine;
			GetDeviceScrollSizes( nMapMode, sizeTotal, sizePage, sizeLine );
			// you need to call SetScrollSizes in your OnInitialUpdate override
			if( nMapMode > 0 )
				DoScrollViewport(GetScrollPosition());
			break;
		}
	case WM_HSCROLL:
	case WM_VSCROLL:
		{
			int nScrollCode = (int) LOWORD(wParam);  
			if ( nScrollCode != SB_ENDSCROLL)
			{
				CPoint ptScroll;
				SCROLLINFO siH;
				SCROLLINFO siV;
				GetScrollInfo(SB_HORZ, &siH);
				GetScrollInfo(SB_VERT, &siV);
				if(nScrollCode == SB_THUMBTRACK)
				{
					ptScroll.x = siH.nTrackPos;
					ptScroll.y = siV.nTrackPos;
				}
				else
				{
					ptScroll.x = siH.nPos;
					ptScroll.y = siV.nPos;
				}

				DoScrollViewport(ptScroll);
				bHandled = TRUE;
			}
			break;
		}
	case WM_MOUSEWHEEL:
		{
			DoScrollViewport(GetScrollPosition());
		}
	}

	return bHandled;
}

//@doc MvcScrollView_T
//@mfunc Prepares the device context for drawing.
//@rdesc void
//@parm CDC* | pDC | Points to the device context to be used for rendering an image.
//@parm CPrintInfo* | pInfo | Points to a CPrintInfo structure that describes the 
// current print job if OnPrepareDC is being called for printing or print preview; 
// the m_nCurPage member specifies the page about to be printed. This parameter is 
// NULL if OnPrepareDC is being called for screen display.
template<class base_t>
void MvcScrollView_T<base_t>::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	ASSERT(CScrollView::m_nMapMode == MM_TEXT);
	MvcWrapper_T<base_t>::OnPrepareDC(pDC);
	CView::OnPrepareDC(pDC, pInfo);     // For default Printing behavior
}

//@doc MvcScrollView_T
//@mfunc Called by MFC framework to scroll the view.
//@rdesc TRUE if view scrolled; otherwise FALSE.
//@parm CSize | sizeScroll | Horizontal and vertical offset to scroll.
//@parmopt BOOL | bDoScroll | TRUE | Indicates if scroll should actually be performed.
//@comm Overrides OnScrollBy in CScrollView in order to prevent the window from scrolling.
// This function is identical to OnScrollBy in CScrollView, except that the call to
// CWnd::ScrollWindow has been removed.
template<class base_t>
BOOL MvcScrollView_T<base_t>::OnScrollBy(CSize sizeScroll, BOOL bDoScroll)
{
	int xOrig, x;
	int yOrig, y;

	// don't scroll if there is no valid scroll range (ie. no scroll bar)
	CScrollBar* pBar;
	DWORD dwStyle = GetStyle();
	pBar = GetScrollBarCtrl(SB_VERT);
	if ((pBar != NULL && !pBar->IsWindowEnabled()) ||
		(pBar == NULL && !(dwStyle & WS_VSCROLL)))
	{
		// vertical scroll bar not enabled
		sizeScroll.cy = 0;
	}
	pBar = GetScrollBarCtrl(SB_HORZ);
	if ((pBar != NULL && !pBar->IsWindowEnabled()) ||
		(pBar == NULL && !(dwStyle & WS_HSCROLL)))
	{
		// horizontal scroll bar not enabled
		sizeScroll.cx = 0;
	}

	// adjust current x position
	xOrig = x = GetScrollPos(SB_HORZ);
	int xMax = GetScrollLimit(SB_HORZ);
	x += sizeScroll.cx;
	if (x < 0)
		x = 0;
	else if (x > xMax)
		x = xMax;

	// adjust current y position
	yOrig = y = GetScrollPos(SB_VERT);
	int yMax = GetScrollLimit(SB_VERT);
	y += sizeScroll.cy;
	if (y < 0)
		y = 0;
	else if (y > yMax)
		y = yMax;

	// did anything change?
	if (x == xOrig && y == yOrig)
		return FALSE;

	if (bDoScroll)
	{
		// do scroll and update scroll positions
		if (x != xOrig)
			SetScrollPos(SB_HORZ, x);
		if (y != yOrig)
			SetScrollPos(SB_VERT, y);
	}
	return TRUE;
}

//@doc MvcScrollView_T
//@mfunc Delegates to the embedded viewport
//@rdesc void
//@parm CDC* | pDC | The device context to draw on
template<class base_t>
void MvcScrollView_T<base_t>::OnDraw(CDC* pDC)
{
	if (m_bUpdateScrollBars)
	{
		UpdateScrollBarPos();
		UpdateScrollBarSize();
		m_bUpdateScrollBars = FALSE;
	}

	base_t::Draw(pDC);
}

//@doc MvcScrollView_T
//@mfunc Scrolls the embedded viewport in response to WM_xSCROLL or WM_SIZE messages
//@rdesc void
//@parm CPoint | ptScrollPos | The current scroll position, in logical units
//@comm
// To keep the scroll view and embedded viewport in sync, the logical origin of the
// viewport is scrolled to match the CScrollView.
template<class base_t>
void MvcScrollView_T<base_t>::DoScrollViewport(CPoint ptScrollPos)
{
	CSize szLog(ptScrollPos.x, ptScrollPos.y);
	base_t::DPtoLP(&szLog);
	ptScrollPos.x = szLog.cx;
	ptScrollPos.y = szLog.cy;

	// same as Pan, except using absolute coordinates, not offsets
	base_t::SetLogOrigin(ptScrollPos);
	base_t::InvalidateVisual(this, FALSE, TRUE);
}

template<class base_t>
void MvcScrollView_T<base_t>::UpdateScrollBarPos()
{
	CPoint ptLogOrigin=base_t::GetLogOrigin();

	// Convert logical origin of viewport into device coordinates
	// that can be used to adjust the scrollbars.
	CSize szDev(ptLogOrigin.x, ptLogOrigin.y);
	base_t::LPtoDP(&szDev);
	CPoint ptScrollPos(szDev.cx, szDev.cy);

	SetScrollPos(SB_HORZ, ptScrollPos.x, TRUE);
	SetScrollPos(SB_VERT, ptScrollPos.y, TRUE);
}

template<class base_t>
void MvcScrollView_T<base_t>::UpdateScrollBarSize()
{
	//
	// The scroll view mapping mode is always set to MM_TEXT, so device
	// coordinates in the viewport are the same as logical coordinates used
	// by the scrollbars. Logical coordinates in the viewport must be
	// converted to device units before being used to set the scrollbar
	// positions or sizes.
	//

	CSize szTotal = base_t::GetVirtualSize();
	base_t::LPtoDP(&szTotal);

	CSize szPage(GetBounds().Size());

	CRect rcInnerMargins;
	base_t::GetMargins(rcInnerMargins);
	CSize szInnerMargins(rcInnerMargins.left + rcInnerMargins.right,
	                     rcInnerMargins.top + rcInnerMargins.bottom);

	szTotal = szTotal + szInnerMargins;

	SetScrollSizes(MM_TEXT, szTotal, szPage, m_szLineScroll);
}

// If visual part changes position, reset scrollbar rects
template<class base_t>
CPoint MvcScrollView_T<base_t>::SetOrigin(int x, int y)
{
	CPoint pt=MvcWrapper_T<base_t>::SetOrigin(x,y);
	UpdateScrollBarPos();
	return pt;
}

template<class base_t>
CPoint MvcScrollView_T<base_t>::SetLogOrigin(int x, int y)
{
	CPoint ptPrev = MvcWrapper_T<base_t>::SetLogOrigin(x, y);
	UpdateScrollBarPos();
	return ptPrev;
}

template<class base_t>
void MvcScrollView_T<base_t>::SetVirtualSize(int cx,int cy)
{
	MvcWrapper_T<base_t>::SetVirtualSize(cx,cy);
	UpdateScrollBarPos();
	UpdateScrollBarSize();
}

template<class base_t>
void MvcScrollView_T<base_t>::SetVirtualOrigin(int x,int y)
{
	MvcWrapper_T<base_t>::SetVirtualOrigin(x,y);
	UpdateScrollBarPos();
}

template<class base_t>
CSize MvcScrollView_T<base_t>::SetExtents(int cx, int cy)
{
	CSize size=MvcWrapper_T<base_t>::SetExtents(cx,cy);
	m_bUpdateScrollBars = TRUE;
	return size;
}

template<class base_t>
CSize MvcScrollView_T<base_t>::SetSize(int cx, int cy)
{
	CSize size=MvcWrapper_T<base_t>::SetSize(cx,cy);
	UpdateScrollBarPos();
	UpdateScrollBarSize();
	return size;
}

template<class base_t>
CSize MvcScrollView_T<base_t>::SetLogSize(int cx, int cy)
{
	CSize size=MvcWrapper_T<base_t>::SetLogSize(cx,cy);
	UpdateScrollBarPos();
	UpdateScrollBarSize();
	return size;
}

// Logical extents have changed. Adjust scrollbars to compensate
template<class base_t>
CSize MvcScrollView_T<base_t>::SetLogExtents(int cx, int cy)
{
	CSize sizeExtents=MvcWrapper_T<base_t>::SetLogExtents(cx,cy);
	m_bUpdateScrollBars = TRUE;
	return sizeExtents;
}

// Logical extents have changed. Adjust scrollbars to compensate
template<class base_t>
CSize MvcScrollView_T<base_t>::SetLogScaling(float fScaleWidth,
                                             float fScaleHeight)
{
	CSize sizeExtents = MvcWrapper_T<base_t>::SetLogScaling(fScaleWidth, fScaleHeight);
	UpdateScrollBarPos();
	UpdateScrollBarSize();
	return sizeExtents;
}


//
// MVC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR


#endif // __COMPAT_MVCSCROLLVIEW_H__
