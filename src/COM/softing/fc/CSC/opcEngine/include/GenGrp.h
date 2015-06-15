//-----------------------------------------------------------------------------
//                                  OPC ENGINE                                |
//                                                                            |
//  Filename    : GENGRP.H                                                    |   
//  Version     : 2.01.0.00.release                                           |
//  Date        : 7-May-1999                                                  |
//                                                                            |
//  Description : Group object related classes                                |
//                - GenericGroup: OPC group object                            |
//                - OPCAdvise: Client advise object (OPC V1.0)                |
//                                                                            |
//  CHANGE_NOTES                                                              | 
//                                                                            |
//  date      name      change                                                |
//  ------------------------------------------------------------------------- |
//  12/23/98  RT        added IOPCAsyncIO2 support                            |
//  04/21/99  RT        added report driven protocol support                  |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _GENGRP_H_
#define _GENGRP_H_

#include <opcda.h>
#include "GenTree.h"
#include "GenNS.h"
#include "GenReq.h"
#include "OPCTrans.h"


//-----------------------------------------------------------------------------
// OPCAdvise                                                                  |
//-----------------------------------------------------------------------------

class OPCENGINE_EXPORT OPCAdvise
{
public:
	OPCAdvise();
	OPCAdvise(const OPCAdvise& src);
	~OPCAdvise();

	CLIPFORMAT m_format;		// data format of advise
	DWORD m_connection;			// connection id
	IAdviseSink *m_adviseSink;	// client's advise sink
}; // OPCAdvise

template<> void AFXAPI ConstructElements<OPCAdvise> (OPCAdvise* elements, int count);
template<> void AFXAPI DestructElements<OPCAdvise> (OPCAdvise* elements, int count);
template<> void AFXAPI CopyElements<OPCAdvise> (OPCAdvise* dest, const OPCAdvise* src, int count);




//-----------------------------------------------------------------------------
// GenericGroup                                                               |
//-----------------------------------------------------------------------------

class OPCGroup;
class OPCServer;
class OPCTransaction;
class OPCEngine;
class GenericItem;
class GenericRequest;
class GenericServer;

typedef CMap<DWORD, DWORD, OPCAdvise, OPCAdvise&> AdviseMap;


class OPCENGINE_EXPORT GenericGroup : public GenericBranch
{
friend OPCGroup;
friend OPCServer;
friend OPCTransaction;
friend OPCEngine;
friend GenericRequest;
friend GenericServer;
	
public:
	GenericGroup(
			IN OPTIONAL BYTE updateMode = poll,
			IN OPTIONAL BYTE whatItemList = GENLIST_CREATE_LIST | GENLIST_CREATE_MAP, 
			IN OPTIONAL int blockSizeItemList = 5, 
			IN OPTIONAL int hashSizeItemList = 17);  

	enum groupUpdateMode
	{
		poll,		// poll all items and check for changes
		report		// use report list to get the changed items
	};

	enum methodOPCGroup
	{
		// IOPCGroupStateMgt
		opcmGroupStateMgtGetState,
		opcmSetState,
		opcmSetName,
		opcmCloneGroup,
		// IOPCPublicGroupStateMgt
		opcmPublicGroupStateMgtGetState,
		opcmMoveToPublic,
		// IOPCSyncIO
		opcmSyncIORead,
		opcmSyncIOWrite,
		// IOPCAsyncIO
		opcmAsyncIORead,
		opcmAsyncIOWrite,
		opcmCancel,
		opcmRefresh,
		// IOPCItemMgt
		opcmAddItems,
		opcmValidateItems,
		opcmRemoveItems,
		opcmSetActiveState,
		opcmSetClientHandles,
		opcmSetDatatypes,
		opcmCreateEnumerator,
		// IDataObject
		opcmDAdvise,
		opcmDUnadvise,
		// IOPCAsyncIO2
		opcmAsyncIORead2,
		opcmAsyncIOWrite2,
		opcmCancel2,
		opcmRefresh2,
		opcmGetEnable,
		opcmSetEnable,
		// IConnectionPointContainer
		opcmEnumConnectionPoints,
		opcmFindConnectionPoint
	}; // enum methodOPC

