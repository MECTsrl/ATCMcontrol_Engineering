// 
// Stingray Software Extension Classes
// Copyright (C) 1995 Stingray Software Inc.
// All Rights Reserved
// 
// This source code is only intended as a supplement to the
// Stingray Extension Class product.
// See the SEC help files for detaild information
// regarding using SEC classes.
// 
//  Authors:		Don
//  Description:	Declarations for SECToolBarManager
//  Created:		August 1996
//

#ifdef WIN32

#ifndef __TBARMGR_H__
#define __TBARMGR_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#ifndef __SBARMGR_H__
#include "toolkit\sbarmgr.h"
#endif

#ifndef __TBARCUST_H__
#include "toolkit\tbarcust.h"
#endif

#ifndef __TBTNSTD_H__
#include "toolkit\tbtnstd.h"
#endif

#ifndef __SDOCKSTA_H__
#include "toolkit\sdocksta.h"
#endif

#ifndef __TBMPMGR_H__
#include "toolkit\TBmpMgr.h"
#endif

//
// SEC Extension DLL
// Initialize declaration context
//

#ifdef _SECDLL
	#undef AFXAPP_DATA
	#define AFXAPP_DATA AFXAPI_DATA
	#undef AFX_DATA
	#define AFX_DATA    SEC_DATAEXT
#endif //_SECDLL

/////////////////////////////////////////////////////////////////////////////
// SECMenuMap - used in calls to SetMenuMap (maps menu resource ID to
//              bit flag)
//
struct SECMenuMap
{
	UINT  m_nID;				// Menu resource ID	
	DWORD m_dwBit;				// Bit setting
};

/////////////////////////////////////////////////////////////////////////////
// SECToolBarManager
//

// Registered messages forwarded by ToolBar manager
extern const int wmSECNotifyMsg;			// ToolBar notification
extern const int wmSECToolBarWndNotify;		// SECWndBtn derivative notification

//@doc SECToolBarManager 
//@class Implements the toolbar management to allow runtime customization of SECCustomToolBar/SECMenuBar objects.
//@BASE public | SECControlBarManager
//@xref <c SECControlBarManager> <c SECCustomToolBar> <c SECMenuBar>
class SECToolBarManager : public SECControlBarManager
{
	DECLARE_DYNAMIC(SECToolBarManager)

// Construction
public:
	
	//@access Construction/Initialization

	//@cmember
	/* Constructor*/
	SECToolBarManager();

	//@cmember
	/* Constructor*/
	SECToolBarManager(CFrameWnd* pFrameWnd,CFrameWnd* pOwnerFrame=NULL);

	//@access Operations
public:
	//@cmember
	/* Define a customizable toolbar default state.*/
	void DefineDefaultToolBar(UINT nID, 
							  const CString& strTitle, 
							  UINT    nBtnCount, 
							  UINT*   lpBtnIDs,
							  DWORD   dwAlignment   = CBRS_ALIGN_ANY,
							  UINT    nDockBarID    = AFX_IDW_DOCKBAR_TOP,
							  UINT    nDockNextToID = NULL,
							  BOOL    bDocked       = TRUE,
							  BOOL    bVisible      = TRUE);

	//@cmember
	/* Define a customizable toolbar default state based on a toolbar resource.*/
	void DefineDefaultToolBar(UINT nID, 
							  const CString& strTitle, 
							  UINT    nToolbarID,
							  UINT&	  nRetButtonCount,
							  UINT*&  pRetButtonArray,
							  DWORD   dwAlignment   = CBRS_ALIGN_ANY,
							  UINT    nDockBarID    = AFX_IDW_DOCKBAR_TOP,
							  UINT    nDockNextToID = NULL,
							  BOOL    bDocked       = TRUE,
							  BOOL    bVisible      = TRUE);

	// Defines the layout of the menu bar.
	void LayoutMenuBar(UINT nBtnCount, UINT* lpBtnIDs);
	void GetMenuBarLayout(UINT& nBtnCount, const UINT*& lpBtnIDs) const;

	// Iterates all menu resource ids used to preconfigure the menubar.
	BOOL SetMenuInfo(int nCount, UINT nIDMenu, ...);

	// Returns TRUE if the mainframe is currently enabled. FALSE if we are
	// in customize-mode and we are pretending that it is disabled.
	BOOL IsMainFrameEnabled();

	// Put's all toolbars in/out of customize mode.
	void EnableConfigMode(BOOL bEnable=TRUE);
	BOOL InConfigMode() const;

	// @cmember
	/* Returns TRUE (with the btn rect. in question) if we are currently processing a button command.*/
	BOOL IsToolBarCommand(CRect& rect) const;

	//@cmember
	/* Returns the toolbar under the given window rect.*/
	virtual SECCustomToolBar* ToolBarUnderRect(const CRect& rect) const;

	//@cmember
	/* Returns the toolbar pointer for a given default toolbar ID. */
	virtual SECCustomToolBar* ToolBarFromID(const UINT nToolBarID) const;

