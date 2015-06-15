

// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__AD7F0A05_0D6D_457F_8365_116D676E3916__INCLUDED_)
#define AFX_STDAFX_H__AD7F0A05_0D6D_457F_8365_116D676E3916__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#define _ATL_APARTMENT_THREADED

#include <afx.h>
#include <afxwin.h>
#include <afxdisp.h>
#include <Afxtempl.h>
#include <afxcmn.h>
#include <assert.h>


#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>
#include <atlctl.h>



#import <msxml.dll> raw_interfaces_only named_guids rename_namespace("MSXML_4C")
using namespace MSXML_4C;



#include "XMLHelper\XMLDocument.h"
#include "XMLHelper\XMLNode.h"
#include "XMLHelper\XMLNodeList.h"
#include "XMLHelper\IdPathHelp.h"
#include "XMLHelper\XMLNodeIter.h"

#include "CESysDef.h"

#include "defs.h"


//#define TAKE_TIME_DEFINE 
#include "TakeTime.h"


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__AD7F0A05_0D6D_457F_8365_116D676E3916__INCLUDED)
