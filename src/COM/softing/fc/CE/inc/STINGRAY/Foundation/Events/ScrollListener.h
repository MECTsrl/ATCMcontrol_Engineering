/////////////////////////////////////////////////////////////////////////////
// ScrollListener.h : Declares mouse listener interface and adapter class.
//
// Stingray Software Extension Classes
// Copyright (C) 1999 Stingray Software Inc,
// All rights reserved.
//
// This source code is only intended as a supplement to
// the Objective Toolkit Class Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding
// the Objective Toolkit product.
//
// Author:       Jeff Boenig
// Description:  Framework independent event model.
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Events/ScrollListener.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

#pragma once

#ifndef __SCROLLLISTENER_H__
#define __SCROLLLISTENER_H__

#include <foundation\SflGuids.h>
#include <foundation\Events\EventListener.h>

namespace stingray {
namespace foundation {

///////////////////////////////////////////////////////////////////////////
//@doc IScrollListener
//
//@class IScrollListener | Interface to handling scroll events. Scroll event
// listeners map various scroll events onto the virtual member functions for
// handling.
//
//@mfunc bool | IScrollListener | OnHScroll | Handle horizontal scroll events.
//@rdesc TRUE if event is handled; otherwise FALSE.
//@parm int | nScrollCode | Indicates the type of movement (line, page, thumb)
//@parm short | nScrollPos | The position the scrollbar has been moved to.
//@parm HWND | hScrollWnd | Window handle of scrollbar that caused the event.
//
//@mfunc bool | IScrollListener | OnVScroll | Handle vertical scroll events.
//@rdesc TRUE if event is handled; otherwise FALSE.
//@parm int | nScrollCode | Indicates the type of movement (line, page, thumb)
//@parm short | nScrollPos | The position the scrollbar has been moved to.
//@parm HWND | hScrollBar | Window handle of scrollbar that caused the event.
//
//@base public | IEventListener
//
class __declspec(uuid("DF46F837-8A05-4d88-9E4B-263BACD8B5C5"))
				IScrollListener : public IEventListener
{
public:
	//@cmember
	/* Handle horizontal scroll events. */
	virtual bool OnHScroll(int nScrollCode, short nScrollPos, HWND hScrollBar) = 0;
	//@cmember
	/* Handle vertical scroll events. */
	virtual bool OnVScroll(int nScrollCode, short nScrollPos, HWND hScrollBar) = 0;
};

///////////////////////////////////////////////////////////////////////////
//@doc CScrollAdapter
//
//@class CScrollAdapter | 
//
//@base public | CEventListenerBase
//
class CScrollAdapter : public CEventListenerBase<IScrollListener>
{
// Operations
public:
	//@cmember
	/* Handle horizontal scroll events. */
	virtual bool OnHScroll(int nScrollCode, short nScrollPos, HWND hScrollBar)
	{
		nScrollCode;
		nScrollPos;
		hScrollBar;
		return false;
	}

	//@cmember
	/* Handle vertical scroll events. */
	virtual bool OnVScroll(int nScrollCode, short nScrollPos, HWND hScrollBar)
	{
		nScrollCode;
		nScrollPos;
		hScrollBar;
		return false;
	}
};

};	// namespace stingray::foundation
};  // namespace stingray

#endif  // #ifndef __SCROLLLISTENER_H__
