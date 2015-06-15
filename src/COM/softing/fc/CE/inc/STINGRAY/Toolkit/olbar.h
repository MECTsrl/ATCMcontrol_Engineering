/////////////////////////////////////////////////////////////////////////////
// olbar.h : Declaration of SECShortcutBar
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
// Author:       Justin Rudd
// Description:  Declaration of SECShortcutBar
//

// adinsert

#ifdef WIN32

#ifndef __SECOUTLOOKBAR_H__
#define __SECOUTLOOKBAR_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#ifndef __AFXTEMPL_H__
#include <afxtempl.h>
#endif

#ifndef __SECITER_H__
#include "toolkit\seciter.h"
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

// Constants
#define SEC_DEFAULT_ID					(0xffffffff)
#define SEC_IDW_BARCLIENT_FIRST			(0xEB00)
#define SEC_IDW_BARCLIENT_LAST			(0xEBFF)
#define SEC_WS_DEFAULT_BARCLIENT		(WS_CHILD|WS_VISIBLE)
#define SEC_DEFAULT_ANIMATION_SPEED		(7)
#define SEC_DEFAULT_ANIMATION_STEP 		(8)

//SECShortcutBar Styles
#define SEC_OBS_VERT					(0x0001)
#define SEC_OBS_HORZ					(0x0002)
#define SEC_OBS_BUTTONFEEDBACK			(0x0004)
#define SEC_OBS_CONTEXTMENU				(0x0008)
#define SEC_OBS_ANIMATESCROLL			(0x0010)
#define SEC_OBS_BUTTONFOCUS				(0x0020)

//composite alignment mask
#define SEC_OBS_ALIGN					(SEC_OBS_VERT				|\
										 SEC_OBS_HORZ)
//composite styles mask
#define SEC_OBS_STYLES					(SEC_OBS_ALIGN				|\
										 SEC_OBS_BUTTONFEEDBACK		|\
                                         SEC_OBS_CONTEXTMENU		|\
										 SEC_OBS_ANIMATESCROLL		|\
										 SEC_OBS_BUTTONFOCUS)


#define SEC_DEFAULT_OUTLOOKBAR			(WS_VISIBLE|WS_CHILD|SEC_OBS_VERT)

#ifdef _DEBUG

	#define ASSERT_BarExists(index) \
		if( BarExists(index) == FALSE ) \
		{\
			TRACE(_T("ERROR - Trying to access bar( %d ) which does not exist\n"),index); \
			ASSERT(FALSE); \
		}

#else

	#define ASSERT_BarExists(index)

#endif

// Forward Declarations
class SECBar;
class SECListBar;

// Types
typedef CArray<SECBar*,SECBar*> SECBarArray;

/////////////////////////////////////////////////////////////////////////////
// SECShortcutBar window

//@doc SECShortcutBar 
//@class SECShortcutBar offers the look and feel of the Microsoft Outlook Bar. 
//@comm 
// The shortcut bar acts as a container class and can hold any embedded CWnd-derived objects. 
// It can also be embedded anywhere.
//
// When the shortcut bar is usedas an Outlook Bar type interface it maintains a list of bars. 
// Each bar is the button that can be seen. 
//
// See the DEMO sample in the \SAMPLES\TOOLKIT\STANDARD\TABS\LISTBAR directory for a demonstration 
// of the SECShortcutBar class.
//@xref<c SECBar> <c SECListBar>
//@BASE public | CWnd
class SECShortcutBar : public CWnd
{
	DECLARE_DYNCREATE(SECShortcutBar)

	//---------------------------------------------------------------------//
	// 						   Public Interface							   //
	//---------------------------------------------------------------------//
public:
	//Construction/Destruction
	//@Access Public Members
	//@Access Constructors/Destructors
	//@cmember
	/* Constructs the SECShortcutBar object*/
	SECShortcutBar();
	//@cmember
	/* Destructs the SECShortcutBar object*/
	virtual ~SECShortcutBar();

	//@Access Initializations
	//@cmember
	/* Creates the SECShortcutBar window*/
	virtual BOOL Create( CWnd* pParentWnd, DWORD dwStyle = SEC_DEFAULT_OUTLOOKBAR, UINT nID = AFX_IDW_PANE_FIRST );

