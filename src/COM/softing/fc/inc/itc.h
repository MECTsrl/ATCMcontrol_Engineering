/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/inc/itc.h 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: itc.h $
 *                       $Logfile: /4CReleased/V2.20.00/COM/softing/fc/inc/itc.h $
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
 *== Interthread communication classes. See class description for details.
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  07.11.2000  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/
#ifndef ITC_H
#define ITC_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include <malloc.h>
//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*
#ifndef ITC_ASSERT
#define ITC_ASSERT(x) assert((x))
#endif
//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

//------------------------------------------------------------------*
/**
 *  class ITCMonitor - Inter Thread Communication Monitor.
 *
 *  Interthread communication class based on 3 semaphores. Performs
 *	os-level handling of synchronization and thereby monitors and protects
 *  the access to a buffer/queue. 
 *  The three semaphores are named nonEmpty, isIdle and nonFull. 
 *  The corresponding member variables nonEmpty [0] , isIdle [1] , nonFull[2] 
 *  contain the index to the corresponding semaphore.
 *  The meaning of the semaphores is describes as follows:
 *	
 *  1. nonEmpty =1 If the buffer or the queue is not empty.
 *  2. isIdle = 1 No one accesses the buffer or queue.
 *  3. nonFull =1 If the buffer or the queue is not full.
 *  
 *  The access mechanism will be depicted with the methods enterReader,
 *  enterReaderEx, enterWriter and leave. The following flow chart 
 *  describes the read and write access to the queue or buffer being
 *  monitored by ITCMonitor. Use the data flow proposed with this flow
 *  chart and the corresponding methods of ITCMonitor to protect a buffer
 *  or queue with ITCMonitor (sample code see ITCBoundedRawFifo).
 *
 *      <b>Reader:<br></b>
 *          |<br>
 *          v<br>
 *  Wait for isIdle && nonEmpty (WaitforMultipleObject) - enterReader(Ex)<br>
 *          |<br>
 *          v<br>
 *  isIdle && nonEmpty: read<br>
 *          |<br>
 *          v<br>
 *  If buffer/queue is prior full (nonFull == 0)
 *  hence nonFull=1 meaning not full because reader has read an entry - setNonFull()<br>
 *          |<br>
 *          v<br>
 *  If buffer/queue is now empty set nonEmpty = 0 - setEmpty()<br>
 *          |<br>
 *          v<br>
 *  release semaphore isIdle (set to 1) - leave()<br>
 *          |<br>
 *          v<br>
 *      <b>END</b><p>
 *  
 *      <b>Writer:</b><br>
 *          |<br>
 *          v<br>
 *  Wait for isIdle && nonFull (WaitforMultipleObject) - enterWriter()<br>
 *          |<br>
 *          v<br>
 *  isIdle && nonFull: write<br>
 *          |<br>
 *          v<br>
 *  If the buffer/queue is prior empty (nonEmpty == 0), now 
 *  the buffer/queue is no longer empty because write has written an entry.
 *  Hence nonEmpty =1 has to be set to 1 (signalled). - setNonEmpty()<br>
 *          |<br>
 *          v<br>
 *  If the buffer/queue is now full - reset nonFull (=0) - setFull()<br>
 *          |<br>
 *          v<br>
 *  release semaphore isIdle (set to 1) - leave()<br>
 *          |<br>
 *          v<br>
 *      <b>END</b>
 *
*/
class ITCMonitor
{
protected:
    ITCMonitor()
    : accessMode(shared), isFull(0), isEmpty(1)
    {
	    memset(syncObjects, 0, sizeof(syncObjects));
        nonEmpty = reader; //0;
        isIdle	 = shared; //1;
        nonFull  = writer; //2;
    }

	virtual		~ITCMonitor()
    {
	    (void) destroy();
    }

	int create		();
	int destroy		();

	int	lock		(const DWORD timeout);
	int	unlock		();

