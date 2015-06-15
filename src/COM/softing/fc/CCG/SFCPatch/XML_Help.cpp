


#include "stdafx.h"
#include "XML_Help.h"

HRESULT XML_ElementGetTagName(IXMLElement2* pIElement, CString& strName)
{
	BSTR sBuf;
	HRESULT hr = pIElement->get_tagName(&sBuf);
	if (SUCCEEDED(hr))
	{
		strName=sBuf;
		::SysFreeString(sBuf);
	}
	return hr;
}

HRESULT XML_ElementGetAttribute(IXMLElement2* pIElement, const CString& strAttrName, CString& strAttrValue)
{
	CComVariant vValue;
    HRESULT hr = pIElement->getAttribute(CComBSTR(strAttrName), &vValue);
	if (hr == S_FALSE)
	{
		strAttrValue="";
		return hr;
	}

    ASSERT(vValue.vt == VT_BSTR);
    if (vValue.vt != VT_BSTR) {
        HRESULT hr2 = vValue.ChangeType(VT_BSTR);
        if (FAILED(hr2)) return hr2;
    }

    // getAttribute() seems to return the string already decoded !!
    strAttrValue = vValue.bstrVal;
    
	return hr;
}

HRESULT XML_ElementGetAttribute(IXMLElement2* pIElement, const CString& strAttrName, long& lAttrValue)
{
	CString strAttrValue;
	HRESULT hr=XML_ElementGetAttribute(pIElement, strAttrName, strAttrValue);
	if (hr != S_OK)
		return hr;

	lAttrValue = ::_tcstol(strAttrValue, NULL, 0);
	return hr;
}

HRESULT XML_ElementGetAttribute(IXMLElement2* pIElement, const CString& strAttrName, CPoint& ptAttrValue)
{
	CString strAttrValue;
	HRESULT hr=XML_ElementGetAttribute(pIElement, strAttrName, strAttrValue);
	if (hr != S_OK)
		return hr;
    
	LPTSTR pszStop;
	ptAttrValue.x = ::_tcstol(strAttrValue, &pszStop, 0);     // %TBD% should be baselib call !!!!
	pszStop = _tcschr(pszStop, _T(';') );
	if( pszStop )
	{
	pszStop++;
	ptAttrValue.y = ::_tcstol(pszStop, NULL, 0);     // %TBD% should be baselib call !!!!
	} 
	else 
	{
		ASSERT(FALSE);
	}
	return hr;
}

HRESULT XML_ElementGetAttribute(IXMLElement2* pIElement, const CString& strAttrName, CSize& siAttrValue)
{
	CPoint ptAttrValue;
	HRESULT hr=XML_ElementGetAttribute(pIElement, strAttrName, ptAttrValue);
	if (hr != S_OK)
		return hr;
    
	siAttrValue.cx=ptAttrValue.x;
	siAttrValue.cy=ptAttrValue.y;

	return hr;
}

HRESULT XML_CollectionGetItem(IXMLElementCollection *pColl, long lIndex, IXMLElement2** ppIXMLElement)
{
	HRESULT hr;
	CComVariant vIndex = (long) lIndex;
	CComVariant vEmpty;
	IDispatch* pDisp=NULL;

	hr = pColl->item(vIndex, vEmpty, &pDisp);
	if (FAILED(hr)) return hr;
	if (pDisp == NULL) return E_NOINTERFACE;

	hr=pDisp->QueryInterface(IID_IXMLElement2, (LPVOID*)ppIXMLElement);
	pDisp->Release();
	return hr; 
}

