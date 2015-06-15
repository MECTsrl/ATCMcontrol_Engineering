/////////////////////////////////////////////////////////////////////////////
// PropIFace.h : Property interfaces
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
// Description:  Property interfaces.
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Properties/PropIface.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

#pragma once

#ifndef __PROPIFACE_H__
#define __PROPIFACE_H__

#include <Foundation\GuidCast.h>
#include <Foundation\RefCount.h>

namespace stingray {
namespace foundation {

//@type PropertyId | Property identifier type.
//@iex typedef LONG PropertyId;
typedef LONG PropertyId;

// Property style flags
#define PROP_DIRECT      0x00000001
#define PROP_COLOR       0x00000002
#define PROP_FONT        0x00000004
#define PROP_ENUM        0x00000008
#define PROP_CONTAINER	 0x00000010

/////////////////////////////////////////////////////////////////////////////
//@doc IEnumeration
//
//@class IEnumeration | This interface provides access to an enumeration.
// An enumeration is a set of indexed values. Enumerated properties are
// associated with an enumeration and store their values as indices into
// the enumeration . The <c IProperty> interface provides the function
// <mf IProperty::GetEnumeration> to access the enumeration.
//
//@base public | IQueryGuid
//@base public | IRefCount

class IEnumeration : public IQueryGuid, public IRefCount
{
public:
	//@cmember,mfunc
	// Return the number of items in the enumeration.
	//@@rdesc Number of items in the enumeration.
	//@@end
	/* Return the number of items in the enumeration*/
	virtual int GetItemCount() const = 0;

	//@cmember,mfunc
	// Get the value of the given item in the enumeration.
	//@@rdesc void
	//@@parm Index of item to retrieve value of
	//@@parm VARIANT to receive the returned value
	//@@end
	/* Get the value of the given item in the enumeration*/
	virtual void GetItemValue(const int nItemIdx, VARIANT& val) const = 0;

	//@cmember,mfunc
	// Set the value of the given item in the enumeration.
	//@@rdesc void
	//@@parm Index of item in enumeration.
	//@@parm Input value.
	//@@end
	/* Set the value of the given item in the enumeration*/
	virtual void SetItemValue(const int nItemIdx, const VARIANT& val) = 0;

	//@cmember,mfunc
	// Get the description of the given item in the enumeration.
	//@@rdesc void
	//@@parm Index of item in enumeration.
	//@@parm Pointer to buffer in which to return description.
	//@@end
	/* Get the description of the given item in the enumeration*/
	virtual void GetItemDescription(const int nItemIdx, BSTR* bDesc) const = 0;

	//@cmember,mfunc
	// Set the description of the given item in the enumeration.
	//@@rdesc void
	//@@parm Index of item in enumeration.
	//@@parm Pointer to new description for the item.
	//@@end
	/* Set the description of the given item in the enumeration*/
	virtual void SetItemDescription(const int nItemIdx, const OLECHAR* pszDesc) = 0;

	//@cmember,mfunc
	// Add an item to the enumeration.
	//@@rdesc void
	//@@parm Value of item to add.
	//@@parm Description of item to add.
	//@@comm
	// This function appends an item to the enumeration. It is added
	// at the end of the list of items.
	//@@end
	/* Add an item to the enumeration. */
	virtual void AddItem(const VARIANT& val, const OLECHAR* pszDesc) = 0;
};

class __declspec(uuid("D5177CF6-9486-44ce-8A12-E1CA77A057B3")) IEnumeration;

/////////////////////////////////////////////////////////////////////////////
//@doc IProperty
//
//@class IProperty | This interface provides access to a property. All
// properties have a unique ID, name, description, type, and category.
// Properties can optionally be associated with an <c IEnumeration>,
// which makes them enumerated properties. The value of an enumerated
// property is an index into an array of values. This interface only
// provides access to the description of a property (i.e. the metadata).
// This interface does not provide access to the value of the property.
// The value of a property is always accessed through a property
// container <c IPropertyContainer>. Properties are registered with
// properties containers and are accessed through the <c IPropertyContainer>
// interface.
//
//@base public | IQueryGuid
//@base public | IRefCount

class IProperty : public IQueryGuid, public IRefCount
{
public:

	//@cmember,mfunc
	// Get the ID of the property.
	//@@rdesc A property ID
	//@@comm
	// Each property has an identifier that is unique within a container.
	// The property ID is a LONG, which also happens to be compatible with
	// COM property IDs (DISPID).
	//@@end
	/* Get the ID of the property*/
	virtual PropertyId GetId() const = 0;

	//@cmember,mfunc
	// Get the variant type of the property.
	//@@rdesc Data type for the property
	//@@comm
	// This function returns the data type of the property. Since
	// VARIANTs are used for accessing property values, the property
	// data type is described using a VARTYPE.
	//@@end
	/* Get the variant type of the property*/
	virtual VARTYPE GetVariantType() const = 0;

