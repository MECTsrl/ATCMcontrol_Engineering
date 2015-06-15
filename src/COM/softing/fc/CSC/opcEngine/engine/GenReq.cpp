//-----------------------------------------------------------------------------
//                                  OPC ENGINE                                |
//                                                                            |
//  Filename    : GENREQ.H                                                    |
//  Version     : 2.00.0.00.release                                           |
//  Date        : 29-March-1999                                               |
//  Author      : ATCM                                                 |
//                                                                            |
//  Description : I/O Request classes                                         |
//                - GenericRequest: base class for I/O requests               |
//                                                                            |
//  CHANGE_NOTES                                                              |
//                                                                            |
//  date      name      change                                                |
//  ------------------------------------------------------------------------- |
//  01/11/99  RT        added direct and constant I/O modes                   |
//                                                                            |
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include <math.h>
#include "GenReq.h"
#include "GenItem.h"
#include "GenNS.h"
#include "GenCache.h"
#include "GenSrv.h"
#include "OPCEngine.h"
#include "OPCTrace.h"
#include "globals.h"


//-----------------------------------------------------------------------------
// GenericRequest                                                             |
//-----------------------------------------------------------------------------

GenericRequest::GenericRequest(
	IN BYTE operation,
	IN BYTE type,
	IN BYTE ioMode)
 : GenericObject(GENOBJ_TYPE_REQUEST)
{
	m_operation = operation;
	m_type = type;
	m_item = NULL;				
	m_itemTag = NULL;				
	setObjectState(stateInitialized);				
	m_startTime = 0;			
	m_completionSpan = 0;		
	m_result = S_OK;
	m_completedEvent = INVALID_HANDLE_VALUE;
	m_depData = NULL;
	m_flag = 0;

	m_resAddr = NULL;
	m_deviceIOMode = ioMode;

	switch (type)
	{
		case syncDevice:
			if (type == syncDevice)
				createCompletedEvent();	
		case syncCache:
			if (operation == read)
			{
				m_depData = new GenericRequestSyncReadData;
				((GenericRequestSyncReadData *)m_depData)->m_itemStateAddr = NULL;
			}
			else
			{
				m_depData = new GenericRequestWriteData;
				((GenericRequestWriteData *)m_depData)->m_writeValue.Clear();
			}
		break;
		case asyncDevice:
		{
			if (operation == write)
			{
				m_depData = new GenericRequestWriteData;
				((GenericRequestWriteData *)m_depData)->m_writeValue.Clear();
			}
		}
		break;
		case syncProperty:
		{
			createCompletedEvent();	
			m_depData = new GenericRequestSyncPropertyData;
		}
		break;
		case report:
		{
			setObjectState(stateCompleted);				
			doStatistics();
		}
		break;
	}
}

GenericRequest::~GenericRequest()
{
	// release objects
	setItem(NULL);
	setItemTag(NULL);

	if (m_operation == write)
	{
		GenericRequestWriteData *wd = getWriteData();
		if (wd)
			wd->m_writeValue.Clear();
	}

	if (m_depData)
		delete m_depData;

	if ((m_type == syncDevice) || (m_type == syncProperty))
	{
		if (m_completedEvent != INVALID_HANDLE_VALUE)
		{
			GenericPointer<OPCEngine> engine = ::getOPCEngine();
			engine->pushPoolEvent(m_completedEvent);
		}
	}	
}


//-----------------------------------------------------------------------------
// setItem
//
// - set item object
// - release previous item
//
// returns:
//		TRUE
//
BOOL GenericRequest::setItem(
	IN GenericItem *item) // item object
{ 
	_TRACE(TL_DEB, TG_REQ, (_T("set item 0x%X"), item));
	if (m_item)
		m_item->release();

	if (item)
	{
		m_objectTraceId.Empty();
		item->addRef();
	}
	m_item = item;
	return TRUE;
} // setItem