	// Returns the buttons for a given default toolbar ID.
	BOOL GetDefaultBtns(UINT nID, int& nCount, UINT*& lpBtnIDs);

	//@cmember
	/* Creates a new 'user' toolbar*/
	SECCustomToolBar* CreateUserToolBar(LPCTSTR lpszTitle = NULL);

	//@cmember
	/* Loads the toolbar resource (to be shared by all toolbars)*/
	BOOL LoadToolBarResource(LPCTSTR lpszStdBmpName, 
							 LPCTSTR lpszLargeBmpName = NULL);

	//@cmember
	/* Loads the toolbar resource (to be shared by all toolbars)*/
	BOOL LoadToolBarResource(UINT nIDStdBmp,
							 UINT nIDLargeBmp = 0);
	
	// Alternative's to LoadToolBarResource ... used for defining several
	// toolbar resources, then a singe call to LoadToolBarResource() to load
	// them.

	//@cmember
	/* Loads the toolbar resource (to be shared by all toolbars)*/
	BOOL LoadToolBarResource();

	//@cmember
	/* Appends a toolbar resource to the current LoadToolBarResource image*/
	BOOL AddToolBarResource(LPCTSTR lpszStdBmpName, 
							LPCTSTR lpszLargeBmpName = NULL);

	//@cmember
	/* Appends a toolbar resource to the current LoadToolBarResource image*/
	BOOL AddToolBarResource(UINT nIDStdBmp,
							UINT nIDLargeBmp = 0);

	//@cmember
	/* Append a new bitmap resource for use with the available custom toolbar buttons*/
	BOOL AddBitmapResource(LPCTSTR lpszStdBmpName,
						   LPCTSTR lpszLargeBmpName,
						   const UINT* lpIDArray, UINT nIDCount);

	//@cmember
	/* Append a new bitmap resource for use with the available custom toolbar buttons*/
	BOOL AddBitmapResource(UINT nIDstdBmpName,
						   UINT nIDLargeBmpName,
						   const UINT* lpIDArray, UINT nIDCount);

	//@cmember
	/* Append a new bitmap for use with the available custom toolbar buttons*/
	BOOL AddBitmap(HBITMAP hBmpSmall, HBITMAP hBmpLarge,
				   const UINT* lpIDArray, UINT nIDCount);

	// Returns the toolbar managers, bitmap manager object.
	SECBmpMgr* GetBmpMgr() const { return m_pBmpMgr; }

	// Returns the rectangle within which toolbar buttons cannot be dropped
	void GetNoDropRect(CRect& rect) const;

	//@cmember
	/* Runs the view toolbars dialog.*/
	int ExecViewToolBarsDlg();

	//@cmember
	/* Passes notification through to all buttons of nID*/
	void InformBtns(UINT nID, UINT nCode, void* pData);

	// Functions for enabling/disabling tooltips and flyby help
	//@access State Configuration

	//@cmember
	/* Enable tooltips for all custom toolbars*/
	void EnableToolTips(BOOL bEnable=TRUE);

	//@cmember
	/* Enable flyby help for all custom toolbars*/
	void EnableFlyBy(BOOL bEnable=TRUE);

	//@cmember
	/* Return current tooltip state*/
	BOOL ToolTipsEnabled() const;

	//@cmember
	/* Return current flyby help state*/
	BOOL FlyByEnabled() const;

	//@cmember
	/* Enable large button mode*/
	void EnableLargeBtns(BOOL bEnable=TRUE);

	//@cmember
	/* Return current large button mode status*/
	BOOL LargeBtnsEnabled() const;

	//@cmember
	/* Enable "cool look" mode*/
	void EnableCoolLook(BOOL bEnable=TRUE,DWORD dwExCoolLookStyles=CBRS_EX_COOLBORDERS|CBRS_EX_GRIPPER);

	//@cmember
	/* Return current "cool look" mode*/
	BOOL CoolLookEnabled() const;

	// Function to initialise info in toolbar.
	void SetToolBarInfo(SECCustomToolBar* pToolBar);

	// Initializes the menu map information across all menubars
	void SetMenuMap(const SECMenuMap* pMap, int nCount);

	//@cmember
	/* Intialize the toolbar button map information across all toolbars*/
	void SetButtonMap(const SECBtnMapEntry* pMap);

	//@cmember
	/* Get the button map information*/
	const SECBtnMapEntry* GetButtonMap() const;

	// Return the aux button map.
	const SECBtnMapEntry* GetAuxButtonMap() const;

	// Save/Restore manager state (i.e. tooltips, cool look etc.)
	virtual void LoadState(LPCTSTR lpszProfileName);
	virtual BOOL LoadState(SECPersistentTreeNode* pBarNode);	
	virtual void SaveState(LPCTSTR lpszProfileName) const;
	virtual BOOL SaveState(SECPTNFactory& nodeFactory,
						   SECPersistentTreeNode* pParent);		

	// This is called by LoadState to establish default toolbar
	// layout when no configured workspace was loaded. If you
	// are not using a workspace manager, call this directly
	// to establish a default toolbar layout.

