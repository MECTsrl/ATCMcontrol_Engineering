/////////////////////////////////////////////////////////////////////////////
// PropAccessor.h : Property accessor class.
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
// Description:  Property accessor class.
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Properties/PropAccessor.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

#pragma once

#ifndef __PROPACCESSOR_H__
#define __PROPACCESSOR_H__

#ifdef _MFC_VER
	#ifndef __AFXCONV_H__
	#include <afxconv.h>
	#endif //__AFXCONV_H__
#else
	#ifndef __ATLCONV_H__
	#include <atlconv.h>
	#endif //__ATLCONV_H__
#endif //_MFC_VER

#pragma warning (disable:4786)  // identifier truncated to 255 in debug info


namespace stingray {
namespace foundation {

/////////////////////////////////////////////////////////////////////////////
//@doc CPropertyAccessor
//
//@class CPropertyAccessor | A property accessor encapsulates storage and
// retrieval of a property.  Property accessors define a GetValue and
// PutValue method that is used by a property container to store and
// retrieve a property value. The property container associates each
// accessor object with a property ID. This class implements
// <mf CPropertyAccessor::GetValue> and <mf CPropertyAccessor::PutValue>
// by storing two function pointers - one for getting the value and one
// for storing the value. The signature of the get and put functions must
// match one of the signatures recognized by this class. This class defines
// a series of embedded typedef's to define the get and put function
// signatures that can be used. There is a constructor for each accessor
// function type supported to make it easy to construct a property
// accessor.
//
//@tcarg class | T | The class that the property accessor invokes
// the get and put methods on.
//
//@xref <c CPropertyContainer>
//

template <class T>
class CPropertyAccessor
{
// Embedded types
public:
	typedef T _SourceClass;

	enum AccessorType
	{
		UNDEFINED_ACCESSOR,
		VARIANT_ACCESSOR,
		CHAR_ACCESSOR,
		BYTE_ACCESSOR,
		SHORT_ACCESSOR,
		USHORT_ACCESSOR,
		INT_ACCESSOR,
		UINT_ACCESSOR,
		LONG_ACCESSOR,
		ULONG_ACCESSOR,
		FLOAT_ACCESSOR,
		DOUBLE_ACCESSOR,
		TSTR_ACCESSOR,
		BSTR_ACCESSOR
	};

	typedef VARIANT (T::*VARIANTGET)() const;
	typedef void (T::*VARIANTPUT)(const VARIANT& val);

	typedef CHAR (T::*CHARGET)() const;
	typedef void (T::*CHARPUT)(const CHAR val);

	typedef BYTE (T::*BYTEGET)() const;
	typedef void (T::*BYTEPUT)(const BYTE val);

	typedef SHORT (T::*SHORTGET)() const;
	typedef void (T::*SHORTPUT)(const SHORT val);

	typedef USHORT (T::*USHORTGET)() const;
	typedef void (T::*USHORTPUT)(const USHORT val);

	typedef INT (T::*INTGET)() const;
	typedef void (T::*INTPUT)(const INT val);

	typedef UINT (T::*UINTGET)() const;
	typedef void (T::*UINTPUT)(const UINT val);

	typedef LONG (T::*LONGGET)() const;
	typedef void (T::*LONGPUT)(const LONG val);

	typedef ULONG (T::*ULONGGET)() const;
	typedef void (T::*ULONGPUT)(const ULONG val);

	typedef FLOAT (T::*FLOATGET)() const;
	typedef void (T::*FLOATPUT)(const FLOAT val);

	typedef DOUBLE (T::*DOUBLEGET)() const;
	typedef void (T::*DOUBLEPUT)(const DOUBLE val);

	typedef LPCTSTR (T::*TSTRGET)() const;
	typedef void (T::*TSTRPUT)(LPCTSTR val);

	typedef void (T::*BSTRGET)(BSTR& val) const;
	typedef void (T::*BSTRPUT)(const OLECHAR* val);

// Constructors/destructor
public:

	//@cmember,mfunc
	// Construct a property accessor object.
	//@@syntax CPropertyAccessor()
	//@@syntax CPropertyAccessor(const CPropertyAccessor& src)
	//@@syntax CPropertyAccessor(BYTEGET pByteGet, BYTEPUT pBytePut)
	//@@syntax CPropertyAccessor(SHORTGET pShortGet, SHORTPUT pShortPut)
	//@@syntax CPropertyAccessor(USHORTGET pUShortGet, USHORTPUT pUShortPut)
	//@@syntax CPropertyAccessor(INTGET pIntGet, INTPUT pIntPut)
	//@@syntax CPropertyAccessor(UINTGET pUIntGet, UINTPUT pUIntPut)
	//@@syntax CPropertyAccessor(LONGGET pLongGet, LONGPUT pLongPut)
	//@@syntax CPropertyAccessor(ULONGGET pULongGet, ULONGPUT pULongPut)
	//@@syntax CPropertyAccessor(FLOATGET pFloatGet, FLOATPUT pFloatPut)
	//@@syntax CPropertyAccessor(DOUBLEGET pDoubleGet, DOUBLEPUT pDoublePut)
	//@@syntax CPropertyAccessor(TSTRGET pTStrGet, TSTRPUT pTStrPut)
	//@@syntax CPropertyAccessor(BSTRGET pBStrGet, BSTRPUT pBStrPut)
	//@@comm
	// This class provides overloaded constructors for each possible
	// get and put function signature. There is also a copy constructor
	// and a default constructor.
	//@@end
	/* Construct a property accessor object*/

	CPropertyAccessor()
	{
		m_type = UNDEFINED_ACCESSOR;
	}

	CPropertyAccessor(const CPropertyAccessor& src)
	{
		m_type = src.m_type;
		m_get = src.m_get;
		m_put = src.m_put;
	}

	CPropertyAccessor(CHARGET pCharGet, CHARPUT pCharPut)
	{
		m_type = CHAR_ACCESSOR;
		m_get.pChar = pCharGet;
		m_put.pChar = pCharPut;
	}

	CPropertyAccessor(BYTEGET pByteGet, BYTEPUT pBytePut)
	{
		m_type = BYTE_ACCESSOR;
		m_get.pByte = pByteGet;
		m_put.pByte = pBytePut;
	}

	CPropertyAccessor(SHORTGET pShortGet, SHORTPUT pShortPut)
	{
		m_type = SHORT_ACCESSOR;
		m_get.pShort = pShortGet;
		m_put.pShort = pShortPut;
	}

	CPropertyAccessor(USHORTGET pUShortGet, USHORTPUT pUShortPut)
	{
		m_type = USHORT_ACCESSOR;
		m_get.pUShort = pUShortGet;
		m_put.pUShort = pUShortPut;
	}

	CPropertyAccessor(INTGET pIntGet, INTPUT pIntPut)
	{
		m_type = INT_ACCESSOR;
		m_get.pInt = pIntGet;
		m_put.pInt = pIntPut;
	}

	CPropertyAccessor(UINTGET pUIntGet, UINTPUT pUIntPut)
	{
		m_type = UINT_ACCESSOR;
		m_get.pUInt = pUIntGet;
		m_put.pUInt = pUIntPut;
	}

	CPropertyAccessor(LONGGET pLongGet, LONGPUT pLongPut)
	{
		m_type = LONG_ACCESSOR;
		m_get.pLong = pLongGet;
		m_put.pLong = pLongPut;
	}

	CPropertyAccessor(ULONGGET pULongGet, ULONGPUT pULongPut)
	{
		m_type = ULONG_ACCESSOR;
		m_get.pULong = pULongGet;
		m_put.pULong = pULongPut;
	}

	CPropertyAccessor(FLOATGET pFloatGet, FLOATPUT pFloatPut)
	{
		m_type = FLOAT_ACCESSOR;
		m_get.pFloat = pFloatGet;
		m_put.pFloat = pFloatPut;
	}

	CPropertyAccessor(DOUBLEGET pDoubleGet, DOUBLEPUT pDoublePut)
	{
		m_type = DOUBLE_ACCESSOR;
		m_get.pDouble = pDoubleGet;
		m_put.pDouble = pDoublePut;
	}

	CPropertyAccessor(TSTRGET pTStrGet, TSTRPUT pTStrPut)
	{
		m_type = TSTR_ACCESSOR;
		m_get.pTStr = pTStrGet;
		m_put.pTStr = pTStrPut;
	}

