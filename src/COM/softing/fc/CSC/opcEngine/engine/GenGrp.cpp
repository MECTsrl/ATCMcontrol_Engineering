//-----------------------------------------------------------------------------
//                                  OPC ENGINE                                |
//                                                                            |
//  Filename    : GENGRP.CPP                                                  |   
//  Version     : 2.01.0.00.release                                           |
//  Date        : 7-May-1999                                                  |
//  Author      : ATCM                                                 |
//                                                                            |
//  Description : GenericGroup class                                          |
//                                                                            |
//  CHANGE_NOTES                                                              | 
//                                                                            |
//  date      name      change                                                |
//  ------------------------------------------------------------------------- |
//                                                                            |
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "GenGrp.h"
#include "GenNS.h"
#include "OPCEngine.h"
#include "OPCTrace.h"
#include "GenSrv.h"
#include "GenItem.h"
#include "GenCache.h"
#include "globals.h"
#include <opcerror.h>


//-----------------------------------------------------------------------------
// GenericGroup                                                               |
//-----------------------------------------------------------------------------

GenericGroup::GenericGroup(
	IN OPTIONAL BYTE updateMode,	// update mode of the group
	IN OPTIONAL BYTE whatItemList,		// generic list type of item list
	IN OPTIONAL int blockSizeItemList,	// block size of item list
	IN OPTIONAL int hashSizeItemList)	// hash table size of item map
 : GenericBranch(GENOBJ_TYPE_GROUP, GENOBJ_TYPE_LEAF, whatItemList, 0, blockSizeItemList, 0, hashSizeItemList, 0)
{
	_TRACE(TL_DEB, TG_GRP, (_T("> constructor")));
	TIME_ZONE_INFORMATION timeZone;
	DWORD timeZoneRet;
	// set system default time zone
	timeZoneRet = GetTimeZoneInformation(&timeZone);

	m_me = NULL;
	m_active = FALSE;
	m_updateRate = 1000;
	m_clientHandle = 0;
	if (timeZoneRet == TIME_ZONE_ID_DAYLIGHT)
		m_timeBias = timeZone.Bias + timeZone.StandardBias + timeZone.DaylightBias;
	else
		m_timeBias = timeZone.Bias + timeZone.StandardBias;
	m_deadBand = 0.0;
	m_lcid = 0;
	m_serverHandle = (OPCHANDLE)this;
	if (updateMode == poll)
		m_cyclicTransaction = new OPCTransaction(OPCTransaction::cyclic, this, OPCTransaction::allConnections);
	else
		m_cyclicTransaction = new OPCTransaction(OPCTransaction::report, this, OPCTransaction::allConnections);
	m_enable = TRUE;

	m_initCnt = 0;
	m_correctNTtimer = 0;

	m_publicTemplate = NULL;
	m_removed = FALSE;
	m_initTime = 0;
	m_trustClientsItemHandles = FALSE;
	m_updateMode = updateMode;

	_TRACE(TL_DEB, TG_GRP, (_T("< constructor")));
}


GenericGroup::~GenericGroup(void)
{
	_TRACE(TL_DEB, TG_GRP, (_T("> destructor")));
	m_removed = TRUE;
	m_cyclicTransaction->release();
	m_advisesLock.Lock();
	m_advises.RemoveAll();
	m_advisesLock.Unlock();
	_TRACE(TL_DEB, TG_GRP, (_T("< destructor")));
}


HRESULT GenericGroup::setName(
	IN LPCTSTR name)
{
	HRESULT res;
	res = GenericElement::setName(name);
	if (SUCCEEDED(res))
	{
		m_cyclicTransaction->m_objectTraceId.Empty();
	}
	return res;
}

//-----------------------------------------------------------------------------
// getItemByHandle
//
// - retrieve item pointer by the server handle value
//
// returns:
//		pointer to the item
//		NULL - no item with this server handle
//
GenericItem *GenericGroup::getItemByHandle(
	IN OPCHANDLE serverHandle)	// server handle
{
	GenericItem *item;

	// check if to trust the client
	if (m_trustClientsItemHandles)
	{
		item = (GenericItem *)serverHandle;
		item->addRef();
		return item;
	}

	GenericList<GenericLeaf> list(getLeafList());
	CString itemKey;
	POSITION pos;

	// check item with standard name = key
	itemKey.Format(_T("ITM%08.8X"), serverHandle);
	list.lock();
	item = (GenericItem *)list.find(itemKey);
	if (item)
	{
		_TRACE(TL_DEB, TG_GRP, (_T("itemByHandle h:0x%X %s"), serverHandle, item->getObjTraceId()));
		return item;
	}

	// no item found -> may changed item name
	pos = list.getStartPosition();
	while(pos)
	{
		item = (GenericItem *)list.getNext(pos);
		if (item->getServerHandle() == serverHandle)
		{ // group found
			_TRACE(TL_DEB, TG_GRP, (_T("itemByHandle h:0x%X %s"), serverHandle, item->getObjTraceId()));
			item->addRef();
			return item;
		}
	}
	_TRACE(TL_INF, TG_GRP, (_T("no itemByHandle h:0x%X"), serverHandle));
	return NULL;
} // getItemByHandle


