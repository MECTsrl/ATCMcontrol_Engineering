#ifndef __STDAFX_H_
#define __STDAFX_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


#if !defined(AFX_STDAFX_H__6F53EC3B_C482_11D1_89C6_0060080993FF__INCLUDED_)
#define AFX_STDAFX_H__6F53EC3B_C482_11D1_89C6_0060080993FF__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC OLE automation classes
#endif // _AFX_NO_OLE_SUPPORT


//#ifndef _AFX_NO_DB_SUPPORT
//#include <afxdb.h>			// MFC ODBC database classes
//#endif // _AFX_NO_DB_SUPPORT

//#ifndef _AFX_NO_DAO_SUPPORT
//#include <afxdao.h>			// MFC DAO database classes
//#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxmt.h>          // Multithread sync objects

#include <afxtempl.h>

#include <winsvc.h>

#pragma warning( push,3 )
//#pragma warning(disable:4786)
#pragma warning(disable:4018)
#include "toolkit/secall.h"
#pragma warning( pop )  /*restore original warning level*/


#include <atlbase.h>
extern CComModule _Module;
#include <atlcom.h>

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__6F53EC3B_C482_11D1_89C6_0060080993FF__INCLUDED_)

#endif // __STDAFX_H_