	CPropertyAccessor(BSTRGET pBStrGet, BSTRPUT pBStrPut)
	{
		m_type = BSTR_ACCESSOR;
		m_get.pBStr = pBStrGet;
		m_put.pBStr = pBStrPut;
	}

// Attributes
protected:
	//@cmember
	/* Indicates the function signature of the get and put functions*/
	AccessorType m_type;

	//@cmember
	/* Pointer to the get function*/
	union
	{
		VARIANTGET pVariant;
		CHARGET pChar;
		BYTEGET pByte;
		SHORTGET pShort;
		USHORTGET pUShort;
		INTGET pInt;
		UINTGET pUInt;
		LONGGET pLong;
		ULONGGET pULong;
		FLOATGET pFloat;
		DOUBLEGET pDouble;
		TSTRGET pTStr;
		BSTRGET pBStr;
	} m_get;

	//@cmember
	/* Pointer to the put function*/
	union
	{
		VARIANTPUT pVariant;
		CHARPUT pChar;
		BYTEPUT pByte;
		SHORTPUT pShort;
		USHORTPUT pUShort;
		INTPUT pInt;
		UINTPUT pUInt;
		LONGPUT pLong;
		ULONGPUT pULong;
		FLOATPUT pFloat;
		DOUBLEPUT pDouble;
		TSTRPUT pTStr;
		BSTRPUT pBStr;
	} m_put;

// Operations
public:
	//@cmember,mfunc
	// Retrieve a property value.
	//@@rdesc void
	//@@parm Pointer to the object on which to invoke the get function on.
	//@@parm Output parameter to receive the property value.
	//@@comm
	// This function invokes the get function on the given source object
	// in order to retrieve the value.
	//@@end
	/* Retrieve a property value*/
	void GetValue(const _SourceClass* pObj, VARIANT& propVal) const
	{
		USES_CONVERSION;

		switch (m_type)
		{
		case CHAR_ACCESSOR:
			propVal.vt = VT_UI1;
			if (m_get.pChar != NULL)
			{
				propVal.bVal = (pObj->*m_get.pChar)();
			}
			break;

		case BYTE_ACCESSOR:
			propVal.vt = VT_UI1;
			if (m_get.pByte != NULL)
			{
				propVal.bVal = (pObj->*m_get.pByte)();
			}
			break;

		case SHORT_ACCESSOR:
			propVal.vt = VT_I2;
			if (m_get.pShort != NULL)
			{
				propVal.iVal = (pObj->*m_get.pShort)();
			}
			break;

		case USHORT_ACCESSOR:
			propVal.vt = VT_I2;
			if (m_get.pUShort != NULL)
			{
				propVal.iVal = (pObj->*m_get.pUShort)();
			}
			break;

		case INT_ACCESSOR:
			propVal.vt = VT_I4;
			if (m_get.pInt != NULL)
			{
				propVal.lVal = (pObj->*m_get.pInt)();
			}
			break;

		case UINT_ACCESSOR:
			propVal.vt = VT_I4;
			if (m_get.pUInt != NULL)
			{
				propVal.lVal = (pObj->*m_get.pUInt)();
			}
			break;

		case LONG_ACCESSOR:
			propVal.vt = VT_I4;
			if (m_get.pLong != NULL)
			{
				propVal.lVal = (pObj->*m_get.pLong)();
			}
			break;

		case ULONG_ACCESSOR:
			propVal.vt = VT_I4;
			if (m_get.pULong != NULL)
			{
				propVal.lVal = (pObj->*m_get.pULong)();
			}
			break;

		case FLOAT_ACCESSOR:
			propVal.vt = VT_R4;
			if (m_get.pFloat != NULL)
			{
				propVal.fltVal = (pObj->*m_get.pFloat)();
			}
			break;

		case DOUBLE_ACCESSOR:
			propVal.vt = VT_R8;
			if (m_get.pDouble != NULL)
			{
				propVal.dblVal = (pObj->*m_get.pDouble)();
			}
			break;

		case TSTR_ACCESSOR:
			propVal.vt = VT_BSTR;
			if (m_get.pTStr != NULL)
			{
				LPCTSTR lpszVal = (pObj->*m_get.pTStr)();
				propVal.bstrVal = ::SysAllocString(T2COLE(lpszVal));
			}
			break;

		case BSTR_ACCESSOR:
			propVal.vt = VT_BSTR;
			if (m_get.pBStr != NULL)
			{
				(pObj->*m_get.pBStr)(propVal.bstrVal);
			}
			break;
		}
	}

