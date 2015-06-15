/////////////////////////////////////////////////////////////////////////////
// LayoutBase.h : SFL Layout Manager 
//
// Stingray Software Windows Template Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Gilberto Araya
// Description:  SFL Layout Manager: Layout Node interface
//


#pragma once
#pragma warning(disable:4786)

#if !defined(_SFL)
#define _SFL
#endif


#include <objbase.h>
#include <Foundation\SflCommon.h>
#if !defined(_SFL_MFC_SUPPORT)
#include <Foundation\APIStructs\GCUtil.h>
#endif
#include <Foundation\Patterns\Iterator.h>


namespace stingray {
namespace foundation {


//@doc


// Layout Node Styles

//@enum Flags for ILayoutNode Get/SetMinMaxSize operation
enum MinMaxSize {
	NoMinSize = 0x0001,		//@emem Minimum size value not to be used
	NoMaxSize = 0x0002		//@emem Maximum size value not to be used
};

//@enum Generic layout node extended styles
enum ExtendedStyles {
	// Default extended styles:
	Defaults = 0,					//@emem no optimizations by default
	// Node size/movement restrictions
	NoSizeHorizontal = 0x0001,		//@emem Fixed width
	NoSizeVertical   = 0x0002,		//@emem Fixed height
	NoMoveHorizontal = 0x0004,		//@emem Fixed horizontal position
	NoMoveVertical   = 0x0008,		//@emem Fixed vertical position
	// Optimized redraw:
	OptimizeRedraw   = 0x0010		//@emem Optimize redraw operation
};


//////////////////////////////////////////////////////
// Interface ILayoutNode

struct CLayoutTransaction
{
	CLayoutTransaction(HDC hdc = NULL): hDWPOperation(0), hdcOutput(hdc) {}
	~CLayoutTransaction() {}

	void Init(int nNumWindows)
	{
		hDWPOperation = ::BeginDeferWindowPos(nNumWindows);
	}

	void Commit()
	{
		::EndDeferWindowPos(hDWPOperation);
		hDWPOperation = 0;
	}

	void DeferWindowPos (HWND hWnd, HWND hWndInsertAfter, CRect rcPos, UINT uFlags)
	{
		hDWPOperation = ::DeferWindowPos(hDWPOperation, hWnd, hWndInsertAfter, rcPos.left, rcPos.top, rcPos.Width(), rcPos.Height(), uFlags);
	}


	bool IsValid() const {return hDWPOperation != 0;}

