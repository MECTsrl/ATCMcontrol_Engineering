/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSC/AsyncOPC/AsyncAccess.cpp 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: AsyncAccess.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSC/AsyncOPC/AsyncAccess.cpp $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                4CSC
 *
 * =COMPONENT           AsyncOPC
 *
 * =CURRENT      $Date: 28.02.07 19:00 $
 *               $Revision: 1 $
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
 *  06.08.01  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/



//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "AsyncOPC.h"
#include "AsyncAccess.h"
#include "ItemStringList.h"
#include "CSC_OnlineServer.h"
#include "CSC_OnlineServer_i.c"

#include "SyncAccessGlobals.h"

//----  Static Initializations:   ----------------------------------*


// AsyncAccess.cpp : Implementation of CAsyncAccess

/////////////////////////////////////////////////////////////////////////////
// CAsyncAccess

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CSyncAccess

//------------------------------------------------------------------*
CAsyncAccess::CAsyncAccess()
{ 
    TRACE(_T("CAsyncAccess::CAsyncAccess()\n"));
}

//------------------------------------------------------------------*
CAsyncAccess::~CAsyncAccess()
{ 
    TRACE(_T("CAsyncAccess::~CAsyncAccess()\n"));
    for (POSITION pos = m_itemNameMap.GetStartPosition(); pos != NULL; )
    {
        CString strKey;
        CItem *pItem;
        m_itemNameMap.GetNextAssoc(pos, strKey, pItem);
        pItem->Cleanup();
        pItem->Release();
    }
}


/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*	<B>Subscribe</B>
*
*	Description = subscribe item to opc server 
*	<p>
*	Specification = 
*	<p>
*	@param sItem  item path, e.g. [w-machinename].ResourceX.VariableN
*	<p>
*
*	@return
*	<p>
*
*/
STDMETHODIMP CAsyncAccess::Subscribe(BSTR sItem)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    TRACE(_T("Subscribe %s\n"), (LPCTSTR) CString(sItem));
    // separate config name from pure variable name
    CString strItem=sItem;
	return DoSubscribe(strItem);
}