	//@cmember,mfunc
	// Store a property value.
	//@@rdesc void
	//@@parm Pointer to the object on which to invoke the put function on.
	//@@parm Input value used to set the property value.
	//@@comm
	// This function invokes the put function on the given source object
	// in order to store the value.
	//@@end
	/* Store a property value*/
	void PutValue(_SourceClass* pObj, const VARIANT& propVal)
	{
		USES_CONVERSION;

		switch (m_type)
		{
		case CHAR_ACCESSOR:
			if (m_put.pChar != NULL && propVal.vt == VT_UI1)
			{
				(pObj->*m_put.pChar)(propVal.bVal);
			}
			break;

		case BYTE_ACCESSOR:
			if (m_put.pByte != NULL && propVal.vt == VT_UI1)
			{
				(pObj->*m_put.pByte)(propVal.bVal);
			}
			break;

		case SHORT_ACCESSOR:
			if (m_put.pShort != NULL && propVal.vt == VT_I2)
			{
				(pObj->*m_put.pShort)(propVal.iVal);
			}
			break;

		case USHORT_ACCESSOR:
			if (m_put.pUShort != NULL && propVal.vt == VT_I2)
			{
				(pObj->*m_put.pUShort)(propVal.iVal);
			}
			break;

		case INT_ACCESSOR:
			if (m_put.pInt != NULL && propVal.vt == VT_I4)
			{
				(pObj->*m_put.pInt)(propVal.lVal);
			}
			break;

		case UINT_ACCESSOR:
			if (m_put.pUInt != NULL && propVal.vt == VT_I4)
			{
				(pObj->*m_put.pUInt)(propVal.lVal);
			}
			break;

		case LONG_ACCESSOR:
			if (m_put.pLong != NULL && propVal.vt == VT_I4)
			{
				(pObj->*m_put.pLong)(propVal.lVal);
			}
			break;

		case ULONG_ACCESSOR:
			if (m_put.pULong != NULL && propVal.vt == VT_I4)
			{
				(pObj->*m_put.pULong)(propVal.lVal);
			}
			break;

		case FLOAT_ACCESSOR:
			if (m_put.pFloat != NULL && propVal.vt == VT_R4)
			{
				(pObj->*m_put.pFloat)(propVal.fltVal);
			}
			break;

		case DOUBLE_ACCESSOR:
			if (m_put.pDouble != NULL && propVal.vt == VT_R8)
			{
				(pObj->*m_put.pDouble)(propVal.dblVal);
			}
			break;

		case TSTR_ACCESSOR:
			if (m_put.pTStr != NULL && propVal.vt == VT_BSTR)
			{
				(pObj->*m_put.pTStr)(OLE2CT(propVal.bstrVal));
			}
			break;

		case BSTR_ACCESSOR:
			if (m_put.pBStr != NULL && propVal.vt == VT_BSTR)
			{
				(pObj->*m_put.pBStr)(propVal.bstrVal);
			}
			break;
		}
	}
};

/////////////////////////////////////////////////////////////////////////////
//@doc CPropertyNoopAccessor
//
//@class CPropertyNoopAccessor | This class implements a property accessor
// that does nothing. It is used as a parameter for the <c CPropertyContainer>
// when property accessors are not used by the container to store and retrieve
// property values.

class CPropertyNoopAccessor
{
// Embedded types
public:
	typedef void _SourceClass;

	//@cmember,mfunc
	// This function does nothing.
	//@@rdesc void
	//@@end
	/* This function does nothing*/
	void GetValue(const _SourceClass* pObj, VARIANT& propVal) const
	{
		pObj;
		propVal;
	}

	//@cmember,mfunc
	// This function does nothing.
	//@@rdesc void
	//@@end
	/* This function does nothing*/
	void PutValue(_SourceClass* pObj, const VARIANT& propVal)
	{
		pObj;
		propVal;
	}
};


}; // namespace stingray::foundation
}; // namespace stingray

#endif  // #ifndef __PROPACCESSOR_H__
