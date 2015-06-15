/////////////////////////////////////////////////////////////////////////////
// AxProperty.h : ActiveX property class
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
// Description:  ActiveX property class
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Properties/AxProperty.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

#pragma once

#ifndef __AXPROPERTY_H__
#define __AXPROPERTY_H__

#include <comdef.h>

#include <Foundation\Properties\PropIface.h>
#include <Foundation\Properties\Enumeration.h>


namespace stingray {
namespace foundation {

/////////////////////////////////////////////////////////////////////////////
//@doc CAxProperty
//
//@class CAxProperty | This class implements the <c IProperty> for ActiveX
// control properties. Rather than make a copy of property data such as
// name and description, this class maintains a pointer to the ActiveX
// control and the dispatch ID for the property. It implements the IProperty
// interface by using IDispatch interface.
//
//@base public | IProperty

class CAxProperty  : public IProperty
{
// Embedded types
public:
	enum subType
	{
		stNone    = 0x00010000,  // no additional type info is provided
		stBool    = 0x00020000,  // the value is a True/False enum
		stUser    = 0x00040000,  // the value is a user defined enumeration
		stBrowse  = 0x00080000   // the value is an enumeration supporting IPerPropertyBrowse
	};

// Constructors/destructor
public:
	//@cmember,mfunc
	// Construct an ActiveX property
	//@@end
	/* Construct an ActiveX property*/
	CAxProperty()
	{
		m_ulRefCount = 0L;
		m_dispId = 0;
		m_pDisp = NULL;
		m_pTypeInfo = NULL;
		m_pEnumeration = NULL;
	}

	virtual ~CAxProperty()
	{
		if (m_pEnumeration != NULL)
			m_pEnumeration->Release();
	}

// Attributes
protected:
	//@cmember
	/* Reference count*/
	ULONG m_ulRefCount;
	//@cmember
	/* Dispatch ID of property*/
	DISPID m_dispId;
	//@cmember
	/* Pointer to dispatch interface of ActiveX control*/
	IDispatch* m_pDisp;				// dispatch of the control
	//@cmember
	/* type info of user defined type (can be NULL)*/
	ITypeInfo* m_pTypeInfo;			// 
	//@cmember
	/* Sub-type*/
	int m_subType;
	//@cmember
	/* Property name*/
	_bstr_t m_name;
	//@cmember
	/* Property description*/
	_bstr_t m_desc;
	//@cmember
	/* Pointer to enumerated values*/
	IEnumeration* m_pEnumeration;	

// Operations
public:
	//@cmember,mfunc
	// Initialize the property using ActiveX control information.
	//@@rdesc Returns true if successful, otherwise false.
	//@@parm IDispatch interface of ActiveX control.
	//@@parm Dispatch ID of property to initialize.
	//@@parm Name of property.
	//@@parm Property description
	//@@parm Optional pointer to type information
	//@@end
	/* Get the enumeration for the property*/
	bool Initialize(IDispatch* pDisp, DISPID di, BSTR bstrName, BSTR bstrDesc, ITypeInfo* pTypeInfo = NULL)
	{
		// ITypeInfo* p is only valid for user defined types, fonts, and  colors
		_ASSERTE(pDisp);

		m_dispId = di;
		m_name = bstrName;
		m_desc = bstrDesc;

		if (pDisp != NULL)
			pDisp->AddRef();
		if (m_pDisp != NULL)
			m_pDisp->Release();
		m_pDisp = pDisp;

		if (pTypeInfo != NULL)
			pTypeInfo->AddRef();
		if (m_pTypeInfo != NULL)
			m_pTypeInfo->Release();
		m_pTypeInfo = pTypeInfo;

		if (IsEnumerated())
		{
			m_subType = stUser;
			CreateEnumeration();
		}
		else if (IsBool())
		{
			m_subType = stBool;
		}
		else if (IsPerPropertyBrowse())
		{
			m_subType = stBrowse;
		}

		return true;
	}

	//@cmember,mfunc
	// Get the ID of the property.
	//@@rdesc A property ID
	//@@comm
	// Each property has an identifier that is unique within a container.
	// The property ID is a LONG, which also happens to be compatible with
	// COM property IDs (DISPID).
	//@@end
	/* Get the ID of the property*/
	virtual PropertyId GetId() const
	{
		return m_dispId;
	}

