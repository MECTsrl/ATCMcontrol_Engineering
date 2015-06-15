
// STEditorView.h : interface of the CSTEditorView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_STEDITORVIEW_H__9F791445_09AC_11D2_B9A5_006008749B3D__INCLUDED_)
#define AFX_STEDITORVIEW_H__9F791445_09AC_11D2_B9A5_006008749B3D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include "SECEditView.h"
#include "baselib.h"
#include "StdEdAttr.h"
#include "fccallback.h"
#include "PrgEditorView.h"
#include "Flag.h"

#define WM_EDITOR_POSITION_CHANGED   (WM_USER + 666)

class CXMLArchive;
class CSTEditorDoc;
class CWatchpoint;
class CBreakpoint;
class CPrgEdStep;
class CPrgEdAction;
class CPrgEdTransition;


class CSTEditorView : public CPrgEditorView<SECEditView>
{
protected: // create from serialization only
    CSTEditorView();
    DECLARE_DYNCREATE(CSTEditorView)

// Attributes
public:
    CPrgEditorDoc* GetDocument();
    void DrawInto (CDC *pDC, CSize drawAreaSize);

// Operations
public:
    virtual void SerializeText (CArchive &ar);
    void SetDelayedText(const  CString& s);
    void InsertFBCall(const CString &strXML);
    // SECEditView requires to override this
    virtual SECEdit    *GetEdit (){ return GetEditor(GetEditID());}
    typedef enum {              // useable editor features
        STF_DEBUGGING,
        STF_MONITORING,
        STF_USERHELP,
        STF_AUTOFMT,        // AUTOFMT 10.08.06 SIS
        STF_POU_INS,
        STF_DECL_INS,
        STF_STMT_INS,
        STF_LAST
    } STFeatureSet;

    CFlag<STFeatureSet> m_Features;

    void        SetEditID(UINT id)          { m_EditorID = id;  }
    UINT        GetEditID()                 { return m_EditorID;}

    void ExtendMenuBySFCItems (CMenu *pMenu);

	virtual void AutoIndent(BOOL bSaveUndoRecord);	// AUTOFMT 07.08.06 SIS
    virtual BOOL FormatSelection();                 // AUTOFMT 10.08.06 SIS

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSTEditorView)
	public:
    virtual void OnDraw(CDC* pDC);  // overridden to draw this view
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
    virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
    virtual void OnInitialUpdate();
    virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CSTEditorView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:

    virtual BOOL ExtendContextMenu (CMenu *pMenu);
    virtual void SubClassDraw (CDC *pDC, CRect drawArea);
    virtual void GetSubclassFlags (PLINEDESC pLine, char *pchFlags);
    virtual bool OverrideTextBackground (char chFlag, COLORREF &backgroundColor);
    virtual bool OverrideTextForeground (char chFlag, COLORREF &foregroundColor);
    virtual COLORREF GetGutterColor (COLORREF clrEmpty);
    virtual void OnIdle ();
    virtual BOOL InitializeDataSource (COleDataSource *pDataSource, CLIPFORMAT clipFormat, HGLOBAL hGlobal);
    virtual bool OnModifyContent ();

    // AUTOFMT 07.08.06 SIS >>
    BOOL IsIndentIncreaseKeyword(const CString& crsKeyword);
    BOOL IsIndentDecreaseKeyword(const CString& crsKeyword);
    void CheckDecreaseIndent(PLINEDESC pLine, BOOL bSaveUndoRecord);
    void CheckIncreaseIndent(PLINEDESC pLine, BOOL bSaveUndoRecord);
    void AdaptIndent(PLINEDESC pLine, int iTargetCol, BOOL bSaveUndoRecord, BOOL bFormatMode = FALSE);
    CString GetFirstKeyword(PLINEDESC pLine);
    BOOL IsCorrespondingStartKeyword(const CString& crsKeyword, const CString& crsKeywordTest);
    void IndentRemoveHiddenSpaces(PLINEDESC pLine);
    PLINEDESC GetNextNonEmptyLine(PLINEDESC pLine, BOOL bIncludeCurrentLine = FALSE);
    // AUTOFMT 07.08.06 SIS <<

