// CEExecute.cpp : Implementation of CCEExecute
#include "stdafx.h"
#include "CEExecute.h"
#include "CEMsg_i.c"
#include "ExeDelegator.h"


/////////////////////////////////////////////////////////////////////////////
// CCEExecute

//------------------------------------------------------------------*
/**
 * constructor.
 *
 *  initialize exe master object and create synchronize event 
 *  for synchronous call (see ExecuteCommand()).
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
CCEExecute::CCEExecute()
{
    m_tExeMaster.SetCeExecutePtr(this);
    m_hSynchronizeEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    ASSERT(m_hSynchronizeEvent != NULL);
}

//------------------------------------------------------------------*
/**
 * final release.
 *
 *  close synchronize event.
 *
 * @param           
 * @return          -
 * @exception       -
 * @see             -
*/
void CCEExecute::FinalRelease()
{
    ::CloseHandle(m_hSynchronizeEvent);
}

//------------------------------------------------------------------*
/**
 * fire message.
 *
 *  call fire method of ICEMessage connection point.
 *
 * @param            crstrMessage: message to be displayed in message view.
 * @return           -
 * @exception        -
 * @see              -
*/
void CCEExecute::FireMessage(const CString& crstrMessage, HRESULT hr /*= S_OK*/)
{
    CComBSTR    sMessage(crstrMessage);

    Fire_OnNewMessage(NULL, E_FACILITY_CEEXE, hr, sMessage);
}


//------------------------------------------------------------------*
/**
 * get interface pointer.
 *
 * @param           ppIExeMsg: pointer to internal exe message interface pointer
 * @return          result from QueryInterface()
 * @exception       -
 * @see             -
*/
HRESULT CCEExecute::GetInterfacePointer(IExeMsgInternal** ppIExeMsg)
{
    HRESULT hr = QueryInterface(IID_IExeMsgInternal, (void**)ppIExeMsg);
    return hr;
}


//------------------------------------------------------------------*
/**
 * execution of a single exe command.
 * returns when process is ready or timeout occurred
 *
 * @param   sCommand: single exe command to be executed
 *                  may be empty to trigger auto close
 * @param   pICEProjectInfo: pointer to project info interface
 * @param   dwTimeout: timeout for execution (0 means infinite)
 * @param   bSynchronous: synchronous call?
 * @param   pdwReturnValue: return value of process or 0xFFFFFFFF when
 *                          timeout occurred
 *         
 * @return  S_OK :      process returned without error
 *          S_FALSE:    process returned with error or timeout occurred
 *          E_FAIL :    process could not be executed
 *
*/
STDMETHODIMP CCEExecute::ExecuteCommand
(
    /*[in,string]*/ BSTR            sCommand,
    /*[in]*/        ICEProjInfo*    pICEProjectInfo,
    /*[in]*/        DWORD           dwTimeout,
    /*[in]*/        BOOL            bSynchronous,
    /*[in]*/        BOOL            bShowWindow,
    /*[out]*/       DWORD*          pdwReturnValue
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    
    CString strCommand = sCommand;

    DWORD   dwSessionId;
    HRESULT hr;

    hr = m_tExeMaster.OpenSession(dwSessionId, TRUE, bShowWindow, bSynchronous);
    if(hr != S_OK)
    {
        return hr;
    }

    ::ResetEvent(m_hSynchronizeEvent);

    hr = m_tExeMaster.WriteCommand(dwSessionId, strCommand, dwTimeout);
    if(hr != S_OK)
    {
        return hr;
    }

    strCommand.Empty();
    hr = m_tExeMaster.WriteCommand(dwSessionId, strCommand, 0);
    if(hr != S_OK)
    {
        return hr;
    }

    AtlWaitWithMessageLoop(m_hSynchronizeEvent);

    *pdwReturnValue = m_tExeMaster.GetReturnCode(dwSessionId);
    if(m_tExeMaster.GetTimeoutFlag(dwSessionId))
    {
        hr = S_FALSE;
    }
    
    return hr;
}

//------------------------------------------------------------------*
/**
 *
 * @param   pdwSessionId: session id (to be used to identify a session
 *          writing commands or closing)
 * @param   pICEProjectInfo: pointer to project info interface
 * @param   bAutoClose: if true, the session is closed automatically when
 *                      an empty command is encountered.
 *
 * @return  S_OK : ok
 *          E_FAIL : open session failed
*/
STDMETHODIMP CCEExecute::OpenExeSession
(
    /*[out]*/   DWORD*          pdwSessionId,
    /*[in]*/    ICEProjInfo*    pICEProjectInfo,
    /*[in]*/    BOOL            bAutoClose
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    
//    HRESULT hr = m_tExeMaster.OpenSession(*pdwSessionId, pICEProjectInfo, bAutoClose);
    HRESULT hr = m_tExeMaster.OpenSession(*pdwSessionId, bAutoClose);

    return hr;
}

//------------------------------------------------------------------*
/**
 *
 * @param   dwSessionId: session id from OpenExeSession()
 * @param   sCommand: single exe command to be executed
 *                  may be empty to trigger auto close
 * @param   dwTimeout: timeout for execution (0 means infinite)
 *         
 * @return  S_OK : ok
 *          E_FAIL : write failed
 *
*/
STDMETHODIMP CCEExecute::WriteExeCommand
(
    /*[in]*/        DWORD       dwSessionId,
    /*[in,string]*/ BSTR        sCommand,
    /*[in]*/        DWORD       dwTimeout
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    CString strCommand = sCommand;

    HRESULT hr = m_tExeMaster.WriteCommand(dwSessionId, strCommand, dwTimeout);

    return hr;
}

//------------------------------------------------------------------*
/**
 *
 * @param   dwSessionId: session id from OpenExeSession()
 * @param   bKill: if true, current process will be killed
 *         
 * @return  S_OK : ok
 *          E_FAIL : close failed (never should occur)
 *
*/
STDMETHODIMP CCEExecute::CloseExeSession
(
    /*[in]*/    DWORD       dwSessionId,
    /*[in]*/    BOOL        bKill
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    HRESULT hr = m_tExeMaster.CloseSession(dwSessionId, bKill);

    return hr;
}


//------------------------------------------------------------------*
/**
 *
 * @param   bKill: if true, all running processes will be killed
 *         
 * @return  S_OK : ok
 *          E_FAIL : close failed (never should occur)
 *
*/
STDMETHODIMP CCEExecute::CloseAllExeSessions
(
    /*[in]*/    BOOL        bKill
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    HRESULT hr = m_tExeMaster.CloseAllSessions(bKill);

    return hr;
}

