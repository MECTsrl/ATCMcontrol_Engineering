/////////////////////////////////////////////////////////////////////////////
// MFCEventRouter.h
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
// Description:  MFC Event model.
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Events/MFCEventRouter.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

#pragma once

#if !defined(_SFL)
#define _SFL
#endif

#include <foundation\Events\Event.h>
#include <foundation\Events\EventRouter.h>
#include <foundation\Events\EventFactory.h>

namespace stingray {
namespace foundation {

// IMsgHandler - Interface that is compatible with CWnd's message handling functions. IMsgHandler
// serves as a default base for CMFCEventRouter. 
class IMsgHandler
{
public:	
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult) = 0;
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) = 0;
};

////////////////////////////////////////////////////////////////////////////////
// CMFCEventRouter - Acts as a bridge between MFC's command/message handling mechanism and the 
// SFL event routing mechanism. Messages and commands are forwarded to the RouteEvent() method implemented
// by the router class.

template <typename router, typename MsgHandlerbase = IMsgHandler>
class CMFCEventRouter : public MsgHandlerbase
{
public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
	{
		BOOL bHandled = FALSE;
		// Create the event and route it to event listeners.
		CEventFactory* pEventFactory = GetEventFactory();
		IEvent* pIEvent = NULL;

		if (pHandlerInfo != NULL)
		{
			// This message is a request for handler info.
			pIEvent = pEventFactory->CreateCommandQueryEvent(nID);
		}
		else if (nCode == CN_UPDATE_COMMAND_UI)
		{
			// Create a command update UI event.
			CCmdUI* pCmdUI = (CCmdUI*) pExtra;
			pCmdUI;
		}
		else
		{
			// Regular command event.
			pIEvent = pEventFactory->CreateCommandEvent(nID, nCode);
		}

		if (pIEvent != NULL)
		{
			// Route event to event listeners.
			router* pT = static_cast<router*>(this);
			bHandled = pT->RouteEvent(pIEvent);
			pIEvent->Release();
		}

		return bHandled;
	}

	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		BOOL bHandled = FALSE;	
		// Create an event, using the event factory and route it to the event listeners.		
		IEvent* pIEvent = GetEventFactory()->CreateWindowsEvent(message, wParam, lParam);
		if (pIEvent != NULL)
		{
			router* pT = static_cast<router*>(this);
			bHandled = pT->RouteEvent(pIEvent);
			pIEvent->Release();
		}
		
		return bHandled;
	}	

	virtual CEventFactory* GetEventFactory()
	{
		static CEventFactory eventFactory;
		return &eventFactory;
	}
};


};	// namespace stingray::foundation
};  // namespace stingray