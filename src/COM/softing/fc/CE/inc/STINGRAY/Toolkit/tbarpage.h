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
//  Description:	Declarations for SECToolBarSheet/Page
//  Created:		May 1996
//

#ifdef WIN32

#ifndef __TBARPAGE_H__
#define __TBARPAGE_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#ifndef __SECRES_H__
#include "toolkit\secres.h"
#endif

#ifndef __TBARCUST_H__
#include "toolkit\tbarcust.h"
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
// Forward references
//
class SECToolBarManager;
class SECCustomizeToolBar;
class SECCustomToolBar;


/////////////////////////////////////////////////////////////////////////////
// SECToolBarSheet
//
/////////////////////////////
//@doc SECCustomToolBar
//@mfunc   | SECToolBarSheet | SECToolBarSheet | The number of buttons currently on the toolbar.
//@syntax SECToolBarSheet(UINT nIDCaption = IDS_TOOLBAR_CUSTOMIZE, Cwnd* pParentWnd = NULL, UINT iSelectPage = 0 : CPropertySheet(nIDCaption, pParentWnd, iSelectPage) {}
//@syntax SECToolBarSheet(LPCTSTR nIDCaption, Cwnd* pParentWnd = NULL, UINT iSelectPage = 0 : CPropertySheet(nIDCaption, pParentWnd, iSelectPage) {}
//@parm UINT | nIDCaption | Caption ID for toolbar property sheet.
//@parm LPCTSTR | nIDCaption | Resource name of caption for toolbar property sheet.
//@parm Cwnd* | pParentWnd | The parent window.
//@parm UINT | iSelectPage | Index of selected property sheet page.


// AutoDuck tag block block for SECToolBarSheet 
// adinsert AutoDuck insertion point for SECToolBarSheet 
//@doc SECToolBarSheet 
//@class The SECToolBarSheet class constructs a property sheet used in conjunction 
// with toolbar button templates created by the classes SECToolBarCmdPage and 
// SECToolBarsPage.  
//@comm
// SECToolBarSheet supports a Customize dialog that lets the user 
// create a custom toolbar with a set of buttons in a style geared toward a particular 
// task.  This dialog displays a list of toolbars to the user, and allows the user to 
// manipulate these toolbars.  Through the toolbar dialog, the user can choose between 
// large or small buttons, tooltips enabled or disabled, and conventional appearance 
// or the new cool look.
//
// See the sample TOOLBAR in the \SAMPLES\TOOLKIT\STANDARD\DOCKING\TOOLBAR directory for a demonstration of 
// this class.
//@xref <c SECCustomToolBar> <c SECToolBarCmdPage>
//@base public | CPropertySheet
class SECToolBarSheet : public CPropertySheet
{
// Construction
public:

	// If an ASSERT fires inside constructor, it is possible youd
	// did not include the Objective Toolkit resources. Please consult
	// the OT user's guide for the proper method of inclusion.
	//@cmember
	/* todo Add a description*/
	SECToolBarSheet(UINT nIDCaption = IDS_TOOLBAR_CUSTOMIZE, 
					CWnd* pParentWnd = NULL, UINT iSelectPage = 0) :
				CPropertySheet(nIDCaption, pParentWnd, iSelectPage) {}

	SECToolBarSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, 
					UINT iSelectPage = 0) :
				CPropertySheet(pszCaption, pParentWnd, iSelectPage) {}

protected:
	// Implementation, data members.

	// Implementation - message map and entries
	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT OnFloatStatus(WPARAM, LPARAM);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};


/////////////////////////////////////////////////////////////////////////////
// SECBtnGroup - Describes and manages the UI for a group of buttons
//
class SECBtnGroup : public CObject
{
public:
	SECBtnGroup(const CString& strTitle, UINT nBtnCount, UINT* lpBtnIDs);
	virtual ~SECBtnGroup();

	// Creates the user interface.
	virtual BOOL Create(CWnd* pParentWnd, SECToolBarManager* pManager, UINT nID) = 0;

	// Called to layout the user interface.
	virtual void DoSize(CRect& /* rect */) {}

	// Called to switch the user interface into config mode.
	virtual void EnableConfigMode(BOOL /* bEnable */) {}

	// Returns the window associated with the user interface
	CWnd*   GetWnd() const;

	// Returns the title of this group
	LPCTSTR GetTitle() const;

protected:
	CWnd*	m_pWnd;						// Pointer to groups window
	CString m_lpszTitle;				// Btn group title
	UINT    m_nBtnCount;				// No. of btns in group
	UINT*   m_lpBtnIDs;					// List of btn IDs.

private:
	SECBtnGroup();
};

inline CWnd* SECBtnGroup::GetWnd() const
{
	return m_pWnd;
}

