/* $Header: /4CReleased/V2.20.00/Target/4CWin/Control/WinVMM_Ext/StdAfx.h 1     24.02.08 18:29 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: StdAfx.h $
 *						 $Logfile: /4CReleased/V2.20.00/Target/4CWin/Control/WinVMM_Ext/StdAfx.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				RunTime
 *
 * =COMPONENT			WinVMM
 *
 * =CURRENT 	 $Date: 24.02.08 18:29 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

#if !defined(AFX_STDAFX_H__477A61B9_096E_11D5_8705_0050DA6AA705__INCLUDED_)
#define AFX_STDAFX_H__477A61B9_096E_11D5_8705_0050DA6AA705__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT


#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__477A61B9_096E_11D5_8705_0050DA6AA705__INCLUDED_)

/* ---------------------------------------------------------------------------- */
