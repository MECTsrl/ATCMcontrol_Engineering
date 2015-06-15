// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__335603AE_F734_412E_B4D8_C8606A48AAA9__INCLUDED_)
#define AFX_STDAFX_H__335603AE_F734_412E_B4D8_C8606A48AAA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afx.h>
#include <afxwin.h>
#include <assert.h>
#include <Afxtempl.h>


#import <msxml.dll> raw_interfaces_only named_guids rename_namespace("MSXML_4C")
using namespace MSXML_4C;



#include "XMLHelper\XMLDocument.h"
#include "XMLHelper\XMLNode.h"
#include "XMLHelper\XMLNodeList.h"
#include "XMLHelper\IdPathHelp.h"
#include "XMLHelper\XMLNodeIter.h"

#include "CESysDef.h"

// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__335603AE_F734_412E_B4D8_C8606A48AAA9__INCLUDED_)
