/////////////////////////////////////////////////////////////////////////////
// UIUpdate.h : User interface update support
//
// Stingray Software Extension Classes
// Copyright (C) 1999 Stingray Software Inc,
// All rights reserved.
//
// Author:       George Shepherd
// Description:  User interface update support
//

#pragma once

#include <vector>

#include "Foundation\sflres.h"
#include "Foundation\RefCount.h"
#include "Foundation\Events\WindowListener.h"
#include "Foundation\Events\UIUpdateEvent.h"
#include "Foundation\Events\UIUpdateListener.h"

#include "Foundation\Controls.h"

namespace stingray {
namespace foundation {

static HBITMAP s_hbmMenuDot = NULL;

////////////////////////////////////////////////////////////////////////////
//@doc SFL UIUpdating 
//@topic SFL UIUpdating | SFL provides a fully functional UI Updating system
// for keeping your application's user interface consistent with the state 
// of the application 
//@comm SFL's User Interface Updating mechanism is comprised of 
// several components. These components include <c CUIUpdateGenerator>, the <c IEventRouter> 
// interface, <c CUIUpdateAdapter>, and an interface named <c IIdleHandler>. 
//@comm The UIUpdating mechanism works like this. Any class wishing to implement User 
// Interface updating handles idle-time processing by plugging in its own idle-time 
// processing interface while processing the WM_CREATE message. We'll see the specific 
// calls for doing that shortly. The class performing User Interface updating also 
// inherits from a class named CUIUpdateGenerator, which has a member function named  
// GenerateUIUpdates. The class performing User Interface 
// updating handles idle-time processing by calling GenerateUIUpdates. GenerateUIUpdates 
// goes through the menu, toolbars, and status bars that have been registered in to the 
// User Interface Updating mechanism, creating a wrapper class for each User Interface 
// item (menu, tool bar button, status pane). The User Interface mechanism generates a 
// WM_UIUPDATE message and routes the event, where the class performing User Interface 
// updating may modify the element handles it .
//@comm Classes wanting to receive UIUpdating events inherit from CUIUpdateAdapter, 
// which redirects the ui updating logic to a handler function. The handler function 
// takes each incoming User Interface element and massages it in a way appropriate for 
// the current state of the application. For example, if the toolbar is already showing, 
// you my want to put a check mark on the Toolbar toggling menu option. When the toolbar 
// is hidden, you may wish to remove that check mark. 

///////////////////////////////////////////////////////////////////////////
//@doc CUIUpdateMenu
//
// CUIUpdateMenu
// CUIUpdateMenu - Menu updating wrapper class
//
//@mfunc | CUIUpdateMenu | CUIUpdateMenu | Constructor
//@comm Loads the dot bitmap if not already loaded
//
//@mfunc virtual void | CUIUpdateMenu | SetText | Set the menu text
//@parm UINT | nCommandID | ID associated with the menu 
//@parm LPCTSTR | lpsz | New text for the menu
//
//@mfunc virtual void | CUIUpdateMenu | Enable | Enable or disable the menu item 
//@parm UINT | nCommandID | ID associated with the menu 
//@parm bool | bEnable | Flag indicating enable or disable
//
//@mfunc virtual void | CUIUpdateMenu | SetRadio | Set or unset the radio button
//@parm UINT | nCommandID | ID associated with the menu 
//@parm bool | bSetCheck = true | Flag indicating set or unset 
//
//@mfunc virtual void | CUIUpdateMenu | SetCheck | Set or unset check
//@parm UINT | nCommandID | ID associated with the menu 
//@parm int | nState = 1| State of the check 
//
//@class CUIUpdateMenu | This class implements the IUIUpdateTarget
// interface for menus.
//
// CUIUpdateMenu includes the following member functions 
//  <mf CUIUpdateMenu::CUIUpdateMenu>,
//  <mf CUIUpdateMenu::SetText>,
//  <mf CUIUpdateMenu::Enable>,
//  <mf CUIUpdateMenu::SetRadio>, and
//  <mf CUIUpdateMenu::SetCheck>,
//
//
class CUIUpdateMenu : public CRefCountImpl<IUIUpdateElement>
{
// Constructors/destructor
public:
	//@cmember
	/* Constructor-- create a new menu wrapper with an existing menu handle */
	CUIUpdateMenu(HMENU hMenu) :
		m_hMenu(hMenu)
	{
		if (!s_hbmMenuDot)
		{
			s_hbmMenuDot = LoadBitmap(_Module.m_hInst, MAKEINTRESOURCE(IDC_MENURADIOBUTTON));
		}
	}

// Attributes
protected:
	HMENU m_hMenu;

// Operations
public:
	//@cmember
	/* Get the menu handle */
	operator HMENU()
	{
		return m_hMenu;
	}