//-----------------------------------------------------------------------------
// setItemTag
//
// - set item tag object
// - release previous item tag
//
// returns:
//		TRUE
//
BOOL GenericRequest::setItemTag(
	IN GenericItemTag *itemTag) // tag object
{ 
	_TRACE(TL_DEB, TG_REQ, (_T("set item tag 0x%X"), itemTag));
	if (m_itemTag)
		m_itemTag->release();

	if (itemTag)
	{
		m_objectTraceId.Empty();
		itemTag->addRef();
	}
	m_itemTag = itemTag;
	return TRUE;
} // setItemTag


//-----------------------------------------------------------------------------
// getObjTraceId
//
// - get objects trace string id
//
// returns:
//		trace id string
//
LPCTSTR GenericRequest::getObjTraceId(void)
{ 
	if (m_objectTraceId.IsEmpty())
	{
		if (m_itemTag)
			m_objectTraceId.Format(_T("{%s|%8.8X| %s(%d/%d)}"), getObjTypeString(), this, m_itemTag->getName(), m_operation, m_type);
		else
		{
			if (m_item)
				m_objectTraceId.Format(_T("{%s|%8.8X| %s(%d/%d)}"), getObjTypeString(), this, m_item->getName(), m_operation, m_type);
			else
				m_objectTraceId.Format(_T("{%s|%8.8X| (%d/%d)}"), getObjTypeString(), this, m_operation, m_type);
		}
		m_objectTraceId.FreeExtra();
	}

	return m_objectTraceId; 
} // getObjTraceId


//-----------------------------------------------------------------------------
// getItem
//
// - get item object
//
// returns:
//		pointer to item object (maybe NULL)
//
GenericItem *GenericRequest::getItem(void)
{ 
	if (m_item)
		m_item->addRef(); 
	return m_item; 
} // getItem


//-----------------------------------------------------------------------------
// getItemTag
//
// - get tag object
//
// returns:
//		pointer to tag object (maybe NULL)
//
GenericItemTag *GenericRequest::getItemTag(void)
{ 
	if (m_item)
		return m_item->getItemTag();
	else
	{
		if (m_itemTag)
			m_itemTag->addRef(); 
		return m_itemTag; 
	}
} // getItemTag


//-----------------------------------------------------------------------------
// getCache
//
// - get cache object
//
// returns:
//		pointer to cache object (maybe NULL)
//
GenericCache *GenericRequest::getCache(void)
{ 
	if (m_item)
		return m_item->getCache(); 
	else
		if (m_itemTag)
			return m_itemTag->getCache(); 

	return NULL; 
} // getCache


//-----------------------------------------------------------------------------
// start
//
// - start request, set started flag
// - add to queue
// - signal start event
//
BOOL GenericRequest::start(void)
{ 
	_TRACE(TL_DEB, TG_REQ, (_T("> start")));

	GenericPointer<GenericItemTag> tag = getItemTag();
	if (!tag)
		return FALSE;	// -> error
	
	CSingleLock lock(getRequestStateLock(), TRUE);

	if (isStarted())
		return TRUE;

	setObjectState(stateStarted);				
	lock.Unlock();
	
	m_startTime = ::GetTickCount();	

	if (isDeviceRequest())	
	{					
		// device request 
		_TRACE(TL_DEB, TG_REQ, (_T("start device request")));

		switch (m_deviceIOMode)
		{
			case direct:
			{
				tag->handleRequest(this);
			}
			break;
			case queue:
			{
				// insert request into item queue and set start event
				// get the right queue and event
				tag->addToQueue(this, m_operation == read);
				tag->signalEvent(m_operation == read);	
			}
			break;
			case constant:
			{
				complete();
			}
			break;
			case delegate:
			{
				GenericPointer<GenericRequest> req;

				req = createDelegateRequest();
				if ((GenericRequest *)req)
				{
					req->start();
				}
				else
					complete(E_FAIL);
			}
			break;
		}
	}
	else
	{
		// cache request 
		_TRACE(TL_DEB, TG_REQ, (_T("start cache request")));
		complete();
	}

	_TRACE(TL_DEB, TG_REQ, (_T("< start")));
	return TRUE;
} // start