//-----------------------------------------------------------------------------
// validateItem 
//
// - validate item parameters 
//
// returns:
//		S_OK                - validated item parameters
//		OPC_E_INVALIDITEMID - invalid item id
//		OPC_E_UNKNOWNITEMID - no namespace tag with the item id
//		OPC_E_BADTYPE       - bad requested data type
//		OPC_E_UNKNOWNPATH   - unknown access path
//
HRESULT GenericGroup::validateItem(
	IN LPCTSTR id,					// item id
	IN LPCTSTR accessPath,			// access path
	IN VARTYPE reqDataType,			// requested data type
	OUT VARTYPE *nativeDataType,	// native data type 
	OUT DWORD *accessRights,		// access rights
	OUT OPTIONAL GenericItemTag **ppTag)// tag object
{ 
	_TRACE(TL_DEB, TG_GRP, (_T("> validateItem %s"), id));
	GenericPointer<OPCEngine> engine = ::getOPCEngine();
	GenericPointer<GenericServer> server = (GenericServer *)getParentBranch();
	GenericPointer<OPCRootNS> root;
	GenericPointer<GenericTag> tag;
	GenericPointer<GenericTagProperty> tagProp;
	GenericItemTag *itemTag;
	LPCTSTR bad = NULL;
	TCHAR propDel;
	TCHAR *propStart;
	DWORD propId = 0;
	CString itemId;
	HRESULT res = S_OK;
	BOOL ok = TRUE;

	if (!server)
		return E_FAIL;

	if (nativeDataType)	*nativeDataType = 0;
	if (accessRights) *accessRights = 0;

	// check if item id contains property id
	propDel = engine->getPropertyDelimiter();
	if ((propStart = _tcschr(id, propDel)) != NULL)
	{ // found property delimiter
		// remove property part from tag name
		itemId = id;
		*(((LPTSTR)(LPCTSTR)itemId) + (propStart - id)) = _T('\0');
		id = (LPCTSTR)itemId;
		propId = _tcstoul(propStart + 1, NULL, 0);
	}

	// validate item id -> check for not allowed chars
	engine->getTreeDelimiter(NULL, &bad);
	ok &= (_tcslen(id) != 0);
    ok &= (_tcspbrk(id, bad) == NULL);
	if (!ok)
	{
		_TRACE(TL_INF, TG_GRP, (_T("no valid item id %s"), id));
		return OPC_E_INVALIDITEMID;
	}

	// find namespace tag
	root = engine->getNamespaceRoot();
	tag = (GenericTag *)root->findLeaf(id, TRUE);
	if (!tag)
	{
		_TRACE(TL_DEB, TG_GRP, (_T("%s is no static tag"), id));
		tag = server->createTag(id);
		if (!tag)
		{
			_TRACE(TL_INF, TG_GRP, (_T("%s is no tag"), id));
			return OPC_E_UNKNOWNITEMID;
		}
		tag->setDynamic(TRUE);
	}
	itemTag = (GenericItemTag *)tag;

	// find property
	if (propId != 0)
	{
		tagProp = server->createTagProperty(tag, id, propId);
		if (!itemTag)
		{
			_TRACE(TL_INF, TG_GRP, (_T("%s has no property %lu"), id, propId));
			return OPC_E_UNKNOWNITEMID;
		}
		itemTag = (GenericItemTag *)tagProp;
	}

	if (nativeDataType) *nativeDataType = itemTag->getNativeDatatype();
	if (accessRights) *accessRights = itemTag->getAccessRights();

	// check data type
	if (FAILED(res = itemTag->supportReqDatatype(reqDataType)))
	{
		_TRACE(TL_INF, TG_GRP, (_T("bad datatype %u"), reqDataType));
		return res;
	}

	// check access path
	if (!itemTag->checkAccessPath(accessPath))
	{
		_TRACE(TL_INF, TG_GRP, (_T("%bad access path %s"), accessPath));
		return OPC_E_UNKNOWNPATH;
	}

#ifdef DEMO_VERSION
	// demo version limited items
	{
	GenericList<GenericLeaf> list(getLeafList());

	if (list.getCount() >= 3)
	{
		_TRACE(TL_INF, TG_GRP, (_T("DEMO Version!!! only two items per group")));
		return E_FAIL;
	}
	}
#endif

	// return the found tag
	if (ppTag)
	{
		*ppTag = itemTag;
		(*ppTag)->addRef();
	}
	
	_TRACE(TL_DEB, TG_GRP, (_T("< validateItem OK")));
	return S_OK; 
} // validateItem


