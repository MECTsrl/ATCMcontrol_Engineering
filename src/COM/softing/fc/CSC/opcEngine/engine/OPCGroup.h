//-----------------------------------------------------------------------------
//                                  OPC ENGINE                                |
//                                                                            |
//  Filename    : OPCGROUP.H                                                  |   
//  Version     : 2.00.0.00.release                                           |
//  Date        : 29-March-1999                                               |
//  Author      : ATCM                                                 		  |
//                                                                            |
//  Description : OPCGroup class                                              |
//                                                                            |
//  CHANGE_NOTES                                                              | 
//                                                                            |
//  date      name      change                                                |
//  ------------------------------------------------------------------------- |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _OPCGROUP_H_
#define _OPCGROUP_H_

#include <opcda.h>
#include "globals.h"
#include "OPCObj.h"

//-----------------------------------------------------------------------------
// OPCGroup                                                                   |
//-----------------------------------------------------------------------------

class OPCServer;
class GenericGroup;

class OPCGroup : public IOPCGroupStateMgt,
				 public IOPCPublicGroupStateMgt,
				 public IOPCSyncIO,
				 public IOPCAsyncIO,
				 public IOPCAsyncIO2,
				 public IOPCItemMgt,
				 public IDataObject,
				 public IConnectionPointContainerImpl<OPCGroup>,
				 public IConnectionPointImpl<OPCGroup, &IID_IOPCDataCallback, CComDynamicUnkArray>,
				 public CComObjectRoot
{
public:
	OPCGroup();
	~OPCGroup();

	BEGIN_COM_MAP(OPCGroup)
		COM_INTERFACE_ENTRY(IOPCGroupStateMgt)
		COM_INTERFACE_ENTRY(IOPCPublicGroupStateMgt)
		COM_INTERFACE_ENTRY(IOPCSyncIO)
		COM_INTERFACE_ENTRY(IOPCAsyncIO)
		COM_INTERFACE_ENTRY(IOPCAsyncIO2)
		COM_INTERFACE_ENTRY(IOPCItemMgt)
		COM_INTERFACE_ENTRY(IDataObject)
#if _MSC_VER >= 1200
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
#else
		COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
#endif
	END_COM_MAP()

	BEGIN_CONNECTION_POINT_MAP(OPCGroup)
		CONNECTION_POINT_ENTRY(IID_IOPCDataCallback)
	END_CONNECTION_POINT_MAP()

	// IOPCGroupStateMgt interface
	STDMETHODIMP GetState(OUT DWORD *pUpdateRate, OUT BOOL *pActive, 
			OUT LPWSTR *ppName, OUT LONG *pTimeBias, OUT FLOAT *pDeadband,
			OUT DWORD *pLCID, OUT OPCHANDLE *phClientGroup,
			OUT OPCHANDLE *phServerGroup);

	STDMETHODIMP SetState(IN DWORD *pRequestedUpdateRate, 
			OUT DWORD *pRevisedUpdateRate, IN BOOL *pActive, IN LONG *pTimeBias,
			IN FLOAT *pPercentDeadband, IN DWORD *pLCID,
			IN OPCHANDLE *phClientGroup);

	STDMETHODIMP SetName(IN LPCWSTR szName);

	STDMETHODIMP CloneGroup(IN LPCWSTR szName, IN REFIID riid,
			OUT LPUNKNOWN *ppUnk);

	// IOPCPublicGroupStateMgt interface
	STDMETHODIMP GetState(OUT BOOL *pPublic);

	STDMETHODIMP MoveToPublic(void);

	// IOPCItemMgt interface
	STDMETHODIMP AddItems(IN DWORD dwNumItems, IN OPCITEMDEF *pItemArray,
			OUT OPCITEMRESULT **ppAddResults, OUT HRESULT **ppErrors);

	STDMETHODIMP ValidateItems(IN DWORD dwNumItems, IN OPCITEMDEF *pItemArray,
			IN BOOL bBlobUpdate, OUT OPCITEMRESULT **ppValidationResults,
			OUT HRESULT **ppErrors);

	STDMETHODIMP RemoveItems(IN DWORD dwNumItems, IN OPCHANDLE *phServer,
			OUT HRESULT **ppErrors);

	STDMETHODIMP SetActiveState(IN DWORD dwNumItems, IN OPCHANDLE *phServer,
			IN BOOL bActive, OUT HRESULT **ppErrors);

	STDMETHODIMP SetClientHandles(IN DWORD dwNumItems, IN OPCHANDLE *phServer,
			IN OPCHANDLE *phClient, OUT HRESULT **ppErrors);

	STDMETHODIMP SetDatatypes(IN DWORD dwNumItems, IN OPCHANDLE *phServer,
			IN VARTYPE *pRequestedDatatypes, OUT HRESULT **ppErrors);
 
	STDMETHODIMP CreateEnumerator(IN REFIID riid, OUT LPUNKNOWN * ppUnk);

	// IOPCSyncIO interface
	STDMETHODIMP Read(IN OPCDATASOURCE dwSource, IN DWORD dwNumItems, 
			IN OPCHANDLE *phServer, OUT OPCITEMSTATE **ppItemValues,
			OUT HRESULT **ppErrors);

	STDMETHODIMP Write(IN DWORD dwNumItems, IN OPCHANDLE *phServer, 
			IN VARIANT *pItemValues, OUT HRESULT **ppErrors);

	// IOPCAsyncIO interface
	STDMETHODIMP Read(IN DWORD dwConnection, IN OPCDATASOURCE dwSource,
			IN DWORD dwNumItems, IN OPCHANDLE *phServer, 
			OUT DWORD *pTransactionID, OUT HRESULT **ppErrors);

	STDMETHODIMP Write(IN DWORD dwConnection, IN DWORD dwNumItems, 
			IN OPCHANDLE *phServer, IN VARIANT *pItemValues, 
			IN DWORD *pTransactionID, OUT HRESULT **ppErrors);

	STDMETHODIMP Refresh(IN DWORD dwConnection, IN OPCDATASOURCE dwSource,
			OUT DWORD *pTransactionID);

	STDMETHODIMP Cancel(IN DWORD dwTransactionID);

	// IOPCAsyncIO2 interface
	STDMETHODIMP Read(IN DWORD dwNumItems, IN OPCHANDLE *phServer, 
			IN DWORD dwTransactionID, OUT DWORD *pdwCancelID, 
			OUT HRESULT **ppErrors);

	STDMETHODIMP Write(IN DWORD dwNumItems, IN OPCHANDLE *phServer, 
			IN VARIANT *pItemValues, IN DWORD dwTransactionID, 
			OUT DWORD *pdwCancelID, OUT HRESULT **ppErrors);

	STDMETHODIMP Refresh2(IN OPCDATASOURCE dwSource, IN DWORD dwTransactionID, 
			OUT DWORD *pdwCancelID);

	STDMETHODIMP Cancel2(IN DWORD dwCancelID);

	STDMETHODIMP SetEnable(IN BOOL bEnable);

	STDMETHODIMP GetEnable(OUT BOOL *pbEnable);

	// IDataObject interface
	STDMETHODIMP DAdvise(IN FORMATETC* format, IN DWORD advf, 
			IN IAdviseSink* pAdvSink, OUT DWORD *pdwConnection);

	STDMETHODIMP DUnadvise(IN DWORD dwConnection);

	STDMETHODIMP EnumDAdvise(OUT IEnumSTATDATA **ppenumAdvise);

	STDMETHODIMP GetData(IN FORMATETC *formatIn, OUT STGMEDIUM *medium);

	STDMETHODIMP QueryGetData(IN FORMATETC *format);

	STDMETHODIMP GetDataHere(IN FORMATETC *format, OUT STGMEDIUM *medium);
  
	STDMETHODIMP GetCanonicalFormatEtc(IN FORMATETC* formatIn,
			OUT FORMATETC* formatOut);
  
	STDMETHODIMP SetData(IN FORMATETC* format, IN STGMEDIUM* medium,
			OUT BOOL fRelease);

	STDMETHODIMP EnumFormatEtc(IN DWORD dwDirection, 
			OUT IEnumFORMATETC** ppenumFormatEtc);


	STDMETHODIMP Advise (IN IUnknown* pUnk, OUT DWORD* pdwCookie);

	STDMETHODIMP Unadvise (IN DWORD dwCookie);

	// initialize group object
	void init(OPCServer *server, GenericGroup *group);

	// object trace id
	LPCTSTR getObjTraceId(void);


	CComDynamicUnkArray *getAdvises(void);
	CSyncObject *getAdvisesLockObj(void);

	BOOL releaseMe(void);
	void releaseServer(void);

protected:
	CString m_objectTraceId;	// object trace id string 
	POSITION m_objectTracePos;	// object trace pointer
	OPCServer *m_server;		// server object
	GenericGroup *m_me;			// generic group object
	CCriticalSection m_advisesLock;	// lock object for advises map

	// handle common interface function parameters
	HRESULT itemArrayCheckAndAlloc(IN DWORD dwNumItems, IN void *p1, IN void *p2,	
			OUT void **p3, OUT HRESULT **ppErrors);

	// create transaction from interface parameters
	HRESULT createTransaction(IN BOOL read, IN BYTE connectionType, 
			IN DWORD connection, IN OPCDATASOURCE source, IN DWORD numItems, 
			IN OPCHANDLE *serverHandles, IN VARIANT *itemValues, 
			OUT OPCTransaction **transaction, OUT DWORD *transactionID, 
			OUT HRESULT **errors);

	// fill request list from interface parameters
	HRESULT fillRequestList(IN GenericRequest::requestType reqType,
			IN GenericRequest::requestOperation reqOp, IN DWORD accessRight,
			IN DWORD numItems, IN OPCHANDLE *serverHandles, 
			IN VARIANT *writeItemValues, IN BOOL checkAccessRights,
			OUT RequestList *reqList, OUT OPCITEMSTATE **readItemValues, 
			OUT HRESULT **errors);

	// copy existing cyclic transaction
	void copyCyclicTransaction(IN DWORD dwConnection, IN OPCDATASOURCE dwSource,
			IN BYTE connectionType, OUT OPCTransaction **dest);

	BOOL connectedDataCallbackInterface(void);
}; // OPCGroup

inline CComDynamicUnkArray *OPCGroup::getAdvises(void)
{ IConnectionPointImpl<OPCGroup, &IID_IOPCDataCallback, CComDynamicUnkArray>* p = this; return &p->m_vec; }

inline CSyncObject *OPCGroup::getAdvisesLockObj(void)
{ return &m_advisesLock; }

typedef OPCObject<OPCGroup> OPCGroupObj;

#endif