	//@cmember
	/* Get the menu handle */
	HMENU GetMenu()
	{
		return m_hMenu;
	}

	//@cmember
	/* Set the text for the menu */
	virtual void SetText(const UINT nCommandID, LPCTSTR lpsz)
	{
		MENUITEMINFO mi;

		::ZeroMemory(&mi,sizeof(MENUITEMINFO));

		mi.cbSize=sizeof(MENUITEMINFO);
		mi.fMask=MIIM_STATE;

		if(lpsz)
		{
			UINT nState = GetMenuState(m_hMenu, nCommandID, MF_BYCOMMAND);
			nState &= ~(MF_BITMAP|MF_OWNERDRAW|MF_SEPARATOR);
			ModifyMenu(m_hMenu, nCommandID, MF_BYCOMMAND |
						MF_STRING | nState, nCommandID, lpsz);
		}
	}

	//@cmember
	/* Enable or disable the menu item */
	virtual void Enable(const UINT nCommandID, bool bEnable = true)
	{
		MENUITEMINFO mi;

		::ZeroMemory(&mi,sizeof(MENUITEMINFO));

		mi.cbSize=sizeof(MENUITEMINFO);
		mi.fMask=MIIM_STATE;

		if(bEnable)
		{
			mi.fState=MFS_ENABLED;
		} else {
			mi.fState=MFS_DISABLED | MFS_GRAYED;
		}

		SetMenuItemInfo(m_hMenu, nCommandID, FALSE, &mi);

	}

	//@cmember
	/* Set or unset the radio button within the menu */
	virtual void SetRadio(const UINT nCommandID, bool bSetCheck = true)
	{
		SetCheck(bSetCheck);

		if (s_hbmMenuDot != NULL)
		{

			if (bSetCheck)
			{
				SetMenuItemBitmaps(m_hMenu, 
					nCommandID, 
					MF_BYCOMMAND,
					NULL, 
					s_hbmMenuDot);
			} else
			{
				SetMenuItemBitmaps(m_hMenu, 
					nCommandID, 
					MF_BYCOMMAND,
					NULL, 
					NULL);
			}
		}
	}

	//@cmember
	/* Set or usnet the check within an menu item */
	virtual void SetCheck(const UINT nCommandID, int nState = 1)
	{
		MENUITEMINFO mi;

		::ZeroMemory(&mi,sizeof(MENUITEMINFO));

		mi.cbSize=sizeof(MENUITEMINFO);
		mi.fMask= MIIM_STATE;

		if(nState == 1)
		{
			mi.fState= MFS_CHECKED;
		} else
		{
			mi.fState= MFS_UNCHECKED;
		}

		SetMenuItemInfo(m_hMenu, nCommandID, FALSE, &mi);
	}

