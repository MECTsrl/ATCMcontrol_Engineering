// InstSelfReg.h: Schnittstelle für die Klasse CInstSelfReg.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INSTSELFREG_H__3B08DF00_6C30_4F6C_B600_7BC630F05FB3__INCLUDED_)
#define AFX_INSTSELFREG_H__3B08DF00_6C30_4F6C_B600_7BC630F05FB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#ifndef FCINST_API
#define FCINST_API extern "C" __declspec(dllimport)
#endif
#ifndef FCINST_DECL
#define FCINST_DECL WINAPI
#endif




FCINST_API LONG FCINST_DECL SelfregDLL(HWND hWnd, LPSTR lpszPath, LPSTR lpszFile);
FCINST_API LONG FCINST_DECL UnregDLL(HWND hWnd, LPSTR lpszPath, LPSTR lpszFile);
FCINST_API LONG FCINST_DECL SelfregExe(HWND hWnd, LPSTR lpszPath, LPSTR lpszFile);
FCINST_API LONG FCINST_DECL UnregExe(HWND hWnd, LPSTR lpszPath, LPSTR lpszFile);




#endif // !defined(AFX_INSTSELFREG_H__3B08DF00_6C30_4F6C_B600_7BC630F05FB3__INCLUDED_)
