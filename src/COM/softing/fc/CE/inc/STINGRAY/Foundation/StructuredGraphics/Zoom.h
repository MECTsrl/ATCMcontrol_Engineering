/////////////////////////////////////////////////////////////////////////////
// Zoom.h : SFL Windowing package 
//
// Stingray Software Foundation Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Jeff Boenig
// Description:  Support for zooming windows
//

#pragma once

#ifndef __ZOOM_H__
#define __ZOOM_H__

#if !defined(_SFL)
#define _SFL
#endif

#include "Foundation\GuidCast.h"

namespace stingray {
namespace foundation {

//////////////////////////////////////////////////////////////////////////
//@doc IZoom
//
//@class IZoom | This interface defines functions for setting the
// magnification of an object that supports zooming. The <mf IZoom::ZoomExtents>
// function does the work of adjusting a device context to account for the
// zoom magnification.
//
//@base public | IQueryGuid

class __declspec(uuid("8407B2B4-4B5E-11d3-AF1B-006008AFE059")) IZoom : public IQueryGuid
{
public:

	//@cmember,mfunc
	// Set logical scaling as percent magnification.
	//@@rdesc Previous magnification setting
	//@@parm Zoom factor for X axis
	//@@parm Zoom factor for Y axis
	//@@end
	/* Set logical scaling as percent magnification*/
	virtual CSize SetMagnification(const int nPctX, const int nPctY) = 0;

	//@cmember,mfunc
	// Get logical scaling as percent magnification.
	//@@rdesc Zoom factor
	//@@end
	/* Get logical scaling as percent magnification*/
	virtual CSize GetMagnification() const = 0;

	//@cmember,mfunc
	// Increase logical scaling by a percentage.
	//@@rdesc Previous magnification setting
	//@@parm Increase in magnification for X axis
	//@@parm Increase in magnification for Y axis
	//@@end
	/* Increase logical scaling by a percentage*/
	virtual CSize IncreaseMagnification(const int nPctX, const int nPctY) = 0;

	//@cmember,mfunc
	// Decrease logical scaling by a percentage.
	//@@rdesc Previous magnification setting
	//@@parm Decrease in magnification for X axis
	//@@parm Decrease in magnification for Y axis
	//@@end
	/* Decrease logical scaling by a percentage*/
	virtual CSize DecreaseMagnification(const int nPctX, const int nPctY) = 0;

	//@cmember,mfunc
	// Adjust window and viewport extents for magnification.
	//@@rdesc void
	//@@parm In,out parameter to receive adjusted extent value
	//@@parm In,out parameter to receive adjusted extent value
	//@@end
	/* Adjust window and viewport extents for magnification*/
	virtual void ZoomExtents(CSize& szWndExt, CSize& szVpExt) const = 0;
};

};	// namespace stingray::foundation
};  // namespace stingray

#endif  // #ifndef __ZOOM_H__
