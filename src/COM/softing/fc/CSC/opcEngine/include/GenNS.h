//-----------------------------------------------------------------------------
//                                  OPC ENGINE                                |
//                                                                            |
//  Filename    : GENNS.H                                                     |   
//  Version     : 2.01.0.00.release                                           |
//  Date        : 7-May-1999                                                  |
//                                                                            |
//  Description : Namespace related classes                                   |
//                - GenericItemTag: base class for tag and tag properties     |
//                - GenericTag: leaf object in process tree                   |
//                - GenericTagProperty: property of tag object                |
//                - GenericNode: branch object in namespace tree              |
//                - OPCRootNS: root object of namespace tree                  |
//                                                                            |
//  CHANGE_NOTES                                                              | 
//                                                                            |
//  date      name      change                                                |
//  ------------------------------------------------------------------------- |
//  12/21/98  RT        added GenericItemTag and GenericTagProperty           |
//  01/08/99  RT        moved queue data from item to item tag                |
//  04/22/99  RT        added report driven protocol support                  |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _GENNS_H_
#define _GENNS_H_

#include <opcda.h>
#include "GenTree.h"
#include "GenReq.h"

class GenericItem;
typedef CTypedPtrList<CPtrList, GenericItem *> ItemList;

class GenericRequest;

//-----------------------------------------------------------------------------
// GenericItemTag                                                             |
//-----------------------------------------------------------------------------

// application data for queued I/O handling
typedef struct _tagGenericItemTagQueueData
{
	HANDLE m_readEvent;				// read start event
	HANDLE m_writeEvent;			// write start event

	RequestList *m_readQueue;		// read queue
	RequestList *m_writeQueue;		// write queue

#ifdef _AFXDLL
	CSyncObject *m_readQueueLock;	// read queue lock
	CSyncObject *m_writeQueueLock;	// write queue lock
#else
	HANDLE m_readQueueLock;			// read queue lock
	HANDLE m_writeQueueLock;		// write queue lock
#endif
} GenericItemTagQueueData;
	

class GenericCache;

class OPCENGINE_EXPORT GenericItemTag : public GenericLeaf
{
friend GenericRequest;
friend GenericGroup;

public:
	// device I/O mode of item tag 
	enum tagDeviceIOMode
	{
		direct = 1,		// direct I/O (calls handleRequest)
		queue,			// queued I/O (uses GenericItemTagQueueData)
		constant		// constant tag value (setConstantValue)
	};

	GenericItemTag(IN DWORD objType); 
	
	// item tag name
	virtual HRESULT setName(IN LPCTSTR name);

	// pointer to cache object
	virtual BOOL setCache(IN GenericCache *cache);
	GenericCache *getCache(void);
	BOOL hasCache(IN GenericCache *cache) const;

	// access rights
	virtual BOOL setAccessRights(IN DWORD rights);
	DWORD getAccessRights(void) const;

	// native datatype
	virtual BOOL setNativeDatatype(IN VARTYPE datatype);
	VARTYPE getNativeDatatype(void) const;

	// datatype conversion
	virtual BOOL supportDatatype(IN VARTYPE dataType);
	virtual HRESULT supportReqDatatype(IN VARTYPE reqDataType);

	// access paths
	virtual BOOL setAccessPaths(IN CStringList *accessPaths);
	void getAccessPaths(OUT CStringList *accessPaths) const;
	virtual BOOL checkAccessPath(IN CString accessPath);

	// I/O mode
	virtual BYTE getDeviceIOMode(void) = 0;
	void setDeviceIOMode(BYTE deviceIOMode);

	// dynamic craeated tag item
	virtual BOOL setDynamic(IN BOOL dynamic);
	BOOL getDynamic(void) const;

	// request handling
	virtual void handleRequest(IN GenericRequest *request);

	// constant item tag value (sets I/O mode to constant)
	BOOL setConstantValue(LPVARIANT value);

	// report handling
	DWORD notifyAllItems(void);
	void notifyItems(IN ItemList &itemList);
	virtual void getAllItems(OUT ItemList &itemList);

	// read queue
	BOOL setReadQueue(IN RequestList *queue);
	RequestList *getReadQueue(void) const;
	BOOL setReadEvent(IN HANDLE event);
	HANDLE getReadEvent(void) const;

	// write queue
	BOOL setWriteQueue(IN RequestList *queue);
	RequestList *getWriteQueue(void) const;
	BOOL setWriteEvent(IN HANDLE event);
	HANDLE getWriteEvent(void) const;

#ifdef _AFXDLL
	BOOL setReadQueueLockObj(IN CSyncObject *sync);
	CSyncObject *getReadQueueLockObj(void) const;
	BOOL setWriteQueueLockObj(IN CSyncObject *sync);
	CSyncObject *getWriteQueueLockObj(void) const;
#else
	BOOL setReadQueueLockHandle(IN HANDLE mutex);
	HANDLE getReadQueueLockHandle(void) const;
	BOOL setWriteQueueLockHandle(IN HANDLE mutex);
	HANDLE getWriteQueueLockHandle(void) const;
#endif

protected:
	~GenericItemTag(void); 

