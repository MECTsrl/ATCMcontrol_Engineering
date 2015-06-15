//-----------------------------------------------------------------------------
//                                                                            |



//                                                                            |



//                                                                            |
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                           OPC TOOLBOX - SOAeS                              |
//                                                                            |
//  Filename    : SOAeSServer.h                                               |
//  Version     : 3.10.release                                                |
//  Date        : 4-March-2002                                                |
//                                                                            |
//  Description : OPC Server object related classes                           |
//                - SOAeSServer: OPC AE server object                         |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _SOAESSERVER_H_
#define _SOAESSERVER_H_

#include <opccomn.h>
#include <opc_ae.h>
#include "SOCmnElement.h"
#include "SOSrvServer.h"

#pragma pack(push,4)


//-----------------------------------------------------------------------------
// SOAeSServer                                                                |
//-----------------------------------------------------------------------------

class SOAeSComServer;
class SOAeSComSubscription;
class SOAeSSubscription;
class SOAeSAreaBrowser;
class SOAeSRefresh;
class SOAeSAttributeValueDADescription;

class SOAES_EXPORT SOAeSServer : 
	public SOSrvServer,
	virtual public SOCmnElementListLeafAdditional
{
friend SOAeSComServer;
friend SOAeSComSubscription;
friend SOAeSAreaBrowser;
friend SOAeSRefresh;
friend SOAeSSubscription;

public:
	SOAeSServer(IN OPTIONAL BYTE serverInstance = 0);  

	// tree management
	virtual BOOL addLeaf(IN SOCmnElement *newLeaf);
	virtual BOOL addAdditionalElement(IN SOCmnElement *newEL);
	virtual LPCTSTR getObjTraceId(void);

	// time of last client notification
	BOOL getUpdateTime(OUT LPFILETIME time) const;

	// IOPCEventServer methods
	virtual HRESULT getStatus(OUT OPCEVENTSERVERSTATUS *status);
	virtual DWORD queryAvailableFilters(void);

	// IConnectionPoint method
	BOOL createConnectedShutdownInterfaceList(OUT SOCmnList<IOPCShutdown> *shutdownList);

	// start the sending of the shutdown request to the OPC client
	HRESULT startShutdownRequest(IN LPCTSTR reason, 
			IN OPTIONAL HANDLE event = INVALID_HANDLE_VALUE);
	// send a shutdown request to the OPC client
	virtual HRESULT sendShutdownRequest(IN LPCTSTR reason);

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
	virtual ~SOAeSServer();

	FILETIME m_lastUpdateTime;		// time of last notification to OPC client

	// create subscription
	virtual HRESULT createSubscription(IN BOOL active, IN DWORD bufferTime,
			IN DWORD maxSize, IN OPCHANDLE clientSubscriptionHandle,
			OUT SOAeSSubscription **subsrc, OUT DWORD *revisedBufferTime,
			OUT DWORD *revisedMaxSize);

	// create area browser
	virtual HRESULT createAreaBrowser(OUT SOAeSAreaBrowser **areaBrowser);

	// string based area space
	virtual BOOL isAreaName(IN LPCTSTR areaName);
	virtual BOOL isSourceName(IN LPCTSTR sourceName);
	virtual void getChildNamesOfAreaName(IN LPCTSTR areaName, BOOL areaOrSourceChilds,
			LPCTSTR stringFilter, OUT SOCmnStringList &nameList);
	virtual void getConditionNamesOfSourceName(IN LPCTSTR sourceName, 
			OUT SOCmnStringList &condNameList);

	// support condition operations
	virtual BOOL supportEnableCondition(IN BOOL enable, IN BOOL areaOrSource);
	virtual BOOL supportGetConditionState(void);
	virtual BOOL supportTranslateToItemIDs(void);

	// string based (not object based) condition handling
	virtual BOOL enableConditionByAreaName(IN BOOL enable, IN LPCTSTR areaName);
	virtual BOOL enableConditionBySourceName(IN BOOL enable, IN LPCTSTR sourceName);
	virtual void refreshConditions(IN SOAeSRefresh *refresh);
	virtual void cancelRefreshConditions(IN SOAeSRefresh *refresh);
	virtual HRESULT createConditionByName(IN LPCTSTR sourceName, IN LPCTSTR conditionName,
			OUT SOAeSCondition **condition);
	virtual HRESULT ackConditionByName(IN LPCTSTR sourceName, IN LPCTSTR conditionName,
			IN LPCTSTR ackID, IN LPCTSTR ackComment, LPFILETIME activeTime, DWORD cookie);
	virtual HRESULT translateToItemIDsByName(IN LPCTSTR sourceName, IN SOAeSCategory *category,
			IN LPCTSTR conditionName, IN LPCTSTR subConditionName, 
			OUT SOCmnList<SOAeSAttributeValueDADescription> *descrDAList);

	virtual SOCmnObject *getObjectCmnElementIList(void);

	// working thread helpers
	virtual DWORD doWork(IN DWORD startTime);
	virtual DWORD sendEventNotifications(IN DWORD now, IN DWORD minSpan);
	virtual DWORD handleRefresh(IN DWORD now, IN DWORD minSpan);
	void aggregateConditions(IN SOCmnElement *parent, OUT SOCmnList<SOAeSEvent> *eventList);

	// query interface 
	virtual HRESULT queryInterface(REFIID iid, void ** object);

	// what type of server object
	virtual DWORD getSpecID(void);

	// startup synchronisation
	virtual HRESULT waitForAreaSpaceStartup(void);
	virtual HRESULT waitForIOStartup(void);

	// interface function callbacks
	virtual HRESULT onBSetLocaleID(IN LCID dwLcid);
	virtual HRESULT onBGetLocaleID(OUT LCID *pdwLcid);
	virtual HRESULT onBQueryAvailableLocaleIDs(OUT DWORD *pdwCount, OUT LCID **pdwLcid);
	virtual HRESULT onBGetErrorString(IN HRESULT dwError, OUT LPWSTR *ppString);
	virtual HRESULT onBSetClientName(IN LPCWSTR szName);
	virtual HRESULT onBGetStatus(OUT OPCEVENTSERVERSTATUS **eventServerStatus);
	virtual HRESULT onBCreateEventSubscription(IN BOOL active, IN DWORD bufferTime, IN DWORD maxSize, IN OPCHANDLE clientSubscriptionHandle, IN REFIID riid, OUT LPUNKNOWN *unknown, OUT DWORD *revisedBufferTime, OUT DWORD *revisedMaxSize);
	virtual HRESULT onBQueryAvailableFilters(OUT DWORD *filterMask);
	virtual HRESULT onBQueryEventCategories(IN DWORD eventType, OUT DWORD *count, OUT DWORD **eventCategories, OUT LPWSTR **eventCategoryDescs);
	virtual HRESULT onBQueryConditionNames(IN DWORD eventCategory, OUT DWORD *count, OUT LPWSTR **conditionNames);
	virtual HRESULT onBQuerySubConditionNames(IN LPWSTR conditionName, OUT DWORD *count, OUT LPWSTR **subConditionNames);
	virtual HRESULT onBQuerySourceConditions(IN LPWSTR source, OUT DWORD *count, OUT LPWSTR **conditionNames);
	virtual HRESULT onBQueryEventAttributes(IN DWORD eventCategory, OUT DWORD *count, OUT DWORD **attrIDs, OUT LPWSTR **attrDescs, OUT VARTYPE **attrTypes);
	virtual HRESULT onBTranslateToItemIDs(IN LPWSTR source, IN DWORD eventCategory, IN LPWSTR conditionName, IN LPWSTR subconditionName, IN DWORD count, OUT DWORD *assocAttrIDs, OUT LPWSTR **attrItemIDs, OUT LPWSTR **nodeNames, OUT CLSID **clsids);
	virtual HRESULT onBGetConditionState(IN LPWSTR source, IN LPWSTR conditionName, IN DWORD numEventAttrs, IN DWORD *attributeIDs, OUT OPCCONDITIONSTATE **conditionState);
	virtual HRESULT onBEnableConditionByArea(IN DWORD numAreas, IN LPWSTR *areas);
	virtual HRESULT onBEnableConditionBySource(IN DWORD numSources,IN LPWSTR *sources);
	virtual HRESULT onBDisableConditionByArea(IN DWORD numAreas,IN LPWSTR *areas);
	virtual HRESULT onBDisableConditionBySource(IN DWORD numSources, IN LPWSTR *sources);
	virtual HRESULT onBAckCondition(IN DWORD count, IN LPWSTR acknowledgerID, IN LPWSTR comment, IN LPWSTR *source, IN LPWSTR *conditionName, IN FILETIME *activeTime, IN DWORD *cookie, OUT HRESULT **errors);
	virtual HRESULT onBCreateAreaBrowser(IN REFIID riid, IN LPUNKNOWN *unknown);
	virtual HRESULT onBAdvise(IN LPUNKNOWN unknown, OUT DWORD *cookie);
	virtual HRESULT onBUnadvise(IN DWORD cookie);
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
	virtual void onEGetStatus(IN HRESULT res, IN OPCEVENTSERVERSTATUS **eventServerStatus);
	virtual void onECreateEventSubscription(IN HRESULT res, IN BOOL active, IN DWORD bufferTime, IN DWORD maxSize, IN OPCHANDLE clientSubscriptionHandle, IN REFIID riid, IN LPUNKNOWN *unknown, IN DWORD *revisedBufferTime, IN DWORD *revisedMaxSize);
	virtual void onEQueryAvailableFilters(IN HRESULT res, IN DWORD *filterMask);
	virtual void onEQueryEventCategories(IN HRESULT res, IN DWORD eventType, IN DWORD *count, IN DWORD **eventCategories, IN LPWSTR **eventCategoryDescs);
	virtual void onEQueryConditionNames(IN HRESULT res, IN DWORD eventCategory, IN DWORD *count, IN LPWSTR **conditionNames);
	virtual void onEQuerySubConditionNames(IN HRESULT res, IN LPWSTR conditionName, IN DWORD *count, IN LPWSTR **subConditionNames);
	virtual void onEQuerySourceConditions(IN HRESULT res, IN LPWSTR source, IN DWORD *count, IN LPWSTR **conditionNames);
	virtual void onEQueryEventAttributes(IN HRESULT res, IN DWORD eventCategory, IN DWORD *count, IN DWORD **attrIDs, IN LPWSTR **attrDescs, IN VARTYPE **attrTypes);
	virtual void onETranslateToItemIDs(IN HRESULT res, IN LPWSTR source, IN DWORD eventCategory, IN LPWSTR conditionName, IN LPWSTR subconditionName, IN DWORD count, IN DWORD *assocAttrIDs, IN LPWSTR **attrItemIDs, IN LPWSTR **nodeNames, IN CLSID **clsids);
	virtual void onEGetConditionState(IN HRESULT res, IN LPWSTR source, IN LPWSTR conditionName, IN DWORD numEventAttrs, IN DWORD *attributeIDs, IN OPCCONDITIONSTATE **conditionState);
	virtual void onEEnableConditionByArea(IN HRESULT res, IN DWORD numAreas, IN LPWSTR *areas);
	virtual void onEEnableConditionBySource(IN HRESULT res, IN DWORD numSources,IN LPWSTR *sources);
	virtual void onEDisableConditionByArea(IN HRESULT res, IN DWORD numAreas,IN LPWSTR *areas);
	virtual void onEDisableConditionBySource(IN HRESULT res, IN DWORD numSources, IN LPWSTR *sources);
	virtual void onEAckCondition(IN HRESULT res, IN DWORD count, IN LPWSTR acknowledgerID, IN LPWSTR comment, IN LPWSTR *source, IN LPWSTR *conditionName, IN FILETIME *activeTime, IN DWORD *cookie, IN HRESULT **errors);
	virtual void onECreateAreaBrowser(IN HRESULT res, IN REFIID riid, IN LPUNKNOWN *unknown);
	virtual void onEAdvise(IN HRESULT res, IN LPUNKNOWN unknown, IN DWORD *cookie);
	virtual void onEUnadvise(IN HRESULT res, IN DWORD cookie);
	virtual void onEGetChildren(IN HRESULT res, IN LPCWSTR parent, IN BOOL withAttributes, IN LPWSTR *children);
	virtual void onECollectChanges(IN HRESULT res, IN LPCWSTR objects, IN DWORD doCollect);
	virtual void onEGetChanges(IN HRESULT res, IN LPWSTR *changes);
	virtual void onEGetStatistics(IN HRESULT res, IN LPWSTR *statistics);
	virtual void onEGetObject(IN HRESULT res, IN LPCWSTR objectDescription, OUT LPWSTR *object);
}; // SOAeSServer

inline BOOL SOAeSServer::getUpdateTime(OUT LPFILETIME time) const
{ if (!time) return FALSE;
  time->dwLowDateTime = m_lastUpdateTime.dwLowDateTime; 
  time->dwHighDateTime = m_lastUpdateTime.dwHighDateTime; return TRUE; }




//-----------------------------------------------------------------------------
// SOAeSObjectRoot                                                            |
//-----------------------------------------------------------------------------

class SOAES_EXPORT SOAeSObjectRoot : 
	public SOSrvObjectRoot
{
public:
	SOAeSObjectRoot(void); 

	// objects watch data
	virtual void getObjWatchData(IN void *pXMLDocumentInterface, IN void *pXMLParentNodeInterface, 
			IN BOOL withAttributes,	IN LPWSTR objNodeName, OPTIONAL OUT void **pXMLObjectNodeInterface = NULL);
}; // SOAeSObjectRoot


#pragma pack(pop)
#endif
