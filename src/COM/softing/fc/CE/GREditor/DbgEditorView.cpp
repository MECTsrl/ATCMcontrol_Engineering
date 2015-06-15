

/*doc*=======================================================================*/
/*inc*=======================================================================*/

#include "stdafx.h"
#include "greditor.h"
#include "DbgEditorView.h"
#include "ElemEditorView.h"
#include "GrEditorUtil.h"
#include "SrvrItem.h"

#include "CEGRError.h"
#include "CEBPManError.h"

#include "utilif.h"
#include "Watchpoint.h"
#include "CESysDef.h"

#include "PrgEditorView.h"
#include "PrgEditorDoc.h"

/*const*=====================================================================*/
/*macro*=====================================================================*/
/*glovar*====================================================================*/
/*type*======================================================================*/
/*locvar*====================================================================*/

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*proto*=====================================================================*/
/*func*======================================================================*/


class CGrEditVarDlg : public CDialog
{
// Construction
public:
    CGrEditVarDlg (CPrgEditorDoc *pDoc, const CString &strNamePreset, const CString &strValuePreset, CWnd *pParent=NULL);
    
// Dialog Data
    //{{AFX_DATA(CGrEditVarDlg)
    enum { IDD = IDD_GR_EDITVARIABLE };
    CEdit   m_VarNameCtrl;
    CComboBox   m_VarValueCtrl;
    //}}AFX_DATA
    
// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CGrEditVarDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL
    
// Implementation
protected:
    CPrgEditorDoc *m_pDoc;
    CString       m_strNamePreset;
    CString       m_strValuePreset;
    
    // Generated message map functions
    //{{AFX_MSG(CGrEditVarDlg)
    afx_msg void OnApply();
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    afx_msg void OnSelendokVarvalue();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
        
private:
    bool Apply ();
};

CGrEditVarDlg::CGrEditVarDlg(CPrgEditorDoc *pDoc, const CString &strNamePreset, const CString &strValuePreset, CWnd *pParent)
: CDialog (CGrEditVarDlg::IDD, pParent),
  m_pDoc (pDoc),
  m_strNamePreset (strNamePreset),
  m_strValuePreset (strValuePreset)
{
   //{{AFX_DATA_INIT(CGrEditVarDlg)
   //}}AFX_DATA_INIT
}

void CGrEditVarDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CGrEditVarDlg)
   DDX_Control(pDX, IDC_GR_VARNAME,  m_VarNameCtrl);
   DDX_Control(pDX, IDC_GR_VARVALUE, m_VarValueCtrl);
   //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGrEditVarDlg, CDialog)
   //{{AFX_MSG_MAP(CGrEditVarDlg)
   ON_BN_CLICKED(IDC_GR_APPLY, OnApply)
   ON_CBN_SELENDOK(IDC_GR_VARVALUE, OnSelendokVarvalue)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGrEditVarDlg message handlers

void CGrEditVarDlg::OnApply ()
{
   Apply ();
}

bool CGrEditVarDlg::Apply ()
{
    CString strVariableName;
    CString strVariableValue;
    CComVariant varValue;
    HRESULT hr;
    LONG lHandle;
    BL_IEC_TYP iecType;
    USES_CONVERSION;

    m_VarNameCtrl.GetWindowText (strVariableName);

    m_VarValueCtrl.GetWindowText (strVariableValue);

    if (strVariableName.IsEmpty ())
        return (true);

    hr = m_pDoc->AddWatchItem (strVariableName, lHandle);   // writes lHandle
    if (FAILED (hr))
    {
        AfxMessageBox (IDS_GR_WRONG_VAR_SYNTAX);
        return (false);
    }
    ASSERT (lHandle != 0);
    if (lHandle == 0)
        return (false);

    if (m_pDoc->IsSimpleWatchItemType (lHandle, iecType))
    {
        if (iecType == BL_LREAL && !strVariableValue.IsEmpty()
            && strVariableValue.Find(_T('.'))<0)
            strVariableValue += _T(".0");
        hr = BL_StringToVariant (T2COLE (strVariableValue), iecType, NULL, BL_USE_IEC_FORMAT, &varValue);
        if (FAILED (hr)) {
            AfxMessageBox (IDS_GR_WRONG_VALUE_SYNTAX);
            return (false);
        }
    }
    else
    {
        varValue = strVariableValue;
    };

    hr = m_pDoc->ConnectWatchItem (lHandle);
    if (FAILED (hr))
    {
        m_pDoc->RemoveWatchItem (lHandle);
        AfxMessageBox (IDS_GR_CANNOT_WRITE_VAR);
        return (false);
    };

    hr = m_pDoc->WriteWatchItem (lHandle, varValue);
    if (FAILED (hr))
    {
        m_pDoc->DisconnectWatchItem (lHandle);
        m_pDoc->RemoveWatchItem (lHandle);
        AfxMessageBox (IDS_GR_CANNOT_WRITE_VAR);
        return (false);
    };

    m_pDoc->DisconnectWatchItem (lHandle);
    m_pDoc->RemoveWatchItem (lHandle);

    OnSelendokVarvalue ();
    return (true);
}

void CGrEditVarDlg::OnOK ()
{
   if (Apply ())
   {
      CDialog::OnOK ();
   }
}

BOOL CGrEditVarDlg::OnInitDialog ()
{
   if (!CDialog::OnInitDialog ())
   {
      return (FALSE);
   }
   
   m_VarNameCtrl.SetWindowText (m_strNamePreset);
   
   m_VarValueCtrl.AddString (m_strValuePreset);
   m_VarValueCtrl.SetWindowText (m_strValuePreset);
   
   return TRUE;  // return TRUE unless you set the focus to a control
   // EXCEPTION: OCX Property Pages should return FALSE
}

void CGrEditVarDlg::OnSelendokVarvalue ()
{
   CString strValue;
   
   m_VarValueCtrl.GetWindowText (strValue);
   
   if (m_VarValueCtrl.FindString (0, strValue) < 0)
   {
      m_VarValueCtrl.AddString (strValue);
   }
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static const int WPHorizontalMargin=3;
static const int WPVerticalMargin=2;

static const int SUSPEND_RESUME_WATCHPOINTS = 10000;

IMPLEMENT_DYNCREATE(CDbgEditorView, CElemEditorView)

CDbgEditorView::CDbgEditorView()
{
   m_bIsStopped    = false;
   m_bShowAbbrevNames = true;
   m_DrawTarget    = ActiveView;
   m_bHaveWPFont   = m_WatchpointFont.CreateStockObject (DEFAULT_GUI_FONT) == TRUE;
   m_bRedrawAfterSFCUpdate = false;
}

CDbgEditorView::~CDbgEditorView()
{
   m_DataTip.SetPopupCB (NULL);
   m_DataTip.SetTextRequestCB (NULL);
   m_DataTip.SetActionQueryCB (NULL);
   
   RemoveDataTip ();

   if (m_bHaveWPFont)
	{
      m_WatchpointFont.DeleteObject();
   }
}

BEGIN_MESSAGE_MAP(CDbgEditorView, CElemEditorView)
	//{{AFX_MSG_MAP(CDbgEditorView)
   ON_COMMAND(ID_GR_SET_WATCHPOINT, OnSetWatchpoint)
	ON_COMMAND(ID_GR_CLEAR_WATCHPOINT, OnClearWatchpoint)
	ON_COMMAND(ID_GR_CLEAR_ALL_WATCHPOINTS, OnClearAllWatchpoints)
	ON_COMMAND(ID_GR_TOGGLE_BREAKPOINT, OnToggleBreakpoint)
	ON_COMMAND(ID_GR_TOGGLE_BREAKPOINT_ENABLED, OnToggleBreakpointEnabled)
	ON_COMMAND(ID_GR_REMOVE_ALL_BREAKPOINTS, OnRemoveAllBreakpoints)
	ON_COMMAND(ID_GR_CONTINUE_TASK, OnContinueTask)
    ON_COMMAND(ID_GR_STEP_INTO, OnStepInto)
    ON_COMMAND(ID_GR_STEP_OVER, OnStepOver)
    ON_COMMAND(ID_GR_STEP_OUT,  OnStepOut)
	ON_UPDATE_COMMAND_UI(ID_GR_SET_WATCHPOINT, OnUpdateMonitor)
	ON_UPDATE_COMMAND_UI(ID_GR_CLEAR_WATCHPOINT, OnUpdateMonitor)
	ON_UPDATE_COMMAND_UI(ID_GR_CLEAR_ALL_WATCHPOINTS, OnUpdateMonitor)
	ON_UPDATE_COMMAND_UI(ID_GR_TOGGLE_BREAKPOINT, OnUpdateDebug)
	ON_UPDATE_COMMAND_UI(ID_GR_TOGGLE_BREAKPOINT_ENABLED, OnUpdateDebug)
	ON_UPDATE_COMMAND_UI(ID_GR_REMOVE_ALL_BREAKPOINTS, OnUpdateDebug)
	ON_UPDATE_COMMAND_UI(ID_GR_CONTINUE_TASK, OnUpdateDebug)
	ON_UPDATE_COMMAND_UI(ID_GR_STEP_INTO, OnUpdateDebug)
	ON_UPDATE_COMMAND_UI(ID_GR_STEP_OVER, OnUpdateDebug)
	ON_UPDATE_COMMAND_UI(ID_GR_STEP_OUT, OnUpdateDebug)
   ON_COMMAND(ID_GR_TOGGLE_MANUAL_MODE, OnToggleManualMode)
   ON_COMMAND(ID_GR_PERFORM_CYCLE, OnPerformCycle)
   ON_COMMAND(ID_GR_FORCE_TRANSITION_ONCE, OnForceTransitionOnce)
   ON_COMMAND(ID_GR_FORCE_TRANSITION, OnForceTransition)
   ON_COMMAND(ID_GR_BLOCK_TRANSITION, OnBlockTransition)
   ON_COMMAND(ID_GR_FORCE_ALL_TRANSITIONS, OnForceAllTransitions)
   ON_COMMAND(ID_GR_FORCE_ACTION_ONCE, OnForceActionOnce)
   ON_COMMAND(ID_GR_FORCE_ACTION, OnForceAction)
   ON_COMMAND(ID_GR_BLOCK_ACTION, OnBlockAction)
   ON_COMMAND(ID_GR_BLOCK_ALL_ACTIONS, OnBlockAllActions)
   ON_COMMAND(ID_GR_WRITE_WATCHPOINT, OnWriteWatchpoint)
   ON_COMMAND(ID_GR_BLOCK_ALL_TRANSITIONS, OnBlockAllTransitions)
   ON_COMMAND(ID_GR_REVERT_ALL_TRANSITIONS, OnRevertAllTransitions)
   ON_COMMAND(ID_GR_GLOBALLY_FORCE_ALL_TRANSITIONS, OnGloballyForceAllTransitions)
   ON_COMMAND(ID_GR_FORCE_ALL_ACTIONS, OnForceAllActions)
   ON_COMMAND(ID_GR_REVERT_ALL_ACTIONS, OnRevertAllActions)
   ON_COMMAND(ID_GR_GLOBALLY_BLOCK_ALL_ACTIONS, OnGloballyBlockAllActions)
   ON_UPDATE_COMMAND_UI(ID_GR_TOGGLE_MANUAL_MODE, OnUpdateSFCMenu)
   ON_UPDATE_COMMAND_UI(ID_GR_PERFORM_CYCLE, OnUpdateSFCMenu)
   ON_UPDATE_COMMAND_UI(ID_GR_FORCE_TRANSITION_ONCE, OnUpdateSFCMenu)
   ON_UPDATE_COMMAND_UI(ID_GR_FORCE_TRANSITION, OnUpdateSFCMenu)
   ON_UPDATE_COMMAND_UI(ID_GR_BLOCK_TRANSITION, OnUpdateSFCMenu)
   ON_UPDATE_COMMAND_UI(ID_GR_FORCE_ALL_TRANSITIONS, OnUpdateSFCMenu)
   ON_UPDATE_COMMAND_UI(ID_GR_FORCE_ACTION_ONCE, OnUpdateSFCMenu)
   ON_UPDATE_COMMAND_UI(ID_GR_FORCE_ACTION, OnUpdateSFCMenu)
   ON_UPDATE_COMMAND_UI(ID_GR_BLOCK_ACTION, OnUpdateSFCMenu)
   ON_UPDATE_COMMAND_UI(ID_GR_BLOCK_ALL_ACTIONS, OnUpdateSFCMenu)
   ON_UPDATE_COMMAND_UI(ID_GR_WRITE_WATCHPOINT, OnUpdateMonitor)
   ON_UPDATE_COMMAND_UI(ID_GR_BLOCK_ALL_TRANSITIONS, OnUpdateSFCMenu)
   ON_UPDATE_COMMAND_UI(ID_GR_REVERT_ALL_TRANSITIONS, OnUpdateSFCMenu)
   ON_UPDATE_COMMAND_UI(ID_GR_GLOBALLY_FORCE_ALL_TRANSITIONS, OnUpdateSFCMenu)
   ON_UPDATE_COMMAND_UI(ID_GR_FORCE_ALL_ACTIONS, OnUpdateSFCMenu)
   ON_UPDATE_COMMAND_UI(ID_GR_REVERT_ALL_ACTIONS, OnUpdateSFCMenu)
   ON_UPDATE_COMMAND_UI(ID_GR_GLOBALLY_BLOCK_ALL_ACTIONS, OnUpdateSFCMenu)
	//}}AFX_MSG_MAP
   ON_MESSAGE(SUSPEND_RESUME_WATCHPOINTS, OnSuspendResumeWatchpoints)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXT, 0, 0xffff, OnToolTipNfy)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CElemEditorView drawing

