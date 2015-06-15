/////////////////////////////////////////////////////////////////////////////
// EventRouterMap.h
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
// Authors:      
// Description:  Framework independent event model.
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Events/EventRouterMap.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

#pragma once

#ifndef __EVENTROUTERMAP_H__
#define __EVENTROUTERMAP_H__

#if !defined(_SFL)
#define _SFL
#endif

#if defined(_SFL_ATL_SUPPORT)

#include <foundation\Events\Event.h>
#include <foundation\Events\EventRouter.h>
#include <foundation\Events\EventFactory.h>

namespace stingray {
namespace foundation {

////////////////////////////////////////////////////////////////////////////////
// CEventRouterMapWrapperBase - Derives from the event router template parameter
// and mixes in a CMessageMap. Messages passed to the message map are forwarded
// to the RouteEvent() method.

template <typename routerbase_t>
class CEventRouterMapWrapper : public routerbase_t, public CMessageMap
{
public:
	// BOOL Required due to override
	virtual BOOL ProcessWindowMessage( HWND, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT&, DWORD dwMsgMapID = 0 )
	{
		dwMsgMapID = 0;
		bool bHandled = false;

		IEvent* pIEvent = GetEventFactory()->CreateWindowsEvent(uMsg, wParam, lParam);
		if (pIEvent != NULL)
		{
			bHandled = RouteEvent(pIEvent);
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

////////////////////////////////////////////////////////////////////////////////
// CEventRouterMap - Used to mixin a CMessageMap into a class that
// supports the IEventRouter interface.  Template parameter is the derived
// class type.  This class downcasts to the derived class type in order to
// call the RouteEvent() method.

template <typename T>
class CEventRouterMap : public CMessageMap
{
public:
	virtual BOOL ProcessWindowMessage( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID = 0 )
	{
		// To get rid of the warning... 
		hWnd;
		dwMsgMapID;
		lResult;

		bool bHandled = FALSE;
		T* pT = static_cast<T*>(this);

		IEvent* pIEvent = GetEventFactory()->CreateWindowsEvent(uMsg, wParam, lParam);
		if (pIEvent != NULL)
		{
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

#endif  // #if defined(_SFL_ATL_SUPPORT)

#endif  // #ifndef __EVENTROUTERMAP_H__
