
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "ExeThread.h"
#include "ExeMaster.h"
#include "resource.h"
#include "utilif.h"
#include "ExeDelegatorErr.h"

//----  Static Initializations:   ----------------------------------*


//------------------------------------------------------------------*
/**
 * constructor.
 *
 * @param           pExeMaster: pointer to CExeMaster object
 * @param           dwSessionId: session id for identification purposes
 * @param           bAutoClose: close session automatically if empty 
 *                              command is encountered?
 * @param           bShowWindow: show process window?
 * @param           bSynchronous: synchronous call?
 * @return          -
 * @exception       -
 * @see             -
*/
CExeThread::CExeThread
(
    CExeMaster* pExeMaster /*=NULL*/,
    DWORD dwSessionId /*=0xFFFFFFFF*/,
    BOOL bAutoClose /*=FALSE*/,
    BOOL bShowWindow /*=TRUE*/,
    BOOL bSynchronous /*=TRUE*/
)
{
    ASSERT(pExeMaster);
    ASSERT(dwSessionId != 0xFFFFFFFF);
    m_pExeMaster = pExeMaster;
    m_dwSessionId = dwSessionId;
    m_dwWriteTimeout = 22000;
    m_bKill = FALSE;
    m_iCommandCount = 0;
    m_hExitEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    m_dwReturnCode = 0xFFFFFFFF;
    m_bTimeout = FALSE;
    m_bAutoClose = bAutoClose;
    m_bShowWindow = bShowWindow;
    m_bSynchronous = bSynchronous;
}

//------------------------------------------------------------------*
/**
 * destructor.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
CExeThread::~CExeThread()
{
}


//------------------------------------------------------------------*
/**
 * get timout flag.
 *
 * @param           -
 * @return          timout occurred?
 * @exception       -
 * @see             -
*/
BOOL CExeThread::GetTimeoutFlag()
{
    return m_bTimeout;
}

//------------------------------------------------------------------*
/**
 * get return code.
 *
 * @param           -
 * @return          return code of process.
 * @exception       -
 * @see             -
*/
DWORD CExeThread::GetReturnCode()
{
    return m_dwReturnCode;
}


//------------------------------------------------------------------*
/**
 * write command.
 *
 * @param            crstrCommand: command string
 * @param            dwTimeout: timeout in ms
 * @return           -
 * @exception        -
 * @see              -
*/
HRESULT CExeThread::WriteCommand(const CString& crstrCommand, DWORD dwTimeout /*= 0*/)
{
    if(dwTimeout == 0)
    {
        dwTimeout = INFINITE;
    }
    CExeThreadData*  pData = new CExeThreadData;
    assert(pData);
    pData->m_strCommand = crstrCommand;
    pData->m_dwTimeout = dwTimeout;
    ++m_iCommandCount;
    if(write(m_iCommandCount, pData, m_dwWriteTimeout) == thread_error)
    {
        return E_FAIL;
    }
    return S_OK;
}


//------------------------------------------------------------------*
/**
 * kill process.
 *
 *  set exit event for process.
 *
 * @param           -
 * @return          S_OK: successful
 * @exception       -
 * @see             -
*/
HRESULT CExeThread::Kill()
{
    HRESULT hr = S_OK;
    m_bKill = TRUE;
    ::SetEvent(m_hExitEvent);
//    hr = destroy();
    return hr;
}

//------------------------------------------------------------------*
/**
 * stop process.
 *
 *  set exit event for process.
 *
 * @param           -
 * @return          S_OK: successful
 * @exception       -
 * @see             -
*/
HRESULT CExeThread::Stop()
{
    HRESULT hr = S_OK;
    m_bKill = FALSE;
    ::SetEvent(m_hExitEvent);
//    hr = destroy();
    return hr;
}


//------------------------------------------------------------------*
/**
 * on initialize.
 *
 *  this function is called once at the beginning from within the thread.
 *
 * @param           -
 * @return          thread_noerror: OK
 *                  thread_error: error occurred
 * @exception       -
 * @see             -
*/
int CExeThread::OnInitialize()
{
    HRESULT hr;

    hr = ::CoInitializeEx(NULL,COINIT_MULTITHREADED);
    if(hr != S_OK)
    {
        return thread_error;
    }

    hr = ::CoGetInterfaceAndReleaseStream(m_pIStream, IID_IExeMsgInternal, (void**)&m_pIExeMsg);
    if(hr != S_OK)
    {
        return thread_error;
    }

    ASSERT(m_pIExeMsg);

    return thread_noerror;
}


