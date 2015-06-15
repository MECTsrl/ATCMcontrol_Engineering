//-----------------------------------------------------------------------------
//                                  OPC ENGINE                                |
//                                                                            |
//  Filename    : OPCTRANS.CPP                                                |
//  Version     : 2.00.0.00.release                                           |
//  Date        : 29-March-1999                                               |
//                                                                            |
//  Description : Classes for asynchonous OPC transactions                    |
//                - OPCTransaction: OPC transaction                           |
//                                                                            |
//  CHANGE_NOTES                                                              |
//                                                                            |
//  date      name      change                                                |
//  ------------------------------------------------------------------------- |
//  12/23/98  RT        support OPC V2.0 connections                          |
//                                                                            |
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "GenReq.h"
#include "GenItem.h"
#include "GenNS.h"
#include "GenCache.h"
#include "OPCTrans.h"
#include "OPCTrace.h"
#include "globals.h"
#include "GenSrv.h"
#include "OPCEngine.h"
#include "OPCGroup.h"

OPCTransaction::OPCTransaction(
	IN OPCTransaction::transactionType type,
	IN GenericGroup *group,
	IN BYTE connectionType, 
	IN OPTIONAL DWORD connection)
 : GenericObject(GENOBJ_TYPE_TRANSACTION)
{
	m_group = group;
	m_objectTraceId.Empty();
	m_type = type;
	m_connection = connection;
	m_started = FALSE;
	m_connectionType = connectionType; 
	_TRACE(TL_DEB, TG_TAC, (_T("> constructor group %s con 0x%X type %d"), group->getObjTraceId(), connection, type));

	m_clientTransId = 0;
	if ((type == cyclic) || (type == report))
		m_transactionId = 0;
	else		
	{
		m_group->addRef();
		m_transactionId = (DWORD)this;
	}
	m_reqList.create();
	_TRACE(TL_DEB, TG_TAC, (_T("< constructor")));
}


OPCTransaction::~OPCTransaction(void)
{
	_TRACE(TL_DEB, TG_TAC, (_T("> destructor")));
	m_reqList.free();

	if ((m_type != cyclic) && (m_type != report))
		m_group->release();
	_TRACE(TL_DEB, TG_TAC, (_T("< destructor")));
}

//-----------------------------------------------------------------------------
// getObjTraceId
//
// - get objects trace string id
//
// returns:
//		trace id string
//
LPCTSTR OPCTransaction::getObjTraceId(void)
{ 
	if (m_objectTraceId.IsEmpty())
	{
		m_objectTraceId.Format(_T("{%s|%8.8X| %s(%X)}"), getObjTypeString(), this, m_group->getName(), m_connection);
		m_objectTraceId.FreeExtra();
	}

	return m_objectTraceId; 
} // getObjTraceId


BOOL OPCTransaction::insertRequest(
	IN GenericRequest *req)
{
	GenericList<GenericRequest> reqList(getRequestList());
	return reqList.add(req);
}

BOOL OPCTransaction::removeRequest(
	IN GenericRequest *req)
{
	GenericList<GenericRequest> reqList(getRequestList());
	req->addRef();
	if (reqList.remove(req))
		req->cancel(S_OK);

	req->release();
	return TRUE;
}

GenericRequest *OPCTransaction::getRequestByItem(
	IN GenericItem *srcItem)
{
	GenericList<GenericRequest> reqList(getRequestList());
	GenericPointer<GenericItem> item;
	GenericRequest *req;
	POSITION pos;

	pos = reqList.getStartPosition();
	while (pos)
	{
		req = reqList.getNext(pos);
		item = req->getItem();
		if (((GenericItem *)item) && (srcItem == (GenericItem *)item))
		{
			req->addRef();
			_TRACE(TL_DEB, TG_TAC, (_T("request %s found for item %s"), req->getObjTraceId(), srcItem->getObjTraceId()));
			return req;
		}
	}

	_TRACE(TL_INF, TG_TAC, (_T("request for item %s not found"), srcItem->getObjTraceId()));
	return NULL;
}