	GenericCache *m_cache;		// cache object
	DWORD m_accessRights;		// access rights
	CStringList *m_accessPaths;	// access paths
	VARTYPE m_nativeDatatype;	// native datatype
	BYTE m_deviceIOMode;		// device I/O mode
	BYTE m_dynamic;				// dynamic created tag
	GenericItemTagQueueData *m_queueData;
								// queued I/O mode data

	// add to queue 
	virtual BOOL addToQueue(IN GenericRequest *req, IN BOOL readQueue);
	virtual BOOL signalEvent(IN BOOL readEvent);
	virtual void addToRequestList(IN RequestList *queue, 
		IN GenericRequest *req);
	void createQueueData(void);

	// callbacks
	virtual void onSetAccessRights(IN DWORD rights);
	virtual void onSetNativeDatatype(IN VARTYPE datatype);
	virtual void onSetAccessPaths(IN CStringList *accessPaths);

}; // GenericItemTag

inline BOOL GenericItemTag::hasCache(GenericCache *cache) const
{ return (m_cache == cache); }

inline DWORD GenericItemTag::getAccessRights(void) const
{ return m_accessRights; }

inline VARTYPE GenericItemTag::getNativeDatatype(void) const
{ return m_nativeDatatype; }

inline void GenericItemTag::setDeviceIOMode(BYTE deviceIOMode)
{ m_deviceIOMode = deviceIOMode; }

inline BOOL GenericItemTag::getDynamic(void) const
{ return m_dynamic; }

inline BOOL GenericItemTag::setReadEvent(IN HANDLE event)
{  createQueueData(); m_queueData->m_readEvent = event; return TRUE; }

inline HANDLE GenericItemTag::getReadEvent(void) const
{ if (m_queueData) return m_queueData->m_readEvent; else return INVALID_HANDLE_VALUE; }

inline BOOL GenericItemTag::setWriteEvent(IN HANDLE event)
{ createQueueData(); m_queueData->m_writeEvent = event; return TRUE; }

inline HANDLE GenericItemTag::getWriteEvent(void) const
{ if (m_queueData) return m_queueData->m_writeEvent; else return INVALID_HANDLE_VALUE; }

inline BOOL GenericItemTag::setReadQueue(IN RequestList *queue)
{ createQueueData(); m_queueData->m_readQueue = queue; return TRUE; }

inline RequestList *GenericItemTag::getReadQueue(void) const
{ if (m_queueData) return m_queueData->m_readQueue; else return NULL; }

inline BOOL GenericItemTag::setWriteQueue(IN RequestList *queue)
{ createQueueData(); m_queueData->m_writeQueue = queue; return TRUE; }

inline RequestList *GenericItemTag::getWriteQueue(void) const
{ if (m_queueData) return m_queueData->m_writeQueue; else return NULL; }

#ifdef _AFXDLL

inline CSyncObject *GenericItemTag::getReadQueueLockObj(void) const
{ if (m_queueData) return m_queueData->m_readQueueLock; else return NULL; }

inline BOOL GenericItemTag::setReadQueueLockObj(IN CSyncObject *sync)
{ createQueueData(); m_queueData->m_readQueueLock = sync; return TRUE; }

inline BOOL GenericItemTag::setWriteQueueLockObj(IN CSyncObject *sync)
{ createQueueData(); m_queueData->m_writeQueueLock = sync; return TRUE; }

inline CSyncObject *GenericItemTag::getWriteQueueLockObj(void) const
{ if (m_queueData) return m_queueData->m_writeQueueLock; else return NULL; }

#else

inline HANDLE GenericItemTag::getReadQueueLockHandle(void) const
{ if (m_queueData) return m_queueData->m_readQueueLock; else return INVALID_HANDLE_VALUE; }

inline BOOL GenericItemTag::setReadQueueLockHandle(IN HANDLE mutex)
{ createQueueData(); m_queueData->m_readQueueLock = mutex; return TRUE; }

inline BOOL GenericItemTag::setWriteQueueLockHandle(IN HANDLE mutex)
{ createQueueData(); m_queueData->m_writeQueueLock = mutex; return TRUE; }

inline HANDLE GenericItemTag::getWriteQueueLockHandle(void) const
{ if (m_queueData) return m_queueData->m_writeQueueLock; else return INVALID_HANDLE_VALUE; }

#endif

typedef CTypedPtrList<CPtrList, GenericItemTag *> ItemTagList;




//-----------------------------------------------------------------------------
// GenericTagProperty                                                         |
//-----------------------------------------------------------------------------

// data of tag property
typedef struct _tagOPCTagPropertyData
{
	DWORD m_pid;		// property id
	LPTSTR m_descr;		// description
	VARTYPE m_datatype;	// data type
} OPCTagPropertyData;

