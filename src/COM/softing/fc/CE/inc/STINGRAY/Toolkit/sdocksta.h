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
//  Author:			Dean Hallman
//  Description:	Declarations for SECControlBarInfo and SECDockState
//  Created:		August 1996
//
#if defined(WIN32)

#ifndef __SDOCKSTA_H__
#define __SDOCKSTA_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#ifndef __SECPERSTREE_H__
#include "toolkit\perstree.h"
#endif

#ifndef __SBARCORE_H__
#include "toolkit\sbarcore.h"
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
class SECControlBarManager;

/////////////////////////////////////////////////////////////////////////////
// SECDockState
//
class SECDockState : public CDockState
{
	DECLARE_SERIAL(SECDockState)

// Construction
public:
	SECDockState();
	SECDockState(SECControlBarManager* pControlBarManager);

// Attributes
protected:
	SECControlBarManager* m_pControlBarManager;

// Operations
public:
	virtual void Serialize(CArchive& ar);
	virtual void SaveState(LPCTSTR lpszProfileName);
	virtual BOOL SaveState(SECPTNFactory& nodeFactory,SECPersistentTreeNode* pRoot);
	virtual void LoadState(LPCTSTR lpszProfileName);
	virtual BOOL LoadState(SECPersistentTreeNode* pRoot);
	virtual BOOL GetDockState(const CFrameWnd* pFrameWnd);
	virtual BOOL SetDockState(CFrameWnd* pFrameWnd);
	void Clear(); //deletes all the barinfo's
	void CleanUpControlBarState();

	// For memory management
	void SetAllocList(CPtrList* pList) { m_pAllocList=pList; }

	// Override to create your own controlbar info object
	virtual SECControlBarInfo* CreateCBInfoObject(SECControlBarManager* pMgr);

	// Utility functions used by SetDockState
	virtual void SetControlBarInfoObjects(CFrameWnd* pFrameWnd,BOOL bMDI);
	virtual void ApplyControlBarExtendedStyles();
	virtual void ApplyControlBarInfoMDIChildren(CFrameWnd* pFrameWnd);


// Implementation
public:
	~SECDockState();

protected:

	// Utility functions used by SetDockState
	virtual SECControlBar* DynCreateControlBar(const CString& strClassName,
							CFrameWnd* pParentWnd,SECControlBarInfo* pInfo);
	virtual CRuntimeClass* LocateRuntimeClass(const CString& strClassName);

	CPtrList* m_pAllocList;
};

/////////////////////////////////////////////////////////////////////////////
// SECControlBarInfoEx

class SECControlBarInfoEx : public CObject
{
	DECLARE_DYNCREATE(SECControlBarInfoEx)

// Constructors
public:
	SECControlBarInfoEx();

// Operations
public:
	virtual void Serialize(CArchive& ar, SECDockState* pDockState);
	virtual BOOL LoadState(LPCTSTR lpszSection, SECDockState* pDockState);
	virtual BOOL LoadState(SECPersistentTreeNode* pBarNode);
	virtual BOOL SaveState(LPCTSTR lpszSection);
	virtual BOOL SaveState(SECPTNFactory& nodeFactory,
						   SECPersistentTreeNode* pBarNode);
};

/////////////////////////////////////////////////////////////////////////////
// SECControlBarInfo

class SECControlBarInfo : public CControlBarInfo
{
public:
// Implementation
	SECControlBarInfo();
	SECControlBarInfo(SECControlBarManager* pControlBarManager);

// Attributes
  	CSize m_szDockHorz; // Dimensions when docked horizontally
	CPoint m_ptDockHorz;
	CSize m_szDockVert; // Dimensions when docked vertically
	CSize m_szFloat;    // Dimensions when floating
	DWORD m_dwMRUDockingState; // Remember docking state when "Allow Docking" unchecked
	DWORD m_dwDockStyle;
	float m_fPctWidth;
	DWORD m_dwStyle;    // creation style (used for layout)
	DWORD m_dwExStyle;  // Extended Style Bits
	BOOL m_bPreviouslyFloating;  // After unfloat as MDI child,
									// should we dock or float it
	BOOL m_bMDIChild;				// Floating as an MDI child
	WINDOWPLACEMENT m_wpMDIChild;	// WINDOWPLACEMENT info for MDI child.

	// New parms for dynamic controlbar creation
	CString m_strClassName;		// Class name 
	CString m_strWindowName;	// Window name
	int m_nResourceID;			// Dialogbar resource id

	SECControlBarInfoEx* m_pBarInfoEx;
	DWORD m_dwBarTypeID;
	SECControlBarManager* m_pControlBarManager;

	void Serialize(CArchive& ar, SECDockState* pDockState);
	virtual BOOL LoadState(LPCTSTR lpszProfileName, int nIndex, SECDockState* pDockState);
	virtual BOOL LoadState(SECPersistentTreeNode* pBarNode,SECDockState* pDockState);
	virtual BOOL SaveState(LPCTSTR lpszProfileName, int nIndex);
	virtual BOOL SaveState(SECPTNFactory& nodeFactory,SECPersistentTreeNode* pParent,
						   int nIndex);


// Implementation
public:
	virtual ~SECControlBarInfo();

protected:
	virtual BOOL LoadCControlBarInfoState(SECPersistentTreeNode* pBarNode,
										  SECDockState* pDockState);
	virtual BOOL LoadSECControlBarInfoState(SECPersistentTreeNode* pBarNode);
	virtual BOOL SaveCControlBarInfoState(SECPTNFactory& nodeFactory,
										  SECPersistentTreeNode* pBarNode);
	virtual BOOL SaveSECControlBarInfoState(SECPTNFactory& nodeFactory,
										  SECPersistentTreeNode* pBarNode);
};

//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif // __SDOCKSTA_H__

#endif // WIN32
