// InstConsole.h: Schnittstelle für die Klasse CInstConsole.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INSTCONSOLE_H__7F0B46A7_6818_43C5_B53B_A7DC8FD12A08__INCLUDED_)
#define AFX_INSTCONSOLE_H__7F0B46A7_6818_43C5_B53B_A7DC8FD12A08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#ifndef FCINST_API
#define FCINST_API extern "C" __declspec(dllimport)
#endif
#ifndef FCINST_DECL
#define FCINST_DECL WINAPI
#endif




FCINST_API  void FCINST_DECL DeleteVirtualConsoleDir(HWND hWnd);
FCINST_API  void FCINST_DECL CreateVirtualConsoleDir(HWND hWnd, LPSTR lpszPath);
FCINST_API  void FCINST_DECL StartConsoleServices(HWND hWnd);
FCINST_API  void FCINST_DECL StopConsoleServices(HWND hWnd);
FCINST_API  void FCINST_DECL GetIEInstallDir(HWND hWnd, LPSTR lpszPath);

#endif // !defined(AFX_INSTCONSOLE_H__7F0B46A7_6818_43C5_B53B_A7DC8FD12A08__INCLUDED_)
