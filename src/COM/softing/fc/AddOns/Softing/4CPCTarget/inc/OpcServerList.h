

#ifndef __OPCSERVERLIST_H_
#define __OPCSERVERLIST_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Defines:    ------------------------------------------------*
#ifndef OPCUTIL_API
#define OPCUTIL_API __declspec(dllexport)
#endif

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*
typedef CList<CLSID, CLSID&>    listCLSID;

// =Enums
typedef enum {
    opcDA_any = 0,
    opcDA1_0 = 1,
    opcDA2_0 = 2
} OPC_VERSION;

typedef enum {
    localRegDB,
    remoteRegDB,
    opcEnum
} BROWSE_METHOD;


// =Operations
#ifdef __cplusplus
extern "C" 
{
#endif // __cplusplus
HRESULT OPCUTIL_API __stdcall   OPC_EnumByCategory(LPCTSTR pszMachineName, OPC_VERSION type, BROWSE_METHOD method, OUT listCLSID &servers);
HRESULT OPCUTIL_API __stdcall   OPC_GetServerDetails(LPCTSTR pszMachineName, CLSID server, BROWSE_METHOD method, OUT CString& strProgID, OUT CString& strReadableName, OUT OPC_VERSION& type);
HRESULT OPCUTIL_API __stdcall   OPC_CLSIDFromProgID(LPCTSTR pszMachineName, BROWSE_METHOD method, LPCTSTR pszProgId, OUT LPCLSID clsid);

#ifdef __cplusplus
}//extern "C" 
#endif // __cplusplus


#endif // __OPCSERVERLIST_H_