/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*	<B>SubscribeStringList</B>
*
*	Description = subscribe item list to opc server 
*	<p>
*	Specification = 
*	<p>
*	@param sItems  item paths (without common path) seperated by '|'
*	<p>
*	@param sCommonPath path that is common to all items, e.g. "[w-xx].Resource1.". sCommonPath may be empty 
*	<p>
*	@return
*	<p>
*
*/
STDMETHODIMP CAsyncAccess::SubscribeStringList(BSTR sItems, BSTR sCommonPath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    //TRACE(_T("Subscribe List %s\n"), (LPCTSTR) CString(sItems));

    HRESULT hr = S_OK;
	CString strCommonPath=sCommonPath;
	CString strItems=sItems;
	CItemStringList itemList(strCommonPath,strItems);

	for (int ii=0; ii<itemList.GetSize(); ii++)
	{
		HRESULT hr2=DoSubscribe(itemList.GetAt(ii));
		if (hr2 != S_OK)
			hr = S_FALSE;
	}
	return hr;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*	<B>ReadStringList</B>
*
*	Description = read qvts (Quality Value Timestamp) of item list from opc server 
*	<p>
*	Specification = 
*	<p>
*	@param sItems  item paths (without common path) seperated by '|'
*	<p>
*	@param sCommonPath path that is common to all items, e.g. "[w-xx].Resource1.". sCommonPath may be empty 
*	<p>
*	@param psValues [out] list of qvts seperated by '|',
*	"quality item1|value item1|timestamp item1|quality item2| ...".
*	timestamp is number of seconds since 1/1/1970   
*   <p>
*	@return S_OK, errors are coded into psValues
*	<p>
*
*/
STDMETHODIMP CAsyncAccess::ReadStringList(BSTR sItems, BSTR sCommonPath, BSTR *psValues)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    //TRACE(_T("Read List %s\n"), (LPCTSTR) CString(sItems));

	if (psValues == NULL) {
        return E_POINTER;
    }
    *psValues = NULL;

	HRESULT hr = S_OK;
	CString strCommonPath=sCommonPath;
	CString strItems=sItems;
	CString strValues;
	CItemStringList itemList(strCommonPath,strItems);

	for (int ii=0; ii<itemList.GetSize(); ii++)
	{
	    CString strValue;
		HRESULT hr2=DoReadString(itemList.GetAt(ii), strValue);
		if (hr2 != S_OK)
			hr = S_FALSE;
		strValues += strValue;
		if (ii<itemList.GetSize()-1)
			strValues+='|';
	}

    *psValues = strValues.AllocSysString();

	return hr;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*	<B>ReadStringListTyped</B>
*
*	Description = read qvts (Quality Value Timestamp) of item list from opc server. Values are
*	formatted according to IEC 1131 
*	<p>
*	Specification = 
*	<p>
*	@param sItems  item paths (without common path) seperated by '|'
*	<p>
*	@param sCommonPath path that is common to all items, e.g. "[w-xx].Resource1.". sCommonPath may be empty 
*	<p>
*	@param psValues [out] list of qvts seperated by '|',
*	"quality item1|value item1|timestamp item1|quality item2| ...".
*	timestamp is number of seconds since 1/1/1970   
*   <p>
*	@return S_OK, errors are coded into psValues
*	<p>
*
*/
STDMETHODIMP CAsyncAccess::ReadStringListTyped(BSTR sItems, BSTR sCommonPath, BSTR *psValues)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    //TRACE(_T("Read List Typed%s\n"), (LPCTSTR) CString(sItems));

	if (psValues == NULL) {
        return E_POINTER;
    }
    *psValues = NULL;
	
	HRESULT hr = S_OK;
	CString strCommonPath=sCommonPath;
	CString strItems=sItems;
	CString strValues;
	CItemStringList itemList(strCommonPath,strItems);
	CString strValue;

	for (int ii=0; ii<itemList.GetSize()/2; ii++)
	{
		HRESULT hr2=DoReadStringTyped(itemList.GetAt(2*ii+1),itemList.GetAtSimple(2*ii), strValue);
		if (hr2 != S_OK)
			hr = S_FALSE;
		strValues += strValue;
		if (ii<itemList.GetSize()/2-1)
			strValues+='|';
	}

    *psValues = strValues.AllocSysString();
	
	return hr;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*	<B>ReadStringListTypedAndRaw</B>
*
*	Description = read qvts (Quality Value Timestamp) of item list from the value cache, that 
*	was filled by a previous ReadStringList or ReadStringListTyped call. Values are
*	formatted according to IEC 1131 
*	<p>
*	Specification = 
*	<p>
*	@param sItems  item paths (without common path) seperated by '|'
*	<p>
*	@param sCommonPath path that is common to all items, e.g. "[w-xx].Resource1.". sCommonPath may be empty 
*	<p>
*	@param psValues [out] list of qvts seperated by '|',
*	"quality item1|value item1|timestamp item1|quality item2| ...".
*	timestamp is number of seconds since 1/1/1970   
*   <p>
*	@return S_OK, errors are coded into psValues
*	<p>
*
*/
STDMETHODIMP CAsyncAccess::ReadStringListTypedAndRaw(BSTR sItems, BSTR sCommonPath, BSTR *psValues)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    //TRACE(_T("Read List Typed%s\n"), (LPCTSTR) CString(sItems));

	if (psValues == NULL) {
        return E_POINTER;
    }
    *psValues = NULL;
	
	HRESULT hr = S_OK;
	CString strCommonPath=sCommonPath;
	CString strItems=sItems;
	CString strValues;
	CItemStringList itemList(strCommonPath,strItems);
	CString strValue;

	for (int ii=0; ii<itemList.GetSize()/2; ii++)
	{
		HRESULT hr2=DoReadStringTypedAndRaw(itemList.GetAt(2*ii+1),itemList.GetAtSimple(2*ii), strValue);
		if (hr2 != S_OK)
			hr = S_FALSE;
		strValues += strValue;
		if (ii<itemList.GetSize()/2-1)
			strValues+='|';
	}

    *psValues = strValues.AllocSysString();
	
	return hr;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*	<B>Unsubscribe</B>
*
*	Description = unsubscribe item from opc server 
*	<p>
*	Specification = 
*	<p>
*	@param sItem  item path, e.g. [w-machinename].ResourceX.VariableN
*	<p>
*
*	@return
*	<p>
*
*/
STDMETHODIMP CAsyncAccess::Unsubscribe(BSTR sItem)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    TRACE(_T("Unsubscribe %s\n"), (LPCTSTR) CString(sItem));
	
	CString strItem = sItem;
	return DoUnsubscribe(strItem);
}