	// Accessor functions...Get/Set
	//@Access Accessor Fuctions
	//@cmember
	/* Set the CRuntimeClass used for SECBar's*/
	void SetBarClass( CRuntimeClass* const pBarClass );
	//@cmember
	/* Get the CRuntimeClass used for SECBar's*/
	CRuntimeClass* GetBarClass() const;

	//@cmember
	/* Set the CRuntimeClass used for SECListBar's*/
	void SetListBarClass( CRuntimeClass* const pBarClass );
	//@cmember
	/* Get the CRuntimeClass used for SECBar's*/
	CRuntimeClass* GetListBarClass() const;

	//@cmember
	/* Set the CRuntimeClass used for SECShortcutListCtrl's*/
	void SetListCtrlClass( CRuntimeClass* const pBarClass );
	//@cmember
	/* Get the CRuntimeClass used for SECShortcutListCtrl's*/
	CRuntimeClass* GetListCtrlClass() const;

	void SetMinPaneSize( const int& minSize );
	int GetMinPaneSize() const;

	//@cmember
	/* Set the font point size to use for SECBar*/
	void SetFontPointSize( const int& iFontPointSize );
	//@cmember
	/* Get the font point size to use for SECBar*/
	int GetFontPointSize() const;

	//@cmember
	/* Set the font name to use for SECBar*/
	void SetFontName( const CString& sFontName );
	//@cmember
	/* Get the font name to use for SECBar*/
	const CString& GetFontName() const;

	//@cmember
	/* Set the Animation speed to use (in ms)*/
	void SetAnimationSpeed( const int& iAnimationSpeed );
	//@cmember
	/* Get the Animation speed to use (in ms)*/
	int GetAnimationSpeed() const;

	//@cmember
	/* Set the Animation Step (# of frames in animation)*/
	void SetAnimationStep( const int& iAnimationStep );
	//@cmember
	/* Get the Animation Step (# of frames in animation)*/
	int GetAnimationStep() const;

	//@cmember
	/* Set menu for SECBar to use*/
	void SetBarMenu( HMENU hMenu, int iIndex = -1 );
	//@cmember
	/* Set menu for SECBar to use*/
	void SetBarMenu( CMenu* pSubMenu, int iIndex = -1, int iLevel = -1 );

	//@cmember
	/* Set font for SECBar to use*/
	void SetBarFont( CFont* pFont, int iIndex = -1 );
	//@cmember
	/* Set font for SECBar to use*/
	void SetBarFont( HFONT hFont, int iIndex = -1 );

	//@cmember
	/* Set color to use as filler in animated scrolls*/
	void SetBackFillColor( COLORREF color );
	//@cmember
	/* Set brush to use as filler in animated scrolls*/
	void SetBackFillColor( CBrush* pBackFillBrush );

	//@cmember
	/* Set color of the focus rect for SECBar's*/
	void SetFocusRectColor( COLORREF color, int iIndex = -1 );
	//@cmember
	/* Get color of the focus rect for SECBar's*/
	COLORREF GetFocusRectColor( int iIndex = -1 ) const;

	//@cmember
	/* Set color of the bar text for SECBar's*/
	void SetTextColor( COLORREF color, int iIndex = -1 );
	//@cmember
	/* Get color of the bar text for SECBar's*/
	COLORREF GetTextColor(int iIndex = -1) const;

	//@cmember
	/* Set background color of the bar*/
	void SetBkColor( COLORREF color, int iIndex = -1 );
	//@cmember
	/* Get background color of the bar*/
	COLORREF GetBkColor(int iIndex = -1) const;

	//@cmember
	/* Set background color of the bar*/
	void SetPaneBkColor( COLORREF color, int iIndex = -1 );
	//@cmember
	/* Get background color of the bar*/
	COLORREF GetPaneBkColor(int iIndex = -1) const;

	//Operations...
	//@Access Queries
	//@cmember
	/* Queries the number of bars in SECShortcutBar*/
    int GetBarCount() const;

	//@cmember
	/* Returns the CWnd associated with the Bar*/
	CWnd* GetBarWnd( int iIndex ) const;

