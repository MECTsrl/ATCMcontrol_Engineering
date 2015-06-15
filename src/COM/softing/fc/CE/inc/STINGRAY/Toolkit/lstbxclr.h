// 
// Stingray Software Extension Classes
// Copyright (C) 1995 Stingray Software Inc.
// All Rights Reserved
// 
// This source code is only intended as a supplement to the
// Stingray Extension Class product.
// See the SEC help files for detailed information
// regarding using SEC classes.
// 
//  Author:			John Williston
//  Description:	Declaration for SECColorsListBox
//  Created:		August 1996
//

#ifndef __LSTBXCLR_H__
#define __LSTBXCLR_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

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

/////////////////////////////////////////////////////////////////////////////
// SECColorsListBox window

/////////////////////////////
// AutoDuck tag block block for SECColorsListBox 

// adinsert AutoDuck insertion point for SECColorsListBox 
//@doc SECColorsListBox 
//@class The SECColorsListBox class implements an owner-drawn list box that 
// displays a list of color names with the actual color beside each name.  
// Double-clicking on a color pops up a color edit box, displaying the color 
// and allowing the user to edit the color.  After the user is done, the 
// color is written back to the selected list box item.
//
//@comm See the TOOLMENU sample in the \SAMPLES\TOOLKIT\STANDARD\UIEXT\TOOLMENU 
// directory for a  demonstration of this class.
//@base public | CListBox
class SECColorsListBox : public CListBox
{

public:

	//@access Creation/Initialization

	//@cmember
	/* Constructs an SECColorsListBox object. */
	SECColorsListBox(BOOL bAllowDblClickEdit = TRUE);

	// Attributes
public:

	//@access Operations

	//@cmember
	/* The specified list box is converted to a color list box.*/
	BOOL ReplaceListbox(CListBox* pWnd);

	//@cmember
	/* Adds a color to the list box.*/
	int AddColor(const TCHAR* pcszName, COLORREF crColor);
	
	//@cmember
	/* Returns the text and color at the specified index*/
	BOOL GetColor(int iIndex, CString& str, COLORREF& cr);
	
	//@cmember
	/* Sets the text and color at the specified index.*/
	BOOL SetColor(int iIndex, const TCHAR* pcszName, COLORREF cr);
	
	//@cmember
	/* Pops up a color edit box to edit the selected color.*/
	BOOL EditSelectedColor();

public:

	// access Overrides

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SECColorsListBox)

	// Called when a list box with an owner-draw style is created. 
		virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	// Called when a visual aspect of an owner-draw list view control changes. 
		virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(SECColorsListBox)

	// Called when the user double-clicks the left mouse button. 
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	//}}AFX_MSG

	
	// Implementation
public:
	virtual ~SECColorsListBox();

private:
	BOOL m_bAllowDblClickEdit;
	void FastRect(CDC *pDC, CRect* pRect, COLORREF cr);

	// Generated message map functions
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////

//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif	// __LSTBXCLR_H__

