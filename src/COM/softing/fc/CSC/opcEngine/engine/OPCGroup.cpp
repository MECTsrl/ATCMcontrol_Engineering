//-----------------------------------------------------------------------------
//                                  OPC ENGINE                                |
//                                                                            |
//  Filename    : OPCGROUP.CPP                                                |   
//  Version     : 2.00.0.00.release                                           |
//  Date        : 29-March-1999                                               |
//  Author      : ATCM                                                 |
//                                                                            |
//  Description : OPCGroup class                                              |
//                                                                            |
//  CHANGE_NOTES                                                              | 
//                                                                            |
//  date      name      change                                                |
//  ------------------------------------------------------------------------- |
//                                                                            |
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "OPCEngine.h"
#include "OPCServer.h"
#include "OPCGroup.h"
#include "GenSrv.h"
#include "GenGrp.h"
#include "GenItem.h"
#include "enum.h"
#include "OPCTrace.h"
#include <opcerror.h>


const UINT OPCSTMFORMATDATA = ::RegisterClipboardFormat(_T("OPCSTMFORMATDATA"));
const UINT OPCSTMFORMATDATATIME = ::RegisterClipboardFormat(_T("OPCSTMFORMATDATATIME"));
const UINT OPCSTMFORMATWRITECOMPLETE = ::RegisterClipboardFormat(_T("OPCSTMFORMATWRITECOMPLETE"));


//-----------------------------------------------------------------------------
// OPCGroup                                                                  |
//-----------------------------------------------------------------------------

OPCGroup::OPCGroup()
{
	m_objectTracePos = NULL;
	_TRACE(TL_DEB, TG_GRP, (_T("> constructor")));
	_TRACE_CREATE_COMGRP();
	m_server = NULL;
	m_me = NULL;
	_TRACE(TL_DEB, TG_GRP, (_T("< constructor")));
}

OPCGroup::~OPCGroup()
{
	_TRACE(TL_DEB, TG_GRP, (_T("> destructor")));
	_TRACE_DELETE_COMGRP();

	if (m_me)
	{
		if (m_me->isPublicGroup())
		{
			_TRACE(TL_DEB, TG_GRP, (_T("public group")));
			// public group COM object destroys the tree objects
			GenericPointer<GenericServer> srv = (GenericServer *)m_me->getParentBranch();
			GenericPointer<OPCTransaction> tac = (OPCTransaction *)m_me->getCyclicTransaction();

			// remove item objects
			m_me->removeAllLeafs(FALSE);

			if((GenericServer *)srv) 
			{
				// remove transaction from server
				srv->removeTransaction(tac);
				// remove tree object from server
				srv->removeBranch(m_me);
			}
		}
	}

	if (m_server)
	{
		// release server object
		((OPCObject<OPCServer> *)m_server)->internalRelease();
	}

	if (m_me)
	{
		m_me->release();	// release tree object
		m_me = NULL;
	}

	::lastCOMObjectActions();
	_TRACE(TL_DEB, TG_GRP, (_T("< destructor")));
}

BOOL OPCGroup::releaseMe(void)
{
	BOOL ret = TRUE;
	if (m_me)
	{
		m_me->m_meSync.Lock();
		ret = (m_me->m_me != NULL);
		m_me->m_me = NULL;
		m_me->m_meSync.Unlock();

		if (ret)
		{
			while(m_dwRef > 1)
				Sleep(0);
		}
	}
	return ret;
}

void OPCGroup::init(OPCServer *server, GenericGroup *group)
{ 
	_TRACE(TL_DEB, TG_GRP, (_T("> init %s %s"), server->getObjTraceId(), group->getObjTraceId()));
	m_server = server;
	m_me = group;
	m_me->m_me = this;
	m_me->addRef();	

	// keep server alive till group is freed
	((OPCObject<OPCServer> *)m_server)->internalAddRef();	
	_TRACE(TL_DEB, TG_GRP, (_T("< init")));
};


//-----------------------------------------------------------------------------
// getObjTraceId
//
// - get objects trace string id
//
// returns:
//		trace id string
//
LPCTSTR OPCGroup::getObjTraceId(void)
{ 
	if (m_objectTraceId.IsEmpty())
	{
		m_objectTraceId.Format(_T("(group|%8.8X)"), this);
		m_objectTraceId.FreeExtra();
	}
	return m_objectTraceId; 
} // getObjTraceId



//-----------------------------------------------------------------------------
// IOPCGroupStateMgt                                                          |
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// GetState (interface IOPCGroupStateMgt)
//
// - get attributes of the group
//
// returns:
//		S_OK          - all got
//		E_FAIL        - group allready removed
//		E_OUTOFMEMORY - can't allocate memory for name string
//
STDMETHODIMP OPCGroup::GetState(
	OUT DWORD *pUpdateRate,			// update rate
	OUT BOOL *pActive,				// activation state
	OUT LPWSTR *ppName,				// name
	OUT LONG *pTimeBias,			// time zone
	OUT FLOAT *pDeadband,			// deadband
	OUT DWORD *pLCID,				// locale id
	OUT OPCHANDLE *phClientGroup,	// client handle
	OUT OPCHANDLE *phServerGroup)	// server handle
{
	_TRACE(TL_DEB, TG_GRP, (_T("> GetState")));
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);
  
	if (m_me->isRemoved())
	{
		_TRACE(TL_INF, TG_GRP, (_T("group already removed")));
		return E_FAIL;
	}

	if (ppName) 
	{
		USES_CONVERSION;
		CString name = m_me->getName();
		*ppName = (LPWSTR) ::CoTaskMemAlloc((name.GetLength() + 1) * sizeof(WCHAR));
		if (*ppName == NULL)
		{
			_ERROR(TG_GRP, OPCENGINE_ERR_MEMORY,(_T("GetState: Out of memory")));
			return E_OUTOFMEMORY;
		}
		wcscpy(*ppName, T2COLE((LPCTSTR)name));
	}
	if (pUpdateRate)
		*pUpdateRate = m_me->getUpdateRate();
	if (pActive)
		*pActive = m_me->getActive();
	if (pTimeBias)
		*pTimeBias = m_me->getTimeBias();
	if (pDeadband)
		*pDeadband = m_me->getDeadBand();
	if (pLCID)
		*pLCID = m_me->getLCID();
	if (phClientGroup)
		*phClientGroup = m_me->getClientHandle();
	if (phServerGroup)
		*phServerGroup = m_me->getServerHandle();

	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IOPCGroupStateMgt::GetState: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_GRP, (_T("< GetState")));
	return S_OK;
} // GetState


//-----------------------------------------------------------------------------
// SetState (interface IOPCGroupStateMgt)
//
// - sets the attributes of the group
//
// returns:
//		S_OK                  - set attributes
//		E_FAIL                - group allready removed
//		OPC_S_UNSUPPORTEDRATE - update rate is not the requested
//
STDMETHODIMP OPCGroup::SetState(
	IN DWORD *pRequestedUpdateRate,	// reqested update rate
	OUT DWORD *pRevisedUpdateRate,	// revised update rate
	IN BOOL *pActive,				// activation state
	IN LONG *pTimeBias,				// time zone
	IN FLOAT *pPercentDeadband,		// deadband
	IN DWORD *pLCID,				// locale id
	IN OPCHANDLE *phClientGroup)	// client handle
{
	_TRACE(TL_DEB, TG_GRP, (_T("> SetState")));
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);
	HRESULT res = S_OK;
	DWORD setUpdateRate;

	if (m_me->isRemoved())
	{
		_TRACE(TL_INF, TG_GRP, (_T("group already removed")));
		return E_FAIL;
	}

	// set group parameters
	if (pActive)
		m_me->setActive(*pActive);
	if (pRequestedUpdateRate)
		m_me->setUpdateRate(*pRequestedUpdateRate);
	if (phClientGroup)
		m_me->setClientHandle(*phClientGroup);
	if (pTimeBias)
		m_me->setTimeBias(*pTimeBias);
	if (pPercentDeadband)
		m_me->setDeadBand(*pPercentDeadband);
	if (pLCID)
		m_me->setLCID(*pLCID);

	// check update rate
	setUpdateRate = m_me->getUpdateRate();
	if (pRequestedUpdateRate)
	{
		if (setUpdateRate != *pRequestedUpdateRate)
		{
			res = OPC_S_UNSUPPORTEDRATE;
		}
	}
	if (pRevisedUpdateRate) 
	{
		*pRevisedUpdateRate = setUpdateRate;
	}

	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IOPCGroupStateMgt::SetState: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_GRP, (_T("< SetState (0x%X)"), res));
    return res;
} // SetState


