/////////////////////////////////////////////////////////////////////////////
// PropContainer.h : Implementation of IPropertyContainer interface
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
// Description:  Implementation of IPropertyContainer interface.
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Properties/PropContainer.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

#pragma once

#ifndef __PROPCONTAINER_H__
#define __PROPCONTAINER_H__

#ifndef _SFL_NO_PRAGMA_WARNINGS
	#if 1200 <= _MSC_VER
		#pragma warning (push)
	#endif
	#pragma warning (disable:4786)  // identifier truncated to 255 in debug info
#endif

#include <map>

#include <Foundation\RefCount.h>
#include <Foundation\Properties\PropIface.h>
#include <Foundation\Properties\PropAccessor.h>
#include <Foundation\Properties\Property.h>
#include <Foundation\String\SflString.h>

#define SFL_PROPERTY_MAP(T)  \
	virtual stingray::foundation::CPropertyContainer<_ContainerInterface, _PropertyAccessor>::Map& GetPropertyMap() const \
			{ \
				static stingray::foundation::CPropertyContainer<_ContainerInterface, _PropertyAccessor>::Map propMap; \
				return propMap; \
			}

namespace stingray {
namespace foundation {

const int CONVERSION_BUFFER_SIZE	= 256;
const TCHAR OD_HEX_FORMAT_STRING[]	= _T("0x%08X");


/////////////////////////////////////////////////////////////////////////////
//@doc CPropertyContainer
//
//@class CPropertyContainer | This class provides a default implementation
// of the <c IPropertyContainer> interface. This class stores an entry in
// map for each property. Each entry in the property map contains an
// <c IProperty> pointer and a pointer to a property accessor. The
// <mf CPropertyContainer::GetPropertyValue> and <mf CPropertyContainer::PutPropertyValue>
// methods are implemented by looking up the property in the map and using
// the accessor to either store or retrieve the value. Properties are
// registered with the property container using the
// <mf CPropertyContainer::RegisterProperty> method.
//
//@base public | CRefCountImpl

template <class _Interface = IPropertyContainer, 
		  class _Accessor = CPropertyNoopAccessor>
class CPropertyContainer : public CRefCountImpl<_Interface>
{
// Embedded types
public:
	
	typedef CPropertyContainer<_Interface,_Accessor>	_ThisClass;
	typedef _Accessor									_PropertyAccessor;
	typedef _Interface									_ContainerInterface;

	// The Map class implements a collection of property entries
	// for the property container.
	class Map
	{
	public:
		// The Entry class encapsulates an IProperty pointer and
		// an accessor. Entries are stored in the map.
		class Entry
		{
		public:
			IProperty* m_pProperty;
			_Accessor* m_pAccessor;

			Entry(IProperty* pProperty = NULL) :
				m_pProperty(pProperty),
				m_pAccessor(NULL)
			{
				if (m_pProperty != NULL)
					m_pProperty->AddRef();
			}

			Entry(IProperty* pProperty, const _Accessor& accessor) :
				m_pProperty(pProperty)
			{
				if (m_pProperty != NULL)
					m_pProperty->AddRef();
				m_pAccessor = new _Accessor(accessor);
			}

			Entry(const Entry& src)
			{
				m_pProperty = src.m_pProperty;
				if (m_pProperty != NULL)
					m_pProperty->AddRef();
				m_pAccessor = NULL;
				if (src.m_pAccessor != NULL)
					m_pAccessor = new _Accessor(*src.m_pAccessor);
			}

			~Entry()
			{
				delete m_pAccessor;
				if (m_pProperty != NULL)
					m_pProperty->Release();
			}

			Entry& operator=(const Entry& src)
			{
				if (src.m_pProperty != NULL)
					src.m_pProperty->AddRef();
				if (m_pProperty != NULL)
					m_pProperty->Release();
				m_pProperty = src.m_pProperty;
				if (m_pAccessor != NULL)
				{
					delete m_pAccessor;
					m_pAccessor = NULL;
				}
				if (src.m_pAccessor != NULL)
				{
					m_pAccessor = new _Accessor(*src.m_pAccessor);
				}
				return *this;
			}
		};

		~Map()
		{
			DeleteAll();
		}

