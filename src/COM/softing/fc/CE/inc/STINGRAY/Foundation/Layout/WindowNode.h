/////////////////////////////////////////////////////////////////////////////
// WindowNd.h : SFL Layout Manager
//
// Stingray Software Windows Template Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Gilberto Araya
// Description:  SFL Layout Manager: Window layout node
//


#pragma once

#if !defined(_SFL)
#define _SFL
#endif


#include <foundation\Layout\LayoutNode.h>


namespace stingray {
namespace foundation {

//@doc


//@class CWindowLayoutNode |
// Implements a reactive node associated with a child window. 
// Layout operations are translated to Windows API calls to move or resize the
// associated window.
//@base public | CLayoutNode<lt>ILayoutNode<gt>
//@supint ILayoutNode | Basic layout node interface.
class __declspec(uuid("8B1647C2-45E5-11d3-9D92-00C04F91E286")) CWindowLayoutNode : 
	public CLayoutNode<CWindowLayoutNode, ILayoutNode> 
{
public:
	// Initialization/Destruction
	CWindowLayoutNode()
		: m_hNodeWnd(NULL),	m_hParentWnd(NULL), m_bIsActiveX(false) {}
	virtual ~CWindowLayoutNode() {}

	// ILayoutNode interface

	virtual bool IsNodeAttached() const
	{return (m_hNodeWnd != 0);}

	virtual long GetNodeId() const
	{
		return ::GetWindowLong(m_hNodeWnd, GWL_ID);
	}

	virtual int GetDeferWindowCount() const;

	virtual void Init(HWND hwndParent, HWND hwndChild = 0);
	virtual void Show(bool bShow = true);

protected:
	// Overridables	
	// Window invalidation is about to occur
	virtual void OnInvalidateRect(const CRect& rectInvalid, bool bDoErase);
	// Operations
	virtual bool OnRealizeNode(CLayoutTransaction& aTransaction);

protected:

	// Implementation Member Variables
	HWND m_hNodeWnd, m_hParentWnd;
	bool m_bIsActiveX;

};


};	// namespace stingray::foundation
};	// namespace stingray

