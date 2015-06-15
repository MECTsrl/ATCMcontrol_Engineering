/* $Header: /4CReleased/V2.20.00/Target/4CWin/ONLINE/OnlServ/Globals.h 1     28.02.07 19:11 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: Globals.h $
 *						 $Logfile: /4CReleased/V2.20.00/Target/4CWin/ONLINE/OnlServ/Globals.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Target / Online
 *
 * =COMPONENT			OnlServ
 *
 * =CURRENT 	 $Date: 28.02.07 19:11 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

/* ----  Includes:	 ---------------------------------------------------------- */

#if !defined(_GLOBALS_H_) || defined(DEFINE_GLOBAL_VARS)
#undef _GLOBALS_H_
#define _GLOBALS_H_


//----  Aggregate Includes:   ------------------------------------------------*

//----  Forward Class Definitions:   -----------------------------------------*

//----  Defines:    ----------------------------------------------------------*
//--------- The following #defines rule creation / reference of global vars:
//  DEFINE_GLOBAL_VARS is set:      Define/instantiate the global variables
//  DEFINE_GLOBAL_VARS not set:     only declare globals for external reference
//
//  DEFINE_GLOBAL_VARS must be #defined only once in a application,
//  most commonly in the implementation of the CWinApp derived appl, class!!!
//
#undef DEFINE
#undef INIT_WITH
#if defined (DEFINE_GLOBAL_VARS)
#   define DEFINE
#   define INIT_WITH(a)         =a
#else
#   define DEFINE               extern
#   define INIT_WITH(a)
#endif


//----  Globals:    ----------------------------------------------------------*

//----  Prototypes:    -------------------------------------------------------*

//----  Typedefs:    ---------------------------------------------------------*
#if !defined (DEFINE_GLOBAL_VARS)
typedef enum {
    iecVarType,     // is a IEC variable
    sysVarType,     // is a system variable
    conVarType      // is a console variable
} VAR_TYPE;
#endif

//----  Global References:   -------------------------------------------------*
DEFINE LCID     g_lcid							INIT_WITH(MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT));

DEFINE CString  g_strRegBasePath;

//********  namespace literals 
DEFINE const TCHAR g_csSystem[]					INIT_WITH(_T("__system"));
DEFINE const TCHAR g_csResSRT[]					INIT_WITH(_T("__srt"));
DEFINE const TCHAR g_csResSRT0[]				INIT_WITH(_T("__srt0"));
DEFINE const TCHAR g_csResCon[]					INIT_WITH(_T("__console"));


//********  Sections and Entries for INI-file / Registry
DEFINE const TCHAR g_cs4Control[]				INIT_WITH(_T("Softing\\4ControlV2"));
DEFINE const TCHAR g_csApp[]					INIT_WITH(_T("WinSCS"));
DEFINE const TCHAR g_APP_ID[]					INIT_WITH(_T("{cc473905-0371-4868-ba16-44190dbcd8bf}"));
DEFINE const TCHAR g_ProgID_OPCServer[]			INIT_WITH(_T("4C_20_Win.Online_Server."));
DEFINE const TCHAR g_ProgID_ControlMgt[]		INIT_WITH(_T("4C_20_Win.ControlMgt."));
DEFINE const TCHAR g_ProgID_CommissioningMgt[]	INIT_WITH(_T("4C_20_Win.CommissioningMgt."));

DEFINE const TCHAR g_csRemoteLicEnabled[]		INIT_WITH(_T("RemoteLic"));
DEFINE BOOL  g_bSupportDebugDownload			INIT_WITH(TRUE);

//------    [SysVars]
// config globals:
DEFINE const TCHAR g_csSVarProjectName[]		INIT_WITH(_T("ProjectName"));
DEFINE const TCHAR g_csSVarRes0Name[]			INIT_WITH(_T("ResourceName0"));

// resource globals:
DEFINE const TCHAR g_csSysVar[]					INIT_WITH(_T("SysVars"));
DEFINE const TCHAR g_csResourceState[]			INIT_WITH(_T("ResourceState"));
DEFINE const TCHAR g_csDownloadCounter[]		INIT_WITH(_T("DownloadCounter"));
DEFINE const TCHAR g_csSVarSecDLoad[]			INIT_WITH(_T("DLoad"));
DEFINE const TCHAR g_csSVarDate[]				INIT_WITH(_T("Date"));
DEFINE const TCHAR g_csSVarUser[]				INIT_WITH(_T("User"));
DEFINE const TCHAR g_csSVarHost[]				INIT_WITH(_T("Host"));
DEFINE const TCHAR g_csSVarPrjGUID[]			INIT_WITH(_T("PrjGUID"));
DEFINE const TCHAR g_csSVarDLoadGUID[]			INIT_WITH(_T("DLoadGUID"));
DEFINE const TCHAR g_csSVarCleared[]			INIT_WITH(_T("Cleared"));
DEFINE const TCHAR g_csSVarSecLic[]				INIT_WITH(_T("License"));
DEFINE const TCHAR g_csSVarLicDoesExpire[]		INIT_WITH(_T("DoesExpire"));
DEFINE const TCHAR g_csSVarLicExpires[]			INIT_WITH(_T("Expires"));
DEFINE const TCHAR g_csSVarLicHours[]			INIT_WITH(_T("Hours"));
DEFINE const TCHAR g_csSVarLicMaxIOs[]			INIT_WITH(_T("maxIOs"));
DEFINE const TCHAR g_csSVarSecSecurity[]        INIT_WITH(_T("Security"));
DEFINE const TCHAR g_csSVarSecurityIsEnabled[]  INIT_WITH(_T("isEnabled"));
DEFINE const TCHAR g_csSVarSecurityPassword[]   INIT_WITH(_T("Password"));
DEFINE const TCHAR g_csSVarSecurityRights[]     INIT_WITH(_T("Rights"));
DEFINE const TCHAR g_csSVarSecDebug[]			INIT_WITH(_T("Debug"));
DEFINE const TCHAR g_csSVarDebugIsEnabled[]     INIT_WITH(_T("isEnabled"));
DEFINE const TCHAR g_csSVarDebugStartedDate[]   INIT_WITH(_T("Date"));
DEFINE const TCHAR g_csSVarDebugUser[]          INIT_WITH(_T("User"));
DEFINE const TCHAR g_csSVarDebugHost[]          INIT_WITH(_T("Host"));

DEFINE const TCHAR g_csSoftRT[]					INIT_WITH(_T("softRT"));
DEFINE const TCHAR g_csConsole[]				INIT_WITH(_T("console"));

#endif

/* ---------------------------------------------------------------------------- */
