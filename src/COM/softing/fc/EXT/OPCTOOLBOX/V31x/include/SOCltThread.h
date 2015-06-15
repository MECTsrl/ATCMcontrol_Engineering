//-----------------------------------------------------------------------------
//                                                                            |



//                                                                            |



//                                                                            |
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                           OPC TOOLBOX - SOClt                              |
//                                                                            |
//  Filename    : SOCltThread.h                                               |
//  Version     : 3.10.release                                                |
//  Date        : 4-March-2002                                                |
//                                                                            |
//  Description : Worker thread classes                                       |
//                - SOCltWorkerThread: worker thread class of server object   |
//                - SOCltInterfaceAccess: COM interface access class          |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _SOCLTWORKERTHREAD_H_
#define _SOCLTWORKERTHREAD_H_

#include "SOCltTask.h"

#if _MSC_VER == 1100

extern const CLSID CLSID_StdGlobalInterfaceTable;

#ifndef __IGlobalInterfaceTable_FWD_DEFINED__
#define __IGlobalInterfaceTable_FWD_DEFINED__
typedef interface IGlobalInterfaceTable IGlobalInterfaceTable;
#endif 	/* __IGlobalInterfaceTable_FWD_DEFINED__ */

#ifndef __IGlobalInterfaceTable_INTERFACE_DEFINED__
#define __IGlobalInterfaceTable_INTERFACE_DEFINED__

typedef /* [unique] */ IGlobalInterfaceTable __RPC_FAR *LPGLOBALINTERFACETABLE;

EXTERN_C const IID IID_IGlobalInterfaceTable;

    MIDL_INTERFACE("00000146-0000-0000-C000-000000000046")
    IGlobalInterfaceTable : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE RegisterInterfaceInGlobal( 
            /* [in] */ IUnknown __RPC_FAR *pUnk,
            /* [in] */ REFIID riid,
            /* [out] */ DWORD __RPC_FAR *pdwCookie) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RevokeInterfaceFromGlobal( 
            /* [in] */ DWORD dwCookie) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetInterfaceFromGlobal( 
            /* [in] */ DWORD dwCookie,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppv) = 0;
        
    };

#endif
#endif
    

#pragma pack(push,4)

//-----------------------------------------------------------------------------
// SOCltInterfaceAccess                                                       |
//-----------------------------------------------------------------------------

#ifndef _ATL_NO_UUIDOF
#define _ATL_IIDOF(x) __uuidof(x)
#else
#define _ATL_IIDOF(x) IID_##x
#endif

template <class T, const IID* piid>
class SOCltInterfaceAccess
{
public:
	SOCltInterfaceAccess()
	{
		m_cookie = 0;
		m_creatorThread = 0;
	}

	~SOCltInterfaceAccess()
	{
		_ASSERTION(!m_ptr, "Interface not released");
		_ASSERTION(!m_cookie, "Cookie left");
	}

	HRESULT putInterface(T* p)
	{
		HRESULT res = S_OK;
    CComPtr<IGlobalInterfaceTable> pGIT;

    if ( 0 != m_cookie || NULL != p )
    {
      res = ::CoCreateInstance(CLSID_StdGlobalInterfaceTable,
                          NULL,
                          CLSCTX_INPROC_SERVER,
                          IID_IGlobalInterfaceTable,
                          (void **)&pGIT);
      _ASSERTION( SUCCEEDED(res) , "Failed to create Global Interface Table");
    }
   
		if ( 0 != m_cookie )
		{
      _ASSERTION( ::GetCurrentThreadId() == m_creatorThread, "Wrong thread" );

			res = pGIT->RevokeInterfaceFromGlobal(m_cookie);		
			_ASSERTION( SUCCEEDED(res), "IGlobalInterfaceTable::RevokeInterfaceFromGlobal failed");

			m_creatorThread = 0;
			m_cookie = 0;
			m_ptr = NULL;
		}

		res = S_OK;
    _ASSERTION(!m_ptr, "Interface not released");

		if ( p )
		{
			res = pGIT->RegisterInterfaceInGlobal(p, *piid, &m_cookie);
			_ASSERTION( SUCCEEDED(res), "IGlobalInterfaceTable::RegisterInterfaceInGlobal failed");

			if ( SUCCEEDED(res) ) 
			{
				m_ptr = p;
				m_creatorThread = ::GetCurrentThreadId();
			}
		}

    return res;
	}

