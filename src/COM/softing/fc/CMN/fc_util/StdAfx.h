/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_util/StdAfx.h 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: StdAfx.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_util/StdAfx.h $
 *
 * =PROJECT             7VH102  ATCMControl
 *
 * =SWKE                CMN
 *
 * =COMPONENT           UTIL
 *
 * =CURRENT             $Date: 28.02.07 19:00 $
 *                      $Revision: 1 $
 *
 * =ENVIRONMENT         Win32:
 *                      MS Windows NT 4.0
 *                      MSVC++ V5.0 / MFC 4.2 / ATL 2.0 / DAO 3.5
 *                      MS Java 2.0
 *                      MS Visual SourceSafe 5.0
 *
 * =REFERENCES
 *==
 *----------------------------------------------------------------------------*
 *

 

 

 *==
 *
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *	stdafx.h : include file for standard system include files,
 *	or project specific include files that are used frequently, but are changed 
 *	infrequently
 *----------------------------------------------------------------------------*
 * =MODIFICATIONS
 * $History: StdAfx.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CMN/fc_util
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CMN/fc_util
 * 
 * *****************  Version 2  *****************
 * User: Su           Date: 10.04.01   Time: 12:36
 * Updated in $/4Control/COM/softing/fc/CMN/fc_util
 * 
 * *****************  Version 1  *****************
 * User: Ef           Date: 23.01.01   Time: 17:10
 * Created in $/4Control/COM/softing/fc/CMN/fc_util
 * utils and baselib from V1.3
 * 
 * *****************  Version 5  *****************
 * User: Rf           Date: 22.03.99   Time: 17:04
 * Updated in $/4Control/COM/softing/fc/CMN/UTIL
 * 
 * *****************  Version 4  *****************
 * User: Je           Date: 23.11.98   Time: 12:08
 * Updated in $/4Control/COM/softing/fc/CMN/UTIL
 * * moved XMLEncode/Decode from BaseLib to Util
 * 
 * *****************  Version 3  *****************
 * User: Vo           Date: 30.06.98   Time: 16:54
 * Updated in $/4Control/COM/softing/fc/CMN/UTIL
 * UTIL_GetDataTypeName
 * 
 * *****************  Version 2  *****************
 * User: Vo           Date: 9.06.98    Time: 14:12
 * Updated in $/4Control/COM/softing/fc/CMN/UTIL
 * New function UTIL_IsAdmin
 * 
 * *****************  Version 1  *****************
 * User: Su           Date: 3/26/98    Time: 10:13a
 * Created in $/4Control/COM/softing/fc/CMN/UTIL
 *==
 ******************************************************************************
H<<*/

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


#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxmt.h>          // Multithread sync objects

#include <afxtempl.h>

#include <winsvc.h>
#include "fc_tools\fc_defines.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__6F53EC3B_C482_11D1_89C6_0060080993FF__INCLUDED_)
