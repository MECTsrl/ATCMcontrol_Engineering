#if !defined(AFX_STDAFX_H__D222B714_B6AC_11D1_A7FB_0000B459DC11__INCLUDED_)
#define AFX_STDAFX_H__D222B714_B6AC_11D1_A7FB_0000B459DC11__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define STRICT

#include <afxwin.h>
#include <afxdisp.h>
#include <afxtempl.h>
#include <afxcoll.h>

#define _WIN32_WINNT 0x0400
//#define _ATL_APARTMENT_THREADED
#define _ATL_FREE_THREADED

#define GSD_PARSE_LOGFILE       (_T("gsd_parse"))
#define GSD_PARSE_LOGFILE_EXT   (_T(".log"))

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>
#include <atlctl.h>
#include "fc_tools\fc_defines.h"
#include "mfcmap.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__D222B714_B6AC_11D1_A7FB_0000B459DC11__INCLUDED)