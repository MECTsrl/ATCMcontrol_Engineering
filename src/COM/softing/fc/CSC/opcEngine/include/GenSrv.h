//-----------------------------------------------------------------------------
//                                  OPC ENGINE                                |
//                                                                            |
//  Filename    : GENSRV.H                                                    |
//  Version     : 2.01.0.00.release                                           |
//  Date        : 7-May-1999                                                  |
//                                                                            |
//  Description : OPC Server object related classes                           |
//                - GenericServer: OPC server object                          |
//                - OPCRootOPC: root object of OPC runtime tree               |
//                                                                            |
//  CHANGE_NOTES                                                              |
//                                                                            |
//  date      name      change                                                |
//  ------------------------------------------------------------------------- |
//  12/02/98  RT        added IOPCShutdown access                             |
//  12/04/98  RT        added IOPCCommon support                              |
//  12/16/98  RT        added IOPCItemProperties support                      |
//  04/22/99  RT        added report driven protocol support                  |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _GENSRV_H_
#define _GENSRV_H_

#include <opcda.h>
#include "GenTree.h"
#include "GenNS.h"
#include "GenReq.h"
#include "OPCTrans.h"


//-----------------------------------------------------------------------------
// GenericServer                                                              |
//-----------------------------------------------------------------------------

class OPCServer;
class OPCGroup;
class OPCTransaction;
class GenericGroup;
class GenericItem;

class OPCENGINE_EXPORT GenericServer : public GenericBranch
{
friend OPCServer;
friend OPCGroup;
friend OPCTransaction;
friend GenericGroup;
friend GenericItem;

public:
	GenericServer(IN OPTIONAL BOOL publicGroups = FALSE, 
			IN OPTIONAL DWORD minRequestLoopTime = INFINITE,
			IN OPTIONAL BYTE whatGroupList = GENLIST_CREATE_LIST,
			IN OPTIONAL int blockSizeGroupList = 1,
			IN OPTIONAL int hashSizeGroupList = 5);

	enum methodOPCServer
	{
		// IOPCServer
		opcmAddGroup,
		opcmGetErrorString,
		opcmGetGroupByName,
		opcmGetStatus,
		opcmRemoveGroup,
		opcmCreateGroupEnumerator,
		// IOPCServerPublicGroups
		opcmGetPublicGroupByName,
		opcmRemovePublicGroup,
		// IOPCBrowseServerAddressSpace
		opcmQueryOrganization,
		opcmChangeBrowsePosition,
		opcmBrowseOPCItemIDs,
		opcmGetItemID,
		opcmBrowseAccessPath,
		// IPersistFile
		opcmIsDirty,
		opcmLoad,
		opcmSave,
		opcmSaveCompleted,
		opcmGetCurFileName,
		// IOPCCommon
		opcmSetLocaleID,
		opcmGetLocaleID,
		opcmQueryAvailableLocaleIDs,
		opcmSetClientName,
		// IOPCItemProperties
		opcmQueryAvailableProperties,
		opcmGetItemProperties,
		opcmLookupItemIDs,
		// IConnectionPointContainer
		opcmEnumConnectionPoints,
		opcmFindConnectionPoint
	}; // enum methodOPCServer

	// special release method
	virtual LONG release(void);	

	virtual BOOL addLeaf(IN GenericLeaf *newLeaf, IN OPTIONAL int index = -1);
	virtual BOOL addBranch(IN GenericBranch *newBranch, 
			IN OPTIONAL int index = -1);

	// timeout of requests
	BOOL setDuetimeRead(IN DWORD time);
	DWORD getDuetimeRead(void) const;
	BOOL setDuetimeWrite(IN DWORD time);
	DWORD getDuetimeWrite(void) const;

	// locale Id
	DWORD getLCID(void) const;

	// client name
	CString getClientName(void) const;

	// start event of request handling thread
	HANDLE getStartEvent(void) const;

	// time of last client update
	BOOL getUpdateTime(OUT LPFILETIME time) const;

	// bandwidth
	virtual DWORD getBandwidth(void);

	// IOPCCommon methods
	HRESULT getErrorString(IN HRESULT error, OUT CString &string);
	virtual DWORD queryAvailableLCIDs(OUT LCID **lcidArray);