//-----------------------------------------------------------------------------
// addItem 
//
// - add item to group
//
// returns:
//		S_OK                - validated item parameters
//		OPC_E_INVALIDITEMID - invalid item id
//		OPC_E_UNKNOWNITEMID - no namespace tag with the item id
//		OPC_E_BADTYPE       - bad requested data type
//		OPC_E_UNKNOWNPATH   - unknown access path
//
HRESULT GenericGroup::addItem(
	IN LPCTSTR id,					// item id
	IN LPCTSTR accessPath,			// access path
	IN BOOL active,					// activation state
	IN OPCHANDLE clientHandle,		// client handle
	IN VARTYPE reqDataType,			// requested data type
	OUT GenericItem **item,			// item object
	OUT VARTYPE *nativeDataType,	// native data type
	OUT DWORD *accessRights)		// access rights
{ 
	_TRACE(TL_DEB, TG_GRP, (_T("> addItem %s"), id));
	GenericPointer<GenericItemTag> tag;
	GenericItem *itm = NULL;
	HRESULT res;

	res = validateItem(id, accessPath, reqDataType, nativeDataType, accessRights, (GenericItemTag **)&tag);

	if (SUCCEEDED(res))
	{
		GenericPointer<OPCEngine> engine = ::getOPCEngine();
		CString itemName;
		GenericPointer<GenericCreator> creator;	

		// create OPC tree object
		creator = engine->getCreator();
		itm = creator->createItem(this, tag);
	
		if (itm == NULL)
			return E_OUTOFMEMORY;

		itemName.Format(_T("ITM%08.8X"), itm);

		// set name
		itm->setName(itemName);	
		// add to tree
		addLeaf(itm);

		// set item parameters
		itm->setItemTag(tag);
		_TRACE(TL_INF, TG_GRP, (_T("added item %s"), itm->getObjTraceId()));
		itm->setActive(active);
		itm->setClientHandle(clientHandle);
		itm->setReqDatatype(reqDataType);
		itm->setAccessPath(accessPath);
		itm->setClientHandle(clientHandle);
		itm->setServerHandle((OPCHANDLE)itm);
	}		

	if (item != NULL)
		*item = itm;
	else
	{
		if (itm)
			itm->release();
	}

	_TRACE(TL_DEB, TG_GRP, (_T("< addItem (0x%X)"), res));
	return res;
} // AddItem


//-----------------------------------------------------------------------------
// removeItem 
//
// - remove item fro, group
//
// returns:
//		S_OK                - removed item
//		OPC_E_INVALIDHANDLE - invalid item handle
//
HRESULT GenericGroup::removeItem(
	IN OPCHANDLE serverHandle)	// server handle
{ 
	_TRACE(TL_DEB, TG_GRP, (_T("> removeItem 0x%X"), serverHandle));
	GenericPointer<GenericItem> item;

	// get item
	item = getItemByHandle(serverHandle);
	if (!item)
	{
		return OPC_E_INVALIDHANDLE;
	}

	item->setActive(FALSE);
	removeLeaf(item);
	_TRACE(TL_INF, TG_GRP, (_T("removed item")));
	return S_OK; 
} // removeItem


