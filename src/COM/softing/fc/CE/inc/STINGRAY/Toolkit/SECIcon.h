/////////////////////////////////////////////////////////////////////////////
// SECIcon.h: interface for the SECIcon class.
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
// Author:       Justin Rudd
// Description:  Declaration of SECIcon
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Toolkit/SECIcon.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

#ifndef __SECICON_H__
#define __SECICON_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#ifdef WIN32

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
// SECIcon

class SECIcon : public CObject
{
	DECLARE_DYNAMIC(SECIcon)

	//---------------------------------------------------------------------//
	// 						   Public Interface							   //
	//---------------------------------------------------------------------//

public:
	SECIcon();
	virtual ~SECIcon();

	// Attributes ...Publicly accessible member variables...
	HICON m_hIcon;
	int   m_cxWidth;
	int   m_cyHeight;

	// Accessors ...Get and Set functions...
	HICON GetIcon() const;
	void  SetIcon( HICON hIcon );

	//Operations
	operator BOOL() const { return BOOL(m_hIcon != NULL); }
	//---------------------------------------------------------------------//
	// 							Public Overridables						   //
	//---------------------------------------------------------------------//
public:
	virtual BOOL LoadIcon( LPCTSTR lpszIcon, int iDesiredWidth, int iDesiredHeight );
	
	virtual BOOL LoadIcon( UINT nIcon, int iDesiredWidth, int iDesiredHeight )
		{ 
			return LoadIcon(MAKEINTRESOURCE(nIcon), iDesiredWidth, iDesiredHeight);
		}

	virtual void Draw( CDC& dc, int iLeft, int iTop, CBrush* pBrush = NULL );
	virtual void Draw( CDC& dc, int iLeft, int iTop, int cxWidth, int cyHeight, CBrush* pBrush = NULL );

	//---------------------------------------------------------------------//
	// 							Protected Overridables					   //
	//---------------------------------------------------------------------//
protected:

	//---------------------------------------------------------------------//
	// 						Protected Implementation					   //
	//---------------------------------------------------------------------//
protected:

	// Implementation Member Variables

	// Implementation Member Functions

	//---------------------------------------------------------------------//
	// 						Private Implementation						   //
	//---------------------------------------------------------------------//
private:
};

//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif // WIN32
#endif // __SECICON_H__


