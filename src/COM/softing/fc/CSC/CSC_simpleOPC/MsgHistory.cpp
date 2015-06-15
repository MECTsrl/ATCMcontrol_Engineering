/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_simpleOPC/MsgHistory.cpp 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: MsgHistory.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_simpleOPC/MsgHistory.cpp $
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
 *==
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

#include "MsgHistory.h"
#include "SyncAccess.h"

//----  Static Initializations:   ----------------------------------*

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CFCMessageHistory

//------------------------------------------------------------------*
CFCMessageHistory::CFCMessageHistory()
{
   m_cachedHistoryID = -1;
   m_pControlMgt = NULL;
   m_lSessionKey = 0;

}

//------------------------------------------------------------------*
CFCMessageHistory::~CFCMessageHistory()
{
    Disconnect();
}

//------------------------------------------------------------------*
HRESULT
    CFCMessageHistory::FinalConstruct()
{
    return S_OK;
}

//------------------------------------------------------------------*
void
    CFCMessageHistory::FinalRelease()
{
    Disconnect();
}


//=======================================================================
//      interface   IFCMessageHistoryScripted
//=======================================================================
//------------------------------------------------------------------*
STDMETHODIMP
    CFCMessageHistory::Connect(/*[in, string]*/ BSTR sPassword)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    
    if (m_strMachineName.IsEmpty())
    {
        // set to local machine name
        DWORD dwLen = MAX_COMPUTERNAME_LENGTH;
        VERIFY(::GetComputerName(m_strMachineName.GetBuffer(MAX_COMPUTERNAME_LENGTH + 1), &dwLen));
        m_strMachineName.ReleaseBuffer();
    }
    return DoConnect(sPassword);
}

//------------------------------------------------------------------*
STDMETHODIMP
    CFCMessageHistory::Disconnect()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    return DoDisconnect();
}

