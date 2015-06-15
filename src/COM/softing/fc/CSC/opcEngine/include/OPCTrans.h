//-----------------------------------------------------------------------------
//                                  OPC ENGINE                                |
//                                                                            |
//  Filename    : OPCTRANS.H                                                  |
//  Version     : 2.01.0.00.release                                           |
//  Date        : 7-May-1999                                                  |
//                                                                            |
//  Description : Classes for asynchonous OPC transactions                    |
//                - OPCTransaction: OPC transaction                           |
//                                                                            |
//  CHANGE_NOTES                                                              |
//                                                                            |
//  date      name      change                                                |
//  ------------------------------------------------------------------------- |
//  12/23/98  RT        support OPC V2.0 connections                          |
//  04/22/99  RT        added report transaction type                         |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _OPCTRANS_H_
#define _OPCTRANS_H_

#include "GenObj.h"

//-----------------------------------------------------------------------------
// OPCTransaction                                                             |
//-----------------------------------------------------------------------------

class OPCTransaction;
typedef CTypedPtrList<CPtrList, OPCTransaction *> TransactionList;

class OPCGroup;
class GenericServer;
class GenericGroup;
class GenericRequest;

class OPCENGINE_EXPORT OPCTransaction : public GenericObject
{
friend GenericServer;
friend GenericGroup;
friend OPCGroup;

public:
	// type of OPC transaction
	enum transactionType
	{
		read,		// read transaction
		write,		// write transaction
		refresh,	// refresh transaction
		cyclic,		// cyclic read transaction
		report		// cyclic report transaction
	};

	// type of client connections
	enum connectionType
	{
		dataObject,			// IDataObject (OPC V1.0)
		connectionPoint,	// IConnectionPoint (OPC V2.0)
		allConnections		// all connections
	};

	// server transaction id (== cancel id (OPC V2.0))
	DWORD getTransactionId(void);

	// client transaction id
	DWORD getClientTransactionId(void);
	void setClientTransactionId(DWORD transactionId);

	// transaction type
	transactionType getType(void);
	BOOL isCyclic(void);

	// generic group of transaction
	GenericGroup *getGroup(void);

	// connection of transaction (0 for cyclic transactions)
	DWORD getConnection(void);

	// request list
	GenericList<GenericRequest> *getRequestList(void);

	// request list handling
	BOOL insertRequest(IN GenericRequest *req);
	BOOL removeRequest(IN GenericRequest *req);

	GenericRequest *getRequestByItem(IN GenericItem *srcItem);
	DWORD getRequestCount(IN OPTIONAL BOOL doLock = TRUE, 
		IN OPTIONAL BOOL onlyChanged = FALSE, IN OPTIONAL DWORD connection = 0);

	// actions on request list
	void startRequests(void);
	void cancelRequests(void);

	// actions on transaction
	BOOL update(IN DWORD now, IN DWORD dueTime, OUT DWORD *span);
	void complete(void);
	HRESULT cancel(void);

	// complete all report requests
	BOOL completeReportRequests(void);

	// object trace id
	virtual LPCTSTR getObjTraceId(void);

protected:
	OPCTransaction(IN transactionType type, IN GenericGroup *group,
		IN BYTE connectionType, IN OPTIONAL DWORD connection = 0);
	virtual ~OPCTransaction(void);

	GenericList<GenericRequest> m_reqList;// request list
	BYTE m_type;						// transaction type
	BYTE m_started;						// started requests of transaction
	BYTE m_connectionType;				// type of client connections
	DWORD m_connection;					// connection id
	GenericGroup *m_group;				// generic group object

	DWORD m_transactionId;				// transaction id
	DWORD m_clientTransId;				// clients transaction id (OPC V2.0)

	// server transaction id (== cancel id (OPC V2.0))
	void setTransactionId(DWORD transactionId);

	// create OPC data V1.0
	HRESULT createReadData(IN CLIPFORMAT format, IN DWORD connection, 
		OUT HANDLE *memory);
	HRESULT createWriteData(OUT HANDLE *memory);

	// create OPC data V2.0
	HRESULT sendReadData(IN IOPCDataCallback *dataCallback);
	HRESULT sendWriteData(IN IOPCDataCallback *dataCallback);

}; // OPCTransaction


inline DWORD OPCTransaction::getTransactionId(void)
{ return m_transactionId; }

inline DWORD OPCTransaction::getConnection(void)
{ return m_connection; }

inline void OPCTransaction::setTransactionId(DWORD transactionId)
{ m_transactionId = transactionId; }

inline DWORD OPCTransaction::getClientTransactionId(void)
{ return m_clientTransId; }

inline void OPCTransaction::setClientTransactionId(DWORD transactionId)
{ m_clientTransId = transactionId; }

inline BOOL OPCTransaction::isCyclic(void)
{ return (m_type == cyclic); }

inline OPCTransaction::transactionType OPCTransaction::getType(void)
{ return (OPCTransaction::transactionType)m_type; }

inline GenericList<GenericRequest> *OPCTransaction::getRequestList(void) 
{ return &m_reqList; }

#endif

