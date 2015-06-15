//-----------------------------------------------------------------------------
//                                                                            |



//                                                                            |



//                                                                            |
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                           OPC TOOLBOX - SODaS                              |
//                                                                            |
//  Filename    : SODaSRequest.h                                              |
//  Version     : 3.10.release                                                |
//  Date        : 4-March-2002                                                |
//                                                                            |
//  Description : I/O Request classes                                         |
//                - SODaSRequest: base class for I/O requests                 |
//                - SODaSRequestSyncReadData: requests synchron read data     |
//                - SODaSRequestSyncWriteData: requests synchron write data   |
//                - SODaSRequestAsyncWriteData: requests asynchron write data |
//                - SODaSRequestSyncReadPropertyData: requests synchron	      |
//                                                    property read data      |
//                - SODaSRequestDelegateData: requests delegated data         |
//                - SODaSRequestReportData: requests report data              |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _SODASREQUEST_H_
#define _SODASREQUEST_H_

#include <opcda.h>
#include "SOCmnObject.h"

#pragma pack(push,4)

//-----------------------------------------------------------------------------
// Request Data (depends on I/O mode, operation and  type of the request) 

class SODaSItemTag;
class SODaSRequest;
class SODaSCache;


//-----------------------------------------------------------------------------
// SODaSRequestSyncReadData                                                   |
//-----------------------------------------------------------------------------
// synchron read data
class SODaSRequestSyncReadData
{
public:
	SODaSRequestSyncReadData(void);
	~SODaSRequestSyncReadData(void);
	
	// result of request
	void setResultAddress(IN HRESULT *resAddr);
	HRESULT getResult(void);
	void setResult(HRESULT res);

	// read buffer (only used for sync read operation)
	void setItemStateAddress(OPCITEMSTATE *itemStateAddr);
	OPCITEMSTATE *getItemStateAddress(void);

protected:
	HRESULT *m_resAddr;					// address of result buffer (only for sync)
	OPCITEMSTATE *m_itemStateAddr;		// read output buffer (only for sync read)
}; // SODaSRequestSyncReadData


//-----------------------------------------------------------------------------
// SODaSRequestSyncWriteData                                                  |
//-----------------------------------------------------------------------------
// synchron write data
class SODaSRequestSyncWriteData
{
public:
	SODaSRequestSyncWriteData(void);
	~SODaSRequestSyncWriteData(void);

	// result of request
	void setResultAddress(IN HRESULT *resAddr);
	HRESULT getResult(void);
	void setResult(HRESULT res);

	// write buffer (only used for write operation)
	BOOL setWriteValue(IN LPCVARIANT value, IN LCID localeID, IN SODaSItemTag *tag);
	BOOL getWriteValue(OUT LPVARIANT value);

protected:
	HRESULT *m_resAddr;					// address of result buffer (only for sync)
	SOCmnVariant m_writeValue;			// value to write (only for write)
}; // SODaSRequestSyncWriteData


//-----------------------------------------------------------------------------
// SODaSRequestAsyncWriteData                                                 |
//-----------------------------------------------------------------------------
// asynchron write data
class SODaSRequestAsyncWriteData
{
public:
	SODaSRequestAsyncWriteData(void);
	~SODaSRequestAsyncWriteData(void);

	// result of request
	HRESULT getResult(void);
	void setResult(HRESULT res);

	// write buffer (only used for write operation)
	BOOL setWriteValue(IN LPCVARIANT value, IN LCID localeID, IN SODaSItemTag *tag);
	BOOL getWriteValue(OUT LPVARIANT value);

protected:
	HRESULT m_result;					// result of request
	SOCmnVariant m_writeValue;			// value to write (only for write)
}; // SODaSRequestAsyncWriteData


//-----------------------------------------------------------------------------
// SODaSRequestSyncReadPropertyData                                           |
//-----------------------------------------------------------------------------
// synchron property read data
class SODaSRequestSyncReadPropertyData
{
public:
	SODaSRequestSyncReadPropertyData(void);
	~SODaSRequestSyncReadPropertyData(void);

	// result of request
	void setResultAddress(IN HRESULT *resAddr);
	HRESULT getResult(void);
	void setResult(HRESULT res);