	// return
	//	           0: if entered
	//	WAIT_TIMEOUT: if timed out 
	//  currently in case of error process will be terminated
	int	enterReader		(const DWORD timeout);
	int	enterWriter		(const DWORD timeout);
    int enterReaderEx   (int nUsrEvnts,HANDLE *pEvents,DWORD *pWaitResult,DWORD timeout);
    // return value: 0 - read access 1 - look at the result.

	int       leave		();

	// only to be called by reader between enterReader() and leave
	int		setNonFull	();		// 2 be called by reader
	void	setEmpty	();		// 2 be called by reader

	// only to be called by writer between enterWriter() and leave
	void	setFull		();		// 2 be called by writer
	int		setNonEmpty ();		// 2 by called by writer

protected:
	void	handleOsCallErrors(LPTSTR msg);
private:
	enum	AccessMode { reader = 0, shared = 1, writer = 2};
	int     enter	(const AccessMode accessMode, const DWORD timeout);
	int		post	(AccessMode which);		// reader || writer

	HANDLE		syncObjects[3];		// array of 3 semaphores

	// semaphore numbers (= indexes for syncObjects)
	int	nonEmpty;   // initially not signalled \
                    //		                    | reader
	int	isIdle;		// initially   	signalled   +
                    //		                    | writers
    int	nonFull;    // initially   	signalled  /

	AccessMode	accessMode;

	int		isFull;  //1 - queue buffer monitored is full
	int		isEmpty; //1 - queue buffer monitored is empty

};

////////////////////////////////////////////////////////////////////////////
// ITCMonitor
////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------*
/**
 * creates the 3 semaphores nonEmpty, isIdle, nonFull, 
 * and initializes the semaphores with the following inital values:
 * 
 * nonEmpty - initially not signalled (0)
 * isIdle - initally signalled (1)
 * nonFull - initally signalled (1)
 *
 * The counterpart method destroy closes all 3 semaphores.
 * 
 * @return          Returns last error code of OS call if a semaphore
 *                  could not be created. All other semaphores that have
 *                  been created prior to the error will be deleted.
 * @see             destroy()
*/
inline int	 ITCMonitor::create(void)
{
	int		fail = 0;

	for (int i = nonEmpty; i <= nonFull; i++) {
		syncObjects[i] = CreateSemaphore(0, (i == nonEmpty) ? 0L : 1L, 1, 0);
		if (syncObjects[i] == 0) {
			fail = GetLastError();
			(void) destroy();
			break;
		}
	}

	return fail;
}

//------------------------------------------------------------------*
/**
 * Closes all 3 semaphores of the ITCMonitor. Should be called by the
 * dtor of the ITCMonitor class to clean up the semaphores. Additionally
 * the flag isEmpty will be set and the flag isFull will be reset.
 *
 * @return  Returns last error code of OS call if a semaphore
 *          could not be closed. 
 * @see     create()             
*/
inline int	 ITCMonitor::destroy(void)
{
	int	fail = 0;

	for (int i = nonEmpty; i <= nonFull; i++) {
		if (syncObjects[i]) {
			if (CloseHandle(syncObjects[i]) != TRUE) {
				fail = GetLastError();
				break;
			}
			syncObjects[i] = 0;
		}
	}
	isFull = 0; isEmpty = 1;
	
	return fail;
}

//------------------------------------------------------------------*
/**
 * Trys to get read access to the buffer/queue being protected by
 * the ITCMonitor within a time period determined by timeout.
 * Waits for the two semaphores nonEmpty and isIdle to be signalled
 * saying that no other thread context accesses the buffer/queue and
 * that the buffer/queue is not empty. In that case enterReader gains
 * the access to the buffer/queue and sets the access mode to reader.
 * The method returns WAIT_TIMEOUT in the event that the time period
 * timeout has been elapsed before access is granted.
 *
 * @param           - DWORD timeout - determines the time period to
 *                  wait for access grant.
 * @return          - 0 if success, WAIT_TIMEOUT in case of time out.
 * @see             enterReaderEx(), enterWriter()
*/
inline int ITCMonitor::enterReader(const DWORD timeout)
{
	int	fail = 0;
	DWORD	succ = 0;
    //reader and shared
    succ = WaitForMultipleObjects(2, &syncObjects[reader], TRUE, timeout); 
	if (succ < WAIT_OBJECT_0 || succ >= WAIT_OBJECT_0 + 2) {
		if (succ == WAIT_TIMEOUT) {
			fail = WAIT_TIMEOUT;
		} else {
			handleOsCallErrors(_T("WaitForMultipleObjects()"));
		}
	} else {
		accessMode = reader;
	}
	return fail;

}