//-----------------------------------------------------------------------------
// complete
//
// - complete request, set completed flag
// - signal completed event if set
//
BOOL GenericRequest::complete(
	IN OPTIONAL HRESULT result,
	IN OPTIONAL BOOL validWriteQuality,
	IN OPTIONAL WORD writeQuality,
	IN OPTIONAL BOOL errorBeforeStart)
{ 
	_TRACE(TL_DEB, TG_REQ, (_T("> complete")));

	CSingleLock lock(getRequestStateLock(), TRUE);

	if (isCompleted())
		return TRUE;

	if ((!isStarted()) && (!errorBeforeStart))
		return FALSE;

	setObjectState(stateCompleted);				

	m_completionSpan = getTimeSpan(m_startTime, ::GetTickCount());
	setResult(result);	// request completed -> request handled 
	lock.Unlock();
	_TRACE(TL_INF, TG_REQ, (_T("completed request (0x%X) %lu ms"), result, m_completionSpan));
	doStatistics();

	if (!errorBeforeStart)
	{
		if (m_operation == write)
		{ // write operation -> update cache
			GenericPointer<GenericCache> cache = getCache(); 

			if (SUCCEEDED(getResult()))
			{
				GenericRequestWriteData *data = getWriteData();
				cache->updateValue(data->m_writeValue);
				if (!validWriteQuality)
					cache->setQuality(OPC_QUALITY_GOOD);
				else
					cache->setQuality(writeQuality);
			}
			else
			{ // write error
				if (!validWriteQuality)
				{
					FILETIME timeStamp;

					cache->getTimeStamp(&timeStamp);
					if ((timeStamp.dwLowDateTime != 0) || (timeStamp.dwHighDateTime != 0))
						cache->setQuality(OPC_QUALITY_LAST_KNOWN);
					else
						cache->setQuality(OPC_QUALITY_COMM_FAILURE);
				}
				else
					cache->setQuality(writeQuality);
			}

			if (m_item)
			{
				GenericPointer<GenericGroup> group = (GenericGroup *)m_item->getParentBranch();
				if (!!group)
				{
					if (group->getUpdateMode() == GenericGroup::report)
					{ // notifiy clients
						GenericPointer<GenericItemTag> tag = getItemTag();
						tag->notifyAllItems();			
					}
				}
				m_item->onWrite();
			}
		}
		else
		{
			if ((m_deviceIOMode == delegate) && SUCCEEDED(result))
			{
				completeDelegateRequest();
			}
			if (m_item)
				m_item->onRead(isDeviceRequest());
		}

		if (m_type == delegatedDevice)
		{
			GenericRequestDelegateData *delegateData;

			delegateData = getDelegateData();
			delegateData->m_request->complete(result, validWriteQuality, writeQuality, FALSE);
			delegateData->m_request->release();
			release();
		}
	}

	if (m_completedEvent != INVALID_HANDLE_VALUE)
	{
		return ::SetEvent(m_completedEvent);
	}

	_TRACE(TL_DEB, TG_REQ, (_T("< complete")));
	return TRUE;
} // complete


//-----------------------------------------------------------------------------
// pending
//
// - set request pending
//
BOOL GenericRequest::pending(void)
{ 
	_TRACE(TL_DEB, TG_REQ, (_T("> pending")));
	CSingleLock lock(getRequestStateLock(), TRUE);
	if (isPending())
		return TRUE;

	if ((!isStarted()) || (isCompleted()))
		return FALSE;

	setObjectState(statePending);				
	lock.Unlock();
	_TRACE(TL_DEB, TG_REQ, (_T("< pending")));
	return TRUE;
} // pending


BOOL GenericRequest::cancel(
	IN OPTIONAL HRESULT compResult,
	IN OPTIONAL BOOL changeCacheQuality,
	IN OPTIONAL WORD cacheQuality)
{
	_TRACE(TL_DEB, TG_REQ, (_T("> cancel")));

	CSingleLock lock(getRequestStateLock(), TRUE);
	if (isCanceled())
		return TRUE;

	if (isCompleted())
		return FALSE;

	complete(compResult);
	setObjectState(stateCanceled);
	lock.Unlock();

	if (changeCacheQuality)
	{
		GenericPointer<GenericCache> cache = getCache(); 
		cache->setQuality(cacheQuality);
	}		

	if ((isDeviceRequest()) && (isStarted()) && (m_deviceIOMode == queue))	
	{
		GenericPointer<GenericItemTag> tag = getItemTag();
		if ((GenericItemTag *)tag)
			tag->signalEvent(m_operation == read);	
	}

	_TRACE(TL_DEB, TG_REQ, (_T("< cancel")));
	return TRUE;
}