	// tree management
	virtual BOOL addLeaf(IN GenericLeaf *newLeaf, IN OPTIONAL int index = -1);
	virtual BOOL addBranch(IN GenericBranch *newBranch, 
			IN OPTIONAL int index = -1);
	virtual HRESULT setName(IN LPCTSTR name);

	// activation state
	virtual BOOL setActive(IN BOOL active);
	BOOL getActive(void) const;

	// update rate in ms 
	virtual BOOL setUpdateRate(IN DWORD updateRate);
	DWORD getUpdateRate(void) const;

	// client handle from OPC client
	virtual BOOL setClientHandle(IN OPCHANDLE clientHandle);
	OPCHANDLE getClientHandle(void) const;

	// time zone
	virtual BOOL setTimeBias(IN LONG timeBias);
	LONG getTimeBias(void) const;

	// deadband in percent
	virtual BOOL setDeadBand(IN FLOAT deadBand);
	FLOAT getDeadBand(void) const;

	// local Id
	virtual BOOL setLCID(IN DWORD lcid);
	DWORD getLCID(void) const;

	// enable flag for asynch I/O
	virtual BOOL setEnable(IN BOOL enable);
	BOOL getEnable(void) const;

	// server handle
	OPCHANDLE getServerHandle(void) const;

	// IOPCItemMgt
	virtual HRESULT validateItem(IN LPCTSTR id, IN LPCTSTR accessPath, 
			IN VARTYPE reqDataType, OUT VARTYPE *nativeDataType, 
			OUT DWORD *accessRights, OUT OPTIONAL GenericItemTag **tag = NULL);
	virtual HRESULT addItem(IN LPCTSTR id, IN LPCTSTR accessPath, 
			IN BOOL active,	IN OPCHANDLE clientHandle, IN VARTYPE reqDataType, 
			OUT GenericItem **item, OUT VARTYPE *nativeDataType, 
			OUT DWORD *accessRights);
	virtual HRESULT removeItem(IN OPCHANDLE serverHandle);

	GenericItem *getItemByHandle(IN OPCHANDLE serverHandle);

	// IOPCGroupStateMgt
	virtual HRESULT clone(IN GenericServer *server, IN LPCTSTR name, 
			OUT GenericGroup **clone);

	// IOPCPublicGroupStateMgt
	virtual BOOL isPublic(void);

	// removed flag
	BOOL isRemoved(void);

	// don't check the client provided item handles
	void trustClientsItemHandles(BOOL trust);

	// get the pointer of the cyclic transaction
	OPCTransaction *getCyclicTransaction();

	// update mode of the group
	enum groupUpdateMode getUpdateMode(void); 

protected:
	~GenericGroup();

	void *m_me;
	CMutex m_meSync;
	DWORD m_updateRate;			// update rate in ms 
	OPCHANDLE m_clientHandle;	// client handle from OPC client
	LONG m_timeBias;			// time zone
	FLOAT m_deadBand;			// deadband in percent
	DWORD m_lcid;				// local Id
	OPCHANDLE m_serverHandle;	// server handle 
	OPCTransaction *m_cyclicTransaction;
								// cyclic transaction
	ULONG m_span;				// time in ms untill the start of the cyclic transaction
	DWORD m_initTime;			// init time of the cyclic transaction in ms since system start
	DWORD m_initCnt;			// init cyclic transaction counter
	DWORD m_correctNTtimer;		// NT timer correction value
	AdviseMap m_advises;		// advises map
#ifdef _AFXDLL
	CCriticalSection m_advisesLock;	// lock object for advises map
#else
	CMutex m_advisesLock;		// lock object for advises map
#endif
	GenericGroup *m_publicTemplate;	
								// template object of public group
	BYTE m_active;				// activation state
	BYTE m_removed;				// removed flag
	BYTE m_enable;				// enable flag for async I/O
	BYTE m_trustClientsItemHandles;	// trust the client provided item handles
	BYTE m_updateMode;			// update mode of the group

