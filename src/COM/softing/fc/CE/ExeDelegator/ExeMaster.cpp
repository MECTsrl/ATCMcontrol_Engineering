
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "ExeMaster.h"
#include "ExeThread.h"
#include "CEExecute.h"
#include "utilif.h"
#include "ExeDelegatorErr.h"

//----  Static Initializations:   ----------------------------------*


CExeMaster::CExeMaster()
{
    m_dwSessionCount = 0;
    m_pCeExecute = NULL;
}

//------------------------------------------------------------------*
/**
 * set pointer to CCEExecute object.
 *
 *  store pointer to CCEExecute object for calling its FireMessage() method.
 *
 * @param           pCeExecute: pointer to CCEExecute object
 * @return          -
 * @exception       -
 * @see             -
*/
void CExeMaster::SetCeExecutePtr(CCEExecute* pCeExecute)
{
    m_pCeExecute = pCeExecute;
}

//------------------------------------------------------------------*
/**
 * destructor.
 *
 *  delete all exe thread objects.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
CExeMaster::~CExeMaster()
{
    if(!m_tThreadList.IsEmpty())
    {
        CExeThread* pExeThread;
        POSITION    pos = m_tThreadList.GetHeadPosition();
        while(pos)
        {
            pExeThread = m_tThreadList.GetNext(pos);
            ASSERT(pExeThread);
            if(pExeThread)
            {
                delete pExeThread;
            }
        }
    }
}


//------------------------------------------------------------------*
/**
 * open session.
 *
 * @param            rdwSessionId: id of execute session (needed to reference new session)
 * @param            bAutoClose: close thread after empty command?
 * @param            bShowWindow: show process window?
 * @param            bSynchronous: synchronous call?
 * @return           S_OK: successful
 *                   E_OUTOFMEMORY: no memory left
 *                   error return values of GetInterfacePointer() or CExeThread::create()
 * @exception        -
 * @see              -
*/
HRESULT CExeMaster::OpenSession(DWORD& rdwSessionId, BOOL bAutoClose /*= TRUE*/, BOOL bShowWindow /*=TRUE*/, BOOL bSynchronous /*=TRUE*/)
{
    HRESULT hr;
    DWORD   dwNewSessionId = m_dwSessionCount + 1;
    CExeThread* pExeThread = new CExeThread(this, dwNewSessionId, bAutoClose, bShowWindow, bSynchronous);

    if(!pExeThread)
    {
        return E_OUTOFMEMORY;
    }

    IExeMsgInternal*    pIExeMsg;

    hr = m_pCeExecute->GetInterfacePointer(&pIExeMsg);
    if(hr != S_OK)
    {
        return hr;
    }

    pExeThread->m_pIStream = NULL;

    hr = ::CoMarshalInterThreadInterfaceInStream(IID_IExeMsgInternal, pIExeMsg, &pExeThread->m_pIStream);
    if(hr == S_OK)
    {
        // create thread object and start thread
        hr = pExeThread->create(10, 0, NULL);
        if(hr != S_OK)
        {
            CString str;
            UTIL_FormatMessage(_Module.GetResourceInstance(),str.GetBuffer(_MAX_PATH),
                _MAX_PATH, E_CEEXE_NO_THREAD);
            str.ReleaseBuffer();
	        m_pCeExecute->FireMessage(str, hr);
            pIExeMsg->Release();
            return hr;
        }
        m_tThreadList.AddTail(pExeThread);
        rdwSessionId = dwNewSessionId;
        ++m_dwSessionCount;
    }
    pIExeMsg->Release();

    return hr;
}


//------------------------------------------------------------------*
/**
 * write command.
 *
 * @param            dwSessionId: session id
 * @param            crstrCommand: command string
 * @param            dwTimeout: timeout
 * @return           
 * @exception        -
 * @see              
*/
HRESULT CExeMaster::WriteCommand(DWORD dwSessionId, const CString& crstrCommand, DWORD dwTimeout /*=0*/)
{
    HRESULT hr;
    CExeThread* pExeThread = FindExeThread(dwSessionId);
    if(pExeThread)
    {
        hr = pExeThread->WriteCommand(crstrCommand, dwTimeout);
        return hr;
    }
    return E_FAIL;
}

//------------------------------------------------------------------*
/**
 * close session with certain id.
 *
 * @param            dwSessionId: session id
 * @param            bKill: kill running process?
 * @return           S_OK or result from Kill() resp. Stop()
 * @see              
*/
HRESULT CExeMaster::CloseSession(DWORD dwSessionId, BOOL bKill /*= TRUE*/)
{
    HRESULT hr;
    CExeThread* pExeThread = FindExeThread(dwSessionId);
    if(pExeThread)
    {
        if(bKill)
        {
            hr = pExeThread->Kill();
        }
        else
        {
            hr = pExeThread->Stop();
        }
        return hr;
    }
    return E_FAIL;
}


//------------------------------------------------------------------*
/**
 * close all sessions.
 *
 * @param            bKill: kill processes?
 * @return           S_OK or last result from Kill() resp. Stop()
 * @see              
*/
HRESULT CExeMaster::CloseAllSessions(BOOL bKill /*= TRUE*/)
{
    HRESULT hr;
    HRESULT hrReturn = S_OK;

    CExeThread* pExeThread;
    POSITION    pos = m_tThreadList.GetHeadPosition();
    while(pos)
    {
        pExeThread = m_tThreadList.GetNext(pos);
        if(bKill)
        {
            hr = pExeThread->Kill();
            if(hr != S_OK)
            {
                hrReturn = hr;
            }
        }
        else
        {
            hr = pExeThread->Stop();
            if(hr != S_OK)
            {
                hrReturn = hr;
            }
        }
    }
    return hrReturn;
}






//------------------------------------------------------------------*
/**
 * find exe thread.
 *
 * @param           dwSessionId: session id of thread
 * @return          pointer to exe thread object or NULL if not found
 * @see             
*/
CExeThread* CExeMaster::FindExeThread(DWORD dwSessionId)
{
    CExeThread* pExeThread = NULL;
    POSITION    pos = m_tThreadList.GetHeadPosition();
    while(pos)
    {
        pExeThread = m_tThreadList.GetNext(pos);
        ASSERT(pExeThread);
        if(pExeThread)
        {
            if(pExeThread->GetSessionId() == dwSessionId)
            {
                return pExeThread;
            }
        }
    }
    return NULL;
}

//------------------------------------------------------------------*
/**
 * get timeout flag.
 *  this is set if timeout occurred.
 *
 * @param           dwSessionId: session id of thread
 * @return          timeout flag
*/
BOOL CExeMaster::GetTimeoutFlag(DWORD dwSessionId)
{
    BOOL    bTimeout = FALSE;
    CExeThread* pExeThread = FindExeThread(dwSessionId);
    ASSERT(pExeThread);
    if(pExeThread)
    {
        bTimeout = pExeThread->GetTimeoutFlag();
    }
    return bTimeout;
}

//------------------------------------------------------------------*
/**
 * get return code of thread.
 *
 * @param           dwSessionId: session id of thread
 * @return          return code of thread
*/
DWORD CExeMaster::GetReturnCode(DWORD dwSessionId)
{
    DWORD   dwReturnCode = 0xFFFFFFFF;
    CExeThread* pExeThread = FindExeThread(dwSessionId);
    ASSERT(pExeThread);
    if(pExeThread)
    {
        dwReturnCode = pExeThread->GetReturnCode();
    }
    return dwReturnCode;
}