/////////////////////////////////////////////////////////////////////////////
// CElemEditorView diagnostics

#ifdef _DEBUG
void CDbgEditorView::AssertValid() const
{
	CElemEditorView::AssertValid();
}

void CDbgEditorView::Dump(CDumpContext& dc) const
{
	CElemEditorView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDbgEditorView message handlers

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::OnInitialUpdate() 
{
   CElemEditorView::OnInitialUpdate();
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	if (lHint == CPrgEditorDoc::ToolTipNfyHint)
	{
		OnUpdateToolTipNotify( (TTNfyData*)pHint );
		return;
	};

   if (lHint == CPrgEditorDoc::PreDisconnectHint)
	{
		RemoveDataTip ();
		return;
	};

	if (lHint == CPrgEditorDoc::ShowWatchpointHint)
	{
		OnShowWatchpoint ((CWatchpoint *)pHint);
		return;
	};

	if (lHint == CPrgEditorDoc::HideWatchpointHint)
	{
		OnHideWatchpoint ((CWatchpoint *)pHint);
		return;
	};

   if (lHint == CPrgEditorDoc::WatchpointDataHint)
	{
		UpdateWatchpoint ((CWatchpoint *)pHint);
		return;
	};

	if (lHint == CPrgEditorDoc::ShowBreakpointHint)
	{
		OnShowBreakpoint ((CBreakpoint *)pHint);
		return;
	};

	if (lHint == CPrgEditorDoc::HideBreakpointHint)
	{
		OnHideBreakpoint ((CBreakpoint *)pHint);
		return;
	};

	if (lHint == CPrgEditorDoc::ShowStoppointHint)
	{
		OnShowStoppoint ();
		return;
	};

	if (lHint == CPrgEditorDoc::HideStoppointHint)
	{
		OnHideStoppoint ();
		return;
	};

   if (lHint == CPrgEditorDoc::ExpressionChangedHint)
   {
      if ((LONG)pHint == m_lDataTipHandle)
      {
         m_DataTip.Follow (true);
      }
      return;
   }

   if (lHint == CPrgEditorDoc::ManualModeToggledHint)
   {
      OnManualModeToggled ();
      return;
   }

   if (lHint == CPrgEditorDoc::StepAttributeChangedHint)
   {
      OnStepAttributeChanged ((CPrgEdStep *)pHint);
      return;
   }
    
   if (lHint == CPrgEditorDoc::ActionAttributeChangedHint)
   {
      OnActionAttributeChanged ((CPrgEdAction *)pHint);
      return;
   }

   if (lHint == CPrgEditorDoc::AllActionsOffToggledHint)
   {
      OnAllActionsOffToggled ();
      return;
   }

   if (lHint == CPrgEditorDoc::TransitionAttributeChangedHint)
   {
      OnTransitionAttributeChanged ((CPrgEdTransition *)pHint);
      return;
   }
    
   if (lHint == CPrgEditorDoc::AllTransitionsOnToggledHint)
   {
      OnAllTransitionsOnToggled ();
      return;
   }
   
   if (lHint == CPrgEditorDoc::EndSFCUpdateHint)
   {
      OnEndSFCUpdate ();
      return;
   };

//   CElemEditorView::OnUpdate (pSender, lHint, pHint);
}




/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
CWatchpoint *CDbgEditorView::HitWatchpoint (CPoint point, CPoint &grabPosition)
{
   CGrEditorDoc *pDoc;
   POSITION pPosition;
   CWatchpoint *pBestWP=NULL;
   CPrgWPClientData *pBestClientData=NULL;
   CRect rect;
   
   pDoc = GetDocument ();
   
   pPosition = pDoc->GetFirstWPPosition ();
   while (pPosition != NULL)
   {
      CWatchpoint *pWP;
      CPrgWPClientData *pClientData;
      
      pWP = pDoc->GetNextWP (pPosition);
      ASSERT (pWP != NULL);
      if (pWP == NULL)
         continue;
      
      pClientData = (CPrgWPClientData *)pWP->GetClientData ();
      ASSERT (pClientData != NULL);
      if (pClientData == NULL)
         continue;
      
      rect = pClientData->GetArea ();
      
      if (point.x >= rect.left && point.x < rect.right &&
         point.y >= rect.top && point.y < rect.bottom)
      {
         if (pBestWP == NULL || pClientData->GetZ () < pBestClientData->GetZ ())
         {
            pBestWP = pWP;
            pBestClientData = pClientData;
         }
      }
   }

   if (pBestWP == NULL)
   {
      return (NULL);
   }

   rect = pBestClientData->GetArea ();
   
   grabPosition.x = point.x - rect.left;
   grabPosition.y = point.y - rect.top;

   return (pBestWP);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::OnShowWatchpoint (CWatchpoint *pWP)
{
   if (pWP->GetPosition().GetPosQual() == EDP_STLINE)            // header
   {
      return;
   } else // if (pWP->GetPosition().GetPosQual == EDP_GRSTBOX)  
          // EDP_GRBOX EDP_GRREGION EDP_GRBEGIN EDP_GREND EDP_GRLINE EDP_GRSIGNAL
          // body (gr)
   {
	   CClientDC dc (this);

	   BuildWatchpointText (pWP);
	   UpdateWatchpointArea (pWP, &dc);

	   if (GetDocument ()->IsInPlaceActive ())
	   {
		   Invalidate ();
		   UpdateWindow ();
	   }
	   else
	   {
		   COleServerItem *pItem;

		   pItem = GetDocument ()->GetEmbeddedItem ();
		   assert (pItem != NULL);
		   if (pItem != NULL)
		   {
			   pItem->NotifyChanged ();
		   }
	   }
   }
/*   else
   {
      ASSERT(0&&"redirect to the STAction-View");
   }
*/
}





/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
bool CDbgEditorView::UpdateWatchpointVisibility (CPrgWPClientData *pClientData, CRect drawArea)
{
	CRect rect;
	CRect clientRect;
	bool  bVisible;

   rect = pClientData->GetLargestArea ();

	if (m_DrawTarget == ActiveView)
	{
		GetClientRect (&clientRect);
	}
	else
	{
		clientRect = drawArea;
	}

   clientRect.OffsetRect( m_LastScrollPos );

	bVisible = rect.bottom > clientRect.top  && rect.top  < clientRect.bottom &&
	           rect.right  > clientRect.left && rect.left < clientRect.right;

	if (m_DrawTarget == ActiveView)
	{
      pClientData->SetActiveVisibility (bVisible);
	}
	else
	{
		pClientData->SetInactiveVisibility (bVisible);
	}

	return (bVisible);
}


/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::OnSuspendResumeWatchpoints (WPARAM, LPARAM)
{
   SuspendInvisibleWatchpoints ();
}


/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::SuspendInvisibleWatchpoints ()
{
	CGrEditorDoc *pDoc;
	POSITION pPosition;

   if (m_bEnteredSuspending)
   {
      return;
   }
   m_bEnteredSuspending = true;

	pDoc = GetDocument ();

	pPosition = pDoc->GetFirstWPPosition ();
	while (pPosition != NULL)
	{
		CWatchpoint *pWP;
      CPrgWPClientData *pClientData;
		bool bVisible;

		pWP = pDoc->GetNextWP (pPosition);
		ASSERT (pWP != NULL);
		if (pWP == NULL)
			continue;

      pClientData = (CPrgWPClientData *)pWP->GetClientData ();
		ASSERT (pClientData != NULL);
		if (pClientData == NULL)
			continue;

		if (pDoc->IsInPlaceActive ())
		{
			bVisible = pClientData->GetActiveVisibility ();
		}
		else
		{
			bVisible = pClientData->GetInactiveVisibility ();
		};

		if (bVisible)
		{
			pDoc->ResumeWatchpoint (pWP);
		}
		else
		{
			pDoc->SuspendWatchpoint (pWP);
		}
	}

   m_bEnteredSuspending = false;
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::UpdateWatchpoint (CWatchpoint *pWP)
{
	CPrgWPClientData *pClientData;
	CGrEditorDoc *pDoc;

	pDoc = GetDocument();

	if (pDoc->IsInPlaceActive())
	{
        // SIS bug fix d-2055
        // find ST-Editor to calculate offset rect within GR-EditorView
       UINT             uiEditorSearch = pWP->GetPosition().GetCX();
       UINT             uiEditor;
       CSTEditorView*   pEditView;
       CPoint           tOffset(0,0);

       POSITION         pos = m_STViewMap.GetStartPosition();
       while(pos)
       {
           m_STViewMap.GetNextAssoc(pos, uiEditor, pEditView);
           if(pEditView->GetEditID() == uiEditorSearch)
           {
               CRect    tSTClientRect;
               CRect    tGRClientRect;

               pEditView->GetClientRect(&tSTClientRect);
               pEditView->ClientToScreen(&tSTClientRect);
               tSTClientRect.left += pEditView->m_iMarkSpace;
               GetClientRect(&tGRClientRect);
               ClientToScreen(&tGRClientRect);
               tOffset = tSTClientRect.TopLeft() - tGRClientRect.TopLeft();
               break;
           }
       }

		CClientDC dc (this);
		CRect rect;
      OnPrepareDC(&dc);

		pClientData = (CPrgWPClientData *)pWP->GetClientData ();
		assert (pClientData != NULL);
		if (pClientData == NULL)
			return;

      if (pWP->GetPosition().GetPosQual() == EDP_GRLDBOX) 
      {
		   BuildWatchpointText (pWP);

		   rect = pClientData->GetArea ();
		   WPAreaToClientArea (rect);
         rect.InflateRect(1,1);
         rect.OffsetRect(tOffset);   // SIS: client rect of ST to GR coordinates
		   InvalidateRect (&rect);

		   UpdateWindow ();
      } else {
		   UpdateWatchpointExtent (pWP, &dc);
		   rect = pClientData->GetArea ();
           
		   WPAreaToClientArea (rect);
           rect.OffsetRect(tOffset);   // SIS: client rect of ST to GR coordinates

		   InvalidateRect (&rect);

		   BuildWatchpointText (pWP);

		   UpdateWatchpointExtent (pWP, &dc);
		   rect = pClientData->GetArea ();
		   WPAreaToClientArea (rect);
           rect.OffsetRect(tOffset);   // SIS: client rect of ST to GR coordinates

		   InvalidateRect (&rect);

		   UpdateWindow ();
      }
	}
	else
	{
		COleServerItem *pItem;

		BuildWatchpointText (pWP);

		pItem = pDoc->GetEmbeddedItem ();
		ASSERT (pItem != NULL);
		if (pItem != NULL)
		{
			pItem->NotifyChanged ();
		};
	};
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::UpdateWatchpoints ()
{
	CClientDC     dc (this);
	CGrEditorDoc *pDoc;
	POSITION      pPosition;
   EDP_POS_QUALIFIER qual;

   if (m_bEnteredSuspending)
   {
      return;
   }

	pDoc = GetDocument ();
   if (pDoc == NULL)
   {
      return;
   };

	pPosition = pDoc->GetFirstWPPosition ();
	while (pPosition != NULL)
	{
		CWatchpoint *pWP;

		pWP = pDoc->GetNextWP (pPosition);
		ASSERT (pWP != NULL);
      if (pWP == NULL)
         continue;

      qual = pWP->GetPosition().GetPosQual(); 
      if ((qual == EDP_GRBOX   ) || 
          (qual == EDP_GRREGION) || 
          (qual == EDP_GRBEGIN ) || 
          (qual == EDP_GREND   ) || 
          (qual == EDP_GRLINE  ) || 
          (qual == EDP_GRLDBOX ) || 
          (qual == EDP_GRSTBOX ) ||
          (qual == EDP_GRSIGNAL))
      {
         UpdateWatchpointArea (pWP, &dc);
      }
	}
}


/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::UpdateWatchpointArea (CWatchpoint *pWP, CDC *pDC)
{
	CPrgWPClientData *pClientData;
	long lDisplayX;
	long lDisplayY;
	long lDisplayOffsetX;
	long lDisplayOffsetY;
	CRect rect;
	CGrEditorDoc *pDoc;

	pDoc = GetDocument ();

	pClientData = (CPrgWPClientData *)pWP->GetClientData ();
	ASSERT (pClientData != NULL);
	if (pClientData == NULL)
	{
		return;
	};

   switch (pWP->GetPosition().GetPosQual())
   {
   case EDP_GRLDBOX:
      rect.left   = pClientData->GetStartPosition().GetX();
      rect.top    = pClientData->GetStartPosition().GetY();
      rect.right  = pClientData->GetEndPosition().GetX();
      rect.bottom = pClientData->GetEndPosition().GetY();
      GridToDoc(rect);
   	pClientData->SetArea (rect);
      break;

//   case EDP_STLINE: // ?? todo
   case EDP_GRBOX:
	   pClientData->GetDisplay (lDisplayX, lDisplayY, lDisplayOffsetX, lDisplayOffsetY);
      rect.left = rect.right  = lDisplayX * m_iZGrid + lDisplayOffsetX;
      rect.top  = rect.bottom = lDisplayY * m_iZGrid + lDisplayOffsetY;
      pClientData->SetArea (rect);
      UpdateWatchpointExtent (pWP, pDC);
      break;
   default: ASSERT(0); break;
   }
}


/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::DrawWatchpoints (CDC *pDC, CRect drawArea)
{
	CGrEditorDoc *pDoc;
   CArray<CWatchpoint *, CWatchpoint *> watchpoints;
   int iIndex;

	pDoc = GetDocument ();
   if (pDoc == NULL)
   {
      return;
   }
   
   pDoc->ZOrderWatchpoints (watchpoints);
   
   for (iIndex = watchpoints.GetSize () - 1; iIndex >= 0; --iIndex)
   {
      DrawWatchpoint (pDC, drawArea, watchpoints.GetAt (iIndex));
   };
   
   PostMessage (SUSPEND_RESUME_WATCHPOINTS);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::DrawWatchpoint (CDC *pDC, CRect drawArea, CWatchpoint *pWP)
{
   CPrgWPClientData *pClientData;
   CPen   *pOldPen;
   CBrush *pOldBrush;
	CFont  *pOldFont=NULL;
	CRect   rect;
	CRect   r;
   bool    bScreen = !pDC->IsPrinting();
   COLORREF OldTextCol;

   pClientData = (CPrgWPClientData *)pWP->GetClientData ();
	ASSERT (pClientData != NULL);
	if (pClientData == NULL)
	{
		return;
	}

   if ((pWP->GetPosition().GetPosQual() == EDP_STLINE) || 
       (pWP->GetPosition().GetPosQual() == EDP_GRSTBOX))  // skip header and STaction watchpoints
   {
      return;
   }

   switch (pWP->GetPosition().GetPosQual())
   {
   case EDP_STLINE: // todo
   case EDP_GRBOX:
      UpdateWatchpointExtent (pWP, pDC);

	   if (!UpdateWatchpointVisibility (pClientData, drawArea))
	   {
		   return;
	   }
	   if (m_bHaveWPFont)
	   {
		   pOldFont = pDC->SelectObject (&m_WatchpointFont);
	   }
      pOldBrush  = pDC->SelectObject(GDIHelper::GetBrush(bScreen, BT_FBLKBK));
      pOldPen    = pDC->SelectObject(GDIHelper::GetPen  (bScreen, PE_GRID  )); 
      OldTextCol = pDC->SetTextColor(GDIHelper::GetColor(bScreen, CO_NORMAL));

	   rect = pClientData->GetArea ();

      pDC->FillSolidRect (&rect, pClientData->GetBackgroundColor ());
      pDC->Rectangle (&rect);
      pDC->SetBkMode( TRANSPARENT );
	   pDC->TextOut (rect.left + WPHorizontalMargin, rect.top + WPVerticalMargin, pClientData->GetText ());

      if (pClientData->GetCrossedOut ())
      {
         CPen pen (PS_SOLID, 1, RGB (255, 0, 0));
         CPen *pOldPen;
   
         pOldPen = pDC->SelectObject (&pen);
   
         pDC->MoveTo (rect.left, rect.top);
         pDC->LineTo (rect.right - 1, rect.bottom - 1);
   
         pDC->MoveTo (rect.right - 1, rect.top);
         pDC->LineTo (rect.left, rect.bottom - 1);
   
         pDC->SelectObject (pOldPen);
      }

      pDC->SetTextColor(OldTextCol);
      pDC->SelectObject(pOldPen); 
      pDC->SelectObject(pOldBrush);

	   if (m_bHaveWPFont && pOldFont)
	   {
         pDC->SelectObject(pOldFont);
      }
      break;

   case EDP_GRLDBOX:
      UpdateWatchpointArea (pWP, pDC);

	   if (!UpdateWatchpointVisibility (pClientData, drawArea))
	   {
		   return;
	   }
      break;
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::DrawActionInfos (CDC *pDC, CRect drawArea)
{
   CGrEditorDoc *pDoc;
   CList<CPrgEdAction *, CPrgEdAction *> actions;
   POSITION pPosition;
   
   pDoc = GetDocument ();
   if (pDoc == NULL)
   {
      return;
   }
   
   if (pDoc->AreAllActionsBlocked ())
   {
      pDoc->GetAllActions (actions);
   }
   else
   {
      pDoc->GetActions (actions, CPrgEditorDoc::ActionForcedOnce);
      pDoc->GetActions (actions, CPrgEditorDoc::ActionForcedPermanently);
      pDoc->GetActions (actions, CPrgEditorDoc::ActionBlocked);
   };
   
   pPosition = actions.GetHeadPosition ();
   while (pPosition != NULL)
   {
      CPrgEdAction *pAction;
      
      pAction = actions.GetNext (pPosition);
      ASSERT (pAction != NULL);
      if (pAction == NULL)
         continue;
      
      DrawActionInfo (pDC, drawArea, pAction);
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::DrawActionInfo (CDC *pDC, CRect drawArea, CPrgEdAction *pAction)
{
   CGrEditorDoc *pDoc;
   CString strText;
   CPoint  p;
   CElem  *pE;

   pDoc = GetDocument ();
   if (pDoc == NULL)
   {
      return;
   }
   
   p.x = pAction->GetX();
   p.y = pAction->GetY();

   pE = pDoc->GetElemList()->GetCurElem (p);
   ASSERT(pE);
   if (pE == NULL)
   {
      return;
   }

   p.x += pE->GetSize().cx;
  
   if (pDoc->AreAllActionsBlocked ())
   {
      strText = "blocked";
   }
   else if (pAction->IsForcedPermanently ())
   {
      strText = "forced permanently";
   }
   else if (pAction->IsForcedOnce ())
   {
      strText = "forced";
   }
   else if (pAction->IsBlocked ())
   {
      strText = "blocked";
   };
   
   DrawSFCInfo (pDC, drawArea, p, strText);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::DrawTransitionInfos (CDC *pDC, CRect drawArea)
{
   CGrEditorDoc *pDoc;
   CList<CPrgEdTransition *, CPrgEdTransition *> transitions;
   POSITION pPosition;
   
   pDoc = GetDocument ();
   if (pDoc == NULL)
   {
      return;
   }
   
   if (pDoc->AreAllTransitionsForced ())
   {
      pDoc->GetAllTransitions (transitions);
   }
   else
   {
      pDoc->GetTransitions (transitions, CPrgEditorDoc::TransitionForcedOnce);
      pDoc->GetTransitions (transitions, CPrgEditorDoc::TransitionForcedPermanently);
      pDoc->GetTransitions (transitions, CPrgEditorDoc::TransitionBlocked);
   }
   
   pPosition = transitions.GetHeadPosition ();
   while (pPosition != NULL)
   {
      CPrgEdTransition *pTransition;
      
      pTransition = transitions.GetNext (pPosition);
      ASSERT (pTransition != NULL);
      if (pTransition == NULL)
         continue;
      
      DrawTransitionInfo (pDC, drawArea, pTransition);
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::DrawTransitionInfo (CDC *pDC, CRect drawArea, CPrgEdTransition *pTransition)
{
   CGrEditorDoc *pDoc;
   CString strText;
   CPoint  p;
   CElem  *pE;
   
   pDoc = GetDocument ();
   if (pDoc == NULL)
   {
      return;
   }
   
   p.x = pTransition->GetX();
   p.y = pTransition->GetY();
   
   pE = pDoc->GetElemList()->GetCurElem (p);
   ASSERT(pE);
   if (pE == NULL)
   {
      return;
   }

   p.x += pE->GetSize().cx;
   p.y += 1;
  
   if (pDoc->AreAllTransitionsForced () || pTransition->IsForcedPermanently ())
   {
      strText = "forced permanently";
   }
   else if (pTransition->IsForcedOnce ())
   {
      strText = "forced";
   }
   else if (pTransition->IsBlocked ())
   {
      strText = "blocked";
   };
   
   DrawSFCInfo (pDC, drawArea, p, strText);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::DrawSFCInfo (CDC *pDC, CRect drawArea, 
   const CPoint &position, const CString &strText)
{
   CPen   *pOldPen;
   CBrush *pOldBrush;
	CRect   r;
   bool    bScreen = !pDC->IsPrinting();
   COLORREF OldTextCol;

   CGrEditorDoc *pDoc;
   CRect  rect;
   CFont *pOldFont=NULL;
   CSize  extent;
   CRect  clientRect;
   bool   bVisible;
   
   pDoc = GetDocument ();
   if (pDoc == NULL)
   {
      return;
   }

   if (m_bHaveWPFont)
   {
      pOldFont = pDC->SelectObject (&m_WatchpointFont);
   }
   
   extent = pDC->GetTextExtent (strText);
   extent.cx += 2 * WPHorizontalMargin;
   extent.cy += 2 * WPVerticalMargin;
   
   rect.left   = position.x * m_iZGrid;
   rect.top    = position.y * m_iZGrid;
   rect.right  = rect.left + extent.cx;
   rect.bottom = rect.top + extent.cy;
   
	if (m_DrawTarget == ActiveView)
	{
		GetClientRect (&clientRect);
	}
	else
	{
		clientRect = drawArea;
	}

   clientRect.OffsetRect( m_LastScrollPos );

	bVisible = rect.bottom > clientRect.top  && rect.top  < clientRect.bottom &&
	           rect.right  > clientRect.left && rect.left < clientRect.right;

   if (bVisible)
   {
/*
      CRect clipRect;
      
      WPAreaToClientArea (rect);
      
      clipRect = drawArea;
      clipRect.left += m_iMarkSpace;
      pDC->IntersectClipRect (&clipRect);
*/      
      pOldBrush  = pDC->SelectObject(GDIHelper::GetBrush(bScreen, BT_FBLKBK));
      pOldPen    = pDC->SelectObject(GDIHelper::GetPen  (bScreen, PE_GRID  )); 
      OldTextCol = pDC->SetTextColor(GDIHelper::GetColor(bScreen, CO_NORMAL));

      pDC->Rectangle (&rect);
      pDC->SetBkMode( TRANSPARENT );
	   pDC->TextOut (rect.left + WPHorizontalMargin, rect.top + WPVerticalMargin, strText);

      pDC->SetTextColor(OldTextCol);
      pDC->SelectObject(pOldPen); 
      pDC->SelectObject(pOldBrush);

//      ::SelectClipRgn (pDC->m_hDC, NULL);
   }

	if (m_bHaveWPFont && pOldFont)
	{
      pDC->SelectObject(pOldFont);
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
CWatchpoint *CDbgEditorView::FindWatchpointByPoint (CPoint point)
{
	CGrEditorDoc *pDoc;
	POSITION pPosition;
   unsigned int uiBestZ=0;
   CWatchpoint *pBestWP=NULL;
   
   CWatchpoint *pWP;
   CPrgWPClientData *pClientData;
   CRect    area;

	pDoc = GetDocument ();

   // search at cursor position ..
   pPosition = pDoc->GetFirstWPPosition ();
	while (pPosition != NULL)
	{
		pWP = pDoc->GetNextWP (pPosition);
		ASSERT (pWP != NULL);
		if (pWP == NULL)
			continue;

      pClientData = (CPrgWPClientData *)pWP->GetClientData ();
		ASSERT (pClientData != NULL);
		if (pClientData == NULL)
			continue;

		area = pClientData->GetArea ();

		if (point.x >= area.left && point.x < area.right &&
		    point.y >= area.top  && point.y < area.bottom)
      {
         unsigned int uiZ;
         
         uiZ = pClientData->GetZ ();
         
         if (pBestWP == NULL || uiZ < uiBestZ)
         {
            pBestWP = pWP;
            uiBestZ = uiZ;
         }
      }
	}
   
   return (pBestWP);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
bool CDbgEditorView::BuildBreakpoint (CStdEditorPosition& bp_position)
{
   CGrEditorDoc       *pDoc;
   CPoint  p;
   CRect   r;
   bool    bRes=false;

   pDoc = GetDocument();

   p = m_LogCurPos - m_LastScrollPos;
   GetClientRect(&r);

   if( (p.x < m_iDZGrid) && (p.y < m_iDZGrid) ) 
   {
      bp_position = CStdEditorPosition(EDP_GRBEGIN);
      bRes = true;
   } 
   else if( (p.x > r.right-m_iDZGrid) && (p.y > r.bottom-m_iDZGrid) ) 
   {
      bp_position = CStdEditorPosition(EDP_GREND);
      bRes = true;
   } 
   else 
   {
      CElem *pE;

      pE = pDoc->GetElemList()->SearchElem(m_LogCurPos, m_iZGrid);
      if( pE && 
          ((pE->GetType() == ET_FB         ) ||
           (pE->GetType() == ET_FUNCTION   ) ||
           (pE->GetType() == ET_PROCEDURE  ) ||
           (pE->GetType() == ET_RETURN     ) ||
           (pE->GetType() == ET_TRANSITION && !((CElemTran*)pE)->GetName().IsEmpty()) ||
           (pE->GetType() == ET_ACTION     ) ||
           (pE->GetType() == ET_VARIN      ) ||
           (pE->GetType() == ET_VAROUT     ) ||
           (pE->GetType() == ET_VARINOUT   ) ||
           (pE->GetType() == ET_CONTACT    ) ||
           (pE->GetType() == ET_COIL       ) ||
           (pE->GetType() == ET_ACTIONBLOCK && pE->GetSize().cy > 2)))
      {
         bp_position = CStdEditorPosition(EDP_GRBOX, pE->GetOrigin().x, pE->GetOrigin().y);
         bRes = true;
      }
      
      if (bRes == false)
      {
         pE = pDoc->GetElemList()->GetSelectElem();
         if( pE && 
             ((pE->GetType() == ET_FB         ) ||
              (pE->GetType() == ET_FUNCTION   ) ||
              (pE->GetType() == ET_PROCEDURE  ) ||
              (pE->GetType() == ET_JUMP       ) ||
              (pE->GetType() == ET_RETURN     ) ||
              (pE->GetType() == ET_TRANSITION  && !((CElemTran*)pE)->GetName().IsEmpty()) ||
              (pE->GetType() == ET_ACTION     ) ||
              (pE->GetType() == ET_VARIN      ) ||
              (pE->GetType() == ET_VAROUT     ) ||
              (pE->GetType() == ET_VARINOUT   ) ||
              (pE->GetType() == ET_CONTACT    ) ||
              (pE->GetType() == ET_COIL       ) ||
              (pE->GetType() == ET_ACTIONBLOCK && pE->GetSize().cy > 2)) )
         {
            bp_position = CStdEditorPosition(EDP_GRBOX, pE->GetOrigin().x, pE->GetOrigin().y);
            bRes = true;
         }
      }
   } 
   return bRes;
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::OnShowBreakpoint (CBreakpoint * pBP)
{
   CGrEditorDoc* pDoc = GetDocument();
   if (pDoc)
   {
	   if (pDoc->IsInPlaceActive ())
	   {
         if (pBP)
         {
            CPoint context = CPoint(pBP->GetPosition().GetX(), pBP->GetPosition().GetY());
            POSITION   pos = m_STViewMap.GetStartPosition();
            CSTEditorView *pView;
            UINT Key;

            while (pos)
            {
               m_STViewMap.GetNextAssoc(pos, Key, pView);
               if (context == pView->GetContext())
               {
                  CSECEditor* pEdit = pDoc->GetEditor(pView->GetEditID());
                  if (pEdit)
                  {
                     pEdit->ShowBreakpoint(pBP->GetPosition().GetCY (), true);
                  } else {
                     ASSERT(0);
                  }
                  break;
               }
            }
         }
		   Invalidate ();
		   UpdateWindow ();
	   }
	   else
	   {
		   COleServerItem *pItem = pDoc->GetEmbeddedItem ();
		   assert (pItem != NULL);
		   if (pItem != NULL)
		   {
			   pItem->NotifyChanged ();
		   }
      }
	}
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::OnHideBreakpoint (CBreakpoint * pBP)
{
   CGrEditorDoc* pDoc = GetDocument();
   if (pDoc)
   {
	   if (pDoc->IsInPlaceActive ())
	   {
         if (pBP)
         {
            CPoint context = CPoint(pBP->GetPosition().GetX(), pBP->GetPosition().GetY());
            POSITION   pos = m_STViewMap.GetStartPosition();
            CSTEditorView *pView;
            UINT Key;

            while (pos)
            {
               m_STViewMap.GetNextAssoc(pos, Key, pView);
               if (context == pView->GetContext())
               {
                  CSECEditor* pEdit = pDoc->GetEditor(pView->GetEditID());
                  if (pEdit)
                  {
                     pEdit->ShowBreakpoint(pBP->GetPosition().GetCY (), false);
                  } else {
                     ASSERT(0);
                  }
                  break;
               }
            }
         }
		   Invalidate ();
		   UpdateWindow ();
	   }
	   else
	   {
		   COleServerItem *pItem = pDoc->GetEmbeddedItem ();
		   assert (pItem != NULL);
		   if (pItem != NULL)
		   {
			   pItem->NotifyChanged ();
		   }
	   }
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::OnShowStoppoint ()
{
   m_bIsStopped = false;
   CStdEditorPosition position;

   CGrEditorDoc* pDoc = GetDocument();
   if (pDoc)
   {
	   if (pDoc->IsInPlaceActive ())
	   {
         if (pDoc->GetStopPosition (position))
         {
            CPoint context = CPoint(position.GetX(), position.GetY());
            POSITION   pos = m_STViewMap.GetStartPosition();
            CSTEditorView *pView;
            UINT Key;
            CRect r;

            while (pos)
            {
               m_STViewMap.GetNextAssoc(pos, Key, pView);
               if (context == pView->GetContext())
               {
                  CSECEditor* pEdit = pDoc->GetEditor(pView->GetEditID());
                  long l=position.GetCY();

                  if (pEdit)
                  {
                     if (l != -1)
                     {
                        pEdit->ShowStoppoint(l, true);
                        // update size in view for using CenterLine !!!
                        pView->GetClientRect(&r);
                        ::SendMessage(pView->m_hWnd, WM_SIZE, SIZE_RESTORED, 
                           MAKELONG(r.Width(), r.Height()));
                        pView->CenterLine(pEdit->GoToLineNo(position.GetCY()));
                     }
                  } else {
                     ASSERT(0);
                  }
                  break;
               }
            }
         }
		   Invalidate ();
		   UpdateWindow ();
	   }
	   else
	   {
		   COleServerItem *pItem = pDoc->GetEmbeddedItem ();
		   assert (pItem != NULL);
		   if (pItem != NULL)
		   {
			   pItem->NotifyChanged ();
		   }
	   }
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::OnHideStoppoint ()
{
   m_bIsStopped = true;
   CStdEditorPosition position;

   CGrEditorDoc* pDoc = GetDocument();
   if (pDoc)
   {
	   if (pDoc->IsInPlaceActive ())
	   {
         if (pDoc->GetStopPosition (position))
         {
            CPoint context = CPoint(position.GetX(), position.GetY());
            POSITION   pos = m_STViewMap.GetStartPosition();
            CSTEditorView *pView;
            UINT Key;

            while (pos)
            {
               m_STViewMap.GetNextAssoc(pos, Key, pView);
               if (context == pView->GetContext())
               {
                  CSECEditor* pEdit = pDoc->GetEditor(pView->GetEditID());
                  if (pEdit)
                  {
                     pEdit->ShowStoppoint(position.GetCY(), false);
                  } else {
                     ASSERT(0);
                  }
                  break;
               }
            }
         }
		   Invalidate ();
		   UpdateWindow ();
	   }
	   else
	   {
		   COleServerItem *pItem = pDoc->GetEmbeddedItem ();
		   assert (pItem != NULL);
		   if (pItem != NULL)
		   {
			   pItem->NotifyChanged ();
		   }
	   }
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::OnManualModeToggled ()
{
   m_bRedrawAfterSFCUpdate = true;
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::OnStepAttributeChanged (CPrgEdStep *pStep)
{
   long lY;
   
   lY = pStep->GetY ();
   
/*SFC*/
//    if (lY >= m_iFirstLineNo && lY <= LastLineNo ())
   {
      m_bRedrawAfterSFCUpdate = true;
   };
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::OnActionAttributeChanged (CPrgEdAction *pAction)
{
   long lY;
   
   lY = pAction->GetY ();
   
/*SFC*/
//    if (lY >= m_iFirstLineNo && lY <= LastLineNo ())
   {
      m_bRedrawAfterSFCUpdate = true;
   };
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::OnAllActionsOffToggled ()
{
   m_bRedrawAfterSFCUpdate = true;
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::OnTransitionAttributeChanged (CPrgEdTransition *pTransition)
{
   long lY;
   
   lY = pTransition->GetY ();
   
/*SFC*/
//   if (lY >= m_iFirstLineNo && lY <= LastLineNo ())
   {
      m_bRedrawAfterSFCUpdate = true;
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::OnAllTransitionsOnToggled ()
{
    m_bRedrawAfterSFCUpdate = true;
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::OnEndSFCUpdate ()
{
   if (!m_bRedrawAfterSFCUpdate)
   {
      return;
   }
   
   m_bRedrawAfterSFCUpdate = false;
   
   if (GetDocument ()->IsInPlaceActive ())
   {
      Invalidate ();
      UpdateWindow ();
   }
   else
   {
      COleServerItem *pItem;
      
      pItem = GetDocument ()->GetEmbeddedItem ();
      ASSERT (pItem != NULL);
      if (pItem != NULL)
      {
         pItem->NotifyChanged ();
      }
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::OnSetWatchpoint ()
{
	CGrEditorDoc *pDoc;
	HRESULT hr;

	pDoc = GetDocument ();

	if (pDoc)
   {
      CPoint   gridPos;
      CElem   *pE;
      CRect    r;

      gridPos = m_CurPos;

      pE = pDoc->GetElemList()->SearchElem (m_LogCurPos, m_iZGrid);
      if (pE == NULL)
      {
         pE = pDoc->GetElemList()->GetSelectElem();
         if (pE)
         {
            pE->GetRect(r, m_iZGrid);
            DocToGrid(r);
            gridPos = r.CenterPoint();
         }
      } else {
         if (pE->SearchTextRect(m_LogCurPos, r, m_iZGrid))
         {
            DocToGrid(r);
            gridPos = r.CenterPoint();
         }
      }
      pDoc->SetLastActiveView(this);
      
      CStdEditorPosition pos(EDP_GRBOX, gridPos.x, gridPos.y );
      hr = pDoc->SetWatchpoint (pos, true);
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::OnClearWatchpoint ()
{
   CGrEditorDoc *pDoc;
	CWatchpoint  *pWP;
   HRESULT hr;

   pDoc = GetDocument();

   if (pDoc)
   {
	   pWP = FindWatchpointByPoint( m_LogCurPos );

	   if (pWP && 
          pWP->GetPosition().GetPosQual() != EDP_GRLDBOX)
      {
         hr = pDoc->ClearWatchpoint( pWP );
      }
	}
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::OnSetLadderWatchpoints ()
{
	CGrEditorDoc *pDoc = GetDocument ();
	HRESULT hr;

	if (pDoc)
   {
      CElemPtrList *pL = pDoc->GetElemList();
      POSITION pos = pL->GetHeadPosition();
      CPoint   gridPos;
      CRect    r;

      while(pos)
      {
         CElem* pE = (CElem*) pL->GetNext(pos);
         if (pE &&
            (pE->GetType() == ET_CONTACT ||
             pE->GetType() == ET_COIL))
         {
            pE->GetTextRect(0, r, 1);
            gridPos = r.CenterPoint();

            pDoc->SetLastActiveView(this);

            CStdEditorPosition edipos(EDP_GRLDBOX, gridPos.x, gridPos.y , -1, -1);
            hr = pDoc->SetWatchpoint (edipos, false);
         }
      }
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::OnClearLadderWatchpoints ()
{
   CGrEditorDoc *pDoc = GetDocument();
   CWatchpoint  *pWP;
   HRESULT hr;

   if (pDoc)
   {
      POSITION pos;
      
      pos = pDoc->GetFirstWPPosition ();
      while (pos)
      {
         pWP  = pDoc->GetNextWP (pos);
         if (pWP && 
             pWP->GetPosition().GetPosQual() == EDP_GRLDBOX)
         {
            hr = pDoc->ClearWatchpoint( pWP );
         }
      }
	}
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::OnClearAllWatchpoints ()
{
   CGrEditorDoc *pDoc;

	pDoc = GetDocument ();
	if (pDoc == NULL)
		return;

	pDoc->ClearAllWatchpoints ();
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out: true  - Contact, Coil found (and toggled or not)
       false - no Ladder Elem -> switch to Edit Mode ?
-----------------------------------------------------------------------------*/
bool CDbgEditorView::OnToggleLadder ()
{
   CGrEditorDoc *pDoc = GetDocument ();
   if (pDoc == NULL)
   {
      return false;
   }
   
   HRESULT hr;
   long    lHandle;
   CString strName;
   CString strValue;
   CStdEditorPosition startPosition;
   CStdEditorPosition endPosition;
   CEExpressionState state;
   CComVariant varValue;
   BL_IEC_TYP  iecType;
   CElem      *pE;
   CPoint      p;

   USES_CONVERSION;

   pE = pDoc->GetElemList()->SearchElem (m_LogCurPos, m_iZGrid);
   if (pE == NULL || 
       !((pE->GetType() == ET_CONTACT) || (pE->GetType() == ET_COIL)))
   {
//    ::MessageBeep (0xFFFFFFFF);
      return false;
   }
   // p points to contact/coil text and user can click at entire contact/coil
   p = pE->GetOrigin();
   p.x += pE->GetSize().cx / 2;
   p.y += 1;
   p = GridToClient(p);
   
   strName = GetContextExpression (p, startPosition, endPosition);
   if (strName.IsEmpty ())
   {
      ::MessageBeep (0xFFFFFFFF);
      return true;
   }
   
   hr = pDoc->AddWatchItem (strName, lHandle);  // writes lHandle
   if (hr != S_OK)
   {
      ::MessageBeep (0xFFFFFFFF);
      return true;
   }
   ASSERT (lHandle != 0);
   if (lHandle == 0)
      return true;
   
   hr = pDoc->ConnectWatchItem (lHandle);
   if (hr != S_OK)
   {
      ::MessageBeep (0xFFFFFFFF);
      pDoc->RemoveWatchItem (lHandle);
      return true;
   }
   
   state = pDoc->GetWatchExpressionState (lHandle);
   
   if (state == CEUnknownSubscription || 
       state == CENotConnected || 
       state == CELoadingValue || 
       state == CEEmptyValue)
   {
      pDoc->DisconnectWatchItem (lHandle);
      pDoc->RemoveWatchItem (lHandle);
      ::MessageBeep (0xFFFFFFFF);
      return true;
   };
   
   if (state == CEGoodExpression || state == CEReloadingValue)
   {
      strValue = pDoc->GetWatchExpressionValue (lHandle);
   };
   
   if (pDoc->IsSimpleWatchItemType (lHandle, iecType))
   {
      if (iecType == BL_LREAL && !strValue.IsEmpty()
         && strValue.Find(_T('.'))<0)
         strValue += _T(".0");
      hr = BL_StringToVariant (T2COLE (strValue), iecType, NULL, BL_USE_IEC_FORMAT, &varValue);
      if (FAILED (hr)) {
         AfxMessageBox (IDS_GR_WRONG_VALUE_SYNTAX);
         return true;
      }
   }
   else
   {
      varValue = strValue;
   };
   
   if (varValue.vt == VT_BOOL)
      varValue.boolVal = !varValue.boolVal;
   
   hr = pDoc->WriteWatchItem (lHandle, varValue);
   if (FAILED (hr))
   {
      pDoc->DisconnectWatchItem (lHandle);
      pDoc->RemoveWatchItem (lHandle);
      AfxMessageBox (IDS_GR_CANNOT_WRITE_VAR);
      return true;
   };
   
   pDoc->DisconnectWatchItem (lHandle);
   pDoc->RemoveWatchItem (lHandle);

   return true;
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::OnWriteWatchpoint ()
{
   CGrEditorDoc *pDoc;
   CWatchpoint *pWP;
   CString strExpression;
   CString strValue;
//   CElem *pE;
   CStdEditorPosition position;
   
   pDoc = GetDocument ();
   if (pDoc == NULL)
   {
      return;
   }
   
   pWP = FindWatchpointByPoint (m_LogCurPos);
   
   if (pWP != NULL)
   {
      CPrgWPClientData *pClientData;
      
      pClientData = (CPrgWPClientData *)pWP->GetClientData ();
      if (pClientData == NULL)
      {
         return;
      };
      strExpression = pClientData->GetVariableName ();
      
      strValue = pDoc->GetWatchExpressionValue (pWP->GetWatchItem ());
   }
   else
   {
      HRESULT hr = S_FALSE;
      CStdEditorPosition startPosition;
      CStdEditorPosition endPosition;
      CStdEditorPosition context;
      long lHandle;
      CEExpressionState state;
      CPoint  p = DocToClient(m_LogCurPos);
		CRect r;
      
      strExpression = GetContextExpression (p, startPosition, endPosition);
		if (!strExpression.IsEmpty())
         hr = pDoc->AddWatchItem (strExpression, lHandle);  // writes lHandle
      if (hr != S_OK)
      {
         CElem *pE = pDoc->GetElemList()->GetSelectElem();
         if (pE)
         {
            pE->GetRect(r, m_iZGrid);
            DocToClient(r);
            p = r.CenterPoint();

				strExpression = GetContextExpression (p, startPosition, endPosition);
				if (!strExpression.IsEmpty())
		    		hr = pDoc->AddWatchItem (strExpression, lHandle);  // writes lHandle
         }
      }
		if (hr != S_OK)
		{
			::MessageBeep (0xFFFFFFFF);
			return;
		}
      
      hr = pDoc->ConnectWatchItem (lHandle);
      if (hr != S_OK)
      {
         ::MessageBeep (0xFFFFFFFF);
         pDoc->RemoveWatchItem (lHandle);
         return;
      };
      
      state = pDoc->GetWatchExpressionState (lHandle);
      
      if (state == CEUnknownSubscription || 
          state == CENotConnected || 
          state == CEEmptyValue)
      {
         pDoc->DisconnectWatchItem (lHandle);
         pDoc->RemoveWatchItem (lHandle);
         ::MessageBeep (0xFFFFFFFF);
         return;
      };
      
      if (state == CEGoodExpression || state == CEReloadingValue)
      {
         strValue = pDoc->GetWatchExpressionValue (lHandle);
      };
      
      pDoc->DisconnectWatchItem (lHandle);
      pDoc->RemoveWatchItem (lHandle);
   };
   
   CGrEditVarDlg dialog (pDoc, strExpression, strValue, this);
   
   dialog.DoModal ();
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::OnToggleBreakpoint ()
{
    CStdEditorPosition bp_position;
	CGrEditorDoc *pDoc;
	HRESULT       hr;

	pDoc = GetDocument ();
	if (pDoc == NULL)
		return;
   ASSERT( pDoc->GetEditMode() == EM_MONITOR );

   if( !BuildBreakpoint (bp_position) )
   {
	   pDoc->SystemMessage (E_CE_GR_NO_VALID_BREAKPOINT_POSITION);
      return;
   } 
   
   if (pDoc->LookupBreakpoint (bp_position) == NULL)
	{
      hr = pDoc->SetBreakpoint (bp_position);

	   if (FAILED (hr))
	   {
		   if (hr == E_CE_BPMAN_INVALID_BREAKPOINT_POSITION)
		   {
			   pDoc->SystemMessage (E_CE_GR_NO_VALID_BREAKPOINT_POSITION);
		   }
		   else
		   {
			   pDoc->SystemMessage (E_CE_GR_CANNOT_SET_BREAKPOINT);
		   }
	   }
	}
	else
	{
      hr = pDoc->ClearBreakpoint (bp_position);

      if (FAILED (hr))
	   {
		   pDoc->SystemMessage (E_CE_GR_CANNOT_CLEAR_BREAKPOINT);
	   }
	}
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::OnRemoveAllBreakpoints ()
{
   CGrEditorDoc *pDoc;
   HRESULT hr;
   
   pDoc = GetDocument ();
   if (pDoc == NULL)
      return;
   
   hr = pDoc->ClearInstanceBreakpoints ();
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::OnToggleBreakpointEnabled ()
{
   CStdEditorPosition bp_position;
   CGrEditorDoc *pDoc;
   HRESULT hr;
   CBreakpoint *pBP;
   
   pDoc = GetDocument ();
   if (pDoc == NULL)
      return;
   ASSERT( pDoc->GetEditMode() == EM_MONITOR );
   
   if( !BuildBreakpoint (bp_position) )
   {
      pDoc->SystemMessage (E_CE_GR_NO_VALID_BREAKPOINT_POSITION);
      return;
   } 
   
   pBP = pDoc->LookupBreakpoint (bp_position);
   if (pBP == NULL)
   {
      MessageBeep (0xFFFFFFFF);
      return;
   }
   
   if (pBP->IsActive ())
   {
      hr = pDoc->DeactivateBreakpoint (bp_position);
      
      if (FAILED (hr))
      {
         pDoc->SystemMessage (E_CE_GR_CANNOT_DEACTIVATE_BREAKPOINT);
      }
   }
   else
   {
      hr = pDoc->ActivateBreakpoint (bp_position);
      
      if (FAILED (hr))
      {
         if (hr == E_CE_BPMAN_INVALID_BREAKPOINT_POSITION)
         {
            pDoc->SystemMessage (E_CE_GR_INVALID_BREAKPOINT_POSITION);
         }
         else
         {
            pDoc->SystemMessage (E_CE_GR_CANNOT_ACTIVATE_BREAKPOINT);
         }
      }
   }
   
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::OnUpdateMonitor (CCmdUI *pCmdUI)
{
   CGrEditorDoc *pDoc;
   
   pDoc = GetDocument ();
   if (pDoc == NULL)
   {
      pCmdUI->Enable (FALSE);
      return;
   };
   
   if (!pDoc->CanMonitor ())
   {
      pCmdUI->Enable (FALSE);
      return;
   }
   
   switch (pCmdUI->m_nID) 
   {
   case ID_GR_CLEAR_ALL_WATCHPOINTS:
      pCmdUI->Enable (pDoc->GetFirstWPPosition () != NULL);
      break;
      
   default:
      pCmdUI->Enable (TRUE);
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::OnUpdateDebug (CCmdUI *pCmdUI)
{
   CGrEditorDoc *pDoc;
   
   pDoc = GetDocument ();
   if (pDoc == NULL)
   {
      pCmdUI->Enable (FALSE);
      return;
   }
   
   if (!pDoc->CanDebug ())
   {
      pCmdUI->Enable (FALSE);
      return;
   }
   
   switch (pCmdUI->m_nID)
   {
   case ID_GR_TOGGLE_BREAKPOINT:
      {
         CStdEditorPosition position;
         
         pCmdUI->Enable ( BuildBreakpoint (position) );
      }
      break;
      
   case ID_GR_TOGGLE_BREAKPOINT_ENABLED:
      {
         CStdEditorPosition position;
         
         pCmdUI->Enable ( BuildBreakpoint (position) && 
            (pDoc->LookupBreakpoint (position) != NULL));
      }
      break;
      
   case ID_GR_REMOVE_ALL_BREAKPOINTS:
      pCmdUI->Enable (pDoc->GetFirstBPPosition () != NULL);
      break;
      
   default:
      pCmdUI->Enable (TRUE);
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::OnContinueTask ()
{
   CGrEditorDoc *pDoc;
   HRESULT hr;
   
   pDoc = GetDocument ();
   if (pDoc == NULL)
      return;
   
   hr = pDoc->ContinueTask ();
   if (FAILED (hr))
   {
      AfxMessageBox (IDS_GR_CANNOT_CONTINUE_TASK);
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::OnStepInto ()
{ 
   CGrEditorDoc *pDoc;
   HRESULT hr;
   
   pDoc = GetDocument ();
   if (pDoc == NULL)
      return;
   
   hr = pDoc->StepTask (1);
   if (FAILED (hr))
   {
      AfxMessageBox (IDS_GR_CANNOT_STEP_INTO);
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::OnStepOver ()
{ 
   CGrEditorDoc *pDoc;
   HRESULT hr;
   
   pDoc = GetDocument ();
   if (pDoc == NULL)
      return;
   
   hr = pDoc->StepTask (2);
   if (FAILED (hr))
   {
      AfxMessageBox (IDS_GR_CANNOT_STEP_OVER);
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::OnStepOut ()
{ 
   CGrEditorDoc *pDoc;
   HRESULT hr;
   
   pDoc = GetDocument ();
   if (pDoc == NULL)
      return;
   
   hr = pDoc->StepTask (3);
   if (FAILED (hr))
   {
      AfxMessageBox (IDS_GR_CANNOT_STEP_OUT);
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::OnToggleManualMode ()
{
   HRESULT hr;
   CGrEditorDoc *pDoc;
   bool bInManualMode;
   
   pDoc = GetDocument ();
   if (pDoc == NULL)
   {
      return;
   }
   
   if (!pDoc->InManualMode (bInManualMode))
   {
      return;
   }
   
   hr = pDoc->SetManualMode (!bInManualMode);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::OnPerformCycle ()
{
   HRESULT hr;
   CGrEditorDoc *pDoc;
   bool bInManualMode;
   bool bInCycle;
   
   pDoc = GetDocument ();
   if (pDoc == NULL)
   {
      return;
   }
   
   if (!pDoc->InManualMode (bInManualMode))
   {
      AfxMessageBox (IDS_GR_CANNOT_PERFORM_CYCLE);
      return;
   }
   
   if (!bInManualMode)
   {
      AfxMessageBox (IDS_GR_CANNOT_PERFORM_CYCLE);
      return;
   }
   
   if (!pDoc->InSFCCycle (bInCycle))
   {
      AfxMessageBox (IDS_GR_CANNOT_PERFORM_CYCLE);
      return;
   }
   
   if (bInCycle)
   {
      AfxMessageBox (IDS_GR_CANNOT_PERFORM_CYCLE);
      return;
   }
   
   hr = pDoc->PerformSFCCycle ();
   if (FAILED (hr))
   {
      AfxMessageBox (IDS_GR_CANNOT_PERFORM_CYCLE);
      return;
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::OnForceTransitionOnce ()
{
   HRESULT hr;
   CGrEditorDoc *pDoc;
   CElem *pE;
   CPoint p;
   
   pDoc = GetDocument ();
   if (pDoc == NULL)
   {
      return;
   }
   
   pE = pDoc->GetElemList()->SearchElem(m_LogCurPos, m_iZGrid);
   if (pE)
   {
      p = pE->GetOrigin();
   } else {
      p = CPoint(-1, -1);
   }
   
   hr = pDoc->ForceTransitionOnce (!pDoc->IsTransitionForcedOnce (p.x, p.y), p.x, p.y);

   if (FAILED (hr))
   {
      AfxMessageBox (IDS_GR_CANNOT_FORCE_TRANSITION);
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::OnForceTransition ()
{
   HRESULT hr;
   HRESULT hrResult=S_OK;
   CGrEditorDoc *pDoc;
   CElem *pE;
   CPoint p;
   
   pDoc = GetDocument ();
   if (pDoc == NULL)
   {
      return;
   }
   
   pE = pDoc->GetElemList()->SearchElem(m_LogCurPos, m_iZGrid);
   if (pE)
   {
      p = pE->GetOrigin();
   } else {
      p = CPoint(-1, -1);
   }
   
   hr = pDoc->ForceTransitionOnce (false, p.x, p.y);
   if (FAILED (hr) && hrResult == S_OK)
   {
      hrResult = hr;
   };
   
   hr = pDoc->BlockTransition (false, p.x, p.y);
   if (FAILED (hr) && hrResult == S_OK)
   {
      hrResult = hr;
   };
   
   hr = pDoc->ForceTransition (!pDoc->IsTransitionForcedPermanently (p.x, p.y), p.x, p.y);
   if (FAILED (hr) && hrResult == S_OK)
   {
      hrResult = hr;
   };
   
   if (FAILED (hrResult))
   {
      AfxMessageBox (IDS_GR_CANNOT_FORCE_TRANSITION);
   };
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::OnBlockTransition ()
{
   HRESULT hr;
   HRESULT hrResult=S_OK;
   CGrEditorDoc *pDoc;
   CElem *pE;
   CPoint p;
   
   pDoc = GetDocument ();
   if (pDoc == NULL)
   {
      return;
   };
   
   pE = pDoc->GetElemList()->SearchElem(m_LogCurPos, m_iZGrid);
   if (pE)
   {
      p = pE->GetOrigin();
   } else {
      p = CPoint(-1, -1);
   }
   
   hr = pDoc->ForceTransitionOnce (false, p.x, p.y);
   if (FAILED (hr) && hrResult == S_OK)
   {
      hrResult = hr;
   };
   
   hr = pDoc->ForceTransition (false, p.x, p.y);
   if (FAILED (hr) && hrResult == S_OK)
   {
      hrResult = hr;
   };
   
   hr = pDoc->BlockTransition (!pDoc->IsTransitionBlocked (p.x, p.y), p.x, p.y);
   if (FAILED (hr) && hrResult == S_OK)
   {
      hrResult = hr;
   };
   
   if (FAILED (hrResult))
   {
      AfxMessageBox (IDS_GR_CANNOT_BLOCK_TRANSITION);
   };
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::OnForceAllTransitions ()
{
   HRESULT hr;
   HRESULT hrResult=S_OK;
   CGrEditorDoc *pDoc;
   
   pDoc = GetDocument ();
   if (pDoc == NULL)
   {
      return;
   }
   
   hr = pDoc->ForceAllTransitionsOnce (false);
   if (FAILED (hr) && hrResult == S_OK)
   {
      hrResult = hr;
   };
   
   hr = pDoc->BlockAllTransitions (false);
   if (FAILED (hr) && hrResult == S_OK)
   {
      hrResult = hr;
   };
   
   hr = pDoc->ForceAllTransitions (true);
   if (FAILED (hr) && hrResult == S_OK)
   {
      hrResult = hr;
   };
   
   if (FAILED (hrResult))
   {
      AfxMessageBox (IDS_GR_CANNOT_FORCE_TRANSITIONS);
   };
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::OnBlockAllTransitions ()
{
   HRESULT hr;
   HRESULT hrResult=S_OK;
   CGrEditorDoc *pDoc;
   
   pDoc = GetDocument ();
   if (pDoc == NULL)
   {
      return;
   };
   
   hr = pDoc->ForceAllTransitionsOnce (false);
   if (FAILED (hr) && hrResult == S_OK)
   {
      hrResult = hr;
   };
   
   hr = pDoc->ForceAllTransitions (false);
   if (FAILED (hr) && hrResult == S_OK)
   {
      hrResult = hr;
   };
   
   hr = pDoc->BlockAllTransitions (true);
   if (FAILED (hr) && hrResult == S_OK)
   {
      hrResult = hr;
   };
   
   if (FAILED (hrResult))
   {
      AfxMessageBox (IDS_GR_CANNOT_FORCE_TRANSITION);
   };
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::OnRevertAllTransitions ()
{
   HRESULT hr;
   HRESULT hrResult=S_OK;
   CGrEditorDoc *pDoc;
   
   pDoc = GetDocument ();
   if (pDoc == NULL)
   {
      return;
   };
   
   hr = pDoc->ForceAllTransitionsOnce (false);
   if (FAILED (hr) && hrResult == S_OK)
   {
      hrResult = hr;
   };
   
   hr = pDoc->ForceAllTransitions (false);
   if (FAILED (hr) && hrResult == S_OK)
   {
      hrResult = hr;
   };
   
   hr = pDoc->BlockAllTransitions (false);
   if (FAILED (hr) && hrResult == S_OK)
   {
      hrResult = hr;
   };
   
   if (FAILED (hrResult))
   {
      AfxMessageBox (IDS_GR_CANNOT_FORCE_TRANSITION);
   };
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::OnGloballyForceAllTransitions ()
{
   HRESULT hr;
   CGrEditorDoc *pDoc;
   
   pDoc = GetDocument ();
   if (pDoc == NULL)
   {
      return;
   };
   
   hr = pDoc->GloballyForceAllTransitions (!pDoc->AreAllTransitionsForcedGlobally ());
   
   if (FAILED (hr))
   {
      AfxMessageBox (IDS_GR_CANNOT_FORCE_TRANSITIONS);
   };
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::OnForceActionOnce ()
{
   HRESULT hr;
   CGrEditorDoc *pDoc;
   CElem *pE;
   CPoint p;
   
   pDoc = GetDocument ();
   if (pDoc == NULL)
   {
      return;
   }
   
   pE = pDoc->GetElemList()->SearchElem(m_LogCurPos, m_iZGrid);
   if (pE)
   {
      p = pE->GetOrigin();
   } else {
      p = CPoint(-1, -1);
   }
   
   hr = pDoc->ForceActionOnce (!pDoc->IsActionForcedOnce (p.x, p.y), p.x, p.y);
   if (FAILED (hr))
   {
      AfxMessageBox (IDS_GR_CANNOT_FORCE_ACTION);
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::OnForceAction ()
{
   HRESULT hr;
   HRESULT hrResult=S_OK;
   CGrEditorDoc *pDoc;
   CElem *pE;
   CPoint p;
   
   pDoc = GetDocument ();
   if (pDoc == NULL)
   {
      return;
   }
   
   pE = pDoc->GetElemList()->SearchElem(m_LogCurPos, m_iZGrid);
   if (pE)
   {
      p = pE->GetOrigin();
   } else {
      p = CPoint(-1, -1);
   }
   
   hr = pDoc->ForceActionOnce (false, p.x, p.y);
   if (FAILED (hr) && hrResult == S_OK)
   {
      hrResult = hr;
   };
   
   hr = pDoc->BlockAction (false, p.x, p.y);
   if (FAILED (hr) && hrResult == S_OK)
   {
      hrResult = hr;
   };
   
   hr = pDoc->ForceAction (!pDoc->IsActionForcedPermanently (p.x, p.y), p.x, p.y);
   if (FAILED (hr) && hrResult == S_OK)
   {
      hrResult = hr;
   };
   
   if (FAILED (hrResult))
   {
      AfxMessageBox (IDS_GR_CANNOT_FORCE_ACTION);
   };
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::OnBlockAction ()
{
   HRESULT hr;
   HRESULT hrResult=S_OK;
   CGrEditorDoc *pDoc;
   CElem *pE;
   CPoint p;
   
   pDoc = GetDocument ();
   if (pDoc == NULL)
   {
      return;
   }
   
   pE = pDoc->GetElemList()->SearchElem(m_LogCurPos, m_iZGrid);
   if (pE)
   {
      p = pE->GetOrigin();
   } else {
      p = CPoint(-1, -1);
   }
   
   hr = pDoc->ForceActionOnce (false, p.x, p.y);
   if (FAILED (hr) && hrResult == S_OK)
   {
      hrResult = hr;
   };
   
   hr = pDoc->ForceAction (false, p.x, p.y);
   if (FAILED (hr) && hrResult == S_OK)
   {
      hrResult = hr;
   };
   
   hr = pDoc->BlockAction (!pDoc->IsActionBlocked (p.x, p.y), p.x, p.y);
   if (FAILED (hr) && hrResult == S_OK)
   {
      hrResult = hr;
   };
   
   if (FAILED (hr))
   {
      AfxMessageBox (IDS_GR_CANNOT_BLOCK_ACTION);
   };
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::OnForceAllActions ()
{
   HRESULT hr;
   HRESULT hrResult=S_OK;
   CGrEditorDoc *pDoc;
   
   pDoc = GetDocument ();
   if (pDoc == NULL)
   {
      return;
   };
   
   hr = pDoc->ForceAllActionsOnce (false);
   if (FAILED (hr) && hrResult == S_OK)
   {
      hrResult = hr;
   };
   
   hr = pDoc->BlockAllActions (false);
   if (FAILED (hr) && hrResult == S_OK)
   {
      hrResult = hr;
   };
   
   hr = pDoc->ForceAllActions (true);
   if (FAILED (hr) && hrResult == S_OK)
   {
      hrResult = hr;
   };
   
   if (FAILED (hrResult))
   {
      AfxMessageBox (IDS_GR_CANNOT_FORCE_ACTION);
   };
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::OnBlockAllActions ()
{
   HRESULT hr;
   HRESULT hrResult=S_OK;
   CGrEditorDoc *pDoc;
   
   pDoc = GetDocument ();
   if (pDoc == NULL)
   {
      return;
   }
   
   hr = pDoc->ForceAllActionsOnce (false);
   if (FAILED (hr) && hrResult == S_OK)
   {
      hrResult = hr;
   };
   
   hr = pDoc->ForceAllActions (false);
   if (FAILED (hr) && hrResult == S_OK)
   {
      hrResult = hr;
   };
   
   hr = pDoc->BlockAllActions (true);
   if (FAILED (hr) && hrResult == S_OK)
   {
      hrResult = hr;
   };
   
   if (FAILED (hrResult))
   {
      AfxMessageBox (IDS_GR_CANNOT_FORCE_ACTION);
   };
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::OnRevertAllActions ()
{
   HRESULT hr;
   HRESULT hrResult=S_OK;
   CGrEditorDoc *pDoc;
   
   pDoc = GetDocument ();
   if (pDoc == NULL)
   {
      return;
   };
   
   hr = pDoc->ForceAllActionsOnce (false);
   if (FAILED (hr) && hrResult == S_OK)
   {
      hrResult = hr;
   };
   
   hr = pDoc->ForceAllActions (false);
   if (FAILED (hr) && hrResult == S_OK)
   {
      hrResult = hr;
   };
   
   hr = pDoc->BlockAllActions (false);
   if (FAILED (hr) && hrResult == S_OK)
   {
      hrResult = hr;
   };
   
   if (FAILED (hrResult))
   {
      AfxMessageBox (IDS_GR_CANNOT_FORCE_ACTION);
   };
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::OnGloballyBlockAllActions ()
{
   HRESULT hr;
   CGrEditorDoc *pDoc;
   
   pDoc = GetDocument ();
   if (pDoc == NULL)
   {
      return;
   };
   
   hr = pDoc->BlockAllActions (!pDoc->AreAllActionsBlockedGlobally ());
   
   if (FAILED (hr))
   {
      AfxMessageBox (IDS_GR_CANNOT_BLOCK_ACTIONS);
   };
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::OnUpdateSFCMenu (CCmdUI *pCmdUI)
{
    bool bEnabled;
    bool bChecked;

    GetSFCMenuProperties (pCmdUI->m_nID, bEnabled, bChecked);

    pCmdUI->Enable (bEnabled);
    pCmdUI->SetCheck (bChecked);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::GetSFCMenuProperties (UINT uiId, bool &bEnabled, bool &bChecked)
{
   CGrEditorDoc *pDoc;
   bool bInManualMode;
   bool bInCycle;
   CElem *pE;
   CPoint p;
   
   pDoc = GetDocument ();
   if (pDoc == NULL)
   {
      bEnabled = false;
      bChecked = false;
      return;
   }
   
   if (!pDoc->CanDebug () || !pDoc->IsConnected ())
   {
      bEnabled = false;
      bChecked = false;
      return;
   };

   if (pDoc->GetCurrLang() != EL_ALL)
   {
      bEnabled = false;
      bChecked = false;
      return;
   }

   pE = pDoc->GetElemList()->SearchElem(m_LogCurPos, m_iZGrid);
   if (pE)
   {
      p = pE->GetOrigin();
   } else {
      p = CPoint(-1, -1);
   }
   
   switch (uiId)
   {
   case ID_GR_TOGGLE_MANUAL_MODE:
      bEnabled = pDoc->InManualMode (bInManualMode);
      bChecked = bEnabled && bInManualMode;
      break;
      
   case ID_GR_PERFORM_CYCLE:
      bEnabled = pDoc->InManualMode (bInManualMode) && bInManualMode &&
                 pDoc->InSFCCycle (bInCycle) && !bInCycle;
      bChecked = false;
      break;
      
   case ID_GR_FORCE_TRANSITION_ONCE:
      bEnabled = true;
      bChecked = pDoc->IsTransitionForcedOnce (p.x, p.y);
      break;
      
   case ID_GR_FORCE_TRANSITION:
      bEnabled = true;
      bChecked = pDoc->IsTransitionForcedPermanently (p.x, p.y);
      break;
      
   case ID_GR_BLOCK_TRANSITION:
      bEnabled = true;
      bChecked = pDoc->IsTransitionBlocked (p.x, p.y);
      break;
      
   case ID_GR_FORCE_ALL_TRANSITIONS:
      bEnabled = true;
      bChecked = false;
      break;
      
   case ID_GR_BLOCK_ALL_TRANSITIONS:
      bEnabled = true;
      bChecked = false;
      break;
      
   case ID_GR_REVERT_ALL_TRANSITIONS:
      bEnabled = true;
      bChecked = false;
      break;
      
   case ID_GR_GLOBALLY_FORCE_ALL_TRANSITIONS:
      bEnabled = true;
      bChecked = pDoc->AreAllTransitionsForcedGlobally ();
      break;
      
   case ID_GR_FORCE_ACTION_ONCE:
      bEnabled = true;
      bChecked = pDoc->IsActionForcedOnce (p.x, p.y);
      break;
      
   case ID_GR_FORCE_ACTION:
      bEnabled = true;
      bChecked = pDoc->IsActionForcedPermanently (p.x, p.y);
      break;
      
   case ID_GR_BLOCK_ACTION:
      bEnabled = true;
      bChecked = pDoc->IsActionBlocked (p.x, p.y);
      break;
      
   case ID_GR_FORCE_ALL_ACTIONS:
      bEnabled = true;
      bChecked = false;
      break;
      
   case ID_GR_BLOCK_ALL_ACTIONS:
      bEnabled = true;
      bChecked = false;
      break;
      
   case ID_GR_REVERT_ALL_ACTIONS:
      bEnabled = true;
      bChecked = false;
      break;
      
   case ID_GR_GLOBALLY_BLOCK_ALL_ACTIONS:
      bEnabled = true;
      bChecked = pDoc->AreAllActionsBlockedGlobally ();
      break;
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
bool CDbgEditorView::IsLadderActive(CPoint p)
{
   CGrEditorDoc *pDoc;
   bool bActive=false;
   
   pDoc = GetDocument ();
   if (pDoc != NULL &&
       pDoc->IsModeMonitor ())
   {
      bActive = pDoc->IsLadderActive (p.x, p.y);
   }

   return bActive;
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
bool CDbgEditorView::IsStepActive(CPoint p)
{
   CGrEditorDoc *pDoc;
   bool bActive=false;
   
   pDoc = GetDocument ();
   if (pDoc != NULL &&
       pDoc->IsModeMonitor ())
   {
      bActive = pDoc->IsStepActive (p.x, p.y);
   }

   return bActive;
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
bool CDbgEditorView::IsActionActive(CPoint p)
{
   CGrEditorDoc *pDoc;
   bool bActive=false;
   
   pDoc = GetDocument ();
   if (pDoc != NULL &&
       pDoc->IsModeMonitor ())
   {
      bActive = pDoc->IsActionActive (p.x, p.y);
   }

   return bActive;
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
bool CDbgEditorView::IsActionForced(CPoint p)
{
   CGrEditorDoc *pDoc;
   bool bForced=false;
   
   pDoc = GetDocument ();
   if (pDoc != NULL &&
       pDoc->IsModeMonitor ())
   {
      bForced = pDoc->IsActionForced (p.x, p.y);
   }

   return bForced;
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
bool CDbgEditorView::IsActionBlocked(CPoint p)
{
   CGrEditorDoc *pDoc;
   bool bBlocked=false;
   
   pDoc = GetDocument ();
   if (pDoc != NULL &&
       pDoc->IsModeMonitor ())
   {
      bBlocked = pDoc->AreAllActionsBlocked () || pDoc->IsActionBlocked (p.x, p.y);
   }

   return bBlocked;
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
bool CDbgEditorView::IsTransitionForced (CPoint p)
{
   CGrEditorDoc *pDoc;
   bool bForced=false;
   
   pDoc = GetDocument ();
   if (pDoc != NULL &&
       pDoc->IsModeMonitor ())
   {
      bForced = pDoc->AreAllTransitionsForced () || pDoc->IsTransitionForced (p.x, p.y);
   }

   return bForced;
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
bool CDbgEditorView::IsTransitionBlocked(CPoint p)
{
   CGrEditorDoc *pDoc;
   bool bBlocked=false;
   
   pDoc = GetDocument ();
   if (pDoc != NULL &&
       pDoc->IsModeMonitor ())
   {
      bBlocked = pDoc->IsTransitionBlocked (p.x, p.y);
   }

   return bBlocked;
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
bool CDbgEditorView::IsManualMode()
{
   CGrEditorDoc *pDoc;
   bool   bInManualMode;

   pDoc = GetDocument ();
   if (pDoc)
   {
      return pDoc->InManualMode (bInManualMode) && bInManualMode;
   } else {
      return false;
   }
}


/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CDbgEditorView::WPAreaToClientArea (CRect& rect)
{
    rect.OffsetRect( -m_LastScrollPos );
}

/*func*------------------------------------------------------------------------
  for datatips - check point for string ..
  in :point    - client pos
  out:startPos - 
      endPos   - 
      CString  - 
-----------------------------------------------------------------------------*/
CString CDbgEditorView::GetContextExpression (CPoint point, 
                                              CStdEditorPosition &startPos, 
                                              CStdEditorPosition &endPos)
{
   CGrEditorDoc *pDoc;
   CString       strText;
   CStdEditorPosition   posDoc;

   pDoc = GetDocument ();
   if (pDoc == NULL)
   {
      return _T("");
   }

   point = ClientToDoc(point);
   posDoc   = CStdEditorPosition(EDP_GRBOX, point.x, point.y, -1, -1); 
   startPos = CStdEditorPosition(EDP_GRBOX, 0, 0, -1, -1);

   switch (pDoc->GetEditMode())
   {
      case EM_NORMAL:
         strText = pDoc->GetSmartTextGr(posDoc, startPos, endPos, true, m_iZGrid);
         break;

      case EM_MONITOR:
         strText = pDoc->GetSmartTextGr(posDoc, startPos, endPos, false, m_iZGrid);
         strText.TrimLeft (_T("\t "));
         strText.TrimRight(_T("\t "));
         break;
   }

   return strText;
}
