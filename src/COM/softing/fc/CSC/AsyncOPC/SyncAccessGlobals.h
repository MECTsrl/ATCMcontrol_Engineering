/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSC/AsyncOPC/SyncAccessGlobals.h 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: SyncAccessGlobals.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSC/AsyncOPC/SyncAccessGlobals.h $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                4CSC
 *
 * =COMPONENT           AsyncOPC
 *
 * =CURRENT      $Date: 28.02.07 19:00 $
 *               $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  07.08.01  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/

#ifndef __SYNCACCESSGLOBALS_H_
#define __SYNCACCESSGLOBALS_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*
#define EOS         _T('\0')

//----  Globals:    ------------------------------------------------*
static const TCHAR s_chMachineNameLeadIn  = _T('[');
static const TCHAR s_chMachineNameLeadOut = _T(']');
static const TCHAR s_chAltMachineNameLeadIn  = _T('{');
static const TCHAR s_chAltMachineNameLeadOut = _T('}');

const CString gstrSeperator = _T("|");
const TCHAR gchSeperator = _T('|');

//----  Prototypes:    ---------------------------------------------*
CString DecorateConfigName(LPCTSTR pszMachineName);
HRESULT SplitMachineName(LPCTSTR pszVarName, CString &strPureMachineName, CString &strRemainder);
CString GetConfigName(LPCTSTR pszVarName, CString& strPureName);

#endif // __SYNCACCESSGLOBALS_H_


/*
 *----------------------------------------------------------------------------*
 *  $History: SyncAccessGlobals.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CSC/AsyncOPC
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CSC/AsyncOPC
 * 
 * *****************  Version 1  *****************
 * User: Ca           Date: 8.08.01    Time: 9:41
 * Created in $/4Control/COM/softing/fc/CSC/AsyncOPC
 * copied from V1.3
 *==
 *----------------------------------------------------------------------------*
*/
