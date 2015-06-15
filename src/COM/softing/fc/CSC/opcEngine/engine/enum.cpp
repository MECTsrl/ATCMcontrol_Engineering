//-----------------------------------------------------------------------------
//                                  OPC ENGINE                                |
//                                                                            |
//  Filename    : ENUM.CPP                                                    |   
//  Version     : 2.00.0.00.release                                           |
//  Date        : 29-March-1999                                               |
//  Author      : ATCM                                                 |
//                                                                            |
//  Description : DCOM enumerator classes                                     |
//                - COMStringEnum: string enumerator                          |
//                - COMUnknownEnum: IUnknown enumerator                       |
//                - OPCItemEnum: OPCITEMATTRIBUTES enumerator                 |
//                                                                            |
//  CHANGE_NOTES                                                              | 
//                                                                            |
//  date      name      change                                                |
//  ------------------------------------------------------------------------- |
//                                                                            |
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "enum.h"
#include "OPCTrace.h"


//-----------------------------------------------------------------------------
// COMStringEnum                                                              |
//-----------------------------------------------------------------------------

COMStringEnum::COMStringEnum()
{
	m_objectTraceId.Format(_T("(enumS| %X)"), this);
	_TRACE(TL_DEB, TG_COM, (_T("created COMStringEnum")));
	m_curPos = m_list.GetHeadPosition();
}

COMStringEnum::~COMStringEnum()
{
	::lastCOMObjectActions();
	_TRACE(TL_DEB, TG_COM, (_T("deleted COMStringEnum")));
}

BOOL COMStringEnum::releaseMe(void)
{ return TRUE; }

STDMETHODIMP COMStringEnum::Next(
	IN ULONG celt, 
	OUT LPOLESTR *rgelt, 
	OUT ULONG *pceltFetched)
{
	ULONG i = 0;
	ULONG cnt = 0;
	CString name;
	USES_CONVERSION;

	for (i = 0; (i < celt) && (m_curPos); i++)
	{
		name = m_list.GetNext(m_curPos);
		rgelt[i] = (LPWSTR) ::CoTaskMemAlloc((name.GetLength() + 1) * sizeof(WCHAR));
		::wcscpy(rgelt[i], T2COLE(name));
	}

	if (pceltFetched)
	{
		*pceltFetched = i;
	}
    return (celt == i) ? S_OK : S_FALSE;
}

STDMETHODIMP COMStringEnum::Skip(
	IN ULONG celt)
{
	ULONG i = 0;

	for (i = 0; (i < celt) && (m_curPos); i++)
	{
		m_list.GetNext(m_curPos);
	}	

    return (celt == i) ? S_OK : S_FALSE;
}

STDMETHODIMP COMStringEnum::Reset(void)
{
	m_curPos = m_list.GetHeadPosition();
	return S_OK;
}

STDMETHODIMP COMStringEnum::Clone(OUT IEnumString ** ppEnumString)
{
	if (!ppEnumString)
		return E_INVALIDARG;

	COMStringEnumObject *clone = new COMStringEnumObject;
	POSITION pos;
	POSITION clonePos;
	BOOL isCurPos;
	
	pos = m_list.GetHeadPosition();
	while (pos)
	{
		if (pos != m_curPos)
			isCurPos = FALSE;
		else
			isCurPos = TRUE;
		clonePos =clone->m_list.AddTail(m_list.GetNext(pos));
		if (isCurPos)
			clone->m_curPos = clonePos;
	}

	clone->QueryInterface(IID_IEnumString, (void **)ppEnumString);

	return S_OK;
}


//-----------------------------------------------------------------------------
// COMUnknownEnum                                                              |
//-----------------------------------------------------------------------------

COMUnknownEnum::COMUnknownEnum()
{
	m_objectTraceId.Format(_T("(enumU| %X)"), this);
	_TRACE(TL_DEB, TG_COM, (_T("created COMUnknownEnum")));
	m_curPos = m_list.GetHeadPosition();
}

COMUnknownEnum::~COMUnknownEnum()
{
	IUnknown *unk;
	POSITION pos;
	
	pos = m_list.GetHeadPosition();
	while (pos)
	{
		unk = m_list.GetNext(pos);
		unk->Release();
	}
	::lastCOMObjectActions();
	_TRACE(TL_DEB, TG_COM, (_T("deleted COMUnknownEnum")));
}

BOOL COMUnknownEnum::releaseMe(void)
{ return TRUE; }

STDMETHODIMP COMUnknownEnum::Next(
	IN ULONG celt, 
	OUT IUnknown **rgelt, 
	OUT ULONG *pceltFetched)
{
	ULONG i = 0;
	ULONG cnt = 0;

	for (i = 0; (i < celt) && (m_curPos); i++)
	{
		rgelt[i] = m_list.GetNext(m_curPos);
		rgelt[i]->AddRef();
	}

	if (pceltFetched)
	{
		*pceltFetched = i;
	}
    return (celt == i) ? S_OK : S_FALSE;
}

STDMETHODIMP COMUnknownEnum::Skip(
	IN ULONG celt)
{
	ULONG i = 0;

	for (i = 0; (i < celt) && (m_curPos); i++)
	{
		m_list.GetNext(m_curPos);
	}	

    return (celt == i) ? S_OK : S_FALSE;
}

STDMETHODIMP COMUnknownEnum::Reset(void)
{
	m_curPos = m_list.GetHeadPosition();
	return S_OK;
}

