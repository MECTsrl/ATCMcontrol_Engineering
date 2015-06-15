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
//  Authors:		Matthew
//  Description:	Declarations for SECStdMenuBtn
//  Created:		January 1999
//

#ifdef WIN32

#ifndef __STDMENUBTN_H__
#define __STDMENUBTN_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

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


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// SECStdMenuBtn class
//
/////////////////////////////
// AutoDuck tag block block for SECStdMenuBtn

// adinsert AutoDuck insertion point for SECStdMenuBtn
//@doc SECStdMenuBtn
//@class The SECStdMenuBtn class is derived from SECStdBtn.
// It provides a method to have a toolbar button display a popup
// menu instead of executing one command. 
//@comm 
// 
// A SECStdMenuBtn is instantiated by implementing the STD_MENU_BUTTON
// macro in BUTTON_MAP macro used by the custom toobar framework.
//
//@xref <c SECStdBtn> <c SECCustomToolBar> <c SECToolBarManager>
class SECStdMenuBtn : public SECStdBtn  
{
public:
//Contruction
	SECStdMenuBtn();
	virtual ~SECStdMenuBtn();
//Public methods
	//@access Operations

	//@cmember
	/* Returns a CMenu pointer to the button submenu.*/
	CMenu* GetMenu();

	//@cmember
	/* Sets the popup menu for the button.*/
	void SetNewMenu(UINT nNewResID, UINT nSubmenu = 0, UINT nTrackStyle = TPM_LEFTALIGN);
//Overrides
	virtual UINT BtnPressUp(CPoint point);
	virtual BOOL BtnPressDown(CPoint point);
	virtual void Init(SECCustomToolBar* pToolBar, const UINT* pData);
//Declaratives
	DECLARE_BUTTON(SECStdMenuBtn)
//Internals
private:
	CMenu m_Menu;
	UINT m_nSubMenuIdx;
	UINT m_TrackStyle;
	void DoMenuPopup();
};

//Button macro definition
#define STD_MENU_BUTTON(id, style, menuID, submenuindex, alignstyle) \
	{BUTTON_CLASS(SECStdMenuBtn), id, style|TBBS_BUTTON, menuID, submenuindex, alignstyle, 0, 0},


//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif // __STDMENUBTN_H__

#endif // WIN32