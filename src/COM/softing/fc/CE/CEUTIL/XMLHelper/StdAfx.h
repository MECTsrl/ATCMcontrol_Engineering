// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__63EA3456_496D_4AF1_A24F_C1120B81194B__INCLUDED_)
#define AFX_STDAFX_H__63EA3456_496D_4AF1_A24F_C1120B81194B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afx.h>
#include <afxwin.h>
#include <afxtempl.h>

#include <atlbase.h>

#import <msxml.dll> raw_interfaces_only named_guids rename_namespace("MSXML_4C")
using namespace MSXML_4C;


#include "CESysDef.h"


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__63EA3456_496D_4AF1_A24F_C1120B81194B__INCLUDED_)

