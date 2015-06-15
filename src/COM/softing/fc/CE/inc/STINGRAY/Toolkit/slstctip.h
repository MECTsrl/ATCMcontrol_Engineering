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
//  Author:		Jan


#ifdef WIN32

#ifndef __SLSTCTIP_H__
#define __SLSTCTIP_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#ifndef _INC_WINDOWSX
#include <windowsx.h>
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

#define TOOLTIP_TIMER			(0xf001)

// Macro for converting non-client mouse button messages to regular mouse message
#define MAKE_NCMOUSEXTOMOUSEX(X)							\
		  { X->wParam = (X->message == WM_NCLBUTTONDOWN)?MK_LBUTTON:MK_RBUTTON;	\
			X->message = X->message + 0x0160; }


class SECListClient;

class SECListTip : public CWnd
{
// Construction
public:
	SECListTip( SECListClient* );

// Attributes
protected:
	int m_nHitItem;
	int m_nHitSubItem;
	SECListClient *m_pLC;
	CString       m_strText;
	CRect         m_rectItem;

	void SetWindowText( const CString &str);

// Operations
public:
	void Refresh(void);

// Overrides
virtual void Hide();
virtual BOOL HookMouseMove(UINT iHitFlags, int nHitItem, int nHitCol, CPoint pt);
virtual BOOL PreTranslateMessage( MSG* pMsg );

// Implementation
public:
	virtual ~SECListTip();

	// Generated message map functions
protected:
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);

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

#endif // __SLSTCTIP_H__

#endif // WIN32
