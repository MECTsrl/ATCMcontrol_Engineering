//-----------------------------------------------------------------------------
//                                                                            |



//                                                                            |



//                                                                            |
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                           OPC TOOLBOX - SODaC                              |
//                                                                            |
//  Filename    : SODaCGroup.h                                                |
//  Version     : 3.10.release                                                |
//  Date        : 4-March-2002                                                |
//                                                                            |
//  Description : Group object related classes                                |
//                - SODaCGroup: proxy for OPC group object                    |
//                - SODaCGroupDefault: default group class for creator        |
//                - SODaCTaskSyncRead: synchron read task                     |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _SODACGROUP_H_
#define _SODACGROUP_H_

#include "SOCltElement.h"
#include "SODaCEntry.h"
#include "SODaCItem.h"


#pragma pack(push,4)

//-----------------------------------------------------------------------------
// SODaCGroup                                                                 |
//-----------------------------------------------------------------------------

// connection type
#define SODAC_CFG_CONNECTION_TYPE_ADDGROUP					1
#define SODAC_CFG_CONNECTION_TYPE_GETPUBLICGROUPBYNAME		2

// data retrieval
#define SODAC_CFG_DATA_RETRIEVAL_ASYNCADVISE	1
#define SODAC_CFG_DATA_RETRIEVAL_SYNCREAD		2

// group update what parameters
#define SODAC_GRPPAR_NAME			0x00000001
#define SODAC_GRPPAR_UPDATERATE		0x00000002
#define SODAC_GRPPAR_TIMEBIAS		0x00000004
#define SODAC_GRPPAR_DEADBAND		0x00000008
#define SODAC_GRPPAR_LCID			0x00000010
#define SODAC_GRPPAR_SERVERHANDLE	0x00000020
#define SODAC_GRPPAR_CLIENTHANDLE	0x00000100
#define SODAC_GRPPAR_ACTIVE			0x00000200
#define SODAC_GRPPAR_PUBLIC			0x10000000
#define SODAC_GRPPAR_ALL_MGT		0x0000033F
#define SODAC_GRPPAR_ALL_PUBMGT		0x10000000
#define SODAC_GRPPAR_ALL			0xFFFFFFFF

class SODaCAdviseSink;
class SODaCDataCallback;
class SODaCItem;