//------------------------------------------------------------------*
/**
 * Trys to get write access to the buffer/queue being protected by
 * the ITCMonitor within a time period determined by timeout.
 * Waits for the two semaphores isIdle and nonFull to be signalled
 * saying that no other thread context accesses the buffer/queue and
 * that the buffer/queue is not full. In that case enterWriter gains
 * the access to the buffer/queue and sets the access mode to writer.
 * The method returns WAIT_TIMEOUT in the event that the time period
 * timeout has been elapsed before access is granted.
 *
 * @param           - DWORD timeout - determines the time period to
 *                  wait for access grant.
 * @return          - 0 if success, WAIT_TIMEOUT in case of time out.
 * @see             enterReaderEx(), enterReader()
 *
*/
inline int ITCMonitor::enterWriter(const DWORD timeout)
{
	int	fail = 0;
	DWORD	succ = 0;

    //shared and writer
	succ = WaitForMultipleObjects(2, &syncObjects[shared], TRUE, timeout); 
	if (succ < WAIT_OBJECT_0 || succ >= WAIT_OBJECT_0 + 2) {
		if (succ == WAIT_TIMEOUT) {
			fail = WAIT_TIMEOUT;
		} else {
			handleOsCallErrors(_T("WaitForMultipleObjects()"));
		}
	} else {
		accessMode = writer;
	}
	return fail;
}

//------------------------------------------------------------------*
/**
 * Trys to get read access to the buffer/queue being protected by
 * the ITCMonitor within a time period determined by timeout.
 * Waits for one of the argument nUsrEvents user events to be signalled 
 * or the two semaphores nonEmpty and isIdle to be signalled.
 * saying that no other thread context accesses the buffer/queue and
 * that the buffer/queue is not empty.
 *
 * The following wait condition must be met to return enterReaderEx:
 * (1 of n user events == 1) || (nonEmpty == 1 && isIdle == 1).
 *  
 * Case (1 of n user events == 1): enterReaderEx returns the event that
 * is signalled in the parameter pWaitResult.
 * Case (nonEmpty == 1 && isIdle == 1): enterReaderEx gains the access
 * to the buffer/queue and sets the access mode to reader.
 *
 * @param           nUsrEvnts - number of user events that make up the
 *                  additional wait condition (see above)
 * @param           pEvents - reference to nUserEvnts event handles.
 * @param           pWaitResult - returns the user event that is signalled
 *                  in case that return value == 1 (look first at the 
 *                  return value.
 * @param           timeout - determines the time period to wait for 
 *                  access grant.
 * @return          0 - read access, 1 - one of user events signalled, look 
 *                  at parameter pWaitResult. 
 *
 * @see             enterReader(), flow chart with class description, 
 *                  SDK - wait functions.
*/
inline int ITCMonitor::enterReaderEx(int nUsrEvnts,HANDLE *pEvents,DWORD *pWaitResult,DWORD timeout)
// return value: 0 - read access 1 - look at the result.
{
	DWORD	succ;
    DWORD   dwCount;
    HANDLE *pHandles;

    pHandles = (HANDLE*) alloca(sizeof(HANDLE) * (nUsrEvnts+1));
    
    for(int i=0;i<nUsrEvnts;i++) {
        pHandles[i] = pEvents[i];
    }
    pHandles[i] = syncObjects[reader];
    dwCount = nUsrEvnts + 1;

	succ = WaitForMultipleObjects(dwCount,pHandles,FALSE, timeout);
    //All events
    if(succ != (WAIT_OBJECT_0 + dwCount - 1)) {
        //one of the user events signalled state
        *pWaitResult = succ;
        return 1; //1 means look at result
    }
    else { 
        //the reader event = signalled state
        //now wait for shared/isIdle
        succ = WaitForSingleObject(syncObjects[shared],INFINITE); 
        // read access granted
        accessMode = reader;
        return 0; //0 =  read access
    }
}

