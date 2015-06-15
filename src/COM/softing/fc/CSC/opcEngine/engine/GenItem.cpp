//-----------------------------------------------------------------------------
//                                  OPC ENGINE                                |
//                                                                            |
//  Filename    : GENITEM.CPP                                                 |   
//  Version     : 2.00.0.00.release                                           |
//  Date        : 29-March-1999                                               |
//  Author      : ATCM			                                              |
//                                                                            |
//  Description : Item object related classes                                 |
//                - GenericItem: OPC item object                              |
//                                                                            |
//  CHANGE_NOTES                                                              |
//                                                                            |
//  date      name      change                                                |
//  ------------------------------------------------------------------------- |
//  01/08/99  RT        moved queue data from item to item tag                |
//                                                                            |
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "GenItem.h"
#include "GenCache.h"
#include "GenNS.h"
#include "GenSrv.h"
#include "OPCEngine.h"
#include "OPCTrace.h"


CCriticalSection g_cycDataLock;

//-----------------------------------------------------------------------------
// GenericItem                                                                |
//-----------------------------------------------------------------------------

GenericItem::GenericItem(void)
 : GenericLeaf(GENOBJ_TYPE_ITEM)
{
	m_accessPath = _T("?");
	m_itemTag = NULL;
	m_active = FALSE;
	m_reqDatatype = VT_EMPTY;
	m_clientHandle = 0;
	m_serverHandle = 0;
	m_cycData = NULL;
}

GenericItem::~GenericItem()
{
	setCyclicData(NULL);
	// release objects
	setItemTag(NULL);
}


//-----------------------------------------------------------------------------
// getCache
//
// - get cache object
//
// returns:
//		pointer to cache object (maybe NULL)
//
GenericCache *GenericItem::getCache(void)
{ 
	if (m_itemTag)
		return m_itemTag->getCache(); 
	return NULL; 
}


//-----------------------------------------------------------------------------
// setItemTag
//
// - set item tag object
// - release previous tag
//
// returns:
//		TRUE
//
BOOL GenericItem::setItemTag(
	IN GenericItemTag *tag) // tag object
{ 
	onSetItemTag(tag);
	if (m_itemTag)
		m_itemTag->release();

	if (tag)
	{
		_TRACE(TL_INF, TG_ITEM, (_T("set item tag to %s"), tag->getObjTraceId()));
		tag->addRef();
	}
	else
	{
		_TRACE(TL_INF, TG_ITEM, (_T("set item tag to NULL")));
	}

	m_objectTraceId.Empty();		
	m_itemTag = tag;
	return TRUE;
} // setItemTag


//-----------------------------------------------------------------------------
// getItemTag
//
// - get tag object
//
// returns:
//		pointer to tag object (maybe NULL)
//
GenericItemTag *GenericItem::getItemTag(void)
{ 
	if (m_itemTag)
		m_itemTag->addRef(); 
	return m_itemTag; 
} // getItemTag


//-----------------------------------------------------------------------------
// setReqDatatype
//
// - set requested data type
//
// returns:
//		S_OK          - support data type
//		OPC_E_BADTYPE - not support data type
//
HRESULT GenericItem::setReqDatatype(
	IN VARTYPE reqDatatype)	// requested data type
{
	HRESULT res;

	if (FAILED(res = m_itemTag->supportReqDatatype(reqDatatype)))
		return res;

	_TRACE(TL_INF, TG_ITEM, (_T("set requested datatype to %u"), reqDatatype));
	onSetReqDatatype(reqDatatype);
	m_reqDatatype = reqDatatype;
	return S_OK;
} // setReqDatatype


//-----------------------------------------------------------------------------
// setAccessPath
//
// - set access path
//
// returns:
//		TRUE
//
BOOL GenericItem::setAccessPath(
	IN LPCTSTR accessPath)	// access path
{
	_TRACE(TL_INF, TG_ITEM, (_T("set access path to %s"), accessPath));
	onSetAccessPath(accessPath);
	m_accessPath = accessPath;
	return TRUE;
} // setAccessPath


