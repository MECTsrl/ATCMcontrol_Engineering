//-----------------------------------------------------------------------------
//                                  OPC ENGINE                                |
//                                                                            |
//  Filename    : GENNS.H                                                     |   
//  Version     : 2.00.0.00.release                                           |
//  Date        : 29-March-1999                                               |
//  Author      : ATCM                                                 |
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
//                                                                            |
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "GenNS.h"
#include "GenCache.h"
#include "OPCTrace.h"
#include "opcengine.h"
#include "GenItem.h"
#include "GenSrv.h"
#include <opcerror.h>


GenericItemTag::GenericItemTag(
	IN DWORD objType)	// generic object type
 : GenericLeaf(GENOBJ_TYPE_ITEM_TAG | objType)
{
	m_accessPaths = NULL;
	m_accessRights = 0;
	m_nativeDatatype = VT_EMPTY;
	m_cache = NULL;
	m_deviceIOMode = 0;
	m_dynamic = FALSE;
	m_queueData = NULL;
}

GenericItemTag::~GenericItemTag(void)
{
	setCache(NULL);
	if (m_accessPaths)
	{
		m_accessPaths->RemoveAll();
		delete m_accessPaths;
	}
	if (m_queueData)
		delete m_queueData;
}


void GenericItemTag::createQueueData(void)
{ 
	if (!m_queueData)
	{
		m_queueData = new GenericItemTagQueueData; 
		m_queueData->m_readQueue = NULL;
		m_queueData->m_writeQueue = NULL;
		m_queueData->m_readEvent = INVALID_HANDLE_VALUE;
		m_queueData->m_writeEvent = INVALID_HANDLE_VALUE;
#ifdef _AFXDLL
		m_queueData->m_readQueueLock = NULL;
		m_queueData->m_writeQueueLock = NULL;
#else
		m_queueData->m_readQueueLock = INVALID_HANDLE_VALUE;
		m_queueData->m_writeQueueLock = INVALID_HANDLE_VALUE;
#endif
	}
}

HRESULT GenericItemTag::setName(
	IN LPCTSTR name)
{
	HRESULT res;
	res = GenericElement::setName(name);
	if ((SUCCEEDED(res)) && (m_cache))
		m_cache->m_objectTraceId.Empty();
	return res;
}

//-----------------------------------------------------------------------------
// getCache
//
// - create and initialize cache object if NULL
// - get cache object
//
// returns:
//		pointer to cache object 
//
GenericCache *GenericItemTag::getCache(void) 
{ 
	if (!m_cache)
	{
		GenericPointer<OPCEngine> engine = ::getOPCEngine();
		GenericPointer<GenericCreator> creator = engine->getCreator();
		creator->lock();
		if (!m_cache)
		{
			m_cache = creator->createCache(this);

			if (m_cache)
			{
				m_cache->setDatatype(m_nativeDatatype);
				m_cache->init();
			}
		}
		creator->unlock();
	}

	if (m_cache)
		m_cache->addRef(); 

	return m_cache; 
} // getCache


//-----------------------------------------------------------------------------
// setCache
//
// - set cache object
// - release previous cache
//
// returns:
//		TRUE
//
BOOL GenericItemTag::setCache(
	IN GenericCache *cache)	// cache object
{
	if (m_cache)
		m_cache->release();

	if (cache)
		cache->addRef();

	m_cache	= cache;
	return TRUE;
} // setCache


//-----------------------------------------------------------------------------
// setAccessPaths
//
// - set access paths
//
// returns:
//		TRUE
//
BOOL GenericItemTag::setAccessPaths(CStringList *accessPaths)
{
	POSITION pos;
	CString apath;	

	onSetAccessPaths(accessPaths);
	if (m_accessPaths)
		m_accessPaths->RemoveAll();
	else
		m_accessPaths = new CStringList(1);

	pos = accessPaths->GetHeadPosition();
	while(pos)
	{
		apath = (LPCTSTR)accessPaths->GetNext(pos);
		m_accessPaths->AddTail(apath);
	}
	return TRUE;
} // setAccessPaths


//-----------------------------------------------------------------------------
// getAccessPaths
//
// - get access paths
//
void GenericItemTag::getAccessPaths(CStringList *accessPaths) const
{
	if (m_accessPaths)
	{
		POSITION pos;
		CString apath;	

		pos = m_accessPaths->GetHeadPosition();
		while(pos)
		{
			apath = m_accessPaths->GetNext(pos);
			accessPaths->AddTail(apath);
		}
	}
} // getAccessPaths


