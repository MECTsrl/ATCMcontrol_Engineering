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

#ifndef __SLSTCLNT_H__
#define __SLSTCLNT_H__

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

/*
	Normally a class like this would be a set of pure virtual functions, but for some reason VC 4.x won't let me use,
	 a pure virtual interface as a default template parameter.  So these are just stubs that do nothing...
*/

class SECListClient
{
public:
	virtual HWND GetHwnd() const
		{ return NULL; }
	virtual BOOL IsWordWrapEnabled() const
		{ return FALSE; }
	virtual BOOL ProcessAndDestroyEditLabel( BOOL )
		{ return FALSE; }
	virtual int  GetSubItemHit( int ) const
		{ return -1; }
	virtual BOOL GetOrigin( LPPOINT ) const
		{ return FALSE; }
	virtual void GetInsideRect( LPRECT ) const
		{}
	virtual BOOL GetItemRect( int, int, LPRECT, UINT ) const
		{ return FALSE; }
	virtual CString GetItemText( int, int ) const
		{ return _T(""); }
	virtual void Client2Screen( LPRECT )
		{}
	virtual CWnd* Capture( BOOL )
		{ return NULL; }
	virtual BOOL IsSelected( int ) const
		{ return FALSE; }
	virtual COLORREF GetSelTextColor() const
		{ return 0; }
	virtual COLORREF GetSelTextBkColor() const
		{ return 0; }
	virtual COLORREF GetToolTipTextColor() const
		{ return 0; }
	virtual COLORREF GetToolTipBkColor() const
		{ return 0; }
	virtual CFont* GetLabelFont() const 
		{ return NULL; }
	virtual UINT GetStateFromItemIndex(int, UINT) const
		{ return 0; }
};

//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif //__SLSTCLNT_H__

#endif //WIN32