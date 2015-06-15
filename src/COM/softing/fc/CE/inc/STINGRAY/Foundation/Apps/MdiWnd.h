/////////////////////////////////////////////////////////////////////////////
// MdiWnd.h : SFL MDI support
//
// Stingray Software Windows Template Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Gilberto Araya
// Description:  MDI Support.
//


#pragma once

#if !defined(_SFL)
#define _SFL
#endif

#include <foundation\sflres.h>
#include <foundation\apps\FrameWnd.h>

namespace stingray {
namespace foundation {


/////////////////////////////////////////////////////////////////////////////
// Helper classes


/////////////////////////////////////////////////////////////////////////////
// CWindowMenuLocator
// CWindowMenuLocator Locate menus
//
//@topic CWindowMenuLocator Class Overview | Locate menues 
//
//@doc CWindowMenuLocator
//
//@mfunc HMENU | CWindowMenuLocator| GetWindowMenu | Get the window menu
//@rdesc Returns the window menu handle 
//@parm HMENU | hMainMenu | Main window handle
//@comm 
//
//@class CWindowMenuLocator | This class is useful for finding menus for MDI implementations
//
// CWindowMenuLocator includes the following member function
//  <mf CMDIChildImpl::GetWindowMenu>, 
//
//@tcarg UINT | nResCaption = ID_WINDOW_MENU_TEXT | Window caption
//@tcarg UINT | nLocale = LOCALE_USER_DEFAULT | Locale
//
//@comm 
//
template <UINT nResCaption = ID_WINDOW_MENU_TEXT, UINT nLocale = LOCALE_USER_DEFAULT>
class CWindowMenuLocator
{
public:
	CWindowMenuLocator();

