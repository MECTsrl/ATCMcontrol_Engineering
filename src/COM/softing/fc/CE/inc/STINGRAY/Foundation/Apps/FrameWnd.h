/////////////////////////////////////////////////////////////////////////////
// FrameWnd.h
//
// Stingray Software Windows Template Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//



#pragma once

#if !defined(_SFL)
#define _SFL
#endif


#include <Foundation\Apps\ContainerWnd.h>

namespace stingray {
namespace foundation {


// CS_VREDRAW and CS_HREDRAW class styles cause flicker

#define DECLARE_FRAME_WND_CLASS(FrameClassName) \
static CWndClassInfo& GetWndClassInfo() \
{ \
	static CWndClassInfo wc = \
	{ \
		{ sizeof(WNDCLASSEX), CS_DBLCLKS, StartWindowProc, \
		  0, 0, NULL, NULL, NULL, (HBRUSH)(COLOR_WINDOW + 1), NULL, FrameClassName, NULL }, \
		NULL, NULL, IDC_ARROW, TRUE, 0, _T("") \
	}; \
	return wc; \
}


// Added for code symmetry, 

#define DECLARE_FRAME_WND_CLASS_EX(FrameClassName, style, bkgnd) \
	DECLARE_WND_CLASS_EX(FrameClassName, style, bkgnd)

#define DECLARE_FRAME_WND_SUPERCLASS(FrameClassName, OrigWndClassName) \
	DECLARE_WND_SUPERCLASS(FrameClassName, OrigWndClassName)


/////////////////////////////////////////////////////////////////////////////
// Main Frame Window

/////////////////////////////
// AutoDuck tag block for CFrameWindowImpl
//
//@doc CFrameWindowImpl

//@mdata  HMENU | CFrameWindowImpl | m_hMenu | This window's menu handle

//@mdata  HACCEL | CFrameWindowImpl | m_hAccTable | This window's accelerator table

//@mfunc | CFrameWindowImpl | CFrameWindowImpl | Construct a frame window

//@mfunc LRESULT | CFrameWindowImpl | OnInitMenuPopup | called whenever the menu is about to be shown
//@parm UINT | uint | SW_ constant specifying how to show the window 
//@rdesc New reference count value.
//@comm called whenever the menu is about to be shown

//@mfunc LRESULT | CFrameWindowImpl | UpdateCommandUI | called whenever the menu is about to be shown
//@parm UINT | uint | COMMAND ID to handle 
//@parm WPARAM | wparam | WPARAM parameter
//@parm LPARAM | lparam | LPARAM parameter
//@parm BOOL& | rb | Was the function handled?
//@rdesc result of function
//@comm called whenever the menu is about to be shown

//@mfunc LRESULT | CFrameWindowImpl | OnDestroy | Called when the window is being destroyed
//@parm UINT | uint | COMMAND ID to handle 
//@parm WPARAM | wparam | WPARAM parameter
//@parm LPARAM | lparam | LPARAM parameter
//@parm BOOL& | rb | Was the function handled?
//@rdesc result of function
//@comm called whenever the window is being destroyed. Posts a
//  quit message to caus the application to halt. 

//@mfunc LRESULT | CFrameWindowImpl | Create | Use this function to dynamically create a window
//@parm HWND | hWndParent | Handle to parent window
//@parm RECT& | rcPos | Rectangle position on screen
//@parm LPCTSTR | lpszWindowName | caption to use 
//@parm DWORD | dwStyle | Window style to use 
//@parm DWORD | dwExStyle | Extended window style to use 
//@parm HMENU |	hMenu | Menu handle to use 
//@parm LPVOID | lpCreateParam | Pointer to parameter to be passed in during creation
//@rdesc Window handle of the window that was created
//@comm Use this function to dynamically create a window 

//@class CFrameWindowImpl | Implements a frame window 

//@tcarg typename | _Derived | Name of most derived class.
//@tcarg unsigned int | _nResource | Integer resource ID for menu, window caption, icon,
// and accelerators. Default value is 0.
//@tcarg typename | _Traits | Traits class that provides the style and extended style
// for the frame window. Default is CFrameWinTraits.
//@tcarg typename | _Base | Base window class. Default is CWindow.
//@tcarg typename | _LayoutPlugin | Layout manager class. Different types of layout
// managers can be plugged into the frame window. The default is CLayoutManager.

//@base pubic | CContainerWindowImpl
//

template <typename _Derived, 
			unsigned int _nResource = 0, 
			typename _Traits = CFrameWinTraits, 
			typename _Base = CWindow,
			typename _LayoutPlugin = foundation::CLayoutManager<_Derived> >
class CFrameWindowImpl:
	public CContainerWindowImpl<_Derived, _Traits, _Base, _LayoutPlugin>
{
	typedef CFrameWindowImpl<_Derived, _nResource, _Traits, _Base, _LayoutPlugin> _thisClass;
	typedef CContainerWindowImpl<_Derived, _Traits, _Base, _LayoutPlugin> _windowBase;

public:

	DECLARE_FRAME_WND_CLASS(NULL)

	//@cmember
	/* Constructor */
	CFrameWindowImpl();

	BEGIN_MSG_MAP(_thisClass)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        MESSAGE_HANDLER(WM_INITMENUPOPUP, OnInitMenuPopup)
		CHAIN_MSG_MAP(_windowBase)
	END_MSG_MAP()

public:	

	//@cmember
	/* Handles the WM_INITPOPUP message */
	LRESULT OnInitMenuPopup(UINT uint, WPARAM wparam, LPARAM lparam, BOOL& rb)
	{
		_Derived* pDerived = static_cast<_Derived*>(this);
		pDerived->UpdateCommandUI(uint, wparam, lparam, rb);
		return 0;
	}

	//@cmember
	/* Place holder for UpdateCommandUI */
	LRESULT UpdateCommandUI(UINT uint, WPARAM wparam, LPARAM lparam, BOOL& rb)
	{
		// To get rid of the warnings...
		uint;
		wparam;
		lparam;
		rb;

		return 0;
	}

	//@cmember
	/* Handles WM_DESTROY */
	LRESULT OnDestroy(UINT,WPARAM,LPARAM, BOOL& bHandled)
	{
		// Destroy menu if allocated
		if (m_hMenu != NULL) {
			::DestroyMenu(m_hMenu);
			m_hMenu = NULL;
		}

		// If top level, send quit message
		if (!(GetStyle() & (WS_CHILD | WS_POPUP))) {
			::PostQuitMessage(0);
		}
		bHandled = FALSE;
		return 0L;
	}

	//@cmember
	/* Use this function to create the window */
	HWND Create (
		HWND hWndParent, 
		RECT& rcPos, 
		LPCTSTR lpszWindowName = 0,
		DWORD dwStyle = 0, DWORD dwExStyle = 0,
		HMENU hMenu = 0, LPVOID lpCreateParam = 0
	)
	{
		// If none given, extract the menu from the resource file
		if (!hMenu) {
			m_hMenu = ::LoadMenu(_Module.GetResourceInstance(), MAKEINTRESOURCE(_nResource));
		}
		else {
			m_hMenu = hMenu;
		}

		if (!m_hAccTable) {
			m_hAccTable = ::LoadAccelerators(_Module.GetResourceInstance(), MAKEINTRESOURCE(_nResource));
		}

		TCHAR szCaption[255];
		if (!lpszWindowName) {
			if (::LoadString(_Module.GetResourceInstance(), _nResource, szCaption, 255)) {
				lpszWindowName = szCaption;
			}
		}

		return _windowBase::Create(hWndParent, rcPos, lpszWindowName, dwStyle, dwExStyle, reinterpret_cast<UINT>(m_hMenu), lpCreateParam);
	}


public:
	//@cmember
	/* Menu handle for the frame window */
	HMENU m_hMenu;

	//@cmember
	/* Accelerator table */
	HACCEL m_hAccTable;

};


template <typename _Derived, unsigned int _nResource, typename _Traits, typename _Base, typename _LayoutPlugin>
CFrameWindowImpl<_Derived, _nResource, _Traits, _Base>::CFrameWindowImpl():
	m_hMenu(NULL), m_hAccTable(NULL)
{
	// Modify the class information to include the given icon
	CWndClassInfo& wci = _Derived::GetWndClassInfo();
	if (!wci.m_atom) {
		wci.m_wc.hIcon = ::LoadIcon(_Module.GetResourceInstance(), MAKEINTRESOURCE(_nResource));
		wci.m_wc.hIconSm = reinterpret_cast<HICON>(::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(_nResource), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR));
	}
}



};	// namespace stingray::foundation
};	// namespace stingray