	// IOPCServer methods
	virtual HRESULT addGroup(IN LPCTSTR name, IN BOOL active, 
			IN DWORD reqUpdateRate,	IN OPCHANDLE clientHandle, IN LONG *pTimeBias, 
			IN FLOAT *pPercentDeadband, IN DWORD lcid, OUT GenericGroup **group, 
			OUT DWORD *revUpdateRate);
	virtual HRESULT removeGroup(IN OPCHANDLE serverHandle, 
			IN OPTIONAL BOOL remPublicGrp = FALSE);
	virtual HRESULT getErrorString(IN HRESULT error, IN DWORD lcid, 
			OUT CString &string);
	virtual HRESULT getStatus(OUT OPCSERVERSTATUS *status);

	// group retrival 
	virtual GenericGroup *getGroupByName(IN LPCTSTR name);
	virtual GenericGroup *getGroupByHandle(IN OPCHANDLE serverHandle);

	// is public groups server
	BOOL isPublicGroups(void) const;

	// start the sending of the shutdown request to the OPC client
	HRESULT startShutdownRequest(IN LPCTSTR reason, 
			IN OPTIONAL HANDLE event = INVALID_HANDLE_VALUE);
	// send a shutdown request to the OPC client
	virtual HRESULT sendShutdownRequest(IN LPCTSTR reason);

	// check if connection to OPC client is OK
	BOOL checkClientConnection(void);
	// disconnect the OPC client belonging to the server object -> deletes the server object
	void disconnectClient(void);

	// main routine of request handling thread 
	DWORD requestThread(void);

    // standard string filter implementations
	static BOOL stdMatchStringFilter(IN LPCTSTR string, IN LPCTSTR filter);
	static BOOL opcMatchStringFilter(IN LPCTSTR string, IN LPCTSTR filter, IN BOOL caseSensitive);

	CCriticalSection *getRequestStateLock(void);

protected:
	virtual ~GenericServer();

	void *m_me;
	CMutex m_meSync;
	GenericPointer<GenericBranch> m_curBrowse;
									// current browse node
	CString m_curBrowsePath;		// current browse path
	DWORD m_dueTimeRead;			// timeout of asynchronous read request
	DWORD m_dueTimeWrite;			// timeout of asynchronous write request
	FILETIME m_lastUpdateTime;		// time of last update to OPC client
	DWORD m_bandwidth;				// bandwidth of server
	HANDLE m_events[2];				// events of request handling thread
	HANDLE m_requestThread;			// thread handle of request handling thread
	DWORD m_requestId;			// thread handle of request handling thread
	TransactionList m_transaction;	// transaction list
	CCriticalSection m_transactionLock;
									// lock for transaction list
	BYTE m_transactionListChanged;	// changed transaction list
	DWORD m_lcid;					// local Id
	CString m_clientName;			// client name
	BYTE m_publicGroupTemplates;	// is public groups server
	DWORD m_minRequestLoopTime;
	CCriticalSection m_requestStateLock;

	// locale Id
	virtual BOOL setLCID(IN DWORD lcid);

	// client name
	virtual BOOL setClientName(IN LPCTSTR clientName);

	// callbacks
	virtual void onSetLCID(IN DWORD lcid);
	virtual void onSetClientName(IN LPCTSTR clientName);

	// IOPCItemProperties	
	virtual HRESULT queryAvailableProperties(IN GenericTag *tag,
			OUT TagPropertyDataList &propertyList);
	virtual HRESULT queryAvailableDynamicProperties(IN LPCTSTR itemId,
			OUT TagPropertyDataList &propertyList);

	virtual BOOL isDynamicTagProperty(IN LPCTSTR itemId, IN DWORD propId);
	BOOL isStandardTagProperty(IN DWORD propId);

