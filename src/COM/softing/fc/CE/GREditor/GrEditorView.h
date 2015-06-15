#if !defined(AFX_GREDITORVIEW_H__59119A23_6422_11D2_9A3D_0000C0D26FBC__INCLUDED_)
#define AFX_GREDITORVIEW_H__59119A23_6422_11D2_9A3D_0000C0D26FBC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StdEdAttr.h"
#include "DbgEditorView.h"
#include "GrEditorDoc.h"

#include "BookMark.h"

//#import "HMISim.tlb" no_namespace, named_guids

class CWatchpoint;
//struct BookMark;

class CGrEditorView : public CDbgEditorView
{
protected: // create from serialization only
	CGrEditorView();
	DECLARE_DYNCREATE(CGrEditorView)

   typedef enum
   {
     zoom_to_default,
     zoom_out_small,
     zoom_in_small,
     zoom_in,
     zoom_out,
   }ZoomOp;

// Attributes
public:
	// drawing ..
   BOOL           m_bActive;
   bool           m_bTick;          // timer has 'ticked' for OnMouseMove outside client
   int            m_bTickCount;     // number of continuous tick counts for progression
   int            m_iTickDelta;     // number of increments
   // find ..
	CFindReplaceDialog* m_pFindReplaceDlg; // find or replace dialog
   SearchDat      m_SearchDat;

   // drag and drop functionality
   COleDropTarget m_dropTarget;     // for drop target functionality
   CPoint         m_dragPoint;      // current position
   bool           m_dragFirst;      // flag for first step
   bool           m_dragActive;     // flag for active dragging (auto scroll - focus rects !!!)
   CSize          m_dragSize;       // size of dragged object
   CSize          m_dragOffset;     // offset between pt and drag object corner
   CPoint         m_prevDragPoint;  // last position
   BOOL           m_bDragDataAcceptable;
   bool           m_bDropSource;
   
   //hmi simulator
   //IQtEditorPtr ptr;

protected:
	UINT           m_uTimerID;       // only for debug
	BOOL           m_bDebug;
   // drawing ..
	bool           m_bDrawOnScreen;
   CPoint         m_LastPos;        // for getposition
   // watch ..
	CWatchpoint   *m_pTrackedWP;
	CPoint         m_WPGrabPosition;

   CElemList      m_ClipElem;
   bool           m_bLocalSpacing;
   typedef CArray<BookMark,BookMark> BookMarkTab;
   BookMarkTab    m_BookMarkTab;
   int            m_BookMarkPos;
   float          m_fOldZoom;

   CList<CString, CString&> m_listLruPous;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGrEditorView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual BOOL DestroyWindow();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll = TRUE);
	//}}AFX_VIRTUAL

// Implementation

public:
	virtual ~CGrEditorView();

	        void  NextSearchPos ();
           void  ExtendMenuBySFCItems (CMenu *pMenu);
           DWORD GetSFCMenuStyle (UINT uiId);
           void  ResyncScrollSizes ();       // ensure scroll info is up-to-date
	        void  SetPageSize ();
           void  DrawMarks (CDC *pDC, CGrEditorDoc *pDoc, CRect drawArea);
	        void  DrawGrid (CDC *pDC, CGrEditorDoc *pDoc);
	        void  DrawList (CDC *pDC, CGrEditorDoc *pDoc, CElemList *pElemList);

           void  DrawBookMarks (CDC *pDC, CGrEditorDoc *pDoc);
	        void  DrawFixPoints (CDC *pDC, CGrEditorDoc *pDoc);

           void  DrawSelections (CDC *pDC, CGrEditorDoc *pDoc, CElemList *pElemList, CRect drawArea);
	        void  DrawPage (CDC *pDC, CGrEditorDoc *pDoc);
           void  DrawInto (CDC *pDC, CSize drawAreaSize);

   virtual void  EventDown();
	virtual void  EventUp();
	virtual void  EventCancel();
	virtual void  EventEnter();

           void  DeleteElem();
	        void  InvertPin();

	        void  OnKeyMove(UINT nChar);
           void  OnEditFindReplace(BOOL bFindOnly);
	        void  OnFindNext(LPCTSTR lpszFind, BOOL bNext, BOOL bCase);
           void  OnTextNotFound(LPCTSTR);
           void  OnReplaceSel(LPCTSTR lpszFind, BOOL bNext, BOOL bCase, LPCTSTR lpszReplace);
           void  OnReplaceAll(LPCTSTR lpszFind, LPCTSTR lpszReplace, BOOL bCase);
           bool  FindText(bool bUpdateScreen=true, bool bWrap=true, bool bOnlyBody=false); 
           LRESULT OnFindReplaceCmd(WPARAM, LPARAM lParam);
           BOOL  InitializeReplace();
           BOOL  SameAsSelected(LPCTSTR lpszCompare, BOOL bCase);
           void  GetSelectedText(CString& strResult);
           void  ReplaceSel(LPCTSTR lpszNewText);

           bool  CheckSlots(void);
		   
		//connect with the HMI simulator 
		/*void instantiateServer();
		void releaseServer();
		void putInputComponentOnSimulator();
		void putOutputComponentOnSimulator();*/
 
protected:
	virtual void  OnIdle ();

   // added for drop-target functionality
           BOOL  GetObjectInfo(COleDataObject* pDataObject);
   virtual BOOL  OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
   virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD grfKeyState, CPoint point);
   virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD grfKeyState, CPoint point);
   virtual void  OnDragLeave();
           void DetectSignalChange();
           void InitSignalChangeDetect();

