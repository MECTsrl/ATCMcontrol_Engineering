// 
// Stingray Software Extension Classes
// Copyright (C) 1995 Stingray Software Inc.
// All Rights Reserved
// 
// This source code is only intended as a supplement to the
// Stingray Extension Class product.
// See the SEC help files for detailed information
// regarding using SEC classes.
// 
//  Author:       Dean Hallman
//  Description:  Declarations for SECTab and SECTabControlBase
//

#ifndef __TABCTRLB_H__
#define __TABCTRLB_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
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

// Default width/height of tab icon
#define SEC_TAB_DEFICON_CX	16
#define SEC_TAB_DEFICON_CY	16

/////////////////////////////////////////////////////////////////////////////
// SECTab

/////////////////////////////
// AutoDuck tag block block for SECTab 
//@doc SECTab
//@mdata CString  | SECTab | m_csLabel | This member is storage for the label which is drawn on this tab.  If you change
// this label by setting its value directly (rather than through the SECTabControl interface), you must cause a layout and 
// redraw to occur in order to see the effect of your modification.  To do this, call SECTabControl::RecalcLayout followed
// by SECTabControl::Invalidate.
//@xref<c SECTab>

//@doc SECTab
//@mdata int  | SECTab | m_nWidth | This member is storage for the width (in pixels) of this tab.  If you change this 
// member by setting its value directly (rather than through the SECTabControl interface), you must cause a layout and
// redraw to occur in order to see the effect of your modification.  To do this, call SECTabControl::RecalcLayout followed
// by SECTabControl::Invalidate.
//@xref<c SECTab>

//@doc SECTab
//@mdata int  | SECTab | m_nHeight | This member is storage for the height (in pixels) of this tab.  If you change this
// member by setting its value directly (rather than through the SECTabControl interface), you must cause a layout and
// redraw to occur in order to see the effect of your modification.  To do this, call SECTabControl::RecalcLayout followed
// by SECTabControl::Invalidate.
//@xref<c SECTab>

//@doc SECTab
//@mdata BOOL  | SECTab | m_bSelected | This member returns TRUE if tab is selected.
//@xref<c SECTab>

//@doc SECTab
//@mdata CObject*  | SECTab | m_pClient | This member maintains a pointer to the client of this tab.  Generally, this will 
// be a CWnd object which should be displayed when this tab is selected.  However, its tab is irrelevant to the SECTab and
// SECTabControl class, so you can associate any kind of client with tabs you like. 
//@xref<c SECTab>

//@doc SECTab
//@mdata HMENU  | SECTab | m_hMenu | This member is storage for an optional menu resource.  If you would like to associate
// a menu with a particular tab (that is, the menu is displayed when the tab is active), set this member accordingly.  Note,
// it is your responsibility to both set and use this menu value.  If you want to have a menu displayed when a tab is 
// selected, you will have to cause the menu change to occur (it is not done by default).
//@xref<c SECTab>

//@doc SECTab
//@mdata void*  | SECTab | m_pExtra | This member stores an opaque pointer to an optional user-defined buffer.  If you 
// would like to associate some user-defined data with each tab in a tab control, you can accomplish this by setting this
// member to point to that data.  Alternatively, you might prefer to derive your own class from SECTab.  The same can be 
// accomplished in this way, but you avoid the hassle of creating an extra structure if more than one extra data member is
// required.
//@xref<c SECTab>



// adinsert AutoDuck insertion point for SECTab 
//@doc SECTab 
//@class The SECTab class defines the attributes of one tab within a tab control.  
//@comm
// All information that 
// varies on a per tab basis (i.e., tab label, width, etc.) is stored here.  There are two occasions 
// when you might need to make use of this class.  First, if you would like to get direct access to the 
// tab attributes, you can call SECTabControl::<mf SECTabControl::GetTab> which returns a reference to an 
// object of this class. Once you have a reference to an SECTab object, you can retrieve or set values 
// and have the modifications be reflected with the next redraw.
//
// Knowledge of this class is also required when you need to derive from it.  For example, you may want 
// to derive your own tab classes from SECTab and add your domain-specific attributes there.  So, 
// when a new tab is activated, you have easy access to your own data members that
// are associated with the active tab.  To do this, you’ll need to override the 
// SECTabControl::<mf SECTabControl::CreateNewTab> member and create an instance of your own class 
// (which must be derived from SECTab).
//
// See the TABDEMO sample in the \SAMPLES\TOOLKIT\STANDARD\TABS\TABDEMO directory for a demonstration of this class.
//@xref<c SECTabControl>
//@BASE public|CObject
class SECTab : public CObject
{
    DECLARE_DYNAMIC(SECTab);
	
// Attributes
public:
	//@cmember
	/* The string which is rendered as the tab’s label.*/
    CString m_csLabel;

