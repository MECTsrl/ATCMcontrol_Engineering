//-----------------------------------------------------------------------------
//                                                                            |



//                                                                            |



//                                                                            |
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                           OPC TOOLBOX - SODaS                              |
//                                                                            |
//  Filename    : SODaSItem.h                                                 |
//  Version     : 3.10.release                                                |
//  Date        : 4-March-2002                                                |
//                                                                            |
//  Description : OPC Item object related classes                             |
//                - SODaSItem: OPC DA item object                             |
//                - SODaSItemICyclic: Cyclic data interface of the item       |
//                - SODaSItemCyclicOne: Only one client cyclci data support   |
//                - SODaSItemCyclicFull: Full cyclic data support             |
//                - SODaSItemDefault: Default configuration for item objects  |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _SODASITEM_H_
#define _SODASITEM_H_

#pragma pack(push,4)

#include <opcda.h>
#include "SOCmnElement.h"
#include "SODaSItemTag.h"
#include "SODaSRequest.h"
#include "SODaSGroup.h"

//-----------------------------------------------------------------------------
// SODaSItemICyclic                                                           |
//-----------------------------------------------------------------------------

class SODAS_EXPORT SODaSItemICyclic
{
friend SODaSTransaction;
protected:
	SODaSItemICyclic();
	virtual ~SODaSItemICyclic();

	virtual void createCyclicData(void);
	virtual void destroyCyclicData(void);

	// last value send to the OPC client
	virtual BOOL setSendValue(IN DWORD connection, IN LPCVARIANT value, IN WORD quality);
	virtual BOOL getSendValue(IN DWORD connection, OUT LPVARIANT value, OUT WORD *quality);
}; // SODaSItemICyclic


//-----------------------------------------------------------------------------
// SODaSItemCyclicFull                                                        |
//-----------------------------------------------------------------------------

class SODAS_EXPORT SODaSItemCyclicFull : virtual public SODaSItemICyclic
{
protected:
	SOCmnList<SOCmnVariant> m_sendValues;	// previous values send to the OPC clients
	SOCmnDWORDList m_sendQualities;			// previous qualities send to the OPC client

	SODaSItemCyclicFull();
	virtual ~SODaSItemCyclicFull();

	virtual void createCyclicData(void);
	virtual void destroyCyclicData(void);

	// last value send to the OPC client
	virtual BOOL setSendValue(IN DWORD connection, IN LPCVARIANT value, IN WORD quality);
	virtual BOOL getSendValue(IN DWORD connection, OUT LPVARIANT value, OUT WORD *quality);
}; // SODaSItemCyclicFull


//-----------------------------------------------------------------------------
// SODaSItemCyclicOne                                                         |
//-----------------------------------------------------------------------------

class SODAS_EXPORT SODaSItemCyclicOne : virtual public SODaSItemICyclic
{
protected:
	WORD m_sendQuality;			// previous quality send to the OPC client 
	SOCmnVariant m_sendValue;	// previous value send to the OPC clients 

	SODaSItemCyclicOne();

	virtual void destroyCyclicData(void);

	// last value send to the OPC client
	virtual BOOL setSendValue(IN DWORD connection, IN LPCVARIANT value, IN WORD quality);
	virtual BOOL getSendValue(IN DWORD connection, OUT LPVARIANT value, OUT WORD *quality);
}; // SODaSItemCyclicOne




//-----------------------------------------------------------------------------
// SODaSItem                                                                  |
//-----------------------------------------------------------------------------

class SODAS_EXPORT SODaSItem : 
	public SOCmnElement,
	virtual public SODaSItemICyclic
{
friend SODaSRequest;
friend SODaSGroup;
friend SODaSServer;
friend SODaSTransaction;

public:
	SODaSItem(); 

	// watch overload 
	virtual BOOL setParentBranch(IN SOCmnElement *parent);

	// cache object
	SODaSCache *getCache(void);

	// server object
	SODaSServer *getServer(void);

	// item tag object
	virtual BOOL setItemTag(IN SODaSItemTag *tag);
	SODaSItemTag *getItemTag(void);
	virtual SOCmnString getItemID(void);

	// activation state
	virtual BOOL setActive(IN BOOL active);
	BOOL getActive(void) const;
	BOOL isActive(void);

	// client handle from OPC client
	virtual BOOL setClientHandle(IN OPCHANDLE clientHandle);
	OPCHANDLE getClientHandle(void) const;

	// requested datatype
	virtual HRESULT setReqDatatype(IN VARTYPE reqDatatype);
	VARTYPE getReqDatatype(void) const;

	// access path
	virtual BOOL setAccessPath(IN LPCTSTR accessPath);
	virtual SOCmnString getAccessPath(void);

	// server handle
	OPCHANDLE getServerHandle(void) const;

	// object trace id
	virtual LPCTSTR getObjTraceId(void);

	// objects watch data
	virtual void getObjWatchData(IN void *pXMLDocumentInterface, IN void *pXMLParentNodeInterface, 
			IN BOOL withAttributes,	IN LPWSTR objNodeName, OPTIONAL OUT void **pXMLObjectNodeInterface = NULL);

protected:
	virtual ~SODaSItem();

	SODaSItemTag *m_itemTag;	// tag object
	OPCHANDLE m_clientHandle;	// client handle
	VARTYPE m_reqDatatype;		// requested data type
	BYTE m_active;				// activation state

	// callbacks
	virtual void onRead(IN BOOL deviceRead);
	virtual void onWrite(void);
	virtual void onSetActive(IN BOOL active);
	virtual void onSetClientHandle(IN OPCHANDLE clientHandle);
	virtual void onSetReqDatatype(IN VARTYPE requestedType);
	virtual void onSetItemTag(IN SODaSItemTag *tag);
}; // SODaSItem


inline BOOL SODaSItem::getActive(void) const
{ return (BOOL)m_active; }

inline BOOL SODaSItem::isActive(void) 
{ SOCmnPointer<SODaSGroup> grp = (SODaSGroup  *)getParentBranch(); if (grp.isNotNull()) return ((m_active) && (grp->getActive())); else return FALSE; }

inline OPCHANDLE SODaSItem::getClientHandle(void) const
{ return m_clientHandle; }

inline VARTYPE SODaSItem::getReqDatatype(void) const
{ return m_reqDatatype; }

inline OPCHANDLE SODaSItem::getServerHandle(void) const
{ return (OPCHANDLE)this; }

#define GenericItem SODaSItemDefault




//-----------------------------------------------------------------------------
// SODaSItemDefault                                                           |
//-----------------------------------------------------------------------------

class SODAS_EXPORT SODaSItemDefault : 
	public SODaSItem,
#ifndef SO_WINCE
	virtual public SODaSItemCyclicFull
#else
	virtual public SODaSItemCyclicOne
#endif
{
public:
	SODaSItemDefault(void);
}; // SODaSItemDefault

#pragma pack(pop)
#endif