//-----------------------------------------------------------------------------
// SetName (interface IOPCGroupStateMgt)
//
// - sets the name of the group
//
// returns:
//		S_OK                - set name
//		E_FAIL              - group allready removed
//		E_INVALIDARG        - no valid name or parameter
//		OPC_E_DUPLICATENAME - no unique name
//
STDMETHODIMP OPCGroup::SetName(
	IN LPCWSTR szName)	// name
{
	_TRACE(TL_DEB, TG_GRP, (_T("> SetName %s"), szName));
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);

	if (szName == NULL)
	{
		_WARNING(TG_GRP, OPCENGINE_WRN_IF_PRM,(_T("SetName: bad input parameter")));
		return E_INVALIDARG;
	}

	if (m_me->isRemoved())
	{
		_TRACE(TL_INF, TG_GRP, (_T("group already removed")));
		return E_FAIL;
	}

	if (m_me->isPublic())
	{
		_WARNING(TG_GRP, OPCENGINE_WRN_GRP_PUBLIC,(_T("No SetName on public group")));
		return E_FAIL;
	}

	USES_CONVERSION;
	CString name(OLE2CT(szName));
	HRESULT res;
	res = m_me->setName(name);
	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IOPCGroupStateMgt::SetName: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_GRP, (_T("SetName %s (0x%X)"), szName, res));
	return res;
} // SetName


//-----------------------------------------------------------------------------
// CloneGroup (interface IOPCGroupStateMgt)
//
// - creates a copy of the group
//
// returns:
//		S_OK                - created new group
//		E_FAIL              - group allready removed
//		E_INVALIDARG        - group name not valid
//		OPC_E_DUPLICATENAME - group name not unique
//
STDMETHODIMP OPCGroup::CloneGroup(
	IN LPCWSTR szName,		// name
	IN REFIID riid,			// interface id
	OUT LPUNKNOWN *ppUnk)	// interface pointer
{
	USES_CONVERSION;
	CString name(OLE2CT(szName));
	_TRACE(TL_DEB, TG_GRP, (_T("> CloneGroup %s"), name));
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);
	HRESULT res;
	GenericPointer<GenericGroup> grp;

	if (ppUnk == NULL) 
	{
		_WARNING(TG_GRP, OPCENGINE_WRN_IF_PRM,(_T("CloneGroup: bad output parameter")));
		return E_INVALIDARG;
	}
	*ppUnk = NULL;

	if (m_me->isRemoved())
	{
		_TRACE(TL_INF, TG_GRP, (_T("group already removed")));
		return E_FAIL;
	}

	// create tree group object
	res = m_me->clone((GenericServer *)m_me->m_parent, name, (GenericGroup **)grp);

	if (SUCCEEDED(res))
	{
		// create COM group object
		if (m_server)
			res = m_server->createOPCGroup(grp, riid, ppUnk);
		else
			res = E_FAIL;
	}

	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IOPCGroupStateMgt::CloneGroup: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_GRP, (_T("< CloneGroup (0x%X)"), res));
	return res;
} // CloneGroup



//-----------------------------------------------------------------------------
// IOPCPublicGroupStateMgt                                                    |
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// GetState (interface IOPCPublicGroupStateMgt)
//
// - checks if the group is public or not
//
// returns:
//		S_OK          - return if is public group
//		E_INVALIDARG  - NULL pointer parameter
//
STDMETHODIMP OPCGroup::GetState(
	OUT BOOL *pPublic)
{
	_TRACE(TL_DEB, TG_GRP, (_T("> GetState(Public)")));
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);
	if (m_me->isRemoved())
	{
		_TRACE(TL_INF, TG_GRP, (_T("group already removed")));
		return E_FAIL;
	}

	if (pPublic == NULL)
	{
		_WARNING(TG_GRP, OPCENGINE_WRN_IF_PRM,(_T("GetState: bad output parameter")));
		return E_INVALIDARG;
	}

	*pPublic = m_me->isPublicGroup();

	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IOPCPublicGroupStateMgt::GetState: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_INF, TG_GRP, (_T("GetState(Public) %s"), BOOL2STR(*pPublic)));
	return S_OK;
} // GetState


//-----------------------------------------------------------------------------
// MoveToPublic (interface IOPCPublicGroupStateMgt)
//
// - converts a private group into a public
//
// returns:
//		S_OK				- group is now a public group
//		OPC_E_DUPLICATENAME - group name is no unique public group name
//
STDMETHODIMP OPCGroup::MoveToPublic(void)
{
	_TRACE(TL_DEB, TG_GRP, (_T("> MoveToPublic")));
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);
	if (m_me->isRemoved())
	{
		_TRACE(TL_INF, TG_GRP, (_T("group already removed")));
		return E_FAIL;
	}

	GenericPointer<OPCEngine> engine = ::getOPCEngine();
	GenericPointer<GenericServer> pgserver = engine->getPublicGroups();
	HRESULT res;

	if (!pgserver)
	{
		_TRACE(TL_INF, TG_GRP, (_T("no public group server")));
		return E_FAIL;
	}

	// copy group to public groups server
	GenericPointer<GenericGroup> clone;
	res = m_me->clone(pgserver, m_me->getName(), (GenericGroup **)clone);

	if (SUCCEEDED(res))
	{
		// move group to public 
		// -> creates a public template with server handle of the group
		clone->m_serverHandle = m_me->m_serverHandle;
		m_me->m_publicTemplate = (GenericGroup *)clone;

		m_me->onMoveToPublic();

		// release server reference for instance of public group COM object
		OPCGroupObj *me = (OPCGroupObj *)m_me->m_serverHandle;
		me->internalRelease();
	}

	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IOPCPublicGroupStateMgt::MoveToPublic: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_GRP, (_T("< MoveToPublic (0x%X)"), res));
	return res;
} // MoveToPublic


//-----------------------------------------------------------------------------
// IOPCItemMgt                                                                |
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// itemArrayCheckAndAlloc
//
// - checks pointer and allocates error return buffer for item management 
//   routines
//
// returns:
//		S_OK          - created all items without error
//		E_FAIL        - group allready removed
//		E_INVALIDARG  - NULL pointer parameter
//		E_OUTOFMEMORY - can't allocate result buffers
//
HRESULT OPCGroup::itemArrayCheckAndAlloc(
	IN DWORD dwNumItems,	// number of items
	IN void *p1,			// pointer which should not be NULL
	IN void *p2,			// pointer which should not be NULL
	OUT void **p3,			// pointer pointer which should not be NULL
	OUT HRESULT **ppErrors)	// errors
{
	// check pointer parameter
	if ((!p3) || (!ppErrors))
	{
		if (p3)
			*p3 = NULL;
		if (ppErrors)
			*ppErrors = NULL;
		_WARNING(TG_GRP, OPCENGINE_WRN_IF_PRM,(_T("OPC Group: bad output parameter")));
		return E_INVALIDARG;
	}	

	if ((!p1) || (!p2))
	{
		_WARNING(TG_GRP, OPCENGINE_WRN_IF_PRM,(_T("OPC Group: bad input parameter")));
		return E_INVALIDARG;
	}	

	if (m_me->isRemoved())	// group already removed ?
	{
		_TRACE(TL_INF, TG_GRP, (_T("group already removed")));
		return E_FAIL;
	}

	// create return data
	*ppErrors = (HRESULT*)CoTaskMemAlloc(dwNumItems * sizeof(HRESULT));
	if (*ppErrors == NULL)	
	{
		_ERROR(TG_GRP, OPCENGINE_ERR_MEMORY,(_T("Out of memory")));
		return E_OUTOFMEMORY;
	}
	
	return S_OK;
} // itemArrayCheckAndAlloc