//-----------------------------------------------------------------------------
// checkAccessPath
//
// - checks if access path is OK
//
// returns:
//		TRUE  - access path is OK
//		FALSE - access path not OK
//
BOOL GenericItemTag::checkAccessPath(
	IN CString accessPath)	// access path
{
	_TRACE(TL_DEB, TG_NS, (_T("> checkAccessPath %s"), accessPath));
	if (accessPath.IsEmpty())
		return TRUE;

	if (m_accessPaths)
	{
		POSITION pos;
		CString apath;	

		pos = m_accessPaths->GetHeadPosition();
		while(pos)
		{
			apath = m_accessPaths->GetNext(pos);
			if (apath == accessPath)
			{
				_TRACE(TL_DEB, TG_NS, (_T("supported access path %s"), accessPath));
				return TRUE;
			}
		}
	}
	_TRACE(TL_DEB, TG_NS, (_T("bad access path %s"), accessPath));
	return FALSE;
} // checkAccessPath


//-----------------------------------------------------------------------------
// setNativeDatatype
//
// - sets native datatype
//
// returns:
//		TRUE
//
BOOL GenericItemTag::setNativeDatatype(
	IN VARTYPE datatype)	// new native datatype
{ 
	_TRACE(TL_INF, TG_NS, (_T("set datatype %u"), datatype));
	if (!supportDatatype(datatype))
		return FALSE;

	onSetNativeDatatype(datatype);
	m_nativeDatatype = datatype; 

	if (m_cache)
		m_cache->setDatatype(m_nativeDatatype);

	return TRUE; 
} // setNativeDatatype


BOOL GenericItemTag::setAccessRights(IN DWORD rights)
{ 
	onSetAccessRights(rights);
	m_accessRights = rights; 
	return TRUE; 
}


BOOL GenericItemTag::supportDatatype(
	IN VARTYPE dataType)	// data type
{
	switch (dataType)
	{
		case VT_EMPTY:
		case VT_UI1:
		case VT_I2:
		case VT_I4:
		case VT_R4:
		case VT_R8:
		case VT_BOOL:
		case VT_BSTR:
		case VT_DATE:
		case VT_DECIMAL:
		case (VT_ARRAY | VT_UI1):
		case (VT_ARRAY | VT_I2):
		case (VT_ARRAY | VT_I4):
		case (VT_ARRAY | VT_R4):
		case (VT_ARRAY | VT_R8):
		case (VT_ARRAY | VT_BOOL):
		case (VT_ARRAY | VT_BSTR):
		case (VT_ARRAY | VT_DATE):
		case (VT_ARRAY | VT_DECIMAL):
			return TRUE;
		default:
			return FALSE;
	}
}

//-----------------------------------------------------------------------------
// supportReqDatatype
//
// - checks if the requested data type is supported
//
// returns:
//		S_OK          - support data type
//		OPC_E_BADTYPE - not support data type
//
HRESULT GenericItemTag::supportReqDatatype(
	IN VARTYPE reqDataType)	// requested data type
{
	_TRACE(TL_DEB, TG_NS, (_T("> supportDatatype %u"), reqDataType));
	HRESULT res;

	// supported datatype ?
	if (!supportDatatype(reqDataType))
	{ 
		return OPC_E_BADTYPE;
	}

	// simple checks
	if ((reqDataType == VT_EMPTY) || (reqDataType == (VT_ARRAY | VT_EMPTY)))
		return S_OK;
	if (m_nativeDatatype == reqDataType)
		return S_OK;

	// try type conversion
	res = OPC_E_BADTYPE;
	switch (m_nativeDatatype)
	{
		case VT_UI1:
		case VT_I2:
		case VT_I4:
		case VT_R4:
		case VT_R8:
		case VT_DECIMAL:
		case VT_BOOL:
		{
			switch (reqDataType)
			{
				case VT_UI1:
				case VT_I2:
				case VT_I4:
				case VT_R4:
				case VT_R8:
				case VT_DECIMAL:
				case VT_BOOL:
				case VT_BSTR:
					res = S_OK;
				break;
			}
		}
		break;
		case VT_DATE:
		{
			if (reqDataType == VT_BSTR)
				res = S_OK;
		}
		break;
	}

	_TRACE(TL_DEB, TG_NS, (_T("< supportReqDatatype %u -> %u (0x%X)"), m_nativeDatatype, reqDataType, res));
	return res;
} // supportReqDatatype

BOOL GenericItemTag::setDynamic(
	IN BOOL dynamic)
{ 
	m_dynamic = dynamic; 
	return TRUE; 
}