	// The GUID map implements the QueryGuid function
	BEGIN_GUID_MAP(CUIUpdateMenu)
		GUID_ENTRY(IUIUpdateElement)
		GUID_ENTRY(IQueryGuid)
		GUID_ENTRY(IRefCount)
	END_GUID_MAP
};

///////////////////////////////////////////////////////////////////////////
//
// CUIUpdateToolbar
// CUIUpdateToolbar - Menu updating wrapper class
//
//@doc CUIUpdateToolbar
//
//@mfunc | CUIUpdateToolbar | CUIUpdateToolbar | Constructor
//@comm Assigns toolbar
//
//@mfunc virtual void | CUIUpdateToolbar | SetText | Set the text
//@parm UINT | nCommandID | ID associated with the button
//@parm LPCTSTR | lpsz | New text for the button
//@comm Toolbar implementation does nothing 
//
//@mfunc virtual void | CUIUpdateToolbar | Enable | Enable or disable the toolbar button
//@parm UINT | nCommandID | ID associated with the button 
//@parm bool | bEnable | Flag indicating enable or disable
//@comm 
//
//@mfunc virtual void | CUIUpdateToolbar | SetRadio | Set or unset the toolbar button as radio button
//@parm UINT | nCommandID | ID associated with the button
//@parm bool | bSetCheck = true | Flag indicating set or unset 
//@comm 
//
//@mfunc virtual void | CUIUpdateToolbar | SetCheck | Set or unset check
//@parm UINT | nCommandID | ID associated with the button
//@parm int | nState = 1| State of the check 
//@comm 
//
//@class CUIUpdateToolbar | This class implements the IUIUpdateTarget
// interface for tool bars. 
//
// CUIUpdateToolbar includes the following member functions 
//  <mf CUIUpdateToolbar::CUIUpdateToolbar>,
//  <mf CUIUpdateToolbar::SetText>,
//  <mf CUIUpdateToolbar::Enable>,
//  <mf CUIUpdateToolbar::SetRadio>, and
//  <mf CUIUpdateToolbar::SetCheck>,
//
class CUIUpdateToolbar : public CRefCountImpl<IUIUpdateElement>
{
// Constructors/destructor
public:
	//@cmember
	/* Constructor attaches a toolbar to the toolbar menu updating item */
	CUIUpdateToolbar(CToolBar* pToolBar)
	{
		m_pToolBar = pToolBar;
	}

// Attributes
protected:
	CToolBar* m_pToolBar;

// Operations
public:

	//@cmember
	/* Get the toolbar */
	operator CToolBar*()
	{
		return m_pToolBar;
	}

	//@cmember
	/* Get the toolbar */
	CToolBar* GetToolBar()
	{
		return m_pToolBar;
	}

	//@cmember
	/* Set the text for a toolbar button. Does nothing with toolbars */
	virtual void SetText(const UINT nCommandID, LPCTSTR lpstr)
	{
		// To get rid of warning...
		lpstr;
		nCommandID;

		// Can't set toolbar text...
	}

	//@cmember
	/* Enable or disable a toolbar button */
	virtual void Enable(const UINT nCommandID, bool bEnable = true)
	{
		if (m_pToolBar)
		{
			m_pToolBar->EnableButton(nCommandID, bEnable);
		}
	}

	//@cmember
	/* Set the toolbar button as a radio button */
	virtual void SetRadio(const UINT nCommandID, bool bSetRadio = true)
	{
		// To get rid of warning...
		nCommandID;
		SetCheck(bSetRadio);
	}

	//@cmember
	/* Press or unpress the toolbar button. */
	virtual void SetCheck(const UINT nCommandID, int nState = 1)
	{
		if (m_pToolBar)
		{
			m_pToolBar->CheckButton(nCommandID, nState);
		}
	}

