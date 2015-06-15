// 
// Stingray Software Extension Classes
// Copyright (C) 1996 Stingray Software Inc.
// All Rights Reserved
// 
// This source code is only intended as a supplement to the
// Stingray Extension Class product.
// See the  help files for detailed information
// regarding using these classes.
// 
//  Authors:		Dean Hallman and Justin Rudd

#ifdef WIN32

#ifndef __SLSTVW_H__
#define __SLSTVW_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#ifndef __SEC_LISTBASEV_H__
#include "toolkit\slstbaseV.h"
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



/////////////////////////////
// AutoDuck tag block block for SECListView 
// adinsert AutoDuck insertion point for SECListView 
//@doc SECListView 
//@class SECListView | This is an internal base class for Stingray list and tree classes.
//@base public | SEC_LISTCLASS
//@xref <c SEC_LISTCLASS>

class SECListView : public SECListBaseV  
{
	DECLARE_DYNCREATE( SECListView )

public:
	//@access Construction/Initialization
	//@cmember
	/* Construction*/
	SECListView();
	virtual ~SECListView();

	SECListView& GetListCtrl() const;

	// printing
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual void OnPrint( CDC* pDC, CPrintInfo* pInfo );

protected:
	// CView overrides
	virtual void OnDraw( CDC* );
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	
	// misc
	virtual BOOL NextPage( CDC* pDC );
	virtual void InternalPrepareDC( CDC* pDC );
	virtual BOOL GoToPage( UINT nPage, CDC* pDC );

	// fonts
	virtual BOOL InitPrinterFonts( CDC* pDC );
	virtual BOOL ReleasePrinterFonts( );


	// core overrides
	virtual void SECGetClientRect(LPRECT lpRect) const;
	virtual CDC* GetActiveDC( BOOL& bDelete );



	CFont*	m_pOldLabelFont;	// saved screen font for drawing label text
	CFont*	m_pOldHeaderFont;	// saved screen font for drawing header text

	CFont	m_printLabelFont;	// font used when printing label text
	CFont	m_printHeaderFont;	// font used when printing the header control

	RECT	m_rectPage;			// we need this in our override of SECGetClientRect
	BOOL	m_bIsPrinting;		// we need this in our override of SECGetClientRect

	int		m_nScreenTopIndex;	// the top item number before printing
	UINT	m_nCurPage;

	CDC*	m_pPrinterDC;

 


protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};

//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif // __SLSTVW_H__

#endif // WIN32

