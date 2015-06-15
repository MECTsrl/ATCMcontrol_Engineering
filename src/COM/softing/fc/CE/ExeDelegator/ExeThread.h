#ifndef __EXETHREAD_H_
#define __EXETHREAD_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "ThrdTmpl.h"
#include "CEExecute.h"
#include "ExeMsgThread.h"

//----  Forward Class Definitions:   -------------------------------*
class CExeMaster;
class CExeThreadData;

//----  Defines:    ------------------------------------------------*
typedef CArray<CExeThreadData, CExeThreadData&> T_CommandArray;
//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


//------------------------------------------------------------------*
/**
 *  class CExeThreadData
 *
 *  - data class used by CExeThread for a single command
 *
 *  @see    CExeThread
 */
class CExeThreadData
{
public:
    CExeThreadData() 
    {
        ::ZeroMemory(&m_tProcInfo, sizeof(PROCESS_INFORMATION));
        m_dwTimeout = 0;
    }
    void Delete() { delete this; }

public:
    CString                 m_strCommand;
    DWORD                   m_dwTimeout;
    PROCESS_INFORMATION     m_tProcInfo;

};



//------------------------------------------------------------------*
/**
 *  class CExeThread
 *
 *  - thread object for the execution of subsequent commands
 *  - derived from receiver thread CRecThread<>
 *  - start / stop thread process
 *  - redirect messages to message view using CExeMsgThread
 *  - check timeout
 *
 *  @see    CRecThread
 */
class CExeThread : public CRecThread<CExeThreadData>
{
public:
    CExeThread(CExeMaster* pExeMaster = NULL, DWORD dwSessionId = 0xFFFFFFFF, BOOL bAutoClose = FALSE, BOOL bShowWindow = TRUE, BOOL bSynchronous = TRUE);
    virtual ~CExeThread();

    HRESULT WriteCommand(const CString& crstrCommand, DWORD dwTimeout = 0);
    HRESULT Stop();
    HRESULT Kill();

    DWORD   GetSessionId();

    BOOL    GetTimeoutFlag();
    DWORD   GetReturnCode();



protected:
    virtual int processInput(int iCommand, CExeThreadData* pData);

    virtual int OnInitialize();     // register stdout, stderr

    virtual int OnExit();           // unregister stdout, stderr

    void FireSystemMessage(const CString& crstrMessage);

    BOOL StartMessageThread();

    BOOL StopMessageThread();

    BOOL StopThread(HANDLE& rhProcess);

    void OutputErrorMessage(HRESULT hr);

private:
    void SetSessionId(DWORD dwSessionId);

public:
    IStream*        m_pIStream;

protected:
    CExeMaster*     m_pExeMaster;

    DWORD           m_dwSessionId;      // session id
    DWORD           m_dwWriteTimeout;   // timeout for writing a command

    HANDLE          m_hStdoutRead;
    HANDLE          m_hStdoutWrite;
    HANDLE          m_hStdinRead;
    HANDLE          m_hStdinWrite;

    IExeMsgInternal*    m_pIExeMsg;

    CExeMsgThread   m_tMessageThread;

    BOOL            m_bKill;            // terminate running process when thread stops?

    HANDLE          m_hExitEvent;       // is set in Stop() or Kill() to leave processInput()

    int             m_iCommandCount;    // counter for commands

    CExeMsgThreadData   m_tData;

    DWORD           m_dwReturnCode;     // return code of process
    BOOL            m_bTimeout;         // timeout occurred?
    BOOL            m_bAutoClose;       // close session if emtpy command is ecountered?
    BOOL            m_bShowWindow;      // show window of started process?
    BOOL            m_bSynchronous;     // synchronous mode?


};
#endif // __EXETHREAD_H_