	// The GUID map implements the QueryGuid function
	BEGIN_GUID_MAP(CUIUpdateToolbar)
		GUID_ENTRY(IUIUpdateElement)
		GUID_ENTRY(IQueryGuid)
		GUID_ENTRY(IRefCount)
	END_GUID_MAP
};

///////////////////////////////////////////////////////////////////////////
//
// CUIUpdateStatusbar
// CUIUpdateStatusbar - Menu updating wrapper class
//
//@doc CUIUpdateStatusbar
//
//@mfunc | CUIUpdateStatusbar | CUIUpdateStatusbar | Constructor
//@comm 
//
//@mfunc virtual void | CUIUpdateStatusbar | SetText | Set the pane text
//@parm UINT | nCommandID | ID associated with the pane
//@parm LPCTSTR | lpsz | New text for the menu
//@comm d
//
//@mfunc virtual void | CUIUpdateStatusbar | Enable | Enable or disable the pane 
//@parm UINT | nCommandID | ID associated with the menu 
//@parm bool | bEnable | Flag indicating enable or disable
//@comm 
//
//@mfunc virtual void | CUIUpdateStatusbar | SetRadio | Set or unset the radio button
//@parm UINT | nCommandID | ID associated with the pane  
//@parm bool | bSetCheck = true | Flag indicating set or unset 
//@comm 
//
//@mfunc virtual void | CUIUpdateStatusbar | SetCheck | Set or unset check
//@parm UINT | nCommandID | ID associated with the pane 
//@parm int | nState = 1| State of the check 
//@comm 
//
//@class CUIUpdateStatusbar | This class implements the IUIUpdateTarget
// interface for status bars.
//
// CUIUpdateStatusbar includes the following member functions 
//  <mf CUIUpdateStatusbar::CUIUpdateStatusbar>,
//  <mf CUIUpdateStatusbar::SetText>,
//  <mf CUIUpdateStatusbar::Enable>,
//  <mf CUIUpdateStatusbar::SetRadio>, and
//  <mf CUIUpdateStatusbar::SetCheck>,
//
class CUIUpdateStatusbar : public CRefCountImpl<IUIUpdateElement>
{
public:
	//@cmember
	/* Constructor attaches status bar to UIUpdate mechanism */
	CUIUpdateStatusbar(CStatusBar* pStatusBar)
	{
		m_pStatusBar = pStatusBar;
	}

// Attributes
protected:
	CStatusBar* m_pStatusBar;

// Operations
public:
	//@cmember
	/* Get the status bar */
	operator CStatusBar*()
	{
		return m_pStatusBar;
	}

	//@cmember
	/* Get the status bar */
	CStatusBar* GetStatusBar()
	{
		return m_pStatusBar;
	}

	//@cmember
	/* Set the pane text */
	virtual void SetText(const UINT nCommandID, LPCTSTR lpszText)
	{
		if (m_pStatusBar)
		{
			int nType = 0;

			int nPane = m_pStatusBar->IDToPaneIndex(nCommandID);

			m_pStatusBar->SetText(nPane, lpszText, nType);
		}
	}

	//@cmember
	/* Enable a pane */
	virtual void Enable(const UINT nCommandID, bool bEnable = true)
	{
		// To get rid of warning...
		nCommandID;
		bEnable;

		if (m_pStatusBar)
		{
		}
	}

	//@cmember
	/* SetRadio disabled for panes */
	virtual void SetRadio(const UINT nCommandID, bool bSetRadio = true)
	{
		// To get rid of warning...
		nCommandID;
		bSetRadio;

		// Can't do it...
	}

	//@cmember
	/* SetCheck disabled for panes */
	virtual void SetCheck(const UINT nCommandID, int nState = 1)
	{
		// To get rid of warning...
		nCommandID;
		nState;

		if (m_pStatusBar)
		{
		}
	}

