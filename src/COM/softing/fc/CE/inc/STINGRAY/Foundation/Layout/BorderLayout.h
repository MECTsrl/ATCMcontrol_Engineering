/////////////////////////////////////////////////////////////////////////////
// BorderLayout.h : SFL Layout Manager
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


#include <foundation\Layout\LayoutNode.h>


namespace stingray {
namespace foundation {

//@doc


//@enum BorderPosition | 
// Define the four areas surrounding a node, where a border can be drawn.<nl>
enum BorderPosition 
{ 
	Top = 0x01,		//@emem Top | Tob border
	Bottom = 0x02,	//@emem Bottom | Bottom border
	Left = 0x04,	//@emem Left | Left border
	Right = 0x08	//@emem Right | Right border
};

//@enum BorderOrientation |
// Orientation of the drawings in the border. Affects items
// like grippers and buttons.
enum BorderOrientation
{
	Vertical = 0x00,	//@emem Vertical orientation.
	Horizontal = 0x01	//@emem Horizontal orientation.
};


//@interface IBorderLayout | 
// Special functionality for border nodes. It offers control over the position
// and orientation of the border decorators, as well as about the visibility of them.<nl>
// Border nodes are limited to one child node, called the contained node. Their objective
// is to provide some decoration that surrounds that specific element, such as gripper, edges
// or other 3-D gadgets that appear around the node; similarly to the non-client area of a window.
//@base ILayoutNode | Basic layout node functionality.
interface __declspec(uuid("36E885E0-6637-11d3-9D9B-00C04F91E286")) IBorderLayout:
	public ILayoutNode
{
	//@meth,method ILayoutNode* | GetContainedNode | 
	// Returns the node contained by this border.
	//@@rdesc Pointer to the ILayoutNode interface of the one node being decorated
	//  by the border node.
	virtual ILayoutNode* GetContainedNode() const = 0;

	//@meth,method void | SetBorderMargins | 
	// Sets the thickness of each border position independently. A thickness of 0 indicates no border
	// is wanted in that given position.
	//@@syntax pBorderNode->SetBorderMargins(nLeft, nTop, nRight, nBottom);
	//@@parm UINT | nLeft | Thickness of left border in pixels.
	//@@parm UINT | nTop | Thickness of top border in pixels.
	//@@parm UINT | nRight | Thickness of right border in pixels.
	//@@parm UINT | nBottom | Thickness of bottom border in pixels.
	virtual void SetBorderMargins(UINT nLeft, UINT nTop, UINT nRight, UINT nBottom) = 0;

	//@meth,method void | GetBorderMargins |
	// Retrieves the thickness in pixels assigned to each one of the border positions.
	//@@syntax pBorderNode->GetBorderMargins(nLeft, nTop, nRight, nBottom);
	//@@parm UINT& | nLeft | Thickness of left border in pixels.
	//@@parm UINT& | nTop | Thickness of top border in pixels.
	//@@parm UINT& | nRight | Thickness of right border in pixels.
	//@@parm UINT& | nBottom | Thickness of bottom border in pixels.
	virtual void GetBorderMargins(UINT& nLeft, UINT& nTop, UINT& nRight, UINT& nBottom) const = 0;

	//@meth,method BorderOrientation | Get/SetBorderOrientation |
	// Sets or retrieves the orientation attribute of the border node.
	// Some border node derivatives, like the gripper node, are orientation
	// sensitive, meaning that the way they display is dependent on this 
	// attribute.
	//@@syntax pBorderNode->SetBorderOrientation(orientation);
	//@@syntax BorderOrientation orientation = pBorderNode->GetBorderOrientation();
	//@@parm BorderOrientation | orientation | Orientation of the border node, either Horizontal
	//  or Vertical. Must be a vaule taken from the <t BorderOrientation> enumeration.
	virtual void SetBorderOrientation(BorderOrientation orientation) = 0;
	virtual BorderOrientation GetBorderOrientation() const = 0;

	//@meth,method void | ShowBorder | 
	//  Shows or hides the border. When the border node is hidden it still participates
	//  in the layout operations, but its area is totally covered by the contained node.
	//@@syntax pBorderNode->ShowBorder([bShow, [bRealizeChange]]);
	//@@parm bool | bShow | If true (default value), the border is shown, if false it gets hidden.
	//@@parm bool | bRealizeChange | If true (default value), layout is recalculated so the change takes effect
	// immediately.
	virtual void ShowBorder(bool bShow = true, bool bRealizeChange = true) = 0;

	//@meth,method bool | IsBorderVisible |
	// Returns the visibility attribute for this border node. Use the <mf IBorderLayout::ShowBorder> method
	// to set this attribute. When the border node is hidden it still participates in the layout operations, 
	// but its area is totally covered by the contained node.
	virtual bool IsBorderVisible() const = 0;
};


/////////////////////////////////////////////////////////////////////////
// CBorderLayoutNode


//@class CBorderLayoutBase |
//  Default implementation of the IBorderLayout interface. This class is designed
//  to be used as a base class by all layout node types that need to provide an
//  implementation of that interface.
//@tcarg typename | _Derived | Most derived class in the inheritance hierarchy.
//@tcarg typename | _LayoutIface | Most derived layout interface implemented by _Derived. Must be a 
// IBorderLayout descendant.
//@tcarg typename | _Base | Class this class is derived from.
//@base public | _Base 
// The actual base class is determined by the third template parameter. By default it is 
// CLayoutNode<lt>_Derived, _LayoutIface<gt>, although for most border nodes implemented in SFL the actual base
// base class will be CDCLayoutBase<lt>_Derived, _LayoutIface<gt>.
template <typename _Derived, typename _LayoutIface = IBorderLayout, typename _Base = CLayoutNode<_Derived, _LayoutIface> >
class CBorderLayoutBase: 
	public _Base
{
public:

	CBorderLayoutBase(UINT nBorder = 0, BorderOrientation nOrient = Horizontal);
	CBorderLayoutBase(UINT nLeft, UINT nTop, UINT nRight, UINT nBottom, BorderOrientation nOrient = Horizontal);


public:
	// IBorderLayout interface
	virtual ILayoutNode* GetContainedNode() const;
	virtual void SetBorderMargins(UINT nLeft, UINT nTop, UINT nRight, UINT nBottom);
	virtual void GetBorderMargins(UINT& nLeft, UINT& nTop, UINT& nRight, UINT& nBottom) const;
	virtual void ShowBorder(bool bShow = true, bool bRealizeChange = true);
	virtual bool IsBorderVisible() const {return m_bShowBorder;}
	virtual void SetBorderOrientation(BorderOrientation orientation) {m_orientation = orientation; RecalcLayout(GetCurrentRect());}
	virtual BorderOrientation GetBorderOrientation() const {return m_orientation;}

	// ILayoutNode interface
	virtual int AddLayoutNode(ILayoutNode* pNode);
	virtual int RemoveLayoutNode(ILayoutNode* pNode);
	virtual void SetPreferredRect(const CRect& rcPreferred);
	virtual void GetPreferredRect(CRect& rcPreferred) const;


protected:
	// CLayoutNode overrides
	virtual CRect OnRecalcLayout(const CRect& rcDesired, bool bChildNegotiate, bool bStrictRecalc);

protected:
	// Implementation Member Variables
	UINT m_nBorderLeft, m_nBorderRight, m_nBorderTop, m_nBorderBottom;
	BorderOrientation m_orientation;
	bool m_bShowBorder;
	int m_nPendingDeltaTop, m_nPendingDeltaBottom, m_nPendingDeltaLeft, m_nPendingDeltaRight;

protected:
	// Implementation overridables
	virtual void UpdateBorderSizes(UINT nLeft, UINT nTop, UINT nRight, UINT nBottom);
	virtual CRect GetContainedNodeRect(CRect rcBorder);

	// Implementation member functions
	void AdjustBorderProperties(ILayoutNode* pChild);

};


template <typename _Derived, typename _LayoutIface, typename _Base>
inline
CBorderLayoutBase<_Derived, _LayoutIface, _Base>::CBorderLayoutBase (
	UINT nBorder, 
	BorderOrientation nOrient
	):
m_nBorderLeft(nBorder), m_nBorderRight(nBorder), m_nBorderTop(nBorder), m_nBorderBottom(nBorder),
m_orientation(nOrient), m_bShowBorder(nBorder != 0),
m_nPendingDeltaTop(0), m_nPendingDeltaBottom(0), m_nPendingDeltaLeft(0), m_nPendingDeltaRight(0)
{
}


template <typename _Derived, typename _LayoutIface, typename _Base>
inline
CBorderLayoutBase<_Derived, _LayoutIface, _Base>::CBorderLayoutBase (
	UINT nLeft,
	UINT nTop,
	UINT nRight,
	UINT nBottom,
	BorderOrientation nOrient
): 
m_nBorderLeft(nLeft), m_nBorderRight(nRight), m_nBorderTop(nTop), m_nBorderBottom(nBottom),
m_orientation(nOrient), m_bShowBorder((nLeft != 0) || (nRight != 0) || (nTop != 0) || (nBottom != 0)),
m_nPendingDeltaTop(0), m_nPendingDeltaBottom(0), m_nPendingDeltaLeft(0), m_nPendingDeltaRight(0)
{
}


template <typename _Derived, typename _LayoutIface, typename _Base>
inline
ILayoutNode* CBorderLayoutBase<_Derived, _LayoutIface, _Base>::GetContainedNode (
) const
{
	int nImmediateChildren = GetChildrenCount();
	_ASSERTE(nImmediateChildren == 1 || nImmediateChildren == 1);

	ILayoutNode* pContained = 0;
	// No contained node
	if (nImmediateChildren == 1) {
		pContained = *(m_Children.begin());
	}

	return pContained;
}


template <typename _Derived, typename _LayoutIface, typename _Base>
inline
void CBorderLayoutBase<_Derived, _LayoutIface, _Base>::SetBorderMargins (
	UINT nLeft, 
	UINT nTop, 
	UINT nRight, 
	UINT nBottom
)
{
	m_nBorderLeft   = nLeft; 
	m_nBorderTop    = nTop;
	m_nBorderRight  = nRight; 
	m_nBorderBottom = nBottom;
}


template <typename _Derived, typename _LayoutIface, typename _Base>
inline
void CBorderLayoutBase<_Derived, _LayoutIface, _Base>::GetBorderMargins (
	UINT& nLeft, 
	UINT& nTop, 
	UINT& nRight, 
	UINT& nBottom
) const
{
	nLeft   = m_nBorderLeft; 
	nTop    = m_nBorderTop;
	nRight  = m_nBorderRight; 
	nBottom = m_nBorderBottom;
}


template <typename _Derived, typename _LayoutIface, typename _Base>
inline
void CBorderLayoutBase<_Derived, _LayoutIface, _Base>::ShowBorder (
	bool bShow,
	bool bRealizeChange
)
{
	if (m_bShowBorder == bShow) {
		return;
	}

	// Cache the new state, and recalculate child layout.
	m_bShowBorder = bShow;
	UpdateBorderSizes(m_nBorderLeft, m_nBorderTop, m_nBorderRight, m_nBorderBottom);
	RecalcLayout();

	// Realize now, if prompted to.
	if (bRealizeChange) {
		CLayoutTransaction aTransaction;

		RealizeNode(aTransaction);
	}
}


template <typename _Derived, typename _LayoutIface, typename _Base>
void CBorderLayoutBase<_Derived, _LayoutIface, _Base>::UpdateBorderSizes (
	UINT nLeft,
	UINT nTop,
	UINT nRight,
	UINT nBottom
)
{
	if (m_bShowBorder) {
		SetBorderMargins(nLeft, nTop, nRight, nBottom);
	}
	else {
		SetBorderMargins(0, 0, 0, 0);
	}
}


template <typename _Derived, typename _LayoutIface, typename _Base>
CRect CBorderLayoutBase<_Derived, _LayoutIface, _Base>::GetContainedNodeRect (
	CRect rcBorder
)
{
	CRect rcChild = rcBorder;
	rcChild.DeflateRect(m_nBorderLeft, m_nBorderTop, m_nBorderRight, m_nBorderBottom);

	// Make sure the child is still within the bounds of its parent!
	rcChild.left  = _SFLMAX(rcChild.left, rcBorder.left);
	rcChild.left  = _SFLMIN(rcChild.left, rcBorder.right);
	rcChild.top   = _SFLMAX(rcChild.top, rcBorder.top);
	rcChild.top   = _SFLMIN(rcChild.top, rcBorder.bottom);
	rcChild.right = _SFLMIN(rcChild.right, rcBorder.right);
	rcChild.right = _SFLMAX(rcChild.right, rcBorder.left);
	rcChild.bottom = _SFLMIN(rcChild.bottom, rcBorder.bottom);
	rcChild.bottom = _SFLMAX(rcChild.bottom, rcBorder.top);

	return rcChild;
}


template <typename _Derived, typename _LayoutIface, typename _Base>
int CBorderLayoutBase<_Derived, _LayoutIface, _Base>::AddLayoutNode (
	ILayoutNode* pNode
)
{
	// Ensure that the border wrapper
	int nImmediateChildren = GetChildrenCount();
	if (nImmediateChildren >= 1) {
		throw std::runtime_error("Contained node already in place");
	}

	return _LayoutBase::AddLayoutNode(pNode);
}


template <typename _Derived, typename _LayoutIface, typename _Base>
int CBorderLayoutBase<_Derived, _LayoutIface, _Base>::RemoveLayoutNode (
	ILayoutNode* pNode
)
{
	return _LayoutBase::RemoveLayoutNode(pNode);
}


template <typename _Derived, typename _LayoutIface, typename _Base>
inline
void CBorderLayoutBase<_Derived, _LayoutIface, _Base>::SetPreferredRect (
	const CRect& rcPreferred
)
{
	// Apply the adjusted preferred rect to the contained node as well
	ILayoutNode* pContained = GetContainedNode();
	if (pContained) {
		CRect rcChildPreferred = rcPreferred;
		rcChildPreferred.DeflateRect(m_nBorderLeft, m_nBorderTop, m_nBorderRight, m_nBorderBottom);
		pContained->SetPreferredRect(rcChildPreferred);
	}
}


template <typename _Derived, typename _LayoutIface, typename _Base>
inline
void CBorderLayoutBase<_Derived, _LayoutIface, _Base>::GetPreferredRect (
	CRect& rcPreferred
) const
{
	// Our preferred rect is the contained preferred rect + the border space
	ILayoutNode* pContained = GetContainedNode();
	if (pContained) {
		pContained->GetPreferredRect(rcPreferred);
		rcPreferred.InflateRect(m_nBorderLeft, m_nBorderTop, m_nBorderRight, m_nBorderBottom);
	}
	else {
		rcPreferred = GetCurrentRect();
	}
}


template <typename _Derived, typename _LayoutIface, typename _Base>
CRect CBorderLayoutBase<_Derived, _LayoutIface, _Base>::OnRecalcLayout (
	const CRect& rcDesired, 
	bool bChildNegotiate, 
	bool /*bStrictRecalc*/
)
{
	// To get rid of warning...
	bChildNegotiate;

	CRect rcNew = rcDesired;

	// If negatively sized, trim off at 0 width/height
	rcNew.right  = _SFLMAX(rcNew.right, rcNew.left);
	rcNew.bottom = _SFLMAX(rcNew.bottom, rcNew.top);
	
	// Apply this new rect size
	SetCurrentRect(rcNew);

	// Shrink this new rect to contain children, as specified by the margins
	ILayoutNode* pChild = GetContainedNode();
	if (pChild) {
		CRect rcChild = GetContainedNodeRect(rcNew);

		// Reset this border's min size large enough to contain the bordered node
		AdjustBorderProperties(pChild);

		// And dish off recalc to that child
		pChild->RecalcLayout(rcChild, false, true);
	}
 
 	return rcNew;
}


template <typename _Derived, typename _LayoutIface, typename _Base>
void CBorderLayoutBase<_Derived, _LayoutIface, _Base>::AdjustBorderProperties (
	ILayoutNode* pChild
)
{
	m_bVisible = m_bVisible = pChild->IsVisible();
	if (!m_bVisible) {
		SetMinMaxSize(CSize(0,0),CSize(0,0),0);				
		return;
	}	

	// Get contained node's min/max settings	
	CSize szMin,szMax;
	DWORD dwMinMaxFlags;
	pChild->GetMinMaxSize(szMin, szMax, dwMinMaxFlags);

	// Add the border padding
	szMin.cx += m_nBorderLeft + m_nBorderRight;
	szMin.cy += m_nBorderTop + m_nBorderBottom;
	szMax.cx += m_nBorderLeft + m_nBorderRight;
	szMax.cy += m_nBorderTop + m_nBorderBottom;

	SetMinMaxSize(szMin, szMax, dwMinMaxFlags);
}

 

/////////////////////////////////////////////////////////////////////////
// CBorderLayout

class __declspec(uuid("36E885E1-6637-11d3-9D9B-00C04F91E286")) CBorderLayout:
	public CBorderLayoutBase<CBorderLayout>
{
public:
	typedef CBorderLayoutBase<CBorderLayout> _LayoutBase;
};




};	// namespace stingray::foundation
};	// namespace stingray

