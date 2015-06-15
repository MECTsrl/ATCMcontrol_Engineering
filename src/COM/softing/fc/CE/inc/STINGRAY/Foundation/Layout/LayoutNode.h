/////////////////////////////////////////////////////////////////////////////
// LayoutNode.h : SFL Layout Manager node base definition
//
// Stingray Software Windows Template Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Gilberto Araya
// Description:  SFL Layout Manager node base definition
//


#pragma once
#pragma warning(disable:4786)

#if !defined(_SFL)
#define _SFL
#endif


#include <algorithm>
#include <functional>
#include <foundation\SflCommon.h>
#include <foundation\SflDef.h>
#include <foundation\Patterns\Composite.h>
#include <foundation\Patterns\Factory.h>
#include <foundation\Events\WinEvent.h>
#include <foundation\Events\EventRouter.h>
#include <foundation\Events\EventListener.h>
#include <foundation\Layout\LayoutBase.h>



namespace stingray {
namespace foundation {


//////////////////////////////////////////////////////////////////////////////
// CLayoutNodeImpl

//@doc

class __declspec(uuid("DBDB26B0-5195-11d3-9D94-00C04F91E286")) LayoutComposite;


//@class CLayoutNodeImpl |
// Default implementation of the <c ILayoutNode> interface.
//@base public | _LayoutInterface
//@tcarg typename | _LayoutInterface | Interface derived from ILayoutNode, 
// realized by the most derived layout node class.
template <typename _LayoutIface>
class CLayoutNodeImpl: 
	public _LayoutIface,
	public CComposite<ILayoutNode, &__uuidof(LayoutComposite)>
{

	typedef CComposite<ILayoutNode, &__uuidof(LayoutComposite)> _compositeBase;
public:
	// Initialization/Destruction
	CLayoutNodeImpl();
	virtual ~CLayoutNodeImpl() {};

protected:
	// ILayoutNode properties implementation

	// Implementation member variables
	bool m_bVisible;

	CSize m_sizeMin;
	CSize m_sizeMax;
	DWORD m_dwMinMaxFlags;
	bool  m_bRecalcDirty;
	bool  m_bInvalidateDirty;
	DWORD m_dwExStyle;
	int   m_nNCOffsetLeft;
	int   m_nNCOffsetTop;
	bool  m_bLockRealizeNode;
	bool  m_bHorizontalNode;
	// MFCism
	// CString m_strTitle;

	CRect m_rcInitial;
	CRect m_rcCurrent;
	CRect m_rcPreferred;
	CRect m_rcOld;
	CRect m_rcInvalidate;


public:
	// Composite design pattern operations
	virtual int GetChildrenCount (bool bRecursive = false) const 
	{
		if (bRecursive) {
			return _compositeBase::GetAllDescendantsCount();
		}
		else {
			return _compositeBase::GetChildrenCount();
		}
	}
	virtual ILayoutNode* GetParentNode() {return _compositeBase::GetParent();}
	virtual void SetParentNode(ILayoutNode* pNode) {_compositeBase::SetParent(pNode);}
	virtual ILayoutNode* GetRootParentNode() {return _compositeBase::GetRoot();}
	virtual int AddLayoutNode (ILayoutNode* pNode) 
	{
		// Make sure the node has been properly created and initialized
		// before adding to parent!
		if(!pNode->IsNodeAttached()) {
			return -1;
		}

		return _compositeBase::AddChild(pNode);
	}
	virtual int RemoveLayoutNode(ILayoutNode* pNode) {return _compositeBase::RemoveChild(pNode);}
	virtual ILayoutNode::Iterator* GetChildNodesIterator() {return _compositeBase::GetChildIterator();}
	virtual ILayoutNode* FindChildNode(long nId);


	// Accessors

	// Gets the visibility state
	virtual bool IsVisible() const {return m_bVisible;}

	// Gets/Sets the minimum/maximum node size information.
	virtual void GetMinMaxSize(CSize& sizeMin, CSize& sizeMax, DWORD& dwFlags) const;
	virtual void SetMinMaxSize(const CSize& sizeMin, const CSize& sizeMax, DWORD dwFlags = 0);

	// Marks as clean/dirty for needed position recalculation.
	virtual void SetRecalcDirty(bool bDirty = true, bool bSetAncestorsDirty = false);

	// Gets/Marks as dirty for needed invalidation.
	virtual bool IsInvalidateDirty() const {return m_bInvalidateDirty;}
	virtual void SetInvalidateDirty(bool bDirty = true) {m_bInvalidateDirty = bDirty;}

	// Gets/sets/modifies the node extended style information.
	virtual DWORD GetNodeExStyle() const { return m_dwExStyle; }
	virtual void SetNodeExStyle(DWORD dwExStyle = 0, bool bApplyToChildren = false);
	virtual void ModifyNodeStyleEx(DWORD dwRemove = 0, DWORD dwAdd = 0, bool bApplyToChildren = false);

	// Gets/Sets the realize node lockdown mode (prevents RealizeNode from being processed)
	virtual bool GetLockRealizeNode() const { return m_bLockRealizeNode; }
	virtual void SetLockRealizeNode(bool bLock = true, bool bApplyToChildren = true);

	virtual int GetDeferWindowCount() const { return 0; }
	virtual bool IsNodeAttached() const { return true; }
	virtual long GetNodeId() const {return 0;}

	// Sets the offset value for positioning nodes relative to a nonclient area
	virtual void GetNonClientOffset(int& nLeft, int& nTop) const { nLeft = m_nNCOffsetLeft; nTop = m_nNCOffsetTop; }
	virtual void SetNonClientOffset(int nLeft, int nTop) { m_nNCOffsetLeft=nLeft; m_nNCOffsetTop=nTop;}

	// Sets the base node orientation setting. 
	virtual bool GetNodeHorizontal() const { return m_bHorizontalNode; }
	virtual void SetNodeHorizontal(bool bHorz = true, bool bApplyToChildren = false);


	
	// Rectangle manipulation/queries

	// Gets/Sets initial rectangle setting.
	virtual void GetInitialRect(CRect& rcInitial) const { rcInitial = m_rcInitial;}
	virtual void SetInitialRect(const CRect& rcInitial = CRect());

	// Gets/Sets the preferred rectangle setting.
	virtual void GetPreferredRect(CRect& rcPreferred) const { rcPreferred = m_rcPreferred;}
	virtual void SetPreferredRect(const CRect& rcPreferred) { m_rcPreferred = rcPreferred;}

	// Gets/Sets current rectangle setting.
	virtual CRect GetCurrentRect() const {return m_rcCurrent;}
	virtual CRect GetCurrentRectWithinMinMax() const;
	virtual void SetCurrentRect(const CRect& rcCurrent) {m_rcCurrent = rcCurrent;}

	// Gets/Resets the rectangle setting previous to the last recalculation.
	virtual CRect GetOldRect() const { return m_rcOld; }
	virtual void SetOldRect(const CRect& rcOld) {m_rcOld = rcOld;}

	// Invalidation rectangle setting.
	virtual CRect GetInvalidateRect() const { return m_rcInvalidate; }
	virtual void SetInvalidateRect(const CRect& rcInv) {m_rcInvalidate = rcInv;}

	virtual void FitRectWithinNodeMinMax(CRect& rc) const;


	// Operations

	// Initializes the node and attaches it to an actual window, if necessary
	virtual void Init(HWND hwndParent, HWND hwndChild = 0);

	// Changes the visibility state of the node
	virtual void Show(bool bShow = true);

	// Recalculates the position of this and all child nodes.
	virtual CRect RecalcLayout(const CRect& rcDesired = CRect(),
							   bool bChildNegotiate = true,
							   bool bStrictRecalc = false);

	// "Realize" the node on the screen.
	// This member recursively kicks off RealizeNodeHelper, in general
	// users should override RealizeNodeHelper, not RealizeNode.
	// Applies the latest recalculation to actual screen display.
	virtual bool RealizeNode(CLayoutTransaction& aTransaction);

	// "Smart" invalidation used by RealizeNode, invalidate minimum change!
	// In general, you will not want to override any of these members, but
	// rather override OnInvalidateRect with specific invalidation code.
	virtual void OptimizedRedraw();

	// Kicks off the node painting invalidation.
	virtual bool PerformInvalidation(bool bDoErase, bool bApplyToChildren = true);

	// If applicable, erase/paint the node as needed (optimization above RealizeNode)
	virtual bool EraseNodeBkgnd(HDC hDC = NULL, bool bApplyToChildren = true);
	virtual void PaintNode(HDC hDC = NULL, bool bApplyToChildren = true);

	// Recalculates the Z-Order for this node as appropriate.
	virtual void RecalcZOrder();


protected:
	// Implementation overridables

	// Override to selectively allow or reject recalculation attempts.
	virtual bool DoProcessRecalcLayout(bool bStrict, const CRect& rcDesired);

	// Callback on RecalcLayout. Override this for custom layout.
	virtual CRect OnRecalcLayout(const CRect& rcDesired, bool bChildNegotiate, bool bStrictRecalc);

	// last chance to override node rectangle after RecalcLayout, 
	// but before RealizeNode
	// Override to hook into node "manual overrides"
	virtual void OnApplyOverrides(const CRect& rcOld, CRect& rcNew);

	// Override to hook into parent-child layout negotiation
	virtual void OnNegotiateNodeRect(CRect& rcNode);


	virtual bool OnRealizeNode(CLayoutTransaction& /*aTransaction*/) { return true; }	// NoOp for base class

	// Node painting invalidation is occurring. Override for custom redraw optimizations.
	virtual void OnInvalidateRect(const CRect& /*rcInvalid*/, bool /*bDoErase*/) {}

	virtual void EraseInvalidRects();

	// Calculates the minimum bounding rect based on the current rect of this and all children nodes.
	virtual void CalcMinBoundingRect(CRect& rcBounding);


};	// CLayoutNodeImpl<>



/////////////////////////////////////////////////////////////////////
// foundation::CLayoutNodeImpl implementation
//


template <typename _LayoutIface>
CLayoutNodeImpl<_LayoutIface>::CLayoutNodeImpl (
)
: m_bVisible(true),
  m_dwMinMaxFlags(NoMaxSize), 
  m_bRecalcDirty(true), m_bInvalidateDirty(false),
  m_dwExStyle(Defaults), 
  m_nNCOffsetLeft(0), m_nNCOffsetTop(0),
  m_bLockRealizeNode(false), m_bHorizontalNode(true),
  m_rcInitial(0, 0, 0, 0), m_rcCurrent(0, 0, 0, 0),
  m_rcPreferred(0, 0, 0, 0), m_rcOld(0, 0, 0, 0), m_rcInvalidate(0, 0, 0, 0),
  m_sizeMin(0, 0), m_sizeMax(0, 0)
{
}



//////////////////////////////////
// Attributes

// Composite

template <typename _LayoutIface>
ILayoutNode* CLayoutNodeImpl<_LayoutIface>::FindChildNode (
	long nId
)
{
	ILayoutNode* pNode = 0;
	bool bFound = false;
	for (Children::iterator i = m_Children.begin(); !bFound && (i != m_Children.end()); ++i) {
		bFound = ((*i)->GetNodeId() == nId);
		if (bFound) {
			pNode = *i;
		}
	}

	return pNode;
}


// Accessors


template <typename _LayoutIface>
void CLayoutNodeImpl<_LayoutIface>::GetMinMaxSize (
	CSize& sizeMin, 
	CSize& sizeMax, 
	DWORD& dwFlags
) const
{
	sizeMin = m_sizeMin;
	sizeMax = m_sizeMax;
	dwFlags = m_dwMinMaxFlags;	
}



template <typename _LayoutIface>
void CLayoutNodeImpl<_LayoutIface>::SetMinMaxSize (
	const CSize& sizeMin, 
	const CSize& sizeMax, 
	DWORD dwFlags
)
{
// Sets the minimum/maximum size settings for this node. 
// Flags can be OR'd together from the MinMaxSize enumeration
	m_sizeMin = sizeMin;
	m_sizeMax = sizeMax;
	m_dwMinMaxFlags = dwFlags;

	// Minimum must not be larger than maximum!
#ifdef _DEBUG
	if((!(dwFlags & NoMinSize))&&
	   (!(dwFlags & NoMaxSize))) {
		/*_ASSERTE(m_sizeMin.cx<=m_sizeMax.cx);
		_ASSERTE(m_sizeMin.cy<=m_sizeMax.cy);*/
	}
#endif
}



template <typename _LayoutIface>
void CLayoutNodeImpl<_LayoutIface>::SetRecalcDirty (
	bool bDirty, 	
	bool bSetAncestorsDirty
) 
{
	m_bRecalcDirty = bDirty;
	ILayoutNode* pParent = GetParentNode();

	if ((bSetAncestorsDirty) && (pParent)) {
		pParent->SetRecalcDirty(bDirty, bSetAncestorsDirty);
	}
}



template <typename _LayoutIface>
void CLayoutNodeImpl<_LayoutIface>::SetNodeExStyle (
	DWORD dwExStyle,
	bool bApplyToChildren
)
{
	// Extended styles can be OR'd together from the ExtendedStyles enumeration
	m_dwExStyle = dwExStyle;

	// PreOrder traversal of all child nodes
	if (bApplyToChildren) {
		Children::iterator i;
		for (i = m_Children.begin(); i != m_Children.end(); ++i) {
			(*i)->SetNodeExStyle(dwExStyle, true);
		}
	}
}


template <typename _LayoutIface>
void CLayoutNodeImpl<_LayoutIface>::ModifyNodeStyleEx (
	DWORD dwRemove,
	DWORD dwAdd,
	bool bApplyToChildren
)
{
	// Extended styles can be OR'd together from the ExtendedStyles enumeration
	// modify style for this node
	DWORD dwStyle = GetNodeExStyle();
	dwStyle &= (~(dwRemove));
	dwStyle |= dwAdd;
	SetNodeExStyle(dwStyle, false);

	// and optionally recursively modify style of all children
	if (bApplyToChildren) {
		Children::iterator i;
		for (i = m_Children.begin(); i != m_Children.end(); ++i) {
			(*i)->ModifyNodeStyleEx(dwRemove, dwAdd, true);
		}
	}
}


template <typename _LayoutIface>
void CLayoutNodeImpl<_LayoutIface>::SetLockRealizeNode (
	bool bLock,
	bool bApplyToChildren
)
{
	m_bLockRealizeNode = bLock;

	if (bApplyToChildren) {
		Children::iterator i;
		for (i = m_Children.begin(); i != m_Children.end(); ++i) {
			(*i)->SetLockRealizeNode(bLock, false);
		}
	}
}


template <typename _LayoutIface>
void CLayoutNodeImpl<_LayoutIface>::SetNodeHorizontal (
	bool bHorz,
	bool bApplyToChildren
)
{
	m_bHorizontalNode=bHorz;

	if (bApplyToChildren) {
		Children::iterator i;
		for (i = m_Children.begin(); i != m_Children.end(); ++i) {
			(*i)->SetNodeHorizontal(bHorz, true);
		}
	}	
}



template <typename _LayoutIface>
void CLayoutNodeImpl<_LayoutIface>::SetInitialRect (
	const CRect& rcInitial
) 
{
	CRect rcInitialBounds = rcInitial;
	if (rcInitialBounds.IsRectEmpty()) {
		CalcMinBoundingRect(rcInitialBounds);
	}

	m_rcInitial = rcInitialBounds;
	SetCurrentRect(rcInitialBounds);
	SetPreferredRect(rcInitialBounds);

	// Make sure min/max size is not outside of initial rect
	if (m_sizeMin.cx > rcInitialBounds.Width()) {
		m_sizeMin.cx = rcInitialBounds.Width();
	}
	if (m_sizeMin.cy > rcInitialBounds.Height()) {
		m_sizeMin.cy = rcInitialBounds.Height();
	}
	if (m_sizeMax.cx < rcInitialBounds.Width()) {
		m_sizeMax.cx = rcInitialBounds.Width();
	}
	if (m_sizeMax.cy < rcInitialBounds.Height()) {
		m_sizeMax.cy = rcInitialBounds.Height();
	}
}


template <typename _LayoutIface>
CRect CLayoutNodeImpl<_LayoutIface>::GetCurrentRectWithinMinMax (
) const
{
	CRect rcNode = GetCurrentRect();
	FitRectWithinNodeMinMax(rcNode);
	return rcNode;
}




//////////////////////////////////
// Operations


template <typename _LayoutIface>
void CLayoutNodeImpl<_LayoutIface>::Init (
	HWND hwndParent, 
	HWND hwndChild
)
{
	// To remove warnings...
	hwndChild;
	hwndParent;

	// If child nodes have been added already, adjust the initial rectangle
	// to include the rectangles of those child nodes
	if (m_Children.size() > 0) {
		CRect rcInitial(0, 0, 0, 0);
		GetInitialRect(rcInitial);
		Children::iterator i;
		for (i = m_Children.begin(); i != m_Children.end(); ++i) {
			CRect rcChild(0, 0, 0, 03);
			(*i)->GetInitialRect(rcChild);

			if (rcInitial.IsRectEmpty()) {
				rcInitial = rcChild;
			}
			else {
				rcInitial.UnionRect(rcInitial, rcChild);
			}
		}
		SetInitialRect(rcInitial);
	}
}


template <typename _LayoutIface>
void CLayoutNodeImpl<_LayoutIface>::Show (
	bool bShow
)
{
	m_bVisible = bShow;

	// Apply recursively changes to all descendant nodes
	Children::iterator i;
	for (i = m_Children.begin(); i != m_Children.end(); ++i) {
		(*i)->Show(bShow);
	}

	if (bShow) {
		CLayoutTransaction layouttransaction;
		RealizeNode(layouttransaction);
	}
}


// This member is the primary interface for kicking off a new layout recalculation.
// Based on the paramters passed in, RecalcLayout will position all child nodes as
// desired, then recursively kick off a RecalcLayout for each in turn. 
// If creating your own layout algorithm, it is recommended you override OnRecalcLayout,
// rather than this function.
// 
// RecalcLayout only calculates new node positions. Use RealizeNode to apply the new positions to the screen.
template <typename _LayoutIface>
CRect CLayoutNodeImpl<_LayoutIface>::RecalcLayout (
	const CRect& rcDesired,
	bool bChildNegotiate,
	bool bStrictRecalc
)
{
	CRect rcOld = GetCurrentRect();
	CRect rcNew = rcDesired;

	if (rcNew.IsRectEmpty()) {
		rcNew = rcOld;
	}

	// Recalcs can be expensive - only do this when required!
	if (!DoProcessRecalcLayout(bStrictRecalc, rcNew)) {
		return rcOld;
	}

	// Fire off to the virtual callback
	rcNew = OnRecalcLayout(rcNew, bChildNegotiate, bStrictRecalc);

	// Style overrides receive precedence over all other considerations!
	OnApplyOverrides(rcOld, rcNew);
	SetCurrentRect(rcNew);

	// If there was any change, flag for "smart invalidation" at the Realize stage
	if (!rcNew.EqualRect(&rcOld)) {
		SetInvalidateDirty();
	}

	// Done with recalc, clear dirty bit to avoid unnecessary non-strict recalcs.
	SetRecalcDirty(false);

	return rcNew;
}


// Realize this and all child nodes to the screen.
// This member is used to apply the results of a RecalcLayout to the screen.
// The separation of these 2 mechanism allows a layout the capability of performing
// several recalculations with only 1 screen refresh, hence reducing flicker
// on rapid window movements.
template <typename _LayoutIface>
bool CLayoutNodeImpl<_LayoutIface>::RealizeNode (
	CLayoutTransaction& aTransaction
) 
{
	// If currently in lockdown mode, return instantly.
	if (m_bLockRealizeNode)	{
		return false;
	}

	// If node hidden, also return
	if (!IsVisible()) {
		return false;
	}

	bool bTransactionOwner = false;	

	if (!aTransaction.IsValid()) {
		// Starting a transaction
		bTransactionOwner = true;

		// Need initial node count for BeginDeferWindowPos. Traverse all children and sum up.
		int nNodesToUpdate = 0;
		Children::const_iterator i;
		for (i = m_Children.begin(); i != m_Children.end(); ++i) {
			nNodesToUpdate += (*i)->GetDeferWindowCount();
		}

		if (nNodesToUpdate < 0) {
			return false;
		}

		if (nNodesToUpdate > 0) {
			aTransaction.Init(nNodesToUpdate);
		}
	}

	// Update all nodes in 1 flicker free burst. Need to pass hDWP pointer 
	// to all nodes implementing DeferWindowPos. If derived node does not 
	// utilize DeferWindowPos, this parm should be ignored.
	// recursively realize all children
	Children::iterator i;
	for (i = m_Children.begin(); i != m_Children.end(); ++i) {
		ILayoutNode* pNode = *i;
		pNode->RealizeNode(aTransaction);
	}

	OnRealizeNode(aTransaction);
	// Save the old rectangle to calculate minimum flicker invalidation
	SetInvalidateRect(GetOldRect());
	SetOldRect(GetCurrentRect());

	if (bTransactionOwner) {
		aTransaction.Commit();
		// Scan this and all child nodes for optimized redraw invalidations
		// (optimized redraw can be set on a per-node basis)
		OptimizedRedraw();
	}

	return true;
}


// Optimization above RealizeNode to erase the background upon which this
// node sits (applicable only to self-drawing, "window-less" nodes)
template <typename _LayoutIface>
bool CLayoutNodeImpl<_LayoutIface>::EraseNodeBkgnd (
	HDC hDC,
	bool bApplyToChildren
)
{
	bool bResult = false;
	if (bApplyToChildren) {
		Children::iterator i;
		for (i = m_Children.begin(); i != m_Children.end(); ++i) {
			bResult |= (*i)->EraseNodeBkgnd(hDC, true);
		}
	}

	return bResult;
}


// Optimization above RealizeNode repaint this node 
// (applicable only to self-drawing, "window-less" nodes)
template <typename _LayoutIface>
void CLayoutNodeImpl<_LayoutIface>::PaintNode (
	HDC hDC,
	bool bApplyToChildren
)
{
	if (bApplyToChildren) {
		Children::iterator i;
		for (i = m_Children.begin(); i != m_Children.end(); ++i) {
			(*i)->PaintNode(hDC, true);
		}
	}
}



// Kick off a node repaint invalidation of this and all children nodes.
template <typename _LayoutIface>
bool CLayoutNodeImpl<_LayoutIface>::PerformInvalidation (
	bool bDoErase,
	bool bApplyToChildren
)
{
	// Mark our visitation
	SetInvalidateDirty(false);

	// Calculate optimized invalidation rects for this node (this code works well
	// in the generic sense since only dealing with abstract rectangles) and feed
	// to overrideable OnInvalidateRect for subclass specific invalidation.
	if (bDoErase) {
		// We are erasing this pass. Calculate minimum eraseable invalidation 
		// rects to minimize flicker.
		if(m_dwExStyle & OptimizeRedraw) {
			EraseInvalidRects();
		}
		else {
			OnInvalidateRect(GetCurrentRect(), true);
		}
	}
	else {
		// If not erasing, we are drawing. Use current rect to redraw node.
		OnInvalidateRect(GetCurrentRect(), false);
	}

	// PreOrder traversal of all child nodes
	if (bApplyToChildren) {
		Children::iterator i;
		for (i = m_Children.begin(); i != m_Children.end(); ++i) {
			(*i)->PerformInvalidation(bDoErase, true);
		}
	}

	return true;
}



// This function can be called to rectify the Z-Order associated with this
// particular node. Derived classes will override this function to apply
// specific Z-Order implementation (note this need not necessarily be
// an hWnd operation - his can also refer to other images (like a ray-traced
// GDI image or the like)).
// Derived classes should conform to a preorder traversal, i.e. invoke the
// base class first before special processing. This will allow leaf nodes
// to be above the Z-Order from root nodes, as approp.
template <typename _LayoutIface>
void CLayoutNodeImpl<_LayoutIface>::RecalcZOrder (
)
{
	// Base class does nothing but delegate to children
	Children::iterator i;
	for (i = m_Children.begin(); i != m_Children.end(); ++i) {
		(*i)->RecalcZOrder();
	}
}


/////////////////////////////////////////////////////////////////////////////
// Protected Overridables
//

// This simple virtual callback is invoked by RecalcLayout to selectively accept/abort a recalc.
template <typename _LayoutIface>
bool CLayoutNodeImpl<_LayoutIface>::DoProcessRecalcLayout (
	bool bStrict,
	const CRect& rcDesired
)
{
	bool bDirty = m_bRecalcDirty;
	m_bRecalcDirty = false;
	CRect rcCurrent = GetCurrentRect();
	
	if ((bStrict) || (bDirty) || (!rcCurrent.EqualRect(&rcDesired))) {
		 return true;
	}
	else {
		return false;
	}
}


// Override this function to provide your own custom layout algorithm.
template <typename _LayoutIface>
CRect CLayoutNodeImpl<_LayoutIface>::OnRecalcLayout (
	const CRect& rcDesired,
	bool bChildNegotiate,
	bool bStrictRecalc
)
{

	// Base class implements the default layout algorithm: 
	// Child nodes maintain same position.

	// Set this node's rect, offering up to negotiation if non-strict
	CRect rcDesired2 = rcDesired;
	if (bChildNegotiate) {
		OnNegotiateNodeRect(rcDesired2);
	}

	SetCurrentRect(rcDesired2);

	int nChildNodes = GetChildrenCount();
	if(nChildNodes < 1) {
		return rcDesired2;
	}

	// Get initial rect info. This can be used to maintain constant positioning
	// of child windows.
	CRect rcInitial(0, 0, 0, 0);
	GetInitialRect(rcInitial);
	int nOffsetX = rcDesired2.left - rcInitial.left;
	int nOffsetY = rcDesired2.top - rcInitial.top;

	// recursively calculate all children nodes as well
	CRect rcNodeInitial(0, 0, 0, 0);
	CRect rcNodeDesired(0, 0, 0, 0);

	Children::iterator i;
	for (i = m_Children.begin(); i != m_Children.end(); ++i) {
		ILayoutNode* pNode = *i;
		// ASSERT(pNode);

		// Calc rectDesired to maintain constant positioning
		pNode->GetInitialRect(rcNodeInitial);
		rcNodeDesired.left = rcNodeInitial.left + nOffsetX;
		rcNodeDesired.right = rcNodeDesired.left + rcNodeInitial.Width();
		rcNodeDesired.top = rcNodeInitial.top + nOffsetY;
		rcNodeDesired.bottom = rcNodeDesired.top + rcNodeInitial.Height();

		// Now hand off the recal to child...
		pNode->RecalcLayout(rcNodeDesired, bChildNegotiate, bStrictRecalc);
	}

	return rcDesired2;
}


// Override default node sizing behavior based on set style flags
// Return false to abort recalc.
template <typename _LayoutIface>
void CLayoutNodeImpl<_LayoutIface>::OnApplyOverrides (
	const CRect& rcOld,
	CRect& rcNew
) 
{

	if (m_dwExStyle & NoSizeHorizontal) {
		rcNew.right = rcNew.left + rcOld.Width();
	}
	if (m_dwExStyle & NoSizeVertical) {
		rcNew.bottom = rcNew.top + rcOld.Height();
	}
	if (m_dwExStyle & NoMoveHorizontal) { 
		rcNew.left = rcOld.left;
	}
	if (m_dwExStyle & NoMoveVertical) {
		rcNew.top = rcOld.top;
	}
}


// Perform all node rectangle negotiations, including min/max sizing.
template <typename _LayoutIface>
void CLayoutNodeImpl<_LayoutIface>::OnNegotiateNodeRect (
	CRect& rcNode
)
{
	// Do we have a maximum size? If so, accomodate as needed.
	if (!(m_dwMinMaxFlags & NoMaxSize)) {
		if (rcNode.Width() > m_sizeMax.cx) {
		   rcNode.right = rcNode.left + m_sizeMax.cx;
		}
		if (rcNode.Height() > m_sizeMax.cy) {
		   rcNode.bottom = rcNode.top + m_sizeMax.cy;
		}
	}

	// Do we have a minimum size? If so, accomodate as needed.
	if (!(m_dwMinMaxFlags & NoMinSize)) {
		if (rcNode.Width() < m_sizeMin.cx) {
		   rcNode.right = rcNode.left + m_sizeMin.cx;
		}
		if (rcNode.Height() < m_sizeMin.cy) {
		   rcNode.bottom = rcNode.top + m_sizeMin.cy;
		}
	}
}



// Recursive invalidation helper
// If optimized redraw is set, we must manually iterate through all nodes,
// erasing and redrawing only mimimal necessary changes. Much smoother, but
// also more CPU intensive (and may not work properly for misbehaving nodes).
// Invalidate only those branches which are colored "dirty"
// Look for nodes with the invalidate dirty bit set. When found, hand
// off to PerformInvalidation for a clean 2-pass invalidation of all
// child nodes down that particular dirty branch (since a dirty parent
// necissitates an invalidation of all contained children).
template <typename _LayoutIface>
void CLayoutNodeImpl<_LayoutIface>::OptimizedRedraw (
)
{
	// Determine optimization state for this node
	bool bOptimized = (m_dwExStyle & OptimizeRedraw) != 0;
	if ((!bOptimized) || (!IsInvalidateDirty())) {
		// Either this node is clean, or optimization has been set for ignore.
		// In either case, we now want to continue searching the children
		// for a pending optimized invalidation.
		SetInvalidateDirty(false);				// clear in case not optimized set

		// Recurse children
		Children::iterator i;
		for (i = m_Children.begin(); i != m_Children.end(); ++i) {
			(*i)->OptimizedRedraw();
		}
	}
	else {
		// Perform invalidation in 2 passes: first make all needed erasures,
		// then all needed repaints (this insures a clean repaint).
		PerformInvalidation(true);		// erase all old children rects
		PerformInvalidation(false);		// repaint all new children rects

		// PerformInvalidation has traversed the remainder of this branch,
		// return now and recursively examine other branches.
	}		
}
//////////////////////////////////
// Protected Implementation
//


// If current rectangle has changed, determine between 0 and 4 rectangles
// outside the current rect that must be erased for proper (and clean)
// painting. 
template <typename _LayoutIface>
void CLayoutNodeImpl<_LayoutIface>::EraseInvalidRects (
)
{
	// ASSERT(m_dwExStyle & OptimizeRedraw);

	CRect rcInvalidate = GetInvalidateRect();
	CRect rcCurrent = GetCurrentRect();

	CRect rc;
	if (rcInvalidate.top < rcCurrent.top) {
		rc = rcInvalidate;
		rc.bottom = rcCurrent.top;
		OnInvalidateRect(rc, true);
	}
	if (rcInvalidate.left < rcCurrent.left) {
		rc = rcInvalidate;
		rc.right = rcCurrent.left;
		OnInvalidateRect(rc, true);
	}
	if (rcInvalidate.right > rcCurrent.right) {
		rc = rcInvalidate;
		rc.left = rcCurrent.right;
		OnInvalidateRect(rc, true);
	}
	if (rcInvalidate.bottom > rcCurrent.bottom) {
		rc = rcInvalidate;
		rc.top = rcCurrent.bottom;
		OnInvalidateRect(rc, true);
	}
}


template <typename _LayoutIface>
void CLayoutNodeImpl<_LayoutIface>::FitRectWithinNodeMinMax (
	CRect& rc
) const
{
	// Get sizing info
	CSize szMin, szMax;
	DWORD dwMinMaxFlags;
	GetMinMaxSize(szMin, szMax, dwMinMaxFlags);

	// And truncate within min/max
	if (!(dwMinMaxFlags & NoMaxSize)) 
	{
		if (rc.Width() > szMax.cx) {
			rc.right = rc.left + szMax.cx;
		}
		if (rc.Height() > szMax.cy) {
			rc.bottom = rc.top + szMax.cy;
		}
	}

	if (!(dwMinMaxFlags & NoMinSize)) {
		if (rc.Width() < szMin.cx) {
			rc.right = rc.left + szMin.cx;
		}
		if (rc.Height() < szMin.cy) {
			rc.bottom = rc.top + szMin.cy;
		}
	}
}



// Use this function to calculate the smallest rectangle to contain all of this node and all children nodes.
template <typename _LayoutIface>
void CLayoutNodeImpl<_LayoutIface>::CalcMinBoundingRect (
	CRect& rcBounding
)
{
	CRect rcNodeCurrent(0, 0, 0, 0);
	rcBounding.SetRectEmpty();

	// Iterate through top level nodes, calculating
	// the bounding rect for all. Since child nodes completely
	// contain grandchildren, 1 level will be deep enough.
	bool bInitialPass = true;
	Children::iterator i;
	for (i = m_Children.begin(); i != m_Children.end(); ++i) {
		ILayoutNode* pNode = *i;
		rcNodeCurrent = pNode->GetCurrentRect();

		// initialize with first rect
		if (bInitialPass) { 
			rcBounding = rcNodeCurrent;
			bInitialPass = false;
		}
		else {
			// size with additional nodes
			rcBounding.left = _SFLMIN(rcBounding.left, rcNodeCurrent.left);
			rcBounding.right = _SFLMAX(rcBounding.right, rcNodeCurrent.right);
			rcBounding.top = _SFLMIN(rcBounding.top, rcNodeCurrent.top);
			rcBounding.bottom = _SFLMAX(rcBounding.bottom, rcNodeCurrent.bottom);
		}
	}
}



/////////////////////////////////////////////////////////////////////
// CLayoutNode<> definition
//

#define BEGIN_WINDOW_EVENT_MAP(_class)	\
	virtual bool HandleEvent(IEvent* pIEvent) \
	{ \
		BOOL bHandled = FALSE;	\
		IWinEvent* pWinEvent = guid_cast<IWinEvent*>(pIEvent);	\
		if (!pWinEvent) {		\
			goto map_return;	\
		}	\


#define HANDLE_WINDOW_EVENT(message, member)	\
		if (pWinEvent->GetMessageID() == message) {	\
			UINT iMsg = pWinEvent->GetMessageID();	\
			WPARAM wParam = pWinEvent->GetWParam();	\
			LPARAM lParam = pWinEvent->GetLParam();	\
			bHandled = TRUE;	\
			LRESULT lResult = member(iMsg, wParam, lParam, bHandled);	\
			pWinEvent->SetLResult(lResult);	\
			goto map_return;	\
		}	\

#define END_WINDOW_EVENT_MAP()	\
	map_return:	\
	return bHandled? TRUE : FALSE;	\
	}	\


#define DECLARE_LAYOUT_FACTORY(_class)	\
		typedef CObjectFactory<ILayoutNode, _class> CNodeFactory;	\
		static ILayoutNode* CreateNode() {CNodeFactory factory; return factory.CreateObject();}	\
		static void DeleteNode(ILayoutNode* p) {CNodeFactory factory; factory.DestroyObject(p);}



template <typename _Derived, typename _LayoutIface> 
class CLayoutNode:
	public CLayoutNodeImpl<_LayoutIface>,
	public IEventRouter,
	public IEventListener
{
private:
	typedef CLayoutNode<_Derived, _LayoutIface> _thisClass;

public:
	typedef _thisClass _LayoutBase;
	typedef _LayoutIface LayoutInterface;

public:

	// ILayoutNode 
	virtual const GUID* GetGUID() const {return &__uuidof(_Derived);}

	DECLARE_LAYOUT_FACTORY(_Derived)


	// IRefCount
	// RefCounting is not implemented in Layout Nodes
	// Lifetime is controlled by the owner of the node tree, when the parent node is destroyed
	// it will destroy all its children through their class factories.

	ULONG STDMETHODCALLTYPE AddRef() {return 1;}
	ULONG STDMETHODCALLTYPE Release() {return 0;}

	// IEventRouter
	virtual bool RouteEvent(IEvent* pIEvent);
	// Listener list is handled through the composite pattern
	virtual bool AddListener(IEventListener* /*pIListener*/) {return false;}
	virtual bool RemoveListener(IEventListener* /*pIListener*/) {return false;}

	// IEventHandler
	BEGIN_WINDOW_EVENT_MAP(_thisClass)
	END_WINDOW_EVENT_MAP()


	BEGIN_GUID_MAP(_thisClass)
		GUID_ENTRY(ILayoutNode)
		GUID_ENTRY(_LayoutIface)
		GUID_ENTRY(IEventRouter)
		GUID_ENTRY(IEventListener)
		GUID_ENTRY(_Derived)
		GUID_ENTRY_IID(__uuidof(LayoutComposite), _compositeBase)
	END_GUID_MAP

};


template <typename _Derived, typename _LayoutIface> 
bool CLayoutNode<_Derived, _LayoutIface>::RouteEvent (
	IEvent* pIEvent
)
{
	Children::const_iterator i;

	bool bHandled = false;

	IEventListener* pHandler = guid_cast<IEventListener*>(this);
	if (pHandler) {
		bHandled = pHandler->HandleEvent(pIEvent);
	}

	if (!bHandled) {
		for (i = m_Children.begin(); (!bHandled) && (i != m_Children.end()); ++i) {
			IEventRouter* pRouter = guid_cast<IEventRouter*>(*i);
			if (pRouter) {
				bHandled = pRouter->RouteEvent(pIEvent);
			}
		}
	}

	return bHandled;
}





};	// namespace stingray::foundation
};	// namespace stingray