	// The GUID map implements the QueryGuid function
	BEGIN_GUID_MAP(CUIUpdateStatusbar)
		GUID_ENTRY(IUIUpdateElement)
		GUID_ENTRY(IQueryGuid)
		GUID_ENTRY(IRefCount)
	END_GUID_MAP
};

///////////////////////////////////////////////////////////////////////////
// CUIUpdateGenerator
//
// CUIUpdateGenerator - Menu updating wrapper class
//
//@doc CUIUpdateGenerator
//
//@mfunc | CUIUpdateGenerator | CUIUpdateGenerator | Constructor
//
//@mfunc void | CUIUpdateGenerator | SetUIUpdateRouter | Set the UIUpdateRouter
//@parm <c IEventRouter>* | pIRouter | UI router to add
//@comm 
//
//@mfunc void | CUIUpdateGenerator | RegisterMenu | Register a menu with the UIUpdating mechanism
//@parm HMENU | hMenu | Menu to register
//@comm 
//
//@mfunc void | CUIUpdateGenerator | RegisterToolBar | Register a Toolbar with the UIUpdating mechanism
//@parm <c CToolBar>* | pToolBar | Toolbar to register
//@comm 
//
//@mfunc void | CUIUpdateGenerator | RegisterStatusBar | Register a status bar with the UIUpdating mechanism
//@parm <c CStatusBar>* | pStatusBar | Status bar to register
//@comm 
//
//@mfunc int | CUIUpdateGenerator | HandleMenu | Handle a menu by sending it through the machinery
//?@rdesc Returns the number of items handled 
//@parm HMENU | hMenu | Menu to handle 
//@comm 
//
//@mfunc int | CUIUpdateGenerator | HandleMenu | Handle a menu that's already encased within a menu wrapper by sending it through the machinery
//@rdesc Returns the number of items handled 
//@parm <c CUIUpdateMenu>* | pUIUpdateMenu | Menu wrapper to handle 
//@comm 
//
//@mfunc int | CUIUpdateGenerator | HandleToolBar | Handle a single toolbar by sending it through the machinery
//@rdesc Returns the number of items handled 
//@parm <c CToolBar>* | pToolBar | Toolbar to handle 
//@comm 
//
//@mfunc int | CUIUpdateGenerator | HandleToolBar | Handle a toolbar that's already encased within a toolbar wrapper by sending it through the machinery
//@rdesc Returns the number of items handled 
//@parm <c CUIUpdateToolbar>* | pUIUpdateToolbar | Toolbar wrapper to handle 
//@comm 
//
//@mfunc int | CUIUpdateGenerator | HandleStatusBar | Handle a single status bar by sending it through the machinery
//@rdesc Returns the number of items handled 
//@parm <c CStatusBar>* | pStatus Bar | Status bar to handle 
//@comm 
//
//@mfunc int | CUIUpdateGenerator | HandleStatusBar | Handle a status bar that's already encased within a status bar wrapper by sending it through the machinery
//@rdesc Returns the number of items handled 
//@parm <c CUIUpdateStatusBar>* | pUIUpdateStatusBar | Status bar wrapper to handle 
//@comm 
//
//@mfunc void | CUIUpdateGenerator | HandleMenus | Handle all the registered menus
//@comm 
//
//@mfunc void | CUIUpdateGenerator | HandleToolbars | Handle all the registered toolbars
//@comm 
//
//@mfunc void | CUIUpdateGenerator | HandleStatusBars | Handle all the registered status bars
//@comm 
//
//@mfunc virtual void | CUIUpdateGenerator | GenerateUIUpdates | Generate UI Updates 
//@comm Default implementation simply calls <mf CUIUpdateGenerator::HandleMenus>, 
// <mf CUIUpdateGenerator::HandleToolbars>, and <mf CUIUpdateGenerator::HandleStatusBars>.
//
//@mfunc virtual void | CUIUpdateGenerator | ReleaseUIElements | Release the UIElements 
//@comm Default implementation simply releases all registered menus, toolbars, and status bars
//
//@class CUIUpdateGenerator | This class fires UI update events to a
// specified event router for the set of registered UI update target
// objects.
//
// CUIUpdateGenerator includes the following member functions 
//  <mf CUIUpdateGenerator::CUIUpdateGenerator>,
//  <mf CUIUpdateGenerator::HandleMenu>,
//  <mf CUIUpdateGenerator::HandleToolBar>,
//  <mf CUIUpdateGenerator::HandleStatusBar>, 
//  <mf CUIUpdateGenerator::HandleMenus>,
//  <mf CUIUpdateGenerator::HandleToolBars>,
//  <mf CUIUpdateGenerator::HandleStatusBars>,
//  <mf CUIUpdateGenerator::GenerateUIUpdates>,
//  <mf CUIUpdateGenerator::ReleaseUIElements>
//
//
//
class CUIUpdateGenerator
{
// Embedded types
public:
	typedef std::vector<CUIUpdateMenu*> Menus;
	typedef std::vector<CUIUpdateToolbar*> Toolbars;
	typedef std::vector<CUIUpdateStatusbar*> Statusbars;

// Constructors/destructor
public:
	//@cmember
	/* Constructor */
	CUIUpdateGenerator()
	{
		m_pUIUpdateRouter = NULL;
	}