		typedef	std::map<PropertyId, Entry > _PropMap;
		_PropMap m_map;

		
		// copies all properties from source map. accessors are
		// not copied.
		bool CopyProperties(const Map& sourceMap)
		{
			bool success = true;
			const count = sourceMap.GetNumEntries();
			for(int idx = 0; idx < count && success; ++idx)
			{
				success = AddEntry(sourceMap.GetPropertyAt(idx));
			}

			if (!success)
			{
				DeleteAll();
			}

			return success;
		}

		bool AddEntry(IProperty* pProp, const _Accessor& accessor)
		{
			if (pProp == NULL)
				return false;

			Entry entry(pProp, accessor);
			m_map[pProp->GetId()] = entry;

			return true;
		}

		bool AddEntry(IProperty* pProp)
		{
			if (pProp == NULL)
				return false;

			Entry entry(pProp);
			m_map[pProp->GetId()] = entry;

			return true;
		}

		int GetNumEntries() const
		{
			return static_cast<int>(m_map.size());
		}

		IProperty* GetProperty(const PropertyId id) const
		{
			_PropMap::const_iterator iEntry = m_map.find(id);
			if (iEntry != m_map.end())
				return ((*iEntry).second.m_pProperty);
			return NULL;
		}

		_Accessor* GetAccessor(const PropertyId id) const
		{
			_PropMap::const_iterator iEntry = m_map.find(id);
			if (iEntry != m_map.end())
				return ((*iEntry).second.m_pAccessor);
			return NULL;
		}

		IProperty* GetPropertyAt(const int nIdx) const
		{
			if (nIdx >= static_cast<int>(m_map.size()))
			{
				return NULL;
			}

			_PropMap::const_iterator iEntry = m_map.begin();

			for (int nCurIndex = 0; nCurIndex < nIdx; nCurIndex++)
			{
				iEntry++;
			}

			return (*iEntry).second.m_pProperty;
		}

		IProperty* GetPropertyByName(OLECHAR* propName) const
		{
			if (propName == NULL)
				return NULL;

			IProperty* pProp = NULL;

			_PropMap::const_iterator iEntry = m_map.begin();

			while (iEntry != m_map.end() && pProp == NULL)
			{
				IProperty* pCurProp = (*iEntry).second.m_pProperty;
				_ASSERTE(pCurProp != NULL);
				BSTR bCurName;
				pCurProp->GetName(bCurName);
				if (bCurName != NULL && (wcscmp(bCurName, propName) == 0))
				{
					pProp = pCurProp;
					::SysFreeString(bCurName);
					bCurName = NULL;
				}
				else
				{
					iEntry++;
					::SysFreeString(bCurName);
					bCurName = NULL;
				}
			}

			return pProp;
		}

		IProperty* GetAccessorAt(const int nIdx) const
		{
			if (nIdx >= m_map.size())
			{
				return NULL;
			}

			_PropMap::const_iterator iEntry = m_map.begin();

			for (int nCurIndex = 0; nCurIndex < nIdx; nCurIndex++)
			{
				iEntry++;
			}

			return (*iEntry).second.m_pAccessor;
		}

		void DeleteAll()
		{
			m_map.clear();
		}

	};

// Operations
public:
	SFL_PROPERTY_MAP(_ThisClass)

	//@cmember,mfunc
	// Register a property with the container.
	//@@syntax bool RegisterProperty(IProperty* pProp)
	//@@syntax bool RegisterProperty(IProperty* pProp, const _Accessor& accessor)
	//@@syntax bool RegisterProperty(const PropertyId id, LPCTSTR lpszName, LPCTSTR lpszDesc, const _Accessor& accessor = _Accessor(), VARTYPE type = VT_EMPTY, DWORD dwStyle = PROP_DIRECT, IEnumeration *pIEnumeration = NULL)
	//@@syntax bool RegisterProperty(const PropertyId id, UINT nIdName, UINT nIdDesc, const _Accessor& accessor = _Accessor(), VARTYPE type = VT_EMPTY, DWORD dwStyle = PROP_DIRECT, IEnumeration *pIEnumeration = NULL)
	//@@rdesc Returns true if property successfully registered, otherwise false.
	//@@comm
	// There are several different overloaded versions of this function. All of the
	// RegisterProperty functions wind up adding an entry to the property map.
	//@@end
	/* Register a property with the container*/