void OPCTransaction::startRequests(void)
{
	_TRACE(TL_DEB, TG_TAC, (_T("> startRequests")));
	GenericList<GenericRequest> reqList(getRequestList());
	POSITION posReq;
	GenericRequest *req;

	if (m_started == TRUE)
	{ // transaction not completed
		posReq = reqList.getStartPosition();
		while (posReq)
		{
			req = reqList.getNext(posReq);
			if (!((req->isCompleted()) || (!req->isStarted())))
			{ // previous cyclic request running
			  // -> cancel previous try, forget the new one
				req->cancel(E_FAIL, TRUE, OPC_QUALITY_LAST_USABLE);
				req->m_flag |= GENREQ_FLAG_CANCELED;
			}
		}
		complete();
	}

	posReq = reqList.getStartPosition();
	while (posReq)
	{
		req = reqList.getNext(posReq);
		if ((req->m_flag & GENREQ_FLAG_CANCELED) == 0)
		{
			if (isCyclic() && req->isCompleted())
			{
				req->reset();
			}

			if (SUCCEEDED(req->getResult()))
			{
				req->start();
			}
			else
			{
				// error before start (bad type, bad rights, ...)
				req->complete(req->getResult(), FALSE, OPC_QUALITY_GOOD, TRUE);
			}
		}
		else
		{
			// the canceled request will be added the next time again
			req->m_flag &= ~GENREQ_FLAG_CANCELED;
		}
	}

	m_started = TRUE;
	_TRACE(TL_DEB, TG_TAC, (_T("< startRequests")));
}

void OPCTransaction::cancelRequests(void)
{
	_TRACE(TL_DEB, TG_TAC, (_T("> cancelRequests")));
	GenericList<GenericRequest> reqList(getRequestList());
	POSITION posReq;
	GenericRequest *req;

	posReq = reqList.getStartPosition();
	while (posReq)
	{
		req = reqList.getNext(posReq);
		req->cancel();
	}
	m_started = FALSE;
	_TRACE(TL_DEB, TG_TAC, (_T("< cancelRequests")));
}

GenericGroup *OPCTransaction::getGroup(void)
{ 
	m_group->addRef(); 
	return m_group; 
}

BOOL OPCTransaction::completeReportRequests(void)
{
	if (m_type == report)
	{
		GenericList<GenericRequest> reqList(getRequestList());
	
		if (!reqList.isEmpty())
		{
			_TRACE(TL_DEB, TG_TAC, (_T("completed report %d requests"), reqList.getCount()));
			m_started = TRUE;
		}

		return TRUE;
	}

	return FALSE;
}


BOOL OPCTransaction::update(
	IN DWORD now, 
	IN DWORD dueTime,
	IN OUT DWORD *span)
{
	BOOL fire = FALSE;
	DWORD abortSpan = INFINITE;
	_TRACE(TL_DEB, TG_TAC, (_T("> update")));

	if (m_started)
	{
		if (m_type != report)
		{
			GenericList<GenericRequest> reqList(getRequestList());
			POSITION posReq;
			GenericRequest *req;
			int compCnt = 0;
			DWORD reqTime;

			// count the completed requests
			posReq = reqList.getStartPosition();
			while (posReq)
			{
				req = reqList.getNext(posReq);
				if (req->isStarted())
				{ // request is started
					if (req->isCompleted())
					{ // request is completed
						compCnt++;
						_TRACE(TL_DEB, TG_TAC, (_T("request completed %s"), req->getObjTraceId()));
					}
					else
					{
						if (dueTime != INFINITE)	
						{
							// check max completion time
							reqTime = getTimeSpan(req->getStartTime(), now);

							_TRACE(TL_DEB, TG_TAC, (_T("%s reqTime %lu (%lu, %lu)"), req->getObjTraceId(), reqTime, req->getStartTime(), now));
							if (reqTime < dueTime)
							{
								// calculate span to max completion time
								DWORD reqSpan = dueTime - reqTime;
								if (abortSpan > reqSpan)
								{
									abortSpan = reqSpan;
									break;
								}
							}
							else
							{ // max completion time ellapsed
							  // -> abort request
								req->cancel(E_FAIL, TRUE);
								compCnt++;
							}
						}
					}
				} 
			}

			_TRACE(TL_DEB, TG_TAC, (_T("update %d requests completed"), reqList.getCount()));

			if (compCnt == reqList.getCount())
			{ // all requests completed
				// complete transaction
				complete();		
				fire = TRUE;
			}
		}
		else
		{
			// report transaction
			// in a started report transaction all requests are completed
			complete();		
			fire = TRUE;
		}
	}

	if (span)
		*span = abortSpan;

	_TRACE(TL_DEB, TG_TAC, (_T("< update %s"), BOOL2STR(fire)));
	return fire;
}



