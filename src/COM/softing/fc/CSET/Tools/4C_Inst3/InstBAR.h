// InstBAR.h: interface for the InstBAR class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INSTBAR_H__1C4E4962_C82B_461F_A6D3_17D36BD50C90__INCLUDED_)
#define AFX_INSTBAR_H__1C4E4962_C82B_461F_A6D3_17D36BD50C90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#ifndef FCINST_API
#define FCINST_API extern "C" __declspec(dllimport)
#endif
#ifndef FCINST_DECL
#define FCINST_DECL WINAPI
#endif



FCINST_API LONG FCINST_DECL RemoveBRShare(HWND hWnd);
FCINST_API LONG FCINST_DECL ShareBRDir(HWND hWnd, LPSTR lpszDir);



#endif // !defined(AFX_INSTBAR_H__1C4E4962_C82B_461F_A6D3_17D36BD50C90__INCLUDED_)
