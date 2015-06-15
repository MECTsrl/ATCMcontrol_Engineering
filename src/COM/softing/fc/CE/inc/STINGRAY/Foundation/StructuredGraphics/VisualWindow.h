/////////////////////////////////////////////////////////////////////////////
// VisualWindow.h : SFL Model-View-Controller architecture 
//
// Stingray Software Foundation Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Jeff Boenig
// Description:  SFL MVC: Visual window interface definition
//

#pragma once

#ifndef __VISUALWINDOW_H__
#define __VISUALWINDOW_H__

#include <foundation\guidcast.h>

namespace stingray {
namespace foundation {

//////////////////////////////////////////////////////////////////////////
//@doc IVisualWindow
//
//@class IVisualWindow | This interface provides access to a window
// handle. It is used to abstract away the differences between windowed
// and windowless visual objects. A windowed visual object implements
// the <mf IVisualWindow::GetWindowHandle> function by returning its
// own window handle. A windowless visual object usually implements
// the <mf IVisualWindow::GetWindowHandle> function by returning the
// handle of its parent. This is used by MVC viewports.
//
//@base public | IQueryGuid

struct __declspec(uuid("722E1FCB-034F-4030-A600-3140A9D23DB4"))
	IVisualWindow : public IQueryGuid
{
	//@cmember,mfunc
	// Return the window handle for this object.
	//@@rdesc Window handle
	//@@end
	/* Return the window handle*/
	virtual HWND GetWindowHandle() = 0;
};

};	// namespace stingray::foundation
};  // namespace stingray

#endif  // #ifndef __VISUALWINDOW_H__
