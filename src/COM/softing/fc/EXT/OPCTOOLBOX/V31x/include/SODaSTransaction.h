//-----------------------------------------------------------------------------
//                                                                            |



//                                                                            |



//                                                                            |
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                           OPC TOOLBOX - SODaS                              |
//                                                                            |
//  Filename    : SODaSTransaction.h                                          |
//  Version     : 3.10.release                                                |
//  Date        : 4-March-2002                                                |
//                                                                            |
//  Description : Class for asynchonous transactions                          |
//                - SODaSTransaction: transaction                             |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _SODASTRANSACTION_H_
#define _SODASTRANSACTION_H_

#include "SOCmnObject.h"

#pragma pack(push,4)

//-----------------------------------------------------------------------------
// SODaSTransaction                                                           |
//-----------------------------------------------------------------------------

class SODaSComGroup;
class SODaSServer;
class SODaSGroup;
class SODaSRequest;
class SODaSDataCallback;

class SODAS_EXPORT SODaSTransaction : public SOCmnObject
{
friend SODaSServer;
friend SODaSGroup;
friend SODaSComGroup;

public:
	// type of OPC transaction
	enum transactionType
	{
		read,		// read transaction
		write,		// write transaction
		refresh,	// refresh transaction
		cyclic		// cyclic transaction
	};

	// type of client connections
	enum connectionType
	{
		dataObject,			// IDataObject (OPC V1.0)
		connectionPoint,	// IConnectionPoint (OPC V2.0)
		allConnections		// all connections
	};

	SODaSTransaction(IN SODaSGroup *group, IN BYTE type, 
		IN BYTE connectionType, IN OPTIONAL DWORD connection = 0);

	// server transaction id (== cancel id (OPC V2.0))
	DWORD getTransactionId(void);

	// client transaction id
	DWORD getClientTransactionId(void);
	void setClientTransactionId(DWORD transactionId);

	// transaction type
	transactionType getType(void);
	BOOL isCyclic(void);

	// generic group of transaction
	SODaSGroup *getGroup(void);

	// connection of transaction (0 for cyclic transactions)
	DWORD getConnection(void);

	// request list
	SOCmnList<SODaSRequest> *getRequestList(void);

	// request list handling
	BOOL insertRequest(IN SODaSRequest *req);
	BOOL removeRequest(IN SODaSRequest *req);

	SODaSRequest *getRequestByItem(IN SODaSItem *srcItem);
	DWORD getRequestCount(void);

	// actions on transaction
	virtual void start(void);
	virtual BOOL update(IN DWORD now, IN DWORD dueTime, OUT DWORD *span);
	virtual void complete(void);
	virtual void cancel(void);

	// actions on request list
	virtual void handleRequests(IN SOCmnList<SODaSRequest> *reqList);
	void cancelRequests(void);

	// object trace id
	virtual LPCTSTR getObjTraceId(void);

	BOOL isStarted(void) { return m_started; }

protected:
	virtual ~SODaSTransaction(void);

	SOCmnList<SODaSRequest> m_reqList;	// request list
	BYTE m_type;						// transaction type
	BYTE m_started;						// started requests of transaction
	BYTE m_connectionType;				// type of client connections
	DWORD m_connection;					// connection id
	SODaSGroup *m_group;				// generic group object

	DWORD m_transactionId;				// transaction id
	DWORD m_clientTransId;				// clients transaction id (OPC V2.0)

	// server transaction id (== cancel id (OPC V2.0))
	void setTransactionId(DWORD transactionId);

	// send OPC data to the client
	HRESULT sendReadData(IN SODaSDataCallback *advise);
	HRESULT sendWriteData(IN SODaSDataCallback *advise);
}; // SODaSTransaction


inline DWORD SODaSTransaction::getTransactionId(void)
{ return m_transactionId; }

inline DWORD SODaSTransaction::getConnection(void)
{ return m_connection; }

inline void SODaSTransaction::setTransactionId(DWORD transactionId)
{ m_transactionId = transactionId; }

inline DWORD SODaSTransaction::getClientTransactionId(void)
{ return m_clientTransId; }

inline void SODaSTransaction::setClientTransactionId(DWORD transactionId)
{ m_clientTransId = transactionId; }

inline BOOL SODaSTransaction::isCyclic(void)
{ return (m_type == cyclic); }

inline SODaSTransaction::transactionType SODaSTransaction::getType(void)
{ return (SODaSTransaction::transactionType)m_type; }

inline SOCmnList<SODaSRequest> *SODaSTransaction::getRequestList(void) 
{ return &m_reqList; }

inline DWORD SODaSTransaction::getRequestCount(void)
{ SOCmnList<SODaSRequest> reqList(getRequestList()); return reqList.getCount(); }

#define OPCTransaction SODaSTransaction

#pragma pack(pop)
#endif

