/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE/VarSession.cpp 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: VarSession.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE/VarSession.cpp $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                4CSC
 *
 * =COMPONENT           CSC Online Control
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
 *  14.03.2001  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/
#include "stdafx.h"
#include "CSC_OnlineServer.h"
#include "VarSession.h"
#include "4CVariable.h"
#include "4CVariables.h"
#include "CSC_OnlineErr.h"
#include "opcDA_i.c"

#include "CSC_OnlSrv.h"

//----  Local Defines:   -------------------------------------------*
#define CSC_COMMAND_REQ_TO  (5000)
#define CSC_COMMAND_REQ_TO_LONG  (50000)
//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CFCVarSessionThread

//------------------------------------------------------------------*
CFCVarSessionThread::CFCVarSessionThread() : m_CommandList()
{
   m_pRawVarSession = NULL;
   m_pVarSession = NULL;
   m_pVarSessionStream = NULL;
   m_hThreadWakeUp = CreateEvent(NULL, FALSE, FALSE, NULL);
   m_sMachineName = NULL;
   m_cookie = 0;
   m_lKey = 0;
   m_pControlMgt = NULL;
   m_pCheckConnection = NULL;
   m_pOPCServer = NULL;
   m_pGroup1 = NULL;
   m_bShutdownRequest = false;
   m_bAddVarsRequest = false;
   m_bRemVarsRequest = false;
   m_bWriteVarsRequest = false;
   m_bCommandRequest = false;
   m_bConnected = false;
}

//------------------------------------------------------------------*
CFCVarSessionThread::~CFCVarSessionThread()
{
   CloseHandle(m_hThreadWakeUp);
   if (m_sMachineName != NULL)
      SysFreeString(m_sMachineName);
   if (m_pRawVarSession != NULL)
      m_pRawVarSession->Release();
}

//------------------------------------------------------------------*
HRESULT CFCVarSessionThread::DoConnect()
{
    HRESULT hr = S_OK;

    TrcPrint(TRC_INTERFACE, _T("VarSession::DoConnect(%s)\n"),(LPCTSTR) m_pRawVarSession->m_sMachineName);

    hr = ConnectControl();
    if(FAILED(hr)) {
        DoDisconnect();
        return hr;
    }

    hr = ConnectOPC();
    if(FAILED(hr)) {
        DoDisconnect();
        return hr;
    }


    Login();
    return S_OK;
}
//------------------------------------------------------------------*
HRESULT CFCVarSessionThread::ConnectOPC()
{
    HRESULT hr = S_OK;
    MULTI_QI queue[1];
    COSERVERINFO     serverInfo;
    DWORD dummy;
    CComPtr<ICustomConnect> pConn;
    CComPtr<IUnknown> pUnk;
    CComBSTR sInit;
    // set up server node info:
	serverInfo.dwReserved1 = 0;
	serverInfo.dwReserved2 = 0;
	serverInfo.pwszName = m_sMachineName;
    serverInfo.pAuthInfo = NULL;
    queue[0].pItf = NULL;
    queue[0].hr = 0;

    if(m_pRawVarSession->m_clsidConn.IsEmpty()) { //no custom connect - stay with defaults
        queue[0].pIID = &IID_IOPCServer;

	    hr = CoCreateInstanceEx(m_pRawVarSession->m_clsidOPC, NULL, 
            CLSCTX_REMOTE_SERVER | CLSCTX_LOCAL_SERVER,
            &serverInfo, FC_ARRAY_LEN(queue), queue);
        
        if (FAILED(hr)) {
            return hr;
        }
        m_pOPCServer = (IOPCServer*) queue[0].pItf;
    }
    else {
        //custom connect - try to locate via ICustomConnect
        queue[0].pIID = &IID_ICustomConnect;

	    hr = CoCreateInstanceEx(m_pRawVarSession->m_clsidConn, NULL, 
            CLSCTX_REMOTE_SERVER | CLSCTX_LOCAL_SERVER,
            &serverInfo, FC_ARRAY_LEN(queue), queue);
       
        if (FAILED(hr)) {
            return hr;
        }

        pConn.Attach((ICustomConnect*) queue[0].pItf);
        
        sInit = m_pRawVarSession->m_strInit;

        hr = pConn->CreateVarSession(sInit,&pUnk);
        if (FAILED(hr)) {
            return hr;
        }

        hr = pUnk->QueryInterface(IID_IOPCServer,(void**)&m_pOPCServer);

        if (FAILED(hr)) {
            return hr;
        }

        if(m_pOPCServer == NULL) {
            return E_NOINTERFACE;
        }

    }
    
    hr = m_pOPCServer->AddGroup(L"CSC_Online", TRUE, 1000, 0, NULL, NULL, LOCALE_NEUTRAL,
                                &m_hGroup1, &dummy, IID_IOPCItemMgt, (IUnknown**)&m_pGroup1);

    if (FAILED(hr)) {
        return hr;
    }

    IDataObject *pDataObject;
    hr = m_pGroup1->QueryInterface(IID_IDataObject, (void **)&pDataObject);
    if (FAILED(hr)) {
        return hr;
    }

    DWORD dwConnection;
    FORMATETC format;
    format.cfFormat = RegisterClipboardFormat(_T("OPCSTMFORMATDATATIME"));
    format.ptd = NULL;
    format.dwAspect = DVASPECT_CONTENT;
    format.lindex = -1;
    format.tymed = TYMED_HGLOBAL;
    hr = pDataObject->DAdvise(&format, 0, this, &dwConnection);
    pDataObject->Release();

    if (FAILED(hr)) {
        return hr;
    }

    return S_OK;
}


HRESULT CFCVarSessionThread::ConnectControl()
{
    HRESULT hr = S_OK;
    MULTI_QI queue[1];
    COSERVERINFO     serverInfo;
	DWORD clsctx;
    CComPtr<ICustomConnect> pConn;
    CComPtr<IUnknown>pUnk;
    CComBSTR sInit;

    try {
        queue[0].pItf = NULL;
        queue[0].hr = 0;
        // set up server node info:
	    serverInfo.dwReserved1 = 0;
	    serverInfo.dwReserved2 = 0;
        USES_CONVERSION;
	    serverInfo.pwszName = m_sMachineName;
	    serverInfo.pAuthInfo = 0;
	    clsctx = CLSCTX_REMOTE_SERVER | CLSCTX_LOCAL_SERVER;

        if(m_pRawVarSession->m_clsidConn.IsEmpty()) { //no custom connect - stay with defaults
            // now connect to CSC Online Server
            queue[0].pIID = &IID_IControlMgt;
            hr = ::CoCreateInstanceEx(m_pRawVarSession->m_clsidCtrl, NULL, clsctx, &serverInfo, FC_ARRAY_LEN(queue), queue);

            if(FAILED(hr)) {
                TrcPrint(TRC_ERROR, _T("VarSession::ConnectToControl failed hr=0x%08X\n"), hr);
                _com_raise_error(hr);
            }

            m_pControlMgt = (IControlMgt*) queue[0].pItf;
        }
        else { //go with custom connect coclass and ask for ICustomConnect
            queue[0].pIID = &IID_ICustomConnect;
            
            hr = ::CoCreateInstanceEx(m_pRawVarSession->m_clsidConn, NULL, clsctx, &serverInfo, FC_ARRAY_LEN(queue), queue);
            if(FAILED(hr)) {
                TrcPrint(TRC_ERROR, _T("VarSession::ConnectToControl failed hr=0x%08X\n"), hr);
                _com_raise_error(hr);
            }

            pConn.Attach((ICustomConnect*)queue[0].pItf);
            sInit = m_pRawVarSession->m_strInit;
            hr = pConn->CreateControlManagement(sInit,&pUnk);
            if(FAILED(hr)) {
                TrcPrint(TRC_ERROR, _T("VarSession::ConnectToControl failed hr=0x%08X\n"), hr);
                _com_raise_error(hr);
            }

            hr = pUnk->QueryInterface(IID_IControlMgt,(void**)&m_pControlMgt);
            if(FAILED(hr)) {
                TrcPrint(TRC_ERROR, _T("VarSession::ConnectToControl failed hr=0x%08X\n"), hr);
                _com_raise_error(hr);
            }
        }

        //Query Connection interface for connection check
        hr = m_pControlMgt->QueryInterface(IID_IConnection,(void**)&m_pCheckConnection);
        if(FAILED(hr)) {
            TrcPrint(TRC_ERROR, _T("VarSession::ConnectToControl failed hr=0x%08X\n"), hr);
            _com_raise_error(hr);
        }

    }
    catch(_com_error err) {
        return err.Error();
    }
    return S_OK;
}

HRESULT CFCVarSessionThread::Login()
{
    if(!m_pControlMgt) {
        return E_NOINTERFACE;
    }

    if(m_lKey) {
        return S_OK;
    }
    
    TrcPrint(TRC_INTERFACE, _T("VarSession::Login(%s)\n"),(LPCTSTR)m_pRawVarSession->m_sMachineName);

    HRESULT hr = m_pControlMgt->Login(m_pRawVarSession->m_sUser,
        m_pRawVarSession->m_sPassword,&m_lKey);

    if(FAILED(hr)) {
        m_lKey = 0;
    }
    return hr;
}

HRESULT CFCVarSessionThread::Logout()
{
    HRESULT hr;
    
    if(!m_lKey || !m_pControlMgt) {
        //already logged out
        return S_OK;
    }

    TrcPrint(TRC_INTERFACE, _T("VarSession::Logout(%s)\n"),(LPCTSTR)m_pRawVarSession->m_sMachineName);

    hr = m_pControlMgt->Logout(m_lKey);
    m_lKey = 0;
    return hr;   
}

//------------------------------------------------------------------*
HRESULT CFCVarSessionThread::DoDisconnect()
{
    TrcPrint(TRC_INTERFACE, _T("VarSession::DoDisconnect(%s)\n"),(LPCTSTR) m_pRawVarSession->m_sMachineName);

    if(m_pGroup1) {
        //Release group
        FC_RELEASE_PTR(m_pGroup1);
        //Remove from server
        HRESULT hr = m_pOPCServer->RemoveGroup(m_hGroup1, TRUE);
    }

    FC_RELEASE_PTR(m_pOPCServer);
    Logout();
    FC_RELEASE_PTR(m_pControlMgt);
    FC_RELEASE_PTR(m_pCheckConnection);
    CoDisconnectObject(this, 0);
    return S_OK;
}

void CFCVarSessionThread::InitiateConnection()
{
    CSingleLock lock(&m_pRawVarSession->m_cs, TRUE);
    m_bConnected = true;
    POSITION pos = m_pRawVarSession->m_varClientHandleMap.GetStartPosition();
    while (pos != NULL) {
        long key;
        CFCVariable *pItem;
        m_pRawVarSession->m_varClientHandleMap.GetNextAssoc(pos, key, pItem);
        pItem->AddRef();
        m_addVarsList.AddTail(pItem);
        m_bAddVarsRequest = true;
    }
}

void CFCVarSessionThread::ForceResubscription()
{
    CSingleLock lock(&m_pRawVarSession->m_cs, TRUE);
    if (!m_bConnected)
        return;
    POSITION pos = m_pRawVarSession->m_varClientHandleMap.GetStartPosition();
    while (pos != NULL) {
        long key;
        CFCVariable *pItem;
        m_pRawVarSession->m_varClientHandleMap.GetNextAssoc(pos, key, pItem);
        if (pItem->getServerHandle() == -1) {
            pItem->AddRef();
            m_addVarsList.AddTail(pItem);
            m_bAddVarsRequest = true;
        }
    }
}

