/////////////////////////////////////////////////////////////////////////////
// CollectStl.h : STL version of collection classes.
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
// Author:       Jeff Boenig
// Description:  STL version of collection classes.
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Patterns/MFC/CollectStl.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

#ifndef __SFL_COLLECTSTL_H__
#define __SFL_COLLECTSTL_H__

#pragma warning (disable : 4018)
#pragma warning (disable : 4100)
#pragma warning (disable : 4238)
#pragma warning (disable : 4663)
#pragma warning (disable : 4244)

#include <vector>

using std::vector;

namespace stingray {
namespace foundation {

#define _GET_ITERPOS_VAL(pbp)  (((CCIterPos_T<size_type>*) pbp)->m_value)

template <class element_t,	class arg_t>
class CCArray_T : public vector<element_t>, public ICollection_T<element_t>
{
public:

	typedef vector<element_t> ElementVector;
	typedef vector<element_t>::iterator ElementIterator;
	typedef CCIterPos_T<size_type> ElementIteratorPos;

	virtual element_t iGet(CCIterPos* pbp) const
	{
		CCArray_T<element_t,arg_t>* pThis = const_cast<CCArray_T<element_t,arg_t>*>(this);

		try
		{
			size_type pos = _GET_ITERPOS_VAL(pbp);
			if (pos < size())
				return pThis->at(pos);
		}
		catch (out_of_range)
		{
			return NULL;
		}

		return NULL;
	}

	virtual element_t* iGetPtr(CCIterPos* pbp)
	{
		try
		{
			return &(at(_GET_ITERPOS_VAL(pbp)));
		}
		catch (out_of_range)
		{
			return NULL;
		}
	}

	virtual BOOL iEOC(CCIterPos* pbp) const
	{
		if (_GET_ITERPOS_VAL(pbp) >= 0 && _GET_ITERPOS_VAL(pbp) < size())
			return FALSE;
		else
			return TRUE;
	}

	virtual void iFirst(CCIterPos* pbp) const
	{
		_GET_ITERPOS_VAL(pbp) = 0;
	}

	virtual void iNext(CCIterPos* pbp) const
	{
		_GET_ITERPOS_VAL(pbp)++;
	}

	virtual void iPrev(CCIterPos* pbp) const
	{
		_GET_ITERPOS_VAL(pbp)--;
	}

	virtual void iLast(CCIterPos* pbp) const
	{
		_GET_ITERPOS_VAL(pbp) = (size() - 1);
	}

	virtual int GetSize() const
	{
		return size();
	}

	virtual void iSet(CCIterPos* pbp, element_t elem)
	{
		at(_GET_ITERPOS_VAL(pbp)) = elem;
	}

	virtual void iRemoveAt(CCIterPos* pbp)
	{
		ElementIterator iElem;
		iElem = begin() + _GET_ITERPOS_VAL(pbp);
		erase(iElem);
	}

	virtual void Remove(element_t elem)
	{
		ElementIterator iElem;

		for (iElem = begin(); iElem != end(); iElem++)
		{
			if (*iElem == elem)
			{
				erase(iElem);
				return;
			}
		}
	}

	virtual element_t iInsert(CCIterPos* pbp, element_t elem)
	{
		ElementIterator iElem;
		iElem = begin() + _GET_ITERPOS_VAL(pbp);
		insert(iElem, elem);
		return elem;
	}

	virtual BOOL iFindNext(CCIterPos* pbp, element_t elem) const
	{
		return FindNext(_GET_ITERPOS_VAL(pbp), elem);
	}

	virtual BOOL iFindPrev(CCIterPos* pbp, element_t elem) const
	{
		return FindPrev(_GET_ITERPOS_VAL(pbp), elem);
	}

	virtual BOOL FindNext(size_type& pos, element_t elem) const
	{
		BOOL bFound = FALSE;
		size_type posSeek = pos;

		while (!bFound && (++posSeek) < size())
		{
			if (elem == at(posSeek))
				bFound = TRUE;
		}

		posSeek = 0;

		while (!bFound && (++posSeek) < size())
		{
			if (elem == at(posSeek))
				bFound = TRUE;
		}

		if (bFound)
			pos = posSeek;

		return bFound;
	}

	virtual BOOL FindPrev(size_type& pos, element_t elem) const
	{
		BOOL bFound = FALSE;
		int posSeek = pos;

		while (!bFound && (--posSeek) >= 0)
		{
			if (elem == at((size_type)posSeek))
				bFound = TRUE;
		}

		posSeek = (int) (size() - 1);

		while (!bFound && (--posSeek) >= 0)
		{
			if (elem == at((size_type)posSeek))
				bFound = TRUE;
		}

		if (bFound)
			pos = (size_type)posSeek;

		return bFound;
	}

	virtual void RemoveAll()
	{
		clear();
	}

	virtual int FindIndex(element_t elem) const
	{
		CCArray_T<element_t,arg_t>* pThis = const_cast<CCArray_T<element_t,arg_t>*>(this);
		ElementIterator iElem;
		int nIdx = -1;
		int nCurIdx = 0;

		for (iElem = pThis->begin(); nIdx == -1 && iElem != pThis->end(); iElem++)
		{
			if (elem == *iElem)
				nIdx = nCurIdx;
			else
				nCurIdx++;
		}

		return nIdx;
	}

	virtual CCIterPos* iCreateIteratorPosition() const
	{
		return new ElementIteratorPos;
	}

	element_t GetAt(int nIndex) const
	{
		try
		{
			return at(nIndex);
		}
		catch (out_of_range)
		{
			return NULL;
		}
	}

	element_t& ElementAt(int nIndex)
	{
		return at(nIndex);
	}

	int Add(element_t newElement)
	{
		push_back(newElement);
		return (size() - 1);
	}

	int Append(const CCArray_T<element_t,arg_t>& src)
	{
		int nCount = 0;
		ElementIterator iElem;
		CCArray_T<element_t,arg_t>* pSrc = const_cast<CCArray_T<element_t,arg_t>*>(&src);

		for (iElem = pSrc->begin(); iElem != pSrc->end(); iElem++)
		{
			push_back(*iElem);
			nCount++;
		}

		return nCount;
	}

	void RemoveAt(int nIndex, int nCount = 1)
	{
		if (nIndex < size())
		{
			ElementIterator iFirst = begin() + nIndex;

			if (nCount == 1)
			{
				erase(iFirst);
			}
			else
			{
				ElementIterator iLast = iFirst + (nCount - 1);
				erase(iFirst, iLast);
			}
		}
	}

	int GetUpperBound() const
	{
		return (size()-1);
	}

	void SetSize(int nNewSize, int nGrowBy = -1)
	{
		if (nGrowBy != -1)
		{
			reserve(size() + nGrowBy);
		}

		resize(nNewSize);
	}

	void SetAt(int nIndex, arg_t newElement)
	{
		at(nIndex) = newElement;
	}

	void SetAtGrow(int nIndex, arg_t newElement)
	{
		if (nIndex >= size())
		{
			resize(nIndex + 1);
		}

		at(nIndex) = newElement;
	}

	void InsertAt(int nIndex, arg_t newElement, int nCount = 1)
	{
		ElementIterator iElem;
		iElem = begin() + nIndex;
		insert(iElem, nCount, newElement);
	}
};

};  // namespace stingray::foundation
};  // namespace stingray

#endif // __SFL_COLLECTSTL_H__

