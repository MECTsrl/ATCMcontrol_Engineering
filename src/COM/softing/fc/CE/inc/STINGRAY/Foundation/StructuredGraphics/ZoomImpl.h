/////////////////////////////////////////////////////////////////////////////
// ZoomImpl.h : SFL Windowing package 
//
// Stingray Software Foundation Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Jeff Boenig
// Description:  Support for zooming windows
//

#pragma once

#ifndef __ZOOMIMPL_H__
#define __ZOOMIMPL_H__

#if !defined(_SFL)
#define _SFL
#endif

#include "Foundation\GuidCast.h"
#include "Foundation\StructuredGraphics\Zoom.h"

namespace stingray {
namespace foundation {

//////////////////////////////////////////////////////////////////////////
//@doc IZoomImpl
//
//@class IZoomImpl | This class provides a default implementation of the
// <c IZoom> interface.
//
//@base public | IZoom

class IZoomImpl : public IZoom
{
// Constructors/destructor
public:
	//@cmember,mfunc
	// Default constructor for IZoomImpl
	//@@end
	/* Default constructor for IZoomImpl*/
	IZoomImpl() :
		m_nZoomPctX(100),
		m_nZoomPctY(100)
	{
	}

// Attributes
protected:
	//@cmember
	/* Magnification value for X axis*/
	int m_nZoomPctX;
	//@cmember
	/* Magnification value for Y axis*/
	int m_nZoomPctY;

// Operations
public:

	// GUID map implements the QueryGuid() function
	BEGIN_GUID_MAP(IZoomImpl)
		GUID_ENTRY(IQueryGuid)
		GUID_ENTRY(IZoom)
	END_GUID_MAP

	//@cmember,mfunc
	// Set logical scaling as percent magnification.
	//@@rdesc Previous magnification setting
	//@@parm Zoom factor for X axis
	//@@parm Zoom factor for Y axis
	//@@end
	/* Set logical scaling as percent magnification*/
	virtual CSize SetMagnification(const int nPctX, const int nPctY)
	{
		CSize szPrevMag(m_nZoomPctX, m_nZoomPctY);
		m_nZoomPctX = nPctX;
		m_nZoomPctY = nPctY;
		return szPrevMag;
	}

	//@cmember,mfunc
	// Get logical scaling as percent magnification.
	//@@rdesc Zoom factor
	//@@end
	/* Get logical scaling as percent magnification*/
	virtual CSize GetMagnification() const
	{
		return CSize(m_nZoomPctX, m_nZoomPctY);
	}

	//@cmember,mfunc
	// Increase logical scaling by a percentage.
	//@@rdesc Previous magnification setting
	//@@parm Increase in magnification for X axis
	//@@parm Increase in magnification for Y axis
	//@@end
	/* Increase logical scaling by a percentage*/
	virtual CSize IncreaseMagnification(const int nPctX, const int nPctY)
	{
		CSize szMag(GetMagnification());
		szMag.cx += nPctX;
		szMag.cy += nPctY;
		return SetMagnification(szMag.cx, szMag.cy);
	}

	//@cmember,mfunc
	// Decrease logical scaling by a percentage.
	//@@rdesc Previous magnification setting
	//@@parm Decrease in magnification for X axis
	//@@parm Decrease in magnification for Y axis
	//@@end
	/* Decrease logical scaling by a percentage*/
	virtual CSize DecreaseMagnification(const int nPctX, const int nPctY)
	{
		CSize szMag(GetMagnification());
		szMag.cx -= nPctX;
		szMag.cy -= nPctY;
		return SetMagnification(szMag.cx, szMag.cy);
	}

	//@cmember,mfunc
	// Adjust window and viewport extents for magnification.
	//@@rdesc void
	//@@parm In,out parameter to receive adjusted extent value
	//@@parm In,out parameter to receive adjusted extent value
	//@@end
	/* Adjust window and viewport extents for magnification*/
	virtual void ZoomExtents(CSize& szWndExt, CSize& szVpExt) const
	{
		// routine breaks down if you scale beyond 1000%. 
		// need data normalization.
		// should really check zoom percentage to avoid invalid floating point numbers

		float fScaleWidth = 100.0f / m_nZoomPctX;
		float fScaleHeight = 100.0f / m_nZoomPctY;

		szWndExt.cx = static_cast<long>(szWndExt.cx * fScaleWidth * 1000);
		szWndExt.cx = (szWndExt.cx < 1) ? (1) : szWndExt.cx;
		szWndExt.cy = static_cast<long>(szWndExt.cy * fScaleHeight * 1000);
		szWndExt.cy = (szWndExt.cy < 1) ? (1) : szWndExt.cy;

		szVpExt.cx *= 1000;
		szVpExt.cy *= 1000;
	}
};

};	// namespace stingray::foundation
};  // namespace stingray

#endif  // #ifndef __ZOOMIMPL_H__
