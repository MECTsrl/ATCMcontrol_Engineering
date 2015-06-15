/////////////////////////////////////////////////////////////////////////////
// SECiter.h: interface for the iterator classes.
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
// Author:       Justin Rudd
// Description:  Declaration of iterator classes
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Toolkit/seciter.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

#ifdef WIN32

#ifndef __SECITER_H__
#define __SECITER_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#ifndef __AFXTEMPL_H__
#include <afxtempl.h>
#endif

//
// SEC Extension DLL
// Initialize declaration context
//
#ifdef _SECDLL
	#undef AFXAPP_DATA
	#define AFXAPP_DATA AFXAPI_DATA
	#undef AFX_DATA
	#define AFX_DATA    SEC_DATAEXT
#endif //_SECDLL

// Constants

// Macros

// Types

// Forward Declarations

template<class TYPE>
class SECIterator
{
protected:
	SECIterator(){}
public:
	virtual ~SECIterator(){}
	virtual void First() = 0;
	virtual void Last() = 0;
	virtual void Next() = 0;
	virtual void Prev() = 0;
	virtual TYPE Item() = 0;
	virtual BOOL IsDone() const = 0;
};

template<class TYPE>
class SECIteratorPtr
{
public:
	//construction/destruction
	SECIteratorPtr()
		{ m_pIterator = NULL; }
	SECIteratorPtr( SECIterator<TYPE>* pIterator )
		{ m_pIterator = pIterator; }
	SECIteratorPtr( const SECIterator<TYPE>& source )
		{ *this = source; }
	~SECIteratorPtr()
		{ delete m_pIterator; } //safe to delete NULL pointer

	//normal operators...
	void operator=( SECIterator<TYPE>* pIterator )
		{
			delete m_pIterator;
			m_pIterator = pIterator;
		}
	const SECIteratorPtr& operator=( const SECIteratorPtr<TYPE>& source )
		{
			if( this != &source )
			{
				delete m_pIterator;
				m_pIterator = source.m_pIterator;
			}
			return *this;
		}
	SECIterator<TYPE>* operator->()
		{
			ASSERT(m_pIterator);
			return m_pIterator;
		}
	SECIterator<TYPE>& operator*()
		{
			ASSERT(m_pIterator);
			return *m_pIterator;
		}

	//type-cast operators...
	operator SECIterator<TYPE>*()
		{
			ASSERT(m_pIterator);
			return m_pIterator;
		}
	operator SECIterator<TYPE>&()
		{
			ASSERT(m_pIterator);
			return *m_pIterator;
		}

protected:
	SECIterator<TYPE>* m_pIterator;
};

namespace SECArrayIteratorTypes
{
	class IArrayInterface
	{
	protected:
		int m_iItem;
		
		IArrayInterface()
			{
				m_iItem = -1;
			}
		void FirstImp()
			{
				m_iItem = 0;
			}
		void LastImp( int iItem )
			{
				m_iItem = iItem;
			}
		void NextImp()
			{
				m_iItem++;
			}
		void PrevImp()
			{
				m_iItem--;
			}
	};

	template<class TYPE,class ARG_TYPE>
	class SECCArrayIterator : public SECIterator<TYPE>, private IArrayInterface
	{
	protected:
		CArray<TYPE,ARG_TYPE>& m_ar;
				
	public:
		SECCArrayIterator( CArray<TYPE,ARG_TYPE>& ar ) : m_ar(ar) 
			{

			}
		virtual ~SECCArrayIterator()
			{ 
		
			}

		virtual void First()
			{ 
				FirstImp();
			}
		virtual void Last()
			{ 
				LastImp( m_ar.GetSize() - 1 );
			}
		virtual void Next()
			{ 
				NextImp();
			}
		virtual void Prev()
			{ 
				PrevImp();
			}
		virtual BOOL IsDone() const
			{
				return BOOL( (m_iItem == m_ar.GetSize()) || (m_iItem < 0) );
			}

		virtual TYPE Item()
			{
				ASSERT(!IsDone());
				return m_ar[m_iItem];
			}
	};

	template<class BASE_CLASS,class TYPE>
	class SECCTypedPtrArrayIterator : public SECIterator<TYPE>, private IArrayInterface
	{
	protected:
		CTypedPtrArray<BASE_CLASS,TYPE>& m_ar;

	public:
		SECCTypedPtrArrayIterator( CTypedPtrArray<BASE_CLASS,TYPE>& ar) : m_ar(ar)
			{

			}
		virtual ~SECCTypedPtrArrayIterator()
			{	

			}

		virtual void First()
			{
				FirstImp();
			}
		virtual void Last()
			{
				LastImp(m_ar.GetSize()-1);
			}
		virtual void Next()
			{
				NextImp();
			}
		virtual void Prev()
			{
				PrevImp();
			}
		virtual BOOL IsDone() const
			{
				return BOOL( (m_iItem == m_ar.GetSize()) || (m_iItem < 0) );
			}
		virtual TYPE Item()
			{
				ASSERT(!IsDone());
				return m_ar[m_iItem];
			}
	};
}

namespace SECIteratorFactory
{
	template<class TYPE,class ARG_TYPE>
	SECIterator<TYPE>* CreateIterator( CArray<TYPE,ARG_TYPE>& ar )
		{
			return new SECArrayIteratorTypes::SECCArrayIterator<TYPE,ARG_TYPE>(ar);
		}

	template<class BASE_CLASS,class TYPE>
	SECIterator<TYPE>* CreateIterator( CTypedPtrArray<BASE_CLASS,TYPE>& ar )
		{
			return new SECArrayIteratorTypes::SECCTypedPtrArrayIterator<BASE_CLASS,TYPE>(ar);
		}
}

//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif //__SECITER_H__

#endif //WIN32