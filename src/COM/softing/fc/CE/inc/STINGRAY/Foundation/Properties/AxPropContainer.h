/////////////////////////////////////////////////////////////////////////////
// AxPropContainer.h : Implementation of IPropertyContainer interface for
// ActiveX controls.
//
// Stingray Software Extension Classes
// Copyright (C) 1999 Stingray Software Inc,
// All rights reserved.
//
// This source code is only intended as a supplement to
// the Objective Toolkit Class Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding
// the Objective Toolkit product.
//
// Authors:      Jeff Boenig
// Description:  Implementation of IPropertyContainer interface for
// ActiveX controls.
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Properties/AxPropContainer.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

#pragma once

#ifndef __AXPROPCONTAINER_H__
#define __AXPROPCONTAINER_H__

#pragma warning (disable:4786)  // identifier truncated to 255 in debug info

#include <vector>

#include <Foundation\Properties\PropIface.h>
#include <Foundation\Properties\Property.h>
#include <Foundation\Properties\PropContainer.h>
#include <Foundation\Properties\AxProperty.h>

namespace stingray {
namespace foundation {

/////////////////////////////////////////////////////////////////////////////
//@doc CAxPropertyContainer
//
//@class CAxPropertyContainer | This template class extends a property
// container with the ability to register properties in an ActiveX
// control.
//
//@tcarg class | _ContainerBase | Base container class that is extended
// by this template wrapper.
//
//@base public | _ContainerBase
//@xref <c CPropertyContainer>, <c IPropertyContainer>
//

template <class _ContainerBase = CPropertyContainer>
class CAxPropertyContainer : public _ContainerBase
{
// Embedded types
public:
	typedef std::vector<int> IdList;

// Constructors/destructor
public:

// Attributes
protected:
	IdList m_idarray;

// Operations
public:
	//@cmember,mfunc
	// Return a pointer to the ActiveX control.
	//@@rdesc IUnknown pointer to the ActiveX control associated with
	// this container.
	//@@comm
	// This is a pure virtual function, making this an abstract base class.
	// Derived class bind an ActiveX control to the container by implementing
	// this function.
	//@@end
	/* Return a pointer to the ActiveX control*/
	virtual IUnknown* GetAxControl() = 0;