//-----------------------------------------------------------------------------
// clone
//
// - creates a copy of the group
//
// returns:
//		S_OK                - created new group
//		E_INVALIDARG        - group name not valid
//		OPC_E_DUPLICATENAME - group name not unique
//
HRESULT GenericGroup::clone(
	IN GenericServer *server,	// server object of clone
	IN LPCTSTR name,			// group name
	OUT GenericGroup **clone)	// created clone
{ 
	_TRACE(TL_DEB, TG_GRP, (_T("> clone %s"), name));
	HRESULT res;
	LONG timeBias = getTimeBias();
	FLOAT deadband = getDeadBand();
	BOOL createPubTempl;
	OPCHANDLE clientHandle = NULL;

	if (!server)
		return E_FAIL;

	// creating a public template by moveToPublic?
	// -> set client handles to NULL
	createPubTempl = server->isPublicGroups();	
	if (!createPubTempl)
		clientHandle = getClientHandle();

	// create tree group object
	res = server->addGroup(name, FALSE, getUpdateRate(), clientHandle, 
			&timeBias, &deadband, getLCID(), clone, NULL);
	
	if (SUCCEEDED(res))
	{
		// cloning a public template?
		if (isPublicTemplate())
		{
			// -> creates a public group
			(*clone)->m_publicTemplate = this;
		}

		// clone all items
		GenericList<GenericLeaf> list(getLeafList());
		GenericPointer<GenericItemTag> tag;
		GenericItem *item;
		GenericItem *clonedItem;
		POSITION pos;
		VARTYPE canDT;		// native data type
		DWORD accessRights;	// access rights

		// for all items of the group
		list.lock();
		pos = list.getStartPosition();
		while (pos)
		{
			item = (GenericItem *)list.getNext(pos);
			tag = item->getItemTag();
			if (!createPubTempl)
				clientHandle = item->getClientHandle();

			if (SUCCEEDED((*clone)->addItem(tag->getFullName(), item->getAccessPath(), item->getActive(),
					clientHandle, item->getReqDatatype(), &clonedItem, 
					&canDT, &accessRights)))
			{
				clonedItem->release();
			}
		}
		_TRACE(TL_INF, TG_GRP, (_T("cloned group -> %s"), (*clone)->getObjTraceId()));
		onClone(*clone);
	}
	_TRACE(TL_DEB, TG_GRP, (_T("< clone (0x%X)"), res));
	return res; 
} // clone

BOOL GenericGroup::isPublicGroup(void) const
{
	return (NULL != m_publicTemplate);
}

BOOL GenericGroup::isPublicTemplate(void) const
{
	if (m_parent)
	{
		return ((GenericServer *)m_parent)->isPublicGroups();
	}
	return FALSE;
}

//-----------------------------------------------------------------------------
// isPublic
//
// - checks if the group is a public group
//
// returns:
//		TRUE  - public group
//		FALSE - private group
//
BOOL GenericGroup::isPublic(void)
{
	if (isPublicGroup())
		return TRUE;

	return isPublicTemplate();
} // isPublic


HRESULT GenericGroup::syncRead(
	IN RequestList &reqList,
	IN OPCDATASOURCE source,	// data source: cache or device 
	IN LPCTSTR user)
{ 
	_TRACE(TL_DEB, TG_GRP, (_T("> syncRead")));
	return ((GenericServer *)m_parent)->syncRead(reqList, source, user);
}

HRESULT GenericGroup::syncWrite(
	IN RequestList &reqList, 
	IN LPCTSTR user)
{ 
	_TRACE(TL_DEB, TG_GRP, (_T("> syncWrite")));
	POSITION pos;
	GenericRequest *req;	
	GenericPointer<GenericCache> cache;	
	GenericPointer<GenericItem> item;	
	HANDLE *events;
	HRESULT waitRes;
	DWORD i;
	DWORD waitCnt;
	DWORD waitTime;
	DWORD startWaitTime;
	DWORD timeSpan;
	DWORD reqCnt = reqList.GetCount();
	BOOL waitOK = TRUE;

	// device read must wait for events
	_TRACE(TL_INF, TG_GRP, (_T("sync device write; %d items"), reqCnt));
	events = (HANDLE *)malloc(sizeof(HANDLE) * reqCnt);
	if (!events)
		return E_OUTOFMEMORY;

	// start all requests
	i = 0;
	pos = reqList.GetHeadPosition();
	while (pos)
	{
		req = (GenericRequest *)reqList.GetNext(pos);
		req->start();
		// get the completed event
		events[i] = req->getCompletedEvent();
		i++;
	}

	// wait for completion
	_TRACE(TL_DEB, TG_GRP, (_T("wait for completion")));
	i = 0;
	startWaitTime = ::GetTickCount();
	do
	{
		timeSpan = getTimeSpan(startWaitTime, ::GetTickCount());		
		if (timeSpan < ((GenericServer *)m_parent)->getDuetimeWrite())
			waitTime =  ((GenericServer *)m_parent)->getDuetimeWrite() - timeSpan;
		else
			waitTime = 0;
		waitCnt = ((reqCnt - i) <= MAXIMUM_WAIT_OBJECTS) ? (reqCnt - i) : MAXIMUM_WAIT_OBJECTS;
		waitRes = ::WaitForMultipleObjects(waitCnt, &events[i], TRUE, waitTime);
		waitOK &= ((waitRes >= WAIT_OBJECT_0) && (waitRes < (HRESULT)(WAIT_OBJECT_0 + waitCnt)));
		i += waitCnt;
	} while (i < reqCnt);

	free(events);
	_TRACE(TL_DEB, TG_GRP, (_T("all requests completed or time ellapsed")));

	if (!waitOK)
	{
		// at least one write havn't completed
		pos = reqList.GetHeadPosition();
		while (pos)
		{
			req = (GenericRequest *)reqList.GetNext(pos);
			if (!req->isCompleted())
			{
				// not completed after due time
				// -> abort request
				_TRACE(TL_DEB, TG_GRP, (_T("cancel request %s; not completed in time"), req->getObjTraceId()));
				req->cancel(E_FAIL, TRUE);			
			}
		}
	}

	_TRACE(TL_DEB, TG_GRP, (_T("< syncWrite")));
	return S_OK; 
}

