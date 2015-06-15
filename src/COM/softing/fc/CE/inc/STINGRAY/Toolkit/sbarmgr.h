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
//  Authors:		Dean Hallman
//  Description:	Declarations for SECControlBarManager
//  Created:		August 1996
//

#ifdef WIN32

#ifndef __SBARMGR_H__
#define __SBARMGR_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#ifndef __SECPERSTREE_H__
#include "toolkit\perstree.h"
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

// Forward Declarations
class SECControlBarInfo;
class SECControlBarInfoEx;

/////////////////////////////////////////////////////////////////////////////
// SECControlBarManager
//

/////////////////////////////
// AutoDuck tag block block for SECControlBarManager 

// adinsert AutoDuck insertion point for SECControlBarManager 
//@doc SECControlBarManager 
//@class The SECControlBarManager class manages control bars as well as the
// state of an application's main frame.  It supports dynamic save and restore
// of control bars.
//@comm
// Included in Objective Toolkit is <c SECControlBar>, which replaces CControlBar as
// the base class for all your control bars.  SECControlBar adds support for sizing
// while docked, automatic stretching when resized and a default context menu with
// facilities for adding and removing menu items.
//
// See the VIZ sample in the \SAMPLES\TOOLKIT\STANDARD\DOCKING\VIZ directory for a demonstration 
// of this class.
//@base public | CObject
class SECControlBarManager : public CObject
{
	DECLARE_DYNAMIC(SECControlBarManager)

// Construction
public:
	//@cmember
	/* Default constructor.*/
	SECControlBarManager();
	//@cmember
	/* Constructor taking a pointer to the parent frame window.*/
	SECControlBarManager(CFrameWnd* pFrameWnd);

// Operations
public:
	// Returns the a pointer to the main frame.
	//@cmember
	/* Returns the a pointer to the main frame.*/
	virtual CFrameWnd* GetFrameWnd() const;

	// Save/Restore manager state
	// virtual overload provided for over-ride only. No functionality implemented.
	virtual void LoadState(LPCTSTR lpszProfileName);

	// Locates and loads the PTN object
	virtual BOOL LoadState(SECPersistentTreeNode* pRoot);

	// virtual overload provided for over-ride only. No functionality implemented.
	virtual void SaveState(LPCTSTR lpszProfileName) const;

	// Support for dynamic control bar save/restore. Use when
	// a variable number of control bars of a particular type
	// must be saved and restored.  (i.e. User-defined toolbars)
	virtual BOOL SaveState(SECPTNFactory& nodeFactory,
						   SECPersistentTreeNode* pParent);	
	//@cmember
	/* Virtual overload provided for override only. No functionality implemented in this base class. It will differentiate between a tool bar and a menu bar in the custom classes.*/
	virtual DWORD GetBarTypeID(CControlBar* pBar) const;
	//@cmember
	/* Virtual overload provided for override only. No functionality implemented in this base class. It will create an appropriate extended information pointer (pInfoEx) for the bar info provided so that the bar may be persistant.*/
	virtual SECControlBarInfoEx* CreateControlBarInfoEx(SECControlBarInfo*) const;
	// Undock member Virtual overload provided for override only. No functionality implemented in this base class. It will dynamicly create a bar from a SECCONTROLBARINFO pointer.
	virtual CControlBar* DynCreateControlBar(SECControlBarInfo* pBarInfo);

// Overrideables
protected:

// Implementation
public:
	// Destructor
	virtual ~SECControlBarManager();

// Attributes
protected:
	// Protected pointer to the main frame
	CFrameWnd* m_pFrameWnd;	

protected:
};

//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif // __SBARMGR_H__

#endif // WIN32

