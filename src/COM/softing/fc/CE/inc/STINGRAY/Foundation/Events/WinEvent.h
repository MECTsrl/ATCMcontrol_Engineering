/////////////////////////////////////////////////////////////////////////////
// WinEvent.h : Interface to window events.
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
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Events/WinEvent.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

#pragma once

#ifndef __WINEVENT_H__
#define __WINEVENT_H__

#if !defined(_SFL)
#define _SFL
#endif

#include <foundation\Events\Event.h>
#include <foundation\Events\EventListener.h>

#if (_MFC_VER <= 0x0420)
#pragma warning (disable : 4097)
#endif // _MFC_VER <= 0x0420

namespace stingray {
namespace foundation {

///////////////////////////////////////////////////////////////////////////
//@doc IWinEvent
//
//@class IWinEvent | This class provides an interface for all Windows
// events. A Windows event consists of a message ID, a WORD parameter, and
// a LONG parameter.
//
//@base public | IEvent
//
interface __declspec(uuid("BAA3AE89-5B14-4e39-9305-264A4D681773"))
				IWinEvent : public IEvent
{
	//@cmember
	/* Return the message ID. */
	virtual UINT GetMessageID() const = 0;

	//@cmember
	/* Return the WORD parameter. */
	virtual WPARAM GetWParam() const = 0;

	//@cmember
	/* Set the WORD parameter. */
	virtual void SetWParam(const WPARAM wParam) = 0;

	//@cmember
	/* Return the LONG parameter. */
	virtual LPARAM GetLParam() const = 0;

	//@cmember
	/* Set the LONG parameter. */
	virtual void SetLParam(const LPARAM lParam) = 0;

	//@cmember
	/* Return the message result. */
	virtual LRESULT GetLResult() const = 0;

	//@cmember
	/* Set the message result. */
	virtual void SetLResult(const LRESULT lResult) = 0;
};

///////////////////////////////////////////////////////////////////////////
//@doc CWinEventBase
//
//@class CWinEventBase | This class encapsulates a Windows event as an object.
// A Windows event consists of a message ID, a WORD parameter, and a LONG
// parameter. This class serves as a base class for specific types of
// Windows events.
//
//@mfunc | CWinEventBase | CWinEventBase | Constructs a CWinEventBase object.
//@parm const UINT | nMessageID | Windows message ID (WM_*).
//@parmopt WPARAM | wParam | 0 | Word parameter for windows message.
//@parmopt LPARAM | lParam | 0 | Long parameter for windows message.
//
//@mfunc UINT | CWinEventBase | GetMessageID | Return the message ID.
//@rdesc Windows message ID.
//
//@mfunc UINT | CWinEventBase | GetWParam | Return the WORD parameter.
//@rdesc WORD parameter for Windows message.
//
//@mfunc UINT | CWinEventBase | SetWParam | Set the WORD parameter.
//@rdesc void
//@parm const WPARAM | wParam | WORD parameter for Windows message.
//
//@mfunc UINT | CWinEventBase | GetLParam | Return the LONG parameter.
//@rdesc LONG parameter for Windows message.
//
//@mfunc UINT | CWinEventBase | SetLParam | Set the LONG parameter.
//@rdesc void
//@parm const LPARAM | lParam | LONG parameter for Windows message.
//
template<class interfacebase_t>
class CWinEventBase : public interfacebase_t
{
protected:
	//@cmember
	/* Reference count. */
	ULONG m_ulRefCount;
	//@cmember
	/* Windows message ID. */
	UINT m_nMessageID;
	//@cmember
	/* WORD parameter for message. */
	WPARAM m_wParam;
	//@cmember
	/* LONG parameter for message. */
	LPARAM m_lParam;
	//@cmember
	/* Message result. */
	LRESULT m_lResult;

public:

	//@cmember
	/* Construct a windows event from a message ID, WPARAM, and LPARAM. */
	CWinEventBase(const UINT nMessageID, const WPARAM wParam = 0, const LPARAM lParam = 0L):
		m_nMessageID(nMessageID), m_wParam(wParam), m_lParam(lParam), m_lResult(0)
	{
		m_ulRefCount = 1;
	}

	//@cmember
	/* Retrieve a pointer to an interface supported by this object. */
	virtual bool QueryGuid(REFGUID guid, void **ppvObj)
	{
		void *rvP = NULL;

		if (guid == __uuidof(IEvent))
		{
			rvP = static_cast<IEvent*>(this);
		}
		else if (guid == __uuidof(IWinEvent))
		{
			rvP = static_cast<IWinEvent*>(this);
		}
		else if(guid == __uuidof(interfacebase_t))
		{
			rvP = static_cast<interfacebase_t*>(this);
		}

		*ppvObj = rvP;

		return (rvP != NULL);
	}

	//@cmember
	/* Add a reference to this object. */
	ULONG STDMETHODCALLTYPE AddRef()
	{
		return ++m_ulRefCount;
	}

	//@cmember
	/* Release a reference to this object. */
	ULONG STDMETHODCALLTYPE Release()
	{
		ULONG ulRefCount = --m_ulRefCount;
		if (m_ulRefCount == 0)
		{
			delete this;
		}
		return ulRefCount;
	}


	virtual bool Dispatch(IQueryGuid* pQG)
	{
		IEventListener* pListener = guid_cast<IEventListener*>(pQG);
		bool bHandled = false;
		if (pListener)
		{
			bHandled = pListener->HandleEvent(this);
		}
		return bHandled;
	}


	//@cmember
	/* Return the message ID. */
	virtual UINT GetMessageID() const
	{
		return m_nMessageID;
	}

	//@cmember
	/* Return the WORD parameter. */
	virtual WPARAM GetWParam() const
	{
		return m_wParam;
	}

	//@cmember
	/* Set the WORD parameter. */
	virtual void SetWParam(const WPARAM wParam)
	{
		m_wParam = wParam;
	}

	//@cmember
	/* Return the LONG parameter. */
	virtual LPARAM GetLParam() const
	{
		return m_lParam;
	}

	//@cmember
	/* Set the LONG parameter. */
	virtual void SetLParam(const LPARAM lParam)
	{
		m_lParam = lParam;
	}

	//@cmember
	/* Return the message result. */
	virtual LRESULT GetLResult() const
	{
		return m_lResult;
	}

	//@cmember
	/* Set the message result. */
	virtual void SetLResult(const LRESULT lResult)
	{
		m_lResult = lResult;
	}
};

typedef CWinEventBase<IWinEvent> WinEvent;

};	// namespace stingray::foundation
};  // namespace stingray

#endif  // #ifndef __WINEVENT_H__
