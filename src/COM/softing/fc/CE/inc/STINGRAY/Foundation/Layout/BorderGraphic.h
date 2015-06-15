/////////////////////////////////////////////////////////////////////////////
// BorderGraphic.h : SFL Layout Manager
//
// Stingray Software Windows Template Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Gilberto Araya
// Description:  SFL Layout Manager border drawing nodes
//


#pragma once

#if !defined(_SFL)
#define _SFL
#endif


#include <foundation\Layout\BorderLayout.h>
#include <foundation\Layout\DCNode.h>
#include <foundation\Graphics\GCCore.h>


namespace stingray {
namespace foundation {

//@doc


/////////////////////////////////////////////////////////////////////////
// CBorderGraphic


//@class CBorderGraphic | 
// Base layout class for painted borders.
// Borders are decorators around other layout nodes, where certain 
// area is left between the boundaries of the border node's rectangle
// and the contained node's rectangle. That space is then filled with some
// visual effect, depending on the node type.
//@tcarg typename | _Derived | Most derived descendent class.
//@tcarg typename | _LayoutInterface | Interface derived from ILayoutNode that the node implements.
//@supint ILayoutNode | Basic layout node interface.
//@supint _LayoutInterface | Second template parameter. Must be an interface derived from ILayoutNode.
//@base public | CBorderLayoutBase<lt>_Derived, _LayoutInterface, CDCLayoutBase<lt>_Derived, _LayoutInterface<gt><gt>
template <typename _Derived, typename _LayoutIface = IBorderLayout>
class CBorderGraphic:
	public CBorderLayoutBase<_Derived, _LayoutIface, CDCLayoutBase<_Derived, _LayoutIface> >
{
public:
	typedef CBorderLayoutBase<_Derived, _LayoutIface, CDCLayoutBase<_Derived, _LayoutIface> > _LayoutBase;

	CBorderGraphic(UINT nBorder = 0, BorderOrientation nOrient = Horizontal)
		: _LayoutBase(nBorder, nOrient)
	{}

	CBorderGraphic(UINT nLeft, UINT nTop, UINT nRight, UINT nBottom, BorderOrientation nOrient = Horizontal)
		: _LayoutBase(nLeft, nTop, nRight, nBottom, nOrient)
	{}

public:

	// ILayoutNode interface
	virtual bool EraseNodeBkgnd(HDC hDC = NULL, bool bApplyToChildren = true);

protected:
	// CDCLayoutBase overrides
	virtual void OnPaint(HDC hDC = NULL);

protected:
	// Implementation overridables
	virtual void DrawBorderRegion(HDC hDC, BorderPosition border, CRect rcBorder);

	virtual void CalcBorderRects(CRect& rcLeft, CRect& rcTop, CRect& rcRight, CRect& rcBottom) const;
};


template <typename _Derived, typename _LayoutIface>
bool CBorderGraphic<_Derived, _LayoutIface>::EraseNodeBkgnd (
	HDC hDC, 
	bool bApplyToChildren
)
{
	if (bApplyToChildren) {
		_LayoutBase::EraseNodeBkgnd(hDC, true);
	}

	return true;
}


template <typename _Derived, typename _LayoutIface>
void CBorderGraphic<_Derived, _LayoutIface>::OnPaint (
	HDC hDC
)
{
	if (m_bShowBorder) {
		CRect rcLeft, rcRight, rcTop, rcBottom;
		CalcBorderRects(rcLeft, rcTop, rcRight, rcBottom);
		DrawBorderRegion(hDC, Left, rcLeft);
		DrawBorderRegion(hDC, Top, rcTop);
		DrawBorderRegion(hDC, Right, rcRight);
		DrawBorderRegion(hDC, Bottom, rcBottom);
	}
}


template <typename _Derived, typename _LayoutIface>
void CBorderGraphic<_Derived, _LayoutIface>::DrawBorderRegion (
	HDC hDC, 
	BorderPosition /*pos*/,
	CRect rcBorder
)
{
	::FillRect(hDC, &rcBorder, reinterpret_cast<HBRUSH>(COLOR_BTNFACE + 1));
}


template <typename _Derived, typename _LayoutIface>
void CBorderGraphic<_Derived, _LayoutIface>::CalcBorderRects (
	CRect& rcLeft, 
	CRect& rcTop, 
	CRect& rcRight, 
	CRect& rcBottom
) const
{
	// 2 possible rectangle layouts, depending on the orientation.
	//   Horz:		      Vert:
	//   ---------		  ---------
	//   | | T | |		  |   T   |
	//   | |---| |        |-------|
	//   | |   | |        | |   | |
	//   |L|   |R|        |L|   |R|
	//   | |   | |        | |   | |
	//	 | |---| |		  |-------|
	// 	 | | B | |		  |   B   |
	// 	 ---------		  ---------
	CRect rcNode = GetCurrentRect();
	rcLeft = rcTop = rcRight = rcBottom = rcNode;

	int nInnerLeft = _SFLMIN(static_cast<long>(rcNode.left + m_nBorderLeft), rcNode.right);
	int nInnerRight = _SFLMAX(static_cast<long>(rcNode.right - m_nBorderRight), rcNode.left);
	int nInnerTop = _SFLMIN(static_cast<long>(rcNode.top + m_nBorderTop), rcNode.bottom);
	int nInnerBottom = _SFLMAX(static_cast<long>(rcNode.bottom - m_nBorderBottom), rcNode.top);

	rcLeft.right = nInnerLeft;
	if (m_orientation == Vertical) {
		rcLeft.top = nInnerTop;
		rcLeft.bottom = nInnerBottom;
	}

	rcRight.left = nInnerRight;
	if (m_orientation == Vertical) {
		rcRight.top = nInnerTop;
		rcRight.bottom = nInnerBottom;
	}

	rcTop.bottom = nInnerTop;
	if (m_orientation == Horizontal) {
		rcTop.left = nInnerLeft;
		rcTop.right = nInnerRight;
	}

	rcBottom.top = nInnerBottom;
	if (m_orientation == Horizontal) {
		rcBottom.left = nInnerLeft;
		rcBottom.right = nInnerRight;
	}
}



/////////////////////////////////////////////////////////////////////////
// CBorderEdge

//@interface IBorderEdge |
// Provides functionality for edge decorators.
//@base public | IBorderLayout
interface __declspec(uuid("5EE9EAE3-67B2-11d3-9D9C-00C04F91E286")) IBorderEdge:
	public IBorderLayout
{
	//@prop,property UINT | Get/SetBorderEdgesToDraw |
	// Retrieves or sets the combination of borders on which an edge will be painted.
	//@@syntax UINT nBorders = pBorderNode->GetBorderEdgesToDraw();
	//@@syntax pNode->SetBorderEdgesToDraw(nBorders);
	//@@parm UINT | nBorders | Specifies the border on which an edge will be drawn. Must
	//  be a combination of values from the <t BorderPosition> enumeration, using the logical 
	//  operator OR.
	virtual void SetBorderEdgesToDraw(UINT nBorders) = 0;
	virtual UINT GetBorderEdgesToDraw() const = 0;
};

enum DefaultEdgeDimensions
{
	nDefaultExteriorPad = 0,
	nDefaultEdgeSize    = 2,
	nDefaultInteriorPad = 0
};


//@class CBorderEdge |
// Implementation of edge borders. Edge border nodes paint an edge around
// the contained nodes, in the exact borders the are configured to using the
// methods in the <i IBorderEdge> interface. An "edge" is a 3D line that gives
// the appearance of a raised frame where the contained node resides.
//@supint ILayoutNode | Basic layout node functionality.
//@supint IBorderLayout | Border node functionality.
//@supint IBorderEdge | Edge display.
//@base | CBorderGraphic <lt>CBorderEdge, IBorderEdge<gt>
class __declspec(uuid("5EE9EAE4-67B2-11d3-9D9C-00C04F91E286")) CBorderEdge
: public CBorderGraphic<CBorderEdge, IBorderEdge>
{
public:
	typedef CBorderGraphic<CBorderEdge, IBorderEdge> _LayoutBase;
	CBorderEdge();
	CBorderEdge(const CBorderEdge& rhs): 
		_LayoutBase(rhs), m_nBordersToDraw(rhs.m_nBordersToDraw),
		m_nExteriorPad(rhs.m_nExteriorPad), m_nInteriorPad(rhs.m_nInteriorPad), m_nEdgeSize(rhs.m_nEdgeSize),
		m_penHilite(NULL), m_penShadow(NULL)
	{}

public:
	// IBorderEdge interface
	virtual void SetBorderEdgesToDraw(UINT nBorders) {m_nBordersToDraw = nBorders; UpdateBorderSizes();}
	virtual UINT GetBorderEdgesToDraw() const {return m_nBordersToDraw;}

protected:
	// CDCLayoutBase overrides
	virtual void OnPaint(HDC hDC = NULL);

	// CBorderGraphic overrides
	virtual void DrawBorderRegion(HDC hDC, BorderPosition border, CRect rcBorder);

	// Implementation
	void UpdateBorderSizes();

	virtual void DrawExteriorPadding(HDC hDC, const CRect rcDraw, BorderPosition border, BorderOrientation orientation);
	virtual void DrawClientEdge(HDC hDC, const CRect rcDraw, BorderPosition border, BorderOrientation orientation);
	virtual void DrawInteriorPadding(HDC hDC, const CRect rcDraw, BorderPosition border, BorderOrientation orientation);
	CRect CalcDrawingRect(const CRect rcBorderTotal, UINT nStartPad, UINT nStopPad, BorderPosition border);

protected:
	UINT m_nBordersToDraw;
	UINT m_nExteriorPad, m_nEdgeSize, m_nInteriorPad;
	CGDIPen m_penHilite;
	CGDIPen m_penShadow;

public:
};



/////////////////////////////////////////////////////////////////////////
// CGripperWrapper


template <typename _Derived, typename _LayoutIface = IBorderLayout>
class CGripperBase:
	public CBorderGraphic<_Derived, _LayoutIface>
{
public:
	typedef CBorderGraphic<_Derived, _LayoutIface> _LayoutBase;

	CGripperBase(UINT nLeft = 0, UINT nTop = 0, UINT nRight = 0, UINT nBottom = 0, BorderOrientation nOrient = Horizontal)
		: _LayoutBase(nLeft, nTop, nRight, nBottom, nOrient)
	{}


};


// Constants: default dimensions
enum DefaultBorderThickness 
{
	nDefBorderThickLeft   = 6,
	nDefBorderThickTop    = 3,
	nDefBorderThickRight  = 6,
	nDefBorderThickBottom = 3,
	nDefGripperThickness = 10
};

// Constant zone info for CalcInternalGripperZones below
enum DefaultGripperZones 
{
	nZone1Single = 2,
	nZone2 = 2,
	nZone3 = 2,
	nZoneExA = 1,
	nZoneExB = 11,
	nZoneExC = 2,
	nZoneExD = nZoneExB,
	nZoneExE = 0,
	nZoneEx2 = nZoneExB
};


//@class CGripperWrapper |
// Border layout node that draws a gripper around a displayable element. 
// A gripper is an area that resembles a handle, drawn with a 3-D effect.
// Grippers are usually used for drag and drop operations, although the layout
// manager does not support this functionality at this time.<nl>
// Grippers can have orientation horizontal or vertical, and can be shown on
// any of the four areas of the border: left, top, bottom and right.
//@base public | CBorderGraphic<lt>CGripperWrapper, IBorderLayout<gt>
//@supint ILayoutNode | Basic layout node functionality.
//@supint IBorderLayout | Additional functionality for border nodes.
class __declspec(uuid("5EE9EAE0-67B2-11d3-9D9C-00C04F91E286")) CGripperWrapper
: public CGripperBase<CGripperWrapper>
{

public:
	typedef CGripperBase<CGripperWrapper> _LayoutBase;

	CGripperWrapper():
		_LayoutBase(nDefBorderThickLeft, nDefBorderThickTop, nDefBorderThickRight, nDefBorderThickBottom, Horizontal),
		m_GripperPosition(Left), 
		m_nGripperThickness(nDefGripperThickness), m_bSingleGripper(false)
	{}

protected:
	// IBorderLayout interface
	virtual void SetBorderOrientation(BorderOrientation orientation);

	// CBorderLayoutBase<>
	virtual CRect GetContainedNodeRect(CRect rcBorder);
	
	// CBorderGraphic
	virtual void DrawBorderRegion(HDC hDC, BorderPosition pos, CRect rcBorder);
	virtual void CalcBorderRects(CRect& rcLeft, CRect& rcTop, CRect& rcRight, CRect& rcBottom) const;


protected:
	// Data members

	// Gripper properties
	BorderPosition m_GripperPosition;
	unsigned int m_nGripperThickness;
	bool m_bSingleGripper;


protected:
	// Helper data structures

	// Internal settings for gripper component calculations
	// Zone information can be applied to horizontal or vertical grippers, 
	// following this general diagram
	// ----------------------------------------------------  ^   ^
	// |                                                  |  1   |
	// |                                                  |  |   |
	// |                                                  |  v   |
	// |  ----------------------------------------------  |  ^   |
	// |                                                  |  2   |
	// |  ----------------------------------------------  |  v   |
	// |                                                  |  ^   |
	// |                                                  |  3   | 
	// |                                                  |  v  nThicknessSecondary	
	// |  ----------------------------------------------  |  ^   |
	// |                                                  |  2   | 
	// |  ----------------------------------------------  |  v   |  
	// |                                                  |  ^   |  	
	// |                                                  |  4   |
	// |                                                  |  |   |	
	// ----------------------------------------------------  v   v
	// <A>                                              <B>
	// <----------------- nThicknessPrimary -------------->
	//
	// nThicknessPrimary and Secondary are independent of the actual
	// orientation this gripper will use.
	struct CGripperInternalZones 
	{
		UINT m_nZone1,m_nZone2,m_nZone3,m_nZone4;
		UINT m_nZoneA,m_nZoneB;
	};


protected:
	// Implementation helpers
	virtual void DrawGripper(HDC hDC, CRect rcGripper);

	void CalculateGripperComponents(const CRect& rcGripper, CRect& rcGripperOutside, CRect& rcGripperInside);
	void CalculateGripperComponents(const CRect& rcGripper, const CGripperInternalZones& aZoneInfo, bool bHorz,
		CRect& rcGripperOutside, CRect& rcGripperInside);
	void CalculateZoneInformation(CGripperInternalZones& aZoneInfo, bool bHorz, 
		UINT nThicknessPrimary, UINT nThicknessSecondary);

protected:

};



};	// namespace stingray::foundation
};	// namespace stingray