	HDC GetDC() const {return hdcOutput;}
	HDC SetDC(HDC hdcNew) {HDC hdcOld = hdcOutput; hdcOutput = hdcNew; return hdcOld;}
	operator HDWP () {return hDWPOperation;}

protected:
	HDWP  hDWPOperation;
	HDC	  hdcOutput;
};



//////////////////////////////////////////////////////
// Interface ILayoutNode


//@interface ILayoutNode | 
// Basic functionality implemented by all types of Layout Nodes.
// Layout nodes are expected to realize this interface.
// Specific layout node classes may specialize this interface to provide
// additional functionality.
interface _declspec(uuid("8B1647C0-45E5-11d3-9D92-00C04F91E286")) ILayoutNode:
	public IQueryGuid
{

	typedef foundation::IteratorBase<ILayoutNode*> Iterator;

	// Composite pattern operations
	

	//@meth,method 
	// Returns the number of child nodes this node has.
	virtual int GetChildrenCount (bool bRecursive = false) const = 0;
	//@meth,method
	// Returns this node's parent node.
	virtual ILayoutNode* GetParentNode() = 0;
	//@meth,method
	// Set this node's parent node.
	virtual void SetParentNode(ILayoutNode* pNode) = 0;
	//@meth,method
	// Returns the root node of the layout tree.
	virtual ILayoutNode* GetRootParentNode() = 0;
	//@meth,method
	// Adds the layout node pointed by pNode to the list
	// of this node's children.
	virtual int AddLayoutNode(ILayoutNode* pNode) = 0;
	//@meth,method 
	// Removes the layout node pointed by pNode from the list
	// of this node's children.
	virtual int RemoveLayoutNode(ILayoutNode* pNode) = 0;
	//@meth,method
	// Returns an iterator to traverse the collection of 
	// child nodes.
	virtual Iterator* GetChildNodesIterator() = 0;

	//@meth,method ILayoutNode* | FindChildNode | 
	// Return the first child node whose Id is equal to the
	// parameter nId.
	virtual ILayoutNode* FindChildNode(long nId) = 0;

	//@prop,property GUID* | GetGUID | 
	// Returns a GUID that identifies the exact class
	// this object is an instance of.
	//@@syntax guid = GetGUID();
	virtual const GUID* GetGUID() const = 0;

	// Accessors

	//@meth,method Returns the node's visibility state
	//@@rdesc If the return value is true, the visible element associated 
	//	with this layout node should be displayed. If false, the element
	//	is hidden, so it will not be displayed even if its designated area
	//	is visible on the screen.
	virtual bool IsVisible() const = 0;

	//@prop,property CSize,CSize| Get/SetMinMaxSize |
	// Gets/Sets the minimum/maximum node size information.
	//@@syntax pNode->GetMinMaxSize(sizeMinimum, sizeMaximum, dwFlags);
	//@@syntax pNode->SetMinMaxSize(sizeMinimum, sizeMaximum, dwFlags);
	//@@parm CSize | sizeMinimum | Minimum dimensions this node is allowed to take.
	//@@parm CSize | sizeMaximum | Maximum dimensions this node can grow to.
	//@@parm DWORD | dwFlags | Specify whether either of the previous parameters has to be ignored. 
	// Use a value from the <t MinMaxSize> enumerated type.
	virtual void GetMinMaxSize(CSize& sizeMin, CSize& sizeMax, DWORD& dwFlags) const = 0; 
	virtual void SetMinMaxSize(const CSize& sizeMin, const CSize& sizeMax, DWORD dwFlags = 0) = 0;

	//@prop,property bool | SetRecalcDirty |
	// Marks as clean/dirty for position recalculation.
	//@@syntax pNode->SetRecalcDirty(bDirty[, bAncestorsDirty]);
	//@@parm bool | bDirty | Specifies whether this node needs a recalc operation to be performed.
	//@@parm bool | bAncestorsDirty | Specifies whether the operations must be recursively applied to the
	// node's ancestors. Default is false, meaning that the operation only has effect on this node.
	virtual void SetRecalcDirty(bool bDirty = true, bool bSetAncestorsDirty = false) = 0;

	//@prop,property bool | Set/IsInvalidateDirty |
	// Returns or sets whether this node needs an invalidation operation to be performed.
	//@@syntax bDirty = pNode->IsInvalidateDirty();
	//@@syntax pNode->SeInvalidateDirty(bDirty);
	//@@parm bool | bDirty | Indicates the need of this node to be invalidated.
	virtual bool IsInvalidateDirty() const = 0;
	virtual void SetInvalidateDirty(bool bDirty = true) = 0;

	//@prop,property DWORD | Get/Set/ModifyNodeExStyle |
	// Retrieves or sets the extended style of the node.
	//@@syntax dwStyle = pNode->GetNodeExStyle();
	//@@syntax pNode->SetNodeExStyle(dwStyle, [bApplyToChildren]);
	//@@syntax pNode->ModifyNodeStyleEx(dwStylesToRemove, dwStylesToAdd[, bApplyToChildren]);
	//@@parm DWORD | dwExStyle | Mask that specifies the styles assigned to the node. Should be a
	//	combination of values from the <t ExtendedStyles> enumeration using the logical OR operator.
	//@@parm bool | bApplyToChildren | Specifies whether the Set or Modify operation should be
	//	recursively applied to all descendents of this node. Default is false.
	//@@parm DWORD | dwStylesToRemove | Specifies a set of styles to be removed from the node's style mask. Should be a
	//	combination of values from the <t ExtendedStyles> enumeration using the logical OR operator.
	//@@parm DWORD | dwStylesToAdd| Specifies a set of styles to be removed from the node's style mask. Should be a
	//	combination of values from the <t ExtendedStyles> enumeration using the logical OR operator.
	//@@comm A node's extended styles allow the programmer to configure certain aspects of how the layout node will behave.<nl>
	// The ModifyNodeStyleEx() function allows removing and adding some styles in a single atomic operation, without having
	// to combine manually the values of the new styles with the existing mask.
	virtual DWORD GetNodeExStyle() const = 0;
	virtual void SetNodeExStyle(DWORD dwExStyle = 0, bool bApplyToChildren = false) = 0;
	virtual void ModifyNodeStyleEx(DWORD dwRemove = 0, DWORD dwAdd = 0, bool bApplyToChildren = false) = 0;

	//@prop,property bool | Get/SetLockRealizeNode |
	// Prevents the RealizeNode operation from being processed in this node.
	// Useful for redraw optimizations.
	//@@syntax bLock = pNode->GetLockRealizeNode();
	//@@syntax pNode->SetLockRealizeNode(bLock[, bApplyToChildren]);
	//@@parm bool | bLock | Value of the lock attribute.
	//@@parm bool | bApplyToChildren | Specifies whether the Set operation should be
	//	recursively applied to all descendents of this node. Default is true.
	virtual bool GetLockRealizeNode() const = 0;
	virtual void SetLockRealizeNode(bool bLock = true, bool bApplyToChildren = true) = 0;

	//@meth,method int | GetDeferWindowCount |
	// Queries the number of child items to be drawn by a DeferWindowPos call in RealizeNode
	//@@rdesc Number of window nodes in the descendents of this node.
	//@@comm Layout nodes associated with windows often must be treated differently from other nodes
	// due to their particular invalidation and redrawing characteristics. This method allows us to 
	// know how many of such nodes are in the subtree that has this node as a root.
	virtual int GetDeferWindowCount() const = 0;

	//@prop,property bool | IsNodeAttached |
	// Returns whether this node is attacched to a realizable screen element,
	// e.g. a window.
	//@@rdesc true if the node is attached, false otherwise.
	virtual bool IsNodeAttached() const = 0;
	//@prop,property long | GetNodeId | 
	// Returns the Id of the element associated with this node.
	//@@rdesc Id of the element associated with the node. For example, in the case
	// of window layout nodes, this should be the window Id.
	virtual long GetNodeId() const = 0;

	//@prop,property int,int | Get/SetNonClientOffset | 
	// Returns or sets the offset value for positioning nodes relative to a nonclient area
	//@@syntax pNode->GetNonClientOffset(nLeft, nTop);
	//@@syntax pNode->SetNonClientOffset(nLeft, nTop);
	//@@parm int | nLeft | x-coordinate of the TopLeft corner of the node's rectangle.
	//@@parm int | nTop | y-coordinate of the TopLeft corner of the node's rectangle.
	virtual void GetNonClientOffset(int& nLeft, int& nTop) const = 0;
	virtual void SetNonClientOffset(int nLeft, int nTop)  = 0;

	//@prop,property bool | Get/SetNodeHorizontal |
	// Sets the base node orientation setting. 
	//@@syntax bHorz = pNode->GetNodeHorizontal();
	//@@syntax pNode->SetNodeHorizontal([bHorz[, bApplyToChildren]]);
	//@@parm bool | bHorz | If true, this node's orientation is horizontal, vertical otherwise.
	//@@parm bool | bApplyToChildren | Specifies whether the Set operation should be
	//	recursively applied to all descendents of this node. Default is false.
	virtual bool GetNodeHorizontal() const = 0;
	virtual void SetNodeHorizontal(bool bHorz = true, bool bApplyToChildren = false) = 0;


	// Rectangle manipulation/queries

	//@prop,property CRect | Get/SetInitialRect |
	// Retrieves or sets the rectangle initially occupied by the node.
	//@@syntax pNode->GetInitialRect(rcInitial);
	//@@syntax pNode->SetInitialRect(rcInitial);
	//@@parm CRect | rcInitial | Initial rectangle assigned to this layout node.
	virtual void GetInitialRect(CRect& rectInitial) const = 0;
	virtual void SetInitialRect(const CRect& rectInitial = CRect()) = 0;

	//@prop,property CRect | Get/SetInitialRect |
	// Retrieves or sets the rectangle the node would prefer to occupy.
	//@@syntax pNode->GetPreferredRect(rcPreferred);
	//@@syntax pNode->SetPreferredRect(rcPreferred);
	//@@parm CRect | rcPreferred | Node's preference regarding the area of the screen it would like
	//	to occupy. 
	//@@comm This property contains the node's preference on the area it occupies on the screen.
	// The parent node might take this information in consideration when calculating, although it
	// is not required to do so; and even if it does it is not guaranteed to respect the node's wishes.
	virtual void GetPreferredRect(CRect& rectPreferred) const = 0;
	virtual void SetPreferredRect(const CRect& rectPreferred) = 0;

	//@prop,property CRect | Get/SetCurrentRect |
	// Retrieves or sets the rectangular area the node is currently responsible for.
	//@@syntax rcCurrent = pNode->GetCurrentRect();
	//@@syntax rcCurrent = pNode->GetCurrentRectWithinMinMax();
	//@@syntax pNode->SetCurrentRect(rcCurrent);
	//@@parm CRect | rcCurrent | Rectangular area of the screen assigned to this layout node.
	//@@comm Every layout node is assigned a rectangular area within the boundaries of its parent's
	// area. The node is responsible for laying out all displayable elements that are participating in
	// the layout operation and are located within that area. <nl>
	// The GetCurrentWithinMinMax() operation retrieves the current rectangle constrained to the minimum
	// and maximum sizes specified to the node using a <mf ILayoutNode::Get/SetMinMaxSize()> call.
	virtual CRect GetCurrentRect() const = 0;
	virtual CRect GetCurrentRectWithinMinMax() const = 0;
	virtual void SetCurrentRect(const CRect& rectCurrent) = 0;

	//@prop,property CRect | Get/SetOldRect |
	// Retrieves or sets the rectangular area the node used to occupy before the previous RecalcLayout() operation.
	//@@syntax rcOld = pNode->GetOldRect();
	//@@syntax pNode->SetOldRect(rcOld);
	//@@parm CRect | rcOld | Rectangular area of the screen assigned previously to this layout node.
	//@@comm The value of this property is only valid in the meantime between the Recalculation and the Realization
	// processes. At any other moment, its value is the current rectangle.
	virtual CRect GetOldRect() const = 0;
	virtual void SetOldRect(const CRect& rectOld) = 0;

	//@prop,property CRect | Get/SetInvalidateRect |
	// Retrieves or sets the rectangular area the node requests to be invalidated for the next drawing operation.
	//@@syntax rcInvalid = pNode->GetInvalidateRect();
	//@@syntax pNode->SetInvalidateRect(rcInvalid);
	//@@parm CRect | rcInvalid | Rectangular area within the node's current rectangle whose display needs to be updated.
	virtual CRect GetInvalidateRect() const = 0;
	virtual void SetInvalidateRect(const CRect& rectInv) = 0;

	//@meth,method void | FitRectWithinNodeMinMax |
	// Constrains the passed rectangle to the maximum and minimum dimensions specified
	// for the node using a call to <mf ILayoutNode::Get/SetMinMaxSize()>.
	//@@parm CRect | rc | Rectangle to constrain. The rectangle is adjusted only if it is larger
	// than the specified maximum dimensions; or if it is smaller than the minimum.
	virtual void FitRectWithinNodeMinMax(CRect& rc) const = 0;


	// Operations
	//@meth,method void | Init | 
	// Initializes this layout node.
	//@@parm HWND | hwndParent | Handle to the master window associated with the entire layout operation.
	//@@parm HWND | hwndChild | Handle to a child window associated specificly with this node. Valid only in the case
	// of window layout nodes. Default is NULL;
	//@@comm The node is initialized by assigning the master window associated with the
	// entire layout operation. Optionally, a child window can be also associated with this 
	// specific node. Initialization is a required process for every layout node before it can participate in any layout recalculation.
	virtual void Init(HWND hwndParent, HWND hwndChild = 0) = 0;

	//@meth,method void | Show |
	// Shows or hides the visible elements associated with the node.
	//@@parm bool | bShow | Node's new visibility state. Default is true, meaning that the node is visible.
	//@@comm Execution of this operation affects the visibility state returned by the <mf ILayoutNode::IsVisible> property.
	virtual void Show(bool bShow = true) = 0;

	//@meth,method CRect | RecalcLayout | 
	// Performs the layout recalculation operation
	//@@rdesc New rectangle as solicited by the node. If this is an strict recalculation, the return value must be
	//	equal to the rcDesired parameter.
	//@@parm CRect | rcDesired | Rectangle being assigned
	//@@parm bool | bChildNegotiate | Enables negotiation capabilities for the child. Some laout algorithms may use 
	//	a two-steps recalculation process with a first step of negotiation. Here, the child nodes have the opportunity to
	//	suggest to their parent node the rectangle they want associated to them.
	//@@parm bool | bStrictRecalc | If true, the child must comply with the rectangle passed by the parent node. 
	//	The child node's suggestions will be ignored.
	//@@comm The new rectangle returned will not be commited
	// until after the RealizeNode operation. 
	virtual CRect RecalcLayout(const CRect& rectDesired,
							   bool bChildNegotiate = true,
							   bool bStrictRecalc = false) = 0;

	//@meth,method bool | RealizeNode |
	// Realizes the changes in position and size of the layout node as calculated in a recent 
	// execution of the <mf ILayoutNode::RecalcLayout> operation
	//@@rdesc true if the operation was successful, false otherwise.
	//@@parm CLayoutTransaction | aTransaction | 
	// A CLayoutTransaction structure contains information that needs to be shared amog all
	// layout nodes participating in a Realize operation, for practical or optimization purposes.
	// The default for this parameter is a transaction initialized by default, which allocates a new device context
	// and starts a DeferWindowPos transaction.
	virtual bool RealizeNode(CLayoutTransaction& aTransaction) = 0;

	//@meth,method void | OptimizedRedraw | 
	// Optimized drawing. Called at the end of the realization process to perform
	// any painting logic that have been deferred until the last moment.
	// Descendent layout classes should implement this operation if it is justified 
	// to prevent flicker or other painting problems.
	virtual void OptimizedRedraw() = 0;
	//@meth,method bool | PerformInvalidation |
	// Invalidates the area indicated by the node's invalid rectangle.
	//@@rdesc true if operation was succesful, false otherwise.
	//@@parm bool | bDoErase | Erase the background before the paint operation.
	//@@parm bool | bApplyToChildren | Specifies whether the operation should be
	//	recursively applied to all descendents of this node. Default is true.
	virtual bool PerformInvalidation(bool bDoErase, bool bApplyToChildren = true) = 0;
	//@meth,method bool | EraseNodeBkgnd |
	//	Erases the background of the rectangular area assigned to the node.
	//	Usually called as part of the processing of the WM_ERASEBKGND windows message.
	//@@syntax pNode->EraseNodeBkgnd(hDC[, bApplyToChildren]);
	//@@rdesc true if operation was succesful, false otherwise.
	//@@parm HDC | hDC | Device context
	//@@parm bool | bApplyToChildren | Specifies whether the operation should be
	//	recursively applied to all descendents of this node. Default is true.
	virtual bool EraseNodeBkgnd(HDC hDC = NULL, bool bApplyToChildren = true) = 0;
	//@meth,method void | PaintNode | 
	// Performs all necessary display operations. Usually called as part of the response
	// to the WM_PAINT message.
	//@@parm HDC | hDC | Device context
	//@@parm bool | bApplyToChildren | Specifies whether the operation should be
	//	recursively applied to all descendents of this node. Default is true.
	virtual void PaintNode(HDC hDC = NULL, bool bApplyToChildren = true) = 0;

	// Recalculates the Z-Order for this node as appropriate.
	virtual void RecalcZOrder() = 0;

};



};	// namespace stingray::foundation
};	// namespace stingray

