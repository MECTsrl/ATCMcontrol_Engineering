//-----------------------------------------------------------------------------
//                                                                            |



//                                                                            |



//                                                                            |
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                           OPC TOOLBOX - SOAeC                              |
//                                                                            |
//  Filename    : SOAeCServer.h                                               |
//  Version     : 3.10.release                                                |
//  Date        : 4-March-2002                                                |
//                                                                            |
//  Description : Server object related classes                               |
//                - SOAeCServer: server object                                |
//                - SOAeCServerDefault: default server object class           |
//                - SOAeCConditionState: condition state data                 |
//                - SOAeCSubConditionState: subcondition state data           |
//                - SOAeCTranslateResult: translate to item id data           |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _SOAECSERVER_H_
#define _SOAECSERVER_H_

#pragma pack(push,4)

class SOAeCEntry;
class SOAeCServer;
class SOAeCSubscription;
class SOAeCEvent;
class SOAeCTranslateResult;
class SOAeCAreaBrowser;
class SOAeCEventBrowser;

//-----------------------------------------------------------------------------
// SOAeCSubConditionState                                                     |
//-----------------------------------------------------------------------------

class SOAEC_EXPORT SOAeCSubConditionState : public SOCmnData
{
public:
	SOAeCSubConditionState();
	virtual ~SOAeCSubConditionState();

	SOCmnString m_name;
  SOCmnString m_definition;
  DWORD m_severity;
  SOCmnString m_description;
};




//-----------------------------------------------------------------------------
// SOAeCConditionState                                                        |
//-----------------------------------------------------------------------------

class SOAEC_EXPORT SOAeCConditionState : public SOCmnData
{
public:
	SOAeCConditionState();
	virtual ~SOAeCConditionState();

	WORD m_state;
	SOAeCSubConditionState	m_asc; // active subcondition
	WORD m_quality;
	FILETIME m_lastAckTime;
	FILETIME m_subCondLastActive;
	FILETIME m_condLastActive;
	FILETIME m_condLastInactive;
	SOCmnString m_acknowledgerID;
	SOCmnString m_comment;
	SOCmnList<SOAeCSubConditionState> m_subconds;
	SOCmnList<SOCmnVariant> m_eventAttrs;
};




//-----------------------------------------------------------------------------
// SOAeCServer                                                                |
//-----------------------------------------------------------------------------

// server errors

#define SOAEC_ERR_SRV_CREATE_SERVER		    0xE0401101		
#define SOAEC_ERR_SRV_GET_ERROR_STRING	  0xE0401102
#define SOAEC_ERR_SRV_GET_STATUS		      0xE0401103
#define SOAEC_ERR_SRV_QUERY_ORG			      0xE0401104
#define SOAEC_ERR_SRV_CHANGE_BRS_POS	    0xE0401105
#define SOAEC_ERR_SRV_BRS_ITEMIDS		      0xE0401106
#define SOAEC_ERR_SRV_GET_ITEMID		      0xE0401107
#define SOAEC_ERR_SRV_BRS_ACCESS_PATHS	  0xE0401108
#define SOAEC_ERR_SRV_LOAD				        0xE0401109
#define SOAEC_ERR_SRV_SAVE				        0xE040110A
#define SOAEC_ERR_SRV_SAVE_COMPLETED	    0xE040110B
#define SOAEC_ERR_SRV_GET_CUR_FILE		    0xE040110C
#define SOAEC_ERR_SRV_GET_LOCALE_ID		    0xE040110D
#define SOAEC_ERR_SRV_SET_LOCALE_ID		    0xE040110E
#define SOAEC_ERR_SRV_SET_CLIENT_NAME	    0xE040110F
#define SOAEC_ERR_SRV_QUERY_LCIDS		      0xE0401111
#define SOAEC_ERR_SRV_QUERY_PROPERTIES	  0xE0401112
#define SOAEC_ERR_SRV_GET_PROP_VALUE	    0xE0401113
#define SOAEC_ERR_SRV_GET_PROP_ITEMID	    0xE0401114
#define SOAEC_ERR_PAR_OUTPUT       		    0xE0401115
#define SOAEC_ERR_OUT_OF_MEMORY    		    0xE0401116
#define SOAEC_ERR_COM_QI           		    0xE0401117

