/////////////////////////////////////////////////////////////////////////////
// CppIface.h : Definitions and templates for C++ interfaces.
//
// Stingray Software Extension Classes
// Copyright (C) 1997 Stingray Software Inc,
// All rights reserved.
//
// This source code is only intended as a supplement to
// the Objective Toolkit Class Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding
// the Objective Toolkit product.
//
// Author:       Jeff Boenig and Sam Robinson
// Description:  This module contains useful templates for C++ interfaces
//               based on IUnknown.
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/CppIface.h 1     28.02.07 18:58 Ln $

#ifndef __CPPIFACE_H__
#define __CPPIFACE_H__

#include <unknwn.h>
#include <assert.h>

#include "Foundation\GuidCast.h"
#include "Foundation\RefCount.h"

namespace stingray {
namespace foundation {

/////////////////////////////////////////////////////////////////////////////
// CIQueryGuidImpl
//
// CIUnknownImpl is derived from this and QueryInterface is implemented in
// terms if guid_cast. This is for backward compatibility.
template<class base, const IID *piid = &__uuidof(base)>
class CIQueryGuidImpl : public base
{
public:
	virtual bool QueryGuid(REFGUID guid, void **ppvObj)
	{
		*ppvObj = NULL;

		if (guid == *piid) {
			*ppvObj = static_cast<base*>(this);
		}

		return (*ppvObj != NULL);
	}
};

/////////////////////////////////////////////////////////////////////////////
// CIUnknownImpl

//@doc CIUnknownImpl
//@mfunc HRESULT | CIUnknownImpl | QueryInterface | Retrieve a pointer to
// an interface supported by this object.
//@parm REFIID | riid | Identifier of the interface being requested. 
//@parm void ** | ppvObj | Address of pointer variable that receives the
// interface pointer requested in riid.
//@rdesc S_OK if the interface is supported, E_NOINTERFACE if not.
//@comm Checks for IUnknown and the IID passed in as a template parameter.

//@class Implements QueryInterface for a given interface. Derives from
// CRefCountImpl in order to implement reference counting.

//@base pubic | CRefCountImpl

template<class base, const IID *piid = &__uuidof(base)>
class CIUnknownImpl : public CRefCountImpl<base>
{
public:

	//@cmember
	/* Retrieve a pointer to an interface supported by this object. */
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid,void **ppvObj)
	{
		/* Old Way
		*ppvObj = NULL;

		if (riid == *piid)
		{
			*ppvObj = static_cast<base *>(this);
		}
		else if (riid == IID_IUnknown)
		{
			*ppvObj = static_cast<base *>(this);
		}
		if (*ppvObj != NULL)
		{
			this->AddRef();
			return S_OK;
		}

		return E_NOINTERFACE;
		*/

		// New way, in terms of QueryGuid
		if(!QueryGuid(riid, ppvObj) && !QueryGuid(&IID_IUnknown, ppvObj))
		{
			return E_NOINTERFACE;
		}
		else
		{
			// ASSERT(*ppvObj)
			*ppvObj->AddRef();
			return S_OK;
		}
	}

	virtual bool QueryGuid(REFGUID guid, void **ppvObj)
	{
		*ppvObj = NULL;

		if (guid == *piid)
		{
			*ppvObj = static_cast<base*>(this);
		}

		return (*ppvObj != NULL);
	}
};

/////////////////////////////////////////////////////////////////////////////
// RefCountImpl_T

//@doc RefCountImpl_T
//@mfunc | RefCountImpl_T | RefCountImpl_T | Default constructor sets reference
// count to zero.

//@doc RefCountImpl_T
//@mfunc ULONG | RefCountImpl_T | AddRef | Add a reference to this object.
//@rdesc New reference count value.

//@doc RefCountImpl_T
//@mfunc ULONG | RefCountImpl_T | Release | Release a reference to this object.
//@rdesc New reference count value.

//@class Template to implement reference counting for an interface. This class
// derives itself from the template parameter passed in and overrides AddRef
// and Release.

template<class base_t>
class RefCountImpl_T : public base_t
{
public:

	//@cmember
	/* Constructor to set reference count to zero. */
	RefCountImpl_T()
	{
		m_ulRefCount = 0;
	}

