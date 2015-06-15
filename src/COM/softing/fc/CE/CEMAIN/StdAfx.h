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

// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__D624416B_0BB9_4336_9116_AAE938F62503__INCLUDED_)
#define AFX_STDAFX_H__D624416B_0BB9_4336_9116_AAE938F62503__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxtempl.h>

#include <atlbase.h>
extern CComModule _Module;
#include <atlcom.h>

#pragma warning( push,3 )
//#pragma warning(disable:4786)
#pragma warning(disable:4018)
#include "toolkit/secall.h"
#pragma warning( pop )  /*restore original warning level*/

#import <msxml.dll> raw_interfaces_only named_guids rename_namespace("MSXML_4C")
using namespace MSXML_4C;

#include "fc_tools\fc_defines.h"
#include "fc_todebug\fc_assert.h"
#include "comhelper.h"
#include "cesysdef.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__D624416B_0BB9_4336_9116_AAE938F62503__INCLUDED_)

#endif // __STDAFX_H_

