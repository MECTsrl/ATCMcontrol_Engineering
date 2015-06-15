/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSC/AsyncOPC/OPCConnection.cpp 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: OPCConnection.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSC/AsyncOPC/OPCConnection.cpp $
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
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
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
#include "opcDA.h"
#include "Item.h"
#include "OPCConnection.h"
#include "CSC_OnlineServer.h"

//----  Static Initializations:   ----------------------------------*




/////////////////////////////////////////////////////////////////////////////
// CConnection

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CCriticalSection  COPCConnection::sm_guardConnectionMap;
connectionMap     COPCConnection::sm_connectionMap;

/////////////////////////////////////////////////////////////////////////////
// CConnection
//------------------------------------------------------------------*
COPCConnection::COPCConnection()
{
    m_bConnected = false;
    m_bShutdownRequest = false;
    m_pOPCServer = NULL;
    m_hGroup1 = 0;
    m_pItemMgt = NULL;
    m_pSyncIO = NULL;
}

//------------------------------------------------------------------*
COPCConnection::~COPCConnection()
{
    while (!m_activeItemList.IsEmpty()) {
        CItem *pItem = m_activeItemList.RemoveHead();
        pItem->Release();
    }
    while (!m_removedItemList.IsEmpty()) {
        CItem *pItem = m_removedItemList.RemoveHead();
        pItem->Release();
    }
    while (!m_writeRequestList.IsEmpty()) {
        CWriteRequest *pRequest = m_writeRequestList.RemoveHead();
        pRequest->Release();
    }
}


//------------------------------------------------------------------*
HRESULT COPCConnection::LookupConnection(LPCTSTR strMachineName, CItem *pItem, COPCConnection**ppConnection)
{
    CSingleLock(&sm_guardConnectionMap, TRUE);
    if (pItem == NULL || ppConnection == NULL)
        return E_POINTER;
    *ppConnection = NULL;

    CString strKey = strMachineName;
    strKey.MakeLower();
    if (!sm_connectionMap.Lookup(strKey, *ppConnection)) {
        CComObject<COPCConnection> *pNewConn = NULL;
        HRESULT hr = CComObject<COPCConnection>::CreateInstance(&pNewConn);
        if (FAILED(hr))
          return hr;
        *ppConnection = pNewConn;
        (*ppConnection)->AddRef();
        (*ppConnection)->m_sMachineName = strMachineName;
        sm_connectionMap.SetAt(strKey, *ppConnection);
        (*ppConnection)->StartThread();
    }
    (*ppConnection)->AddRef();
    (*ppConnection)->AddItem(pItem);
    return S_OK;
}

//------------------------------------------------------------------*
HRESULT COPCConnection::ReleaseConnection(COPCConnection *pConnection, CItem *pItem)
{
    CSingleLock(&sm_guardConnectionMap, TRUE);
    if (pConnection == NULL || pItem == NULL)
      return E_POINTER;

    CString strKey = pConnection->m_sMachineName;
    strKey.MakeLower();
    COPCConnection *pConn = NULL;
    sm_connectionMap.Lookup(strKey, pConn);
    _ASSERTE(pConn == pConnection);
    if (pConn != pConnection)
        return E_FAIL;
    pConnection->RemoveItem(pItem);
    if (pConnection->IsEmpty()) {
      pConnection->StopThread();
      sm_connectionMap.RemoveKey(strKey);
      pConnection->Release();
    }
    return S_OK;
}

//------------------------------------------------------------------*
HRESULT COPCConnection::SubmitWriteRequest(CItem *pItem, VARIANT Value)
{
    CSingleLock lock(&m_cs, TRUE);
    if (!m_bConnected || pItem->m_hServer == 0)
        return E_FAIL;
    CComObject<CWriteRequest> *pNewWriteReq = NULL;
    HRESULT hr = CComObject<CWriteRequest>::CreateInstance(&pNewWriteReq);
    if (FAILED(hr))
        return hr;
    CWriteRequest *pWriteReq = pNewWriteReq;
    pWriteReq->AddRef();
    pWriteReq->Init(pItem, Value);
    m_writeRequestList.AddTail(pWriteReq);
    return S_OK;
}

