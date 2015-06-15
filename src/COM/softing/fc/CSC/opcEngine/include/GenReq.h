//-----------------------------------------------------------------------------
//                                  OPC ENGINE                                |
//                                                                            |
//  Filename    : GENREQ.H                                                    |
//  Version     : 2.01.0.00.release                                           |
//  Date        : 7-May-1999                                                  |
//                                                                            |
//  Description : I/O Request classes                                         |
//                - GenericRequest: base class for I/O requests               |
//                                                                            |
//  CHANGE_NOTES                                                              |
//                                                                            |
//  date      name      change                                                |
//  ------------------------------------------------------------------------- |
//  01/11/99  RT        added direct and constant I/O modes                   |
//  04/22/99  RT        added report request type                             |
//  04/23/99  RT        moved cyclic data form the request to the item        |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _GENREQ_H_
#define _GENREQ_H_

#include <opcda.h>
#include "GenObj.h"

//-----------------------------------------------------------------------------
// GenericRequest                                                               |
//-----------------------------------------------------------------------------

#define GENREQ_FLAG_STARTED		0x01
#define GENREQ_FLAG_PENDING		0x02
#define GENREQ_FLAG_COMPLETED	0x04
#define GENREQ_FLAG_CANCELED	0x08
#define GENREQ_FLAG_PROCESSED	0x10

class GenericRequest;
typedef CTypedPtrList<CPtrList, GenericRequest *> RequestList;
typedef CMap<DWORD, DWORD, COleVariant, LPCVARIANT> SendValuesMap;
typedef CMap<DWORD, DWORD, WORD, WORD> SendQualitiesMap;

class GenericItem;
class GenericItemTag;
class GenericCache;
class OPCTransaction;
class GenericGroup;

// Request Data (depends on I/O mode, operation and  type of the request) 
// synchron read data
typedef struct _tagGenericRequestSyncReadData
{
	OPCITEMSTATE *m_itemStateAddr;		// read output buffer (only for sync read)
} GenericRequestSyncReadData;
// write data
typedef struct _tagGenericRequestWriteData
{
	COleVariant m_writeValue;			// value to write (only for write)
} GenericRequestWriteData;
// synchron property read data
typedef struct _tagGenericRequestSyncPropertyData
{
	LPVARIANT m_readValueAddr;			// read output buffer (only for sync read property)
} GenericRequestSyncPropertyData;
// delegate I/O data
typedef struct _tagGenericRequestDelegateData
{
	GenericRequest *m_request;			// starter request (only for delegate read)
} GenericRequestDelegateData;


class OPCENGINE_EXPORT GenericRequest : public GenericObject
{
friend OPCTransaction;

public:
	// operation of request
	enum requestOperation
	{
		read,				// read request
		write				// write request
	};

	// type of request
	enum requestType
	{
		syncCache,			// synchron cache 
		syncDevice,			// synchron device
		asyncCache,			// asynchron cache
		asyncDevice,		// asynchron device
		cyclic,				// cyclic device 
		syncProperty,		// synchron property
		delegatedDevice,	// delegated device
		report				// report
	};

	// device I/O mode of request 
	enum requestDeviceIOMode
	{
		direct = 1,			// direct I/O
		queue,				// queued I/O
		constant,			// constant value
		delegate			// delegated request
	};

	// request state
	enum requestState
	{
		stateInitialized = 0,
		stateStarted	 = GENREQ_FLAG_STARTED,
		statePending	 = GENREQ_FLAG_STARTED | GENREQ_FLAG_PENDING,
		stateCompleted	 = GENREQ_FLAG_STARTED | GENREQ_FLAG_COMPLETED,
		stateCanceled	 = GENREQ_FLAG_STARTED | GENREQ_FLAG_COMPLETED | GENREQ_FLAG_CANCELED,	
		stateProcessed	 = GENREQ_FLAG_STARTED | GENREQ_FLAG_COMPLETED | GENREQ_FLAG_PROCESSED,
		stateProcessedC	 = GENREQ_FLAG_STARTED | GENREQ_FLAG_COMPLETED | GENREQ_FLAG_PROCESSED | GENREQ_FLAG_CANCELED
	};

	GenericRequest(IN BYTE operation, IN BYTE type, IN BYTE ioMode);

