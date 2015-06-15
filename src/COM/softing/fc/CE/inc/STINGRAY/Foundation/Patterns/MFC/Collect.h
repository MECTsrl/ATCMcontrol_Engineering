/////////////////////////////////////////////////////////////////////////////
// Collect.h : Declaration of base collection classes
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
// Author:       Dean Hallman
// Description:  Declaration of base collection classes.
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Patterns/MFC/Collect.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $


#ifndef __SFL_SECCOLLECT_H__
#define __SFL_SECCOLLECT_H__

namespace stingray {
namespace foundation {

// define our own min/max macros. The min/max macros defined in Windef.h will
// be undefined if the preprocessor NOMINMAX is defined. NOMINMAX is required
// by Rogue Wave DBTools.
#ifndef mvcmax
#define mvcmax(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef mvcmin
#define mvcmin(a,b)            (((a) < (b)) ? (a) : (b))
#endif

// The Iterator_T template has const iterator support added, and one of 
// its data members is declared as mutable.  This causes problems for 
// compiler using MFC versions prior to 6.0.  For this reason, we have 
// defined two macros:  CMN_ITERATOR_CONST and CMN_ITERATOR_MUTABLE to 
// be used in the place of the const keyword and the mutable keyword.  
// In VC 4.x and 5, these macros map to white space.

#ifndef CMN_ITERATOR_CONST
	// If using VC 6, define this macro to const
	#if (_MFC_VER >= 0x0600)
	#define CMN_ITERATOR_CONST const
#else
	// else if using VC 4.x and 5, define the macro as white space
	#define CMN_ITERATOR_CONST
	#endif // _MFC_VER >= 0x0600
#endif // CMN_ITERATOR_CONST


#ifndef CMN_ITERATOR_MUTABLE
	// If using VC 6, define this macro to mutable
	#if (_MFC_VER >= 0x0600)
	#define CMN_ITERATOR_MUTABLE mutable
#else
	// else if using VC 4.x or 5, define the macro as white space
	#define CMN_ITERATOR_MUTABLE
	#endif // _MFC_VER >= 0x0600
#endif // CMN_ITERATOR_MUTABLE 

// Position Specification Class
// The way inwhich position is specified varies between integers
// and linked lists.  For this reason, we have a collection type
// independent way of specifying the position of an item in a
// collection.
class CCIterPos
{
public:
	CCIterPos() {
	};
	
