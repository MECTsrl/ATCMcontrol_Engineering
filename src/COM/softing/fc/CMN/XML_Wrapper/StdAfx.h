// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__E1682776_6B32_11D2_A871_0000B459DC11__INCLUDED_)
#define AFX_STDAFX_H__E1682776_6B32_11D2_A871_0000B459DC11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afx.h>
#include <afxtempl.h>       // template lists

#include <atlbase.h>
#include <afxwin.h>

// ATCMControl assert and tools DLL
#undef assert
#define assert  _ASSERTE
//#include "toassert.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__E1682776_6B32_11D2_A871_0000B459DC11__INCLUDED_)
