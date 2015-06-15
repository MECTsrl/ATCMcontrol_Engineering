/////////////////////////////////////////////////////////////////////////////
// CWindowCreateEvent.h : Interface for mouse events.
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
// Authors:      Jeff Boenig
// Description:  Framework independent event model.
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Events/WindowEvents.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

#pragma once

#ifndef __WINDOWEVENTS_H__
#define __WINDOWEVENTS_H__

#include <foundation\SflGuids.h>
#include <foundation\Events\WinEvent.h>

namespace stingray {
namespace foundation {

///////////////////////////////////////////////////////////////////////////
//@doc IWindowCreateEvent | Interface for mouse events.
//
//@class IWindowCreateEvent | Interface to mouse events. This interface derives
// from IWinEvent.
//
//@base public | IWinEvent
//
class __declspec(uuid("1ED09371-40A9-11d3-AF0D-006008AFE059"))
					IWindowCreateEvent : public IWinEvent
{
public:
	//@cmember
	/* Return pointer to CREATESTRUCT structure. */
	virtual LPCREATESTRUCT GetCreateStruct() const = 0;
};

///////////////////////////////////////////////////////////////////////////
//@doc CWindowCreateEvent
//
//@class CWindowCreateEvent | This class encapsulates a Windows size event. The
// LONG parameter of the Windows message contains the width and height
// specified by the size event.
//
//@base public | CWinEventBase
//
class CWindowCreateEvent : public CWinEventBase<IWindowCreateEvent>
{
// Constructor/destructor
public:

	//@cmember
	/* Construct a size event from a message ID, WPARAM, and LPARAM. */
	CWindowCreateEvent(const UINT nMessageID, const WPARAM wParam = 0, const LPARAM lParam = 0L);

// Operations
public:

	//@cmember
	/* Dispatches the event to the given event listener. */
	virtual bool Dispatch(IQueryGuid* pIListener);

	//@cmember
	/* Return pointer to CREATESTRUCT structure. */
	virtual LPCREATESTRUCT GetCreateStruct() const;
};

///////////////////////////////////////////////////////////////////////////
//@doc CWindowDestroyEvent
//
//@class CWindowDestroyEvent | This class encapsulates a Windows size event. The
// LONG parameter of the Windows message contains the width and height
// specified by the size event.
//
//@base public | CWinEventBase
//
class CWindowDestroyEvent : public IWinEvent
{
// Constructor/destructor
public:

	//@cmember
	/* Construct a size event from a message ID, WPARAM, and LPARAM. */
	CWindowDestroyEvent(const WPARAM wParam = 0, const LPARAM lParam = 0L);

// Attributes
protected:
	//@cmember
	/* Reference count. */
	ULONG m_ulRefCount;

// Operations
public:

	//@cmember
	/* Retrieve a pointer to an interface supported by this object. */
	virtual bool QueryGuid(REFGUID guid, void **ppvObj);

	//@cmember
	/* Add a reference to this object. */
	virtual ULONG STDMETHODCALLTYPE AddRef();

	//@cmember
	/* Release a reference to this object. */
	virtual ULONG STDMETHODCALLTYPE Release();

	//@cmember
	/* Dispatches the event to the given event listener. */
	virtual bool Dispatch(IQueryGuid* pIListener);

	//@cmember
	/* Return the message ID. */
	virtual UINT GetMessageID() const;

	//@cmember
	/* Return the WORD parameter. */
	virtual WPARAM GetWParam() const;

	//@cmember
	/* Set the WORD parameter. */
	virtual void SetWParam(const WPARAM wParam);

	//@cmember
	/* Return the LONG parameter. */
	virtual LPARAM GetLParam() const;

	//@cmember
	/* Set the LONG parameter. */
	virtual void SetLParam(const LPARAM lParam);

	//@cmember
	/* Return the message result. */
	virtual LRESULT GetLResult() const;

	//@cmember
	/* Set the message result. */
	virtual void SetLResult(const LRESULT lResult);

};

///////////////////////////////////////////////////////////////////////////
//@doc IWindowSizeEvent | Interface for mouse events.
//
//@class IWindowSizeEvent | Interface to mouse events. This interface derives
// from IWinEvent.
//
//@base public | IWinEvent
//
class __declspec(uuid("DB2FC42C-FE00-47a3-80DF-22DFD4FE5E54"))
					IWindowSizeEvent : public IWinEvent
{
public:
	//@cmember
	/* Return the resizing flag. */
	virtual UINT GetFlag() const = 0;

	//@cmember
	/* Return the width specified by the size message. */
	virtual long GetWidth() const = 0;

	//@cmember
	/* Return the width specified by the size message. */
	virtual long GetHeight() const = 0;
};

///////////////////////////////////////////////////////////////////////////
//@doc CWindowSizeEvent
//
//@class CWindowSizeEvent | This class encapsulates a Windows size event. The
// LONG parameter of the Windows message contains the width and height
// specified by the size event.
//
//@base public | CWinEventBase
//
class CWindowSizeEvent : public CWinEventBase<IWindowSizeEvent>
{
// Constructor/destructor
public:

	//@cmember
	/* Construct a size event from a message ID, WPARAM, and LPARAM. */
	CWindowSizeEvent(const UINT nMessageID, const WPARAM wParam = 0, const LPARAM lParam = 0L);

// Operations
public:

	//@cmember
	/* Dispatches the event to the given event listener. */
	virtual bool Dispatch(IQueryGuid* pIListener);

	//@cmember
	/* Return the resizing flag. */
	virtual UINT GetFlag() const;

	//@cmember
	/* Return the width specified by the size message. */
	virtual long GetWidth() const;

	//@cmember
	/* Return the width specified by the size message. */
	virtual long GetHeight() const;
};

///////////////////////////////////////////////////////////////////////////
//@doc IWindowPaintEvent | Interface for mouse events.
//
//@class IWindowPaintEvent | Interface to mouse events. This interface derives
// from IWinEvent.
//
//@base public | IWinEvent
//
class __declspec(uuid("10C387AD-7462-46a5-9A59-17EAADE1CE15"))
					IWindowPaintEvent : public IWinEvent
{
public:
	//@cmember
	/* Return the device context. */
	virtual HDC GetDC() const = 0;
};

///////////////////////////////////////////////////////////////////////////
//@doc CWindowPaintEvent
//
//@class CWindowPaintEvent | This class encapsulates a Windows paint event.
//
//@base public | CWinEventBase
//
class CWindowPaintEvent : public CWinEventBase<IWindowPaintEvent>
{
// Constructor/destructor
public:

	//@cmember
	/* Construct a size event from a message ID, WPARAM, and LPARAM. */
	CWindowPaintEvent(const UINT nMessageID, const WPARAM wParam = 0, const LPARAM lParam = 0L);

// Operations
public:

	//@cmember
	/* Dispatches the event to the given event listener. */
	virtual bool Dispatch(IQueryGuid* pIListener);

	//@cmember
	/* Return the device context. */
	virtual HDC GetDC() const;
};


///////////////////////////////////////////////////////////////////////////
//@doc IWindowEraseBkgndEvent | Interface for erase background events.
//
//@class IWindowEraseBkgndEvent | Interface to erase background events. This interface derives
// from IWinEvent.
//
//@base public | IWinEvent
//
class __declspec(uuid("512FEF03-0739-4cd1-A468-41E0C67F2A33"))
					IWindowEraseBkgndEvent : public IWinEvent
{
public:
	//@cmember
	/* Return the device context. */
	virtual HDC GetDC() const = 0;
};

///////////////////////////////////////////////////////////////////////////
//@doc CWindowEraseBkgndEvent
//
//@class CWindowEraseBkgndEvent | This class encapsulates a Windows erase background message.
//
//@base public | CWinEventBase
//
class CWindowEraseBkgndEvent : public CWinEventBase<IWindowEraseBkgndEvent>
{
// Constructor/destructor
public:

	//@cmember
	/* Construct an erase background event from a message ID, WPARAM, and LPARAM. */
	CWindowEraseBkgndEvent(const UINT nMessageID, const WPARAM wParam = 0, const LPARAM lParam = 0L);

// Operations
public:

	//@cmember
	/* Dispatches the event to the given event listener. */
	virtual bool Dispatch(IQueryGuid* pIListener);

	//@cmember
	/* Return the device context. */
	virtual HDC GetDC() const;
};


///////////////////////////////////////////////////////////////////////////
//@doc IWindowTimerEvent | Interface for timer events.
//
//@class IWindowTimerEvent | Interface for timer events. This interface derives
// from IWinEvent.
//
//@base public | IWinEvent
//
class __declspec(uuid("3CB65660-6096-11d3-8459-006008BFFE10"))
					IWindowTimerEvent : public IWinEvent
{
public:
	//@cmember
	/* Return the identifier of the timer. */
	virtual UINT GetTimerID() const = 0;
};

///////////////////////////////////////////////////////////////////////////
//@doc CWindowTimerEvent
//
//@class CWindowTimerEvent | This class encapsulates a Windows timer message.
//
//@base public | CWinEventBase
//
class CWindowTimerEvent : public CWinEventBase<IWindowTimerEvent>
{
// Constructor/destructor
public:

	//@cmember
	/* Construct a timer event from a message ID, WPARAM, and LPARAM. */
	CWindowTimerEvent(const UINT nMessageID, const WPARAM wParam = 0, const LPARAM lParam = 0L);

// Operations
public:

	//@cmember
	/* Dispatches the event to the given event listener. */
	virtual bool Dispatch(IQueryGuid* pIListener);

	//@cmember
	/* Return the identifier of the timer. */
	virtual UINT GetTimerID() const;
};

};	// namespace stingray::foundation
};  // namespace stingray

#endif  // #ifndef __WINDOWEVENTS_H__
