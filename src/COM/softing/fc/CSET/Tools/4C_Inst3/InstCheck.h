/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/4C_Inst3/InstCheck.h 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: InstCheck.h $
 *                       $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/4C_Inst3/InstCheck.h $
 *
 * =PROJECT				CAK1020  ATCMControl V2.0
 *
 * =SWKE                4CE
 *
 * =COMPONENT           CSET
 *
 * =CURRENT      $Date: 28.02.07 19:00 $
 *           $Revision: 1 $
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
 *  01.03.2000  KT      File created
 *  see history at end of file !
 *==
 *******************************************************************************
 H<<*/


// InstCheck.h: interface for the InstCheck class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INSTCHECK_H__1F7618B1_F37E_11D4_B8EC_002018641833__INCLUDED_)
#define AFX_INSTCHECK_H__1F7618B1_F37E_11D4_B8EC_002018641833__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAXVERSIONLEN 30


#ifndef FCINST_API
#define FCINST_API extern "C" __declspec(dllimport)
#endif
#ifndef FCINST_DECL
#define FCINST_DECL WINAPI
#endif



// interface
FCINST_API LONG FCINST_DECL ModifyEnvironmentVariable(HWND hWnd, BOOL bSystem, LPTSTR pszName, LPTSTR pszValue);
FCINST_API LONG FCINST_DECL CheckForInstalledMFC(HWND hWnd);
FCINST_API LONG FCINST_DECL StartExecutable(HWND hWnd, LPSTR lpszPath, LPSTR lpszCmd, LPSTR lpszParam);
FCINST_API LONG FCINST_DECL ModifyFileSecurity (HWND hWnd, LPSTR lpszPath);
FCINST_API LONG FCINST_DECL GetProductVersionNumber(HWND hWnd, LPSTR lpszPath, LPSTR lpszFile, LPSTR lpszVersion);

// helper
BOOL CheckVersionNumber(LPCSTR pszVersion, LARGE_INTEGER liCheckVersion);
BOOL GetFileVersion(LPCSTR pszPath, CString & strVersion);
BOOL CheckVersionNumberOfFile(LPCSTR lpszPath, LARGE_INTEGER liVersion);


HRESULT GetLocalMachineName(LPTSTR pszMachineName, size_t SizeChars); 




/*
extern "C" __declspec(dllexport) LONG WINAPI Installed4Control(LPTSTR pszVersion);
extern "C" __declspec(dllexport) LONG WINAPI InstalledAsp(LPTSTR pszVersion);
extern "C" __declspec(dllexport) LONG WINAPI InstalledIEVersion(LPTSTR pszVersion);
extern "C" __declspec(dllexport) LONG WINAPI InstalledJavaSDK(LPTSTR pszVersion);
extern "C" __declspec(dllexport) LONG WINAPI InstalledJavaVM(LPTSTR pszVersion);
extern "C" __declspec(dllexport) LONG WINAPI InstalledOSVersion(LPTSTR pszVersion);
extern "C" __declspec(dllexport) LONG WINAPI InstalledWWW(LPTSTR pszVersion);



BOOL Check4Control(CString & strVersion);
BOOL CheckASP(CString & strVersion);
BOOL CheckIEVersion(CString & strVersion);
BOOL CheckJavaSDK(CString & strVersion);
BOOL CheckJavaSDKPath(LPCSTR pszPath);
BOOL CheckJavaVM(CString & strVersion);
BOOL CheckOSVersion(CString & strVersion, DWORD * pdwPlatformId, BOOL bInstControl);
BOOL CheckWWW(CString & strVersion);
BOOL RegisterJavaSDK(LPCSTR pszPath);
*/





#endif // !defined(AFX_INSTCHECK_H__1F7618B1_F37E_11D4_B8EC_002018641833__INCLUDED_)

/*
 *----------------------------------------------------------------------------*
 *  $History: InstCheck.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/4C_Inst3
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CSET/Tools/4C_Inst3
 * 
 * *****************  Version 6  *****************
 * User: Jd           Date: 16.09.02   Time: 16:36
 * Updated in $/4Control/COM/softing/fc/CSET/Tools/4C_Inst3
 * 
 * *****************  Version 5  *****************
 * User: Jd           Date: 28.11.01   Time: 14:39
 * Updated in $/4Control/COM/softing/fc/CSET/Tools/4C_Inst3
 * Added HWnd parameter, for installer windows handle. Display message
 * boxes in error cases.
 * 
 * *****************  Version 4  *****************
 * User: Jd           Date: 20.11.01   Time: 16:13
 * Updated in $/4Control/COM/softing/fc/CSET/Tools/4C_Inst3
 * 
 * *****************  Version 3  *****************
 * User: Kt           Date: 9.04.01    Time: 15:04
 * Updated in $/4Control/COM/softing/fc/CSET/4C_Inst3
 * added 'FindRegistryKey()'
 * 
 * *****************  Version 2  *****************
 * User: Kt           Date: 2.03.01    Time: 12:26
 * Updated in $/4Control/COM/softing/fc/CSET/4C_Inst3
 * added sourcesafe header
 *==
 *----------------------------------------------------------------------------*
*/