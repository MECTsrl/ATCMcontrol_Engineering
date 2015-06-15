#if !defined(AFX_ELEMEDITORVIEW_H__EA412B23_8DB4_11D2_A449_0000C0D26FBC__INCLUDED_)
#define AFX_ELEMEDITORVIEW_H__EA412B23_8DB4_11D2_A449_0000C0D26FBC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StdAfx.h"
#include "Elem.h"
#include "ElemPtrList.h"
#include "ElemFB.h"
#include "ElemSfc.h"
#include "ElemLd.h"
#include "PropEdit.h"
#include "GrEditorDoc.h"
#include "STEditorView.h"

typedef enum {          // state of cursor for SetCursor
   CT_INIT,             //  at start, for saving 
   CT_NORMAL,           //  over empty area
   CT_LINE,             //  line draw mode
   CT_LINE_START,       //  ready to draw line
   CT_ELEM,             //  over elements
   CT_SIZESEL,          //  select resize corner 
   CT_SIZENESW,         //  selected element resize corner NE-SW
   CT_SIZENS,           //  selected element resize corner  N-S
   CT_SIZENWSE,         //  selected element resize corner NW-SE
   CT_SIZEWE,           //  selected element resize corner  W-E
   CT_INSCOL,           //  insert col
   CT_INSCOL1,          //  insert col disabled
   CT_DELCOL,           //  delete col
   CT_DELCOL1,          //  delete col disabled
   CT_INSROW,           //  insert row
   CT_INSROW1,          //  insert row disabled
   CT_DELROW,           //  delete row
   CT_DELROW1,          //  delete row disabled

   CT_LAST
} CURSOR_TYPE;



/////////////////////////////////////////////////////////////////////////////
// CElemEditorView view data structures

typedef CMap<UINT,UINT, CSTEditorView*,CSTEditorView*> CSTViewMap;


typedef enum
{
   AT_FBD,
   AT_LD,
   AT_ST
}ActionType;


/////////////////////////////////////////////////////////////////////////////
// CElemEditorView view


class CElemEditorView : public CScrollView
{
protected:
   CElemEditorView ();          // protected constructor used by dynamic creation
   void AttachSTView(CSTEditorView *&pView, CElemAct *pAct);
   void DetachSTView(CElemAct *pAct);
   CSTEditorView* CreateSTView(CElemAct* pAct);
   void CreateSTViews();
   void DeleteSTViews();

   DECLARE_DYNCREATE (CElemEditorView)

// Attributes
public:
   // utils
   int         m_iZGrid;         // zoomed grid (m_fZoom * m_iGrid from document)
   int         m_iDZGrid;        // double of m_iZGrid;
   int         m_iHZGrid;        //   half of m_iZGrid;
   CPoint      m_LastScrollPos;

   // edit ..
   CPropEdit  *m_pPropEdit;
   CSTEditorView *m_pSEC_Edit;   // active instance of CSTEditorView
   
   CFont       m_PropFont;
   CFont       m_FixedFont;
	bool        m_bFontCreated;

   CText      *m_pPropText;
   CElem      *m_pPropElem;
   CElem      *m_pAutoElem;
   bool        m_bUseAutoSize;
   bool        m_bWarnOnSignalChange;

protected:
   CElemLine  *m_EditLine;
   CElemPtrList *m_Polygon;      // auto-routed line
   BOOL        m_bIsCaptured;
   TElemFlags  m_NextActionType;
   CSTViewMap  m_STViewMap;

   CSize       GetInsertSize(UINT RegKey);
   void        SetInsertSize(UINT RegKey, CSize stdSize);
public:
   CPoint      m_BeforeOpPos;
   CPoint      m_EndPos;
   CPoint      m_StartPos;
   CElem      *m_CurElem;       // element under actual cursor position
   CPoint      m_CurPos;        // actual pos in raster coords 
   CPoint      m_LogCurPos;     // actual pos in logical coords (for mouse move) 
   CPoint      m_OldCurPos;     // old pos in logical coords (for mouse move/resize) 
   bool        m_bFirstMove;
   bool        m_bEditNext;     // TempVar renaming cycle

   CURSOR_TYPE m_eCursorMode;
   BOOL        m_bUpdateMousePos; // if window has lost focus and mouse moved ..
   RS_TYPE     m_eResizeMode;
   bool        m_bIgnoreForFBCall;
   bool        m_NoAutoScroll;   // for auto scrolling
   
// Operations
public:
	void PropPower(CElemPower* pE);
   // utils
   CGrEditorDoc* GetDocument ();
   
   virtual void EventDown   (void);
   virtual void EventUp     (void);
   virtual void EventCancel (void);
   virtual void EventEnter  (void);
   
   void   CheckMode (void);
   CSize  GetCmtSize (CString str);
   void   GrReleaseCapture();
   void   GrSetCapture();
   