//-----------------------------------------------------------------------------
// AddItems (interface IOPCItemMgt)
//
// - add items to the group
//
// returns:
//		S_OK          - created all items without error
//		S_FALSE       - created some, but not all items without error
//		E_FAIL        - group allready removed
//		E_INVALIDARG  - NULL pointer parameter
//		E_OUTOFMEMORY - can't allocate result buffers
//		OPC_E_PUBLIC  - can't add items to public group
//
STDMETHODIMP OPCGroup::AddItems(
	IN DWORD dwNumItems,				// number of items
	IN OPCITEMDEF *pItemArray,			// item input parameter
	OUT OPCITEMRESULT **ppAddResults,	// item result parameter
	OUT HRESULT **ppErrors)				// errors
{ 
	_TRACE(TL_DEB, TG_GRP, (_T("> AddItems cnt:%lu"), dwNumItems));
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);
	USES_CONVERSION;
	HRESULT res = S_OK;
	GenericItem *item;
	
	// item management standard parameter check
	if (FAILED(res = itemArrayCheckAndAlloc(dwNumItems, pItemArray, pItemArray, (void **)ppAddResults, ppErrors)))
		return res;

	if (m_me->isPublic())	// public group
	{ // can't add items to public group
		_WARNING(TG_GRP, OPCENGINE_WRN_GRP_PUBLIC,(_T("Can't add items to public group")));
		return OPC_E_PUBLIC;
	}

	// create return data
	*ppAddResults = (OPCITEMRESULT*) CoTaskMemAlloc(dwNumItems * sizeof(OPCITEMRESULT));
	if (*ppAddResults == NULL)	
	{
		CoTaskMemFree(*ppErrors);		
		*ppErrors = NULL;
		_ERROR(TG_GRP, OPCENGINE_ERR_MEMORY,(_T("AddItems: Out of memory")));
		return E_OUTOFMEMORY;
	}

	// add every item
	for (DWORD i = 0; i < dwNumItems; i++)
	{
		(*ppErrors)[i] = m_me->addItem(OLE2T(pItemArray[i].szItemID), 
				OLE2T(pItemArray[i].szAccessPath), pItemArray[i].bActive,
				pItemArray[i].hClient, pItemArray[i].vtRequestedDataType,
				&item, &((*ppAddResults)[i].vtCanonicalDataType),
				&((*ppAddResults)[i].dwAccessRights));

		(*ppAddResults)[i].wReserved = 0;
		(*ppAddResults)[i].dwBlobSize = 0;
		(*ppAddResults)[i].pBlob = NULL;
		if (SUCCEEDED((*ppErrors)[i])) 
		{
			(*ppAddResults)[i].hServer = item->getServerHandle();
			item->release();
		}
		else
		{
			(*ppAddResults)[i].hServer = 0;
			res = S_FALSE;
		}
	}

	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IOPCItemMgt::AddItems: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_GRP, (_T("< AddItems (0x%X)"), res));
	return res; 
} // AddItems


//-----------------------------------------------------------------------------
// ValidateItems (interface IOPCItemMgt)
//
// - validate items 
//
// returns:
//		S_OK          - validated all items without error
//		S_FALSE       - validated some, but not all items without error
//		E_FAIL        - group allready removed
//		E_INVALIDARG  - NULL pointer parameter
//		E_OUTOFMEMORY - can't allocate result buffers
//
STDMETHODIMP OPCGroup::ValidateItems(
	IN DWORD dwNumItems,					// number of items
	IN OPCITEMDEF *pItemArray,				// item input parameter
	IN BOOL /* bBlobUpdate */,					
	OUT OPCITEMRESULT **ppValidationResults,// item result parameter
	OUT HRESULT **ppErrors)					// errors
{ 
	_TRACE(TL_DEB, TG_GRP, (_T("> ValidateItems cnt:%lu"), dwNumItems));
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);
	USES_CONVERSION;
	HRESULT res = S_OK;

	// item management standard parameter check
	if (FAILED(res = itemArrayCheckAndAlloc(dwNumItems, pItemArray, pItemArray, (void **)ppValidationResults, ppErrors)))
		return res;

	// create return data
	*ppValidationResults = (OPCITEMRESULT*) CoTaskMemAlloc(dwNumItems * sizeof(OPCITEMRESULT));
	if (*ppValidationResults == NULL)	
	{
		CoTaskMemFree(*ppErrors);		
		*ppErrors = NULL;
		_ERROR(TG_GRP, OPCENGINE_ERR_MEMORY,(_T("ValidateItems: Out of memory")));
		return E_OUTOFMEMORY;
	}

	// validate every item
	for (DWORD i = 0; i < dwNumItems; i++)
	{
		(*ppErrors)[i] = m_me->validateItem(OLE2T(pItemArray[i].szItemID), 
				OLE2T(pItemArray[i].szAccessPath), pItemArray[i].vtRequestedDataType,
				&((*ppValidationResults)[i].vtCanonicalDataType),
				&((*ppValidationResults)[i].dwAccessRights));

		(*ppValidationResults)[i].hServer = NULL;
		(*ppValidationResults)[i].wReserved = 0;
		(*ppValidationResults)[i].dwBlobSize = 0;
		(*ppValidationResults)[i].pBlob = NULL;

		if (FAILED((*ppErrors)[i])) 
		{
			res = S_FALSE;
		}
	}

	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IOPCItemMgt::ValidateItems: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_GRP, (_T("< ValidateItems (0x%X)"), res));
	return res; 
} // ValidateItems


//-----------------------------------------------------------------------------
// RemoveItems (interface IOPCItemMgt)
//
// - remove items 
//
// returns:
//		S_OK          - removed all items without error
//		S_FALSE       - some item handles were invalid
//		E_FAIL        - group allready removed
//		E_INVALIDARG  - NULL pointer parameter
//		E_OUTOFMEMORY - can't allocate result buffers
//
STDMETHODIMP OPCGroup::RemoveItems(
	IN DWORD dwNumItems,	// number of items
	IN OPCHANDLE *phServer,	// items
	OUT HRESULT **ppErrors)	// errors
{ 
	_TRACE(TL_DEB, TG_GRP, (_T("> RemoveItems cnt:%lu"), dwNumItems));
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);
	HRESULT res = S_OK;

	// item management standard parameter check
	if (FAILED(res = itemArrayCheckAndAlloc(dwNumItems, phServer, phServer, (void **)ppErrors, ppErrors)))
		return res;

	if (m_me->isPublic())	// public group
	{ // can't remove items from public group
		_WARNING(TG_GRP, OPCENGINE_WRN_GRP_PUBLIC,(_T("Can't remove items from public group")));
		return OPC_E_PUBLIC;
	}

	// remove every item
	for (DWORD i = 0; i < dwNumItems; i++)
	{
		(*ppErrors)[i] = m_me->removeItem(phServer[i]);
		if (FAILED((*ppErrors)[i])) 
			res = S_FALSE;
	}

	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IOPCItemMgt::RemoveItems: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_GRP, (_T("< RemoveItems (0x%X"), res));
	return res; 
} // RemoveItems


//-----------------------------------------------------------------------------
// SetActiveState (interface IOPCItemMgt)
//
// - set activation state of the items
//
// returns:
//		S_OK          - set activation state of all items without error
//		S_FALSE       - some item handles were invalid
//		E_FAIL        - group allready removed
//		E_INVALIDARG  - NULL pointer parameter
//		E_OUTOFMEMORY - can't allocate result buffers
//
STDMETHODIMP OPCGroup::SetActiveState(
	IN DWORD dwNumItems,	// number of items
	IN OPCHANDLE *phServer,	// items
	IN BOOL bActive,		// activation state
	OUT HRESULT **ppErrors)	// errors
{ 
	_TRACE(TL_DEB, TG_GRP, (_T("> SetActiveState cnt:%lu"), dwNumItems));
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);
	GenericPointer<GenericItem> item;
	HRESULT res = S_OK;

	// item management standard parameter check
	if (FAILED(res = itemArrayCheckAndAlloc(dwNumItems, phServer, phServer, (void **)ppErrors, ppErrors)))
		return res;

	// remove every item
	for (DWORD i = 0; i < dwNumItems; i++)
	{
		// get item
		item = m_me->getItemByHandle(phServer[i]);
		if ((GenericItem *)item)
		{
			item->setActive(bActive);
			(*ppErrors)[i] = S_OK;
		}
		else
		{
			(*ppErrors)[i] = OPC_E_INVALIDHANDLE;
			res = S_FALSE;
		}
	}

	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IOPCItemMgt::SetActiveState: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_GRP, (_T("< SetActiveState (0x%X)"), res));
	return res; 
} // SetActiveState


