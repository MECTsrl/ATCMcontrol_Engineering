/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/inc/thrdtmpl.h 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: thrdtmpl.h $
 *                       $Logfile: /4CReleased/V2.20.00/COM/softing/fc/inc/thrdtmpl.h $
 *
 * =PROJECT				CAK1020  ATCMControl V2.0
 *
 * =SWKE                INC
 *
 * =COMPONENT           INC
 *
 * =CURRENT      $Date: 28.02.07 19:01 $
 *           $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *

 



 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *== Thread template classes for general purpose
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  07.11.2000  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/

#ifndef __THRDTMPL_H_
#define __THRDTMPL_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "itc.h"
//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*
#ifndef THREADTMPL_ASSERT
#define THREADTMPL_ASSERT(x) assert((x))
#endif

#define RECTHREAD_KILLTO	INFINITE //42000L
#define	RECTHREAD_TO		(60 * 1000L)
#define RECTHREAD_NORMAL	1

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

////////////////////////////////////////////////////////////////////////////
//	Thread templates 
// 
/////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------*
/**
 *  class CReceiverThread - simple receiver thread with fifo queue.
 *  Realizes a thread with a fifo (first-in-first-out) queue. The 
 *  queue can be used as a command queue and to transfer data to
 *  the thread execution context. The entry size of the queue is
 *  fixed - hence only entries of the same type shall be stored 
 *  in the queue. One queue entry constist of a container object
 *  holding:<br>
 *  1.) a reference to a TData object (TData* - can be null)<br>
 *	2.)	a reference to a TParam object (TParam * - can be null)<br>
 *  3.) a command identifier<br>
 *	The thread reads the entries from the queue that have been 
 *  written into the queue by the method write.
 *  The thread can be created and is executing immediately by 
 *  calling the method create. The thread can also be created
 *  in suspended mode by createSuspended and can be resumed later
 *  by calling Resume.
 *  The TData and TParam objects will not be deleted directly.
 *  Rather a method TData/TParam::Delete will be called to allow 
 *  the user to decide whether to delete the object or not.<br>
 *  <b>Usage:</b><br>
 *  1)  derive a class from it and overwrite processInput() for processing
 *      In processInput you will do the real data processing<br>
 *  2)  Overwrite OnInitialize() method if you have init actions to do before
 *	    Thread function enters the working loop (e.g Initialization of COM).<br>
 *  3)  Overwrite OnExit() method if you have exit actions to do after thread
 *      has left the working loop (e.g. deinitialization of COM).<br>
 *  4)  Overwrite OnDebugOutput() to perform debug messaging or pass a message
 *      to the user or an interface.<br>
 *  5)  instantiate the subclass<br>
 *  6)  call create() or createSuspended() to create the thread.<br>
 *  7)  call Resume() if appropriate and if you called createSuspended() before.<br>
 *  8)  call destroy() to exit the thread or destroy the object
 *      if you destroy the object the destructor will handle the destruction
 *      of the thread.<br>
 *  
 *  Normally you should not overwrite create(),createSuspended() or destroy(). 
 *  For special purposes these methods can be overwritten but do not forget
 *  to call the method of the super class.
 *  cleanup() should normally not be called directly. Rather it will be called
 *  from the thread context if a destroy request is serviced. It cleans up
 *  all entries in the queue but does not service the requests associated with
 *  the entry (i.e. does not call processInput()). Hence cleanup() should only be
 *  invoked directly if you want to remove all entries from the queue without
 *  exiting the thread.
 *
 *  @see    class ITCBoundedRawFifo (itc.h).
*/
template<class TData,class TParam>class CReceiverThread 
{
    /**
    * CContainer holds the data entry in the fifo.
    */
    struct CContainer
    {  
	public:
        /** command id */
		int		m_nCommand;
        /** item data*/
        TData*  m_pItem;
        /** additional param*/
		TParam*	m_pParam;
    };

protected:
    /** the first-in first-out queue */
    ITCBoundedRawFifo	m_itcFifo;
    /** thread id */
    DWORD	            m_threadId;
    /** thread handle */
    HANDLE				m_threadHandle;
    /** the size of the datagram */
    size_t              m_size;
    /** thread suspended? */
	bool				m_bSuspended;
    /** Exit event */
    HANDLE              m_hExitEvnt;
public:
            CReceiverThread(void);
    virtual ~CReceiverThread(void);

    virtual HRESULT	create(int nObjects = 100);
	virtual HRESULT createSuspended(int nObjects = 100);
	virtual void Resume(void);
    virtual int		destroy();
    void    cleanup();    
    
    int     schedule();

	int	write(int nCommmand, TData* pData,
		TParam* pParam, DWORD timeout = RECTHREAD_TO);

    enum {thread_noerror = 0, thread_error = -1, thread_break = 1};
protected:
	virtual int processInput(int nCommand,TData *pData,TParam *pParam);
    virtual void OnDebugOutput(LPCTSTR pszFormat,...);
    virtual int OnInitialize(void);
    virtual int OnExit(void);
private:
	static DWORD WINAPI threadFunction	(void *p);
    int		write(void*	buffer,	int	nBytes,DWORD timeout);
};

/**
 * Default ctor of CReceiverThread.
 * Resets members.
 *        
*/
template<class TData,class TParam>
CReceiverThread<TData,TParam>::CReceiverThread(void)
{
	m_bSuspended = false;
	m_threadHandle = NULL;
	m_threadId = 0;
	m_size = 0;
    m_hExitEvnt = NULL;
}

/**
 * Dtor of CReceiverThread.
 * Invokes destroy().
 *        
*/
template<class TData,class TParam>
CReceiverThread<TData,TParam>::~CReceiverThread(void)
{
	destroy();
}

//------------------------------------------------------------------*
/**
 * Create establishes the queue with parameter nObjects entries and 
 * creates the thread.Call createSuspended() to create the thread
 * in suspended mode. Call destroy() to destroy the thread. Avoid
 * calling create() twice without calling destroy() in between.
 *
 * @param - nObjects - number of entries in the queue to set up.
 *          default value is 100.
 *        
 * @return  HRESULT of last error if thread creation failed.
 *          The queue will be cleaned up in this case.<br> 
 *          S_OK if overall creation succeeded.
 * @see     createSuspended(), destroy()
*/
template<class TData,class TParam>
HRESULT CReceiverThread<TData,TParam>::create(int nObjects)
{
    HRESULT hr = S_OK;
    
    if(m_threadHandle) { //already created
        return S_OK;
    }
    
    /*exit event will be created
      it is an auto reset event - if you set it with SetEvent(..) it will 
      automatically be reset by any WaitForSingle/MultipleObject() which is waiting for the event.
    */
    m_hExitEvnt = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    if(!m_hExitEvnt) { //event cannot be created something must be wrong
        THREADTMPL_ASSERT(m_hExitEvnt);
        hr = HRESULT_FROM_WIN32(GetLastError());
        return hr;
    }

    m_itcFifo.create(sizeof(CContainer), nObjects);
    m_size = sizeof(CContainer);
	m_bSuspended = false;

	m_threadHandle = CreateThread(0, 4096,threadFunction, this, 0, &m_threadId);

    if(!m_threadHandle) {
        hr = HRESULT_FROM_WIN32(GetLastError());
        m_itcFifo.destroy();
        ::CloseHandle(m_hExitEvnt);
        m_hExitEvnt = NULL;
    }

    return hr;
}

