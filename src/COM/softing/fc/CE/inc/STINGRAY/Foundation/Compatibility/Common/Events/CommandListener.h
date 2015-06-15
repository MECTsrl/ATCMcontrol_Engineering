/////////////////////////////////////////////////////////////////////////////
// CommandListener.h : Declares command listener interface and adapter class.
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
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Compatibility/Common/Events/CommandListener.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

#ifndef __COMPAT_COMMANDLISTENER_H__
#define __COMPAT_COMMANDLISTENER_H__

#include "Foundation\Compatibility\Common\CmnGuids.h"

#ifndef __COMPAT_EVENTLISTENER_H__
#include "Foundation\Compatibility\Common\Events\EventListener.h"
#endif

namespace rw
{

///////////////////////////////////////////////////////////////////////////
//@doc ICommandListener
//
//@class ICommandListener | Interface to handling command events. Command
// event listeners map command and command query events onto virtual member
// functions.
//
//@base public | IEventListener
//
class __declspec(uuid("47E1CE3A-D500-11d2-8CAB-0010A4F36466"))
				ICommandListener : public IEventListener
{
public:
	//@cmember
	/* Called when a command query event is received by the command listener. */
	virtual BOOL OnCommandQuery(UINT nID) = 0;
	//@cmember
	/* Called when a command event is received by the command listener. */
	virtual BOOL OnCommand(UINT nID, int nNotifyCode) = 0;
};

///////////////////////////////////////////////////////////////////////////
//@doc CommandAdapter
//
//@class CommandAdapter | The command adapter class supplies a default
// implementation of the <c ICommandListener> interface. The
// <mf CommandAdapter::HandleEvent> function deciphers the event and maps
// it to a member function for handling. This class supplies a default
// implementation for all handler messages in the ICommandListener
// interface that always return FALSE. Command listeners can be derived
// from this class that override and implement only the handlers they are
// interested in.
//
//@base public | EventListener_T
//
class CommandAdapter : public EventListener_T<ICommandListener, &IID_ICommandListener>
{
// Operations
public:
	//@cmember
	/* Called when a command query event is received by the command listener. */
	virtual BOOL OnCommandQuery(UINT nID);

	//@cmember
	/* Called when a command event is received by the command listener. */
	virtual BOOL OnCommand(UINT nID, int nNotifyCode);
};

};  // end namespace rw

#endif // __COMPAT_COMMANDLISTENER_H__