	//@cmember,mfunc
	// Get the variant type of the property.
	//@@rdesc Data type for the property
	//@@comm
	// This function returns the data type of the property. Since
	// VARIANTs are used for accessing property values, the property
	// data type is described using a VARTYPE.
	//@@end
	/* Get the variant type of the property*/
	virtual VARTYPE GetVariantType() const
	{
		_ASSERTE(m_pDisp);
		VARTYPE vt = VT_EMPTY;
		CComDispatchDriver dd(m_pDisp);
		CComVariant var;
		HRESULT hr = dd.GetProperty(m_dispId, &var);
		if (SUCCEEDED(hr))
		{
			vt = var.vt;
		}
		return vt;
	}

	//@cmember,mfunc
	// Get the style flags for the property.
	//@@rdesc DWORD containing style flags for the property
	//@@comm
	// Style flags are used to control how the property is displayed
	// and edited.
	//@@end
	/* Get the style flags for the property*/
	virtual DWORD GetStyleFlags() const
	{
		DWORD dwStyle = 0;

		if (IsEnumerated())
		{
			dwStyle = PROP_ENUM;
		}
		else if (IsBool())
		{
			dwStyle = PROP_ENUM;
		}
		else if (IsPerPropertyBrowse())
		{
			dwStyle = PROP_ENUM;
		}
		else if (IsColor())
		{
			dwStyle = PROP_COLOR;
		}
		else if (IsFont())
		{
			dwStyle = PROP_FONT;
		}
		else
		{
			dwStyle = PROP_DIRECT;
		}

		return dwStyle;
	}

	//@cmember,mfunc
	// Get the name of the property.
	//@@rdesc void
	//@@parm String in which to return the name of the property.
	//@@end
	/* Get the name of the property*/
	virtual void GetName(BSTR& bName) const
	{
		bName = m_name.copy();
	}

	//@cmember,mfunc
	// Get the description of the property.
	//@@rdesc void
	//@@parm String in which to return the description of the property.
	//@@end
	/* Get the description of the property*/
	virtual void GetDescription(BSTR& bDesc) const
	{
		bDesc = m_desc.copy();
	}

	//@cmember,mfunc
	// Get the category of the property.
	//@@rdesc Integer that identifiers the category to which the property
	// belongs or -1 if it doesn't belong to a category.
	//@@comm
	// Categories are used as a grouping mechanism for properties. The name
	// of a category is acquired by passing the category ID to the property
	// container.
	//@@end
	/* Get the category of the property*/
	virtual int GetCategory() const
	{
		return -1;
	}

	//@cmember,mfunc
	// Get the enumeration for the property.
	//@@rdesc Pointer to an enumeration.
	//@@comm
	// If the property is an enumerated property, this function returns a
	// pointer to the enumeration that describes the possible values for
	// the property. The value of an enumerated property is an index into
	// the enumeration. This function returns NULL for properties that are
	// not enumerated properties.
	//@@xref <c IEnumeration>
	//@@end
	/* Get the enumeration for the property*/
	virtual IEnumeration* GetEnumeration() const
	{
		if(m_pEnumeration)
			m_pEnumeration->AddRef();
		return m_pEnumeration;
	}

	//@cmember,mfunc
	// Determines if property is a color.
	//@@rdesc Returns true if property is a color, otherwise false
	//@@end
	/* Determines if property is a color*/
	bool IsColor() const
	{
		bool bRet;
		switch(GetId())
		{
		case DISPID_BACKCOLOR:
		case DISPID_FORECOLOR:
		case DISPID_BORDERCOLOR:
		case DISPID_FILLCOLOR:
			bRet = true;
			break;
		default:
			bRet = false;
		}

		if(!bRet)	// check for OLE_COLOR alias
			bRet = IsOleColorAlias();

		return bRet;
	}

	//@cmember,mfunc
	// Determines if property is a font.
	//@@rdesc Returns true if property is a font, otherwise false
	//@@end
	/* Determines if property is a font*/
	bool IsFont() const
	{
		bool bRet;
		switch(GetId())
		{
		case DISPID_FONT:
			bRet = true;
			break;
		default:
			bRet = false;
		}

		if(!bRet && m_pTypeInfo)
		{
			TYPEATTR *pta = NULL;
			if(SUCCEEDED(m_pTypeInfo->GetTypeAttr(&pta)))
			{
				// check the IID for IFontDisp
				if(::IsEqualIID(pta->guid, IID_IFontDisp))
					bRet = true;
				m_pTypeInfo->ReleaseTypeAttr(pta);
			}
		}

		return bRet;
	}