	//@cmember
	/* The width of this tab.*/
    int m_nWidth;

	//@cmember
	/* The height of this tab.*/
	int m_nHeight;

	//@cmember
	/* True if tab is selected.*/
    BOOL m_bSelected;

    // Object associated with this tab
	
	//@cmember
	/* The client associated with this tab.*/
	CObject* m_pClient; 

	//@cmember
	/* The menu associated with this tab.*/
    HMENU m_hMenu;

	// Optionally used to associate extra data
	//@cmember
	/* Any user-defined tab associated with this tab.*/
    void* m_pExtra;  

    SECTab();

// Implementation
protected:
    ~SECTab();

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif
};

/////////////////////////////////////////////////////////////////////////////
// SECTabControlBase window

// Tab Control Messages
#define TCM_TABSEL			(WM_USER+1000)
#define TCM_TABDBLCLK		(WM_USER+1001)
#define TCM_TABSELCLR		(WM_USER+1002)
#define TCM_TABREACTIVATE	(WM_USER+1003)

// TCM_TABSEL lParam info
typedef struct tagSECTABINFO {
	int nID;					// control id of the tab control or tab window
	int nIndexActiveTab;		// index of currently active tab
	int nIndexPrevTab;			// index of previously active tab, or -1 if none
	CObject* pActiveClient;		// pointer to client associated with the active tab
	CObject* pPrevClient;		// pointer to client associated with previous tab
} SECTABINFO;

// Tab Control Styles
#define TCS_TABS_ON_BOTTOM	0x0010
#define TCS_TABS_ON_TOP		0x0020
#define TCS_TABS_ON_LEFT	0x0040
#define TCS_TABS_ON_RIGHT	0x0080
#define TCS_TAB_ORIENTATIONS 0x00f0

/////////////////////////////
// AutoDuck tag block block for SECTabControlBase 


// adinsert AutoDuck insertion point for SECTabControlBase 
//@doc SECTabControlBase 
//@class The SECTabControlBase class defines the interface of a tab control.  
//@comm
// A tab control is a rectanglar window containing one row of tabs and (optionally) buttons which 
// allow the user to scroll the tabs left and right.  Tab controls are typically used in dialog boxes 
// or frame windows to provide the user with access to several pages of information within one 
// window.  By using a tab control, an application can define multiple pages for the same area of a
// window or dialog box.  Each page consists of a set of information or a group of controls that the 
// application displays when the user selects the corresponding tab.
//
// Tabbed windows, on the other hand, are derived from the classes <c SECTabWndBase>, <c SECTabWnd> 
// and <c SEC3DTabWnd>.  <c SECTabWndBase> contains a list of pages and a row of tabs.  The latter is
// implemented as a single instance of tab control of type <c SECTabControl>.  The single tab control 
// object manages and displays an array of tabs.  This tab control is sized and positioned by its 
// <c SECTabWndBase> parent such that no overlap exists between the pages and the tab control.  
// So, a <c SECTabWndBase> contains an instance of a <c SECTabControl> (and zero or more pages).  
// This organization allows the <c SECTabControl> to be used in a different context.  It may be 
// helpful to think of tab controls as child windows, whereas tabbed windows are self-sustaining 
// parent windows that interact with the document/view architecture.
//
// SECTabControlBase doesn’t actually implement the tab control UI functionality.  It
// is simply an abstract base class that defines the methods used to operate on a generic tab 
// control.  For example, SECTabControlBase defines methods for adding, removing, deleting, 
// invalidating tabs, etc. (it provides default implementations for these methods as well).  
// However, this class does not implement the look and feel of the tab control.  That responsibility 
// is delegated to derived classes.  The derived classes inherit the methods common to all
// tab controls and add the implementation details necessary to define the look and feel of 
// the particular tab control.
//
// There are currently two implementations of the SECTabControlBase interface: <c SECTabControl>
// and <c SEC3DTabControl>.  <c SECTabControl> implements a two-dimensional look and feel.  
// <c SEC3DTabControl> implements a three-dimensional look and feel with optional icon.  
// The two-dimensional tab control is more space-efficient, while the three-dimensional tab control 
// gives a more elegant look and simpler usage model.
// 
// Note, you cannot instantiate SECTabControlBase directly, as it is an abstract base class.  
// Instead, you must instance one of <c SECTabControl> or <c SEC3DTabControl>.

