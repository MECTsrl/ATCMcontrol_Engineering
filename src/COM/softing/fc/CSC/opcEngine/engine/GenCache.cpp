//-----------------------------------------------------------------------------
//                                  OPC ENGINE                                |
//                                                                            |
//  Filename    : GENCACHE.CPP                                                |   
//  Version     : 2.00.0.00.release                                           |
//  Date        : 29-March-1999                                               |
//  Author      : ATCM                                                 |
//                                                                            |
//  Description : Cache classes                                               |
//                - GenericCache: cache object for item tag values            |
//                                                                            |
//  CHANGE_NOTES                                                              |
//                                                                            |
//  date      name      change                                                |
//  ------------------------------------------------------------------------- |
//  10/29/98  RT        added updateCache method                              |
//  01/03/99  RT        changed relation between cache and tag from 1:1 to 1:n|
//                                                                            |
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include <malloc.h>
#include "GenCache.h"
#include "GenNS.h"
#include "OPCTrace.h"
#include "OPCEngine.h"



CCriticalSection s_cacheLock;

//-----------------------------------------------------------------------------
// GenericCache                                                               |
//-----------------------------------------------------------------------------

GenericCache::GenericCache(void)
 : GenericObject(GENOBJ_TYPE_CACHE)
{
	m_datatype = VT_EMPTY;
	m_objectTraceId.Empty();
	m_value.Clear();
	m_quality = OPC_QUALITY_BAD;
    m_timeStamp.dwLowDateTime  = 0L;
    m_timeStamp.dwHighDateTime = 0L;
}

GenericCache::~GenericCache(void)
{
}


void GenericCache::init(void)
{
	VARIANT value;
	// initialize cache entry
	::VariantInit(&value);
	if ((m_datatype & VT_ARRAY) == VT_ARRAY)
	{ // array
		SAFEARRAYBOUND dim; 
		dim.lLbound = 0; 
		dim.cElements = 0; 
		value.vt = m_datatype;
		value.parray = ::SafeArrayCreate(m_datatype & ~VT_ARRAY, 1, &dim);
        //vo ++: the following code is necessary to marshal Arrays of DECIMAL properly
        if (m_datatype & VT_DECIMAL)
        {
            value.parray->fFeatures |= FADF_VARIANT;
        }
        // vo ++
	}
	else
	{
		value.vt = m_datatype;
		switch (m_datatype)
		{
			case VT_UI1:
				value.bVal = 0;
			break;
			case VT_I2:
				value.iVal = 0;
			break;
			case VT_I4:
				value.lVal = 0;
			break;
			case VT_R4:
				value.fltVal = 0.0;
			break;
			case VT_R8:
				value.dblVal = 0.0;
			break;
			case VT_BOOL:
				value.boolVal = FALSE;
			break;
			case VT_DATE:
				value.date = 0;
			break;
			case VT_DECIMAL:
				value.decVal.wReserved = 0;
				value.decVal.signscale = 0;
				value.decVal.Hi32 = 0;
				value.decVal.Mid32 = 0;
				value.decVal.Lo32 = 0;
			break;
			case VT_BSTR:
			{
				USES_CONVERSION;
				value.bstrVal = ::SysAllocString(T2OLE(_T("")));
			}
			break;
		}
	}

	m_value = value;		
	::VariantClear(&value);
}

//-----------------------------------------------------------------------------
// getObjTraceId
//
// - get objects trace string id
//
// returns:
//		trace id string
//
LPCTSTR GenericCache::getObjTraceId(void)
{ 
	if (m_objectTraceId.IsEmpty())
	{
		m_objectTraceId.Format(_T("{%s|%8.8X}"), getObjTypeString(), this);
		m_objectTraceId.FreeExtra();
	}

	return m_objectTraceId; 
} // getObjTraceId