//------------------------------------------------------------------*
/**
 * locks the ITCMonitor and thereby the object (buffer,queue) being
 * protected by the monitor. Neither reader nor writer methods have
 * access to the object being monitored until the counterpart method
 * unlock has been called. The semaphore isIdle will be used by 
 * reader and writer methods and therefore is used for the locking.
 *
 * @param           - DWORD timeout - determines the time period to
 *                  wait for lock grant.
 * @return          - 0 if success, WAIT_TIMEOUT in case of time out.                  
 * @see             unlock()             
*/
inline int	ITCMonitor::lock(const DWORD timeout)
{
	DWORD	succ = WaitForSingleObject(syncObjects[shared], timeout); 
	if (succ != WAIT_OBJECT_0) {
		handleOsCallErrors(_T("WaitForMultipleObjects()"));
	}

	return (int) succ;
}


//------------------------------------------------------------------*
/**
 * Unlocks the ITCMonitor and thereby the object (buffer,queue) being
 * protected by the monitor. Lock has to be called prior to the call 
 * to unlock.Neither reader nor writer methods have access to the object 
 * being monitored until unlock has been called. 
 * The semaphore isIdle will be used by reader and writer methods and 
 * therefore is used for the locking. 
 *
 * @see             lock()             
*/
inline int	ITCMonitor::unlock()
{
	return post(shared);
}


//------------------------------------------------------------------*
/**
 * leave should be called as a last operation after a read/write process
 * In the event of an read process (accessMode == reader) the semaphore
 * nonEmpty will be set if the isEmpty flag is reset signalling that the
 * buffer/queue is not empty after the read operation. Otherwise the buffer
 * or queue is empty after the read operation and the semaphore nonEmpty
 * !!remains!! reset.
 * In the case of an write process (accessMode == writer) the semaphore 
 * nonFull will be set if the isFull flag is reset signalling that the
 * buffer/queue is not full after the write operation. Otherwise the 
 * buffer/queue is full after the write operation and the semaphore nonFull
 * !!remains!! reset.
 * Finally leave sets the isIdle semaphore signalling that the buffer/queue
 * is unlocked now for the the next access (reader or writer).
 * <b>ATTENTION:</b> For proper operation always call leave as last operation after
 * a read/write process.
 * <b>NOTE:</b> The wait functions invoked by enterReader(), enterWriter() always 
 * decrease the semaphore count by one if the semaphore is signalled and the 
 * wait function returns. Hence according to the current states of the ITC-
 * Monitor the semaphore counts have to be incremented by ReleaseSemaphore.
 * This is done by the method post.
 *
 * @see - post(), flow chart with class description,SDK- ReleaseSemaphore
*/
inline int ITCMonitor::leave()
{
	ITC_ASSERT((accessMode == reader) || (accessMode == writer));

	int	fail = 0;

	switch (accessMode) {
	case reader:
        if (!isEmpty) { //still not empty - set nonEmpty semaphore
			post(reader);
		}
		break;

	case writer:
        if (!isFull) { //still not full - set nonFull semaphore
			post(writer);
		}
		break;
	}
	// still no thread can enter
	accessMode = shared;
	fail = post(shared);
	// the door is now opened
	return fail;
}


//------------------------------------------------------------------*
/**
 * post releases the semaphore idetified by parameter which.
 * The wait functions invoked by enterReader(), enterReaderEx(), enterWriter()
 * always decrease the semphore count by one if the semaphore is signalled and the 
 * wait function returns. Hence according to the current states of the ITC-
 * Monitor the semaphore counts have to be incremented by ReleaseSemaphore.
 *
 * @param           which - identifying the semaphore to release (increments
 *                  semaphore count by one.
 * @return          0 - ReleaseSemaphore succeeded, 1 - ReleaseSemaphore failed
 *
 * @see             flow chart with class description, leave, SDK- ReleaseSemaphore
*/
inline int ITCMonitor::post(AccessMode which)
{
    //0 everything ok 1 - oops
	ITC_ASSERT (which == reader || which == writer || which == shared);
	BOOL	osSucc;
	if ((osSucc = ReleaseSemaphore(syncObjects[which], 1, 0)) != TRUE) {
		handleOsCallErrors(_T("ReleaseSemaphore()"));
	}
	return osSucc != TRUE;
}

