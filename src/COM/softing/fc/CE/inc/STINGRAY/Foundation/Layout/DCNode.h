/////////////////////////////////////////////////////////////////////////////
// DCNode.h : SFL Layout Manager
//
// Stingray Software Windows Template Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Gilberto Araya
// Description:  SFL Layout Manager: Windowless, DC based layout nodes
//


#pragma once



#if !defined(_SFL)
#define _SFL
#endif


#include <foundation\Layout\LayoutNode.h>

#ifndef _SFL_NO_PRAGMA_WARNINGS
	#if 1200 <= _MSC_VER
		#pragma warning(push,3)
	#endif
	#pragma warning(disable:4097)
	#pragma warning(disable:4786)
#endif

namespace stingray {
namespace foundation {

//@doc


//@class CDCLayoutBase |
//  Implements a layout node which visual representation is accomplished
//  by painting directly on the area of the master window. <nl>
//  Useful for a variety of situations, like:<nl>
//  - Images and pictures without needing a separate child window.<nl>
//  - Decorations around other elements, like grippers and borders.<nl>
//  - Splitters.<nl>
//@tcarg typename | _Derived | Most derived layout node class. The CDCLayoutBase class is intended to be used as an intermediate base class,
//	not as a concrete node type.
//@tcarg typename | _LayoutIface | Interface the node class is going to supply. Must be or derive from ILayoutNode, which is the default.
//@base public | CLayoutNode<lt>_Derived, _LayoutIface<gt>
//@supint ILayoutNode | Basic layout node interface.
//@supint _LayoutIface | Second template parameter, identifies an interface derived from ILayoutNode.
template <typename _Derived, typename _LayoutIface = ILayoutNode>
class __declspec(uuid("7DBD31D0-6153-11d3-9D9A-00C04F91E286")) CDCLayoutBase: 
	public CLayoutNode<_Derived, _LayoutIface>
{
public:

	CDCLayoutBase(): m_hWnd(0), m_hrgnClip(NULL) {}


public:
	// ILayoutNode interface
	virtual bool IsNodeAttached() const {return (m_hWnd != 0);}
	virtual int GetDeferWindowCount() const;
	virtual void Init(HWND hwndParent, HWND hwndChild = 0);

	// Paint the node
	virtual void PaintNode(HDC hDC = NULL, bool bApplyToChildren = true);
	virtual void OnInvalidateRect(const CRect& rcInvalid, bool bDoErase);
	virtual bool OnRealizeNode(CLayoutTransaction& aTransaction);
	virtual bool EraseNodeBkgnd(HDC hDC = NULL, bool bApplyToChildren = true);


protected:

	//@cmember,mfunc Executes the painting logic of the node (overridable).
	//@@parm HDC | hDC | Handle to device context.
	//@@comm Called internally in the <mf ILayoutNode::PaintNode> implementation,
	//  this function is intended to be overridden in descendent classes to paint
	//  over the rectangle assigned to the node. 
	/* Executes the painting logic of the node */
	virtual void OnPaint(HDC hDC = NULL) = 0;
	//@cmember,mfunc
	// Sets up the DC for drawing
	virtual bool PrepareDC(HDC);
	// Undo the setup done in PrepareDC, restoring the DC to its original state
	virtual void RestoreDC(HDC);


protected:

	// Implementation Member Variables
	HWND m_hWnd;
	CPoint m_ptOrigin;

	HRGN m_hrgnClip;
};


template <typename _Derived, typename _LayoutIface>
int CDCLayoutBase<_Derived, _LayoutIface>::GetDeferWindowCount (
) const
{
	return 1;
}


template <typename _Derived, typename _LayoutIface>
void CDCLayoutBase<_Derived, _LayoutIface>::Init (
	HWND hwndParent, 
	HWND hwndChild
)
{
	m_hWnd = hwndParent;
	_LayoutBase::Init(hwndParent, hwndChild);
}


template <typename _Derived, typename _LayoutIface>
void CDCLayoutBase<_Derived, _LayoutIface>::OnInvalidateRect (
	const CRect& rcInvalid,
	bool bDoErase
)
{
	::InvalidateRect(m_hWnd, rcInvalid, bDoErase);
}


template <typename _Derived, typename _LayoutIface>
bool CDCLayoutBase<_Derived, _LayoutIface>::OnRealizeNode (
	CLayoutTransaction& aTransaction
)
{
	bool bOwnDC = false;
	HDC hdcToDraw = aTransaction.GetDC();
	bool bResult = true;
	try {
		if (hdcToDraw == NULL) {
			hdcToDraw = ::GetDC(m_hWnd);
			if (hdcToDraw) {
				bOwnDC = true;
			}
		}

		PaintNode(hdcToDraw, false);

		// Keep the DC for parent processing
		if (bOwnDC) {
			aTransaction.SetDC(hdcToDraw);
		}

		bResult = _LayoutBase::OnRealizeNode(aTransaction);
	}
	catch (std::runtime_error& ) {
		bResult = false;
	}

	if (bOwnDC) {
		aTransaction.SetDC(NULL);
		::ReleaseDC(m_hWnd, hdcToDraw);
	}

	return bResult;
}


template <typename _Derived, typename _LayoutIface>
void CDCLayoutBase<_Derived, _LayoutIface>::PaintNode (
	HDC hDC, 
	bool bApplyToChildren
)
{
	_LayoutBase::PaintNode(hDC, bApplyToChildren);

	if (PrepareDC(hDC)) {
		OnPaint(hDC);
	}
	else {
		return;
	}

	RestoreDC(hDC);
}


template <typename _Derived, typename _LayoutIface>
bool CDCLayoutBase<_Derived, _LayoutIface>::PrepareDC (
	HDC hdcToDraw
)
{
	if (!hdcToDraw) {
		return false;
	}

	// Set the clipping region of the DC to the area covered by the drawing node
	CRect rcCurrent = GetCurrentRect();
	m_hrgnClip = ::CreateRectRgn(0, 0, 0, 0);
	if (::GetClipRgn(hdcToDraw, m_hrgnClip) == 0) {
		// Clip region is null
		CRect rcClip;
		::GetClipBox(hdcToDraw, &rcClip);
		::SetRectRgn(m_hrgnClip, rcClip.left, rcClip.top, rcClip.right, rcClip.bottom);
	}

	::IntersectClipRect(hdcToDraw, rcCurrent.left, rcCurrent.top, rcCurrent.right, rcCurrent.bottom);

	// Offset the viewport origin to account for node offset value
	int nNCLeft, nNCTop;
	GetNonClientOffset(nNCLeft, nNCTop);
	::GetViewportOrgEx(hdcToDraw, &m_ptOrigin);
	::SetViewportOrgEx(hdcToDraw, m_ptOrigin.x - nNCLeft, m_ptOrigin.y - nNCTop, &m_ptOrigin);

	return true;
}


template <typename _Derived, typename _LayoutIface>
void CDCLayoutBase<_Derived, _LayoutIface>::RestoreDC (
	HDC hdcToDraw
)
{
	// Restore the original viewport origin in case other drawing logic depends on it.
	::SetViewportOrgEx(hdcToDraw, m_ptOrigin.x, m_ptOrigin.y, &m_ptOrigin);

	// Restore the original clipping region minus the area already painted by the drawing node
	CRect rcCurrent = GetCurrentRect();
	::SelectClipRgn(hdcToDraw, m_hrgnClip);
	::ExcludeClipRect(hdcToDraw, rcCurrent.left, rcCurrent.top, rcCurrent.right, rcCurrent.bottom);
	::DeleteObject(m_hrgnClip);
	m_hrgnClip = NULL;
}


template <typename _Derived, typename _LayoutIface>
bool CDCLayoutBase<_Derived, _LayoutIface>::EraseNodeBkgnd (
	HDC hDC, 
	bool bApplyToChildren
)
{
	if (bApplyToChildren) {
		_LayoutBase::EraseNodeBkgnd(hDC, true);
	}

	// Exclude the area occupied by this node from the clipping region.
	// The implication is that the OnPaint method is responsible for 
	// filling the entire region.
	// Override if not all the region will be drawn upon.
	CRect rcCurrent = GetCurrentRect();
	::ExcludeClipRect(hDC, rcCurrent.left, rcCurrent.top, rcCurrent.right, rcCurrent.bottom);

	// Message has been processed
	return true;
}




};	// namespace stingray::foundation
};	// namespace stingray


#ifndef _SFL_NO_PRAGMA_WARNINGS
	#if 1200 <= _MSC_VER
		#pragma warning (pop)
	#endif
#endif