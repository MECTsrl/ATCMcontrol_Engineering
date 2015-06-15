/////////////////////////////////////////////////////////////////////////////
// MvcLogicalPart.h : Declaration of MvcLogicalPart
//
// Stingray Software Extension Classes
// Copyright (C) 1997 Stingray Software Inc,
// All rights reserved.
//
// This source code is only intended as a supplement to
// the Objective Toolkit Class Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding
// the Objective Toolkit product.
//
// Author:       Dean Hallman
// Description:  Declaration of MvcLogicalPart interface.
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Compatibility/MVC/MvcLogicalPart.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

// @doc MvcLogicalPart

#ifndef __COMPAT_MVCLOGICALPART_H__
#define __COMPAT_MVCLOGICALPART_H__

// Includes

#ifndef __COMPAT_MVCVISUALPART_H__
#include "MvcVisualPart.h"
#endif

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

///////////////////////////////////////////////////////////////////////////
//
//  YAxisDirection

//@enum YAxisDirection | Enumeration defining direction positive Y-Axis
// The following enumerations are also used as multipliers in functions
// which depend upon the direction of the Y-Axis. Do not change the values
// of the enumerations. 
enum YAxisDirection
{
	YAXIS_UP = -1, // Don't change!
	YAXIS_DOWN = 1 // Don't change!
};

///////////////////////////////////////////////////////////////////////////
//
//  MvcLogicalPart

//@doc MvcLogicalPart
//@mdata int | MvcLogicalPart | m_nMapMode | The mapping mode as defined by the device context
//@xref <c MvcLogicalPart>

//@doc MvcLogicalPart
//@mdata CPoint | MvcLogicalPart | m_ptLogOrg | The logical origin of this visual part
//@xref <c MvcLogicalPart>

//@doc MvcLogicalPart
//@mdata Extents | MvcLogicalPart | m_ext | The extents struct maps logical units to container units
//@xref <c MvcLogicalPart>

//@doc MvcLogicalPart
//@mdata float | MvcLogicalPart | m_fScaleWidth | The horizontal scaling factor
//@xref <c MvcLogicalPart>

//@doc MvcLogicalPart
//@mdata float | MvcLogicalPart | m_fScaleHeight | The vertical scaling factor
//@xref <c MvcLogicalPart>


//@class
// An MvcLogicalPart is a type of visual part that establish its own logical
// coordinate system and adds support for zooming and panning.  You may
// already be familar with a logical coordinate system from MFC's CScrollView
// and the SetWindowExt/SetViewportExt members of the CDC.  It is basically
// the same idea.  The logical part establishes a unit of measure which it
// uses to measure and draw to its client area.  That unit of measure may be
// pixels, inches, millimeters or something application-specific.  When the
// logical part is instructed to draw itself, it first prepares the given DC
// so that its units of measure are in effect for its subsequent drawing.
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
//@base public | MvcVisualPart
//


class MvcLogicalPart : public MvcVisualPart
{
// Constructor(s) & destructor
public:
	//@access Construction

	//@cmember
	/* Constructor*/
	MvcLogicalPart(MvcVisualPart* pContainer = NULL);
	virtual ~MvcLogicalPart();

// Attributes
protected:
	//@access Protected Members
	
	//@cmember
	/* The mapping mode as defined by the device context*/
	int m_nMapMode;

	//@cmember
	/* The logical origin of this visual part*/
	CPoint m_ptLogOrg;

	// @cmember,mstruct
	/* The extents struct maps logical units to container units*/
	struct Extents
	{
		CSize Logical;		// @@field  Extents are not the same as size! The logical extents in 
							// combination with the container extents define a ratio for mapping
		CSize Container;	// @@field The container extents in combination with the logical extents
							// define a ratio for mapping logical units to container units
	};

	//@cmember
	/* The extents struct maps logical units to container units*/
	struct Extents m_ext;
		
	// zooming factors
	//@cmember
	/* The horizontal scaling factor*/
	float m_fScaleWidth;
	//@cmember
	/* The vertical scaling factor*/
	float m_fScaleHeight;

// Operations
public:
	//@access Operations

	////////////// Device Context //////////////

	//@cmember
	/* Readies the device context for subsequent rendering or logical to device coordinate mapping*/
	virtual void OnPrepareDC(CDC* pDC);

	//@cmember
	/* Normalizes and scales the extents just before jamming them into the DC*/
	virtual void NormalizeAndScaleExtents(CSize& szWndExt, CSize& szVpExt) const;

	////////////// Mapping modes //////////////

	//@cmember
	/* Retrieve the mapping mode used for logical to device coordinate translation*/
	virtual int GetMapMode() const;

	//@cmember
	/* Set the mapping mode used for logical to device coordinate translation*/
	virtual int SetMapMode(int nMapMode);

	//@cmember
	/* Gets the direction of the Y axis */
	virtual YAxisDirection GetYAxisDirection() const;