STDMETHODIMP COMUnknownEnum::Clone(OUT IEnumUnknown ** ppEnumUnknown)
{
	IUnknown *unknown;

	if (!ppEnumUnknown)
		return E_INVALIDARG;

	COMUnknownEnumObject *clone = new COMUnknownEnumObject;
	POSITION pos;
	POSITION clonePos;
	BOOL isCurPos;
	
	pos = m_list.GetHeadPosition();
	while (pos)
	{
		if (pos != m_curPos)
			isCurPos = FALSE;
		else
			isCurPos = TRUE;
		unknown = m_list.GetNext(pos);
		unknown->AddRef();
		clonePos = clone->m_list.AddTail(unknown);
		if (isCurPos)
			clone->m_curPos = clonePos;
	}

	clone->QueryInterface(IID_IEnumUnknown, (void **)ppEnumUnknown);

	return S_OK;
}


//-----------------------------------------------------------------------------
// OPCItemEnum                                                              |
//-----------------------------------------------------------------------------

OPCItemEnum::OPCItemEnum()
{
	m_objectTraceId.Format(_T("(enumI| %X)"), this);
	_TRACE(TL_DEB, TG_COM, (_T("created OPCItemEnum")));
	m_curPos = m_list.GetHeadPosition();
}

OPCItemEnum::~OPCItemEnum()
{
	GenericItem *item;
	POSITION pos;
	
	pos = m_list.GetHeadPosition();
	while (pos)
	{
		item = m_list.GetNext(pos);
		item->release();
	}
	::lastCOMObjectActions();
	_TRACE(TL_DEB, TG_COM, (_T("deleted OPCItemEnum")));
}

BOOL OPCItemEnum::releaseMe(void)
{ return TRUE; }


STDMETHODIMP OPCItemEnum::Next(
	IN ULONG celt, 
	OUT OPCITEMATTRIBUTES **rgelt, 
	OUT ULONG *pceltFetched)
{
	ULONG i = 0;
	ULONG cnt = 0;
	GenericItem *item;
	GenericPointer<GenericItemTag> tag;
	CString str;
	OPCITEMATTRIBUTES *itemArray;
	USES_CONVERSION;

	if ((!rgelt) || (celt < 1) )
		return E_INVALIDARG;

	*rgelt = NULL;
	itemArray = (OPCITEMATTRIBUTES*)CoTaskMemAlloc(celt*sizeof(OPCITEMATTRIBUTES));
	if (itemArray == NULL)
	   return E_OUTOFMEMORY;

	for (i = 0; (i < celt) && (m_curPos); i++)
	{
		item = m_list.GetNext(m_curPos);
		tag = item->getItemTag();

		str = tag->getFullName();
		itemArray[i].szItemID = (LPWSTR) ::CoTaskMemAlloc((str.GetLength() + 1) * sizeof(WCHAR));
		::wcscpy(itemArray[i].szItemID, T2COLE(str));

		str = item->getAccessPath();
		itemArray[i].szAccessPath = (LPWSTR) ::CoTaskMemAlloc((str.GetLength() + 1) * sizeof(WCHAR));
		::wcscpy(itemArray[i].szAccessPath, T2COLE(str));

		itemArray[i].bActive = item->getActive();
		itemArray[i].hClient = item->getClientHandle();
		itemArray[i].hServer = item->getServerHandle();
		itemArray[i].vtRequestedDataType = item->getReqDatatype();
		itemArray[i].pBlob = NULL;
		itemArray[i].dwBlobSize = 0;
		itemArray[i].dwAccessRights = tag->getAccessRights();
		itemArray[i].vtCanonicalDataType = tag->getNativeDatatype();
		if (tag->is(GENOBJ_TYPE_TAG))
		{
			GenericTag *tagV = (GenericTag *) ((GenericItemTag *)tag);
			itemArray[i].dwEUType = tagV->getEUType();
			tagV->getEUInfo(&itemArray[i].vEUInfo);
		}
		else
		{
			itemArray[i].dwEUType = OPC_NOENUM;
			::VariantInit(&itemArray[i].vEUInfo);
		}
	}

	if (pceltFetched)
	{
		*pceltFetched = i;
	}

	if (i != 0)
		*rgelt = itemArray;
	else
	{
		if (celt != 0)
			CoTaskMemFree(itemArray);
		*rgelt = NULL;
	}
	
    return (celt == i) ? S_OK : S_FALSE;
}

STDMETHODIMP OPCItemEnum::Skip(
	IN ULONG celt)
{
	ULONG i = 0;

	for (i = 0; (i < celt) && (m_curPos); i++)
	{
		m_list.GetNext(m_curPos);
	}	

    return (celt == i) ? S_OK : S_FALSE;
}

STDMETHODIMP OPCItemEnum::Reset(void)
{
	m_curPos = m_list.GetHeadPosition();
	return S_OK;
}

STDMETHODIMP OPCItemEnum::Clone(OUT IEnumOPCItemAttributes ** ppEnumOPCItemAttr)
{
	GenericItem *item;

	if (!ppEnumOPCItemAttr)
		return E_INVALIDARG;

	OPCItemEnumObject *clone = new OPCItemEnumObject;
	POSITION pos;
	POSITION clonePos;
	BOOL isCurPos;
	
	pos = m_list.GetHeadPosition();
	while (pos)
	{
		if (pos != m_curPos)
			isCurPos = FALSE;
		else
			isCurPos = TRUE;
		item = m_list.GetNext(pos);
		item->addRef();
		clonePos = clone->m_list.AddTail(item);
		if (isCurPos)
			clone->m_curPos = clonePos;
	}

	clone->QueryInterface(IID_IEnumOPCItemAttributes, (void **)ppEnumOPCItemAttr);
	return S_OK;
}
