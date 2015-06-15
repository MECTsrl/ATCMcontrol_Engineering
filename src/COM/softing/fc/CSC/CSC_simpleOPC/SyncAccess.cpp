/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_simpleOPC/SyncAccess.cpp 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: SyncAccess.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_simpleOPC/SyncAccess.cpp $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                4CSC
 *
 * =COMPONENT           simple sync OPC client
 *
 * =CURRENT      $Date: 28.02.07 19:00 $
 *               $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    Simple Client for 4C CSC Online Server (OPC++).
 *    CSC_SimpleOPC provides synchronous read and write operations. The list
 *    operations are optimized regarding the number of opc (dcom) calls.
 *    CSyncAcces is a wrapper for CItemContainer which holds the items/groups of the 
 *    opc servers. 
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  07.08.01  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/



//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "SyncAccess.h"
#include "ItemContainer.h"
#include "ItemStringList.h"
#include "CSC_OnlineServer.h"

#include "SyncAccessGlobals.h"

//----  Static Initializations:   ----------------------------------*

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSyncAccess

//------------------------------------------------------------------*
CSyncAccess::CSyncAccess()
  : m_strServerNameOPC(_T("4CONTROL_CSC.Online_Server"))
{ 
//    TRACE(_T("ctor CSyncAccess\n"));
    m_bOnStartPageCalled = FALSE;
    m_pControlMgt = NULL;
    m_lSessionKey = 0;
}

//------------------------------------------------------------------*
CSyncAccess::~CSyncAccess()
{ 
//    TRACE(_T("dor CSyncAccess\n"));
    _ASSERTE(!m_pControlMgt);
}



