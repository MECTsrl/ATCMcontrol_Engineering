/* $Header: /4CReleased/V2.20.00/Target/TEMPLATES/Online/OnlServ/StdAfx.h 1     28.02.07 18:54 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: StdAfx.h $
 *						 $Logfile: /4CReleased/V2.20.00/Target/TEMPLATES/Online/OnlServ/StdAfx.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Target / Online
 *
 * =COMPONENT			OnlServ
 *
 * =CURRENT 	 $Date: 28.02.07 18:54 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

#ifndef _STDAFX_H_
#define _STDAFX_H_

//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define STRICT

#define _WIN32_WINNT 0x0400
#define _ATL_FREE_THREADED

#define _ATL_NO_UUIDOF


// #include <afxwin.h>
// #include <afxdisp.h>
#include <afxtempl.h>
#include <afxcoll.h>
#include <afxmt.h>
#include <afxadv.h>
#include <afxdisp.h>
#include <process.h>

#include <afxsock.h>		// MFC socket extensions
#include <afxinet.h>

#include <atlbase.h>

// OPC engine
#include <opcDA.h>
#include <OPCEngine.h>
#include <GenNS.h>
#include <genCache.h>


#include "fc_todebug/fc_assert.h"       // 4C assert and tools DLL
#include "trace.h" 

#include "resource.h"
#include "globals.h"
#include "security.h"
#include "CSC_OnlineServer.h"
#include "CSC_OnlineServerExt.h"
#include "CSC_OnlSrv.h"
#include <baseLib.h>

//----  Globals:    ------------------------------------------------*


//----  Typedefs/Enums:    -----------------------------------------*
//----  Prototypes:    ---------------------------------------------*

extern CLSID CLSID_OPCServer;
extern CLSID CLSID_ControlMgt;
extern CLSID CLSID_CommissioningMgt;
extern LPCTSTR g_StartParam;




//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module

#include "ExeModule.h"

extern CExeModule _Module;

#include <atlcom.h>


#include "CpBreakpointNotification.h"
#include "CpCommissioningMgtNotification.h"
#include "CpSimpleMsgNotification.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

/////////////////////////////////////////////////////////////////////
#endif // _STDAFX_H_

/* ---------------------------------------------------------------------------- */