//------------------------------------------------------------------*
HRESULT COPCConnection::StartThread()
{
    AddRef();
    DWORD threadID;
    HANDLE hThread = CreateThread(NULL, 0, ThreadMain, (LPVOID)this, 0, &threadID);
    if (hThread == NULL)
      return E_FAIL;

    CloseHandle(hThread);
    return S_OK;
}
//------------------------------------------------------------------*
HRESULT COPCConnection::StopThread()
{
    m_bShutdownRequest = true;
    return S_OK;
}

//------------------------------------------------------------------*
HRESULT COPCConnection::AddItem(CItem *pItem)
{
    CSingleLock lock(&m_cs, TRUE);
    if (pItem == NULL)
        return E_POINTER;
    _ASSERTE(pItem->m_hServer == 0);
    pItem->AddRef();
    m_activeItemList.AddTail(pItem);
    return S_OK;
}

//------------------------------------------------------------------*
HRESULT COPCConnection::RemoveItem(CItem *pItem)
{
    CSingleLock lock(&m_cs, TRUE);
    if (pItem == NULL)
        return E_POINTER;
    POSITION pos = m_activeItemList.Find(pItem);
    if (pos == NULL)
       return E_FAIL;
    m_activeItemList.RemoveAt(pos);
    pItem->Release();
    if (m_bConnected) {
      pItem->AddRef();
      m_removedItemList.AddTail(pItem);
    }
    return S_OK;
}

//------------------------------------------------------------------*
DWORD WINAPI COPCConnection::ThreadMain(LPVOID lpParam)
{
    CoInitializeEx(NULL, COINIT_MULTITHREADED);
    COPCConnection *_this = (COPCConnection *)lpParam;
    HRESULT hr = _this->ConnectThread();
    _this->Release();
    CoUninitialize();
    return hr;
}

//------------------------------------------------------------------*
HRESULT COPCConnection::ConnectThread()
{
    while (!m_bShutdownRequest) {
      if (!m_bConnected) {
          HRESULT hr = DoConnect();
          if (FAILED(hr)) {
              Sleep(5000);
          }
      }
      if (m_bConnected) {
        HRESULT hr = DoWorkItem();
        if (FAILED(hr)) {
            DoDisconnect();
            Sleep(5000);
        }
      }
      Sleep(1000);
    }
    if (m_bConnected)
      DoDisconnect();
    return S_OK;
}

//------------------------------------------------------------------*
HRESULT COPCConnection::DoConnect()
{
    USES_CONVERSION;
    HRESULT hr;

    _ASSERTE(!m_bConnected);

    MULTI_QI queue[1];
    queue[0].pIID = &IID_IOPCServer;
    queue[0].pItf = NULL;
    queue[0].hr = 0;

    // set up server node info:
    COSERVERINFO     serverInfo;
	serverInfo.dwReserved1 = 0;
	serverInfo.dwReserved2 = 0;
	serverInfo.pwszName = T2OLE(m_sMachineName);
    serverInfo.pAuthInfo = NULL;

    //HACK hardcode from C:\4c2\COM\softing\fc\AddOns\Softing\4CPCTarget\inc\CSC_OnlSrv.idl
    //CLSID_OPCServer 379165A5-C2BF-47a8-8F60-73EDE1FD6001
    CLSID CLSIDOPCServer; //HACK
    CLSIDFromString(L"{5425B8A4-754D-496a-AF31-7D78A530E58D}", &CLSIDOPCServer);//HACK
    
    hr = CoCreateInstanceEx(CLSIDOPCServer, NULL, CLSCTX_REMOTE_SERVER | CLSCTX_LOCAL_SERVER,
                            &serverInfo, 1, queue);
    if (FAILED(hr)) {
        return hr;
    }
    m_pOPCServer = (IOPCServer*) queue[0].pItf;

    DWORD dummy;
    hr = m_pOPCServer->AddGroup(L"AsyncOPC", TRUE, 1000, 0, NULL, NULL, LOCALE_NEUTRAL,
                                &m_hGroup1, &dummy, IID_IOPCItemMgt, (IUnknown**)&m_pItemMgt);

    if (FAILED(hr)) {
        m_pOPCServer->Release();
        m_pOPCServer=NULL;
        return hr;
    }

    hr = m_pItemMgt->QueryInterface(IID_IOPCSyncIO, (void **)&m_pSyncIO);
    if (FAILED(hr)) {
        m_pItemMgt->Release();
        m_pItemMgt=NULL;
        m_pOPCServer->RemoveGroup(m_hGroup1, TRUE);
        m_pOPCServer->Release();
        m_pOPCServer=NULL;
        return hr;
    }

    TRACE(_T("COPCConnection::DoConnect to %s\n"), (LPCTSTR)m_sMachineName);
    m_bConnected = true;
    return S_OK;
}