	// read buffer (only used for sync property read operation)
	void setReadValueAddress(LPVARIANT readValueAddr);
	LPVARIANT getReadValueAddress(void);

	// tag object
	BOOL setItemTag(IN SODaSItemTag *itemTag);
	SODaSItemTag *getItemTag(void);

	// cache object
	SODaSCache *getCache(void);

protected:
	HRESULT *m_resAddr;					// address of result buffer (only for sync)
	SODaSItemTag *m_itemTag;			// property object
	LPVARIANT m_readValueAddr;			// read output buffer (only for sync read property)
}; // SODaSRequestSyncReadPropertyData


//-----------------------------------------------------------------------------
// SODaSRequestDelegateData                                                   |
//-----------------------------------------------------------------------------
// delegate I/O data
class SODaSRequestDelegateData
{
public:
	SODaSRequestDelegateData(void);
	~SODaSRequestDelegateData(void);

	// result of request
	HRESULT getResult(void);
	void setResult(HRESULT res);

	// original request (only used for delegate)
	BOOL setOriginalRequest(IN SODaSRequest *req);
	SODaSRequest *getOriginalRequest(void);

	// tag object
	BOOL setItemTag(IN SODaSItemTag *itemTag);
	SODaSItemTag *getItemTag(void);

	// cache object
	SODaSCache *getCache(void);

protected:
	HRESULT m_result;					// result of request
	SODaSItemTag *m_itemTag;			// tag object
	SODaSRequest *m_request;			// starter request (only for delegate read)
}; // SODaSRequestDelegateData


//-----------------------------------------------------------------------------
// SODaSRequestReportData                                                     |
//-----------------------------------------------------------------------------
// report I/O data
class SODaSRequestReportData
{
public:
	SODaSRequestReportData(void);
	~SODaSRequestReportData(void);

	// result of request
	HRESULT getResult(void) { return m_result; }
	void setResult(HRESULT res) { m_result = res; }

	// report value, quality and time stamp (only used for report requests)
	BOOL setReportValue(IN LPCVARIANT value) { if (value) { m_value = value; return TRUE; } return FALSE; }
	BOOL getReportValue(OUT LPVARIANT value) { if (value) { m_value.copyTo(value); return TRUE; } return FALSE; }
	void setReportQuality(IN WORD quality) { m_quality = quality; }
	WORD getReportQuality(void) { return m_quality; }
	BOOL setReportTimeStamp(IN LPFILETIME timeStamp) { if (timeStamp) { m_timeStamp.dwLowDateTime = timeStamp->dwLowDateTime; m_timeStamp.dwHighDateTime = timeStamp->dwHighDateTime; return TRUE; } return FALSE; }
	BOOL getReportTimeStamp(OUT LPFILETIME timeStamp) { if (timeStamp) { timeStamp->dwLowDateTime = m_timeStamp.dwLowDateTime; timeStamp->dwHighDateTime = m_timeStamp.dwHighDateTime; return TRUE; } return FALSE; }

protected:
	HRESULT m_result;     // result of request
	FILETIME m_timeStamp; // time stamp
	SOCmnVariant m_value; // value
	WORD m_quality;       // quality
}; // SODaSRequestReportData




//-----------------------------------------------------------------------------
// SODaSRequest                                                               |
//-----------------------------------------------------------------------------

#define SODASREQUEST_FLAG_STARTED   0x01
#define SODASREQUEST_FLAG_PENDING   0x02
#define SODASREQUEST_FLAG_COMPLETED 0x04
#define SODASREQUEST_FLAG_CANCELED  0x08
#define SODASREQUEST_FLAG_PROCESSED 0x10
#define SODASREQUEST_FLAG_TOPROCESS 0x20

#define SODASREQUEST_TYPE_SYNC		0x04
#define SODASREQUEST_TYPE_ASYNC		0x08
#define SODASREQUEST_TYPE_DEVICE	0x01
#define SODASREQUEST_TYPE_CACHE		0x02
#define SODASREQUEST_TYPE_ITEM		0x10
#define SODASREQUEST_TYPE_PROPERTY	0x20
#define SODASREQUEST_TYPE_CYCLIC	0x80
#define SODASREQUEST_TYPE_REPORT	0x40

class SODaSRequest;
class SODaSItem;
class SODaSItemTag;
class SODaSCache;
class SODaSTransaction;
class SODaSGroup;
class SODaSServer;