	////////////////// Logical / Container Mapping Queries /////////////////

	//@cmember
	/* Convert logical units to container units*/
	void LPtoCP( LPPOINT lpPoints, int nCount = 1 ) const;
	void LPtoCP( LPRECT lpRect ) const;
	void LPtoCP( LPSIZE lpSize ) const;
	//@cmember
	/* Convert container units to logical units*/
	void CPtoLP( LPPOINT lpPoints, int nCount = 1 ) const;
	void CPtoLP( LPRECT lpRect ) const;
	void CPtoLP( LPSIZE lpSize ) const;

	BOOL LPtoLP(const int nSourceMode, int& cx, int& cy);

	////////////// Logical / Container Unit Mapping Manipulation //////////////

	//@cmember
	/* Sets the logical and container extents for an axis*/
	virtual void SetAxisExtents(MvcAxis axis, int logical, int container);

	//@cmember
	/* Gets the logical and container extents for an axis*/
	virtual void GetAxisExtents(MvcAxis axis, int& logical, int& container);

	//@cmember
	/* Sets the container extents.  Note: Extents are not the same as size.*/
	virtual CSize SetExtents(int cx, int cy);
	inline CSize SetExtents(const CSize& szExt)
			{ return SetExtents(szExt.cx, szExt.cy); }

	//@cmember
	/* Get the container extents.  Note: Extents are not the same as size.*/
	virtual CSize GetExtents() const;

	//@cmember
	/* Get the logical extents.  Note: Extents are not the same as size.*/
	virtual CSize GetLogExtents() const;

	//@cmember
	/* Set the logical extents.  Note: Extents are not the same as size.*/
	virtual CSize SetLogExtents(int cx, int cy);
	inline CSize SetLogExtents(const CSize& szLog)
			{ return SetLogExtents(szLog.cx, szLog.cy); }

	//@cmember
	/* Indicates if extents can be modified. Extents can only be modified if the*/
	// mapping mode is MM_ANISOTROPIC or MM_ISOTROPIC.
	virtual BOOL ExtentsFixed() const;

	////////////// Logical Origin Accessors //////////////

	//@cmember
	/* Retrieve the origin in logical units (i.e. scroll position)*/
	virtual CPoint GetLogOrigin() const;

	//@cmember
	/* Set the origin in logical units (i.e. scroll position)*/
	virtual CPoint SetLogOrigin(int x, int y);
	inline CPoint SetLogOrigin(const CPoint& ptLog)
			{ return SetLogOrigin(ptLog.x, ptLog.y); }

	//@cmember
	/* Moves the logical origin by the specified deltas*/
	virtual CPoint OffsetLogOrigin(int xOff, int yOff);

	//@cmember
	/* Center the logical rectangle about the given point*/
	virtual void SetLogCenter(int x, int y);
	inline void SetLogCenter(const CPoint& ptCenter)
			{ SetLogCenter(ptCenter.x, ptCenter.y); }

	//@cmember
	/* Returns the current center point in logical units*/
	inline CPoint GetLogCenter() const;

	////////////// Logical Size Accessors //////////////

	//@cmember
	/* Returns the width and height of the component in logical units*/
	virtual CSize GetLogSize() const;

	//@cmember
	/* Sets the width and height of the component in logical units*/
	virtual CSize SetLogSize(int cx, int cy);
	inline CSize SetLogSize(const CSize& szLog)
			{ return SetLogSize(szLog.cx, szLog.cy); }

	////////////// Logical Bounds Accessors ////////////

	//@cmember
	/* Returns the logical bounds as a rectangle*/
	inline CRect GetLogBounds() const;

	//@cmember
	/* Returns top, left edge of logical bounds.*/
	inline CPoint TopLeftLog() const;

	//@cmember
	/* Returns the top-right edge of the logical bounds*/
	inline CPoint TopRightLog() const;

	//@cmember
	/* Returns bottom, left edge of logical bounds*/
	inline CPoint BottomLeftLog() const;

	//@cmember
	/* Returns bottom, right edge of logical bounds*/
	inline CPoint BottomRightLog() const;

	//@cmember
	/* Returns the logical bounds as a rectangle clipped to the virtual size*/
	CRect GetClippedLogBounds() const;

	////////////// Panning Support //////////////

	//@cmember
	/* Moves this logical part's defining logical rectangle by the specified deltas*/
	virtual CRect Pan(int xOff, int yOff);

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

	////////////// Magnification and Scaling Members ////////////

	//@cmember
	/* Get logical scaling as percent magnification*/
	virtual CSize GetMagnification() const;

	//@cmember
	/* Set logical scaling as percent magnification*/
	virtual CSize SetMagnification(const int nPctX, const int nPctY);

	//@cmember
	/* Increase logical scaling by a percentage*/
	virtual CSize IncreaseMagnification(const int nPctX, const int nPctY);