//------------------------------------------------------------------*
/**
 * CreateSuspended establishes the queue with parameter nObjects entries and 
 * creates the thread in suspended mode. Call create() to create the 
 * thread and invoke execution immediately.
 * Call destroy() to destroy the thread. Avoid calling createSuspended()
 * twice without calling destroy() in between. Call Resume() to initiate
 * execution of the thread.
 *
 * @param - nObjects - number of entries in the queue to set up.
 *          default value is 100.
 *        
 * @return  HRESULT of last error if thread creation failed.
 *          The queue will be cleaned up in this case.<br>
 *          S_OK if overall creation succeeded.
 * @see     create(), destroy(), Resume().
*/
template<class TData,class TParam>
HRESULT CReceiverThread<TData,TParam>::createSuspended(int nObjects)
{
    HRESULT hr = S_OK;

    if(m_threadHandle) { //already created
        return S_OK;
    }

    /*exit event will be created
      it is an auto reset event - if you set it with SetEvent(..) it will 
      automatically be reset by any WaitForSingle/MultipleObject() which is waiting for the event.
    */
    m_hExitEvnt = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    if(!m_hExitEvnt) { //event cannot be created something must be wrong
        THREADTMPL_ASSERT(m_hExitEvnt);
        hr = HRESULT_FROM_WIN32(GetLastError());
        return hr;
    }

	m_itcFifo.create(sizeof(CContainer), nObjects);
    m_size = sizeof(CContainer);
	m_bSuspended = true;

	m_threadHandle = CreateThread(0, 4096,threadFunction, this, CREATE_SUSPENDED, &m_threadId);

    if(!m_threadHandle) {
        hr = HRESULT_FROM_WIN32(GetLastError());
        m_itcFifo.destroy();
        ::CloseHandle(m_hExitEvnt);
        m_hExitEvnt = NULL;
    }

    return hr;
}

//------------------------------------------------------------------*
/**
 * Resume shall only be called if createSuspended() has been called
 * prior. The method resumes the thread that has been created in
 * suspended mode. Method returns at once without error indication
 * if the thread has not been created in suspended mode
 *        
 * @see     createSuspended()
*/
template<class TData,class TParam>
void CReceiverThread<TData,TParam>::Resume(void)
{
	if(m_bSuspended && m_threadHandle) {
		m_bSuspended = false;
		::ResumeThread(m_threadHandle);
	}
}

//------------------------------------------------------------------*
/**
 * Overwrite this method if you have init actions to do before
 * Thread function enters the working loop (e.g Initialization of COM).
 * Avoid calling any of the threads methods in the OnInitialize method override.
 * Do not call destroy() and create() methods in the OnInitialize method override.
 *
 * @return  enum value thread_noerror if no error occured in method.<br>
 * @return  enum value thread_error if an error during this method
 *          occured and it is desired to exit the thread at once.<br>
 *          The default non override of this method always returns
 *          thread_noerror.
 *
 * @see - Usage section of class CReceiverThread description.
*/
template<class TData,class TParam>
int CReceiverThread<TData,TParam>::OnInitialize(void)
{
    return thread_noerror;
}

//------------------------------------------------------------------*
/**
 * Overwrite Exit method if you have exit actions to do after thread
 * has left the working loop (e.g. deinitialization of COM). Avoid
 * calling any of the threads methods in the OnExit method override.
 * Do not call destroy() and create methods in the OnExit method override.
 *
 * @return  enum value thread_noerror if no error occured in method.<br>
 * @return  enum value thread_error if an error during this method
 *          occured and it is desired to exit the thread at once.<br>
 *          The default non override of this method always returns
 *          thread_noerror.
 *
 * @see - Usage section of class CReceiverThread  description.
*/
template<class TData,class TParam>
int CReceiverThread<TData,TParam>::OnExit(void)
{
    return thread_noerror;
}

//------------------------------------------------------------------*
/**
 * Sends an destroy request to the thread context and waits for the
 * thread to exit. If the thread does not exit after a time period
 * defined by RECTHREAD_KILLTO the thread will be terminated. But this
 * constitutes a <b>fatal error situation</b>. Hence the user code of the
 * derived thread subclass shall be checked for potential deadlock
 * situations. 
 * After the thread has been exited or terminated (fatal) the internal
 * members will be reset to the Ctor state of the class. The queue will
 * be destroyed as well. Normally you should not call this method if
 * create or createSuspended has not been called previously.
 *        
 * @see     create(),createSuspended()
*/
template<class TData,class TParam>
int CReceiverThread<TData,TParam>::destroy(void)
{
	if (m_threadHandle) {
        ::SetEvent(m_hExitEvnt);
		DWORD dwSucc = WaitForSingleObject(m_threadHandle,RECTHREAD_KILLTO);
		
        if(dwSucc != WAIT_OBJECT_0) {
            //fatal exit code
            //OPCC_DebugOut(_T("COpcReceiver: fatal thread termination handle = %d"),m_threadHandle);
            TerminateThread(m_threadHandle,0);
        }
        
        ::CloseHandle(m_threadHandle);
		m_threadHandle = NULL;
        ::CloseHandle(m_hExitEvnt);
        m_hExitEvnt = NULL;
		m_threadId = 0;
    	m_bSuspended = false;

        //do a cleanup again
        cleanup();
		m_itcFifo.destroy();
        m_size = 0;
		
	}
	return 0;
}

//------------------------------------------------------------------*
/**
 *  cleanup should normally not be called directly. Rather it will be called
 *  from the thread context if a destroy request is serviced. It cleans up
 *  all entries in the queue but does not service the requests associated with
 *  the entry (i.e. does not call processInput). Hence cleanup should only be
 *  invoked directly if you want to remove all entries from the queue without
 *  exiting the thread.
 *
 * @see     class CReceiverThread description.
*/
template<class TData,class TParam>
void CReceiverThread<TData,TParam>::cleanup(void)
{
    CContainer datagram;

    while(m_itcFifo.read(&datagram, sizeof(datagram),0) != WAIT_TIMEOUT) {
        if(datagram.m_pItem)
            datagram.m_pItem->Delete();

		if(datagram.m_pParam)
			datagram.m_pParam->Delete();
    }
}