BOOL GenericItemTag::signalEvent(
	IN BOOL readEvent)
{
	_TRACE(TL_DEB, TG_ITEM, (_T("> signalEvent read:%s"), BOOL2STR(readEvent)));
	if (m_queueData)
	{
		if (readEvent)
		{
			if (m_queueData->m_readEvent)
			{
				::SetEvent(m_queueData->m_readEvent);
				_TRACE(TL_DEB, TG_ITEM, (_T("< signalEvent 0x%X"), m_queueData->m_readEvent));
				return TRUE;
			}
		}
		else
		{
			if (m_queueData->m_writeEvent)
			{
				::SetEvent(m_queueData->m_writeEvent);
				_TRACE(TL_DEB, TG_ITEM, (_T("< signalEvent 0x%X"), m_queueData->m_writeEvent));
				return TRUE;
			}
		}
	}

	_TRACE(TL_DEB, TG_ITEM, (_T("< signalEvent; no event set")));
	return FALSE;
}

BOOL GenericItemTag::addToQueue(
	IN GenericRequest *req, 
	IN BOOL readQueue)
{
	_TRACE(TL_DEB, TG_ITEM, (_T("> addToQueue %s read:%s"), req->getObjTraceId(), BOOL2STR(readQueue)));
	RequestList *queue = NULL;
#ifdef _AFXDLL
	CSyncObject *sync = NULL;
#else
	HANDLE mutex = INVALID_HANDLE_VALUE;
#endif

	if (m_queueData)
	{
		if (readQueue)
		{
			queue = m_queueData->m_readQueue;
#ifdef _AFXDLL
			sync = m_queueData->m_readQueueLock;
#else
			mutex = m_queueData->m_readQueueLock;
#endif
		}	
		else
		{
			queue = m_queueData->m_writeQueue;
#ifdef _AFXDLL
			sync = m_queueData->m_writeQueueLock;
#else
			mutex = m_queueData->m_writeQueueLock;
#endif
		}
	}	

#ifdef _AFXDLL
	if ((queue) && (sync))
#else
	if ((queue) && (mutex != INVALID_HANDLE_VALUE))
#endif
	{
#ifdef _AFXDLL
		sync->Lock();
#else
		::WaitForSingleObject(mutex, INFINITE);
#endif
		if (queue->Find(req))
		{
#ifdef _AFXDLL
			sync->Unlock();
#else
			::ReleaseMutex(mutex);
#endif
			_TRACE(TL_DEB, TG_ITEM, (_T("< addToQueue; already in queue 0x%X"), queue));
			return TRUE;
		}

		req->addRef();					// queue entry is new reference to object
		addToRequestList(queue, req);	// add to queue
#ifdef _AFXDLL
		sync->Unlock();
#else
		::ReleaseMutex(mutex);
#endif
		_TRACE(TL_DEB, TG_ITEM, (_T("< addToQueue; added to queue 0x%X"), queue));
		return TRUE;
	}

	_TRACE(TL_DEB, TG_ITEM, (_T("< addToQueue; no queue set")));
	return FALSE;
}

void GenericItemTag::addToRequestList(
	IN RequestList *queue,
	IN GenericRequest *req)
{
	queue->AddTail(req);
}

void GenericItemTag::handleRequest(
	IN GenericRequest *request)
{
	_ERROR(TG_NS, OPCENGINE_ERR_ASSERT, (_T("GenericItemTag::handleRequest must be implemented in child class for direct I/O")));
	GenericPointer<GenericCache> cache = getCache();
	cache->setQuality(OPC_QUALITY_BAD);
	request->complete(E_FAIL);
}

void GenericItemTag::onSetAccessRights(IN DWORD rights)
{}
void GenericItemTag::onSetNativeDatatype(IN VARTYPE datatype)
{}
void GenericItemTag::onSetAccessPaths(IN CStringList *accessPaths)
{}

BYTE GenericItemTag::getDeviceIOMode(void)
{
	if (m_deviceIOMode)
		return m_deviceIOMode;
	else
		return (GenericRequest::requestDeviceIOMode)GenericRequest::direct;
}

BOOL GenericItemTag::setConstantValue(IN LPVARIANT value)
{
	GenericPointer<GenericCache> cache = getCache();
	cache->setValue(value);
	cache->setQuality(OPC_QUALITY_GOOD);
	setDeviceIOMode(GenericRequest::constant);
	return TRUE;
}