void OPCTransaction::complete(void)
{
	_TRACE(TL_DEB, TG_TAC, (_T("> complete")));
	DWORD connection;

	m_started = FALSE;

	if ((m_connectionType == allConnections) || (m_connectionType == connectionPoint))
	{
		m_group->m_meSync.Lock();
		OPCObject<OPCGroup> *grp = (OPCObject<OPCGroup> *)m_group->m_me;
		if (grp)
			grp->internalAddRef();
		m_group->m_meSync.Unlock();

		if (!grp)
			return;

		CSingleLock lock(grp->getAdvisesLockObj(), TRUE);
		CComDynamicUnkArray *advises = grp->getAdvises();
		IUnknown** pp;
		IOPCDataCallback *dataCallback;

		pp = advises->begin();
		while (pp < advises->end())
		{
			if (*pp != NULL)
			{
				dataCallback = (IOPCDataCallback *)*pp;
				if (m_type == write)
				{
					sendWriteData(dataCallback);
				}		
				else
				{
					if (((m_type != cyclic) && (m_type != report)) || (m_group->getEnable()))
						sendReadData(dataCallback);
				}		
			}
			pp++;
		}

		lock.Unlock();
		grp->internalRelease();
	}

	if ((m_connectionType == allConnections) || (m_connectionType == dataObject))
	{
		GenericPointer<GenericServer> srv = (GenericServer *)m_group->getParentBranch();
		AdviseMap* adviseMap;
		OPCAdvise advise;
		STGMEDIUM medium;
		FORMATETC formatetc;
		POSITION pos;

		formatetc.dwAspect = DVASPECT_CONTENT;
		formatetc.ptd = NULL;
		formatetc.tymed = TYMED_HGLOBAL;
		formatetc.lindex = -1;

		CSingleLock lock(m_group->getAdvisesLockObj(), TRUE);
		adviseMap = m_group->getAdvises();
		
		pos = adviseMap->GetStartPosition();
		while (pos)
		{
			adviseMap->GetNextAssoc(pos, connection, advise);

			if ((m_type == cyclic) || (m_type == report) || (m_connection == connection))
			{
				// possible advise
				if (m_type == write)
				{
					if (advise.m_format == OPCSTMFORMATWRITECOMPLETE) 
					{
						// get the data
						if (S_OK == createWriteData(&medium.hGlobal))
						{
							medium.tymed = TYMED_HGLOBAL;
							medium.pUnkForRelease = NULL;
							formatetc.cfFormat = advise.m_format;
			
							_TRACE(TL_INF, TG_TAC, (_T("send write data to connection 0x%X"), connection));
							// send advise
							advise.m_adviseSink->OnDataChange(&formatetc, &medium);
							m_group->onWriteComplete(this);
							::ReleaseStgMedium(&medium);
						}
					}
				}
				else
				{ // read, refresh and cyclic
					if ((advise.m_format == OPCSTMFORMATDATA) ||
						(advise.m_format == OPCSTMFORMATDATATIME))
					{ // only the right advises
						// get the data
						
						if (S_OK == createReadData(advise.m_format, connection, 
										&medium.hGlobal))
						{
							medium.tymed = TYMED_HGLOBAL;
							medium.pUnkForRelease = NULL;
							formatetc.cfFormat = advise.m_format;

							_TRACE(TL_INF, TG_TAC, (_T("send read data to connection 0x%X (%s)"), connection, CLIPFORMAT2STR(advise.m_format)));
							// send advise
							advise.m_adviseSink->OnDataChange(&formatetc, &medium);
							::ReleaseStgMedium(&medium);

							// new update time
							if (m_type != read)
							{
								::CoFileTimeNow(&srv->m_lastUpdateTime);					
								m_group->onDataChange(this);
							}
							else
								m_group->onReadComplete(this);
						}
					}
				}
			}
		}
	}
					
	_TRACE(TL_DEB, TG_TAC, (_T("< complete")));
}