//------------------------------------------------------------------*
/**
 * schedule constitutes the thread's execution body.
 *
 * <b>Do not call this function directly under any circumstances.</b>
 *
 * The method calls the OnInitialize() method first which might have been over
 * written by a user thread subclass to perform user specific init-
 * ializations. The schedule enters the endless working loop.
 * In the working loop schedule reads the entries from the queue that have been 
 * written into the queue by the method write. With an entry read schedule
 * calls processInput to transfer the data to the user code. The user
 * code resides in a processInput override. After calling processInput
 * schedule calls TParam::Delete and TData::Delete for the entry.
 * Both object references can be null. In that case the deletion operations
 * mentioned above will not be performed.
 * In case of an suicide command which indicates an destroy request, the
 * queue will be cleaned up by calling cleanup and schedule leaves
 * the working loop. Finally before exiting the thread schedule calls
 * OnExit() which might have been overwritten by the user thread subclass 
 * to perform user specific exit operations.
 *
 * @see - Usage section of class CReceiverThread  description.
*/
template<class TData,class TParam>
int CReceiverThread<TData,TParam>::schedule(void)
{
    static int nReceives;
    int nRet,iThreadError;
	DWORD dwResult;

	OnDebugOutput(_T("\nCReceiverThread::threadFunction started id=0x%x\n"),m_threadId);

    if(OnInitialize() == thread_error) {//error in OnInitialize;
        OnExit();
        OnDebugOutput(_T("\nCReceiverThread::threadFunction exited id=0x%x\n"),m_threadId);
        return 1; //exit code
    }

    for (;;) { //receive threads work function endless loop
        CContainer datagram;
        bool bExit = false;

		nRet = m_itcFifo.readEx(&datagram, sizeof(datagram),1,&m_hExitEvnt,
			&dwResult,INFINITE);

        if (!nRet) { //normal datagram received
			nReceives++;

            iThreadError = processInput(datagram.m_nCommand,datagram.m_pItem,datagram.m_pParam);
            
            if(datagram.m_pItem) {
				datagram.m_pItem->Delete();
            }

			if(datagram.m_pParam)
				datagram.m_pParam->Delete();


            if(iThreadError == thread_error || iThreadError == thread_break) {
                bExit = true;
            }
		}
		else { //event trigger for exit event
			if(dwResult == WAIT_OBJECT_0) {
                bExit = true;
			}
			else { //error case
				OnDebugOutput(_T("\nCRecThread::threadFunction error %d\n"),dwResult);
			}
		}

        if(bExit == true){
            cleanup();
			break;
		}
    }

    if(OnExit() == thread_error) {
        OnDebugOutput(_T("\nCReceiverThread::threadFunction exited id=0x%x\n"),m_threadId);
        return 1;
    }

	OnDebugOutput(_T("\nCReceiverThread::threadFunction exited id=0x%x\n"),m_threadId);
    return 0;
}

//------------------------------------------------------------------*
/**
 * private method to write an entry to the FIFO (first-in-first-out) 
 * queue of the thread class.
 *
 * @param   buffer - pointer to the entry.
 * @param   nBytes - number of bytes to write.
 * @param   timeout - timeout value to wait for queue's write access grant.
 * @return  WAIT_TIMEOUT in case of an time out with 
 *          ITCBoundedRawFifo::write()<br>
 * @return  0 - success
 *          
 * @see     class ITCBoundedRawFifo
*/
template<class TData,class TParam>
int	CReceiverThread<TData,TParam>::write(void*	buffer,	int	nBytes,DWORD timeout)
{
	return m_itcFifo.write(buffer, nBytes, timeout);
}

//------------------------------------------------------------------*
/**
 * Writes an entry resp. command request to the queue. The entry resp.
 * request will then be read by the thread context and processInput
 * will be called in the threads context to perform the user operation.
 * The user operation shall be performed in the processInput override.
 * parameter nCommand shall not be 0 because this value constitutes a
 * reserved value.
 *
 * @param       nCommand - command identifier - shall not be 0 because
 *              this value constitutes a reserved value.
 * @param       TData * - reference to a TData object.
 * @param       TParam * - reference to a TParam object.
 * @param       timeout - timeout value to wait for queue's write access grant.
 * @return      WAIT_TIMEOUT in case of an time out with 
 *              ITCBoundedRawFifo::write()<br>
 * @return      0 - success
 *          
 * @see         class ITCBoundedRawFifo
*/
template<class TData,class TParam>
int	CReceiverThread<TData,TParam>::write(int nCommand, TData * pData,
	TParam *pParam, DWORD timeout)
{
	CContainer datagram;

	Resume();

    datagram.m_nCommand = nCommand; //signature for normal write
	datagram.m_pItem = pData;
	datagram.m_pParam = pParam;

	return write(&datagram, sizeof(datagram),timeout);
}

//------------------------------------------------------------------*
/**
 * overwrite processInput for processing.
 * In processInput you the real data processing shall be performed.
 * processInput will be called by schedule in the thread context if
 * schedule has read an entry resp. request.
 * The override in the user's subclass shall perform the data processing
 * as desired. Do not call any of the threads methods from processInput
 * (except cleanup if this is desired).
 *
 * @param   nCommand - command identifier of the request being written to
 *          the queue.
 * @param   TData * - reference to a TData object. Do not delete TData
 *          object via this reference during processInput.
 * @param   TParam * - reference to a TParam object. Do not delete TParam
 *          object via this reference during processInput.
 * @return  always return enum value thread_noerror. 
 *
 * @see     cleanup(),schedule(), Usage section of class CReceiverThread  description.
*/
template<class TData,class TParam>
int CReceiverThread<TData,TParam>::processInput(int nCommand,TData * pItem,
	TParam *pParam)
{
    return thread_noerror;
}

//------------------------------------------------------------------*
/**
 * Overwrite OnDebugOutput to perform debug messaging or pass a message
 * to the user or an interface.
 * Avoid calling any of the threads methods in the OnDebugOutput override.
 * Do not call destroy and create methods in the OnDebugOutput override.
 *
 * @param       pszFormat - Format string (like printf)
 *              ... - any number of additional arguments referenced in 
 *              argument pszFormat. Number of arguments must comply with
 *              number of referenced arguments in the format string.
 *        
 * @see         Usage section of class CReceiverThread  description.
*/
template<class TData,class TParam>
void CReceiverThread<TData,TParam>::OnDebugOutput(LPCTSTR pszFormat,...)
{
}

//------------------------------------------------------------------*
/**
 * Constitutes the private static thread function of the CReceiverThread
 * class. Obtains reference to CReceiverThread class from the void
 * reference and calls schedule to invoke the thread body function.
 *
 * @param   p - reference to CReceiverThread thread object.
 *          shall not be null.
 *        
 * @return  0
 * @see     schedule.
*/
template<class TData,class TParam>
DWORD WINAPI CReceiverThread<TData,TParam>::threadFunction(void *p)
{
	CReceiverThread *pReceive = (CReceiverThread*) p;
	THREADTMPL_ASSERT(pReceive);

    return pReceive->schedule();
}

