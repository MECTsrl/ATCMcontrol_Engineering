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
//  Author:			Dean Hallman
//  Description:	Declarations for SECDockContext
//  Created:		May 1996
//
#if defined(WIN32) 

#ifndef __SDOCKCNT_H__
#define __SDOCKCNT_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

// CDockBar is an private, undocumented MFC class that we subclass.
// Hence, the necessity to include afxpriv.h
#ifndef __AFXPRIV_H__
#include <afxpriv.h>
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
// SECDockContext

// Forward Declarations
class SECDockBar;
class SECControlBar;

class SECDockContext : public CDockContext
{
// Construction
public:
    SECDockContext(CControlBar* pBar);

// Attributes
protected:
	CRect m_rectFocus;
	CPoint m_ptStartDrag;
	CPoint m_ptPrev;
	int m_nPosDockingRow;
	BOOL m_bPreviouslyFloating;  // After unfloat as MDI child,
									// should we dock or float it

	CSize m_sizeMinTrack;      // Minimum tracking size
	CSize m_sizeMaxTrack;      // Maximum tracking size

// Operations
public:

// Double Click Operations
	virtual void ToggleDocking();
	virtual void ToggleMDIFloat();

// Drag Operations
	virtual void StartDrag(CPoint pt);
	void Move(CPoint pt);       // called when mouse has moved
	void EndDrag();             // drop

	// Resize Operations
	virtual void StartResize(int nHitTest, CPoint pt);
	void Stretch(CPoint pt);
	void EndResize();

	// Minimum and maximum tracking size
	CSize GetMinTrackingSize();
	void SetMinTrackingSize(int cx, int cy);
	void SetMinTrackingSize(CSize szMin);

	CSize GetMaxTrackingSize();
	void SetMaxTrackingSize(int cx, int cy);
	void SetMaxTrackingSize(CSize szMax);

// Operations
	void CancelLoop();

// Implementation
public:
    ~SECDockContext();
    BOOL Track();
    void DrawFocusRect(BOOL bRemoveRect = FALSE);
	DWORD CanDock();

	friend class SECControlBar;
};

inline
CSize SECDockContext::GetMinTrackingSize()
{
	return m_sizeMinTrack;
}

inline
void SECDockContext::SetMinTrackingSize(int cx, int cy)
{
	m_sizeMinTrack = CSize(cx,cy);
}

inline
void SECDockContext::SetMinTrackingSize(CSize szMin)
{
	m_sizeMinTrack = szMin;
}

inline
CSize SECDockContext::GetMaxTrackingSize()
{
	return m_sizeMaxTrack;
}

inline
void SECDockContext::SetMaxTrackingSize(int cx, int cy)
{
	m_sizeMaxTrack = CSize(cx,cy);
}

inline
void SECDockContext::SetMaxTrackingSize(CSize szMax)
{
	m_sizeMaxTrack = szMax;
}


//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif // __SDOCKCNT_H__

#endif // WIN32
