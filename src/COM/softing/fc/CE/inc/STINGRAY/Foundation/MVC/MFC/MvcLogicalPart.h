/////////////////////////////////////////////////////////////////////////////
// MvcLogicalPart.h : Declaration of MvcLogicalPart
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
// Description:  Declaration of MvcLogicalPart interface.
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/MVC/MFC/MvcLogicalPart.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

// @doc MvcLogicalPart

#pragma once

#ifndef __MFC_MVCLOGICALPART_H__
#define __MFC_MVCLOGICALPART_H__

// Includes

#include <Foundation\MVC\MFC\MvcVisualPart.h>
#include <Foundation\MVC\MFC\MvcVisualComponent.h>
#include <Foundation\MVC\MvcLogicalPart.h>

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

//@type MvcLogicalPart | Instantiates the <c CMvcLogicalPart> template with <t MvcVisualPart> as the base class.
// It preserves compatibility with previous versions of MVC.
//@iex typedef stingray::foundation::CMvcLogicalPart< MvcVisualPart > MvcLogicalPart;
//@xref <c CMvcLogicalPart>
typedef stingray::foundation::CMvcLogicalPart< MvcVisualPart > MvcLogicalPart;

};  // namespace stingray::foundation
};  // namespace stingray

///////////////////////////////////////////////////////////////////////////

//
// MVC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif  // #ifndef __MFC_MVCLOGICALPART_H__
