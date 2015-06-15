
#if !defined(AFX_CEWATCHVIEW_H__FA85C926_E5B6_11D2_A00A_00A024363DFC__INCLUDED_)
#define AFX_CEWATCHVIEW_H__FA85C926_E5B6_11D2_A00A_00A024363DFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// CeWatchView.h : main header file for CEWATCHVIEW.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CCeWatchViewApp : See CeWatchView.cpp for implementation.

class CCeWatchViewApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CEWATCHVIEW_H__FA85C926_E5B6_11D2_A00A_00A024363DFC__INCLUDED)
