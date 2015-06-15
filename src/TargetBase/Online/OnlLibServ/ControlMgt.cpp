/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibServ/ControlMgt.cpp 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: ControlMgt.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibServ/ControlMgt.cpp $
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
#include "Security.h"
#include "OpcItem.h"
#include "SysVar.h"
#include "SysVarPool.h"
#include "iecResource.h"

#include "ControlMgt.h"
#include "cfacility.h"
#include "NSpaceholder.h"
#include "CommissioningMgt.h"
#include "CSC_OnlineErr.h"
#include "connection.h"
#include "config.h"


//----  Local Defines:   -------------------------------------------*
#define DEFAULT_CC_HERROR       MAKE_HRESULT(1, E_FACILITY_CONTROL, 0)

//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CList<CControlMgt*, CControlMgt*>  CControlMgt::sm_ListControlMgt;
CCriticalSection                   CControlMgt::sm_csListControlMgt;


//------------------------------------------------------------------*
CControlMgt::CControlMgt()
{
    m_hWakeup = NULL;
    m_hThread = NULL;
    m_bShutdownRequest = TRUE;
    m_lSessionKey = 0;

}

//------------------------------------------------------------------*
CControlMgt::~CControlMgt()
{
    if (m_hThread != NULL)
        CloseHandle(m_hThread);
    if (m_hWakeup != NULL)
        CloseHandle(m_hWakeup);
    while (!m_qMessageQueue.IsEmpty()) {
        BSTR pDummy = m_qMessageQueue.RemoveHead();
        SysFreeString(pDummy);
    }
}

//------------------------------------------------------------------*
HRESULT
    CControlMgt::FinalConstruct()
{
    return S_OK;
}

//------------------------------------------------------------------*
void
    CControlMgt::FinalRelease()
{
    ConnectControlMessages(FALSE);
    StopThread();
}

//------------------------------------------------------------------*
HRESULT
    CControlMgt::SendMsgToEng(BSTR pszMessage)
{
    CSingleLock lock(&sm_csListControlMgt, TRUE);
    POSITION pos = sm_ListControlMgt.GetHeadPosition();
    while (pos != NULL) {
        CControlMgt *pControlMgt = sm_ListControlMgt.GetNext(pos);
        pControlMgt->printString(0, pszMessage, 0);
    }
    return S_OK;
}

//------------------------------------------------------------------*
HRESULT
    CControlMgt::CheckKey(long lKey, BOOL bWriteAccess)
{
    if (lKey == 0 || lKey != m_lSessionKey)
        return E_INVALIDARG;

    if (bWriteAccess && m_SessionRights == CSecurity::ReadOnly)
        return E_ACCESSDENIED;

    return S_OK;
}


//=======================================================================
//      interface   IControlMgt
//=======================================================================

