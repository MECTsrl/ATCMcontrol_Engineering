//-----------------------------------------------------------------------------
//                                                                            |



//                                                                            |



//                                                                            |
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                           OPC TOOLBOX - SOAeS                              |
//                                                                            |
//  Filename    : SOAeSEntry.h                                                |
//  Version     : 3.10.release                                                |
//  Date        : 4-March-2002                                                |
//                                                                            |
//  Description : Entry point for Server Toolkit                              |
//                - SOAeSEntry: Entry point class                             |
//                - SOAeSCreator: Object factory for toolkit objects          |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _SOAESENTRY_H_
#define _SOAESENTRY_H_

#include <opc_ae.h>
#include "SOAeSCategory.h"
#include "SOAeSCondition.h"

#pragma pack(push,4)


//-----------------------------------------------------------------------------
// SOAeSCreator                                                               |
//-----------------------------------------------------------------------------

class SOAeSServer;
class SOAeSSubscription;
class SOAeSAreaBrowser;
class SOAeSCondition;
class SOAeSEvent;
class SOAeSAreaSpaceRoot;
class SOAeSEventSpaceRoot;
class SOAeSObjectRoot;

class SOAES_EXPORT SOAeSCreator : public SOSrvCreator
{
public:
	virtual SOAeSServer *createServer(IN BYTE serverInstance);
	virtual SOAeSSubscription *createSubscription(IN SOAeSServer *server);
	virtual SOAeSAreaBrowser *createAreaBrowser(IN SOAeSServer *server);
	virtual SOAeSEvent *createConditionEvent(IN SOAeSCondition *cond);
	virtual SOAeSEvent *createSimpleEvent(void);
	virtual SOAeSEvent *createTrackingEvent(void);
	virtual SOAeSAttributeValue *createAttributeValue(void);
	virtual SOAeSAttributeValue *createAttributeValue(IN SOAeSCondition *cond);
	virtual SOAeSAreaSpaceRoot *createAreaSpaceRoot(IN BYTE serverInstance);
	virtual SOAeSEventSpaceRoot *createEventSpaceRoot(void);
	virtual SOAeSObjectRoot *createObjectRoot(void);
}; // SOAeSCreator




//-----------------------------------------------------------------------------
// SOAeSEntry                                                                 |
//-----------------------------------------------------------------------------

#define SOAES_SERVER_INSTANCE_COUNT 10

class SOAeSServer;
class SOAeSEventSpaceRoot;
class SOAeSAreaSpaceRoot;

class SOAES_EXPORT SOAeSEntry : public SOSrvEntry
{
friend SOAeSServer;

public:
	SOAeSEntry(void);

	// object factory for OPC objects
	void setCreator(IN SOAeSCreator *creator);
	SOAeSCreator *getCreator(void);
	virtual SOSrvCreator *getSrvCreator(void);

	// event space
	SOAeSEventSpaceRoot *getEventSpaceRoot(void);

	// area space
	SOAeSAreaSpaceRoot *getAreaSpaceRoot(IN OPTIONAL BYTE serverInstance = 0);

	// helpers for InProc server export functions
	virtual HRESULT getClassObject(IN REFCLSID rclsid, IN REFIID riid, 
			OUT LPVOID *ppvObj);
	virtual HRESULT canUnloadNow(void);

	// OPC server state
	BOOL setServerState(IN OPCEVENTSERVERSTATE serverState);
	OPCEVENTSERVERSTATE getServerState(void) const;

	// start and stop of entry threads
	virtual BOOL start(void);
	virtual BOOL stop(void);

	// enable condition survey
	void enableConditionSurvey(OPTIONAL IN BOOL checkConditions = TRUE);
	BOOL getCheckConditions(void) const;

	// condition survey thread 
	void startSurvey(void);
	void stopSurvey(void);
	DWORD surveyThread(void);

	// condition groups
	SOCmnList<SOAeSConditionGroup> *getConditionGroupList(void);
	SOAeSConditionGroup * getConditionGroup(IN DWORD updateRate);
	virtual void connectAttributeValueUpdate(void);