DWORD OPCTransaction::getRequestCount(
	IN OPTIONAL BOOL doLock,
	IN OPTIONAL BOOL onlyChanged, 
	IN OPTIONAL DWORD connection)
{
	GenericList<GenericRequest> reqList(getRequestList());
	DWORD count = 0;

	if (doLock) 
		reqList.lock();

	if (((m_type == cyclic) || (m_type == report)) && (onlyChanged))
	{ 
		GenericPointer<GenericItem> item;
		GenericPointer<GenericCache> cache;
		POSITION pos;
		GenericRequest *req;
		WORD quality;
		VARIANT value;

		pos = reqList.getStartPosition();
		while (pos)
		{
			req = reqList.getNext(pos);
			item = req->getItem();
			cache = req->getCache();
			quality = cache->getQuality(req);
			cache->getValue(req, item->getReqDatatype(), &value);
			if (req->hasChanged(connection, &value, quality))
				count++;
			::VariantClear(&value);
		}
	}
	else
		count = reqList.getCount();

	if (doLock) 
		reqList.unlock();
	
	_TRACE(TL_DEB, TG_TAC, (_T("request count:%lu onlyChanged:%s connection:0x%X"), count, BOOL2STR(onlyChanged), connection));
	return count;
}


HRESULT OPCTransaction::createReadData(
	IN CLIPFORMAT format,
	IN DWORD connection, 
	OUT HANDLE *memory)
{
	GenericPointer<GenericItem> item;
	GenericPointer<GenericCache> cache;
	GenericPointer<GenericItemTag> tag;
	GenericRequest *req;
	OPCGROUPHEADER *groupHeader;
	OPCITEMHEADER1 *itemHeader;
	BYTE *pHeader;
	BYTE *pVariants;
	BYTE *pActHeader;
	BYTE *pActVariants;
	DWORD actHeaderOff;
	DWORD actVariantsOff;
	DWORD headerSize;
	DWORD variantsSize;
	DWORD idx;
	DWORD itemHeaderSize;
	VARIANT value;
	VARIANT *pValue;
	DWORD valueSize;
	POSITION pos;
	BOOL isBSTR;
	BOOL isARRAY;
	BYTE *pMemory;
	HRESULT res;	
	USES_CONVERSION;
	WORD quality;
	GenericList<GenericRequest> reqList(getRequestList());

	if (format == OPCSTMFORMATDATA)
	{
		_TRACE(TL_DEB, TG_TAC, (_T("send data")));
		itemHeaderSize = sizeof(OPCITEMHEADER2);
	}
	else
	{
		_TRACE(TL_DEB, TG_TAC, (_T("send data and time")));
		itemHeaderSize = sizeof(OPCITEMHEADER1);
	}

	// allocate memory
	pHeader = (BYTE *)malloc(1024);
	pVariants = (BYTE *)malloc(1024);
	if ((pHeader == NULL) ||(pVariants == NULL))
		AfxThrowMemoryException();
	headerSize = 1024;
	variantsSize = 1024;

	pActHeader = pHeader;
	pActVariants = pVariants;
	actVariantsOff = 0;

	// group header
	groupHeader = (OPCGROUPHEADER *)pActHeader;
	groupHeader->dwSize = 0;
	groupHeader->hClientGroup = m_group->getClientHandle();
	groupHeader->dwTransactionID = m_transactionId;
	groupHeader->hrStatus = S_OK;
	groupHeader->dwItemCount = 0;
	pActHeader += sizeof(OPCGROUPHEADER);
	actHeaderOff = sizeof(OPCGROUPHEADER);

	// item header and value
	pos = reqList.getStartPosition();
	idx = 0;
	while (pos)
	{
		req = reqList.getNext(pos);
		req->setObjectState(req->getObjectState() | GENREQ_FLAG_PROCESSED);

		item = req->getItem();
		cache = req->getCache();
		quality = cache->getQuality(req);
		cache->getValue(req, item->getReqDatatype(), &value);
		if (req->hasChanged(connection, &value, quality))
		{
			tag = req->getItemTag();

			//-- item header --
			if ((actHeaderOff + itemHeaderSize) > headerSize)
			{ // realloc header
				headerSize += 1024;
				pHeader = (BYTE *)realloc(pHeader, headerSize);
				if (pHeader == NULL)
					return E_OUTOFMEMORY;
				pActHeader = pHeader + actHeaderOff;				
				groupHeader = (OPCGROUPHEADER *)pHeader;
			}

			// set item header
			itemHeader = (OPCITEMHEADER1 *)pActHeader;
			itemHeader->hClient = item->getClientHandle();
			itemHeader->dwValueOffset = actVariantsOff;
			itemHeader->wQuality = quality;
			itemHeader->wReserved = 0;
			if (format == OPCSTMFORMATDATATIME)
				cache->getTimeStamp(req, &itemHeader->ftTimeStampItem);

			// cache request on inactive item -> quality is out of service
			if ((req->isCacheRequest()) && (!(item->getActive() && m_group->getActive())))
					itemHeader->wQuality = OPC_QUALITY_OUT_OF_SERVICE;

			// if no good quality -> change group header status
			if ((itemHeader->wQuality & OPC_QUALITY_GOOD) != OPC_QUALITY_GOOD)
				groupHeader->hrStatus = S_FALSE;


			//-- item value --
			valueSize = cache->getValueSize(&value);

			if ((actVariantsOff + valueSize) > variantsSize)
			{ // realloc header
				variantsSize += (valueSize < 1024) ? 1024 : valueSize;
				pVariants = (BYTE *)realloc(pVariants, variantsSize);
				if (pVariants == NULL)
					return E_OUTOFMEMORY;
				pActVariants = pVariants + actVariantsOff;				
			}

			pValue = (VARIANT *) (pActVariants);
			memcpy(pValue, &value, sizeof(VARIANT));

			isBSTR = ((item->getReqDatatype() == VT_BSTR) || 
				      ( (item->getReqDatatype() == VT_EMPTY) && 
                        (tag->getNativeDatatype() == VT_BSTR)));
			isARRAY = (((item->getReqDatatype() & VT_ARRAY) == VT_ARRAY) || 
					   ( (item->getReqDatatype() == VT_EMPTY) && 
						 ((tag->getNativeDatatype() & VT_ARRAY) == VT_ARRAY)));
			
			if (isBSTR)
			{ // string
				DWORD *pBstrCount = (DWORD *)(pValue + 1);

				*pBstrCount = valueSize - sizeof(VARIANT) - 6;
				pValue->bstrVal = (BSTR)(pBstrCount + 1);
				wcscpy(pValue->bstrVal, value.bstrVal);
				pValue->bstrVal = NULL;
			}

			if (isARRAY)
			{ 
				SAFEARRAY *pArray = (SAFEARRAY *)(pValue + 1);
				BYTE *data = (BYTE *)(pArray + 1);
				ULONG dataSize = (value.parray->cbElements * value.parray->rgsabound[0].cElements);
				void *saData;

				memcpy(pArray, value.parray, sizeof(SAFEARRAY));
				pArray->pvData = NULL;

				SafeArrayAccessData(value.parray, &saData);
				memcpy(data, saData, dataSize);
				if (value.vt == (VT_BSTR | VT_ARRAY))
				{
					BSTR *bstr = (BSTR *)saData;
					BYTE *pActBArray = data + dataSize; 
					ULONG i;
					DWORD *pBstrCount;
					BSTR pBstr;

					for (i = 0; i < value.parray->rgsabound[0].cElements; i++)
					{
						pBstrCount = (DWORD *)(pActBArray);
						pBstr = (BSTR)(pBstrCount + 1);
		
						*pBstrCount = (_tcslen(OLE2CT(bstr[i])) * 2);
						wcscpy(pBstr, bstr[i]);
						pActBArray += *pBstrCount + 6;
					}
				}
				SafeArrayUnaccessData(value.parray);
			}

			_TRACE(TL_DEB, TG_TAC, (_T("createReadData item 0x%X value:%s quality:0x%X"), itemHeader->hClient, VARIANT2STR(&value), itemHeader->wQuality));	
			req->setSendValue(connection, &value, itemHeader->wQuality);

			idx++;
			pActHeader += itemHeaderSize;
			pActVariants += valueSize;
			actHeaderOff = (pActHeader - pHeader);
			actVariantsOff = (pActVariants - pVariants);
		}
		::VariantClear(&value);
	}

	// set item and size
	groupHeader->dwItemCount = idx;
	groupHeader->dwSize = actHeaderOff + actVariantsOff;
	// correct value offsets
	pActHeader = (BYTE *)(groupHeader + 1);
	for (idx = 0; idx < groupHeader->dwItemCount; idx++)
	{
		itemHeader = (OPCITEMHEADER1 *)pActHeader;
		itemHeader->dwValueOffset += actHeaderOff;	
		pActHeader += itemHeaderSize;		
	}

	_TRACE(TL_INF, TG_TAC, (_T("createReadData items:%lu size:%lu result: 0x%X"), groupHeader->dwItemCount, groupHeader->dwSize, groupHeader->hrStatus));

	if (idx > 0)
	{
		// copy data to HGLOBAL
		*memory = ::GlobalAlloc(GMEM_MOVEABLE, groupHeader->dwSize);
		if (*memory == NULL)
			return E_OUTOFMEMORY;
		pMemory = (BYTE *)::GlobalLock(*memory);
		memcpy(pMemory, pHeader, actHeaderOff);
		memcpy(pMemory + actHeaderOff, pVariants, actVariantsOff);
		::GlobalUnlock(*memory);
		res = S_OK;
	}
	else
		res = S_FALSE;

	free(pHeader);
	free(pVariants);

	return res;
} // createReadData