	//@cmember,mfunc
	// Register the properties of the ActiveX control with the property container.
	//@@rdesc Returns the number of properties registered.
	//@@comm
	// This function calls <mf CAxPropertyContainer::GetAxControl> to get the
	// ActiveX control. Then it examines the properties in the ActiveX control
	// and adds them to the container's property map.
	//@@end
	/* Register the properties of the ActiveX control with the property container*/
	int RegisterAxProperties()
	{
		int nPropCount = 0;

		USES_CONVERSION;

		IUnknown *pObj = GetAxControl();
		_ASSERTE(pObj);
		CComQIPtr<IDispatch> spControlDisp = pObj;
		_ASSERTE(spControlDisp);
		pObj->Release();

		CComPtr<ITypeInfo> spTypeInfo;
		HRESULT hr = spControlDisp->GetTypeInfo(0, LOCALE_SYSTEM_DEFAULT, &spTypeInfo);

		if (FAILED(hr))
			return 0;

		TYPEATTR* pta;
		hr = spTypeInfo->GetTypeAttr(&pta);

		// if it's a dual interface (TKIND_INTERFACE), we have to get the type info for
		// the dispatch part.
		if (SUCCEEDED(hr) && (pta->typekind == TKIND_INTERFACE))
		{
			// Get the dual
			CComPtr<ITypeInfo> spInfoTemp;
			HREFTYPE hRef;
			HRESULT hr = spTypeInfo->GetRefTypeOfImplType(UINT_MAX, &hRef);
			if (FAILED(hr))
				return 0;

			hr = spTypeInfo->GetRefTypeInfo(hRef, &spInfoTemp);
			if (FAILED(hr))
				return 0;

			spTypeInfo->ReleaseTypeAttr(pta);
			spTypeInfo = spInfoTemp;
			hr = spTypeInfo->GetTypeAttr(&pta);
		}

		if (FAILED(hr))
			return 0;

		// sweep through looking for property put
		m_idarray.clear();
		for (int i = 0; i < pta->cFuncs; i++)
		{
			FUNCDESC* pfd;
			hr = spTypeInfo->GetFuncDesc(i, &pfd);
			if (SUCCEEDED(hr))
			{
				if (pfd->invkind & INVOKE_PROPERTYPUT)
					m_idarray.push_back(pfd->memid);
			}
		}

		CComQIPtr<ITypeInfo2> spTypeInfo2 = spTypeInfo;

		// now find prop get methods
		for (i = 0; i < pta->cFuncs; i++)
		{
			bool bPropCreated = false;

			FUNCDESC* pfd;
			hr = spTypeInfo->GetFuncDesc(i, &pfd);
			if (SUCCEEDED(hr))
			{
				if (IsBrowsable(pfd, spTypeInfo2))
				{
					switch (pfd->elemdescFunc.tdesc.vt)
					{
					case VT_EMPTY:
					case VT_NULL:
					case VT_I2:
					case VT_I4:
					case VT_R4:
					case VT_R8:
					case VT_CY:
					case VT_DATE:
					case VT_BSTR:
					case VT_ERROR:
					case VT_BOOL:
					case VT_VARIANT:
					case VT_DECIMAL:
					case VT_I1:
					case VT_UI1:
					case VT_UI2:
					case VT_UI4:
					case VT_INT:
					case VT_UINT:
						{
							// normal property
							bPropCreated = RegisterAxProperty(pfd, spTypeInfo, NULL);
							break;
						}
					case VT_USERDEFINED:
						{
							// enum or typedef (alias)
							CComPtr<ITypeInfo> spUserTypeInfo;
							HREFTYPE hrt = pfd->elemdescFunc.tdesc.hreftype;
							VARTYPE vt = VT_USERDEFINED;
							HRESULT hr2 = GetEnumTypeInfo(spTypeInfo, hrt, &spUserTypeInfo);
							if(FAILED(hr2))
								vt = GetUserDefinedType(spTypeInfo, hrt);
							bPropCreated = RegisterAxProperty(pfd, spTypeInfo, spUserTypeInfo);
							break;
						}
					case VT_PTR:
						{
							// IFontDisp* properties only
							CComBSTR bstrName;
							CComBSTR bstrDocString;
							HRESULT hr;
							hr = spTypeInfo->GetDocumentation(pfd->memid, &bstrName, &bstrDocString, NULL, NULL);
							
							TYPEDESC* ptdesc = pfd->elemdescFunc.tdesc.lptdesc;
							_ASSERTE(ptdesc);
							if(ptdesc->vt == VT_USERDEFINED)
							{
								CComPtr<ITypeInfo> spUserTypeInfo;
								HRESULT hr2 = GetEnumTypeInfo(spTypeInfo, ptdesc->hreftype, &spUserTypeInfo);
								if(SUCCEEDED(hr2) && spUserTypeInfo)
								{
									TYPEATTR *ptypa = NULL;
									if(SUCCEEDED(spUserTypeInfo->GetTypeAttr(&ptypa)))
									{
										// check the IID for IFontDisp
										if(::IsEqualIID(ptypa->guid, IID_IFontDisp))
											bPropCreated = RegisterAxProperty(pfd, spTypeInfo, spUserTypeInfo);
										spUserTypeInfo->ReleaseTypeAttr(ptypa);
									}
								}
							}
								
							break;
						}
					}
				}
				spTypeInfo->ReleaseFuncDesc(pfd);
			}

			if (bPropCreated)
			{
				nPropCount++;
			}
		}

		spTypeInfo->ReleaseTypeAttr(pta);

		return nPropCount;
	}

	//@cmember,mfunc
	// Get a property value using the property ID.
	//@@rdesc Returns true if property value found, otherwise false.
	//@@parm Property ID to retrieve value for
	//@@parm Output parameter to receive property value.
	//@@comm This function uses the IDispatch interface of the ActiveX
	// control associated with the container to retrieve the value of
	// the property from the control. If that fail, the function call
	// is delegated to the base class.
	//@@end
	/* Get a property value using the property ID*/
	virtual bool GetPropertyValue(const PropertyId propId, VARIANT& propVal)
	{
		CComQIPtr<IDispatch> spDisp(GetAxControl());
		_ASSERTE(spDisp);
		CComDispatchDriver dd(spDisp);
		HRESULT hr = dd.GetProperty(propId, &propVal);

		if (SUCCEEDED(hr))
		{
			return true;
		}

		return _ContainerBase::GetPropertyValue(propId, propVal);
	}