HRESULT GenericGroup::asyncRead(
	IN OPCTransaction* transaction,
	IN LPCTSTR user)
{ 
	_TRACE(TL_INF, TG_GRP, (_T("async read; transaction %s"), transaction->getObjTraceId()));
	transaction->startRequests();
	((GenericServer *)m_parent)->addTransaction(transaction);
	return S_OK; 
}

HRESULT GenericGroup::asyncWrite(
	IN OPCTransaction* transaction,
	IN LPCTSTR user)
{ 
	_TRACE(TL_INF, TG_GRP, (_T("async write; transaction %s"), transaction->getObjTraceId()));
	transaction->startRequests();
	((GenericServer *)m_parent)->addTransaction(transaction);
	return S_OK; 
}

HRESULT GenericGroup::asyncRefresh(
	IN OPCTransaction* transaction,
	IN LPCTSTR user)
{ 
	_TRACE(TL_INF, TG_GRP, (_T("async refresh; transaction %s"), transaction->getObjTraceId()));
	transaction->startRequests();
	((GenericServer *)m_parent)->addTransaction(transaction);
	return S_OK; 
}

HRESULT GenericGroup::asyncCancel(
	IN DWORD transactionId,
	IN LPCTSTR user)
{ 
	_TRACE(TL_INF, TG_GRP, (_T("async cancel; transactionId 0x%X"), transactionId));
	BOOL ret = ((GenericServer *)m_parent)->removeTransaction(NULL, transactionId);
	return (ret) ? S_OK : E_FAIL;
}

HRESULT GenericGroup::advise2(IN IUnknown* pUnk, IN DWORD cookie, IN LPCTSTR user)
{ 
	onAdvise2(cookie);
	return S_OK; 
}
void GenericGroup::unadvise2(IN DWORD cookie, IN LPCTSTR user)
{ 
	onUnadvise2(cookie);
}

HRESULT GenericGroup::advise(
	IN IDataObject *pDataObj, 
	IN FORMATETC* format, 
	IN DWORD advf, 
	IN IAdviseSink* pAdvSink, 
	IN LPCTSTR user, 
	OUT DWORD *connection)
{ 
	CSingleLock lock(&m_advisesLock, TRUE);
	OPCAdvise advise, search;
	DWORD idx;

	for (idx = 1; m_advises.Lookup(idx, search); idx++)
		;

	*connection = idx;
	advise.m_connection = idx;
	advise.m_format = format->cfFormat;
	advise.m_adviseSink = pAdvSink;
	pAdvSink->AddRef();
	m_advises.SetAt(idx, advise);
	onAdvise(advise.m_format, advise.m_connection);

	_TRACE(TL_INF, TG_GRP, (_T("advise; format:%s connection:0x%X"), CLIPFORMAT2STR(format->cfFormat),*connection));
	return S_OK;
}

HRESULT GenericGroup::unadvise(
	IN DWORD connection,
	IN LPCTSTR user)
{ 
	HRESULT res = OLE_E_NOCONNECTION;
	CSingleLock lock(&m_advisesLock, TRUE);
	
	if (m_advises.RemoveKey(connection))
	{
		onUnadvise(connection);
		res = S_OK;
	}

	_TRACE(TL_INF, TG_GRP, (_T("unadvise; connection:0x%X (0x%X)"), connection, res));
	return res;
}

BOOL GenericGroup::validConnection(
	IN DWORD connection, 
	IN BOOL read)
{
	_TRACE(TL_DEB, TG_GRP, (_T("> validConnection:0x%X read:%s"), connection, BOOL2STR(read)));
	BOOL valid = FALSE;
	OPCAdvise advise;

	if (m_advises.Lookup(connection, advise))
	{
		if (read)
		{
			if ((advise.m_format == OPCSTMFORMATDATA) ||
				(advise.m_format == OPCSTMFORMATDATATIME))
				valid = TRUE;
		}
		else
		{
			if (advise.m_format == OPCSTMFORMATWRITECOMPLETE) 
				valid = TRUE;
		}
	}

	_TRACE(TL_DEB, TG_GRP, (_T("> validConnection:%s"), BOOL2STR(valid)));
	return valid;
}