HRESULT OPCTransaction::createWriteData(
	OUT HANDLE *memory)
{
	GenericRequest *req;
	GenericPointer<GenericItem> item;
	DWORD nrReqs = getRequestCount();
	DWORD size;
	OPCGROUPHEADERWRITE *groupHeader;
	OPCITEMHEADERWRITE *itemHeader;
	BYTE *actPtr;
	POSITION pos;
	GenericList<GenericRequest> reqList(getRequestList());

	// allocate memory
	size = sizeof(OPCGROUPHEADERWRITE) + (nrReqs * (sizeof(OPCITEMHEADERWRITE)));
	*memory = ::GlobalAlloc(GMEM_MOVEABLE, size);
	if (*memory == NULL)
		return E_OUTOFMEMORY;

	actPtr = (BYTE *)::GlobalLock(*memory);

	// group header
	groupHeader = (OPCGROUPHEADERWRITE *)actPtr;
	groupHeader->hClientGroup = m_group->getClientHandle();
	groupHeader->dwTransactionID = m_transactionId;
	groupHeader->hrStatus = S_OK;
	groupHeader->dwItemCount = nrReqs;
	actPtr += sizeof(OPCGROUPHEADERWRITE);

	_TRACE(TL_DEB, TG_TAC, (_T("send write transaction:0x%X items:%lu hClientGroup 0x%X"), 
			groupHeader->dwTransactionID, groupHeader->dwItemCount, groupHeader->hClientGroup));

	// item header
	pos = reqList.getStartPosition();
	while (pos)
	{
		req = reqList.getNext(pos);
		req->setObjectState(req->getObjectState() | GENREQ_FLAG_PROCESSED);
		item = req->getItem();

		itemHeader = (OPCITEMHEADERWRITE *)actPtr;
		itemHeader->hClient = item->getClientHandle();
		itemHeader->dwError = req->getResult();
		_TRACE(TL_DEB, TG_TAC, (_T("send item 0x%X result:0x%X"), itemHeader->hClient, itemHeader->dwError));

		actPtr += sizeof(OPCITEMHEADERWRITE);
	}

	// unlock memory
	::GlobalUnlock(*memory);

	return S_OK;
}