//------------------------------------------------------------------*
/**
 * on exit.
 *
 *  this function is called when the thread is to be terminated from 
 *  within the thread.
 *
 * @param           -
 * @return          thread_noerror: OK
 * @exception       -
 * @see             -
*/
int CExeThread::OnExit()
{
    BOOL bMessages = m_bSynchronous;    // TODO: separate features
    if(!bMessages)
    {
        StopMessageThread();
    }
    m_pIExeMsg->OnReady();
    m_pIExeMsg->Release();
    ::CoUninitialize();
    return thread_noerror;
}


//------------------------------------------------------------------*
/**
 * fire system message.
 *
 *  uses IExeMsgInternal.
 *
 * @param           crstrMessage: message to be displayed in message view.
 * @return          -
 * @exception       -
 * @see             -
*/
void CExeThread::FireSystemMessage(const CString& crstrMessage)
{
    if(m_pIExeMsg)
    {
        CComBSTR    sMessage(crstrMessage);
        m_pIExeMsg->NewExeMessage(sMessage);
    }
}


//------------------------------------------------------------------*
/**
 * new process input.
 *
 *  single command from queue to be executed
 *  a process is started and its output redirected to the message view
 *
 * @param           iCommand: command number
 * @param           pData: command data
 * @return          thread_noerror: ok
 *                  thread_error: error occurred
 * @see             
*/
int CExeThread::processInput(int iCommand, CExeThreadData* pData)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    // now create command thread
    PROCESS_INFORMATION tProcInfo;
    STARTUPINFO         siStartInfo;
    BOOL                bResult;
    SECURITY_ATTRIBUTES saInherit = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};

    SECURITY_ATTRIBUTES saAttr;
    BOOL                bMessages = m_bSynchronous;     // TODO: separate these features

    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = bMessages;
    saAttr.lpSecurityDescriptor = NULL;

    if(bMessages)
    {
        if(!CreatePipe(&m_hStdoutRead, &m_hStdoutWrite, &saAttr, 0))
        {
            OutputErrorMessage(E_CEEXE_NO_PIPE);
        }

        if(!CreatePipe(&m_hStdinRead, &m_hStdinWrite, &saAttr, 0))
        {
            OutputErrorMessage(E_CEEXE_NO_PIPE);
        }
    }

    ::ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
    siStartInfo.cb = sizeof(STARTUPINFO);

    if(bMessages)
    {
        siStartInfo.hStdInput = m_hStdinRead;
        siStartInfo.hStdOutput = m_hStdoutWrite;
        siStartInfo.hStdError = m_hStdoutWrite;
        siStartInfo.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    }
    else
    {
        siStartInfo.dwFlags = STARTF_USESHOWWINDOW;
    }

    if(m_bShowWindow)
    {
        siStartInfo.wShowWindow = SW_SHOW;
    }
    else
    {
        siStartInfo.wShowWindow = SW_HIDE;
    }


    CString strCommand = pData->m_strCommand;
    if(pData->m_strCommand.IsEmpty())
    {
//            if(!m_bSynchronous)
//            {
//                // fire command ready
//            }
        if(m_bAutoClose)
        {
            return thread_break;
        }
        else
        {
            return thread_noerror;
        }
    }

    // SIS, 27.04.04:
    // inherit handles: file handles are also inherited
    // this is a problem for processes that will live longer than
    // the engineering. In this case the lock file handle is not
    // released when the engineering terminates. The project can
    // not be opened until the process terminates also.
    // For this case I use the flag m_bSynchronous.
    // If the process is asynchronous, message routing is disabled
    // and handles need not to be inherited.
    bResult = ::CreateProcess
                    (NULL,          // application name       
                    strCommand.GetBuffer(strCommand.GetLength() + 10),   // command line
                    NULL,           // process security attributes
                    NULL,           // primary thread security attributes
                    bMessages,      // if messages are enabled, call handles are inherited
                    0,              // creation flags
                    NULL,           // use parent's environment
                    NULL,           // use parent's current directory
                    &siStartInfo,   // STARTUPINFO pointer
                    &tProcInfo);   // receives PROCESS_INFORMATION

    // create process may have modified buffer:
    strCommand.ReleaseBuffer();

    if(bMessages)
    {
        ::CloseHandle(m_hStdoutWrite);
        ::CloseHandle(m_hStdinRead);
    }

    if(!bResult)
    {
        DWORD   dwLastError = ::GetLastError();
        CString strMessage;
        CString strTmp;
        UTIL_FormatMessage(_Module.GetResourceInstance(),strMessage.GetBuffer(_MAX_PATH),
            _MAX_PATH, dwLastError);
        strMessage.ReleaseBuffer();
        strTmp.Format(": %s", pData->m_strCommand);
	    FireSystemMessage(strMessage);
        UTIL_FormatMessage(_Module.GetResourceInstance(),strMessage.GetBuffer(_MAX_PATH),
            _MAX_PATH, E_CEEXE_NO_THREAD, strTmp);
        strMessage.ReleaseBuffer();
	    FireSystemMessage(strMessage);
        ::ResetEvent(m_hExitEvent);
        return thread_break;
    }
    else
    {
        CString strMessage;
        strMessage.Format("Starting process: %s", pData->m_strCommand);
        FireSystemMessage(strMessage);
        if(bMessages)
        {
            StartMessageThread();
        }

        if(!m_bSynchronous)
        {
            if(bResult)
            {
                m_dwReturnCode = S_OK;
            }
            else
            {
                m_dwReturnCode = E_FAIL;
            }
            m_pIExeMsg->OnReady();
            return thread_break;    // do not wait for termination     
                                    // TODO asynchronous but messages
        }

        pData->m_tProcInfo = tProcInfo;
        HANDLE  atHandles[2];
        atHandles[0] = m_hExitEvent;
        atHandles[1] = tProcInfo.hProcess;


        DWORD   dwResult;
        dwResult = ::WaitForMultipleObjects(2, atHandles, FALSE, pData->m_dwTimeout);
        ::ResetEvent(m_hExitEvent);

        // if timeout occurred: stop thread and exit
        if(dwResult == WAIT_TIMEOUT)
        {
            m_bTimeout = TRUE;
            StopThread(tProcInfo.hProcess);
            return thread_break;
        }
    
        // thread is to be exited:
        if(dwResult == WAIT_OBJECT_0)
        {
            StopThread(tProcInfo.hProcess);
            return thread_break;
        }
        else
        {
            ::GetExitCodeProcess(tProcInfo.hProcess, &m_dwReturnCode);
            if(m_dwReturnCode != 0)
            {
                return thread_break;
            }
        }
    }
    return thread_noerror;
}