// Generated message map functions
protected:
    //{{AFX_MSG(CSTEditorView)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnGotoLine();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    afx_msg void OnEditScrollDown();
    afx_msg void OnEditScrollUp();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
    //}}AFX_MSG

    afx_msg void OnSetWatchpoint ();
    afx_msg void OnClearWatchpoint ();
    afx_msg void OnClearAllWatchpoints ();
    afx_msg void OnWriteWatchpoint ();
    afx_msg void OnUpdateMonitor (CCmdUI *pCmdUI);
    afx_msg void OnToggleBreakpoint ();
    afx_msg void OnRemoveAllBreakpoints ();
    afx_msg void OnToggleBreakpointEnabled ();
    afx_msg void OnUpdateDebug (CCmdUI *pCmdUI);
    afx_msg void OnInsertFBCall ();
    afx_msg void OnShowVarSelection ();
    afx_msg void OnContinueTask ();
    afx_msg void OnStepInto ();
    afx_msg void OnStepOver ();
    afx_msg void OnStepOut ();
    afx_msg void OnInsertIF ();
    afx_msg void OnInsertCASE ();
    afx_msg void OnInsertFOR ();
    afx_msg void OnInsertWHILE ();
    afx_msg void OnInsertREPEAT ();
    afx_msg void OnInsertAssignment ();
    afx_msg void OnInsertRETURN ();
    afx_msg void OnInsertEXIT ();
    afx_msg void OnInsertVAR ();
    afx_msg void OnInsertCONST();
    afx_msg void OnInsertVARGLOBAL();
    afx_msg void OnInsertVAREXTERNAL();
    afx_msg void OnInsertINITIALSTEP ();
    afx_msg void OnInsertSTEP ();
    afx_msg void OnInsertACTION ();
    afx_msg void OnInsertTRANSITION ();
    afx_msg void OnContextMenuKey ();
    afx_msg void OnSuspendResumeWatchpoints (WPARAM, LPARAM);
    afx_msg void OnToggleManualMode ();
    afx_msg void OnUpdateSFCMenu (CCmdUI *pCmdUI);
    afx_msg void OnPerformCycle ();
    afx_msg void OnForceTransitionOnce ();
    afx_msg void OnForceTransition ();
    afx_msg void OnBlockTransition ();
    afx_msg void OnForceAllTransitions ();
    afx_msg void OnBlockAllTransitions ();
    afx_msg void OnRevertAllTransitions ();
    afx_msg void OnGloballyForceAllTransitions ();
    afx_msg void OnForceActionOnce ();
    afx_msg void OnForceAction ();
    afx_msg void OnBlockAction ();
    afx_msg void OnForceAllActions ();
    afx_msg void OnBlockAllActions ();
    afx_msg void OnRevertAllActions ();
    afx_msg void OnGloballyBlockAllActions ();
    afx_msg void OnViewObjHelp ();
    afx_msg void OnEditorPositionChanged(WPARAM, LPARAM);
    afx_msg void OnInsertLruPou(UINT nID);
    afx_msg void OnXRefChangeRefPositionLast();
    afx_msg void OnXRefChangeRefPositionNext();
    afx_msg void OnXRefSetActiveQuery();
    afx_msg void OnShowVarSelPopup();
    afx_msg void OnUpdateEdit(CCmdUI *pCmdUI);
    afx_msg void OnUpdateFormatSelection(CCmdUI *pCmdUI);   // AUTOFMT 10.08.06 SIS
    afx_msg void OnUpdateEditorSettings(CCmdUI *pCmdUI);    // AUTOFMT 11.08.06 SIS

    afx_msg void OnInsertLD ();
    afx_msg void OnInsertLDN ();
    afx_msg void OnInsertST ();
    afx_msg void OnInsertSTN ();
    afx_msg void OnInsertS ();
    afx_msg void OnInsertR ();
    afx_msg void OnInsertAND ();
    afx_msg void OnInsertANDN ();
    afx_msg void OnInsertOR ();
    afx_msg void OnInsertORN ();
    afx_msg void OnInsertXOR ();
    afx_msg void OnInsertXORN ();
    afx_msg void OnInsertADD ();
    afx_msg void OnInsertSUB ();
    afx_msg void OnInsertMUL ();
    afx_msg void OnInsertDIV ();
    afx_msg void OnInsertGT ();
    afx_msg void OnInsertGE ();
    afx_msg void OnInsertEQ ();
    afx_msg void OnInsertNE ();
    afx_msg void OnInsertLE ();
    afx_msg void OnInsertLT ();
    afx_msg void OnInsertJMP ();
    afx_msg void OnInsertJMPC ();
    afx_msg void OnInsertJMPCN ();
    afx_msg void OnInsertCAL ();
    afx_msg void OnInsertCALC ();
    afx_msg void OnInsertCALCN ();
    afx_msg void OnInsertRET ();
    afx_msg void OnInsertRETC ();
    afx_msg void OnInsertRETCN ();
    afx_msg void OnFormatSelection();   // AUTOFMT 10.08.06 SIS
    afx_msg void OnEditorSettings();    // AUTOFMT 11.08.06 SIS

    DECLARE_MESSAGE_MAP()

    UINT m_uiClipFormatId;
    UINT m_uiCBFormatXMLId;

    int m_iLastLine;
    int m_iLastColumn;



    bool  m_bHaveWPFont;
    CFont m_WatchpointFont;

    CWatchpoint *m_pTrackedWP;
    CPoint      m_WPGrabPosition;

    BOOL    m_bIsInitiallyUpdated;
    CString m_strDelayedText;
    bool    m_bEnteredSuspending;
    bool    m_bRedrawAfterSFCUpdate;
    UINT    m_EditorID;

    CList<CString, CString&> m_listLruPous;