HRESULT OPCTransaction::sendReadData(
	IN IOPCDataCallback *dataCallback)
{
	HRESULT res;
	DWORD groupClientHandle = m_group->getClientHandle();	
	DWORD clientTransId = getClientTransactionId();
	HRESULT masterQuality = S_OK;
	HRESULT masterError = S_OK;
	DWORD itemCount = 0;
	OPCHANDLE *itemClientHandles = NULL;
	VARIANT *itemValues = NULL;
	WORD *itemQualities = NULL;
	FILETIME *itemTimeStamps = NULL;
	HRESULT *itemErrors = NULL;
	DWORD allocatedBufferIdx = 0;
	GenericPointer<GenericItem> item;
	GenericPointer<GenericCache> cache;
	GenericRequest *req;
	POSITION pos;
	CComPtr<IMalloc> malloc;
	DWORD i;
	GenericList<GenericRequest> reqList(getRequestList());
	WORD quality;
	VARIANT value;

	_TRACE(TL_DEB, TG_TAC, (_T("send read data")));
	::CoGetMalloc(MEMCTX_TASK, &malloc);

	// item attributes and values
	pos = reqList.getStartPosition();
	while (pos)
	{
		req = reqList.getNext(pos);

		req->setObjectState(req->getObjectState() | GENREQ_FLAG_PROCESSED);

		item = req->getItem();
		cache = req->getCache();
		quality = cache->getQuality(req);
		cache->getValue(req, item->getReqDatatype(), &value);
		if (req->hasChanged((DWORD)dataCallback, &value, quality))
		{

			// (re)allocate the array
			if (itemCount >= allocatedBufferIdx)
			{
				allocatedBufferIdx += 50;

				itemValues = (VARIANT *)malloc->Realloc(itemValues, sizeof(VARIANT) * allocatedBufferIdx);
				if (itemValues)
				{
					for (i = itemCount; i < allocatedBufferIdx; i++)
						::VariantInit(&itemValues[i]);
				}

				itemClientHandles = (OPCHANDLE *)malloc->Realloc(itemClientHandles, sizeof(OPCHANDLE) * allocatedBufferIdx);
				itemQualities = (WORD *)malloc->Realloc(itemQualities, sizeof(WORD) * allocatedBufferIdx);
				itemTimeStamps = (FILETIME *)malloc->Realloc(itemTimeStamps, sizeof(FILETIME) * allocatedBufferIdx);
				itemErrors = (HRESULT *)malloc->Realloc(itemErrors, sizeof(HRESULT) * allocatedBufferIdx);

				if ((!itemClientHandles) || (!itemQualities) || (!itemTimeStamps) ||
					(!itemErrors) || (!itemValues))
				{
					if (itemClientHandles) malloc->Free(itemClientHandles);
					if (itemQualities) malloc->Free(itemQualities);
					if (itemTimeStamps) malloc->Free(itemTimeStamps);
					if (itemErrors) malloc->Free(itemErrors);
					if (itemValues) 
					{
						for (i = 0; i < itemCount; i++)
							::VariantClear(&itemValues[i]);
						malloc->Free(itemValues);
					}
					return E_OUTOFMEMORY;
				}
			}

			// copy the data
			itemClientHandles[itemCount] = item->getClientHandle();
			itemQualities[itemCount] = quality;
			cache->getTimeStamp(req, &itemTimeStamps[itemCount]);
//			cache->getValue(req, item->getReqDatatype(), &itemValues[itemCount]);
			::VariantCopy(&itemValues[itemCount], &value);
			itemErrors[itemCount] = req->getResult();

			// cache request on inactive item -> quality is out of service
			if ((req->isCacheRequest()) && (!(item->getActive() && m_group->getActive())))
				itemQualities[itemCount] = OPC_QUALITY_OUT_OF_SERVICE;

			// if no good quality -> change master quality
			if ((itemQualities[itemCount] & OPC_QUALITY_GOOD) != OPC_QUALITY_GOOD)
				masterQuality = S_FALSE;

			// if not S_OK as error -> change master error
			if (itemErrors[itemCount] != S_OK)
				masterError = S_FALSE;

			// set the last send value of the connection for last hasChanged check
			_TRACE(TL_DEB, TG_TAC, (_T("sendDataChange item 0x%X value:%s quality:0x%X"), itemClientHandles[itemCount], VARIANT2STR(&itemValues[itemCount]), itemQualities[itemCount]));	
			req->setSendValue((DWORD)dataCallback, &itemValues[itemCount], itemQualities[itemCount]);
			::VariantClear(&value);
			itemCount++;
		}
		::VariantClear(&value);
	}

	// send the data to the client
	if (itemCount > 0)
	{
		if (m_type == read)
		{
			_TRACE(TL_INF, TG_TAC, (_T("send read complete to callback 0x%X"), dataCallback));
			res = dataCallback->OnReadComplete(m_transactionId, groupClientHandle, masterQuality, masterError,
					itemCount, itemClientHandles, itemValues, itemQualities, itemTimeStamps, itemErrors);
			m_group->onReadComplete(this);
		}
		else
		{
			_TRACE(TL_INF, TG_TAC, (_T("send data change to callback 0x%X"), dataCallback));
			res = dataCallback->OnDataChange(clientTransId, groupClientHandle, masterQuality, masterError,
					itemCount, itemClientHandles, itemValues, itemQualities, itemTimeStamps, itemErrors);

			GenericPointer<GenericServer> srv = (GenericServer *)m_group->getParentBranch();
			::CoFileTimeNow(&srv->m_lastUpdateTime);					
			m_group->onDataChange(this);
		}
	}
	else
	{
		_TRACE(TL_INF, TG_TAC, (_T("no data changed")));
	}

	// free the allocated memory
	if (itemClientHandles) malloc->Free(itemClientHandles);
	if (itemQualities) malloc->Free(itemQualities);
	if (itemTimeStamps) malloc->Free(itemTimeStamps);
	if (itemErrors) malloc->Free(itemErrors);
	if (itemValues) 
	{
		for (i = 0; i < itemCount; i++)
			::VariantClear(&itemValues[i]);
		malloc->Free(itemValues);
	}

	return res;
} // sendReadData


