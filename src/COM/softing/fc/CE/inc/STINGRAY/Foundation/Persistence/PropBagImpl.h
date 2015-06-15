/////////////////////////////////////////////////////////////////////////////
// PropBagImpl.h: SFL Property bags
//
// Stingray Software Foundation Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Gilberto Araya
// Description:  Generic behavior of a PropertyBag implementation
//


#pragma once


#include <foundation\string\StringEx.h>
#include <comdef.h>


namespace stingray {
namespace foundation {


// Classes introduced due to the fact that IPropertyBag and IPropertyBag2 have methods
// with the same names, so a class cannot inherit from both of them directly or indirectly if
// those methods remain abstract

class IPropertyBag2_NoDupl: public IPropertyBag2
{
public:
	virtual HRESULT __stdcall Read(ULONG cProperties, PROPBAG2 * pPropBag, IErrorLog * pErrLog, VARIANT * pvarValue, HRESULT* phrError)
	{return PropBag2_Read(cProperties, pPropBag, pErrLog, pvarValue, phrError);}
	virtual HRESULT __stdcall Write(ULONG cProperties, PROPBAG2 * pPropBag, VARIANT * pvarValue)
	{return PropBag2_Write(cProperties, pPropBag, pvarValue);}

protected:
	virtual HRESULT __stdcall PropBag2_Read(ULONG cProperties, PROPBAG2 * pPropBag, IErrorLog * pErrLog, VARIANT * pvarValue, HRESULT* phrError) = 0;
	virtual HRESULT __stdcall PropBag2_Write(ULONG cProperties, PROPBAG2 * pPropBag, VARIANT * pvarValue) = 0;
};

class IPropertyBag_NoDupl: public IPropertyBag
{
public:
	virtual HRESULT __stdcall Read(LPCOLESTR pszPropName, VARIANT* pVar, IErrorLog* pErrorLog)
	{return PropBag_Read(pszPropName, pVar, pErrorLog);}
	virtual HRESULT __stdcall Write(LPCOLESTR pszPropName, VARIANT* pVar)
	{return PropBag_Write(pszPropName, pVar);}

protected:
	virtual HRESULT __stdcall PropBag_Read(LPCOLESTR pszPropName, VARIANT* pVar, IErrorLog* pErrorLog) = 0;
	virtual HRESULT __stdcall PropBag_Write(LPCOLESTR pszPropName, VARIANT* pVar) = 0;
};


class CPropertyBagImpl:
	public IPersistenceStrategy,
	public IPropertyBag2_NoDupl,
	public IPropertyBag_NoDupl
{
public:
	STDMETHOD(Save)(/*[in]*/BSTR bstrPropName, /*[in]*/ IUnknown* pPersistObject);
	STDMETHOD(Load)(/*[in]*/BSTR bstrPropName, /*[out]*/ IUnknown** pPersistObject);

	// IPropertyBag2
	STDMETHOD(LoadObject)(LPCOLESTR pstrName, DWORD dwHint, IUnknown * pUnkObject, IErrorLog * pErrLog);

protected:
	// IPropertyBag
	STDMETHOD(PropBag_Read)(LPCOLESTR pszPropName, VARIANT* pVar, IErrorLog* pErrorLog);
	STDMETHOD(PropBag_Write)(LPCOLESTR pszPropName, VARIANT* pVar);

	// IPropertyBag2
	STDMETHOD(PropBag2_Read)(ULONG cProperties, PROPBAG2 * pPropBag, IErrorLog * pErrLog, VARIANT * pvarValue, HRESULT* phrError);
	STDMETHOD(PropBag2_Write)(ULONG cProperties, PROPBAG2 * pPropBag, VARIANT * pvarValue);

protected:
	// Persistance implementation
	virtual bool ReadProperty(const foundation::string& sPropName, _variant_t& vaValue) = 0;
	virtual void WriteBasicType(const foundation::string& sPropName, const _variant_t& vaValue) = 0;
	virtual void WriteObject(const foundation::string& sPropName, IUnknownPtr spUnk) = 0;
	virtual void WriteSafeArray(const foundation::string& sPropName, _variant_t& vaArray) = 0;

	// Object serialization helpers
	virtual void LoadObject(const _bstr_t& bstrCLSID, IUnknownPtr& spUnk);
	virtual void LoadObject(IUnknownPtr& spUnk);
	virtual void SaveObject(const IUnknownPtr& spUnk, _bstr_t& bstrCLSID);
};


};	// namespace stingray::foundation
};	// namespace stingray