/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*	<B>UnsubscribeStringList</B>
*
*	Description = unsubscribe item list from opc server 
*	<p>
*	Specification = 
*	<p>
*	@param sItems  item paths (without common path) seperated by '|'
*	<p>
*	@param sCommonPath path that is common to all items, e.g. "[w-xx].Resource1.". sCommonPath may be empty 
*	<p>
*	@return
*	<p>
*
*/
STDMETHODIMP CAsyncAccess::UnsubscribeStringList(BSTR sItems, BSTR sCommonPath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    //TRACE(_T("Unsubscribe List%s\n"), (LPCTSTR) CString(sItems));
 
	HRESULT hr = S_OK;
	CString strCommonPath=sCommonPath;
	CString strItems=sItems;
	CItemStringList itemList(strCommonPath,strItems);

	for (int ii=0; ii<itemList.GetSize(); ii++)
	{
		HRESULT hr2=DoUnsubscribe(itemList.GetAt(ii));
		if (hr2 != S_OK)
			hr = S_FALSE;
	}
	return hr;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*	<B>Write</B>
*
*	Description = write item value to opc server 
*	<p>
*	Specification = 
*	<p>
*	@param sItems  item paths 
*	<p>
*	@param newVal  new value of item 
*	<p>
*	@return
*	<p>
*
*/
STDMETHODIMP CAsyncAccess::Write(BSTR sItem, VARIANT newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	TRACE(_T("Write %s\n"), (LPCTSTR) CString(sItem));

	CString strItem=sItem;
    return DoWrite(strItem, newVal);
}

/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*	<B>get_Value</B>
*
*	Description = get item value from opc server 
*	<p>
*	Specification = 
*	<p>
*	@param sItems  item paths 
*	<p>
*	@param pVal  value of item 
*	<p>
*	@return
*	<p>
*
*/
STDMETHODIMP CAsyncAccess::get_Value(BSTR sItem, VARIANT *pVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
//    TRACE(_T("CSyncAccess::get_value on item %s\n"), (LPCTSTR) CString(sItem));

    if (pVal == NULL) {
        return E_POINTER;
    }
    // separate config name from pure variable name
    CString strItem = sItem;
    return DoRead(strItem, pVal);
}

/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*	<B>put_Value</B>
*
*	Description = write item value to opc server 
*	<p>
*	Specification = 
*	<p>
*	@param sItems  item paths 
*	<p>
*	@param newVal  new value of item 
*	<p>
*	@return
*	<p>
*
*/
STDMETHODIMP CAsyncAccess::put_Value(BSTR sItem, VARIANT newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
//    TRACE(_T("CSyncAccess::put_value on item %s\n"), (LPCTSTR) CString(sItem));
   
	CString strItem=sItem;
    return DoWrite(strItem, newVal);
}


HRESULT CAsyncAccess::DoSubscribe(const CString& strItem)
{
    CSingleLock lock(&m_cs, TRUE);
    CString strKey = strItem;
    strKey.MakeLower();
    CItem *pItem;
    if (m_itemNameMap.Lookup(strKey, pItem))
      return S_FALSE;
    CComObject<CItem> *pNewItem = NULL;
    HRESULT hr = CComObject<CItem>::CreateInstance(&pNewItem);
    if (FAILED(hr))
      return hr;
    pItem = pNewItem;
    pItem->AddRef();
    hr = pItem->Init(strItem);
    if (FAILED(hr)) {
      pItem->Release();
      return hr;
    }
    m_itemNameMap.SetAt(strKey, pItem);
    return S_OK;
}
	

HRESULT CAsyncAccess::DoUnsubscribe(const CString& strItem)
{
    CSingleLock lock(&m_cs, TRUE);
    CString strKey = strItem;
    strKey.MakeLower();
    CItem *pItem = NULL;
    if (!m_itemNameMap.Lookup(strKey, pItem))
      return E_FAIL;
    m_itemNameMap.RemoveKey(strKey);
    pItem->Cleanup();
    pItem->Release();
    return S_OK;
}

HRESULT CAsyncAccess::DoRead(const CString& strItem, VARIANT *pValue)
{
    CSingleLock lock(&m_cs, TRUE);
    CString strKey = strItem;
    strKey.MakeLower();
    CItem *pItem = NULL;
    if (!m_itemNameMap.Lookup(strKey, pItem))
      return E_FAIL;
    return pItem->GetQVT(pValue, NULL, NULL);
}

HRESULT CAsyncAccess::DoReadString(const CString& strItem, CString& strValue)
{
    CSingleLock lock(&m_cs, TRUE);
    CString strKey = strItem;
    strKey.MakeLower();
    CItem *pItem = NULL;
    if (!m_itemNameMap.Lookup(strKey, pItem))
    {
        m_valueFormatter.GetBadQVTString(strValue);
        return E_FAIL;
    }
    CComVariant value;
    WORD quality;
    FILETIME time;
    HRESULT hr = pItem->GetQVT(&value, &quality, &time);
    if (FAILED(hr))
    {
        m_valueFormatter.GetBadQVTString(strValue);
        return hr;
    }

    m_valueFormatter.ValueToQVTString(value, quality, time, strValue);
    return S_OK;
}

HRESULT CAsyncAccess::DoReadStringTyped(const CString& strItem,const CString& strType, CString& strValue)
{
    CSingleLock lock(&m_cs, TRUE);
    CString strKey = strItem;
    strKey.MakeLower();
    CItem *pItem = NULL;
    if (!m_itemNameMap.Lookup(strKey, pItem))
    {
        m_valueFormatter.GetBadQVTString(strValue);
        return E_FAIL;
    }
    CComVariant value;
    WORD quality;
    FILETIME time;
    HRESULT hr = pItem->GetQVT(&value, &quality, &time);
    if (FAILED(hr))
    {
        m_valueFormatter.GetBadQVTString(strValue);
        return hr;
    }

    m_valueFormatter.ValueToQVTStringIEC(value, quality, time, strType, strValue);
    return S_OK;
}

HRESULT CAsyncAccess::DoReadStringTypedAndRaw(const CString& strItem,const CString& strType, CString& strValue)
{
    CSingleLock lock(&m_cs, TRUE);
    CString strKey = strItem;
    strKey.MakeLower();
    CItem *pItem = NULL;
    if (!m_itemNameMap.Lookup(strKey, pItem))
    {
        m_valueFormatter.GetBadQVTStringRaw(strValue);
        return E_FAIL;
    }
    CComVariant value;
    WORD quality;
    FILETIME time;
    HRESULT hr = pItem->GetQVT(&value, &quality, &time);
    if (FAILED(hr))
    {
        m_valueFormatter.GetBadQVTStringRaw(strValue);
        return hr;
    }

    m_valueFormatter.ValueToQVTStringIECandRaw(value, quality, time, strType, strValue);
    return S_OK;
}

HRESULT CAsyncAccess::DoWrite(const CString& strItem, VARIANT newVal)
{
    CSingleLock lock(&m_cs, TRUE);
    CString strKey = strItem;
    strKey.MakeLower();
    CItem *pItem = NULL;
    if (!m_itemNameMap.Lookup(strKey, pItem))
        return E_FAIL;
    if (newVal.vt == (VT_VARIANT | VT_BYREF))
        newVal = *newVal.pvarVal;
    if (newVal.vt & VT_BYREF)
        return E_INVALIDARG;
    return pItem->WriteValue(newVal);
}


//------------------------------------------------------------------*
/**
 * ConnectToControlMgt:
 * helper function to get the interface to the start/stop functions.
 * The returned IDispatch interface is actually of type IID_IControlMgt!
 * To avoid problems when the type library is not installed all
 * functions from IControlMgt are called via IDispatch::Invoke
 */
static HRESULT ConnectToControlMgt(LPCTSTR pszNodeName, BSTR sPassword, IDispatch **ppUnk, long* plSessionKey)
{
    if (ppUnk == NULL) {
        return E_POINTER;
    }

    *ppUnk = NULL;
    *plSessionKey = 0;

    USES_CONVERSION;
    MULTI_QI queue[1];
    queue[0].pIID = &IID_IDispatch;
    queue[0].pItf = NULL;
    queue[0].hr = 0;
    ULONG cmq = 1;
	DWORD clsctx;

    // set up server node info:
    COSERVERINFO     serverInfo;
    COSERVERINFO   * pServerInfo = &serverInfo;
	if ((pszNodeName != NULL) && (_tcslen(pszNodeName) > 0))
	{
		pServerInfo = &serverInfo;
		serverInfo.dwReserved1 = 0;
		serverInfo.dwReserved2 = 0;
		serverInfo.pwszName = T2OLE(pszNodeName);
		serverInfo.pAuthInfo = 0;
		clsctx = CLSCTX_REMOTE_SERVER | CLSCTX_LOCAL_SERVER;
	} else
	{
		// try local server
		pServerInfo = 0;		// pointer should be NULL if local
		clsctx = CLSCTX_LOCAL_SERVER;
	}


    //HACK hardcode from C:\4c2\COM\softing\fc\AddOns\Softing\4CPCTarget\inc\CSC_OnlSrv.idl
    //CLSID_ControlMgt 379165A5-C2BF-47a8-8F60-73EDE1FD6001
    CLSID CLSIDControlMgt; //HACK
    CLSIDFromString(L"{379165A5-C2BF-47a8-8F60-73EDE1FD6001}", &CLSIDControlMgt);//HACK

    HRESULT hr = CoCreateInstanceEx(CLSIDControlMgt, NULL, clsctx, pServerInfo, cmq, queue);

    if(SUCCEEDED(hr)) {
        *ppUnk = reinterpret_cast<IDispatch *>(queue[0].pItf);
        if (*ppUnk == NULL)
        {
            hr = E_FAIL;
        }
        else
        {
            VARIANTARG args[2];
            VARIANT    varRet;
            DISPPARAMS disp = {args, NULL, 2, 0};

            VariantInit(&args[0]);
            VariantInit(&args[1]);
            VariantInit(&varRet);
 
            args[1].vt      = VT_BSTR;
            args[1].bstrVal = SysAllocString(L"");
            args[0].vt      = VT_BSTR;
            args[0].bstrVal = SysAllocString(sPassword);


            //hr = (*ppUnk)->Login(L""/*BSTR sUser*/, L"" /* BSTR sPassword*/, plSessionKey); //TODO
            hr = (*ppUnk)->Invoke(1, IID_NULL, LOCALE_NEUTRAL, DISPATCH_METHOD,
                                     &disp, &varRet, NULL, NULL);

            if(FAILED(hr))
            {
                (*ppUnk)->Release();
                *ppUnk = NULL;
            }
            else
            {
                ASSERT(varRet.vt==VT_I4);
                *plSessionKey = varRet.lVal;
            }
            VariantClear(&args[0]);
            VariantClear(&args[1]);
            VariantClear(&varRet);
        }
    }

	return hr;
}

void LogoutControlMgt(IDispatch* pControlMgt, long lSessionKey)
{
    HRESULT    hr;
    VARIANTARG arg;
    DISPPARAMS disp = {&arg, NULL, 1, 0};

    VariantInit(&arg);

    arg.vt   = VT_I4;
    arg.lVal = lSessionKey;

    //hr = pControlMgt->Logout(lSessionKey);
    hr = pControlMgt->Invoke(2, IID_NULL, LOCALE_NEUTRAL, DISPATCH_METHOD,
                             &disp, NULL, NULL, NULL);

    VariantClear(&arg);
    ASSERT(SUCCEEDED(hr));
}



/**
 * the following helper function are used by the console to start/stop resources and tasks.
 * purpose: replace CSC_Online.ocx completely by CSC_SimpleOPC
 */
STDMETHODIMP CAsyncAccess::StartResource( 
            /* [string][in] */ BSTR sFullResourcePath,
            /* [string][in] */ BSTR sPassword)
{
    CString            strPath = sFullResourcePath;
    CString            strMachine, strTarget;
    long               lSessionKey;
    CComPtr<IDispatch> pControlMgt;


    HRESULT hr = SplitMachineName(strPath, strMachine, strTarget);
    if (FAILED(hr))
        return hr;

    hr = ConnectToControlMgt(strMachine, sPassword, &pControlMgt, &lSessionKey);
    if (SUCCEEDED(hr))
    {
        VARIANTARG arg;
        DISPPARAMS disp = {&arg, NULL, 1, 0};

        VariantInit(&arg);

        arg.vt   = VT_I4;
        arg.lVal = lSessionKey;

        //hr = pControlMgt->ContinueResource(lSessionKey);
        hr = pControlMgt->Invoke(6, IID_NULL, LOCALE_NEUTRAL, DISPATCH_METHOD,
                                 &disp, NULL, NULL, NULL);
        VariantClear(&arg);
        LogoutControlMgt(pControlMgt, lSessionKey);    
    }

    return hr;
}

STDMETHODIMP CAsyncAccess::StopResource( 
            /* [string][in] */ BSTR sFullResourcePath,
            /* [string][in] */ BSTR sPassword)
{
    CString strPath = sFullResourcePath;
    CString strMachine, strTarget;
    long    lSessionKey;      

    HRESULT hr = SplitMachineName(strPath, strMachine, strTarget);
    if (FAILED(hr))
        return hr;

    CComPtr<IDispatch> pControlMgt;
    hr = ConnectToControlMgt(strMachine, sPassword, &pControlMgt, &lSessionKey);
    if (SUCCEEDED(hr))
    {
        VARIANTARG arg;
        DISPPARAMS disp = {&arg, NULL, 1, 0};

        VariantInit(&arg);

        arg.vt   = VT_I4;
        arg.lVal = lSessionKey;

        //hr = pControlMgt->PauseResource(lSessionKey);
        hr = pControlMgt->Invoke(5, IID_NULL, LOCALE_NEUTRAL, DISPATCH_METHOD,
                                 &disp, NULL, NULL, NULL);
        VariantClear(&arg);
        LogoutControlMgt(pControlMgt, lSessionKey);    
    }

    return hr;
}

STDMETHODIMP CAsyncAccess::StartAllTasks( 
            /* [string][in] */ BSTR sFullResourcePath,
            /* [string][in] */ BSTR sPassword)
{
    CString strPath = sFullResourcePath;
    CString strMachine, strTarget;
    long    lSessionKey;      

    HRESULT hr = SplitMachineName(strPath, strMachine, strTarget);
    if (FAILED(hr))
        return hr;

    CComPtr<IDispatch> pControlMgt;
    hr = ConnectToControlMgt(strMachine, sPassword, &pControlMgt, &lSessionKey);
    if (SUCCEEDED(hr))
    {
        VARIANTARG arg;
        DISPPARAMS disp = {&arg, NULL, 1, 0};

        VariantInit(&arg);

        arg.vt   = VT_I4;
        arg.lVal = lSessionKey;

        //hr = pControlMgt->StartAllTasksOnResource(lSesseionKey);
        hr = pControlMgt->Invoke(9, IID_NULL, LOCALE_NEUTRAL, DISPATCH_METHOD,
                                 &disp, NULL, NULL, NULL);
        VariantClear(&arg);
        LogoutControlMgt(pControlMgt, lSessionKey);    
    }

    return hr;
}

STDMETHODIMP CAsyncAccess::StopAllTasks( 
            /* [string][in] */ BSTR sFullResourcePath,
            /* [string][in] */ BSTR sPassword)
{
    CString strPath = sFullResourcePath;
    CString strMachine, strTarget;
    long    lSessionKey;      

    HRESULT hr = SplitMachineName(strPath, strMachine, strTarget);
    if (FAILED(hr))
        return hr;

    CComPtr<IDispatch> pControlMgt;
    hr = ConnectToControlMgt(strMachine, sPassword, &pControlMgt, &lSessionKey);
    if (SUCCEEDED(hr))
    {
        VARIANTARG arg;
        DISPPARAMS disp = {&arg, NULL, 1, 0};

        VariantInit(&arg);

        arg.vt   = VT_I4;
        arg.lVal = lSessionKey;

        //hr = pControlMgt->StopAllTasksOnResource(lSessionKey);
        hr = pControlMgt->Invoke(10, IID_NULL, LOCALE_NEUTRAL, DISPATCH_METHOD,
                                 &disp, NULL, NULL, NULL);
        VariantClear(&arg);
        LogoutControlMgt(pControlMgt, lSessionKey);
    }

    return hr;
}
        
STDMETHODIMP CAsyncAccess::StartTask( 
            /* [string][in] */ BSTR sFullTaskPath,
            /* [string][in] */ BSTR sPassword)
{
    CString strPath = sFullTaskPath;
    CString strMachine, strTarget;
    long    lSessionKey;      

    HRESULT hr = SplitMachineName(strPath, strMachine, strTarget);
    if (FAILED(hr))
        return hr;

    int i = strTarget.ReverseFind('.');
    if (i<=0)
        return E_FAIL;

    CString strTask = strTarget.Mid(i+1);
    CString strRes  = strTarget.Left(i);

    CComPtr<IDispatch> pControlMgt;
    hr = ConnectToControlMgt(strMachine, sPassword, &pControlMgt, &lSessionKey);
    if (SUCCEEDED(hr))
    {
        VARIANTARG args[2];
        DISPPARAMS disp = {args, NULL, 2, 0};

        VariantInit(&args[0]);
        VariantInit(&args[1]);

        args[1].vt      = VT_I4;
        args[1].lVal    = lSessionKey;
        args[0].vt      = VT_BSTR;
        args[0].bstrVal = strTask.AllocSysString();

        //hr = pControlMgt->StartTaskOnResource(lSessionKey, CComBSTR(strTask));
        hr = pControlMgt->Invoke(7, IID_NULL, LOCALE_NEUTRAL, DISPATCH_METHOD,
                                 &disp, NULL, NULL, NULL);
        VariantClear(&args[0]);
        VariantClear(&args[1]);
        LogoutControlMgt(pControlMgt, lSessionKey);
    }

    return hr;
}
        
STDMETHODIMP CAsyncAccess::StopTask( 
            /* [string][in] */ BSTR sFullTaskPath,
            /* [string][in] */ BSTR sPassword)
{
    CString strPath = sFullTaskPath;
    CString strMachine, strTarget;
    long    lSessionKey;      

    HRESULT hr = SplitMachineName(strPath, strMachine, strTarget);
    if (FAILED(hr))
        return hr;

    int i = strTarget.ReverseFind('.');
    if (i<=0)
        return E_FAIL;

    CString strTask = strTarget.Mid(i+1);
    CString strRes  = strTarget.Left(i);

    CComPtr<IDispatch> pControlMgt;
    hr = ConnectToControlMgt(strMachine, sPassword, &pControlMgt, &lSessionKey);
    if (SUCCEEDED(hr))
    {
        VARIANTARG args[2];
        DISPPARAMS disp = {args, NULL, 2, 0};

        VariantInit(&args[0]);
        VariantInit(&args[1]);

        args[1].vt      = VT_I4;
        args[1].lVal    = lSessionKey;
        args[0].vt      = VT_BSTR;
        args[0].bstrVal = strTask.AllocSysString();

        //hr = pControlMgt->StopTaskOnResource(lSessionKey, CComBSTR(strTask));
        hr = pControlMgt->Invoke(8, IID_NULL, LOCALE_NEUTRAL, DISPATCH_METHOD,
                                 &disp, NULL, NULL, NULL);
        VariantClear(&args[0]);
        VariantClear(&args[1]);
        LogoutControlMgt(pControlMgt, lSessionKey);
    }

    return hr;
}



/*
 *----------------------------------------------------------------------------*
 *  $History: AsyncAccess.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CSC/AsyncOPC
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CSC/AsyncOPC
 * 
 * *****************  Version 6  *****************
 * User: Ed           Date: 9/03/01    Time: 3:22p
 * Updated in $/4Control/COM/softing/fc/CSC/AsyncOPC
 * handle session password
 * 
 * *****************  Version 5  *****************
 * User: Ef           Date: 20.08.01   Time: 12:18
 * Updated in $/4Control/COM/softing/fc/CSC/AsyncOPC
 * fixed bug in StopAllTasks 
 * 
 * *****************  Version 4  *****************
 * User: Ef           Date: 13.08.01   Time: 17:20
 * Updated in $/4Control/COM/softing/fc/CSC/AsyncOPC
 * adapted all invoke calls to V2.0 interfaces
 * 
 * *****************  Version 3  *****************
 * User: Ef           Date: 8.08.01    Time: 18:15
 * Updated in $/4Control/COM/softing/fc/CSC/AsyncOPC
 * adapted to changes of V2.0
 * 
 * *****************  Version 2  *****************
 * User: Ca           Date: 8.08.01    Time: 9:56
 * Updated in $/4Control/COM/softing/fc/CSC/AsyncOPC
 * 
 * *****************  Version 1  *****************
 * User: Ca           Date: 8.08.01    Time: 9:41
 * Created in $/4Control/COM/softing/fc/CSC/AsyncOPC
 * copied from V1.3
 *==
 *----------------------------------------------------------------------------*
*/