class SODAS_EXPORT SODaSRequest : 
	public SOCmnObject
{
friend SODaSTransaction;

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
		// synchron cache 
		syncCache		= SODASREQUEST_TYPE_SYNC | SODASREQUEST_TYPE_CACHE | SODASREQUEST_TYPE_ITEM,			
		// synchron device
		syncDevice		= SODASREQUEST_TYPE_SYNC | SODASREQUEST_TYPE_DEVICE | SODASREQUEST_TYPE_ITEM,			
		// asynchron cache
		asyncCache		= SODASREQUEST_TYPE_ASYNC | SODASREQUEST_TYPE_CACHE | SODASREQUEST_TYPE_ITEM,
		// asynchron device
		asyncDevice		= SODASREQUEST_TYPE_ASYNC | SODASREQUEST_TYPE_DEVICE | SODASREQUEST_TYPE_ITEM,
		// cyclic device 
		cyclic			= SODASREQUEST_TYPE_ASYNC | SODASREQUEST_TYPE_DEVICE | SODASREQUEST_TYPE_ITEM | SODASREQUEST_TYPE_CYCLIC,
		// synchron property
		syncProperty	= SODASREQUEST_TYPE_SYNC | SODASREQUEST_TYPE_DEVICE | SODASREQUEST_TYPE_PROPERTY,		
		// delegated device	
		delegatedDevice = SODASREQUEST_TYPE_ASYNC | SODASREQUEST_TYPE_DEVICE | SODASREQUEST_TYPE_PROPERTY,
		// report
		report			= SODASREQUEST_TYPE_ASYNC | SODASREQUEST_TYPE_DEVICE |  SODASREQUEST_TYPE_ITEM | SODASREQUEST_TYPE_REPORT,
	};

	// request state
	enum requestState
	{
		stateInitialized = 0,
		stateStarted	 = SODASREQUEST_FLAG_STARTED,
		statePending	 = SODASREQUEST_FLAG_STARTED | SODASREQUEST_FLAG_PENDING,
		stateCompleted	 = SODASREQUEST_FLAG_STARTED | SODASREQUEST_FLAG_COMPLETED,
		stateCanceled	 = SODASREQUEST_FLAG_STARTED | SODASREQUEST_FLAG_COMPLETED | SODASREQUEST_FLAG_CANCELED,
		stateProcessed	 = SODASREQUEST_FLAG_STARTED | SODASREQUEST_FLAG_COMPLETED | SODASREQUEST_FLAG_PROCESSED,
		stateProcessedCancel	 = SODASREQUEST_FLAG_STARTED | SODASREQUEST_FLAG_COMPLETED | SODASREQUEST_FLAG_PROCESSED
	};

	SODaSRequest(IN BYTE operation, IN BYTE type);

	// request type
	BOOL setType(IN enum requestType requestType);
	enum requestType getType(void) const;
	BOOL isCacheRequest(void) const;
	BOOL isDeviceRequest(void) const;

	// request operation
	BOOL setOperation(IN enum requestOperation Operation);
	enum requestOperation getOperation(void) const;

	// item object
	virtual BOOL setItem(IN SODaSItem *item);
	SODaSItem *getItem(void);

	// tag object
	virtual BOOL setItemTag(IN SODaSItemTag *itemTag);
	virtual SODaSItemTag *getItemTag(void);

	// cache object
	virtual SODaSCache *getCache(void);

	// server object
	virtual SODaSServer *getServer(void);

	// start
	virtual BOOL start(IN OPTIONAL BOOL signalQueue = TRUE);
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

	// result of request
	virtual void setResultAddress(IN HRESULT *resAddr);
	virtual HRESULT getResult(void);
	virtual void setResult(HRESULT res);

	// read buffer (only used for sync read operation)
	virtual void setItemStateAddress(OPCITEMSTATE *itemStateAddr);
	virtual OPCITEMSTATE *getItemStateAddress(void);

	// read buffer (only used for sync property read operation)
	virtual void setReadValueAddress(LPVARIANT readValueAddr);
	virtual LPVARIANT getReadValueAddress(void);

	// write buffer (only used for write operation)
	virtual BOOL setWriteValue(IN LPCVARIANT value);
	virtual BOOL getWriteValue(OUT LPVARIANT value);

	// original request (only used for delegate)
	virtual BOOL setOriginalRequest(IN SODaSRequest *req);
	virtual SODaSRequest *getOriginalRequest(void);

	// report value, quality and time stamp (only used for report requests)
	virtual BOOL setReportValue(IN LPCVARIANT value);
	virtual BOOL getReportValue(OUT LPVARIANT value);
	virtual BOOL setReportQuality(IN WORD quality);
	virtual WORD getReportQuality(void);
	virtual BOOL setReportTimeStamp(IN LPFILETIME timeStamp);
	virtual BOOL getReportTimeStamp(OUT LPFILETIME timeStamp);

	// completed event
	HANDLE getCompletedEvent(void);
	void setCompletedEvent(HANDLE event);

	// change between last value and cache
	virtual BOOL hasChanged(IN DWORD connection, IN LPVARIANT value, IN WORD quality);	

	// get the locale id to use for the request
	LCID getLCID(void);

	// object trace id
	virtual LPCTSTR getObjTraceId(void);

	static void signalQueueEvents(IN SOCmnList<SODaSRequest> *reqList);