//-----------------------------------------------------------------------------
// setActive
//
// - set activation state
//
// returns:
//		TRUE
//
BOOL GenericItem::setActive(
	IN BOOL active)	// activation state
{
	GenericPointer<GenericGroup> grp = (GenericGroup *)getParentBranch();
	if (!grp)
		return FALSE;
	GenericPointer<GenericServer> srv = (GenericServer *)(grp->getParentBranch());
	if (!srv)
		return FALSE;

	BOOL prevActive = m_active;

	if (active)
		active = TRUE;	// set 'NOT FALSE(!=0)' to 'TRUE(1)'

	onSetActive(active);
	m_active = (BYTE)active;
	_TRACE(TL_INF, TG_ITEM, (_T("set active to %s"), BOOL2STR(active)));

	// check if to start polling the item value
	if ((m_active) && (!prevActive))
	{ // item was activated 
		if ((m_itemTag->getAccessRights() & OPC_READABLE) == OPC_READABLE)
		{ // tag of item is readable
			// -> start polling
			srv->insertCyclicRequest(this);
		}
	}

	// check if to stop polling the item value
	if ((!m_active) && (prevActive))
	{ // item was deactivated
		// -> stop polling
		srv->removeCyclicRequest(this);
	}

	return TRUE;
} // setActive


//-----------------------------------------------------------------------------
// setClientHandle
//
// - set client handle
//
// returns:
//		TRUE
//
BOOL GenericItem::setClientHandle(
	IN OPCHANDLE clientHandle)	// client handle
{
	_TRACE(TL_INF, TG_ITEM, (_T("set client handle to 0x%X"), clientHandle));
	onSetClientHandle(clientHandle);
	m_clientHandle = clientHandle;
	return TRUE;
} // setClientHandle


//-----------------------------------------------------------------------------
// onRead
//
// - called after completed read
// - default implementation: do nothing
//
void GenericItem::onRead(
	IN BOOL deviceRead)	// device or cache read
{} // onRead


//-----------------------------------------------------------------------------
// onWrite
//
// - called after completed write
// - default implementation: do nothing
//
void GenericItem::onWrite(void)
{} // onWrite


//-----------------------------------------------------------------------------
// onSetActive
//
// - called after activation state is set
// - default implementation: do nothing
//
void GenericItem::onSetActive(
	IN BOOL active)	// activation state
{} // onSetActive


//-----------------------------------------------------------------------------
// onSetClientHandle
//
// - called after client handle is set
// - default implementation: do nothing
//
void GenericItem::onSetClientHandle(
	IN OPCHANDLE clientHandle)	// client handle
{} // onSetClientHandle


//-----------------------------------------------------------------------------
// onSetReqDatatype
//
// - called after requested data type is set
// - default implementation: do nothing
//
void GenericItem::onSetReqDatatype(
	IN VARTYPE requestedType)	// requested data type
{} // onSetReqDatatype


//-----------------------------------------------------------------------------
// onSetAccessPath
//
// - called after access path is set
// - default implementation: do nothing
//
void GenericItem::onSetAccessPath(
	IN LPCTSTR accessPath)	// access path
{} // onSetAccessPath


//-----------------------------------------------------------------------------
// onSetItemTag
//
// - called after access path is set
// - default implementation: do nothing
//
void GenericItem::onSetItemTag(
	IN GenericItemTag *tagItem)	// item tag
{} // onSetItemTag


//-----------------------------------------------------------------------------
// getObjTraceId
//
// - get objects trace string id
//
// returns:
//		trace id string
//
LPCTSTR GenericItem::getObjTraceId(void)
{ 
	if (m_objectTraceId.IsEmpty())
	{
		CString name;
		if (m_itemTag)
			name = m_itemTag->getName();
		else
			name = getName();
		m_objectTraceId.Format(_T("[%s|%8.8X| %s]"), getObjTypeString(), this, name);
		m_objectTraceId.FreeExtra();
	}

	return m_objectTraceId; 
} // getObjTraceId


void GenericItem::setCyclicData(GenericItemCyclicData *cycData)
{ 
	lockCyclicData();
	if (m_cycData)
	{
		m_cycData->m_sendValues.RemoveAll();	
		m_cycData->m_sendQualities.RemoveAll();	
		delete m_cycData;
	}

	m_cycData = cycData;
	unlockCyclicData();
}
