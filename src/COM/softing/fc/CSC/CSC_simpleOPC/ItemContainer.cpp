/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_simpleOPC/ItemContainer.cpp 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: ItemContainer.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_simpleOPC/ItemContainer.cpp $
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
 *    container for items for ONE opc group,
 *    values read by list operations are cached
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
#include "ItemContainer.h"
#include "ItemStringList.h"
#include "items.h"

#include "CSC_OnlineServer.h"
#include "CSC_OnlineServer_i.c"

//----  Static Initializations:   ----------------------------------*

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------*
CItemContainer::CItemContainer(LPCTSTR pszConfigName, LPCTSTR pszMachineName)
  : m_dwReqUpdateRate(1000),
    m_dwRevUpdateRate(0)
{
    m_strConfigName  = pszConfigName;
    m_strMachineName = pszMachineName;
	m_pItems=new CItems();
}

//------------------------------------------------------------------*
CItemContainer::~CItemContainer()
{
    Disconnect();
	delete m_pItems;
}

//------------------------------------------------------------------*
HRESULT
    CItemContainer::Connect()
{
    _ASSERTE(!m_strMachineName.IsEmpty());
    

    CLSID clsid;
	HRESULT hr;
    
	//clsid = CLSID_OPCServer;
    //HACK hardcode from C:\4c2\COM\softing\fc\AddOns\Softing\4CPCTarget\inc\CSC_OnlSrv.idl
    //CLSID_OPCServer 379165A5-C2BF-47a8-8F60-73EDE1FD6001
    CLSIDFromString(L"{5425B8A4-754D-496a-AF31-7D78A530E58D}", &clsid);//HACK

    m_pServer = NULL;
    hr = ConnectToServer(m_strMachineName, clsid, IID_IOPCServer, (IUnknown**)&m_pServer);
    if (FAILED(hr)) {
        return hr;
    }
    if (!m_pServer) {
        return E_NOINTERFACE;
    }

    // add OPC group:
    LONG    lTimeBias = 0;
    FLOAT   fltDeadband = 0.0;

    m_pGroup = NULL;
    m_hGroup = NULL;
    hr = m_pServer->AddGroup(L"simpleOPC", FALSE, m_dwReqUpdateRate, 1, &lTimeBias, &fltDeadband,
                             MAKELCID(MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), SORT_DEFAULT),
                             &m_hGroup, &m_dwRevUpdateRate,
                             IID_IOPCItemMgt, (IUnknown**)&m_pGroup);
    if (FAILED(hr)) {
        return hr;
    }

    m_pSync = NULL;
    hr = m_pGroup->QueryInterface(IID_IOPCSyncIO, (void**)&m_pSync);
    if (FAILED(hr)) {
        return hr;
    }

    return S_OK;
}

//------------------------------------------------------------------*
HRESULT
    CItemContainer::Disconnect()
{
    if (!m_pServer) {
        return S_FALSE;
    }

    m_pSync = NULL;
    m_pGroup = NULL;
    if (m_hGroup != NULL) {
        m_pServer->RemoveGroup(m_hGroup, TRUE);
    }
    m_hGroup = NULL;
    m_pServer = NULL;
    return S_OK;
}

//------------------------------------------------------------------*
HRESULT
    CItemContainer::AddItem(LPCTSTR pszItem)
{
    HRESULT hr = S_FALSE;

    OPCHANDLE   hItem = NULL;
    CString strItem = pszItem;
    strItem.MakeLower();
    if (m_pItems->LookupHandle(strItem, hItem)) {
        // already done, variable in map
        return S_FALSE;
    }

    if (m_pGroup)
    {
        // add to OPC group 
        OPCITEMDEF      item;
        USES_CONVERSION;
        item.szAccessPath = NULL;;
        item.szItemID = T2OLE((LPTSTR)pszItem);
        item.bActive = TRUE;
        item.hClient = 0;
        item.dwBlobSize = 0;
        item.pBlob = NULL;
        item.vtRequestedDataType = VT_EMPTY;
        item.wReserved = 0;
        OPCITEMRESULT   *results = NULL;
        HRESULT         *errors = NULL;
        hr = m_pGroup->AddItems(1, &item, &results, &errors);
        if (SUCCEEDED(hr))
        {
            if (FAILED(errors[0])) {
                hItem = 0;
                hr = S_FALSE;
            }
            else {
                hItem = results[0].hServer;
				m_pItems->SetHandle(strItem, hItem);
                hr = S_OK;
            }
            ::CoTaskMemFree(errors);
            ::CoTaskMemFree(results);
        }
    }
    else {
        hr = S_FALSE;
    }
    return hr;
}

