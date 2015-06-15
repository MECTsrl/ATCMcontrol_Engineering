/////////////////////////////////////////////////////////////////////////////
// XMLDoc.h: SFL XML DOM interface
//
// Stingray Software Foundation Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       
// Description:  Wrapper around the MSXML DOM
//


#pragma once

#ifndef _WIN32_IE
#define _WIN32_IE 0x0501
#endif
#if _WIN32_IE < 0x0500
#error "The XML Property Bag implementation requires Internet Explorer 5.0"
#endif


#include <comdef.h>
#import "msxml.dll" rename_namespace("xml")


namespace stingray {
namespace foundation {

/////////////////////////////////////////////////////////////////////////////
// CXmlDocument

class CXMLDocument
{
protected:
	xml::IXMLDOMDocumentPtr m_spDoc;

public:
	// construct and create a document
	CXMLDocument(){}
	CXMLDocument(LPTSTR strRootTag, LPTSTR strHeader=NULL)
	{
		Create(strRootTag, strHeader);
	}


	// create an empty document with no root tag
	BOOL Create()
	{
		HRESULT hr = ::CoCreateInstance(__uuidof(xml::DOMDocument),
									NULL, CLSCTX_ALL,
									__uuidof(xml::IXMLDOMDocument),
									reinterpret_cast<void**>(&m_spDoc));
		return SUCCEEDED(hr)? TRUE : FALSE;
	}

	// create an empty document with a root tag
	BOOL Create(LPCTSTR strRootTag, LPCTSTR strHeader = NULL)
	{
		USES_CONVERSION;

		if (!strHeader) {
			strHeader = _T("");
		}

		if (!Create()) return FALSE;

		TCHAR szXml[2048];
		wsprintf(szXml, _T("%s\n<%s>\n</%s>\n"), strHeader, strRootTag, strRootTag);

		m_spDoc->loadXML(_bstr_t(szXml));
		return m_spDoc != 0;
	}


	// release the msmxml parser
	void Destroy()
	{
		if (m_spDoc != 0) m_spDoc = NULL;
	}

	// attach to an existing document
	void Attach(xml::IXMLDOMDocument* pDoc, bool bAddRef = true)
	{
		ATLASSERT(pDoc);
		if (m_spDoc != 0) Destroy();
		m_spDoc.Attach(pDoc, bAddRef);
	}

	xml::IXMLDOMDocument* Detach()
	{
		xml::IXMLDOMDocument* pDoc = m_spDoc;
		m_spDoc.Detach();
		return pDoc;
	}

	HRESULT GetDocument(xml::IXMLDOMDocument** ppDoc)
	{
		if (!ppDoc) return E_POINTER;
		*ppDoc = m_spDoc;
		return S_OK;
	}

	// returns the root element
	HRESULT GetRoot(xml::IXMLDOMElement** ppRoot) 
	{
		ATLASSERT((m_spDoc != 0) && ppRoot);
		*ppRoot = 0;
		HRESULT hr = S_OK;
		xml::IXMLDOMNodePtr spNode = m_spDoc->firstChild;
		while ((spNode != 0) && (spNode->nodeType != xml::NODE_ELEMENT)) {
			spNode = spNode->nextSibling;
		}
		if (spNode != 0) {
			hr = spNode->QueryInterface(__uuidof(xml::IXMLDOMElement), reinterpret_cast<void**>(ppRoot));
		}

		return hr;
	}

	bool HasDocument() const
	{
		return m_spDoc != 0;
	}

	// Persistence

	HRESULT Load(IStream* pStream)
	{
		HRESULT hr = S_OK;
		try {
			m_spDoc->load(pStream);
		}
		catch (_com_error& e) {
			hr = e.Error();
		}
		catch (...) {
			hr = E_UNEXPECTED;
		}
		return hr;
	}

	// populate from an existing XML file
	HRESULT LoadFromFile(LPCTSTR strFileName)
	{
		ATLASSERT(strFileName);
		if (m_spDoc == NULL) Create();
		_bstr_t bstrFile(strFileName);
		m_spDoc->load(bstrFile);

		return S_OK;
	}

	// save to an XML file
	HRESULT SaveToFile(LPCTSTR strFileName)
	{
		USES_CONVERSION;
		ATLASSERT((m_spDoc != 0) && strFileName);
		m_spDoc->save(strFileName);

		return S_OK;
	}


};


};	// namespace stingray::foundation
};	// namespace stingray




