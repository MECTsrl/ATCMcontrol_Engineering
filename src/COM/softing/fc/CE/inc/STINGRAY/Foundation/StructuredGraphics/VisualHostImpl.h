/////////////////////////////////////////////////////////////////////////////
// VisualHostImpl.h : SFL Model-View-Controller architecture 
//
// Stingray Software Foundation Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Jeff Boenig
// Description:  SFL MVC: Host architecture implementation
//

#pragma once

#ifndef __VISUALHOSTIMPL_H__
#define __VISUALHOSTIMPL_H__

#include <assert.h>
#include <Foundation\StructuredGraphics\VisualHost.h>
#include <Foundation\Graphics\GCCore.h>

namespace stingray {
namespace foundation {

template<typename T>
struct CVisualHostImpl : public IVisualHost
{
	typedef T _hostwin;

	BEGIN_GUID_MAP(CVisualHostImpl<T>)
		GUID_ENTRY(IVisualHost)
	END_GUID_MAP

	CVisualHostImpl():_m_hWnd((static_cast<_hostwin*>(this))->m_hWnd){
		// no implementation
	};

	virtual ~CVisualHostImpl() {
		// no implementation
	};

	//@cmember
	/* Invalidate the specified rectangle within this visual host. */
	virtual void InvalidateRect(const CRect& rect, BOOL bErase = TRUE, BOOL bRepairNow = FALSE)
	{
		bRepairNow;
		assert( ::IsWindow(_m_hWnd) );// error invalid window
		::InvalidateRect(_m_hWnd, rect, bErase);
	}

	//@cmember
	/* Validate the specified rectangle within this visual host. */
	virtual void ValidateRect(const CRect& rect)
	{
		assert( ::IsWindow(_m_hWnd) );// error invalid window
		::ValidateRect(_m_hWnd, rect);
	}

private:
	// Window handle
	HWND& _m_hWnd;
};


};	// namespace stingray::foundation
};  // namespace stingray

#endif  // #ifndef __VISUALHOSTIMPL_H__