//-----------------------------------------------------------------------------
// addLeaf
//
// - adds leaf to leaf list, if leaf is a item object
//
// returns:
//		TRUE  - added
//		FALSE - not added
//
BOOL GenericGroup::addLeaf(
	IN GenericLeaf *newLeaf, 
	IN OPTIONAL int index)
{
	if (!newLeaf)
		return FALSE;

	if (newLeaf->is(GENOBJ_TYPE_ITEM))
		return GenericBranch::addLeaf(newLeaf, index);
	else
		return FALSE;
} // addLeaf


//-----------------------------------------------------------------------------
// addBranch
//
// - don't add branch into group object
//
// returns:
//		FALSE - not added
//
BOOL GenericGroup::addBranch(
	IN GenericBranch *newBranch, 
	IN OPTIONAL int index)
{
	return FALSE;
} // addBranch


//-----------------------------------------------------------------------------
// setActive
//
// - set activation state
//
// returns:
//		TRUE
//
BOOL GenericGroup::setActive(
	IN BOOL active)	// activation state
{
	_TRACE(TL_DEB, TG_GRP, (_T("> setActive:%s"), BOOL2STR(active)));
	BOOL prevActive = m_active;

	if (active)
		active = TRUE;	// set 'NOT FALSE(!=0)' to 'TRUE(1)'

	onSetActive(active);

	// check if to activate update rate mechanism
	if ((active) && (!prevActive))
	{ // group was activated 
		initSpan(0);	// -> start update rate mechanism
	}

	m_active = active;
	_TRACE(TL_INF, TG_GRP, (_T("set active to %s"), BOOL2STR(active)));

	// check if to activate update rate mechanism
	if ((active) && (!prevActive))
	{ // group was activated 
		::SetEvent(((GenericServer *)m_parent)->getStartEvent());	// signal async thread
	}

	_TRACE(TL_DEB, TG_GRP, (_T("< setActive")));
	return TRUE;
} // setActive


//-----------------------------------------------------------------------------
// setUpdateRate
//
// - set update rate
//
// returns:
//		TRUE
//
BOOL GenericGroup::setUpdateRate(
	IN DWORD updateRate)	// update rate
{
	GenericPointer<OPCEngine> engine = ::getOPCEngine();
	DWORD reqUR = updateRate;
    
    if(updateRate==0)     //HACK d-2108 avoid 100%CPU load if requested is 0
    {                     //HACK d-2108 avoid 100%CPU load if requested is 0
        updateRate = 100; //HACK d-2108 avoid 100%CPU load if requested is 0
    }                     //HACK d-2108 avoid 100%CPU load if requested is 0

	if (updateRate < engine->getMinUpdateRate()) 
	{
		updateRate = engine->getMinUpdateRate();			
	}
	if ((updateRate < 10) && (m_updateMode == poll))
	{
		updateRate = 10;			// min poll update rate 10 ms
	}
	if (updateRate > 3456000000)
	{
		updateRate = 3456000000;	// max update rate 40 days
	}
	_TRACE(TL_INF, TG_GRP, (_T("set update rate to %lu; requested %lu"), updateRate, reqUR));
	onSetUpdateRate(updateRate);
	m_updateRate = updateRate;

	::SetEvent(((GenericServer *)m_parent)->getStartEvent());	// signal async thread

	return TRUE;
} // setUpdateRate


//-----------------------------------------------------------------------------
// setClientHandle
//
// - set client handle
//
// returns:
//		TRUE
//
BOOL GenericGroup::setClientHandle(
	IN OPCHANDLE clientHandle)	// client handle
{
	_TRACE(TL_INF, TG_GRP, (_T("set client handle to 0x%X"), clientHandle));
	onSetClientHandle(clientHandle);
	m_clientHandle = clientHandle;
	return TRUE;
} // setClientHandle


//-----------------------------------------------------------------------------
// setTimeBias
//
// - set time zone
//
// returns:
//		TRUE
//
BOOL GenericGroup::setTimeBias(
	IN LONG timeBias)	// time zone
{
	_TRACE(TL_INF, TG_GRP, (_T("set time bias to %li"), timeBias));
	onSetTimeBias(timeBias);
	m_timeBias = timeBias;
	return TRUE;
} // setTimeBias