DWORD GenericItemTag::notifyAllItems(void)
{
	GenericPointer<OPCEngine> engine = ::getOPCEngine();
	GenericPointer<OPCRootNS> nsRoot = engine->getNamespaceRoot();
	GenericPointer<GenericCache> cache = getCache();
	ItemList itemList;
	GenericItem *item;
	POSITION pos;
	DWORD itemCnt = 0;

	getAllItems(itemList);
	if (!itemList.IsEmpty())
	{
		itemCnt += itemList.GetCount();
		notifyItems(itemList);
		pos = itemList.GetHeadPosition();
		while(pos)
		{
			item = itemList.GetNext(pos);
			item->release();		
		}
		itemList.RemoveAll();
	}

	if (nsRoot->getUseTagLinks())
	{
		ItemTagList itemTagList;
		GenericItemTag *itemTag;
		POSITION itPos;

		cache->getAllItemTags(itemTagList);
		if (!itemTagList.IsEmpty())
		{
			itPos = itemTagList.GetHeadPosition();
			while(itPos)
			{
				itemTag = itemTagList.GetNext(itPos);

				if (itemTag != this)
				{				
					itemTag->getAllItems(itemList);
					if (!itemList.IsEmpty())
					{
						itemCnt += itemList.GetCount();
						notifyItems(itemList);
						pos = itemList.GetHeadPosition();
						while(pos)
						{
							item = itemList.GetNext(pos);
							item->release();		
						}
						itemList.RemoveAll();
					}
				}
				itemTag->release();
			}
			itemTagList.RemoveAll();
		}
	}

	return itemCnt;
}

void GenericItemTag::notifyItems(
	IN ItemList &itemList)
{
	GenericPointer<OPCEngine> engine = ::getOPCEngine();
	GenericPointer<GenericCreator> creator = engine->getCreator();
	GenericItem *item;
	POSITION pos;
	GenericPointer<GenericGroup> group;
	GenericPointer<GenericServer> srv;
	GenericPointer<GenericRequest> req;
	GenericPointer<OPCTransaction> tac;
	GenericPointer<GenericServer> signalServer;
	BOOL addRequest;

	pos = itemList.GetHeadPosition();
	while(pos)
	{
		item = itemList.GetNext(pos);

		group = (GenericGroup *)item->getParentBranch();

		if ((GenericGroup *)group)
		{
			if ((group->getUpdateMode() == GenericGroup::report) &&
				(item->isActive()))
			{
				srv = (GenericServer *)group->getParentBranch();
				if ((GenericServer *)srv)
				{
					if (!srv->isPublicGroups())
					{ // not the public groups server
						addRequest = TRUE;
						{
						POSITION posReq;
						GenericList<GenericRequest> reqList;
						GenericRequest *curReq;
						GenericPointer<GenericItem> curItem;

						tac = group->getCyclicTransaction();
						reqList = tac->getRequestList();
						posReq = reqList.getStartPosition();
						while (posReq)
						{
							curReq = reqList.getNext(posReq);
							curItem = curReq->getItem();
							if ((GenericItem *)curItem == item)
							{
								addRequest = FALSE;
								break;
							}
						}
						}
						if (addRequest)
						{
							if ((!!signalServer) && ((GenericServer *)srv != (GenericServer *)signalServer))
							{
								::SetEvent(signalServer->getStartEvent());	// signal async thread
								signalServer.release();
							}

							req = creator->createRequest(srv, item, this, GenericRequest::read, GenericRequest::report, getDeviceIOMode());
							req->setItem(item);
							tac->insertRequest(req);
							if (group->getUpdateRate() < 10)
							{
								signalServer = srv;
								signalServer.addRef();
							}	
							req.release();
						}
					}
				}
			}
		}
	}	

	if (!!signalServer)
		::SetEvent(signalServer->getStartEvent());	// signal async thread
}


void GenericItemTag::getAllItems(
	OUT ItemList &items)
{
	GenericPointer<OPCEngine> engine = ::getOPCEngine();
	GenericPointer<OPCRootOPC> opcRoot = engine->getOPCRoot();
	GenericList<GenericBranch> serverList(opcRoot->getBranchList());
	POSITION posSrv;
	GenericServer *server;	
	GenericList<GenericBranch> groupList;
	POSITION posGrp;
	GenericGroup *group;	
	GenericList<GenericLeaf> itemList;
	POSITION posItm;
	GenericItem *item;	
	GenericPointer<GenericItemTag> itemTag;

	posSrv = serverList.getStartPosition();
	while(posSrv)
	{
		server = (GenericServer *)serverList.getNext(posSrv);
		
		groupList = server->getBranchList();
		posGrp = groupList.getStartPosition();
		while(posGrp)
		{
			group = (GenericGroup *)groupList.getNext(posGrp);

			if ((group->getUpdateMode() == GenericGroup::report) &&
				(group->getActive()))
			{
				itemList = group->getLeafList();
				posItm = itemList.getStartPosition();
				while(posItm)
				{
					item = (GenericItem *)itemList.getNext(posItm);
					if (item->getActive())
					{
						itemTag = item->getItemTag();
						if ((GenericItemTag *)itemTag == this)
						{
							item->addRef();
							items.AddTail(item);
						}
					}
				}
				itemList.unlock();			
			}
		}
		groupList.unlock();
	}
}