	bool RegisterProperty(IProperty* pProp)
	{
		bool bSuccess = false;

		if (pProp != NULL)
		{
			bSuccess = GetPropertyMap().AddEntry(pProp);
			bSuccess = true;
		}

		return bSuccess;
	}

	bool RegisterProperty(IProperty* pProp, const _Accessor& accessor)
	{
		bool bSuccess = false;

		if (pProp != NULL)
		{
			bSuccess = GetPropertyMap().AddEntry(pProp, accessor);
			bSuccess = true;
		}

		return bSuccess;
	}

	bool RegisterProperty(const PropertyId id,
	                      LPCTSTR lpszName,
	                      LPCTSTR lpszDesc,
	                      const _Accessor& accessor = _Accessor(),
						  VARTYPE type = VT_EMPTY,
						  DWORD dwStyle = PROP_DIRECT,
						  IEnumeration *pIEnumeration = NULL)
	{
		bool bSuccess = false;
	
		CProperty* pProp = new CProperty(id, lpszName, lpszDesc);		
		_ASSERTE(pProp != NULL);
		if (pProp != NULL)
		{
			pProp->AddRef();
			pProp->SetVariantType(type);
			pProp->SetEnumeration(pIEnumeration);
			pProp->SetStyleFlags(dwStyle);
			bSuccess = RegisterProperty(pProp, accessor);
			pProp->Release();
		}

		return bSuccess;
	}

	bool RegisterProperty(const PropertyId id,
	                      UINT nIdName,
	                      UINT nIdDesc,
						  const _Accessor& accessor = _Accessor(),
						  VARTYPE type = VT_EMPTY,
						  DWORD dwStyle = PROP_DIRECT,
						  IEnumeration *pIEnumeration = NULL)
	{
		// Property name and descriptions
		CString propName;
		CString propDesc;
		
		bool bSuccess = propName.LoadString(nIdName) != 0;
		
		if (bSuccess)
		{
			bSuccess = propDesc.LoadString(nIdDesc) != 0;
		}

		if (bSuccess)
		{
			CProperty* pProp = new CProperty(id, propName, propDesc);		
			_ASSERTE(pProp != NULL);
			if (pProp != NULL)
			{
				pProp->AddRef();
				pProp->SetVariantType(type);
				pProp->SetEnumeration(pIEnumeration);
				pProp->SetStyleFlags(dwStyle);
				bSuccess = RegisterProperty(pProp, accessor);
				pProp->Release();
			}
		}

		return bSuccess;
	}

	//@cmember,mfunc
	// Get the number of properties in the container.
	//@@rdesc Number of properties in container.
	//@@end
	/* Get the number of properties in the container*/
	virtual int GetPropertyCount() const
	{
		return GetPropertyMap().GetNumEntries();
	}

	//@cmember,mfunc
	// Get the property using an index into the container.
	//@@rdesc Pointer to property.
	//@@parm Zero-based index of property to retrieve.
	//@@comm
	// This function can be used in conjunction with the GetPropertyCount
	// method to iterate over all of the properties in the container.
	//@@end
	/* Get the property using an index into the container*/
	virtual IProperty* GetPropertyAt(const int nPropIdx) const
	{
		return GetPropertyMap().GetPropertyAt(nPropIdx);
	}

	//@cmember,mfunc
	// Retrieve a property by unique identifier.
	//@@rdesc Pointer to property.
	//@@parm Property identifier.
	//@@end
	/* Retrieve a property by unique identifier*/
	virtual IProperty* GetProperty(const PropertyId propId) const
	{
		return GetPropertyMap().GetProperty(propId);
	}

	//@cmember,mfunc
	// Retrieve a property by name.
	//@@rdesc Pointer to property.
	//@@parm Name of property to retrieve.
	//@@comm
	// This function looks up the property by name and returns it.
	//@@end
	/* Retrieve a property by name*/
	virtual IProperty* GetPropertyByName(OLECHAR* propName) const
	{
		return GetPropertyMap().GetPropertyByName(propName);
	}

