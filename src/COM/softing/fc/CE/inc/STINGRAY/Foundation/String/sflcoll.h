/////////////////////////////////////////////////////////////////////////////
// foundationcoll.h : MFC Lookalike collections
// implemented via the STL
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
// Authors:      Steve Danielson
// Description:  MFC Lookalike Collections
//
// Use very similar to the MFC Collections:
//
// using namespace foundation::stingray;
// ...
// CMap< ... >
//
// Note when using a position, you must do this now:
//   CMap< ... >::POSITION pos;
// instead of:
//   POSITION pos; // Old Way
//
// In MFC, POSITION is essentially a void*,
// here it wraps an STL iterator.
// Once you have the pos, use as normal:
//
// while(pos) ... or while(pos != NULL) ...
// 
// Note that Dump, Serialize, etc is missing
// and also and COb* collections
// 
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/String/sflcoll.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

#ifndef __SFLCOLL_H__
#define __SFLCOLL_H__

#ifndef _WINDOWS_
#include <windows.h>
#endif //_WINDOWS_

#ifndef _INC_CRTDBG
#include <crtdbg.h>
#endif //_INC_CRTDBG

#ifndef _INC_TCHAR
#include <tchar.h>
#endif //_INC_TCHAR

#ifndef _SFL_NO_PRAGMA_WARNINGS
	#if 1200 <= _MSC_VER
		#pragma warning(push,3)
	#endif
	#pragma warning(disable:4876)
	#pragma warning(disable:4284) // STL, not a UDT or reference to a UDT. Errors if infix notation used.
#endif

// Included because of our typedefs
// for CMapStringToString etc...
#ifndef __SFLSTRING_H__
#include "sflstring.h"   // Note to change when the paths get rearranged
#endif //__SFLSTRING_H__

#ifndef _MAP_
#include <map>
#endif // _MAP_

#ifndef _VECTOR_
#include <vector>
#endif // _VECTOR_

#ifndef _LIST_
#include <list>
#endif // _LIST_

namespace stingray {
namespace foundation {

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
class CMap
{
public:
	class POSITION
	{
		friend class CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>;
	public:
		POSITION(const int nNull = NULL)
			{ _ASSERTE(nNull == NULL); m_bNull = true; }

		operator bool() const // Allows while(pos) and while(NULL != pos)
			{ return !m_bNull; }

		bool operator!=(const int nNull) const // Allows while(pos != NULL) only
			{ _ASSERTE(nNull == NULL); return !m_bNull; }

		bool operator==(const int nNull) const // Allows if(pos == NULL) only
			{ _ASSERTE(nNull == NULL); return m_bNull; }

		bool operator==(const POSITION& rhs) const
			{ return m_bNull == rhs.m_bNull && m_iter == rhs.m_iter; }

		const POSITION& operator=(const int nNull) // Allows assignment to NULL
			{ _ASSERTE(nNull == NULL); m_bNull = true ; return *this; }

	private:
		bool m_bNull;

		std::map<KEY, VALUE>::const_iterator m_iter;
	};

	CMap(int nBlockSize = 10);
	~CMap();

	int GetCount() const;
	bool IsEmpty() const;

	bool Lookup(ARG_KEY key, VALUE& rValue) const;

	VALUE& operator[](ARG_KEY key);

	void SetAt(ARG_KEY key, ARG_VALUE newValue);

	bool RemoveKey(ARG_KEY key);
	void RemoveAll();

	CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::POSITION GetStartPosition() const;
	void GetNextAssoc(CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::POSITION& rNextPosition, KEY& rKey, VALUE& rValue) const;