typedef CTypedPtrList<CPtrList, OPCTagPropertyData *> TagPropertyDataList;

class GenericTag;

class OPCENGINE_EXPORT GenericTagProperty : public GenericItemTag
{
public:
	GenericTagProperty(IN GenericTag *tag, IN DWORD propertyId); 

	// object identity
	virtual CString getKey(void);
	virtual BOOL hasKey(LPCTSTR key);

	void getPropertyData(OPCTagPropertyData *propData);

	// property id
	DWORD getPropertyId(void) const;

	// tag 
	GenericTag *getTag(void);

	// request handling
	virtual void handleRequest(IN GenericRequest *request);
	virtual BYTE getDeviceIOMode(void);

	// special release method
	virtual LONG release(void);	

protected:
	~GenericTagProperty(); 

	GenericTag *m_tag;		// tag object
	DWORD m_propertyId;		// property id

	BOOL handleStandardProperties(IN GenericRequest *request);
}; // GenericTagProperty

inline DWORD GenericTagProperty::getPropertyId(void) const
{ return m_propertyId; }

typedef CTypedPtrList<CPtrList, GenericTagProperty *> TagPropertyList;




//-----------------------------------------------------------------------------
// GenericTag                                                                 |
//-----------------------------------------------------------------------------

typedef struct _tagOPCEUInformation
{
	COleVariant m_info;	// EU info
	OPCEUTYPE m_type;	// EU type
} OPCEUInformation;

class OPCENGINE_EXPORT GenericTag : public GenericItemTag
{
public:
	GenericTag(); 
	
	// tag properties
	GenericList<GenericTagProperty> *getPropertyList(void);
	virtual GenericTagProperty *addProperty(IN DWORD propertyId);
	void removeAllProperties(void);

	// engineering units information and type
	BOOL getEUInfo(OUT LPVARIANT info);
	OPCEUTYPE getEUType(void) const;

	void clearEU(void);
	virtual BOOL setEUAnalog(IN double low, IN double high);
	virtual BOOL setEUEnumerated(IN CStringList *enumList, IN OPTIONAL LONG startIndex = 0);

	// I/O mode
	virtual BYTE getDeviceIOMode(void);

	// special release method
	virtual LONG release(void);	

	virtual BOOL setParentBranch(IN GenericBranch *parent);

protected:
	~GenericTag(); 

	GenericList<GenericTagProperty> m_propertyList;	// tag properties list
	OPCEUInformation *m_EUInfo;						// EU information
}; // GenericTag

inline OPCEUTYPE GenericTag::getEUType(void) const
{ if (!m_EUInfo) return OPC_NOENUM; else return m_EUInfo->m_type; }

inline GenericList<GenericTagProperty> *GenericTag::getPropertyList(void)
{ return &m_propertyList; }

inline GenericTag *GenericTagProperty::getTag(void)
{ if (m_tag) { m_tag->addRef(); return m_tag;} else return NULL; }




//-----------------------------------------------------------------------------
// GenericNode                                                                |
//-----------------------------------------------------------------------------

class OPCENGINE_EXPORT GenericNode : public GenericBranch
{
public:
	GenericNode(IN OPTIONAL DWORD supportedChildObj = GENOBJ_TYPE_LEAF | GENOBJ_TYPE_BRANCH, 
			IN OPTIONAL BYTE whatTagList = GENLIST_CREATE_LIST, IN OPTIONAL BYTE whatNodeList = GENLIST_CREATE_LIST, 
			IN OPTIONAL int blockSizeTagList = 10, IN OPTIONAL int blockSizeNodeList = 10,
			IN OPTIONAL int hashSizeTagList = 17, IN OPTIONAL int hashSizeNodeList = 17); 

	virtual BOOL addLeaf(IN GenericLeaf *newLeaf, IN OPTIONAL int index = -1);
	virtual BOOL addBranch(IN GenericBranch *newBranch, 
			IN OPTIONAL int index = -1);

	void removeAllProperties(void);
}; // GenericNode




//-----------------------------------------------------------------------------
// OPCRootNS                                                                  |
//-----------------------------------------------------------------------------

class OPCENGINE_EXPORT OPCRootNS : public GenericBranch
{
public:
	OPCRootNS(); 

	virtual BOOL addLeaf(IN GenericLeaf *newLeaf, IN OPTIONAL int index = -1);
	virtual BOOL addBranch(IN GenericBranch *newBranch, 
			IN OPTIONAL int index = -1);

	void removeAllProperties(void);

	BOOL getUseTagLinks(void) const;
	BOOL setUseTagLinks(IN BOOL use);

protected:
	BYTE m_useTagLinks;
}; // OPCRootNS

inline BOOL OPCRootNS::getUseTagLinks(void) const
{ return m_useTagLinks; }

inline BOOL OPCRootNS::setUseTagLinks(IN BOOL use)
{ m_useTagLinks = use; return TRUE; }

#endif