	HMENU GetWindowMenu(HMENU hMainMenu) const;

private:
	static TCHAR szWindowMenuText[16];
};


template <UINT nResCaption, UINT nLocale>
CWindowMenuLocator<nResCaption, nLocale>::CWindowMenuLocator()
{
	if (szWindowMenuText[0] == '\0') {
		// The first instance should load the string from the resources
		::LoadString(_Module.GetResourceInstance(), nResCaption, szWindowMenuText, sizeof(szWindowMenuText));
	}
}

template <UINT nResCaption, UINT nLocale>
HMENU CWindowMenuLocator<nResCaption, nLocale>::GetWindowMenu(HMENU hMainMenu) const
{
	HMENU hWindowMenu = NULL;
	int nTextLen = _tcslen(szWindowMenuText);
	TCHAR *szBuffer = reinterpret_cast<TCHAR*>(_alloca(nTextLen * sizeof(*szBuffer)));
	int nItems = GetMenuItemCount(hMainMenu);

	for (int iItem = 0; (iItem < nItems) && (hWindowMenu == NULL); ++iItem) {
		int nItemStringLen = ::GetMenuString(hMainMenu, iItem, szBuffer, 0, MF_BYPOSITION);
		if (nItemStringLen == nTextLen) {
			::GetMenuString(hMainMenu, iItem, szBuffer, nTextLen + 1, MF_BYPOSITION);
			if (::CompareString(nLocale, 0, szWindowMenuText, -1, szBuffer, -1) == CSTR_EQUAL) {
				hWindowMenu = ::GetSubMenu(hMainMenu, iItem);
			}
		}
	}

	return hWindowMenu;
}

template <UINT nResCaption, UINT nLocale>
TCHAR CWindowMenuLocator<nResCaption, nLocale>::szWindowMenuText[] = _T("");

/////////////////////////////////////////////////////////////////////////////
// MDI ChildImpl
// CMDIChildImpl - MDI Child Window implementation
//
//@doc CMDIChildImpl
//
//@mfunc | CMDIChildImpl | CMDIChildImpl | Constructor
//@comm 
//
//@mfunc HWND | CMDIChildImpl | Create | Create the MDI Child window
//@rdesc Returns the window handle of the MDI Child window
//@parm HWND | hWndParent | Parent window handle 
//@parm RECT& | rcPos | Rectangle area on the screen
//@parm LPCTSTR | lpszCaption = 0 | caption for the window
//@parm DWORD | dwStyle = 0 | Window creation flags
//@parm DWORD | dwExStyle = 0 | Extended window creation flags
//@parm HMENU | hMenu = 0 | Menu handle to use for hte window
//@parm LPARAM | lpCreateParam = 0 | Window creation parameter-- value the window can use 
//  during creation
//@comm 
//
//@mfunc LRESULT  | CMDIChildImpl | OnMdiActivate | Handle MDI window activation
//@rdesc Returns 0
//@parm UINT | uInt = 0 | Not used
//@parm WPARAM | wParam = 0 | Not used
//@parm LPARAM | lParam = 0 | Passed on to the window baing activated
//@parm BOOL& | rBool | Not used
//@comm 
//
//@class CMDIChildImpl | MDI Child window implementation 
//
// CMDIChildImpl includes the following member functions 
//  <mf CMDIChildImpl::CMDIChildImpl>, 
//  <mf CMDIChildImpl::Create>, and
//  <mf CMDIChildImpl::OnMdiActivate>
//
//@tcarg typename | _Derived | The ultimately-derived class
//@tcarg unsigned int | _nResource | Resource ID 
//@tcarg typename | _Traits | (CFrameWinTraits) Window creation flags
//@tcarg typename | _Base | (CWindow) Window base class
//@tcarg typename | _LayoutPlugin | (CLayoutManager<lt>_Derived<gt>) Layout manager base class
//@tcarg typename | _WindowMenuLocator | (CWindowMenuLocator<lt>ID_WINDOW_MENU_TEXT<gt>) Window menu locator
//
//@base public | CFrameWindowImpl<lt>_Derived, _nResource, _Traits, _Base, _LayoutPlugin<gt>
//
//@comm MDI applications are more complex than SDI applications because MDI 
// applications have to be built to handle multiple open windows and documents 
// at once. The MDI specification stipulates several kinds of windows including 
// the main MDI Frame window <c CMDIFrameImpl>, the MDI Client window <c CMDIClientWindow> , and 
// the MDI Child window (this class)
//
//@comm SFL's CMDIChildImpl class manages the MDI child's menu, the WM_MDIACTIVATE 
//  message to set up that menu within the MDI frame, and the special steps 
//  involved in creating an MDI child window. CMDIChildImpl is usually used as 
//  a base class for MDI child windows within your application. 
//
template <typename _Derived, 
			unsigned int _nResource, 
			typename _Traits = CFrameWinTraits, 
			typename _Base = CWindow, 
			typename _LayoutPlugin = foundation::CLayoutManager<_Derived>,
			typename _WindowMenuLocator = CWindowMenuLocator<ID_WINDOW_MENU_TEXT> >
class CMDIChildImpl :
	public CFrameWindowImpl<_Derived, _nResource, _Traits, _Base, _LayoutPlugin>
{
	typedef CFrameWindowImpl<_Derived, _nResource, _Traits, _Base, _LayoutPlugin> _windowBase;

public:

	//@cmember
	/* Constructor */
	CMDIChildImpl():
		m_hwndMDIClient(NULL), m_hMenuWindow(NULL)
	{
		// MDI Child windows must delegate to an special default procedure
		m_pfnSuperWindowProc = DefMDIChildProc;
	}


	BEGIN_MSG_MAP(CMDIFrameImpl)
	    MESSAGE_HANDLER(WM_MDIACTIVATE, OnMdiActivate)
		CHAIN_MSG_MAP(_windowBase)
	END_MSG_MAP()

public:
	//@cmember
	/* Create the MDI child window */
	HWND Create (
		HWND hwndParent, /* should be an MDI Client */
		RECT& rcPos, 
		LPCTSTR lpszCaption = 0,
		DWORD dwStyle = 0, DWORD dwExStyle = 0,
		HMENU hMenu = 0, 
		LPARAM lpCreateParam = 0
	)
	{
		m_hwndMDIClient = hwndParent;

		// Load items from resources, just as in the parent class.
		// Processing will not be delegated, hence this is done here again.
		if (!hMenu) {
			m_hMenu = ::LoadMenu(_Module.GetResourceInstance(), MAKEINTRESOURCE(_nResource));
		}
		else {
			m_hMenu = hMenu;
		}
		_WindowMenuLocator menuLocator;
		m_hMenuWindow = menuLocator.GetWindowMenu(m_hMenu);


		TCHAR szCaption[255];
		if (!lpszCaption) {
			if (::LoadString(_Module.GetResourceInstance(), _nResource, szCaption, 255)) {
				lpszCaption = szCaption;
			}
		}

		// Since creation will not be delegated to ATL, make sure the class
		// is registered in Windows
		CWndClassInfo& clsInfo = _Derived::GetWndClassInfo();
		ATOM atom = clsInfo.Register(&m_pfnSuperWindowProc);


		// ATL Subclassing requirement: register a thunk in the module
		_Module.AddCreateWndData(&m_thunk.cd, this);

		// Do not delegate to the inherited Create function.
		// Instead, follow the special procedure Windows requires
		// for child frames in the MDI paradigm.
		// That is, fill the appropiate structure with data
		// and send the corresponding message to the client window.
		MDICREATESTRUCT mdics;
		mdics.szClass = reinterpret_cast<LPCTSTR>(MAKELONG(atom, 0));
		mdics.szTitle = lpszCaption;
		mdics.hOwner  = _Module.GetModuleInstance();
		mdics.x       = rcPos.left;
		mdics.y       = rcPos.top;
		mdics.cx      = rcPos.right - rcPos.left;
		mdics.cy      = rcPos.bottom - rcPos.top;
		mdics.lParam  = lpCreateParam;
		mdics.style   = _Derived::GetWndStyle(dwStyle);

		HWND hwndChild = reinterpret_cast<HWND>(::SendMessage(m_hwndMDIClient, WM_MDICREATE, 0, reinterpret_cast<LPARAM>(&mdics)));
		if (!::IsWindow(hwndChild)) {
			hwndChild = NULL;
		}

		return hwndChild;
    }

	//@cmember
	/* Handle the WM_MDIACTIVATE message */
	LRESULT OnMdiActivate(UINT, WPARAM, LPARAM lParam, BOOL&)
	{
        // Restore the main frame menu or show this frame's,
		// depending on whether this window is being activated or deactivated
		bool bActivated = reinterpret_cast<HWND>(lParam) == m_hWnd;
		HWND hwndMainFrame = ::GetParent(m_hwndMDIClient);

		if (bActivated && m_hMenu ) {
			::SendMessage(m_hwndMDIClient, WM_MDISETMENU, reinterpret_cast<WPARAM>(m_hMenu), reinterpret_cast<LPARAM>(m_hMenuWindow));
			::DrawMenuBar(hwndMainFrame);
		}
		else {
			::SendMessage(hwndMainFrame, WM_MDISETMENU, 0, 0);
		}

		return 0 ;
	}


protected:
	HWND m_hwndMDIClient;
	HMENU m_hMenuWindow;
};



/////////////////////////////////////////////////////////////////////////////
// MDI Client

typedef CWinTraits<WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_VISIBLE, WS_EX_CLIENTEDGE> CMDIClientTraits;

/////////////////////////////////////////////////////////////////////////////
// CMDIClientWindow
// CMDIClientWindow - MDI Client Window implementation
//
//@topic CMDIClientWindow Class Overview | An MDI application needs to contain an 
// MDI client window within the main frame window. The job of the MDI client 
// window is to manage SFL's MDI Child window creation. You usually don't need 
// to use this class explicitly-the class is usually mixed into the frame window
//
//@doc CMDIClientWindow
//
//@mfunc | CMDIClientWindow | CMDIClientWindow | Constructor
//
//@mfunc HWND | CMDIClientWindow | Create | Create the MDI Child window
//@rdesc Returns the window handle of the MDI Child window
//@parm HWND | hWndParent | Parent window handle 
//@parm RECT& | rcPos | Rectangle area on the screen
//@parm CLIENTCREATESTRUCT* | lpCreateParam = NULL | Window creation parameter
//@parm UINT | nID = MDIClientID | Client ID 
//@parm LPCTSTR | szWindowName = 0 | Name of the window
//@parm DWORD | dwStyle = 0 | Window creation flags
//@parm DWORD | dwExStyle = 0 | Extended window creation flags
//@comm 
//
//@class CMDIClientWindow | MDI Client window class 
//
// CMDIClientWindow includes the following member functions 
//  <mf CMDIClientWindow::Create>
//
//@tcarg typename | _Traits | Window creation flags
//@tcarg typename | _Base | Window base class
//
//@base public | _Base
//
//@comm You usually don't need to use this class explicitly-the class is usually 
// mixed into the frame window
//
template <typename _Traits = CMDIClientTraits, typename _Base = CWindow>

class CMDIClientWindow : 
	public _Base
{
	typedef _Base _windowBase;

public:

	CMDIClientWindow(HWND hwnd): _windowBase(hwnd)
	{}

public:

	HWND Create (
		HWND hWndParent, 
		RECT& rcPos = rcDefault,
		CLIENTCREATESTRUCT* lpCreateParam = NULL,
		UINT nID = MDIClientID, 
		LPCTSTR szWindowName = NULL,
		DWORD dwStyle = 0, DWORD dwExStyle = 0
	)
	{
		dwStyle = _Traits::GetWndStyle(dwStyle);
		dwExStyle = _Traits::GetWndExStyle(dwExStyle);
		
		return _windowBase::Create(_T("MDICLIENT"), hWndParent, rcPos, szWindowName, 
			dwStyle, dwExStyle, nID, lpCreateParam);
	}

public:
	enum {
		MDIClientID = 1
	};

};

/////////////////////////////////////////////////////////////////////////////
// CMDIFrame
// CMDIFrame - MDI Frame Window implementation
//
//@topic CMDIFrame Class Overview | The job of SFL's CMDIFrame class is to frame 
// an MDI application and manages the application's MDI-ness. This means being 
// able to perform such functions as finding the currently active MDI child window, 
// toggling through the application's windows, cascading the windows, and tiling 
// the windows. 
//
//@doc CMDIFrame
//
//@mfunc | CMDIFrame | CMDIFrame | Constructor
//@comm 
//
//@mfunc HWND | CMDIFrame | MDIGetActive | Get the Active MDI Child window
//@rdesc Returns the window handle of the active MDI Child window
//@parm BOOL* | lpbMaximized = NULL | flag indicating whether or not the window is maximized
//@comm 
//
//@mfunc void | CMDIFrame | MDIActivate | Activate a specific MDI child window
//@parm HWND | HWND hWndChildToActivate | Window to activate 
//@comm 
//
//@mfunc void | CMDIFrame | MDINext | Activate the next MDI window
//@parm HWND | hWndChild | Handle to the window to activate 
//@parm bool | bPrevious = false | Flag to indicate the next or previous window
//@comm If the bPrevious parameter is zero, the system activates the next MDI child window 
// and places the child window identified by the hwndChild parameter behind all 
// other child windows. If this parameter is nonzero, the system activates the 
// previous child window, placing it in front of the child window identified 
// by hwndChild. 
//
//@mfunc void | CMDIFrame | MDIMaximize | Maximize an MDI child window
//@parm HWND | hWndChildToMaximize | Handle to the window to maximize
//@parm bool | bPrevious = false | Flag to indicate the next or previous window
//@comm 
//
//@mfunc void | CMDIFrame | MDIRestore | Restore an MDI child window
//@parm HWND | hWndChildToRestore | Handle to the window to restore
//@comm 
//
//@mfunc void | CMDIFrame | MDIDestroy | Destroy an MDI child window
//@parm HWND | hWndChildToDestroy | Handle to the window to destroy 
//@comm 
//
//@mfunc BOOL | CMDIFrame | MDICascade | Cascade the MDI windows
//@rdesc Returns true if the function succeeds
//@parm UINT | uFlags = 0 | Cascade flags
//@comm Flag can be either MDITILE_SKIPDISABLED, which prevents disabled MDI 
// child windows from being cascaded, or MDITILE_ZORDER, which in Windows 2000
// arranges the windows in Z order. 
//
//@mfunc BOOL | CMDIFrame | MDITile | Tile the MDI windows 
//@rdesc Returns true if the function succeeds
//@parm UINT | uFlags = MDITILE_HORIZONTAL | Tiling flags
//@comm Flag can be either MDITILE_HORIZONTAL, which tiles windows horizontally, or 
// MDITILE_VERTICAL, which tiles windows vertically. 
//
//@mfunc void | CMDIFrame | MDIIconArrange | Arrange all minimized MDI child windows
//@comm Does not affect child windows that are not minimized
//
//@mfunc HMENU | CMDIFrame | MDISetMenu | Set the menu
//@rdesc Returns the old menu 
//@parm HMENU | hMenuFrame | New frame menu
//@parm HMENU | hMenuWindow | New window menu
//@comm Sets the frame and window menus
//
//@mfunc HMENU | CMDIFrame | MDIRefreshMenu | Refresh the Window menu of the MDI frame window. 
//@rdesc Returns the old frame window menu 
//@comm Refreshes the menu
//
//@class CMDIFrame | MDI frame window class 
//
// CMDIFrame includes the following member functions 
//  <mf CMDIFrame::MDIGetActive>,
//  <mf CMDIFrame::MDIActivate>,
//  <mf CMDIFrame::MDINext>,
//  <mf CMDIFrame::MDIMaximize>,
//  <mf CMDIFrame::MDIRestore>,
//  <mf CMDIFrame::MDIDestroy>,
//  <mf CMDIFrame::MDICascade>,
//  <mf CMDIFrame::MDITile>,
//  <mf CMDIFrame::MDIIconArrange>,
//  <mf CMDIFrame::MDISetMenu>, and
//  <mf CMDIFrame::MDIRefreshMenu>
//
//@tcarg typename | _MDIClient = CMDIClientWindow<lt><gt> | Type of client area
//
//@base public | CWindow
//
template <typename _MDIClient = CMDIClientWindow<> >
class CMDIFrame:
	public CWindow
{
protected:
	typedef _MDIClient CMDIClientWindow;

public:
	//@cmember
	/* Constructor */
	CMDIFrame(HWND hwndFrame = NULL, HWND hwndClient = NULL) : CWindow(hwndFrame), m_wndClient(hwndClient)
	{}

	//@cmember
	/* Assignment operator */
	CMDIFrame& operator=(HWND hwnd)
	{
		m_hWnd = hwnd;
		m_wndClient = reinterpret_cast<HWND>(NULL);
		return *this;
	}

public:
	CMDIClientWindow m_wndClient;

public:
	// MDI operations

	//@cmember
	/* Get the active window */
	HWND MDIGetActive(BOOL* lpbMaximized = NULL)
	{
		ATLASSERT(m_wndClient.IsWindow());
		return reinterpret_cast<HWND>(m_wndClient.SendMessage(WM_MDIGETACTIVE, 0, (LPARAM)lpbMaximized));
	}

	//@cmember
	/* Activate and MDI child */
	void MDIActivate(HWND hWndChildToActivate)
	{
		ATLASSERT(m_wndClient.IsWindow());
		ATLASSERT(::IsWindow(hWndChildToActivate));
		m_wndClient.SendMessage(WM_MDIACTIVATE, reinterpret_cast<WPARAM>(hWndChildToActivate), 0);
	}

	//@cmember
	/* Assign the next MDI child */
	void MDINext(HWND hWndChild, bool bPrevious = false)
	{
		ATLASSERT(m_wndClient.IsWindow());
		m_wndClient.SendMessage(WM_MDINEXT, reinterpret_cast<WPARAM>(hWndChild), static_cast<LPARAM>(bPrevious));
	}

	//@cmember
	/* Maximize and MDI window . */
	void MDIMaximize(HWND hWndChildToMaximize)
	{
		ATLASSERT(m_wndClient.IsWindow());
		m_wndClient.SendMessage(WM_MDIMAXIMIZE, reinterpret_cast<WPARAM>(hWndChildToMaximize));
	}

	//@cmember
	/* Restore an MDI Window. */
	void MDIRestore(HWND hWndChildToRestore)
	{
		ATLASSERT(m_wndClient.IsWindow());
		ATLASSERT(::IsWindow(hWndChildToRestore));
		m_wndClient.SendMessage(WM_MDIICONARRANGE, reinterpret_cast<WPARAM>(hWndChildToRestore));
	}

	//@cmember
	/* Destroy an MDI Window */
	void MDIDestroy(HWND hWndChildToDestroy)
	{
		ATLASSERT(m_wndClient.IsWindow());
		ATLASSERT(::IsWindow(hWndChildToDestroy));
		m_wndClient.SendMessage(WM_MDIDESTROY, reinterpret_cast<WPARAM>(hWndChildToDestroy));
	}

	//@cmember
	/* Cascade the windows */
	BOOL MDICascade(UINT uFlags = 0)
	{
		ATLASSERT(m_wndClient.IsWindow());
		return reinterpret_cast<BOOL>(m_wndClient.SendMessage(WM_MDICASCADE, reinterpret_cast<WPARAM>(uFlags)));
	}

	//@cmember
	/* Tile the windows */
	BOOL MDITile(UINT uFlags = MDITILE_HORIZONTAL)
	{
		ATLASSERT(m_wndClient.IsWindow());
		return reinterpret_cast<BOOL>(m_wndClient.SendMessage(WM_MDITILE, reinterpret_cast<WPARAM>(uFlags)));
	}

	//@cmember
	/* Arrange the icons */
	void MDIIconArrange()
	{
		ATLASSERT(m_wndClient.IsWindow());
		m_wndClient.SendMessage(WM_MDIICONARRANGE);
	}

	//@cmember
	/* Set the menu */
	HMENU MDISetMenu(HMENU hMenuFrame, HMENU hMenuWindow)
	{
		ATLASSERT(m_wndClient.IsWindow());
		return reinterpret_cast<HMENU>(m_wndClient.SendMessage(WM_MDISETMENU, reinterpret_cast<WPARAM>(hMenuFrame), reinterpret_cast<LPARAM>(hMenuWindow)));
	}

	//@cmember
	/* Refresh the menu */
	HMENU MDIRefreshMenu()
	{
		ATLASSERT(m_wndClient.IsWindow());
		return reinterpret_cast<HMENU>(m_wndClient.SendMessage(WM_MDIREFRESHMENU));
	}

};

/////////////////////////////////////////////////////////////////////////////
// CMDIFrameImpl
// CMDIFrameImpl - MDI Frame Window implementation
//
//@doc CMDIFrameImpl
//
//@mfunc | CMDIFrameImpl | CMDIFrameImpl | Constructor
//@comm Prepares the frame implementation for construction
//
//@mfunc HWND | CMDIFrameImpl | Create | Create the MDI Child window
//@rdesc Returns the window handle of the MDI Child window
//@parm HWND | hWndParent | Parent window handle 
//@parm RECT& | rcPos | Rectangle area on the screen
//@parm LPCTSTR | szWindowCaption = 0 | Caption for the window
//@parm DWORD | dwStyle = 0 | Window creation flags
//@parm DWORD | dwExStyle = 0 | Extended window creation flags
//@parm LPVOID | lpCreateParam = 0 | Parameter to pass to the window upon creation
//
//@class CMDIFrameImpl | MDI Frame window implementation
//
// CMDIFrameImpl includes the following member functions 
//  <mf CMDIFrameImpl::CMDIFrameImpl> and
//  <mf CMDIFrameImpl::Create>,
//
//@tcarg typename | _MDIClient | (CMDIClientWindow<lt><gt>) Type of client area
//
//@base public | CWindow
//
//@comm The <c CMDIFrame> class is still an intermediate layer in the MDI-ness of an 
// application. The final layer in an MDI application is usually CMDIFrameImpl. 
//
template <
		typename _Derived, 
		unsigned int _nResource, 
		typename _Traits = CFrameWinTraits, 
		typename _MDIClient = CMDIClientWindow<>, 
		typename _Base = CMDIFrame<_MDIClient>,
		typename _LayoutPlugin = foundation::CLayoutManager<_Derived> >
class CMDIFrameImpl:
	public CFrameWindowImpl<_Derived, _nResource, _Traits, _Base, _LayoutPlugin>
{
public:
	typedef CFrameWindowImpl<_Derived, _nResource, _Traits, _Base, _LayoutPlugin> _windowBase;

	//@cmember
	/* Constructor */
	CMDIFrameImpl()
	{
		// Modify the class information to include the given icon
		CWndClassInfo& wci = GetWndClassInfo();
		if (!wci.m_atom) {
			wci.m_wc.hIcon = ::LoadIcon(_Module.GetResourceInstance(), MAKEINTRESOURCE(_nResource));
			wci.m_wc.hIconSm = reinterpret_cast<HICON>(::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(_nResource), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR));
		}
	}

