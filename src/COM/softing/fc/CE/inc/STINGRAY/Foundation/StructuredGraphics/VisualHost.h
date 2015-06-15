/////////////////////////////////////////////////////////////////////////////
// VisualHost.h : SFL Model-View-Controller architecture 
//
// Stingray Software Foundation Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Jeff Boenig
// Description:  SFL MVC: Visual host interface definition
//


#pragma once

#ifndef __VISUALHOST_H__
#define __VISUALHOST_H__

#include <foundation\guidcast.h>
#include <foundation\Graphics\GcCore.h>

namespace stingray {
namespace foundation {

///////////////////////////////////////////////////////////////////////////
//@doc IVisualHost
//
//@class IVisualHost | Interface to a visual host object. A visual host
// is a container for visual objects. This interface provides services
// to the visual objects in the container.
//
//@base public | IQueryGuid

struct __declspec(uuid("9CB45634-684F-459a-8DD3-492FB3E40FF4"))
	IVisualHost : public IQueryGuid
{
	//@cmember
	/* Invalidate the specified rectangle within this visual host. */
	virtual void InvalidateRect(const CRect& rect, BOOL bErase = TRUE, BOOL bRepairNow = FALSE) = 0;
	//@cmember
	/* Validate the specified rectangle within this visual host. */
	virtual void ValidateRect(const CRect& rect) = 0;
};

};	// namespace stingray::foundation
};  // namespace stingray

#endif  // #ifndef __VISUALHOST_H__
