//-----------------------------------------------------------------------------
//                                                                            |



//                                                                            |



//                                                                            |
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                           OPC TOOLBOX - SODaC                              |
//                                                                            |
//  Filename    : SODaCItem.h                                                 |
//  Version     : 3.10.release                                                |
//  Date        : 4-March-2002                                                |
//                                                                            |
//  Description : Item object related classes                                 |
//                - SODaCItem: proxy for OPC item object                      |
//                - SODaCItemIAttr: interface class for item attributes       |
//                - SODaCItemAttrFull: full item attributes support           |
//                - SODaCItemAttrMin: minimal item attributes support         |
//                - SODaCItemIData: interface class for item I/O data         |
//                - SODaCItemDataFull: full item I/O data support             |
//                - SODaCItemDefault: default item class for creator          |
//                - SODaCItemStateTransitionTask: item state transition task  |
//                - SODaCItemTaskUpdate: item attribute value update task     |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _SODACITEM_H_
#define _SODACITEM_H_

#include "SOCltElement.h"
#include "SODaCEntry.h"

#pragma pack(push,4)

//-----------------------------------------------------------------------------
// SODaCItemIAttr                                                             |
//-----------------------------------------------------------------------------

class SODaCGroup;

class SODAC_EXPORT SODaCItemIAttr
{
	friend SODaCGroup;
public:
	SODaCItemIAttr();

	// item data
	virtual BOOL setAccessPath(LPCTSTR accessPath);
	virtual SOCmnString	getAccessPath(void);

	virtual BOOL setItemID(LPCTSTR itemID);
	virtual SOCmnString	getItemID(void);

	virtual VARTYPE getReqDatatype(void);
	virtual BOOL setReqDatatype(IN VARTYPE datatype);

	virtual VARTYPE getNativeDatatype(void);
	virtual DWORD getAccessRights(void);
	virtual BOOL getEU(OUT OPCEUTYPE *type, OUT LPVARIANT info);
	virtual OPCHANDLE getClientHandle(void);

protected:
	virtual ~SODaCItemIAttr();

	// server provided item data
	virtual BOOL setNativeDatatype(IN VARTYPE datatype);
	virtual BOOL setAccessRights(IN DWORD accessRights);
	virtual BOOL setEU(IN OPCEUTYPE type, IN LPCVARIANT info);

	// data member callbacks
	virtual void onSetReqDatatype(void);
	virtual void onSetAccessPath(void);
	virtual void onSetItemID(void);
	virtual void onSetNativeDatatype(void);
	virtual void onSetAccessRights(void);
	virtual void onSetEU(void);

	virtual LPCTSTR getIntTraceId(void);
	virtual SOCmnObject *getObjectDaCItemIAttr(void);
}; // SODaCItemIAttr


//-----------------------------------------------------------------------------
// SODaCItemAttrFull                                                          |
//-----------------------------------------------------------------------------

class SODAC_EXPORT SODaCItemAttrFull : virtual public SODaCItemIAttr
{
public:
	SODaCItemAttrFull();

	// item data
	virtual BOOL setAccessPath(LPCTSTR accessPath);
	virtual SOCmnString	getAccessPath(void);

	virtual BOOL setItemID(LPCTSTR itemID);
	virtual SOCmnString	getItemID(void);

	virtual VARTYPE getReqDatatype(void);
	virtual BOOL setReqDatatype(IN VARTYPE datatype);

	virtual VARTYPE getNativeDatatype(void);
	virtual DWORD getAccessRights(void);
	virtual BOOL getEU(OUT OPCEUTYPE *type, OUT LPVARIANT info);

protected:
	SOCmnString	m_itemID;			// fully quallified item ID
	SOCmnString	m_accessPath;		// access path
	VARTYPE	m_reqDatatype;			// requested datatype
	VARTYPE	m_nativeDatatype;		// native datatype of the namespace tag
	DWORD m_accessRights;			// access rights
	OPCEUTYPE m_EUType;				// engineering unit type
	SOCmnVariant m_EUInfo;			// engineering units