void GenericRequest::reset(void)
{
	_TRACE(TL_DEB, TG_REQ, (_T("reset request")));
	setObjectState(stateInitialized);
	setResult(S_OK);
}


BOOL GenericRequest::hasChanged(
	IN DWORD connection,
	IN LPVARIANT value,
	IN WORD quality)	
{
	if ((m_type != cyclic) && (m_type != report))
		return TRUE;

	if (!m_item)
		return TRUE;

	//-- cyclic requests --
	GenericPointer<GenericItemTag> tag = getItemTag();		
	VARIANT prevSendValue;
	WORD prevSendQuality;
	BOOL valueChanged;

	//-- get previous send value for the connection --
	if (!getSendValue(connection, &prevSendValue, &prevSendQuality))
	{
		_TRACE(TL_DEB, TG_REQ, (_T("no value send before to connection %X"), connection));
		return TRUE;
	}

	//-- compare quality --
	if (prevSendQuality != quality)
	{
		_TRACE(TL_DEB, TG_REQ, (_T("changed quality")));
		::VariantClear(&prevSendValue);
		return TRUE;
	}

	//-- compare value --
	if (value->vt != (VT_ARRAY | VT_BSTR))
	{	
		COleVariant prevSendValueObj(prevSendValue);
        if (prevSendValue.vt == value->vt)
        {
            // do special handlimng for VT_DECIMAL:
            switch (value->vt) 
			{
				case VT_DECIMAL:
					valueChanged = ::memcmp(&value->decVal, &prevSendValue.decVal, sizeof(DECIMAL)) != 0;
				break;
				// NOTE: compare float and double by bits, since the FP returns == for comparison
				//   between a valid real value and NAN/-INF/+INF
				case VT_R4:
					valueChanged = ::memcmp(&value->fltVal, &prevSendValue.fltVal, sizeof(float)) != 0;
				break;
				case VT_R8:
					valueChanged = ::memcmp(&value->dblVal, &prevSendValue.dblVal, sizeof(double)) != 0;
				break;
				default:
					valueChanged = !(prevSendValueObj == *value);
                break;
            }
        }
        else 
		{
            // different datatypes, never match!
            valueChanged = TRUE;
        }
	}
	else
	{ // COleVariant can't compare of BSTR arrays !
		LONG lBoundP, uBoundP, lBoundN, uBoundN;

		::SafeArrayGetLBound(prevSendValue.parray, 1, &lBoundP);
		::SafeArrayGetUBound(prevSendValue.parray, 1, &uBoundP);
		::SafeArrayGetLBound(value->parray, 1, &lBoundN);
		::SafeArrayGetUBound(value->parray, 1, &uBoundN);

		if ((lBoundN == lBoundP) && (uBoundN == uBoundP))	
		{
			// the same number of array members 
			// check the strings
			BSTR *bstrP, *bstrN;
			LONG i;

			valueChanged = FALSE;
	
			// access the data
			::SafeArrayAccessData(prevSendValue.parray, (void **)&bstrP);
			::SafeArrayAccessData(value->parray, (void **)&bstrN);

			// compare the strings
			for (i = 0; i <= uBoundN - lBoundN; i++)
			{
				if (wcscmp(bstrP[i], bstrN[i]) != 0)
				{
					valueChanged = TRUE;
					break;
				}
			}

			// release the array locks
			::SafeArrayUnaccessData(prevSendValue.parray);
			::SafeArrayUnaccessData(value->parray);
		}		
		else
			valueChanged = TRUE;
	}

	//-- percent deadband --
	if (valueChanged)
	{
		// check deadband only if value changed.
		GenericPointer<GenericGroup> group = (GenericGroup *)m_item->getParentBranch();

		if (((GenericGroup *)group) && (tag->is(GENOBJ_TYPE_TAG)))
		{
			GenericTag *tagV = (GenericTag *) ((GenericItemTag *)tag);

			if (tagV->getEUType() == OPC_ANALOG)
			{ // percent deadband only counts for analog tags
				VARIANT limits;
				VARIANT locvalue;
				GenericPointer<GenericCache> cache = getCache();
				double *limit;
				double deadband = 0.0;

				// calculate deadband
				tagV->getEUInfo(&limits); 
				if (limits.vt == (VT_R8 | VT_ARRAY))
				{
					if (limits.parray->rgsabound[0].cElements == 2)
					{
						SafeArrayAccessData(limits.parray, (void **)&limit);
						deadband = (limit[1] - limit[0]) * (double)group->getDeadBand();
						SafeArrayUnaccessData(limits.parray);
					}
				}
				::VariantClear(&limits);
				
				// convert values to double
				if ( (cache->changeDatatype(&prevSendValue, VT_R8, &prevSendValue)) &&
					 (cache->changeDatatype(value, VT_R8, &locvalue)) )
				{
					valueChanged = (fabs(prevSendValue.dblVal - locvalue.dblVal) > fabs(deadband));
					_TRACE(TL_INF, TG_REQ, (_T("hasChanged %s deadband %lf difference %lf"), BOOL2STR(valueChanged), fabs(deadband), fabs(prevSendValue.dblVal - locvalue.dblVal)));
				}
				::VariantClear(&locvalue);
			}
		}
	}

	::VariantClear(&prevSendValue);
	if (valueChanged)
	{
		_TRACE(TL_DEB, TG_REQ, (_T("changed value")));
		return TRUE;
	}
	_TRACE(TL_DEB, TG_REQ, (_T("not changed")));
	return FALSE;
}