	//@cmember
	/* Constructor to set reference count to zero. */
	ULONG STDMETHODCALLTYPE AddRef()
	{
		return ++m_ulRefCount;
	}

	//@cmember
	/* Constructor to set reference count to zero. */
	ULONG STDMETHODCALLTYPE Release()
	{
		ULONG ulRefCount = --m_ulRefCount;
		if (m_ulRefCount == 0)
			delete this;
		return ulRefCount;
	}

protected:
	//@cmember
	/* Stores reference count for object. */
	ULONG m_ulRefCount;
};

/////////////////////////////////////////////////////////////////////////////
// IUnknownImpl_T

//@doc IUnknownImpl_T
//@mfunc HRESULT | IUnknownImpl_T | QueryInterface | Retrieve a pointer to
// an interface supported by this object.
//@parm REFIID | riid | Identifier of the interface being requested. 
//@parm void ** | ppvObj | Address of pointer variable that receives the
// interface pointer requested in riid.
//@rdesc S_OK if the interface is supported, E_NOINTERFACE if not.
//@comm Checks for IUnknown and the IID passed in as a template parameter.

//@class Implements QueryInterface for a given interface. Derives from
// RefCountImpl_T in order to implement reference counting.

//@base pubic | RefCountImpl_T

template<class base_t, const IID *piid = &__uuidof(base_t)>
class IUnknownImpl_T : public RefCountImpl_T<base_t>
{
public:

	//@cmember
	/* Retrieve a pointer to an interface supported by this object. */
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid,void **ppvObj)
	{
		*ppvObj = NULL;

		if (riid == *piid)
		{
			*ppvObj = static_cast<base_t *>(this);
		}
		else if (riid == IID_IUnknown)
		{
			*ppvObj = static_cast<base_t *>(this);
		}

		if (*ppvObj != NULL)
		{
			this->AddRef();
			return S_OK;
		}

		return E_NOINTERFACE;
	}
};

/////////////////////////////////////////////////////////////////////////////
// IPtr
//
//@class
// IPtr | An IPtr encapsulates pointers to interfaces that support
// the IRefCount interface and automatically performs reference
// counting on the object.
//
//@tcarg class | element |
//  The class of objects that the interface pointer can reference. This
//  class must support the IRefCount interface.

// The default argument for piid assumes that the GUID being passed in was 
// declared using the form 
// struct __declspec(uuid("00000000-0000-0000-c000-000000000046")) IUnknown;
// You can refer to the VC include file comdef.h for more examples.
//   If you have not added this to your declaration, then you must supply a
// value for piid.