	// request type
	BOOL setType(IN enum requestType requestType);
	enum requestType getType(void) const;
	BOOL isCacheRequest(void) const;
	BOOL isDeviceRequest(void) const;

	// request operation
	BOOL setOperation(IN enum requestOperation Operation);
	enum requestOperation getOperation(void) const;

	// request I/O mode
	BOOL setDeviceIOMode(IN enum requestDeviceIOMode ioMode);
	enum requestDeviceIOMode getDeviceIOMode(void) const;

	// item object
	virtual BOOL setItem(IN GenericItem *item);
	GenericItem *getItem(void);

	// tag object
	virtual BOOL setItemTag(IN GenericItemTag *itemTag);
	GenericItemTag *getItemTag(void);

	// cache object
	GenericCache *getCache(void);

	// start
	virtual BOOL start(void);
	BOOL isStarted(void) const;

	// complete
	virtual BOOL complete(IN OPTIONAL HRESULT result = S_OK, 
			IN OPTIONAL BOOL validWriteQuality = FALSE,
			IN OPTIONAL WORD writeQuality = OPC_QUALITY_UNCERTAIN,
			IN OPTIONAL BOOL errorBeforeStart = FALSE);
	BOOL isCompleted(void) const;

	// pending 
	virtual BOOL pending(void);
	BOOL isPending(void) const;

	// cancel
	virtual BOOL cancel(IN OPTIONAL HRESULT compResult = E_FAIL,
			IN OPTIONAL BOOL changeCacheQuality = FALSE,
			IN OPTIONAL WORD cacheQuality = OPC_QUALITY_LAST_KNOWN);
	BOOL isCanceled(void) const;

	// reset request object state and values
	void reset(void);

	// start time
	DWORD getStartTime(void) const;

	// time span between start and completion
	ULONG getCompletionSpan(void) const;

	// user name
	BOOL setUser(IN LPCTSTR user);
	CString getUser(void) const;

	// result of request
	void setResultAddress(IN HRESULT *resAddr);
	HRESULT getResult(void);
	void setResult(HRESULT res);

	// read buffer (only used for sync read operation)
	void setItemStateAddress(OPCITEMSTATE *itemStateAddr);
	OPCITEMSTATE *getItemStateAddress(void);

	// read buffer (only used for sync property read operation)
	void setReadValueAddress(LPVARIANT readValueAddr);
	LPVARIANT getReadValueAddress(void);

	// write buffer (only used for write operation)
	BOOL setWriteValue(IN LPCVARIANT value);
	BOOL getWriteValue(OUT LPVARIANT value);

	// completed event
	HANDLE getCompletedEvent(void);
	void setCompletedEvent(HANDLE event);

	// last value send to the OPC client
	BOOL setSendValue(IN DWORD connection, IN LPCVARIANT value, IN WORD quality);
	BOOL getSendValue(IN DWORD connection, OUT LPVARIANT value, OUT WORD *quality);
	BOOL hasChanged(IN DWORD connection,	IN LPVARIANT value,
	IN WORD quality);	// change between last value and cache

	// object trace id
	virtual LPCTSTR getObjTraceId(void);

protected:
	~GenericRequest();	

	BYTE m_type;						// request type
	BYTE m_operation;					// request operation 
	BYTE m_deviceIOMode;				// request device I/O mode
	BYTE m_flag;
	GenericItem *m_item;				// item object
	GenericItemTag *m_itemTag;			// tag object
	DWORD m_startTime;					// start time in ms since system start
	ULONG m_completionSpan;				// time span between start and completion
	HRESULT m_result;					// result of request
	HANDLE m_completedEvent;			// event signals when request is completed
	HRESULT *m_resAddr;					// address of result buffer (only for sync)

	void *m_depData;					// type and operation dependend data

	// create completed event
	BOOL createCompletedEvent(void);

	// statistics
	void doStatistics(void);

	GenericRequestSyncReadData *getSyncReadData(void);
	GenericRequestSyncPropertyData *getSyncPropertyData(void);
	GenericRequestWriteData *getWriteData(void);