//------------------------------------------------------------------*
/**
 * sets the isFull flag. Should be called by the user of ITCMonitor
 * during a write operation when the write operation detects that
 * the buffer/queue monitored is full. Should be called by writer 
 * between enterWriter() and leave.
 *
 * @see             flow chart with class description
*/
inline void ITCMonitor::setFull(void)
{
	isFull = 1;
}

//------------------------------------------------------------------*
/**
 * If the buffer/queue is full (isFull set) the method resets the
 * isFull flag signalling that the buffer/queue is no longer full.
 * Accordingly the method sets the nonFull semaphore (no longer full).
 * Should be called by the reader between enterReader() and leave.
 *
 * @return          return of post()
 * @see             post(),flow chart with class description
*/
inline int ITCMonitor::setNonFull(void)
{
	if (isFull) {
		isFull = 0;
		return post(writer);
	}
	return 0;
}


//------------------------------------------------------------------*
/**
 * sets the isEmpty flag. Should be called by the user of ITCMonitor
 * during a read operation if the read operation detects that
 * the buffer/queue monitored is empty. Should be called by reader 
 * between enterReader() or enterReaderEx() and leave.
 *
 * @see             flow chart with class description
*/
inline void ITCMonitor::setEmpty(void)
{
	isEmpty = 1;
}


//------------------------------------------------------------------*
/**
 * If the buffer/queue is empty (isEmpty set) the method resets the
 * isEmpty flag signalling that the buffer/queue is no longer empty.
 * Accordingly the method sets the nonEmpty semaphore (no longer empty).
 * Should be called by the writer between enterWriter() and leave().
 *
 * @see             flow chart with class description
*/
inline int ITCMonitor::setNonEmpty(void)
{
	if (isEmpty) {
		isEmpty = 0;
		return post(reader);
	}
	return 0;
}


//------------------------------------------------------------------*
/**
 * handles OS call errors. Asserts the SDK function that caused the
 * error along with the error code.
 *
*/
inline void ITCMonitor::handleOsCallErrors(LPTSTR msg)
{
	char eMsg[128];
	sprintf(eMsg,"%s failed eCode = %d",msg,GetLastError());
    ITC_ASSERT(eMsg);
}

//------------------------------------------------------------------*
/**
 *  class ITCBoundedRawFifo - Inter Thread Bounded Raw FIFO.
 *  Bounded raw byte oriented FIFO buffer. The protection against 
 *  concurrent access to the buffer will be facilitated by the
 *  super class ITCMonitor. All entries of the buffer have to
 *  have the same size. The buffer is bounded meaning that the
 *  size of the buffer resp. number of entries in the buffer is
 *  limited. Three methods will be provided to access the buffer:
 *  read,readEx,write.
 *  used for multiple writers / single reader
 *  rules:
 *  1. when reader reads buffer, writers are blocked
 *  2. when one writer writes to buffer, reader is blocked
 *  3. when one writer writes to buffer, other writers are blocked 
 *
 *  @see    ITCMonitor
*/
class ITCBoundedRawFifo : public ITCMonitor {

public:
    ITCBoundedRawFifo()
    :	entrySize(0), nEntries(0), buffer(0), readPos(0), writePos(0)
    {}
    
	virtual		~ITCBoundedRawFifo()
    {
	    (void) destroy();
    }

	int create	(int ntrySize, int nNtries);
	int	destroy	();