class SODAC_EXPORT SODaCGroup : 
	public SOCltElement,
	virtual public SOCmnElementNameString,
	virtual public SOCmnElementListLeaf
{
  friend SODaCServer;
  friend SODaCItem;
  friend SODaCAdviseSink;
  friend SODaCDataCallback;
  friend SODaCItemTaskUpdate;
  friend SODaCItemStateTransitionTask;

public:
	// connection type
	enum connectionType
	{
		ctNone					= 0,
		ctAddGroup				= SODAC_CFG_CONNECTION_TYPE_ADDGROUP,
		ctGetPublicGroupByName	= SODAC_CFG_CONNECTION_TYPE_GETPUBLICGROUPBYNAME
	};

	// data retrieval 
	enum dataRetrieval
	{
		drAsyncAdvise	= SODAC_CFG_DATA_RETRIEVAL_ASYNCADVISE,
		drSyncRead		= SODAC_CFG_DATA_RETRIEVAL_SYNCREAD
	};

	SODaCGroup(
			IN OPTIONAL WORD itemListType = SOCMNLIST_TYPE_ORG_LIST);  

	// query COM interface
	virtual HRESULT queryInterface(IN REFIID riid, IN void **interf);

	// update rate in ms 
	BOOL setReqUpdateRate(IN DWORD updateRate);
	DWORD getUpdateRate(OUT OPTIONAL DWORD *reqUpdateRate = NULL);

	// time zone
	BOOL setTimeBias(IN LONG timeBias);
	LONG getTimeBias(void) const;

	// deadband in percent
	BOOL setDeadBand(IN FLOAT deadBand);
	FLOAT getDeadBand(void) const;

	// local Id
	BOOL setLCID(IN DWORD lcid);
	DWORD getLCID(void) const;

	// server handle
	OPCHANDLE getServerHandle(void) const;

	// client handle from OPC client
	OPCHANDLE getClientHandle(void) const;

	// public flag
	BOOL setPublic(IN BOOL pub);
	BOOL getPublic(void) const;

	// connection type
	connectionType getConnectionType(void);
	BOOL setConnectionType(IN connectionType conType);

	// data retrieval method
	dataRetrieval getDataRetrieval(void);
	BOOL setDataRetrieval(IN dataRetrieval dataRet);

	// flags for valid parameter
	DWORD getValidParameters(void) const;
	void resetValidParameters(void);

	// get item object
	SODaCItem *getItemByClientHandle(IN OPCHANDLE clientHandle);
	SODaCItem *getItemByServerHandle(IN OPCHANDLE clientHandle);

	// item management
	HRESULT validateItems(IN SOCmnList<SODaCItem> *itemList);
	HRESULT updateItems(IN BOOL fromServer, IN SOCmnList<SODaCItem> *itemList, IN OPTIONAL DWORD whatParameters = SODAC_ITMPAR_ALL);

	// item creation
	SODaCItem *addItem(IN OPCITEMATTRIBUTES *itemAttr);
	SODaCItem *addItem(IN LPCTSTR itemID, IN OPTIONAL VARTYPE reqDatatype = VT_EMPTY, 
		IN OPTIONAL LPCTSTR accessPath = _T(""));

	// synchronous I/O
	virtual HRESULT syncRead(IN OPCDATASOURCE source, IN OPTIONAL SOCmnList<SODaCItem> *itemList = NULL);
	HRESULT syncWrite(IN SOCmnList<SODaCItem> *itemList);

	// asynchronous I/O
	HRESULT asyncRead(IN OPCDATASOURCE source, IN SOCmnList<SODaCItem> *itemList, 
			IN OPTIONAL DWORD transactionID = 0, OUT OPTIONAL DWORD *cancelID = NULL);
	HRESULT asyncWrite(IN SOCmnList<SODaCItem> *itemList, IN OPTIONAL DWORD transactionID = 0,
			OUT OPTIONAL DWORD *cancelID = NULL);
	HRESULT asyncRefresh(IN OPCDATASOURCE source, IN OPTIONAL DWORD transactionID = 0,
			OUT OPTIONAL DWORD *cancelID = NULL);
	HRESULT asyncCancel(IN DWORD cancelID);

	// advising
	virtual HRESULT advise(void);
	virtual HRESULT unadvise(void);
	virtual BOOL getReadConnection(OUT DWORD *connection);
	virtual BOOL getWriteConnection(OUT DWORD *connection);

	// tree management
	virtual BOOL addLeaf(IN SOCmnElement *newLeaf);
	virtual BOOL addBranch(IN SOCmnElement *newBranch);

	// don't check the server provided item handles
	virtual BOOL trustServerItemHandles(void);

	// serialization
	virtual BOOL serialize(IN OUT SOCltStorage& ar, IN BOOL isStoring);

	// get child object by name
	SODaCItem *getItem(IN LPCTSTR itemID);

protected:
	DWORD m_reqUpdateRate;			// requested update rate in ms 
	DWORD m_revUpdateRate;			// revised update rate in ms 
	LONG m_timeBias;				// time zone
	FLOAT m_deadBand;				// deadband in percent
	DWORD m_lcid;					// local Id
	OPCHANDLE m_serverHandle;		// server handle 
	DWORD m_validParameter;			// valid parameters
	BYTE m_public;					// public 
	BYTE m_isPublic;				// is public group?
	BYTE m_connectionType;			// connection type of the group
	BYTE m_dataRetrieval;			// data retrival method of the group

	BYTE m_active;					// activation state of the group
	BYTE m_asyncDirty;				// asynchronous data to copy
	BYTE m_correctNTtimer;			// NT timer correction value
	ULONG m_span;					// time in ms untill the start of the next cyclic read
	DWORD m_initTime;				// init time of the cyclic read in ms since system start
	SODaCAdviseSink *m_adviseSink;	// advise sink for asynchronous results OPC V1.0A
	SODaCDataCallback *m_dataCallback;// advise sink for asynchronous results OPC V2.0

	SOCltInterfaceAccess<IUnknown, &IID_IUnknown> m_unknown;	
	SOCltInterfaceAccess<IOPCItemMgt, &_ATL_IIDOF(IOPCItemMgt)> m_itemMgt;
	SOCltInterfaceAccess<IOPCSyncIO, &_ATL_IIDOF(IOPCSyncIO)> m_syncIO;
	SOCltInterfaceAccess<IOPCAsyncIO, &_ATL_IIDOF(IOPCAsyncIO)> m_asyncIO;
	SOCltInterfaceAccess<IOPCAsyncIO2, &_ATL_IIDOF(IOPCAsyncIO2)> m_asyncIO2;	
	SOCltInterfaceAccess<IOPCGroupStateMgt, &_ATL_IIDOF(IOPCGroupStateMgt)> m_grpState;	
	SOCltInterfaceAccess<IOPCPublicGroupStateMgt, &_ATL_IIDOF(IOPCPublicGroupStateMgt)> m_grpStatePub;	

	CComPtr<IMalloc> m_malloc;		// IMalloc interface
	WORD m_initCnt;					// init cyclic transaction counter

	virtual ~SODaCGroup(void);

	// data member callbacks
	virtual void onSetReqUpdateRate(void);
	virtual void onSetRevUpdateRate(void);
	virtual void onSetTimeBias(void);
	virtual void onSetDeadBand(void);
	virtual void onSetLCID(void);
	virtual void onSetPublic(void);

	// I/O callbacks
	virtual void onDataChangeBegin(void);
	virtual void onDataChangeEnd(void);

	virtual void onSyncRead(IN OPCDATASOURCE source, IN SOCmnList<SODaCItem> *itemList, IN HRESULT res);
	virtual void onReadAdvise(IN BYTE *data);
	virtual void onWriteAdvise(IN BYTE *data);
	virtual void onReadAdvise(IN DWORD dwTransid, IN OPCHANDLE hGroup,
            IN HRESULT hrMasterquality, IN HRESULT hrMastererror,
            IN DWORD dwCount, IN OPCHANDLE *phClientItems,
            IN VARIANT *pvValues, IN WORD *pwQualities, IN FILETIME *pftTimeStamps,
            IN HRESULT *pErrors);
	virtual void onWriteAdvise(IN DWORD dwTransid,IN OPCHANDLE hGroup,
            IN HRESULT hrMastererr, IN DWORD dwCount, 
			IN OPCHANDLE *pClienthandles, IN HRESULT *pErrors);
	virtual void onCancelAdvise(IN DWORD dwTransid, IN OPCHANDLE hGroup);

	// set the target state of the items of the group
	void setItemsTargetState(IN BYTE targetState);
	// do the state transitions for the items of the group
	virtual HRESULT doItemsStateTransition(void);

	// group management
	virtual HRESULT doUpdate(IN BOOL fromServer, IN OPTIONAL DWORD whatParameters = SODAC_GRPPAR_ALL_MGT);

	// do the state transitions
	virtual HRESULT doStateTransition(BOOL deep);

	// do the diconnection of the group (helper of doStateTransition)
	HRESULT doDisconnect(void);

	// create item proxies for existing group (Get...GroupByName)
	HRESULT createItems(void);	

	// query all interfaces
	HRESULT queryAllInterfaces(IUnknown* punknown);	

	// item list helper
	HRESULT fillServerHandles(IN SOCmnList<SODaCItem> *itemList, OUT DWORD *numItems,
		OUT OPCHANDLE **handles);

	virtual SOCmnObject *getObjectCmnElementIName(void);
	virtual SOCmnObject *getObjectCmnElementIList(void);

	DWORD m_cookieSyncRead;		// task for cyclic reads

}; // SODaCGroup