BOOL GenericRequest::getWriteValue(OUT LPVARIANT value)
{
	GenericRequestWriteData *data = getWriteData();
	::VariantInit(value); 
	if (data)
		return (::VariantCopy(value, &data->m_writeValue) == S_OK); 
	else
		return FALSE;
}


BOOL GenericRequest::setWriteValue(
	IN LPCVARIANT value)
{
	_TRACE(TL_DEB, TG_REQ, (_T("> setWriteValue")));
	GenericRequestWriteData *data = getWriteData();
	if (data)
	{
		GenericPointer<GenericItemTag> tag = getItemTag();
		VARTYPE natDT = tag->getNativeDatatype();
		BOOL ok = TRUE;

		if (natDT == value->vt)
			data->m_writeValue = value;
		else
		{
			GenericPointer<GenericCache> cache = getCache();
			COleVariant tmp;
			tmp = value; 
			ok = cache->changeDatatype(&tmp, natDT, &tmp);
			if (ok)
				data->m_writeValue = tmp; 
		}

		_TRACE(TL_DEB, TG_REQ, (_T("< setWriteValue %s %s"), VARIANT2STR(data->m_writeValue), BOOL2STR(ok)));
		return ok;
	}
	else
		return FALSE;
}


BOOL GenericRequest::getSendValue(
	IN DWORD connection,
	OUT LPVARIANT value, 
	OUT WORD *quality)
{ 
	BOOL ret;
	GenericItemCyclicData *data = NULL;
	GenericPointer<GenericItem> item = getItem();

	if (!!item)
	{
		item->lockCyclicData();
		data = item->getCyclicData();
	}

	::VariantInit(value); 
	if (data)
	{
		if (ret = data->m_sendQualities.Lookup(connection, *quality))
		{
			COleVariant sendVal;
			if (ret = data->m_sendValues.Lookup(connection, sendVal))
			{
				ret =  (::VariantCopy(value, (LPVARIANT)sendVal) == S_OK); 
			}
		}
		item->unlockCyclicData();
		return ret;
	}
	else
	{
		if (!!item)
			item->unlockCyclicData();
		*quality = OPC_QUALITY_BAD;
		return FALSE;
	}
}