	//@cmember,mfunc
	// Determines if property is an enumeration.
	//@@rdesc Returns true if property is an enumeration, otherwise false
	//@@end
	/* Determines if property is an enumeration*/
	bool IsEnumerated() const
	{
		if(!m_pTypeInfo)
			return false;
		bool bRet = false;
		TYPEATTR *pta=NULL;
		m_pTypeInfo->GetTypeAttr(&pta);
		if(pta && pta->typekind == TKIND_ENUM)
			bRet = true;
		if(pta)
			m_pTypeInfo->ReleaseTypeAttr(pta);
		return bRet;
	}

	//@cmember,mfunc
	// Determines if property is a boolean.
	//@@rdesc Returns true if property is a boolean, otherwise false
	//@@end
	/* Determines if property is a boolean*/
	bool IsBool() const
	{
		return ((GetVariantType() == VT_BOOL) && !m_pTypeInfo);
	}

	//@cmember,mfunc
	// Determines if property is an OLE color alias.
	//@@rdesc Returns true if property is an OLE color alias, otherwise false
	//@@end
	/* Determines if property is an OLE color alias*/
	bool IsOleColorAlias() const
	{
		if(!m_pTypeInfo)
			return false;
		USES_CONVERSION;
		bool bRet = false;
		TYPEATTR* pta = NULL;
		m_pTypeInfo->GetTypeAttr(&pta);
		if(pta && pta->typekind == TKIND_ALIAS)
		{
			// check for OLE_COLOR
			CComPtr<ITypeLib> ptlib;
			UINT nIndex;
			HRESULT hr = m_pTypeInfo->GetContainingTypeLib(&ptlib, &nIndex);  
			if (FAILED(hr))  
			{   
				bRet = false;
			}
			else
			{
				CComBSTR bstrName;
				CComBSTR bstrDoc;
				hr = ptlib->GetDocumentation(nIndex, &bstrName, &bstrDoc, NULL, NULL);
				if(SUCCEEDED(hr))
				{
					USES_CONVERSION;
					TCHAR buf[256];
					lstrcpy(buf, OLE2T(bstrName));
					if(lstrcmp(_T("OLE_COLOR"), buf) == 0)
						bRet = true;
				}
					
			}
		}

		if (pta)
		{
			m_pTypeInfo->ReleaseTypeAttr(pta);
		}

		return bRet;
	}

	//@cmember,mfunc
	// Determines if the control supports per property browsing.
	//@@rdesc Returns true per property browsing supported, otherwise false.
	//@@end
	/* Determines if the control supports per property browsing*/
	bool IsPerPropertyBrowse() const
	{
		CALPOLESTR castr;
		CADWORD cadw;
		CComQIPtr<IPerPropertyBrowsing> spBrowse = m_pDisp;
		if(!spBrowse)
			return false;
		HRESULT hr = CAxPPBEnumeration::GetBrowseArrays(spBrowse, GetId(), &castr, &cadw);
		if(hr == S_OK)	// do not use SUCCEEDED. S_false is common and not usable.
		{
			CAxPPBEnumeration::ReleaseBrowseArrays(&castr, &cadw);
			return true;
		}
		return false;
	}

	// The GUID map implements the QueryGuid function
	BEGIN_GUID_MAP(CAxProperty)
		GUID_ENTRY(IProperty)
		GUID_ENTRY(IQueryGuid)
		GUID_ENTRY(IRefCount)
	END_GUID_MAP

	virtual ULONG STDMETHODCALLTYPE AddRef()
	{
		return ++m_ulRefCount;
	}

	virtual ULONG STDMETHODCALLTYPE Release()
	{
		ULONG ulRefCount = --m_ulRefCount;
		if (m_ulRefCount == 0)
			delete this;
		return ulRefCount;
	}

// Implementation
protected:
	void CreateEnumeration()
	{
		switch (m_subType)
		{
		case stBool:
			m_pEnumeration = new CBoolEnumeration;
			break;
		case stUser:
			m_pEnumeration = new CAxUserEnumeration(m_pTypeInfo);
			break;
		case stBrowse:
			CComQIPtr<IPerPropertyBrowsing> spBrowse = m_pDisp;
			m_pEnumeration = new CAxPPBEnumeration(spBrowse, GetId());
			break;
		}
	}
};


}; // namespace stingray::foundation
}; // namespace stingray

#endif  // #ifndef __AXPROPERTY_H__