void CFCVarSessionThread::TerminateConnection()
{
    //Lock var session - no requests can be added
    CSingleLock lock(&m_pRawVarSession->m_cs, TRUE);
    m_bConnected = false;

    //Remove any pending add var requests
    if (m_bAddVarsRequest) {
        do {
           m_addVarsList.RemoveHead()->Release();
        }
        while (!m_addVarsList.IsEmpty());
        m_bAddVarsRequest = false;
    }
    
    //Remove any pending write var requests
    if (m_bWriteVarsRequest) {
        m_writeReqList.RemoveAll();
        m_bWriteVarsRequest = false;
    }

    //Remove any pending remove var requests
    if (m_bRemVarsRequest) {
        do {
           m_removeVarsList.RemoveHead()->Release();
        }
        while (!m_removeVarsList.IsEmpty());
        m_bRemVarsRequest = false;
    }

    //Remove any pending commands
    if(m_bCommandRequest) {
        m_CommandList.RemoveAll();
        m_bCommandRequest = false;
    }

    //No shutdown - set server handle of vars subscribed to invalid
    if (!m_bShutdownRequest) {
        POSITION pos = m_pRawVarSession->m_varClientHandleMap.GetStartPosition();
        while (pos != NULL) {
            long key;
            CFCVariable *pItem;
            m_pRawVarSession->m_varClientHandleMap.GetNextAssoc(pos, key, pItem);
            pItem->setServerHandle(0);
        }
    }
}

//------------------------------------------------------------------*
HRESULT CFCVarSessionThread::DoCheckConnection()
{
    HRESULT hr = E_CSC_NO_CONNECT_TO_CONTROL;
    if (m_pCheckConnection)
    {
        hr = m_pCheckConnection->CheckConnection();
        if(SUCCEEDED(hr)) {
            ForceResubscription();
            return hr;
        }

        TRACE(_T("VarSession: DoCheckConnection '%ls' fails 0x%08X!\n"),
                  m_sMachineName, hr);
        TrcPrint(TRC_WARNING, _T("VarSession: DoCheckConnection '%ls' fails 0x%08X!\n"),
                  m_sMachineName, hr);

        //Do not call any interface method except Release()
        FC_RELEASE_PTR(m_pGroup1);
        FC_RELEASE_PTR(m_pOPCServer);
        FC_RELEASE_PTR(m_pControlMgt);
        FC_RELEASE_PTR(m_pCheckConnection);
        m_lKey = 0;
        CoDisconnectObject(this, 0);
    }

    return hr;
}

//------------------------------------------------------------------*
HRESULT CFCVarSessionThread::DoWorkItem()
{
    CSingleLock lock(&m_pRawVarSession->m_cs, TRUE);
    for (;;) {
        if (m_bShutdownRequest)
          break;
        else
        if (m_bAddVarsRequest) {
            int nVars = m_addVarsList.GetCount();
            CFCVariable **pVars = new CFCVariable*[nVars];
            if (!pVars)
                  return E_OUTOFMEMORY;
            for (int i=0; i<nVars; i++)
              pVars[i] = m_addVarsList.RemoveHead();
            m_bAddVarsRequest = false;
            lock.Unlock();
            OPCITEMDEF *pItems = new OPCITEMDEF [nVars];
            if (!pItems)
                  return E_OUTOFMEMORY;
            for (i=0; i<nVars; i++) {
              pItems[i].szAccessPath = NULL;
              pItems[i].szItemID = pVars[i]->m_name.AllocSysString();
              pItems[i].bActive = TRUE;
              pItems[i].hClient = pVars[i]->getClientHandle();
              pItems[i].dwBlobSize = 0;
              pItems[i].pBlob = NULL;
              pItems[i].vtRequestedDataType = VT_EMPTY;
              pItems[i].wReserved = 0;
            }

            OPCITEMRESULT *pResult;
            HRESULT *pErrors;
            HRESULT hr = m_pGroup1->AddItems(nVars, pItems, &pResult, &pErrors);
            if (FAILED(hr)) {
                for (i=0; i<nVars; i++) {
                  SysFreeString(pItems[i].szItemID);
                  pVars[i]->Release();
                }
                delete []pItems;
                delete []pVars;
                return hr;
            }
            lock.Lock();
            if (!m_bShutdownRequest)
              for (i=0; i<nVars; i++) {
                if (SUCCEEDED(pErrors[i])) {
                    pVars[i]->setServerHandle(pResult[i].hServer);
                }
                else {
                    pVars[i]->setServerHandle(-1);
                    TRACE(_T("VarSession: AddItem '%ls' to '%ls' fails 0x%08X!\n"), pItems[i].szItemID, m_sMachineName, pErrors[i]);
                    TrcPrint(TRC_WARNING, _T("VarSession: AddItem '%ls' to '%ls' fails 0x%08X!\n"), pItems[i].szItemID, m_sMachineName, pErrors[i]);
                }
              }
            for (i=0; i<nVars; i++) {
              SysFreeString(pItems[i].szItemID);
              pVars[i]->Release();
            }
            CoTaskMemFree(pResult);
            CoTaskMemFree(pErrors);
            delete []pVars;
            delete []pItems;
        }
        else
        if (m_bWriteVarsRequest) {
            int reqCount=0, iListCount;

            m_writeReqList.EnterCritical();
            //the absolute list count - synchronize count with the copy 
            //retrieved with GetItemList()
            iListCount = m_writeReqList.GetCount();

            //get a copy of the request list, calls AddRef for each entry in list
            CFCVariableWriteReq **ppRequests = m_writeReqList.GetItemList();
            if(!ppRequests) {
                m_writeReqList.LeaveCritical(); //leave request list
                return E_OUTOFMEMORY;
            }

            //Now remove all pending requests, because a copy keeps request alive for
            //servicing below
            m_writeReqList.RemoveAll();
            m_writeReqList.LeaveCritical(); //leave request list
            m_bWriteVarsRequest = false;
            lock.Unlock();
            //from here new requests can be entered.

            //Allocate handle and value array.
            OPCHANDLE *pHandles = new OPCHANDLE[iListCount];
            VARIANT *pValues = new VARIANT[iListCount];

            if (!pHandles || !pValues) {
                //Free the copy of the request list
                m_writeReqList.FreeItemList(ppRequests);
                return E_OUTOFMEMORY;
            }

            //Fill the Handles and value array - only those 
            //variables with a valid server handle will be added to the handle,value arrays
            for(int i = 0;ppRequests[i];i++) {
               CFCVariableWriteReq *pReq = ppRequests[i];
               pHandles[reqCount] = pReq->m_pVariable->getServerHandle();
               if (pHandles[reqCount] != 0 && pHandles[reqCount] != -1) {
                  ::VariantInit(&pValues[reqCount]);
                  //this is a bit dirty cause references are copied as well
                  pValues[reqCount] = pReq->m_Value;
                  reqCount++;
               }
            }

            //Now service all the pending requests.
            if (reqCount>0) { //any requests ?
                IOPCSyncIO *pSyncIO;
                HRESULT *pErrors;
                HRESULT hr = m_pGroup1->QueryInterface(IID_IOPCSyncIO, (void **)&pSyncIO);
                if (SUCCEEDED(hr)) {
                    hr = pSyncIO->Write(reqCount, pHandles, pValues, &pErrors);
                    pSyncIO->Release();
                }
                if (FAILED(hr)) {
                    delete []pHandles;
                    delete []pValues;
                    //Free the copy of the request list
                    m_writeReqList.FreeItemList(ppRequests);
                    return hr;
                }
                while (reqCount-->0) {
                    if (FAILED(pErrors[reqCount])) {
                        //TODO
                        TRACE(_T("VarSession: WriteValue '%s' on '%ls' fails 0x%08X!\n"),
                                  _T(""), m_sMachineName, pErrors[reqCount]);
                        TrcPrint(TRC_WARNING, _T("VarSession: WriteValue '%s' on '%ls' fails 0x%08X!\n"),
                                  _T(""), m_sMachineName, pErrors[reqCount]);
                    }
                }
                CoTaskMemFree(pErrors);
            }
            delete []pHandles;
            delete []pValues;
            //Free the copy of the request list
            m_writeReqList.FreeItemList(ppRequests);
            lock.Lock();
        }
        else
        if (m_bRemVarsRequest) {
            int i=0;
            OPCHANDLE *pHandles = new OPCHANDLE[m_removeVarsList.GetCount()];
            if (!pHandles)
                  return E_OUTOFMEMORY;
            while(!m_removeVarsList.IsEmpty()) {
               CFCVariable *pVar = m_removeVarsList.RemoveHead();
               pHandles[i] = pVar->getServerHandle();
               pVar->setServerHandle(0);
               if (pHandles[i] != 0 && pHandles[i] != -1)
                  i++;
               pVar->Release();
            }
            m_bRemVarsRequest = false;
            lock.Unlock();
            if (i>0) {
                HRESULT *pErrors;
                HRESULT hr  = m_pGroup1->RemoveItems(i, pHandles, &pErrors);
                if (FAILED(hr)) {
                    delete []pHandles;
                    return hr;
                }
                CoTaskMemFree(pErrors);
            }
            delete []pHandles;
            lock.Lock();
        }
        else 
        if (m_bCommandRequest) {
            //Handle the commands
            //get a copy of the request list, calls AddRef for each entry in list
            CCommandReq **ppRequests = m_CommandList.GetItemList();
            if(!ppRequests) {
                return E_OUTOFMEMORY;
            }
            
            //reset command request flag and remove all commands
            m_bCommandRequest = false;
            m_CommandList.RemoveAll();
            lock.Unlock();
            //Serve commands of this cycle
            for(int i= 0;ppRequests[i];i++) {
                ExecuteCommand(ppRequests[i]);
            }
            m_CommandList.FreeItemList(ppRequests);
        }
        else
          break;
    }
    return S_OK;
}

void CFCVarSessionThread::ExecuteCommand(CCommandReq* pReq)
{
    HRESULT hr = S_OK;

    if(!m_pControlMgt) {
        pReq->SetErrorCode(E_NOINTERFACE);
        pReq->SetEvent();
        return;
    }

    switch(pReq->GetType()) {
        case CCommandReq::ColdStart:
            hr = m_pControlMgt->ColdStartResource(m_lKey);
            break;
        case CCommandReq::WarmStart:
            hr = m_pControlMgt->WarmStartResource(m_lKey);
            break;
        case CCommandReq::Pause:
            hr = m_pControlMgt->PauseResource(m_lKey);
            break;
        case CCommandReq::Continue:
            hr = m_pControlMgt->ContinueResource(m_lKey);
            break;
        case CCommandReq::StartAllTasks:
            hr = m_pControlMgt->StartAllTasksOnResource(m_lKey);
            break;
        case CCommandReq::StopAllTasks:
            hr = m_pControlMgt->StopAllTasksOnResource(m_lKey);
            break;
        case CCommandReq::StartTask:
            { CTaskCommand* p = dynamic_cast<CTaskCommand*>(pReq);
              assert(p);
              hr = m_pControlMgt->StartTaskOnResource(m_lKey,p->m_sTask);
            }
            break;
        case CCommandReq::StopTask:
            { CTaskCommand* p = dynamic_cast<CTaskCommand*>(pReq);
              assert(p);
              hr = m_pControlMgt->StopTaskOnResource(m_lKey,p->m_sTask);
            }
            break;
        case CCommandReq::CustDownUpLoad:
            {
                CCustomCommand* p = dynamic_cast<CCustomCommand*>(pReq);
                assert(p);
                hr = CustDownUpLoad(p);
            }
            break;
        case CCommandReq::ProjectInfoUpload:
            {
                CProjectInfoCommand* p = dynamic_cast<CProjectInfoCommand*>(pReq);
                assert(p);
                hr = ProjectInfoUpload(p);
            }
            break;
        case CCommandReq::ProjectDownload:
            {
                CCustomCommand* p = dynamic_cast<CCustomCommand*>(pReq);
                assert(p);
                hr = ProjectDownload(p);
            }
            break;
        case CCommandReq::ProjectUpload:
            {
                CCustomCommand* p = dynamic_cast<CCustomCommand*>(pReq);
                assert(p);
                hr = ProjectUpload(p);
            }
            break;
        case CCommandReq::CustCmd1:
            {
                CCustomCommand* p = dynamic_cast<CCustomCommand*>(pReq);
                assert(p);
                hr = CustCmd1(p);
            }
            break;
        case CCommandReq::CustCmd2:
            {
                CCustomCommand* p = dynamic_cast<CCustomCommand*>(pReq);
                assert(p);
                hr = CustCmd2(p);
            }
            break;
        case CCommandReq::CustCmd3:
            {
                CCustomCommand* p = dynamic_cast<CCustomCommand*>(pReq);
                assert(p);
                hr = CustCmd3(p);
            }
            break;
        case CCommandReq::CustCmd4:
            {
                CCustomCommand* p = dynamic_cast<CCustomCommand*>(pReq);
                assert(p);
                hr = CustCmd4(p);
            }
            break;
        case CCommandReq::CustCmd5:
            {
                CCustomCommand* p = dynamic_cast<CCustomCommand*>(pReq);
                assert(p);
                hr = CustCmd5(p);
            }
            break;
        case CCommandReq::LoadFile:
            {
                CCustomCommand* p = dynamic_cast<CCustomCommand*>(pReq);
                assert(p);
                hr = LoadFile(p);
            }
            break;
        case CCommandReq::SaveFile:
            {
                CCustomCommand* p = dynamic_cast<CCustomCommand*>(pReq);
                assert(p);
                hr = SaveFile(p);
            }
            break;
        case CCommandReq::Dir:
            {
                CCustomCommand* p = dynamic_cast<CCustomCommand*>(pReq);
                assert(p);
                hr = Dir(p);
            }
            break;
        default:
            break;
    }
    pReq->SetErrorCode(hr);
    pReq->SetEvent();
}