//-----------------------------------------------------------------------------
// setDeadBand
//
// - set deadband
//
// returns:
//		TRUE
//
BOOL GenericGroup::setDeadBand(
	IN FLOAT deadBand)	// deadband
{
	_TRACE(TL_INF, TG_GRP, (_T("set deadband to %f"), deadBand));
	onSetDeadBand(deadBand);
	m_deadBand = deadBand;
	return TRUE;
} // setDeadBand


//-----------------------------------------------------------------------------
// setLCID
//
// - set local Id
//
// returns:
//		TRUE
//
BOOL GenericGroup::setLCID(
	IN DWORD lcid)	// local Id
{
	_TRACE(TL_INF, TG_GRP, (_T("set locale id to 0x%X"), lcid));
	onSetLCID(lcid);
	m_lcid = lcid;
	return TRUE;
} // setLCID


//-----------------------------------------------------------------------------
// setEnable
//
// - set enable
//
// returns:
//		TRUE
//
BOOL GenericGroup::setEnable(
	IN BOOL enable)	// enable
{
	_TRACE(TL_INF, TG_GRP, (_T("set enable to %s"), BOOL2STR(enable)));
	onSetEnable(enable);
	m_enable = enable;
	return TRUE;
} // setEnable


//-----------------------------------------------------------------------------
// onClone
//
// - called after having cloned group
// - default implementation: do nothing
//
void GenericGroup::onClone(
	IN GenericGroup *clone)	// new group 
{} // onClone


//-----------------------------------------------------------------------------
// onMoveToPublic
//
// - called after moved group to public
// - default implementation: do nothing
//
void GenericGroup::onMoveToPublic(void)
{} // onMoveToPublic


//-----------------------------------------------------------------------------
// onAdvise
//
// - called after installed advise to client
// - default implementation: do nothing
//
void GenericGroup::onAdvise(
	IN CLIPFORMAT format,	// data format 
	IN DWORD connection)	// connection id
{} // onAdvise


//-----------------------------------------------------------------------------
// onUnadvise
//
// - called after canceling advise from client
// - default implementation: do nothing
//
void GenericGroup::onUnadvise(
	IN DWORD connection)	// connection id
{} // onUnadvise


//-----------------------------------------------------------------------------
// onAdvise2
//
// - called after installed advise to client
// - default implementation: do nothing
//
void GenericGroup::onAdvise2(
	IN DWORD cookie)	// connection id
{} // onAdvise2


//-----------------------------------------------------------------------------
// onUnadvise2
//
// - called after canceling advise from client
// - default implementation: do nothing
//
void GenericGroup::onUnadvise2(
	IN DWORD cookie)	// connection id
{} // onUnadvise2


//-----------------------------------------------------------------------------
// onUpdate
//
// - called after updating client advise
// - default implementation: do nothing
//
void GenericGroup::onUpdateCyclicTransaction(void)
{} // onUpdate


//-----------------------------------------------------------------------------
// onSetActive
//
// - called after activation state is set
// - default implementation: do nothing
//
void GenericGroup::onSetActive(
	IN BOOL active)	// activation state
{} // onSetActive


//-----------------------------------------------------------------------------
// onSetUpdateRate
//
// - called after update rate is set
// - default implementation: do nothing
//
void GenericGroup::onSetUpdateRate(
	IN DWORD updateRate)	// update rate
{} // onSetUpdateRate


//-----------------------------------------------------------------------------
// onSetClientHandle
//
// - called after client handle is set
// - default implementation: do nothing
//
void GenericGroup::onSetClientHandle(
	IN OPCHANDLE clientHandle)	// client handle
{} // onSetClientHandle


//-----------------------------------------------------------------------------
// onSetTimeBias
//
// - called after time zone is set
// - default implementation: do nothing
//
void GenericGroup::onSetTimeBias(
	IN LONG timeBias)	// time zone
{} // onSetTimeBias


//-----------------------------------------------------------------------------
// onSetDeadBand
//
// - called after deadband is set
// - default implementation: do nothing
//
void GenericGroup::onSetDeadBand(
	IN FLOAT percentDeadBand)	// deadband
{} // onSetDeadBand


//-----------------------------------------------------------------------------
// onSetLCID
//
// - called after local id is set
// - default implementation: do nothing
//
void GenericGroup::onSetLCID(
	IN DWORD lcid)
{} // onSetLCID


//-----------------------------------------------------------------------------
// onSetEnable
//
// - called before enable is set
// - default implementation: do nothing
//
void GenericGroup::onSetEnable(
	IN BOOL enable)
{} // onSetEnable

