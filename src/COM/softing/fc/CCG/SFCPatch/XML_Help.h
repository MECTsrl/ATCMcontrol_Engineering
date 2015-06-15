

#ifndef __XML_HELP_H_
#define __XML_HELP_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include <msxml.h>      // this includes header of IE4 delivered msxml control !!
#include "msXML_2.h"      // must include this local MIDL compiled header!!

HRESULT XML_ElementGetTagName(IXMLElement2* pIElement, CString& strName);
HRESULT XML_ElementGetAttribute(IXMLElement2* pIElement, const CString& strAttrName, CString& strAttrVal);
HRESULT XML_ElementGetAttribute(IXMLElement2* pIElement, const CString& strAttrName, long& lAttrVal);
HRESULT XML_ElementGetAttribute(IXMLElement2* pIElement, const CString& strAttrName, CPoint& ptAttrVal);
HRESULT XML_ElementGetAttribute(IXMLElement2* pIElement, const CString& strAttrName, CSize& siAttrValue);

HRESULT XML_CollectionGetItem(IXMLElementCollection *pColl, long lIndex, IXMLElement2** ppIXMLElement);

#endif