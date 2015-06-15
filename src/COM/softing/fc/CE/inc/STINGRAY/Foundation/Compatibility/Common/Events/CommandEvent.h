/////////////////////////////////////////////////////////////////////////////
// CommandEvent.h : Interface for command events.
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
// Authors:      Jeff Boenig and Bob Powell
// Description:  Framework independent event model.
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Compatibility/Common/Events/CommandEvent.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

#ifndef __COMPAT_COMMANDEVENT_H__
#define __COMPAT_COMMANDEVENT_H__

#include "Foundation\Compatibility\Common\CmnGuids.h"
#include "Foundation\Compatibility\Common\Events\WinEvent.h"

namespace rw
{

///////////////////////////////////////////////////////////////////////////
//@doc ICommandEvent
//
//@class ICommandEvent | Interface to a Windows command event. A command
// event contains a command identifier, a notification code, and possibly
// a handle to the control the generated the event.
//
//@base public | IWinEvent
//
class __declspec(uuid("47E1CE34-D500-11d2-8CAB-0010A4F36466"))
				ICommandEvent : public IWinEvent
{
public:
	//@cmember
	/* Return the command ID. */
	virtual UINT GetCommandID() const = 0;

	//@cmember
	/* Return the notify code. */
	virtual int GetNotifyCode() const = 0;

	//@cmember
	/* Return the control handle. */
	virtual HWND GetControlHandle() const = 0;

	//@cmember
	/* Set the command ID. */
	virtual void SetCommandID(const UINT nCommandID) = 0;

	//@cmember
	/* Set the notify code. */
	virtual void SetNotifyCode(const int nNotifyCode) = 0;

	//@cmember
	/* Set the control handle. */
	virtual void SetControlHandle(HWND hCtl) = 0;
};

///////////////////////////////////////////////////////////////////////////
//@doc CommandEvent
//
//@class CommandEvent | This class encapsulates a Windows command event. A
// command event contains a command identifier, a notification code, and
// possibly a handle to the control the generated the event. The WORD
// parameter contains the command ID and notification code. The control
// handle is stored in the LONG parameter.
//
//@base public | WinEvent_T
//
class CommandEvent : public WinEvent_T<ICommandEvent, &IID_ICommandEvent>
{
// Constructor/destructor
public:

	//@cmember
	/* Construct a command event from a command ID, notify code, and control handle. */
	CommandEvent(UINT nCommandID, int nNotifyCode, HWND hCtl = 0);

// Operations
public:

	//@cmember
	/* Dispatches the event to the given event listener. */
	virtual BOOL Dispatch(IUnknown* pIListener);

	//@cmember
	/* Return the command ID. */
	virtual UINT GetCommandID() const;

	//@cmember
	/* Return the notify code. */
	virtual int GetNotifyCode() const;

	//@cmember
	/* Return the control handle. */
	virtual HWND GetControlHandle() const;

	//@cmember
	/* Set the command ID. */
	virtual void SetCommandID(const UINT nCommandID);

	//@cmember
	/* Set the notify code. */
	virtual void SetNotifyCode(const int nNotifyCode);

	//@cmember
	/* Set the control handle. */
	virtual void SetControlHandle(HWND hCtl);
};

///////////////////////////////////////////////////////////////////////////
//@doc ICommandQueryEvent
//
//@class ICommandQueryEvent | A command query event is sent to command
// listeners to find out if a given command is handled. A command query
// event contains a command ID that indicates the command that is being
// queried.
//
//@base public | IEvent
//
class __declspec(uuid("47E1CE35-D500-11d2-8CAB-0010A4F36466"))
				ICommandQueryEvent : public IEvent
{
	//@cmember
	/* Return the command ID. */
	virtual UINT GetCommandID() const = 0;

	//@cmember
	/* Set the command ID. */
	virtual void SetCommandID(const UINT nCommandID) = 0;
};

///////////////////////////////////////////////////////////////////////////
//@doc CommandQueryEvent
//
//@class CommandQueryEvent | A command query event is sent to command
// listeners to find out if a given command is handled. The command query
// only contains the command ID for a given command.
//
//@base public | ICommandQueryEvent
//
class CommandQueryEvent : public ICommandQueryEvent
{
// Constructor/destructor
public:

	//@cmember
	/* Construct a command query event from a command ID. */
	CommandQueryEvent(UINT nCommandID);

// Attributes
protected:
	//@cmember
	/* Reference count. */
	unsigned long m_ulRefCount;
	//@cmember
	/* Command ID to query. */
	UINT m_nCommandID;

// Operations
public:

	//@cmember
	/* Retrieve a pointer to an interface supported by this object. */
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void ** ppvObject);
	//@cmember
	/* Add a reference to this object. */
	virtual ULONG STDMETHODCALLTYPE AddRef();
	//@cmember
	/* Release a reference to this object. */
	virtual ULONG STDMETHODCALLTYPE Release();

	//@cmember
	/* Dispatches the event to the given event listener. */
	virtual BOOL Dispatch(IUnknown* pIListener);

	//@cmember
	/* Return the command ID. */
	virtual UINT GetCommandID() const;
	//@cmember
	/* Set the command ID. */
	virtual void SetCommandID(const UINT nCommandID);
};

};  // end namespace rw

#endif  // __COMPAT_COMMANDEVENT_H__