	//@cmember,mfunc
	// Return the string name of a property category.
	//@@rdesc Returns true if category name is found, otherwise false.
	//@@parm Identifier of category for which to retrieve name
	//@@parm Return value for category name
	//@@comm
	// Each property can be associated with a category. The <mf IProperty::GetCategory>
	// function determines which category (if any) a property is associated with.
	// It is the property container's responsibility to map the category ID to
	// a name. This function is not implemented by this class. It must be implemented
	// by derived classes.
	//@@end
	/* Return the string name of a property category. */
	virtual bool GetCategoryName(const int nCatId, BSTR& catName) const
	{
		nCatId;
		catName;
		return false;
	}

	//@cmember,mfunc
	// Get a property value using the property ID.
	//@@rdesc Returns true if property value found, otherwise false.
	//@@parm Property ID to retrieve value for
	//@@parm Output parameter to receive property value.
	//@@end
	/* Get a property value using the property ID*/
	virtual bool GetPropertyValue(const PropertyId propId, VARIANT& propVal) const
	{
		bool bSuccess = false;

		_Accessor* pAccessor = GetPropertyMap().GetAccessor(propId);

		if (pAccessor != NULL)
		{
			pAccessor->GetValue(static_cast<const _Accessor::_SourceClass*>(this), propVal);
			bSuccess = true;
		}

		return bSuccess;
	}

	//@cmember,mfunc
	// Retrieve a pointer to a nested property container.
	//@@rdesc Returns true if the property ID passed in is a nested property, otherwise
	// false.
	//@@parm Property ID to retrieve as a nested property.
	//@@parm Output parameter to receive pointer to the nested property container.
	//@@comm
	// Some properties are compound or nested properties that contain properties of their
	// own. For example, a font can be described as a compound property consisting of
	// face name, font height, weight, bold, italic, etc. This function allows the caller
	// to retrieve a property value as an IPropertyContainer. In other words, the value
	// of a font is a pointer to another property container that can then be used to
	// retrieve the individual font properties. This function fails if the property ID
	// passed in does not correspond to a nested property.
	//@@end
	/* Retrieve a pointer to a nested property container*/
	virtual bool GetPropertyContainer(const PropertyId propId, IPropertyContainer** container) const
	{
		propId;
		container;
		return false;
	}

	//@cmember,mfunc
	// Get a property value as a string using the property ID.
	//@@rdesc Returns true if the property value for the given property ID is found and
	// successfully converted to a string, otherwise it returns false.
	//@@parm Property ID to retrieve.
	//@@parm Output parameter to receive the string value.
	//@@comm
	// This function is a convenience function. It saves the caller from having to
	// perform their own VARIANT to string conversions.
	//@@end
	/* Get a property value as a string using the property ID*/
	virtual bool GetPropertyString(const PropertyId propId, BSTR& propVal) const
	{
		_ASSERTE(propVal == NULL);

		IProperty* pProp = GetPropertyMap().GetProperty(propId);

		if (pProp == NULL)
			return false;

		bool bSuccess = false;

		VARIANT val;
		::VariantInit(&val);

		bSuccess = GetPropertyValue(propId, val);
		if (bSuccess)
		{
			TCHAR strValue[CONVERSION_BUFFER_SIZE];
			
			USES_CONVERSION;

			switch(pProp->GetStyleFlags())
			{
			case PROP_FONT:
				{
					break;
				}
			case PROP_ENUM:
				{
					IEnumeration *enumeration = pProp->GetEnumeration();
					if (enumeration)
					{
						//	Look for a matching value in the enumerations. 
						//	Return the description if a match is found.  
						try 
						{
							const int count = enumeration->GetItemCount();
							for (int i = 0; i<count; ++i)
							{
								_variant_t search;
								enumeration->GetItemValue(i, search);
								if (val.vt != search.vt)
								{
									HRESULT hr = ::VariantChangeType(&val, &val, 0, search.vt);
									if (FAILED(hr))
									{
										break;
									}
								}
								if (search == val)
								{
									enumeration->GetItemDescription(i, &propVal);
									bSuccess = true;
									break;
								}
							}
						}
						catch(_com_error &)
						{ 
							// _variant_t threw an exception
						}

						enumeration->Release();
					}

					break;
				}
			case PROP_COLOR:
				{
					_stprintf(strValue, OD_HEX_FORMAT_STRING, val.ulVal);
					propVal = T2BSTR(strValue);
					bSuccess = true;
					break;
				}
			case PROP_DIRECT:
				{
					// conversion using variant changetypeex is really only appropriate for simple variant types
					// asssume we won't get any udt or references. if so, fail.
					LCID lcid = ::GetThreadLocale();
					VARIANT dest;
					::VariantInit(&dest);
					
					HRESULT success = ::VariantChangeTypeEx(&dest, &val, lcid, 0, VT_BSTR);
					if(SUCCEEDED(success))
					{
						propVal = ::SysAllocString(dest.bstrVal);
						bSuccess = true;

						::VariantClear(&dest);
					}

					break;
				} 

			default:
				{
					break;
				}

			} // switch(pProp->GetStyleFlags())
		}

		::VariantClear(&val);

		return bSuccess;
	}