HRESULT OPCTransaction::sendWriteData(
	IN IOPCDataCallback *dataCallback)
{
	GenericRequest *req;
	GenericPointer<GenericItem> item;
	DWORD groupClientHandle = m_group->getClientHandle();	
	HRESULT masterError = S_OK;
	DWORD itemCount = 0;
	DWORD nrReqs = getRequestCount();
	POSITION pos;
	HRESULT res;
	OPCHANDLE *itemClientHandles = NULL;
	HRESULT *itemErrors = NULL;
	CComPtr<IMalloc> malloc;
	DWORD clientTransId = getClientTransactionId();
	GenericList<GenericRequest> reqList(getRequestList());

	_TRACE(TL_DEB, TG_TAC, (_T("send write data transaction:0x%X items:%lu hClientGroup 0x%X"),
								m_transactionId, nrReqs, groupClientHandle));

	::CoGetMalloc(MEMCTX_TASK, &malloc);
	// allocate memory
	itemClientHandles = (OPCHANDLE *)malloc->Alloc(sizeof(OPCHANDLE) * nrReqs);
	itemErrors = (HRESULT *)malloc->Alloc(sizeof(HRESULT) * nrReqs);
	if ((!itemClientHandles) || (!itemErrors))
	{
		if (itemClientHandles) malloc->Free(itemClientHandles);
		if (itemErrors) malloc->Free(itemErrors);
		return E_OUTOFMEMORY;
	}

	// item header
	pos = reqList.getStartPosition();
	while (pos)
	{
		req = reqList.getNext(pos);
		req->setObjectState(req->getObjectState() | GENREQ_FLAG_PROCESSED);
		item = req->getItem();

		// copy the data
		itemClientHandles[itemCount] = item->getClientHandle();
		itemErrors[itemCount] = req->getResult();
		_TRACE(TL_DEB, TG_TAC, (_T("send item 0x%X result:0x%X"), itemClientHandles[itemCount], itemErrors[itemCount]));

		// if not S_OK as error -> change master error
		if (itemErrors[itemCount] != S_OK)
			masterError = S_FALSE;

		itemCount ++;
	}

	_TRACE(TL_INF, TG_TAC, (_T("send write complete to callback 0x%X"), dataCallback));
	res = dataCallback->OnWriteComplete(clientTransId, groupClientHandle, masterError,
				itemCount, itemClientHandles, itemErrors);
	m_group->onWriteComplete(this);

	// free the allocated memory
	if (itemClientHandles) malloc->Free(itemClientHandles);
	if (itemErrors) malloc->Free(itemErrors);

	return res;
} // sendWriteData