//-----------------------------------------------------------------------------
// GenericTag                                                                 |
//-----------------------------------------------------------------------------

GenericTag::GenericTag(void) 
 : GenericItemTag(GENOBJ_TYPE_TAG)
{
	m_EUInfo = NULL;
}

GenericTag::~GenericTag(void)
{
	m_propertyList.free();

	if (m_EUInfo)
	{
		clearEU();
		delete m_EUInfo;
	}
}

LONG GenericTag::release(void)
{
	if (m_dynamic)
	{
		GenericPointer<GenericBranch> parent = getParentBranch();
		GenericList<GenericTagProperty> propList(getPropertyList());
		GenericTagProperty *prop;
		POSITION pos;
		int nRefs = 0; 

		if (!!parent)
		{
			if (parent->getLeafList()->getList())
				nRefs++;
			if (parent->getLeafList()->getMap())
				nRefs++;
		}

		pos = propList.getStartPosition();
		while (pos)
		{
			prop = propList.getNext(pos);
			if (prop->getRefCount() == 1)
				nRefs ++;
		}
		propList.unlock();

		if ((getRefCount() - 1) == nRefs)
		{
			if (!(!parent))
			{
				parent->lockLeafList();
				if ((getRefCount() - 1) == nRefs)
				{
					_TRACE(TL_DEB, TG_NS, (_T("remove dynamic tag %s"), getObjTraceId()));
					m_dynamic = FALSE;	// prevent recursion
					removeAllProperties();
					parent->removeLeaf(this, FALSE);
				}
				parent->unlockLeafList();
			}
		}
	}
	return GenericObject::release();		
}

BOOL GenericTag::setParentBranch(
	IN GenericBranch *parent)
{
	HRESULT res;
	res = GenericElement::setParentBranch(parent);
	return res;
}


GenericTagProperty *GenericTag::addProperty(
	IN DWORD propertyId)
{
	GenericPointer<GenericTagProperty> prop;

	if (!m_propertyList)
	{
		m_propertyList.create();
	}

	prop = m_propertyList.find(propertyId);
	if (!prop)
	{
		GenericPointer<OPCEngine> engine = ::getOPCEngine();
		GenericPointer<GenericCreator> creator = engine->getCreator();
		prop = creator->createTagProperty(this, propertyId);
		m_propertyList.add(prop);
		prop.addRef();
	}

	m_propertyList.unlock();
	return (GenericTagProperty *)prop;
}


BOOL GenericTag::getEUInfo(OUT LPVARIANT info) 
{
	if (!m_EUInfo)
	{
		::VariantInit(info);	// return VT_EMPTY
		return TRUE;
	}
	else
	{
		::VariantInit(info); 
		return (::VariantCopy(info, (LPVARIANT)m_EUInfo->m_info) == S_OK); 
	}
}


void GenericTag::clearEU(void)
{ 
	if (m_EUInfo)
	{
		if ((m_EUInfo->m_type == OPC_ANALOG) && (!(!m_propertyList)))
		{
			m_propertyList.remove(102);
			m_propertyList.remove(103);
		}

		m_EUInfo->m_type = OPC_NOENUM;
		m_EUInfo->m_info.Clear();
	}
}


BOOL GenericTag::setEUAnalog(
	IN double low, 
	IN double high)
{ 
	GenericPointer<GenericTagProperty> tagProp;
	COleVariant value;
	SAFEARRAYBOUND dim; 
	LONG idx;

	dim.lLbound = 0; 
	dim.cElements = 2; 

	if (m_EUInfo)
		clearEU();
	else
		m_EUInfo = new OPCEUInformation;

	m_EUInfo->m_type = OPC_ANALOG;

	m_EUInfo->m_info.vt = (VT_ARRAY | VT_R8);
	m_EUInfo->m_info.parray = ::SafeArrayCreate(VT_R8, 1, &dim);
	idx = 0;
	::SafeArrayPutElement(m_EUInfo->m_info.parray, &idx, &low);
	idx = 1;
	::SafeArrayPutElement(m_EUInfo->m_info.parray, &idx, &high);

	tagProp = addProperty(102);
	value = high;
	tagProp->setConstantValue(value);
	tagProp = addProperty(103);
	value = low;
	tagProp->setConstantValue(value);
	return TRUE; 
}