	//@cmember,mfunc
	// Set a property value using the property ID.
	//@@rdesc Returns true if the property value for the given property ID is found and
	// the value successfully assigned, otherwise it returns false.
	//@@parm Property ID to store.
	//@@parm Value to store in property.
	//@@end
	/* Set a property value using the property ID*/
	virtual bool PutPropertyValue(const PropertyId propId, const VARIANT& propVal)
	{
		bool bSuccess = false;

		_Accessor* pAccessor = GetPropertyMap().GetAccessor(propId);

		if (pAccessor != NULL)
		{
			pAccessor->PutValue(static_cast<_Accessor::_SourceClass*>(this), propVal);
			bSuccess = true;
		}

		return bSuccess;
	}

	//@cmember,mfunc
	// Set a property value as string using the property ID.
	//@@rdesc Returns true if the property value for the given property ID is found and
	// the string is successfully converted and assigned, otherwise it returns false.
	//@@parm Property ID to store.
	//@@parm String representation of value to store in property.
	//@@comm
	// This function is a convenience function. It saves the caller from having to
	// perform their own string to VARIANT conversions.
	//@@end
	/* Set a property value as string using the property ID*/
	virtual bool PutPropertyString(const PropertyId propId, const BSTR& propVal)
	{
		
		_ASSERTE(propVal == NULL);

		IProperty* pProp = GetPropertyMap().GetProperty(propId);

		if (pProp == NULL)
			return false;

		bool bSuccess = false;

		VARIANT val;
		::VariantInit(&val);

		bSuccess = GetPropertyValue(propId, val);
		if (bSuccess)
		{
			
			USES_CONVERSION;

			switch(pProp->GetStyleFlags())
			{
			case PROP_FONT:
				{
					break;
				}
			case PROP_ENUM:
				{		
					break;
				}
			case PROP_COLOR:
				{
					break;
				}
			case PROP_DIRECT:
				{
					// conversion using variant changetypeex is really only appropriate for simple variant types
					// asssume we won't get any udt or references. if so, fail.
					LCID lcid = ::GetThreadLocale();
					VARIANT newValue;
					::VariantInit(&newValue);
					newValue.vt = VT_BSTR;
					newValue.bstrVal = ::SysAllocString(propVal);
					
					// Cooerce the string representation to the appropriate type.
					HRESULT success = ::VariantChangeTypeEx(&newValue, &newValue, lcid, 0, pProp->GetVariantType());
					if(SUCCEEDED(success))
					{
						success = ::VariantCopy(&val, &newValue);
						if(SUCCEEDED(success))
						{
							// Finally, replace the original value.
							bSuccess = PutPropertyValue(propId, val);
						}

						::VariantClear(&newValue);
					}

					break;
				} 

			default:
				{
					break;
				}

			} // switch(pProp->GetStyleFlags())
		}

		::VariantClear(&val);

		return bSuccess;
	}

	BEGIN_GUID_MAP(_ThisClass)
		GUID_ENTRY(IPropertyContainer)
	END_GUID_MAP
};

}; // namespace stingray::foundation
}; // namespace stingray

#ifndef _SFL_NO_PRAGMA_WARNINGS
	#if 1200 <= _MSC_VER
		#pragma warning (pop)
	#endif
#endif

#endif  // #ifndef __PROPCONTAINER_H__