private:
    CWatchpoint *HitWatchpoint (CPoint point, CPoint &grabPosition);
    void TrackWP (CPoint point);
    bool ConvertMousePosition (const CPoint &mousePoint, CStdEditorPosition &position);
    bool ConvertPositionMouse (const CStdEditorPosition &position, CPoint &mousePoint);


    void OnShowWatchpoint (CWatchpoint *pWP);
    void BuildWatchpointText (CWatchpoint *pWP);
    void UpdateWatchpointArea (CWatchpoint *pWP, CDC *pDC);
    void UpdateWatchpointExtent (CWatchpoint *pWP, CDC *pDC);
    bool UpdateWatchpointVisibility (CPrgWPClientData *pClientData, CRect drawArea);
    void SuspendInvisibleWatchpoints ();
    void DrawWatchpoints (CDC *pDC, CRect drawArea);
    void DrawWatchpoint (CDC *pDC, CRect drawArea, CWatchpoint *pWP);

    void DrawActionInfos (CDC *pDC, CRect drawArea);
    void DrawActionInfo (CDC *pDC, CRect drawArea, CPrgEdAction *pAction);
    void DrawTransitionInfos (CDC *pDC, CRect drawArea);
    void DrawTransitionInfo (CDC *pDC, CRect drawArea, CPrgEdTransition *pTransition);
    void DrawSFCInfo (CDC *pDC, CRect drawArea, const CStdEditorPosition &position, const CString &strText);
    CWatchpoint *FindWatchpointByPoint (CPoint point);
    void WPAreaToClientArea (CRect &area);
    void ClientAreaToWPArea (CRect &area);

    void DrawMarks (CDC *dc, RECT *rect, PLINEDESC pLine);
    void DrawBreakpoint (CDC *pDC, RECT *rect, PLINEDESC pLine);
    void DrawStoppoint (CDC *pDC, RECT *rect, PLINEDESC pLine);

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

    void GotoPosition (const CStdEditorPosition &position);
    void GetPosition (CStdEditorPosition &position);
    void GotoLocation (const CString &strLocation);

    void OnInsertFBCallHint (const CString &strFB);
    void OnInsertVariable(const CString &strVar);

    HRESULT BuildFunctionCallFromXML (CXMLArchive &ar, CString &strCall);
    HRESULT BuildFBCallFromXML (CXMLArchive &ar, CString &strCall);
    HRESULT BuildConstantValueFromXML (CXMLArchive &ar, CString &strCall);

    HRESULT InsertDeclaration (const CString &strXML);
    HRESULT BuildFBDeclaration (CXMLArchive &ar, CString &strDecl);
    HRESULT BuildSDTDeclaration (CXMLArchive &ar, CString &strDecl);

    void InsertTextLines (CString strLines);

    void SystemMessage (HRESULT hr);

    void RecreateCaret ();

    CString GetCursorContextExpression (CStdEditorPosition &startPosition, CStdEditorPosition &endPosition);
    CString GetContextExpression (CPoint point, CStdEditorPosition &startPosition, CStdEditorPosition &endPosition);
    CPoint GetCursorPoint ();

    void ScrollLeftColumn ();
    void ScrollRightColumn ();

    void GetSFCMenuProperties (UINT uiId, bool &bEnabled, bool &bChecked);
    DWORD GetSFCMenuStyle (UINT uiId);

    LPCTSTR GetDataTypeName(UINT uiIECType);
};

#ifndef _DEBUG  // debug version in STEditorView.cpp
inline CPrgEditorDoc* CSTEditorView::GetDocument()
   { return (CPrgEditorDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STEDITORVIEW_H__9F791445_09AC_11D2_B9A5_006008749B3D__INCLUDED_)

