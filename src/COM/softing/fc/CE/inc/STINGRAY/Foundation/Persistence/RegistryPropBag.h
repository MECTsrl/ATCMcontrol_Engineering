/////////////////////////////////////////////////////////////////////////////
// RegistryPropBag.h: SFL Property bags
//
// Stingray Software Foundation Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Gilberto Araya
// Description:  Definition of the CRegistryPropertyBag class
//


#pragma once

#include "SflPropBagsRes.h"       // main symbols
#include "PropBagImpl.h"
#include "RegistryUtil.h"
#include <foundation\string\StringEx.h>


namespace stingray {
namespace foundation {


/////////////////////////////////////////////////////////////////////////////
// CRegistryPropertyBag

class ATL_NO_VTABLE CRegistryPropertyBag : 
	public CComObjectRoot,
	public CComCoClass<CRegistryPropertyBag,&CLSID_RegistryPropertyBag>,
	public IDispatchImpl<CPropertyBagImpl, &IID_IPersistenceStrategy, &LIBID_SflPropBags, 2, 0>, 
	public ISupportErrorInfo
{
public:
	CRegistryPropertyBag(): m_preg(0) {}

DECLARE_REGISTRY_RESOURCEID(IDR_REGISTRYPROPERTYBAG)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CRegistryPropertyBag)
	COM_INTERFACE_ENTRY2(IDispatch, IPersistenceStrategy)
	COM_INTERFACE_ENTRY(IPersistenceStrategy)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IPropertyBag2)
	COM_INTERFACE_ENTRY(IPropertyBag)
END_COM_MAP()

public:
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IPersistenceStrategy
	STDMETHOD(Init)(VARIANT varInitParam);
	STDMETHOD(Commit)();

// IPropertyBag2
	STDMETHOD(CountProperties)(ULONG * pcProperties);
	STDMETHOD(GetPropertyInfo)(ULONG iProperty, ULONG cProperties, PROPBAG2 * pPropBag, ULONG * pcProperties);

protected:
	// Instance data
	foundation::string m_sRootKey;
	foundation::CRegistry m_regBase;

	foundation::CRegistry* m_preg;

	// Overriden implementation operations
	virtual bool ReadProperty(const foundation::string& sPropName, _variant_t& vaValue);
	virtual void WriteBasicType(const foundation::string& sPropName, const _variant_t& vaValue);
	virtual void WriteObject(const foundation::string& sPropName, IUnknownPtr spUnk);
	virtual void WriteSafeArray(const foundation::string& sPropName, _variant_t& vaArray);

	// Implementation operations
	virtual bool ReadPropertyFromKey(const foundation::string& sPropName, _variant_t& vaValue, foundation::CRegistry& regKey);
	virtual void WriteObjectToKey(IUnknownPtr spUnk, foundation::CRegistry& regKey);
	virtual void WriteSafeArrayToKey(_variant_t& vaArray, foundation::CRegistry& regKey);
	virtual void WriteBasicTypeToKey(const foundation::string& sPropName, const _variant_t& vaValue, foundation::CRegistry& regKey);

	// Requirement: regKey must correspond to an object store.
	// No verification is done
	virtual void ReadObjectFromKey(foundation::CRegistry& regKey, IUnknownPtr& spNewObject);
	virtual void ReadSafeArrayFromKey(foundation::CRegistry& regKey, _variant_t& vaArray);
};


};	// namespace stingray::foundation
};	// namespace stingray