	BOOL setDelegateData(GenericRequestDelegateData *delegData);
	GenericRequestDelegateData *getDelegateData(void);
	virtual GenericRequest *createDelegateRequest(void);
	virtual BOOL completeDelegateRequest(void);

	CCriticalSection *getRequestStateLock(void);
}; // GenericRequest


inline BOOL GenericRequest::setType(IN enum GenericRequest::requestType requestType)
{ m_type = (BYTE) requestType; return TRUE; }

inline enum GenericRequest::requestType GenericRequest::getType(void) const
{ return (GenericRequest::requestType) m_type; }

inline BOOL GenericRequest::isCacheRequest(void) const
{ return ( (m_type == GenericRequest::syncCache)  || 
		   (m_type == GenericRequest::asyncCache) ); }

inline BOOL GenericRequest::isDeviceRequest(void) const
{ return ( (m_type == GenericRequest::syncDevice)  || 
		   (m_type == GenericRequest::asyncDevice) || 
		   (m_type == GenericRequest::delegatedDevice) || 
		   (m_type == GenericRequest::syncProperty) || 
		   (m_type == GenericRequest::cyclic) ); }

inline BOOL GenericRequest::setOperation(IN enum GenericRequest::requestOperation operation)
{ m_operation = (BYTE)operation; return TRUE; }

inline enum GenericRequest::requestOperation GenericRequest::getOperation(void) const
{ return (GenericRequest::requestOperation) m_operation; }

inline BOOL GenericRequest::setDeviceIOMode(IN enum GenericRequest::requestDeviceIOMode ioMode)
{ m_deviceIOMode = ioMode; return TRUE; }

inline enum GenericRequest::requestDeviceIOMode GenericRequest::getDeviceIOMode(void) const
{ return (GenericRequest::requestDeviceIOMode) m_deviceIOMode; }

inline BOOL GenericRequest::isStarted(void) const
{ return (GENREQ_FLAG_STARTED == (getObjectState() & GENREQ_FLAG_STARTED)); }

inline BOOL GenericRequest::isCompleted(void) const
{ return (GENREQ_FLAG_COMPLETED == (getObjectState() & GENREQ_FLAG_COMPLETED)); }

inline BOOL GenericRequest::isPending(void) const
{ return (GENREQ_FLAG_PENDING == (getObjectState() & GENREQ_FLAG_PENDING)); }

inline BOOL GenericRequest::isCanceled(void) const
{ return (GENREQ_FLAG_CANCELED == (getObjectState() & GENREQ_FLAG_CANCELED)); }

inline DWORD GenericRequest::getStartTime(void) const
{ return m_startTime; }

inline ULONG GenericRequest::getCompletionSpan(void) const
{ return m_completionSpan; }

inline BOOL GenericRequest::setUser(IN LPCTSTR /* user */)
{ return TRUE; }

inline CString GenericRequest::getUser(void) const
{ return CString(_T("")); }

inline void GenericRequest::setResultAddress(HRESULT *resAddr)
{ m_resAddr = resAddr; }

inline HRESULT GenericRequest::getResult(void)
{ if (m_resAddr) return *m_resAddr; else return m_result; }

inline void GenericRequest::setResult(HRESULT res)
{ if (m_resAddr) *m_resAddr = res; else m_result = res; }

inline HANDLE GenericRequest::getCompletedEvent(void)
{ return m_completedEvent; }

inline void GenericRequest::setCompletedEvent(HANDLE event)
{ m_completedEvent = event; }

inline GenericRequestSyncReadData *GenericRequest::getSyncReadData(void)
{ if ((m_type == syncCache) || ((m_type == syncDevice) && (m_operation == read))) return (GenericRequestSyncReadData *)m_depData; else return NULL; }

inline GenericRequestSyncPropertyData *GenericRequest::getSyncPropertyData(void)
{ if ((m_type == syncProperty) && (m_operation == read)) return (GenericRequestSyncPropertyData *)m_depData; else return NULL; }

inline GenericRequestWriteData *GenericRequest::getWriteData(void)
{ if (m_operation == write) return (GenericRequestWriteData *)m_depData; else return NULL; }

inline GenericRequestDelegateData *GenericRequest::getDelegateData(void)
{ if (m_type == delegatedDevice) return (GenericRequestDelegateData *)m_depData; else return NULL; }

#endif

