/////////////////////////////////////////////////////////////////////////////
// MvcVisualComponent.h : Declaration of MvcVisualComponent
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
// Description:  Declaration of MvcVisualComponent interface.
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/MVC/MFC/MvcVisualComponent.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

//@doc MvcVisualComponent

#pragma once

#ifndef __MFC_MVCVISUALCOMPONENT_H__
#define __MFC_MVCVISUALCOMPONENT_H__

#include <Foundation\MVC\MvcVisualComponent.h>

//
// MVC Extension DLL
// Initialize declaration context
//
#ifdef _SFLDLL
	#undef AFXAPP_DATA
	#define AFXAPP_DATA AFXAPI_DATA
	#undef AFX_DATA
	#define AFX_DATA    SFL_DATAEXT
#endif //_SFLDLL

namespace stingray {
namespace foundation {

//@type MvcVisualComponent | This is an alias for <c CMvcVisualComponent> used to preserve
// compatibility with previous versions of MVC.
//@iex typedef CMvcVisualComponent MvcVisualComponent;
//@xref <c CMvcVisualComponent>
typedef CMvcVisualComponent MvcVisualComponent;

};  // namespace stingray::foundation
};  // namespace stingray

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif  // #ifndef __MFC_MVCVISUALCOMPONENT_H__