//-----------------------------------------------------------------------------
// setValue
//
// - set cached value
// - convert value to native datatype of tag
//
// returns:
//		TRUE  - all ok
//		FALSE - can't convert to native datatype or write only tag
//
BOOL GenericCache::setValue(
	IN LPCVARIANT value)	// new value
{
	BOOL ok = TRUE;
	FILETIME timeStamp;

	if ((m_datatype == value->vt) || (m_datatype == VT_EMPTY))
	{
	    ::CoFileTimeNow(&timeStamp);
		onSetTimeStamp(&timeStamp);
		m_timeStamp.dwLowDateTime = timeStamp.dwLowDateTime; m_timeStamp.dwHighDateTime = timeStamp.dwHighDateTime;
		onSetValue(value);
		s_cacheLock.Lock();
		m_value = value; 
		s_cacheLock.Unlock();
	}
	else
	{
		COleVariant tmp;

		tmp = value; 
		ok = changeDatatype(&tmp, m_datatype, &tmp);
		if (ok)
		{
			::CoFileTimeNow(&timeStamp);
			onSetTimeStamp(&timeStamp);
			m_timeStamp.dwLowDateTime = timeStamp.dwLowDateTime; m_timeStamp.dwHighDateTime = timeStamp.dwHighDateTime;
			onSetValue(tmp);
			s_cacheLock.Lock();
			m_value = tmp; 
			s_cacheLock.Unlock();
		}
	}

	_TRACE(TL_INF, TG_CACHE, (_T("setValue %s %s"), VARIANT2STR(m_value), BOOL2STR(ok)));
	return ok; 
} // setValue


//-----------------------------------------------------------------------------
// updateValue
//
// - update cached value
//
// returns:
//		TRUE  - all ok
//		FALSE - can't convert to native datatype or write only tag
//
BOOL GenericCache::updateValue(
	IN LPCVARIANT value)	// new value
{
	if (!(m_datatype & VT_ARRAY))
		return setValue(value);
	else
	{ // cache value is an array
		// -> don't set the whole array! set only the elements of the value new
		LONG lBoundC, uBoundC;
		LONG lBoundV, uBoundV;
		LONG lBound, uBound;
		BYTE *dataC, *dataV;
		FILETIME timeStamp;

		if (m_value.vt != value->vt)
			return FALSE;

		if (m_value.parray->cbElements == 0)
			return setValue(value);	// set array if no current elements

		::CoFileTimeNow(&timeStamp);
		onSetTimeStamp(&timeStamp);
		m_timeStamp.dwLowDateTime = timeStamp.dwLowDateTime; m_timeStamp.dwHighDateTime = timeStamp.dwHighDateTime;
		onSetValue(value);

		s_cacheLock.Lock();

		SafeArrayGetLBound(m_value.parray, 1, &lBoundC);
		SafeArrayGetUBound(m_value.parray, 1, &uBoundC);
		SafeArrayGetLBound(value->parray, 1, &lBoundV);
		SafeArrayGetUBound(value->parray, 1, &uBoundV);

		uBound = (uBoundC < uBoundV) ? uBoundV : uBoundC; 
		lBound = (lBoundC > lBoundV) ? lBoundV : lBoundC; 
		if ((lBoundC > lBoundV) || (uBoundC < uBoundV))
		{ 
			SAFEARRAYBOUND dim; 
			dim.lLbound = lBound;
			dim.cElements = (uBound - lBound + 1); 
			SafeArrayRedim(m_value.parray, &dim);
            // vo ++: set new bounds
            lBoundC = lBound;
            uBoundC = uBound;
            // vo --
		}

		SafeArrayAccessData(m_value.parray, (void **)&dataC);
		SafeArrayAccessData(value->parray, (void **)&dataV);
		if (value->vt != (VT_BSTR | VT_ARRAY))
		{
			int elementSize = 0;
			switch (m_value.vt & ~VT_ARRAY)
			{
				case VT_UI1: elementSize = 1; break;
				case VT_I2: case VT_BOOL: elementSize = 2; break;
				case VT_I4: case VT_R4: elementSize = 4; break;
				case VT_R8: case VT_DATE: elementSize = 8; break;
				case VT_DECIMAL: elementSize = 16; break;
			}

			memcpy(dataC + ((lBoundV - lBoundC) * elementSize), dataV, (uBoundV - lBoundV + 1) * elementSize);				
		}
		else
		{
			BSTR *bstrC, *bstrV;
			LONG iC, iV;

			bstrC = (BSTR *)dataC;
			bstrV = (BSTR *)dataV;

			for (iV = 0, iC = (lBoundV - lBoundC); iV <= uBoundV-lBoundV; iV++, iC++)
			{
				::SysFreeString(bstrC[iC]);
				bstrC[iC] = ::SysAllocString(bstrV[iV]);
			}
		}
		SafeArrayUnaccessData(m_value.parray);
		SafeArrayUnaccessData(value->parray);
		s_cacheLock.Unlock();

		_TRACE(TL_INF, TG_CACHE, (_T("updateValue [%d,%d] %s "), lBoundV, uBoundV, VARIANT2STR(m_value)));
	}

	return TRUE;
} // updateValue