//------------------------------------------------------------------*
/**
 *  class CRecThread - simple receiver thread with fifo queue and
 *  list of events.
 *  Realizes a thread with a fifo (first-in-first-out) queue. The 
 *  queue can be used as a command queue and to transfer data to
 *  the thread execution context. The entry size of the queue is
 *  fixed - hence only entries of the same type shall be stored 
 *  in the queue. One queue entry constist of a container object
 *  holding:<br>
 *  1.) a reference to a TData object (TData* - can be null)<br>
 *  2.) a command identifier<br>
 *	The thread reads the entries from the queue that have been 
 *  written into the queue by the method write().
 *  The thread can be created and is executing immediately by 
 *  calling the method create().
 *  The TData object will not be deleted directly after the entry
 *  has been read and processInput() has been performed. Rather a 
 *  method TParam::Delete() will be called for that entry to allow 
 *  the user to decide whether to delete the object or not.
 *  Additionally a list of events can be specified to allow a 
 *  processing in an override of the method OnEvent() if one of the
 *  events has been signalled through SDK function ::SetEvent().<br>
 *
 *  <b>Usage:</b><br>
 *  1)  derive a class from CRecThread and overwrite processInput() for
 *      processing. In processInput() you will do the real data processing<br>
 *  2)  Overwrite OnInitialize() method if you have init actions to do before
 *	    Thread function enters the working loop (e.g Initialization of COM).<br>
 *  3)  Overwrite OnExit() method if you have exit actions to do after thread
 *      has left the working loop (e.g. deinitialization of COM).<br>
 *  4)  Overwrite OnDebugOutput() to perform debug messaging or pass a message
 *      to the user or an interface.<br>
 *  5)  Overwrite OnEvent() to perform an operation if one of the events
 *      that have been specified with create becomes signalled.<br>
 *  4)  instantiate the subclass<br>
 *  5)  call create() to create the thread.<br>
 *  6)  call destroy() to exit the thread or destroy the object
 *      if you destroy the object the destructor will handle the destruction
 *      of the thread.<br>
 *  
 *  Normally you should not overwrite create() or destroy(). 
 *  For special purposes these methods can be overwritten but <b>do not forget
 *  to call the method of the super class</b>.
 *  cleanup() should normally not be called directly. Rather it will be called
 *  from the thread context if a destroy request is serviced. It cleans up
 *  all entries in the queue but does not service the requests associated with
 *  the entry (i.e. does not call processInput()). Hence cleanup() should only be
 *  invoked directly if you want to remove all entries from the queue without
 *  exiting the thread.
 *
 *  @see    class ITCBoundedRawFifo (itc.h).
*/
template<class TData>class CRecThread 
{
    /**
    * CContainer holds the data entry in the fifo.
    */
    struct CContainer
    {  
	public:
        /** command id */
		int		m_nCommand; 
        /** item data */
        TData*  m_pItem; 
    };

protected: //can be used by customized destroy()
    /** the first-in-first-out queue */
    ITCBoundedRawFifo	m_itcFifo; 
    /** the size of the datagram */
    size_t              m_size;
    /** number of events */
	int					m_nUsrEvents; 
    /** reference to the event handle array */
	HANDLE*				m_pEvents; 
    /** thread id */
    DWORD	            m_threadId; 
    /** thread handle */
    HANDLE				m_threadHandle; 
    /** Exit event */
    HANDLE              m_hExitEvnt;

public:
            CRecThread(void);
    virtual ~CRecThread(void);

    virtual HRESULT	create(int nObjects,int nUsrEvnts,HANDLE *pEvents);
    virtual HRESULT	destroy();
    void    cleanup();    
    
    int     schedule();

	virtual int	write(int nCommand, TData *pData,DWORD timeout = RECTHREAD_TO);

    enum {thread_noerror = 0, thread_error = -1, thread_break = 1};
protected:
	virtual int processInput(int nCommand, TData *);
    virtual void OnDebugOutput(LPCTSTR pszFormat,...);
	virtual int OnInitialize(void); //overwrite for thread specific init
    virtual int OnExit(void); //overwrite for thread specific exit
    virtual int OnEvent(HANDLE hEvnt);

private:
	static DWORD WINAPI threadFunction	(void *p);
	int		write(void*	buffer,	int	nBytes,DWORD timeout);
    int     OnEventProxy(int nIndex);
};

/**
 * Default ctor of CRecThread.
 * Resets members.
 *        
*/
template<class TData>
CRecThread<TData>::CRecThread(void)
    : m_threadHandle(NULL),m_threadId(0),m_size(0),m_nUsrEvents(0),m_pEvents(NULL),m_hExitEvnt(NULL)
{
}

/**
 * Dtor of CRecThread.
 * Invokes destroy().
 *        
*/
template<class TData>
CRecThread<TData>::~CRecThread(void)
{
	destroy();
}

//------------------------------------------------------------------*
/**
 * Create establishes the queue with nObjects entries and 
 * creates the thread. Call destroy() to destroy the thread. Avoid
 * calling create() twice without calling destroy() in between. 
 * Additionally create() allocates a copy of the event handle array
 * being passed with argument pEvents. The number of events parameter
 * nUsrEvnts shall commensurate with the pEvents handle array.
 * The number of user events can be 0 if no additional event
 * processing is desired. If events are specified the user has to
 * call the SDK function ::SetEvent() to set one of the events passed
 * to create. Hence the method OnEvent() will be called if one of
 * the events passed to create gets signalled. OnEvent() in turn can
 * be overwritten to perform the desired user operation. The event
 * handles have to be created and closed (::CloseHandle) outside of
 * the context of this class.
 *
 * @param   nObjects - number of entries in the queue to set up.
 *          default value is 100.
 * @param   nUsrEvnts - number of events; shall commensurate with 
 *          the pEvents handle array. The number of user events can
 *          be 0 if no additional event is desired.
 * @param   pEvents - reference to the array with user events if
 *          nUsrEvnts > 0. Do not pass a null pointer if nUsrEvnts
 *          > 0.
 *        
 * @return  HRESULT - of last error if thread creation failed.<br>
 * @return  HRESULT - S_OK if overall creation succeeded.<br>
 * @return  HRESULT - E_OUTOFMEMORY in case of an out of memory situation.
 *          In an error case the queue and a copy of the handle array will 
 *          be cleaned up.<br>
 * @see     destroy()
*/
template<class TData>
HRESULT CRecThread<TData>::create(int nObjects,int nUsrEvnts,HANDLE *pEvents)
{
	HRESULT hr = S_OK;

    if(m_threadHandle) { //already created
        return S_OK;
    }

    /*exit event will be created
      it is an auto reset event - if you set it with SetEvent(..) it will 
      automatically be reset by any WaitForSingleObject() which is waiting for the event.
    */
    m_hExitEvnt = CreateEvent(NULL, FALSE, FALSE, NULL);
    if(!m_hExitEvnt) { //event cannot be created something must be wrong
        THREADTMPL_ASSERT(m_hExitEvnt);
        hr = HRESULT_FROM_WIN32(GetLastError());
        return hr;
    }

	m_itcFifo.create(sizeof(CContainer), nObjects);
    m_size = sizeof(CContainer);
	m_nUsrEvents = nUsrEvnts+1; //number of user events + exit event

    if(m_nUsrEvents) {
	    m_pEvents = new HANDLE[m_nUsrEvents];
	    if(!m_pEvents) {
		    m_itcFifo.destroy();
            ::CloseHandle(m_hExitEvnt);
            m_hExitEvnt = NULL;
		    m_nUsrEvents = 0;
		    m_size = 0;
		    return E_OUTOFMEMORY;
	    }

        m_pEvents[0] = m_hExitEvnt; //first of all is the exit event
        if(nUsrEvnts) {
            for(int i=0;i<nUsrEvnts;i++) {
                m_pEvents[i+1] = pEvents[i];
            }
        }
    }

	m_threadHandle = CreateThread(0, 4096,threadFunction, this, 0, &m_threadId);

    if(!m_threadHandle) {
        hr = HRESULT_FROM_WIN32(GetLastError());
        if(m_pEvents) {
		    delete [] m_pEvents;
		    m_pEvents = NULL;
        }
        
        ::CloseHandle(m_hExitEvnt);
        m_hExitEvnt = NULL;
        m_itcFifo.destroy();
		m_nUsrEvents = 0;
		m_size = 0;
    }
    return hr;
}

//------------------------------------------------------------------*
/**
 * Overwrite this method if you have init actions to do before
 * Thread function enters the working loop (e.g Initialization of COM).
 * Avoid calling any of the threads methods in the OnInitialize
 * method override. Do not call destroy() and create() methods 
 * in the OnInitialize method override.
 *
 * @return  enum value thread_noerror if no error occured in method.<br>
 * @return  enum value thread_error if an error during this method
 *          occured and it is desired to exit the thread at once.<br>
 *          The default non override of this method always returns
 *          thread_noerror.
 * @see     Usage section of class CRecThread description.
*/
template<class TData>
int CRecThread<TData>::OnInitialize(void)
{
    return thread_noerror;
}