//------------------------------------------------------------------*
/*static*/ DWORD CFCVarSessionThread::ThreadMain(LPVOID lpParam)
{
    InterlockedIncrement(&gs_nRunningThreads);
    CoInitializeEx(NULL, COINIT_MULTITHREADED);

    CFCVarSessionThread *_this = (CFCVarSessionThread *)lpParam;
    HRESULT hr = _this->ConnectThread();
    _this->Release();

    CoUninitialize();
    InterlockedDecrement(&gs_nRunningThreads);
    return hr;
}

//------------------------------------------------------------------*
HRESULT CFCVarSessionThread::ConnectThread()
{
    HRESULT hr = CoGetInterfaceAndReleaseStream(m_pVarSessionStream, IID__FCVarSessionNotificationInternal,
                                                (void **)&m_pVarSession);
    m_pVarSessionStream = NULL;
    if (FAILED(hr))
      return hr;

    while(!m_bShutdownRequest) {
        hr = DoConnect();
        if (FAILED(hr)) {
            m_pVarSession->OnStateChanged(m_cookie, interrupted, hr);
            ResetEvent(m_hThreadWakeUp);
            if (!m_bShutdownRequest)
              WaitForSingleObject(m_hThreadWakeUp, 20000);
            continue;
        }
        InitiateConnection();
        m_pVarSession->OnStateChanged(m_cookie, RW_established, S_OK);
        while (!m_bShutdownRequest) {
            hr = DoWorkItem();
            if (FAILED(hr)) {
              m_pVarSession->OnStateChanged(m_cookie, interrupted, hr);
              break;
            }
            if (WAIT_TIMEOUT == WaitForSingleObject(m_hThreadWakeUp, 10000)) {
                hr = DoCheckConnection();
                if (FAILED(hr)) {
                  m_pVarSession->OnStateChanged(m_cookie, interrupted, hr);
                  break;
                }
            }
        }
        TerminateConnection();
        DoDisconnect();

    }

    //m_pVarSession->OnStateChanged(m_cookie, regular_shutdown, S_OK);

    m_pVarSession->Release();
    m_pVarSession = NULL;
    return S_OK;
}

//------------------------------------------------------------------*
HRESULT CFCVarSessionThread::StartThread(CFCVarSession *pRawVarSession, long cookie)
{
    HRESULT hr;
    try {
        DWORD threadID;
        HANDLE hThread = NULL;

        hr = pRawVarSession->get_MachineName(&m_sMachineName);
        if (FAILED(hr))
          return hr;

        hr = CoMarshalInterThreadInterfaceInStream(IID__FCVarSessionNotificationInternal,
                                                   (_FCVarSessionNotificationInternal*)pRawVarSession,
                                                   &m_pVarSessionStream);
        if (FAILED(hr))
          return hr;
        pRawVarSession->AddRef();
        m_pRawVarSession = pRawVarSession;
        m_cookie = cookie;

        AddRef();
        hThread = CreateThread(NULL, 0, ThreadMain, (LPVOID)this, 0, &threadID);
        if (hThread == NULL)
          return E_FAIL;

        CloseHandle(hThread);
    }
    catch(CMemoryException *e) {
        e->Delete();
        return E_OUTOFMEMORY;
    }
    return S_OK;
}

//------------------------------------------------------------------*
HRESULT CFCVarSessionThread::StopThread()
{
    CSingleLock lock(&m_pRawVarSession->m_cs, TRUE);
    m_bShutdownRequest = true;
    POSITION pos = m_pRawVarSession->m_varClientHandleMap.GetStartPosition();
    while (pos != NULL) {
        long key;
        CFCVariable *pItem;
        m_pRawVarSession->m_varClientHandleMap.GetNextAssoc(pos, key, pItem);
        pItem->setServerHandle(0);
    }
    lock.Unlock();
    SetEvent(m_hThreadWakeUp);
    return S_OK;
}

//------------------------------------------------------------------*
HRESULT CFCVarSessionThread::SubscribeVariable(CFCVariable *pVariable)
{
    CSingleLock lock(&m_pRawVarSession->m_cs, TRUE);
    if (!m_bConnected) {
      return S_OK;
    }
    pVariable->AddRef();
    m_addVarsList.AddTail(pVariable);
    m_bAddVarsRequest = true;
    lock.Unlock();
    SetEvent(m_hThreadWakeUp);
    return S_OK;
}

//------------------------------------------------------------------*
HRESULT CFCVarSessionThread::UnSubscribeVariable(CFCVariable *pVariable)
{
    CSingleLock lock(&m_pRawVarSession->m_cs, TRUE);
    if (!m_bConnected) {
      return S_OK;
    }
    pVariable->AddRef();
    m_removeVarsList.AddTail(pVariable);
    m_bRemVarsRequest = true;
    lock.Unlock();
    SetEvent(m_hThreadWakeUp);
    return S_OK;
}

//------------------------------------------------------------------*
HRESULT CFCVarSessionThread::SubmitWriteRequest(CFCVariableWriteReq *pRequest)
{
    CSingleLock lock(&m_pRawVarSession->m_cs, TRUE);
    if (!m_bConnected || pRequest->m_pVariable->getServerHandle() == -1) {
      return E_FAIL;
    }
    m_writeReqList.Add(pRequest);
    m_bWriteVarsRequest = true;
    lock.Unlock();
    SetEvent(m_hThreadWakeUp);
    return S_OK;
}

HRESULT CFCVarSessionThread::SubmitCommandRequest(CCommandReq* pReq)
{
    CSingleLock lock(&m_pRawVarSession->m_cs, TRUE);
    if (!m_bConnected) {
      return E_FAIL;
    }
    
    m_CommandList.Add(pReq);
    m_bCommandRequest = true;
    lock.Unlock();
    SetEvent(m_hThreadWakeUp);
    return S_OK;
}
//------------------------------------------------------------------*
/**
*/
//  IAdviseSink interface
STDMETHODIMP_(void) CFCVarSessionThread::OnDataChange(LPFORMATETC pFE, LPSTGMEDIUM pSTM)
{
    // Verify the format follows the OPC spec
    if (TYMED_HGLOBAL != pFE->tymed) {
        return;
    }
    if (pSTM->hGlobal == 0) {
        return;
    }

    // It must be a data advise
    BYTE* buffer = (BYTE*) GlobalLock(pSTM->hGlobal);
    if (!buffer) {
        return;
    }
    
    if (((OPCGROUPHEADER*) buffer)->dwItemCount != 0) {
        m_pVarSession->OnDataReceived(m_cookie, (long)buffer);
    }


    GlobalUnlock(pSTM->hGlobal);

    return;
}
   
//------------------------------------------------------------------*
/**
*/
STDMETHODIMP_(void) CFCVarSessionThread::OnViewChange(DWORD /*dwAspect*/, LONG /*lindex*/)
{
    return;
}
   
//------------------------------------------------------------------*
/**
*/
STDMETHODIMP_(void) CFCVarSessionThread::OnRename(LPMONIKER /*pmk*/)
{
    return;
}

//------------------------------------------------------------------*
/**
*/
STDMETHODIMP_(void) CFCVarSessionThread::OnSave(void)
{
    return;
}

//------------------------------------------------------------------*
/**
*/
STDMETHODIMP_(void) CFCVarSessionThread::OnClose(void)
{
    return;
}
   
/////////////////////////////////////////////////////////////////////////////
// CFCVariableWriteReq

//------------------------------------------------------------------*
CFCVariableWriteReq::CFCVariableWriteReq(CFCVariable *pVariable, VARIANT value)
: CCommandReq(CCommandReq::VarWriteReq)
{
    ASSERT(pVariable != NULL);
    pVariable->AddRef();
    m_pVariable = pVariable;
    m_Value = value;
}

//------------------------------------------------------------------*
CFCVariableWriteReq::~CFCVariableWriteReq()
{
    if (m_pVariable != NULL)
        m_pVariable->Release();
}

/////////////////////////////////////////////////////////////////////////////
// CFCVarSession

//------------------------------------------------------------------*
CFCVarSession::CFCVarSession()
: m_clsidOPC() , m_clsidCtrl(), m_clsidConn(), m_sMachineName(),
m_strInit()
{
    m_state = not_connected;
    m_dwNextClientHandle = 1;
    m_cookieThread = 1;
    m_pVarSessionThread = NULL;
}

//------------------------------------------------------------------*
CFCVarSession::~CFCVarSession()
{
}

//=======================================================================
//      interface   IFCVarSession
//=======================================================================

//------------------------------------------------------------------*
STDMETHODIMP CFCVarSession::Connect(/*[in, string]*/BSTR sUserName,/*[in, string]*/BSTR sPassword)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    FC_PCHECK_HR(sUserName,E_POINTER);
    FC_PCHECK_HR(sPassword,E_POINTER);
    m_sUser = sUserName;
    m_sPassword = sPassword;

    TrcPrint(TRC_INTERFACE, _T("VarSession: Connect(%s)\n"),(LPCTSTR) m_sMachineName);
    CSingleLock lock(&m_cs, TRUE);
    if (m_pVarSessionThread != NULL)
        return S_OK;

    CComObject<CFCVarSessionThread> *pVarSessionThread;
    HRESULT hr = CComObject<CFCVarSessionThread>::CreateInstance(&pVarSessionThread);
    if (FAILED(hr))
      return hr;

    m_pVarSessionThread = pVarSessionThread;
    // update caller that connection will soon be established!
    m_pVarSessionThread->AddRef();
    m_state = waiting_for_connect;
    FireStateChangeHelper(m_state);
    hr = m_pVarSessionThread->StartThread(this, m_cookieThread);
    
    if (FAILED(hr))
      return hr;
    
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CFCVarSession::Disconnect()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    HRESULT hr = S_OK;

    CSingleLock lock(&m_cs, TRUE);
    TrcPrint(TRC_INTERFACE, _T("VarSession: Disconnect(%s)\n"),(LPCTSTR) m_sMachineName);
    // update caller that connection will soon be established!
    if (m_pVarSessionThread == NULL)
        return S_OK;
    m_cookieThread++;
    m_state = regular_shutdown;
    FireStateChangeHelper(regular_shutdown);
    m_pVarSessionThread->StopThread();
    m_pVarSessionThread->Release();
    m_pVarSessionThread = NULL;

    SetAllQualitiesBad();
    m_state = not_connected;
    m_sUser.Empty();
    FireStateChangeHelper(not_connected);
    return hr;
}

//------------------------------------------------------------------*
void CFCVarSession::AddVariable(CFCVariable *pVar)
{
    CSingleLock lock(&m_cs, TRUE);
    pVar->AddRef();
    pVar->Init2(m_dwNextClientHandle, this);
    m_varClientHandleMap.SetAt(m_dwNextClientHandle, pVar);
    m_dwNextClientHandle++;
    if (m_pVarSessionThread!=NULL)
        m_pVarSessionThread->SubscribeVariable(pVar);
}

