// InstProfibus.h: Schnittstelle für die Klasse CInstProfibus.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INSTPROFIBUS_H__34DF6ED0_E88F_4814_9A0C_4AE3A65E5CA2__INCLUDED_)
#define AFX_INSTPROFIBUS_H__34DF6ED0_E88F_4814_9A0C_4AE3A65E5CA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#ifndef FCINST_API
#define FCINST_API extern "C" __declspec(dllimport)
#endif
#ifndef FCINST_DECL
#define FCINST_DECL WINAPI
#endif




FCINST_API LONG FCINST_DECL CheckForOldProfiboard(HWND hWnd, LONG showMessage);
FCINST_API LONG FCINST_DECL InstallRTCESrv(HWND hWnd);
FCINST_API LONG FCINST_DECL UninstallRTCESrv(HWND hWnd);




#endif // !defined(AFX_INSTPROFIBUS_H__34DF6ED0_E88F_4814_9A0C_4AE3A65E5CA2__INCLUDED_)
