#if !defined(AFX_STDAFX_H__67FEBD36_F3B1_11D1_89F2_0060080993FF__INCLUDED_)
#define AFX_STDAFX_H__67FEBD36_F3B1_11D1_89F2_0060080993FF__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define STRICT

#include <afxwin.h>
#include <afxdisp.h>
#include <afxtempl.h>

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxcmn.h>

#define _WIN32_WINNT 0x0400
#define _ATL_APARTMENT_THREADED

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>
#include "comdef.h"
#include "fc_todebug\fc_assert.h"
#include "fc_tools\fc_defines.h"
#include "comhelper.h"
//use standard assert defined in fc_assert.h as assert mechanism.
#define SECMAPASSERT(x) assert((x))
#include "refobj.h"
#include "csvif.h"
#include "dldefs.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__67FEBD36_F3B1_11D1_89F2_0060080993FF__INCLUDED)