//------------------------------------------------------------------*
STDMETHODIMP
    CControlMgt::Login(/*[in,string]*/ BSTR sUser, /*[in,string]*/ BSTR sPassword, /*[out,retval]*/ long* plKey)
{
	BOOL bHandled = FALSE;

	HRESULT hr = Adapt_Login(sUser, sPassword, plKey, &bHandled);
	if (FAILED(hr) || bHandled == TRUE) {
		return hr;
	}

    if (sUser == NULL || sPassword == NULL || plKey == NULL)
        return E_POINTER;

    CString strPassword = sPassword;
    m_SessionRights = g_security.GetUserRights(strPassword);
    m_lSessionKey = m_SessionRights == CSecurity::NoRights ? 0 : GetTickCount()|1;
    *plKey = m_lSessionKey;
    return m_lSessionKey != 0 ? S_OK : E_ACCESSDENIED;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CControlMgt::Logout(/*[in]*/ long lKey)
{
	BOOL bHandled = FALSE;

	HRESULT hr = Adapt_Logout(lKey, &bHandled);
	if (FAILED(hr) || bHandled == TRUE) {
		return hr;
	}

    hr = CheckKey(lKey);
    if (FAILED(hr)) {
        return hr;
    }

    m_lSessionKey = 0;
    return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CControlMgt::ColdStartResource(/*[in]*/ long lKey)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

	BOOL bHandled = FALSE;

	HRESULT hr = Adapt_ColdStartResource(lKey, &bHandled);
	if (FAILED(hr) || bHandled == TRUE) {
		return hr;
	}

    hr = CheckKey(lKey, TRUE);
    if (FAILED(hr)) {
        return hr;
    }

	hr = myConnection.SendCommand(FALSE, CMD_COLD_START, NULL, 0);

	if (SUCCEEDED(hr))
	{
		myConnection.CheckVersion();
        if (myConnection.IsLogon())
          CCommissioningMgt::ibnConfigurationChanged();
	}

    return hr;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CControlMgt::WarmStartResource(/*[in]*/ long lKey)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

	BOOL bHandled = FALSE;

	HRESULT hr = Adapt_WarmStartResource(lKey, &bHandled);
	if (FAILED(hr) || bHandled == TRUE) {
		return hr;
	}

    hr = CheckKey(lKey, TRUE);
    if (FAILED(hr)) {
        return hr;
    }

	hr = myConnection.SendCommand(FALSE, CMD_WARM_START, NULL, 0);

	if (SUCCEEDED(hr))
	{
		myConnection.CheckVersion();
		if (myConnection.IsLogon())
          CCommissioningMgt::ibnConfigurationChanged();
	}

    return hr;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CControlMgt::PauseResource(/*[in]*/ long lKey)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

	BOOL bHandled = FALSE;

	HRESULT hr = Adapt_PauseResource(lKey, &bHandled);
	if (FAILED(hr) || bHandled == TRUE) {
		return hr;
	}

    hr = CheckKey(lKey, TRUE);
    if (FAILED(hr)) {
        return hr;
    }

    return myConnection.SendCommand(FALSE, CMD_STOP_RESOURCE, NULL, 0);
}

//------------------------------------------------------------------*
STDMETHODIMP
    CControlMgt::ContinueResource(/*[in]*/ long lKey)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

	BOOL bHandled = FALSE;

	HRESULT hr = Adapt_ContinueResource(lKey, &bHandled);
	if (FAILED(hr) || bHandled == TRUE) {
		return hr;
	}

    hr = CheckKey(lKey, TRUE);
    if (FAILED(hr)) {
        return hr;
    }

    return myConnection.SendCommand(FALSE, CMD_START_RESOURCE, NULL, 0);
}

//------------------------------------------------------------------*
STDMETHODIMP
    CControlMgt::StartTaskOnResource(/*[in]*/ long lKey, BSTR sTask)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

	BOOL bHandled = FALSE;

	HRESULT hr = Adapt_StartTaskOnResource(lKey, sTask, &bHandled);
	if (FAILED(hr) || bHandled == TRUE) {
		return hr;
	}

    hr = CheckKey(lKey, TRUE);
    if (FAILED(hr)) {
        return hr;
    }

    hr = E_FAIL;
    CString strTask = sTask;
    long taskid = 0;
    strTask.MakeLower();
    
	if (CSC_LookupTask(strTask, taskid)) 
	{
		IEC_UINT uTask = TRANSLATE_SHORT((IEC_UINT)taskid);

		hr = myConnection.SendCommand(TRUE, CMD_START_TASK, (BYTE *)&uTask, sizeof(uTask));
    }

    return hr;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CControlMgt::StopTaskOnResource(/*[in]*/ long lKey, BSTR sTask)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

	BOOL bHandled = FALSE;

	HRESULT hr = Adapt_StopTaskOnResource(lKey,sTask, &bHandled);
	if (FAILED(hr) || bHandled == TRUE) {
		return hr;
	}

    hr = CheckKey(lKey, TRUE);
    if (FAILED(hr)) {
        return hr;
    }

    hr = E_FAIL;
    CString strTask = sTask;
    long taskid = 0;
    strTask.MakeLower();

    if (CSC_LookupTask(strTask, taskid)) 
	{
		IEC_UINT uTask = TRANSLATE_SHORT((IEC_UINT)taskid);

		hr = myConnection.SendCommand(TRUE, CMD_STOP_TASK, (BYTE *)&uTask, sizeof(uTask));
    }

    return hr;
}


