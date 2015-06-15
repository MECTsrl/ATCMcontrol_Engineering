//-----------------------------------------------------------------------------
//                                                                            |



//                                                                            |



//                                                                            |
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                           OPC TOOLBOX - SODaS                              |
//                                                                            |
//  Filename    : SODaSServer.h                                               |
//  Version     : 3.10.release                                                |
//  Date        : 4-March-2002                                                |
//                                                                            |
//  Description : OPC Server object related classes                           |
//                - SODaSServer: OPC DA server object                         |
//                - SODaSObjectRoot: root object of OPC runtime object tree   |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _SODASSERVER_H_
#define _SODASSERVER_H_

#include <opcda.h>
#include "SOCmnElement.h"
#include "SODaSItemTag.h"
#include "SODaSTag.h"
#include "SODaSRequest.h"
#include "SODaSTransaction.h"
#include "SOSrvServer.h"

#pragma pack(push,4)

//-----------------------------------------------------------------------------
// SODaSServer                                                                |
//-----------------------------------------------------------------------------

class SODaSComServer;
class SODaSComGroup;
class SODaSTransaction;
class SODaSGroup;
class SODaSItem;
class SODaSTag;
class SODaSNode;

class SODAS_EXPORT SODaSServer : 
	public SOSrvServer,
	public SOSrvBrowser,
	virtual public SOCmnElementListBranch
{
friend SODaSComServer;
friend SODaSComGroup;
friend SODaSTransaction;
friend SODaSGroup;
friend SODaSItem;

public:
	SODaSServer(IN OPTIONAL BOOL publicGroups = FALSE, 
			IN OPTIONAL BYTE serverInstance = 0,
			IN OPTIONAL DWORD minRequestLoopTime = INFINITE);  

	virtual BOOL addBranch(IN SOCmnElement *newBranch);
	virtual LPCTSTR getObjTraceId(void);

	// timeout of requests
	BOOL setDuetimeRead(IN DWORD time);
	DWORD getDuetimeRead(void) const;
	BOOL setDuetimeWrite(IN DWORD time);
	DWORD getDuetimeWrite(void) const;

	// time of last client update
	BOOL getUpdateTime(OUT LPFILETIME time) const;

	// bandwidth
	virtual DWORD getBandwidth(void);

	// IOPCServer methods
	virtual HRESULT getStatus(OUT OPCSERVERSTATUS *status);

	virtual HRESULT addGroup(IN LPCTSTR name, IN BOOL active, 
			IN DWORD reqUpdateRate,	IN OPCHANDLE clientHandle, IN LONG *pTimeBias, 
			IN FLOAT *pPercentDeadband, IN DWORD lcid, OUT SODaSGroup **group, 
			OUT DWORD *revUpdateRate, IN OPTIONAL BOOL addWatchChange = TRUE);
	virtual HRESULT removeGroup(IN OPCHANDLE serverHandle, 
			IN OPTIONAL BOOL remPublicGrp = FALSE);

	// group retrival 
	virtual SODaSGroup *getGroupByName(IN LPCTSTR name);
	virtual SODaSGroup *getGroupByHandle(IN OPCHANDLE serverHandle);

	// is public groups server
	BOOL isPublicGroups(void) const;

	// start the sending of the shutdown request to the OPC client
	HRESULT startShutdownRequest(IN LPCTSTR reason, 
			IN OPTIONAL HANDLE event = INVALID_HANDLE_VALUE);
	// send a shutdown request to the OPC client
	virtual HRESULT sendShutdownRequest(IN LPCTSTR reason);

	// transaction list
	SOCmnList<SODaSTransaction> *getTransactionList(void) { return &m_transactionList; }

	// check if connection to OPC client is OK
	virtual BOOL checkClientConnection(void);
	// disconnect the OPC client belonging to the server object -> deletes the server object
	virtual void disconnectClient(void);

	// COM object handling
	void *getMe(void);
	ULONG releaseMe(IN void* me);

	virtual void getCLSID(OUT CLSID &classID);

	// objects watch data
	virtual void getObjWatchData(IN void *pXMLDocumentInterface, IN void *pXMLParentNodeInterface, 
			IN BOOL withAttributes,	IN LPWSTR objNodeName, OPTIONAL OUT void **pXMLObjectNodeInterface = NULL);

protected:
	virtual ~SODaSServer();

	DWORD m_dueTimeRead;			// timeout of synchronous read request
	DWORD m_dueTimeWrite;			// timeout of synchronous write request
	FILETIME m_lastUpdateTime;		// time of last update to OPC client
	DWORD m_bandwidth;				// bandwidth of server
	SOCmnList<SODaSTransaction> m_transactionList;	// transaction list
									// lock for transaction list
	BYTE m_publicGroupTemplates;	// is public groups server

	// IOPCItemProperties	
	virtual void queryAvailablePropertiesByItemID(IN LPCTSTR itemId,
			OUT SOCmnList<SODaSPropertyData> &propertyList);
	virtual BOOL isPropertyName(IN LPCTSTR itemId, IN DWORD propId, IN LPCTSTR propIdString);

	// IOPCBrowseServerAddressSpace
	// callbacks for dynamic namespace
	virtual BOOL isNodeName(IN LPCTSTR fullName);
	virtual BOOL isTagName(IN LPCTSTR fullName);
	virtual BOOL isBranchName(IN LPCTSTR fullName);
	virtual BOOL isLeafName(IN LPCTSTR fullName);
	virtual HRESULT getTagAccessPathsByItemID(IN LPCTSTR name, 
			OUT SOCmnStringList &pathList);
	virtual VARTYPE getTagDatatypeByItemID(IN LPCTSTR fullName);
	virtual DWORD getAccessRightsByItemID(IN LPCTSTR fullName);
	virtual void getChildNamesOfNodeName(IN LPCTSTR nodeName, IN enum browserFilterType browseType, 
			IN LPCTSTR filterCriteria, IN VARTYPE dataTypeFilter, 
			IN DWORD accessRightsFilter, OUT SOCmnStringList &nameList);

	virtual HRESULT getChildNames(IN enum browserFilterType browseType, 
			IN LPCTSTR filterCriteria, IN VARTYPE dataTypeFilter, 
			IN DWORD accessRightsFilter, OUT SOCmnStringList &nameList);
	virtual SOCmnElement *getBrowserRoot(void);
	virtual SOCmnObject *getObjectSrvBrowser(void);
	virtual BOOL ignoreFilterTypeOnFlatOrg(void);
	virtual BOOL matchAdditionalFilters(IN SOCmnElement *obj, 
			IN VARTYPE dataTypeFilter, IN DWORD accessRightsFilter);

	// string based creation of namespace tag and node objects
	virtual SODaSTag *createTag(IN LPCTSTR fullName);
	virtual void initTag(IN SODaSTag *tag);
	virtual SODaSNode *createNode(IN LPCTSTR fullName);

	// string based property object creation
	virtual SODaSProperty *createProperty(IN SOCmnElement *element, IN LPCTSTR itemId, IN DWORD propId, IN LPCTSTR propName);
	virtual void initProperty(IN SODaSProperty *property);
	virtual HRESULT canCreateProperty(IN SOCmnElement *element, IN LPCTSTR itemId, IN DWORD propId);

	// property item ids
	virtual DWORD getPropertyIDByName(IN LPCTSTR parentName, IN LPCTSTR propName);
	virtual HRESULT getPropertyItemID(IN SOCmnElement *element, IN LPCTSTR itemId, IN DWORD propId, 
			OUT SOCmnString &propItemID);

	// synchron I/O 
	virtual HRESULT syncRead(IN SOCmnList<SODaSRequest> &reqList, IN OPCDATASOURCE source);
	virtual HRESULT syncWrite(IN SOCmnList<SODaSRequest> &reqList);

	// string filter used for browsing
	virtual BOOL matchStringFilter(IN LPCTSTR string, IN LPCTSTR filter);

	// transaction list management
	virtual void addTransaction(IN SODaSTransaction *trans);
	virtual BOOL removeTransaction(IN SODaSTransaction *trans);
	virtual HRESULT cancelTransaction(IN DWORD transId);

	// transaction actions	
	virtual DWORD doWork(IN DWORD startTime);
	virtual DWORD updateTransactions(IN DWORD now, IN DWORD minSpan);
	virtual DWORD startCyclicTransactions(IN DWORD now, IN DWORD minSpan);

	// public groups server methods
	virtual BOOL canRemovePublicGroup(IN SODaSGroup *group);

	// handle the last external release of the COM object
	BOOL handleLastExternalReleaseWithInternalRefs(IN void *object, IN void *comServer);

	// query interface for own interfaces
	virtual HRESULT queryInterface(IN REFIID iid, IN void ** object);

	virtual SOCmnObject *getObjectCmnElementIList(void);
	
	// what type of server object
	virtual DWORD getSpecID(void);

	// standard properties
	virtual BOOL isStandardTagProperty(IN DWORD propId);
	virtual BOOL isStandardNodeProperty(IN DWORD propId);
	virtual void getStandardTagProperties(IN VARTYPE tagNativeDatatype,
			OUT SOCmnList<SODaSPropertyData> &propertyList);
	virtual void getStandardNodeProperties(OUT SOCmnList<SODaSPropertyData> &propertyList);

	// startup synchronisation
	virtual HRESULT waitForNameSpaceStartup(void);
	virtual HRESULT waitForIOStartup(void);

	// interface function callbacks
	virtual HRESULT onBSetLocaleID(IN LCID dwLcid);
	virtual HRESULT onBGetLocaleID(OUT LCID *pdwLcid);
	virtual HRESULT onBQueryAvailableLocaleIDs(OUT DWORD *pdwCount, OUT LCID **pdwLcid);
	virtual HRESULT onBGetErrorString(IN HRESULT dwError, OUT LPWSTR *ppString);
	virtual HRESULT onBSetClientName(IN LPCWSTR szName);
	virtual HRESULT onBAdvise(IN LPUNKNOWN unknown, OUT DWORD *cookie);
	virtual HRESULT onBUnadvise(IN DWORD cookie);
	virtual HRESULT onBAddGroup(IN LPCWSTR szName, IN BOOL bActive, IN DWORD dwRequestedUpdateRate, IN OPCHANDLE hClientGroup, IN LONG *pTimeBias, IN FLOAT *pPercentDeadband, IN DWORD dwLCID, OUT OPCHANDLE *phServerGroup, OUT DWORD *pRevisedUpdateRate, IN REFIID riid, OUT LPUNKNOWN * ppUnk);
	virtual HRESULT onBGetErrorString(IN HRESULT dwError, IN LCID dwLocale, OUT LPWSTR *ppString);
	virtual HRESULT onBGetGroupByName(IN LPCWSTR szName, IN REFIID riid, OUT LPUNKNOWN *ppUnk);
	virtual HRESULT onBGetStatus(OUT OPCSERVERSTATUS **ppServerStatus);
	virtual HRESULT onBRemoveGroup(IN OPCHANDLE hServerGroup, IN BOOL bForce);
	virtual HRESULT onBCreateGroupEnumerator(IN OPCENUMSCOPE dwScope, IN REFIID riid, OUT LPUNKNOWN *ppUnk);
	virtual HRESULT onBGetPublicGroupByName(IN LPCWSTR szName, IN REFIID riid, OUT LPUNKNOWN *ppUnk);
	virtual HRESULT onBRemovePublicGroup(IN OPCHANDLE hServerGroup, IN BOOL bForce);
	virtual HRESULT onBQueryAvailableProperties(IN LPWSTR szItemID, OUT DWORD *pdwCount, OUT DWORD **ppPropertyIDs, OUT LPWSTR **ppDescriptions, OUT VARTYPE **ppvtDataTypes);
	virtual HRESULT onBGetItemProperties(IN LPWSTR szItemID, IN DWORD dwCount, IN DWORD *pdwPropertyIDs, OUT VARIANT **ppvData, OUT HRESULT **ppErrors);
	virtual HRESULT onBLookupItemIDs(IN LPWSTR szItemID, IN DWORD dwCount, IN DWORD *pdwPropertyIDs, OUT LPWSTR **ppszNewItemIDs, OUT HRESULT **ppErrors);
	virtual HRESULT onBQueryOrganization(OUT OPCNAMESPACETYPE *pNameSpaceType);
	virtual HRESULT onBChangeBrowsePosition(IN OPCBROWSEDIRECTION dwBrowseDirection, IN LPCWSTR szString);
	virtual HRESULT onBBrowseOPCItemIDs(IN OPCBROWSETYPE dwBrowseFilterType, IN LPCWSTR szFilterCriteria, IN VARTYPE vtDataTypeFilter, IN DWORD dwAccessRightsFilter, OUT LPENUMSTRING *ppIEnumString);
	virtual HRESULT onBGetItemID(IN LPWSTR szItemDataID, OUT LPWSTR *szItemID);
	virtual HRESULT onBBrowseAccessPaths(IN LPCWSTR szItemID, OUT LPENUMSTRING *ppIEnumString);
	virtual HRESULT onBGetClassID(OUT CLSID *pClassID);
	virtual HRESULT onBIsDirty(void);
	virtual HRESULT onBLoad(IN LPCOLESTR pszFileName, IN DWORD dwMode);
	virtual HRESULT onBSave(IN LPCOLESTR pszFileName, IN BOOL fRemember);
	virtual HRESULT onBSaveCompleted(IN LPCOLESTR pszFileName);
	virtual HRESULT onBGetCurFile(OUT LPOLESTR *ppszFileName);
	virtual HRESULT onBGetChildren(IN LPCWSTR parent, IN BOOL withAttributes, OUT LPWSTR *children);
	virtual HRESULT onBCollectChanges(IN LPCWSTR objects, IN DWORD doCollect);
	virtual HRESULT onBGetChanges(OUT LPWSTR *changes);
	virtual HRESULT onBGetStatistics(OUT LPWSTR *statistics);
	virtual HRESULT onBGetObject(IN LPCWSTR objectDescription, OUT LPWSTR *object);
	virtual void onESetLocaleID(IN HRESULT res, IN LCID dwLcid);
	virtual void onEGetLocaleID(HRESULT res, IN LCID *pdwLcid);
	virtual void onEQueryAvailableLocaleIDs(IN HRESULT res, IN DWORD *pdwCount, IN LCID **pdwLcid);
	virtual void onEGetErrorString(IN HRESULT res, IN HRESULT dwError, IN LPWSTR *ppString);
	virtual void onESetClientName(IN HRESULT res, IN LPCWSTR szName);
	virtual void onEAdvise(IN HRESULT res, IN LPUNKNOWN unknown, IN DWORD *cookie);
	virtual void onEUnadvise(IN HRESULT res, IN DWORD cookie);
	virtual void onEAddGroup(IN HRESULT res, IN LPCWSTR szName, IN BOOL bActive, IN DWORD dwRequestedUpdateRate, IN OPCHANDLE hClientGroup, IN LONG *pTimeBias, IN FLOAT *pPercentDeadband, IN DWORD dwLCID, OUT OPCHANDLE *phServerGroup, IN DWORD *pRevisedUpdateRate, IN REFIID riid, IN LPUNKNOWN *ppUnk);
	virtual void onEGetErrorString(IN HRESULT res, IN HRESULT dwError, IN LCID dwLocale, IN LPWSTR *ppString);
	virtual void onEGetGroupByName(IN HRESULT res, IN LPCWSTR szName, IN REFIID riid, IN LPUNKNOWN *ppUnk);
	virtual void onEGetStatus(IN HRESULT res, IN OPCSERVERSTATUS **ppServerStatus);
	virtual void onERemoveGroup(IN HRESULT res,IN OPCHANDLE hServerGroup, IN BOOL bForce);
	virtual void onECreateGroupEnumerator(IN HRESULT res, IN OPCENUMSCOPE dwScope, IN REFIID riid, IN LPUNKNOWN *ppUnk);
	virtual void onEGetPublicGroupByName(IN HRESULT res, IN LPCWSTR szName, IN REFIID riid, IN LPUNKNOWN *ppUnk);
	virtual void onERemovePublicGroup(IN HRESULT res, IN OPCHANDLE hServerGroup, IN BOOL bForce);
	virtual void onEQueryAvailableProperties(IN HRESULT res, IN LPWSTR szItemID, IN DWORD *pdwCount, IN DWORD **ppPropertyIDs, IN LPWSTR **ppDescriptions, IN VARTYPE **ppvtDataTypes);
	virtual void onEGetItemProperties(IN HRESULT res, IN LPWSTR szItemID, IN DWORD dwCount, IN DWORD *pdwPropertyIDs, IN VARIANT **ppvData, IN HRESULT **ppErrors);
	virtual void onELookupItemIDs(IN HRESULT res, IN LPWSTR szItemID, IN DWORD dwCount, IN DWORD *pdwPropertyIDs, IN LPWSTR **ppszNewItemIDs, IN HRESULT **ppErrors);
	virtual void onEQueryOrganization(IN HRESULT res, IN OPCNAMESPACETYPE *pNameSpaceType);
	virtual void onEChangeBrowsePosition(IN HRESULT res, IN OPCBROWSEDIRECTION dwBrowseDirection, IN LPCWSTR szString);
	virtual void onEBrowseOPCItemIDs(IN HRESULT res, IN OPCBROWSETYPE dwBrowseFilterType, IN LPCWSTR szFilterCriteria, IN VARTYPE vtDataTypeFilter, IN DWORD dwAccessRightsFilter, IN LPENUMSTRING *ppIEnumString);
	virtual void onEGetItemID(IN HRESULT res, IN LPWSTR szItemDataID, IN LPWSTR *szItemID);
	virtual void onEBrowseAccessPaths(IN HRESULT res, IN LPCWSTR szItemID, IN LPENUMSTRING *ppIEnumString);
	virtual void onEGetClassID(IN HRESULT res, IN CLSID *pClassID);
	virtual void onEIsDirty(IN HRESULT res);
	virtual void onELoad(IN HRESULT res, IN LPCOLESTR pszFileName, IN DWORD dwMode);
	virtual void onESave(IN HRESULT res, IN LPCOLESTR pszFileName, IN BOOL fRemember);
	virtual void onESaveCompleted(IN HRESULT res, IN LPCOLESTR pszFileName);
	virtual void onEGetCurFile(IN HRESULT res, IN LPOLESTR *ppszFileName);
	virtual void onEGetChildren(IN HRESULT res, IN LPCWSTR parent, IN BOOL withAttributes, IN LPWSTR *children);
	virtual void onECollectChanges(IN HRESULT res, IN LPCWSTR objects, IN DWORD doCollect);
	virtual void onEGetChanges(IN HRESULT res, IN LPWSTR *changes);
	virtual void onEGetStatistics(IN HRESULT res, IN LPWSTR *statistics);
	virtual void onEGetObject(IN HRESULT res, IN LPCWSTR objectDescription, OUT LPWSTR *object);
}; // SODaSServer