//------------------------------------------------------------------*
HRESULT COPCConnection::DoDisconnect()
{
    TRACE(_T("COPCConnection::DoDisconnect from %s\n"), (LPCTSTR)m_sMachineName);
    CSingleLock lock(&m_cs, TRUE);
    _ASSERTE(m_bConnected);
    m_bConnected = false;
    FILETIME timeNow;
    CoFileTimeNow(&timeNow);
    POSITION pos = m_activeItemList.GetHeadPosition();
    while (pos != NULL) {
        CComVariant Value;
        WORD quality;
        FILETIME time;
        CItem *pItem = m_activeItemList.GetNext(pos);
        pItem->m_hServer = 0;
        pItem->GetQVT(&Value, &quality, &time);
        quality = OPC_QUALITY_BAD;
        time = timeNow;
        pItem->SetQVT(Value, quality, time);
    }
    while (!m_removedItemList.IsEmpty()) {
        CItem *pItem = m_removedItemList.RemoveHead();
        pItem->Release();
    }
    while (!m_writeRequestList.IsEmpty()) {
        CWriteRequest *pRequest = m_writeRequestList.RemoveHead();
        pRequest->Release();
    }
    lock.Unlock();

    m_pSyncIO->Release();
    m_pSyncIO=NULL;
    m_pItemMgt->Release();
    m_pItemMgt=NULL;
    
    HRESULT hr = m_pOPCServer->RemoveGroup(m_hGroup1, TRUE);
    m_pOPCServer->Release();
    m_pOPCServer=NULL;

    return S_OK;
}

//------------------------------------------------------------------*
HRESULT COPCConnection::DoWorkItem()
{
    HRESULT hr = S_OK;
    CSingleLock lock(&m_cs, TRUE);
    _ASSERTE(m_bConnected);
    itemList addItems;
    itemList activeItems;
    writeRequestList writeRequests;
    itemList removedItems;
    POSITION pos = m_activeItemList.GetHeadPosition();
    while(pos != NULL) {
        CItem *pItem = m_activeItemList.GetNext(pos);
        pItem->AddRef();
        if (pItem->m_hServer == 0) {
            pItem->AddRef();
            addItems.AddTail(pItem);
        }
        activeItems.AddTail(pItem);
    }
    while (!m_writeRequestList.IsEmpty())
        writeRequests.AddTail(m_writeRequestList.RemoveHead());
    while (!m_removedItemList.IsEmpty())
        removedItems.AddTail(m_removedItemList.RemoveHead());
    lock.Unlock();
    if (addItems.GetCount()>0) {
        hr = DoAddItems(addItems);
        if (FAILED(hr)) {
          while(!addItems.IsEmpty())
            addItems.RemoveHead()->Release();
          while(!writeRequests.IsEmpty())
            writeRequests.RemoveHead()->Release();
          while(!activeItems.IsEmpty())
            activeItems.RemoveHead()->Release();
          while(!removedItems.IsEmpty())
            removedItems.RemoveHead()->Release();
          return hr;
        }
    }
    if (writeRequests.GetCount()>0) {
        hr = DoWriteItems(writeRequests);
        if (FAILED(hr)) {
          while(!writeRequests.IsEmpty())
            writeRequests.RemoveHead()->Release();
          while(!activeItems.IsEmpty())
            activeItems.RemoveHead()->Release();
          while(!removedItems.IsEmpty())
            removedItems.RemoveHead()->Release();
          return hr;
        }
    }
    if (activeItems.GetCount()>0) {
        hr = DoReadItems(activeItems);
        if (FAILED(hr)) {
          while(!activeItems.IsEmpty())
            activeItems.RemoveHead()->Release();
          while(!removedItems.IsEmpty())
            removedItems.RemoveHead()->Release();
          return hr;
        }
    }
    if (removedItems.GetCount()>0) {
      hr = DoRemoveItems(removedItems);
      if (FAILED(hr)) {
        while(!removedItems.IsEmpty())
          removedItems.RemoveHead()->Release();
      }
    }
    return hr;
}

