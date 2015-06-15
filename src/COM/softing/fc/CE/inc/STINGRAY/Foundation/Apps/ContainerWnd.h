/////////////////////////////////////////////////////////////////////////////
// ContainerWnd.h : SFL Window Container
//
// Stingray Software Foundation Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Gilberto Araya
// Description:  Integration between windowing framework and Layout Manager
//


#pragma once

#if !defined(_SFL)
#define _SFL
#endif

#include <foundation\SflCommon.h>
#include <foundation\Layout\LayoutManager.h>


namespace stingray {
namespace foundation {


/////////////////////////////////////////////////////////////////////////////
// CContainerImplBase- Container window description
//
//
//@doc CContainerImplBase
//
//@class CContainerImplBase | This class is the main class mixing a window class with 
// an SFL plugin
//
//@tcarg typename | _Derived | The ultimately-derived class
//@tcarg typename | _Traits | Window creation flags 
//@tcarg typename | _BaseImpl | The base class 
//@tcarg typename | _LayoutPlugin | Layout plugin 
//
//@base public | _BaseImpl
//@base public | _LayoutPlugin
//
//@comm CContainerImplBase is the main class mixing a window class 
// with one of SFL's layout plugins. 
//
template <typename _Derived, typename _Traits, typename _BaseImpl, typename _LayoutPlugin >
class CContainerImplBase:
	public _BaseImpl,
	public _LayoutPlugin
{
protected:
	typedef _BaseImpl _WindowBaseImpl;
	typedef CContainerImplBase<_Derived, _Traits, _BaseImpl, _LayoutPlugin> _ContainerBase;

public:
	BEGIN_MSG_MAP(CContainerImplBase)
		CHAIN_MSG_MAP(_LayoutPlugin)
	END_MSG_MAP()
};


/////////////////////////////////////////////////////////////////////
// Specific specializations based on exepected use cases 
// of the container class

/////////////////////////////////////////////////////////////////////////////
// CContainerWindowImpl - Container window Implementation
//
//@doc CContainerWindowImpl
//
//@class CContainerWindowImpl | Convenient implementation version of a container window.
//
//@tcarg typename | _Derived | The ultimately-derived class
//@tcarg typename | _Traits | Window creation flags 
//@tcarg typename | _Base | The base class 
//@tcarg typename | _LayoutPlugin | Layout manager base class
//
//@base public | <c CContainerImplBase>
//
//@comm CContainerWindowImpl is a convenient implementation of the container window 
//
template <typename _Derived, typename _Traits, typename _Base = CWindow, typename _LayoutPlugin = foundation::CLayoutManager<_Derived> >
class CContainerWindowImpl : 
	public CContainerImplBase<_Derived, _Traits, CWindowImpl<_Derived, _Base, _Traits>, _LayoutPlugin >
{
};

/////////////////////////////////////////////////////////////////////////////
// CContainerDialogImpl - Container dialog Implementation
//
//@doc CContainerDialogImpl
//
//@class CContainerDialogImpl | Convenient implementation version of a container window.
//
//@tcarg typename | _Derived | The ultimately-derived class
//@tcarg typename | _Traits | Window creation flags 
//@tcarg typename | _Base | The base class 
//
//@base public | <c CContainerImplBase>
//
//@comm CContainerDialogImpl is a convenient implementation of the container dialog 
//
template <typename _Derived>
class CContainerDialogImpl : 
	public CContainerImplBase<_Derived, CNullTraits, CAxDialogImpl<_Derived>, foundation::CLayoutManager<_Derived, WM_INITDIALOG> >
{
};


#if defined(__ATLCTL_H__)
// Only when implementing ActiveX controls
template <typename _Derived, typename _Traits = CControlWinTraits>
class CContainerControlImpl : 
	public CContainerImplBase<_Derived, _Traits, CComCompositeControl<_Derived>, foundation::CLayoutManager<_Derived, WM_INITDIALOG> >
{
};

#endif


};	// namespace stingray::foundation
};	// namespace stingray