BOOL GenericRequest::setSendValue(
	IN DWORD connection,
	IN LPCVARIANT value, 
	IN WORD quality)
{
	BOOL ret = FALSE;

	switch(m_type)
	{
		case cyclic:
		case report:
		case asyncCache:
		case asyncDevice:
		{
			GenericPointer<GenericItem> item = getItem();
			GenericItemCyclicData *data = NULL;
			
			if (!!item)
			{
				item->lockCyclicData();
				data = item->getCyclicData();
			}
			if (data)
			{
				data->m_sendQualities.SetAt(connection, quality);
				data->m_sendValues.SetAt(connection, value);
				ret = TRUE;
			}
			if (!!item)
				item->unlockCyclicData();
		}
		break;
		default:
		break;
	}

	return ret;
}

void GenericRequest::setItemStateAddress(OPCITEMSTATE *itemStateAddr)
{ 
	GenericRequestSyncReadData *data = getSyncReadData();
	if (data)
		data->m_itemStateAddr = itemStateAddr; 
}

OPCITEMSTATE *GenericRequest::getItemStateAddress(void)
{
	GenericRequestSyncReadData *data = getSyncReadData();
	if (data)
		return data->m_itemStateAddr; 
	else
		return NULL;
}

void GenericRequest::setReadValueAddress(LPVARIANT readValueAddr)
{ 
	GenericRequestSyncPropertyData *data = getSyncPropertyData();
	if (data)
		data->m_readValueAddr = readValueAddr; 
}

LPVARIANT GenericRequest::getReadValueAddress(void)
{
	GenericRequestSyncPropertyData *data = getSyncPropertyData();
	if (data)
		return data->m_readValueAddr; 
	else
		return NULL;
}

void GenericRequest::doStatistics(void)
{
	GenericPointer<OPCEngine> engine = ::getOPCEngine();
	OPCStatistics *stat = engine->getStatisticsBuffer();

	switch (m_type)
	{
		case cyclic:
			stat->req.cyclicRead++;
			if (getResult() == S_OK) 
			{
				stat->req.timeRead += m_completionSpan;
				if (m_completionSpan > stat->req.maxTimeRead)
					stat->req.maxTimeRead = m_completionSpan;
			}
			else
				stat->req.cyclicReadBad++;
			
		break;
		case report:
			stat->req.reportRead++;
			if (FAILED(getResult())) 
				stat->req.reportReadBad++;
		break;
		case asyncDevice:
			if (m_operation == read)
			{
				stat->req.asyncDeviceRead++;
				if (getResult() == S_OK) 
				{
					stat->req.timeRead += m_completionSpan;
					if (m_completionSpan > stat->req.maxTimeRead)
						stat->req.maxTimeRead = m_completionSpan;
				}
				else
					stat->req.asyncDeviceReadBad++;
			}
			else
			{
				stat->req.asyncWrite++;
				if (getResult() == S_OK) 
				{
					stat->req.timeWrite += m_completionSpan;
					if (m_completionSpan > stat->req.maxTimeWrite)
						stat->req.maxTimeWrite = m_completionSpan;
				}
				else
					stat->req.asyncWriteBad++;
			}
		break;
		case asyncCache:
			stat->req.asyncCacheRead++;
			if (getResult() != S_OK) stat->req.asyncCacheReadBad++;
		break;
		case syncDevice:
		case syncProperty:
			if (m_operation == read)
			{
				stat->req.syncDeviceRead++;
				if (getResult() == S_OK) 
				{
					stat->req.timeRead += m_completionSpan;
					if (m_completionSpan > stat->req.maxTimeRead)
						stat->req.maxTimeRead = m_completionSpan;
				}
				else
					stat->req.syncDeviceReadBad++;
			}
			else
			{
				stat->req.syncWrite++;
				if (getResult() == S_OK) 
				{
					stat->req.timeWrite += m_completionSpan;
					if (m_completionSpan > stat->req.maxTimeWrite)
						stat->req.maxTimeWrite = m_completionSpan;
				}
				else
					stat->req.syncWriteBad++;
			}
		break;
		case syncCache:
			stat->req.syncCacheRead++;
			if (getResult() != S_OK) stat->req.syncCacheReadBad++;
		break;
	}

	engine->releaseStatisticsBuffer();
}