HRESULT COPCConnection::DoAddItems(itemList &addItems)
{
    int nItems = addItems.GetCount();
    OPCITEMDEF *pItems = new OPCITEMDEF [nItems];
    if (!pItems) {
        return E_OUTOFMEMORY;
    }
    POSITION pos = addItems.GetHeadPosition();
    int i=0;
    while (pos != NULL) {
        CItem *pItem = addItems.GetNext(pos);
        pItems[i].szAccessPath = NULL;
        pItem->GetTagName(&pItems[i].szItemID);
        pItems[i].bActive = TRUE;
        pItems[i].hClient = 0;
        pItems[i].dwBlobSize = 0;
        pItems[i].pBlob = NULL;
        pItems[i].vtRequestedDataType = VT_EMPTY;
        pItems[i].wReserved = 0;
        i++;
    }

    OPCITEMRESULT *pResult;
    HRESULT *pErrors;
    HRESULT hr = m_pItemMgt->AddItems(nItems, pItems, &pResult, &pErrors);
    if (FAILED(hr)) {
        for (i=0; i<nItems; i++) {
          SysFreeString(pItems[i].szItemID);
        }
        delete []pItems;
        return hr;
    }
    i=0;
    while (!addItems.IsEmpty()) {
        CItem *pItem = addItems.RemoveHead();
        if (SUCCEEDED(pErrors[i])) {
            _ASSERTE(pResult[i].hServer != 0);
            pItem->m_hServer = pResult[i].hServer;
        } else {
            TRACE(_T("COPCConnection::AddItem %ls on %s fails %x\n"),
                      pItems[i].szItemID, (LPCTSTR)m_sMachineName, pErrors[i]);
        }
        i++;
        pItem->Release();
    }
    for (i=0; i<nItems; i++) {
      SysFreeString(pItems[i].szItemID);
    }
    CoTaskMemFree(pResult);
    CoTaskMemFree(pErrors);
    delete []pItems;
    return hr;
}

HRESULT COPCConnection::DoReadItems(itemList &activeItems)
{
    OPCHANDLE *phItems = new OPCHANDLE[activeItems.GetCount()];
    if (!phItems) {
        return E_OUTOFMEMORY;
    }
    POSITION pos = activeItems.GetHeadPosition();
    int iCount=0;
    while (pos != NULL) {
        CItem *pItem = activeItems.GetNext(pos);
        if (pItem->m_hServer != 0) {
            phItems[iCount++] = pItem->m_hServer;
        }
    }

    if (iCount == 0) {
        delete []phItems;
        while(!activeItems.IsEmpty())
            activeItems.RemoveHead()->Release();
        return S_OK;
    }

    OPCITEMSTATE *pResult;
    HRESULT *pErrors;
    HRESULT hr = m_pSyncIO->Read(OPC_DS_CACHE, iCount, phItems, &pResult, &pErrors);
    delete []phItems;
    if (FAILED(hr)) {
        return hr;
    }

    int i=0;
    while(!activeItems.IsEmpty()) {
        CItem *pItem = activeItems.RemoveHead();
        if (pItem->m_hServer != 0) {
            if (SUCCEEDED(pErrors[i])) {
                pItem->SetQVT(pResult[i].vDataValue, pResult[i].wQuality, pResult[i].ftTimeStamp);
            }
            i++;
        }
        pItem->Release();
    }
    _ASSERTE(i == iCount);
    for (i=0; i<iCount; i++)
      VariantClear(&pResult[i].vDataValue);
    CoTaskMemFree(pResult);
    CoTaskMemFree(pErrors);
    return hr;
}

