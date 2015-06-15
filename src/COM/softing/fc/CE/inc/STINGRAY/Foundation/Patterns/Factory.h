/////////////////////////////////////////////////////////////////////////////
// Factory.h : SFL Object factory design pattern
//
// Stingray Software Windows Template Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Gilberto Araya
// Description:  SFL Object factory design pattern templated implementation
//


#pragma once

#ifndef _SFL_NO_PRAGMA_WARNINGS
	#if 1200 <= _MSC_VER
		#pragma warning(push,3)
	#endif
	#pragma warning(disable:4786)
#endif

#if !defined(_SFL)
#define _SFL
#endif


#include <memory>
#include <foundation\GuidCast.h>

namespace stingray {
namespace foundation {


template <typename _Base>
class CObjectFactoryBase
{
public:
    virtual _Base* CreateObject() const = 0;
	virtual void DestroyObject(_Base* pObject) const = 0;
};


template <typename _Base, typename _Derived, typename _A = std::allocator<_Derived> >
class CObjectFactory: 
	public CObjectFactoryBase<_Base>
{
public:
    virtual _Base* CreateObject() const 
	{ 
		_A a; 
		_Derived* p = a.allocate(1, NULL); 
		a.construct(p, _Derived()); 
		return p;
	}

	virtual void DestroyObject(_Base* pObject) const
	{ 
		_A a; 
		_Derived* pD = guid_cast<_Derived*>(pObject);
		if (pD) {
			a.destroy(pD);
			a.deallocate(pD, 1);
		}
	}
};




};	// namespace stingray::foundation
};	// namespace stingray

#ifndef _SFL_NO_PRAGMA_WARNINGS
	#if 1200 <= _MSC_VER
		#pragma warning (pop)
	#endif
#endif