BOOL GenericRequest::setDelegateData(
	IN GenericRequestDelegateData *delegData)
{ 
	if (m_type == delegatedDevice) 
	{
		if (m_depData) 
			delete m_depData;

		m_depData = delegData;
		return TRUE;
	}
	return FALSE;
}


GenericRequest *GenericRequest::createDelegateRequest(void)
{
	GenericRequest *req = NULL;
	GenericPointer<GenericItemTag> itemTag = getItemTag();

	if ((GenericItemTag *)itemTag)
	{
		if (itemTag->is(GENOBJ_TYPE_TAG_PROPERTY))
		{
			GenericTagProperty *prop = (GenericTagProperty *)((GenericItemTag *)itemTag);
			DWORD pid = prop->getPropertyId();

			if ((pid >= 2) && (pid <= 4) && (getOperation() == GenericRequest::read))
			{
				GenericPointer<OPCEngine> engine = ::getOPCEngine();
				GenericPointer<GenericCreator> creator = engine->getCreator();
				GenericPointer<GenericTag> tag = prop->getTag();
				GenericRequestDelegateData *delegateData = new GenericRequestDelegateData;

				req = creator->createRequest(NULL, NULL, tag, GenericRequest::read, GenericRequest::delegatedDevice, (GenericRequest::requestDeviceIOMode)tag->getDeviceIOMode());
				req->setItemTag(tag);
				addRef();
				delegateData->m_request = this;
				req->setDelegateData(delegateData);
				req->addRef();
			}
		}
	}

	return req;
}

BOOL GenericRequest::completeDelegateRequest(void)
{
	GenericPointer<GenericItemTag> itemTag = getItemTag();
	BOOL ret = FALSE;

	if ((GenericItemTag *)itemTag)
	{
		if (itemTag->is(GENOBJ_TYPE_TAG_PROPERTY))
		{
			GenericTagProperty *prop = (GenericTagProperty *)((GenericItemTag *)itemTag);
			DWORD pid = prop->getPropertyId();

			if ((pid >= 2) && (pid <= 4) && (getOperation() == GenericRequest::read))
			{
				GenericPointer<GenericTag> tag = prop->getTag();
				GenericPointer<GenericCache> tagCache = tag->getCache();
				GenericPointer<GenericCache> cache = getCache();
	
				cache->setQuality(tagCache->getQuality());
				switch (pid)
				{
					case 2:
					{
						VARIANT value;
						tagCache->getValue(this, VT_EMPTY, &value);
						cache->updateValue(&value);	
						::VariantClear(&value);
					}
					break;
					case 3:
					{
						COleVariant value;
						value = (SHORT)tagCache->getQuality();
						cache->updateValue(&value);	
					}
					break;
					case 4:
					{
						FILETIME ft;
						SYSTEMTIME st;
						VARIANT value;

						::VariantInit(&value);
						value.vt = VT_DATE;
						tagCache->getTimeStamp(&ft);
						::FileTimeToSystemTime(&ft, &st);
						::SystemTimeToVariantTime(&st, &value.date);
						cache->updateValue(&value);	
						::VariantClear(&value);
					}
					break;
				}
				ret = TRUE;
			}
		}
	}

	return ret;
}

BOOL GenericRequest::createCompletedEvent(void)
{   
	if (INVALID_HANDLE_VALUE != m_completedEvent) 
		return TRUE;

	GenericPointer<OPCEngine> engine = ::getOPCEngine();
	m_completedEvent = engine->popPoolEvent();
	return (INVALID_HANDLE_VALUE != m_completedEvent);
}

CCriticalSection s_defReqStateLock;

CCriticalSection *GenericRequest::getRequestStateLock(void)
{
	if (m_item)
	{
		GenericPointer<GenericGroup> group = (GenericGroup *)m_item->getParentBranch();
		if (!!group)
		{
			GenericPointer<GenericServer> server = (GenericServer *)group->getParentBranch();
			if (!!server)
				return server->getRequestStateLock();
		}
	}

	return &s_defReqStateLock;
}