HRESULT COPCConnection::DoWriteItems(writeRequestList &writeRequests)
{
    OPCHANDLE *phItems = new OPCHANDLE[writeRequests.GetCount()];
    VARIANT *pValues = new VARIANT[writeRequests.GetCount()];
    if (!phItems || !pValues) {
        return E_OUTOFMEMORY;
    }
    int iCount = 0;
    while (!writeRequests.IsEmpty()) {
        CWriteRequest *pRequest = writeRequests.RemoveHead();
        CItem *pItem = NULL;
        pRequest->GetItem(&pItem);
        if (pItem->m_hServer != 0) {
            phItems[iCount] = pItem->m_hServer;
            pRequest->GetValue(&pValues[iCount]);
            iCount++;
        }
        pItem->Release();
        pRequest->Release();
    }
    if (iCount == 0) {
        delete []phItems;
        delete []pValues;
        return S_OK;
    }

    HRESULT *pErrors;
    HRESULT hr = m_pSyncIO->Write(iCount, phItems, pValues, &pErrors);
    while(iCount-->0)
      VariantClear(&pValues[iCount]);

    delete []phItems;
    delete []pValues;

    if (FAILED(hr)) {
        return hr;
    }

    CoTaskMemFree(pErrors);
    return hr;
}

HRESULT COPCConnection::DoRemoveItems(itemList &removedItems)
{
    OPCHANDLE *phItems = new OPCHANDLE[removedItems.GetCount()];
    if (!phItems) {
        return E_OUTOFMEMORY;
    }
    int iCount = 0;
    while (!removedItems.IsEmpty()) {
        CItem *pItem = removedItems.RemoveHead();
        if (pItem->m_hServer != 0) {
            phItems[iCount++] = pItem->m_hServer;
            pItem->m_hServer = 0;
        }  
        pItem->Release();
    }

    if (iCount == 0) {
        delete[] phItems;
        return S_OK;
    }

    HRESULT *pErrors;
    HRESULT hr = m_pItemMgt->RemoveItems(iCount, phItems, &pErrors);
    delete[] phItems;
    if (FAILED(hr)) {
        return hr;
    }

    CoTaskMemFree(pErrors);
    return hr;
}
/////////////////////////////////////////////////////////////////////////////
// CWriteRequest
//------------------------------------------------------------------*
CWriteRequest::CWriteRequest()
{
}

//------------------------------------------------------------------*
CWriteRequest::~CWriteRequest()
{
}

//------------------------------------------------------------------*
HRESULT CWriteRequest::Init(CItem *pItem, VARIANT Value)
{
    m_pItem = pItem;
    m_Value = Value;
    return S_OK;
}

//------------------------------------------------------------------*
HRESULT CWriteRequest::GetItem(CItem **ppItem)
{
    if (ppItem == NULL)
        return E_POINTER;
    *ppItem = m_pItem;
    if (*ppItem == NULL)
        return E_FAIL;
    (*ppItem)->AddRef();
    return S_OK;
}

//------------------------------------------------------------------*
HRESULT CWriteRequest::GetValue(VARIANT *pValue)
{
    if (pValue == NULL)
        return E_POINTER;
    VariantInit(pValue);
    return VariantCopy(pValue, &m_Value);
}


/*
 *----------------------------------------------------------------------------*
 *  $History: OPCConnection.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CSC/AsyncOPC
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CSC/AsyncOPC
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 8.08.01    Time: 18:15
 * Updated in $/4Control/COM/softing/fc/CSC/AsyncOPC
 * adapted to changes of V2.0
 * 
 * *****************  Version 1  *****************
 * User: Ca           Date: 8.08.01    Time: 9:41
 * Created in $/4Control/COM/softing/fc/CSC/AsyncOPC
 * copied from V1.3
 *==
 *----------------------------------------------------------------------------*
*/