	//@cmember
	/* Destructor releases the registered update router */
	virtual ~CUIUpdateGenerator()
	{
		if (m_pUIUpdateRouter != NULL)
		{
			m_pUIUpdateRouter->Release();
		}

		ReleaseUIElements();
	}

// Attributes
protected:
	IEventRouter* m_pUIUpdateRouter;
	Menus m_Menus;
	Toolbars m_Toolbars;
	Statusbars m_Statusbars;

// Operations
public:
	//@cmember
	/* Registers an event router */
	void SetUIUpdateRouter(IEventRouter* pIRouter)
	{
		if (pIRouter != NULL)
		{
			pIRouter->AddRef();
		}

		if (m_pUIUpdateRouter != NULL)
		{
			m_pUIUpdateRouter->Release();
		}

		m_pUIUpdateRouter = pIRouter;
	}

	//@cmember
	/* Registers a menu with the UIUpdating mechanism */
	void RegisterMenu(HMENU hMenu)
	{
		if (hMenu)
		{
			CUIUpdateMenu* pUIUpdateMenu = new CUIUpdateMenu(hMenu);
			_ASSERTE(pUIUpdateMenu);
			pUIUpdateMenu->AddRef();
			m_Menus.push_back(pUIUpdateMenu);

			int nIndexMax = ::GetMenuItemCount(hMenu);

			for (int nIndex = 0; nIndex < nIndexMax; nIndex++)
			{
				HMENU hSubMenu = ::GetSubMenu(hMenu, nIndex);
				if (hSubMenu)
				{
					RegisterMenu(hSubMenu);
				}
			}
		}
	}

	//@cmember
	/* Registers a toolbar with the UIUpdating mechanism */
	void RegisterToolBar(CToolBar* pToolBar)
	{
		if (pToolBar)
		{
			CUIUpdateToolbar* pUIUpdateToolbar = new CUIUpdateToolbar(pToolBar);
			_ASSERTE(pUIUpdateToolbar);
			pUIUpdateToolbar->AddRef();
			m_Toolbars.push_back(pUIUpdateToolbar);
		}
	}

	//@cmember
	/* Registers a status bar with the UIUpdating mechanism */
	void RegisterStatusBar(CStatusBar* pStatusBar)
	{
		if (pStatusBar)
		{
			CUIUpdateStatusbar* pUIUpdateStatusbar = new CUIUpdateStatusbar(pStatusBar);
			_ASSERTE(pUIUpdateStatusbar);
			pUIUpdateStatusbar->AddRef();
			m_Statusbars.push_back(pUIUpdateStatusbar);
		}
	}

	//@cmember
	/* Handle a single menu  */
	int HandleMenu(HMENU hMenu)
	{
		int nItems = 0;

		if (hMenu == NULL)
			return 0;

		CUIUpdateMenu* pUIUpdateMenu = new CUIUpdateMenu(hMenu);
		_ASSERTE(pUIUpdateMenu);
		pUIUpdateMenu->AddRef();

		nItems += HandleMenu(pUIUpdateMenu);

		int nIndexMax = ::GetMenuItemCount(hMenu);

		for (int nIndex = 0; nIndex < nIndexMax; nIndex++)
		{
			HMENU hSubMenu = ::GetSubMenu(hMenu, nIndex);
			if (hSubMenu)
			{
				nItems += HandleMenu(hSubMenu);
			}
		}

		pUIUpdateMenu->Release();

		return nItems;
	}