// The SECTabControlBase class contains and manages an array of tabs.  Most SECTabControlBase 
// operations take an integer index as their argument which specifies the tab the operation 
// should affect.  This index is equal to the position of the tab within the tab control, with 
// the leftmost tab specified by index 0 and the rightmost tab specified by index n (where n is 
// one less than the total number of tabs).  If a tab’s index is unknown, you can usually get it 
// using either the <mf SECTabControlBase::FindTab> or <mf SECTabControlBase::GetActiveTab> member 
// functions.  Each tab in this array is an 
// instance of <c SECTab> (or a derived class).  The <c SECTab> class defines the attributes common 
// to all tabs, regardless of their look and feel (i.e., the label, associated client, etc.).
// If you would like to associate domain-specific data with the tabs, you can derive a new tab 
// class from <c SECTab> and add  your properties there (see the member <SECTabControlBase::CreateNewTab>).
//
// Each tab in the tab control has an associated tab client.  A tab client is the object or data 
// associated with the tab.  The <c SECTab> object stores an opaque pointer to this data 
// since it is only meaningful to the object which created the tab control.  In the case of the 
// <c SECTabWnd> class (which creates an SECTabControlBase-derived object as a child control), 
// this client data is the CWnd which should be shown when the associated tab is active.  
// (Note: the SECTabWnd parent shows and hides the associated windows, not the SECTabControlBase).
//
// SECTabControlBase supports the selection of one or more tabs at a time.  Mouse events are
// handled to determine which tab the user intends to select.  After a change in tab selection
// occurs, the tab control sends a TCM_TABSEL message to the parent of the tab control.  
// Included with this message (as the wparam) is the index of the active tab.  In handling this 
// message, the parent will usually call <mf SECTabControlBase::GetTabInfo> to get the client
// data associated with the active tab.  When the user double-clicks the left mouse button on a tab, 
// the tab control sends a TCM_TABDBLCLK message to its parent along with the index of the 
// affected tab (specified by the wparam).
//
// See the TABDEMO sample in the \SAMPLES\TOOLKIT\STANDARD\TABS\TABDEMO directory for a demonstration 
// of SECTabControlBase.
//@xref<c SECTabControl> <c SEC3DTabControl> <c SECTab> <c SECTabWnd>
//@BASE public | CWnd
class SECTabControlBase : public CWnd
{
    DECLARE_DYNAMIC(SECTabControlBase);

// Construction
public:
	//@Access Public Members
	//@Access Constructors
	//@cmember
	/* Constructs an SECTabControlBase object.*/
    SECTabControlBase();
    ~SECTabControlBase();

// Overridables
	//@Access Overridable
	//@cmember
	/* Creates the tab control and attaches it to the SECTabControlBase object.*/
    virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

	//@cmember
	/* Select the visual attributes of the tab control such as the fonts and colors used to draw it.*/
    virtual void Initialize(DWORD dwStyle);

	//@cmember
	/* A "factory method" used to instantiate a new tab when AddTab or InsertTab is called.*/
	virtual SECTab* CreateNewTab() const;

	//@cmember
	/* Called when tab attributes or tab control size has changed to recompute any positions and sizes that*/
	// may have changed as a result.
	virtual void RecalcLayout();