//------------------------------------------------------------------*
HRESULT                 
	CItemContainer::AddItemList(LPCTSTR pszCommonPath, LPCTSTR pszItemsList)
{
    HRESULT hr = S_FALSE;
	CString strItems = pszItemsList;
	CString strCommonPath = pszCommonPath;
	
	//
	// create item array with size of list
	//
	CItemStringList itemList(pszCommonPath, strItems);
	if (itemList.GetSize() <= 0)
	{
		return S_FALSE;
	}
    OPCHANDLE hItem = NULL;
    OPCITEMDEF* items = new OPCITEMDEF[itemList.GetSize()];
    OPCITEMRESULT*  results = NULL;
    HRESULT*         errors = NULL;

	//
	// fill item array
	//
	CString strItem;
	USES_CONVERSION;
	int cntValidItems=0;
    for (int ii=0; ii<itemList.GetSize(); ii++)
    {		
		hItem = NULL;
		strItem = itemList.GetAt(ii);
		strItem.MakeLower();
		if ( ! m_pItems->LookupHandle(strItem, hItem) )
		{
			items[cntValidItems].szAccessPath = NULL;
			items[cntValidItems].szItemID = T2OLE((LPTSTR)(LPCTSTR) strItem);
			items[cntValidItems].bActive = TRUE;
			items[cntValidItems].hClient = 0;
			items[cntValidItems].dwBlobSize = 0;
			items[cntValidItems].pBlob = NULL;
			items[cntValidItems].vtRequestedDataType = VT_EMPTY;
			items[cntValidItems].wReserved = 0;	
			cntValidItems++;
		}
	}

	
	if (cntValidItems <=0)
	{
		delete[] items;
		return S_FALSE;
	}
	
	//
	// opc call 
	//
	if (m_pGroup)
		hr = m_pGroup->AddItems(cntValidItems, items, &results, &errors);
	else
		hr = S_FALSE;

	//
	// fill handle map
	//
	if (errors!=NULL && results!=0)
	{
		cntValidItems = 0;
		for ( ii=0; ii<itemList.GetSize(); ii++)
		{		
			hItem = NULL;
			strItem = itemList.GetAt(ii);
			strItem.MakeLower();
			if ( ! m_pItems->LookupHandle(strItem, hItem))
			{
				if (FAILED((errors)[cntValidItems]))
				{
					hItem = 0;
				}
				else 
				{
					hItem = (results)[cntValidItems].hServer;
					m_pItems->SetHandle(strItem, hItem);
			}

				cntValidItems++;
			}
 
		}
	}

	::CoTaskMemFree(errors);
	::CoTaskMemFree(results);
	delete[] items;
    return hr;
}

//------------------------------------------------------------------*
HRESULT
    CItemContainer::RemoveItem(LPCTSTR pszItem)
{
    HRESULT hr = S_FALSE;
    OPCHANDLE   hItem = NULL;
    CString strItem = pszItem;
    strItem.MakeLower();
    if (!m_pItems->LookupHandle(strItem, hItem)) {
        // already done, variable not in map at all
        return S_FALSE;
    }

    if (m_pGroup)
    {
        HRESULT         *errors = NULL;
        hr = m_pGroup->RemoveItems(1, &hItem, &errors);
        if (SUCCEEDED(hr))
		{
			m_pItems->Remove(strItem);
            ::CoTaskMemFree(errors);
        }
    }
    return hr;
}

//------------------------------------------------------------------*
HRESULT                 
	CItemContainer::RemoveItemList(LPCTSTR pszCommonPath, LPCTSTR pszItemsList)
{

	if (!m_pGroup)
		return S_FALSE;

    HRESULT hr = S_FALSE;
	CString strItems = pszItemsList;
	CString strCommonPath = pszCommonPath;

	//
	// create item array with size of list
	//
	CItemStringList itemList(pszCommonPath, strItems);
	if (itemList.GetSize() <= 0)
	{
		return S_FALSE;
	}
    OPCHANDLE* hItems = new OPCHANDLE[itemList.GetSize()];
    HRESULT*         errors = NULL;

	//
	// fill handle array
	//
	OPCHANDLE hItem;
	CString strItem;
	int cntValidItems=0;
    for (int ii=0; ii<itemList.GetSize(); ii++)
    {		
		hItem = NULL;
		strItem = itemList.GetAt(ii);
		strItem.MakeLower();
		if ( m_pItems->LookupHandle(strItem, hItem) )
		{
			hItems[cntValidItems] = hItem;
			cntValidItems++;
		}
	}
	
	if (cntValidItems <= 0)
	{
		delete[] hItems;
		return S_FALSE;
	}

	//
	// opc call 
	//
	hr = m_pGroup->RemoveItems(cntValidItems, hItems, &errors);

	//
	// delete entries from handle map
	//
	if (errors != 0)
	{
		for (ii=0; ii<itemList.GetSize(); ii++)
		{		
			strItem = itemList.GetAt(ii);
			strItem.MakeLower();
			m_pItems->Remove(strItem);
		}
	}

	::CoTaskMemFree(errors);
	delete[] hItems;
    return hr;
}