	// I/O handling
	virtual HRESULT syncRead(IN RequestList &reqList, IN OPCDATASOURCE source, IN LPCTSTR user);
	virtual HRESULT syncWrite(IN RequestList &reqList, IN LPCTSTR user);
	virtual HRESULT asyncRead(IN OPCTransaction* transaction, IN LPCTSTR user);
	virtual HRESULT asyncWrite(IN OPCTransaction* transaction, IN LPCTSTR user);
	virtual HRESULT asyncRefresh(IN OPCTransaction* transaction, IN LPCTSTR user);
	virtual HRESULT asyncCancel(IN DWORD transactionId, IN LPCTSTR user);
	virtual HRESULT advise(IN IDataObject *pDataObj, IN FORMATETC* format, 
			IN DWORD advf, IN IAdviseSink* pAdvSink, IN LPCTSTR user, 
			OUT DWORD *connection);
	virtual HRESULT advise2(IN IUnknown* pUnk, IN DWORD cookie, IN LPCTSTR user);
	virtual HRESULT unadvise(IN DWORD connection, IN LPCTSTR user);
	virtual void unadvise2(IN DWORD cookie, IN LPCTSTR user);

	// callbacks
	virtual void onClone(IN GenericGroup *clone);
	virtual void onMoveToPublic(void);
	virtual void onAdvise(IN CLIPFORMAT format, IN DWORD connection);
	virtual void onAdvise2(IN DWORD cookie);
	virtual void onUnadvise(IN DWORD connection);
	virtual void onUnadvise2(IN DWORD cookie);
	virtual void onUpdateCyclicTransaction(void);
	virtual void onSetActive(IN BOOL active);
	virtual void onSetUpdateRate(IN DWORD updateRate);
	virtual void onSetClientHandle(IN OPCHANDLE clientHandle);
	virtual void onSetTimeBias(IN LONG timeBias);
	virtual void onSetDeadBand(IN FLOAT percentDeadBand);
	virtual void onSetLCID(IN DWORD lcid);
	virtual void onSetEnable(IN BOOL enable);
	virtual void onDataChange(IN OPCTransaction *transaction);
	virtual void onReadComplete(IN OPCTransaction *transaction);
	virtual void onWriteComplete(IN OPCTransaction *transaction);

	// access to OPC method
	virtual BOOL accessToMethod(IN REFIID iid, 
			IN enum methodOPCGroup method, IN LPCTSTR user);

	// cyclic transaction
	virtual BOOL updateSpan(IN DWORD now, IN DWORD *span);
	virtual void initSpan(IN DWORD toLate);

	// advise management
	BOOL validConnection(IN DWORD connection, IN BOOL read);
	AdviseMap *getAdvises(void);
	CSyncObject *getAdvisesLockObj(void);

	// public group
	BOOL isPublicGroup(void) const;
	BOOL isPublicTemplate(void) const;

	// removed flag
	void setRemoved(void);

}; // GenericGroup


inline BOOL GenericGroup::getActive(void) const
{ return m_active; }

inline DWORD GenericGroup::getUpdateRate(void) const
{ return m_updateRate; }

inline OPCHANDLE GenericGroup::getClientHandle(void) const
{ return m_clientHandle; }

inline LONG GenericGroup::getTimeBias(void) const
{ return m_timeBias; }

inline FLOAT GenericGroup::getDeadBand(void) const
{ return m_deadBand; }

inline DWORD GenericGroup::getLCID(void) const
{ return m_lcid; }

inline OPCTransaction *GenericGroup::getCyclicTransaction()
{ m_cyclicTransaction->addRef(); return m_cyclicTransaction; }

inline void GenericGroup::setRemoved(void)
{ m_removed = TRUE; }

inline AdviseMap *GenericGroup::getAdvises(void)
{ return &m_advises; }

inline CSyncObject *GenericGroup::getAdvisesLockObj(void)
{ return &m_advisesLock; }

inline BOOL GenericGroup::getEnable(void) const
{ return m_enable; }

inline void GenericGroup::trustClientsItemHandles(BOOL trust)
{ m_trustClientsItemHandles = trust; }

inline enum GenericGroup::groupUpdateMode GenericGroup::getUpdateMode(void)
{ return (enum groupUpdateMode)m_updateMode; }

#endif
