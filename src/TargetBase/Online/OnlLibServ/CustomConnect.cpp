/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibServ/CustomConnect.cpp 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: CustomConnect.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibServ/CustomConnect.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / Online
 *
 * =COMPONENT			OnlLibServ
 *
 * =CURRENT 	 $Date: 28.02.07 19:06 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"
#include "CustomConnect.h"


//------------------------------------------------------------------*
const TCHAR NoGoodChars[] = _T(" '?\"/\\<>*|:~");

//------------------------------------------------------------------*
static BOOL CheckInitString(LPCTSTR sInit)
{
    if (sInit == NULL || *sInit == L'\0' || _tcspbrk(sInit, NoGoodChars) != NULL)
        return FALSE;

/*
    if (lstrcmpi(sInit, _T(".")) == 0   ||
        lstrcmpi(sInit, _T("..")) == 0  ||
        lstrcmpi(sInit, _T("CON")) == 0 ||
        lstrcmpi(sInit, _T("PRN")) == 0 ||
        lstrcmpi(sInit, _T("AUX")) == 0 ||
        lstrcmpi(sInit, _T("NUL")) == 0)
      return FALSE;

    if (lstrcmpi(_T("COM1"), sInit) <= 0 &&
        lstrcmpi(sInit, _T("COM9")) <= 0 &&
        lstrlen(sInit) == 4)
      return FALSE;

    if (lstrcmpi(_T("LPT1"), sInit) <= 0 &&
        lstrcmpi(sInit, _T("LPT9")) <= 0 &&
        lstrlen(sInit) == 4)
      return FALSE;
*/

    return TRUE;
}

static CString StripCommParam(BSTR sInit)
{
    USES_CONVERSION;
    LPTSTR lpszPart = W2T(sInit);
    LPTSTR lpszFull = W2T(sInit);
    LPTSTR lpsz = _tcschr(lpszPart, _T(','));
    if (lpsz != NULL)
        *lpsz = _T('\0');

    if (!CheckInitString(lpszPart))
        return lpszPart;

    //HACK!
    CRegKey keySW;
    LONG lRes = keySW.Open(HKEY_LOCAL_MACHINE, _T("Software"));
    if (lRes == ERROR_SUCCESS)
    {
        CRegKey key4C;
        lRes = key4C.Create(keySW, g_cs4Control);
        if (lRes == ERROR_SUCCESS)
        {
            CRegKey keyApp;
            lRes = keyApp.Create(key4C, g_csApp);
            if (lRes == ERROR_SUCCESS)
            {
                CRegKey keySysVar;
                lRes = keySysVar.Create(keyApp, g_csSysVar);
                if (lRes == ERROR_SUCCESS)
                {
                    CRegKey keyResult;
                    lRes = keyResult.Create(keySysVar, lpszPart);
                    if (lRes == ERROR_SUCCESS)
                    {
                         keyResult.SetValue(lpszFull, _T("CommParam"));
                    }
                }
            }
        }
    }

    return lpszPart;
}

/////////////////////////////////////////////////////////////////////////////
// CCustomConnect
CCriticalSection CCustomConnect::cs;


//------------------------------------------------------------------*
CCustomConnect::CCustomConnect()
{
}

//------------------------------------------------------------------*
CCustomConnect::~CCustomConnect()
{
}