inline LPCTSTR SECBtnGroup::GetTitle() const
{
	return m_lpszTitle;
}


/////////////////////////////////////////////////////////////////////////////
// SECTBtnGroup - Describes and manages the UI for a group of toolbar
//                buttons
//
class SECTBBtnGroup : public SECBtnGroup
{
public:
	SECTBBtnGroup(LPCTSTR lpszTitle, UINT nBtnCount, UINT* lpBtnIDs);

	virtual BOOL Create(CWnd* pParentWnd, SECToolBarManager* pManager, UINT nID);
	virtual void DoSize(CRect& rect);
	virtual void EnableConfigMode(BOOL bEnable);
};


/////////////////////////////////////////////////////////////////////////////
// SECMenuBtnGroup - Describes and manages the UI for a group of menu
//                   buttons
//
class SECMenuBtnGroup : public SECBtnGroup
{
public:
	SECMenuBtnGroup(LPCTSTR lpszTitle, UINT nBtnCount, UINT* lpBtnIDs);

	virtual BOOL Create(CWnd* pParentWnd, SECToolBarManager* pManager, UINT nID);
};


/////////////////////////////////////////////////////////////////////////////
// SECMenuListBox - implements a listbox of menus (which can be tracked
//                  to a toolbar.
//
class SECMenuListBox : public CListBox
{
	DECLARE_DYNAMIC(SECMenuListBox)

public:
	SECMenuListBox();
	virtual ~SECMenuListBox() {}

	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	void SetManager(SECToolBarManager* pManager);

protected:
	SECToolBarManager* m_pManager;			// Pointer to toolbar manager

	BOOL m_bWinVerLessThan4;
	UINT MenuItemFromPoint(CPoint pt, BOOL& bOutside) const;

	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

inline void SECMenuListBox::SetManager(SECToolBarManager* pManager)
{
	m_pManager = pManager;
}


/////////////////////////////////////////////////////////////////////////////
// SECToolBarCmdPage
//
/////////////////////////////
// AutoDuck tag block block for SECToolBarCmdPage 





// adinsert AutoDuck insertion point for SECToolBarCmdPage 
//@doc SECToolBarCmdPage 
//@class The <c SECToolBarCmdPage> class presents all the available toolbar buttons to 
// the user.  These buttons can be dragged onto an existing toolbar or used to 
// create a new toolbar.  SECToolBarCmdPage should only be used in an SECToolBarSheet 
// (cannot be used directly in a CPropertySheet).
// The developer should use the DefineBtnGroup function to define at least one button 
// group.  Also, SECToolBarCmdPage needs to be used in conjunction with a toolbar 
// manager.  
// @ex The following example might make this clearer: |
//  SECToolBarSheet sheet;
//  SECToolBarCmdPage page;
//	static UINT BASED_CODE fileButtons[4] =
//	{
//		ID_FILE_NEW,
//		ID_FILE_OPEN,
//		ID_FILE_SAVE,
//		ID_FILE_SAVE_ALL
//	};
//	static UINT BASED_CODE editButtons[3] =
//	{
//		ID_EDIT_COPY,
//		ID_EDIT_CUT,
//		ID_EDIT_PASTE
//	};
//	page.SetManager(m_pManager);
//	page.DefineBtnGroup(_T("File"), 4, &fileButtons);
//	page.DefineBtnGroup(_T("Edit"), 3, &editButtons);
//	sheet.AddPage(&page);
//
//sheet.DoModal();
//
//See the sample TOOLBAR in the \SAMPLES\TOOLKIT\STANDARD\DOCKING\TOOLBAR directory for a demonstration of this class.
//@xref <c SECToolBarSheet> <c SECToolBarManager> <c SECCustomToolbar> <c SECToolBarsDlg>
//@base public | CPropertyPage
class SECToolBarCmdPage : public CPropertyPage
{
	DECLARE_DYNCREATE(SECToolBarCmdPage)

// Construction
public:
	//@cmember
	/* Constructs an SECToolBarCmdPage object.*/
	SECToolBarCmdPage();
	//@cmember
	/* Constructs an SECToolBarCmdPage object.*/
	SECToolBarCmdPage(UINT nIDTemplate, UINT nIDCaption = 0);
	//@cmember
	/* Constructs an SECToolBarCmdPage object.*/
	SECToolBarCmdPage(LPCTSTR lpszTemplate, UINT nIDCaption = 0);

protected:
	void Initialize();

// Attributes
public:
	enum { IDD = IDD_TOOLBAR_CUSTOMIZE };

// Operations
public:
	//@cmember
	/* Defines the toolbar manager.*/
	void SetManager(SECToolBarManager* pManager);
	//@cmember
	/* Defines a button group, consisting of a title and an array of button IDs*/
	void DefineBtnGroup(LPCTSTR lpszTitle, int nBtnCount, UINT* lpBtnIDs);
	void DefineMenuGroup(LPCTSTR lpszTitle, int nBtnCount, UINT* lpBtnIDs);