BOOL GenericTag::setEUEnumerated(
	IN CStringList *enumList,
	IN OPTIONAL LONG startIndex)
{
	SAFEARRAYBOUND dim; 
	POSITION pos;
	LONG idx;
	BSTR bstr;
	CString enumStr;
	USES_CONVERSION;

	if (!enumList)
		return FALSE;

	dim.lLbound = startIndex; 
	dim.cElements = enumList->GetCount(); 

	if (m_EUInfo)
		clearEU();
	else
		m_EUInfo = new OPCEUInformation;

	m_EUInfo->m_type = OPC_ENUMERATED;

	m_EUInfo->m_info.vt = (VT_ARRAY | VT_BSTR);
	m_EUInfo->m_info.parray = ::SafeArrayCreate(VT_BSTR, 1, &dim);

	pos = enumList->GetHeadPosition();

	idx = dim.lLbound;
	while(pos)
	{
		enumStr = enumList->GetNext(pos);
	    bstr = ::SysAllocString(T2COLE((LPCTSTR)enumStr)); 
		::SafeArrayPutElement(m_EUInfo->m_info.parray, &idx, bstr);
		::SysFreeString(bstr);
		idx++;
	}

	return TRUE; 
}

BYTE GenericTag::getDeviceIOMode(void)
{
	if (!m_deviceIOMode)
		return (GenericRequest::requestDeviceIOMode)GenericRequest::queue;
	else
		return m_deviceIOMode;
}

void GenericTag::removeAllProperties(void)
{
	GenericList<GenericTagProperty> propList;
	addRef();
	propList = getPropertyList();
	propList.clear();
	release();
}


//-----------------------------------------------------------------------------
// GenericTagProperty                                                         |
//-----------------------------------------------------------------------------

GenericTagProperty::GenericTagProperty(IN GenericTag *tag, IN DWORD propertyId) 
 : GenericItemTag(GENOBJ_TYPE_TAG_PROPERTY)
{
	BOOL propHand = FALSE;

	m_tag = tag;
	m_tag->addRef();
	m_propertyId = propertyId;
	m_accessRights = OPC_READABLE;

	// ID set 1 - OPC specific properties
	if ((m_propertyId >= 1) && (m_propertyId <= 6))
	{
		switch (m_propertyId)
		{
			case 1:	setName(_T("Item Canonical DataType"));	setNativeDatatype(VT_I2); break;
			case 2:	setName(_T("Item Value")); setNativeDatatype(m_tag->getNativeDatatype()); break;
			case 3:	setName(_T("Item Quality")); setNativeDatatype(VT_I2); break;
			case 4:	setName(_T("Item Timestamp")); setNativeDatatype(VT_DATE); break;
			case 5:	setName(_T("Item Access Rights")); setNativeDatatype(VT_I4); break;
			case 6:	setName(_T("Server Scan Rate")); setNativeDatatype(VT_R4); break;
		}
		propHand = TRUE;
	}

	// ID set 2 - recommended properties
	if ((m_propertyId >= 100) && (m_propertyId <= 108))
	{
		switch (m_propertyId)
		{
			case 100: setName(_T("EU Units")); setNativeDatatype(VT_BSTR); break;
			case 101: setName(_T("Item Description")); setNativeDatatype(VT_BSTR); break;
			case 102: setName(_T("High EU")); setNativeDatatype(VT_R8);	break;
			case 103: setName(_T("Low EU")); setNativeDatatype(VT_R8); break;
			case 104: setName(_T("High Instrument Range")); setNativeDatatype(VT_R8); break;
			case 105: setName(_T("Low Instrument Range")); setNativeDatatype(VT_R8); break;
			case 106: setName(_T("Contact Close Label")); setNativeDatatype(VT_BSTR); break;
			case 107: setName(_T("Contact Open Label")); setNativeDatatype(VT_BSTR); break;
			case 108: setName(_T("Item Timezone")); setNativeDatatype(VT_I4); break;
		}
		propHand = TRUE;
	}

	// ID set 3 - operator display related properties
	if ((m_propertyId >= 200) && (m_propertyId <= 207))
	{
		switch (m_propertyId)
		{
			case 200: setName(_T("Default Display")); setNativeDatatype(VT_BSTR); break;
			case 201: setName(_T("Current Foreground Color")); setNativeDatatype(VT_I4); break;
			case 202: setName(_T("Current Background Color")); setNativeDatatype(VT_I4);	break;
			case 203: setName(_T("Current Blink")); setNativeDatatype(VT_BOOL); break;
			case 204: setName(_T("BMP File")); setNativeDatatype(VT_BSTR); break;
			case 205: setName(_T("Sound File")); setNativeDatatype(VT_BSTR); break;
			case 206: setName(_T("HTML File")); setNativeDatatype(VT_BSTR); break;
			case 207: setName(_T("AVI File")); setNativeDatatype(VT_BSTR); break;
		}
		propHand = TRUE;
	}

	if (!propHand)
	{
		CString name;
		name.Format(_T("Property %lu"), m_propertyId);
		setName(name); 
	}
}