//------------------------------------------------------------------*
HRESULT
    CItemContainer::UpdateItems()
{
    HRESULT hr = E_NOTIMPL;
    return hr;
}

//------------------------------------------------------------------*
HRESULT
    CItemContainer::ReadItem(LPCTSTR pszItem, VARIANT *pVal, WORD &wQuality, FILETIME &ftTimeStamp)
{
    OPCHANDLE   hItem = NULL;
    CString strItem = pszItem;
    strItem.MakeLower();
    if (!m_pItems->LookupHandle(strItem, hItem)) {
        // unknown item
        return E_FAIL;
    }

    HRESULT hr = S_FALSE;
    if (m_pGroup)
    {
        if (!m_pSync) {
            return E_NOINTERFACE;   // have no if or no connection
        }
        
        HRESULT         *errors = NULL;
        OPCITEMSTATE    *states = NULL;
        hr = m_pSync->Read(OPC_DS_DEVICE, 1, &hItem, &states, &errors);
        // preset to no read!
        wQuality = OPC_QUALITY_BAD;
        SYSTEMTIME  now;
        GetSystemTime(&now);
        SystemTimeToFileTime(&now, &ftTimeStamp);
        if (SUCCEEDED(hr))
        {
            if (SUCCEEDED(errors[0]))
            {
                hr = ::VariantCopy(pVal, &states[0].vDataValue);
                _ASSERTE(SUCCEEDED(hr));
                VariantClear(&states[0].vDataValue);
                if (SUCCEEDED(hr))
                {
                    wQuality = states[0].wQuality;
                    ftTimeStamp = states[0].ftTimeStamp;
                }
            }
            ::CoTaskMemFree(errors);
            ::CoTaskMemFree(states);
        }
    }

    return hr;
}

//------------------------------------------------------------------*
HRESULT
    CItemContainer::ReadItemList(const CString& strCommonPath, 
		const CString& strItems, CString& strValues)
{
	HRESULT hr = E_FAIL;
	strValues="";
	//
	// create item array with size of list
	//
	CItemStringList itemList(strCommonPath, strItems);
	if (itemList.GetSize() <= 0)
	{
		return S_FALSE;
	}


	if ( !m_pGroup )
	{
		m_valueFormatter.GetBadQVTs(itemList.GetSize(), strValues);
		return S_FALSE;
	}

    if (!m_pSync)
	{
		m_valueFormatter.GetBadQVTs(itemList.GetSize(), strValues);
        return E_NOINTERFACE;   // have no if or no connection
	}

    OPCHANDLE* hItems = new OPCHANDLE[itemList.GetSize()];
    OPCITEMSTATE*	 states = NULL;
    HRESULT*         errors = NULL; 
	CString strItem;
	for (int ii=0; ii<itemList.GetSize(); ii++)
	{
		strItem=itemList.GetAt(ii);
		strItem.MakeLower();
	    if (!m_pItems->LookupHandle(strItem, hItems[ii]))
		{
			hItems[ii]=0;
		}
	}

	//
	// pure OPC call
	//
	hr = m_pSync->Read(OPC_DS_DEVICE, itemList.GetSize(), hItems, &states, &errors);

	//
	// format values
	//
	m_valueFormatter.ValuesToQVTString(itemList.GetSize(), states, errors, strValues);

	//
	// fill cache
	//
	if (states!=0 && errors!=0)
	{
		for (ii=0; ii< itemList.GetSize(); ii++)
		{
			m_pItems->SetItemState(hItems[ii], states[ii]);
		}

		//
		// free resources
		//
		for (ii=0; ii< itemList.GetSize(); ii++)
		{
			::VariantClear(&(states[ii].vDataValue));
		}
	}
	else
	{
		for (ii=0; ii< itemList.GetSize(); ii++)
		{
			m_pItems->ClearItemState(hItems[ii]);
		}
	}

    ::CoTaskMemFree(errors);
    ::CoTaskMemFree(states);
	delete[] hItems;
	return hr;
}