	//@cmember
	/* Sets the alignment style*/
	void SetAlignStyle( DWORD dwAlign );
	//@cmember
	/* Gets the alignment style*/
	DWORD GetAlignStyle() const;

	//@cmember
	/* Modifies the bar style*/
	void ModifyBarStyle( DWORD dwRemove, DWORD dwAdd, BOOL bRecalcRedraw = TRUE );
	//@cmember
	/* Queries the bar style*/
	DWORD GetBarStyle() const;

	//@cmember
	/* Get SECBar associated with active bar*/
	SECBar& GetActiveBar() const;
	//@cmember
	/* Queries wether or not we have an active bar*/
	BOOL    HasActiveBar() const;
	//@cmember
	/* Returns the index of the active bar*/
	int		GetActiveIndex() const;

	//@cmember
	/* Queries for the style SEC_OBS_VERT*/
	BOOL IsVertAlign() const;
	//@cmember
	/* Queries for the style SEC_OBS_HORZ*/
	BOOL IsHorzAlign() const;
	//@cmember
	/* Queries for a particular style*/
	BOOL IsStyleSet( DWORD dwStyle ) const;

	//@cmember
	/* Creates an iterator to use for traversal*/
	SECIterator<SECBar*>* CreateBarIterator() const;

	//@cmember
	/* Returns a SECBar at specified index*/
	SECBar&	GetBar( int iIndex ) const;
	//@cmember
	/* Returns a SECBar* at specified index*/
	SECBar* GetBarPtr( int iIndex ) const;
	//@cmember
	/* Returns index of the bar at specified point.*/
	int HitBar( const CPoint& pt );

	//---------------------------------------------------------------------//
	// 						   Public Overrideables						   //
	//---------------------------------------------------------------------//
public:
	//@Access Public Overrideables
	//@cmember
	/* Adds a new bar at the end*/
    virtual SECBar* AddBar(CWnd* pWnd,LPCTSTR lpszLabel,BOOL bRecalc=FALSE);
	//@cmember
	/* Adds a new bar at the end*/
	virtual SECBar* AddBar(CRuntimeClass* pViewClass,LPCTSTR lpszLabel,
						   CCreateContext* pContext = NULL,BOOL bRecalc = FALSE, UINT nID = SEC_DEFAULT_ID);
	//@cmember
	/* Adds a new SECListBar at the end*/
	virtual SECListBar* AddBar( LPCTSTR lpszLabel, BOOL bRecalc=FALSE);

	//@cmember
	/* Inserts a new bar*/
	virtual SECBar* InsertBar( int iIndex, CWnd* pWnd, LPCTSTR lpszLabel, BOOL bRecalc=FALSE);
	//@cmember
	/* Inserts a new bar*/
	virtual SECBar* InsertBar( int iIndex, CRuntimeClass* pViewClass, LPCTSTR lpszLabel, 
		                       CCreateContext* pContext = NULL, BOOL bRecalc = FALSE, UINT uID = SEC_DEFAULT_ID );
	//@cmember
	/* Inserts a new SECListBar*/
	virtual SECListBar* InsertBar( int iIndex, LPCTSTR lpszLabel, BOOL bRecalc=FALSE);

	//@cmember
	/* Removes a bar at specified index*/
	virtual void RemoveBar( int iIndex );

	//@cmember
	/* Renames a bar at specified index*/
	virtual void RenameBar( int iIndex, LPCTSTR lpszLabel );

	//@cmember
	/* Activates bar at specified index*/
    virtual void ActivateBar(int nIndex);
	
	//legacy API.  Will be removed at the end of the beta cycle
	void SelectPane( int i ){ ActivateBar(i); }

	virtual BOOL RecalcLayout();

	//@cmember
	/* Disables bar at specified index*/
	virtual void DisableBar( int iIndex );
	//@cmember
	/* Enables bar at specified index*/
	virtual void EnableBar( int iIndex );