inline OPCHANDLE SODaCGroup::getClientHandle(void) const
{ return (OPCHANDLE)this; }

inline OPCHANDLE SODaCGroup::getServerHandle(void) const
{ return m_serverHandle; }

inline LONG SODaCGroup::getTimeBias(void) const
{ return m_timeBias; }

inline FLOAT SODaCGroup::getDeadBand(void) const
{ return m_deadBand; }

inline DWORD SODaCGroup::getLCID(void) const
{ return m_lcid; }

inline BOOL SODaCGroup::getPublic(void) const
{ return m_public; }

inline SODaCGroup::connectionType SODaCGroup::getConnectionType(void)
{ return (SODaCGroup::connectionType)m_connectionType; }

inline BOOL SODaCGroup::setConnectionType(IN SODaCGroup::connectionType conType)
{ m_connectionType = (BYTE)conType; return TRUE; }

inline SODaCGroup::dataRetrieval SODaCGroup::getDataRetrieval(void)
{ return (SODaCGroup::dataRetrieval)m_dataRetrieval; }

inline BOOL SODaCGroup::setDataRetrieval(IN SODaCGroup::dataRetrieval dataRet)
{ m_dataRetrieval = (BYTE)dataRet; return TRUE; }

inline DWORD SODaCGroup::getValidParameters(void) const
{ return m_validParameter; }

inline void SODaCGroup::resetValidParameters(void) 
{ m_validParameter = 0; }

#define ClientGroup SODaCGroupDefault


//-----------------------------------------------------------------------------
// SODaCGroupDefault                                                          |
//-----------------------------------------------------------------------------

class SODAC_EXPORT SODaCGroupDefault :  
	public SODaCGroup,
	virtual public SOCltElementErrorStore
{
public:
	SODaCGroupDefault(IN OPTIONAL WORD itemListType = SOCMNLIST_TYPE_ORG_LIST);  

protected:
	virtual SOCmnObject *getObjectCltElementIError(void);
};


//-----------------------------------------------------------------------------
// SODaCTaskSyncRead                                                          |
//-----------------------------------------------------------------------------

class SODAC_EXPORT SODaCTaskSyncRead : public SOCltCyclicTask
{
public:
  SODaCTaskSyncRead(IN SOCltElement* element, IN DWORD cycle);
	virtual void execute();
}; // SOCltTaskStateTransition

#pragma pack(pop)
#endif 