	// server provided item data
	virtual BOOL setNativeDatatype(IN VARTYPE datatype);
	virtual BOOL setAccessRights(IN DWORD accessRights);
	virtual BOOL setEU(IN OPCEUTYPE type, IN LPCVARIANT info);
}; // SODaCItemAttrFull


//-----------------------------------------------------------------------------
// SODaCItemAttrMin                                                           |
//-----------------------------------------------------------------------------

class SODAC_EXPORT SODaCItemAttrMin : virtual public SODaCItemIAttr
{
public:
	virtual BOOL setItemID(LPCTSTR itemID);
	virtual SOCmnString	getItemID(void);
protected:
	SOCmnString	m_itemID;			// fully quallified item ID
}; // SODaCItemAttrMin




//-----------------------------------------------------------------------------
// SODaCItemIData                                                             |
//-----------------------------------------------------------------------------

class SODaCAdviseSink;
class SODaCDataCallback;
class SODaCGroup;

class SODAC_EXPORT SODaCItemIData
{
	friend SODaCAdviseSink;
	friend SODaCDataCallback;
	friend SODaCGroup;

public:
	SODaCItemIData();

	// read value
	virtual HRESULT getReadResult(void) const;
	virtual WORD getReadQuality(void) const;
	virtual BOOL getReadTimeStamp(OUT LPFILETIME timeStamp) const;
	virtual BOOL getReadValue(LPVARIANT value);

	// write value
	virtual HRESULT getWriteResult(void) const;
	virtual BOOL setWriteValue(LPCVARIANT value);
	virtual BOOL getWriteValue(LPVARIANT value);

protected:
	virtual ~SODaCItemIData();

	// read value
	virtual BOOL setReadResult(IN HRESULT res);
	virtual BOOL setReadQuality(IN WORD quality);
	virtual BOOL setReadTimeStamp(IN LPFILETIME timeStamp);
	virtual BOOL setReadValue(IN LPCVARIANT value);

	// write value
	virtual BOOL setWriteResult(IN HRESULT res);

	// I/O callbacks
	virtual void onSetReadResult(void);
	virtual void onSetReadValue(void);
	virtual void onSetReadQuality(void);
	virtual void onSetReadTimeStamp(void);
	virtual void onSetWriteResult(void);
	virtual void onSetWriteValue(void);

	virtual LPCTSTR getIntTraceId(void);
	virtual SOCmnObject *getObjectDaCItemIData(void);
};


//-----------------------------------------------------------------------------
// SODaCItemDataFull                                                          |
//-----------------------------------------------------------------------------

class SODAC_EXPORT SODaCItemDataFull : virtual public SODaCItemIData
{
public:
	SODaCItemDataFull();

	// read value
	virtual HRESULT getReadResult(void) const;
	virtual WORD getReadQuality(void) const;
	virtual BOOL getReadTimeStamp(OUT LPFILETIME timeStamp) const;
	virtual BOOL getReadValue(LPVARIANT value);

	// write value
	virtual HRESULT getWriteResult(void) const;
	virtual BOOL setWriteValue(LPCVARIANT value);
	virtual BOOL getWriteValue(LPVARIANT value);

protected:
	WORD m_readQuality;				// read quality
	HRESULT m_readResult;			// read result
	FILETIME m_readTimeStamp;		// read time stamp
	SOCmnVariant m_readValue;		// read value
	HRESULT m_writeResult;			// write result
	SOCmnVariant m_writeValue; 		// write value

	// read value
	virtual BOOL setReadResult(IN HRESULT res);
	virtual BOOL setReadQuality(IN WORD quality);
	virtual BOOL setReadTimeStamp(IN LPFILETIME timeStamp);
	virtual BOOL setReadValue(IN LPCVARIANT value);

	// write value
	virtual BOOL setWriteResult(IN HRESULT res);
}; // SODaCItemDataFull


//-----------------------------------------------------------------------------
// SODaCItemStateTransitionTask                                               |
//-----------------------------------------------------------------------------