	//@cmember
	/* Load a default toolbar configuration state.*/
	virtual void SetDefaultDockState();

	// Support for dynamic control bar save/restore. Use when
	// a variable number of control bars of a particular type
	// must be saved and restored.  (i.e. User-defined toolbars)
	virtual DWORD GetBarTypeID(CControlBar* pBar) const;
	virtual SECControlBarInfoEx* CreateControlBarInfoEx(SECControlBarInfo*) const;
	virtual CControlBar* DynCreateControlBar(SECControlBarInfo* pBarInfo);

// Overrideables
protected:

// Implementation
public:
	virtual ~SECToolBarManager();

	// Class used to hold info. about the current commands
	class SECCurrentCommand
	{
	public:
		SECCurrentCommand() : m_nRef(1), m_bIn(FALSE) {}

		// This object is reference counted ...
		LONG AddRef();
		LONG Release();

		// Functions to set/get the command state
		void SetCommand(BOOL bIn, const CRect& rect);
		BOOL InCommand(CRect& rect) const;

	protected:
		~SECCurrentCommand();

		LONG  m_nRef;		// My reference count
		CRect m_rect;		// Rectangle of button executing current command
		BOOL  m_bIn;		// TRUE if a button is current executing a command
	};

	// Enable/Disable the main frame while a customize dialog is up (disabling
	// all child windows of the mainframe).
	void EnableMainFrame();
	void DisableMainFrame();

	// Function returns the object that stores the current command state. Note
	// that the object is reference counted, so you must call Release when you
	// have finished with it.
	SECCurrentCommand* GetCurCommandObject() const;

	// Function which receives notifications from toolbar.
	void BarNotify(int nID, SECCustomToolBar::BarNotifyCode notify);

	// Function specifing which window should receive the toolbar notifications
	void AddNotify(CWnd* pWnd);
	void RemoveNotify(CWnd* pWnd);

	// Sets a window within which buttons cannot be dropped.
	void SetNoDropWnd(CWnd* pWnd);

	// Returns the reference to pointer to toolbar with current config focus.
	SECCustomToolBar* GetConfigFocus() const { return m_pConfigFocus; }
	void SetConfigFocus(SECCustomToolBar* pFocus) { m_pConfigFocus = pFocus; }

	SECBtnDrawData& GetDrawData() { return m_drawData; }

	void ChangeBarStyle(BOOL bAdd, DWORD dwStyle);
	void ChangeExBarStyle(BOOL bAdd, DWORD dwExStyle);

	// Performs one-time initialisation
	void Initialise();

protected:
	// Toggles bitmap between large and small
	virtual BOOL ChangeBitmap();

	virtual void ReparentToolBars();

	// Implementation, data members.
	struct SECDefaultToolBar			// Record for a default toolbar
	{
		UINT    nID;					// The toolbar ID
		CString lpszTitle;				// The toolbar title
		UINT    nBtnCount;				// No. of default buttons
		UINT*   lpBtnIDs;				// List of default buttons
		DWORD   dwAlignment;			// Default alignment mode 
		UINT    nDockBarID;				// Default bar to dock in
		UINT    nDockNextToID;			// Default bar to dock next to
		BOOL    bDocked;				// FALSE if bar is initialially float
		BOOL    bVisible;				// TRUE if bar is visible
	};

	CDWordArray m_enabledList;			// List of windows which have been
										// disabled (EnableMainFrame)
	CPtrArray m_defaultBars;			// Array of default toolbars
	CPtrArray m_notifyWnds;				// Wnd's to pass toolbar notifications
	CWnd* m_pNoDropWnd;					// Wnd that won't accept dropped btns
	BOOL m_bMainFrameEnabled;			// TRUE if EnableMainFrame called
	BOOL m_bConfig;						// TRUE if in toolbar customize mode
	BOOL m_bToolTips;					// TRUE if tooltips enabled
	BOOL m_bFlyBy;						// TRUE if flyby help enabled
	BOOL m_bCoolLook;					// TRUE if "Cool" look enabled
	BOOL m_bLargeBmp;					// TRUE if using large bitmaps
	const SECBtnMapEntry* m_pBtnMap;	// Maps ID's to button types.
	SECBtnMapEntry* m_pAuxBtnMap;		// Aux version of button map.
	SECCustomToolBar* m_pConfigFocus;	// Toolbar with current config focus
	SECCurrentCommand* m_pCmd;			// Holds status about current command
	UINT* m_lpMenuBarBtnIDs;			// IDs for default menu bar layout
	UINT m_nMenuBarBtnCount;			// No. of btns in m_lpMenuBarBtnIDs
	SECBmpMgr* m_pBmpMgr;				// Manages all our bitmap resources
	SECBtnDrawData m_drawData;			// Shared drawing resources
	CFrameWnd* m_pOwnerFrame;			// Owner frame window, don't confuse with parent

public:
	CRuntimeClass* m_pToolBarClass;		// Used to create toolbars.
};

//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif // __TBARMGR_H__

#endif // WIN32