//-----------------------------------------------------------------------------
// getValue
//
// - get cached value
// - convert value to the requested datatype
//
// returns:
//		TRUE  - all ok
//		FALSE - can't copy cached value
//				can't convert to requested datatype; but value is copied !
//
BOOL GenericCache::getValue(
	IN VARTYPE typeWanted, 
	OUT LPVARIANT value)
{
    BOOL ok = getValue(value);

    if ((typeWanted != VT_EMPTY) && (typeWanted != (VT_EMPTY | VT_ARRAY)) &&
		(typeWanted != value->vt)) 
	{
		ok |= changeDatatype(value, typeWanted, value); 
		_TRACE(TL_DEB, TG_CACHE, (_T("getValue changed datatype to %d ok:%s"), typeWanted, BOOL2STR(ok)));
    }

    return ok;
} // getValue

BOOL GenericCache::getValue(
	OUT LPVARIANT value)
{
	BOOL ok;
	::VariantInit(value); 
	s_cacheLock.Lock();
	ok = (::VariantCopy(value, &m_value) == S_OK); 
	s_cacheLock.Unlock();
	_TRACE(TL_DEB, TG_CACHE, (_T("getValue %s ok:%s"), VARIANT2STR(m_value), BOOL2STR(ok)));
	return ok;
}

BOOL GenericCache::getValue(IN GenericRequest *req, IN VARTYPE typeWanted, OUT LPVARIANT value)
{
	GenericPointer<GenericItemTag> itemTag = req->getItemTag();

	if ((itemTag->getAccessRights() & OPC_READABLE) != OPC_READABLE)  
	{ 
		::VariantInit(value); 
		return FALSE; 
	}
	else 
		return getValue(typeWanted, value); 
}


WORD GenericCache::getQuality(IN GenericRequest *req)
{ 
	GenericPointer<GenericItemTag> itemTag = req->getItemTag();
	
	if ((itemTag->getAccessRights() & OPC_READABLE) != OPC_READABLE) 
		return OPC_QUALITY_BAD;
	else 
		return m_quality; 
}

BOOL GenericCache::getTimeStamp(IN GenericRequest *req, OUT LPFILETIME timeStamp)
{ 
	GenericPointer<GenericItemTag> itemTag = req->getItemTag();

	if (!timeStamp) 
		return FALSE;
	
	if ((itemTag->getAccessRights() & OPC_READABLE) != OPC_READABLE) 
		{ timeStamp->dwLowDateTime = 0; timeStamp->dwHighDateTime = 0; }	
	else
		{ timeStamp->dwLowDateTime = m_timeStamp.dwLowDateTime; timeStamp->dwHighDateTime = m_timeStamp.dwHighDateTime; } 
	return TRUE; 
}




BOOL GenericCache::setQuality(
	IN WORD quality)
{ 
	_TRACE(TL_DEB, TG_CACHE, (_T("setQuality 0x%X"), quality));
	onSetQuality(quality);
	m_quality = quality; 
	return TRUE; 
}

BOOL GenericCache::setTimeStamp(
	IN LPFILETIME timeStamp)
{ 
	_TRACE(TL_DEB, TG_CACHE, (_T("setTimeStamp")));
	onSetTimeStamp(timeStamp);
	m_timeStamp.dwLowDateTime = timeStamp->dwLowDateTime; m_timeStamp.dwHighDateTime = timeStamp->dwHighDateTime;
	return TRUE; 
}