	//@cmember
	/* Handle a single menu  */
	int HandleMenu(CUIUpdateMenu* pUIUpdateMenu)
	{
		if (pUIUpdateMenu == NULL)
			return 0;

		int nItems = 0;

		// For each UIElement, grab the command ID,
		//  create a command target wrapper for it...
		int nIndexMax = ::GetMenuItemCount(*pUIUpdateMenu);

		for (int nIndex = 0; nIndex < nIndexMax; nIndex++)
		{
			HMENU hSubMenu = ::GetSubMenu(*pUIUpdateMenu, nIndex);

			if (!hSubMenu)
			{
				// Check to find out if it's a separator...
				UINT nCommandID = GetMenuItemID(*pUIUpdateMenu, nIndex);

				if (m_pUIUpdateRouter != NULL)
				{
					CUIUpdateEvent* pUIUpdateEvent;

					pUIUpdateEvent = new CUIUpdateEvent(nCommandID, pUIUpdateMenu);
					_ASSERTE(pUIUpdateEvent);
					pUIUpdateEvent->AddRef();

					m_pUIUpdateRouter->RouteEvent(pUIUpdateEvent);

					pUIUpdateEvent->Release();

					nItems++;
				}
			}
		}

		return nItems;
	}

	//@cmember
	/* Handle a single toolbar  */
	int HandleToolBar(CToolBar* pToolBar)
	{
		int nItems = 0;
		CUIUpdateToolbar* pUIUpdateToolbar = new CUIUpdateToolbar(pToolBar);
		_ASSERTE(pUIUpdateToolbar);
		pUIUpdateToolbar->AddRef();
		nItems = HandleToolBar(pUIUpdateToolbar);
		pUIUpdateToolbar->Release();
		return nItems;
	}

	//@cmember
	/* Handle a single status bar */
	int HandleToolBar(CUIUpdateToolbar* pUIUpdateToolbar)
	{
		if (pUIUpdateToolbar == NULL)
			return 0;

		CToolBar* pToolBar = pUIUpdateToolbar->GetToolBar();
		if (pToolBar == NULL)
			return 0;

		int nItems = pToolBar->GetButtonCount();

		for (int nIndex = 0; nIndex < nItems; nIndex++)
		{
			TBBUTTON tbButton;
			if (pToolBar->GetButton(nIndex, &tbButton))
			{
				if (m_pUIUpdateRouter != NULL)
				{
					CUIUpdateEvent* pUIUpdateEvent;

					pUIUpdateEvent = new CUIUpdateEvent(tbButton.idCommand, pUIUpdateToolbar);
					_ASSERTE(pUIUpdateEvent);
					pUIUpdateEvent->AddRef();

					m_pUIUpdateRouter->RouteEvent(pUIUpdateEvent);

					pUIUpdateEvent->Release();
				}
			}
		}

		return nItems;
	}

	//@cmember
	/* Handle a single status bar  */
	int HandleStatusBar(CStatusBar* pStatusBar)
	{
		int nItems = 0;
		CUIUpdateStatusbar* pUIUpdateStatusbar = new CUIUpdateStatusbar(pStatusBar);
		_ASSERTE(pUIUpdateStatusbar);
		pUIUpdateStatusbar->AddRef();
		nItems = HandleStatusBar(pUIUpdateStatusbar);
		pUIUpdateStatusbar->Release();
		return nItems;
	}