/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*	<B>FinalRelease</B>
*
*	Description = delete item containers
*	<p>
*	Specification = 
*	<p>
*	@exception
*	<p>
*	@see
*	<p>
*/
void CSyncAccess::FinalRelease()
{
    TRACE(_T("CSyncAccess::FinalRelease\n"));
    for (POSITION pos = m_configNameMap.GetStartPosition(); pos != NULL; )
    {
        CString strKey;
        CItemContainer *pContainer = NULL;
        m_configNameMap.GetNextAssoc(pos, strKey, pContainer);
        _ASSERTE(pContainer != NULL);
        delete pContainer;
    }
    m_configNameMap.RemoveAll();
    if(m_pControlMgt)
    {
        m_pControlMgt->Logout(m_lSessionKey);
        m_pControlMgt = NULL;
        m_lSessionKey = 0;
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
STDMETHODIMP CSyncAccess::Subscribe(BSTR sItem)
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
STDMETHODIMP CSyncAccess::SubscribeStringList(BSTR sItems, BSTR sCommonPath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    //TRACE(_T("Subscribe List %s\n"), (LPCTSTR) CString(sItems));

	CString strCommonPath=sCommonPath;
	CString strItems=sItems;
    return DoSubscribeStringList(strItems, strCommonPath);
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
STDMETHODIMP CSyncAccess::ReadStringList(BSTR sItems, BSTR sCommonPath, BSTR *psValues)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    //TRACE(_T("Read List %s\n"), (LPCTSTR) CString(sItems));

	if (psValues == NULL) {
        return E_POINTER;
    }
    *psValues = NULL;

	CString strCommonPath=sCommonPath;
	CString strItems=sItems;
	CString strValues;
	   
	//TRACE(_T("Read List %s\n"), (LPCTSTR) strItems.Left(500));
    HRESULT hr = DoReadStringList(strItems, strCommonPath, strValues);
	//TRACE(_T("Read List returned %s\n"), (LPCTSTR) strValues.Left(490));

    *psValues = strValues.AllocSysString();
	
	hr=S_OK; //return always S_OK due to problems in IE4
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
STDMETHODIMP CSyncAccess::ReadStringListTyped(BSTR sItems, BSTR sCommonPath, BSTR *psValues)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    //TRACE(_T("Read List Typed%s\n"), (LPCTSTR) CString(sItems));

	if (psValues == NULL) {
        return E_POINTER;
    }
    *psValues = NULL;
	
	CString strCommonPath=sCommonPath;
	CString strItems=sItems;
	CString strValues;
    HRESULT hr = DoReadStringListTyped(strItems, strCommonPath, strValues);

    *psValues = strValues.AllocSysString();
	
	hr=S_OK; //return always S_OK due to problems in IE4
	return hr;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*	<B>ReadCachedStringListTyped</B>
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
STDMETHODIMP CSyncAccess::ReadCachedStringListTyped(BSTR sItems, BSTR sCommonPath, BSTR *psValues)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    //TRACE(_T("Read List Typed%s\n"), (LPCTSTR) CString(sItems));

	if (psValues == NULL) {
        return E_POINTER;
    }
    *psValues = NULL;
	
	CString strCommonPath=sCommonPath;
	CString strItems=sItems;
	CString strValues;
    HRESULT hr = DoReadCachedStringListTyped(strItems, strCommonPath, strValues);

    *psValues = strValues.AllocSysString();
	
	hr=S_OK; //return always S_OK due to problems in IE4
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
STDMETHODIMP CSyncAccess::Unsubscribe(BSTR sItem)
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
STDMETHODIMP CSyncAccess::UnsubscribeStringList(BSTR sItems, BSTR sCommonPath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    //TRACE(_T("Unsubscribe List%s\n"), (LPCTSTR) CString(sItems));
 
	CString strCommonPath=sCommonPath;
	CString strItems=sItems;
    return DoUnsubscribeStringList(strItems, strCommonPath);
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
STDMETHODIMP CSyncAccess::Write(BSTR sItem, VARIANT newVal)
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
STDMETHODIMP CSyncAccess::get_Value(BSTR sItem, VARIANT *pVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
//    TRACE(_T("CSyncAccess::get_value on item %s\n"), (LPCTSTR) CString(sItem));

    if (pVal == NULL) {
        return E_POINTER;
    }
    // separate config name from pure variable name
    CString strPureVarName;
    // lookup container
    CItemContainer *pContainer = LookupContainer(sItem, strPureVarName);
    if (pContainer == NULL) {
        return E_FAIL;
    }
    
    _ASSERTE(pContainer != NULL);
    WORD wQuality;
    FILETIME ftTimeStamp;
    HRESULT hr;
    hr = pContainer->ReadItem(strPureVarName, pVal, wQuality, ftTimeStamp);
    return hr;
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
STDMETHODIMP CSyncAccess::put_Value(BSTR sItem, VARIANT newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
//    TRACE(_T("CSyncAccess::put_value on item %s\n"), (LPCTSTR) CString(sItem));
   
	CString strItem=sItem;
    return DoWrite(strItem, newVal);
}


//------------------------------------------------------------------*
STDMETHODIMP
    CSyncAccess::InterfaceSupportsErrorInfo(REFIID riid)
{
    static const IID* arr[] = 
    {
        &IID_ISyncAccess,
    };
    for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
    {
        if (InlineIsEqualGUID(*arr[i],riid))
            return S_OK;
    }
    return S_FALSE;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CSyncAccess::OnStartPage (IUnknown* pUnk)  
{
    TRACE(_T("CSyncAccess::OnStartPage\n"));
    if(!pUnk)
        return E_POINTER;

/*
    CComPtr<IScriptingContext> spContext;
    HRESULT hr;

    // Get the IScriptingContext Interface
    hr = pUnk->QueryInterface(IID_IScriptingContext, (void **)&spContext);
    if(FAILED(hr))
        return hr;

    // Get Request Object Pointer
    hr = spContext->get_Request(&m_piRequest);
    if(FAILED(hr))
    {
        spContext.Release();
        return hr;
    }

    // Get Response Object Pointer
    hr = spContext->get_Response(&m_piResponse);
    if(FAILED(hr))
    {
        m_piRequest.Release();
        return hr;
    }
    
    // Get Server Object Pointer
    hr = spContext->get_Server(&m_piServer);
    if(FAILED(hr))
    {
        m_piRequest.Release();
        m_piResponse.Release();
        return hr;
    }
    
    // Get Session Object Pointer
    hr = spContext->get_Session(&m_piSession);
    if(FAILED(hr))
    {
        m_piRequest.Release();
        m_piResponse.Release();
        m_piServer.Release();
        return hr;
    }

    // Get Application Object Pointer
    hr = spContext->get_Application(&m_piApplication);
    if(FAILED(hr))
    {
        m_piRequest.Release();
        m_piResponse.Release();
        m_piServer.Release();
        m_piSession.Release();
        return hr;
    }
*/
    m_bOnStartPageCalled = TRUE;
    return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CSyncAccess::OnEndPage ()  
{
    TRACE(_T("CSyncAccess::OnEndPage\n"));
    m_bOnStartPageCalled = FALSE;
    // Release all interfaces
/*
    m_piRequest.Release();
    m_piResponse.Release();
    m_piServer.Release();
    m_piSession.Release();
    m_piApplication.Release();
*/
    return S_OK;
}

HRESULT CSyncAccess::DoSubscribe(const CString& strItem)
{

	CString strPureVarName;

	// lookup container, create if not in map yet!
	CItemContainer *pContainer = LookupContainer((LPCTSTR) strItem, strPureVarName, TRUE);
	_ASSERTE(pContainer != NULL);
	if (pContainer == NULL) {
		return E_FAIL;
	}

	return pContainer->AddItem(strPureVarName);
}
	
HRESULT CSyncAccess::DoSubscribeStringList(const CString& strItems, const CString& strCommonPath)
{
	if (strCommonPath == "")
		return DoSubscribeStringListOpt(strItems);

	//
	// separate config name from pure variable name
	//
    CString strPureVarName;
    
	//
	// lookup container, create if not in map yet!
	//
    CItemContainer *pContainer = LookupContainer(strCommonPath, strPureVarName, TRUE);
    _ASSERTE(pContainer != NULL);
    
	if (pContainer == NULL) {
        return E_FAIL;
    }
	
    return pContainer->AddItemList(strPureVarName, strItems);
}

HRESULT CSyncAccess::DoSubscribeStringList(const CString& strItems)
{
	HRESULT hr = S_OK;
	CItemStringList itemList("",strItems);

	for (int ii=0; ii<itemList.GetSize(); ii++)
	{
		HRESULT hr2=DoSubscribe(itemList.GetAt(ii));
		if (hr2 != S_OK)
			hr = S_FALSE;
	}
	return hr;
}

HRESULT CSyncAccess::DoSubscribeStringListOpt(const CString& strItems)
{
	HRESULT hr = S_OK;
	CItemStringList itemList("",strItems);
	CString strGroupItems, strGroupCommonPath;

	int nn=itemList.GetGroupCount();
	for (int ii=0; ii<nn; ii++)
	{
		itemList.GetGroup(ii, strGroupItems, strGroupCommonPath);
		if (strGroupCommonPath == "")
		{
			hr = DoSubscribeStringList(strGroupItems);
		}
		else
		{
			hr = DoSubscribeStringList(strGroupItems, strGroupCommonPath);
		}
	}

	return hr;
}

HRESULT CSyncAccess::DoUnsubscribe(const CString& strItem)
{
    CString strPureVarName;
    // lookup container
    CItemContainer *pContainer = LookupContainer((LPCTSTR) strItem, strPureVarName);
    if (pContainer == NULL) {
        return E_FAIL;
    }
    
    _ASSERTE(pContainer != NULL);
    HRESULT hr;
    hr = pContainer->RemoveItem(strPureVarName);
    // check if container is empty now
    if (pContainer->IsEmpty())
    {
        // remove if empty
        CString strConfig = pContainer->getConfigName();
        m_configNameMap.RemoveKey(strConfig);
        delete pContainer;
        pContainer = NULL;
    }

    return hr;
}

HRESULT CSyncAccess::DoUnsubscribeStringList(const CString& strItems, const CString& strCommonPath)
{
	if (strCommonPath == "")
		return DoUnsubscribeStringListOpt(strItems);

	//
	// separate config name from pure variable name
	//
    CString strPureVarName;
    
	//
	// lookup container
	//
    CItemContainer *pContainer = LookupContainer(strCommonPath, strPureVarName);
    
	if (pContainer == NULL) {
        return E_FAIL;
    }
	
    HRESULT hr=pContainer->RemoveItemList(strPureVarName, strItems);
    
	if (pContainer->IsEmpty())
    {
        // remove if empty
        CString strConfig = pContainer->getConfigName();
        m_configNameMap.RemoveKey(strConfig);
        delete pContainer;
        pContainer = NULL;
    }
	return hr;
}

HRESULT CSyncAccess::DoUnsubscribeStringList(const CString& strItems)
{
	HRESULT hr = S_OK;
	CItemStringList itemList("",strItems);
	CString strPureVarName;

	for (int ii=0; ii<itemList.GetSize(); ii++)
	{
		HRESULT hr2=DoUnsubscribe(itemList.GetAt(ii));
		if (hr2 != S_OK)
			hr = S_FALSE;
	}
	return hr;
}

HRESULT CSyncAccess::DoUnsubscribeStringListOpt(const CString& strItems)
{
	HRESULT hr = S_OK;
	CItemStringList itemList("",strItems);
	CString strGroupItems, strGroupCommonPath;

	int nn=itemList.GetGroupCount();
	for (int ii=0; ii<nn; ii++)
	{
		itemList.GetGroup(ii, strGroupItems, strGroupCommonPath);
		if (strGroupCommonPath == "")
		{
			hr = DoUnsubscribeStringList(strGroupItems);
		}
		else
		{
			hr = DoUnsubscribeStringList(strGroupItems, strGroupCommonPath);
		}
	}

	return hr;
}

HRESULT CSyncAccess::DoReadString(const CString& strItem, CString& strValue)
{
	//
	// separate config name from pure variable name
	//
    CString strPureVarName;
    
	//
	// lookup container, create if not in map yet!
	//
    CItemContainer *pContainer = LookupContainer(strItem, strPureVarName, TRUE);
    _ASSERTE(pContainer != NULL);
    
	if (pContainer == NULL) {
        return E_FAIL;
    }
	
    return pContainer->ReadItemList("", strPureVarName, strValue);
}

HRESULT CSyncAccess::DoReadStringList(const CString& strItems, const CString& strCommonPath, CString& strValues)
{
	if (strCommonPath == "")
		return DoReadStringListOpt(strItems, strValues);

	//
	// separate config name from pure variable name
	//
    CString strPureVarName;
    
	//
	// lookup container, create if not in map yet!
	//
    CItemContainer *pContainer = LookupContainer(strCommonPath, strPureVarName, TRUE);
    _ASSERTE(pContainer != NULL);
    
	if (pContainer == NULL) {
        return E_FAIL;
    }
	
    return pContainer->ReadItemList(strPureVarName, strItems, strValues);
}

HRESULT CSyncAccess::DoReadStringList(const CString& strItems, CString& strValues)
{
	HRESULT hr = S_OK;
	CItemStringList itemList("",strItems);
	CString strValue;

	for (int ii=0; ii<itemList.GetSize(); ii++)
	{
		HRESULT hr2=DoReadString(itemList.GetAt(ii), strValue);
		if (hr2 != S_OK)
			hr = S_FALSE;
		strValues += strValue;
		if (ii<itemList.GetSize()-1)
			strValues+='|';
	}
	return hr;
}

HRESULT CSyncAccess::DoReadStringListOpt(const CString& strItems, CString& strValues)
{
	HRESULT hr = S_OK;
	CItemStringList itemList("",strItems);
	CString strGroupItems, strGroupCommonPath, strGroupValues;

	int nn=itemList.GetGroupCount();
	for (int ii=0; ii<nn; ii++)
	{
		itemList.GetGroup(ii, strGroupItems, strGroupCommonPath);
		if (strGroupCommonPath == "")
		{
			hr = DoReadStringList(strGroupItems, strGroupValues);
		}
		else
		{
			hr = DoReadStringList(strGroupItems, strGroupCommonPath, strGroupValues);
		}
	}

	//
	// now read data from cache
	//
	hr = DoReadCachedStringList(strItems, strValues);
	return hr;
}

HRESULT CSyncAccess::DoReadStringTyped(const CString& strItem,const CString& strType, CString& strValue)
{
		//
	// separate config name from pure variable name
	//
    CString strPureVarName;
    
	//
	// lookup container, create if not in map yet!
	//
    CItemContainer *pContainer = LookupContainer(strItem, strPureVarName, TRUE);
    _ASSERTE(pContainer != NULL);
    
	if (pContainer == NULL) {
        return E_FAIL;
    }
	strPureVarName= strType + "|" + strPureVarName;
    return pContainer->ReadItemListTyped("", strPureVarName, strValue);
}

HRESULT CSyncAccess::DoReadStringListTyped(const CString& strItems, const CString& strCommonPath, CString& strValues)
{
	if (strCommonPath == "")
		return DoReadStringListTyped(strItems, strValues);
	//
	// separate config name from pure variable name
	//
    CString strPureVarName;
    
	//
	// lookup container, create if not in map yet!
	//
    CItemContainer *pContainer = LookupContainer(strCommonPath, strPureVarName, TRUE);
    _ASSERTE(pContainer != NULL);
    
	if (pContainer == NULL) {
        return E_FAIL;
    }
	
    return pContainer->ReadItemListTyped(strPureVarName, strItems, strValues);
}


HRESULT CSyncAccess::DoReadStringListTyped(const CString& strItems, CString& strValues)
{
	HRESULT hr = S_OK;
	CItemStringList itemList("",strItems);
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
	return hr;
}

HRESULT CSyncAccess::DoReadCachedString(const CString& strItem, CString& strValue)
{
	//
	// separate config name from pure variable name
	//
    CString strPureVarName;
    
	//
	// lookup container, create if not in map yet!
	//
    CItemContainer *pContainer = LookupContainer(strItem, strPureVarName, TRUE);
    _ASSERTE(pContainer != NULL);
    
	if (pContainer == NULL) {
        return E_FAIL;
    }
    return pContainer->ReadCachedItemList("", strPureVarName, strValue);
}

HRESULT CSyncAccess::DoReadCachedStringList(const CString& strItems, const CString& strCommonPath, CString& strValues)
{
	if (strCommonPath == "")
		return DoReadCachedStringList(strItems, strValues);
	//
	// separate config name from pure variable name
	//
    CString strPureVarName;
    
	//
	// lookup container, create if not in map yet!
	//
    CItemContainer *pContainer = LookupContainer(strCommonPath, strPureVarName, TRUE);
    _ASSERTE(pContainer != NULL);
    
	if (pContainer == NULL) {
        return E_FAIL;
    }
	
    return pContainer->ReadCachedItemList(strPureVarName, strItems, strValues);
}

HRESULT CSyncAccess::DoReadCachedStringList(const CString& strItems, CString& strValues)
{
	HRESULT hr = S_OK;
	CItemStringList itemList("",strItems);
	CString strValue;

	for (int ii=0; ii<itemList.GetSize(); ii++)
	{
		HRESULT hr2=DoReadCachedString(itemList.GetAt(ii), strValue);
		if (hr2 != S_OK)
			hr = S_FALSE;
		strValues += strValue;
		if (ii<itemList.GetSize()-1)
			strValues+='|';
	}
	return hr;
}

HRESULT CSyncAccess::DoReadCachedStringTyped(const CString& strItem,const CString& strType, CString& strValue)
{
	//
	// separate config name from pure variable name
	//
    CString strPureVarName;
    
	//
	// lookup container, create if not in map yet!
	//
    CItemContainer *pContainer = LookupContainer(strItem, strPureVarName, TRUE);
    _ASSERTE(pContainer != NULL);
    
	if (pContainer == NULL) {
        return E_FAIL;
    }
	strPureVarName= strType + "|" + strPureVarName;
    return pContainer->ReadCachedItemListTyped("", strPureVarName, strValue);
}

HRESULT CSyncAccess::DoReadCachedStringListTyped(const CString& strItems, const CString& strCommonPath, CString& strValues)
{
	if (strCommonPath == "")
		return DoReadCachedStringListTyped(strItems, strValues);
	//
	// separate config name from pure variable name
	//
    CString strPureVarName;
    
	//
	// lookup container, create if not in map yet!
	//
    CItemContainer *pContainer = LookupContainer(strCommonPath, strPureVarName, TRUE);
    _ASSERTE(pContainer != NULL);
    
	if (pContainer == NULL) {
        return E_FAIL;
    }
	
    return pContainer->ReadCachedItemListTyped(strPureVarName, strItems, strValues);
}

HRESULT CSyncAccess::DoReadCachedStringListTyped(const CString& strItems, CString& strValues)
{
	HRESULT hr = S_OK;
	CItemStringList itemList("",strItems);
	CString strValue;

	for (int ii=0; ii<itemList.GetSize()/2; ii++)
	{
		HRESULT hr2=DoReadCachedStringTyped(itemList.GetAt(2*ii+1),itemList.GetAtSimple(2*ii), strValue);
		if (hr2 != S_OK)
			hr = S_FALSE;
		strValues += strValue;
		if (ii<itemList.GetSize()/2-1)
			strValues+='|';
	}
	return hr;
}

HRESULT CSyncAccess::DoWrite(const CString& strItem, VARIANT newVal)
{
    // separate config name from pure variable name
    CString strPureVarName;
    // lookup container
    CItemContainer *pContainer = LookupContainer((LPCTSTR)strItem, strPureVarName);
    if (pContainer == NULL) {
        return E_FAIL;
    }
    
    return pContainer->WriteItem(strPureVarName, newVal);
}

//------------------------------------------------------------------*
CItemContainer *
    CSyncAccess::LookupContainer(LPCTSTR pszVarName, CString& strPureName, BOOL bCreate /*= FALSE*/)
{
    CString strConfig;

    strConfig = GetConfigName(pszVarName, strPureName);
    strConfig.MakeLower();

    // create container if not here yet
    CItemContainer *pContainer = NULL;
    if (   !m_configNameMap.Lookup(strConfig, pContainer)
        && bCreate)
    {
        // new machine/server, create container and add to map
        CString strMachineName;
        // assume config name as escaped machine name!
        CString strRemainder;
        SplitMachineName(strConfig, strMachineName, strRemainder);

        pContainer = new CItemContainer(strConfig, strMachineName);
        if (pContainer == NULL) {
            return NULL;
        }
        pContainer->Connect();
        m_configNameMap.SetAt(strConfig, pContainer);
    }
    return pContainer;
}

//------------------------------------------------------------------*
CItemContainer *
    CSyncAccess::LookupContainer(BSTR sVarName, CString& strPureName, BOOL bCreate /*= FALSE*/)
{
    return LookupContainer(CString(sVarName), strPureName, bCreate);
}


//------------------------------------------------------------------*
/**
 * ConnectToControlMgt:
 * helper function to get the interface to the start/stop functions.
 */
HRESULT ConnectToControlMgt(LPCTSTR pszNodeName, BSTR sPassword, IControlMgt **ppUnk, long* plSessionKey)
{
    if (ppUnk == NULL) {
        return E_POINTER;
    }
    *ppUnk = NULL;

    USES_CONVERSION;
    MULTI_QI queue[1];
    queue[0].pIID = &IID_IControlMgt;
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
        *ppUnk = reinterpret_cast<IControlMgt *>(queue[0].pItf);
        if (*ppUnk == NULL)
            hr = E_FAIL;

        hr = (*ppUnk)->Login(CComBSTR(L""), sPassword, plSessionKey); //TODO
        if(FAILED(hr))
        {
            (*ppUnk)->Release();
            *ppUnk = NULL;
        }
    }

	return hr;
}

/**
 * the following helper function are used by the console to start/stop resources and tasks.
 * purpose: replace CSC_Online.ocx completely by CSC_SimpleOPC
 */
STDMETHODIMP CSyncAccess::StartResource( 
            /* [string][in] */ BSTR sFullResourcePath,
            /* [in][string] */ BSTR sPassword)
{
    CString strPath = sFullResourcePath;
    CString strMachine, strTarget;

    HRESULT hr = SplitMachineName(strPath, strMachine, strTarget);
    if (FAILED(hr))
        return hr;

    if(!m_pControlMgt)
    {
        hr = ConnectToControlMgt(strMachine, sPassword, &m_pControlMgt, &m_lSessionKey);
    }
    if (SUCCEEDED(hr))
    {
        hr = m_pControlMgt->ContinueResource(m_lSessionKey);
    }

    return hr;
}

STDMETHODIMP CSyncAccess::StopResource( 
            /* [string][in] */ BSTR sFullResourcePath,
            /* [in][string] */ BSTR sPassword)
{
    CString strPath = sFullResourcePath;
    CString strMachine, strTarget;

    HRESULT hr = SplitMachineName(strPath, strMachine, strTarget);
    if (FAILED(hr))
        return hr;

    
    if(!m_pControlMgt)
    {
        hr = ConnectToControlMgt(strMachine, sPassword, &m_pControlMgt, &m_lSessionKey);
    }
    if (SUCCEEDED(hr))
    {
        hr = m_pControlMgt->PauseResource(m_lSessionKey);
    }

    return hr;
}

STDMETHODIMP CSyncAccess::StartAllTasks( 
            /* [string][in] */ BSTR sFullResourcePath,
            /* [in][string] */ BSTR sPassword)
{
    CString strPath = sFullResourcePath;
    CString strMachine, strTarget;

    HRESULT hr = SplitMachineName(strPath, strMachine, strTarget);
    if (FAILED(hr))
        return hr;

    if(!m_pControlMgt)
    {
        hr = ConnectToControlMgt(strMachine, sPassword, &m_pControlMgt, &m_lSessionKey);
    }
    if (SUCCEEDED(hr))
    {
        hr = m_pControlMgt->StartAllTasksOnResource(m_lSessionKey);
    }

    return hr;
}

STDMETHODIMP CSyncAccess::StopAllTasks( 
            /* [string][in] */ BSTR sFullResourcePath,
            /* [in][string] */ BSTR sPassword)
{
    CString strPath = sFullResourcePath;
    CString strMachine, strTarget;

    HRESULT hr = SplitMachineName(strPath, strMachine, strTarget);
    if (FAILED(hr))
        return hr;

    if(!m_pControlMgt)
    {
        hr = ConnectToControlMgt(strMachine, sPassword, &m_pControlMgt, &m_lSessionKey);
    }
    if (SUCCEEDED(hr))
    {
        hr = m_pControlMgt->StopAllTasksOnResource(m_lSessionKey);
    }

    return hr;
}
        
STDMETHODIMP CSyncAccess::StartTask( 
            /* [string][in] */ BSTR sFullTaskPath,
            /* [in][string] */ BSTR sPassword)
{
    CString strPath = sFullTaskPath;
    CString strMachine, strTarget;

    HRESULT hr = SplitMachineName(strPath, strMachine, strTarget);
    if (FAILED(hr))
        return hr;

    int i = strTarget.ReverseFind('.');
    if (i<=0)
        return E_FAIL;

    CString strTask = strTarget.Mid(i+1);
    CString strRes  = strTarget.Left(i);

    if(!m_pControlMgt)
    {
        hr = ConnectToControlMgt(strMachine, sPassword, &m_pControlMgt, &m_lSessionKey);
    }
    if (SUCCEEDED(hr))
    {
        hr = m_pControlMgt->StartTaskOnResource(m_lSessionKey, CComBSTR(strTask));
    }

    return hr;
}
        
STDMETHODIMP CSyncAccess::StopTask( 
            /* [string][in] */ BSTR sFullTaskPath,
            /* [in][string] */ BSTR sPassword)
{
    CString strPath = sFullTaskPath;
    CString strMachine, strTarget;

    HRESULT hr = SplitMachineName(strPath, strMachine, strTarget);
    if (FAILED(hr))
        return hr;

    int i = strTarget.ReverseFind('.');
    if (i<=0)
        return E_FAIL;

    CString strTask = strTarget.Mid(i+1);
    CString strRes  = strTarget.Left(i);

    if(!m_pControlMgt)
    {
        hr = ConnectToControlMgt(strMachine, sPassword, &m_pControlMgt, &m_lSessionKey);
    }
    if (SUCCEEDED(hr))
    {
        hr = m_pControlMgt->StopTaskOnResource(m_lSessionKey, CComBSTR(strTask));
    }

    return hr;
}

/*
 *----------------------------------------------------------------------------*
 *  $History: SyncAccess.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_simpleOPC
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CSC/CSC_simpleOPC
 * 
 * *****************  Version 3  *****************
 * User: Ed           Date: 9/03/01    Time: 3:23p
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_simpleOPC
 * handle session password
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 8.08.01    Time: 18:15
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_simpleOPC
 * adapted to changes of V2.0
 * 
 * *****************  Version 1  *****************
 * User: Ca           Date: 8.08.01    Time: 9:59
 * Created in $/4Control/COM/softing/fc/CSC/CSC_simpleOPC
 * copied from V1.3
 *==
 *----------------------------------------------------------------------------*
*/
