// 
// Stingray Software Extension Classes
// Copyright (C) 1995 Stingray Software Inc.
// All Rights Reserved
// 
// This source code is only intended as a supplement to the
// Stingray Extension Class product.
// See the SEC help files for detaild information
// regarding using SEC classes.
// 
//  Author:		Scot Wingo
//  Description:	SECBitmapDialog definition
//  Created:		Sun Jun 25 22:54:12 1995
// CHANGELOG: 
//
//
//
//
//

#ifndef __SECBMPDG_H__
#define __SECBMPDG_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#ifndef __SFL_SECDIB_H__
#include "Foundation\Image\MFC\secdib.h"
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
// SECBitmapDialog 
#define SEC_BITMAP_TILE 	0
#define SEC_BITMAP_CENTER	1
#define SEC_BITMAP_FILL     2

/////////////////////////////
// AutoDuck tag block block for SECBitmapDialog 





// adinsert AutoDuck insertion point for SECBitmapDialog 
//@doc SECBitmapDialog 
//@class The SECBitmapDialog class is derived from CDialog. It implements a dialogue with a bitmap in the background.
// SECBitmapDialog supports three different bitmap modes.
//@flag SEC_BITMAP_TILE | The bitmap is tiled in the dialog background.
//@flag SEC_BITMAP_CENTER | The bitmap is placed in the center of the dialog background.
//@flag SEC_BITMAP_FILL | The bitmap is sized so that it completely fills the dialog background.
//@comm The BMPDIALOG32 sample in the \SAMPLES\TOOLKIT\STANDARD\UIEXT\BMPDIALOG32\ directory demonstrates how to create
// SECBitmapDialogs with all three display options.
//@ex Create a tiled brickwall bitmap|The example shows how to create a dialog with a brickwall bitmap tiled in the background.
//SECBitmapDialog bmpDlg(IDD_MODAL_BMPDLG);
//bmpDlg.SetBitmap(IDB_BRICKWALL, SEC_BITMAP_TILE);
//bmpDlg.DoModal();
//@BASE public|CDialog
class SECBitmapDialog : public CDialog
{
	DECLARE_DYNAMIC(SECBitmapDialog)

// Construction
//@access Public Members
public:
	//@cmember
	/* Creates a modal dialogue*/
	SECBitmapDialog(UINT nDialogID, CWnd* pParent = NULL);
	//@cmember
	/* Creates a modal dialogue*/
	SECBitmapDialog(LPCTSTR lpcDialogName, CWnd* pParent = NULL);

	//@cmember
	/* Sets the specified bitmap in the dialogue*/
	BOOL SetBitmap(UINT nResBitmapID, int nType = SEC_BITMAP_TILE);
	//@cmember
	/* Sets the specified bitmap in the dialogue*/
    BOOL SetBitmap(LPCTSTR lpcResBitmapName, int nType = SEC_BITMAP_TILE);
	//@cmember
	/* Sets the specified bitmap in the dialogue*/
	BOOL SetBitmap(stingray::foundation::SECImage* pImage, int nType = SEC_BITMAP_TILE);

// Attributes
public:

protected:
	stingray::foundation::SECDib*	m_pDib;
	CBrush  m_hollowBrush;
	int     m_nType; //Tiled, centered or filled?

//#if defined(UNDER_CE) & (_MFC_VER != 0x0420)

	CPalette* SetPalette(CPalette* pPalette);
	CPalette* m_pPalette;

//#else

//	HPALETTE SetPalette(HPALETTE hPalette);
//	HPALETTE m_hPalette;

//#endif


// Operations
public:
	
	void SetNullBitmap();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SECBitmapDialog)
	//}}AFX_VIRTUAL

protected:
	virtual HBRUSH OnStaticCtlColor(CDC* pDC, CWnd* pWnd);


// Implementation
public:
	virtual ~SECBitmapDialog();

	// Generated message map functions
protected:
	virtual void UseHalftonePalette(BOOL bHalftone = TRUE);
	virtual HBRUSH GetHollowBrush();
	//{{AFX_MSG(SECBitmapDialog)
	afx_msg BOOL OnQueryNewPalette();
	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
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


#endif // __SECBMPDG_H__