class SODAC_EXPORT SODaCItemStateTransitionTask : public SOCltTaskStateTransition
{
public:
	SODaCItemStateTransitionTask(IN SOCltElement* element, IN BYTE targetState, IN BOOL deep);
	virtual void execute();
}; // SODaCItemStateTransitionTask


//-----------------------------------------------------------------------------
// SODaCItemTaskUpdate                                                        |
//-----------------------------------------------------------------------------

class SODAC_EXPORT SODaCItemTaskUpdate : public SOCltTaskUpdate
{
public:
	SODaCItemTaskUpdate(IN SOCltElement* element, IN BOOL from_server, IN DWORD whatParameters);
	virtual void execute();
};  // SODaCItemTaskUpdate 



//-----------------------------------------------------------------------------
// SODaCItem                                                                  |
//-----------------------------------------------------------------------------

// item update what parameters
#define SODAC_ITMPAR_REQDATATYPE	0x00000001
#define SODAC_ITMPAR_EU				0x00000002
#define SODAC_ITMPAR_ALL			0x00000003

// flag for asyncDirty flag (async received data)
#define SODAC_ITEM_ASYNCDIRTY_NONE			0x00
#define SODAC_ITEM_ASYNCDIRTY_READVALUE		0x01
#define SODAC_ITEM_ASYNCDIRTY_READQUALITY	0x02
#define SODAC_ITEM_ASYNCDIRTY_READTIMESTAMP	0x04
#define SODAC_ITEM_ASYNCDIRTY_READRESULT	0x08
#define SODAC_ITEM_ASYNCDIRTY_WRITERESULT	0x10

class SODaCAdviseSink;
class SODaCDataCallback;
class SODaCGroup;

class SODAC_EXPORT SODaCItem :  
	public SOCltElement,
	virtual public SODaCItemIAttr,
	virtual public SODaCItemIData
{
	friend SODaCGroup;
	friend SODaCAdviseSink;
	friend SODaCDataCallback;

public:
	SODaCItem();

	// start the communication for all items of the group
	virtual BOOL start(IN OPTIONAL BOOL deep = TRUE, IN OPTIONAL HANDLE handle = SOCLT_EXEC_AUTO_SYNC);

	// connect group and create all items
	virtual BOOL connect(IN OPTIONAL BOOL deep = TRUE, IN OPTIONAL HANDLE handle = SOCLT_EXEC_AUTO_SYNC);		

	// disconnect group and remove all items
	virtual BOOL disconnect(IN OPTIONAL HANDLE handle = SOCLT_EXEC_AUTO_SYNC);	

	// item server handle
	OPCHANDLE getServerHandle(void);

	// I/O
	HRESULT read(IN OPCDATASOURCE source);
	HRESULT write(void);

	// item management
	virtual HRESULT update(IN BOOL fromServer, IN OPTIONAL DWORD whatParameters = SODAC_ITMPAR_ALL);
	
	// serialization
	virtual BOOL serialize(IN OUT SOCltStorage& ar, IN BOOL isStoring);

protected:
	OPCHANDLE m_serverHandle;		// server handle of the item
	BYTE m_active;					// activation state

	virtual ~SODaCItem();

	BOOL setServerHandle(IN OPCHANDLE serverHandle);
}; // SODaCItem

inline BOOL SODaCItem::setServerHandle(OPCHANDLE serverHandle)
{ m_serverHandle = serverHandle; return TRUE; }

inline OPCHANDLE SODaCItem::getServerHandle(void)
{ return m_serverHandle; }



#define ClientItem SODaCItemDefault


//-----------------------------------------------------------------------------
// SODaCItemDefault                                                           |
//-----------------------------------------------------------------------------

class SODAC_EXPORT SODaCItemDefault :  
	public SODaCItem,
	virtual public SODaCItemAttrFull,
	virtual public SODaCItemDataFull,
	virtual public SOCltElementErrorStore
{
protected:
	virtual SOCmnObject *getObjectCltElementIError(void);
	virtual SOCmnObject *getObjectDaCItemIData(void);
	virtual SOCmnObject *getObjectDaCItemIAttr(void);
};

#pragma pack(pop)
#endif 