//-----------------------------------------------------------------------------
// SetClientHandles (interface IOPCItemMgt)
//
// - set client handle of the items
//
// returns:
//		S_OK          - set client handle of all items without error
//		S_FALSE       - some item handles were invalid
//		E_FAIL        - group allready removed
//		E_INVALIDARG  - NULL pointer parameter
//		E_OUTOFMEMORY - can't allocate result buffers
//
STDMETHODIMP OPCGroup::SetClientHandles(
	IN DWORD dwNumItems,	// number of items
	IN OPCHANDLE *phServer,	// items
	IN OPCHANDLE *phClient,	// client handles
	OUT HRESULT **ppErrors)	// errors
{ 
	_TRACE(TL_DEB, TG_GRP, (_T("> SetClientHandles cnt:%lu"), dwNumItems));
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);
	GenericPointer<GenericItem> item;
	HRESULT res = S_OK;

	// item management standard parameter check
	if (FAILED(res = itemArrayCheckAndAlloc(dwNumItems, phServer, phClient, (void **)ppErrors, ppErrors)))
		return res;

	// remove every item
	for (DWORD i = 0; i < dwNumItems; i++)
	{
		// get item
		item = m_me->getItemByHandle(phServer[i]);
		if ((GenericItem *)item)
		{
			item->setClientHandle(phClient[i]);
			(*ppErrors)[i] = S_OK;
		}
		else
		{
			(*ppErrors)[i] = OPC_E_INVALIDHANDLE;
			res = S_FALSE;
		}
	}

	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IOPCItemMgt::SetClientHandles: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_GRP, (_T("< SetClientHandles (0x%X)"), res));
	return res; 
} // SetClientHandles


//-----------------------------------------------------------------------------
// SetDatatypes (interface IOPCItemMgt)
//
// - set client handle of the items
//
// returns:
//		S_OK          - set client handle of all items without error
//		S_FALSE       - some item handles were invalid
//		E_FAIL        - group allready removed
//		E_INVALIDARG  - NULL pointer parameter
//		E_OUTOFMEMORY - can't allocate result buffers
//
STDMETHODIMP OPCGroup::SetDatatypes(
	IN DWORD dwNumItems,	// number of items
	IN OPCHANDLE *phServer,	// items
	IN VARTYPE *pRequestedDatatypes, 
	OUT HRESULT **ppErrors)	// errors
{ 
	_TRACE(TL_DEB, TG_GRP, (_T("> SetDatatypes cnt:%lu"), dwNumItems));
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);
	GenericPointer<GenericItem> item;
	HRESULT res = S_OK;

	// item management standard parameter check
	if (FAILED(res = itemArrayCheckAndAlloc(dwNumItems, phServer, pRequestedDatatypes, (void **)ppErrors, ppErrors)))
		return res;

	// remove every item
	for (DWORD i = 0; i < dwNumItems; i++)
	{
		// get item
		item = m_me->getItemByHandle(phServer[i]);
		if ((GenericItem *)item)
			(*ppErrors)[i] = item->setReqDatatype(pRequestedDatatypes[i]);
		else
			(*ppErrors)[i] = OPC_E_INVALIDHANDLE;

		if (FAILED((*ppErrors)[i]))
		{
			res = S_FALSE;
		}
	}

	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IOPCItemMgt::SetDatatypes: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_GRP, (_T("< SetDatatypes (0x%X)"), res));
	return res; 
} // SetDatatypes

 
//-----------------------------------------------------------------------------
// CreateEnumerator (interface IOPCItemMgt)
//
// - create item enumerator
//
// returns:
//		S_OK          - set client handle of all items without error
//		S_FALSE       - some item handles were invalid
//		E_FAIL        - group allready removed
//		E_INVALIDARG  - NULL pointer parameter
//
STDMETHODIMP OPCGroup::CreateEnumerator(
	IN REFIID riid,			// interface id 
	OUT LPUNKNOWN * ppUnk)	// interface pointer
{ 
	_TRACE(TL_DEB, TG_GRP, (_T("> CreateEnumerator")));
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);
	HRESULT res = S_OK;

	// parameter check
	if (ppUnk == NULL)
	{
		_WARNING(TG_GRP, OPCENGINE_WRN_IF_PRM,(_T("CreateEnumerator: bad output parameter")));
		return E_INVALIDARG;
	}
	*ppUnk = NULL;

	if (m_me->isRemoved())
	{
		_TRACE(TL_INF, TG_GRP, (_T("group already removed")));
		return E_FAIL;
	}

	OPCItemEnumObject *attrList = new OPCItemEnumObject;
	GenericList<GenericLeaf> list(m_me->getLeafList());
	GenericItem *item;
	POSITION pos;

	// for all items of the group
	list.lock();
	pos = list.getStartPosition();
	while(pos)
	{
		item = (GenericItem *)list.getNext(pos);
		_TRACE(TL_DEB, TG_GRP, (_T("item: %s"), item->getObjTraceId()));
		item->addRef();
		attrList->m_list.AddTail(item);
	}
	list.unlock();

	// reset list 
	if (attrList->m_list.IsEmpty())
	{
		res = S_FALSE;		// no list entry -> return OK but nothing found
		delete attrList;	// free list enumerator
		*ppUnk = NULL;			
	}
	else
	{
		_TRACE(TL_INF, TG_GRP, (_T("created enumerator with %d items"), attrList->m_list.GetCount()));
		attrList->Reset();
		res = attrList->QueryInterface(riid, (void **)ppUnk);
	}

	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IOPCItemMgt::CreateEnumerator: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_GRP, (_T("< CreateEnumerator (0x%X)"), res));
    return res;
} // CreateEnumerator



//-----------------------------------------------------------------------------
// IOPCSyncIO                                                                 |
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Read
//
// - Synchronous read of an item value
//
// returns:
//		S_OK          - read values of all items without error
//		S_FALSE       - at least one read failed
//		E_FAIL        - group allready removed
//		E_INVALIDARG  - NULL pointer parameter
//		E_OUTOFMEMORY - can't allocate result buffers
//
STDMETHODIMP OPCGroup::Read(
	IN OPCDATASOURCE dwSource,		// data source: cache or device 
	IN DWORD dwNumItems,			// number of items
	IN OPCHANDLE *phServer,			// items
	OUT OPCITEMSTATE **ppItemValues,// values
	OUT HRESULT **ppErrors)			// errors
{ 
	_TRACE(TL_DEB, TG_GRP, (_T("> SyncRead cnt:%lu"), dwNumItems));
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);
	GenericRequest::requestType reqType;
	GenericRequest *req;
	RequestList reqList;
	POSITION pos;
	HRESULT res;
	DWORD i;

	// check the dwSource parameter
	if ((dwSource != OPC_DS_CACHE) && (dwSource != OPC_DS_DEVICE))
		return E_INVALIDARG;

	// item array standard parameter check
	if (FAILED(res = itemArrayCheckAndAlloc(dwNumItems, phServer, phServer, (void **)ppItemValues, ppErrors)))
		return res;

	// create return data
	*ppItemValues = (OPCITEMSTATE*) CoTaskMemAlloc(dwNumItems * sizeof(OPCITEMSTATE));
	if (*ppItemValues == NULL)	
	{
		CoTaskMemFree(*ppErrors);		
		*ppErrors = NULL;
		_ERROR(TG_GRP, OPCENGINE_ERR_MEMORY,(_T("SyncRead: Out of memory")));
		return E_OUTOFMEMORY;
	}

	if (dwSource == OPC_DS_CACHE)
		reqType = GenericRequest::syncCache;
	else
		reqType = GenericRequest::syncDevice;

	// create request list 
	fillRequestList(reqType, GenericRequest::read, OPC_READABLE, dwNumItems, 
					phServer, NULL, TRUE, &reqList, ppItemValues, ppErrors);

	if (!reqList.IsEmpty())
	{
		// perform read
		res = m_me->syncRead(reqList, dwSource, _T(""));
	}

	// free request list
	pos = reqList.GetHeadPosition();
	while (pos)
	{
		req = (GenericRequest *)reqList.GetNext(pos);
		req->release();
	}	
	reqList.RemoveAll();

	// check for errors
	if (SUCCEEDED(res))
	{
		for (i = 0; i < dwNumItems; i++)
		{
			if (FAILED((*ppErrors)[i]))
			{
				(*ppItemValues)[i].vDataValue.vt = VT_EMPTY;
				res = S_FALSE;
			}
		}
	}
	else
	{ // failed return code
		// free item specific errors 
		CoTaskMemFree(*ppErrors);		
		*ppErrors = NULL;
		// free item values 
		CoTaskMemFree(*ppItemValues);		
		*ppItemValues = NULL;
	}

	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IOPCSyncIO::Read: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_GRP, (_T("< SyncRead (0x%X)"), res));
	return res; 
} // Read

