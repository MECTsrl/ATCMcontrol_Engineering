/////////////////////////////////////////////////////////////////////////////
// CEventFactory.h : Interface to event factory class.
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
// Author:       Jeff Boenig and Bob Powell
// Description:  Framework independent event model.
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Events/EventFactory.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

#pragma once

#ifndef __EVENTFACTORY_H__
#define __EVENTFACTORY_H__

#include <foundation\Events\Event.h>

namespace stingray {
namespace foundation {

///////////////////////////////////////////////////////////////////////////
//@doc CEventFactory
//
//@class An event factory creates events based on various types of
// input parameters and filter criterion. It consists of filter
// methods and create methods for different categories of events.
// It translates Windows messages into event objects that can be
// passed to event routers and event listeners. It can also be used
// to generate custom events. This class serves as a base class for
// derived event factories, which can provide their own custom
// filtering and creation methods.
//
class CEventFactory
{
// Operations
public:
	//@cmember
	/* Filter out unwanted Windows messages. */
	virtual bool FilterWindowsEvent(UINT message, WPARAM wParam, LPARAM lParam);
	//@cmember
	/* Create a Windows event. */
	virtual IEvent* CreateWindowsEvent(UINT message, WPARAM wParam, LPARAM lParam);

	//@cmember
	/* Filter out unwanted Windows command messages. */
	virtual bool FilterCommandEvent(UINT nID, int nCode);
	//@cmember
	/* Create a Windows command event. */
	virtual IEvent* CreateCommandEvent(UINT nID, int nCode);

	//@cmember
	/* Create a command query event. */
	virtual IEvent* CreateCommandQueryEvent(UINT nID);
};

};	// namespace stingray::foundation
};  // namespace stingray

#endif  // #ifndef __EVENTFACTORY_H__
