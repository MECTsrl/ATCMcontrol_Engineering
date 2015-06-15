/////////////////////////////////////////////////////////////////////////////
// TransRect.h : Declaration of CTransposableRect
//
// Stingray Software Windows Template Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Mark Isham, Gilberto Araya
// Description:  SFL Transposable Rect
//


#pragma once

#if !defined(_SFL)
#define _SFL
#endif


#include <foundation\APIStructs\gcutil.h>


namespace stingray {
namespace foundation {


/////////////////////////////////////////////////////////////////////////////
// CTransposableRect


class CTransposableRect : public CRect {
public:
	// Construction/Initialization
	CTransposableRect(bool bHorz = true): m_bHorz(bHorz) {}
	CTransposableRect(const RECT& srcRect, bool bHorz = true): CRect(srcRect), m_bHorz(bHorz) {}

	const CTransposableRect& operator=(const CTransposableRect& rhs)
	{
		if (&rhs != this) {
			left   = rhs.left;
			right  = rhs.right;
			top    = rhs.top;
			bottom = rhs.bottom;
			m_bHorz = rhs.m_bHorz;
		}
		return *this;
	}

	// Set the implicit orientation of this rectangle
	void SetHorzOrient(bool bHorz = true) {m_bHorz=bHorz;}


	// Orientation-neutral functions

	// Set left/top or right/bottom based on set orientation.
	// If horz, primary LT is left, primary RB is right,
	// secondary TL is top, off BR is bottom. Reversed for vertical.
	void LT(int nLeftOrTop) {if (m_bHorz) left = nLeftOrTop; else top = nLeftOrTop;}
	void RB(int nRightOrBottom) {if (m_bHorz) right = nRightOrBottom; else bottom=nRightOrBottom;}
	void OffTL(int nLeftOrTop) {if (m_bHorz) top = nLeftOrTop; else left = nLeftOrTop;}
	void OffBR(int nRightOrBottom) {if (m_bHorz) bottom = nRightOrBottom; else right=nRightOrBottom;}

	// Get versions of above
	int GetLT() {if (m_bHorz) return left; else return top;}
	int GetRB() {if (m_bHorz) return right; else return bottom;}
	int GetOffTL() {if (m_bHorz) return top; else return left;}
	int GetOffBR() {if (m_bHorz) return bottom;	else return right;}

	// orienation neutral size functions
	int PrimarySize() {if (m_bHorz) return Width(); else	return Height();}
	int SecondarySize() {if (m_bHorz) return Height(); else return Width();}

protected:
	bool m_bHorz;
};



};	// namespace stingray::foundation
};	// namespace stingray