BOOL GenericCache::supportDatatype(
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

BOOL GenericCache::changeDatatype(
	IN VARIANT *value,
	IN VARTYPE type,
	OUT VARIANT *changedValue)
{
	return (::VariantChangeType(changedValue, value, 0, type) == S_OK);
}

//-----------------------------------------------------------------------------
// setDatatype
//
// - sets datatype
//
// returns:
//		TRUE
//
BOOL GenericCache::setDatatype(
	IN VARTYPE datatype)	// new native datatype
{ 
	_TRACE(TL_INF, TG_NS, (_T("set datatype %u"), datatype));
	if (!supportDatatype(datatype))
		return FALSE;

	onSetDatatype(datatype);
	m_datatype = datatype; 
	return TRUE; 
} // setDatatype


void GenericCache::onSetValue(IN LPCVARIANT value)
{}
void GenericCache::onSetQuality(IN WORD quality)
{}
void GenericCache::onSetTimeStamp(IN LPFILETIME timeStamp)
{}
void GenericCache::onSetDatatype(IN VARTYPE datatype)
{}

ULONG GenericCache::getValueSize(
	IN OPTIONAL LPVARIANT val)
{
	LPVARIANT value;
	ULONG size  = sizeof(VARIANT);
	USES_CONVERSION;

	if (!val)
		value = m_value;
	else
		value = val;

	if ((value->vt & VT_ARRAY) == 0)
	{ // no array
		if (value->vt == VT_BSTR)
		{
			size += 6 + (_tcslen(OLE2CT(value->bstrVal)) * 2);
		}
	}
	else
	{ // array
		if (value->parray->cDims == 1)
		{
			size += sizeof(SAFEARRAY) +
				(value->parray->cbElements * value->parray->rgsabound[0].cElements);

			if (value->vt == (VT_BSTR | VT_ARRAY))
			{
				BSTR *bstr;
				ULONG i;

				SafeArrayAccessData(value->parray, (void **)&bstr);
				for (i = 0; i < value->parray->rgsabound[0].cElements; i++)
				{
					size += 6 + (_tcslen(OLE2CT(bstr[i])) * 2);
				}
				SafeArrayUnaccessData(value->parray);
			}
		}
		else
		{
			TRACE(TL_ERR, TG_CACHE, (_T("only arrays with one dimesion supported!")));
		}
	}

	return size;
}


void GenericCache::getAllItemTags(
	OUT ItemTagList &itemTagList)
{
	GenericPointer<OPCEngine> engine = ::getOPCEngine();
	GenericPointer<OPCRootNS> nsRoot = engine->getNamespaceRoot();

	addItemTagsOfNode(nsRoot, itemTagList);
}


void GenericCache::addItemTagsOfNode(
	IN GenericBranch *branch,
	OUT ItemTagList &itemTagList)
{
	POSITION pos;
	{
	GenericPointer<GenericCache> cache;
 	GenericList<GenericLeaf> leafList(branch->getLeafList());
	GenericList<GenericTagProperty> propList;
	GenericTag *tag;
	GenericTagProperty *prop;
	POSITION posProp;

	pos = leafList.getStartPosition();
	while(pos)
	{
		tag = (GenericTag *)leafList.getNext(pos);
		if (tag->hasCache(this))
		{
			tag->addRef();
			itemTagList.AddTail(tag);
		}

		propList = tag->getPropertyList();
		posProp = propList.getStartPosition();
		while(posProp)
		{
			prop = (GenericTagProperty *)propList.getNext(posProp);
			if (prop->hasCache(this))
			{
				prop->addRef();
				itemTagList.AddTail(prop);
			}
		}
		propList.unlock();
	}
	leafList.unlock();
	}

 	GenericList<GenericBranch> branchList(branch->getBranchList());
	GenericNode *node;

	pos = branchList.getStartPosition();
	while(pos)
	{
		node = (GenericNode *)branchList.getNext(pos);
		addItemTagsOfNode(node, itemTagList);
	}
}