//------------------------------------------------------------------*
/**
 * Overwrite OnExit method if you have exit actions to do after thread
 * has left the working loop (e.g. deinitialization of COM). Avoid
 * calling any of the threads methods in the OnExit method override.
 * Do not call destroy() and create() methods in the OnExit method override.
 *
 * @return  enum value thread_noerror if no error occured in method.<br>
 * @return  enum value thread_error if an error during this method
 *          occured and it is desired to exit the thread at once.<br>
 *          The default non override of this method always returns
 *          thread_noerror.
 * @see     Usage section of class CRecThread description.
*/
template<class TData>
int CRecThread<TData>::OnExit(void)
{
    return thread_noerror;
}

//------------------------------------------------------------------*
/**
 * Sends an destroy request to the thread context and waits for the
 * thread to exit. If the thread does not exit after a time period
 * defined by RECTHREAD_KILLTO the thread will be terminated. But this
 * constitutes a <b>fatal error situation</b>. Hence the user code of the
 * derived thread subclass shall be checked for potential deadlock
 * situations. 
 * After the thread has been exited or terminated (fatal) the internal
 * members will be reset to the Ctor state of the class. The queue will
 * be destroyed as well as the copy of the event handle array created
 * with create. The SDK function ::CloseHandle() will not be called for
 * the event handles because only a copy of the handle array is main-
 * tained in this class. Normally you should not call this method if
 * create() has not been called previously.
 *        
 * @see     create()
*/
template<class TData>
HRESULT CRecThread<TData>::destroy(void)
{
	if (m_threadHandle) {
        ::SetEvent(m_hExitEvnt);
		DWORD dwSucc = WaitForSingleObject(m_threadHandle,RECTHREAD_KILLTO);
		
        if(dwSucc != WAIT_OBJECT_0) {
            //fatal exit code
            TerminateThread(m_threadHandle,0);
        }
        
        ::CloseHandle(m_threadHandle);
		m_threadHandle = NULL;
        ::CloseHandle(m_hExitEvnt);
        m_hExitEvnt = NULL;
        m_threadId = 0;

        //do a cleanup again
        cleanup();
		m_itcFifo.destroy();
		m_size = 0;

		if(m_pEvents) {
			delete [] m_pEvents;
			m_pEvents = NULL;
            m_nUsrEvents = 0;
		}
	}
	return S_OK;
}

//------------------------------------------------------------------*
/**
 *  cleanup should normally not be called directly. Rather it will be called
 *  from the thread context if a destroy request is serviced. It cleans up
 *  all entries in the queue but does not service the requests associated with
 *  the entry (i.e. does not call processInput). Hence cleanup should only be
 *  invoked directly if you want to remove all entries from the queue without
 *  exiting the thread.
 *
 * @see     CRecThread class description.
*/
template<class TData>
void CRecThread<TData>::cleanup(void)
{
    CContainer datagram;

    while(m_itcFifo.read(&datagram, sizeof(datagram),0) != WAIT_TIMEOUT) {
        if(datagram.m_pItem) {
            datagram.m_pItem->Delete();
        }
    }
}

//------------------------------------------------------------------*
/**
 * schedule constitutes the thread's execution body.
 *
 * <b>Do not call this function directly under any circumstances.</b>
 *
 * The method calls the OnInitialize() method first which might have been over
 * written by a user thread subclass to perform user specific init-
 * ializations. The schedule enters the endless working loop.
 * In the working loop schedule reads the entries from the queue that have been 
 * written into the queue by the method write(). With an entry read schedule
 * calls processInput() to transfer the data to the user code. The user
 * code resides in a processInput() override. After calling processInput()
 * schedule calls TData::Delete() for the entry. The object references can be null.
 * In that case the deletion operation mentioned above will not be performed.
 *
 * If one of the events that have been specified with the create() method
 * gets signalled schedule calls the OnEventProxy() method with the index
 * of the event in the event handle array. OnEventProxy() in turn calls 
 * the OnEvent() method which has to be overwritten by the users subclass.
 * This functionality can optionally be omitted if no events are specified
 * with a previous create() call.
 *
 * In case of an suicide command which indicates an destroy request, the
 * queue will be cleaned up by calling cleanup() and schedule leaves
 * the working loop. Finally before exiting the thread schedule calls
 * OnExit() which might have been overwritten by the user thread subclass 
 * to perform user specific exit operations.
 *
 * @see - Usage section of class CRecThread description.
*/
template<class TData>
int CRecThread<TData>::schedule(void)
{
    static int nReceives;
    int	nRet,iThreadError;
	DWORD dwResult;

    OnDebugOutput(_T("\nCRecThread::threadFunction started id=0x%x\n"),m_threadId);
    
    if(OnInitialize() == thread_error) {//error in OnInitialize;
        OnExit();
        OnDebugOutput(_T("\nCRecThread::threadFunction exited id=0x%x\n"),m_threadId);
        return 1; //exit code
    }

    for (;;) { //receive threads work function endless loop
        CContainer datagram;
        bool bExit = false;

		nRet = m_itcFifo.readEx(&datagram, sizeof(datagram),m_nUsrEvents,m_pEvents,
			&dwResult,INFINITE);

        if (!nRet) { //normal datagram received
			nReceives++;

            iThreadError = processInput(datagram.m_nCommand,datagram.m_pItem);
        
            if(datagram.m_pItem) {
				datagram.m_pItem->Delete();
            }

            if(iThreadError == thread_error || iThreadError == thread_break) {
                bExit = true;
            }

		}
		else { //event trigger - look at the result 
			if(dwResult >= WAIT_OBJECT_0 && 
				dwResult <= WAIT_OBJECT_0 + m_nUsrEvents -1) {
				
				iThreadError = OnEventProxy(dwResult - WAIT_OBJECT_0);

                if(iThreadError == thread_error || iThreadError == thread_break) {
                    bExit = true;
                }

			}
			else { //error case
				OnDebugOutput(_T("\nCRecThread::threadFunction error %d\n"),dwResult);
			}
		}

        if(bExit == true){
            cleanup();
			break;
		}
    }

    if(OnExit() == thread_error) {
        OnDebugOutput(_T("\nCRecThread::threadFunction exited id=0x%x\n"),m_threadId);
        return 1;
    }
    
    OnDebugOutput(_T("\nCRecThread::threadFunction exited id=0x%x\n"),m_threadId);
    return 0;
}

//------------------------------------------------------------------*
/**
 * private method to write an entry to the FIFO (first-in-first-out) 
 * queue of the thread class.
 *
 * @param   buffer - pointer to the entry.
 * @param   nBytes - number of bytes to write.
 * @param   timeout - timeout value to wait for queue's write access grant.
 * @return  WAIT_TIMEOUT in case of an time out with 
 *          ITCBoundedRawFifo::write()<br>
 * @return  0 - success
 *          
 * @see     class ITCBoundedRawFifo
*/
template<class TData>
int	CRecThread<TData>::write(void*	buffer,	int	nBytes,DWORD timeout)
{
	return m_itcFifo.write(buffer, nBytes, timeout);
}

//------------------------------------------------------------------*
/**
 * Writes an entry resp. command request to the queue. The entry resp.
 * request will then be read by the thread context and processInput
 * will be called in the threads context to perform the user operation.
 * The user operation shall be performed in the processInput override.
 * nCommand shall not be 0 because this value constitutes a reserved value.
 *
 * @param       nCommand - command identifier - shall not be 0 because
 *              this value constitutes a reserved value.
 * @param       TData * - reference to a TData object.
 * @param       timeout - timeout value to wait for queue's write access grant.
 * @return      WAIT_TIMEOUT in case of an time out with 
 *              ITCBoundedRawFifo::write<br>
 * @return      0 - success
 *          
 * @see         class ITCBoundedRawFifo
*/
template<class TData>
int	CRecThread<TData>::write(int nCommand,TData * pData,DWORD timeout)
{
	CContainer datagram;

    datagram.m_nCommand = nCommand; //signature for normal write
	datagram.m_pItem = pData;
	return write(&datagram, sizeof(datagram),RECTHREAD_TO);
}