//------------------------------------------------------------------*
void CFCVarSession::RemoveVariable(CFCVariable *pVar)
{
    CSingleLock lock(&m_cs, TRUE);
    CFCVariable *pVar2 = NULL;
    m_varClientHandleMap.Lookup(pVar->getClientHandle(), pVar2);
    ASSERT(pVar2 == pVar);
    if (pVar2 == pVar) {
        m_varClientHandleMap.RemoveKey(pVar->getClientHandle());
        if (m_pVarSessionThread!=NULL)
            m_pVarSessionThread->UnSubscribeVariable(pVar);
        pVar->Release();
    }
    
}

//------------------------------------------------------------------*
HRESULT CFCVarSession::WriteValueAsync(CFCVariable *pVar, VARIANT val)
{
    HRESULT hr = E_FAIL;
    CFCVariableWriteReq* pRequest = NULL;

    CSingleLock lock(&m_cs, TRUE);
    if (m_pVarSessionThread!=NULL) {
        CFCVariable *pVar2 = NULL;
        m_varClientHandleMap.Lookup(pVar->getClientHandle(), pVar2);
        ASSERT(pVar2 == pVar);
        if (pVar2 == pVar) {
            
            pRequest = new CFCVariableWriteReq(pVar,val); //ctor calls AddRef()
            if(!pRequest) {
                return E_OUTOFMEMORY;
            }
            pRequest->AddRef();
            hr = m_pVarSessionThread->SubmitWriteRequest(pRequest);
            pRequest->Release();
        }
    }
    return hr;
}

void CFCVarSession::SetAddressInfo(CString & strMachine, CGuid & guid, CString & strInit)
{
    m_sMachineName = strMachine;
    m_clsidConn = guid;
    m_strInit = strInit;

    if(m_clsidConn.IsEmpty()) {
        m_clsidCtrl = CLSID_ControlMgt;
        m_clsidOPC = CLSID_OPCServer;
    }
    else {
        //everything works over coclass custom connect and ICustomConnect interface.
        m_clsidCtrl.Empty();
        m_clsidOPC.Empty();
    }
}

//------------------------------------------------------------------*
STDMETHODIMP CFCVarSession::StartResource()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    HRESULT hr = S_OK;
    DWORD dw;
    CCommandReq* pReq = NULL;

    if(m_state != waiting_for_connect &&
        m_state != RW_established) {
        return E_CSC_SESSION_NOT_CONNECTED;
    }

    hr = CreateCommand(CCommandReq::Continue,pReq);
    if(FAILED(hr)) {
        return hr;
    }

    pReq->AddRef(); //additional reference command queue
    hr = m_pVarSessionThread->SubmitCommandRequest(pReq);
    if(FAILED(hr)) {
        pReq->Release(); //release of the reference for queue
        FC_RELEASE_PTR(pReq); //release of own reference
        return hr;
    }

    dw = WaitWithMessageLoop(pReq->GetEvent(),CSC_COMMAND_REQ_TO);
    if(dw != WAIT_OBJECT_0) { //Time out
        FC_RELEASE_PTR(pReq); //release of own reference
        return E_CSC_REQUEST_TIMEOUT;
    }
    
    //WAIT_OBJECT_0
    hr = pReq->GetErrorCode();
    FC_RELEASE_PTR(pReq); //release of own reference
    TrcPrint(TRC_INTERFACE, _T("VarSession: StartResource returned hr=0x%08X!\n"), hr);
	return hr;
}

//------------------------------------------------------------------*
STDMETHODIMP CFCVarSession::StopResource()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    HRESULT hr = S_OK;
    DWORD dw;
    CCommandReq* pReq = NULL;

    if(m_state != waiting_for_connect &&
        m_state != RW_established) {
        return E_CSC_SESSION_NOT_CONNECTED;
    }

    if (m_pVarSessionThread == NULL) {
        return E_FAIL;
    }

    hr = CreateCommand(CCommandReq::Pause,pReq);
    if(FAILED(hr)) {
        return hr;
    }

    pReq->AddRef(); //additional reference command queue
    hr = m_pVarSessionThread->SubmitCommandRequest(pReq);
    if(FAILED(hr)) {
        pReq->Release(); //release of the reference for queue
        FC_RELEASE_PTR(pReq); //release of own reference
        return hr;
    }

    dw = WaitWithMessageLoop(pReq->GetEvent(),CSC_COMMAND_REQ_TO);
    if(dw != WAIT_OBJECT_0) { //Time out
        FC_RELEASE_PTR(pReq); //release of own reference
        return E_CSC_REQUEST_TIMEOUT;
    }
    
    //WAIT_OBJECT_0
    hr = pReq->GetErrorCode();
    FC_RELEASE_PTR(pReq); //release of own reference
    TrcPrint(TRC_INTERFACE, _T("VarSession: StopResource returned hr=0x%08X!\n"), hr);
	return hr;
}

//------------------------------------------------------------------*
STDMETHODIMP CFCVarSession::ColdstartResource()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    HRESULT hr = S_OK;
    DWORD dw;
    CCommandReq* pReq = NULL;

    if(m_state != waiting_for_connect &&
        m_state != RW_established) {
        return E_CSC_SESSION_NOT_CONNECTED;
    }

    hr = CreateCommand(CCommandReq::ColdStart,pReq);
    if(FAILED(hr)) {
        return hr;
    }

    pReq->AddRef(); //additional reference command queue
    hr = m_pVarSessionThread->SubmitCommandRequest(pReq);
    if(FAILED(hr)) {
        pReq->Release(); //release of the reference for queue
        FC_RELEASE_PTR(pReq); //release of own reference
        return hr;
    }

    dw = WaitWithMessageLoop(pReq->GetEvent(),CSC_COMMAND_REQ_TO);
    if(dw != WAIT_OBJECT_0) { //Time out
        FC_RELEASE_PTR(pReq); //release of own reference
        return S_OK;      // no error after coldstart timeout, the OSAI target
                          // needs very long for this call, because of rebooting the target
                          // the user can control the results in the message view.
                          // General question is: the timout says nothing about the real success of the command
    }
    
    //WAIT_OBJECT_0
    hr = pReq->GetErrorCode();
    FC_RELEASE_PTR(pReq); //release of own reference
    TrcPrint(TRC_INTERFACE, _T("VarSession: ColdstartResource returned hr=0x%08X!\n"), hr);
	return hr;
}

//------------------------------------------------------------------*
STDMETHODIMP CFCVarSession::WarmstartResource()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    HRESULT hr = S_OK;
    DWORD dw;
    CCommandReq* pReq = NULL;

    if(m_state != waiting_for_connect &&
        m_state != RW_established) {
        return E_CSC_SESSION_NOT_CONNECTED;
    }

    hr = CreateCommand(CCommandReq::WarmStart,pReq);
    if(FAILED(hr)) {
        return hr;
    }

    pReq->AddRef(); //additional reference command queue
    hr = m_pVarSessionThread->SubmitCommandRequest(pReq);
    if(FAILED(hr)) {
        pReq->Release(); //release of the reference for queue
        FC_RELEASE_PTR(pReq); //release of own reference
        return hr;
    }

    dw = WaitWithMessageLoop(pReq->GetEvent(),CSC_COMMAND_REQ_TO);
    if(dw != WAIT_OBJECT_0) { //Time out
        FC_RELEASE_PTR(pReq); //release of own reference
        return S_OK;	// see hint at ColdstartResource()!!! 
    }
    
    //WAIT_OBJECT_0
    hr = pReq->GetErrorCode();
    FC_RELEASE_PTR(pReq); //release of own reference
    TrcPrint(TRC_INTERFACE, _T("VarSession: WarmstartResource returned hr=0x%08X!\n"), hr);
	return hr;
}

//------------------------------------------------------------------*
STDMETHODIMP CFCVarSession::StartTaskOnResource(BSTR sTask)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    HRESULT hr = S_OK;
    DWORD dw;
    CCommandReq* pReq = NULL;

    FC_PCHECK_HR(sTask,E_POINTER);

    if(m_state != waiting_for_connect &&
        m_state != RW_established) {
        return E_CSC_SESSION_NOT_CONNECTED;
    }

    hr = CreateTaskCommand(CCommandReq::StartTask,pReq,sTask);
    if(FAILED(hr)) {
        return hr;
    }

    pReq->AddRef(); //additional reference command queue
    hr = m_pVarSessionThread->SubmitCommandRequest(pReq);
    if(FAILED(hr)) {
        pReq->Release(); //release of the reference for queue
        FC_RELEASE_PTR(pReq); //release of own reference
        return hr;
    }

    dw = WaitWithMessageLoop(pReq->GetEvent(),CSC_COMMAND_REQ_TO);
    if(dw != WAIT_OBJECT_0) { //Time out
        FC_RELEASE_PTR(pReq); //release of own reference
        return E_CSC_REQUEST_TIMEOUT;
    }
    
    //WAIT_OBJECT_0
    hr = pReq->GetErrorCode();
    FC_RELEASE_PTR(pReq); //release of own reference
    TrcPrint(TRC_INTERFACE, _T("VarSession: StartTaskOnResource ('%s') returned hr=0x%08X!\n"), (LPCTSTR)CString(sTask), hr);
	return hr;
}

//------------------------------------------------------------------*
STDMETHODIMP CFCVarSession::StopTaskOnResource(BSTR sTask)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    HRESULT hr = S_OK;
    DWORD dw;
    CCommandReq* pReq = NULL;

    FC_PCHECK_HR(sTask,E_POINTER);

    if(m_state != waiting_for_connect &&
        m_state != RW_established) {
        return E_CSC_SESSION_NOT_CONNECTED;
    }

    hr = CreateTaskCommand(CCommandReq::StopTask,pReq,sTask);
    if(FAILED(hr)) {
        return hr;
    }

    pReq->AddRef(); //additional reference command queue
    hr = m_pVarSessionThread->SubmitCommandRequest(pReq);
    if(FAILED(hr)) {
        pReq->Release(); //release of the reference for queue
        FC_RELEASE_PTR(pReq); //release of own reference
        return hr;
    }

    dw = WaitWithMessageLoop(pReq->GetEvent(),CSC_COMMAND_REQ_TO);
    if(dw != WAIT_OBJECT_0) { //Time out
        FC_RELEASE_PTR(pReq); //release of own reference
        return E_CSC_REQUEST_TIMEOUT;
    }
    
    //WAIT_OBJECT_0
    hr = pReq->GetErrorCode();
    FC_RELEASE_PTR(pReq); //release of own reference
    TrcPrint(TRC_INTERFACE, _T("VarSession: StopTaskOnResource ('%s') returned hr=0x%08X!\n"), (LPCTSTR)CString(sTask), hr);
	return hr;
}

//------------------------------------------------------------------*
STDMETHODIMP CFCVarSession::StartAllTasksOnResource()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    HRESULT hr = S_OK;
    DWORD dw;
    CCommandReq* pReq = NULL;

    if(m_state != waiting_for_connect &&
        m_state != RW_established) {
        return E_CSC_SESSION_NOT_CONNECTED;
    }

    hr = CreateCommand(CCommandReq::StartAllTasks,pReq);
    if(FAILED(hr)) {
        return hr;
    }

    pReq->AddRef(); //additional reference command queue
    hr = m_pVarSessionThread->SubmitCommandRequest(pReq);
    if(FAILED(hr)) {
        pReq->Release(); //release of the reference for queue
        FC_RELEASE_PTR(pReq); //release of own reference
        return hr;
    }

    dw = WaitWithMessageLoop(pReq->GetEvent(),CSC_COMMAND_REQ_TO);
    if(dw != WAIT_OBJECT_0) { //Time out
        FC_RELEASE_PTR(pReq); //release of own reference
        return E_CSC_REQUEST_TIMEOUT;
    }
    
    //WAIT_OBJECT_0
    hr = pReq->GetErrorCode();
    FC_RELEASE_PTR(pReq); //release of own reference
    TrcPrint(TRC_INTERFACE, _T("VarSession: StartAllTasksOnResource returned hr=0x%08X!\n"), hr);
	return hr;
}

