/////////////////////////////////////////////////////////////////////////////
// Iterator.h : SFL Polymorphic iterator 
//
// Stingray Software Windows Template Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Gilberto Araya
// Description:  SFL polymorphic iterator implementation
//


#pragma once

#ifndef __ITERATOR_H__
#define __ITERATOR_H__

#ifndef _SFL_NO_PRAGMA_WARNINGS
	#if 1200 <= _MSC_VER
		#pragma warning(push,3)
	#endif
	#pragma warning(disable:4786)
#endif


#if !defined(_SFL)
#define _SFL
#endif


namespace stingray {
namespace foundation {


template <typename _Elem>
class IteratorBase 
{
public:

	typedef _Elem Element;
	virtual ~IteratorBase() {}

public:
	virtual Element Next() = 0;
	virtual Element Prev() = 0;
	virtual Element Current() = 0;
	virtual bool Finished() = 0;
	virtual void Start() = 0;
};



template <typename _Aggr, typename _Iter = _Aggr::iterator, typename _Elem = _Aggr::value_type>
class Iterator
	: public IteratorBase<_Elem>
{
public:
	explicit Iterator(_Aggr& acollection): _coll(acollection) {Start();}

public:

	// Polymorphic iteration methods
	virtual Element Next();
	virtual Element Prev();
	virtual Element Current();
	virtual bool Finished();
	virtual void Start();

private:
	_Aggr& _coll;
	_Iter _it;
};


template <typename _Aggr, typename _Iter, typename _Elem>
Iterator<_Aggr, _Iter, _Elem>::Element 
Iterator<_Aggr, _Iter, _Elem>::Next()
{
	++_it;
	return Current();
}

template <typename _Aggr, typename _Iter, typename _Elem>
Iterator<_Aggr, _Iter, _Elem>::Element 
Iterator<_Aggr, _Iter, _Elem>::Prev()
{
	--_it;
	return Current();
}

template <typename _Aggr, typename _Iter, typename _Elem>
Iterator<_Aggr, _Iter, _Elem>::Element 
Iterator<_Aggr, _Iter, _Elem>::Current()
{
	return *_it;
}

template <typename _Aggr, typename _Iter, typename _Elem>
bool 
Iterator<_Aggr, _Iter, _Elem>::Finished()
{
	return _it == _coll.end();
}

template <typename _Aggr, typename _Iter, typename _Elem>
void 
Iterator<_Aggr, _Iter, _Elem>::Start()
{
	_it = _coll.begin();
}


};
};

#ifndef _SFL_NO_PRAGMA_WARNINGS
	#if 1200 <= _MSC_VER
		#pragma warning(pop)
	#endif
#endif

#endif // #ifndef __ITERATOR_H__