//------------------------------------------------------------------*
/**
 * overwrite processInput for data processing.
 * In processInput you the real data processing shall be performed.
 * processInput will be called by schedule() in the thread context if
 * schedule has read an entry resp. request.
 * The override in the user's subclass shall perform the data processing
 * as desired. Do not call any of the threads methods from processInput
 * (except cleanup() if this is desired).
 *
 * @param   nCommand - command identifier of the request being written to
 *          the queue.
 * @param   TData * - reference to a TData object. Do not delete TData
 *          object via this reference during processInput.
 * @return  always return enum value thread_noerror. 
 *
 * @see     cleanup,schedule, Usage section of class CRecThread description.
*/
template<class TData>
int CRecThread<TData>::processInput(int nCommand,TData * pItem)
{
	return thread_noerror;
}

//------------------------------------------------------------------*
/**
 * Overwrite OnDebugOutput to perform debug messaging or pass a message
 * to the user or an interface.
 * Avoid calling any of the threads methods in the OnDebugOutput override.
 * Do not call destroy and create methods in the OnDebugOutput override.
 *
 * @param       pszFormat - Format string (like printf)
 *              ... - any number of additional arguments referenced in 
 *              parameter pszFormat. Number of arguments must comply with
 *              number of referenced arguments in the format string.
 *        
 * @see         Usage section of class CRecThread description.
*/
template<class TData>
void CRecThread<TData>::OnDebugOutput(LPCTSTR pszFormat,...)
{
}

//------------------------------------------------------------------*
/**
 * will be called by schedule() in case of one of the user events that
 * have been specified with create() gets signalled. The method looks
 * up the event handle inside the array of event handles. and calls
 * the OnEvent() method to invoke the user defined code. Users who 
 * want to facilitate the event mechanism have to override OnEvent() in
 * the user specific subclass of CRecThread and perform the necessary
 * operations in the OnEvent() method.
 *
 * @param       nIndex - index inside the array of user events.
 * @return      enum value returned by OnEvent().
 * @see         OnEvent(),schedule(). 
 *              Usage section of class CRecThread description.
*/
template <class TData>
int CRecThread<TData>::OnEventProxy(int nIndex)
{
	if(nIndex >= m_nUsrEvents) //be tolerant here
		return thread_noerror;

	HANDLE hEvent = m_pEvents[nIndex];

    if(hEvent == m_hExitEvnt) {
        //exit event signalled - goodbye
        return thread_break;
    }

	OnDebugOutput(_T("\nCRecThread::OnEvent handle= 0x%x index= %d\n"),hEvent,nIndex);

	return OnEvent(hEvent);
}
//------------------------------------------------------------------*
/**
 * will be called by schedule indirectly in case of one of the user
 * events that have been specified with create gets signalled. 
 * The argument hEvent identifies the event handle of the event that
 * is signalled. Users who want to facilitate the event mechanism have
 * to override OnEvent() in the user specific subclass of CRecThread and
 * perform the necessary operations in the OnEvent() method.
 *
 * @param       hEvent - event handle of the event that is signalled.       
 * @return      enum value thread_noerror if it's intended not
 *              to exit the thread.<br>
 *              enum value thread_break if it's intended to exit the
 *              thread with no error indication (0).<br>
 *              enum value thread_error if it's intended to exit the
 *              thread with an error indication (1).<br>
 *              Default value is thread_noerror.
 * @see         OnEventProxy(),schedule().
 *              Usage section of class CRecThread description.
*/
template <class TData>
int CRecThread<TData>::OnEvent(HANDLE hEvent)
{
    return thread_noerror;
}

//------------------------------------------------------------------*
/**
 * Constitutes the private static thread function of the CRecThread
 * class. Obtains reference to CRecThread class from the void
 * reference and calls schedule to invoke the thread body function.
 *
 * @param   p - reference to CRecThread thread object.
 *          shall not be null.
 *        
 * @return  int - exit code of thread.
 * @see     schedule.
*/
template<class TData>
DWORD WINAPI CRecThread<TData>::threadFunction(void *p)
{
    int iExit;
	CRecThread *pReceive = (CRecThread*) p;
	THREADTMPL_ASSERT(pReceive);
    iExit = pReceive->schedule();
	return iExit;
}

//------------------------------------------------------------------*
/**
 *  class CSigThread - Event thread encapsulated in a class.
 *  with a signal event the thread will be waked up and
 *  does some action in function Schedule(). This is a virtual method
 *  which can be overwritten for real world processing.
 *  The signal event can be retrieved after a call to Create() by
 *  GetSignalEvent(). Additionally a TData reference can be passed 
 *  to Create() to set data into the thread. A background processing
 *  can be accomplished by setting the timeout interval with Create() or
 *  with SetTimeOutInterval() and overriding OnTimeOut() to perform the 
 *  background processing.
 *
 *  Usage:
 *
 *  1)  derive a class from it and overwrite Schedule() for processing<br>
 *  2)  Overwrite OnInitialize() method if you have init actions to do before
 *	    Thread function enters the working loop (e.g Initialization of COM).<br>
 *  3)  Overwrite OnExit() method if you have exit actions to do after thread
 *      has left the working loop (e.g. deinitialization of COM).<br>
 *  4)  Overwrite OnTimeOut() method if you have to do some background processing
 *      OnTimeOut() will be called every timeout interval if no event has been
 *      signalled.<br>
 *  5)  instantiate the class<br>
 *  6)  call Create() - specify the timeout interval with Create().<br>
 *  7)  call GetSignalEvent() for the signal event handle.<br>
 *  8)  from another context set the event by ::SetEvent()
 *      if you have something to process.<br>
 *  9)  Modify the timeout interval if desired with SetTimeOutInterval().<br>
 *  10)  call Destroy() to destroy the thread or destroy the object
 *      if you destroy the object the destructor will handle the destruction
 *      of the thread.<br>
 *  11) DumpThreadInfo() gives debug information on thread creation,termination 
 *      and if base class's Schedule() will be called the schedule events. <br>
 *
*/
template<class TData>
class CSigThread
{
protected:
	HANDLE  m_hThread;			/**< handle of the thread */
	DWORD   m_dwId;				/**< thread id */
	HANDLE  m_hSignalEvnt;		/**< signal event for thread */
	BOOL    m_fTerminate;		/**< terminate flag */
    DWORD   m_dwMillisec;       /**< milliseconds to wait in loop */
    TData*  m_pData;            /**< additional thread data */
public:
    enum {thread_noerror = 0, thread_error = -1, thread_break = 1};
	CSigThread(void);
	virtual ~CSigThread(void);

	virtual int Create(DWORD,TData * pData); 
	virtual int Destroy(void);			

    virtual int OnInitialize();  
    virtual int OnExit();        
	virtual int Schedule(void);  
    virtual int OnTimeOut(void);
    