STDMETHODIMP OPCGroup::Write(
	IN DWORD dwNumItems, 
	IN OPCHANDLE *phServer, 
	IN VARIANT *pItemValues, 
	OUT HRESULT **ppErrors)
{ 
	_TRACE(TL_DEB, TG_GRP, (_T("> SyncWrite cnt:%lu"), dwNumItems));
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);
	GenericRequest *req;
	RequestList reqList;
	POSITION pos;
	HRESULT res;
	DWORD i;

	// item array standard parameter check
	if (FAILED(res = itemArrayCheckAndAlloc(dwNumItems, phServer, pItemValues, (void **)ppErrors, ppErrors)))
		return res;

	// create request list 
	fillRequestList(GenericRequest::syncDevice, GenericRequest::write, OPC_WRITEABLE, 
					dwNumItems, phServer, pItemValues, TRUE, &reqList, NULL, ppErrors);

	if (!reqList.IsEmpty())
	{
		// perform write 
		res = m_me->syncWrite(reqList, _T(""));
	}

	// free request list
	pos = reqList.GetHeadPosition();
	while (pos)
	{
		req = (GenericRequest *)reqList.GetNext(pos);
		req->release();
	}	
	reqList.RemoveAll();

	// check for errors
	if (SUCCEEDED(res))
	{
		for (i = 0; i < dwNumItems; i++)
		{
			if (FAILED((*ppErrors)[i]))
			{
				res = S_FALSE;
			}
		}
	}
	else
	{ // failed return code
		// free item specific errors 
		CoTaskMemFree(*ppErrors);		
		*ppErrors = NULL;
	}

	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IOPCSyncIO::Write: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_GRP, (_T("< SyncWrite (0x%X)"), res));
	return res; 
}

HRESULT OPCGroup::createTransaction(
	IN BOOL read,
	IN BYTE connectionType,
	IN DWORD connection, 
	IN OPCDATASOURCE source,
	IN DWORD numItems, 
	IN OPCHANDLE *serverHandles, 
	IN VARIANT *itemValues, 
	OUT OPCTransaction **transaction,
	OUT DWORD *transactionID, 
	OUT HRESULT **errors)
{
	OPCTransaction::transactionType type;
	GenericRequest::requestOperation reqOp;
	GenericRequest::requestType reqType;
	HRESULT res;
	DWORD accessRight;

	_TRACE(TL_DEB, TG_GRP, (_T("> createTransaction")));

	if (source == OPC_DS_CACHE)
		reqType = GenericRequest::asyncCache;
	else
		reqType = GenericRequest::asyncDevice;

	if (read)
	{
		type = OPCTransaction::read;
		reqOp = GenericRequest::read;
		accessRight = OPC_READABLE;
	}
	else
	{
		type = OPCTransaction::write;
		reqOp = GenericRequest::write;
		accessRight = OPC_WRITEABLE;
	}

	OPCTransaction *tac = new OPCTransaction(type, m_me, connectionType, connection);

	// create request list 
	res = fillRequestList(reqType, reqOp, accessRight, numItems, 
					serverHandles, itemValues, (connectionType != OPCTransaction::dataObject), tac->getRequestList()->getList(), NULL, errors);

	if (SUCCEEDED(res))
	{
		*transactionID = tac->getTransactionId();
		*transaction = tac;
	}
	else
	{
		*transactionID = 0;
		*transaction = NULL;
		tac->release();	// free transaction
	}

	_TRACE(TL_DEB, TG_GRP, (_T("< createTransaction (0x%X)"), res));
	return res;
}


HRESULT OPCGroup::fillRequestList(
	IN GenericRequest::requestType reqType,
	IN GenericRequest::requestOperation reqOp,
	IN DWORD accessRight,
	IN DWORD numItems, 
	IN OPCHANDLE *serverHandles, 
	IN VARIANT *writeItemValues, 
	IN BOOL checkForErrors,
	OUT RequestList *reqList,
	OUT OPCITEMSTATE **readItemValues,
	OUT HRESULT **errors)
{
	_TRACE(TL_DEB, TG_GRP, (_T("> fillRequestList")));
	HRESULT res = S_OK;
	GenericPointer<OPCEngine> engine = ::getOPCEngine();
	GenericPointer<GenericItem> item;
	GenericPointer<GenericItemTag> tag;
	GenericPointer<GenericServer> srv = (GenericServer *)m_me->getParentBranch();
	GenericPointer<GenericCreator> creator = engine->getCreator();
	DWORD i;
	GenericPointer<GenericRequest> req;
	BOOL addReqToList;

	// create request list 
	for (i = 0; i < numItems; i++)
	{
		// get item
		item = m_me->getItemByHandle(serverHandles[i]);
		if ((GenericItem *)item)
		{ 
			// check access rights
			tag = item->getItemTag();
			if ((!checkForErrors) || ((tag->getAccessRights() & accessRight) == accessRight))
			{ // valid access rights or do not check the access rights
				// create request
				(*errors)[i] = S_OK;
				req = creator->createRequest(srv, item, tag, reqOp, reqType, (GenericRequest::requestDeviceIOMode)tag->getDeviceIOMode());
				req->setItem(item);
				addReqToList = TRUE;

				switch (reqType)
				{
					case GenericRequest::syncCache:
					case GenericRequest::syncDevice:
					{
						req->setResultAddress((*errors)+i);
						req->setResult(E_FAIL);

						if (reqOp == GenericRequest::read)
							req->setItemStateAddress((*readItemValues)+i);
						else
						{
							if (!req->setWriteValue(&writeItemValues[i]))
							{
								addReqToList = FALSE;
								req->setResult(OPC_E_BADTYPE);
								res = S_FALSE;
							}
						}
					}
					break;
					case GenericRequest::asyncCache:
					case GenericRequest::asyncDevice:
					{
						req->setCompletedEvent(srv->getStartEvent());

						if ((tag->getAccessRights() & accessRight) != accessRight)	// valid access rights
						{
							req->setResult(OPC_E_BADRIGHTS);
						}

						if (reqOp == GenericRequest::write)
						{
							if (!req->setWriteValue(&writeItemValues[i]))
							{
								if (!checkForErrors)
								{
									req->setResult(OPC_E_BADTYPE);
								}
								else
								{
									addReqToList = FALSE;
									(*errors)[i] = OPC_E_BADTYPE;
									res = S_FALSE;
								}
							}
						}
					}
					break;
				}

				if (addReqToList)
				{
					req.addRef();
					reqList->AddTail((GenericRequest *)req);
				}
				else
				{
					_TRACE(TL_INF, TG_GRP, (_T("item[%lu] (0x%X)"), i, req->getResult()));
					req.release();
				}
			}
			else
			{
				_TRACE(TL_INF, TG_GRP, (_T("item[%lu] no access rights"), i));
				(*errors)[i] = OPC_E_BADRIGHTS;
				res = S_FALSE;
			}
		}
		else
		{
			_TRACE(TL_WRN, TG_GRP, (_T("item[%lu] invalid item handle 0x%X"), i, serverHandles[i]));
			(*errors)[i] = OPC_E_INVALIDHANDLE;
			res = S_FALSE;
		}
	}	

	_TRACE(TL_DEB, TG_GRP, (_T("< fillRequestList (0x%X)")));
	return res;
}

STDMETHODIMP OPCGroup::Read(
	IN DWORD dwConnection, 
	IN OPCDATASOURCE dwSource,
	IN DWORD dwNumItems, 
	IN OPCHANDLE *phServer, 
	OUT DWORD *pTransactionID, 
	OUT HRESULT **ppErrors)
{ 
	_TRACE(TL_DEB, TG_GRP, (_T("> AsyncRead cnt:%lu"), dwNumItems));
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);
	HRESULT res;
	GenericPointer<OPCTransaction> tac;

	// out parameter ok
	if (!pTransactionID)
	{
		_WARNING(TG_GRP, OPCENGINE_WRN_IF_PRM,(_T("AsyncRead: bad output parameter")));
		return E_INVALIDARG;
	}
	*pTransactionID = 0;

	// check the dwSource parameter
	if ((dwSource != OPC_DS_CACHE) && (dwSource != OPC_DS_DEVICE))
		return E_INVALIDARG;

	if (m_me->isRemoved())
	{
		if (ppErrors)
			*ppErrors = NULL;
		_TRACE(TL_INF, TG_GRP, (_T("group already removed")));
		return E_FAIL;
	}

	// check if valid read connection
	if (!m_me->validConnection(dwConnection, TRUE))
	{
		if (ppErrors)
			*ppErrors = NULL;
		_WARNING(TG_GRP, OPCENGINE_WRN_BAD_CONNECTION,(_T("No valid read connection %lu"), dwConnection));
		return CONNECT_E_NOCONNECTION;
	}

	// item array standard parameter check
	if (FAILED(res = itemArrayCheckAndAlloc(dwNumItems, phServer, phServer, (void **)ppErrors, ppErrors)))
		return res;

	res = createTransaction(TRUE, OPCTransaction::dataObject, dwConnection, dwSource, dwNumItems, phServer, NULL, (OPCTransaction **)tac, pTransactionID, ppErrors);

	if (S_OK == res)
	{ // transaction was created with no error
		// -> start async read transaction
		res = m_me->asyncRead(tac, _T(""));
	}
	else
	{ // transaction was created with an error
		if (SUCCEEDED(res))
		{ // result was S_FALSE (invalid server handle)
			// free transaction
			*pTransactionID = 0;
			tac.release();	// free transaction
		}
		else
		{ // failed return code
			// free item specific errors 
			CoTaskMemFree(*ppErrors);		
			*ppErrors = NULL;
		}
	}

	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IOPCAsyncIO::Read: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_GRP, (_T("< AsyncRead (0x%X)"), res));
	return res; 
}