#define SOAEC_INF_SRV_VERSION			        0x20401001


// supported opc version
#define SOAEC_VERSION_OPC_AE10          	0x0010


class SOAEC_EXPORT SOAeCServer 
  :   public SOCltServer, 
      virtual public SOCmnElementListLeaf
{
friend SOAeCSubscription;

public:
	SOAeCServer(IN OPTIONAL WORD subsListType = SOCMNLIST_TYPE_ORG_LIST);  

public:
	// serialization
	virtual BOOL serialize(IN OUT SOCltStorage& ar, IN BOOL isStoring);

	// OPC server version
	WORD getSupportedOPCVersion(void);

	// IOPCEventServer	interface
	HRESULT getStatus(OUT OPCEVENTSERVERSTATUS *serverStatus, OUT SOCmnString *vendorInfo);
	HRESULT enableConditionByArea(IN SOCmnStringList* areas,IN BOOL enable);
	HRESULT enableConditionByArea(IN LPCTSTR area,IN BOOL enable);
	HRESULT enableConditionBySource(IN SOCmnStringList* sources,IN BOOL enable);
	HRESULT enableConditionBySource(IN LPCTSTR source,IN BOOL enable);

	BOOL ackConditions(IN SOCmnList<SOAeCEvent> *events, // SOCMNLIST_TYPE_ORG_LIST | SOCMNLIST_TYPE_OBJ_OBJECT
										IN OPTIONAL LPCTSTR acknowledgerID = NULL,
										IN OPTIONAL LPCTSTR comment = NULL, 
										IN OPTIONAL HANDLE handle = SOCLT_EXEC_AUTO_SYNC);

	HRESULT translateToItemIDs(
										IN LPCTSTR source, 
										IN DWORD eventCategory, 
										IN LPCTSTR conditionName,
										IN LPCTSTR subconditionName,
										IN SOCmnDWORDList* assocAttrIDs,
										OUT SOCmnList<SOAeCTranslateResult>& translateResults);

	HRESULT getConditionState( IN LPCTSTR source, IN LPCTSTR condName, 
														 IN OPTIONAL SOCmnDWORDList* eventAttrIDs, 
														 OUT SOAeCConditionState& state);

	// query COM interface
	virtual HRESULT queryInterface(IN REFIID riid, OUT void **interf);

	// subscription creation
	SOAeCSubscription *addSubscription(void);

	// tree management
	virtual BOOL addLeaf(IN SOCmnElement *newLeaf);
	virtual BOOL addBranch(IN SOCmnElement *newBranch);

	// Browsing
	SOAeCAreaBrowser* getAreaBrowser(void);
	SOAeCEventBrowser* getEventBrowser(void);


protected:
	WORD m_version;					    // supported OPC version of the server

	SOCltInterfaceAccess<IOPCEventServer, &_ATL_IIDOF(IOPCEventServer)> m_server;		  // IOPCServer interface of server object

protected:

	virtual HRESULT doStateTransition(BOOL deep);
  virtual HRESULT doUpdate(IN BOOL fromServer, IN OPTIONAL DWORD whatParameters);
	virtual HRESULT doDisconnect(void);

protected:
	virtual ~SOAeCServer(void);
}; // SOAeCServer

inline WORD SOAeCServer::getSupportedOPCVersion(void)
{ return m_version; }




//-----------------------------------------------------------------------------
// SOAeCServerDefault                                                         |
//-----------------------------------------------------------------------------

class SOAEC_EXPORT SOAeCServerDefault :  
	public SOAeCServer,
	virtual public SOCltElementErrorStore
{
public:
	SOAeCServerDefault(IN OPTIONAL WORD groupListType = SOCMNLIST_TYPE_ORG_LIST);  

protected:
	virtual SOCmnObject *getObjectCltElementIError(void);
};




//-----------------------------------------------------------------------------
// SOAeCTranslateResult                                                       |
//-----------------------------------------------------------------------------

class SOAEC_EXPORT SOAeCTranslateResult : public SOCmnData
{
public:
	SOAeCTranslateResult();
	virtual ~SOAeCTranslateResult();

	SOCmnString m_attrItemID;
	SOCmnString m_nodeName;
	CLSID m_clsid;
};


#pragma pack(pop)
#endif 
