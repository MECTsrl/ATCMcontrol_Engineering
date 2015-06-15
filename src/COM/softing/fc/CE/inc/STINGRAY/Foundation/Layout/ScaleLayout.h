/////////////////////////////////////////////////////////////////////////////
// ScaleLayout.h : SFL Layout Manager
//
// Stingray Software Windows Template Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Gilberto Araya
// Description:  SFL Layout Manager: Scale layout algorithm
//


#pragma once

#if !defined(_SFL)
#define _SFL
#endif


#include <foundation\Layout\LayoutNode.h>


namespace stingray {
namespace foundation {

//@doc


//@class CScaleLayout |
/* Implementation of the Scale layout algorithm.<nl>
   The Scale layout maintains all children with a constant aspect ratio to the parent scale node. 
   In other words, the child node's top, left, right, and bottom coordinates are stored as percentages 
   of the parent node's size and are resolved to actual pixel values with each recalculation.
   This guarantees a constant aspect ratio regardless of the size of the parent node.
*/
//@base public | CLayoutNode<lt>CScaleLayout, ILayoutNode<gt>
//@supint ILayoutNode | Basic layout node interface.
class __declspec(uuid("8B1647C1-45E5-11d3-9D92-00C04F91E286")) CScaleLayout: 
	public CLayoutNode<CScaleLayout, ILayoutNode>
{
public:
	// Initialization/Destruction
	CScaleLayout() {}
	virtual ~CScaleLayout() {}


protected:
	// ILayoutNode interface
	virtual CRect OnRecalcLayout(const CRect& rectDesired,
								 bool bChildNegotiate, bool bStrictRecalc);

protected:
	// Implementation

	virtual CRect CalcNewChildNodeRect(const CRect& rectNodeInitial,
								  	   const CRect& rectNodeNew,
								  	   const CRect& rectChildInitial);
};




};

};