/////////////////////////////////////////////////////////////////////////////
// ViewportNode.h : SFL Layout Manager
//
// Stingray Software Windows Template Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Gilberto Araya
// Description:  SFL Layout Manager: MVC Viewport layout node
//


#pragma once

#if !defined(_SFL)
#define _SFL
#endif

#ifndef _SFL_NO_PRAGMA_WARNINGS
	#if 1200 <= _MSC_VER
		#pragma warning(push,3)
	#endif
	#pragma warning(disable:4097)
#endif

#include <foundation\Layout\LayoutNode.h>
#include <foundation\Layout\DCNode.h>

#include <foundation\String\sflstring.h>
#include <foundation\StructuredGraphics\Visual.h>


namespace stingray {
namespace foundation {

//@doc


//@interface IViewportNode |
//  Interface that helps integrate SFL's layout manager with the Model-View-Controller (MVC)
//  architecture. 
//@base | ILayoutNode
interface __declspec(uuid("0B3556B0-6171-11d3-9D9A-00C04F91E286")) IViewportNode:
	public ILayoutNode
{
	//@meth,method void | Init |
	// Initializes a viewport layout node. Unlike the version of Init() 
	// found in ILayoutNode, it receives in its second parameter a pointer
	// to the IVisual interface implemented by the viewport. 
	//@@syntax pVPNode->Init(hwndParent, pVP);
	//@@parm HWND | hwndParent | Window associated to the entire layout manager structure.
	//@@parm IVisual* | pVP | Pointer to the IVisual interface implemented by the viewport.
	//@@comm
	// After this method returns, the layout node is permanently associated with the
	// viewport pointed by pVP. If this viewport gets destroyed by some means other than
	// upon the window destruction, you need to manually destroy this layout node.
	virtual void Init(HWND hwndParent, foundation::IVisual* pVP) = 0;
};


//@class CViewportNode |
// Implementation of the IViewportNode interface.<nl>
// Provides integration of the Layout Manager service with the
// MVC architecture, by allowing MVC windowless viewports to participate
// in layout operations.<nl>
//@base public | CDCLayoutBase<lt>CViewportNode, IViewportNode<gt>
//@comm 
// There are two types of viewports in MVC: windowed viewports and windowless
// viewports. The former are implemented as real windows, so they can be attached
// to standard window layout nodes. Windowless viewports draw directly on the area 
// of their master window, they do not have a window of their own. This characteristic
// is very similar to the functionality <c CDCLayoutBase> offers in the context
// of the layout manager. CViewportNode specializes CDCLayoutBase by delegating the
// painting processes to the corresponding IVisual methods, and performing the appropiate
// steps in RealizeNode to assign the new area to the viewport.
class __declspec(uuid("0B3556B1-6171-11d3-9D9A-00C04F91E286")) CViewportNode:
	public CDCLayoutBase<CViewportNode, IViewportNode>
{
public:
	typedef CDCLayoutBase<CViewportNode, IViewportNode> _LayoutBase;

	CViewportNode(): m_pVP(0) {}

public:
	virtual void Init(HWND hwndParent, HWND hwndChild = 0);
	virtual bool OnRealizeNode(CLayoutTransaction& aTransaction);

	virtual void Init(HWND hwndParent, foundation::IVisual* pVP);

protected:
	// CDCLayoutBase overrides

	virtual void OnPaint(HDC hDC = NULL);
	virtual bool PrepareDC(HDC);
	virtual void RestoreDC(HDC);

private:
	IVisual* m_pVP;

};



};
};


#ifndef _SFL_NO_PRAGMA_WARNINGS
	#if 1200 <= _MSC_VER
		#pragma warning (pop)
	#endif
#endif