	HRESULT getInterface(T** ppv)
	{
		_ASSERTION(ppv, "NULL pointer passed");

		HRESULT res = S_OK;    
		*ppv = NULL;

    if (!m_ptr) return E_NOINTERFACE;
		
		if ( ::GetCurrentThreadId() == m_creatorThread )
		{
			(*ppv = m_ptr)->AddRef();
      res = S_OK;
		}
		else
		{
      CComPtr<IGlobalInterfaceTable> pGIT;
      res = ::CoCreateInstance(CLSID_StdGlobalInterfaceTable,
                          NULL,
                          CLSCTX_INPROC_SERVER,
                          IID_IGlobalInterfaceTable,
                          (void **)&pGIT);
      _ASSERTION( SUCCEEDED(res) , "Failed to create Global Interface Table");

			res = pGIT->GetInterfaceFromGlobal(m_cookie, *piid, (void**)ppv);
			_ASSERTION( SUCCEEDED(res), "IGlobalInterfaceTable::GetInterfaceFromGlobal failed");
		}

    return res;
	}

	void operator=(T* p)
	{
		putInterface(p);
	}

	BOOL operator !(void)
	{
		return !m_ptr;
	}

protected:
	CComPtr<T> m_ptr;
	DWORD m_cookie;
	DWORD m_creatorThread;
}; // SOCltInterfaceAccess


//-----------------------------------------------------------------------------
// SOCltWorkerThread                                                          |
//-----------------------------------------------------------------------------

class SOCLT_EXPORT SOCltWorkerThread
{
  // thread entry point
	friend DWORD WINAPI _workThread(void* context);

public:
	SOCltWorkerThread();
	virtual ~SOCltWorkerThread();

  void shutdown(IN DWORD timeout = INFINITE);

  // Shedule a task for execution in worker thread's contexts
	BOOL addTask(IN SOCltTask* task, IN BOOL wakeup = TRUE);

  // Shedule a recurrent task
  BOOL addCyclicTask(IN SOCltCyclicTask* task,OUT DWORD* cookie = NULL);

  // Remove a recurrent task
  BOOL removeCyclicTask(IN DWORD cookie);

  // Get a recurrent task
  SOCltCyclicTask* getCyclicTask(IN DWORD cookie);

  // Signal worker thread
	void wakeUp();

  // Get thread ID
	DWORD getThreadID();

  // Get thread HANDLE
	HANDLE getThreadHandle();

  // set worker thread's priority
  BOOL setThreadPriority(IN int priority);

  void setErrorCallback(IN SOCltElementIError* error);

protected:
  void cleanUp(void);


	SOCmnList<SOCltTask>       m_taskQueue;	  // task queue
	SOCmnList<SOCltCyclicTask> m_cyclicQueue;	// cyclic task queue
	HANDLE m_events[3];				                // worker thread controlling events
	HANDLE m_workThread;			                // handle of workerthread
	DWORD m_workThreadId;			                // thread id of worker thread

	// worker thread main routine
	DWORD workThread(void);  

  SOCltElementIError* m_error;              
};

inline DWORD SOCltWorkerThread::getThreadID()
{	return m_workThreadId; }

inline HANDLE SOCltWorkerThread::getThreadHandle()
{	return m_workThread; }

inline void SOCltWorkerThread::setErrorCallback(IN SOCltElementIError* error)
{ m_error = error; }


#pragma pack(pop)

#endif // _SOCLTWORKERTHREAD_H_

