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

#if!defined _SFL_SECDC_H_
#define _SFL_SECDC_H_

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif


//
// CMN Extension DLL
// Initialize declaration context
//
#ifdef _SFLDLL
	#undef AFXAPP_DATA
	#define AFXAPP_DATA AFXAPI_DATA
	#undef AFX_DATA
	#define AFX_DATA    SFL_DATAEXT
#endif //_SFLDLL


namespace stingray {
namespace foundation {

class SECMemDC : public CDC  
{
public:
	SECMemDC( CDC& dcSrc, CRect rcSrc );
	SECMemDC( CDC* pdcSrc, CRect rcSrc );

	virtual ~SECMemDC();

protected:
	void InitDC();

	CDC* m_pSrcDC;
	CBitmap m_bmp, *m_pOldBitmap;
	CRect m_rcSrc;

	CPoint m_ptVp, m_ptWnd;
	CSize m_szVp, m_szWnd;
};

};  // namespace stingray::foundation
};  // namespace stingray

//
// CMN Extension DLL
// Reset declaration context
//
#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif // _SFL_SECDC_H_

#endif // WIN32