//------------------------------------------------------------------*
HRESULT
    CItemContainer::ReadItemListTyped(const CString& strCommonPath, 
		const CString& strItems, CString& strValues)
{
	HRESULT hr = E_FAIL;
	strValues="";

	//
	// create item array with size of list
	//
	CItemStringList itemList(strCommonPath, strItems);
	if (itemList.GetSize()/2 <= 0)
	{
		return S_FALSE;
	}

	if ( !m_pGroup )
	{
		m_valueFormatter.GetBadQVTs(itemList.GetSize()/2, strValues);
		return S_FALSE;
	}

    if (!m_pSync)
	{
		m_valueFormatter.GetBadQVTs(itemList.GetSize()/2, strValues);
        return E_NOINTERFACE;   // have no if or no connection
	}

    OPCHANDLE* hItems = new OPCHANDLE[itemList.GetSize()/2];
    OPCITEMSTATE*	 states = NULL;
    HRESULT*         errors = NULL; 
	CString strItem;
	CStringArray strTypes;
	for (int ii=0; ii<itemList.GetSize()/2; ii++)
	{
		strTypes.Add(itemList.GetAtSimple(2*ii));
		strItem=itemList.GetAt(2*ii+1);
		strItem.MakeLower();
	    if (!m_pItems->LookupHandle(strItem, hItems[ii]))
		{
			hItems[ii]=0;
		}
	}

	hr = m_pSync->Read(OPC_DS_DEVICE, itemList.GetSize()/2, hItems, &states, &errors);

	//
	// format values
	//
	m_valueFormatter.ValuesToQVTStringIEC(itemList.GetSize()/2, states, errors, strTypes, strValues);

	//
	// fill cache
	//
	if (states!=0 && errors!=0)
	{
		for (ii=0; ii< itemList.GetSize()/2; ii++)
		{
			m_pItems->SetItemState(hItems[ii], states[ii]);
		}

		//
		// free resources
		//
		for (ii=0; ii< itemList.GetSize()/2; ii++)
		{
			::VariantClear(&(states[ii].vDataValue));
		}
	}
	else
	{
		for (ii=0; ii< itemList.GetSize()/2; ii++)
		{
			m_pItems->ClearItemState(hItems[ii]);
		}
	}

    ::CoTaskMemFree(errors);
    ::CoTaskMemFree(states);
	delete[] hItems;
	return hr;
}

//------------------------------------------------------------------*
HRESULT
    CItemContainer::ReadCachedItemList(const CString& strCommonPath, 
		const CString& strItems, CString& strValues)
{
	HRESULT hr = E_FAIL;
	strValues="";

	//
	// create item array with size of list
	//
	CItemStringList itemList(strCommonPath, strItems);
	if (itemList.GetSize() <= 0)
	{
		return S_FALSE;
	}
    OPCHANDLE* hItems = new OPCHANDLE[itemList.GetSize()];
    OPCITEMSTATE*	 states = new OPCITEMSTATE[itemList.GetSize()];
    HRESULT*         errors = new HRESULT[itemList.GetSize()]; 
	CString strItem;
	CStringArray strTypes;
	for (int ii=0; ii<itemList.GetSize(); ii++)
	{
		strItem=itemList.GetAt(ii);
		strItem.MakeLower();
	    if (!m_pItems->LookupHandle(strItem, hItems[ii]))
		{
			hItems[ii]=0;
		}
		::VariantInit(&states[ii].vDataValue);
	}

	hr=ReadCache(itemList.GetSize(), hItems, states, errors);

	//
	// format values
	//
	m_valueFormatter.ValuesToQVTString(itemList.GetSize(), states, errors, strValues);

	//
	// free resources
	//
	for (ii=0; ii< itemList.GetSize(); ii++)
	{
		::VariantClear(&(states[ii].vDataValue));
	}
    delete[] errors;
    delete[] states;
	delete[] hItems;
	return hr;
}

