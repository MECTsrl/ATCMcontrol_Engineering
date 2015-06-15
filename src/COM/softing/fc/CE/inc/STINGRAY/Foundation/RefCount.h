#ifndef __REFCOUNT_H__
#define __REFCOUNT_H__

namespace stingray {
namespace foundation {

/////////////////////////////////////////////////////////////////////////////
//@doc IRefCount
//
//@class IRefCount | This class provides methods for reference counting
// objects. It is used in order to do interface based programming in C++.
// The AddRef and Release methods it defines have the same signature as
// IUnknown.

class __declspec(uuid("8407B2B0-4B5E-11d3-AF1B-006008AFE059")) IRefCount
{
public:
	//@cmember,mfunc
	// Add a reference to this object.
	//@@rdesc New reference count value.
	//@@end
	/* Add a reference to this object*/
	virtual ULONG STDMETHODCALLTYPE AddRef() = 0;
	//@cmember,mfunc
	// Release a reference to this object.
	//@@rdesc New reference count value.
	//@@end
	/* Release a reference to this object*/
	virtual ULONG STDMETHODCALLTYPE Release() = 0;
};

/////////////////////////////////////////////////////////////////////////////
//@doc CRefCountImpl
//
//@mfunc | CRefCountImpl | CRefCountImpl | Default constructor sets reference
// count to zero.

//@doc CRefCountImpl
//@mfunc ULONG | CRefCountImpl | AddRef | Add a reference to this object.
//@rdesc New reference count value.

//@doc CRefCountImpl
//@mfunc ULONG | CRefCountImpl | Release | Release a reference to this object.
//@rdesc New reference count value.

//@class Template to implement reference counting for an interface. This class
// derives itself from the template parameter passed in and overrides AddRef
// and Release.

template<class base_t>
class CRefCountImpl : public base_t
{
public:

	//@cmember
	/* Constructor to set reference count to zero. */
	CRefCountImpl()
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

#define IRefCountImpl_T CRefCountImpl

template<class base_t>
class CNoRefCountImpl : public base_t
{
public:
	//@cmember
	/* Constructor to set reference count to zero. */
	ULONG STDMETHODCALLTYPE AddRef()
	{
		return 1L;
	}

	//@cmember
	/* Constructor to set reference count to zero. */
	ULONG STDMETHODCALLTYPE Release()
	{
		return 1L;
	}
};

};  // namespace stingray::foundation
};  // namespace stingray

#endif   // __REFCOUNT_H__
