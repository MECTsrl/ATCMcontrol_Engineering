/////////////////////////////////////////////////////////////////////////////
// PersistSTL.h : Persistence support for STL containers 
//
// Stingray Software Foundation Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//

#pragma once

#include <foundation\util\sflcom.h>


namespace stingray {
namespace foundation {


//////////////////////////////////////////
// Cleaner functors


// NoopCleaner: Do not perform any additional clean-up operation.
// Adequate for using with smart pointers.
// 
template <typename _Elem>
struct NoopCleaner
{
	void operator () (_Elem ) const
	{
		;	// Noop
	}
};


// ReleaseCleaner: Avoids the need for a smart pointer to be used
// in the container when storing reference counted objects
// 
template <typename _Elem>
struct ReleaseCleaner
{
	void operator () (_Elem elem) const
	{
		elem->Release();
	}
};



//////////////////////////////////////////
// Extractor functors
// Extract the element from a variant variable
// returned by the persist media

template <typename _COMInterface>
struct ExtractCOMInterface
{

	_COMInterface* operator() (const VARIANT& vaElem) const
	{
		if (vaElem.vt != VT_UNKNOWN) {
			return NULL;
		}
		if (!vaElem.punkVal) {
			return NULL;
		}
		_COMInterface* pCOMInterface;
		HRESULT hr = vaElem.punkVal->QueryInterface(__uuidof(_COMInterface), reinterpret_cast<void**>(&pCOMInterface));
		return pCOMInterface;
	}

};


template <typename _ElementType, typename _COMInterface = IUnknown>
struct ExtractCPPInterface
{
	_ElementType* operator() (const VARIANT& vaElem) const
	{
		if (vaElem.vt != VT_UNKNOWN) {
			return NULL;
		}
		if (!vaElem.punkVal) {
			return NULL;
		}

		_COMInterface* pCOMInterface;
		HRESULT hr = vaElem.punkVal->QueryInterface(__uuidof(_COMInterface), reinterpret_cast<void**>(&pCOMInterface));
		if (FAILED(hr)) return NULL;
		_ElementType* pElem = guid_cast<_ElementType*>(pCOMInterface);
		return pElem;
	}
};



//////////////////////////////////////////
// CPersistableContainer class
// Specializes STL containers providing a default implementation
// of the IPersistPropertyBag COM interface
//

template <typename _Container, const GUID* _CLSID, typename _Extractor, typename _Cleanup = ReleaseCleaner<_Container::value_type> >
class CPersistableContainer :
	public _Container,
	public CComObjectRootEx<CComSingleThreadModel>,
	public IPersistPropertyBag
{
public:

	typedef _Container ContainerType;
	typedef _Container::value_type ElementType;

	~CPersistableContainer()
	{
		_Cleanup cleanup;
		std::for_each(begin(), end(), cleanup);
	}

public:
	BEGIN_COM_MAP(CPersistableContainer)
		COM_INTERFACE_ENTRY(IPersistPropertyBag)
		COM_INTERFACE_ENTRY2(IPersist, IPersistPropertyBag)
	END_COM_MAP_NO_PURE()
	ATL_IUNKNOWN_IMPL_NODESTROY()


public:
	// IPersistPropertyBag implementation
	virtual HRESULT STDMETHODCALLTYPE GetClassID(CLSID* pClassID)
	{
		*pClassID = *_CLSID;
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE InitNew()
	{
		clear();
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE Load(IPropertyBag* pPropBag, IErrorLog* pErrorLog)
	{
		HRESULT hr = S_OK;
		_variant_t vaProp;
		
		clear();

		hr = pPropBag->Read(OLESTR("NumElements"), &vaProp, pErrorLog);
		if (FAILED(hr)) return E_FAIL;
		long nNumElements = static_cast<long>(vaProp);
		vaProp.Clear();

		for (long i = 0; i < nNumElements; ++i) {
			OLECHAR sPropName[24];
			// Retrieve persisted elements, one by one
			::wsprintfW(sPropName, OLESTR("Element%d"), i);
			vaProp.Clear();
			hr = pPropBag->Read(sPropName, &vaProp, pErrorLog);
			if (FAILED(hr)) return E_FAIL;

			ElementType elem = GetElementFromVariant(vaProp);
			insert(elem);
		}
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE Save(IPropertyBag* pPropBag, BOOL /*fClearDirty*/, BOOL /*fSaveAllProperties*/)
	{
		HRESULT hr = S_OK;
		_variant_t vaProp;
		vaProp = static_cast<long>(size());
		hr = pPropBag->Write(OLESTR("NumElements"), &vaProp);
		if (FAILED(hr)) return E_FAIL;

		iterator pos;
		int i;
		for (pos = begin(), i = 0; pos != end(); ++pos, ++i) {
			OLECHAR sPropName[32];
			// Persist all elements, one by one
			::wsprintfW(sPropName, OLESTR("Element%d"), i);
			vaProp.Clear();
			vaProp.vt = VT_UNKNOWN;
			(*pos)->QueryInterface(__uuidof(IUnknown), reinterpret_cast<void**>(&(vaProp.punkVal)));
			hr = pPropBag->Write(sPropName, &vaProp);
			if (FAILED(hr)) return E_FAIL;
		}

		return S_OK;
	}


protected:

	// Operations

	virtual ElementType GetElementFromVariant(const VARIANT& vaElem)
	{
		_Extractor extractor;
		return extractor(vaElem);
	}

};

};	// namespace stingray::foundation
};	// namespace stingray