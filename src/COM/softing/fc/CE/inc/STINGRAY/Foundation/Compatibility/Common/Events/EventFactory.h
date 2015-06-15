/////////////////////////////////////////////////////////////////////////////
// EventFactory.h : Interface to event factory class.
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
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Compatibility/Common/Events/EventFactory.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

#ifndef __COMPAT_EVENTFACTORY_H__
#define __COMPAT_EVENTFACTORY_H__

#ifndef __COMPAT_EVENT_H__
#include "Foundation\Compatibility\Common\Events\Event.h"
#endif

namespace rw
{

///////////////////////////////////////////////////////////////////////////
//@doc EventFactory
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
class EventFactory
{
// Operations
public:
	//@cmember
	/* Filter out unwanted Windows messages. */
	virtual BOOL FilterWindowsEvent(UINT message, WPARAM wParam, LPARAM lParam);
	//@cmember
	/* Create a Windows event. */
	virtual IEvent* CreateWindowsEvent(UINT message, WPARAM wParam, LPARAM lParam);

	//@cmember
	/* Filter out unwanted Windows command messages. */
	virtual BOOL FilterCommandEvent(UINT nID, int nCode);
	//@cmember
	/* Create a Windows command event. */
	virtual IEvent* CreateCommandEvent(UINT nID, int nCode);

	//@cmember
	/* Create a command query event. */
	virtual IEvent* CreateCommandQueryEvent(UINT nID);
};

};  // end namespace rw

#endif // __COMPAT_EVENTFACTORY_H__
