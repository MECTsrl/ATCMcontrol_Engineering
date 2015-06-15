/////////////////////////////////////////////////////////////////////////////
// MvcLogicalPart.h : SFL Model-View-Controller architecture 
//
// Stingray Software Foundation Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Jeff Boenig
// Description:  SFL MVC: Declaration of Logical Part class
//

#pragma once

#ifndef __MVCLOGICALPART_H__
#define __MVCLOGICALPART_H__

#include <Foundation\StructuredGraphics\LogCoordinatesImpl.h>
#include <Foundation\StructuredGraphics\Zoom.h>
#include <Foundation\Mvc\MvcVisualPart.h>

namespace stingray {
namespace foundation {

///////////////////////////////////////////////////////////////////////////
//@doc CMvcLogicalPart
//
//@class CMvcLogicalPart | An MvcLogicalPart is a type of visual part that
// establish its own logical coordinate system and adds support for zooming
// and panning.  You may already be familar with a logical coordinate
// system from MFC's CScrollView and the SetWindowExt/SetViewportExt
// members of the CDC.  It is basically the same idea.  The logical part
// establishes a unit of measure which it uses to measure and draw to its
// client area.  That unit of measure may be pixels, inches, millimeters
// or something application-specific.  When the logical part is instructed
// to draw itself, it first prepares the given DC so that its units of
// measure are in effect for its subsequent drawing.
//
// The logical part maintains a ratio which defines the number of logical
// units per unit in the container.  In other words, the container may
// have its own logical coordinate system which is different than this
// logical parts.  The logical part uses this ratio to map from the
// parent's units to its own and vice versa. This mapping ratio is officially
// known in Windows parlence as the "extents".  It is important to recognize
// that extents are not the same as size.  The extents specify a very simple
// ratio that allows us to transform a logical point or size to a container
// point or size and vice versa. For example, an extent would specify
// "5 logical units map to 2 container units on the X axis".  The logical
// part maintains an extents structure which stores these ratios for the X
// and Y axes.  The extents struct is used by the LPtoCP and CPtoLP members
// to map logical points and sizes in the logical part to points and size
// in the container and vice versa.
//
// There is another important characteristic of MvcLogicalPart: they can be
// nested. Like any visual component, a logical part can be a child of
// another logical part. This means a child can establish an entirely
// different mapping mode than its parent.  For example, the parent could
// measure its client area in inches, while its child measures its own
// client area in twips.
//
//@tcarg class | _Base | Base class for the logical part.
//
//@base public | _Base
//@base public | ILogCoordinatesImpl
//@base public | IZoom

template <class _Base>
class CMvcLogicalPart : public _Base,
                        public ILogCoordinatesImpl< CMvcLogicalPart<_Base> >,
						public IZoom
{
// Embedded types
public:
	typedef CMvcLogicalPart<_Base> _ThisClass;
	typedef _Base _BaseClass;

// Constructor(s) & destructor
public:
	CMvcLogicalPart()
	{
		m_fScaleWidth = (float)1.0;	
		m_fScaleHeight = (float)1.0;	
	}

// Attributes
protected:
	//@cmember
	/* The horizontal scaling factor*/
	float m_fScaleWidth;
	//@cmember
	/* The vertical scaling factor*/
	float m_fScaleHeight;

// Operations
public:
	//@cmember
	/* Returns top, left edge of logical bounds.*/
	CPoint TopLeftLog() const;

	//@cmember
	/* Returns the top-right edge of the logical bounds*/
	CPoint TopRightLog() const;

	//@cmember
	/* Returns bottom, left edge of logical bounds*/
	CPoint BottomLeftLog() const;

	//@cmember
	/* Returns bottom, right edge of logical bounds*/
	CPoint BottomRightLog() const;

	//@cmember
	/* Returns the current center point in logical units*/
	CPoint GetLogCenter() const;

	//@cmember
	/* Center the logical rectangle about the given point*/
	virtual void SetLogCenter(int x, int y);
	void SetLogCenter(const CPoint& ptCenter);

	//@cmember
	/* Returns the logical bounds as a rectangle clipped to the virtual size*/
	CRect GetClippedLogBounds() const;

	//@cmember
	/* Sets the logical and container extents for an axis*/
	virtual void SetAxisExtents(MvcAxis axis, int logical, int container);

	//@cmember
	/* Gets the logical and container extents for an axis*/
	virtual void GetAxisExtents(MvcAxis axis, int& logical, int& container);

	//@cmember
	/* Set scaling ratio for the logical extents*/
	virtual CSize SetLogScaling(float fScaleWidth=(float)1.0,
	                            float fScaleHeight=(float)1.0);
	CSize SetLogScaling(int xNum, int xDenom, int yNum, int yDenom);

	//@cmember
	/* Sets the scaling factors relative to their current value*/
	virtual CSize ScaleRelative(float fScaleWidth=(float)1.0,
	                            float fScaleHeight=(float)1.0);
	CSize ScaleRelative(int xNum, int xDenom,
	                    int yNum, int yDenom);

	//@cmember
	/* Set logical scaling as percent magnification. */
	virtual CSize SetMagnification(const int nPctX, const int nPctY);

	//@cmember
	/* Get logical scaling as percent magnification. */
	virtual CSize GetMagnification() const;

	//@cmember
	/* Increase logical scaling by a percentage*/
	virtual CSize IncreaseMagnification(const int nPctX, const int nPctY);

	//@cmember
	/* Decrease logical scaling by a percentage*/
	virtual CSize DecreaseMagnification(const int nPctX, const int nPctY);

	//@cmember
	/* Adjust window and viewport extents for magnification. */
	virtual void ZoomExtents(CSize& szWndExt, CSize& szVpExt) const;

	//@cmember
	/* Moves this logical part's defining logical rectangle by the specified deltas*/
	virtual CRect Pan(int xOff,int yOff);

	// @cmember
	/* Moves one line up.*/
	virtual void LineUp(BOOL bVertical = TRUE);

	// @cmember
	/* Moves one line down.*/
	virtual void LineDown(BOOL bVertical = TRUE);

	// @cmember
	/* Moves one page up.*/
	virtual void PageUp(BOOL bVertical = TRUE);

	// @cmember
	/* Moves one page down.*/
	virtual void PageDown(BOOL bVertical = TRUE);

	bool LPtoLP(const int nSourceMode, int& cx, int& cy);

	virtual bool ScaleToDeviceRes(CDC* pDC, CSize& szVpExt);
	
	//@cmember
	/* Invalidate the specified component's entire rectangle*/
	virtual void InvalidateVisual(IVisual* pVisual, BOOL bErase = TRUE, BOOL bRepairNow = FALSE);

	// GUID map implements the function QueryGuid()
	BEGIN_GUID_MAP(_ThisClass)
		GUID_ENTRY(IZoom)
		GUID_CHAIN_ENTRY(ILogCoordinatesImpl< _ThisClass >)
		GUID_CHAIN_ENTRY(_Base)
	END_GUID_MAP
};

//@mfunc Returns top, left edge of logical bounds.
//@rdesc CPoint
template <class _Base>
CPoint CMvcLogicalPart<_Base>::TopLeftLog() const
{
	return GetLogOrigin();
}

//@mfunc Returns the top-right edge of the logical bounds
//@rdesc CPoint
template <class _Base>
CPoint CMvcLogicalPart<_Base>::TopRightLog() const
{
	CPoint pt = GetLogOrigin();
	pt.Offset(GetLogSize().cx, 0);
	return pt;
}

//@mfunc Returns bottom, left edge of logical bounds
//@rdesc CPoint
template <class _Base>
CPoint CMvcLogicalPart<_Base>::BottomLeftLog() const
{
	CPoint pt = GetLogOrigin();
	pt.Offset(0, GetLogSize().cy);
	return pt;
}

//@mfunc Returns bottom, right edge of logical bounds
//@rdesc CPoint
template <class _Base>
CPoint CMvcLogicalPart<_Base>::BottomRightLog() const
{
	CPoint pt = GetLogOrigin();
	pt.Offset(GetLogSize());
	return pt;
}

//@mfunc Returns the center point in logical units
//@rdesc CPoint
template <class _Base>
CPoint CMvcLogicalPart<_Base>::GetLogCenter() const
{
	CRect rcLog = GetLogBounds();
	return rcLog.CenterPoint();
}

//@mfunc Center the logical rectangle about the given point
//@syntax SetLogCenter(int cx, int cy)
//@syntax SetLogCenter(const CPoint& ptLog)
//@rdesc void 
//@parm int | x | The x position of the logical center point
//@parm int | y | The y position of the logical center point
//@parm CPoint | ptCenter | The desired logical center point
template <class _Base>
void CMvcLogicalPart<_Base>::SetLogCenter(int x, int y)
{
	CSize szLog = GetLogSize();
	CPoint ptOffset;

	// Center the logical rectangle about the given point
	ptOffset.x = x - (szLog.cx / 2);
	ptOffset.y = y - (szLog.cy / 2);
	ptOffset -= TopLeftLog();
	Pan(ptOffset.x, ptOffset.y);
}

template <class _Base>
void CMvcLogicalPart<_Base>::SetLogCenter(const CPoint& ptCenter)
{
	SetLogCenter(ptCenter.x, ptCenter.y);
}

//@mfunc Returns the logical bounds as a rectangle clipped to the virtual size.
//@rdesc Logical bounds of the visual object clipped to the virtual size.
template <class _Base>
CRect CMvcLogicalPart<_Base>::GetClippedLogBounds() const
{
	CRect rcBounds = GetLogBounds();

	CSize szVirtual = GetVirtualSize();
	CPoint ptVirtualOrg = GetVirtualOrigin();

	if (szVirtual.cx != 0 && szVirtual.cy != 0)
	{
		rcBounds.left = _SFLMAX(rcBounds.left, ptVirtualOrg.x);
		rcBounds.top = _SFLMAX(rcBounds.top, ptVirtualOrg.y);
		rcBounds.right = _SFLMIN(rcBounds.right, ptVirtualOrg.x+szVirtual.cx);
		rcBounds.bottom = _SFLMIN(rcBounds.bottom, ptVirtualOrg.y+szVirtual.cy);
	}

	return rcBounds;
}

//@doc CMvcLogicalPart
//@mfunc Sets the logical and container extents for an axis
//@rdesc void 
//@parm MvcAxis | axis | The axis that the extent is being set for
//@parm  int | logical | The logical extents for the specified axis
//@parm  int | container | The container extents for the specified axis
template <class _Base>
void CMvcLogicalPart<_Base>::SetAxisExtents(MvcAxis axis, int logical, int container)
{
	if (axis == MVCAXIS_X) {
		m_ext.Logical.cx = logical;
		m_ext.Container.cx = container;
	}
	else {
		m_ext.Logical.cy = logical;
		m_ext.Container.cy = container;
	}
}

//@doc CMvcLogicalPart
//@mfunc Gets the logical and container extents for an axis
//@rdesc void 
//@parm MvcAxis axis
//@parm  int& | logical | an integer to receive the logical extents
//@parm  int& | container |  an integer to receive the container extents
template <class _Base>
void CMvcLogicalPart<_Base>::GetAxisExtents(MvcAxis axis, int& logical, int& container)
{
	if (axis == MVCAXIS_X) {
		logical = m_ext.Logical.cx;
		container = m_ext.Container.cx;
	}
	else {
		logical = m_ext.Logical.cy;
		container = m_ext.Container.cy;
	}
}

//@mfunc Set scaling ratio for the logical extents
//@syntax SetLogScaling(float fScaleWidth, float fScaleHeight, CPoint* pPoint)
//@syntax SetLogScaling(int xNum, int xDenom, int yNum, int yDenom, CPoint* pPoint)
//@rdesc CSize the new scaled logical extents
//@parm int | xNum | The numerator of the x scale ratio
//@parm  int | xDenom | The denominator of the x scale ratio
//@parm  int | yNum | The numerator of the y scale ratio
//@parm  int | yDenom | The denominator of the y scale ratio
//@parmopt float | fScaleWidth | 1.0 | The scale factor for the width
//@parmopt float | fScaleHeight | 1.0 | The scale factor for the height
template <class _Base>
CSize CMvcLogicalPart<_Base>::SetLogScaling(float fScaleWidth,
                                     float fScaleHeight)
{
	// Set new logical scaling ratio
	m_fScaleWidth = fScaleWidth;
	m_fScaleHeight = fScaleHeight;
	return GetLogSize();
}

template <class _Base>
CSize CMvcLogicalPart<_Base>::SetLogScaling(int xNum, int xDenom, int yNum, int yDenom)
{
	ASSERT(xDenom!=0);  // check for divide by zero
	ASSERT(yDenom!=0);  // check for divide by zero
	return SetLogScaling((float)xNum/(float)xDenom,
		                 (float)yNum/(float)yDenom);
}

//@mfunc Zooms in by the specified ratio
//@rdesc The logical size of the visual part after the scaling
//@syntax ScaleRelative(int xNum, int xDenom, int yNum, int yDenom)
//@syntax ScaleRelative(float fScaleWidth, float fScaleHeight)
//@parm int | xNum | The numerator to multiply the width by
//@parm int | xDenom | The denominator to divide the width by
//@parm int | yNum | The numerator to multiply the height by
//@parm int | yDenom | The denominator to divide the height by
//@parm float | fScaleWidth | Fraction to multiply the width by
//@parm float | fScaleHeight | Fraction to multiply the height by
//@parmopt CPoint* | pPoint | NULL | A pointer to a point to anchor
//@comm
// This function scales the logical extents relative to the
// current scaling factor.  For example, calling ScaleRelative()
// repeatedly with the ration of 2/1 will continue to zoom
// in further after each call.
template <class _Base>
CSize CMvcLogicalPart<_Base>::ScaleRelative(float fScaleWidth,
                                     float fScaleHeight)
{
	return SetLogScaling(m_fScaleWidth * fScaleWidth,
		                 m_fScaleHeight * fScaleHeight);
}

template <class _Base>
CSize CMvcLogicalPart<_Base>::ScaleRelative(int xNum, int xDenom,
                                     int yNum, int yDenom)
{
	ASSERT(xDenom!=0);  // check for divide by zero
	ASSERT(yDenom!=0);  // check for divide by zero
	return ScaleRelative((float)xNum/(float)xDenom,
		                 (float)yNum/(float)yDenom);
}

//@mfunc Set logical scaling as percent magnification
//@rdesc Size of logical rectangle after magnification
//@parm Horizontal magnification percentage.
//@parm Vertical magnification percentage.
template <class _Base>
CSize CMvcLogicalPart<_Base>::SetMagnification(const int nPctX, const int nPctY)
{
	float fScaleWidth = 100.0f / (float) nPctX;
	float fScaleHeight = 100.0f / (float) nPctY;
	return SetLogScaling(fScaleWidth, fScaleHeight);
}

//@mfunc Get logical scaling as percent magnification
//@rdesc X and Y percent magnification
template <class _Base>
CSize CMvcLogicalPart<_Base>::GetMagnification() const
{
	float fMagX = ((float)100 / m_fScaleWidth);
	float fMagY = ((float)100 / m_fScaleHeight);
	CSize szMag;
	szMag.cx = (int)(fMagX + 0.5);
	szMag.cy = (int)(fMagY + 0.5);
	return szMag;
}

//@mfunc Increase logical scaling by a percentage
//@rdesc Size of the logical rectangle after magnification
//@parm const int | nPctX | Horizontal magnification percentage increase
//@parm  const int | nPctY | Vertical magnification percentage increase
template <class _Base>
CSize CMvcLogicalPart<_Base>::IncreaseMagnification(const int nPctX, const int nPctY)
{
	CSize szMag = GetMagnification();
	szMag.cx += nPctX;
	szMag.cy += nPctY;
	return SetMagnification(szMag.cx, szMag.cy);
}

//@mfunc Decrease logical scaling by a percentage.
//@rdesc Size of the logical rectangle after magnification
//@parm const int | nPctX | Horizontal magnification percentage decrease
//@parm  const int | nPctY | Vertical magnification percentage decrease
template <class _Base>
CSize CMvcLogicalPart<_Base>::DecreaseMagnification(const int nPctX, const int nPctY)
{
	CSize szMag = GetMagnification();
	szMag.cx -= nPctX;
	szMag.cy -= nPctY;
	return SetMagnification(szMag.cx, szMag.cy);
}

//@mfunc Adjust window and viewport extents for magnification.
//@rdesc void
//@parm Window extents to be adjusted by zoom calculation
//@parm Viewport extents to be adjusted by zoom calculation
template <class _Base>
void CMvcLogicalPart<_Base>::ZoomExtents(CSize& szWndExt, CSize& szVpExt) const
{
	szVpExt; // unused
	// Scale the logical extents to account for zoom factor
	szWndExt.cx = (long) ((float) szWndExt.cx * m_fScaleWidth);
	szWndExt.cx = _SFLMAX(szWndExt.cx, 1L);
	szWndExt.cy = (long) ((float) szWndExt.cy * m_fScaleHeight);
	szWndExt.cy = _SFLMAX(szWndExt.cy, 1L);
}

//@mfunc Moves the logical rect by the specified deltas
//@rdesc The logical rectangle after the translation
//@parm int | xOff | The offset in the X direction in logical units
//@parm int | yOff | The offset in the Y direction in logical units
//@xref <mf CMvcLogicalPart::SetLogOrigin>
//@comm Moves the logical rectangle which defines what is display within the
// logical part by the specified offsets.  In effect, panning is equivalent
// to scrolling the contents of the logical part.  Pan also takes care to
// insure that it doesn't scroll outside the virtual logical space.  In
// other words, the resultant logical rect is bounded by the virtual
// size (esp. useful for scrolling).  If, for some reason, you need to
// scroll outside the virtual space, you can use
// <mf CMvcLogicalPart::SetLogOrigin> or <mf CMvcLogicalPart::OffsetLogOrigin>.
template <class _Base>
CRect CMvcLogicalPart<_Base>::Pan(int xOff,int yOff)
{
	CRect rcLog=GetLogBounds();
	CSize szVirtual=GetVirtualSize();
	CPoint ptVirtualOrg=GetVirtualOrigin();

	// If scrolling off right of virtual space, contain
	if((rcLog.right + xOff) > ptVirtualOrg.x + szVirtual.cx)
		xOff = ptVirtualOrg.x + szVirtual.cx - rcLog.right;
	// If scrolling off left of virtual space, contain
	if((rcLog.left + xOff) < ptVirtualOrg.x)
		xOff = ptVirtualOrg.x - rcLog.left;

	// If scrolling off bottom of virtual space, contain
	if((rcLog.bottom + yOff) > ptVirtualOrg.y + szVirtual.cy)
		yOff = ptVirtualOrg.y + szVirtual.cy - rcLog.bottom;	
	// If scrolling off top of virtual space, contain
	if((rcLog.top + yOff) < ptVirtualOrg.y)
		yOff = ptVirtualOrg.y - rcLog.top;

	OffsetLogOrigin(xOff, yOff);

	return GetLogBounds();
}

//@mfunc Moves the logical rect up or to the left by one logical line
//@parm BOOL | bVerticle | TRUE to scroll vertically.  FALSE to scroll to the left.
//@xref <mf CMvcLogicalPart::PageUp>, <mf CMvcVisualPart::LineDown>
template <class _Base>
void CMvcLogicalPart<_Base>::LineUp(BOOL bVertical)
{
	CSize szLine = GetLogSize();

	// Override this member function if you need to change how the line size
	// is computed.
	szLine.cx /= 45;
	szLine.cy /= 25;

	Pan(-szLine.cx * (bVertical == FALSE), -szLine.cy * (bVertical == TRUE));
}

//@mfunc Moves the logical rect down or to the right by one logical line
//@parm BOOL | bVerticle | TRUE to scroll vertically.  FALSE to scroll to the right.
//@xref <mf CMvcLogicalPart::PageDown>
template <class _Base>
void CMvcLogicalPart<_Base>::LineDown(BOOL bVertical)
{
	CSize szLine = GetLogSize();

	// Override this member function if you need to change how the line size
	// is computed.
	szLine.cx /= 45;
	szLine.cy /= 25;

	Pan(+szLine.cx * (bVertical == FALSE), +szLine.cy * (bVertical == TRUE));
}

//@mfunc Moves the logical rect up or to the left by one logical page
//@parm BOOL | bVerticle | TRUE to scroll vertically.  FALSE to scroll to the left.
//@xref <mf CMvcLogicalPart::LineUp>
template <class _Base>
void CMvcLogicalPart<_Base>::PageUp(BOOL bVertical)
{
	CSize szLog = GetLogSize();
	Pan(-szLog.cx * (bVertical == FALSE), -szLog.cy * (bVertical == TRUE));
}

//@mfunc Moves the logical rect down or to the right by one logical page
//@parm BOOL | bVerticle | TRUE to scroll vertically.  FALSE to scroll to the right.
//@xref <mf CMvcLogicalPart::PageUp>
template <class _Base>
void CMvcLogicalPart<_Base>::PageDown(BOOL bVertical)
{
	CSize szLog = GetLogSize();
	Pan(+szLog.cx * (bVertical == FALSE), +szLog.cy * (bVertical == TRUE));
}

template <class _Base>
bool CMvcLogicalPart<_Base>::ScaleToDeviceRes(CDC* pDC, CSize& szVpExt)
{
	bool bChanged = FALSE;

	//
	// The following mapping modes must be scaled to adjusted for device resolution. Normally,
	// these mapping modes are independent of device resolution. This function makes them
	// dependent on the screen resolution. The screen DPI determines the number of logical
	// units per inch.
	//
	if (m_nMapMode == MM_ANISOTROPIC || m_nMapMode == MM_ISOTROPIC || m_nMapMode == MM_TEXT)
	{
		CDC dcScreen;
		dcScreen.CreateCompatibleDC(0);

		int baseDpiX = dcScreen.GetDeviceCaps(LOGPIXELSX);
		int baseDpiY = dcScreen.GetDeviceCaps(LOGPIXELSY);
		int destDpiX = pDC->GetDeviceCaps(LOGPIXELSX);
		int destDpiY = pDC->GetDeviceCaps(LOGPIXELSY);

		if (baseDpiX != destDpiX || baseDpiY != destDpiY)
		{
			//
			// Resolution of destination device differs from the
			// screen resolution. Scale the viewport extents so
			// that logical units on the destination device are
			// equivalent to logical units on the screen.
			//
			szVpExt.cx = (szVpExt.cx * destDpiX) / baseDpiX;
			szVpExt.cy = (szVpExt.cy * destDpiY) / baseDpiY;
			bChanged = TRUE;
		}
	}

	return bChanged;
}

//@mfunc Map logical point to units specified by the given mapping mode.
//@rdesc TRUE if the conversion was successful, otherwise FALSE
//@parm const int | nTargetMode | Specifies logical units to convert to.
//@parm int& | cx | Reference to width to be mapped
//@parm int& | cy | Reference to height to be mapped
//@comm This function not attempt conversion if the source mode is MM_ANISOTROPIC or MM_ISOTROPIC, or if the
// current mapping mode is the same as the source mode. Otherwise a conversion will be made.
template <class _Base>
bool CMvcLogicalPart<_Base>::LPtoLP(const int nSourceMode, int& cx, int& cy)
{
	static int screenXDpi = 0;
	static int screenYDpi = 0;
	
	if (m_nMapMode == nSourceMode ||
	    nSourceMode == MM_ANISOTROPIC ||
	    nSourceMode == MM_ISOTROPIC)
	{
		return false;
	}

	if (screenXDpi == 0 || screenYDpi == 0)
	{
		CDC dcScreen;
		dcScreen.CreateCompatibleDC(0);
		screenXDpi = dcScreen.GetDeviceCaps(LOGPIXELSX);
		screenYDpi = dcScreen.GetDeviceCaps(LOGPIXELSY);
	}

	// First, convert to twips. A twip is 1/1440 of an inch.
	switch (nSourceMode)
	{
	case MM_TWIPS:
		// That was easy
		break;

	case MM_HIMETRIC:
		cx = (cx * 1440) / 2540;
		cy = (cy * 1440) / 2540;
		break;

	case MM_LOMETRIC:
		cx = (cx * 1440) / 254;
		cy = (cy * 1440) / 254;
		break;

	case MM_HIENGLISH:
		cx = (cx * 1440) / 1000;
		cy = (cy * 1440) / 1000;
		break;

	case MM_LOENGLISH:
		cx = (cx * 1440) / 100;
		cy = (cy * 1440) / 100;
		break;

	case MM_TEXT:
		cx = (cx * 1440) / screenXDpi;
		cy = (cy * 1440) / screenYDpi;
		break;

	case MM_ISOTROPIC:
	case MM_ANISOTROPIC:
		ASSERT(FALSE);
		break;
	}

	// Now, convert to target mode
	switch (m_nMapMode)
	{
	case MM_TWIPS:
		// That was easy
		break;

	case MM_HIMETRIC:
		cx = (cx * 2540) / 1440;
		cy = (cy * 2540) / 1440;
		break;

	case MM_LOMETRIC:
		cx = (cx * 254) / 1440;
		cy = (cy * 254) / 1440;
		break;

	case MM_HIENGLISH:
		cx = (cx * 1000) / 1440;
		cy = (cy * 1000) / 1440;
		break;

	case MM_LOENGLISH:
		cx = (cx * 100) / 1440;
		cy = (cy * 100) / 1440;
		break;

	case MM_TEXT:
		cx = (cx * screenXDpi) / 1440;
		cy = (cy * screenYDpi) / 1440;
		break;

	case MM_ISOTROPIC:
	case MM_ANISOTROPIC:
		// Convert from twips to device units
		cx = (cx * screenXDpi) / 1440;
		cy = (cy * screenYDpi) / 1440;
		// Convert from device units to logical units
		cx = cx * m_ext.Logical.cx;
		if (m_ext.Container.cx != 0)
			cx = cx / m_ext.Container.cx;
		cy = cy * m_ext.Logical.cy;
		if (m_ext.Container.cy != 0)
			cy = cy / m_ext.Container.cy;
		break;
	}

	return true;
}

//@doc CMvcLogicalPart
//@mfunc Invalidates the specified visual component's entire rectangle.
//@rdesc void
//@parm CMvcLogicalPart* | pVC | A pointer to a component to invalidate
//@parmopt BOOL | bErase | TRUE | Should the visual part be completely erased prior to redraw
//@parmopt BOOL | bRepairNow | FALSE | Repair the invalid area immediately
//@comm
// This function invalidates the entire visual component rectangle of the
// component pointed to by pVC.
//@xref <mf CMvcLogicalPart::InvalidateRect>
template <class _Base>
void CMvcLogicalPart<_Base>::InvalidateVisual(IVisual* pVisual, BOOL bErase, BOOL bRepairNow)
{
	CRect rc(0,0,0,0);
	if (pVisual == this)
	{
		rc = GetLogBounds();
	}
	else
	{
		IBounds2D* pIBounds = guid_cast<IBounds2D*>(pVisual);
		if (pIBounds != NULL)
			rc = pIBounds->GetBounds();
	}
	InvalidateRect(rc, bErase, bRepairNow);
}

typedef CMvcLogicalPart< CMvcVisualPartImpl > CMvcLogicalPartImpl;

};	// namespace stingray::foundation
};  // namespace stingray

#endif  // #ifndef __MVCLOGICALPART_H__