inline BOOL SODaSServer::setDuetimeRead(IN DWORD time)
{ m_dueTimeRead = time; return TRUE; }

inline DWORD SODaSServer::getDuetimeRead(void) const
{ return m_dueTimeRead; }

inline BOOL SODaSServer::setDuetimeWrite(IN DWORD time)
{ m_dueTimeWrite = time; return TRUE; }

inline DWORD SODaSServer::getDuetimeWrite(void) const
{ return m_dueTimeWrite; }

inline BOOL SODaSServer::getUpdateTime(OUT LPFILETIME time) const
{ if (!time) return FALSE;
  time->dwLowDateTime = m_lastUpdateTime.dwLowDateTime; 
  time->dwHighDateTime = m_lastUpdateTime.dwHighDateTime; return TRUE; }

inline BOOL SODaSServer::isPublicGroups(void) const
{ return m_publicGroupTemplates; }

#define GenericServer SODaSServer




//-----------------------------------------------------------------------------
// SODaSObjectRoot                                                            |
//-----------------------------------------------------------------------------

class SODAS_EXPORT SODaSObjectRoot : 
	public SOSrvObjectRoot
{
public:
	SODaSObjectRoot(void); 

	virtual BOOL addBranch(IN SOCmnElement *newBranch);

	// objects watch data
	virtual void getObjWatchData(IN void *pXMLDocumentInterface, IN void *pXMLParentNodeInterface, 
			IN BOOL withAttributes,	IN LPWSTR objNodeName, OPTIONAL OUT void **pXMLObjectNodeInterface = NULL);
}; // SODaSObjectRoot

#define OPCRootOPC SODaSObjectRoot

#pragma pack(pop)
#endif