//------------------------------------------------------------------*
STDMETHODIMP CFCVarSession::StopAllTasksOnResource()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    HRESULT hr = S_OK;
    DWORD dw;
    CCommandReq* pReq = NULL;

    if(m_state != waiting_for_connect &&
        m_state != RW_established) {
        return E_CSC_SESSION_NOT_CONNECTED;
    }

    hr = CreateCommand(CCommandReq::StopAllTasks,pReq);
    if(FAILED(hr)) {
        return hr;
    }

    pReq->AddRef(); //additional reference command queue
    hr = m_pVarSessionThread->SubmitCommandRequest(pReq);
    if(FAILED(hr)) {
        pReq->Release(); //release of the reference for queue
        FC_RELEASE_PTR(pReq); //release of own reference
        return hr;
    }

    dw = WaitWithMessageLoop(pReq->GetEvent(),CSC_COMMAND_REQ_TO);
    if(dw != WAIT_OBJECT_0) { //Time out
        FC_RELEASE_PTR(pReq); //release of own reference
        return E_CSC_REQUEST_TIMEOUT;
    }
    
    //WAIT_OBJECT_0
    hr = pReq->GetErrorCode();
    FC_RELEASE_PTR(pReq); //release of own reference
    TrcPrint(TRC_INTERFACE, _T("VarSession: StopAllTasksOnResource returned hr=0x%08X!\n"), hr);
	return hr;
}

//------------------------------------------------------------------*
STDMETHODIMP CFCVarSession::get_SessionState(CSC_SESSION_STATE *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }

    *pVal = m_state;
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CFCVarSession::get_MachineName(BSTR * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }

    *pVal = m_sMachineName.AllocSysString();
    return S_OK;
}


//=======================================================================
//      interface   _FCVarSessionNotificationInternal
//=======================================================================

STDMETHODIMP
    CFCVarSession::OnStateChanged(/*[in]*/ long cookie, /*[in]*/ CSC_SESSION_STATE newState, /*[in]*/ HRESULT hrError)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
    
    CSingleLock lock(&m_cs, TRUE);
    if (cookie != m_cookieThread)
        return E_FAIL;
    
    m_state = newState;
    FireStateChangeHelper(newState,hrError);

    if (newState == interrupted) {
        SetAllQualitiesBad();
    }
    return S_OK;
}

STDMETHODIMP 
    CFCVarSession::OnDataReceived(/*[in]*/ long cookie, /*[in]*/ long hglobal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CSingleLock lock(&m_cs, TRUE);
    if (cookie != m_cookieThread)
        return E_FAIL;


    BYTE *buffer = (BYTE *)hglobal;
    const OPCGROUPHEADER* pHeader = (OPCGROUPHEADER*) buffer;
    // check pHeader->hClientGroup == m_client
    // pHeader->dwTransactionID
    // pHeader->hrStatus
    
    // for each item in the data stream, get the value and 
    int offset = sizeof(OPCGROUPHEADER);
    for (long index = 0; index < (long) pHeader->dwItemCount; index++, offset += sizeof(OPCITEMHEADER1))
    {
        const OPCITEMHEADER1* pItemHeader = (OPCITEMHEADER1*) &buffer[offset];
        VARIANT* pValue = (VARIANT*) &buffer[pItemHeader->dwValueOffset];

        // lookup item
        CFCVariable*  pItem = NULL;
        if (!m_varClientHandleMap.Lookup(pItemHeader->hClient, pItem)) {
            // unknown item, try next
            continue;
        }
        // Strings and arrays are packed in the stream 
        // requiring unpacking
        if (pValue->vt == VT_BSTR)
        {
            pValue->bstrVal = (BSTR) ((BYTE*) pValue + sizeof(VARIANT) + sizeof(DWORD));
        }
        else if ((pValue->vt & VT_ARRAY) == VT_ARRAY)
        {
            pValue->parray = (SAFEARRAY*)((BYTE*)pValue + sizeof(VARIANT));
            pValue->parray->pvData = ((BYTE*)pValue->parray + sizeof(SAFEARRAY));
        }

        DATE timestamp;
        FileTimeToVariantTime(&pItemHeader->ftTimeStampItem, &timestamp);
        pItem->setContent(*pValue, pItemHeader->wQuality, timestamp);
        pItem->m_pVarColl->Fire_OnNewData((long)pItem, *pValue, pItemHeader->wQuality, timestamp);

        // call into registered callback to tell client
    }
    

    return S_OK;
}


//=======================================================================
void CFCVarSession::SetAllQualitiesBad()
{
    CSingleLock lock(&m_cs, TRUE);
    varHandleList varList;
    POSITION pos = m_varClientHandleMap.GetStartPosition();
    while (pos != NULL) {
        long key;
        CFCVariable *pItem;
        m_varClientHandleMap.GetNextAssoc(pos, key, pItem);
        pItem->AddRef();
        varList.AddTail(pItem);
    }

    FILETIME ftNow;
    DATE dtNow;
    CoFileTimeNow(&ftNow);
    FileTimeToVariantTime(&ftNow, &dtNow);
    while (!varList.IsEmpty()) {
        VARIANT value;
        short quality;
        DATE timestamp;
        VariantInit(&value);
        CFCVariable *pItem = varList.RemoveHead();

        pItem->getContent(&value, &quality, &timestamp);
        if (quality != OPC_QUALITY_BAD) {
            quality = OPC_QUALITY_BAD;
            timestamp = dtNow;
            pItem->setContent(value, quality, timestamp);
            pItem->m_pVarColl->Fire_OnNewData((long)pItem, value, quality, timestamp);
        }
        pItem->Release();
        VariantClear(&value);
    }
}

//------------------------------------------------------------------*
/**
 * Helper to call Fire_OnStateChanged().
 *
 * @param   newState - new session (connection) state.
 * @param   hError - possible error code.
 * @see     CSC_SESSION_STATE.
*/
void CFCVarSession::FireStateChangeHelper(CSC_SESSION_STATE newState, HRESULT hError /*= S_OK*/)
{
    TrcPrint(TRC_INTERFACE, _T("VarSession: OnStateChanged(%s) state='%s'\n"),
        (LPCTSTR) m_sMachineName,GetSessionStateString(newState));
    Fire_OnStateChanged(GetUnknown(), newState, hError);
}

HRESULT CFCVarSession::CreateCommand(int iType, CCommandReq* & pReq)
{
    HANDLE hEvent = NULL;
    pReq = NULL;

    if (m_pVarSessionThread == NULL) {
        return E_FAIL;
    }

    hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if(hEvent == NULL) {
        DWORD dwErr = ::GetLastError();
        return HRESULT_FROM_WIN32(dwErr);
    }

    pReq = new CCommandReq(iType,hEvent); //initial ref count = 1
    if(!pReq) {
        ::CloseHandle(hEvent);
        return E_OUTOFMEMORY;
    }

    return S_OK;
}

HRESULT CFCVarSession::CreateTaskCommand(int iType, CCommandReq* & pReq, BSTR sTask)
{
    HANDLE hEvent = NULL;
    pReq = NULL;

    if (m_pVarSessionThread == NULL) {
        return E_FAIL;
    }

    hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if(hEvent == NULL) {
        DWORD dwErr = ::GetLastError();
        return HRESULT_FROM_WIN32(dwErr);
    }

    pReq = new CTaskCommand(iType,sTask,hEvent); //initial ref count = 1
    if(!pReq) {
        ::CloseHandle(hEvent);
        return E_OUTOFMEMORY;
    }

    return S_OK;
}


HRESULT CFCVarSession::CreateProjectInfoCommand(CCommandReq* & pReq, 
                                                BSTR* psProjVers,
	                                            BSTR* psProjName,
	                                            BSTR* psModified,
	                                            BSTR* psCreated,
                                                BSTR* psOwner,
	                                            BSTR* psComment1,
	                                            BSTR* psComment2,
	                                            BSTR* psComment3)
{
    HANDLE hEvent = NULL;
    pReq = NULL;

    if (m_pVarSessionThread == NULL) 
    {
        return E_FAIL;
    }

    hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if(hEvent == NULL) 
    {
        DWORD dwErr = ::GetLastError();
        return HRESULT_FROM_WIN32(dwErr);
    }

    pReq = new CProjectInfoCommand(CCommandReq::ProjectInfoUpload,
                                   psProjVers,
	                               psProjName,
	                               psModified,
	                               psCreated,
                                   psOwner,
	                               psComment1,
	                               psComment2,
	                               psComment3,
                                   hEvent); //initial ref count = 1
    if(!pReq) 
    {
        ::CloseHandle(hEvent);
        return E_OUTOFMEMORY;
    }

    return S_OK;
}


HRESULT CFCVarSession::CreateUpDownloadCommmand(int iType, CCommandReq* & pReq,
                                                BSTR* psNameDown,
                                                BSTR* psNameUp,
                                                SAFEARRAY **ppDataDown, 
                                                SAFEARRAY **ppDataUp)
{
    HANDLE hEvent = NULL;
    pReq = NULL;

    if (m_pVarSessionThread == NULL) 
    {
        return E_FAIL;
    }

    hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if(hEvent == NULL) 
    {
        DWORD dwErr = ::GetLastError();
        return HRESULT_FROM_WIN32(dwErr);
    }

    pReq = new CCustomCommand(iType,
                              psNameDown,
                              psNameUp,
                              ppDataDown, 
                              ppDataUp,
                              hEvent); //initial ref count = 1
    if(!pReq) 
    {
        ::CloseHandle(hEvent);
        return E_OUTOFMEMORY;
    }

    return S_OK;
}


//=======================================================================
//=======================================================================


//------------------------------------------------------------------*
STDMETHODIMP CFCVarSession::get_IsConnected(BOOL * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    FC_PCHECK_HR(pVal,E_POINTER);
    *pVal = m_state == RW_established;
	return S_OK;
}

STDMETHODIMP CFCVarSession::get_CurrentUser(/*[out, retval]*/ BSTR *pVal)
{
    FC_PCHECK_HR(pVal,E_POINTER);
    *pVal = m_sUser;
    return S_OK;
}

STDMETHODIMP CFCVarSession::ForceDisconnect(/*[in, string]*/ BSTR sUsername, /*[in, string]*/BSTR sPassword,
    /*[in, string]*/ BSTR sEngMachineName)
{
    //calls simply Disconnect()
    return Disconnect();
}




//=======================================================================
//      interface   IFCCustOnlCmds
//=======================================================================
//------------------------------------------------------------------*

STDMETHODIMP CFCVarSession::CustDownUpLoad(/*[in]*/ SAFEARRAY **ppDataDown, 
                                           /*[out, retval]*/ SAFEARRAY **ppDataUp)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    HRESULT hr = S_OK;
    DWORD dw;
    CCommandReq* pReq = NULL;

    if(m_state != waiting_for_connect &&
        m_state != RW_established) 
    {
        return E_CSC_SESSION_NOT_CONNECTED;
    }

    SAFEARRAY* pThreadDataDown;
    hr = ::SafeArrayCopy(*ppDataDown, &pThreadDataDown);
    if (hr != S_OK)
    {
        return hr;
    }
    SAFEARRAY* pThreadDataUp;

    hr = CreateUpDownloadCommmand(CCommandReq::CustDownUpLoad,pReq,
                                    NULL, NULL,
                                    &pThreadDataDown, &pThreadDataUp);
    if(FAILED(hr)) {
        ::SafeArrayDestroy(pThreadDataDown);
        return hr;
    }

    pReq->AddRef(); //additional reference command queue
    hr = m_pVarSessionThread->SubmitCommandRequest(pReq);
    if(FAILED(hr)) {
        ::SafeArrayDestroy(pThreadDataDown);
        pReq->Release(); //release of the reference for queue
        FC_RELEASE_PTR(pReq); //release of own reference
        return hr;
    }

    dw = WaitWithMessageLoop(pReq->GetEvent(),CSC_COMMAND_REQ_TO_LONG);
    if(dw != WAIT_OBJECT_0) { //Time out
        CCustomCommand* pCCReq = dynamic_cast<CCustomCommand*>(pReq);
        if (pCCReq)
        {
            pCCReq->m_bCancel = TRUE;
        }
        FC_RELEASE_PTR(pReq); //release of own reference
        return E_CSC_REQUEST_TIMEOUT;
    }
    
    //WAIT_OBJECT_0
    hr = pReq->GetErrorCode();
    FC_RELEASE_PTR(pReq); //release of own reference
    TrcPrint(TRC_INTERFACE, _T("VarSession: CustDownUpLoad returned hr=0x%08X!\n"), hr);
    if (hr==S_OK)
    {
        hr = ::SafeArrayCopy(pThreadDataUp, ppDataUp);
        ::SafeArrayDestroy(pThreadDataUp);
    }
	return hr;
}