public:
           BOOL  DoPasteItem(COleDataObject* pDataObject, bool bWithMoveMode=true);
           bool  DoPasteItem(void);   // for XML from m_ClipElem ..

   static CLIPFORMAT m_uiClipFormatId;
   static CLIPFORMAT m_uiCBFormatXMLId;

private:
   void GetPosition  (CStdEditorPosition &position);
   void GotoPosition (const CStdEditorPosition &position);
   void GotoLocation (const CString &strLocation);

   void TrackWP (CPoint point);

   void OnInsertFBCallHint (const CString &strFB);

   void InvalidateElem      (CElem *pE);
   void InvalidateSelection (void);
   void InvalidateMarkedElem(void);
   void InvalidateRubber    (void);
   void InvalidateLightPins (void);
   void InvalidateFixPoints (void);

   void UpdateModified (void);
   void UpdateColor    (void);
   void OnEditnameStop (void);
   void OnVarSelStop (void);
   void OnUpdateStatus (void);
   void OnSetPageSize  (void);

   void OnChangeZoom(ZoomOp op);
   void OnShortcut(UINT nChar);

   void OnToggleBookmark();
   void OnBookmarkPrev();
   void OnBookmarkNext();
   void OnGotoBookmark(bool  bNext=true);
   void OnClearBookmarks();
   void OnSpacing(EDI_MODE mode);
   void OnToggleFixPoint();
   void OnUpdateSTActions(CElemAct* pAct);

   bool TryChangeToMoveMode();
   bool FindNextTempVar();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
// Generated message map functions
   //{{AFX_MSG(CGrEditorView)
   afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCancelEditSrvr();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnInsFb();
	afx_msg void OnInsVarIn();
	afx_msg void OnInsVarOut();
	afx_msg void OnInsVarinout();
	afx_msg void OnInsLab();
	afx_msg void OnInsJmp();
   afx_msg void OnInsConIn();
   afx_msg void OnInsConOut();
   afx_msg void OnInsCmt();
   afx_msg void OnInsHLine();
   afx_msg void OnInsVLine();
	afx_msg void OnInsRet();
	afx_msg void OnInsStep();
	afx_msg void OnInsTransition();
	afx_msg void OnInsAction();
    afx_msg void OnInsExceptionAction();
	afx_msg void OnInsActionBlock();
	afx_msg void OnInsDivCon();
	afx_msg void OnInsSDivCon();
   afx_msg void OnInsertFBCall ();
	afx_msg void OnInsPower();
	afx_msg void OnInsCtNormal();
	afx_msg void OnInsCtClosed();
	afx_msg void OnInsCtPosTrans();
	afx_msg void OnInsCtNegTrans();
	afx_msg void OnInsClNormal();
	afx_msg void OnInsClNegative();
	afx_msg void OnInsClSet();
	afx_msg void OnInsClReset();
	afx_msg void OnInsClPosTrans();
	afx_msg void OnInsClNegTrans();
	afx_msg void OnEditResize();
	afx_msg void OnEditFind();
	afx_msg void OnEditRepeat();
	afx_msg void OnEditReplace();
	afx_msg void OnEditCol();
	afx_msg void OnEditRow();
	afx_msg void OnEditComment();
	afx_msg void OnEditUncomment();
	afx_msg void OnEditLine();
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg void OnEditClear();
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnEditName();
	afx_msg void OnEditProperty();
    afx_msg void OnSetPropFBDAction();
    afx_msg void OnSetPropLDAction(); 
    afx_msg void OnSetPropSTAction();
	afx_msg void OnEditInstance();
	afx_msg void OnEditAutoSize();
	afx_msg void OnSelAll();
	afx_msg void OnEndPropEdit();
	afx_msg void OnOptSfc();
	afx_msg void OnOptStdSize();
	afx_msg void OnOptChangeWarn();
	afx_msg void OnStepToggleinit();
	afx_msg void OnDebug();
	afx_msg void OnDebug2();
	afx_msg void OnUpdateOptChangeWarn(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditReplace(CCmdUI* pCmdUI);
	afx_msg void OnUpdateNeedText(CCmdUI* pCmdUI);
	afx_msg void OnUpdateNeedFind(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditLine(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditcol(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditrow(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditClear(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEdit(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEMNormal(CCmdUI* pCmdUI);
    afx_msg void OnUpdateEditorVarInOut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditInstamce(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditorLD(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditorSFC(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOptSfc(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOptStdSize(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDebug1(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDebug2(CCmdUI* pCmdUI);
	afx_msg void OnUpdateZoom(CCmdUI* pCmdUI);
	afx_msg void OnZoom();
	afx_msg void OnZoomin();
	afx_msg void OnZoomout();
	afx_msg void OnGrRoute();
	afx_msg void OnUpdateGrRoute(CCmdUI* pCmdUI);
	afx_msg void OnGrUnroute();
	afx_msg void OnUpdateGrUnroute(CCmdUI* pCmdUI);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenuKey ();
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
   afx_msg void OnEditActQual(UINT nID);
   afx_msg void OnUpdateEditor(CCmdUI* pCmdUI);
   afx_msg void OnViewObjHelp ();
   afx_msg void OnEditAutoSize2(WPARAM wParam, LPARAM lParam);
   afx_msg void OnPositionChanged(WPARAM wParam, LPARAM lParam);
   afx_msg void OnShowVarSelPopup();
   afx_msg void OnShowVarSelection ();
   afx_msg void OnInsertLruPou(UINT nID);
   afx_msg void OnXRefChangeRefPositionLast();
   afx_msg void OnXRefChangeRefPositionNext();
   afx_msg void OnXRefSetActiveQuery();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


#endif // !defined(AFX_GREDITORVIEW_H__59119A23_6422_11D2_9A3D_0000C0D26FBC__INCLUDED_)
