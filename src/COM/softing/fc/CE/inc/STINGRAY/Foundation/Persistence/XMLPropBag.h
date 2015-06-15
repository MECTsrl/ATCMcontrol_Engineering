/////////////////////////////////////////////////////////////////////////////
// XMLPropBag.h: SFL Property bags
//
// Stingray Software Foundation Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Gilberto Araya
// Description:  Definition of the CXMLPropertyBag class
//


#pragma once

#ifndef _WIN32_IE
#define _WIN32_IE 0x0501
#endif
#if _WIN32_IE < 0x0500
#error "The XML Property Bag implementation requires Internet Explorer 5.0"
#endif


#include "SflPropBags.h"
#include "SflPropBagsRes.h"       // main symbols
#include "PropBagImpl.h"
#include <foundation\string\StringEx.h>
#import "msxml.dll" rename_namespace("xml")


namespace stingray {
namespace foundation {


/////////////////////////////////////////////////////////////////////////////
// CXMLPropertyBag

class ATL_NO_VTABLE CXMLPropertyBag : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CXMLPropertyBag, &CLSID_XMLPropertyBag>,
	public IDispatchImpl<CPropertyBagImpl, &IID_IPersistenceStrategy, &LIBID_SflPropBags, 2, 0>,
	public ISupportErrorInfo
{
public:
	CXMLPropertyBag();

DECLARE_REGISTRY_RESOURCEID(IDR_FILEPROPERTYBAG)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CXMLPropertyBag)
	COM_INTERFACE_ENTRY2(IDispatch, IPersistenceStrategy)
	COM_INTERFACE_ENTRY(IPersistenceStrategy)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IPropertyBag2)
	COM_INTERFACE_ENTRY(IPropertyBag)
END_COM_MAP()

public:
// COM interfaces

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IPersistanceStrategy
	STDMETHOD(Init)(VARIANT varInitParam);
	STDMETHOD(Commit)();

// IPropertyBag2
	STDMETHOD(CountProperties)(ULONG * pcProperties);
	STDMETHOD(GetPropertyInfo)(ULONG iProperty, ULONG cProperties, PROPBAG2 * pPropBag, ULONG * pcProperties);

protected:
	// Instance data
	foundation::string m_sFileName;
	IStreamPtr m_spStream;
	LARGE_INTEGER m_nStreamPos;

	xml::IXMLDOMDocumentPtr m_spXMLDocument;
	xml::IXMLDOMElementPtr m_spXMLElement;

protected:
	// Overriden implementation operations
	virtual bool ReadProperty(const foundation::string& sPropName, _variant_t& vaValue);
	virtual void WriteBasicType(const foundation::string& sPropName, const _variant_t& vaValue);
	virtual void WriteObject(const foundation::string& sPropName, IUnknownPtr spUnk);
	virtual void WriteSafeArray(const foundation::string& sPropName, _variant_t& vaArray);

	// Internal implementation
	xml::IXMLDOMElementPtr GetBasicTypeElement(const foundation::string& sPropName, const _variant_t& vaValue);
	xml::IXMLDOMElementPtr GetObjectElement(const foundation::string& sPropName, IUnknownPtr spUnk);
	xml::IXMLDOMElementPtr GetSafeArrayElement(const foundation::string& sPropName, _variant_t& vaArray);

	bool ReadPropertyFromElement(xml::IXMLDOMElementPtr spPropertyElement, _variant_t& vaValue);
	void ReadBasicTypeFromElement(xml::IXMLDOMElementPtr spPropertyElement, _variant_t& vaValue);
	void ReadObjectFromElement(xml::IXMLDOMElementPtr spPropertyElement, _variant_t& vaValue);
	void ReadSafeArrayFromElement(xml::IXMLDOMElementPtr spPropertyElement, _variant_t& vaArray);

	static xml::IXMLDOMElementPtr GetPropertyElement(xml::IXMLDOMElementPtr spParent, const foundation::string& sPropName);
	static void ClearSubelement(xml::IXMLDOMElementPtr spParent, const foundation::string& sPropName);

	static foundation::cstring BinaryToString(SAFEARRAY* psaArray);
	static SAFEARRAY* StringToBinary(foundation::cstring& sBinary);

private:

	static LPCTSTR lpszRootTag;
};


};	// namespace stingray::foundation
};	// namespace stingray