GenericTagProperty::~GenericTagProperty(void)
{
	if (m_tag) m_tag->release();
}

LONG GenericTagProperty::release(void)
{
	if ((getRefCount() == 2) && (m_dynamic) && (m_tag))
	{
		GenericList<GenericTagProperty> propList = m_tag->getPropertyList();
		GenericTag *tag = m_tag;
		tag->addRef();
		propList.lock();
		if (getRefCount() == 2)
		{
			_TRACE(TL_DEB, TG_NS, (_T("remove dynamic tag property %lu"), m_propertyId));
			propList.remove(m_propertyId);
		}
		propList.unlock();
		tag->release();
	}

	return (GenericObject::release());		
}

BOOL GenericTagProperty::hasKey(
	IN LPCTSTR key)
{
	DWORD propId = _tcstoul(key, NULL, 0);
	return (propId == m_propertyId);
}

CString GenericTagProperty::getKey(void)
{ 
	CString id;
	id.Format(_T("%lu"), m_propertyId);
	return id; 
}

BOOL GenericTagProperty::handleStandardProperties(
	IN GenericRequest *request)
{
	if ((m_propertyId == 1) || (m_propertyId == 5) || (m_propertyId == 6) ||
		(m_propertyId == 108))
	{
		GenericPointer<GenericCache> cache = getCache();
		COleVariant value;

		switch (m_propertyId)
		{
			case 1:	// native datatype
				value = (SHORT)m_tag->getNativeDatatype();
				cache->setValue(value);
			break;
			case 5:	// access rights
				value = (LONG)m_tag->getAccessRights();
				cache->setValue(value);
			break;
			case 6:	// server scan rate
			{
				GenericPointer<OPCEngine> engine = ::getOPCEngine();
				value = (float)engine->getMinUpdateRate();
				cache->setValue(value);
			}
			break;
			case 108: // item timezone
			{
				TIME_ZONE_INFORMATION timeZone;
				DWORD timeZoneRet;
				LONG timeBias;
				// set system default time zone
				timeZoneRet = GetTimeZoneInformation(&timeZone);

				if (timeZoneRet == TIME_ZONE_ID_DAYLIGHT)
					timeBias = timeZone.Bias + timeZone.StandardBias + timeZone.DaylightBias;
				else
					timeBias = timeZone.Bias + timeZone.StandardBias;

				value = (LONG)timeBias;
				cache->setValue(value);
			}
			break;
		}
		cache->setQuality(OPC_QUALITY_GOOD);
		request->complete();
		return TRUE;
	}

	return FALSE;
}

void GenericTagProperty::handleRequest(
	IN GenericRequest *request)
{
	if (!handleStandardProperties(request))
	{
		_ERROR(TG_NS, OPCENGINE_ERR_ASSERT, (_T("GenericTagProperty::handleRequest must be implemented in child class for not standard properties")));
		GenericPointer<GenericCache> cache = getCache();
		cache->setQuality(OPC_QUALITY_BAD);
		request->complete(E_FAIL);
	}
}

BYTE GenericTagProperty::getDeviceIOMode(void)
{
	if (!m_deviceIOMode)
	{
		if ((m_propertyId >= 2) && (m_propertyId <= 4))
			return (GenericRequest::requestDeviceIOMode)GenericRequest::delegate;
		else
			return (GenericRequest::requestDeviceIOMode)GenericRequest::direct;
	}
	else
		return m_deviceIOMode;
}

void GenericTagProperty::getPropertyData(
	IN OPCTagPropertyData *pD)
{ 
	if (pD) 
	{ 
		pD->m_datatype = getNativeDatatype(); 
		pD->m_descr = _tcsdup((LPCTSTR)getName()); 
		pD->m_pid = m_propertyId; 
	} 
}


//-----------------------------------------------------------------------------
// GenericNode                                                                |
//-----------------------------------------------------------------------------

GenericNode::GenericNode(
	IN OPTIONAL DWORD supportedChildObj,	// supported child object types
	IN OPTIONAL BYTE whatTagList,			// generic list type of tag list
	IN OPTIONAL BYTE whatNodeList,			// generic list type of node list
	IN OPTIONAL int blockSizeTagList,		// block size of tag list and map
	IN OPTIONAL int blockSizeNodeList,		// block size of node list and map
	IN OPTIONAL int hashSizeTagList,		// hash table size of tag map
	IN OPTIONAL int hashSizeNodeList)		// hash table size of node map
 : GenericBranch(GENOBJ_TYPE_NODE, supportedChildObj, whatTagList, whatNodeList, blockSizeTagList, blockSizeNodeList, hashSizeTagList, hashSizeNodeList)
{
}


