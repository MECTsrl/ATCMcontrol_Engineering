// 
// Stingray Software Extension Classes
// Copyright (C) 1997 Stingray Software Inc.
// All Rights Reserved
// 
// This source code is only intended as a supplement to the
// Stingray Extension Class product.
// See the SEC help files for detaild information
// regarding using SEC classes.
// 
//  Authors:		Don
//  Description:	Declarations for SECTBMenuBtn
//  Created:		May 1997
//

#ifdef WIN32

#ifndef __TBTNMENU_H__
#define __TBTNMENU_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#if !defined(__TBARCUST_H__)
#include "toolkit\tbarcust.h"
#endif

#if !defined(__TBTNSTD_H__)
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


class SECTBMenuBtn : public SECStdBtn
{
	DECLARE_BUTTON(SECTBMenuBtn);

public:
	SECTBMenuBtn();

	virtual BOOL BtnPressDown(CPoint point);

	virtual void ShowMenu();
	virtual void CalcAccel();

	// Initialises button after creation.
	virtual void Init(SECCustomToolBar* pToolBar, const UINT* pData);

	CString m_strTitle;					// Title of menu button
	UINT    m_nIDMenu;					// ID of menu resource
	int     m_nIndexMenu;				// Index within menu resource
	TCHAR   m_cAccel;					// Character to accelerate this menu item
	DWORD   m_dwMenuBitFlag;			// Bit flag for menu resource

protected:
	// Drawing functions ...
	virtual void DrawFace(SECBtnDrawData& data, BOOL bForce, int& x, int& y, 
						  int& nWidth, int& nHeight, int nImgWidth = -1);
	// No bitmap, so override to load nothing.
	virtual void LoadItemBitmap(SECBtnDrawData& /* data */) {}

	// Called when toolbar bitmap has changed ... buttons should now adjust
	// their size.
	virtual void AdjustSize();
};

extern HMENU   SecPopupMenuFromMenuTemplate(PMENUITEMTEMPLATE& pItem);
extern BOOL    SecFindItemInMenuTemplate(PMENUITEMTEMPLATE& pItem, UINT nIndex);
extern void    SecAdvanceItemInMenuTemplate(PMENUITEMTEMPLATE& pItem);
extern LPCWSTR SecNameFromMenuTemplate(PMENUITEMTEMPLATE pItem);

#define MENU_BUTTON(id, style, lpszResource, nIndex, lpszTitle) \
	{BUTTON_CLASS(SECTBMenuBtn), id, style|TBBS_BUTTON, (UINT)lpszResource, nIndex, (UINT) lpszTitle, 0, 0},

#define MENU_BUTTON_EX(id, style, lpszResource, nIndex, lpszTitle, dwBitFlag) \
	{BUTTON_CLASS(SECTBMenuBtn), id, style|TBBS_BUTTON, (UINT)lpszResource, nIndex, (UINT) lpszTitle, dwBitFlag, 0},

//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif	// __TBTNMENU_H__

#endif	// WIN32