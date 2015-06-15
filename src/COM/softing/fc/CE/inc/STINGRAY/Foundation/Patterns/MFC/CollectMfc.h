/////////////////////////////////////////////////////////////////////////////
// CollectMfc.h : Declaration of base collection classes
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
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Patterns/MFC/CollectMfc.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $


#ifndef __SFL_COLLECTMFC_H__
#define __SFL_COLLECTMFC_H__

// Includes

#ifndef __AFXTEMPL_H__
#include <afxtempl.h>
#endif

//
// CMN Extension DLL
// Initialize declaration context
//
#ifdef _SFLDLL
	#undef AFXAPP_DATA
	#define AFXAPP_DATA AFXApp_DATA
	#undef AFX_DATA
	#define AFX_DATA    SFL_DATAEXT
#endif //_SFLDLL

namespace stingray {
namespace foundation {

//@doc CCArray_T
//@class A Collection Classes array that supports the ICollection_T interface
// a therefore iteration with Iterator_T.  This class doesn't define a new
// interface, other than that inherited from MFC's CArray class.  The interface
// you should use is the Iterator_T class to access the elements stored in
// a CCArray_T.
//@tcarg class | element_t | The class of the element to store.
//@tcarg class | arg_t | The class of the arguments to certain CArray functions.
template < class element_t,	class arg_t	> class CCArray_T : 
	public CArray<element_t, arg_t>, public ICollection_T<element_t>
{
public:
	virtual element_t iGet(CCIterPos* pbp) const {
		CCIterPos_T<int>* pp = (CCIterPos_T<int>*)pbp;
		if (!iEOC(pbp))
			return GetAt(pp->m_value);
		else
			return NULL;
	};

	virtual element_t* iGetPtr(CCIterPos* pbp) const {
		CCIterPos_T<int>* pp = (CCIterPos_T<int>*)pbp;
		if (!iEOC(pbp))
			return &(m_pData[pp->m_value]);
		else
			return NULL;
	};

	virtual element_t& iGetRef(CCIterPos* pbp) const {
		CCIterPos_T<int>* pp = (CCIterPos_T<int>*)pbp;
		if (!iEOC(pbp))
			return m_pData[pp->m_value];
		else
			return *m_pData;
	};

	virtual BOOL iEOC(CCIterPos* pbp) const {
		CCIterPos_T<int>* pp = (CCIterPos_T<int>*)pbp;
		if (pp->m_value >= 0 && pp->m_value < GetSize())
			return FALSE;
		else
			return TRUE;
	};

	virtual void iFirst(CCIterPos* pbp) const {
		CCIterPos_T<int>* pp = (CCIterPos_T<int>*)pbp;
		pp->m_value = 0;
	};

	virtual void iNext(CCIterPos* pbp) const {
		CCIterPos_T<int>* pp = (CCIterPos_T<int>*)pbp;
		pp->m_value++;
	};

	virtual void iPrev(CCIterPos* pbp) const {
		CCIterPos_T<int>* pp = (CCIterPos_T<int>*)pbp;
		pp->m_value--;
	};

	virtual void iLast(CCIterPos* pbp) const {
		CCIterPos_T<int>* pp = (CCIterPos_T<int>*)pbp;
		pp->m_value = GetUpperBound();
	};

	virtual int GetSize() const {
		return CArray<element_t, arg_t>::GetSize();
	};

	virtual void iSet(CCIterPos* pbp, element_t elem) {
		CCIterPos_T<int>* pp = (CCIterPos_T<int>*)pbp;
		SetAt(pp->m_value, elem);
	};

	virtual void iRemoveAt(CCIterPos* pbp) {
		CCIterPos_T<int>* pp = (CCIterPos_T<int>*)pbp;
		RemoveAt(pp->m_value);
	};

	virtual void Remove(element_t elem) {
		int i;
		for (i = GetUpperBound(); i >= 0; i--) {
			if (elem == GetAt(i))
				RemoveAt(i);
		}
	};

// WNL: Fix for CArray::Append() for VC++ 4.2
#if _MFC_VER == 0x0420
	virtual int Append(const CCArray_T<element_t,arg_t>& src) {
		int nAppendIndex = GetSize();
		for(int i=0; i<src.GetSize(); i++)
			Add(src[i]);
		return nAppendIndex;
	};
#endif

	virtual element_t iInsert(CCIterPos* pbp, element_t elem) {
		CCIterPos_T<int>* pp = (CCIterPos_T<int>*)pbp;
		InsertAt(pp->m_value, elem);
		return elem;
	};

	virtual BOOL iFindNext(CCIterPos* pbp, element_t elem) const {
		CCIterPos_T<int>* pp = (CCIterPos_T<int>*)pbp;
		return FindNext(pp->m_value, elem);
	};

	virtual BOOL iFindPrev(CCIterPos* pbp, element_t elem) const {
		CCIterPos_T<int>* pp = (CCIterPos_T<int>*)pbp;
		return FindPrev(pp->m_value, elem);
	};

	virtual BOOL FindNext(int& pos, element_t elem) const {
		BOOL bFound = FALSE;
		int seek = pos;
		while (!bFound && ++seek < GetSize()) {
			if (elem == GetAt(seek))
				bFound = TRUE;
		}
		seek = -1;
		while (!bFound && ++seek < GetSize()) {
			if (elem == GetAt(seek))
				bFound = TRUE;
		}
		if (bFound)
			pos = seek;
		return bFound;
	};

	virtual BOOL FindPrev(int& pos, element_t elem) const {
		BOOL bFound = FALSE;
		int seek = pos;
		while (!bFound && --seek >= 0) {
			if (elem == GetAt(seek))
				bFound = TRUE;
		}
		seek = GetSize();
		while (!bFound && --seek >= 0) {
			if (elem == GetAt(seek))
				bFound = TRUE;
		}
		if (bFound)
			pos = seek;
		return bFound;
	};

	virtual void RemoveAll() {
		CArray<element_t, arg_t>::RemoveAll();
	};

	virtual int FindIndex(element_t elem) const {
		int nIdx = -1;
		for (int nCurIdx = 0; nCurIdx < GetSize() && nIdx == -1; nCurIdx++)
		{
			if (elem == GetAt(nCurIdx))
				nIdx = nCurIdx;
		}
		return nIdx;
	};

	virtual CCIterPos* iCreateIteratorPosition() const {
		return new CCIterPos_T<int>;
	};
};

//@doc CCStack_T
//@mfunc void | CCStack_T | CCStack_T | This constructor creates a CCStack_T object initialized with the specified maximum depth
//@parm int | nMaxSize | The maximum depth of the stack
//@xref <c CCStack_T>

//@doc CCStack_T
//@mfunc void | CCStack_T | SetMaxSize | Sets the maximum depth of the stack.  Once this depth is reached, the stack "rolls"
//@parm int | nMaxSize | The maximum depth of the stack
//@xref <c CCStack_T>

//@doc CCStack_T
//@mfunc void | CCStack_T | Push | Pushes a new element onto the stack
//@parm element_t | elem | The element to push onto the stack
//@xref <c CCStack_T>

//@doc CCStack_T
//@mfunc void | CCStack_T | Pop | Retrieves and deletes to newest element off the top of the stack
//@xref <c CCStack_T>

//@doc CCStack_T
//@mfunc void | CCStack_T | Peek | Retrieves but does not delete the newest element off the top of the stack
//@xref <c CCStack_T>

//@doc CCStack_T
//@mfunc void | CCStack_T | GetElementCount | Returns the number of elements that currently exist in the stack
//@xref <c CCStack_T>

//@doc CCStack_T
//@class A Collection Classes Stack that implements the First-In, First-Out
// method of storage.  In addition, this stack can function as a circular
// buffer.  Basically, when the stack reaches its maximum depth, the oldest
// element at the bottom of the stack is expired to make room for a new
// element.  Use the SetMaxSize to setup the maximum depth.
//@tcarg class | element_t | The class of the element to store in the stack.
//@tcarg class | arg_t | The class of the arguments to certain CArray functions.
template <class element_t, class arg_t> class CCStack_T : 
	private CArray<element_t, arg_t>, public IStack_T<element_t>
{
protected:
	int m_nIndex;
	int m_nMaxSize;
	int m_cElems;

public:
	CCStack_T<element_t, arg_t>() {
		m_nIndex = -1;
		m_cElems = 0;
		m_nMaxSize = -1;
	}

	//@cmember
	/* This constructor creates a CCStack_T object initialized with the specified maximum depth*/
	CCStack_T<element_t, arg_t>(int nMaxSize) {
		m_nIndex = -1;
		m_cElems = 0;
		SetMaxSize(nMaxSize);
	}

	//@cmember
	/* Sets the maximum depth of the stack.  Once this depth is reached, the stack "rolls"*/
	void SetMaxSize(int nMaxSize) {
		m_nMaxSize = nMaxSize;
		SetSize(m_nMaxSize);
	}

	//@cmember
	/* Returns the number of elements that currently exist in the stack*/
	int GetElementCount() const {
		return m_cElems;
	}

	//@cmember
	/* Pushes a new element onto the stack*/
	element_t Push(element_t elem) {
		element_t oldElem;
		m_nIndex = ( (m_nIndex+1) % m_nMaxSize );
		oldElem = GetAt(m_nIndex);
		SetAt(m_nIndex, elem);
		m_cElems++;
		m_cElems = mvcmin(m_cElems, m_nMaxSize);
		return oldElem;
	}

	//@cmember
	/* Retrieves but does not delete the newest element off the top of the stack*/
	element_t Peek() const {
		return (m_nIndex >= 0 && m_nIndex < m_nMaxSize) ? GetAt(m_nIndex) : NULL;
	}

	//@cmember
	/* Retrieves and deletes to newest element off the top of the stack*/
	element_t Pop() {
		element_t elem;
		if (m_cElems > 0) {
			elem = GetAt(m_nIndex);
			SetAt(m_nIndex, NULL);
			m_nIndex = ( (m_nIndex-1) % m_nMaxSize );
			if (m_nIndex < 0)
				m_nIndex = m_nMaxSize-1;
			m_cElems--;
			m_cElems = mvcmax(m_cElems, 0);
		}
		else
			elem = NULL;
		return elem;
	}
};

};  // namespace stingray::foundation
};  // namespace stingray

//
// CMN Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif // __SFL_SECCOLLECT_H__