	//@cmember
	/* Decrease logical scaling by a percentage*/
	virtual CSize DecreaseMagnification(const int nPctX, const int nPctY);

	//@cmember
	/* Get the current scaling factors*/
	void GetLogScaling(float& fScaleWidth,float& fScaleHeight);

	//@cmember
	/* Set scaling ratio for the logical extents*/
	virtual CSize SetLogScaling(float fScaleWidth=(float)1.0,
	                            float fScaleHeight=(float)1.0);
	inline CSize SetLogScaling(int xNum, int xDenom,
	                            int yNum, int yDenom)
			{
			// no division by zero!
			ASSERT(xDenom!=0);
			ASSERT(yDenom!=0);
			return SetLogScaling((float)xNum/(float)xDenom,
					(float)yNum/(float)yDenom); }


	//@cmember
	/* Sets the scaling factors relative to their current value*/
	virtual CSize ScaleRelative(float fScaleWidth=(float)1.0,
	                              float fScaleHeight=(float)1.0);
	inline CSize ScaleRelative(int xNum, int xDenom,
	                              int yNum, int yDenom)
			{
			// no division by zero!
			ASSERT(xDenom!=0);
			ASSERT(yDenom!=0);
			return ScaleRelative((float)xNum/(float)xDenom,
					(float)yNum/(float)yDenom); }

	////////////// Virtual Size Accessors ////////////

	//@cmember
	/* Set the total virtual size in logical units*/
	virtual void SetVirtualSize(int cx,int cy);
	inline void SetVirtualSize(const CSize& szVirtual)
			{ SetVirtualSize(szVirtual.cx, szVirtual.cy); }

	//@cmember
	/* Set the total virtual size in logical units*/
	virtual void SetVirtualOrigin(int x,int y);
	inline void SetVirtualOrigin(const CPoint& ptVirtualOrg)
			{ SetVirtualSize(ptVirtualOrg.x, ptVirtualOrg.y); }

	//@cmember
	/* Get the total virtual size in logical units*/
	virtual CSize GetVirtualSize() const;

	//@cmember
	/* Get the origin of the virtual coordinate space in logical units.*/
	virtual CPoint GetVirtualOrigin() const;

	////////////// Invalidation ////////////

	//@cmember
	/* Invalidate the specified visual component*/
	virtual void InvalidateVisual(MvcVisualComponent* pVC, BOOL bErase = TRUE, BOOL bRepairNow = FALSE);

	//@cmember
	/* Invalidate the specified rectangle within this visual part*/
	virtual void InvalidateRect(const CRect& rcLog, BOOL bErase = TRUE, BOOL bRepairNow = FALSE);


// Implementation
protected:

	// Undocumented Helper Function
	void CalcConversionFactors(float& xFactor, float& yFactor) const;

	// These are undocumented members which are may change in the future.
	void ScaleToDeviceRes(CDC* pDC);
	BOOL ScaleToDeviceRes(CDC* pDC, CSize& szVpExt);
};



////////////
//
// Inline functions
//
//@doc MvcLogicalPart
//@mfunc Returns top, left edge of logical bounds.
//@rdesc CPoint 
inline CPoint MvcLogicalPart::TopLeftLog() const
{
	return GetLogOrigin();
}

//@doc MvcLogicalPart
//@mfunc Returns the top-right edge of the logical bounds
//@rdesc CPoint 
inline CPoint MvcLogicalPart::TopRightLog() const
{
	CPoint pt = GetLogOrigin();
	pt.Offset(GetLogSize().cx, 0);
	return pt;
}

//@doc MvcLogicalPart
//@mfunc Returns bottom, left edge of logical bounds
//@rdesc CPoint 
inline CPoint MvcLogicalPart::BottomLeftLog() const
{
	CPoint pt = GetLogOrigin();
	pt.Offset(0, GetLogSize().cy);
	return pt;
}

//@doc MvcLogicalPart
//@mfunc Returns bottom, right edge of logical bounds
//@rdesc CPoint 
inline CPoint MvcLogicalPart::BottomRightLog() const
{
	CPoint pt = GetLogOrigin();
	pt.Offset(GetLogSize());
	return pt;
}

//@doc MvcLogicalPart
//@mfunc Returns the logical bounds as a rectangle
//@rdesc CRect 
inline CRect MvcLogicalPart::GetLogBounds() const
{
	CRect rcLog(GetLogOrigin(), GetLogSize());
	// A rect may be denormalized if we have an upward Y-Axis.
	// So, fix it.
	rcLog.NormalizeRect();
	return rcLog;
}

//@doc MvcLogicalPart
//@mfunc Returns the center point in logical units
//@rdesc CPoint
inline CPoint MvcLogicalPart::GetLogCenter() const
{
	CRect rcLog = GetLogBounds();
	return rcLog.CenterPoint();
}

///////////////////////////////////////////////////////////////////////////

//
// MVC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif  // __COMPAT_MVCLOGICALPART_H__