	// return
	//	           0: if read/written
	//	WAIT_TIMEOUT: if timed out 
	int	read	(void* buffer, int nBytes, DWORD timeout);
    int readEx(void *buff,int nBytes,int nUsrEvnts,
               HANDLE *pEvents,DWORD *pWaitResult,DWORD timeout);
    //readEx - if return value == 0 read if return value == 1 look at pWaitResult !
	int	write	(void* buffer, int nBytes, DWORD timeout);

private:

	int		entrySize;		// of one entry
	int		nEntries;		// number of entries in buffer

	LPBYTE	buffer;			// fifo-buffer (size = entries * nEntries)

	int		writePos;		// for any writer
	int		readPos;		// for reader
};


//------------------------------------------------------------------*
/**
 * Creates resp. allocates the raw byte oriented FIFO buffer. 
 * The byte size of the buffer will be determined from the 
 * parameter ntrySize - size of one entry and parameter nNtries -
 * number of entries. Then create allocates the corresponding buffer
 * from the heap.
 *
 * @param           ntrySize - size of one entry (use sizeof operator)
 * @param           nNtries - number of entries in the FIFO buffer.
 * @return          0 - success , -1 - out of memory condition
 * @exception       -
 * @see             
*/
inline int ITCBoundedRawFifo::create(int ntrySize, int nNtries)
{
    //nNtries=1;
	ITC_ASSERT(ntrySize > 0); ITC_ASSERT(nNtries > 0);

	int	fail = 0;

	if ((fail = ITCMonitor::create()) == 0) { 
		if ((buffer = new BYTE [ntrySize * nNtries]) == 0) {
			ITC_ASSERT(buffer);
			fail = -1;
		} else {
			entrySize = ntrySize;
			nEntries = nNtries;
		}
	}
	return fail;
}

//------------------------------------------------------------------*
/**
 * Cleans up the resources occupied by the ITCBoundedRawFifo. The
 * buffer being created with create will be deallocated (delete).
 * and the entrySize, nEntries members will be set to zero. After
 * destroy access to the ITCBoundedRawFifo is no longer allowed.
 * Destroys the ITCMonitor super class as well.
 *
 * @see create.
*/
inline int ITCBoundedRawFifo::destroy()
{
	int	fail = 0;

	if (buffer) {
		delete [] buffer;
		buffer = 0;
		entrySize = nEntries = 0;
		readPos = writePos = 0;
	}
	ITCMonitor::destroy(); 
	return fail;
}

//------------------------------------------------------------------*
/**
 *  protected read from raw byte oriented buffer if NonEmpty and IsIdle 
 *  signalled.
 *  Semaphore NonEmpty - buffer is not empty
 *  Semaphore IsIdle  -  no one's in read/write process to the buffer.
 *  If read detects that the FIFO buffer is empty it sets the empty flag
 *  of the super class ITCMonitor and in turn NonEmpty will be reset (=0).
 *
 * @param           buff - points to the memory area to receive entry from buffer.
 * @param           nBytes - number of bytes must be less or equal to the entrySize.
 * @param           timeout - milliseconds to wait for a access grant.
 * @return          0 - data could be written - success
 *                  WAIT_TIMEOUT - no write access in the specified timeout.
 *
 * @see       readEx      
*/
inline int	ITCBoundedRawFifo::read(void* buff, int nBytes, DWORD timeout)
{
    int fail;
	ITC_ASSERT(entrySize >= nBytes);
    ITC_ASSERT(buffer);

    if ((fail = enterReader(timeout)) == 0) {
        memcpy(buff, &buffer[readPos * entrySize],nBytes);
        if(setNonFull())
            ITC_ASSERT("setNonFull failed");
        if ((readPos = ++readPos % nEntries) == writePos) {
            setEmpty();
        }
		if(leave())
            ITC_ASSERT("leave failed");

	}

	return fail;
}