	//@cmember,mfunc
	// Set a property value using the property ID.
	//@@rdesc Returns true if the property value for the given property ID is found and
	// the value successfully assigned, otherwise it returns false.
	//@@parm Property ID to store.
	//@@parm Value to store in property.
	//@@comm This function uses the IDispatch interface of the ActiveX
	// control associated with the container to store the value of
	// the property from the control. If that fail, the function call
	// is delegated to the base class.
	//@@end
	/* Set a property value using the property ID*/
	virtual bool PutPropertyValue(const PropertyId propId, const VARIANT& propVal)
	{
		CComQIPtr<IDispatch> spDisp(GetAxControl());
		_ASSERTE(spDisp);
		CComDispatchDriver dd(spDisp);
		HRESULT hr = dd.PutProperty(propId, const_cast<VARIANT*>(&propVal));

		if (SUCCEEDED(hr))
		{
			return true;
		}

		return _ContainerBase::PutPropertyValue(propId,propVal);
	}

// Implementation
protected:
	BOOL IsBrowsable(FUNCDESC* pfd, ITypeInfo2* pInfo2)
	{
		// don't show props with any of these flags
		static const WORD wExclude = FUNCFLAG_FRESTRICTED | FUNCFLAG_FHIDDEN | FUNCFLAG_FNONBROWSABLE;

		_ASSERTE(pfd);
		if ((pfd->invkind & INVOKE_PROPERTYGET) && ((pfd->wFuncFlags & wExclude) == 0) && pInfo2)
		{
			// this is not reliable
			//UINT uIndex;
			//HRESULT hr = pInfo2->GetFuncIndexOfMemId(pfd->memid, INVOKE_PROPERTYPUT, &uIndex);
			//return (BOOL)SUCCEEDED(hr);

			// so... brute force check for a matching propput
			IdList::iterator it;
			for(it = m_idarray.begin(); it != m_idarray.end(); it++)
			{
				if( pfd->memid == *it)
					return TRUE;
			}

		}
		
		return FALSE;
	}

	VARTYPE GetUserDefinedType(ITypeInfo *pTI, HREFTYPE hrt)
	{
		CComPtr<ITypeInfo> spTypeInfo;
		VARTYPE vt = VT_USERDEFINED;
		HRESULT hr = E_FAIL;
		hr = pTI->GetRefTypeInfo(hrt, &spTypeInfo);
		if(FAILED(hr))
			return vt;
		TYPEATTR *pta=NULL;

		spTypeInfo->GetTypeAttr(&pta);
		if(pta && pta->typekind == TKIND_ALIAS)
		{
			if (pta->tdescAlias.vt == VT_USERDEFINED)
				GetUserDefinedType(spTypeInfo,pta->tdescAlias.hreftype);
			else
				vt = pta->tdescAlias.vt;
		}

		if(pta)
			spTypeInfo->ReleaseTypeAttr(pta);
		return vt;

	}

	HRESULT GetEnumTypeInfo(ITypeInfo *pTI, HREFTYPE hrt, ITypeInfo** ppEnumInfo)
	{
		CComPtr<ITypeInfo> spTypeInfo;
		HRESULT hr = E_FAIL;
		hr = pTI->GetRefTypeInfo(hrt, &spTypeInfo);
		if(FAILED(hr))
			return hr;
		TYPEATTR *pta=NULL;

		spTypeInfo->GetTypeAttr(&pta);
		if(pta != NULL)
		{
			switch(pta->typekind)
			{
			case TKIND_ALIAS:
				{
					if (pta->tdescAlias.vt == VT_USERDEFINED)
						return GetEnumTypeInfo(spTypeInfo,pta->tdescAlias.hreftype, ppEnumInfo);
					else
						spTypeInfo.CopyTo(ppEnumInfo); // could be color
					break;
				}
			case TKIND_DISPATCH:
				{
					spTypeInfo.CopyTo(ppEnumInfo);
					break;
				}
			case TKIND_ENUM:
				{
					spTypeInfo.CopyTo(ppEnumInfo);
					break;
				}
			}

			spTypeInfo->ReleaseTypeAttr(pta);
		}
		return (*ppEnumInfo != NULL) ? S_OK : E_FAIL;
	}

	bool RegisterAxProperty(FUNCDESC* pfd, ITypeInfo* pTypeInfo, ITypeInfo* pUserTypeInfo)
	{
		bool bSuccess = false;

		IUnknown *pObj = GetAxControl();
		_ASSERTE(pObj);
		CComQIPtr<IDispatch> spControlDisp = pObj;
		_ASSERTE(spControlDisp);
		pObj->Release();

		_ASSERTE(pTypeInfo);
		_ASSERTE(pfd);
		// note: pUserTypeInfo can be NULL
		USES_CONVERSION;
		CComBSTR bstrName;
		CComBSTR bstrDocString;
		CAxProperty* pProp = NULL;

		HRESULT hr;
		// get property description string, if it has one
		hr = pTypeInfo->GetDocumentation(pfd->memid, &bstrName, &bstrDocString, NULL, NULL);
		if (SUCCEEDED(hr))
		{
			pProp = new CAxProperty;
			_ASSERTE(pProp);
			pProp->AddRef();

			hr = pProp->Initialize(spControlDisp, pfd->memid, bstrName, bstrDocString, pUserTypeInfo);
			if (SUCCEEDED(hr))
			{
				hr = _ContainerBase::RegisterProperty(pProp);
				bSuccess = SUCCEEDED(hr);
			}
		}

		if (pProp != NULL)
		{
			pProp->Release();
		}

		return bSuccess;
	}
};

}; // namespace stingray::foundation
}; // namespace stingray

#endif  // #ifndef __AXPROPCONTAINER_H__