//-----------------------------------------------------------------------------
// addLeaf
//
// - adds leaf to leaf list, if leaf is a tag
//
// returns:
//		TRUE  - unique name
//		FALSE - no unique name
//
BOOL GenericNode::addLeaf(
	IN GenericLeaf *newLeaf, 
	IN OPTIONAL int index)
{
	if (!newLeaf)
		return FALSE;

	if (newLeaf->is(GENOBJ_TYPE_TAG))
		return GenericBranch::addLeaf(newLeaf, index);
	else
		return FALSE;
} // addLeaf


//-----------------------------------------------------------------------------
// addBranch
//
// - adds branch to branch list, if branch is a node
//
// returns:
//		TRUE  - unique name
//		FALSE - no unique name
//
BOOL GenericNode::addBranch(
	IN GenericBranch *newBranch, 
	IN OPTIONAL int index)
{
	if (!newBranch)
		return FALSE;

	if (newBranch->is(GENOBJ_TYPE_NODE))
		return GenericBranch::addBranch(newBranch, index);
	else
		return FALSE;
} // addBranch


void GenericNode::removeAllProperties(void)
{
	POSITION pos;

	_TRACE(TL_INF, TG_NS, (_T("remove all properties")));

	GenericList<GenericLeaf> lList(getLeafList());
	if (!(!lList))
	{
		GenericTag *leaf;

		lList.lock();
		pos = lList.getStartPosition();
		while(pos)
		{
			leaf = (GenericTag *)lList.getNext(pos);
			leaf->removeAllProperties();
		}
		lList.unlock();
	}

	GenericList<GenericBranch> bList(getBranchList());
	if (!(!bList))
	{
		GenericNode *branch;

		bList.lock();
		pos = bList.getStartPosition();
		while(pos)
		{
			branch = (GenericNode *)bList.getNext(pos);
			branch->removeAllProperties();
		}
		bList.unlock();
	}	
	_TRACE(TL_DEB, TG_NS, (_T("< removeAllProperties")));
} // removeAllProperties



//-----------------------------------------------------------------------------
// OPCRootNS                                                                  |
//-----------------------------------------------------------------------------

OPCRootNS::OPCRootNS(void)
 : GenericBranch(GENOBJ_TYPE_NS_ROOT, GENOBJ_TYPE_BRANCH | GENOBJ_TYPE_LEAF, GENLIST_CREATE_LIST | GENLIST_CREATE_MAP, GENLIST_CREATE_LIST | GENLIST_CREATE_MAP)
{
	m_useTagLinks = FALSE;
	m_objectTraceId.Format(_T("[%s|%8.8X| %s]"), getObjTypeString(), this, _T("NS root"));
}


//-----------------------------------------------------------------------------
// addLeaf
//
// - adds leaf to leaf list, if leaf is a tag
//
// returns:
//		TRUE  - unique name
//		FALSE - no unique name
//
BOOL OPCRootNS::addLeaf(
	IN GenericLeaf *newLeaf, 
	IN OPTIONAL int index)
{
	if (!newLeaf)
		return FALSE;

	if (newLeaf->is(GENOBJ_TYPE_TAG))
		return GenericBranch::addLeaf(newLeaf, index);
	else
		return FALSE;
} // addLeaf


//-----------------------------------------------------------------------------
// addBranch
//
// - adds branch to branch list, if branch is a node
//
// returns:
//		TRUE  - unique name
//		FALSE - no unique name
//
BOOL OPCRootNS::addBranch(
	IN GenericBranch *newBranch, 
	IN OPTIONAL int index)
{
	if (!newBranch)
		return FALSE;

	if (newBranch->is(GENOBJ_TYPE_NODE))
		return GenericBranch::addBranch(newBranch, index);
	else
		return FALSE;
} // addBranch


//-----------------------------------------------------------------------------
// removeAllProperties
//
// - remove all tag properties of all tags of the namespace tree
//
void OPCRootNS::removeAllProperties(void)
{
	POSITION pos;

	_TRACE(TL_INF, TG_NS, (_T("remove all properties")));

	GenericList<GenericLeaf> lList(getLeafList());
	if (!(!lList))
	{
		GenericTag *leaf;

		lList.lock();
		pos = lList;
		while(pos)
		{
			leaf = (GenericTag *)lList.getNext(pos);
			leaf->removeAllProperties();
		}
		lList.unlock();
	}

	GenericList<GenericBranch> bList(getBranchList());
	if (!(!bList))
	{
		GenericNode *branch;

		bList.lock();
		pos = bList;
		while(pos)
		{
			branch = (GenericNode *)bList.getNext(pos);
			branch->removeAllProperties();
		}
		bList.unlock();
	}	

	_TRACE(TL_DEB, TG_TREE, (_T("< removeAllProperties")));
} // removeAllProperties