	BEGIN_MSG_MAP(CMDIFrameImpl)
		MESSAGE_HANDLER_DELEGATE(WM_CREATE, OnCreate)
		CHAIN_MSG_MAP(_windowBase)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_MDISETMENU, OnMDISetMenu)
	    COMMAND_RANGE_HANDLER(IDC_SFL_MDI_CASCADE, IDC_SFL_MDI_ARRANGEICONS, OnWindowArrange)
	END_MSG_MAP()


public:

	//@cmember
	/* Create the Window */
	HWND Create (
		HWND hwndParent, 
		RECT& rcPos, 
		LPCTSTR lpszCaption = 0,
		DWORD dwStyle = 0, DWORD dwExStyle = 0,
		HMENU hMenu = 0, LPVOID lpCreateParam = 0
	)
	{
		HWND hwndThis =  _windowBase::Create(hwndParent, rcPos, lpszCaption, dwStyle, dwExStyle, hMenu, lpCreateParam);
		return hwndThis;
	}


protected:

	// Message handlers


	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL& bHandled)
	{
		m_hMenu = GetMenu();

		// Create the client window
		CLIENTCREATESTRUCT clientInfo = { 0, 1 };
		CRect rcClient;
		GetClientRect(rcClient);
		HWND hwndClient = m_wndClient.Create(m_hWnd, rcClient, &clientInfo);
		return (hwndClient ? 0 : -1);
	}


	LRESULT OnSize(UINT, WPARAM, LPARAM, BOOL& bHandled)
	{
		// Do not let the default frame procedure to handle this message,
		// as it will try to resize the MDI Client window to the entire
		// client area. 
		// This task has been already taken care of by the layout manager
		bHandled = (m_pLayoutRoot != NULL);
		return 0;
	}


	LRESULT OnMDISetMenu(UINT, WPARAM, LPARAM, BOOL&)
	{
		LRESULT lres = m_wndClient.SendMessage(WM_MDISETMENU, reinterpret_cast<WPARAM>(m_hMenu));
		DrawMenuBar();
		return lres;
	}


	LRESULT OnWindowArrange(WORD , WORD wID, HWND , BOOL& )
	{
		UINT msg;
		WPARAM wparam = 0;
		switch (wID) {
		case IDC_SFL_MDI_CASCADE:
			msg = WM_MDICASCADE;
			break;
		case IDC_SFL_MDI_TILEHORIZONTAL:
			msg = WM_MDITILE;
			wparam = MDITILE_HORIZONTAL;
			break;
		case IDC_SFL_MDI_TILEVERTICAL:
			msg = WM_MDITILE;
			wparam = MDITILE_VERTICAL;
			break;
		case IDC_SFL_MDI_ARRANGEICONS:
			msg = WM_MDIICONARRANGE;
			break;
		}
		return m_wndClient.SendMessage(msg, wparam);
	}