    //------------------------------------------------------------------*
    /**
     * Sets the signal event to call Schedule in the threads context.
     * @see     SDK function ::SetEvent()
    */    
    void SignalEvent()           //signal event
    {
		if(m_hSignalEvnt) {
			if(!SetEvent(m_hSignalEvnt)) // wake up the thread
			    THREADTMPL_ASSERT(!"SetEvent failed");
		}
    }
    
	const HANDLE GetSignalEvent(void) const;

    //------------------------------------------------------------------*
    /**
     * gets the timeout interval value.
     *
     * @return  DWORD - value of the actual timeout interval in ms.
     * @see     SetTimeOutInterval()
    */    
    const DWORD GetTimeOutInterval() const
    {
        return m_dwMillisec;
    }

    //------------------------------------------------------------------*
    /**
     * sets the timeout interval value.
     *
     * @param   dwTimeOut - value of the timeout interval in ms to set.
     * @see     GetTimeOutInterval()
    */    
    void SetTimeOutInterval(DWORD dwTimeOut)
    {
        m_dwMillisec = dwTimeOut;
    }
	
private:
	static DWORD WINAPI ThreadFunction(void *pSigThread);
	//thread function must be static and pSigThread points 2 instance of the class
    void DumpThreadInfo(LPCTSTR pInfo); //only for debugging
};

//------------------------------------------------------------------*
/**
 * Ctor of the class CSigThread - initialized members.
 * Timeout inteval will ve set to inifite.
 *
*/
template<class TData>
CSigThread<TData>::CSigThread(void)
    : m_hThread(NULL),m_dwId(0),m_hSignalEvnt(NULL),m_fTerminate(FALSE)
{
    m_dwMillisec = INFINITE;
    m_pData = NULL;
}

/**
 * Dtor of CSigThread.
 * Invokes Destroy().
 *        
*/
template<class TData>
CSigThread<TData>::~CSigThread(void)
{
    Destroy(); //destroy handles multiple calls to itself
}

//------------------------------------------------------------------*
/**
 *  Creates the signal event for the thread and the thread itself.
 *  Additionally sets the TData reference into the thread class.
 *  The timeout value will be set as well from the argument 
 *  dwMilliSecs. The method returns the enum value thread_error
 *  if the thread and the event have already been created or if 
 *  the creation of the event or the thread fails. Otherwise it returns
 *  thread_noerror.Call Destroy() to destroy the thread. Avoid
 *  calling Create() twice without calling Destroy() in between.
 * 
 *  @param      dwMilliSecs - timeout value for the wait function
 *  @param      TData * - reference to additional data object 
 *              can be null.
 *        
 *  @return     enum value thread_error if the thread and the event
 *              have already been created or if  the creation of the event 
 *              or the thread fails. Otherwise it returns thread_noerror.
 *
 * @see         Destroy().
*/
template<class TData>
int CSigThread<TData>::Create(DWORD dwMilliSecs, TData* pData)
{
    m_pData = pData;

    if(m_hThread || m_hSignalEvnt)
        return thread_error; //still exists

    /*signal event will be created
      it is an auto reset event - if you set it with SetEvent(..) it will 
      automatically be reset by any WaitForSingleObject() which is waiting for the event.
    */
    m_hSignalEvnt = CreateEvent(0, 0, 0, 0);
    
    if(!m_hSignalEvnt) { //event cannot be created something must be wrong
        THREADTMPL_ASSERT(m_hSignalEvnt);
        return thread_error;
    }

    m_fTerminate = FALSE;
    m_dwMillisec = dwMilliSecs;
    //the this pointer will be passed 2 ThreadFunction as param
    //m_dwId contains the thread id for debugging in msdev
	m_hThread = CreateThread(NULL, 4096,ThreadFunction,	this,
							0,&m_dwId);

    DumpThreadInfo(_T("Thread created :"));

	if (!m_hThread) { //thread cannot be created close the handle
        //and return error.
        CloseHandle(m_hSignalEvnt);
        m_hSignalEvnt = 0;
        return thread_error;
	}

    return thread_noerror;
}

//------------------------------------------------------------------*
/**
 * Sets the terminate flag and then the signal event to request a 
 * thread termination and waits for the thread to exit. If the thread
 * does not exit after a time period defined by RECTHREAD_KILLTO the 
 * thread will be terminated. But this constitutes a <b>fatal error 
 * situation</b>. Hence the user code of the derived thread subclass shall 
 * be checked for potential deadlock situations. 
 * After the thread has been exited or terminated (fatal) the internal
 * members will be reset to the Ctor state of the class. The signal 
 * event will be closed as well. Normally you should not call this 
 * method if Create() has not been called previously. Is called automatically
 * by the Dtor of the class. The data reference TData will not be deleted.
 * only set to NULL.
 * 
 * @return      thread_error if the thread or event has already been destroyed.<br>
 *              thread_noerror in all other cases.
 * @see         Create()
*/
template<class TData>
int CSigThread<TData>::Destroy(void)
{
    DWORD dwSucc;

    if(!m_hThread)
        return thread_error; //does not exist

    if(!m_hSignalEvnt)
        return thread_error; //this should not happen

    m_fTerminate = TRUE; //flag termination

    if(!SetEvent(m_hSignalEvnt)) // wake up the thread
        THREADTMPL_ASSERT(!"SetEvent failed");
        
    //waiting for the termination of the thread		
    dwSucc = WaitForSingleObject(m_hThread,RECTHREAD_KILLTO);
    //the following code makes only sense if you define a timeout != INFINITE
    if (dwSucc != WAIT_OBJECT_0) {
        THREADTMPL_ASSERT(!"fatal thread termination");
        TerminateThread(m_hThread,0);
    }

    DumpThreadInfo(_T("Thread terminated :"));

    //close the handles
    if(m_hThread && !CloseHandle(m_hThread))
        THREADTMPL_ASSERT(!"CloseHandle failed");

    if(m_hSignalEvnt && !CloseHandle(m_hSignalEvnt))
        THREADTMPL_ASSERT(!"CloseHandle failed");
    
    m_hThread = NULL;
    m_hSignalEvnt = NULL;
    m_dwId = 0;
    m_dwMillisec = INFINITE;
    m_pData = NULL;
    return thread_noerror;

}

//------------------------------------------------------------------*
/**
 * Returns the handle of the signal event.
 *        
 * @return      HANDLE - handle of the signal event.
 *              can be NULL if the Create() method has not been called
 *              or after Destroy() has been called.
 * 
*/
template<class TData>
const HANDLE CSigThread<TData>::GetSignalEvent(void) const
{
    return m_hSignalEvnt;
}

