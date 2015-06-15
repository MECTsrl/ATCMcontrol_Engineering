

#ifndef _RUNAS_H_
#define _RUNAS_H_




#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#ifndef FCINST_API
#define FCINST_API extern "C" __declspec(dllimport)
#endif
#ifndef FCINST_DECL
#define FCINST_DECL WINAPI
#endif




FCINST_API LONG FCINST_DECL SetOPCServerRunAs(HWND hWnd, LPSTR lpszAppId, LPSTR lpszSrvName);

DWORD CreateUser	(LPTSTR lpszUser, LPTSTR lpszPass, LPTSTR lpszComment);
DWORD HideUser		(LPTSTR lpszUser);
BOOL  GetAdminGroup	(LPTSTR lpszGroup, PDWORD cchGroup);
DWORD AddUserToGroup(LPTSTR lpszGroup, LPTSTR lpszUser);
DWORD SetRunAsOption(LPTSTR lpszAppId, LPTSTR lpszUser, LPTSTR lpszPass);


#endif