void GenericGroup::initSpan(IN DWORD toLate)
{
	_TRACE(TL_DEB, TG_GRP, (_T("> initSpan toLate:%lu"), toLate));
	// correct bad NT Timer (~11 ms interval)
	m_initCnt += ((m_updateRate * 150) >> 10);
	if (m_initCnt < 1000)
		m_correctNTtimer = 0;
	else
	{
		m_correctNTtimer = 10 * (m_initCnt >> 10);
		m_initCnt = m_initCnt & 0x3ff;
	}

	toLate += m_correctNTtimer;

	m_initTime = ::GetTickCount();
	if (toLate < m_updateRate)
		m_span = m_updateRate - toLate;
	else
		m_span = m_updateRate;

	_TRACE(TL_DEB, TG_GRP, (_T("< initSpan span:%lu"), m_span));
}


//-----------------------------------------------------------------------------
// updateSpan
//
// - update time span till next request start
// - start request if time ellapsed
//
// returns:
//		updated time span till next request
//
BOOL GenericGroup::updateSpan(IN DWORD now, IN DWORD *span)
{
	_TRACE(TL_DEB, TG_GRP, (_T("> updateSpan")));
	if (!m_active)
	{
		if (span)
			*span = INFINITE;
		_TRACE(TL_DEB, TG_GRP, (_T("< updateSpan; not active")));
		return FALSE;
	}

	// check for immediate update rate
	if (m_updateRate < 10)
	{
		if (span)
			*span = INFINITE;
	
		_TRACE(TL_DEB, TG_GRP, (_T("< updateSpan; immediate")));
		return TRUE;
	}

	DWORD ellapsed = getTimeSpan(m_initTime, now);
	DWORD updateRate = m_updateRate - m_correctNTtimer;
	BOOL fire = FALSE; 

	if (ellapsed < updateRate)
		m_span = updateRate - ellapsed;
	else	// span ellapsed
	{
		DWORD toLate = ellapsed - updateRate;
		initSpan(toLate);
		onUpdateCyclicTransaction();
		fire = TRUE;
	}

	if (span)
		*span = m_span;

	_TRACE(TL_DEB, TG_GRP, (_T("< updateSpan span:%lu ellapsed:%s"), m_span, BOOL2STR(fire)));
	return fire;
} // updateSpan


BOOL GenericGroup::isRemoved(void)
{ 
	// object marked as removed
	if (m_removed)
		return TRUE;

	if (isPublicGroup())
	{
		// public group check if template of group is removed
		return m_publicTemplate->isRemoved();
	}

	return FALSE;
}

OPCHANDLE GenericGroup::getServerHandle(void) const
{ 
	if (!isPublicGroup())
	{
		return m_serverHandle; 
	}
	else
	{
		return m_publicTemplate->m_serverHandle;
	}
}


void GenericGroup::onDataChange(IN OPCTransaction *transaction)
{}
void GenericGroup::onReadComplete(IN OPCTransaction *transaction)
{}
void GenericGroup::onWriteComplete(IN OPCTransaction *transaction)
{}

//-----------------------------------------------------------------------------
// accessToMethod
//
// - allow the user to call the OPC function of the interface
//
// returns:
//		TRUE  - allow access
//		FALSE - deny access
//
BOOL GenericGroup::accessToMethod(
	IN REFIID iid,				// interface id of called interface
	IN enum methodOPCGroup method,	// method called
	IN LPCTSTR user)			// user name
{ return TRUE; } // accessToMethod



OPCAdvise::OPCAdvise()
{
	m_format = 0;;
	m_connection = 0;
	m_adviseSink = NULL;
}

OPCAdvise::OPCAdvise(const OPCAdvise& src)
{
	m_format = src.m_format;
	m_connection = src.m_connection;
	m_adviseSink = src.m_adviseSink;
}

OPCAdvise::~OPCAdvise()
{
}

template<> void AFXAPI ConstructElements<OPCAdvise> (OPCAdvise* elements, int count)
{
	for (; count--; ++elements)
		new(elements) OPCAdvise;
}

template<> void AFXAPI DestructElements<OPCAdvise> (OPCAdvise* elements, int count)
{
	for (; count--; ++elements)
	{
		if (elements->m_adviseSink)
		{
			elements->m_adviseSink->Release();
			elements->m_adviseSink = NULL;
		}
		elements->~OPCAdvise();
	}
}

template<> void AFXAPI CopyElements<OPCAdvise> (OPCAdvise* dest, const OPCAdvise* src, int count)
{
	for (; count--; ++dest, ++src)
		*dest = *src;
}