	// These do nothing in this wrapper
	UINT GetHashTableSize() const;
	void InitHashTable(UINT hashSize, bool bAllocNow = TRUE);

protected:
	typedef KEY BASE_KEY;
	typedef ARG_KEY BASE_ARG_KEY;
	typedef VALUE BASE_VALUE;
	typedef ARG_VALUE BASE_ARG_VALUE;
private:
	std::map<KEY, VALUE> m_map;
};

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
inline int CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetCount() const
	{ return m_map.size(); }

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
inline bool CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::IsEmpty() const
	{ return m_map.empty(); }

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
inline VALUE& CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::operator[](ARG_KEY key)
	{ return m_map[key]; }

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
inline void CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::SetAt(ARG_KEY key, ARG_VALUE newValue)
	{ m_map[key] = newValue; }

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
inline void CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::RemoveAll()
	{ m_map.clear(); }

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
inline UINT CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetHashTableSize() const
	{ /* Not used, we are using the defaults of std::map */ return 0; }

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
inline void CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::InitHashTable(UINT hashSize, bool bAllocNow)
	{ /* Not used, we are using the defaults of std::map */ hashSize; bAllocNow; }

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::CMap(int nBlockSize)
{ 
	// Not used, we are using the defaults of std::map
	nBlockSize;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::~CMap()
{
	// The destructor of std::map m_map will clean up
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
bool CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::Lookup(ARG_KEY key, VALUE& rValue) const
{
	// Lookup item in the map
	if(m_map.count(key) == 0)
	{
		return false;
	}

	// This is a safe cast because we know the item
	// is there so we won't be creating a new one
	// and we are taking a copy of the value so we
	// can't modify it
	rValue = const_cast<CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>*>(this)->m_map[key];

	return true;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
bool CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::RemoveKey(ARG_KEY key)
{
	if(m_map.count(key) == 0)
	{
		return false;
	}

	m_map.erase(key);

	return true;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::POSITION CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetStartPosition() const
{
	CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::POSITION pos;
	
	if(!IsEmpty())
	{
		pos.m_bNull = false;
		pos.m_iter = m_map.begin();
	}

	return pos;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
void CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetNextAssoc(CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::POSITION& rNextPosition, KEY& rKey, VALUE& rValue) const
{
	// Can't call on an empty map or with an uninitialized position
	_ASSERTE(rNextPosition);
	_ASSERTE(GetCount() > 0);

	// Get the current values from rNextPosition.m_iter;
	rKey = rNextPosition.m_iter->first;
	rValue = rNextPosition.m_iter->second;

	// Now advance the iterator if we can
	rNextPosition.m_iter++;

	if(rNextPosition.m_iter == m_map.end())
	{
		rNextPosition = NULL;
	}
}

// Some typedefs for CTypedPtrMap to derive from
typedef CMap<void*, void*, void*, void*>     CMapPtrToPtr;
typedef CMap<void*, void*, WORD, WORD>       CMapPtrToWord;
typedef CMap<WORD, WORD, void*, void*>       CMapWordToPtr;
typedef CMap<CString, LPCTSTR, void*, void*> CMapStringToPtr;
typedef CMap<void*, void*, CString, LPCTSTR> CMapPtrToString;

/////////////////////////////////////////////////////////////////////////////
// CTypedPtrMap<BASE_CLASS, KEY, VALUE>

template<class BASE_CLASS, class KEY, class VALUE>
class CTypedPtrMap : public BASE_CLASS
{
public:

	CTypedPtrMap(int nBlockSize = 10)
		: BASE_CLASS(nBlockSize) { }

	bool Lookup(BASE_CLASS::BASE_ARG_KEY key, VALUE& rValue) const
		{ return BASE_CLASS::Lookup(key, (BASE_CLASS::BASE_VALUE&)rValue); }

	VALUE& operator[](BASE_CLASS::BASE_ARG_KEY key)
		{ return (VALUE&)BASE_CLASS::operator[](key); }

	void SetAt(KEY key, VALUE newValue)
		{ BASE_CLASS::SetAt(key, newValue); }

	bool RemoveKey(KEY key)
		{ return BASE_CLASS::RemoveKey(key); }

	void GetNextAssoc(BASE_CLASS::POSITION& rPosition, KEY& rKey, VALUE& rValue) const
		{ BASE_CLASS::GetNextAssoc(rPosition, (BASE_CLASS::BASE_KEY&)rKey,
			(BASE_CLASS::BASE_VALUE&)rValue); }
};

/////////////////////////////////////////////////////////////////////////////
// CArray<TYPE, ARG_TYPE>

template<class TYPE, class ARG_TYPE>
class CArray
{
public:
	CArray();
	~CArray();

	int GetSize() const;
	int GetUpperBound() const;
	void SetSize(int nNewSize, int nGrowBy = -1);

	void FreeExtra();
	void RemoveAll();

	TYPE GetAt(int nIndex) const;
	void SetAt(int nIndex, ARG_TYPE newElement);
	TYPE& ElementAt(int nIndex);

	const TYPE* GetData() const;
	TYPE* GetData();

	void SetAtGrow(int nIndex, ARG_TYPE newElement);
	int Add(ARG_TYPE newElement);
	int Append(const CArray& src);
	void Copy(const CArray& src);

	TYPE operator[](int nIndex) const;
	TYPE& operator[](int nIndex);

	void InsertAt(int nIndex, ARG_TYPE newElement, int nCount = 1);
	void RemoveAt(int nIndex, int nCount = 1);
	void InsertAt(int nStartIndex, CArray* pNewArray);
private:
	std::vector<TYPE> m_arr;
};

template<class TYPE, class ARG_TYPE>
inline int CArray<TYPE, ARG_TYPE>::GetSize() const
{
	return m_arr.size();
}

template<class TYPE, class ARG_TYPE>
inline int CArray<TYPE, ARG_TYPE>::GetUpperBound() const
{
	return GetSize() - 1;
}

template<class TYPE, class ARG_TYPE>
inline void CArray<TYPE, ARG_TYPE>::RemoveAll()
{
	m_arr.clear();
}

template<class TYPE, class ARG_TYPE>
inline TYPE CArray<TYPE, ARG_TYPE>::GetAt(int nIndex) const
{
	_ASSERTE(nIndex >= 0 && nIndex < GetSize());
	return m_arr[nIndex];
}

template<class TYPE, class ARG_TYPE>
inline void CArray<TYPE, ARG_TYPE>::SetAt(int nIndex, ARG_TYPE newElement)
{
	_ASSERTE(nIndex >= 0 && nIndex < GetSize());
	m_arr[nIndex] = newElement;
}

template<class TYPE, class ARG_TYPE>
inline TYPE& CArray<TYPE, ARG_TYPE>::ElementAt(int nIndex)
{
	_ASSERTE(nIndex >= 0 && nIndex < GetSize());
	return m_arr[nIndex];
}

template<class TYPE, class ARG_TYPE>
inline const TYPE* CArray<TYPE, ARG_TYPE>::GetData() const
{
	// TODO: Investigate this to see if
	// this is a valid thing to do
	if(GetSize() == 0)
	{
		return NULL;
	}
	else
	{
		return &m_arr[0];
	}
}

template<class TYPE, class ARG_TYPE>
inline TYPE* CArray<TYPE, ARG_TYPE>::GetData()
{
	// TODO: Investigate this to see if
	// this is a valid thing to do
	if(GetSize() == 0)
	{
		return NULL;
	}
	else
	{
		return &m_arr[0];
	}
}

template<class TYPE, class ARG_TYPE>
inline int CArray<TYPE, ARG_TYPE>::Add(ARG_TYPE newElement)
{
	int nIndex = GetSize();
	SetAtGrow(nIndex, newElement);
	return nIndex;
}

template<class TYPE, class ARG_TYPE>
inline TYPE CArray<TYPE, ARG_TYPE>::operator[](int nIndex) const
{
	return GetAt(nIndex);
}

template<class TYPE, class ARG_TYPE>
inline TYPE& CArray<TYPE, ARG_TYPE>::operator[](int nIndex)
{
	return ElementAt(nIndex);
}

template<class TYPE, class ARG_TYPE>
CArray<TYPE, ARG_TYPE>::CArray()
{
	// Nothing to do, using std::vector
}

template<class TYPE, class ARG_TYPE>
CArray<TYPE, ARG_TYPE>::~CArray()
{
	// Nothing to do, using std::vector
}

template<class TYPE, class ARG_TYPE>
void CArray<TYPE, ARG_TYPE>::SetSize(int nNewSize, int nGrowBy)
{
	nGrowBy; // unused in std::vector
	m_arr.resize(nNewSize);
}

template<class TYPE, class ARG_TYPE>
int CArray<TYPE, ARG_TYPE>::Append(const CArray& src)
{
	// cannot append to itself
	_ASSERTE(this != &src);

	int nOldSize = GetSize();

	// Insert at the end of this array
	InsertAt(nOldSize, const_cast<CArray*>(&src));

	return nOldSize;
}

template<class TYPE, class ARG_TYPE>
void CArray<TYPE, ARG_TYPE>::Copy(const CArray& src)
{
	// cannot copy to itself
	_ASSERTE(this != &src);

	m_arr.clear();
	m_arr.reserve(src.GetSize());
	Append(src);
}

template<class TYPE, class ARG_TYPE>
void CArray<TYPE, ARG_TYPE>::FreeExtra()
{
	m_arr.resize(GetSize());
}

template<class TYPE, class ARG_TYPE>
void CArray<TYPE, ARG_TYPE>::SetAtGrow(int nIndex, ARG_TYPE newElement)
{
	_ASSERTE(nIndex >= 0);

	if(nIndex >= GetSize())
	{
		m_arr.resize(nIndex + 1);
	}

	m_arr[nIndex] = newElement;
}

template<class TYPE, class ARG_TYPE>
void CArray<TYPE, ARG_TYPE>::InsertAt(int nIndex, ARG_TYPE newElement, int nCount /* = 1 */)
{
	_ASSERTE(nIndex >= 0);
	_ASSERTE(nCount > 0);

	if (nIndex >= GetSize())
	{
		// adding after the end of the array
		m_arr.resize(nIndex);
	}

	std::vector<TYPE>::iterator iter = m_arr.begin();
	iter += nIndex;
	m_arr.insert(iter, nCount, newElement);
}

template<class TYPE, class ARG_TYPE>
void CArray<TYPE, ARG_TYPE>::RemoveAt(int nIndex, int nCount)
{
	_ASSERTE(nIndex >= 0);
	_ASSERTE(nCount >= 0);
	_ASSERTE(nIndex + nCount <= GetSize());

	std::vector<TYPE>::iterator iter = m_arr.begin();
	iter += nIndex;
	m_arr.erase(iter, (iter + nCount));
}

template<class TYPE, class ARG_TYPE>
void CArray<TYPE, ARG_TYPE>::InsertAt(int nStartIndex, CArray* pNewArray)
{
	// cannot append to itself
	_ASSERTE(pNewArray);
	_ASSERTE(this != pNewArray);
	
	if(nStartIndex > GetSize())
	{
		m_arr.resize(nStartIndex);
	}

	int nIndex = nStartIndex;

	// No batch append so do it manually
	std::vector<TYPE>::const_iterator iter = pNewArray->m_arr.begin();
	while(iter != pNewArray->m_arr.end())
	{
		InsertAt(nIndex, *iter);
		iter++;
		nIndex++;
	}
}

// Typedefs for common array typesCTypedPtrArray
typedef CArray<WORD, WORD>          CWordArray;
typedef CArray<DWORD, DWORD>        CDWordArray;
typedef CArray<UINT, UINT>          CUIntArray;
typedef CArray<CString, LPCTSTR>    CStringArray;

// typedef for base class of CTypedPtrArray
typedef CArray<void*, void*> CPtrArray;

/////////////////////////////////////////////////////////////////////////////
// CTypedPtrArray<BASE_CLASS, TYPE>
template<class BASE_CLASS, class TYPE>
class CTypedPtrArray : public BASE_CLASS
{
public:
	TYPE GetAt(int nIndex) const
		{ return (TYPE)BASE_CLASS::GetAt(nIndex); }

	TYPE& ElementAt(int nIndex)
		{ return (TYPE&)BASE_CLASS::ElementAt(nIndex); }

	void SetAt(int nIndex, TYPE ptr)
		{ BASE_CLASS::SetAt(nIndex, ptr); }

	void SetAtGrow(int nIndex, TYPE newElement)
		{ BASE_CLASS::SetAtGrow(nIndex, newElement); }

	int Add(TYPE newElement)
		{ return BASE_CLASS::Add(newElement); }

	int Append(const CTypedPtrArray<BASE_CLASS, TYPE>& src)
		{ return BASE_CLASS::Append(src); }

	void Copy(const CTypedPtrArray<BASE_CLASS, TYPE>& src)
		{ BASE_CLASS::Copy(src); }

	void InsertAt(int nIndex, TYPE newElement, int nCount = 1)
		{ BASE_CLASS::InsertAt(nIndex, newElement, nCount); }

	void InsertAt(int nStartIndex, CTypedPtrArray<BASE_CLASS, TYPE>* pNewArray)
		{ BASE_CLASS::InsertAt(nStartIndex, pNewArray); }

	TYPE operator[](int nIndex) const
		{ return (TYPE)BASE_CLASS::operator[](nIndex); }

	TYPE& operator[](int nIndex)
		{ return (TYPE&)BASE_CLASS::operator[](nIndex); }
};

/////////////////////////////////////////////////////////////////////////////
// CList<TYPE, ARG_TYPE>
template<class TYPE, class ARG_TYPE>
class CList
{
public:
	class POSITION
	{
		friend class CList<TYPE, ARG_TYPE>;
	public:
		POSITION(const int nNull = NULL)
			{ _ASSERTE(nNull == NULL); m_bNull = true; }

		operator bool() const // Allows while(pos) and while(NULL != pos)
			{ return !m_bNull; }

		bool operator!=(const int nNull) const // Allows while(pos != NULL) only
			{ _ASSERTE(nNull == NULL); return !m_bNull; }

		bool operator==(const int nNull) const // Allows if(pos == NULL) only
			{ _ASSERTE(nNull == NULL); return m_bNull; }

		bool operator==(const POSITION& rhs) const
			{ return m_bNull == rhs.m_bNull && m_iter == rhs.m_iter; }

		const POSITION& operator=(const int nNull) // Allows assignment to NULL
			{ _ASSERTE(nNull == NULL); m_bNull = true ; return *this; }

	private:
		bool m_bNull;

		std::list<TYPE>::iterator m_iter;
	};

	CList(int nBlockSize = 10);
	~CList();

	int GetCount() const;
	bool IsEmpty() const;

	TYPE& GetHead();
	TYPE GetHead() const;
	TYPE& GetTail();
	TYPE GetTail() const;

	TYPE RemoveHead();
	TYPE RemoveTail();

	POSITION AddHead(ARG_TYPE newElement);
	POSITION AddTail(ARG_TYPE newElement);

	void AddHead(CList* pNewList);
	void AddTail(CList* pNewList);

	void RemoveAll();

	CList<TYPE, ARG_TYPE>::POSITION GetHeadPosition() const;
	CList<TYPE, ARG_TYPE>::POSITION GetTailPosition() const;
	TYPE& GetNext(CList<TYPE, ARG_TYPE>::POSITION& rPosition);
	TYPE GetNext(CList<TYPE, ARG_TYPE>::POSITION& rPosition) const;
	TYPE& GetPrev(CList<TYPE, ARG_TYPE>::POSITION& rPosition);
	TYPE GetPrev(CList<TYPE, ARG_TYPE>::POSITION& rPosition) const;

	TYPE& GetAt(CList<TYPE, ARG_TYPE>::POSITION position);
	TYPE GetAt(CList<TYPE, ARG_TYPE>::POSITION position) const;
	void SetAt(CList<TYPE, ARG_TYPE>::POSITION pos, ARG_TYPE newElement);
	void RemoveAt(CList<TYPE, ARG_TYPE>::POSITION position);

	CList<TYPE, ARG_TYPE>::POSITION InsertBefore(CList<TYPE, ARG_TYPE>::POSITION position, ARG_TYPE newElement);
	CList<TYPE, ARG_TYPE>::POSITION InsertAfter(CList<TYPE, ARG_TYPE>::POSITION position, ARG_TYPE newElement);

	CList<TYPE, ARG_TYPE>::POSITION Find(ARG_TYPE searchValue, CList<TYPE, ARG_TYPE>::POSITION startAfter = NULL) const; // Default value of 2nd param is NULL
	CList<TYPE, ARG_TYPE>::POSITION FindIndex(int nIndex) const;

private:
	// mutable so we can more easily a single
	// iterator in the POSITION
	mutable std::list<TYPE> m_list;
};

template<class TYPE, class ARG_TYPE>
inline int CList<TYPE, ARG_TYPE>::GetCount() const
	{ return m_list.size(); }

template<class TYPE, class ARG_TYPE>
inline bool CList<TYPE, ARG_TYPE>::IsEmpty() const
	{ return m_list.empty(); }

template<class TYPE, class ARG_TYPE>
inline TYPE& CList<TYPE, ARG_TYPE>::GetHead()
{
	_ASSERTE(!IsEmpty());
	return m_list.front();
}

template<class TYPE, class ARG_TYPE>
inline TYPE CList<TYPE, ARG_TYPE>::GetHead() const
{
	_ASSERTE(!IsEmpty());
	return m_list.front();
}

template<class TYPE, class ARG_TYPE>
inline TYPE& CList<TYPE, ARG_TYPE>::GetTail()
{
	_ASSERTE(!IsEmpty());
	return m_list.back();
}

template<class TYPE, class ARG_TYPE>
inline TYPE CList<TYPE, ARG_TYPE>::GetTail() const
{
	_ASSERTE(!IsEmpty());
	return m_list.back();
}

template<class TYPE, class ARG_TYPE>
inline CList<TYPE, ARG_TYPE>::POSITION CList<TYPE, ARG_TYPE>::GetHeadPosition() const
{
	CList<TYPE, ARG_TYPE>::POSITION pos;
	if(!IsEmpty())
	{
		pos.m_iter = m_list.begin();
		pos.m_bNull = false;
	}
	return pos;
}

template<class TYPE, class ARG_TYPE>
inline CList<TYPE, ARG_TYPE>::POSITION CList<TYPE, ARG_TYPE>::GetTailPosition() const
{
	CList<TYPE, ARG_TYPE>::POSITION pos;
	if(!IsEmpty())
	{
		pos.m_iter = m_list.end();
		pos.m_iter--;
		pos.m_bNull = false;
	}
	return pos;
}

template<class TYPE, class ARG_TYPE>
inline TYPE& CList<TYPE, ARG_TYPE>::GetNext(CList<TYPE, ARG_TYPE>::POSITION& rPosition)
{
	_ASSERTE(rPosition);
	_ASSERTE(rPosition.m_iter != m_list.end());

	std::list<TYPE>::iterator iter = rPosition.m_iter;

	rPosition.m_iter++;
	if(rPosition.m_iter == m_list.end())
	{
		rPosition = NULL;
	}

	return *iter;
}

template<class TYPE, class ARG_TYPE>
inline TYPE CList<TYPE, ARG_TYPE>::GetNext(CList<TYPE, ARG_TYPE>::POSITION& rPosition) const
{
	_ASSERTE(rPosition);
	_ASSERTE(rPosition.m_iter != m_list.end());

	std::list<TYPE>::iterator iter = rPosition.m_iter;

	rPosition.m_iter++;
	if(rPosition.m_iter == m_list.end())
	{
		rPosition = NULL;
	}

	return *iter;
}

template<class TYPE, class ARG_TYPE>
inline TYPE& CList<TYPE, ARG_TYPE>::GetPrev(CList<TYPE, ARG_TYPE>::POSITION& rPosition)
{
	_ASSERTE(rPosition);

	std::list<TYPE>::iterator iter = rPosition.m_iter;

	if(rPosition.m_iter == m_list.begin())
	{
		rPosition = NULL;
	}
	else
	{
		rPosition.m_iter--;
	}

	return *iter;
}

template<class TYPE, class ARG_TYPE>
inline TYPE CList<TYPE, ARG_TYPE>::GetPrev(CList<TYPE, ARG_TYPE>::POSITION& rPosition) const
{
	_ASSERTE(rPosition);

	std::list<TYPE>::iterator iter = rPosition.m_iter;

	if(rPosition.m_iter == m_list.begin())
	{
		rPosition = NULL;
	}
	else
	{
		rPosition.m_iter--;
	}

	return *iter;
}

template<class TYPE, class ARG_TYPE>
inline TYPE& CList<TYPE, ARG_TYPE>::GetAt(CList<TYPE, ARG_TYPE>::POSITION position)
{
	_ASSERTE(position);

	return *position.m_iter;
}

template<class TYPE, class ARG_TYPE>
inline TYPE CList<TYPE, ARG_TYPE>::GetAt(CList<TYPE, ARG_TYPE>::POSITION position) const
{
	_ASSERTE(position);

	return *position.m_iter;
}

template<class TYPE, class ARG_TYPE>
inline void CList<TYPE, ARG_TYPE>::SetAt(CList<TYPE, ARG_TYPE>::POSITION pos, ARG_TYPE newElement)
{
	_ASSERTE(pos);

	*pos.m_iter = newElement;
}

template<class TYPE, class ARG_TYPE>
CList<TYPE, ARG_TYPE>::CList(int nBlockSize)
{
	// using std::list defaults, so do nothing here
	nBlockSize;
}

template<class TYPE, class ARG_TYPE>
void CList<TYPE, ARG_TYPE>::RemoveAll()
{
	m_list.clear();
}

template<class TYPE, class ARG_TYPE>
CList<TYPE, ARG_TYPE>::~CList()
{
	// using std::list defaults, so do nothing here
}

template<class TYPE, class ARG_TYPE>
CList<TYPE, ARG_TYPE>::POSITION CList<TYPE, ARG_TYPE>::AddHead(ARG_TYPE newElement)
{
	m_list.push_front(newElement);
	CList<TYPE, ARG_TYPE>::POSITION pos;
	pos.m_iter = m_list.begin();
	pos.m_bNull = false;
	return pos;
}

template<class TYPE, class ARG_TYPE>
CList<TYPE, ARG_TYPE>::POSITION CList<TYPE, ARG_TYPE>::AddTail(ARG_TYPE newElement)
{
	m_list.push_back(newElement);
	CList<TYPE, ARG_TYPE>::POSITION pos;
	pos.m_iter = m_list.end();
	pos.m_bNull = false;
	return pos;
}

template<class TYPE, class ARG_TYPE>
void CList<TYPE, ARG_TYPE>::AddHead(CList* pNewList)
{
	_ASSERTE(pNewList);
	if(pNewList->IsEmpty())
	{
		return;
	}

	// Can't use splice because we don't want to empty pNewList
	// Walk from the tail of the list to the head and insert
	// into the head of our list
	std::list<TYPE>::reverse_iterator iter = pNewList->m_list.rbegin();
	while(iter != pNewList->m_list.rend())
	{
		m_list.push_front(*iter++);
	}	
}

template<class TYPE, class ARG_TYPE>
void CList<TYPE, ARG_TYPE>::AddTail(CList* pNewList)
{
	_ASSERTE(pNewList);
	if(pNewList->IsEmpty())
	{
		return;
	}

	// Can't use splice because we don't want to empty pNewList
	// Walk from the head of the list to the tail and insert
	// into the tail of our list
	std::list<TYPE>::iterator iter = pNewList->m_list.begin();
	while(iter != pNewList->m_list.end())
	{
		m_list.push_back(*iter++);
	}	
}

template<class TYPE, class ARG_TYPE>
TYPE CList<TYPE, ARG_TYPE>::RemoveHead()
{
	_ASSERTE(!IsEmpty());

	TYPE oldElement = m_list.front();
	m_list.pop_front();
	return oldElement;
}

template<class TYPE, class ARG_TYPE>
TYPE CList<TYPE, ARG_TYPE>::RemoveTail()
{
	_ASSERTE(!IsEmpty());

	TYPE oldElement = m_list.back();
	m_list.pop_back();
	return oldElement;
}

template<class TYPE, class ARG_TYPE>
CList<TYPE, ARG_TYPE>::POSITION CList<TYPE, ARG_TYPE>::InsertBefore(CList<TYPE, ARG_TYPE>::POSITION position, ARG_TYPE newElement)
{
	if(!position)
	{
		return AddHead(newElement);
	}

	position.m_iter = m_list.insert(position.m_iter, newElement);

	return position;
}

template<class TYPE, class ARG_TYPE>
CList<TYPE, ARG_TYPE>::POSITION CList<TYPE, ARG_TYPE>::InsertAfter(CList<TYPE, ARG_TYPE>::POSITION position, ARG_TYPE newElement)
{
	if(!position)
	{
		return AddHead(newElement);
	}

	position.m_iter = m_list.insert(++position.m_iter, newElement);

	return position;
}

template<class TYPE, class ARG_TYPE>
void CList<TYPE, ARG_TYPE>::RemoveAt(CList<TYPE, ARG_TYPE>::POSITION position)
{
	m_list.erase(position.m_iter);
}

template<class TYPE, class ARG_TYPE>
CList<TYPE, ARG_TYPE>::POSITION CList<TYPE, ARG_TYPE>::FindIndex(int nIndex) const
{
	CList<TYPE, ARG_TYPE>::POSITION pos;
	if(IsEmpty() || nIndex >= GetCount())
	{
		return pos;
	}

	pos.m_iter = m_list.begin();
	pos.m_bNull = false;

	while(nIndex > 0)
	{
		pos.m_iter++;
		nIndex--;
	}

	return pos;
}

template<class TYPE, class ARG_TYPE>
CList<TYPE, ARG_TYPE>::POSITION CList<TYPE, ARG_TYPE>::Find(ARG_TYPE searchValue, CList<TYPE, ARG_TYPE>::POSITION startAfter) const
{
	if(IsEmpty())
	{
		// return NULL position
		return CList<TYPE, ARG_TYPE>::POSITION();
	}

	if(!startAfter)
	{
		startAfter.m_iter = m_list.begin();
	}
	else
	{
		startAfter.m_iter++;
	}

	while(startAfter.m_iter != m_list.end())
	{
		if(searchValue == *startAfter.m_iter)
		{
			startAfter.m_bNull = false;
			return startAfter;
		}
		else
		{
			startAfter.m_iter++;
		}
	}

	// return NULL position
	return CList<TYPE, ARG_TYPE>::POSITION();
}

// typedefs for CTypedPtrList
typedef CList<void*, void*>     CPtrList;
typedef CList<CString, LPCTSTR> CStringList;

template<class BASE_CLASS, class TYPE>
class CTypedPtrList : public BASE_CLASS
{
public:
	CTypedPtrList(int nBlockSize = 10)
		: BASE_CLASS(nBlockSize) { }

	TYPE& GetHead()
		{ return (TYPE&)BASE_CLASS::GetHead(); }

	TYPE GetHead() const
		{ return (TYPE)BASE_CLASS::GetHead(); }

	TYPE& GetTail()
		{ return (TYPE&)BASE_CLASS::GetTail(); }

	TYPE GetTail() const
		{ return (TYPE)BASE_CLASS::GetTail(); }

	TYPE RemoveHead()
		{ return (TYPE)BASE_CLASS::RemoveHead(); }

	TYPE RemoveTail()
		{ return (TYPE)BASE_CLASS::RemoveTail(); }

	TYPE& GetNext(BASE_CLASS::POSITION& rPosition)
		{ return (TYPE&)BASE_CLASS::GetNext(rPosition); }

	TYPE GetNext(BASE_CLASS::POSITION& rPosition) const
		{ return (TYPE)BASE_CLASS::GetNext(rPosition); }

	TYPE& GetPrev(BASE_CLASS::POSITION& rPosition)
		{ return (TYPE&)BASE_CLASS::GetPrev(rPosition); }

	TYPE GetPrev(BASE_CLASS::POSITION& rPosition) const
		{ return (TYPE)BASE_CLASS::GetPrev(rPosition); }

	TYPE& GetAt(BASE_CLASS::POSITION position)
		{ return (TYPE&)BASE_CLASS::GetAt(position); }

	TYPE GetAt(BASE_CLASS::POSITION position) const
		{ return (TYPE)BASE_CLASS::GetAt(position); }

	void SetAt(BASE_CLASS::POSITION pos, TYPE newElement)
		{ BASE_CLASS::SetAt(pos, newElement); }
};

}	// namespace stingray::foundation
}   // namespace stingray


#ifndef _SFL_NO_PRAGMA_WARNINGS
	#if 1200 <= _MSC_VER
		#pragma warning (pop)
	#endif
#endif

#endif //__SFLCOLL_H__


