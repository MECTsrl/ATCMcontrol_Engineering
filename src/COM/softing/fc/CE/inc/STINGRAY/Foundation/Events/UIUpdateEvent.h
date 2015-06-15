/////////////////////////////////////////////////////////////////////////////
// UIUpdateEvent.h : Interface for UIUpdate events.
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
// Authors:      Jeff Boenig and George Shepherd
// Description:  Framework independent event model.
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Events/UIUpdateEvent.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

#pragma once

#ifndef __UIUPDATEEVENT_H__
#define __UIUPDATEEVENT_H__

#include <Foundation\SflGuids.h>
#include <Foundation\SflCommon.h>
#include <Foundation\Events\Event.h>
#include <Foundation\Events\UIUpdateElement.h>

namespace stingray {
namespace foundation {

///////////////////////////////////////////////////////////////////////////
//@doc IUIUpdateEvent
//
//@class IUIUpdateEvent | Interface to a Windows UIUpdate event. A UIUpdate
// event contains a UIUpdate identifier, a notification code, and possibly
// a handle to the control the generated the event.
//
//@base public | IWinEvent
//
class __declspec(uuid("EB4A8E29-4C6A-4a81-87D5-1A8D74C39462"))
				IUIUpdateEvent : public IEvent
{
public:
	//@cmember
	/* Return the command ID. */
	virtual UINT GetCommandID() const = 0;

	//@cmember
	/* Return the update element. */
	virtual IUIUpdateElement* GetUpdateElement() const = 0;

	//@cmember
	/* Set the UIUpdate ID. */
	virtual void SetCommandID(const UINT nCommandID) = 0;

	//@cmember
	/* Set the update element. */
	virtual void SetUpdateElement(IUIUpdateElement* pIUpdateElement) = 0;
};

///////////////////////////////////////////////////////////////////////////
//@doc CUIUpdateEvent
//
//@class CUIUpdateEvent | This class encapsulates a Windows UIUpdate event. A
// UIUpdate event contains a UIUpdate identifier, a notification code, and
// possibly a handle to the control the generated the event. The WORD
// parameter contains the UIUpdate ID and notification code. The control
// handle is stored in the LONG parameter.
//
//@base public | CWinEventBase
//
class CUIUpdateEvent : public IUIUpdateEvent
{
// Constructor/destructor
public:
	//@cmember
	/* Construct a UIUpdate event from a command ID, notify code, and update element. */
	CUIUpdateEvent(UINT nCommandID, IUIUpdateElement* pIUpdateElement = 0);

// Attributes
protected:
	//@cmember
	/* Reference count. */
	unsigned long m_ulRefCount;
	//@cmember
	/* Command ID to query. */
	UINT m_nCommandID;
	//@cmember
	/* Pointer to the update element. */
	IUIUpdateElement* m_pIUpdateElement;

// Operations
public:
	//@cmember
	/* Dispatches the event to the given event listener. */
	virtual bool Dispatch(IQueryGuid* pIListener);

	//@cmember
	/* Return the command ID. */
	virtual UINT GetCommandID() const;

	//@cmember
	/* Return the update element. */
	virtual IUIUpdateElement* GetUpdateElement() const;

	//@cmember
	/* Set the UIUpdate ID. */
	virtual void SetCommandID(const UINT nCommandID);

	//@cmember
	/* Set the update element. */
	virtual void SetUpdateElement(IUIUpdateElement* pIUpdateElement);

	// The GUID map implements the QueryGuid function
	BEGIN_GUID_MAP(CUIUpdateEvent)
		GUID_ENTRY(IUIUpdateEvent)
		GUID_ENTRY(IEvent)
		GUID_ENTRY(IQueryGuid)
		GUID_ENTRY(IRefCount)
	END_GUID_MAP

	//@cmember
	/* Add a reference to this object. */
	virtual ULONG STDMETHODCALLTYPE AddRef();
	//@cmember
	/* Release a reference to this object. */
	virtual ULONG STDMETHODCALLTYPE Release();
};

};	// namespace stingray::foundation
};  // namespace stingray

#endif  // #ifndef __UIUPDATEEVENT_H__