protected:
	// Special case: the Default MDI Frame Window Proc DefFrameProc has a signature
	// different than that of other WindowProcs: it expects the handle of the MDI
	// Client window as an extra parameter.
	// ATL does not have provisions to handle a case like this, so it is necessary
	// to trnascribe most of the implementation of the CWindowImpl here 


	virtual WNDPROC GetWindowProc()
	{
		return MdiFrameProc;
	}

	static LRESULT CALLBACK MdiFrameProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		// NOTE: This is the only changed line of code.
		//       The rest is copied from CWindowImplBaseT::WindowProc
		CMDIFrameImpl*    pThis = (CMDIFrameImpl*)hWnd;

		// set a ptr to this message and save the old value
		MSG msg = { pThis->m_hWnd, uMsg, wParam, lParam, 0, { 0, 0 } };
		const MSG* pOldMsg = pThis->m_pCurrentMsg;
		pThis->m_pCurrentMsg = &msg;
		// pass to the message map to process
		LRESULT lRes;
		BOOL bRet = pThis->ProcessWindowMessage(pThis->m_hWnd, uMsg, wParam, lParam, lRes, 0);
		// restore saved value for the current message
		ATLASSERT(pThis->m_pCurrentMsg == &msg);
		pThis->m_pCurrentMsg = pOldMsg;
		// do the default processing if message was not handled
		if(!bRet) {
			if(uMsg != WM_NCDESTROY)
				lRes = ::DefFrameProc(pThis->m_hWnd, pThis->m_wndClient.m_hWnd, uMsg, wParam, lParam);
			else {
				// unsubclass, if needed
				LONG pfnWndProc = ::GetWindowLong(pThis->m_hWnd, GWL_WNDPROC);
				lRes = DefFrameProc(pThis->m_hWnd, pThis->m_wndClient.m_hWnd, uMsg, wParam, lParam);
				if (pThis->m_pfnSuperWindowProc != ::DefWindowProc && ::GetWindowLong(pThis->m_hWnd, GWL_WNDPROC) == pfnWndProc)
					::SetWindowLong(pThis->m_hWnd, GWL_WNDPROC, (LONG)pThis->m_pfnSuperWindowProc);
				// clear out window handle
				HWND hWnd = pThis->m_hWnd;
				pThis->m_hWnd = NULL;
				// clean up after window is destroyed
				pThis->OnFinalMessage(hWnd);
			}
		}
		return lRes;
	}

};


