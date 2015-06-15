/////////////////////////////////////////////////////////////////////////////
// MvcModel.h : Declaration of MvcModel
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
// Description:  Declaration of MvcModel interface.
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/MVC/MFC/MvcModel.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

//@doc MvcModel

#pragma once

#ifndef __MFC_MVCMODEL_H__
#define __MFC_MVCMODEL_H__

// Includes
#include <Foundation\MVC\MvcModel.h>

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

//@type MvcModel | This is an alias for <c CMvcModel> used to preserve compability with old names.
//@iex typedef stingray::foundation::CMvcModel MvcModel;
//@xref <c CMvcModel>
typedef stingray::foundation::CMvcModel MvcModel;

};  // namespace stingray::foundation
};  // namespace stingray

#endif  // #ifndef __MFC_MVCMODEL_H__
