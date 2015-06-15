/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibServ/StdAfx.h 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: StdAfx.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibServ/StdAfx.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / Online
 *
 * =COMPONENT			OnlLibServ
 *
 * =CURRENT 	 $Date: 28.02.07 19:06 $
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

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define STRICT

#define _WIN32_WINNT 0x0400
#define _ATL_FREE_THREADED

#define _ATL_NO_UUIDOF


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
#include "fc_tools/fc_defines.h"
#include "trace.h" 

#include "resource.h"
#include "Globals.h"		// local copy for the library (only extern declarations!)
#include "CSC_OnlineServer.h"
#include "CSC_OnlineServerExt.h"
#include <baseLib.h>


extern CLSID CLSID_OPCServer;
extern CLSID CLSID_ControlMgt;
extern CLSID CLSID_CommissioningMgt;
extern "C" extern const CLSID CLSID_CustomConnect;
extern LPCTSTR g_StartParam;

#include "ExeModule.h"

extern CExeModule _Module;

#include <atlcom.h>

#include "CpBreakpointNotification.h"
#include "CpCommissioningMgtNotification.h"
#include "CpSimpleMsgNotification.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif

/* ---------------------------------------------------------------------------- */
