/////////////////////////////////////////////////////////////////////////////
// @doc SmartPtr
// @module SmartPtr.h | Pointer class that does automatic reference counting.
// 
// Objective Diagram<tm>
// <nl>Copyright <cp> 1997 Stingray Software, Inc. All rights reserved.
// 
// This source code is only intended as a supplement to the Objective 
// Diagram User's Guide and related electronic documentation provided with 
// the library. See these sources for detailed information regarding the 
// Objective Diagram product.
// 
// Author: Jeff Boenig
// <nl>Created: 01/98
// 
/////////////////////////////////////////////////////////////////////////////

#ifndef __SMARTPTR_H__
#define __SMARTPTR_H__

// MVC Extension DLL
// Initialize declaration context
#ifdef _SFLDLL
	#undef AFXAPP_DATA
	#define AFXAPP_DATA AFXAPI_DATA
	#undef AFX_DATA
	#define AFX_DATA SFL_DATAEXT
#endif //_SFLDLL

/////////////////////////////////////////////////////////////////////
// SmartPtr
//
//@class
// SmartPtr | A SmartPtr encapsulates pointers to objects that support
// the IRefCount interface and automatically performs reference
// counting on the object.
//
//@tcarg class | element_t |
//  The class of objects that the smart pointer can reference. This
//  class must support the IRefCount interface.

template <class T> class SmartPtr
{
	T *m_pObj;
public:
	SmartPtr()
	{
		m_pObj = 0;
	}

	SmartPtr( T *pObj )
	{
		if( ( m_pObj = pObj ) != 0 )
		{
			m_pObj->AddRef();
		}
	}

	SmartPtr( const SmartPtr &src )
	{
		m_pObj = 0;
		*this = src.m_pObj;
	}

	~SmartPtr(void)
	{
		if( m_pObj != 0 )
		{
			m_pObj->Release();
		}
	}

	operator T*(void) const { return m_pObj; }

	operator long(void) const { return (long)m_pObj; }

	T& operator*(void) { return *m_pObj; }

	T* operator->(void) const { return m_pObj; }

	SmartPtr& operator=( T* pObj )
	{
		if( pObj != 0 )
		{
			pObj->AddRef();
		}

		if( m_pObj != 0 )
		{
			m_pObj->Release();
		}

		m_pObj = pObj;

		return *this;
	}

	SmartPtr& operator=( const SmartPtr &src )
	{
		return( *this = src.m_pObj );
	}

	BOOL operator==( const SmartPtr &src )
	{
		return ( m_pObj == src.m_pObj );
	}

	BOOL operator==( T *pObj )
	{
		return ( m_pObj == pObj );
	}

	BOOL operator!=( const SmartPtr &src )
	{
		return ( m_pObj != src.m_pObj );
	}

	BOOL operator!=( T *pObj )
	{
		return ( m_pObj != pObj );
	}
};

/////////////////////////////////////////////////////////////////////////////

// MVC Extension DLL
// Reset declaration context
#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif // __SMARTPTR_H__

/////////////////////////////////////////////////////////////////////////////