STDMETHODIMP CFCVarSession::ProjectInfoUpload(/*[out, string]*/ BSTR* psProjVers,
	                                          /*[out, string]*/ BSTR* psProjName,
	                                          /*[out, string]*/ BSTR* psModified,
	                                          /*[out, string]*/ BSTR* psCreated,
                                              /*[out, string]*/ BSTR* psOwner,
	                                          /*[out, string]*/ BSTR* psComment1,
	                                          /*[out, string]*/ BSTR* psComment2,
                             	              /*[out, string]*/ BSTR* psComment3)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    HRESULT hr = S_OK;
    DWORD dw;
    CCommandReq* pReq = NULL;

    if(m_state != waiting_for_connect &&
        m_state != RW_established) 
    {
        return E_CSC_SESSION_NOT_CONNECTED;
    }

    hr = CreateProjectInfoCommand(pReq,
                                   psProjVers,
                                   psProjName,
                                   psModified,
                                   psCreated,
                                   psOwner,
                                   psComment1,
                                   psComment2,
                                   psComment3);
    if(FAILED(hr)) {
        return hr;
    }

    pReq->AddRef(); //additional reference command queue
    hr = m_pVarSessionThread->SubmitCommandRequest(pReq);
    if(FAILED(hr)) {
        pReq->Release(); //release of the reference for queue
        FC_RELEASE_PTR(pReq); //release of own reference
        return hr;
    }

    dw = WaitWithMessageLoop(pReq->GetEvent(),CSC_COMMAND_REQ_TO);
    if(dw != WAIT_OBJECT_0) { //Time out
        FC_RELEASE_PTR(pReq); //release of own reference
        return E_CSC_REQUEST_TIMEOUT;
    }
    
    //WAIT_OBJECT_0
    hr = pReq->GetErrorCode();
    FC_RELEASE_PTR(pReq); //release of own reference
    TrcPrint(TRC_INTERFACE, _T("VarSession: CustDownUpLoad returned hr=0x%08X!\n"), hr);
	return hr;
}


STDMETHODIMP CFCVarSession::ProjectDownload(/*[in]*/ SAFEARRAY/*(unsigned char)*/ **ppDataDown)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    HRESULT hr = S_OK;
    DWORD dw;
    CCommandReq* pReq = NULL;

    if(m_state != waiting_for_connect &&
        m_state != RW_established) 
    {
        return E_CSC_SESSION_NOT_CONNECTED;
    }

    SAFEARRAY* pThreadDataDown;
    hr = ::SafeArrayCopy(*ppDataDown, &pThreadDataDown);
    if (hr != S_OK)
    {
        return hr;
    }

    hr = CreateUpDownloadCommmand(CCommandReq::ProjectDownload,pReq,
                                    NULL, NULL,
                                    &pThreadDataDown, NULL);

    if(FAILED(hr)) {
        ::SafeArrayDestroy(pThreadDataDown);
        return hr;
    }

    pReq->AddRef(); //additional reference command queue
    hr = m_pVarSessionThread->SubmitCommandRequest(pReq);
    if(FAILED(hr)) {
        ::SafeArrayDestroy(pThreadDataDown);
        pReq->Release(); //release of the reference for queue
        FC_RELEASE_PTR(pReq); //release of own reference
        return hr;
    }

    dw = WaitWithMessageLoop(pReq->GetEvent(),CSC_COMMAND_REQ_TO_LONG);
    if(dw != WAIT_OBJECT_0) { //Time out
        CCustomCommand* pCCReq = dynamic_cast<CCustomCommand*>(pReq);
        if (pCCReq)
        {
            pCCReq->m_bCancel = TRUE;
        }
        FC_RELEASE_PTR(pReq); //release of own reference
        return E_CSC_REQUEST_TIMEOUT;
    }
    
    //WAIT_OBJECT_0
    hr = pReq->GetErrorCode();
    FC_RELEASE_PTR(pReq); //release of own reference
    TrcPrint(TRC_INTERFACE, _T("VarSession: ProjectDownload returned hr=0x%08X!\n"), hr);
	return hr;
}


STDMETHODIMP CFCVarSession::ProjectUpload(/*[out]*/ SAFEARRAY/*(unsigned char)*/ **ppDataUp)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    HRESULT hr = S_OK;
    DWORD dw;
    CCommandReq* pReq = NULL;

    if(m_state != waiting_for_connect &&
        m_state != RW_established) 
    {
        return E_CSC_SESSION_NOT_CONNECTED;
    }

    SAFEARRAY* pThreadDataUp;

    hr = CreateUpDownloadCommmand(CCommandReq::ProjectUpload,pReq,
                                    NULL, NULL,
                                    NULL, &pThreadDataUp);

    if(FAILED(hr)) {
        return hr;
    }

    pReq->AddRef(); //additional reference command queue
    hr = m_pVarSessionThread->SubmitCommandRequest(pReq);
    if(FAILED(hr)) {
        pReq->Release(); //release of the reference for queue
        FC_RELEASE_PTR(pReq); //release of own reference
        return hr;
    }

    dw = WaitWithMessageLoop(pReq->GetEvent(),CSC_COMMAND_REQ_TO_LONG);
    if(dw != WAIT_OBJECT_0) { //Time out
        CCustomCommand* pCCReq = dynamic_cast<CCustomCommand*>(pReq);
        if (pCCReq)
        {
            pCCReq->m_bCancel = TRUE;
        }
        FC_RELEASE_PTR(pReq); //release of own reference
        return E_CSC_REQUEST_TIMEOUT;
    }
    
    //WAIT_OBJECT_0
    hr = pReq->GetErrorCode();
    FC_RELEASE_PTR(pReq); //release of own reference
    TrcPrint(TRC_INTERFACE, _T("VarSession: ProjectUpload returned hr=0x%08X!\n"), hr);
    if (hr==S_OK)
    {
        hr = ::SafeArrayCopy(pThreadDataUp, ppDataUp);
        ::SafeArrayDestroy(pThreadDataUp);
    }
	return hr;
}


STDMETHODIMP CFCVarSession::CustCmd1(/*[in]*/  SAFEARRAY/*(unsigned char)*/ **ppDataDown, 
                                     /*[out]*/ SAFEARRAY/*(unsigned char)*/ **ppDataUp)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    HRESULT hr = S_OK;
    DWORD dw;
    CCommandReq* pReq = NULL;

    if(m_state != waiting_for_connect &&
        m_state != RW_established) 
    {
        return E_CSC_SESSION_NOT_CONNECTED;
    }

    SAFEARRAY* pThreadDataDown;
    hr = ::SafeArrayCopy(*ppDataDown, &pThreadDataDown);
    if (hr != S_OK)
    {
        return hr;
    }
    SAFEARRAY* pThreadDataUp;

    hr = CreateUpDownloadCommmand(CCommandReq::CustCmd1,pReq,
                                    NULL, NULL,
                                    &pThreadDataDown, &pThreadDataUp);
    
    if(FAILED(hr)) {
        ::SafeArrayDestroy(pThreadDataDown);
        return hr;
    }

    pReq->AddRef(); //additional reference command queue
    hr = m_pVarSessionThread->SubmitCommandRequest(pReq);
    if(FAILED(hr)) {
        ::SafeArrayDestroy(pThreadDataDown);
        pReq->Release(); //release of the reference for queue
        FC_RELEASE_PTR(pReq); //release of own reference
        return hr;
    }

    dw = WaitWithMessageLoop(pReq->GetEvent(),CSC_COMMAND_REQ_TO_LONG);
    if(dw != WAIT_OBJECT_0) { //Time out
        CCustomCommand* pCCReq = dynamic_cast<CCustomCommand*>(pReq);
        if (pCCReq)
        {
            pCCReq->m_bCancel = TRUE;
        }
        FC_RELEASE_PTR(pReq); //release of own reference
        return E_CSC_REQUEST_TIMEOUT;
    }
    
    //WAIT_OBJECT_0
    hr = pReq->GetErrorCode();
    FC_RELEASE_PTR(pReq); //release of own reference
    TrcPrint(TRC_INTERFACE, _T("VarSession: CustCmd1 returned hr=0x%08X!\n"), hr);
    if (hr==S_OK)
    {
        hr = ::SafeArrayCopy(pThreadDataUp, ppDataUp);
        ::SafeArrayDestroy(pThreadDataUp);
    }
	return hr;
}


STDMETHODIMP CFCVarSession::CustCmd2(/*[in]*/  SAFEARRAY/*(unsigned char)*/ **ppDataDown, 
                                     /*[out]*/ SAFEARRAY/*(unsigned char)*/ **ppDataUp)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    HRESULT hr = S_OK;
    DWORD dw;
    CCommandReq* pReq = NULL;

    if(m_state != waiting_for_connect &&
        m_state != RW_established) 
    {
        return E_CSC_SESSION_NOT_CONNECTED;
    }

    SAFEARRAY* pThreadDataDown;
    hr = ::SafeArrayCopy(*ppDataDown, &pThreadDataDown);
    if (hr != S_OK)
    {
        return hr;
    }
    SAFEARRAY* pThreadDataUp;

    hr = CreateUpDownloadCommmand(CCommandReq::CustCmd2,pReq,
                                    NULL, NULL,
                                    &pThreadDataDown, &pThreadDataUp);
    
    if(FAILED(hr)) {
        ::SafeArrayDestroy(pThreadDataDown);
        return hr;
    }

    pReq->AddRef(); //additional reference command queue
    hr = m_pVarSessionThread->SubmitCommandRequest(pReq);
    if(FAILED(hr)) {
        ::SafeArrayDestroy(pThreadDataDown);
        pReq->Release(); //release of the reference for queue
        FC_RELEASE_PTR(pReq); //release of own reference
        return hr;
    }

    dw = WaitWithMessageLoop(pReq->GetEvent(),CSC_COMMAND_REQ_TO_LONG);
    if(dw != WAIT_OBJECT_0) { //Time out
        CCustomCommand* pCCReq = dynamic_cast<CCustomCommand*>(pReq);
        if (pCCReq)
        {
            pCCReq->m_bCancel = TRUE;
        }
        FC_RELEASE_PTR(pReq); //release of own reference
        return E_CSC_REQUEST_TIMEOUT;
    }
    
    //WAIT_OBJECT_0
    hr = pReq->GetErrorCode();
    FC_RELEASE_PTR(pReq); //release of own reference
    TrcPrint(TRC_INTERFACE, _T("VarSession: CustCmd2 returned hr=0x%08X!\n"), hr);
    if (hr==S_OK)
    {
        hr = ::SafeArrayCopy(pThreadDataUp, ppDataUp);
        ::SafeArrayDestroy(pThreadDataUp);
    }
	return hr;
}


STDMETHODIMP CFCVarSession::CustCmd3(/*[in]*/  SAFEARRAY/*(unsigned char)*/ **ppDataDown, 
                                     /*[out]*/ SAFEARRAY/*(unsigned char)*/ **ppDataUp)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    HRESULT hr = S_OK;
    DWORD dw;
    CCommandReq* pReq = NULL;

    if(m_state != waiting_for_connect &&
        m_state != RW_established) 
    {
        return E_CSC_SESSION_NOT_CONNECTED;
    }

    SAFEARRAY* pThreadDataDown;
    hr = ::SafeArrayCopy(*ppDataDown, &pThreadDataDown);
    if (hr != S_OK)
    {
        return hr;
    }
    SAFEARRAY* pThreadDataUp;

    hr = CreateUpDownloadCommmand(CCommandReq::CustCmd3,pReq,
                                    NULL, NULL,
                                    &pThreadDataDown, &pThreadDataUp);
    
    if(FAILED(hr)) {
        ::SafeArrayDestroy(pThreadDataDown);
        return hr;
    }

    pReq->AddRef(); //additional reference command queue
    hr = m_pVarSessionThread->SubmitCommandRequest(pReq);
    if(FAILED(hr)) {
        ::SafeArrayDestroy(pThreadDataDown);
        pReq->Release(); //release of the reference for queue
        FC_RELEASE_PTR(pReq); //release of own reference
        return hr;
    }

    dw = WaitWithMessageLoop(pReq->GetEvent(),CSC_COMMAND_REQ_TO_LONG);
    if(dw != WAIT_OBJECT_0) { //Time out
        CCustomCommand* pCCReq = dynamic_cast<CCustomCommand*>(pReq);
        if (pCCReq)
        {
            pCCReq->m_bCancel = TRUE;
        }
        FC_RELEASE_PTR(pReq); //release of own reference
        return E_CSC_REQUEST_TIMEOUT;
    }
    
    //WAIT_OBJECT_0
    hr = pReq->GetErrorCode();
    FC_RELEASE_PTR(pReq); //release of own reference
    TrcPrint(TRC_INTERFACE, _T("VarSession: CustCmd3 returned hr=0x%08X!\n"), hr);
    if (hr==S_OK)
    {
        hr = ::SafeArrayCopy(pThreadDataUp, ppDataUp);
        ::SafeArrayDestroy(pThreadDataUp);
    }
	return hr;
}