	//@cmember
	/* Handle a single status bar   */
	int HandleStatusBar(CUIUpdateStatusbar* pUIUpdateStatusbar)
	{
		if (pUIUpdateStatusbar == NULL)
			return 0;

		CStatusBar* pStatusBar = pUIUpdateStatusbar->GetStatusBar();
		if (pStatusBar == NULL)
			return 0;

		// go through each pane... 
		int nPanes;

		nPanes = pStatusBar->m_nPanes;

		for (int i = 0; i < nPanes; i++)
		{
			// create a status pane update target object
			STATUSPANEINFO* pStatusPaneInfo;

			pStatusPaneInfo = pStatusBar->m_pStatusPanes + i;
			UINT nCommandID = pStatusPaneInfo->m_ID;

			if (m_pUIUpdateRouter != NULL)
			{
				CUIUpdateEvent* pUIUpdateEvent;

				pUIUpdateEvent = new CUIUpdateEvent(nCommandID, pUIUpdateStatusbar);
				_ASSERTE(pUIUpdateEvent);
				pUIUpdateEvent->AddRef();

				m_pUIUpdateRouter->RouteEvent(pUIUpdateEvent);

				pUIUpdateEvent->Release();
			}
		}

		return nPanes;
	}

	//@cmember
	/* Handle all registered menus */
	void HandleMenus()
	{
		for (Menus::iterator posMenu = m_Menus.begin(); posMenu != m_Menus.end(); ++posMenu)
		{
			HandleMenu(*posMenu);
		}
	}

	//@cmember
	/* Handle all registered toolbars */
	void HandleToolbars()
	{
		for (Toolbars::iterator posToolBar = m_Toolbars.begin(); posToolBar != m_Toolbars.end(); ++posToolBar)
		{
			HandleToolBar(*posToolBar);
		}
	}

	//@cmember
	/* Handle all registered statusbars */
	void HandleStatusBars()
	{
		for (Statusbars::iterator posStatusBar = m_Statusbars.begin(); posStatusBar != m_Statusbars.end(); ++posStatusBar)
		{
			HandleStatusBar(*posStatusBar);
		}
	}

	//@cmember
	/* Generate UIUpdates */
	virtual void GenerateUIUpdates()
	{
		HandleMenus();
		HandleToolbars();
		HandleStatusBars();
	}

	CUIUpdateMenu* LocateMenu(HMENU hMenu)
	{
		for (Menus::iterator posMenu = m_Menus.begin(); posMenu != m_Menus.end(); ++posMenu)
		{
			if ((*posMenu)->GetMenu() == hMenu)
				return *posMenu;
		}
		return NULL;
	}

	CUIUpdateToolbar* LocateToolbar(CToolBar* pToolBar)
	{
		for (Toolbars::iterator posToolBar = m_Toolbars.begin(); posToolBar != m_Toolbars.end(); ++posToolBar)
		{
			if ((*posToolBar)->GetToolBar() == pToolBar)
				return *posToolBar;
		}
		return NULL;
	}

	CUIUpdateStatusbar* LocateStatusbar(CStatusBar* pStatusBar)
	{
		for (Statusbars::iterator posStatusBar = m_Statusbars.begin(); posStatusBar != m_Statusbars.end(); ++posStatusBar)
		{
			if ((*posStatusBar)->GetStatusBar() == pStatusBar)
				return *posStatusBar;
		}
		return NULL;
	}

// Implementation
protected:
	//@cmember
	/* Release registered UI Elements */
	virtual void ReleaseUIElements()
	{
		while (m_Menus.size() > 0)
		{
			CUIUpdateMenu* pUIUpdateMenu = m_Menus.back();
			pUIUpdateMenu->Release();
			m_Menus.pop_back();
		}

		while (m_Toolbars.size() > 0)
		{
			CUIUpdateToolbar* pUIUpdateToolbar = m_Toolbars.back();
			pUIUpdateToolbar->Release();
			m_Toolbars.pop_back();
		}

		while (m_Statusbars.size() > 0)
		{
			CUIUpdateStatusbar* pUIUpdateStatusbar = m_Statusbars.back();
			pUIUpdateStatusbar->Release();
			m_Statusbars.pop_back();
		}
	}
};

};	// namespace stingray::foundation
};	// namespace stingray
