


//Msgthrd.cpp : implementation of the worker thread for CompilerNotify
//

#include "stdafx.h"
#include "Msgthrd.h"
#include "CESysDef.h"
#include "fc_todebug\fc_assert.h"

#define BUFSIZE 4096


UINT CompNotifyThreadProc(LPVOID pParam) 
{
	CString str;
	CString strItem;
	int iPos;
	char chBuf[BUFSIZE+1];
	DWORD dwRead;
	
	CCompNotifyThreadInfo* pThreadInfo = (CCompNotifyThreadInfo*)pParam;

	// Wait until the main application thread asks this thread to start the CompNitification
	if(WaitForSingleObject(pThreadInfo->m_hEventStartCompNotify, INFINITE) != WAIT_OBJECT_0) {
		return(0);
	}
	// Reset event to indicate "not done", that is, CompNotify is in progress.
	ResetEvent(pThreadInfo->m_hEventCompNotifyDone);
	FC_DEBUGPRINT1(_T("CEMSG> Start Msg thread id = 0x%X\n"),GetCurrentThreadId());
	
	while (TRUE) 
    {

		// Exit the thread if the main application sets the "kill CompNotify"
		// event. The main application will set the "start CompNotify" event
		// before setting the "kill CompNotify" event.

		if(ReadFile(pThreadInfo->m_hChildStdoutRd, chBuf, BUFSIZE, &dwRead, NULL) && dwRead != 0) 
        {
		    if(WaitForSingleObject(pThreadInfo->m_hEventKillCompNotify, 0) == WAIT_OBJECT_0) 
            {
			    break; // Terminate this thread by exitting the proc.
		    }

            chBuf[dwRead] = 0;
			str += chBuf;
			while((iPos = str.Find(_T("\n"))) != -1) 
            {
				strItem = str.Left(iPos);
                strItem.TrimRight();

				EnterCriticalSection(&pThreadInfo->m_Lock);
				
		        if(WaitForSingleObject(pThreadInfo->m_hEventKillCompNotify, 0) == WAIT_OBJECT_0) 
                {
					LeaveCriticalSection(&pThreadInfo->m_Lock);
			        break; // Terminate this thread by exitting the proc.
		        }
				
				::SendMessage(pThreadInfo->m_hwndNotifyClient, MSGTHRD_NOTIFY, (WPARAM)pThreadInfo->m_uiIndex, (LPARAM)(LPCTSTR)strItem);
				LeaveCriticalSection(&pThreadInfo->m_Lock);

		        if(WaitForSingleObject(pThreadInfo->m_hEventKillCompNotify, 0) == WAIT_OBJECT_0) 
                {
			        break; // Terminate this thread by exitting the proc.
		        }

				strItem = str.Mid(iPos + 1);
				str = strItem;
			}
		} 
		else 
        {
			break;
		}
	} 
    CloseHandle(pThreadInfo->m_hChildStdoutRd);
    pThreadInfo->m_hChildStdoutRd = NULL;

    CloseHandle(pThreadInfo->m_hChildStdinWr);
    pThreadInfo->m_hChildStdinWr = NULL;

	
	/*since the normal AfxEndThread mechanism deletes the associated
	  CWinThread object we must notify the CMsgThrd object that the embedded
	  CWinThread object pointer points to an invalid e.g. deleted object
	  so we set this pointer to NULL on exiting the the procedure*/

    ::PostMessage(pThreadInfo->m_hwndNotifyClient, MSGTHRD_READY, (WPARAM)0, (LPARAM)0);


	* pThreadInfo->m_ppThread = NULL;
	SetEvent(pThreadInfo->m_hEventCompNotifyKilled);
	return(0);
}


CMsgThrd::CMsgThrd() 
{
	m_hEventStartCompNotify		= CreateEvent(NULL, FALSE, FALSE, NULL); // auto reset, initially reset
	m_hEventKillCompNotify		= CreateEvent(NULL, FALSE, FALSE, NULL); // auto reset, initially reset;
	m_hEventCompNotifyKilled	= CreateEvent(NULL, FALSE, FALSE, NULL); // auto reset, initially reset
	m_hEventCompNotifyDone		= CreateEvent(NULL, FALSE, FALSE, NULL); // manual reset, initially set
//	m_hEventCompNotifyDone		= CreateEvent(NULL, TRUE, TRUE, NULL); // manual reset, initially set

	m_ThreadInfo.m_hEventStartCompNotify	= m_hEventStartCompNotify;
	m_ThreadInfo.m_hEventKillCompNotify		= m_hEventKillCompNotify;
	m_ThreadInfo.m_hEventCompNotifyKilled	= m_hEventCompNotifyKilled;
	m_ThreadInfo.m_hEventCompNotifyDone		= m_hEventCompNotifyDone;

	m_ThreadInfo.m_hwndNotifyClient = NULL;
	m_ThreadInfo.m_hChildStdoutRd = NULL;
    m_ThreadInfo.m_hChildStdinWr = NULL;
	m_ThreadInfo.m_ppThread = & m_pCompNotifyWorkerThread;
    m_ThreadInfo.m_uiIndex = 0;

	m_pCompNotifyWorkerThread = NULL;
	InitializeCriticalSection(&m_ThreadInfo.m_Lock);
	return;
}

