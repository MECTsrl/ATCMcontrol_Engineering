/////////////////////////////////////////////////////////////////////////////
// tbtntxt.h : Declaration of SECTBTextButton
//
// Stingray Software Extension Classes
// Copyright (C) 1997 Stingray Software Inc,
// All rights reserved.
//
// This source code is only intended as a supplement to
// the Objective Toolkit Class Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding
// the Objective Toolkit product.
//
// Author:       Mark Isham
// Description:  Declaration of SECTBTextButton
//
// Use this subclass of SECStdBtn to utilize large
// buttons with rendered text. (no text rendered for small icons)


#ifndef __SECTEXTWBDBUTTON_H__
#define __SECTEXTWBDBUTTON_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#ifdef WIN32

// Header Includes
#ifndef __TBTNSTD_H__
#include "toolkit\tbtnstd.h"
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

// Constants

// Macros

// Types

// Forward Declarations

/////////////////////////////////////////////////////////////////////////////
// SECTBTextButton

class SECTBTextButton : public SECStdBtn
{
	//---------------------------------------------------------------------//
	// 						   Public Interface							   //
	//---------------------------------------------------------------------//
public:
	// establish toolbar button interface
	DECLARE_BUTTON(SECTBTextButton)

	// Initialization/Destruction
	SECTBTextButton();
	~SECTBTextButton();

	// Attributes
	// ...Publicly accessible member variables...

	// Use a static font to allow easy reset without mucking
	// with button macros.
	static void SetTextFont(CFont* pFont);
	static CFont* GetTextFont();

	// Accessors
	// ...Get and Set functions...
	void GetButtonText(CString& strText)		{ strText=m_strDrawText; }
	void SetButtonText(const CString& strText)	{ m_strDrawText=strText; }

	// Operations

	//---------------------------------------------------------------------//
	// 							 Overridables							   //
	//---------------------------------------------------------------------//

	// Initializes button after creation.
	virtual void Init(SECCustomToolBar* pToolBar, const UINT* pData);

protected:

	// Hook into the button drawing
	virtual void OnDrawFaceBlt(int xImg,int yImg,int nImgWidth,int nImgHeight,
							   SECBtnDrawData& data,int xDest,int yDest);
	virtual void OnDrawFaceText(SECBtnDrawData& data);

	//---------------------------------------------------------------------//
	// 						Protected Implementation					   //
	//---------------------------------------------------------------------//
protected:

	// Implementation Member Variables
	CString m_strDrawText;
	static CFont* m_pTextFont;
	static m_nFontRefCount;

	// Implementation Member Functions

	//---------------------------------------------------------------------//
	// 						  Base Class Overrides						   //
	//---------------------------------------------------------------------//

	//---------------------------------------------------------------------//
	// 						Message Map Declarations					   //
	//---------------------------------------------------------------------//

	//---------------------------------------------------------------------//
	// 						Private Implementation						   //
	//---------------------------------------------------------------------//
private:

};

// SECTBTextButton helper macro	- BUTTON_CLASS expansion requires use
// of both DECLARE_BUTTON and IMPLEMENT_BUTTON macros in class definition.
#define TEXT_BUTTON(ButtonID, textResourceID) \
	{BUTTON_CLASS(SECTBTextButton), ButtonID, TBBS_BUTTON, textResourceID, 0, 0, \
	 0, 0},
// sam as above, witht he addition of a style
#define TEXT_BUTTON_EX(ButtonID, textResourceID, style) \
	{BUTTON_CLASS(SECTBTextButton), ButtonID, TBBS_BUTTON|style, textResourceID, 0, 0, \
	 0, 0},

/////////////////////////////////////////////////////////////////////////////

//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif	// WIN32

#endif //__SECTEXTWBDBUTTON_H__
