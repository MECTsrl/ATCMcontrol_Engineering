/////////////////////////////////////////////////////////////////////////////
// MVCPersist.h : Persistence support for MVC architecture 
//
// Stingray Software Foundation Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//

#pragma once

#ifndef __MVCPERSIST_H__
#define __MVCPERSIST_H__

#if defined(_SFL_ATL_SUPPORT)

#include <foundation\util\sflcom.h>
#include <foundation\MVC\MvcModel.h>
#include <foundation\persistence\XMLPropBag.h>


namespace stingray {
namespace foundation {


//////////////////////////////////////////
// CMvcPersistableModel

template <typename _Derived, typename _Model = CMvcModel>
class CMvcPersistableModel : 
	public _Model
{
public:

	virtual bool Load(LPCTSTR lpszFileName) = 0;
	virtual bool Save(LPCTSTR lpszFileName) = 0;
};



//////////////////////////////////////////
// CMvcPropertyBagPersistableModel

template <typename _Derived, typename _Model = CMvcModel>
class CMvcPropertyBagPersistableModel :
	public CMvcPersistableModel<_Derived, _Model>,
	public CComObjectRootEx<CComSingleThreadModel>,
	public IPersistPropertyBag
{
	typedef CMvcPropertyBagPersistableModel<_Derived, _Model>	_thisClass;

public:

	BEGIN_COM_MAP(_thisClass)
		COM_INTERFACE_ENTRY(IPersistPropertyBag)
		COM_INTERFACE_ENTRY2(IPersist, IPersistPropertyBag)
	END_COM_MAP_NO_PURE()
	ATL_IUNKNOWN_IMPL_NODESTROY()

public:

	// IPersistPropertyBag implementation

	virtual HRESULT STDMETHODCALLTYPE GetClassID(CLSID* pClassID)
	{
		*pClassID = __uuidof(_Derived);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE InitNew()
	{
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE Load(IPropertyBag* pPropBag, IErrorLog* /*pErrorLog*/)
	{
		HRESULT hr = S_OK;
		try {
			if (!ReadFromPropertyBag(pPropBag)) {
				hr = E_FAIL;
			}
		}
		catch (...) {
			hr = E_FAIL;
		}
		return hr;
	}

	virtual HRESULT STDMETHODCALLTYPE Save(IPropertyBag* pPropBag, BOOL /*fClearDirty*/, BOOL /*fSaveAllProperties*/)
	{
		HRESULT hr = S_OK;
		try {
			if (!WriteToPropertyBag(pPropBag)) {
				hr = E_FAIL;
			}
		}
		catch (...) {
			hr = E_FAIL;
		}
		return hr;
	}


protected:
	// Overridables: implement custom persistence functionality
	// in your derived class

	virtual bool WriteToPropertyBag (
		IPropertyBag* pPropBag
	) = 0;

	virtual bool ReadFromPropertyBag (
		IPropertyBag* pPropBag
	) = 0;

public:

	// CMvcPersistableModel overrides

	virtual bool Load (
		LPCTSTR lpszFileName
	)
	{
		// Create the property bag
		CComObjectCached<foundation::CXMLPropertyBag> propBag;
		propBag.AddRef();

		// Initialize the property bag on the give file 
		HRESULT hr = propBag.Init(CComVariant(lpszFileName));

		// Get the IUnknown interface of this model, 
		// as the property bag will communicate with it
		// excusively through COM interfaces
		IUnknown* punkModel;
		hr = QueryInterface(__uuidof(IUnknown), reinterpret_cast<void**>(&punkModel));
		if (FAILED(hr)) return false;

		// Load the model, using the special IPropertyBag2 method
		// LoadObject to avoid having the property bag dealing
		// with creation issues
		LPOLESTR lpszPropertyName = _Derived::GetModelPropertyString();
		hr = propBag.LoadObject(lpszPropertyName, 0, punkModel, NULL);
		if (FAILED(hr)) return false;
		hr = propBag.Commit();
		if (FAILED(hr)) return false;
		punkModel->Release();

		// Data has changed, update all observers
		UpdateAllObservers();
		return true;
	}


	virtual bool Save (
		LPCTSTR lpszFileName
	)
	{
		// Create the property bag
		CComObjectCached<foundation::CXMLPropertyBag> propBag;
		propBag.AddRef();

		// Initialize the property bag on the give file 
		HRESULT hr = propBag.Init(CComVariant(lpszFileName));

		// Get the IUnknown interface of this model, 
		// as the property bag will communicate with it
		// excusively through COM interfaces
		IUnknown* punkModel;
		hr = QueryInterface(__uuidof(IUnknown), reinterpret_cast<void**>(&punkModel));
		if (FAILED(hr)) return false;

		// Save the model
		LPOLESTR lpszPropertyName = _Derived::GetModelPropertyString();
		hr = propBag.Save(lpszPropertyName, punkModel);
		if (FAILED(hr)) return false;
		hr = propBag.Commit();
		if (FAILED(hr)) return false;
		punkModel->Release();
		return true;
	}


public:

	// Override this static function in the derived class
	// to provide a custom property name for your model
	static LPOLESTR GetModelPropertyString()
	{
		static OLECHAR* lpszPropertyName = OLESTR("MVCModel");
		return lpszPropertyName;
	}
};



};	// namespace stingray::foundation
};	// namespace stingray

#endif  // #if defined(_SFL_ATL_SUPPORT)

#endif  // #ifndef __MVCPERSIST_H__
