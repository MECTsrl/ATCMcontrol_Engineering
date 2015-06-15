/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE/StdAfx.h 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: StdAfx.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE/StdAfx.h $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                4CSC
 *
 * =COMPONENT           CSC Online Control
 *
 * =CURRENT      $Date: 28.02.07 19:00 $
 *               $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *





 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  15.03.2001  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/
/**
 * @file stdafx.h : include file for standard system include files,
 * or project specific include files that are used frequently,
 * but are changed infrequently.
 *
*/
#if !defined(AFX_STDAFX_H__8E2A6B38_60E9_11D1_BCDD_006097E1BE2F__INCLUDED_)
#define AFX_STDAFX_H__8E2A6B38_60E9_11D1_BCDD_006097E1BE2F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// disable some obnoxious warnings when compiling ATL with MFC collections:
#ifdef _DEBUG
#pragma warning(disable : 4786)     // C4786: identifier was truncated to '255' characters in the debug information
#endif

#pragma warning (disable : 4510)   // '__unnamed' : default constructor could not be generated
#pragma warning (disable : 4610)   // union '__unnamed' can never be instantiated - user defined constructor required
//#pragma warning (disable : 4100)   // 'di' : unreferenced formal parameter
#pragma warning (disable : 4244)   // '=' : conversion from 'int' to 'unsigned short', possible loss of data

#define STRICT
#define _WIN32_WINNT 0x0400

#include <afxwin.h>
#include <afxdisp.h>
#include <afxtempl.h>
#include <afxcoll.h>
#include <afxmt.h>

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>
#include <atlctl.h>
#include <comdef.h>

#include "fc_tools\fc_defines.h"
#include "comhelper.h"
#include "traceif.h"
#include "refobj.h"
#include "mfcmap.h"

// reenable warnings
#pragma warning (default : 4510)   // '__unnamed' : default constructor could not be generated
#pragma warning (default : 4610)   // union '__unnamed' can never be instantiated - user defined constructor required
#pragma warning (default : 4100)   // 'di' : unreferenced formal parameter
#pragma warning (default : 4244)   // '=' : conversion from 'int' to 'unsigned short', possible loss of data

HRESULT         GetLocalComputerName(CString &strName);
HRESULT         SplitMachineName(LPCTSTR pszVarName, CString &strPureMachineName, CString &strRemainder);
CString         DecorateConfigName(LPCTSTR pszMachineName);
enum tagCSC_SESSION_STATE;
LPCTSTR         GetSessionStateString(enum tagCSC_SESSION_STATE state);

extern const TCHAR gs_chMachineNameLeadIn;
extern const TCHAR gs_chMachineNameLeadOut;
extern const TCHAR gs_szMachineNameLeadIn[];
extern const TCHAR gs_szMachineNameLeadOut[];
#define EOS         _T('\0')
extern LONG gs_nRunningThreads;

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__8E2A6B38_60E9_11D1_BCDD_006097E1BE2F__INCLUDED)

/*
 *----------------------------------------------------------------------------*
 *  $History: StdAfx.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * 
 * *****************  Version 5  *****************
 * User: Su           Date: 10.07.01   Time: 12:29
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * 
 * *****************  Version 4  *****************
 * User: Su           Date: 19.04.01   Time: 18:35
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * 
 * *****************  Version 3  *****************
 * User: Su           Date: 21.03.01   Time: 16:39
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * 
 * *****************  Version 2  *****************
 * User: Su           Date: 15.03.01   Time: 19:45
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * global trace header and often used headers.
 *==
 *----------------------------------------------------------------------------*
*/
