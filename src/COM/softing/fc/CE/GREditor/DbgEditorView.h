#if !defined(AFX_DBGEDITORVIEW_H__ED4B1AE1_94B2_11D2_A453_0000C0D26FBC__INCLUDED_)
#define AFX_DBGEDITORVIEW_H__ED4B1AE1_94B2_11D2_A453_0000C0D26FBC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ElemEditorView.h"
#include "PrgEditorView.h"

class CDbgEditorView : public CPrgEditorView<CElemEditorView>
{
protected:    
   CDbgEditorView();
   DECLARE_DYNCREATE(CDbgEditorView)
      
      // Attributes
//      enum DrawTarget { ActiveView, InactiveView };
   
   bool               m_bIsStopped;   // flag to show stop position
/*
   bool               m_bHaveWPFont;
   CFont              m_WatchpointFont;
*/   
public:
   DrawTarget         m_DrawTarget;
   
   MSG                m_LastRelayedMessage;
   bool               m_bRedrawAfterSFCUpdate;
   
   // Operations
protected:
   void OnShowWatchpoint (CWatchpoint *pWP);
   bool UpdateWatchpointVisibility (CPrgWPClientData *pClientData, CRect drawArea);
   void UpdateWatchpointArea(CWatchpoint *pWP, CDC *pDC);
   void SuspendInvisibleWatchpoints ();

public:
   void OnSetLadderWatchpoints(void);
   void OnClearLadderWatchpoints(void);
   void UpdateWatchpoint (CWatchpoint *pWP);
   void UpdateWatchpoints ();
   void DrawWatchpoints (CDC *pDC, CRect drawArea);
   void DrawWatchpoint (CDC *pDC, CRect drawArea, CWatchpoint *pWP);
   void DrawActionInfos (CDC *pDC, CRect drawArea);
   void DrawActionInfo (CDC *pDC, CRect drawArea, CPrgEdAction *pAction);
   void DrawTransitionInfos (CDC *pDC, CRect drawArea);
   void DrawTransitionInfo (CDC *pDC, CRect drawArea, CPrgEdTransition *pTransition);
   void DrawSFCInfo (CDC *pDC, CRect drawArea, const CPoint &p, const CString &strText);
   CWatchpoint *FindWatchpointByPoint (CPoint point);
   bool BuildBreakpoint (CStdEditorPosition& bp_position);
   void GetSFCMenuProperties (UINT uiId, bool &bEnabled, bool &bChecked);
public:
   bool IsStepActive       (CPoint p);
   bool IsActionActive     (CPoint p);
   bool IsActionForced     (CPoint p);
   bool IsActionBlocked    (CPoint p);
   bool IsTransitionForced (CPoint p);
   bool IsTransitionBlocked(CPoint p);

   bool IsLadderActive     (CPoint p);

   bool IsManualMode();

public:
   virtual void    WPAreaToClientArea (CRect& rect);
   virtual CString GetContextExpression (CPoint point, CStdEditorPosition &startPos, 
      CStdEditorPosition &endPos);  

   CWatchpoint *HitWatchpoint (CPoint point, CPoint &grabPosition);
   
   void OnShowBreakpoint (CBreakpoint *pBP);
   void OnHideBreakpoint (CBreakpoint *pBP);
   
   void OnShowStoppoint ();
   void OnHideStoppoint ();
   
   void OnManualModeToggled ();
   void OnStepAttributeChanged (CPrgEdStep *pStep);
   void OnActionAttributeChanged (CPrgEdAction *pAction);
   void OnAllActionsOffToggled ();
   void OnTransitionAttributeChanged (CPrgEdTransition *pTransition);
   void OnAllTransitionsOnToggled ();
   void OnEndSFCUpdate ();
   bool OnToggleLadder ();
   // Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CElemEditorView)
public:
   virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnInitialUpdate();
protected:
   //}}AFX_VIRTUAL
   
   // Implementation
protected:
   virtual ~CDbgEditorView();
#ifdef _DEBUG
   virtual void AssertValid() const;
   virtual void Dump(CDumpContext& dc) const;
#endif
   
   // Generated message map functions
   //{{AFX_MSG(CElemEditorView)
   afx_msg void OnSetWatchpoint ();
   afx_msg void OnClearWatchpoint ();
   afx_msg void OnClearAllWatchpoints ();
   afx_msg void OnToggleBreakpoint ();
   afx_msg void OnRemoveAllBreakpoints ();
   afx_msg void OnToggleBreakpointEnabled ();
   afx_msg void OnContinueTask ();
   afx_msg void OnStepInto ();
   afx_msg void OnStepOver ();
   afx_msg void OnStepOut ();
   afx_msg void OnUpdateMonitor (CCmdUI *pCmdUI);
   afx_msg void OnUpdateDebug (CCmdUI *pCmdUI);
   //}}AFX_MSG
   afx_msg void OnWriteWatchpoint ();
   afx_msg void OnSuspendResumeWatchpoints (WPARAM, LPARAM);
   afx_msg void OnToggleManualMode ();
   afx_msg void OnPerformCycle ();
   afx_msg void OnForceTransitionOnce ();
   afx_msg void OnForceTransition ();
   afx_msg void OnBlockTransition ();
   afx_msg void OnForceAllTransitions ();
   afx_msg void OnForceActionOnce ();
   afx_msg void OnForceAction ();
   afx_msg void OnBlockAction ();
   afx_msg void OnBlockAllActions ();
   afx_msg void OnBlockAllTransitions ();
   afx_msg void OnRevertAllTransitions ();
   afx_msg void OnGloballyForceAllTransitions ();
   afx_msg void OnForceAllActions ();
   afx_msg void OnGloballyBlockAllActions ();
   afx_msg void OnRevertAllActions ();
   afx_msg void OnUpdateSFCMenu (CCmdUI *pCmdUI);
   
   DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_DBGEDITORVIEW_H__ED4B1AE1_94B2_11D2_A453_0000C0D26FBC__INCLUDED_)
