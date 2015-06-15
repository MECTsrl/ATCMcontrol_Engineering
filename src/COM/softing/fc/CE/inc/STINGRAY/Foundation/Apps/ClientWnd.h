/////////////////////////////////////////////////////////////////////////////
// WtlClientWnd.h : SFL Client window
//
// Stingray Software Windows Template Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//

#pragma once

#include <foundation\SflCommon.h>


namespace stingray {
namespace foundation {


typedef CWinTraits<WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_CHILD | WS_VISIBLE, WS_EX_STATICEDGE> CClientWindowTraits;


///////////////////////////////////////////////////////////////////////////
//@doc CClientWindowImpl
//
//@class CClientWindowImpl | Base class for client windows
//
//@tcarg typename | typename _Derived | The ultimately-derived class
//@tcarg typename | _Traits = CClientWindowTraits | Window creation flags
//@tcarg typename | _Base = ATL::CWindowImpl<lt>_Derived, CWindow, _Traits<gt> | Base class
//
//@base public | _Base (default is CComModule)
//
//@comm Sometimes your application architecture calls for non-frame windows. 
// For example, many applications require the rendering and drawing code to be 
// separate from the frame. SFL supports this requirement through its client 
// windows. SFL defines its client windows through CClientWindowImpl. 
// Client windows are usually children of frame windows. SFL's client window class 
// is usually mixed in with other classes. 
//
// The main benefit of the CClientWindowImpl is that it defines a window
//  class with client window creation flags. The flags used by CClientWindowTraits
//  include WS_CLIPCHILDREN. WS_CLIPSIBLINGS, WS_CHILD, WS_VISIBLE, WS_EX_STATICEDGE. 
//
template <typename _Derived, typename _Traits = CClientWindowTraits, typename _Base = ATL::CWindowImpl<_Derived, CWindow, _Traits> >
class CClientWindowImpl: 
	public _Base
{
public:
	typedef CClientWindowImpl<_Derived, _Base, _Traits > _thisClass;
	typedef _Base _windowBase;

	BEGIN_MSG_MAP(_thisClass)

	END_MSG_MAP()
};


};	// namespace stingray::foundation
};	// namespace stingray