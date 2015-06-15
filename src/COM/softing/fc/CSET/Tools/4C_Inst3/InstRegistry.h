// InstRegistry.h: Schnittstelle für die Klasse CInstRegistry.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INSTREGISTRY_H__AD7DCFB8_84D8_416E_B5C5_D3E714C36683__INCLUDED_)
#define AFX_INSTREGISTRY_H__AD7DCFB8_84D8_416E_B5C5_D3E714C36683__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#ifndef FCINST_API
#define FCINST_API extern "C" __declspec(dllimport)
#endif
#ifndef FCINST_DECL
#define FCINST_DECL WINAPI
#endif





BOOL DoesLocalMachineKeyExist(LPCSTR szKey);
BOOL FindRegistryKey(HKEY hKey,
					 LPCSTR pszRegistryKey,
					 LPCSTR pszSubString,
                     LPSTR pValue,
					 DWORD * pdwSize);
BOOL QueryRegistryValue(HKEY hKey,
                        LPCSTR pszRegistryKey,
                        LPCSTR pszName,
                        DWORD * pdwType, 
                        LPBYTE pValue,
                        DWORD * pdwSize);


#endif // !defined(AFX_INSTREGISTRY_H__AD7DCFB8_84D8_416E_B5C5_D3E714C36683__INCLUDED_)
