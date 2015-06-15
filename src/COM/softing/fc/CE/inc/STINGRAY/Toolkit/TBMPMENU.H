// 
// Stingray Software Extension Classes
// Copyright (C) 1998Stingray Software Inc.
// All Rights Reserved
// 
// This source code is only intended as a supplement to the
// Stingray Extension Class product.
// See the SEC help files for detaild information
// regarding using SEC classes.
// 
//  Authors:		Don
//  Description:	Declarations for SECBmpMenuPlugIn
//  Created:		Jan 1998
//

#ifdef WIN32

#ifndef __TBMPMENU_H__
#define __TBMPMENU_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif


#ifndef __PLUGIN_H__
#include "Foundation\patterns\MFC\plugin.h"
#endif

#ifndef __TBMPMGR_H__
#include "toolkit\TBmpMgr.h"
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
//  SECBmpMenuItemInfo class
//
//  Holds information about a menu item that we will draw.
//
class SECBmpMenuItemInfo : public CObject
{
	DECLARE_DYNAMIC(SECBmpMenuItemInfo)

public:
	UINT    m_nID;						// The ID of the menu item
	CString m_strText;					// The text of the menu item
	TCHAR   m_cAccel;					// The accelerator character for the item
};


/////////////////////////////////////////////////////////////////////////////
//  SECBmpMenuPlugIn class
//  
//  Implements bitmap menu support (plugging into the window that owns the
//  the menu).
// 
class SECBmpMenuPlugIn : public stingray::foundation::SECWndPlugIn
{
	DECLARE_DYNAMIC(SECBmpMenuPlugIn)

// Construction
public:
	// Note bFixCheckBmps will cause internal bitmaps to be used for check
	// marks. This gets around the problem with the standard check marks
	// being scaled to the current font (where as our bitmaps are fixed to
	// the size of the toolbar bitmap small image).
	SECBmpMenuPlugIn(BOOL bFixCheckBmps = TRUE);

// Attributes
public:

// Operations
public:
	// Adds a toolbar resource for providing bitmap images.
	BOOL AddToolBarResource(LPCTSTR lpszStdBmpName);
	BOOL AddToolBarResource(UINT nIDStdBmp);

	// Adds a bitmap resource for providing bitmap images.
	BOOL AddBitmapResource(LPCTSTR lpszStdBmpName,
						   const UINT* lpIDArray, UINT nIDCount);
	BOOL AddBitmapResource(UINT nIDstdBmpName, const UINT* lpIDArray, UINT nIDCount);

	// Defines a bitmap handle for providing bitmap images.
	BOOL AddBitmap(HBITMAP hBmpSmall, const UINT* lpIDArray, UINT nIDCount);

	// Sets the bitmap manager to use (normally an internal version will be
	// used).
	void SetBmpMgr(SECBmpMgr* pMgr);

	// Defines menu item IDs that are not to be drawn by this class.
	void AddNoSubclass(UINT nID);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SECBmpMenuPlugIn)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~SECBmpMenuPlugIn();

	// Performs garbage collection of various internal allocated structures.
	virtual void GarbageCollect();

	// Entrypoint for frame window to hook WM_INITMENUPOPUP to
	void InitMenuPopup(WPARAM wParam, LPARAM lParam);

protected:
	// SECBmpMenuInfo contains information relating to a specific menu handle,
	// and the items that we have made owner drawn.
	struct SECBmpMenuInfo
	{
		~SECBmpMenuInfo();

		// The handle of the menu.
		HMENU m_hMenu;

		// List of items that we have made owner drawn.
		CList<SECBmpMenuItemInfo*, SECBmpMenuItemInfo*> m_items;
	};

	// Map of menu handles to associated SECBmpMenuInfo structure.
	CMap<HMENU, HMENU, SECBmpMenuInfo*, SECBmpMenuInfo*> m_infos;

	// List of ID's that we are not to make owner drawn.
	CMap<UINT, UINT, UINT, UINT> m_noTouch;

	BOOL       m_bFixCheckBmps;			// TRUE if we are to use our own check mark bitmaps
	SECBmpMgr* m_pBmpMgr;				// Our Bitmap Manager
	CFont      m_fontMenu;				// The font used for drawing menu items
	CFont      m_fontDefaultMenu;		// The font used for drawing default menu items
	HBRUSH     m_hDitherBrush;			// Brush used for dithering background of checked items

	// Function for Creating m_fontMenu.
	virtual void  CreateMenuFont();
	virtual void  CreateDefaultMenuFont();
	
	// Returns the font handle (m_fontMenu) used for drawing menu items.
	HFONT GetMenuFont();
	HFONT GetDefaultMenuFont();

	// Create a mono mask for the given bitmap
	virtual BOOL CreateMask(CDC& dcSrc, CDC& monoDC, CBitmap& bmpMask, CBitmap*& pOldBmp, 
							int cx, int cy, int xSrc);

	// Draws a checked bitmap (background if dithered).
	virtual void DrawChecked(CDC& dcDest, int x, int y, int cx, int cy, CDC& dcSrc, int xSrc);

	// Draws a disabled bitmap
	virtual void DrawDisabled(CDC& dcDest, int x, int y, int cx, int cy, CDC& dcSrc, int xSrc);

	// Method for drawing a bitmap button on a menu item
	virtual void DrawButton(LPDRAWITEMSTRUCT lpDIS, CDC& dc, CRect& r, CBitmap* pBmp, 
							int nBmpIndex, int nBmpWidth, int nBmpHeight);

	// Generated message map functions
protected:
	//{{AFX_MSG(SECBmpMenuPlugIn)
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg LRESULT OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu);
	afx_msg void OnIdleUpdateCmdUI();
	afx_msg void OnSysColorChange();
	afx_msg void OnSettingChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

inline HFONT SECBmpMenuPlugIn::GetMenuFont()
{
	if(m_fontMenu.m_hObject == NULL)
		CreateMenuFont();

	return m_fontMenu;
}
inline HFONT SECBmpMenuPlugIn::GetDefaultMenuFont()
{
	if(m_fontDefaultMenu.m_hObject == NULL)
		CreateDefaultMenuFont();

	return m_fontDefaultMenu;
}


//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

/////////////////////////////////////////////////////////////////////////////

#endif	// __TBMPMENU_H__

#endif	// WIN32