	//@cmember,mfunc
	// Get the style flags for the property.
	//@@rdesc DWORD containing style flags for the property
	//@@comm
	// Style flags are used to control how the property is displayed
	// and edited.
	//@@end
	/* Get the style flags for the property*/
	virtual DWORD GetStyleFlags() const = 0;

	//@cmember,mfunc
	// Get the name of the property.
	//@@rdesc void
	//@@parm String in which to return the name of the property.
	//@@end
	/* Get the name of the property*/
	virtual void GetName(BSTR& bName) const = 0;

	//@cmember,mfunc
	// Get the description of the property.
	//@@rdesc void
	//@@parm String in which to return the description of the property.
	//@@end
	/* Get the description of the property*/
	virtual void GetDescription(BSTR& bDesc) const = 0;

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
	virtual int GetCategory() const = 0;

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
	virtual IEnumeration* GetEnumeration() const = 0;
};

class __declspec(uuid("B1AFA482-093B-4532-93E5-9A64EE00D9EA")) IProperty;

/////////////////////////////////////////////////////////////////////////////
//@doc IPropertyContainer
//
//@class IPropertyContainer | This interface provides access to an object's
// properties. Objects that support properties implement this interface.
// This interfaces provides functions for retrieving the description or
// metadata of properties supported by an object. The description of a
// property is accessed through the <c IProperty> interface. The
// <mf IPropertyContainer::GetProperty>, <mf IPropertyContainer::GetPropertyAt>,
// and <mf IPropertyContainer::GetPropertyByName> functions return an
// <c IProperty> pointer for a given property.
//
// Property containers also provide access to the values of properties. Property
// values are returned as VARIANTs by the the <mf IPropertyContainer::GetPropertyValue>
// function. Property values are set using the
// <mf IPropertyContainer::PutPropertyValue> function. There are also methods
// for getting and putting a properties value as a string.
//
//@base public | IQueryGuid
//@base public | IRefCount

class IPropertyContainer : public IQueryGuid, public IRefCount
{
public:

	//@cmember,mfunc
	// Get the number of properties in the container.
	//@@rdesc Number of properties in container.
	//@@end
	/* Get the number of properties in the container*/
	virtual int GetPropertyCount() const = 0;

	//@cmember,mfunc
	// Get the property using an index into the container.
	//@@rdesc Pointer to property.
	//@@parm Zero-based index of property to retrieve.
	//@@comm
	// This function can be used in conjunction with the GetPropertyCount
	// method to iterate over all of the properties in the container.
	//@@end
	/* Get the property using an index into the container*/
	virtual IProperty* GetPropertyAt(const int nPropIdx) const = 0;

	//@cmember,mfunc
	// Retrieve a property by unique identifier.
	//@@rdesc Pointer to property.
	//@@parm Property identifier.
	//@@end
	/* Retrieve a property by unique identifier*/
	virtual IProperty* GetProperty(const PropertyId propId) const = 0;

	//@cmember,mfunc
	// Retrieve a property by name.
	//@@rdesc Pointer to property.
	//@@parm Name of property to retrieve.
	//@@comm
	// This function looks up the property by name and returns it.
	//@@end
	/* Retrieve a property by name*/
	virtual IProperty* GetPropertyByName(OLECHAR* propName) const = 0;

	//@cmember,mfunc
	// Return the string name of a property category.
	//@@rdesc Returns true if category name is found, otherwise false.
	//@@parm Identifier of category for which to retrieve name
	//@@parm Return value for category name
	//@@comm
	// Each property can be associated with a category. The <mf IProperty::GetCategory>
	// function determines which category (if any) a property is associated with.
	// It is the property container's responsibility to map the category ID to
	// a name.
	//@@end
	/* Return the string name of a property category. */
	virtual bool GetCategoryName(const int nCatId, BSTR& catName) const = 0;

	//@cmember,mfunc
	// Get a property value using the property ID.
	//@@rdesc Returns true if property value found, otherwise false.
	//@@parm Property ID to retrieve value for
	//@@parm Output parameter to receive property value.
	//@@end
	/* Get a property value using the property ID*/
	virtual bool GetPropertyValue(const PropertyId propId, VARIANT& propVal) const = 0;

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
	virtual bool GetPropertyContainer(const PropertyId propId, IPropertyContainer** container) const = 0;

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
	virtual bool GetPropertyString(const PropertyId propId, BSTR& propVal) const = 0;

	//@cmember,mfunc
	// Set a property value using the property ID.
	//@@rdesc Returns true if the property value for the given property ID is found and
	// the value successfully assigned, otherwise it returns false.
	//@@parm Property ID to store.
	//@@parm Value to store in property.
	//@@end
	/* Set a property value using the property ID*/
	virtual bool PutPropertyValue(const PropertyId propId, const VARIANT& propVal) = 0;

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
	virtual bool PutPropertyString(const PropertyId propId, const BSTR& propVal) = 0;
};

class __declspec(uuid("D671E94B-12DC-4369-926D-6502DD26D230")) IPropertyContainer;

}; // namespace stingray::foundation
}; // namespace stingray

#endif  // #ifndef __PROPIFACE_H__