STDMETHODIMP OPCGroup::Write(
	IN DWORD dwConnection, 
	IN DWORD dwNumItems, 
	IN OPCHANDLE *phServer, 
	IN VARIANT *pItemValues, 
	OUT DWORD *pTransactionID, 
	OUT HRESULT **ppErrors)
{ 
	_TRACE(TL_DEB, TG_GRP, (_T("> AsyncWrite cnt:%lu"), dwNumItems));
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);
	GenericPointer<OPCTransaction> tac;
	HRESULT res;

	// out parameter 
	if (!pTransactionID)
	{
		_WARNING(TG_GRP, OPCENGINE_WRN_IF_PRM,(_T("AsyncWrite: bad output parameter")));
		return E_INVALIDARG;
	}
	*pTransactionID = 0;

	if (m_me->isRemoved())
	{
		if (ppErrors)
			*ppErrors = NULL;
		_TRACE(TL_INF, TG_GRP, (_T("group already removed")));
		return E_FAIL;
	}

	// check if valid read connection
	if (!m_me->validConnection(dwConnection, FALSE))
	{
		if (ppErrors)
			*ppErrors = NULL;
		_WARNING(TG_GRP, OPCENGINE_WRN_BAD_CONNECTION,(_T("No valid write connection %lu"), dwConnection));
		return CONNECT_E_NOCONNECTION;
	}

	// item array standard parameter check
	if (FAILED(res = itemArrayCheckAndAlloc(dwNumItems, phServer, pItemValues, (void **)ppErrors, ppErrors)))
		return res;

	res = createTransaction(FALSE, OPCTransaction::dataObject, dwConnection, OPC_DS_DEVICE, dwNumItems, phServer, pItemValues, (OPCTransaction **)tac, pTransactionID, ppErrors);

	if (S_OK == res)
	{ // transaction was created with no error
		// -> start async write transaction
		res = m_me->asyncWrite(tac, _T(""));
	}
	else
	{ // transaction was created with an error
		if (SUCCEEDED(res))
		{ // result was S_FALSE (invalid server handle)
			// free transaction
			*pTransactionID = 0;
			tac.release();	// free transaction
		}
		else
		{ // failed return code
			// free item specific errors 
			CoTaskMemFree(*ppErrors);		
			*ppErrors = NULL;
		}
	}

	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IOPCAsyncIO::Write: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_GRP, (_T("< AsyncWrite (0x%X)"), res));
	return res; 
}

STDMETHODIMP OPCGroup::Refresh(
	IN DWORD dwConnection, 
	IN OPCDATASOURCE dwSource,
	OUT DWORD *pTransactionID)
{ 
	_TRACE(TL_DEB, TG_GRP, (_T("> AsyncRefresh 0x%X"), dwConnection));
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);
	GenericPointer<OPCTransaction> dest;
	HRESULT res;

	// out parameter 
	if (!pTransactionID)
	{
		_WARNING(TG_GRP, OPCENGINE_WRN_IF_PRM,(_T("Refresh: bad output parameter")));
		return E_INVALIDARG;
	}
	*pTransactionID = 0;

	// check the dwSource parameter
	if ((dwSource != OPC_DS_CACHE) && (dwSource != OPC_DS_DEVICE))
		return E_INVALIDARG;

	if (m_me->isRemoved())
	{
		_TRACE(TL_INF, TG_GRP, (_T("group already removed")));
		return E_FAIL;
	}

	// check if valid read connection
	if (!m_me->validConnection(dwConnection, TRUE))
	{
		_WARNING(TG_GRP, OPCENGINE_WRN_BAD_CONNECTION,(_T("No valid read connection %lu"), dwConnection));
		return CONNECT_E_NOCONNECTION;
	}

	// check group and item activation 
	if ((!m_me->getActive()) || 
		((m_me->getUpdateMode() == GenericGroup::poll) && (0 == m_me->m_cyclicTransaction->getRequestCount())))
	{
		_TRACE(TL_INF, TG_GRP, (_T("group not active or no active item in group")));
		return E_FAIL;
	}

	copyCyclicTransaction(dwConnection, dwSource, OPCTransaction::dataObject, (OPCTransaction **)dest);

	if (dest->getRequestCount() == 0)
	{
		_TRACE(TL_INF, TG_GRP, (_T("no active item in group")));
		return E_FAIL;
	}

	res = m_me->asyncRefresh(dest, _T(""));

	if (SUCCEEDED(res))
	{
		*pTransactionID = dest->getTransactionId();
	}

	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IOPCAsyncIO::Refresh: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_GRP, (_T("< AsyncRefresh (0x%X)"), res));
	return res; 
}


STDMETHODIMP OPCGroup::Cancel(
	IN DWORD dwTransactionID)
{ 
	_TRACE(TL_DEB, TG_GRP, (_T("> AsyncCancel 0x%X"), dwTransactionID));
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);
	if (m_me->isRemoved())
	{
		_TRACE(TL_INF, TG_GRP, (_T("group already removed")));
		return E_FAIL;
	}

	HRESULT res;
	res = m_me->asyncCancel(dwTransactionID, _T(""));
	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IOPCAsyncIO::Cancel: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_GRP, (_T("< AsyncCancel (0x%X)"), res));
	return res;
}


BOOL OPCGroup::connectedDataCallbackInterface(void)
{
	IConnectionPointImpl<OPCGroup, &IID_IOPCDataCallback, CComDynamicUnkArray>* p = this;
	return (p->m_vec.begin() < p->m_vec.end());
}


STDMETHODIMP OPCGroup::Read(
	IN DWORD dwNumItems, 
	IN OPCHANDLE *phServer, 
	IN DWORD dwTransactionID, 
	OUT DWORD *pdwCancelID, 
	OUT HRESULT **ppErrors)
{
	_TRACE(TL_DEB, TG_GRP, (_T("> AsyncRead2 cnt:%lu"), dwNumItems));
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);
	HRESULT res;
	GenericPointer<OPCTransaction> tac;

	// out parameter ok
	if (!pdwCancelID)
	{
		_WARNING(TG_GRP, OPCENGINE_WRN_IF_PRM,(_T("AsyncRead2: bad output parameter")));
		return E_INVALIDARG;
	}
	*pdwCancelID = 0;

	if (m_me->isRemoved())
	{
		if (ppErrors)
			*ppErrors = NULL;
		_TRACE(TL_INF, TG_GRP, (_T("group already removed")));
		return E_FAIL;
	}

	// check if connection has been established
	if (!connectedDataCallbackInterface())
	{
		if (ppErrors)
			*ppErrors = NULL;
		_WARNING(TG_GRP, OPCENGINE_WRN_BAD_CONNECTION,(_T("No connection registered")));
		return CONNECT_E_NOCONNECTION;
	}

	// item array standard parameter check
	if (FAILED(res = itemArrayCheckAndAlloc(dwNumItems, phServer, phServer, (void **)ppErrors, ppErrors)))
		return res;

	res = createTransaction(TRUE, OPCTransaction::connectionPoint, 0, OPC_DS_DEVICE, dwNumItems, phServer, NULL, (OPCTransaction **)tac, pdwCancelID, ppErrors);

	if (SUCCEEDED(res))
	{ // transaction was created with no error
		// set client transaction id
		tac->setClientTransactionId(dwTransactionID);

		// -> start async read transaction
		res = m_me->asyncRead(tac, _T(""));
	}
	else
	{ // transaction was created with an error
		// free item specific errors 
		CoTaskMemFree(*ppErrors);		
		*ppErrors = NULL;
	}

	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IOPCAsyncIO2::Read: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_GRP, (_T("< AsyncRead2 (0x%X)"), res));
	return res; 
}

