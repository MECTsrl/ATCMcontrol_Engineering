//-----------------------------------------------------------------------------
//                                  OPC ENGINE                                |
//                                                                            |
//  Filename    : GENITEM.H                                                   |   
//  Version     : 2.01.0.00.release                                           |
//  Date        : 7-May-1999                                                  |
//                                                                            |
//  Description : Item object related classes                                 |
//                - GenericItem: OPC item object                              |
//                                                                            |
//  CHANGE_NOTES                                                              |
//                                                                            |
//  date      name      change                                                |
//  ------------------------------------------------------------------------- |
//  01/08/99  RT        moved queue data from item to item tag                |
//  04/23/99  RT        moved cyclic data form the request to the item        |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _GENITEM_H_
#define _GENITEM_H_

#include <opcda.h>
#include "GenTree.h"
#include "GenNS.h"
#include "GenReq.h"
#include "GenGrp.h"

//-----------------------------------------------------------------------------
// GenericItem                                                                |
//-----------------------------------------------------------------------------

extern CCriticalSection g_cycDataLock;

// cyclic read data
typedef struct _tagGenericItemCyclicData
{
	SendValuesMap m_sendValues;			// previous value send to the OPC clients (only for cyclic)
	SendQualitiesMap m_sendQualities;	// previous quality send to the OPC client (only for cyclic)
} GenericItemCyclicData;

class OPCENGINE_EXPORT GenericItem : public GenericLeaf
{
friend GenericRequest;
friend GenericGroup;
friend GenericServer;

public:
	GenericItem(); 

	// cache object
	GenericCache *getCache(void);

	// item tag object
	virtual BOOL setItemTag(IN GenericItemTag *tag);
	GenericItemTag *getItemTag(void);

	// activation state
	virtual BOOL setActive(IN BOOL active);
	BOOL getActive(void) const;
	BOOL isActive(void) const;

	// client handle from OPC client
	virtual BOOL setClientHandle(IN OPCHANDLE clientHandle);
	OPCHANDLE getClientHandle(void) const;

	// activation state
	virtual HRESULT setReqDatatype(IN VARTYPE reqDatatype);
	VARTYPE getReqDatatype(void) const;

	// access path
	virtual BOOL setAccessPath(IN LPCTSTR accessPath);
	CString getAccessPath(void) const;

	// server handle
	OPCHANDLE getServerHandle(void) const;

	// object trace id
	virtual LPCTSTR getObjTraceId(void);

protected:
	~GenericItem();

	GenericItemTag *m_itemTag;	// tag object
	OPCHANDLE m_clientHandle;	// client handle
	CString m_accessPath;		// access path from client
	OPCHANDLE m_serverHandle;	// server handle
	VARTYPE m_reqDatatype;		// requested data type
	BYTE m_active;				// activation state
	GenericItemCyclicData *m_cycData;	// cyclic data of the item (e.g. send values)

	BOOL setServerHandle(IN OPCHANDLE serverHandle);

	GenericItemCyclicData *getCyclicData(void);
	void setCyclicData(IN GenericItemCyclicData *cycData);
	void lockCyclicData(void);	
	void unlockCyclicData(void);	

	// callbacks
	virtual void onRead(IN BOOL deviceRead);
	virtual void onWrite(void);
	virtual void onSetActive(IN BOOL active);
	virtual void onSetClientHandle(IN OPCHANDLE clientHandle);
	virtual void onSetReqDatatype(IN VARTYPE requestedType);
	virtual void onSetAccessPath(IN LPCTSTR accessPath);
	virtual void onSetItemTag(IN GenericItemTag *tag);

}; // GenericItem


inline BOOL GenericItem::getActive(void) const
{ return (BOOL)m_active; }

inline BOOL GenericItem::isActive(void) const
{ BOOL parAct = FALSE; if (m_parent) parAct = ((GenericGroup *)m_parent)->getActive(); return (m_active && parAct); }

inline OPCHANDLE GenericItem::getClientHandle(void) const
{ return m_clientHandle; }

inline VARTYPE GenericItem::getReqDatatype(void) const
{ return m_reqDatatype; }

inline CString GenericItem::getAccessPath(void) const
{ return m_accessPath; }

inline BOOL GenericItem::setServerHandle(IN OPCHANDLE serverHandle)
{ m_serverHandle = serverHandle; return TRUE; }

inline OPCHANDLE GenericItem::getServerHandle(void) const
{ return m_serverHandle; }

inline GenericItemCyclicData *GenericItem::getCyclicData(void)
{ return m_cycData; }

inline void GenericItem::lockCyclicData(void)
{ g_cycDataLock.Lock(); }

inline void GenericItem::unlockCyclicData(void)
{ g_cycDataLock.Unlock(); }

#endif
