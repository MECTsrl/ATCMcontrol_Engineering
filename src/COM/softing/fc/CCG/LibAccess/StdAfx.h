
#if !defined(AFX_STDAFX_H__4FEFAD00_1771_11D5_A3B3_0008C79B568D__INCLUDED_)
#define AFX_STDAFX_H__4FEFAD00_1771_11D5_A3B3_0008C79B568D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#define _ATL_APARTMENT_THREADED


#ifdef _DEBUG
#define _ATL_DEBUG_QI 
#define ATL_TRACE_LEVEL 4
#define ATL_TRACE_CATEGORY ( atlTraceGeneral )
#endif  //_DEBUG


#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>


#include "fc_tools\fc_libmem.h"
#include "fc_todebug\fc_assert.h"
#include "fc_tools\fc_fileIO.h"
#include "fc_tools\fc_cgarray.h"
#include "fc_tools\fc_string.h"
#include "fc_tools\fc_cstring.h"
#include "newdel.h"

#define CL_MAX_FILE_SIZE 20000000


#include "CG_4CLAccess.h"
#include "fc_tools\fc_carchive.h"
#include "LowLevelLib.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__4FEFAD00_1771_11D5_A3B3_0008C79B568D__INCLUDED)