STDMETHODIMP OPCGroup::Write(
	IN DWORD dwNumItems, 
	IN OPCHANDLE *phServer, 
	IN VARIANT *pItemValues, 
	IN DWORD dwTransactionID, 
	OUT DWORD *pdwCancelID, 
	OUT HRESULT **ppErrors)
{
	_TRACE(TL_DEB, TG_GRP, (_T("> AsyncWrite2 cnt:%lu"), dwNumItems));
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);
	HRESULT res;
	GenericPointer<OPCTransaction> tac;

	// out parameter ok
	if (!pdwCancelID)
	{
		_WARNING(TG_GRP, OPCENGINE_WRN_IF_PRM,(_T("AsyncWrite2: bad output parameter")));
		return E_INVALIDARG;
	}
	*pdwCancelID = 0;

	if (m_me->isRemoved())
	{
		if (ppErrors)
			*ppErrors = NULL;
		_TRACE(TL_INF, TG_GRP, (_T("group already removed")));
		return E_FAIL;
	}

	// check if connection has been established
	if (!connectedDataCallbackInterface())
	{
		if (ppErrors)
			*ppErrors = NULL;
		_WARNING(TG_GRP, OPCENGINE_WRN_BAD_CONNECTION,(_T("No connection registered")));
		return CONNECT_E_NOCONNECTION;
	}

	// item array standard parameter check
	if (FAILED(res = itemArrayCheckAndAlloc(dwNumItems, phServer, pItemValues, (void **)ppErrors, ppErrors)))
		return res;

	res = createTransaction(FALSE, OPCTransaction::connectionPoint, 0, OPC_DS_DEVICE, dwNumItems, phServer, pItemValues, (OPCTransaction **)tac, pdwCancelID, ppErrors);

	if (SUCCEEDED(res))
	{ // transaction was created with no error
		// set client transaction id
		tac->setClientTransactionId(dwTransactionID);

		// -> start async write transaction
		res = m_me->asyncWrite(tac, _T(""));
	}
	else
	{ // transaction was created with an error
		// free item specific errors 
		CoTaskMemFree(*ppErrors);		
		*ppErrors = NULL;
	}

	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IOPCAsyncIO2::Write: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_GRP, (_T("< AsyncWrite2 (0x%X)"), res));
	return res; 
}

STDMETHODIMP OPCGroup::Refresh2(
	IN OPCDATASOURCE dwSource, 
	IN DWORD dwTransactionID, 
	OUT DWORD *pdwCancelID)
{
	_TRACE(TL_DEB, TG_GRP, (_T("> AsyncRefresh2")));
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);
	GenericPointer<OPCTransaction> dest;
	HRESULT res;

	// out parameter 
	if (!pdwCancelID)
	{
		_WARNING(TG_GRP, OPCENGINE_WRN_IF_PRM,(_T("Refresh2: bad output parameter")));
		return E_INVALIDARG;
	}
	*pdwCancelID = 0;

	// check the dwSource parameter
	if ((dwSource != OPC_DS_CACHE) && (dwSource != OPC_DS_DEVICE))
		return E_INVALIDARG;

	if (m_me->isRemoved())
	{
		_TRACE(TL_INF, TG_GRP, (_T("group already removed")));
		return E_FAIL;
	}

	// check if connection registered
	if (!connectedDataCallbackInterface())
	{
		_WARNING(TG_GRP, OPCENGINE_WRN_BAD_CONNECTION,(_T("No connection registered")));
		return CONNECT_E_NOCONNECTION;
	}

	// check group and item activation 
	if ((!m_me->getActive()) || 
		((m_me->getUpdateMode() == GenericGroup::poll) && (0 == m_me->m_cyclicTransaction->getRequestCount())))
	{
		_TRACE(TL_INF, TG_GRP, (_T("group not active or no active item in group")));
		return E_FAIL;
	}

	copyCyclicTransaction(0, dwSource, OPCTransaction::connectionPoint, (OPCTransaction **)dest);
	dest->setClientTransactionId(dwTransactionID);

	if (dest->getRequestCount() == 0)
	{
		_TRACE(TL_INF, TG_GRP, (_T("no active item in group")));
		return E_FAIL;
	}

	res = m_me->asyncRefresh(dest, _T(""));

	if (SUCCEEDED(res))
	{
		*pdwCancelID = dest->getTransactionId();
	}

	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IOPCAsyncIO2::Refresh2: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_GRP, (_T("< AsyncRefresh2 (0x%X)"), res));
	return res; 
}


STDMETHODIMP OPCGroup::Cancel2(
	IN DWORD dwCancelID)
{ 
	_TRACE(TL_DEB, TG_GRP, (_T("> AsyncCancel2 0x%X"), dwCancelID));
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);
	if (m_me->isRemoved())
	{
		_TRACE(TL_INF, TG_GRP, (_T("group already removed")));
		return E_FAIL;
	}

	HRESULT res;
	res = m_me->asyncCancel(dwCancelID, _T(""));
	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IOPCAsyncIO2::Cancel2: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_GRP, (_T("< AsyncCancel2 (0x%X)"), res));
	return res;
}

STDMETHODIMP OPCGroup::SetEnable(
	IN BOOL bEnable)
{ 
	_TRACE(TL_DEB, TG_GRP, (_T("> SetEnable %s"), BOOL2STR(bEnable)));
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);
	HRESULT res;

	if (m_me->isRemoved())
	{
		_TRACE(TL_INF, TG_GRP, (_T("group already removed")));
		return E_FAIL;
	}

	// check if connection registered
	if (!connectedDataCallbackInterface())
	{
		_WARNING(TG_GRP, OPCENGINE_WRN_BAD_CONNECTION,(_T("No connection registered")));
		return CONNECT_E_NOCONNECTION;
	}

	if (m_me->setEnable(bEnable))
		res = S_OK;
	else
		res = E_FAIL;

	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IOPCAsyncIO2::SetEnable: %s"), _TRACE_ELAPSED_TIME));
	return res;
}

STDMETHODIMP OPCGroup::GetEnable(
	OUT BOOL *pbEnable)
{ 
	_TRACE(TL_DEB, TG_GRP, (_T("> GetEnable")));
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);

	// out parameter 
	if (!pbEnable)
	{
		_WARNING(TG_GRP, OPCENGINE_WRN_IF_PRM,(_T("GetEnable: bad output parameter")));
		return E_INVALIDARG;
	}
	*pbEnable = TRUE;

	if (m_me->isRemoved())
	{
		_TRACE(TL_INF, TG_GRP, (_T("group already removed")));
		return E_FAIL;
	}

	// check if connection registered
	if (!connectedDataCallbackInterface())
	{
		_WARNING(TG_GRP, OPCENGINE_WRN_BAD_CONNECTION,(_T("No connection registered")));
		return CONNECT_E_NOCONNECTION;
	}

	*pbEnable = m_me->getEnable();

	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IOPCAsyncIO2::GetEnable: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_GRP, (_T("< GetEnable %s"), BOOL2STR(*pbEnable)));
	return S_OK;
}


void OPCGroup::copyCyclicTransaction(
	IN DWORD dwConnection, 
	IN OPCDATASOURCE dwSource,
	IN BYTE connectionType,
	OUT OPCTransaction **dest)
{
	_TRACE(TL_DEB, TG_GRP, (_T("> copyCyclicTransaction")));
	GenericPointer<OPCTransaction> source = m_me->getCyclicTransaction();
	GenericList<GenericRequest> destList;
	GenericPointer<GenericServer> srv = (GenericServer *)m_me->getParentBranch();
	GenericPointer<OPCEngine> engine = ::getOPCEngine();
	GenericPointer<GenericCreator> creator = engine->getCreator();
	GenericRequest::requestType reqType;
	GenericRequest *destReq;
	GenericPointer<GenericItemTag> tag;
	POSITION pos;

	if (dwSource == OPC_DS_CACHE)
		reqType = GenericRequest::asyncCache;
	else
		reqType = GenericRequest::asyncDevice;

	*dest = new OPCTransaction(OPCTransaction::refresh, m_me, connectionType, dwConnection);
	destList = ((*dest)->getRequestList());

	if (source->getType() == OPCTransaction::cyclic)
	{
		GenericPointer<GenericItem> item;
		GenericRequest *sourceReq;
		GenericList<GenericRequest> srcList(source->getRequestList());

		destList.lock();
		srcList.lock();

		// copy requests of cyclic transaction
		pos = srcList.getStartPosition();
		while (pos)
		{
			sourceReq = srcList.getNext(pos);
			item = sourceReq->getItem();
			tag = item->getItemTag();
			destReq = creator->createRequest(srv, item, tag, GenericRequest::read, reqType, (GenericRequest::requestDeviceIOMode)tag->getDeviceIOMode());
			destReq->setItem(item);
			destReq->setCompletedEvent(srv->getStartEvent());
			destList.add(destReq);
			destReq->release();
		}
	}
	else
	{
		// report transaction
		GenericList<GenericLeaf> itemList(m_me->getLeafList());
		GenericItem *item;

		destList.lock();
		itemList.lock();

		pos = itemList.getStartPosition();
		while (pos)
		{
			item = (GenericItem *)itemList.getNext(pos);
			if (item->getActive())
			{
				tag = item->getItemTag();
				destReq = creator->createRequest(srv, item, tag, GenericRequest::read, reqType, (GenericRequest::requestDeviceIOMode)tag->getDeviceIOMode());
				destReq->setItem(item);
				destReq->setCompletedEvent(srv->getStartEvent());
				destList.add(destReq);
				destReq->release();
			}
		}
	}

	_TRACE(TL_DEB, TG_GRP, (_T("< copyCyclicTransaction")));
}