	// server watch object
	virtual SOSrvWatch *addWatchObject(void);
	void getObjectCountStatistics(IN MSXML::IXMLDOMDocumentPtr pDoc, IN MSXML::IXMLDOMNodePtr pRootNode);
	void incObjectCountStatistics(IN DWORD objType);
	void decObjectCountStatistics(IN DWORD objType);
	void changeConditionStateStatistics(IN BYTE prevState, IN BYTE newState);

	// ATL COM helper methods
	virtual HRESULT	revokeClassObjects(void);
	virtual BOOL addComObjMapEntry(IN const CLSID *pClsid, IN void *pfnUpdateRegistry,
			IN void *pfnGetClassObject, IN void *pfnCreateInstance, IN void *pfnGetObjectDescription,
			IN void *pfnGetCategoryMap, IN void *pfnObjectMain);
	virtual BOOL addComObjMapEntry(IN const CLSID *pClsid, IN DWORD cnt);

protected:
	OPCEVENTSERVERSTATE m_serverState;					// OPC server state
	SOAeSCreator *m_creator;							// creator object
	SOCmnPointer<SOAeSEventSpaceRoot> m_eventSpaceRoot;	// event space root
	SOCmnPointer<SOAeSAreaSpaceRoot> m_areaSpaceRoot[SOAES_SERVER_INSTANCE_COUNT];	// area space root
	HANDLE m_condSurveyThread;							// handle of condition survey thread
	HANDLE m_condSurveyEvents[2];						// events of condition survey thread
	SOCmnList<SOAeSConditionGroup> m_condGroupList;		// list of condition check groups
	BYTE m_checkConditions;								// check conditions in survey thread
	struct SOAeSEntryStatisticsData
	{
		ULONG serverObjs;
		ULONG subscriptionObjs;
		ULONG areaBrowserObjs;
		ULONG areaObjs;
		ULONG sourceObjs;
		ULONG conditionObjs;
		ULONG attributeValueObjs;
		ULONG categoryObjs;
		ULONG attributeObjs;
		ULONG conditionClassObjs;
		ULONG subConditionClassObjs;

		ULONG actAckCond;
		ULONG actNAckCond;
		ULONG nActNAckCond;
	} m_statistics; 

	virtual ~SOAeSEntry(void);

	// survey thread helpers
	virtual DWORD updateConditions(IN SOCmnElement *parent, IN DWORD now, IN DWORD minSpan);

	// ATL module access
	virtual void term(void);
	virtual void init(IN HINSTANCE instanceHandle);
	virtual HRESULT	registerClassObjects(void);
	virtual HRESULT regServer(void);
	virtual HRESULT unregServer(void);
	virtual HRESULT updateRegistryWithRGS(IN BOOL doRegister, IN void *replacementArray);
}; // SOAeSEntry


inline BOOL SOAeSEntry::setServerState(IN OPCEVENTSERVERSTATE serverState)
{ m_serverState = serverState; return TRUE; }

inline OPCEVENTSERVERSTATE SOAeSEntry::getServerState(void) const
{ return m_serverState; }

inline SOAeSCreator *SOAeSEntry::getCreator(void)
{ if (m_creator) m_creator->addRef(); return m_creator; }

inline void SOAeSEntry::enableConditionSurvey(OPTIONAL IN BOOL checkConditions)
{ m_checkConditions = checkConditions; }

inline SOCmnList<SOAeSConditionGroup> *SOAeSEntry::getConditionGroupList(void)
{ return &m_condGroupList; }

inline BOOL SOAeSEntry::getCheckConditions(void) const
{ return m_checkConditions; }




//-----------------------------------------------------------------------------
// Global Functions                                                           |
//-----------------------------------------------------------------------------

// get pointer of the entry object
SOAES_EXPORT SOAeSEntry *getSOAeSEntry(void);

#pragma pack(pop)
#endif