//------------------------------------------------------------------*
STDMETHODIMP CFCMessageHistory::get_MachineName(BSTR * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }

    *pVal = m_strMachineName.AllocSysString();
    return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CFCMessageHistory::put_MachineName(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

    if (m_pMessageIndirect != NULL) {
        return E_FAIL;
    }

    // allow only initial name assignment
    if (!m_strMachineName.IsEmpty()) {
        return E_FAIL;
    }

    m_strMachineName = newVal;

	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CFCMessageHistory::get_ConfigName(BSTR * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }

    *pVal = m_strConfigName.AllocSysString();
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CFCMessageHistory::put_ConfigName(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_pMessageIndirect != NULL) {
        return E_FAIL;
    }

    // allow only initial name assignment
    if (!m_strConfigName.IsEmpty()) {
        return E_FAIL;
    }

    m_strConfigName = newVal;

	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CFCMessageHistory::get_IsConnected(BOOL * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }

    *pVal = m_pMessageIndirect != NULL;

	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CFCMessageHistory::get_MaxHistoryLength(BSTR sResource, long *pVal)
{
    if (pVal == NULL) {
        return E_POINTER;
    }

    if (m_pMessageIndirect != NULL) {
        return m_pMessageIndirect->get_MaxHistoryLength(m_lSessionKey, pVal);
    }
    return E_FAIL;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CFCMessageHistory::put_MaxHistoryLength(BSTR sResource, long newVal)
{
    if (newVal <= 0) {
        return E_INVALIDARG;
    }

    if (m_pMessageIndirect != NULL) {
        return m_pMessageIndirect->put_MaxHistoryLength(m_lSessionKey, newVal);
    }
    return E_FAIL;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CFCMessageHistory::ClearHistoryBuffer(BSTR sResource)
{
    if (m_pMessageIndirect != NULL) {
        return m_pMessageIndirect->ClearHistoryBuffer(m_lSessionKey);
    }
    return E_FAIL;
}


//------------------------------------------------------------------*
STDMETHODIMP
    CFCMessageHistory::get_MinHistoryID(BSTR sResource, long *pVal)
{
    HRESULT hr = E_FAIL;
    if (pVal == NULL)
    {
        return E_POINTER;
    }
    *pVal = 0;

    long lFirst = 0, lLast = 0;
    if (m_pMessageIndirect != NULL) {
        hr = m_pMessageIndirect->GetMinMaxHistoryIDs(m_lSessionKey, &lFirst, &lLast);
    }
    
    if (SUCCEEDED(hr))
    {
        *pVal = lFirst;
    }
    return hr;
}


//------------------------------------------------------------------*
STDMETHODIMP
    CFCMessageHistory::get_MaxHistoryID(BSTR sResource, long *pVal)
{
    HRESULT hr = E_FAIL;
    if (pVal == NULL)
    {
        return E_POINTER;
    }
    *pVal = 0;

    long lFirst = 0, lLast = 0;
    if (m_pMessageIndirect != NULL) {
        hr = m_pMessageIndirect->GetMinMaxHistoryIDs(m_lSessionKey, &lFirst, &lLast);
    }
    
    if (SUCCEEDED(hr))
    {
        *pVal = lLast;
    }
    return hr;
}



//------------------------------------------------------------------*
STDMETHODIMP
    CFCMessageHistory::get_MsgText(BSTR sResource, long idHistory, /*[out, string, retval]*/ BSTR *psMessage)
{
    if (psMessage == NULL)
    {
        return E_POINTER;
    }
    *psMessage = NULL;

    if (m_cachedHistoryID != -1 && idHistory == m_cachedHistoryID) {
        *psMessage = m_cachedMessage.Copy();
        return S_OK;
    }

    HRESULT hr = E_FAIL;
    DATE dt;
    if (m_pMessageIndirect != NULL) {
        hr = m_pMessageIndirect->GetHistoryMessage(m_lSessionKey, idHistory, psMessage, &dt);
    }

    if (SUCCEEDED(hr))
    {
        m_cachedHistoryID = idHistory;
        m_cachedMessage = *psMessage;
        m_cachedDate = VariantTimeToLocalTime(dt);
    }
    return hr;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CFCMessageHistory::get_MsgErrorNo(BSTR sResource, long idHistory, /*[out, retval]*/ HRESULT *phError)
{
    if (phError == NULL)
    {
        return E_POINTER;
    }
    *phError = NULL;

    return E_NOTIMPL;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CFCMessageHistory::get_MsgDate(BSTR sResource, long idHistory, /*[out, retval]*/ DATE *pdt)
{
    if (pdt == NULL)
    {
        return E_POINTER;
    }
    *pdt = 0;

    if (m_cachedHistoryID != -1 && idHistory == m_cachedHistoryID) {
        *pdt = m_cachedDate;
        return S_OK;
    }

    HRESULT hr = E_FAIL;
    BSTR    sMsg = NULL;
    if (m_pMessageIndirect != NULL) {
        hr = m_pMessageIndirect->GetHistoryMessage(m_lSessionKey, idHistory, &sMsg, pdt);
    }

    if (SUCCEEDED(hr))
    {
        *pdt = VariantTimeToLocalTime(*pdt);
        m_cachedHistoryID = idHistory;
        m_cachedMessage = sMsg;
        m_cachedDate = *pdt;

        SysFreeString(sMsg);
    }
    return hr;
}


//=======================================================================

//------------------------------------------------------------------*
HRESULT
    CFCMessageHistory::DoConnect(BSTR sPassword)
{
    // try connect
    HRESULT hr;
    
    m_pMessageIndirect = NULL;
    m_cachedHistoryID = -1;

    _ASSERTE(!m_pControlMgt && m_lSessionKey==0);

    hr = ConnectToControlMgt(NULL, sPassword, &m_pControlMgt, &m_lSessionKey);
    if (SUCCEEDED(hr))
    {
        // attempt successful, access message interface
        CComQIPtr<IMessages, &IID_IMessages>    pMsg = m_pControlMgt;
        if (pMsg) {
            hr = pMsg->Connect(m_lSessionKey);
            if (SUCCEEDED(hr))
                m_pMessageIndirect = pMsg;
            return hr;
        }
    }

    return E_FAIL;
}


//------------------------------------------------------------------*
HRESULT
    CFCMessageHistory::DoDisconnect()
{
    // actually disconnect
    if (m_pMessageIndirect)
        m_pMessageIndirect->Disconnect(m_lSessionKey);
    m_pMessageIndirect = NULL;
    m_cachedHistoryID = -1;
    if (m_cachedMessage != NULL)
        m_cachedMessage.Empty();
    if(m_pControlMgt)
    {
        m_pControlMgt->Logout(m_lSessionKey);
        m_pControlMgt = NULL;
        m_lSessionKey = 0;
    }
	return S_OK;
}



/**
 * VariantTimeToLocalTime: convert an UTC Variant Time to Local Time
 */
DATE VariantTimeToLocalTime(DATE dt)
{
    FILETIME fileTime, localTime;
    SYSTEMTIME sysTime;
    DATE retval = dt;
    VariantTimeToSystemTime(dt, &sysTime) &&
    SystemTimeToFileTime(&sysTime, &fileTime) &&
    FileTimeToLocalFileTime(&fileTime, &localTime) &&
    FileTimeToSystemTime(&localTime, &sysTime) &&
    SystemTimeToVariantTime(&sysTime, &dt);
    return dt;
}


/*
 *----------------------------------------------------------------------------*
 *  $History: MsgHistory.cpp $
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