   // convert, check, ..
   void   DocToClient  (CRect& r);
   CPoint DocToClient  (CPoint p);
   void   DocToGrid    (CRect& r);
   CPoint DocToGrid    (CPoint p);
   
   void   ClientToGrid (CRect& r);
   CPoint ClientToGrid (CPoint p);
   void   ClientToDoc  (CRect& r);
   CPoint ClientToDoc  (CPoint p);
   
   void   GridToClient (CRect& r);
   CPoint GridToClient (CPoint p);
   void   GridToDoc    (CRect& r);
   CPoint GridToDoc    (CPoint p);
   
   void   CheckRastPoint (CPoint& p);
   void   CheckRastRect  (CRect& r);
   
   // edit ..
   bool   OnModifyContent ();
   bool   CheckReadonly();

   void   CreateFont    (void);
   void   ZoomFont      (void);
   void   DeleteFont    (void);

   void   EditLineStart (CPoint FromPos);
   bool   EditLineProc  (void);
   void   EditLineStop  (void);
   
   void   EditNameStart (CText& text, CRect& rect, CElem *pE, bool bEditName=true);
   void   EditNameStop  (bool bAbort);
   
   void   InsertAction (BOOL bBlock, CString strName = _T(""));
   void   InsertCmt    (void);
   void   InsertCon    (CON_TYPE eConType);
   void   InsertDiv    (BOOL bAltDiv);
   void   InsertLabel  (LAB_TYPE eLabType);
   void   InsertVar    (VAR_TYPE eVarType);
   void   InsertRet    (void);
   void   InsertStep   (void);
   void   InsertTran   (void);
   void   InsertPower  (bool bLeft);
   void   InsertContact(LADDER_TYPE eLdType);
   void   InsertCoil   (LADDER_TYPE eLdType);
   CElemLine *InsertLine   (bool bHLine, bool bDirBR); // horizontal line?, to the [right/bottom]?
   
   void   SetActionType(CElemAct* pE, ActionType type);

   void   PropCon   (CElemCon    *pE);
   void   PropLab   (CElemLab    *pE);
   void   PropCmt   (CElemCmt    *pE);
   void   PropAct   (CElemAct    *pE);
   void   PropActBlk(CElemActBlk *pE);
   void   PropVar   (CElemVar    *pE);
   void   PropStep  (CElemStep   *pE);
   void   PropTran  (CElemTran   *pE);
   void   PropDiv   (CElemDiv    *pE);
   void   PropContact(CElemContact*pE);
   void   PropCoil  (CElemCoil   *pE);
   void   PropLine  (CElemLine   *pE);
   void   PropFblk  (CElemFblk   *pE);
   
   void   CheckPropEnd (CElem *pE);
   
   // draw ..
   void   MoveRel (int iDx, int iDy);
   void   MovePos (int iX, int iY);
   void   ScrollRel (int iDx, int iDy, BOOL bEnableMousePos=TRUE);
   void   MoveViewToCursor (BOOL bEnableMousePos=FALSE);
   void   CenterViewToCursor (CPoint p);
   void   SetCursor(HCURSOR hCursor);
   void   UpdateCursor (bool bEnableMousePos=true);
   void   ReSyncCursorPos();
   void   UpdateViewSizePosFont(CElemAct* pAct, CSTEditorView* pView);
   void   UpdateSTActionView(CElemAct* pAct);
   void   OnDeleteSTActions(bool bOnlySelected=true);
   void   SyncSTActionText(CElemPtrList* pList);
//   void   ResizeSTView(CElemAct* pAct, CSTEditorView* pView, int dx, int dy, int sx, int sy);
//   void   ZoomAllSTViews();
   
   void   UnselectAll(void);

   void   UpdateStatus (void);
   void   CheckMousePos (CPoint point);
   
   void   SetCurPos (CPoint newCurPos);
   void   SetZoomedGrid(int iZGrid);
   
   void   Message (PSTR strMsg);      // own helper function for statusline

   // Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CElemEditorView)
public:
   virtual void OnDraw(CDC* pDC);  // overridden to draw this view
protected:
   virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
   virtual BOOL PreTranslateMessage(MSG* pMsg);
   //}}AFX_VIRTUAL
   
   // Implementation
protected:
   virtual ~CElemEditorView();
#ifdef _DEBUG
   virtual void AssertValid() const;
   virtual void Dump(CDumpContext& dc) const;
#endif
   
   // Generated message map functions
   //{{AFX_MSG(CElemEditorView)
   afx_msg int  OnMouseActivate(CWnd *pDesktopWnd, UINT nHitTest, UINT message);
   //}}AFX_MSG
   afx_msg void OnSECEditCtrlChar(WPARAM, LPARAM);
   
   DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in GrEditorView.cpp
inline CGrEditorDoc* CElemEditorView::GetDocument()
   { return (CGrEditorDoc*)m_pDocument; }
#endif
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


#endif // !defined(AFX_ELEMEDITORVIEW_H__EA412B23_8DB4_11D2_A449_0000C0D26FBC__INCLUDED_)