//------------------------------------------------------------------*
/**
 *  protected read from raw byte oriented buffer if NonEmpty and IsIdle 
 *  signalled or aborts the read operation if one of the user events 
 *  is signalled.
 *  Semaphore NonEmpty - buffer is not empty
 *  Semaphore IsIdle  -  no one's in read/write process to the buffer.
 *  In case that one of the user events is signalled the read operation
 *  won't be performed. Rather the method returns immediately with the
 *  event indication in parameter pWaitResult.
 *
 *  If none of the user events is signalled but NonEmpty and IsIdle are
 *  signalled the read operation will be performed.
 *  If read detects that the FIFO buffer is empty it sets the empty flag
 *  of the super class ITCMonitor and in turn NonEmpty will be reset (=0).
 *
 * @param           buff - points to the memory area to receive entry from buffer.
 * @param           nBytes - number of bytes must be less or equal to the entrySize.
 * @param           nUsrEvnts - number of user events that make up the
 * @param           additional wait condition (see above)
 * @param           pEvents - reference to nUserEvnts event handles.
 * @param           pWaitResult - returns the user event that is signalled
 *                  in case that return value == 1 (look first at the 
 *                  return value.
 * @param           timeout - milliseconds to wait for a access grant.
 * @return          0 - data could be read - success, 1 - one of the user 
 *                  events signalled - look at pWaitResult.
 *                  WAIT_TIMEOUT - no write access in the specified timeout.
 *
 * @see       read()
*/
inline int ITCBoundedRawFifo::readEx(void *buff,int nBytes,int nUsrEvnts,
                              HANDLE *pEvents,DWORD *pWaitResult,DWORD timeout)
{
    int fail;
	ITC_ASSERT(entrySize >= nBytes);
    ITC_ASSERT(buffer);

    if((fail = enterReaderEx(nUsrEvnts,pEvents,pWaitResult,timeout)) != 0)
        return fail;//look at the result

    memcpy(buff, &buffer[readPos * entrySize], nBytes);
	if(setNonFull())
        ITC_ASSERT("setNonFull failed");
    if ((readPos = ++readPos % nEntries) == writePos) {
        setEmpty();
	}
	if(leave())
        ITC_ASSERT("leave failed");
	
    return 0;
}

//------------------------------------------------------------------*
/**
 *  protected write to raw byte oriented buffer if NonFull and IsIdle signalled.
 *  NonFull - buffer is not full
 *  IsIdle  - no one's in read/write process to the buffer.
 *
 * @param           buff - points to the data to write to the buffer
 * @param           nBytes - number of bytes must be less or equal to the entrySize.
 * @param           timeout - milliseconds to wait for a access grant.
 * @return          0 - data could be written - success
 *                  -1 - buffer of Fifo is not allocated - fail during create
 *                  WAIT_TIMEOUT - no write access in the specified timeout.
*/
inline int	ITCBoundedRawFifo::write(void* buff, int nBytes, DWORD timeout)
{   int fail;
	ITC_ASSERT(entrySize >= nBytes);
    ITC_ASSERT(buffer);

	if ((fail = enterWriter(timeout)) == 0) {
		memcpy(&buffer[writePos * entrySize], buff, nBytes);
		if(setNonEmpty())
            ITC_ASSERT("setNonEmpty failed");
		if ((writePos = ++writePos % nEntries) == readPos) {
			setFull();
		}
		if(leave())
            ITC_ASSERT("leave failed");
	}
	return fail;
}

#endif //ITC_H
/*
 *----------------------------------------------------------------------------*
 *  $History: itc.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/inc
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 3  *****************
 * User: Su           Date: 19.07.01   Time: 10:15
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 2  *****************
 * User: Su           Date: 31.01.01   Time: 11:06
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 1  *****************
 * User: Su           Date: 24.01.01   Time: 13:42
 * Created in $/4Control/COM/softing/fc/inc
 * Moved to ATCMControl V2.0 Sourcesafe data base.
 * 
 * *****************  Version 2  *****************
 * User: Su           Date: 24.01.01   Time: 13:38
 * Updated in $/4ControlMesse/COM/softing/fc/inc
 * Added comments and class descriptions
 * 
 * *****************  Version 1  *****************
 * User: Su           Date: 14.11.00   Time: 15:40
 * Created in $/4ControlMesse/COM/softing/fc/inc
 * Initial rev.
 *==
 *----------------------------------------------------------------------------*
*/
