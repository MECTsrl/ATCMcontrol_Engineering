/////////////////////////////////////////////////////////////////////////////
// MvcScrollWrapper.h : Declaration of MvcScrollWrapper_T
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
// Author:       Mark Isham
// Description:  Declaration of MvcScrollWrapper_T interface.
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/MVC/MvcScrollWrapper.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

//@doc MvcScrollWrapper_T
//
// MvcScrollWrapper_T class

#pragma once

#ifndef __MVCSCROLLWRAPPER_H__
#define __MVCSCROLLWRAPPER_H__

#if defined(_SFL_ATL_SUPPORT)

#include <Foundation\Controls\AtlControls.h>

#include "Foundation\Debug.h"
#include "Foundation\MVC\MvcWrapper.h"
#include "Foundation\Events\ScrollListener.h"
#include "Foundation\StructuredGraphics\VisualWindow.h"

namespace stingray {
namespace foundation {

/////////////////////////////////////////////////////////////////////////////
//
// MvcScrollWrapper_T
//
//@doc MvcScrollWrapper_T
//@mfunc CScrollBar* | MvcScrollWrapper_T |  GetHScrollBar | Get a pointer to the horizontal scrollbar
//@xref <c MvcScrollWrapper_T>

//@doc MvcScrollWrapper_T
//@mfunc CScrollBar* | MvcScrollWrapper_T |  GetVScrollBar | Get a pointer to the vertical scrollbar
//@xref <c MvcScrollWrapper_T>

//@doc MvcScrollWrapper_T
//@mfunc CScrollBar* | MvcScrollWrapper_T |  GetSBScrollBar | Get a pointer to the scrollbar sizebox
//@xref <c MvcScrollWrapper_T>

//@doc MvcScrollWrapper_T
//@mfunc void | MvcScrollWrapper_T | SetLineUnitScroll | Set the unit change per scrollbar line scroll
//@parm UINT | nLinePixelScroll | number of units change per line scroll
//@xref <c MvcScrollWrapper_T>

//@doc MvcScrollWrapper_T
//@mfunc UINT | MvcScrollWrapper_T | GetLineUnitScroll | Get the unit change per scrollbar line scroll
//@xref <c MvcScrollWrapper_T>

//@doc MvcScrollWrapper_T
//@mfunc void | MvcScrollWrapper_T | SetOptimizedRedraw | Set scroll viewport such that it will not erase on invalidate
//@parm BOOL | bOptimize | Optimize flag
//@xref <c MvcScrollWrapper_T>

//@doc MvcScrollWrapper_T
//@mfunc BOOL | MvcScrollWrapper_T | GetOptimizedRedraw | Get the scrollbar viewport optimize state.
//@xref <c MvcScrollWrapper_T>

//
//@class MvcScrollWrapper_T |
// The MvcScrollWrapper_T class "decorates" a visual part
// with horizontal and/or vertical scrollbars.
//
//@tcarg class | base_t | The class of the visual part to derive from. Must be derived from MvcVisualPart
//@base public | MvcWrapper_T
//
template<class base_t>
class MvcScrollWrapper_T : public MvcWrapper_T<base_t>,
                           public CScrollAdapter
{
	//---------------------------------------------------------------------//
	// 						   Public Interface							   //
	//---------------------------------------------------------------------//

// Styles
public:

	// Constructor(s) & destructor
public:
	MvcScrollWrapper_T();

	// Attributes

	//@cmember
	/* Set scroll viewport such that it will not erase on invalidate*/
	void SetOptimizedRedraw(BOOL bOptimize=TRUE) { m_bOptimizedRedraw=bOptimize; }

	//@cmember
	/* Get the scrollbar viewport optimize state.*/
	BOOL GetOptimizedRedraw() { return m_bOptimizedRedraw; }

	// Operations

	//@cmember
	/* Toggle visibility of scrollbars*/
	BOOL SetScrollBarVisibility(BOOL bHorz=TRUE,BOOL bVert=TRUE);

	//@cmember
	/* Get a pointer to the horizontal scrollbar*/
	ATLControls::CScrollBar* GetHScrollBar() { return &m_wndHorzSB; }
	//@cmember
	/* Get a pointer to the vertical scrollbar*/
	ATLControls::CScrollBar* GetVScrollBar() { return &m_wndVertSB; }
	//@cmember
	/* Get a pointer to the scrollbar sizebox*/
	ATLControls::CScrollBar* GetSBScrollBar() { return &m_wndSizeBox; }

	//@cmember
	/* Set the unit change per scrollbar line scroll*/
	void SetLineUnitScroll(UINT nLinePixelScroll) { m_nLinePixelScroll=nLinePixelScroll; }
	//@cmember
	/* Get the unit change per scrollbar line scroll*/
	UINT GetLineUnitScroll() { return m_nLinePixelScroll; }


	// Overrides
	virtual void OnInitialUpdate();

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

	virtual bool RouteEvent(IEvent* pIEvent);
	virtual void Draw(CDC* pDC);

	BEGIN_GUID_MAP(ThisClass)
		GUID_CHAIN_ENTRY( MvcWrapper_T<base_t>)
		GUID_ENTRY(IScrollListener)
	END_GUID_MAP

	virtual ULONG STDMETHODCALLTYPE AddRef()
	{
		return 1;
	}

	virtual ULONG STDMETHODCALLTYPE Release()
	{
		return 1;
	}

	//---------------------------------------------------------------------//
	// 						Protected Implementation					   //
	//---------------------------------------------------------------------//

protected:


	// Implementation Overridabes
	//@cmember
	/* Allocate and create the scrollbar windows*/
	virtual BOOL CreateScrollBars();
	//@cmember
	/* (Re)Set the horizontal and vertical scrollbar ranges*/
	virtual void SetScrollBarRanges();
	//@cmember
	/* (Re)Set the horizontal and vertical scrollbar page sizes*/
	virtual void SetScrollBarPageSizes();
	//@cmember
	/* (Re)Set the horizontal and vertical scrollbar page sizes*/
	virtual void SetScrollBarPageSizes(CSize sizeLog);
	//@cmember
	/* Hide scrollbars with page size >= range size*/
	virtual BOOL HideScrollBarsWithFullPageSize();
	//@cmember
	/* Reset the position of the scrollbars*/
	virtual void ResetScrollBarRects(CPoint ptParentLogOrigin);
	//@cmember
	/* Reset the position of the scrollbars*/
	void ResetScrollBarRects() { ResetScrollBarRects(CPoint(0,0)); }

	void UpdateScrollBarPos();

	// Undocumented implementation
	virtual void InvalidateScrollInterior(BOOL bRepairNow = TRUE);
	virtual UINT GetNextScrollBarID() 
		{ return MvcScrollWrapper_T<base_t>::m_nAutoID++; }
	virtual bool OnHScroll(int nScrollCode, short nScrollPos, HWND hScrollBar);
	virtual bool OnVScroll(int nScrollCode, short nScrollPos, HWND hScrollBar);
/*#if _MFC_VER > 0x0420
	virtual BOOL OnMouseWheel(int fwKeys,short zDelta,short xPos,short yPos);
#endif*/

	BOOL GetSBVisible(BOOL bHorz);
	virtual void AutoScrollIfBeyondLogBounds();

	// Implementation Variables
	ATLControls::CScrollBar m_wndHorzSB;
	ATLControls::CScrollBar m_wndVertSB;
	ATLControls::CScrollBar m_wndSizeBox;
	UINT m_nLinePixelScroll;
	int m_cyHScroll,m_cxVScroll;
	BOOL m_bDidInitUpdate;
	BOOL m_bOptimizedRedraw;

private:
	// Automatic scrollbar id assignment
	static int m_nAutoID;

	// These should only be used by CreateScrollBars to provide a cached 
	// state to the scrollbar creation. Use GetSBVisible for 
	// subsequent state check.
	BOOL m_bHorzVisible,m_bVertVisible;
};

//
// Inline instantiation
//

template<class base_t>
int MvcScrollWrapper_T<base_t>::m_nAutoID=0xa01;

template<class base_t>
MvcScrollWrapper_T<base_t>::MvcScrollWrapper_T()
{
	m_bHorzVisible=m_bVertVisible=TRUE;
	m_nLinePixelScroll=10;
	m_bDidInitUpdate=FALSE;
	m_bOptimizedRedraw=FALSE;

	// Load scrollbar width/height from system metrics. Override
	// these values in a derived class for custom sizing
	m_cyHScroll=GetSystemMetrics(SM_CYHSCROLL);
	m_cxVScroll=GetSystemMetrics(SM_CXVSCROLL);
}

//
// Public Interface
//

//@mfunc Toggle visibility of scrollbars. Works even if scrollbars not yet created.
//@parm BOOL | bHorz | Show horizontal scrollbar, hide if FALSE
//@parm BOOL | bVert | Show vertical scrollbar, hide if FALSE
//@rdesc Returns TRUE if visibiliy of at least 1 bar has changed
template<class base_t>
BOOL MvcScrollWrapper_T<base_t>::SetScrollBarVisibility(BOOL bHorz,BOOL bVert) {
	BOOL bHorzWasVis=m_bHorzVisible;
	BOOL bVertWasVis=m_bVertVisible;
	m_bHorzVisible=bHorz;
	m_bVertVisible=bVert;

	// Toggle visibility as appropriate
	if(m_wndHorzSB.IsWindow()) {
		if(bHorz)
			 m_wndHorzSB.ShowWindow(SW_SHOWNOACTIVATE);
		else m_wndHorzSB.ShowWindow(SW_HIDE);
		}
	if(m_wndVertSB.IsWindow()) {
		if(bVert)
			 m_wndVertSB.ShowWindow(SW_SHOWNOACTIVATE);
		else m_wndVertSB.ShowWindow(SW_HIDE);
		}
	if(m_wndSizeBox.IsWindow()) {
		if(bHorz && bVert)
			 m_wndSizeBox.ShowWindow(SW_SHOWNOACTIVATE);
		else m_wndSizeBox.ShowWindow(SW_HIDE);
		}

	// Update the margins to reflect latest visibility state
	CRect rcMargins(0,0,0,0);
	if(GetSBVisible(TRUE))  rcMargins.bottom=m_cyHScroll;
	if(GetSBVisible(FALSE)) rcMargins.right=m_cxVScroll;
	MvcScrollWrapper_T<base_t>::SetMargins(rcMargins);

	if((bHorzWasVis != m_bHorzVisible) || (bVertWasVis != m_bVertVisible))
		 return TRUE;
	else return FALSE;
}

//
// Public Overrides
//

template<class base_t>
void MvcScrollWrapper_T<base_t>::OnInitialUpdate() {
	if(m_bDidInitUpdate) return;

	MvcWrapper_T<base_t>::OnInitialUpdate();

	// Allocate and create the scrollbar objects
	CreateScrollBars();

	// Set the initial range of the scrollbars
	SetScrollBarRanges();

	// Hide scrollbars with page size >= range size
	HideScrollBarsWithFullPageSize();

	// Set the initial page sizes of the scrollbars
	SetScrollBarPageSizes();

	// Position the scrollbar as appropriate.
	ResetScrollBarRects();

	// Ensure the srollbar position reflects the current placement
	// of the viewport. This is necessary to support an upward
	// positive Y-Axis. 
	UpdateScrollBarPos();

	m_bDidInitUpdate=TRUE;
}

template<class base_t>
void MvcScrollWrapper_T<base_t>::UpdateScrollBarPos()
{
	CPoint ptLogOrigin(GetLogOrigin());
	if (m_wndHorzSB.IsWindow())
	{
		m_wndHorzSB.SetScrollPos(ptLogOrigin.x);
	}

	if (m_wndVertSB.IsWindow())
	{
		// The scrollbar position depends upon the direction of 
		// the Y-Axis and the location of the viewport's logical origin 
		if(YAXIS_UP == base_t::GetYAxisDirection())
		{
			// For an upward positive Y-Axis, the scrollbar's position is
			// equal to the distance between the viewport's logical origin 
			// and the bottom of the virtual area.
			
			int VirtOriginY = GetVirtualOrigin().y;
			int VirtSizeY	= GetVirtualSize().cy;
			
			// The size of the virtual area should be positive.
			ASSERT(0 <= VirtSizeY);
			// The logical origin of the viewport should be within the virtual area.
			ASSERT(VirtOriginY <= ptLogOrigin.y && ptLogOrigin.y <= (VirtOriginY + VirtSizeY));

			int scrollPos = VirtOriginY + abs(VirtOriginY + VirtSizeY - ptLogOrigin.y);
			m_wndVertSB.SetScrollPos(scrollPos);
		}
		else
		{
			// For downward positive Y-Axis, the scrollbar position is
			// equivalent to the logical origin of the viewport.
			m_wndVertSB.SetScrollPos(ptLogOrigin.y);
		}
	}
}

// If visual part changes position, reset scrollbar rects
template<class base_t>
CPoint MvcScrollWrapper_T<base_t>::SetOrigin(int x, int y) {

	CPoint pt=MvcWrapper_T<base_t>::SetOrigin(x,y);
	UpdateScrollBarPos();
	return pt;
}

template<class base_t>
CPoint MvcScrollWrapper_T<base_t>::SetLogOrigin(int x, int y)
{
	CPoint ptPrev = MvcWrapper_T<base_t>::SetLogOrigin(x, y);
	// And position the scrollbars as appropriate.
	ResetScrollBarRects();

	// Do not attempt to manipulate the scrollbars until the 
	// initial update has occurred
	if(m_wndHorzSB.IsWindow() && m_wndVertSB.IsWindow() && m_wndSizeBox.IsWindow()) {

		// Adjust scrollbars if beyond logical extents
		AutoScrollIfBeyondLogBounds();

		// Set scrollbar position to match logical origin.
		UpdateScrollBarPos();
	}
	return ptPrev;
}

template<class base_t>
void MvcScrollWrapper_T<base_t>::SetVirtualSize(int cx,int cy)
{
	MvcWrapper_T<base_t>::SetVirtualSize(cx,cy);

	// Do not attempt to manipulate the scrollbars until the 
	// initial update has occurred
	if(m_wndHorzSB.IsWindow() && m_wndVertSB.IsWindow() && m_wndSizeBox.IsWindow()) {

		// Set the initial range of the scrollbars
		SetScrollBarRanges();

		// Adjust scrollbars if beyond logical extents
		AutoScrollIfBeyondLogBounds();

		// Reset the page sizes of the scrollbars
		SetScrollBarPageSizes();

		// Set scrollbar position to match logical origin.
		UpdateScrollBarPos();

		// Hide scrollbars with page size >= range size
		if(HideScrollBarsWithFullPageSize())
			// If change occurred, reset scrollbar positions again
			ResetScrollBarRects();

	}
}

template<class base_t>
void MvcScrollWrapper_T<base_t>::SetVirtualOrigin(int x,int y)
{
	MvcWrapper_T<base_t>::SetVirtualOrigin(x,y);

	// Do not attempt to manipulate the scrollbars until the 
	// initial update has occurred
	if(m_wndHorzSB.IsWindow() && m_wndVertSB.IsWindow() && m_wndSizeBox.IsWindow()) {

		// Set the initial range of the scrollbars
		SetScrollBarRanges();

		// Adjust scrollbars if beyond logical extents
		AutoScrollIfBeyondLogBounds();

		// Reset the page sizes of the scrollbars
		SetScrollBarPageSizes();

		// Set scrollbar position to match logical origin.
		UpdateScrollBarPos();

		// Hide scrollbars with page size >= range size
		if(HideScrollBarsWithFullPageSize())
			// If change occurred, reset scrollbar positions again
			ResetScrollBarRects();

	}
}

// Override the SetMargins call to account for scrollbars
template<class base_t>
void MvcScrollWrapper_T<base_t>::Draw(CDC* pDC) {
	MvcWrapper_T<base_t>::Draw(pDC);

	// Support for nested scrollbars. Incomplete.
#if 0
	// The window origin defines the offset for the DC. Subtract
	// any local logical coordinates from this offset to calculate
	// the true offset needed for this scrollbar shift
	CPoint ptWindowOrg=pDC->GetWindowOrg();
	CPoint ptLogOrg=GetLogOrigin();
	CPoint ptOffset=ptWindowOrg-ptLogOrg;
	ResetScrollBarRects(ptOffset);
#endif
}

// A size change has occurred in the physical device rect. For scrollbars
// we want to readjust the logical extents to accomodate the new physical
// rect without changing the mapping mode (i.e. do not perform any
// scaling to accomodate the new physical size, just adjust the scrollbar
// positions as approp).
template<class base_t>
CSize MvcScrollWrapper_T<base_t>::SetExtents(int cx, int cy) {

	// Set extents first
	CSize size=MvcWrapper_T<base_t>::SetExtents(cx,cy);

	// And position the scrollbars as appropriate.
	ResetScrollBarRects();

	// Do not attempt to manipulate the scrollbars until the 
	// initial update has occurred
	if(m_wndHorzSB.IsWindow() && m_wndVertSB.IsWindow() && m_wndSizeBox.IsWindow()) {

		// Adjust scrollbars if beyond logical extents
		AutoScrollIfBeyondLogBounds();

		// Reset the page sizes of the scrollbars
		SetScrollBarPageSizes();

		// Hide scrollbars with page size >= range size
		HideScrollBarsWithFullPageSize();
	}

	// And finally return
	return size;
}

template<class base_t>
CSize MvcScrollWrapper_T<base_t>::SetSize(int cx, int cy)
{
	// Set extents first
	CSize size=MvcWrapper_T<base_t>::SetSize(cx,cy);

	// Position scrollbars first to give instant UI feedback
	// (else scrollbars will seem to "lag" behind fast drag)
	ResetScrollBarRects();

	// Do not attempt to manipulate the scrollbars until the 
	// initial update has occurred
	if(m_wndHorzSB.IsWindow() && m_wndVertSB.IsWindow() && m_wndSizeBox.IsWindow()) {

		// Adjust scrollbars if beyond logical extents
		AutoScrollIfBeyondLogBounds();

		// Reset the page sizes of the scrollbars
		SetScrollBarPageSizes();

		// Hide scrollbars with page size >= range size
		if(HideScrollBarsWithFullPageSize())
			// If change occurred, reset scrollbar positions again
			ResetScrollBarRects();
	}

	// And finally return
	return size;
}

template<class base_t>
CSize MvcScrollWrapper_T<base_t>::SetLogSize(int cx, int cy)
{
	CSize size=MvcWrapper_T<base_t>::SetLogSize(cx,cy);

	// Do not attempt to manipulate the scrollbars until the 
	// initial update has occurred
	if(m_wndHorzSB.IsWindow() && m_wndVertSB.IsWindow() && m_wndSizeBox.IsWindow()) {

		// Adjust scrollbars if beyond logical extents
		AutoScrollIfBeyondLogBounds();

		// Reset the page sizes of the scrollbars
		SetScrollBarPageSizes();

		// And position the scrollbars as appropriate.
		ResetScrollBarRects();

		// Hide scrollbars with page size >= range size
		if(HideScrollBarsWithFullPageSize())
			// If change occurred, reset scrollbar positions again
			ResetScrollBarRects();

		// Set scrollbar position to match logical origin.
		UpdateScrollBarPos();
	}

	return size;
}

// Logical extents have changed. Adjust scrollbars to compensate
template<class base_t>
CSize MvcScrollWrapper_T<base_t>::SetLogExtents(int cx, int cy) {

	CSize sizeExtents=MvcWrapper_T<base_t>::SetLogExtents(cx,cy);

	// Do not attempt to manipulate the scrollbars until the 
	// initial update has occurred
	if(m_wndHorzSB.IsWindow() && m_wndVertSB.IsWindow() && m_wndSizeBox.IsWindow()) {

		// Adjust scrollbars if beyond logical extents
		AutoScrollIfBeyondLogBounds();

		// Reset the page sizes of the scrollbars
		SetScrollBarPageSizes();

		// Hide scrollbars with page size >= range size
		if(HideScrollBarsWithFullPageSize())
			// If change occurred, reset scrollbar positions again
			ResetScrollBarRects();	}

	return sizeExtents;
}

// Logical extents have changed. Adjust scrollbars to compensate
template<class base_t>
CSize MvcScrollWrapper_T<base_t>::SetLogScaling(float fScaleWidth,
                                                float fScaleHeight)
{
	CSize sizeExtents = MvcWrapper_T<base_t>::SetLogScaling(fScaleWidth, fScaleHeight);

	// Do not attempt to manipulate the scrollbars until the 
	// initial update has occurred
	if(m_wndHorzSB.IsWindow() && m_wndVertSB.IsWindow() && m_wndSizeBox.IsWindow()) {

		// Adjust scrollbars if beyond logical extents
		AutoScrollIfBeyondLogBounds();

		// Reset the page sizes of the scrollbars
		SetScrollBarPageSizes(sizeExtents);

		// Hide scrollbars with page size >= range size
		if(HideScrollBarsWithFullPageSize())
			// If change occurred, reset scrollbar positions again
			ResetScrollBarRects();

		// Invalidate the change
		InvalidateScrollInterior(FALSE);
	}

	return sizeExtents;
}

template<class base_t>
bool MvcScrollWrapper_T<base_t>::RouteEvent(IEvent* pIEvent)
{
	bool bHandled = false;

	bHandled = CScrollAdapter::HandleEvent(pIEvent);

	if (!bHandled)
		bHandled = base_t::RouteEvent(pIEvent);

	return bHandled;
}

// 
// Protected Overrides
//


//@mfunc Allocate and create the scrollbar windows
//@rdesc void
// Create all the needed scrollbar objects.
// Use ShowScrollBar function to toggle visibility on/off.
template<class base_t>
BOOL MvcScrollWrapper_T<base_t>::CreateScrollBars()
{
	// Do not call this function more than once!
	ASSERT((!m_wndHorzSB.IsWindow())&&(!m_wndVertSB.IsWindow())&&(!m_wndSizeBox.IsWindow()));

	// get parent window for this scrollbar
	HWND hWndParent = NULL;
	IVisualWindow* pIVisualWnd = guid_cast<IVisualWindow*>(this);
	if (pIVisualWnd != NULL)
		hWndParent = pIVisualWnd->GetWindowHandle();
	if(!hWndParent) return FALSE;

	// Misc data
	CRect rect(0,0,0,0);	// initially empty, will be sized by ResetScrollBarRects
	DWORD dwVisibleStyle;

	// create the "sizebox" to cover any garbage in the lower-right corner
	if(m_bHorzVisible && m_bVertVisible) dwVisibleStyle=WS_VISIBLE;
	else dwVisibleStyle=0;
	VERIFY(m_wndSizeBox.Create(hWndParent, rect, NULL,
								WS_CHILD|dwVisibleStyle|WS_DISABLED|SBS_SIZEBOX|SBS_SIZEBOXBOTTOMRIGHTALIGN,
								0, GetNextScrollBarID()));

	// Horz scrollbar
	if(m_bHorzVisible) dwVisibleStyle=WS_VISIBLE;
	else dwVisibleStyle=0;
	VERIFY(m_wndHorzSB.Create(hWndParent, rect, NULL,
								WS_CHILD|dwVisibleStyle|SBS_HORZ|SBS_BOTTOMALIGN,
								0, GetNextScrollBarID()));

	// Vert scrollbar
	if(m_bVertVisible) dwVisibleStyle=WS_VISIBLE;
	else dwVisibleStyle=0;
	VERIFY(m_wndVertSB.Create(hWndParent, rect, NULL,
								WS_CHILD|dwVisibleStyle|SBS_VERT|SBS_RIGHTALIGN,
								0, GetNextScrollBarID()));

	return TRUE;
}

//@mfunc Set the scrollbar ranges based on current logical sizes
//@rdesc void
template<class base_t>
void MvcScrollWrapper_T<base_t>::SetScrollBarRanges()
{
	// Assumes the scrollbars have already been created
	ASSERT(m_wndHorzSB && m_wndVertSB && m_wndSizeBox);

	SCROLLINFO si;
	si.cbSize=sizeof(SCROLLINFO);
	si.fMask=SIF_RANGE;

	// Total scrollbar range is equivelant to the total virtual size
	// of this visual part
	CPoint ptVirtualOrg=GetVirtualOrigin();
	CSize sizeVirtual=GetVirtualSize();

	// Set the horz range
	si.nMin=ptVirtualOrg.x;
	si.nMax=ptVirtualOrg.x+sizeVirtual.cx;
	m_wndHorzSB.SetScrollInfo(&si);

	// Set the vertical range
	si.nMin=ptVirtualOrg.y;
	si.nMax=ptVirtualOrg.y+sizeVirtual.cy;
	m_wndVertSB.SetScrollInfo(&si);
}

//@mfunc Set the scrollbar page size based on current logical extents
//@syntax SetScrollBarPageSizes()
//@syntax SetScrollBarPageSizes(CSize sizeExtents)
//@parm CSize | sizeLog | Logical size
//@rdesc void
template<class base_t>
void MvcScrollWrapper_T<base_t>::SetScrollBarPageSizes() {
	MvcScrollWrapper_T<base_t>::SetScrollBarPageSizes(GetLogSize());
}

template<class base_t>
void MvcScrollWrapper_T<base_t>::SetScrollBarPageSizes(CSize sizeLog)
{
	// Assumes the scrollbars have already been created
	ASSERT(m_wndHorzSB && m_wndVertSB && m_wndSizeBox);

	SCROLLINFO si;
	si.cbSize=sizeof(SCROLLINFO);
	si.fMask=SIF_PAGE;

	// Set horizontal page size
	si.nPage=sizeLog.cx;
	m_wndHorzSB.SetScrollInfo(&si);
	
	// Ensure page size is positive. The logical size Y component
	// may be negative if the positive Y is upward
	if(sizeLog.cy < 0)
	{
		si.nPage = -sizeLog.cy;
	}
	else
	{
		si.nPage = sizeLog.cy;
	}

	// Set vertical page size
	m_wndVertSB.SetScrollInfo(&si);
}

//@mfunc Hide scrollbars with page size >= range size
//@rdesc True if 1 or more scrollbars has changed visibility.
//@comm
// Queries the current scrollbar page size and range values to determine
// if scrollbar should be hidden from view. If the page size is as large
// as the range, than the scrollbar is hidden from view as it is no longer
// needed. Note: this function will take into account the settings configured
// by SetScrollBarVisibility, thus if the scrollbar is invisible to begin
// with, it will not be redisplayed if the page size is suddenly smaller
// than the range.
template<class base_t>
BOOL MvcScrollWrapper_T<base_t>::HideScrollBarsWithFullPageSize() {
	// Assumes the scrollbars have already been created
	ASSERT(m_wndHorzSB && m_wndVertSB && m_wndSizeBox);

	// utility vars
	BOOL bShowHorz=TRUE;
	BOOL bShowVert=TRUE;
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_PAGE|SIF_RANGE;

	// Query the horz scrollbar
	m_wndHorzSB.GetScrollInfo(&si);
	if((int)si.nPage>=(si.nMax-si.nMin))
		bShowHorz=FALSE;

	// Query the vert scrollbar
	m_wndVertSB.GetScrollInfo(&si);
	if((int)si.nPage>=(si.nMax-si.nMin))
		bShowVert=FALSE; 

	return SetScrollBarVisibility(bShowHorz,bShowVert);
}

//@mfunc Reset the position of the scrollbars
//@parm CPoint | ptParentLogOrigin | Logical origin of parent
template<class base_t>
void MvcScrollWrapper_T<base_t>::ResetScrollBarRects(CPoint ptParentLogOrigin)
{
	// don't bother if scrollbars have not yet been created
	if((!m_wndHorzSB.IsWindow())||(!m_wndVertSB.IsWindow())||(!m_wndSizeBox.IsWindow()))
		return;

	// Misc data init
	CRect rectBar;
	BOOL bSizeBox=(GetSBVisible(TRUE)&&GetSBVisible(FALSE));

	// Get the physical component rect, adjusting for margins and parent's logical origin
	CRect rcBounds(GetBounds());

	CRect rcTotalMargins;
	GetMargins(rcTotalMargins);
	rcBounds.InflateRect(&rcTotalMargins);

	CPoint delta(ptParentLogOrigin.x, ptParentLogOrigin.y);
	rcBounds.OffsetRect(-delta.x, -delta.y);

	CRect rectComponent(rcBounds);
	rectComponent.DeflateRect(&m_rcMargins);

	// First, the horizontal scrollbar
	rectBar = rcBounds;
	rectBar.top = rectComponent.bottom;
	rectBar.right = rectComponent.right;
	m_wndHorzSB.SetWindowPos(NULL,rectBar.left,rectBar.top,rectBar.Width(),
						rectBar.Height(),SWP_NOZORDER);	

	// Second, the vertical scrollbar
	rectBar = rcBounds;
	rectBar.left = rectComponent.right;
	rectBar.bottom = rectComponent.bottom;
	m_wndVertSB.SetWindowPos(NULL,rectBar.left,rectBar.top,rectBar.Width(),
						rectBar.Height(),SWP_NOZORDER);	

	// Finally, the sizebox
	if(bSizeBox) {
		rectBar.top=rectComponent.bottom;
		rectBar.bottom=rectBar.top+m_cyHScroll;
		rectBar.left=rectComponent.right;
		rectBar.right=rectBar.left+m_cxVScroll;
		}
	else rectBar.SetRectEmpty();
	m_wndSizeBox.SetWindowPos(NULL,rectBar.left,rectBar.top,rectBar.Width(),
						rectBar.Height(),SWP_NOZORDER);
}


//
// Protected Implementation
//

// Invalidate the interior of the scrollbar bounding region
template<class base_t>
void MvcScrollWrapper_T<base_t>::InvalidateScrollInterior(BOOL bRepairNow) {
	bRepairNow;
	InvalidateRect(GetBounds(), true);
}

// Scrollbar notification handler
template<class base_t>
bool MvcScrollWrapper_T<base_t>::OnHScroll(int nScrollCode, short nScrollPos, HWND hScrollBar)
{
	nScrollPos; // not used

	// If this message is destined for a different scrollbar, bail out
	ASSERT(m_wndHorzSB);
	if(m_wndHorzSB.m_hWnd!=hScrollBar)
		return false;

	// Interpret the scroll type
	int nDelta = 0;
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	switch(nScrollCode) {
		case SB_LINERIGHT:
			nDelta = (int)m_nLinePixelScroll;
			break;
		case SB_LINELEFT:
			nDelta = -((int)m_nLinePixelScroll);
			break;
		case SB_PAGERIGHT:
			si.fMask = SIF_PAGE;
			m_wndHorzSB.GetScrollInfo(&si);
			nDelta = (int)si.nPage;
			break;
		case SB_PAGELEFT:
			si.fMask = SIF_PAGE;
			m_wndHorzSB.GetScrollInfo(&si);
			nDelta = -((int)si.nPage);
			break;
		case SB_THUMBTRACK:
			// 3 options here: 
			// 1. update immediately
			// 2. display a tooltip
			// 3. do nothing
		case SB_THUMBPOSITION:
			si.fMask = SIF_TRACKPOS;
			m_wndHorzSB.GetScrollInfo(&si);
			nDelta = si.nTrackPos - m_wndHorzSB.GetScrollPos();
			break;
		default:
			break;
	}

	// Offset the logical rect to accomodate the scroll - recall that the
	// logical rect is the visible subset of the virtual canvas (which is
	// then mapped to the device rect)
	if(nDelta!=0) {
		CRect rcLog=GetLogBounds();
		CRect rcLogNew=Pan(nDelta,0);

		// Only invalidate if something changed!
		if(rcLog!=rcLogNew)
			InvalidateScrollInterior();
	}

	return true;
}

// Scrollbar notification handler
template<class base_t>
bool MvcScrollWrapper_T<base_t>::OnVScroll(int nScrollCode, short nScrollPos, HWND hScrollBar)
{
	nScrollPos; // not used

	// If this message is destined for a different scrollbar, bail out
	ASSERT(m_wndVertSB);
	if(m_wndVertSB.m_hWnd!=hScrollBar)
		return false;

	// Interpret the scroll type
	int nDelta = 0;
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	switch(nScrollCode) {
		case SB_LINEUP:
			nDelta = -((int)m_nLinePixelScroll);
			break;
		case SB_LINEDOWN:
			nDelta = (int)m_nLinePixelScroll;
			break;
		case SB_PAGEUP:
			si.fMask = SIF_PAGE;
			m_wndVertSB.GetScrollInfo(&si);
			nDelta = -((int)si.nPage);
			break;
		case SB_PAGEDOWN:
			si.fMask = SIF_PAGE;
			m_wndVertSB.GetScrollInfo(&si);
			nDelta = (int)si.nPage;
			break;
		case SB_THUMBTRACK:
		case SB_THUMBPOSITION:
			si.fMask = SIF_TRACKPOS;
			m_wndVertSB.GetScrollInfo(&si);
			nDelta = si.nTrackPos - m_wndVertSB.GetScrollPos();
			break;
		default:
			break;
	}

	// Adjust the direction of the change to ensure proper movement
	// with respect to the direction of the positive Y-Axis. 
	nDelta *= base_t::GetYAxisDirection();

	// Offset the logical rect to accomodate the scroll - recall that the
	// logical rect is the visible subset of the virtual canvas (which is
	// then mapped to the device rect)
	if(nDelta!=0) {
		CRect rcLog=GetLogBounds();
		CRect rcLogNew=Pan(0,nDelta);

		// Only invalidate if something changed!
		if(rcLog!=rcLogNew)
			InvalidateScrollInterior();
	}

	return true;
}

#if _MFC_VER > 0x0420
// Intellimouse scrolling support
template<class base_t>
BOOL MvcScrollWrapper_T<base_t>::OnMouseWheel(int fwKeys,short zDelta,short xPos,short yPos) {
	xPos; yPos;	// unused

	// Scrollbar visibily affects scrolling action
	BOOL bHorzVis=GetSBVisible(TRUE);
	BOOL bVertVis=GetSBVisible(FALSE);

	// Intellimouse data
	int nLinesToScroll=zDelta/WHEEL_DELTA;
	if(nLinesToScroll==0) return TRUE;	// nothing to do
	BOOL bDidCtrl=fwKeys&MK_CONTROL;

	// Result of this event
	int nDeltaX=0;
	int nDeltaY=0;

	// Calculate how this event should be interpreted
	if(!bDidCtrl) 
	{
		// Default vertical scrolling if possible
		if(bVertVis) 
		{
			// Scroll up/down
			nDeltaY=nLinesToScroll*m_nLinePixelScroll;	// lines to log units
			// Adjust the direction of the change to ensure proper movement
			// with respect to the direction of the positive Y-Axis. 
			nDeltaY *= base_t::GetYAxisDirection();
		}
		// otherwise, apply to horizontal (like Explorer)
		else if(bHorzVis) 
		{
			// Scroll left/right
			nDeltaX=nLinesToScroll*m_nLinePixelScroll;	// lines to log units
		}
	}
	else if(bHorzVis)
	{
		nDeltaX=nLinesToScroll*m_nLinePixelScroll;		// lines to log units
	}

	// Apply the scroll
	if((nDeltaX!=0)||(nDeltaY!=0)) {
		CRect rcLog=GetLogBounds();
		CRect rcLogNew=Pan(-nDeltaX,-nDeltaY);

		// Only invalidate if something changed!
		if(rcLog!=rcLogNew)
			InvalidateScrollInterior();
		}


	return TRUE;
}
#endif

// Determines if scrollbar is currently visible. Note that IsWindowVisible
// will not always work for this implementation, as the parent is invisible
// during the initial update. Therefore, check for the WS_VISIBLE style.
template<class base_t>
BOOL MvcScrollWrapper_T<base_t>::GetSBVisible(BOOL bHorz) {
	ASSERT(m_wndHorzSB && m_wndVertSB);
	if(bHorz)
		return (BOOL)(m_wndHorzSB.GetStyle()&(WS_VISIBLE));
	else 
		return (BOOL)(m_wndVertSB.GetStyle()&(WS_VISIBLE));
}

// When increasing the viewport size, it is possible the right/bottom
// edge will surpass the virtual extents. If this occurs while scrolled
// off the logical origin, start scrolling back the scrollbars to
// display as much non-dead (i.e. within the virtual region) space as
// possible. If we are displaying all the non-dead space, then no 
// other choice remainds but to fill the remaining area with dead space.
template<class base_t>
void MvcScrollWrapper_T<base_t>::AutoScrollIfBeyondLogBounds() {
	CPoint ptLogOrigin=GetLogOrigin();
	CSize sizeLog=GetLogSize();
	CPoint ptVirtualOrg=GetVirtualOrigin();
	CSize sizeVirtual=GetVirtualSize();

	// Check right edge
	int nDeltaX=0;
	int nRightEdge=ptLogOrigin.x+sizeLog.cx;	// physical right
	int nOverlap=nRightEdge-(ptVirtualOrg.x+sizeVirtual.cx);
	if(nOverlap>0) {				// if physical right>logical right
		// right edge beyond virtual extents
		int nPosHorz=m_wndHorzSB.GetScrollPos();
		if(nPosHorz>ptVirtualOrg.x) {
			nDeltaX=_SFLMIN(nPosHorz,nOverlap);	// max change, while pos still >=0
			}
		}

	// Check bottom edge
	int nDeltaY=0;
	int nBottomEdge=ptLogOrigin.y+sizeLog.cy;	// physical bottom
	nOverlap=nBottomEdge-(ptVirtualOrg.y+sizeVirtual.cy);
	if(nOverlap>0) {				// if physical bottom>logical bottom
		// right edge beyond logical extents
		int nPosVert=m_wndVertSB.GetScrollPos();
		if(nPosVert>ptVirtualOrg.y) {
			nDeltaY=_SFLMIN(nPosVert,nOverlap);	// max change, while pos still >=0
			}
		}

	Pan(nDeltaX,nDeltaY);
}

};	// namespace stingray::foundation
};  // namespace stingray

#endif // #if defined(_SFL_ATL_SUPPORT)

#endif  // #ifndef __MVCSCROLLWRAPPER_H__