//------------------------------------------------------------------*
/**
 * ThreadFunction constitutes the thread's execution body.
 *
 * <b>Do not call this function directly under any circumstances.</b>
 *
 * The method calls the OnInitialize() method first which might have been over
 * written by a user thread subclass to perform user specific init-
 * ializations. In case that OnInitalize() method returns thread_error
 * the method calls OnExit() which might have been overwritten by the user
 * thread subclass to perform user specific exit operations.
 * Then ThreadFunction enters the endless working loop.
 * In the working loop ThreadFunction waits for the signal event to be
 * signalled with the defined timeout value. If the timeout value expires
 * ThreadFunction calls OnTimeOut(). OnTimeOut() can be overwritten by the 
 * user in the user's subclass to perform background processing and for
 * example compute a new timeout value and set the new value before return.
 * If OnTimeOut() returns thread_error or thread_break the endless loop will
 * be exited and in turn the thread exits. In case of an signal event being
 * signalled ThreadFunction checks the terminate flag first to exit the 
 * endless working loop if the flag has been set. Otherwise ThreadFunction
 * invokes the Schedule() method. This is a virtual method which can be over-
 * written in the user's subclass for real world processing. If the Schedule()
 * override (this is normally the case) returns thread_error or thread_break
 * the endless loop will be exited and in turn the thread exits.
 * Finally if one of the exit conditions become true the endless working
 * loop will be exited and the thread in turn exits. 
 * Before exiting the thread ThreadFunction calls OnExit() which might 
 * have been overwritten by the user thread subclass  to perform user 
 * specific exit operations.
 *
 * @see - Usage section of CSigThread class description.
*/
template<class TData>
DWORD WINAPI CSigThread<TData>::ThreadFunction(void *pSigThread)
{
    CSigThread * pInstance;
    DWORD dwSucc;
    int iRet;

    pInstance = (CSigThread *)pSigThread;

    if(!pInstance) {
        THREADTMPL_ASSERT(pInstance);
        return 1;//exit code
    }

    if(pInstance->OnInitialize() == thread_error) {//error in OnInitialize;
        pInstance->OnExit();
        return 1; //exit code
    }

	for (;;) { //forever
        dwSucc = WaitForSingleObject(pInstance->m_hSignalEvnt,pInstance->m_dwMillisec);
        
        if(pInstance->m_fTerminate) {
            break;
        }

        if(dwSucc == WAIT_TIMEOUT) {
            iRet = pInstance->OnTimeOut(); //do something background
            if(iRet == thread_error || iRet == thread_break) {
                //break it
                break;
            }
        }

        if(dwSucc == WAIT_OBJECT_0) {
            iRet = pInstance->Schedule();
            if(iRet == thread_error || iRet == thread_break) {
                //break it
                break;
            }
        }
    }

    pInstance->OnExit();
    return 0;
}

//------------------------------------------------------------------*
/**
 * overwrite Schedule for data processing.
 * In Schedule() you the real data processing shall be performed.
 * Schedule will be called by ThreadFunction() in the thread context if
 * the signal event has been set.
 * The override in the user's subclass shall perform the data processing
 * as desired. Do not call any of the threads methods from Schedule()
 *
 * @return  enum value thread_noerror if no error occured in method.<br>
 * @return  enum value thread_error if an error during this method
 *          occured and it is desired to exit the thread at once.<br>
 * @return  enum value thread_break if it is desired to exit the 
 *          thread.<br>
 *          The default non override of this method always returns
 *          thread_noerror.
 *
 * @see     ThreadFunction(), Usage section of CSigThread class description.
*/
template<class TData>
int CSigThread<TData>::Schedule(void) //body of the thread 
{ //do the action here .. what ever is desired
    DumpThreadInfo(_T("CSigThread::Schedule"));
    return thread_noerror;
}

//------------------------------------------------------------------*
/**
 * Overwrite this method if you have init actions to do before
 * Thread function enters the working loop (e.g Initialization of COM).
 * Avoid calling any of the threads methods in the OnInitialize()
 * method override. Do not call Destroy() and Create() methods 
 * in the OnInitialize method override.
 *
 * @return  enum value thread_noerror if no error occured in method.<br>
 * @return  enum value thread_error if an error during this method
 *          occured and it is desired to exit the thread at once.<br>
 *          The default non override of this method always returns
 *          thread_noerror.
 * @see     Usage section of CSigThread class description.
*/
template<class TData>
int CSigThread<TData>::OnInitialize()
{   //do specific initialization here before going in the endless loop
    return thread_noerror;
}

//------------------------------------------------------------------*
/**
 * Overwrite OnExit method if you have exit actions to do after thread
 * has left the working loop (e.g. deinitialization of COM). Avoid
 * calling any of the threads methods in the OnExit method override.
 * Do not call Destroy() and Create() methods in the OnExit method override.
 *
 * @return  enum value thread_noerror if no error occured in method.<br>
 * @return  enum value thread_error if an error during this method
 *          occured and it is desired to exit the thread at once.<br>
 *          The default non override of this method always returns
 *          thread_noerror.
 * @see     Usage section of CSigThread class description.
*/
template<class TData>
int CSigThread<TData>::OnExit()
{
    return thread_noerror;
}

//------------------------------------------------------------------*
/**
 * Override this function to perform background processing each time
 * the timeout value expires. If the timeout value expires
 * ThreadFunction() calls OnTimeOut. OnTimeOut can be overwritten by the 
 * user in the user's subclass to perform background processing and for
 * example compute a new timeout value and set the new value before return.
 * If OnTimeOut returns thread_error or thread_break the endless loop will
 * be exited and in turn the thread exits. 
 *
 * @return  enum value thread_noerror if no error occured in method.<br>
 * @return  enum value thread_error if an error during this method
 *          occured and it is desired to exit the thread at once.<br>
 * @return  enum value thread_break if it is desired to exit the 
 *          thread.The default non override of this method always returns
 *          thread_noerror.
 * @see     Usage section of CSigThread class description.
 *
*/
template<class TData>
int CSigThread<TData>::OnTimeOut()
{   //do background processing here
    return thread_noerror;
}

//------------------------------------------------------------------*
/**
 * Dumps the thread information - thread handle and id to debug
 * output.
*/
template<class TData>
void CSigThread<TData>::DumpThreadInfo(LPCTSTR pInfo)
{
#ifdef _DEBUG
    TCHAR *pOut;
    THREADTMPL_ASSERT(pInfo);

    pOut = new TCHAR[lstrlen(pInfo) + 50];
    wsprintf(pOut,_T("%s Handle: 0x%08x Id: 0x%08x\n"),pInfo,m_hThread,m_dwId);
    OutputDebugString(pOut);
    delete pOut;
#endif
}

#endif // __THRDTMPL_H_
/*
 *----------------------------------------------------------------------------*
 *  $History: thrdtmpl.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/inc
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 6  *****************
 * User: Sis          Date: 29.08.01   Time: 13:45
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 5  *****************
 * User: Su           Date: 19.07.01   Time: 15:42
 * Updated in $/4Control/COM/softing/fc/inc
 * Matched CReceiverThread to the functionality of CRecThread.
 * 
 * *****************  Version 4  *****************
 * User: Su           Date: 19.07.01   Time: 10:28
 * Updated in $/4Control/COM/softing/fc/inc
 * CRecThread will be terminated by an extra event rather than a command.
 * 
 * *****************  Version 3  *****************
 * User: Su           Date: 18.07.01   Time: 19:45
 * Updated in $/4Control/COM/softing/fc/inc
 * CRecThread::OnEventProxy allows for return enum values as defined with
 * the class.
 * 
 * *****************  Version 2  *****************
 * User: Su           Date: 12.02.01   Time: 10:02
 * Updated in $/4Control/COM/softing/fc/inc
 * Doxygen conform documentation.
 * 
 * *****************  Version 1  *****************
 * User: Su           Date: 29.01.01   Time: 11:45
 * Created in $/4Control/COM/softing/fc/inc
 * Moved from ATCMControl V1.x database
 * 
 * *****************  Version 2  *****************
 * User: Su           Date: 29.01.01   Time: 11:44
 * Updated in $/4ControlMesse/COM/softing/fc/inc
 * Added Documentation. Harmonized CRecThread and CReceiverThread.
 * 
 * *****************  Version 1  *****************
 * User: Su           Date: 14.11.00   Time: 15:42
 * Created in $/4ControlMesse/COM/softing/fc/inc
 * Initial rev.
 *==
 *----------------------------------------------------------------------------*
*/