STDMETHODIMP OPCGroup::Advise(
	IN IUnknown* pUnk, 
	OUT DWORD* pdwCookie)
{
	HRESULT res;

	_TRACE(TL_DEB, TG_GRP, (_T("> Advise")));
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);

	if(!pdwCookie)
	{
		_WARNING(TG_GRP, OPCENGINE_WRN_IF_PRM,(_T("Advise: bad output parameter")));
		return E_INVALIDARG ;
	}
	*pdwCookie = 0;

	if (m_me->isRemoved())	// group already removed ?
	{
		_TRACE(TL_INF, TG_GRP, (_T("group already removed")));
		return E_FAIL;
	}

	CSingleLock lock(getAdvisesLockObj(), TRUE);
	res = IConnectionPointImpl<OPCGroup, &IID_IOPCDataCallback, CComDynamicUnkArray>::Advise(pUnk, pdwCookie);

	if (SUCCEEDED(res))
	{
		res = m_me->advise2(pUnk, *pdwCookie, _T(""));

		if (FAILED(res))
		{
			IConnectionPointImpl<OPCGroup, &IID_IOPCDataCallback, CComDynamicUnkArray>::Unadvise(*pdwCookie);			
		}
	}
	lock.Unlock();

    if (SUCCEEDED(res))
	{
		GenericPointer<OPCTransaction> dest;
		copyCyclicTransaction(*pdwCookie, OPC_DS_CACHE, OPCTransaction::connectionPoint, (OPCTransaction **)dest);
		dest->setTransactionId(0);	// no cyclic transaction but id is 0
		m_me->asyncRefresh(dest, _T(""));
	}

	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IConnectionPoint::Advise: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_GRP, (_T("< Advise %X (0x%X)"), *pdwCookie, res));
	return res;
}

STDMETHODIMP OPCGroup::Unadvise (IN DWORD dwCookie)
{
	HRESULT res;

	_TRACE(TL_DEB, TG_GRP, (_T("> Unadvise 0x%X"), dwCookie));
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);
	if (m_me->isRemoved())	// group already removed ?
	{
		_TRACE(TL_INF, TG_GRP, (_T("group already removed")));
		return E_FAIL;
	}

	CSingleLock lock(getAdvisesLockObj(), TRUE);
	res = IConnectionPointImpl<OPCGroup, &IID_IOPCDataCallback, CComDynamicUnkArray>::Unadvise(dwCookie);
	if (SUCCEEDED(res))
		m_me->unadvise2(dwCookie, _T(""));

	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IConnectionPoint::Unadvise: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_GRP, (_T("< Unadvise (0x%X)"), res));
	return res;
}


STDMETHODIMP OPCGroup::DAdvise(
	IN FORMATETC* format, 
	IN DWORD advf, 
	IN IAdviseSink* pAdvSink, 
	OUT DWORD *pdwConnection)
{ 
	HRESULT res;

	_TRACE(TL_DEB, TG_GRP, (_T("> DAdvise")));
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);
	if(!pdwConnection)
	{
		_WARNING(TG_GRP, OPCENGINE_WRN_IF_PRM,(_T("DAdvise: bad output parameter")));
		return E_INVALIDARG ;
	}
	*pdwConnection = 0;
	if(format->dwAspect != DVASPECT_CONTENT)
	{
		_WARNING(TG_GRP, OPCENGINE_WRN_IF_PRM,(_T("DAdvise: bad draw aspect")));
		return DV_E_DVASPECT ;
	}
	if(format->ptd != NULL)
	{
		_WARNING(TG_GRP, OPCENGINE_WRN_IF_PRM,(_T("DAdvise: bad ptd")));
		return E_INVALIDARG;
	}
	if(format->tymed != TYMED_HGLOBAL)
	{
		_WARNING(TG_GRP, OPCENGINE_WRN_IF_PRM,(_T("DAdvise: bad tymed")));
		return DV_E_TYMED ;
	}
	if(format->lindex != -1)
	{
		_WARNING(TG_GRP, OPCENGINE_WRN_IF_PRM,(_T("DAdvise: bad lindex")));
		return DV_E_LINDEX;
	}

    if ( (format->cfFormat != OPCSTMFORMATDATA) &&
		 (format->cfFormat != OPCSTMFORMATDATATIME) &&
		 (format->cfFormat != OPCSTMFORMATWRITECOMPLETE))
	{
		_WARNING(TG_GRP, OPCENGINE_WRN_IF_PRM,(_T("DAdvise: bad format")));
        return DV_E_FORMATETC;
	}

	if (m_me->isRemoved())	// group already removed ?
	{
		_TRACE(TL_INF, TG_GRP, (_T("group already removed")));
		return E_FAIL;
	}

	res = m_me->advise(this, format, advf, pAdvSink, _T(""), pdwConnection);

    if ( (format->cfFormat == OPCSTMFORMATDATA) ||
		 (format->cfFormat == OPCSTMFORMATDATATIME) )
	{
		GenericPointer<OPCTransaction> dest;
		copyCyclicTransaction(*pdwConnection, OPC_DS_CACHE, OPCTransaction::dataObject, (OPCTransaction **)dest);
		dest->setTransactionId(0);	// no cyclic transaction but id is 0
		m_me->asyncRefresh(dest, _T(""));
	}

	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IDataObject::Advise: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_GRP, (_T("< DAdvise %X (0x%X)"), *pdwConnection, res));
	return res;
}

STDMETHODIMP OPCGroup::DUnadvise(
	IN DWORD dwConnection)
{ 
	_TRACE(TL_DEB, TG_GRP, (_T("> DUnadvise 0x%X"), dwConnection));
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);
	if (m_me->isRemoved())	// group already removed ?
	{
		_TRACE(TL_INF, TG_GRP, (_T("group already removed")));
		return E_FAIL;
	}

	HRESULT res;
	res = m_me->unadvise(dwConnection, _T(""));
	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IDataObject::DUnadvise: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_GRP, (_T("< DUnadvise (0x%X)"), res));
	return res;
}

STDMETHODIMP OPCGroup::EnumDAdvise(
	OUT IEnumSTATDATA **ppenumAdvise)
{ return E_NOTIMPL; }

STDMETHODIMP OPCGroup::GetData(
	IN FORMATETC *formatIn, 
	OUT STGMEDIUM *medium)
{ return DV_E_FORMATETC; }

STDMETHODIMP OPCGroup::QueryGetData(
	IN FORMATETC *format)
{ return DV_E_FORMATETC; }

STDMETHODIMP OPCGroup::GetDataHere(
	IN FORMATETC *format, 
	OUT STGMEDIUM *medium)
{ return DV_E_FORMATETC; }
  
STDMETHODIMP OPCGroup::GetCanonicalFormatEtc(
	IN FORMATETC* formatIn,
	OUT FORMATETC* formatOut)
{ return DV_E_FORMATETC; }
  
STDMETHODIMP OPCGroup::SetData(
	IN FORMATETC* format, 
	IN STGMEDIUM* medium,
	OUT BOOL fRelease)
{ return E_NOTIMPL; }

STDMETHODIMP OPCGroup::EnumFormatEtc(
	IN DWORD dwDirection, 
	OUT IEnumFORMATETC** ppenumFormatEtc)
{ return E_NOTIMPL; }


void OPCGroup::releaseServer(void)
{ 
	if (m_server)	
	{ 
		((OPCObject<OPCServer> *)m_server)->internalRelease(); 
		m_server = NULL;
	}
}


