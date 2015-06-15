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
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Compatibility/Common/Events/WinEvent.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

#ifndef __COMPAT_WINEVENT_H__
#define __COMPAT_WINEVENT_H__

#include "Foundation\Compatibility\Common\Events\Event.h"

#if (_MFC_VER <= 0x0420)
#pragma warning (disable : 4097)
#endif // _MFC_VER <= 0x0420

namespace rw
{

///////////////////////////////////////////////////////////////////////////
//@doc IWinEvent
//
//@class IWinEvent | This class provides an interface for all Windows
// events. A Windows event consists of a message ID, a WORD parameter, and
// a LONG parameter.
//
//@base public | IEvent
//
class __declspec(uuid("47E1CE32-D500-11d2-8CAB-0010A4F36466"))
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
};

///////////////////////////////////////////////////////////////////////////
//@doc WinEvent_T
//
//@class WinEvent_T | This class encapsulates a Windows event as an object.
// A Windows event consists of a message ID, a WORD parameter, and a LONG
// parameter. This class serves as a base class for specific types of
// Windows events.
//
//@mfunc | WinEvent_T | WinEvent_T | Constructs a WinEvent_T object.
//@parm const UINT | nMessageID | Windows message ID (WM_*).
//@parmopt WPARAM | wParam | 0 | Word parameter for windows message.
//@parmopt LPARAM | lParam | 0 | Long parameter for windows message.
//
//@mfunc UINT | WinEvent_T | GetMessageID | Return the message ID.
//@rdesc Windows message ID.
//
//@mfunc UINT | WinEvent_T | GetWParam | Return the WORD parameter.
//@rdesc WORD parameter for Windows message.
//
//@mfunc UINT | WinEvent_T | SetWParam | Set the WORD parameter.
//@rdesc void
//@parm const WPARAM | wParam | WORD parameter for Windows message.
//
//@mfunc UINT | WinEvent_T | GetLParam | Return the LONG parameter.
//@rdesc LONG parameter for Windows message.
//
//@mfunc UINT | WinEvent_T | SetLParam | Set the LONG parameter.
//@rdesc void
//@parm const LPARAM | lParam | LONG parameter for Windows message.
//
template<class interfacebase_t, const IID* piid>
class WinEvent_T : public interfacebase_t
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

public:

	//@cmember
	/* Construct a windows event from a message ID, WPARAM, and LPARAM. */
	WinEvent_T(const UINT nMessageID, const WPARAM wParam = 0, const LPARAM lParam = 0L)
	{
		m_nMessageID = nMessageID;
		m_wParam = wParam;
		m_lParam = lParam;
		m_ulRefCount = 1;
	}

	//@cmember
	/* Retrieve a pointer to an interface supported by this object. */
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid,void **ppvObj)
	{
		void *rvP = NULL;

		if (riid == IID_IUnknown)
			rvP = static_cast<IUnknown *>(this);
		else if (riid == IID_IEvent)
			rvP = static_cast<IEvent *>(this);
		else if (riid == IID_IWinEvent)
			rvP = static_cast<IWinEvent *>(this);
		else if(riid == *piid)
			rvP = static_cast<interfacebase_t *>(this);

		if(rvP != NULL)
		{
			AddRef();
		}

		*ppvObj = rvP;

		return (rvP == NULL ? E_NOINTERFACE : S_OK);
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
			delete this;
		return ulRefCount;
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
};

};  // end namespace rw

#endif  // __COMPAT_WINEVENT_H__