// Command message delegation to active MDI Child

#define COMMAND_ID_DELEGATE_ACTIVE(id)	\
	if (uMsg == WM_COMMAND && id == LOWORD(wParam)) { \
		HWND hwndChild = MDIGetActive(); \
		if (hwndChild) { \
			lResult = ::SendMessage(hwndChild, uMsg, wParam, lParam); \
			bHandled = TRUE; \
		} \
		if (bHandled) return TRUE; \
	}


#define COMMAND_RANGE_DELEGATE_ACTIVE(idFirst, idLast)	\
	if (uMsg == WM_COMMAND && LOWORD(wParam) >= idFirst  && LOWORD(wParam) <= idLast) { \
		HWND hwndChild = MDIGetActive(); \
		if (hwndChild) { \
			lResult = ::SendMessage(hwndChild, uMsg, wParam, lParam); \
			bHandled = TRUE; \
		} \
		if (bHandled) return TRUE; \
	}


#define COMMAND_DELEGATE_ACTIVE()	\
	if (uMsg == WM_COMMAND) { \
		HWND hwndChild = MDIGetActive(); \
		if (hwndChild) { \
			lResult = ::SendMessage(hwndChild, uMsg, wParam, lParam); \
			bHandled = TRUE; \
		} \
		if (bHandled) return TRUE; \
	}



};	// namespace stingray::foundation
};	// namespace stingray
