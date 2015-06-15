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
//  Author:             Justin Rudd
//  Description:        Declarations for SECShortcutListCtrl
//  Created:            September 1997


#ifdef WIN32

#ifndef __OLLC_H__
#define __OLLC_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#ifndef __AFXOLE_H__
#include <afxole.h>
#endif

#ifndef __SLSTCTL_H__
#include "toolkit\slstctl.h"
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

#define LVIS_EX_ICONUP				(0x0400)
#define LVIS_EX_ICONDOWN			(0x0800)

#define MOUSE_TIMER					(0xf0f0)

const UINT NM_LB_REORDERED 		= 1100;     // notification sent when items are 
                                       		// reordered
const UINT SEC_NM_ITEMCLICKED	= 1101;		// Notification of item clicked

// Structure utilized for extended information of SEC_NM_ITEMCLICKED notification
typedef struct SEC_SCNMHDR_tag {
	NMHDR nmhdr;
	int iSelItem;
} SEC_SCNMHDR;

/////////////////////////////////////////////////////////////////////////////
// SECShortcutListCtrl window

class SECShortcutListCtrl : public SECListCtrl
{
	DECLARE_DYNCREATE( SECShortcutListCtrl )

//Nested classes...
protected:
	enum
	{
		ScrollUp   = -10,
		ScrollDown = -11
	};

public:
	class ListDropTarget : public COleDropTarget
	{
	public:
		ListDropTarget();

		virtual DROPEFFECT OnDragEnter( CWnd* pWnd, COleDataObject* pData, DWORD dwKeyState, CPoint point );
		virtual DROPEFFECT OnDragOver( CWnd* pWnd, COleDataObject* pData, DWORD dwKeyState, CPoint point );
		virtual BOOL OnDrop( CWnd* pWnd, COleDataObject* pData, DROPEFFECT de, CPoint point );
		virtual void OnDragLeave( CWnd* pWnd );

		SECShortcutListCtrl* m_pList;

		enum
		{
			AboveTopItem    = -1,
			BelowBottomItem = -2
		};

	protected:
		int GetItemAbovePoint( const CPoint& pt );
		int GetItemBelowPoint( const CPoint& pt );

		CRect m_rcLastFilled;
		GUID  m_guidTemp;
	};

	friend class ListDropTarget;

protected:
	class ScrollButton
	{
	public:
		ScrollButton( BOOL bScrollUp );

		enum
		{
			Up,
			Down
		};

		void  SetRect( const CRect& rcBtn );
		CRect GetRect() const;

		void ModifyState( long lRemove, long lAdd );

		virtual void Draw( CDC* pDC );

	protected:
		CRect m_rcBtn;
		long  m_lState;
		BOOL  m_bScrollUp;

		friend class SECShortcutListCtrl;
	};

// Construction
public:
	SECShortcutListCtrl();

// Attributes
protected:
	int m_iLastHitItem;
	int m_iTimerID;
    int m_iDeltaIni;

protected:
	void StartTimer();
	void StopTimer();

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SECShortcutListCtrl)
	//}}AFX_VIRTUAL
	virtual int HitTest( CPoint pt, UINT* pFlags, int& iHitCol ) const;
	virtual BOOL Update( int nItem, BOOL bLabelOnly = FALSE, BOOL bEraseBkgnd = TRUE, BOOL bUpdateBelow = FALSE );
	virtual void DrawSeparator( CDC* pdc, const CRect& rcBounds, int iLoc );
	virtual int  GetMargin() const;
	virtual BOOL SetTextColor(COLORREF rgbText);
	virtual BOOL DeleteItem( int nItem, int cCount = 1 );
	virtual BOOL DeleteItem( Item* pItem );
	virtual BOOL DeleteAllItems();

protected:
	virtual void RecalcLayout();
	virtual void ClearMeasurements();

	virtual void DrawInvalidItems( LvPaintContext* pPC );
	virtual void DrawSubItemIcon( LvPaintContext* pPC );
	virtual void DrawSubItemText( LvPaintContext* pPC );
	virtual void DrawScrollButtons( LvPaintContext* pPC, LPSCROLLINFO pScrollInfo );

	virtual BOOL PrepareDataForTransfer( COleDataSource* pData, LV_ITEM* pLVI );
	virtual void ExtendContextMenu( CMenu& menu );

	virtual ListDropTarget* CreateDropTarget() const;
	virtual ScrollButton* CreateScrollUpButton() const;
	virtual ScrollButton* CreateScrollDownButton() const;

	virtual void DoScrollDown();
	virtual void DoScrollUp();

	virtual CPoint* GetLeftArrow( const CRect& rc, int iLoc );
	virtual CPoint* GetRightArrow( const CRect& rc, int iLoc );

	virtual void _Icon( int iItem, UINT nState );
	virtual UINT _Icon( int iItem );

	virtual void NotifySecSCNMHDR( UINT wNotify, LV_ITEM* pItem );

// Implementation
public:
	virtual ~SECShortcutListCtrl();

	// Generated message map functions
protected:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnKillFocus( CWnd* pNewWnd );
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnContextMenu( CWnd* pWnd, CPoint pos );
	afx_msg void OnListMenu_LargeIcon();
	afx_msg void OnListMenu_SmallIcon();
    afx_msg BOOL OnMouseWheel( UINT nFlags, short zDelta, CPoint pt );
	afx_msg void OnMButtonDown(UINT nFlags, CPoint pt);

	DECLARE_MESSAGE_MAP()

protected:
	ListDropTarget* m_pDropTarget;
	ScrollButton* m_pUpBtn;
	ScrollButton* m_pDownBtn;
	CPoint m_ptLastButtonDown;

private:
	//Since static members don't come across process boundaries to well...we'll make it private...
	static GUID s_dragGuid;
	static BOOL s_bGuidCreated;
	static UINT s_clipformat;
};

typedef struct tagLISTDATA
{
	tagLISTDATA();

	GUID    guid;
	LV_ITEM item;
} LISTDATA, *PLISTDATA;

/////////////////////////////////////////////////////////////////////////////

//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif // __OLLC_H__
#endif // WIN32