STDMETHODIMP CFCVarSession::CustCmd4(/*[in]*/  SAFEARRAY/*(unsigned char)*/ **ppDataDown, 
                                     /*[out]*/ SAFEARRAY/*(unsigned char)*/ **ppDataUp)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    HRESULT hr = S_OK;
    DWORD dw;
    CCommandReq* pReq = NULL;

    if(m_state != waiting_for_connect &&
        m_state != RW_established) 
    {
        return E_CSC_SESSION_NOT_CONNECTED;
    }

    SAFEARRAY* pThreadDataDown;
    hr = ::SafeArrayCopy(*ppDataDown, &pThreadDataDown);
    if (hr != S_OK)
    {
        return hr;
    }
    SAFEARRAY* pThreadDataUp;

    hr = CreateUpDownloadCommmand(CCommandReq::CustCmd4,pReq,
                                    NULL, NULL,
                                    &pThreadDataDown, &pThreadDataUp);
    
    if(FAILED(hr)) {
        ::SafeArrayDestroy(pThreadDataDown);
        return hr;
    }

    pReq->AddRef(); //additional reference command queue
    hr = m_pVarSessionThread->SubmitCommandRequest(pReq);
    if(FAILED(hr)) {
        ::SafeArrayDestroy(pThreadDataDown);
        pReq->Release(); //release of the reference for queue
        FC_RELEASE_PTR(pReq); //release of own reference
        return hr;
    }

    dw = WaitWithMessageLoop(pReq->GetEvent(),CSC_COMMAND_REQ_TO_LONG);
    if(dw != WAIT_OBJECT_0) { //Time out
        CCustomCommand* pCCReq = dynamic_cast<CCustomCommand*>(pReq);
        if (pCCReq)
        {
            pCCReq->m_bCancel = TRUE;
        }
        FC_RELEASE_PTR(pReq); //release of own reference
        return E_CSC_REQUEST_TIMEOUT;
    }
    
    //WAIT_OBJECT_0
    hr = pReq->GetErrorCode();
    FC_RELEASE_PTR(pReq); //release of own reference
    TrcPrint(TRC_INTERFACE, _T("VarSession: CustCmd4 returned hr=0x%08X!\n"), hr);
    if (hr==S_OK)
    {
        hr = ::SafeArrayCopy(pThreadDataUp, ppDataUp);
        ::SafeArrayDestroy(pThreadDataUp);
    }
	return hr;
}


STDMETHODIMP CFCVarSession::CustCmd5(/*[in]*/  SAFEARRAY/*(unsigned char)*/ **ppDataDown, 
                                     /*[out]*/ SAFEARRAY/*(unsigned char)*/ **ppDataUp)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    HRESULT hr = S_OK;
    DWORD dw;
    CCommandReq* pReq = NULL;

    if(m_state != waiting_for_connect &&
        m_state != RW_established) 
    {
        return E_CSC_SESSION_NOT_CONNECTED;
    }

    SAFEARRAY* pThreadDataDown;
    hr = ::SafeArrayCopy(*ppDataDown, &pThreadDataDown);
    if (hr != S_OK)
    {
        return hr;
    }
    SAFEARRAY* pThreadDataUp;

    hr = CreateUpDownloadCommmand(CCommandReq::CustCmd5,pReq,
                                    NULL, NULL,
                                    &pThreadDataDown, &pThreadDataUp);
    
    if(FAILED(hr)) {
        ::SafeArrayDestroy(pThreadDataDown);
        return hr;
    }

    pReq->AddRef(); //additional reference command queue
    hr = m_pVarSessionThread->SubmitCommandRequest(pReq);
    if(FAILED(hr)) {
        ::SafeArrayDestroy(pThreadDataDown);
        pReq->Release(); //release of the reference for queue
        FC_RELEASE_PTR(pReq); //release of own reference
        return hr;
    }

    dw = WaitWithMessageLoop(pReq->GetEvent(),CSC_COMMAND_REQ_TO_LONG);
    if(dw != WAIT_OBJECT_0) { //Time out
        CCustomCommand* pCCReq = dynamic_cast<CCustomCommand*>(pReq);
        if (pCCReq)
        {
            pCCReq->m_bCancel = TRUE;
        }
        FC_RELEASE_PTR(pReq); //release of own reference
        return E_CSC_REQUEST_TIMEOUT;
    }
    
    //WAIT_OBJECT_0
    hr = pReq->GetErrorCode();
    FC_RELEASE_PTR(pReq); //release of own reference
    TrcPrint(TRC_INTERFACE, _T("VarSession: CustCm51 returned hr=0x%08X!\n"), hr);
    if (hr==S_OK)
    {
        hr = ::SafeArrayCopy(pThreadDataUp, ppDataUp);
        ::SafeArrayDestroy(pThreadDataUp);
    }
	return hr;
}


STDMETHODIMP CFCVarSession::LoadFile(/*[in, string]*/ BSTR sName, /*[out]*/ SAFEARRAY/*(unsigned char)*/ **ppData)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    HRESULT hr = S_OK;
    DWORD dw;
    CCommandReq* pReq = NULL;

    if(m_state != waiting_for_connect &&
        m_state != RW_established) 
    {
        return E_CSC_SESSION_NOT_CONNECTED;
    }

    SAFEARRAY* pThreadDataUp;

    hr = CreateUpDownloadCommmand(CCommandReq::LoadFile,pReq,
                                    NULL, &sName,
                                    NULL, &pThreadDataUp);
    
    if(FAILED(hr)) {
        return hr;
    }

    pReq->AddRef(); //additional reference command queue
    hr = m_pVarSessionThread->SubmitCommandRequest(pReq);
    if(FAILED(hr)) {
        pReq->Release(); //release of the reference for queue
        FC_RELEASE_PTR(pReq); //release of own reference
        return hr;
    }

    dw = WaitWithMessageLoop(pReq->GetEvent(),CSC_COMMAND_REQ_TO_LONG);
    if(dw != WAIT_OBJECT_0) { //Time out
        CCustomCommand* pCCReq = dynamic_cast<CCustomCommand*>(pReq);
        if (pCCReq)
        {
            pCCReq->m_bCancel = TRUE;
        }
        FC_RELEASE_PTR(pReq); //release of own reference
        return E_CSC_REQUEST_TIMEOUT;
    }
    
    //WAIT_OBJECT_0
    hr = pReq->GetErrorCode();
    FC_RELEASE_PTR(pReq); //release of own reference
    TrcPrint(TRC_INTERFACE, _T("VarSession: LoadFile returned hr=0x%08X!\n"), hr);
    if (hr==S_OK)
    {
        hr = ::SafeArrayCopy(pThreadDataUp, ppData);
        ::SafeArrayDestroy(pThreadDataUp);
    }
	return hr;
}


STDMETHODIMP CFCVarSession::SaveFile(/*[in, string]*/ BSTR sName, /*[in]*/ SAFEARRAY/*(unsigned char)*/ **ppData)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    HRESULT hr = S_OK;
    DWORD dw;
    CCommandReq* pReq = NULL;

    if(m_state != waiting_for_connect &&
        m_state != RW_established) 
    {
        return E_CSC_SESSION_NOT_CONNECTED;
    }

    SAFEARRAY* pThreadDataDown;
    hr = ::SafeArrayCopy(*ppData, &pThreadDataDown);
    if (hr != S_OK)
    {
        return hr;
    }

    hr = CreateUpDownloadCommmand(CCommandReq::SaveFile,pReq,
                                    &sName, NULL,
                                    &pThreadDataDown, NULL);
    if(FAILED(hr)) {
        ::SafeArrayDestroy(pThreadDataDown);
        return hr;
    }

    pReq->AddRef(); //additional reference command queue
    hr = m_pVarSessionThread->SubmitCommandRequest(pReq);
    if(FAILED(hr)) {
        ::SafeArrayDestroy(pThreadDataDown);
        pReq->Release(); //release of the reference for queue
        FC_RELEASE_PTR(pReq); //release of own reference
        return hr;
    }

    dw = WaitWithMessageLoop(pReq->GetEvent(),CSC_COMMAND_REQ_TO_LONG);
    if(dw != WAIT_OBJECT_0) { //Time out
        CCustomCommand* pCCReq = dynamic_cast<CCustomCommand*>(pReq);
        if (pCCReq)
        {
            pCCReq->m_bCancel = TRUE;
        }
        FC_RELEASE_PTR(pReq); //release of own reference
        return E_CSC_REQUEST_TIMEOUT;
    }
    
    //WAIT_OBJECT_0
    hr = pReq->GetErrorCode();
    FC_RELEASE_PTR(pReq); //release of own reference
    TrcPrint(TRC_INTERFACE, _T("VarSession: SaveFile returned hr=0x%08X!\n"), hr);
	return hr;
}


STDMETHODIMP CFCVarSession::Dir(/*[out]*/ SAFEARRAY/*(BSTR)*/ **ppFileNames)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    HRESULT hr = S_OK;
    DWORD dw;
    CCommandReq* pReq = NULL;

    if(m_state != waiting_for_connect &&
        m_state != RW_established) 
    {
        return E_CSC_SESSION_NOT_CONNECTED;
    }

    SAFEARRAY* pThreadDataUp;

    hr = CreateUpDownloadCommmand(CCommandReq::Dir,pReq,
                                    NULL, NULL,
                                    NULL, &pThreadDataUp);

    if(FAILED(hr)) {
        return hr;
    }

    pReq->AddRef(); //additional reference command queue
    hr = m_pVarSessionThread->SubmitCommandRequest(pReq);
    if(FAILED(hr)) {
        pReq->Release(); //release of the reference for queue
        FC_RELEASE_PTR(pReq); //release of own reference
        return hr;
    }

    dw = WaitWithMessageLoop(pReq->GetEvent(),CSC_COMMAND_REQ_TO_LONG);
    if(dw != WAIT_OBJECT_0) { //Time out
        CCustomCommand* pCCReq = dynamic_cast<CCustomCommand*>(pReq);
        if (pCCReq)
        {
            pCCReq->m_bCancel = TRUE;
        }
        FC_RELEASE_PTR(pReq); //release of own reference
        return E_CSC_REQUEST_TIMEOUT;
    }
    
    //WAIT_OBJECT_0
    hr = pReq->GetErrorCode();
    FC_RELEASE_PTR(pReq); //release of own reference
    TrcPrint(TRC_INTERFACE, _T("VarSession: Dir returned hr=0x%08X!\n"), hr);
    if (hr==S_OK)
    {
        hr = ::SafeArrayCopy(pThreadDataUp, ppFileNames);
        ::SafeArrayDestroy(pThreadDataUp);
    }
	return hr;
}




//=======================================================================
//      interface   IFCCustOnlCmds
//=======================================================================
//------------------------------------------------------------------*

HRESULT CFCVarSessionThread::CustDownUpLoad(CCustomCommand* pReq)
{
    HRESULT hr;

    TrcPrint(TRC_INTERFACE, _T("CustDownUpLoad\n"));

    if(!m_pControlMgt) 
    {
        return E_NOINTERFACE;
    }

    CComQIPtr<ICustOnlCmds, &IID_ICustOnlCmds> pCOC = m_pControlMgt;
    if (!pCOC) 
    {
        return E_NOINTERFACE;
    }

    hr = pCOC->CustDownUpLoad(pReq->m_ppDataDown, pReq->m_ppDataUp);

    ::SafeArrayDestroy(*(pReq->m_ppDataDown));

    if (pReq->m_bCancel)
    {
        ::SafeArrayDestroy(*(pReq->m_ppDataUp));
        hr = E_FAIL;
    }

    return hr;
}