	// Adds a menu item group (containing all menu items in the button map)
	void DefineMenuGroup(LPCTSTR lpszTitle);

	// Sets the button description based on id passed in
	void SetDescription(int nID);

// Overrideables
protected:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	virtual void OnReset();

// Implementation
public:
	~SECToolBarCmdPage();

protected:
	SECCustomToolBar* GetControlBar(UINT nID);

	const SECBtnMapEntry* GetMapEntryFromID(UINT nID,const SECBtnMapEntry* pRoot);

	// Implementation, data members.
	class CGroupArray : public CObArray
	{
	public:
		// Accessing elements
		SECBtnGroup* GetAt(int nIndex) const
			{ return (SECBtnGroup*)CObArray::GetAt(nIndex); }
		SECBtnGroup*& ElementAt(int nIndex)
			{ return (SECBtnGroup*&)CObArray::ElementAt(nIndex); }
		void SetAt(int nIndex, SECBtnGroup* ptr)
			{ CObArray::SetAt(nIndex, ptr); }

		// Potentially growing the array
		void SetAtGrow(int nIndex, SECBtnGroup* newElement)
		   { CObArray::SetAtGrow(nIndex, newElement); }
		int Add(SECBtnGroup* newElement)
		   { return CObArray::Add(newElement); }
		int Append(const CGroupArray& src)
		   { return CObArray::Append(src); }
		void Copy(const CGroupArray& src)
			{ CObArray::Copy(src); }

		// Operations that move elements around
		void InsertAt(int nIndex, SECBtnGroup* newElement, int nCount = 1)
			{ CObArray::InsertAt(nIndex, newElement, nCount); }
		void InsertAt(int nStartIndex, CGroupArray* pNewArray)
		   { CObArray::InsertAt(nStartIndex, pNewArray); }

		// overloaded operator helpers
		SECBtnGroup* operator[](int nIndex) const
			{ return (SECBtnGroup*)CObArray::operator[](nIndex); }
		SECBtnGroup*& operator[](int nIndex)
			{ return (SECBtnGroup*&)CObArray::operator[](nIndex); }
	} m_btnGroups;							// Array of btn groups

	SECToolBarManager* m_pManager;			// My toolbar manager
	int                m_nCurSel;			// The currently selected btn group
	UINT*              m_lpMenuIDs;			// Used to hold dynamic list of menu IDs

	// Implementation - message map and entries
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
	afx_msg void OnSelChange();
	afx_msg LONG OnBarNotify(UINT nID, LONG lParam);
	afx_msg LONG OnToolBarStyleChanging(UINT nEvent, LONG lParam);
};

/////////////////////////////////////////////////////////////////////////////
// SECCustomizeToolBar
//
/////////////////////////////
// AutoDuck tag block block for SECCustomizeToolBar 





// adinsert AutoDuck insertion point for SECCustomizeToolBar 
//@doc SECCustomizeToolBar 
//@class The SECCustomizeToolBar class manages the sizing of a user-defined, custom 
// toolbar having buttons geared toward a particular task.
//@comm
// See the sample TOOLBAR in the \SAMPLES\TOOLKIT\STANDARD\DOCKING\TOOLBAR directory for a demonstration 
// of this class.
//@xref <c SECCustomToolBar> <c SECToolBarSheet> <c SECToolBarCmdPage>
//@base public | SECCustomToolBar
class SECCustomizeToolBar : public SECCustomToolBar
{
// Construction
public:
	//@cmember
	/* Constructs an <c SECCustomizeToolBar> object.*/
	SECCustomizeToolBar();

	DECLARE_DYNAMIC(SECCustomizeToolBar);

// Operations
public:
	//@cmember
	/* Dynamically sizes the toolbar rectangle.*/
	virtual void DoSize(CRect& rect);

// Overrideables
protected:
	//@cmember
	/* Balances the wrapping of the toolbar.*/
	virtual void BalanceWrap(int nRow, Wrapped* pWrap);
	//@cmember
	/* Returns TRUE if the default drag mode is to add.*/
	virtual BOOL GetDragMode() const;
	//@cmember
	/* Returns TRUE if the toolbar accepts dropped buttons.*/
	virtual BOOL AcceptDrop() const;
};

// Function to convert Dialog Base Units into pixels.
extern void SECConvertDialogUnitsToPixels(HFONT hFont, int& cx, int& cy);

//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif // __TBARPAGE_H__

#endif // WIN32

