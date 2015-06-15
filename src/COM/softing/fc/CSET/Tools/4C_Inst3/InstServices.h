// InstServices.h: Schnittstelle für die Klasse CInstServices.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INSTSERVICES_H__0915283E_3725_4629_8044_02C66667740E__INCLUDED_)
#define AFX_INSTSERVICES_H__0915283E_3725_4629_8044_02C66667740E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#ifndef FCINST_API
#define FCINST_API extern "C" __declspec(dllimport)
#endif
#ifndef FCINST_DECL
#define FCINST_DECL WINAPI
#endif




#define SRV_NOTRUNNING 1
#define SRV_RUNNING    2

FCINST_API LONG FCINST_DECL ExistSrv(HWND hWnd, LPSTR lpszSrv);
FCINST_API LONG FCINST_DECL StartSrv(HWND hWnd, LPSTR lpszSrv);
FCINST_API LONG FCINST_DECL StopSrv (HWND hWnd, LPSTR lpszSrv);
FCINST_API LONG FCINST_DECL AskIfStopService(HWND hWnd, LPSTR lpszSrv, LPSTR lpszName);

BOOL IsSrvRunning(LPSTR lpszSrv, BOOL * pbRun);



#endif // !defined(AFX_INSTSERVICES_H__0915283E_3725_4629_8044_02C66667740E__INCLUDED_)