HRESULT OPCTransaction::cancel(void)
{
	HRESULT res;

	cancelRequests();		// cancel the requests

	if ((m_transactionId != 0) && (m_connectionType == connectionPoint))
	{
		m_group->m_meSync.Lock();
		OPCObject<OPCGroup> *grp = (OPCObject<OPCGroup> *)m_group->m_me;
		if (grp)
			grp->internalAddRef();
		m_group->m_meSync.Unlock();

		if (!grp)
			return E_FAIL;

		CSingleLock lock(grp->getAdvisesLockObj(), TRUE);
		CComDynamicUnkArray *advises = grp->getAdvises();
		IUnknown** pp;
		IOPCDataCallback *dataCallback;
		DWORD groupClientHandle = m_group->getClientHandle();	
		DWORD clientTransId = getClientTransactionId();

		pp = advises->begin();
		while (pp < advises->end())
		{
			if (*pp != NULL)
			{
				dataCallback = (IOPCDataCallback *)*pp;

				_TRACE(TL_INF, TG_TAC, (_T("send cancel complete to callback 0x%X"), dataCallback));
				res = dataCallback->OnCancelComplete(clientTransId, groupClientHandle);
			}
			pp++;
		}

		lock.Unlock();
		grp->internalRelease();
	}

	return res;
}