	// IOPCBrowseServerAddressSpace
	// static namespace
	virtual HRESULT queryOrganization(OUT OPCNAMESPACETYPE *nameSpaceType);
	virtual HRESULT changeBrowsePosition(IN OPCBROWSEDIRECTION direction, 
			IN LPCTSTR name);
	virtual HRESULT browseCurrentBranch(IN OPCBROWSETYPE browseType, 
			IN LPCTSTR filterCriteria, IN VARTYPE dataTypeFilter, 
			IN DWORD accessRightsFilter, OUT CStringList &nameList,
			IN OPTIONAL LPCTSTR path = NULL);
	virtual BOOL getBrowseElementPath(IN LPCTSTR name, OUT CString &path);
	// dynamic namespace
	virtual HRESULT changeDynamicBrowsePosition(IN OPCBROWSEDIRECTION direction, 
			IN LPCTSTR name);
	// callbacks for dynamic namespace
	virtual BOOL isDynamicNode(IN LPCTSTR fullName);
	virtual BOOL isDynamicTag(IN LPCTSTR fullName);
	virtual HRESULT getDynamicChildNames(IN OPCBROWSETYPE browseType, 
			IN LPCTSTR filterCriteria, IN VARTYPE dataTypeFilter, 
			IN DWORD accessRightsFilter, OUT CStringList &nameList);
	virtual HRESULT getDynamicBrowseAccessPaths(IN LPCTSTR name, 
			OUT CStringList &pathList);
	virtual VARTYPE getDynamicTagDatatype(IN LPCTSTR fullName);

	// dynamic creation of namespace tag
	virtual GenericTag *createTag(IN LPCTSTR fullName);

	// create tag property object if needed
	GenericTagProperty *createTagProperty(IN GenericTag *tag, IN LPCTSTR itemId, IN DWORD propId);

	// get tag object of static namespace tree
	virtual GenericTag *getTag(IN LPCTSTR fullName);

	// synchron read 
	virtual HRESULT syncRead(IN RequestList &reqList, IN OPCDATASOURCE source, IN LPCTSTR user);

	// string filter used for browsing
	virtual BOOL matchStringFilter(IN LPCTSTR string, IN LPCTSTR filter);

	// transaction list management
	virtual void addTransaction(IN OPCTransaction *trans);
	virtual BOOL removeTransaction(IN OPCTransaction *trans, IN OPTIONAL DWORD transId = 0);

	// cyclic requests 
	virtual BOOL insertCyclicRequest(IN GenericItem *item);
	virtual BOOL removeCyclicRequest(IN GenericItem *item);

	// transaction actions	
	DWORD updateTransactions(IN DWORD now, IN DWORD minSpan);
	DWORD startCyclicTransactions(IN DWORD now, IN DWORD minSpan);

	// group counters
	void updateGroupCounts(IN BOOL add, IN GenericGroup *grp);

	// public groups server methods
	virtual BOOL canRemovePublicGroup(IN GenericGroup *group);

	// access to OPC method
	virtual BOOL accessToMethod(IN REFIID iid, 
			IN enum methodOPCServer method, IN LPCTSTR user);

}; // GenericServer


inline BOOL GenericServer::setDuetimeRead(IN DWORD time)
{ m_dueTimeRead = time; return TRUE; }

inline DWORD GenericServer::getDuetimeRead(void) const
{ return m_dueTimeRead; }

inline BOOL GenericServer::setDuetimeWrite(IN DWORD time)
{ m_dueTimeWrite = time; return TRUE; }

inline DWORD GenericServer::getDuetimeWrite(void) const
{ return m_dueTimeWrite; }

inline BOOL GenericServer::getUpdateTime(OUT LPFILETIME time) const
{ if (!time) return FALSE;
  time->dwLowDateTime = m_lastUpdateTime.dwLowDateTime; 
  time->dwHighDateTime = m_lastUpdateTime.dwHighDateTime; return TRUE; }

inline BOOL GenericServer::isPublicGroups(void) const
{ return m_publicGroupTemplates; }

inline DWORD GenericServer::getLCID(void) const
{ return m_lcid; }

inline CString GenericServer::getClientName(void) const
{ return m_clientName; }

inline HRESULT GenericServer::getErrorString(IN HRESULT error, OUT CString &string)
{ return getErrorString(error, m_lcid, string); }

inline BOOL GenericServer::isStandardTagProperty(IN DWORD propId)
{ return (((propId >= 1) && (propId <= 6)) || (propId == 108)); }

inline CCriticalSection *GenericServer::getRequestStateLock(void)
{ return &m_requestStateLock; }



//-----------------------------------------------------------------------------
// OPCRootOPC                                                                 |
//-----------------------------------------------------------------------------

class OPCENGINE_EXPORT OPCRootOPC : public GenericBranch
{
public:
	OPCRootOPC(); 

	virtual BOOL addLeaf(IN GenericLeaf *newLeaf, IN OPTIONAL int index = -1);
	virtual BOOL addBranch(IN GenericBranch *newBranch, 
			IN OPTIONAL int index = -1);
}; // OPCRootOPC

#endif