CMsgThrd::~CMsgThrd() 
{
	DWORD dwExitCode;
	if(m_pCompNotifyWorkerThread != NULL &&
		GetExitCodeThread(m_pCompNotifyWorkerThread->m_hThread, &dwExitCode) &&
		dwExitCode == STILL_ACTIVE)
	{
		// Kill the worker thread by setting the "kill thread" event.
		// See comment in OnKillWorkerThread for explanation of the sequence
		// of the "kill thread" and "start recalc" events.
		SetEvent(m_hEventKillCompNotify);
		FC_DEBUGPRINT(_T("CEMSG> Wait for thread exit\n"));
		WaitForSingleObject(m_hEventCompNotifyKilled, INFINITE);
	}

	//close all created handles of events
	CloseHandle(m_hEventStartCompNotify);
	CloseHandle(m_hEventKillCompNotify);
	CloseHandle(m_hEventCompNotifyKilled);
	CloseHandle(m_hEventCompNotifyDone);
	DeleteCriticalSection(&m_ThreadInfo.m_Lock);
	return;
}

void CMsgThrd::StartCompNotifyThread(void) 
{
    ASSERT(m_ThreadInfo.m_hChildStdoutRd != NULL);
    ASSERT(m_ThreadInfo.m_hwndNotifyClient != NULL);

	m_ThreadInfo.m_ppThread = & m_pCompNotifyWorkerThread; 

	if(m_pCompNotifyWorkerThread == NULL) {
		// Begin the worker thread.  It is ok to fill in the CThreadInfo
		// structure after the thread has started, because the thread
		// waits for the "start recalc" event before referring to the structure.
		m_pCompNotifyWorkerThread =
			AfxBeginThread(CompNotifyThreadProc, &m_ThreadInfo);
	}

	// The events are initially set or reset in the CreateEvent call;
	// but they may be left in an improperly initialized state if
	// a worker thread has been previously started and then prematurely
	// killed.  Set/reset the events to the proper initial state.
	// Set the "start recalc" event last, since it is the event the
	// triggers the starting of the worker thread recalculation.
	ResetEvent(m_hEventCompNotifyDone);
	ResetEvent(m_hEventKillCompNotify);
	ResetEvent(m_hEventCompNotifyKilled);
	SetEvent(m_hEventStartCompNotify);
	return;
}

void CMsgThrd::SendKillCompNotifyThread(void) 
{
	SetEvent(m_hEventKillCompNotify);
	return;
}


void CMsgThrd::KillCompNotifyThread(void) 
{
	SetEvent(m_hEventKillCompNotify);

	while (MsgWaitForMultipleObjects(1, &m_hEventCompNotifyKilled, FALSE, INFINITE, QS_SENDMESSAGE)
             == WAIT_OBJECT_0+1)
      {   MSG msg; // must process any sent messages here to avoid deadlock!
          while (PeekMessage(&msg, GetNotificationWnd(), 0, 0, PM_REMOVE)) {}
      }
	m_pCompNotifyWorkerThread = NULL;
	return;
}

void CMsgThrd::SetPipeHandle(HANDLE hRead, HANDLE hWrite) 
{
    ASSERT(hRead != NULL);
    ASSERT(hWrite != NULL);
	m_ThreadInfo.m_hChildStdoutRd = hRead;
    m_ThreadInfo.m_hChildStdinWr = hWrite;
	return;
}

void CMsgThrd::SetNotificationWnd(HWND hWnd) 
{
ASSERT(hWnd != NULL);
	m_ThreadInfo.m_hwndNotifyClient = hWnd;
	return;
}

HWND CMsgThrd::GetNotificationWnd(void) 
{
	return m_ThreadInfo.m_hwndNotifyClient;
}

void CMsgThrd::SetIndex(UINT uiIndex) 
{
	m_ThreadInfo.m_uiIndex = uiIndex;
	return;
}

BOOL CMsgThrd::IsThreadRunning(void)
{
    return m_pCompNotifyWorkerThread != NULL;
}