//------------------------------------------------------------------*
HRESULT
    CItemContainer::ReadCachedItemListTyped(const CString& strCommonPath, 
		const CString& strItems, CString& strValues)
{
	HRESULT hr = E_FAIL;
	strValues="";

	//
	// create item array with size of list
	//
	CItemStringList itemList(strCommonPath, strItems);
	if (itemList.GetSize()/2 <= 0)
	{
		return S_FALSE;
	}
    OPCHANDLE* hItems = new OPCHANDLE[itemList.GetSize()/2];
    OPCITEMSTATE*	 states = new OPCITEMSTATE[itemList.GetSize()/2];
    HRESULT*         errors = new HRESULT[itemList.GetSize()/2]; 
	CString strItem;
	CStringArray strTypes;
	for (int ii=0; ii<itemList.GetSize()/2; ii++)
	{
		strTypes.Add(itemList.GetAtSimple(2*ii));
		strItem=itemList.GetAt(2*ii+1);
		strItem.MakeLower();
	    if (!m_pItems->LookupHandle(strItem, hItems[ii]))
		{
			hItems[ii]=0;
		}
		::VariantInit(&states[ii].vDataValue);
	}

	hr=ReadCache(itemList.GetSize()/2, hItems, states, errors);

	//
	// format values
	//
	m_valueFormatter.ValuesToQVTStringIEC(itemList.GetSize()/2, states, errors, strTypes, strValues);

	//
	// free resources
	//
	for (ii=0; ii< itemList.GetSize()/2; ii++)
	{
		::VariantClear(&(states[ii].vDataValue));
	}
    delete[] errors;
    delete[] states;
	delete[] hItems;
	return hr;
}

//------------------------------------------------------------------*
HRESULT
    CItemContainer::WriteItem(LPCTSTR pszItem, VARIANT val)
{
    OPCHANDLE   hItem = NULL;
    CString strItem = pszItem;
    strItem.MakeLower();
    if (!m_pItems->LookupHandle(strItem, hItem)) {
        // unknown item
        return E_FAIL;
    }

    HRESULT hr = S_FALSE;
    if (m_pGroup)
    {
        if (!m_pSync) {
            return E_NOINTERFACE;   // have no if or no connection
        }
        
        HRESULT         *errors = NULL;
        hr = m_pSync->Write(1, &hItem, &val, &errors);
        if (SUCCEEDED(hr))
        {
            if (SUCCEEDED(errors[0]))
            {
                hr = S_OK;
            }
            ::CoTaskMemFree(errors);
        }
    }
    return hr;
}

//------------------------------------------------------------------*
BOOL
    CItemContainer::IsEmpty()
{
    return (m_pItems->GetCount() <= 0);
}


//------------------------------------------------------------------*
HRESULT
    CItemContainer::ConnectToServer(LPCTSTR pszMachineName, CLSID clsid, IID iidInitial, IUnknown **ppUnk)
{
    if (ppUnk == NULL) {
        return E_POINTER;
    }
    *ppUnk = NULL;
    USES_CONVERSION;
    MULTI_QI queue[1];
    queue[0].pIID = &iidInitial;
    queue[0].pItf = NULL;
    queue[0].hr = 0;
    ULONG cmq = 1;
    DWORD clsctx;

    // set up server node info:
    COSERVERINFO     serverInfo;
    COSERVERINFO   * pServerInfo = &serverInfo;
    if ((pszMachineName != NULL) && (_tcslen(pszMachineName) > 0))
    {
        pServerInfo = &serverInfo;
        serverInfo.dwReserved1 = 0;
        serverInfo.dwReserved2 = 0;
        serverInfo.pwszName = T2OLE((LPTSTR)pszMachineName);
        serverInfo.pAuthInfo = 0;
        clsctx = CLSCTX_REMOTE_SERVER | CLSCTX_LOCAL_SERVER;
    } else
    {
        // try local server
        pServerInfo = 0;        // pointer should be NULL if local
        clsctx = CLSCTX_LOCAL_SERVER;
    }

    HRESULT hr = ::CoCreateInstanceEx(clsid, NULL, clsctx, pServerInfo, cmq, queue);
    
    if(SUCCEEDED(hr)) {
        *ppUnk = queue[0].pItf;
    }
    return hr;
}

HRESULT	CItemContainer::ReadCache(int iCount, OPCHANDLE* phServer, OPCITEMSTATE* pItemValues, HRESULT* pErrors)
{
	HRESULT hr=S_OK;
	BOOL bFound;
	for (int ii=0; ii<iCount; ii++)
	{
		bFound=m_pItems->LookupItemState(phServer[ii], pItemValues[ii]);
		if (bFound)
		{
			pErrors[ii]=S_OK;
		}
		else
		{
			pErrors[ii]=E_FAIL;
			hr=S_FALSE;
		}
	}
	return hr;
}

/*
 *----------------------------------------------------------------------------*
 *  $History: ItemContainer.cpp $
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
