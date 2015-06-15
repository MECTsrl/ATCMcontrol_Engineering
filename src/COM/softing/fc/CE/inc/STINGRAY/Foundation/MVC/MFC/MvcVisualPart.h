/////////////////////////////////////////////////////////////////////////////
// MvcVisualPart.h : Declaration of MvcVisualPart
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
// Description:  Declaration of MvcVisualPart interface.
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/MVC/MFC/MvcVisualPart.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

// @doc MvcVisualPart

#pragma once

#ifndef __MFC_MVCVISUALPART_H__
#define __MFC_MVCVISUALPART_H__

#include <Foundation\MVC\MvcVisualPart.h>
#include <Foundation\MVC\MFC\MvcVisualComponent.h>

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

//@doc MvcVisualPart
//@class MvcVisualPart | This class instantiates <c CMvcVisualPart> using
// <t MvcVisualComponent> as the base class. It helps preserve compatibility
// with previous versions of MVC. This class does not declare any members.
//@base public | CMvcVisualPart
class MvcVisualPart : public CMvcVisualPart<MvcVisualComponent, MvcVisualPart>
{
};

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


#endif  // #ifndef __MFC_MVCVISUALPART_H__
