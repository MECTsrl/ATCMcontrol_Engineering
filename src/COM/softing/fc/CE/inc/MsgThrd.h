#ifndef MSGTHRD_H
#define MSGTHRD_H

// Msgthrd.h : interface of the controlling function of the CompilerNotify worker thread
//              and of the CCompNotifyThread class


struct CCompNotifyThreadInfo 
{
	HANDLE m_hEventStartCompNotify;
	HANDLE m_hEventKillCompNotify;
	HANDLE m_hEventCompNotifyKilled;
	HANDLE m_hEventCompNotifyDone;	// indicates that there are no more notifications
	HANDLE m_hChildStdoutRd;		// Pipe-Handle for reading the child output
    HANDLE m_hChildStdinWr;         // Pipe-Handle for writing
	HWND m_hwndNotifyClient;		// window handle of the client which receive the notifications
	CWinThread **m_ppThread;		// address of pointer to the thread
    UINT m_uiIndex;                 // index of message window
	CRITICAL_SECTION m_Lock;		// lock for sendmessage
};

// Controlling function for the worker thread.
UINT CompNotifyThreadProc(LPVOID pParam /* CCompNotifyThreadInfo ptr */);

class CMsgThrd 
{
private:
	CWinThread* m_pCompNotifyWorkerThread;
	CCompNotifyThreadInfo m_ThreadInfo;
	HANDLE m_hEventStartCompNotify;
	HANDLE m_hEventKillCompNotify;
	HANDLE m_hEventCompNotifyKilled;
	HANDLE m_hEventCompNotifyDone;	// indicates that there are no more notifications
public:
	CMsgThrd();
	~CMsgThrd();
	void StartCompNotifyThread(void);
    void SendKillCompNotifyThread(void);
	void KillCompNotifyThread(void);
	void SetPipeHandle(HANDLE hRead,HANDLE hWrite);
	void SetNotificationWnd(HWND hWnd);
	HWND GetNotificationWnd(void);
	void SetIndex(UINT uiIndex);
    BOOL IsThreadRunning(void);
	void Lock();
	void UnLock();
	const DWORD GetThreadId() const;
};

inline void CMsgThrd::Lock()
{
	EnterCriticalSection(&m_ThreadInfo.m_Lock);
}

inline void CMsgThrd::UnLock()
{
	LeaveCriticalSection(&m_ThreadInfo.m_Lock);
}

inline const DWORD CMsgThrd::GetThreadId() const
{
	if(m_pCompNotifyWorkerThread)
		return m_pCompNotifyWorkerThread->m_nThreadID;
	else
		return 0UL;
}

#endif	// MSGTHRD_H
