#ifndef __EXEMASTER_H_
#define __EXEMASTER_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*
class CExeThread;
class CCEExecute;

//----  Defines:    ------------------------------------------------*
typedef CList<CExeThread*, CExeThread*> T_ThreadList;

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


//------------------------------------------------------------------*
/**
 *  class CExeMaster.
 *
 *  - main class of exe delegator
 *  - manage thread objects (CExeThread)
 *    - create / destroy threads (CExeThread)
 *    - dispatch calls to threads
 *
 *  @see    CExeThread
*/
class CExeMaster
{
// public member functions
public:
    CExeMaster();
    virtual ~CExeMaster();

    void SetCeExecutePtr(CCEExecute* pCeExecute);

    HRESULT OpenSession(DWORD& rdwSessionId, BOOL bAutoClose = TRUE, BOOL bShowWindow = TRUE, BOOL bSynchronous = TRUE);
    HRESULT WriteCommand(DWORD dwSessionId, const CString& crstrCommand, DWORD dwTimeout = 0);
    HRESULT CloseSession(DWORD dwSessionId, BOOL bKill = TRUE);
    HRESULT CloseAllSessions(BOOL bKill = TRUE);

    BOOL    GetTimeoutFlag(DWORD dwSessionId);
    DWORD   GetReturnCode(DWORD dwSessionId);

// protected member functions
protected:
    CExeThread* FindExeThread(DWORD dwSessionId);

    HRESULT Kill(DWORD dwSessionId);

// member variables
protected:
    DWORD           m_dwSessionCount;
    T_ThreadList    m_tThreadList;

    CCEExecute*     m_pCeExecute;   // pointer to CCEExecute for sending messages

//    HANDLE*         m_pEvents;
//    int             m_iNumEvents;

};
#endif // __EXEMASTER_H_