	virtual ~CCIterPos() {
	};
};

template <class pos_t> class CCIterPos_T : public CCIterPos
{
public:
	pos_t m_value;

	CCIterPos_T<pos_t>() {
	}

	CCIterPos_T<pos_t>( pos_t pos ) {
		m_value = pos;
	}

	CCIterPos_T<pos_t>( const CCIterPos_T<pos_t> &src )
	{
		*this = src.m_value;
	}

	operator pos_t(void) const {
			return m_value;
	}

	CCIterPos_T<pos_t>& operator=( pos_t pos ) {
		m_value = pos;
		return *this;
	}

	CCIterPos_T<pos_t>& operator=( const CCIterPos_T<pos_t> &src ) {
		return( *this = src.m_value );
	}

	BOOL operator==( const CCIterPos_T<pos_t> &src )
	{
		return ( m_value == src.m_value );
	}

	BOOL operator==( pos_t pos )
	{
		return ( m_value == pos );
	}

	BOOL operator!=( const CCIterPos_T<pos_t> &src )
	{
		return ( m_value != src.m_value );
	}

	BOOL operator!=( pos_t pos )
	{
		return ( m_value != pos );
	}
};

//@doc ICollection_T
//@class A collection interface.  When this interface is mixed in and implemented
// within any type of collection class, iteration via Iterator_T is enabled.
//@tcarg class | element_t | The class of the element to access in the collection.
template <class element_t> class ICollection_T
{
public:

	// Iteration Support
	virtual CCIterPos* iCreateIteratorPosition() const = 0;
	virtual void iFirst(CCIterPos* pbp) const = 0;
	virtual void iNext(CCIterPos* pbp) const = 0;
	virtual void iPrev(CCIterPos* pbp) const = 0;
	virtual void iLast(CCIterPos* pbp) const = 0;
	virtual void iRemoveAt(CCIterPos* pbp) = 0;
	virtual element_t iGet(CCIterPos* pbp) const = 0;
	virtual element_t* iGetPtr(CCIterPos* pbp) const = 0;
	virtual element_t& iGetRef(CCIterPos* pbp) const = 0;
	virtual void iSet(CCIterPos* pbp, element_t elem) = 0;
	virtual element_t iInsert(CCIterPos* pbp, element_t elem) = 0;
	virtual BOOL iFindNext(CCIterPos* pbp, element_t elem) const = 0;
	virtual BOOL iFindPrev(CCIterPos* pbp, element_t elem) const = 0;

	// Queries
	virtual int GetSize() const = 0;
	virtual void RemoveAll() = 0;
	virtual BOOL iEOC(CCIterPos* pbp) const = 0;
};

//@doc IComposite_T
//@mfunc int | IComposite_T | GetChildCount | Return number of children in composite
//@xref <c IComposite_T>

//@doc IComposite_T
//@mfunc element_t | IComposite_T | GetChild | Return a pointer to a child given the collection index
//@xref <c IComposite_T>

//@doc IComposite_T
//@mfunc BOOL | IComposite_T | AddChild | Add a child to the end of the collection
//@xref <c IComposite_T>

//@doc IComposite_T
//@mfunc BOOL | IComposite_T | InsertChildAt | Insert a child into the composite at a given position
//@xref <c IComposite_T>

//@doc IComposite_T
//@mfunc BOOL | IComposite_T | RemoveChildAt | Remove a child at the specified position from the composite
//@xref <c IComposite_T>

//@doc IComposite_T
//@mfunc BOOL | IComposite_T | FindChild | Return the index of a given child in the composite
//@xref <c IComposite_T>

//@doc IComposite_T
//@class Template interface for classes that support composition.  For example, if you require
// a composite visual component, you can simply mix this interface into to your MvcVisualComponent
// derived class.
//@tcarg class | element_t | The class of the element to store in the collection.
//@tcarg class | pos_t | The class used for the iterator.
template <class element_t, class pos_t = int> class IComposite_T
{
public:
	//@cmember
	/* Return number of children in composite*/
	virtual int GetChildCount(BOOL bRecursive) const = 0;

	//@cmember
	/* Return a pointer to a child given the collection index*/
	virtual element_t GetChild(const pos_t pos) const = 0;

	//@cmember
	/* Add a child to the end of the collection*/
	virtual BOOL AddChild(element_t child) = 0;

	//@cmember
	/* Insert a child into the composite at a given position*/
	virtual BOOL InsertChildAt(element_t child, const pos_t pos) = 0;

	//@cmember
	/* Remove specfied child*/
	virtual BOOL RemoveChild(const element_t child) = 0;

	//@cmember
	/* Remove a child at the specified position from the composite*/
	virtual BOOL RemoveChildAt(const pos_t pos) = 0;

	//@cmember
	/* Return the index of a given child in the composite*/
	virtual BOOL FindChild(element_t child, pos_t& pos) const = 0;

	//@cmember
	/* Return the head position in sequence*/
	virtual pos_t GetHeadChildPos() const = 0;

	//@cmember
	/* Return the head position in sequence*/
	virtual pos_t GetTailChildPos() const = 0;

	//@cmember
	/* Return the next position in sequence*/
	virtual pos_t GetNextChildPos(pos_t posOld) const = 0;

	//@cmember
	/* Return the previous position in sequence*/
	virtual pos_t GetPrevChildPos(pos_t posOld) const = 0;

	//@cmember
	/* Set (or reset) this node's parent*/
	virtual void SetParent(element_t parent,BOOL bRemoveFromOldParent=TRUE) = 0;

	//@cmember
	/* Get this node's parent*/
	virtual element_t GetParent() const = 0;
};

template <class element_t> class IStack_T
{
public:
	virtual element_t Push(element_t elem) = 0;
	virtual element_t Peek() const = 0;
	virtual element_t Pop() = 0;
	virtual void SetMaxSize(int nSize) = 0;
};

//@doc Iterator_T
//@mfunc void | Iterator_T | First | Moves the iterator to the first element in the collection
//@xref <c Iterator_T>

//@doc Iterator_T
//@mfunc void | Iterator_T | Next | Moves the iterator to the next element in the collection
//@xref <c Iterator_T>

//@doc Iterator_T
//@mfunc void | Iterator_T | Last | Moves the iterator to the last element in the collection
//@xref <c Iterator_T>

//@doc Iterator_T
//@mfunc void | Iterator_T | Prev | Moves the iterator to the previous element in the collection
//@xref <c Iterator_T>

//@doc Iterator_T
//@mfunc element_t | Iterator_T | Get | Retrieves the element at the current iterator position
//@xref <c Iterator_T>

//@doc Iterator_T
//@mfunc element_t | Iterator_T | GetFirst | Moves the iterator to and retrieves the first element in the collection
//@xref <c Iterator_T>

//@doc Iterator_T
//@mfunc element_t | Iterator_T | GetNext | Moves the iterator to and retrieves the next element in the collection
//@xref <c Iterator_T>

//@doc Iterator_T
//@mfunc element_t | Iterator_T | GetLast | Moves the iterator to and retrieves the last element in the collection
//@xref <c Iterator_T>

//@doc Iterator_T
//@mfunc element_t | Iterator_T | GetPrev | Moves the iterator to and retrieves the previous element in the collection
//@xref <c Iterator_T>

//@doc Iterator_T
//@mfunc BOOL | Iterator_T | FindNext | Finds the next element equal to the specified after the current iterator position.  If found, TRUE is returned and the iterator is moved.
//@parm element_t | elem | The element to search for
//@xref <c Iterator_T>

//@doc Iterator_T
//@mfunc BOOL | Iterator_T | FindPrev | Finds the previous element equal to the specified before the current iterator position.  If found, TRUE is returned and the iterator is moved.
//@parm element_t | elem | The element to search for
//@xref <c Iterator_T>

//@doc Iterator_T
//@mfunc void | Iterator_T | Set | Sets the element within the collection at the current iterator position
//@parm element_t | elem | The element to jam into the collection
//@xref <c Iterator_T>

//@doc Iterator_T
//@mfunc void | Iterator_T | Remove | Removes the element within the collection at the current iterator position
//@xref <c Iterator_T>

//@doc Iterator_T
//@mfunc void | Iterator_T | Insert | Inserts the specified element into the collection at the current iterator position
//@parm element_t | elem | The element to jam into the collection
//@xref <c Iterator_T>

//@doc Iterator_T
//@mfunc BOOL | Iterator_T | EOC | Are we beyond the beginning or end of the collection
//@xref <c Iterator_T>

//@doc Iterator_T
//@mfunc BOOL | Iterator_T | Select | Test whether the current element should be selected or skipped during traversal
//@xref <c Iterator_T>

//@doc Iterator_T
//@mfunc element_t& | Iterator_T | operator* | Retrieve the element at the current iterator position
//@xref <c Iterator_T>

//@doc Iterator_T
//@mfunc Iterator_T& | Iterator_T | operator++ | Advance the iterator to the next element
//@xref <c Iterator_T>

//@doc Iterator_T
//@mfunc Iterator_T& | Iterator_T | operator-- | Move the iterator to the previous element
//@xref <c Iterator_T>

//@doc Iterator_T
//@class Template interface for an iterator.  The iterator class supports iteration
// over any collection that supports the ICollection_T interface.
//@tcarg class | element_t | The class of the element to retrieve as the iterator advances.
template <class element_t> class Iterator_T
{
protected:
	CMN_ITERATOR_MUTABLE CCIterPos* m_pPos;
	ICollection_T<element_t>* m_pCollection;
	CMN_ITERATOR_CONST ICollection_T<element_t>* m_pConstCollection;

public:
	Iterator_T<element_t>() {
		m_pPos = NULL;
		m_pCollection = NULL;
	};

	Iterator_T<element_t>(ICollection_T<element_t>* pCollection) {
		m_pPos = NULL;
		m_pCollection = NULL;
		Init(pCollection);
	};

#if (_MFC_VER >= 0x0600)
	Iterator_T<element_t>(CMN_ITERATOR_CONST ICollection_T<element_t>* pConstCollection) {
		m_pPos = NULL;
		m_pConstCollection = NULL;
		m_pCollection = NULL;
		Init(pConstCollection);
	};
#endif // _MFC_VER >= 0x0600

	virtual ~Iterator_T() {
		delete m_pPos;
	};

	virtual void Init(ICollection_T<element_t>* pCollection) {
		if (m_pCollection != pCollection) {
			m_pCollection = pCollection;
			m_pConstCollection = m_pCollection;
			ASSERT(m_pCollection);		// You must supply a collection!
			if (m_pPos)
				delete m_pPos;
			m_pPos = m_pCollection->iCreateIteratorPosition();
		}
		ASSERT(m_pCollection && m_pPos);
	};

#if (_MFC_VER >= 0x0600)
	virtual void Init(CMN_ITERATOR_CONST ICollection_T<element_t>* pConstCollection) {
		if (m_pConstCollection != pConstCollection) {
			m_pConstCollection = pConstCollection;
			ASSERT(m_pConstCollection);		// You must supply a collection!
			if (m_pPos)
				delete m_pPos;
			m_pPos = m_pConstCollection->iCreateIteratorPosition();
		}
		ASSERT(m_pConstCollection && m_pPos);
	};
#endif // _MFC_VER >= 0x0600

	//@cmember
	/* Tests whether the current element should be selected or skipped during traversal */
	virtual BOOL Select() CMN_ITERATOR_CONST {
		return TRUE;
	};

	//@cmember
	/* Moves the iterator to the first element in the collection*/
	virtual void First() CMN_ITERATOR_CONST {
		m_pConstCollection->iFirst(m_pPos);
		if (!EOC() && !Select())
			Next();
	};

	//@cmember
	/* Moves the iterator to the next element in the collection*/
	virtual void Next() CMN_ITERATOR_CONST {
		do {
			m_pConstCollection->iNext(m_pPos);
		} while (!EOC() && !Select());
	};

	//@cmember
	/* Finds the next element equal to the specified after the current iterator position.  If found, TRUE is returned and the iterator is moved.*/
	virtual BOOL FindNext(element_t elem) CMN_ITERATOR_CONST {
		return m_pConstCollection->iFindNext(m_pPos, elem);
	};

	//@cmember
	/* Moves the iterator to the previous element in the collection*/
	virtual void Prev() CMN_ITERATOR_CONST {
		do {
			m_pConstCollection->iPrev(m_pPos);
		} while (!EOC() && !Select());
	};

	//@cmember
	/* Finds the previous element equal to the specified before the current iterator position.  If found, TRUE is returned and the iterator is moved.*/
	virtual BOOL FindPrev(element_t elem) CMN_ITERATOR_CONST {
		return m_pConstCollection->iFindPrev(m_pPos, elem);
	};

	//@cmember
	/* Moves the iterator to the last element in the collection*/
	virtual void Last() CMN_ITERATOR_CONST {
		m_pConstCollection->iLast(m_pPos);
		if (!EOC() && !Select())
			Prev();
	};

	//@cmember
	/* Retrieves a copy of the element at the current iterator position*/
	virtual element_t Get() CMN_ITERATOR_CONST {
		return m_pConstCollection->iGet(m_pPos);
	};

	/* Deprecated:  Retrieves a pointer to the element at the current iterator position*/
	virtual element_t* GetPtr() CMN_ITERATOR_CONST {
		return m_pConstCollection->iGetPtr(m_pPos);
	};

	/* Deprecated:  Retrieves a reference to the element at the current iterator position*/
	virtual element_t& GetRef() CMN_ITERATOR_CONST {
		return m_pConstCollection->iGetRef(m_pPos);
	};

	//@cmember
	/* Moves the iterator to and retrieves the first element in the collection*/
	inline element_t GetFirst() CMN_ITERATOR_CONST {
		First();
		return Get();
	};

	/* Deprecated:  Moves the iterator to the first element in the collection and returns a pointer to it*/
	inline element_t* GetFirstPtr()  CMN_ITERATOR_CONST {
		First();
		return GetPtr();
	};

	/* Deprecated:  Moves the iterator to the first element in the collection and returns a reference to it*/
	inline element_t& GetFirstRef() CMN_ITERATOR_CONST {
		First();
		return GetRef();
	};

	//@cmember
	/* Moves the iterator to and retrieves the next element in the collection*/
	virtual element_t GetNext() CMN_ITERATOR_CONST {
		Next();
		return Get();
	};

	/* Deprecated:  Moves the iterator to the next element in the collection and returns a pointer to it*/
	virtual element_t* GetNextPtr() CMN_ITERATOR_CONST {
		Next();
		return GetPtr();
	};

	/* Deprecated:  Moves the iterator to the next element in the collection and returns a reference to it*/
	virtual element_t& GetNextRef() CMN_ITERATOR_CONST {
		Next();
		return GetRef();
	};

	//@cmember
	/* Moves the iterator to and retrieves the previous element in the collection*/
	virtual element_t GetPrev() CMN_ITERATOR_CONST {
		Prev();
		return Get();
	};

	/* Deprecated:  Moves the iterator to the previous element in the collection and returns a pointer to it*/
	virtual element_t* GetPrevPtr() CMN_ITERATOR_CONST {
		Prev();
		return GetPtr();
	};

	/* Deprecated:  Moves the iterator to the previous element in the collection and returns a reference to it*/
	virtual element_t& GetPrevRef() CMN_ITERATOR_CONST {
		Prev();
		return GetRef();
	};

	//@cmember
	/* Moves the iterator to and retrieves the last element in the collection*/
	virtual element_t GetLast() CMN_ITERATOR_CONST {
		Last();
		return Get();
	};

	/* Deprecated:  Moves the iterator to and retrieves the last element in the collection*/
	virtual element_t* GetLastPtr() CMN_ITERATOR_CONST {
		Last();
		return GetPtr();
	};

	/* Deprecated:  Moves the iterator to and retrieves the last element in the collection*/
	virtual element_t& GetLastRef() CMN_ITERATOR_CONST {
		Last();
		return GetRef();
	};

	//@cmember
	/* Sets the element within the collection at the current iterator position*/
	virtual void Set(element_t elem) {
		m_pCollection->iSet(m_pPos, elem);
	};

	//@cmember
	/* Removes the element within the collection at the current iterator position*/
	virtual void Remove() {
		m_pCollection->iRemoveAt(m_pPos);
	};

	//@cmember
	/* Inserts the specified element into the collection at the current iterator position*/
	virtual element_t Insert(element_t elem) {
		return m_pCollection->iInsert(m_pPos, elem);
	};

	//@cmember
	/* Are we beyond the beginning or end of the collection */
	virtual BOOL EOC() CMN_ITERATOR_CONST {
		return m_pConstCollection->iEOC(m_pPos);
	};


#if (_MFC_VER >= 0x0600)
	//@cmember
	/* Advance iterator to next element */
	CMN_ITERATOR_CONST Iterator_T& operator++() CMN_ITERATOR_CONST {
		Next();
		return (*this); 
	};
#endif // _MFC_VER >= 0x0600


	Iterator_T& operator++() {
		Next();
		return (*this); 
	};

#if (_MFC_VER >= 0x0600)
	//@cmember
	/* Move iterator to previous element */
	CMN_ITERATOR_CONST Iterator_T& operator--() CMN_ITERATOR_CONST {
		Prev();
		return (*this); 
	};
#endif // _MFC_VER >= 0x0600

	Iterator_T& operator--() {
		Prev();
		return (*this); 
	};

#if (_MFC_VER >= 0x0600)

	//@cmember
	/* Get the element at the current iterator position */
	element_t& operator*(){
		return Get();
	};

	//@cmember
	/* Get the element at the current iterator position */
	CMN_ITERATOR_CONST element_t& operator*() CMN_ITERATOR_CONST {
		return Get();
	};
#endif // _MFC_VER >= 0x0600

};

};  // namespace stingray::foundation
};  // namespace stingray

#if defined(_COLLECT_STL)
#include "CollectStl.h"
#else
#include "CollectMfc.h"
#endif

namespace stingray {
namespace foundation {

//@doc MvcSet_T
//@class A Collection Classes set that supports the ICollection_T interface
// a therefore iteration with Iterator_T.  This class is equivalent to
// the CCArray_T class.
//@tcarg class | element_t | The class of the element to store in the set.
template <class element_t>
class MvcSet_T : public stingray::foundation::CCArray_T<element_t, element_t>
{
};

};  // namespace stingray::foundation
};  // namespace stingray

#endif // __SFL_SECCOLLECT_H__