//------------------------------------------------------------------*
/**
 * stop thread.
 *
 * @param           rhProcess: process handle
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CExeThread::StopThread(HANDLE& rhProcess)
{
    if(m_bKill)
    {
        DWORD   dwReturnCode = 0xFFFFFFFF;
        ::TerminateProcess(rhProcess, dwReturnCode);
    }
    else
    {
        ::CloseHandle(rhProcess);
    }
    return FALSE;
}


//------------------------------------------------------------------*
/**
 * get session id.
 *
 * @param           -
 * @return          session id.
 * @exception       -
 * @see             SetSessionId()
*/
DWORD CExeThread::GetSessionId()
{
    return m_dwSessionId;
}

//------------------------------------------------------------------*
/**
 * set session id.
 *
 * @param           dwSessionId: session id.
 * @return          -
 * @exception       -
 * @see             GetSessionId()
*/
void CExeThread::SetSessionId(DWORD dwSessionId)
{
    m_dwSessionId = dwSessionId;
}


//------------------------------------------------------------------*
/**
 * start message thread.
 *
 *  initializes pipes.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
BOOL CExeThread::StartMessageThread()
{
    m_tData.m_hStdout = m_hStdoutRead;
    m_tData.m_hStdin = m_hStdinWrite;
    m_tData.m_pIExeMsg = m_pIExeMsg;
   
    m_pIExeMsg->AddRef();   // must be released within message thread

    if(m_tMessageThread.Create(32, &m_tData) == thread_error)
    {
        OutputErrorMessage(E_CEEXE_NO_MSG_THREAD);
        return FALSE;
    }
    return TRUE;
}

BOOL CExeThread::StopMessageThread()
{
    BOOL    bReturn = TRUE;
    if(m_tMessageThread.Destroy() == thread_error)
    {
        bReturn  = FALSE;
    }
    return bReturn;
}



//------------------------------------------------------------------*
/**
 * output error message.
 *
 * @param           hr: HRESULT for error
 * @return          -
 * @exception       -
 * @see             
*/
void CExeThread::OutputErrorMessage(HRESULT hr)
{
    if(m_pIExeMsg)
    {
        CString strMessage;
        UTIL_FormatMessage(_Module.GetResourceInstance(),strMessage.GetBuffer(_MAX_PATH),
            _MAX_PATH, hr);
        strMessage.ReleaseBuffer();
	    FireSystemMessage(strMessage);
    }
}