protected:
	virtual ~SODaSRequest();	

	BYTE m_type;						// request type
	BYTE m_operation;					// request operation 
	BYTE m_flag;						// internal flag
	SODaSItem *m_item;					// item object
	DWORD m_startTime;					// start time in ms since system start
	HANDLE m_completedEvent;			// event signals when request is completed
	union
	{
	HRESULT m_result;	// result 
	void *m_depData;	// type and operation dependend data 
	SODaSRequestSyncReadData *m_depSyncReadData;
	SODaSRequestSyncWriteData *m_depSyncWriteData;
	SODaSRequestAsyncWriteData *m_depAsyncWriteData;
	SODaSRequestSyncReadPropertyData *m_depSyncReadPropertyData;
	SODaSRequestDelegateData *m_depDelegateData;
	SODaSRequestReportData *m_depReportData;
	};

	// create completed event
	BOOL createCompletedEvent(void);

	virtual SODaSRequest *createDelegateRequest(void);
	virtual BOOL completeDelegateRequest(void);
}; // SODaSRequest

inline BOOL SODaSRequest::setType(IN enum SODaSRequest::requestType requestType)
{ m_type = (BYTE) requestType; return TRUE; }

inline enum SODaSRequest::requestType SODaSRequest::getType(void) const
{ return (SODaSRequest::requestType) m_type; }

inline BOOL SODaSRequest::isCacheRequest(void) const
{ return ((m_type & SODASREQUEST_TYPE_CACHE) != 0); }

inline BOOL SODaSRequest::isDeviceRequest(void) const
{ return ((m_type & SODASREQUEST_TYPE_DEVICE) != 0); }

inline BOOL SODaSRequest::setOperation(IN enum SODaSRequest::requestOperation operation)
{ m_operation = (BYTE)operation; return TRUE; }

inline enum SODaSRequest::requestOperation SODaSRequest::getOperation(void) const
{ return (SODaSRequest::requestOperation) m_operation; }

inline BOOL SODaSRequest::isStarted(void) const
{ return (SODASREQUEST_FLAG_STARTED == (getObjectState() & SODASREQUEST_FLAG_STARTED)); }

inline BOOL SODaSRequest::isCompleted(void) const
{ return (SODASREQUEST_FLAG_COMPLETED == (getObjectState() & SODASREQUEST_FLAG_COMPLETED)); }

inline BOOL SODaSRequest::isPending(void) const
{ return (SODASREQUEST_FLAG_PENDING == (getObjectState() & SODASREQUEST_FLAG_PENDING)); }

inline BOOL SODaSRequest::isCanceled(void) const
{ return (SODASREQUEST_FLAG_CANCELED == (getObjectState() & SODASREQUEST_FLAG_CANCELED)); }

inline DWORD SODaSRequest::getStartTime(void) const
{ return m_startTime; }

inline HANDLE SODaSRequest::getCompletedEvent(void)
{ return m_completedEvent; }

inline void SODaSRequest::setCompletedEvent(HANDLE event)
{ m_completedEvent = event; }

#define GenericRequest SODaSRequest

#pragma pack(pop)
#endif

