/////////////////////////////////////////////////////////////////////////////
// BorderClient.h : SFL Layout Manager
//
// Stingray Software Windows Template Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Gilberto Araya
// Description:  SFL Layout Manager Border Client algorithm: generic frame layout
//


#pragma once

#if !defined(_SFL)
#define _SFL
#endif


#include <foundation\Layout\LayoutNode.h>



namespace stingray {
namespace foundation {


//@doc

//////////////////////////////////////////////////////////////////////////////
// IBorderClientLayout interface


//@interface IBorderClientLayout |
// Support for the special layout arrangement often encountered in frame windows,
// where there are four special areas attached to the borders of the window called the
// borders, and a space between them called the client area.<nl>
// IBorderClient interface provides the necessary additional functionality to implement
// such arrangement.
//@base public | ILayoutNode
interface __declspec(uuid("C86E1620-5EDD-11d3-9D9A-00C04F91E286")) IBorderClientLayout:
	public ILayoutNode
{
public:

	//@cmember,menum | BorderPosition | |
	// Identify the specific areas designated in the 
	// Border-Client arrangement.
	enum BorderPosition 
	{ 
		Top = 0x01,		//@@emem Top border
		Bottom = 0x02,	//@@emem Bottom border
		Left = 0x04,	//@@emem Left border
		Right = 0x08,	//@@emem Right border
		Client = 0x10	//@@emem Client area
	};

	//@cmember,menum | BorderAlgorithm | |
	// Defines whether the horizontal borders take precedence
	// over the vertical ones or vice-versa.
	enum BorderAlgorithm 
	{
		TopBottomLeftRight,		//@@emem Horizontal borders get priority, then vertical
		LeftRightTopBottom		//@@emem Vertical borders get priority, then horizontal
	};

public:

	//@meth,method int | AddLayoutNode | 
	// Adds a child layout node to this node's children list.
	//@@rdesc Returns the new number of child nodes in this node's children list.
	//@@parm ILayoutNode* | pNode | Node being added.
	//@@parm BorderPosition | position | Area of the Border-Client arrangement the new node will be responsible for.
	//  Must be a member of the BorderPosition enumerated type.
	//@@comm This is a variation of the <mf ILayoutNode::AddLayoutNode> method, that takes a second
	// parameter specifying the area the node being aggregated will occupy. The
	// <mf ILayoutNode::AddLayoutNode> version should not be used with BorderClient nodes,
	// as it does not allow specifying this important piece of information.
	virtual int AddLayoutNode(ILayoutNode* pNode, BorderPosition position) = 0;

	//@prop,property BorderAlgorithm | Get/SetBorderAlgorithm | 
	//  Defines whether the horizontal borders take precedence
	//  over the vertical ones or vice-versa.
	virtual void SetBorderAlgorithm(BorderAlgorithm alg) = 0;
	virtual BorderAlgorithm GetBorderAlgorithm() const = 0;

	//@meth,method bool | LocatePositionFromNode |
	//  Given a layout node, returns the position that node is currently occupying
	//  within the Border-Client designated areas.
	//@@rdesc true if pNode was occupying one of the five designated areas within the Border-Client
	// arrangement. false if the node was not found.
	//@@parm ILayoutNode* | pNode | Node to locate position of.
	//@@parm BorderPosition& | position | Output parameter, which upon return contains the position
	// pNode is located at. Valid only if the return value is true.
	virtual bool LocatePositionFromNode(ILayoutNode* pNode, BorderPosition& position) const = 0;

	//@meth,method ILayoutNode* | LocateNodeFromPosition |
	//  Returns the layout node located at the position indicated.
	//@@rdesc Pointer to the node located at the area indicated by the position parameter. NULL if there
	// is no node assigned to that area.
	//@@parm BorderPosition | position | Area being queried.
	virtual ILayoutNode* LocateNodeFromPosition(BorderPosition position) const = 0;
};



//////////////////////////////////////////////////////////////////////////////
// CBorderClientLayout class


//@class CBorderClientLayout |
/* 
   Implementation of the Border-Client layout arrangement.<nl>
   The Border-Client Layout implements the typical arrangement found in frame windows,
   where there are four designated areas attached to each border of the window, where items 
   like toolbars and status bars are usually placed; and a rectangular space between these 
   "borders" generally called the "client" area.
*/
//@base public | CLayoutNode<lt>IBorderClientLayout <gt>
//@supint ILayoutNode | Basic layout node interface.
//@supint IBorderClientLayout | Specialized functionality for management of the Border-Client arrangement.
class __declspec(uuid("C86E1621-5EDD-11d3-9D9A-00C04F91E286")) CBorderClientLayout:
	public CLayoutNode<CBorderClientLayout, IBorderClientLayout>
{
public:
	CBorderClientLayout(): 
	m_pNodeTop(NULL), m_pNodeBottom(NULL), 
	m_pNodeLeft(NULL), m_pNodeRight(NULL),
	m_pNodeClient(NULL),
	m_BorderAlg(TopBottomLeftRight)
	{}


public:
	
	// IBorderClientLayout interface

	virtual int AddLayoutNode(ILayoutNode* pNode, BorderPosition position);

	void SetBorderAlgorithm(BorderAlgorithm alg) 	{ m_BorderAlg = alg; }
	BorderAlgorithm GetBorderAlgorithm() const		{ return m_BorderAlg; }

	virtual bool LocatePositionFromNode(ILayoutNode* pNode, BorderPosition& position) const;
	virtual ILayoutNode* LocateNodeFromPosition(BorderPosition position) const;


	// ILayoutNode interface
	virtual int AddLayoutNode (ILayoutNode* pNode);
	virtual int RemoveLayoutNode(ILayoutNode* pNode);
	virtual CRect OnRecalcLayout(const CRect& rcDesired, bool bChildNegotiate, bool bStrictRecalc);


protected:
	ILayoutNode* m_pNodeTop;
	ILayoutNode* m_pNodeBottom;
	ILayoutNode* m_pNodeLeft;
	ILayoutNode* m_pNodeRight;
	ILayoutNode* m_pNodeClient;

	BorderAlgorithm m_BorderAlg;

protected:

	// Implementation helpers
	BorderPosition GetAutoInsertPosition() const;

	// Layout Calculation Logic
	virtual void ApplyChildNodeRects(const CRect& rcClient, const CRect& rcTop, const CRect& rcBottom,
								const CRect& rcLeft, const CRect& rcRight);
	virtual CRect RecalcBorderRects(const CRect& rcParent, CRect& rcTop, CRect& rcBottom, CRect& rcLeft, CRect& rcRight);
	virtual CRect CalcClientRect(CRect rcTop, CRect rcBottom, CRect rcLeft, CRect rcRight) const;
	virtual void GetBorderRects(CRect& rcTop, CRect& rcBottom, CRect& rcLeft, CRect& rcRight) const;
	virtual void GetAdjustedBorderRect(ILayoutNode* pNode, CRect& rcNode) const;

	virtual bool SetClientRectWithinMinMax(const CRect& rcParent, CRect& rcClient,
								CRect& rcTop, CRect& rcBottom, CRect& rcLeft, CRect& rcRight);
	virtual void AdjustClientRectOneDimension(ILayoutNode* pBorderBefore, ILayoutNode* pBorderAfter,
								const CRect& rcParent, CRect& rcClient, int nDelta, bool bHorzAdjust);
	virtual void ResetNodeRectsAroundClient(const CRect& rcClient, CRect& rcTop, CRect& rcBottom,
								CRect& rcLeft, CRect& rcRight);

	virtual bool NegotiatePreferredChildNodeRects(const CRect& rcParent, CRect& rcTop, CRect& rcBottom,
								CRect& rcLeft, CRect& rcRight);

};



};	// namespace stingray::foundation
};	// namespace stingray