//------------------------------------------------------------------*
STDMETHODIMP
    CControlMgt::StartAllTasksOnResource(/*[in]*/ long lKey)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    
	BOOL bHandled = FALSE;

	HRESULT hr = Adapt_StartAllTasksOnResource(lKey, &bHandled);
	if (FAILED(hr) || bHandled == TRUE) {
		return hr;
	}

	hr = CheckKey(lKey, TRUE);
    if (FAILED(hr)) {
        return hr;
    }

    return myConnection.SendCommand(FALSE, CMD_START_ALL_TASKS, NULL, 0);
}

//------------------------------------------------------------------*
STDMETHODIMP
    CControlMgt::StopAllTasksOnResource(/*[in]*/ long lKey)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    
	BOOL bHandled = FALSE;

	HRESULT hr = Adapt_StopAllTasksOnResource(lKey, &bHandled);
	if (FAILED(hr) || bHandled == TRUE) {
		return hr;
	}
	
	hr = CheckKey(lKey, TRUE);
    if (FAILED(hr)) {
        return hr;
    }

    return myConnection.SendCommand(FALSE, CMD_STOP_ALL_TASKS, NULL, 0);
}


//=======================================================================
//      interface   IMessages
//=======================================================================

//------------------------------------------------------------------*
STDMETHODIMP
    CControlMgt::GetMinMaxHistoryIDs(/*[in]*/ long lKey, long *pFirst, long *pLast)
{
    HRESULT hr = CheckKey(lKey);
    if (FAILED(hr)) {
        return hr;
    }

    return E_NOTIMPL;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CControlMgt::GetHistoryMessage(/*[in]*/ long lKey, long idHistory, BSTR * psMessage, DATE * pdt)
{
    HRESULT hr = CheckKey(lKey);
    if (FAILED(hr)) {
        return hr;
    }

    return E_NOTIMPL;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CControlMgt::get_MaxHistoryLength(/*[in]*/ long lKey, long *pVal)
{
    HRESULT hr = CheckKey(lKey);
    if (FAILED(hr)) {
        return hr;
    }

    return E_NOTIMPL;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CControlMgt::put_MaxHistoryLength(/*[in]*/ long lKey, long newVal)
{
    HRESULT hr = CheckKey(lKey, TRUE);
    if (FAILED(hr)) {
        return hr;
    }

    return E_NOTIMPL;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CControlMgt::ClearHistoryBuffer(/*[in]*/ long lKey)
{
    HRESULT hr = CheckKey(lKey, TRUE);
    if (FAILED(hr)) {
        return hr;
    }

    return E_NOTIMPL;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CControlMgt::Connect(/*[in]*/ long lKey)
{
    HRESULT hr = CheckKey(lKey);
    if (FAILED(hr)) {
        return hr;
    }

    hr = S_OK;
    if (SUCCEEDED(hr) && Is_Proxy_Connected()) {
        hr = StartThread();
        if (SUCCEEDED(hr))
            hr = ConnectControlMessages(TRUE);
    }

    return hr;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CControlMgt::Disconnect(/*[in]*/ long lKey)
{
    HRESULT hr = CheckKey(lKey);
    if (FAILED(hr)) {
        return hr;
    }

    if (m_hThread != NULL) {
        ConnectControlMessages(FALSE);
        StopThread();
    }

    return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CControlMgt::CheckConnection()
{
    return S_OK;
}

//------------------------------------------------------------------*
HRESULT CControlMgt::StartThread()
{
    if (m_hThread != NULL)
        return E_FAIL;
    m_hWakeup = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (m_hWakeup == NULL)
        return E_FAIL;
    //AddRef();
    m_bShutdownRequest = FALSE;
    m_hThread = CreateThread(NULL, 0, ThreadMain, this, 0, &m_dwThreadID);
    if (m_hThread == NULL) {
        //Release();
        CloseHandle(m_hWakeup);
        m_hWakeup = NULL;
        return E_FAIL;
    }
    return S_OK;
}

//------------------------------------------------------------------*
HRESULT CControlMgt::StopThread()
{
    m_bShutdownRequest = TRUE;
    if (m_hThread == NULL || m_hWakeup == NULL)
        return E_FAIL;
    if (m_dwThreadID == GetCurrentThreadId()) // can't self terminate (would deadlock otherwise)...
        return E_FAIL;                        // this usually happens in CoDisconnectObject call in MessageThread
    SetEvent(m_hWakeup);
    WaitForSingleObject(m_hThread, INFINITE);
    CloseHandle(m_hWakeup);
    m_hWakeup = NULL;
    CloseHandle(m_hThread);
    m_hThread = NULL;
    return S_OK;
}




//=======================================================================
//      interface   IDebugOut
//=======================================================================

//------------------------------------------------------------------*
STDMETHODIMP
    CControlMgt::printString(long /*id*/, BSTR sMessage, DATE /*dt*/)
{
    CSingleLock lock(&m_csMessageQueue, TRUE);
    if (m_bShutdownRequest)
        return E_FAIL;

    if (m_qMessageQueue.GetCount()<1000) {
        BSTR msgCopy = SysAllocString(sMessage);
        if (msgCopy == NULL)
            return E_FAIL;
        m_qMessageQueue.AddTail(msgCopy);
        SetEvent(m_hWakeup);
    }

    return S_OK;
}

//------------------------------------------------------------------*
HRESULT CControlMgt::MessageThread()
{
    while (!m_bShutdownRequest) {
      BSTR lpNextMessage = NULL;
      CSingleLock lock(&m_csMessageQueue, TRUE);
      if (!m_qMessageQueue.IsEmpty())
          lpNextMessage = m_qMessageQueue.RemoveHead();
      lock.Unlock();
      if (lpNextMessage == NULL) {
          WaitForSingleObject(m_hWakeup, INFINITE);
      }
      else {
        HRESULT hr = Fire_OnNewMessage(lpNextMessage);
        SysFreeString(lpNextMessage);
        if (FAILED(hr)) {
            CoDisconnectObject(GetUnknown(), 0);
            return S_OK;//this usually deletes all references, so I can't touch this here any more...
        }
      }
    }

    CSingleLock lock(&m_csMessageQueue, TRUE);
    while (!m_qMessageQueue.IsEmpty()) {
        BSTR pDummy = m_qMessageQueue.RemoveHead();
        SysFreeString(pDummy);
    }
    return S_OK;
}

//------------------------------------------------------------------*
DWORD WINAPI CControlMgt::ThreadMain(LPVOID lpParam)
{
    CoInitializeEx(NULL, COINIT_MULTITHREADED);
    CControlMgt *_this = (CControlMgt *)lpParam;
    HRESULT hr = _this->MessageThread();
    //_this->Release();
    CoUninitialize();
    return hr;
}


//------------------------------------------------------------------*
HRESULT
    CControlMgt::ConnectControlMessages(BOOL bConnect)
{
    CSingleLock lock(&sm_csListControlMgt, TRUE);
    if (bConnect) {
        sm_ListControlMgt.AddTail(this);
        //this->AddRef();
    }
    else {
        POSITION pos = sm_ListControlMgt.Find(this);
        if (pos == NULL)
            return E_FAIL;
        sm_ListControlMgt.RemoveAt(pos);
        //this->Release();
    }
    return S_OK;
}


//------------------------------------------------------------------*
VOID CConnection::HandleMessages(UINT nMessages, LPSTR lpstrMsg, UINT cbMsg)
{
    for (UINT i = 0; i<nMessages; i++){
        UINT len1 = strlen(lpstrMsg)+1;
        if (len1 > cbMsg)
            break;
        CControlMgt::SendMsgToEng(CComBSTR(lpstrMsg));
        lpstrMsg += len1;
        cbMsg -= len1;
    }
}



//=======================================================================
//      interface   ICustOnlCmds
//=======================================================================
//------------------------------------------------------------------*
STDMETHODIMP
    CControlMgt::CustDownUpLoad(/*[in]*/  SAFEARRAY/*(unsigned char)*/ **ppDataDown, 
                                          /*[out]*/ SAFEARRAY/*(unsigned char)*/ **ppDataUp)
{
    HRESULT hr;
    unsigned char HUGEP *parr;

    // Get a pointer to the elements of the array.
    hr = ::SafeArrayAccessData(*ppDataDown, (void HUGEP**)&parr);
    if(FAILED(hr))
    {
        return hr;
    }

    UINT uLen	= ((*ppDataDown)->rgsabound[0]).cElements;
	BYTE *pData = NULL;

	HEAP_ALLOC(pData, uLen);
    
    memcpy(pData, parr, uLen);

    ::SafeArrayUnaccessData(*ppDataDown);	    

	hr = myConnection.TransferData(FALSE, CMD_CUSTOM, &pData, &uLen);

    if (hr!=S_OK)
    {
        if (pData)
        {
            HEAP_FREE(pData, uLen);
        }
        return hr;
    }

    SAFEARRAYBOUND Bound;
    Bound.lLbound = 0;
    Bound.cElements = uLen;

    (*ppDataUp) = ::SafeArrayCreate(VT_UI1, 1, &Bound);
    if (NULL == (*ppDataUp))
    {
        return E_FAIL;    
    }

    long lIndex = 0;
    
    for(lIndex = 0; lIndex<(long)uLen; lIndex++)
    {
        unsigned char ch = pData[lIndex];
        hr = ::SafeArrayPutElement((*ppDataUp), &lIndex, 
                                    (void*)(&ch));
        if (FAILED(hr))
        {
            if (pData)
            {
                HEAP_FREE(pData, uLen);
            }
            return hr;
        }
    }

    if (pData)
    {
        HEAP_FREE(pData, uLen);
    }

    return hr;
}



STDMETHODIMP
    CControlMgt::ProjectInfoUpload(/*[out, string]*/ BSTR* psProjVers,
	                                         /*[out, string]*/ BSTR* psProjName,
	                                         /*[out, string]*/ BSTR* psModified,
	                                         /*[out, string]*/ BSTR* psCreated,
                                             /*[out, string]*/ BSTR* psOwner,
	                                         /*[out, string]*/ BSTR* psComment1,
	                                         /*[out, string]*/ BSTR* psComment2,
	                                         /*[out, string]*/ BSTR* psComment3)
{
    HRESULT hr = S_OK;

    CString strProjVers;
    CString strProjName;
    CString strModified;
    CString strCreated;
    CString strOwner;
    CString strComment1;
    CString strComment2;
    CString strComment3;

    if (psProjVers  == NULL || 
        psProjName  == NULL || 
        psModified  == NULL || 
        psCreated   == NULL || 
        psOwner     == NULL || 
        psComment1  == NULL || 
        psComment2  == NULL || 
        psComment3  == NULL)
    {
        return E_POINTER;
    }

    *psProjVers = NULL;
    *psProjName = NULL; 
    *psModified = NULL; 
    *psCreated  = NULL; 
    *psOwner    = NULL; 
    *psComment1 = NULL; 
    *psComment2 = NULL; 
    *psComment3 = NULL;

    UINT uLen	= 0;
	BYTE *pData = NULL;

	HEAP_ALLOC(pData, uLen);

	hr = myConnection.TransferData(FALSE, CMD_GET_PROJ_INFO, &pData, &uLen);

	if (uLen < sizeof(XProjInfo))
	{
		hr = E_FAIL;
	}

	if (SUCCEEDED(hr))
	{
        try
        {
            XProjInfo* pPI = (XProjInfo*)pData;

            strProjVers = pPI->szProjVers;
            strProjName = pPI->szProjName;
            strModified = pPI->szModified;
            strCreated  = pPI->szCreated;
            strOwner    = pPI->szOwner;
            strComment1 = pPI->szComment1;
            strComment2 = pPI->szComment2;
            strComment3 = pPI->szComment3;

            *psProjVers = strProjVers.AllocSysString();
            *psProjName = strProjName.AllocSysString();
            *psModified = strModified.AllocSysString();
            *psCreated  = strCreated.AllocSysString();
            *psOwner    = strOwner.AllocSysString();
            *psComment1 = strComment1.AllocSysString();
            *psComment2 = strComment2.AllocSysString();
            *psComment3 = strComment3.AllocSysString(); 
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            
            if (*psProjVers)
            {
                SysFreeString(*psProjVers);
            }
            if (*psProjName)
            {
                SysFreeString(*psProjName);
            }
            if (*psModified)
            {
                SysFreeString(*psModified);
            }
            if (*psCreated)
            {
                SysFreeString(*psCreated);
            }
            if (*psOwner)
            {
                SysFreeString(*psOwner);
            }
            if (*psComment1)
            {
                SysFreeString(*psComment1);
            }
            if (*psComment2)
            {
                SysFreeString(*psComment2);
            }
            if (*psComment3)
            {
                SysFreeString(*psComment3);
            }

            hr = E_OUTOFMEMORY;
        }	
    }

	HEAP_FREE(pData, uLen);

    return hr;
}


STDMETHODIMP
    CControlMgt::ProjectDownload(/*[in]*/ SAFEARRAY/*(unsigned char)*/ **ppDataDown)
{
    HRESULT hr;
    unsigned char HUGEP *parr;

    // Get a pointer to the elements of the array.
    hr = ::SafeArrayAccessData(*ppDataDown, (void HUGEP**)&parr);
    if(FAILED(hr))
    {
        return hr;
    }

    UINT uLen	= ((*ppDataDown)->rgsabound[0]).cElements;
	BYTE *pData = NULL;

	HEAP_ALLOC(pData, uLen);
    
    memcpy(pData, parr, uLen);

    ::SafeArrayUnaccessData(*ppDataDown);	    

	hr = myConnection.TransferData(FALSE, CMD_SAVE_PROJECT, &pData, &uLen);

    if (pData)
    {
        HEAP_FREE(pData, uLen);
    }

    return hr;
}


STDMETHODIMP
    CControlMgt::ProjectUpload(/*[out]*/ SAFEARRAY/*(unsigned char)*/ **ppDataUp)
{
    HRESULT hr = S_OK;

    UINT uLen	= 0;
	BYTE *pData = NULL;

	HEAP_ALLOC(pData, uLen);

  	hr = myConnection.TransferData(FALSE, CMD_LOAD_PROJECT, &pData, &uLen);

    if (hr!=S_OK)
    {
        if (pData)
        {
            HEAP_FREE(pData, uLen);
        }
        return hr;
    }

    SAFEARRAYBOUND Bound;
    Bound.lLbound = 0;
    Bound.cElements = uLen;

    (*ppDataUp) = ::SafeArrayCreate(VT_UI1, 1, &Bound);
    if (NULL == (*ppDataUp))
    {
        return E_FAIL;    
    }

    long lIndex = 0;
    
    for(lIndex = 0; lIndex<(long)uLen; lIndex++)
    {
        unsigned char ch = pData[lIndex];
        hr = ::SafeArrayPutElement((*ppDataUp), &lIndex, 
                                    (void*)(&ch));
        if (FAILED(hr))
        {
            if (pData)
            {
                HEAP_FREE(pData, uLen);
            }
            return hr;
        }
    }

    if (pData)
    {
        HEAP_FREE(pData, uLen);
    }

    return hr;
}

//------------------------------------------------------------------*
/**
 * custom command 1
 *
 *  simple online command with no parameters
 *  number of trials can be specified (see below)
 *
 *  meaning of input chars
 *  0: command id (see vmShared.h)
 *  1: number of trials -> DoSimpleCommand()
 *  lower bound of array must be 0
 *
 * @param            ppDataDown: pointer to input data
 * @param            ppDataUp: pointer to output data (may be NULL)
 * @return           hr
 * @exception        -
 * @see              DoSimpleCommand()
*/
STDMETHODIMP CControlMgt::CustCmd1(/*[in]*/  SAFEARRAY/*(unsigned char)*/ **ppDataDown, 
                                   /*[out]*/ SAFEARRAY/*(unsigned char)*/ **ppDataUp)
{
    HRESULT hr;
    unsigned char HUGEP *parr;
    unsigned char ucCommand;            // command id
    unsigned char ucTrials = 1u;        // number of trials

    // Get a pointer to the elements of the array.
    LONG    lUBound;
    hr = ::SafeArrayGetUBound(*ppDataDown, 1, &lUBound);
    if(hr != S_OK)
    {
        return hr;
    }

    if(lUBound < 0)
    {
        return E_FAIL;
    }

    hr = ::SafeArrayAccessData(*ppDataDown, (void HUGEP**)&parr);
    if(FAILED(hr))
    {
        return hr;
    }

    ucCommand = parr[0];       // first element is command
    if(lUBound > 0)
    {
        ucTrials = parr[1];    // second element is number of trials
    }

  	hr = DoSimpleCommand(ucCommand, ucTrials);

    return hr;
}


//------------------------------------------------------------------*
HRESULT CControlMgt::DoSimpleCommand(UINT uCmd, UINT uTrials)
{
	HRESULT hr = S_OK;

    if(uTrials == 1)
    {
        UINT uLen = 0;
	    BYTE *pData = NULL;

	    HEAP_ALLOC(pData, uLen);

	    hr = myConnection.TransferData(FALSE, uCmd, &pData, &uLen);

        if (pData)
        {
            HEAP_FREE(pData, uLen);
        }
    }
    else
    {
    	UINT uResult = (UINT)-1;

        for (IEC_UINT uCount = 0; SUCCEEDED(hr) && uResult != 0 && uCount < uTrials; uCount++) 
	    {
		    UINT uLen	= sizeof(IEC_UINT);
		    BYTE *pData = NULL;

		    if (uCount > 0) {
			    SleepEx(100, TRUE);
		    }

		    HEAP_ALLOC(pData, uLen);

		    *(IEC_UINT*)pData = TRANSLATE_SHORT(uCount);

		    hr = myConnection.TransferData(FALSE, uCmd, &pData, &uLen);

		    if (SUCCEEDED(hr) && uLen < sizeof(IEC_UINT)) {
			    hr = E_FAIL;
		    }
		    
		    if (SUCCEEDED(hr)) {
			    uResult = TRANSLATE_SHORT(*(IEC_UINT *)pData);
		    }

		    HEAP_FREE(pData, uLen);
	    }						
	    if (uCount == uTrials) 
        {
		    hr = E_FAIL;
	    }
    }

	return hr;
}


STDMETHODIMP
    CControlMgt::LoadFile(/*[in, string]*/ BSTR sName, /*[out]*/ SAFEARRAY/*(unsigned char)*/ **ppData)
{
    HRESULT hr;
    
    CString strName = sName;
    
    UINT uLen= strName.GetLength() + 1;

	BYTE *pData = NULL;

	HEAP_ALLOC(pData, uLen);

    strcpy((char*)pData, strName);
    pData[uLen-1] = 0;

	hr = myConnection.TransferData(FALSE, CMD_LOAD_FILE, &pData, &uLen);   

    if (hr!=S_OK)
    {
        if (pData)
        {
            HEAP_FREE(pData, uLen);
        }
        return hr;
    }

    SAFEARRAYBOUND Bound;
    Bound.lLbound = 0;
    Bound.cElements = uLen;

    (*ppData) = ::SafeArrayCreate(VT_UI1, 1, &Bound);
    if (NULL == (*ppData))
    {
        return E_FAIL;    
    }

    long lIndex = 0;
    
    for(lIndex = 0; lIndex<(long)uLen; lIndex++)
    {
        unsigned char ch = pData[lIndex];
        hr = ::SafeArrayPutElement((*ppData), &lIndex, 
                                    (void*)(&ch));
        if (FAILED(hr))
        {
            if (pData)
            {
                HEAP_FREE(pData, uLen);
            }
            return hr;
        }
    }

    if (pData)
    {
        HEAP_FREE(pData, uLen);
    }

    return hr;
}


STDMETHODIMP
    CControlMgt::SaveFile(/*[in, string]*/ BSTR sName, /*[in]*/ SAFEARRAY/*(unsigned char)*/ **ppData)
{
    HRESULT hr;
    unsigned char HUGEP *parr;
    CString strName = sName;

    // Get a pointer to the elements of the array.
    hr = ::SafeArrayAccessData(*ppData, (void HUGEP**)&parr);
    if(FAILED(hr))
    {
        return hr;
    }

    UINT uLenData	= ((*ppData)->rgsabound[0]).cElements;
    UINT uLenString = strName.GetLength() + 1;
    UINT uLen = uLenData + uLenString;

	BYTE *pData = NULL;

	HEAP_ALLOC(pData, uLen);

    strcpy((char*)pData, strName);
    pData[uLenString-1] = 0;
    
    memcpy(&(pData[uLenString]), parr, uLenData);

    ::SafeArrayUnaccessData(*ppData);	    

	hr = myConnection.TransferData(FALSE, CMD_SAVE_FILE, &pData, &uLen); 

    if (pData)
    {
        HEAP_FREE(pData, uLen);
    }

    return hr;
}


STDMETHODIMP
    CControlMgt::Dir(/*[out]*/ SAFEARRAY/*(BSTR)*/ **ppFileNames)
{
    HRESULT hr = S_OK;

    UINT uLen	= 0;
	BYTE *pData = NULL;

	HEAP_ALLOC(pData, uLen);

  	hr = myConnection.TransferData(FALSE, CMD_DIR_CONTENT, &pData, &uLen); 

    if (hr!=S_OK)
    {
        if (pData)
        {
            HEAP_FREE(pData, uLen);
        }
        return hr;
    }

    SAFEARRAYBOUND Bound;
    Bound.lLbound = 0;
    Bound.cElements = uLen;

    (*ppFileNames) = ::SafeArrayCreate(VT_UI1, 1, &Bound);
    if (NULL == (*ppFileNames))
    {
        return E_FAIL;    
    }

    long lIndex = 0;
    
    for(lIndex = 0; lIndex<(long)uLen; lIndex++)
    {
        unsigned char ch = pData[lIndex];
        hr = ::SafeArrayPutElement((*ppFileNames), &lIndex, 
                                    (void*)(&ch));
        if (FAILED(hr))
        {
            if (pData)
            {
                HEAP_FREE(pData, uLen);
            }
            return hr;
        }
    }

    if (pData)
    {
        HEAP_FREE(pData, uLen);
    }

    return hr;
}

/* ---------------------------------------------------------------------------- */
