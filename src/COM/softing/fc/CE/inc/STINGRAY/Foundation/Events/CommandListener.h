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
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Events/CommandListener.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

#pragma once

#ifndef __COMMANDLISTENER_H__
#define __COMMANDLISTENER_H__

#include <foundation\SflGuids.h>
#include <foundation\Events\EventListener.h>

namespace stingray {
namespace foundation {

///////////////////////////////////////////////////////////////////////////
//@doc ICommandListener
//
//@class ICommandListener | Interface to handling command events. Command
// event listeners map command and command query events onto virtual member
// functions.
//
//@base public | IEventListener
//
class __declspec(uuid("81CEDD2C-B2F0-4702-AA2F-D911897F5F33"))
				ICommandListener : public IEventListener
{
public:
	//@cmember
	/* Called when a command query event is received by the command listener. */
	virtual bool OnCommandQuery(UINT nID) = 0;
	//@cmember
	/* Called when a command event is received by the command listener. */
	virtual bool OnCommand(UINT nID, int nNotifyCode) = 0;
};

///////////////////////////////////////////////////////////////////////////
//@doc CCommandAdapter
//
//@class CCommandAdapter | The command adapter class supplies a default
// implementation of the <c ICommandListener> interface. The
// <mf CCommandAdapter::HandleEvent> function deciphers the event and maps
// it to a member function for handling. This class supplies a default
// implementation for all handler messages in the ICommandListener
// interface that always return FALSE. Command listeners can be derived
// from this class that override and implement only the handlers they are
// interested in.
//
//@base public | CEventListenerBase
//
class CCommandAdapter : public CEventListenerBase<ICommandListener>
{
// Operations
public:
	//@cmember
	/* Called when a command query event is received by the command listener. */
	virtual bool OnCommandQuery(UINT nID);

	//@cmember
	/* Called when a command event is received by the command listener. */
	virtual bool OnCommand(UINT nID, int nNotifyCode);
};

///////////////////////////////////////////////////////////////////
// COMMAND map macros for implementing the OnCommand() function

#define BEGIN_COMMAND_MAP(CLASSNAME)  \
	virtual bool OnCommand(UINT nID, int nNotifyCode) { bool bHandled = false; \

#define COMMAND_ENTRY(CMDID,CMDHANDLER)  \
	if (nID == CMDID) { bHandled = CMDHANDLER(nID, nNotifyCode); } \

#define COMMAND_CHAIN_ENTRY(T)  \
	bHandled = T::OnCommand(nID, nNotifyCode); \

#define END_COMMAND_MAP  return bHandled; }

};	// namespace stingray::foundation
};  // namespace stingray

#endif  // #ifndef __COMMANDLISTENER_H__