//------------------------------------------------------------------*
STDMETHODIMP
   CCustomConnect::CreateVarSession(/*[in, string]*/ BSTR sInit, /*[out]*/ IUnknown **ppOPCServer)
{
    USES_CONVERSION;
    CSingleLock lock(&cs, TRUE);
    if (ppOPCServer == NULL)
        return E_POINTER;
    CString strInit = StripCommParam(sInit);
    if (!CheckInitString(strInit))
        return E_INVALIDARG;
    CLSID guid;
    TCHAR szProgID[256];
    lstrcpy(szProgID, g_ProgID_OPCServer);
    lstrcat(szProgID, strInit);
    LPCWSTR lpwstrProgID = T2W(szProgID);
    HRESULT hr = CLSIDFromProgID(lpwstrProgID, &guid);
    if (FAILED(hr)) {
        hr = _Module.RegisterServerInstance(strInit);
        hr = CLSIDFromProgID(lpwstrProgID, &guid);
        for(int i=0; FAILED(hr) && i<100; i++) { // W2K: SP2-HACK
            Sleep(100);
            hr = CLSIDFromProgID(lpwstrProgID, &guid);
        }
    }
    if (FAILED(hr))
        return hr;
    return CoCreateInstance(guid, NULL, CLSCTX_ALL, IID_IUnknown, (void**)ppOPCServer);
}

//------------------------------------------------------------------*
STDMETHODIMP
   CCustomConnect::CreateCommissioningMgt(/*[in, string]*/ BSTR sInit, /*[out]*/ IUnknown **ppCommissionMgt)
{
    USES_CONVERSION;
    CSingleLock lock(&cs, TRUE);
    if (ppCommissionMgt == NULL)
        return E_POINTER;
    CString strInit = StripCommParam(sInit);
    if (!CheckInitString(strInit))
        return E_INVALIDARG;
    CLSID guid;
    TCHAR szProgID[256];
    lstrcpy(szProgID, g_ProgID_CommissioningMgt);
    lstrcat(szProgID, strInit);
    LPCWSTR lpwstrProgID = T2W(szProgID);
    HRESULT hr = CLSIDFromProgID(lpwstrProgID, &guid);
    if (FAILED(hr)) {
        hr = _Module.RegisterServerInstance(strInit);
        hr = CLSIDFromProgID(lpwstrProgID, &guid);
        for(int i=0; FAILED(hr) && i<100; i++) { // W2K: SP2-HACK
            Sleep(100);
            hr = CLSIDFromProgID(lpwstrProgID, &guid);
        }
    }
    if (FAILED(hr))
        return hr;

    //try to avoid second instance...
    CComPtr<IUnknown> pDummy;
    hr = CreateVarSession(sInit, &pDummy);
    if (FAILED(hr))
        return hr;

    return CoCreateInstance(guid, NULL, CLSCTX_ALL, IID_IUnknown, (void**)ppCommissionMgt);
}

//------------------------------------------------------------------*
STDMETHODIMP
   CCustomConnect::CreateControlManagement(/*[in, string]*/ BSTR sInit, /*[out]*/ IUnknown **ppControlMgt)
{
    USES_CONVERSION;
    CSingleLock lock(&cs, TRUE);
    if (ppControlMgt == NULL)
        return E_POINTER;
    CString strInit = StripCommParam(sInit);
    if (!CheckInitString(strInit))
        return E_INVALIDARG;
    CLSID guid;
    TCHAR szProgID[256];
    lstrcpy(szProgID, g_ProgID_ControlMgt);
    lstrcat(szProgID, strInit);
    LPCWSTR lpwstrProgID = T2W(szProgID);
    HRESULT hr = CLSIDFromProgID(lpwstrProgID, &guid);
    if (FAILED(hr)) {
        hr = _Module.RegisterServerInstance(strInit);
        hr = CLSIDFromProgID(lpwstrProgID, &guid);
        for(int i=0; FAILED(hr) && i<100; i++) { // W2K: SP2-HACK
            Sleep(100);
            hr = CLSIDFromProgID(lpwstrProgID, &guid);
        }
    }
    if (FAILED(hr))
        return hr;

    //try to avoid second instance...
    CComPtr<IUnknown> pDummy;
    hr = CreateVarSession(sInit, &pDummy);
    if (FAILED(hr))
        return hr;

    return CoCreateInstance(guid, NULL, CLSCTX_ALL, IID_IUnknown, (void**)ppControlMgt);
}

/* ---------------------------------------------------------------------------- */