	//@cmember
	/* Called when a tab is being activated.*/
	virtual void OnActivateTab(int nTab);

	//Temporarily, a place holder for SEC3DTabControl.
	virtual void DestroyGDIObjects()
				{ /*place holder for SEC3DTabControl for now*/ }

public:

// Operations
	//@Access Operations
	//@cmember
	/* Inserts a new tab with the given label before the currently active tab.*/
    virtual SECTab* InsertTab(int nIndex,
							  LPCTSTR lpszLabel = NULL,
							  CObject *pClient = NULL,
							  HMENU hMenu = 0,
							  void* pExtra = NULL);

	//@cmember
	/* Appends a new tab to the end of the existing array of tabs.*/
    virtual SECTab* AddTab(LPCTSTR lpszLabel = NULL,
						   CObject* pClient = NULL,
						   HMENU hMenu = 0,
						   void* pExtra = NULL);

	//@cmember
	/* Specify an icon to add to a given tab.*/
	virtual void SetTabIcon(int nIndex, HICON hIcon);

	//@cmember
	/* Specify an icon to add to a given tab.*/
	virtual void SetTabIcon(int nIndex, UINT nIDIcon, 
							int cx=SEC_TAB_DEFICON_CX, int cy=SEC_TAB_DEFICON_CY);

	//@cmember
	/* Specify an icon to add to a given tab.*/
	virtual void SetTabIcon(int nIndex, LPCTSTR lpszResourceName,
							int cx=SEC_TAB_DEFICON_CX, int cy=SEC_TAB_DEFICON_CY);

	//@cmember
	/* Removes the tab at the specifed index.*/
    virtual void DeleteTab(int nTab);

	//@cmember
	/* Gives a new name to the tab at the specified index.*/
    virtual void RenameTab(int nTab, LPCTSTR lpszLabel);
     
	// The tab appearing on top of all others
	//@cmember
	/* Causes the specified tab to become the active tab.*/
    virtual void ActivateTab(int nTab); 

	virtual void ReActivateTab();

	//@cmember
	/* Selects the tab with the given index.*/
    virtual void SelectTab(int nTab);

	//@cmember
	/* All tabs are marked as unselected.*/
    virtual void ClearSelection();

	//@cmember
	/* The specified tab is scrolled into view.*/
    virtual void ScrollToTab(int nTab);

	//@cmember
	/* Trigger a redraw of the specified tab.*/
	virtual void InvalidateTab(int nTab);

// Queries
	//@Access Queries
	//@cmember
	/* Returns the number of tabs.*/
    int GetTabCount() const;

	//@cmember
	/* Returns information about the tab with the supplied index.*/
    BOOL GetTabInfo(int nTab, LPCTSTR& lpszLabel, BOOL& bSelected,
    			CObject*& pClient, HMENU& hMenu, void*& pExtra) const;

	//@cmember
	/* Returns the index of the tab with the supplied client data.*/
    BOOL FindTab(const CObject* const pClient, int& nTab) const;

	//@cmember
	/* If any tab is active, its index is returned.*/
    BOOL GetActiveTab(int& nTab) const;

	//@cmember
	/* Checks to see if the particular tab exists.*/
    BOOL TabExists(CObject* pClient) const;

	//@cmember
	/* Checks to see if the tab corresponding to the particular index exists.*/
    BOOL TabExists(int nTab) const;

// Implementation
protected:
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    CObArray *m_pTabArray;
    int m_nActiveTab;
    BOOL m_bActiveTab;
    HCURSOR m_hcurArrow;

    //@Access Protected Members
	//@Access Queries
	//@cmember
	/* Returns a reference to the specified tab object.*/
    SECTab& GetTab(int nTab) const;

	//@cmember
	/* Returns a pointer to the specified tab object.*/
	SECTab* GetTabPtr(int nTab) const;
    
    // Generated message map functions
protected:
    //{{AFX_MSG(SECTabControlBase)
    afx_msg void OnDestroy();
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP();
};

//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif // __TABCTRLB_H__
/////////////////////////////////////////////////////////////////////////////