	//---------------------------------------------------------------------//
	// 						   Protected Overrideables					   //
	//---------------------------------------------------------------------//
protected:
	//@Access Protected Overrideables
	//@cmember
	/* Called when styles changing*/
	virtual void OnStyleChange( DWORD dwRemovedStyles, DWORD dwAddedStyles );
	//@cmember
	/* Called when trying to change bar*/
	virtual BOOL OnChangeBar( int iIndex );
	//@cmember
	/* Called when trying to remove a bar*/
	virtual BOOL OnRemoveBar( int iIndex );
	//@cmember
	/* Called when trying to disable a bar*/
	virtual BOOL OnDisableBar( int iIndex );
	//@cmember
	/* Called when trying to enable a bar*/
	virtual BOOL OnEnableBar( int iIndex );
	//@cmember
	/* Called after creating CWnd for bar object*/
	virtual BOOL OnCreatePaneWnd( CWnd* pwnd );
	//@cmember
	/* Called after creating SECBar object*/
	virtual BOOL OnCreateBar( SECBar* pbar );
		
	//@cmember
	/* Called to create GDI Objects*/
	virtual void ConstructGDIObjects();
	//@cmember
	/* Called to destroy GDI Objects*/
	virtual void DestructGDIObjects();

	virtual void CalcFixedLayout( int iNewSize, int iLastSize );

	virtual void SetupRects( BOOL bVert, CRect& rc );
	virtual void SetPanePositions();

	virtual void InvalidateBar( int iBar );
	virtual void InvalidateBar( SECBar& bar );
	virtual void InvalidateBar( SECBar* pBar );

	//@cmember
	/* Called to create SECBar object*/
	virtual SECBar* CreateNewBar() const;
	//@cmember
	/* Called to create SECListBar objects*/
	virtual SECListBar* CreateNewListBar() const;

	virtual SECBar* DoAddBar( CWnd* pWnd, LPCTSTR lpszLabel, BOOL bRecalc );
	virtual SECBar* DoInsertBar( int iIndex, CWnd* pWnd, LPCTSTR lpszLabel, BOOL bRecalc );
	virtual void    DoPaint( CDC& dc );
	
	virtual void NoAnimateScroll( int iIndex );
	virtual void AnimateScroll( int iOldIndex, int iNewIndex );

	virtual void FindPrevActiveBar( int& riIndex );
	virtual void FindNextActiveBar( int& riIndex );

	//---------------------------------------------------------------------//
	// 						Protected Implementation					   //
	//---------------------------------------------------------------------//
protected:
	// Implementation Member Variables
	int				m_iHitIndex;
	int				m_iActiveIndex;
	int				m_iMinPaneSize;
	int				m_iLastTotalBtnSize;
	int				m_iLastTotalPaneSize;
	int				m_iFontPointSize;
	int				m_iAnimationSpeed;
	int				m_iAnimationStep;
	BOOL			m_bActiveIndex;
	BOOL			m_bBtnDown;
	DWORD			m_dwAlign;
	DWORD			m_dwStyle;
	CRuntimeClass*	m_pBarClass;
	CRuntimeClass*  m_pListBarClass;
	CRuntimeClass*  m_pListCtrlClass;
	CSize			m_lastWndSize;
	CString			m_sFontName;
	CBrush*			m_pbrBackBrush;
	SECBarArray		m_bars;
	CFont			m_defaultVertFont;
	CFont			m_defaultHorzFont;
	COLORREF    m_clrDefaultText;
	COLORREF    m_clrDefaultBk;
	COLORREF	m_clrDefaultPaneBk;
	
	// Implementation Member Functions
	CWnd* CreateWnd( CRuntimeClass* pWndClass, CCreateContext* pContext = NULL, UINT nID = SEC_DEFAULT_ID );
    BOOL BarExists(int nBar) const;
	void CleanUpBackFillBrush();

	//---------------------------------------------------------------------//
	// 						  Base Class Overrides						   //
	//---------------------------------------------------------------------//

public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL PreTranslateMessage(MSG* pMsg);


	//---------------------------------------------------------------------//
	// 						Message Map Declarations					   //
	//---------------------------------------------------------------------//
protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()

		
	//---------------------------------------------------------------------//
	// 						Private Implementation						   //
	//---------------------------------------------------------------------//
private:
	static CString s_strClassName;
};

//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif //__SECOUTLOOKBAR_H__

#endif // WIN32