template <class T, const IID* piid = &__uuidof(T)> class IPtr
{
// Constructors/destructor
public:

	/////////////////////////////////////////////////////////////////////////
	// Default constructor taking no arguments. This will yield a null interface
	// pointer of type T that supports interface piid.
	IPtr()
	{
		m_p=0;
	}

	/////////////////////////////////////////////////////////////////////////
	// Constructor taking a pointer to an interface. This will call addref on
	// the interface and assign the pointer to the member pointer m_p. 
	IPtr(IUnknown* pIUnk)
	{
		void* pV;
		if (pIUnk != 0)
			if(S_OK != pIUnk->QueryInterface(*piid,&pV))
			{
				m_p = 0;
			}
			else
			{
				m_p = (T*)pIUnk;
			}
	}

	/////////////////////////////////////////////////////////////////////////
	// Constructor taking another smart interface pointer.
	IPtr(const IPtr<T,piid>& lp)
	{
		m_p = lp.m_p;
	}
	
	/////////////////////////////////////////////////////////////////////////
	// Destructor
	~IPtr()
	{
		if (m_p)
			m_p->Release();
	}

// Attributes
protected:
	/////////////////////////////////////////////////////////////////////////
	// Member pointer to an interface of type T
	T* m_p;

// Operations
public:

	/////////////////////////////////////////////////////////////////////////
	// Releases the member pointer by assigning it to a temp pointer, setting 
	// it to NULL (0 to avoid MFC dependance) and calling release on the temp 
	// pointer.
	void Release()
	{
		IUnknown* pTemp = m_p;
		if (pTemp)
		{
			m_p = 0;
			pTemp->Release();
		}
	}
	/////////////////////////////////////////////////////////////////////////
	// Overload of the identity operator returning a pointer of the type used
	// as the template argument. This allows the class reference to be used as
	// a pointer
	operator T*() const
	{
		return (T*)m_p;
	}


	/////////////////////////////////////////////////////////////////////////
	// Overload of the -> operator
	T* operator->() const
	{ 
		return m_p;
	}

	/////////////////////////////////////////////////////////////////////////
	// Overload of the * operator
	T& operator*() const
	{
		assert(m_p!=0); 
		return *m_p;
	}
	/////////////////////////////////////////////////////////////////////////
	// Overload of the & operator
	T** operator&()
	{
		assert(m_p==0);
		// The assert on operator& usually indicates a bug.  If this is really
		// what is needed, however, take the address of the m_p member explicitly.
		return &m_p;
	}

	/////////////////////////////////////////////////////////////////////////
	// Overload of the = operator
	IPtr<T,piid>& operator=(IUnknown* pIUnk)
	{	
		if( pIUnk != 0 )
		{
		void* pV;

			pIUnk->AddRef();
			if(S_OK != pIUnk->QueryInterface(*piid,&pV))
			{				
				m_p = 0;
				return *this;
			}

		}

		if( m_p != 0 )
		{
			m_p->Release();
		}
		m_p = (T*)pIUnk;

		return *this;
	}

	/////////////////////////////////////////////////////////////////////////
	// Overload of the = operator
	IPtr<T,piid>& operator=(const IPtr<T,piid>& lp)
	{
	
		if(S_OK != lp->m_p->QueryInterface(lp.iidPtrType))
		{
			m_p = 0;
			return 0;
		}
		return( *this = lp.m_p);
	}

	/////////////////////////////////////////////////////////////////////////
	// Overload of the ! operator
	bool operator!() const
	{
		return (m_p == 0);
	}

	/////////////////////////////////////////////////////////////////////////
	// Overload of the == operator
	bool operator==(T* pT) const
	{
		return m_p == pT;
	}

	/////////////////////////////////////////////////////////////////////////
	// Compare two objects for equivalence
	bool IsEqualObject(IUnknown* pOther)
	{
		if (m_p == 0 && pOther == 0)
			return true; // They are both NULL objects

		if (m_p == 0 || pOther == 0)
			return false; // One is NULL the other is not

		IPtr<IUnknown> punk1;
		IPtr<IUnknown> punk2;
		m_p->QueryInterface(IID_IUnknown, (void**)&punk1);
		pOther->QueryInterface(IID_IUnknown, (void**)&punk2);
		return punk1 == punk2;
	}

};

/////////////////////////////////////////////////////////////////////////////
// Mix_IUnknown_With_IRefCount

//@class This template class resolves the ambiguity that occurs when mixing
// IRefCount and IUnknown together in an interface.  It redeclares AddRef
// and Release so that they are not ambiguous.

template <class _Base>
class Mix_IUnknown_With_IRefCount : public _Base, public IUnknown
{
public:
	virtual ULONG STDMETHODCALLTYPE AddRef() = 0;
	virtual ULONG STDMETHODCALLTYPE Release() = 0;
};

/////////////////////////////////////////////////////////////////////////////
// Mix_IUnknown_With_IQueryGuid_IRefCount

//@class This template class resolves the ambiguity occurs when mixing
// IRefCount and IUnknown together in an interface.  It redeclares AddRef
// and Release so that they are not ambiguous.  It also provides an
// implementation of QueryGuid which calls QueryInterface.

template <class _Base>
class Mix_IUnknown_With_IQueryGuid_IRefCount : public _Base, public IUnknown
{
public:
	virtual bool QueryGuid(REFGUID guid, void **ppvObj)
	{
		HRESULT hr = QueryInterface(guid, ppvObj);
		return (hr == S_OK);
	}

	virtual ULONG STDMETHODCALLTYPE AddRef() = 0;
	virtual ULONG STDMETHODCALLTYPE Release() = 0;
};

};  // namespace stingray::foundation
};  // namespace stingray

#endif   // __CPPIFACE_H__