HRESULT CFCVarSessionThread::ProjectInfoUpload(CProjectInfoCommand* pReq)
{
    TrcPrint(TRC_INTERFACE, _T("CustDownUpLoad\n"));

    if(!m_pControlMgt) 
    {
        return E_NOINTERFACE;
    }

    //CComQIPtr<ICustOnlCmds, &IID_ICustOnlCmds> pCOC = m_pControlMgt;
    CComPtr<ICustOnlCmds> pCOC;
    HRESULT hr = m_pControlMgt->QueryInterface(IID_ICustOnlCmds, (LPVOID*)&pCOC);
    if (!pCOC) 
    {
        return E_NOINTERFACE;
    }
    return pCOC->ProjectInfoUpload(pReq->m_psProjVers,
	                               pReq->m_psProjName,
	                               pReq->m_psModified,
	                               pReq->m_psCreated,
                                   pReq->m_psOwner,
	                               pReq->m_psComment1,
	                               pReq->m_psComment2,
                             	   pReq->m_psComment3);
}


HRESULT CFCVarSessionThread::ProjectDownload(CCustomCommand* pReq)
{
    HRESULT hr;

    TrcPrint(TRC_INTERFACE, _T("CustDownUpLoad\n"));

    if(!m_pControlMgt) 
    {
        return E_NOINTERFACE;
    }

    CComQIPtr<ICustOnlCmds, &IID_ICustOnlCmds> pCOC = m_pControlMgt;
    if (!pCOC) 
    {
        return E_NOINTERFACE;
    }

    hr = pCOC->ProjectDownload(pReq->m_ppDataDown);

    ::SafeArrayDestroy(*(pReq->m_ppDataDown));

    return hr;
}


HRESULT CFCVarSessionThread::ProjectUpload(CCustomCommand* pReq)
{
    HRESULT hr;

    TrcPrint(TRC_INTERFACE, _T("CustDownUpLoad\n"));

    if(!m_pControlMgt) 
    {
        return E_NOINTERFACE;
    }

    CComQIPtr<ICustOnlCmds, &IID_ICustOnlCmds> pCOC = m_pControlMgt;
    if (!pCOC) 
    {
        return E_NOINTERFACE;
    }

    hr = pCOC->ProjectUpload(pReq->m_ppDataUp);

    if (pReq->m_bCancel)
    {
        ::SafeArrayDestroy(*(pReq->m_ppDataUp));
        hr = E_FAIL;
    }

    return hr;
}


HRESULT CFCVarSessionThread::CustCmd1(CCustomCommand* pReq)
{
    HRESULT hr;

    TrcPrint(TRC_INTERFACE, _T("CustDownUpLoad\n"));

    if(!m_pControlMgt) 
    {
        return E_NOINTERFACE;
    }

    CComQIPtr<ICustOnlCmds, &IID_ICustOnlCmds> pCOC = m_pControlMgt;
    if (!pCOC) 
    {
        return E_NOINTERFACE;
    }

    hr = pCOC->CustCmd1(pReq->m_ppDataDown, pReq->m_ppDataUp);

    ::SafeArrayDestroy(*(pReq->m_ppDataDown));

    if (pReq->m_bCancel)
    {
        ::SafeArrayDestroy(*(pReq->m_ppDataUp));
        hr = E_FAIL;
    }

    return hr;
}


HRESULT CFCVarSessionThread::CustCmd2(CCustomCommand* pReq)
{
    HRESULT hr;

    TrcPrint(TRC_INTERFACE, _T("CustDownUpLoad\n"));

    if(!m_pControlMgt) 
    {
        return E_NOINTERFACE;
    }

    CComQIPtr<ICustOnlCmds, &IID_ICustOnlCmds> pCOC = m_pControlMgt;
    if (!pCOC) 
    {
        return E_NOINTERFACE;
    }

    hr = pCOC->CustCmd2(pReq->m_ppDataDown, pReq->m_ppDataUp);

    ::SafeArrayDestroy(*(pReq->m_ppDataDown));

    if (pReq->m_bCancel)
    {
        ::SafeArrayDestroy(*(pReq->m_ppDataUp));
        hr = E_FAIL;
    }

    return hr;
}


HRESULT CFCVarSessionThread::CustCmd3(CCustomCommand* pReq)
{
    HRESULT hr;

    TrcPrint(TRC_INTERFACE, _T("CustDownUpLoad\n"));

    if(!m_pControlMgt) 
    {
        return E_NOINTERFACE;
    }

    CComQIPtr<ICustOnlCmds, &IID_ICustOnlCmds> pCOC = m_pControlMgt;
    if (!pCOC) 
    {
        return E_NOINTERFACE;
    }

    hr = pCOC->CustCmd3(pReq->m_ppDataDown, pReq->m_ppDataUp);
    
    ::SafeArrayDestroy(*(pReq->m_ppDataDown));

    if (pReq->m_bCancel)
    {
        ::SafeArrayDestroy(*(pReq->m_ppDataUp));
        hr = E_FAIL;
    }

    return hr;
}


HRESULT CFCVarSessionThread::CustCmd4(CCustomCommand* pReq)
{
    HRESULT hr;

    TrcPrint(TRC_INTERFACE, _T("CustDownUpLoad\n"));

    if(!m_pControlMgt) 
    {
        return E_NOINTERFACE;
    }

    CComQIPtr<ICustOnlCmds, &IID_ICustOnlCmds> pCOC = m_pControlMgt;
    if (!pCOC) 
    {
        return E_NOINTERFACE;
    }
    
    hr = pCOC->CustCmd4(pReq->m_ppDataDown, pReq->m_ppDataUp);
    
    ::SafeArrayDestroy(*(pReq->m_ppDataDown));

    if (pReq->m_bCancel)
    {
        ::SafeArrayDestroy(*(pReq->m_ppDataUp));
        hr = E_FAIL;
    }

    return hr;
}


HRESULT CFCVarSessionThread::CustCmd5(CCustomCommand* pReq)
{
    HRESULT hr;

    TrcPrint(TRC_INTERFACE, _T("CustDownUpLoad\n"));

    if(!m_pControlMgt) 
    {
        return E_NOINTERFACE;
    }

    CComQIPtr<ICustOnlCmds, &IID_ICustOnlCmds> pCOC = m_pControlMgt;
    if (!pCOC) 
    {
        return E_NOINTERFACE;
    }

    hr = pCOC->CustCmd5(pReq->m_ppDataDown, pReq->m_ppDataUp);

    ::SafeArrayDestroy(*(pReq->m_ppDataDown));

    if (pReq->m_bCancel)
    {
        ::SafeArrayDestroy(*(pReq->m_ppDataUp));
        hr = E_FAIL;
    }

    return hr;
}


HRESULT CFCVarSessionThread::LoadFile(CCustomCommand* pReq)
{
    HRESULT hr;

    TrcPrint(TRC_INTERFACE, _T("CustDownUpLoad\n"));

    if(!m_pControlMgt) 
    {
        return E_NOINTERFACE;
    }

    CComQIPtr<ICustOnlCmds, &IID_ICustOnlCmds> pCOC = m_pControlMgt;
    if (!pCOC) 
    {
        return E_NOINTERFACE;
    }

    hr = pCOC->LoadFile(*(pReq->m_psNameUp), pReq->m_ppDataUp);
    
    if (pReq->m_bCancel)
    {
        ::SafeArrayDestroy(*(pReq->m_ppDataUp));
        hr = E_FAIL;
    }

    return hr;
}

HRESULT CFCVarSessionThread::SaveFile(CCustomCommand* pReq)
{
    HRESULT hr;

    TrcPrint(TRC_INTERFACE, _T("Custom Online Cmds: Save File\n"));

    if(!m_pControlMgt) 
    {
        return E_NOINTERFACE;
    }

    CComQIPtr<ICustOnlCmds, &IID_ICustOnlCmds> pCOC = m_pControlMgt;
    if (!pCOC) 
    {
        return E_NOINTERFACE;
    }

    hr = pCOC->SaveFile(*(pReq->m_psNameDown), pReq->m_ppDataDown);

    ::SafeArrayDestroy(*(pReq->m_ppDataDown));

    return hr;
}

HRESULT CFCVarSessionThread::Dir(CCustomCommand* pReq)
{
    HRESULT hr;

    TrcPrint(TRC_INTERFACE, _T("CustDownUpLoad\n"));

    if(!m_pControlMgt) 
    {
        return E_NOINTERFACE;
    }

    CComQIPtr<ICustOnlCmds, &IID_ICustOnlCmds> pCOC = m_pControlMgt;
    if (!pCOC) 
    {
        return E_NOINTERFACE;
    }

    hr =  pCOC->Dir(pReq->m_ppDataUp);

    if (pReq->m_bCancel)
    {
        ::SafeArrayDestroy(*(pReq->m_ppDataUp));
        hr = E_FAIL;
    }

    return hr;
}




/*
 *----------------------------------------------------------------------------*
 *  $History: VarSession.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE
 * 
 * *****************  Version 3  *****************
 * User: Wi           Date: 10.05.06   Time: 11:32
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * 
 * *****************  Version 2  *****************
 * User: Wi           Date: 10.05.06   Time: 9:52
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * WarmstartResource() returns S_OK!
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * 
 * *****************  Version 17  *****************
 * User: Jd           Date: 8.05.03    Time: 9:52
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * Wait longer for long up/download operataions, perhaps slow
 * connection...
 * 
 * *****************  Version 16  *****************
 * User: Jd           Date: 9.04.03    Time: 10:54
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * OSAI: no error message box after cold start timeout.
 * 
 * *****************  Version 15  *****************
 * User: Jd           Date: 1.04.03    Time: 16:34
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * 
 * *****************  Version 14  *****************
 * User: Jd           Date: 25.03.03   Time: 15:53
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * Asynchron holding of safearrays.
 * 
 * *****************  Version 13  *****************
 * User: Jd           Date: 20.03.03   Time: 16:47
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * More custom online commands.
 * 
 * *****************  Version 12  *****************
 * User: Su           Date: 10.03.03   Time: 10:01
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * Enhanced traces to deliver a more meaningful output. Introduced
 * FireStateChangeHelper to trace all state changes on session.
 * 
 * *****************  Version 11  *****************
 * User: Jd           Date: 25.02.03   Time: 11:49
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * Custom Online commands in non exclusive sessions.
 * 
 * *****************  Version 10  *****************
 * User: Su           Date: 12.09.01   Time: 19:56
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * 
 * *****************  Version 9  *****************
 * User: Su           Date: 29.08.01   Time: 19:10
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * Replaced WaitForSingleObject by WaitWithMessageLoop() due to a STA ->
 * MTA transition.
 * 
 * *****************  Version 8  *****************
 * User: Ef           Date: 17.08.01   Time: 15:23
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * workaround to reset BPs after coldstart/download
 * 
 * *****************  Version 7  *****************
 * User: Su           Date: 13.07.01   Time: 17:47
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * Revised interfaces for version V2.0
 * 
 * *****************  Version 6  *****************
 * User: Su           Date: 11.07.01   Time: 17:42
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * Bugfix with writing variables.
 * 
 * *****************  Version 5  *****************
 * User: Su           Date: 10.07.01   Time: 12:29
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * Revised interfaces for Version 2.0
 * 
 * *****************  Version 4  *****************
 * User: Su           Date: 19.03.01   Time: 19:01
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * Revision
 * 
 * *****************  Version 3  *****************
 * User: Su           Date: 15.03.01   Time: 19:40
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * Made first adaptions to variable CLSIDs, not yet completed.New header,
 * removed private trace files, replaced by tracea.lib
 * 
 * *****************  Version 2  *****************
 * User: Su           Date: 14.03.01   Time: 19:47
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * 
 * *****************  Version 1  *****************
 * User: Su           Date: 14.03.01   Time: 17:21
 * Created in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * Initial rev.
 *==
 *----------------------------------------------------------------------------*
*/
