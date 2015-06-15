/////////////////////////////////////////////////////////////////////////////
// SECRect.h : Declaration of SECRect
//
// Stingray Software Extension Classes
// Copyright (C) 1998 Stingray Software Inc,
// All rights reserved.
//
// This source code is only intended as a supplement to
// the Objective Toolkit Class Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding
// the Objective Toolkit product.
//
// Author:       Mark Isham
// Description:  Declaration of SECRect
//

#ifndef __SFL_SEC_RECT_H__
#define __SFL_SEC_RECT_H__

// Header Includes
//
// SEC Extension DLL
// Initialize declaration context
//
#ifdef _SECDLL
	#undef AFXAPP_DATA
	#define AFXAPP_DATA AFXAPI_DATA
	#undef AFX_DATA
	#define AFX_DATA    SEC_DATAEXT
#endif //_SECDLL

// Constants

// Macros

// Types

// Forward Declarations


namespace stingray {
namespace foundation {

/////////////////////////////////////////////////////////////////////////////
// SECRect

class SECRect : public CRect {
public:
	// Construction/Initialization
	SECRect();
	SECRect(BOOL bHorz);
	SECRect(const RECT& srcRect,BOOL bHorz=TRUE);
	SECRect& operator=(const CRect& rhs);

	// Set the implicit orientation of this rectangle
	void SetHorzOrient(BOOL bHorz=TRUE);

	//
	// Orientation-neutral functions
	//

	// Set left/top or right/bottom based on set orientation.
	// If horz, primary LT is left, primary RB is right,
	// secondary TL is top, off BR is bottom. Reversed for vertical.
	void LT(int nLeftOrTop);			// primary left for horz, top vert
	void RB(int nRightOrBottom);		// primary right for horz, bottom vert
	void OffTL(int nLeftOrTop);			// off side: top for horz, left for vert
	void OffBR(int nRightOrBottom);		// off side: bottom for horz, right for vert

	// Get versions of above
	int GetLT();
	int GetRB();
	int GetOffTL();
	int GetOffBR();

	// orienation neutral size functions
	int PrimarySize();			// Width() for horz, Height() for vert
	int SecondarySize();		// Height() for horz, Width() for vert

protected:
	BOOL m_bHorz;
};

};  // namespace stingray::foundation
};  // namespace stingray

/////////////////////////////////////////////////////////////////////////////

//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR


#endif //__SFL_SEC_RECT_H__
