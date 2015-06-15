


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*


#include "StdAfx.h"
//#include "to_mem.h"
//#include "splitterTabWnd.h"
#include "GenEdFrame.h"
#include "GenEdDocTempl.h"
#include "GenEdView.h"
#include "GenEdDoc.h"
#include "CEMain.h"
#include "MsgRC.h"   // for indicator bitmap
#include "MsgFrame.h"   // for indicator bitmap definitions
#include "MainFrm.h"
#include "GenEdCont.h"
#include "TargetSet.h"
#include "CEProjMan.h"
#include "CESysDef.h"
#include "DbgInfo.h"
#include "SBLLexTree.h"
#include "utilif.h"

#include <assert.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CGenEdFrame

static const int DEFAULT_TAB_COUNT = 4;

#define CEM_UPDATE_TITLE (WM_APP + 0)

IMPLEMENT_DYNCREATE (CGenEdFrame, SECMDIChildWnd)

BEGIN_MESSAGE_MAP(CGenEdFrame, SECMDIChildWnd)
    //{{AFX_MSG_MAP(CGenEdFrame)
    ON_WM_NCACTIVATE ()
    ON_WM_NCDESTROY()
    ON_WM_SYSCOMMAND()
    ON_COMMAND(ID_GENED_ADDINSTANCE, OnAddInstance)
    ON_COMMAND(ID_GENED_REMOVEINSTANCE, OnRemoveInstance)
    ON_COMMAND(ID_GENED_REMOVEALL, OnRemoveAll)
    ON_UPDATE_COMMAND_UI(ID_GENED_REMOVEALL, OnUpdateRemoveAll)
    ON_UPDATE_COMMAND_UI(ID_GENED_REMOVEINSTANCE, OnUpdateRemoveInstance)
    ON_COMMAND(ID_GENED_EXPAND_TABS, OnExpandTabs)
    ON_COMMAND(ID_GENED_COLLAPSE_TABS, OnCollapseTabs)
    ON_UPDATE_COMMAND_UI(ID_GENED_EXPAND_TABS, OnUpdateExpandTabs)
    ON_UPDATE_COMMAND_UI(ID_GENED_COLLAPSE_TABS, OnUpdateCollapseTabs)
    ON_UPDATE_COMMAND_UI(ID_GENED_ADDINSTANCE, OnUpdateAddInstance)
    ON_COMMAND(ID_GENED_REFRESH_ALL, OnRefreshAll)
    ON_COMMAND(ID_GENED_PREVIOUS_TAB, OnPreviousTab)
    ON_COMMAND(ID_GENED_NEXT_TAB, OnNextTab)
    ON_COMMAND(ID_GENED_PREVIOUS_SERVER, OnPreviousServer)
    ON_COMMAND(ID_GENED_NEXT_SERVER, OnNextServer)
	ON_WM_DESTROY()
	ON_WM_CREATE()
    ON_WM_MOUSEACTIVATE()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
//    ON_MESSAGE (WM_SELCHANGED, OnSelChanged)   // user message of splittertabwnd
	ON_MESSAGE(TCM_TABSEL, OnTabSelect)
    ON_MESSAGE(WM_TAB_CONTEXTMENU, OnInstTabContextMenu)
    ON_MESSAGE(RECALCULATE_LAYOUT, OnRecalculateLayout)
    ON_MESSAGE(SET_FOCUS_TO_ACTIVE_CHILD_FRAME, OnSetFocusToActiveChildFrame)
    ON_MESSAGE(CEM_UPDATE_TITLE, OnUpdateTitle)
    ON_WM_MDIACTIVATE()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGenEdFrame construction/destruction

CGenEdFrame::CGenEdFrame ()
: 
    m_ConnectStateCB (this, OnConnectStateCB),
    m_ExclusiveSessionCB (this, OnExclusiveSessionCB)
{
    m_pGenEdWnd = NULL;
    m_pGenEdTabWnd = NULL;
    m_iSelectedTab = 0;
    m_iTabExpandLevel = 0;
    m_bTabbed = false;
    m_bInsertInitialTabs = true;
    m_bHaveAddTab = false;
    m_pContainer = NULL;
    m_bAutoMode = TRUE;
}


CGenEdFrame::~CGenEdFrame ()
{
    if (m_bTabbed)
    {
        CTargetSet *pTargetSet = CEGetTargetSet("Project Targets");

        if(pTargetSet)
        {
            POSITION pPosition;

            pPosition = pTargetSet->GetFirstTargetPosition();
            while (pPosition != NULL)
            {
                CTargetOnlBase *pTarget;

                pTarget = pTargetSet->GetNextTarget(pPosition);
                assert (pTarget != NULL);
                if (pTarget == NULL)
                    continue;

                pTarget->UnregisterConnectStateCB (&m_ConnectStateCB);

                CTargetOnl* pTarg = dynamic_cast<CTargetOnl*> (pTarget);
                if(pTarg)
                {
                    pTarg->UnregisterExclusiveSessionCB (&m_ExclusiveSessionCB);
                }
            }
        }
    }

    if(m_pGenEdWnd)
    {
        delete m_pGenEdWnd;
        m_pGenEdWnd = NULL;
    }
    if(m_pGenEdTabWnd)
    {
        delete m_pGenEdTabWnd;
        m_pGenEdTabWnd = NULL;
    }
}


//------------------------------------------------------------------*
/**
 * set pointer to editor container for frame and tab wnd
 *
 * @param           [in] pContainer: pointer to editor container
 *
*/
void CGenEdFrame::SetContainer(CGenEdCont* pContainer)
{
    m_pContainer = pContainer;
    if(m_pGenEdTabWnd)
    {
        m_pGenEdTabWnd->SetContainer(pContainer);
    }
}


BOOL CGenEdFrame::PreCreateWindow (CREATESTRUCT& cs)
{
    return (SECMDIChildWnd::PreCreateWindow (cs));
}


void CGenEdFrame::RegisterActionCB (FC_CALLBACK_BASE (GenEdFrameActionCB) *pActionCB)
{
    m_ActionCBs.AddTail (pActionCB);
}


void CGenEdFrame::UnregisterActionCB (FC_CALLBACK_BASE (GenEdFrameActionCB) *pActionCB)
{
    POSITION pPosition;

    pPosition = m_ActionCBs.Find (pActionCB);
    assert (pPosition != NULL);

    if (pPosition != NULL)
    {
        m_ActionCBs.RemoveAt (pPosition);
    }
}


/////////////////////////////////////////////////////////////////////////////
// CGenEdFrame diagnostics

#ifdef _DEBUG
void CGenEdFrame::AssertValid () const
{
    SECMDIChildWnd::AssertValid ();
}

void CGenEdFrame::Dump (CDumpContext& dc) const
{
    SECMDIChildWnd::Dump (dc);
}

#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CGenEdFrame message handlers

BOOL CGenEdFrame::UpdateTabs (E_ActivateAction eAction /*= eActivateClass*/)
{
    HRESULT hr;
    int iIndex;
    bool bRemovedCurrent=false;
    bool bActivateInstance = (eAction == eActivateInstance) ? true : false;

    if (!m_bTabbed)
    {
        return (TRUE);
    }

    hr = GetInstances ();
    if (FAILED (hr))
    {
	    return (FALSE);
    }

    // Delete all views that do no longer exist
    for (iIndex = m_arrInst.GetSize () - 1; iIndex >= 0; --iIndex)
    {
        if (!m_UntabbedInstances.getData (m_arrInst[iIndex]))
        {
            if (iIndex + 1 == (int)m_iSelectedTab)
            {
                bRemovedCurrent = true;
            }

            m_arrInst.RemoveAt (iIndex);
            RemoveTab(iIndex + 1);
        }
    }

    RemoveTabbedInstances ();

    if (m_bInsertInitialTabs)
    {
        ASSERT (m_arrInst.GetSize () == 0);

        m_bInsertInitialTabs = false;

        // Restore instance information from settings?
        if(!LoadInstanceInfo(bActivateInstance))
        {
            AddDefaultTabs();
        }
    }
    else
    {
        // if less than DEFAULT_TAB_COUNT instances tabbed -> add default tabs
        if(m_arrInst.GetSize() < DEFAULT_TAB_COUNT)
        {
            AddDefaultTabs();
        }
    }

    if (bRemovedCurrent || eAction == eActivateClass)
    {
        m_pGenEdTabWnd->ActivateTab(0);
    }

    UpdateAddTab();

    return (TRUE);
}



//------------------------------------------------------------------*
/**
 * load instance info.
 *
 * @param            [in] bActivateTab: activate instance tab?
 *                                      Default is true
 * @param            [in] pDocSettings: pointer to document settings
 *                                      if NULL, the method searches for the
 *                                      corresponding document settings itself.
 *                                      Default is NULL
 * @return           
 * @exception        -
 * @see              
*/
BOOL CGenEdFrame::LoadInstanceInfo(bool bActivateTab /*=true*/, CDocumentSettings* pDocSettings /*=NULL*/)
{
    int                 iActiveInstance;

    if(!pDocSettings)
    {
        CCEMAINApp* pApp = dynamic_cast<CCEMAINApp *> (AfxGetApp());
        ASSERT(pApp);
        CGenEdDoc*          pDoc;

        CSettings* pSettings = pApp->GetSettings();
        if(!pSettings)
        {
            return FALSE;
        }
        pDoc = GetCurrentEditorDoc();
        if(!pDoc)
        {
            return FALSE;
        }
        CString strDocFilePath = pDoc->GetPathName();
        pDocSettings = pSettings->m_containerSettings.FindDocument(strDocFilePath, FALSE);
        if(!pDocSettings)
        {
            return FALSE;
        }
    }
    iActiveInstance = pDocSettings->GetActiveInstance();

    int     iInst;
    int     iNumInstances = pDocSettings->GetNumberOfInstances();
    CString strInstance;
    CInstanceSettings* pInstSettings;
    CArray<CInstanceSettings*, CInstanceSettings*>  tInstSetArray;
    POSITION    pos;

    try
    {
        tInstSetArray.SetSize(iNumInstances);
    }
    catch(CMemoryException* pE)
    {
        pE->Delete();
        return FALSE;
    }

    pos = pDocSettings->GetInstanceStartPosition();
    while(pos)
    {
        pDocSettings->GetNextInstance(pos, strInstance, pInstSettings);
        if(pInstSettings)
        {
            iInst = pInstSettings->GetIndex();
            if(iInst >= 0 && iInst < iNumInstances)
            {
                tInstSetArray[iInst] = pInstSettings;
            }
        }
    }
    int iInstInsert = 0;
    for(iInst = 0; iInst < iNumInstances; ++iInst)
    {
        pInstSettings = tInstSetArray[iInst];
        if(pInstSettings)
        {
            strInstance = pInstSettings->GetInstancePath();
            // only, if this instance is existing
            if(m_UntabbedInstances.getData(strInstance))
            {
                AddInstanceTab(iInstInsert++, strInstance, false);
            }
        }
    }
    // activate tab
    if(bActivateTab)
    {
        // if active instance is valid -> activate corresponding tab
        if(iActiveInstance < iInstInsert)
        {
            m_pGenEdTabWnd->ActivateTab(iActiveInstance + 1);
        }
        // else activate class tab.
        else
        {
            m_pGenEdTabWnd->ActivateTab(0);
        }
    }
    return TRUE;
}


BOOL CGenEdFrame::SaveInstanceInfo(CDocumentSettings* pDocSettings /*=NULL*/, BOOL bForce /*=FALSE*/)
{
    int                 iActiveInstance = -1;

    // if auto mode and not force -> do not save instance info
    // instances are added automatically then
    if(m_bAutoMode && !bForce)
    {
        return FALSE;
    }

    // get document settings if not present
    if(!pDocSettings)
    {
        CCEMAINApp* pApp = dynamic_cast<CCEMAINApp *> (AfxGetApp());
        ASSERT(pApp);
        CGenEdDoc*          pDoc;

        CSettings* pSettings = pApp->GetSettings();
        if(!pSettings)
        {
            return FALSE;
        }
        pDoc = GetCurrentEditorDoc();
        if(!pDoc)
        {
            return FALSE;
        }
        CString strDocFilePath = pDoc->GetPathName();
        pDocSettings = pSettings->m_containerSettings.FindDocument(strDocFilePath, TRUE);
        if(!pDocSettings)
        {
            return FALSE;
        }
    }

    // if tab window exists -> store instance info
    if(m_pGenEdTabWnd)
    {
        // reset index info
        POSITION    pos;
        CString     strInstPath;
        CInstanceSettings* pInstSettings;

        pos = pDocSettings->GetInstanceStartPosition();
        while(pos)
        {
            pDocSettings->GetNextInstance(pos, strInstPath, pInstSettings);
            pInstSettings->SetIndex(-1);
        }


        // set info for currently visible instances
        m_pGenEdTabWnd->GetActiveTab(iActiveInstance);
        --iActiveInstance;

        pDocSettings->SetActiveInstance(iActiveInstance);

        int iNumInst = m_arrInst.GetSize();
        for(int iInst = 0; iInst < iNumInst; ++iInst)
        {
            pInstSettings = pDocSettings->FindInstance(m_arrInst[iInst]);
            pInstSettings->SetIndex(iInst);
        }

        // remove not visible instances
        CStringArray    astrInstToDelete;
        pos = pDocSettings->GetInstanceStartPosition();
        while(pos)
        {
            pDocSettings->GetNextInstance(pos, strInstPath, pInstSettings);
            if(pInstSettings->GetIndex() == -1)
            {
                astrInstToDelete.Add(strInstPath);
            }
        }
        iNumInst = astrInstToDelete.GetSize();
        for(iInst = 0; iInst < iNumInst; ++iInst)
        {
            pDocSettings->DeleteInstance(astrInstToDelete[iInst]);
        }
    }

    return TRUE;
}

void CGenEdFrame::AddDefaultTabs()
{
    CStringArray    astrInstances;
    m_UntabbedInstances.getAllStringsFromSubstring("", astrInstances, DEFAULT_TAB_COUNT);
    int iNumInstances = astrInstances.GetSize();
    int iInsertPos;
    int iNumTabs;
    for (int iIndex = 0; iIndex < iNumInstances; ++iIndex)
    {
        CString strInstance = astrInstances[iIndex];
        CTargetOnlBase* pTarget;
        iNumTabs = m_pGenEdTabWnd->GetTabCount();
        iInsertPos = m_bHaveAddTab ? iNumTabs - 1 : iNumTabs;
        ASSERT(iInsertPos >= 0);

        m_arrInst.Add (strInstance);
        InsertTab(RUNTIME_CLASS(CGenEdView), iInsertPos, GetInstanceTabName(iIndex), &m_tCreateContext);
        m_UntabbedInstances.deleteData(strInstance);

        pTarget = GetMachineFromInstance (strInstance);

        if (pTarget != NULL)
        {
            HICON   hIcon = NULL;
            int     iImage = -1;

            GetConnectImage(pTarget->GetConnectState(), iImage);
            if(iImage >= 0)
            {
                hIcon = m_ImageList.ExtractIcon(iImage);
            }
            m_pGenEdTabWnd->SetTabIcon(m_arrInst.GetSize (), hIcon);
        }
    }
}


//------------------------------------------------------------------*
/**
 * on create client.
 *
 *  create tabbed or untabbed window depending on create context
 *
*/
BOOL CGenEdFrame::OnCreateClient (LPCREATESTRUCT lpcs, CCreateContext *pContext) 
{
    CString strClass;
    CString strAdd;

    // get create info -> tabbed or untabbed?
    assert (pContext != NULL);
    m_tCreateContext = *pContext;
    if (pContext != NULL && pContext->m_pNewDocTemplate != NULL)
    {
        CGenEdDocTemplate *pTemplate;

        pTemplate = dynamic_cast<CGenEdDocTemplate *> (pContext->m_pNewDocTemplate);
        assert (pTemplate != NULL);

        if (pTemplate != NULL)
        {
            m_bTabbed = pTemplate->IsTabbed ();
        }
    }

    // create untabbed window
    if (!m_bTabbed)
    {
        CRect rect;
        CSize size;
        CSize viewSize;
        return (SECMDIChildWnd::OnCreateClient (lpcs, pContext));
    }

    // get container -> referenced in tab window
    m_pContainer = CEGetContainer();
    ASSERT(m_pContainer);

    // create tab window
    m_pGenEdTabWnd = new CGenEdTabWnd;
    m_pGenEdTabWnd->SetContainer(m_pContainer);
    m_pGenEdTabWnd->Create(this , WS_CHILD | WS_VISIBLE | TWS_FULLSCROLL | TWS_TABS_ON_BOTTOM
                    | TWS_NOACTIVE_TAB_ENLARGED | TWS_DYNAMIC_ARRANGE_TABS );

    // add class tab
    strClass.LoadString(IDS_CLASS);
    m_pGenEdTabWnd->AddTab(RUNTIME_CLASS(CGenEdView), strClass, &m_tCreateContext);
    m_pGenEdTabWnd->ActivateTab(0);
    m_pGenEdTabWnd->EnableTips(this);
    m_ImageList.Create(IDB_MSG_INDICATORS_NEW, INDICATOR_BITMAP_WIDTH, 0, INDICATOR_TRANSPARENT_COLOR);

    // register session callbacks for targets
    CTargetSet* pTargetSet = CEGetTargetSet("Project Targets");
    if(pTargetSet)
    {
        POSITION pPosition;

        pPosition = pTargetSet->GetFirstTargetPosition ();
        while (pPosition != NULL)
        {
            CTargetOnlBase *pTarget;

            pTarget = pTargetSet->GetNextTarget (pPosition);
            assert (pTarget != NULL);
            if (pTarget == NULL)
                continue;

            pTarget->RegisterConnectStateCB (&m_ConnectStateCB);
            CTargetOnl* pTarg = dynamic_cast<CTargetOnl*> (pTarget);
            if(pTarg)
            {
                pTarg->RegisterExclusiveSessionCB (&m_ExclusiveSessionCB);
            }
        }
    }

    return (TRUE);
}


void CGenEdFrame::OnMDIActivate (BOOL bActivate, CWnd *pActivateWnd, CWnd *pDeactivateWnd)
{
    CRect rect;

    // During deactivation of an MDI frame an inplace active editor window
    // in this frame becomes UI inactive and decommits all of its requested
    // border space. Since we do not want toolbars to disappear in case of
    // deactivation we remember the amount of committed border space before
    // calling the base class (which actually deactivates the frame) and
    // again request this amount of border space after deactivation. Fortu-
    // nately the toolbars have not been hidden explicitly in deactivation
    // process. So as soon as the border space is allocated they reappear
    // magically.

    if (!bActivate)
    {
        rect = m_rectBorder;
    }

    SECMDIChildWnd::OnMDIActivate (bActivate, pActivateWnd, pDeactivateWnd);

    if (!bActivate)
    {
        NegotiateBorderSpace (borderSet, &rect);
    }
}


void CGenEdFrame::GetServerViews(CArray<CGenEdView *, CGenEdView *>& rapServerViews)
{
    int iViewCount;
    int i;
    CGenEdView *pView;

    iViewCount = GetTabCount();

    for (i = 0; i < iViewCount; ++i)
    {
        pView = GetCurrentEditorView (i);
        assert (pView != NULL);
        if (pView == NULL)
            continue;
        rapServerViews.Add(pView);
    };
}



LRESULT CGenEdFrame::OnTabSelect(WPARAM WParam, LPARAM LParam) 
{
    int iIndex=(int)WParam;
    CGenEdDoc *pDoc;

    pDoc = GetCurrentEditorDoc ();
    assert (pDoc != NULL);
    if (pDoc == NULL)
    {
        return (0);
    }

    if ((int)iIndex == m_arrInst.GetSize() + 1)
    {
        SelectInstances(m_arrInst.GetSize());
        m_pGenEdTabWnd->ActivateTab (m_iSelectedTab);
        return (0);
    }

    m_iSelectedTab = iIndex;

    if(iIndex == 0)
    {
        // For class view show document filename in caption

        if(!pDoc->IsReadOnly())
        {
            pDoc->ForceReadOnly (false);
        }

        pDoc->SetInstance (_T (""), _T (""), _T (""));

        OnUpdateFrameTitle (TRUE);

        return (0);
    }

    InstanceSelected (iIndex - 1);

    return (0);
}


CGenEdView *CGenEdFrame::GetCurrentEditorView(int iIndex)
{
    CGenEdView *pView;

    if (m_bTabbed)
    {
        CWnd*   pWndTmp = NULL;
        LPCTSTR psz;
        BOOL    bSelected;
        void*   pTmp;

        if(!m_pGenEdTabWnd->GetTabInfo(iIndex, psz, bSelected, pWndTmp, pTmp))
        {
            return NULL;
        }
        pView = dynamic_cast<CGenEdView *> (pWndTmp);
    }
    else
    {
        CDocument *pDoc;
        POSITION pPosition;

        pDoc = GetActiveDocument ();
        assert (pDoc != NULL);
        if (pDoc == NULL)
        {
            return (false);
        }

        pPosition = pDoc->GetFirstViewPosition();
        assert (pPosition != NULL);
        if (pPosition == NULL)
        {
            return (false);
        }

        pView = dynamic_cast<CGenEdView *> (pDoc->GetNextView (pPosition));
    }

    assert (pView != NULL);
    if (pView == NULL)
    {
        return (NULL);
    }

    return (pView);
}


int CGenEdFrame::GetTabCount()
{
    if (m_pGenEdTabWnd != NULL)
    {
        return (m_pGenEdTabWnd->GetTabCount());
    }

    if (m_pGenEdWnd != NULL)
    {
        return (1);
    }
    return (1);
}


CGenEdDoc *CGenEdFrame::GetCurrentEditorDoc ()
{
    CGenEdView *pView;
    CGenEdDoc *pDoc;

    pView = GetCurrentEditorView(0);
    assert (pView != NULL);
    if (pView == NULL)
    {
        return (NULL);
    }

    pDoc = pView->GetDocument ();
    assert (pDoc != NULL);
    if (pDoc == NULL)
    {
        return (NULL);
    }

    return (pDoc);
}


void CGenEdFrame::SelectInstances (int iInsertBefore)
{
    int iIndex;

    ASSERT ((int)iInsertBefore <= m_arrInst.GetSize ());

    CAddInstanceDlg dialog (&m_UntabbedInstances);

    if (dialog.DoModal () != IDOK)
    {
        if(m_iSelectedTab < m_pGenEdTabWnd->GetTabCount())
        {
            m_pGenEdTabWnd->ActivateTab(m_iSelectedTab);
        }
        else
        {
            m_pGenEdTabWnd->ActivateTab (0);
        }
        return;
    }

    if (m_iSelectedTab != 0 && m_iSelectedTab - 1 >= iInsertBefore)
    {
        m_iSelectedTab += dialog.m_SelectedInstances.GetSize ();
    }

    int iNumInstances = dialog.m_SelectedInstances.GetSize();
    for (iIndex = iNumInstances-1; iIndex >= 0; --iIndex)
    {
        AddInstanceTab(iInsertBefore, dialog.m_SelectedInstances[iIndex], false);
    }
    if(iNumInstances > 0)
    {
        m_bAutoMode = FALSE;
    }
    UpdateAddTab();
}


int CGenEdFrame::FindTabbedInstance (const CString& crstrInstPath, bool bWithTask)
{
    int iIndex;
    CString strLowerInstance;

    strLowerInstance = crstrInstPath;
    strLowerInstance.MakeLower ();

    int iNumInstances = m_arrInst.GetSize();
    for (iIndex = 0; iIndex < iNumInstances; ++iIndex)
    {
        CString strTabbedInstance;

        if (!bWithTask)
        {
            CString strTask;

            SplitInstance (m_arrInst[iIndex], strTabbedInstance, strTask);
        }
        else
        {
            strTabbedInstance = m_arrInst[iIndex];
        }

        strTabbedInstance.MakeLower ();

        if (strLowerInstance == strTabbedInstance)
        {
            return (iIndex);
        }
    }

    return (-1);
}


CString CGenEdFrame::FindUntabbedInstance (const CString& crstrInstPath, bool bWithTask)
{
    if(m_UntabbedInstances.getData(crstrInstPath))
    {
        return m_UntabbedInstances.getOriginalKey(crstrInstPath);
    }
    return _T("");
}


void CGenEdFrame::SplitInstance (const CString& crstrInstPath, CString &strInstanceNoTask, CString &strTask)
{
    int iPos;
    int iPosEnd;

    iPos = crstrInstPath.Find (_T ('@'));
    if (iPos == -1)
    {
        strInstanceNoTask = crstrInstPath;
        strTask = "";
    }
    else
    {
        iPosEnd = crstrInstPath.Find ('.', iPos + 1);

        if (iPosEnd == -1)
        {
            strInstanceNoTask = crstrInstPath.Left (iPos);
            strTask = crstrInstPath.Mid (iPos + 1);
        }
        else
        {
            strInstanceNoTask = crstrInstPath.Left (iPos) + crstrInstPath.Mid (iPosEnd + 1);
            strTask = crstrInstPath.Mid (iPos + 1, iPosEnd - (iPos + 1));
        }
    }
}


//------------------------------------------------------------------*
/**
 * add instance tab.
 *
 * @param            [in] iInsertBefore: the new instance tab will be inserted
 *                                       before the iInsertBefore-th instance
 * @param            [in] strInstanceName: name of the new instance
 * @param            [in] bUpdate: if true, the tab will be activated and
 *                                 UpdateAddTab() will be called.
 *                                 Shall be false, if more than one instance tab
 *                                 is inserted at a time. Call UpdateAddTab() at
 *                                 the end in this case.
 * @return           -
 * @exception        -
 * @see              -
*/
void CGenEdFrame::AddInstanceTab(int iInsertBefore, const CString &strInstanceName, bool bUpdate /*=true*/)
{
    CString strTabText;
    CString strMachine;
    CTargetOnlBase* pTarget;

    m_arrInst.InsertAt (iInsertBefore, strInstanceName);

    strTabText = GetInstanceTabName (iInsertBefore);
    if(!InsertTab(RUNTIME_CLASS(CGenEdView), iInsertBefore + 1, strTabText, &m_tCreateContext))
    {
        return;
    }
    else
    {
//    m_pGenEdTabWnd->SetToolTipText (iInsertBefore + 1, GetInstanceTabFullName (iInsertBefore));

        if(bUpdate)
        {
            m_pGenEdTabWnd->ActivateTab(iInsertBefore + 1);
        }
    }

    pTarget = GetMachineFromInstance (strInstanceName);
    if (pTarget != NULL)
    {
        HICON   hIcon = NULL;
        int     iImage = -1;

        GetConnectImage(pTarget->GetConnectState(), iImage);
        if(iImage >= 0)
        {
            hIcon = m_ImageList.ExtractIcon(iImage);
        }
        m_pGenEdTabWnd->SetTabIcon(iInsertBefore + 1, hIcon);
    }

//    CWnd*   pWndTmp;
//    if(m_pGenEdTabWnd->GetActiveTab(pWndTmp))
//    {
//        pWndTmp->Invalidate();   // not done in InsertView (is it a bug or featuring delayed invalidating???)
//    }

    VERIFY(m_UntabbedInstances.deleteData(strInstanceName));
    if(bUpdate)
    {
    }

    ++iInsertBefore;
}

BOOL CGenEdFrame::InsertTab
(
    CRuntimeClass* pViewClass,
    int nIndex,
    LPCTSTR lpszLabel,
    CCreateContext* pContext /*=NULL*/,
    UINT nID /*= -1*/
)
{
    int iNumTabs = m_pGenEdTabWnd->GetTabCount();

    if(nIndex < iNumTabs)
    {
        m_pGenEdTabWnd->InsertTab(RUNTIME_CLASS(CGenEdView), nIndex, lpszLabel, &m_tCreateContext, nID);
    }
    else
    {
        m_pGenEdTabWnd->AddTab(RUNTIME_CLASS(CGenEdView), lpszLabel, &m_tCreateContext, nID);
    }
    return TRUE;
}

void CGenEdFrame::InstanceSelected(int iIndex)
{
    CGenEdDoc *pGenEdDoc;
    CString strInstance;
    CString strInstanceNoTask;
    CString strTask;
    CTargetOnlBase *pTarget;

    pGenEdDoc = GetCurrentEditorDoc ();
    ASSERT (pGenEdDoc != NULL);
    if (pGenEdDoc == NULL)
    {
        return;
    }

    // For instance view show instance path in caption

    ASSERT (iIndex < m_arrInst.GetSize ());
    if (iIndex >= m_arrInst.GetSize ())
    {
        return;
    }
    strInstance = m_arrInst[iIndex];

    pGenEdDoc->ForceReadOnly (true);

    pTarget = GetMachineFromInstance (strInstance);
    ASSERT (pTarget != NULL);
    if (pTarget == NULL)
    {
        return;
    }

    SplitInstance (strInstance, strInstanceNoTask, strTask);
    pGenEdDoc->SetInstance (strInstanceNoTask, pTarget->GetId (), pTarget->GetControlAddress ());

    OnUpdateFrameTitle (TRUE);

    if (pTarget->IsConnected ())
    {
        pGenEdDoc->NotifyConnect (pTarget->GetControlAddress ());
    }
    pGenEdDoc->NotifyDebugState (pTarget->GetExclusiveSessionState ());
}


void CGenEdFrame::OnAddTarget (CTargetOnlBase *pTarget)
{
    if (m_bTabbed)
    {
        pTarget->RegisterConnectStateCB (&m_ConnectStateCB);
        CTargetOnl* pTarg = dynamic_cast<CTargetOnl*> (pTarget);
        if(pTarg)
        {
            pTarg->RegisterExclusiveSessionCB (&m_ExclusiveSessionCB);
        }
    }
}


void CGenEdFrame::OnRemoveTarget (CTargetOnlBase *pTarget)
{
    if (m_bTabbed)
    {
        pTarget->UnregisterConnectStateCB (&m_ConnectStateCB);
        CTargetOnl* pTarg = dynamic_cast<CTargetOnl*> (pTarget);
        if(pTarg)
        {
            pTarg->UnregisterExclusiveSessionCB (&m_ExclusiveSessionCB);
        }

        int iIndex;
        // reset tab images
        int iNumInstances = m_arrInst.GetSize();
        for (iIndex = 0; iIndex < iNumInstances; ++iIndex)
        {
            if (pTarget == GetMachineFromInstance (m_arrInst[iIndex]))
            {
                m_pGenEdTabWnd->SetTabIcon(iIndex+1, NULL);
            }
        }
    }
}





HRESULT CGenEdFrame::ShowInstance (const CString& crstrInstPath, bool bWithTask)
{
    int iIndex;
    CString strUntabbedInstance;

    if(!m_bTabbed)
    {
        return E_FAIL;
    }

    iIndex = FindTabbedInstance (crstrInstPath, bWithTask);
    if (iIndex >= 0)
    {
        int iActiveTab;
        int iInstTab = iIndex + 1;

        m_pGenEdTabWnd->GetActiveTab(iActiveTab);
        if(iActiveTab != iInstTab)
        {
            m_pGenEdTabWnd->ActivateTab (iInstTab);
        }
        return (S_OK);
    }

    strUntabbedInstance = FindUntabbedInstance (crstrInstPath, bWithTask);
    if (strUntabbedInstance != _T (""))
    {
        AddInstanceTab (m_arrInst.GetSize (), strUntabbedInstance, false);
        m_pGenEdTabWnd->ActivateTab (m_arrInst.GetSize ());
//        ActivateServer();
//        SetFocusToServer();
        UpdateTabs(eActivateNone);
        return (S_OK);
    }
    m_pGenEdTabWnd->ActivateTab(0);

    return (E_FAIL);
}


//------------------------------------------------------------------*
/**
 * get active tab.
 *
 * @param           -
 * @return          index of active tab
 * @exception       -
 * @see             
*/
int CGenEdFrame::GetActiveTab()
{
    int iIndex;
    m_pGenEdTabWnd->GetActiveTab(iIndex);
    return iIndex;
}


void CGenEdFrame::OnInstTabContextMenu (WPARAM tab, LPARAM pos)
{
    CMenu menu;
    CMenu *pPopup;
    LPPOINT point=(LPPOINT)pos;

    ActivateFrame ();

    if (!menu.LoadMenu (IDR_GENED_TABFRAME))
    {
        return;
    }

    pPopup = menu.GetSubMenu (0);
    ASSERT (pPopup != NULL);
    if (pPopup == NULL)
    {
        return;
    }

    if (m_iContextTab >= 1 && m_iContextTab <= m_arrInst.GetSize ())
    {
        pPopup->EnableMenuItem (ID_GENED_REMOVEINSTANCE, MF_ENABLED | MF_BYCOMMAND);
    }
    else
    {
        pPopup->EnableMenuItem (ID_GENED_REMOVEINSTANCE, MF_GRAYED | MF_BYCOMMAND);
    }

    if (m_arrInst.GetSize () != 0)
    {
        pPopup->EnableMenuItem (ID_GENED_REMOVEALL, MF_ENABLED | MF_BYCOMMAND);
    }
    else
    {
        pPopup->EnableMenuItem (ID_GENED_REMOVEALL, MF_GRAYED | MF_BYCOMMAND);
    }

    m_iContextTab = tab;
    pPopup->TrackPopupMenu (TPM_LEFTALIGN | TPM_RIGHTBUTTON, point->x, point->y, this);
}


void CGenEdFrame::ActivateFrame (int nCmdShow)
{
    BeginLayout ();
    SECMDIChildWnd::ActivateFrame (nCmdShow);
    EndLayout ();
}


void CGenEdFrame::ActivateClassTab()
{
    ASSERT(m_pGenEdTabWnd);
    if(m_pGenEdTabWnd)
    {
        m_pGenEdTabWnd->ActivateTab(0);
    }
}

void CGenEdFrame::ActivateServer ()
{
    CGenEdView *pView;

    pView = FindView();
    assert (pView != NULL);
    if (pView == NULL)
        return;

    BeginLayout ();
    pView->ActivateServer();
    EndLayout ();
}


void CGenEdFrame::DeactivateServer()
{
    CGenEdView *pView;

    pView = FindView();
    assert (pView != NULL);
    if (pView == NULL)
        return;

    BeginLayout ();
    pView->DeactivateServer();
    EndLayout ();
}


void CGenEdFrame::SetFocusToServer()
{
    CGenEdView *pView;

    pView = FindView ();
//    assert (pView != NULL);
    if (pView == NULL)
        return;

    BeginLayout ();
    pView->SetFocusToServer();
    EndLayout ();
}


void CGenEdFrame::OnAddInstance () 
{
    int iPosition;

    if (m_iContextTab < 0 || m_iContextTab > m_arrInst.GetSize ())
    {
        iPosition = m_arrInst.GetSize ();
    }
    else
    {
        iPosition = m_iContextTab;
    }

    SelectInstances (iPosition);
}


void CGenEdFrame::OnUpdateAddInstance (CCmdUI *pCmdUI) 
{
    if(m_UntabbedInstances.getNumStrings() > 0)
    {
        pCmdUI->Enable(TRUE);
    }
    else
    {
        pCmdUI->Enable(FALSE);
    }
}


void CGenEdFrame::OnRemoveInstance ()
{
    bool*   pbTmp = (bool*) 1;  // dummy pointer to insert

    if (m_iContextTab <= 0 || m_iContextTab > m_arrInst.GetSize ())
    {
        return;
    }

    m_UntabbedInstances.insertData(m_arrInst[m_iContextTab - 1], pbTmp);

    RemoveTab(m_iContextTab);
    m_arrInst.RemoveAt (m_iContextTab - 1);

    m_bAutoMode = FALSE;

    if (m_iContextTab == (int)m_iSelectedTab)
    {
        m_pGenEdTabWnd->ActivateTab (0);
    }

    UpdateAddTab();
}

//------------------------------------------------------------------*
/**
 * remove tab.
 *
 *  stingray is not so kind as to remove the associated view.
 *  we do this ourselves.
 *
 * @param           [in] iIndex: index of tab to be removed
 *
*/
void CGenEdFrame::RemoveTab(int iIndex)
{
	LPCTSTR     lpszLabel;
	BOOL        bSelected;
	CWnd*       pWnd;
	void*       pExtra;

	// Don't just delete the tab, destroy the associated window too.
	if (m_pGenEdTabWnd->GetTabInfo(iIndex, lpszLabel, bSelected, pWnd, pExtra))
    {
		pWnd->ShowWindow(SW_HIDE);
		pWnd->SendMessage(WM_CLOSE);
	}
    m_pGenEdTabWnd->RemoveTab(iIndex);
}


void CGenEdFrame::OnUpdateRemoveInstance (CCmdUI *pCmdUI) 
{
    pCmdUI->Enable (m_iContextTab >= 1 && m_iContextTab <= m_arrInst.GetSize ());
}


void CGenEdFrame::OnRemoveAll () 
{
    int iIndex;
    CList<CString, CString> instances;
    bool*   pbTmp = (bool*) 1;  // dummy pointer to insert
    int iSize = m_arrInst.GetSize();

    if (iSize == 0)
    {
        return;
    }

    for (iIndex = 0; iIndex < iSize; ++iIndex)
    {
        m_UntabbedInstances.insertData(m_arrInst[iIndex], pbTmp);
        RemoveTab (1);
    }

    m_arrInst.RemoveAll();

    m_iSelectedTab = 0;
    m_pGenEdTabWnd->ActivateTab(m_iSelectedTab);

    m_bAutoMode = FALSE;

    UpdateAddTab();
}


void CGenEdFrame::OnUpdateRemoveAll (CCmdUI *pCmdUI) 
{
    pCmdUI->Enable (m_arrInst.GetSize () != 0);
}


void CGenEdFrame::OnExpandTabs ()
{
    if (m_iTabExpandLevel < GetMaxExpandLevel ())
    {
        ++m_iTabExpandLevel;
    }

    UpdateTabNames ();	
}


void CGenEdFrame::OnUpdateExpandTabs (CCmdUI *pCmdUI) 
{
    pCmdUI->Enable (m_iTabExpandLevel < GetMaxExpandLevel ());
}


void CGenEdFrame::OnCollapseTabs() 
{
    if (m_iTabExpandLevel != 0)
    {
        --m_iTabExpandLevel;
    }

    UpdateTabNames ();
}


void CGenEdFrame::OnUpdateCollapseTabs(CCmdUI *pCmdUI) 
{
    pCmdUI->Enable (m_iTabExpandLevel != 0);
}


void CGenEdFrame::OnUpdateFrameTitle (BOOL bAddToTitle)
{
    OnUpdateTitle(bAddToTitle, 0);
    //PostMessage(CEM_UPDATE_TITLE, (WPARAM)bAddToTitle);
}

LRESULT CGenEdFrame::OnUpdateTitle(WPARAM wParam, LPARAM lParam)
{
    //CDocument* pDocument = GetActiveDocument();
    BOOL    bAddToTitle = (BOOL) wParam;
    CString strNewTitle;
    CString strFileName;

//    TRACE("Update Title\n");

    CGenEdDoc* pDocument =  (CGenEdDoc*) GetActiveDocument();
    if(pDocument)
    {
        CString strPathName;
        strPathName = pDocument->GetPathName();

        CFile  tFile;
        tFile.SetFilePath(strPathName);
        strFileName = tFile.GetFileName();
    }

    if (m_iSelectedTab == 0 || (int)m_iSelectedTab == m_arrInst.GetSize () + 1)
    {   
        if ((GetStyle() & FWS_ADDTOTITLE) == 0)
        {
            return 0;     // leave child window alone!
        }
        
        if (bAddToTitle &&
            pDocument != NULL)
        {
            if (pDocument == NULL)
            {
                strNewTitle = m_strTitle;
            }
            else
            {//ask if the document is modified
                BOOL bMod = pDocument->GetModified();
                if(bMod)
                {
                    strNewTitle = pDocument->GetTitle() + _T(" *");
                }
                else
                {
                    strNewTitle = pDocument->GetTitle();
                }
            }

            if (m_nWindow > 0)
            {
                CString strHelp(strNewTitle);
                strNewTitle.Format("%s: %d", strHelp, m_nWindow);
            }
        }
    }
    else
    {
        if(strFileName.Right(4).CompareNoCase(_T(".gvl")) == 0)
        {
            strNewTitle.Format("%s.(%s)", GetInstanceTabFullName(m_iSelectedTab - 1), strFileName);
        }
        else
        {
            strNewTitle = GetInstanceTabFullName(m_iSelectedTab - 1);
        }
    }
    if(pDocument && pDocument->IsReadOnly())
    {
        CString strReadOnly;
        CString strOldTitle(strNewTitle);
        strReadOnly.LoadString(IDS_READ_ONLY);
        strNewTitle.Format("%s (%s)", strOldTitle, strReadOnly);
    }

    if(strNewTitle.CompareNoCase(m_strOldTitle) != 0)
    {
        m_strOldTitle = strNewTitle;
        // update our parent window first
        GetMDIFrame()->OnUpdateFrameTitle(bAddToTitle);
        // set title if changed, but don't remove completely
        ::AfxSetWindowText(m_hWnd, strNewTitle);
    }
    return 0;
}


void CGenEdFrame::UpdateAddTab()
{
    bool bNeedAddTab;
    if(m_UntabbedInstances.getNumStrings() > 0)
    {
        bNeedAddTab = true;
    }
    else
    {
        bNeedAddTab = false;
    }

    if (bNeedAddTab == m_bHaveAddTab)
    {
        return;
    }

    if (bNeedAddTab)
    {
        CString strAdd;

        strAdd.LoadString (IDS_ADDTAB);
        m_pGenEdTabWnd->AddTab(RUNTIME_CLASS (CGenEdView), strAdd, &m_tCreateContext);

//        strAdd.LoadString (IDS_ADDTAB_LONG);
//        m_pGenEdTabWnd->SetToolTipText (m_arrInst.GetSize () + 1, strAdd);
    }
    else
    {
        RemoveTab(m_arrInst.GetSize () + 1);
    }

    m_bHaveAddTab = bNeedAddTab;
}


void CGenEdFrame::UpdateTabNames () 
{
    int iIndex;
    int iNumInstances = m_arrInst.GetSize();
    for (iIndex = 0; iIndex < iNumInstances; ++iIndex)
    {
        m_pGenEdTabWnd->RenameTab(iIndex + 1, GetInstanceTabName (iIndex));
    }
}


CString CGenEdFrame::GetInstanceTabName (int iIndex)
{
    CString strTabName;
    int iLevel=0;
    int iPos;

    strTabName = GetInstanceTabFullName (iIndex);

    iPos = strTabName.GetLength ();
    while (true)
    {
        while (iPos > 0 && strTabName[iPos - 1] != _T ('.'))
        {
            --iPos;
        }

        if (iPos == 0 || iLevel == m_iTabExpandLevel)
        {
            break;
        }

        --iPos;
        ++iLevel;
    }

    return (strTabName.Mid (iPos));
}


CString CGenEdFrame::GetInstanceTabFullName (int iIndex)
{
    CString strTabName;
    int iPos;

    strTabName = m_arrInst[iIndex];

    iPos = strTabName.Find ('@');
    if (iPos != -1)
    {
        strTabName = strTabName.Left (iPos) + strTabName.Mid (iPos + 1);
    }

    return (strTabName);
}


HRESULT CGenEdFrame::GetInstances ()
{
    HRESULT     hr;
    CGenEdDoc*  pDoc;
    CFile       file;
    SAFEARRAY*  parrInstances;
    long        lIndex;
    long        lLowerBound;
    long        lUpperBound;
    CString     strFileName;

    pDoc = GetCurrentEditorDoc();
    assert (pDoc != NULL);
    if (pDoc == NULL)
    {
        return (E_FAIL);
    }

    // Get document filename
    file.SetFilePath(pDoc->GetPathName());
    strFileName = file.GetFileName();

    // get data type
    CString strDataType;
    // special way for gvl files:
    // in this case, the data type is identical to the file name
    if(strFileName.Right(4).CompareNoCase(_T(".gvl")) == 0)
    {
        strDataType = strFileName;
    }
    else
    {
        // get data type for file using project info
        CString strIdPath;
        int     iFound;

        CComQIPtr<ICEProjInfo>    pProjInfo;
        CComPtr<IUnknown>         pTmp;
        pTmp.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
        pProjInfo = pTmp;

        CComBSTR    sFileName(strFileName);
        CComBSTR    sIdPath;
        hr = pProjInfo->getIdPathFromFileName(sFileName, &sIdPath); 
        if(hr != S_OK)
        {
            return hr;
        }

        strIdPath = sIdPath;
        iFound = strIdPath.ReverseFind(_T('/'));
        if(iFound == -1)
        {
            strDataType = strIdPath;
        }
        else
        {
            strDataType = strIdPath.Mid(iFound + 1);
        }
    }

    // now data type must not be empty
    if(strDataType.IsEmpty())
    {
        return E_FAIL;
    }

    // get instances for data type
    CComPtr<IDebugInfo> pDebugInfo;
    hr = m_pContainer->GetDebugInfo(&pDebugInfo);

    if(!pDebugInfo)
    {
        return E_FAIL;
    }

    IUnknown*   pUnknown = NULL;
    CComQIPtr<IProjectDBI>    pIProjectDBI;

    hr = pDebugInfo->get_Project(&pUnknown);
    if (hr != S_OK)
    {
        return E_FAIL;
    }

    ASSERT (pUnknown);
    if (!pUnknown)
    {
        return E_FAIL;
    }

    pIProjectDBI = pUnknown;
    ASSERT (pIProjectDBI != NULL);
    if (pIProjectDBI == NULL)
    {
        return E_FAIL;
    }
    pUnknown->Release();

    CComBSTR    sDataType(strDataType);

    hr = pIProjectDBI->GetInstancesForDataType(sDataType, &parrInstances);
    if(hr != S_OK)
    {
        return E_FAIL;
    }

    bool*   pbTmp = (bool*) 1;  // dummy pointer to insert
    m_UntabbedInstances.reset();

    ::SafeArrayGetLBound (parrInstances, 1, &lLowerBound);
    ::SafeArrayGetUBound (parrInstances, 1, &lUpperBound);

    for (lIndex = lLowerBound; lIndex <= lUpperBound; ++lIndex)
    {
        BSTR pszInstance;

        ::SafeArrayGetElement (parrInstances, &lIndex, &pszInstance);
        m_UntabbedInstances.insertData(pszInstance, pbTmp);
        ::SysFreeString (pszInstance);
    }

    ::SafeArrayDestroy (parrInstances);

    return (S_OK);
}



void CGenEdFrame::RemoveTabbedInstances ()
{
    int iNumInstances = m_arrInst.GetSize();
    for(int iIndex = 0; iIndex < iNumInstances; ++iIndex)
    {
        m_UntabbedInstances.deleteData(m_arrInst[iIndex]);
    }
}


int CGenEdFrame::GetMaxExpandLevel ()
{
    int iMaxInstanceDepth=0;
    int iIndex;
    int iNumInstances = m_arrInst.GetSize();
    for (iIndex = 0; iIndex < iNumInstances; ++iIndex)
    {
        CString strTabName;
        int iPos;
        int iInstanceDepth=0;

        strTabName = m_arrInst[iIndex];
        int iLength = strTabName.GetLength();
        for (iPos = 0; iPos < iLength; ++iPos)
        {
            if (strTabName[iPos] == '.')
            {
                ++iInstanceDepth;
            }
        }

        if (iInstanceDepth > iMaxInstanceDepth)
        {
            iMaxInstanceDepth = iInstanceDepth;
        }
    }

    return (iMaxInstanceDepth);
}


CTargetOnlBase *CGenEdFrame::MachineFromInstance (const CString& crstrInstPath)
{
    int iPos;
    CString strMachine;
    CTargetOnlBase* pTarget = NULL;
    CString strMachineTmp;

    iPos = crstrInstPath.Find (_T ('}'));
    if (iPos <= 0)
    {
        return (NULL);
    }
    strMachineTmp = crstrInstPath.Left(iPos+1);

    // get target name from project manager
    CComPtr<IUnknown>       pTmp;
    CComQIPtr<ICEProjInfo>  pProjInfo;

    pTmp.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
    pProjInfo = pTmp;

    if(!pProjInfo)
    {
        return NULL;
    }

    CComBSTR    sAddress(strMachineTmp);
    CComBSTR    sTargetIdPath;

    pProjInfo->getTargetNameForAddr(sAddress, &sTargetIdPath);

    strMachineTmp = sTargetIdPath;

    // strip target name
    iPos = strMachineTmp.ReverseFind(CE_XML_IDPATH_SEP_CHAR);
    if (iPos <= 0)
    {
        return (NULL);
    }
    strMachineTmp = strMachineTmp.Right(strMachineTmp.GetLength() - iPos - 1);

    // get target from target set
    CTargetSet* pTargetSet = CEGetTargetSet("Project Targets");
    if(pTargetSet)
    {
        pTarget = pTargetSet->GetTarget(strMachineTmp);
    }
    return (pTarget);
}



CTargetOnlBase *CGenEdFrame::GetMachineFromInstance (const CString& crstrInstPath)
{
    int iPos;
    CTargetOnlBase* pTarget = NULL;
    CString strTargetName;

    iPos = crstrInstPath.Find (_T ('.'));
    if (iPos <= 0)
    {
        return (NULL);
    }
    strTargetName = crstrInstPath.Left(iPos);

    // get target from target set
    CTargetSet* pTargetSet = CEGetTargetSet("Project Targets");
    if(pTargetSet)
    {
        pTarget = pTargetSet->GetTarget(strTargetName);
    }
    return (pTarget);
}


//CPrjDoc *CGenEdFrame::GetProject ()
//{
//    CCEMAINApp *pApp;
//
//    pApp = (CCEMAINApp *)AfxGetApp ();
//    assert (pApp != NULL);
//    if (pApp == NULL)
//    {
//        return (NULL);
//    }
//
//    return (((CCEMAINApp *)AfxGetApp ())->GetProjectDoc ());
//}

//void CGenEdFrame::GetSessionImage(CSC_SESSION_STATE VarSessionState, CSC_SESSION_STATE ExclusiveSessionState, int & iImage)
//{
//    iImage = -1;
//
//    if (interrupted == ExclusiveSessionState)
//    {
//        iImage = IDI_COMM_INTERRUPTED;
//        return;
//    }
//
//    if (interrupted == VarSessionState)
//    {
//        iImage = IDI_INTERRUPTED;
//        return;
//    }
//
//    if (exclusive_established == ExclusiveSessionState)
//    {
//        if (RW_established == VarSessionState)
//        {
//            iImage = IDI_ALL_ESTABLISHED;
//        }
//        else
//        {
//            iImage = IDI_COMM_ESTABLISHED;
//        }
//
//        return;
//    }
//
//    if (RW_established == VarSessionState)
//    {
//        iImage = IDI_ESTABLISHED;
//        return;
//    }
//}

void CGenEdFrame::GetConnectImage(E_TargetOnlConnectState eState, int& iImage)
{
    if(eState == eTOCS_not_connected)
    {
        iImage = -1;
    }
    else
    {
        iImage = 5 + (int)eState;
    }
}

void CGenEdFrame::OnConnectStateCB(CTargetOnlBase *pTarget)
{
    int iIndex;
    int iImage;
    HICON   hIcon = NULL;

    ASSERT (pTarget != NULL);
    if (pTarget == NULL)
        return;
    int iNumInstances = m_arrInst.GetSize();
    for (iIndex = 0; iIndex < iNumInstances; ++iIndex)
    {
        if (pTarget == GetMachineFromInstance (m_arrInst[iIndex]))
        {
            hIcon = NULL;
            GetConnectImage(pTarget->GetConnectState(), iImage);
            if(iImage >= 0)
            {
                hIcon = m_ImageList.ExtractIcon(iImage);
            }
            m_pGenEdTabWnd->SetTabIcon(iIndex+1, hIcon);
        }
    }
}

void CGenEdFrame::OnExclusiveSessionCB (CTargetOnl *pTarget, CSC_SESSION_STATE sessionState, HRESULT hrError)
{
    CGenEdDoc *pDoc;
    HICON   hIcon = NULL;

    ASSERT (pTarget != NULL);
    if (pTarget == NULL)
        return;

    pDoc = dynamic_cast<CGenEdDoc *> (GetActiveDocument ());
    ASSERT (pDoc != NULL);
    if (pDoc != NULL)
    {
        CString strInstance;
        CString strMachineName;
        CString strMachineAddress;

        pDoc->GetInstance (strInstance, strMachineName, strMachineAddress);

        if (strMachineAddress.CompareNoCase (pTarget->GetControlAddress ()) == 0)
        {
            pDoc->NotifyDebugState (sessionState);
        }
    }
}


void CGenEdFrame::OnNcDestroy ()
{
    m_ActionCBs.Execute (GenEdFrameDestroyed, this);

    SECMDIChildWnd::OnNcDestroy ();
}


void CGenEdFrame::OnNcActivate (BOOL bActive)
{
    SECMDIChildWnd::OnNcActivate (bActive);

    m_ActionCBs.Execute (bActive ? GenEdFrameActivated : GenEdFrameDeactivated, this);
}


/////////////////////////////////////////////////////////////////////////////
// CAddInstanceDlg dialog

CAddInstanceDlg::CAddInstanceDlg (CSBLLexTree<bool>* pInstances, CWnd *pParent /*=NULL*/)
: CDialog (CAddInstanceDlg::IDD, pParent),
  m_pInstances (pInstances)
{
	//{{AFX_DATA_INIT(CAddInstanceDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAddInstanceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddInstanceDlg)
	DDX_Control(pDX, IDC_GENED_ADDINSTANCELIST, m_InstanceList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddInstanceDlg, CDialog)
	//{{AFX_MSG_MAP(CAddInstanceDlg)
	ON_NOTIFY(NM_DBLCLK, IDC_GENED_ADDINSTANCELIST, OnDoubleClickInstances)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddInstanceDlg message handlers

void CAddInstanceDlg::OnDoubleClickInstances (NMHDR *pNMHDR, LRESULT *pResult) 
{
    OnOK ();
    *pResult = 0;
}

void CAddInstanceDlg::OnOK () 
{
    m_SelectedInstances.RemoveAll ();

#if _MFC_VER == 0x0421
    int iItem=-1;

    while ((iItem = m_InstanceList.GetNextItem (iItem, LVNI_ALL | LVNI_SELECTED)) != -1)
    {
        m_SelectedInstances.Add (m_InstanceList.GetItemText (iItem, 0));
    }
#else
    POSITION pPosition;

    pPosition = m_InstanceList.GetFirstSelectedItemPosition ();
    if (pPosition == NULL)
    {
        CDialog::OnOK ();
        return;
    }

    while (pPosition != NULL)
    {
        int iIndex;

        iIndex = m_InstanceList.GetNextSelectedItem (pPosition);
        m_SelectedInstances.Add (m_InstanceList.GetItemText (iIndex, 0));
    }
#endif

    CDialog::OnOK();
}


BOOL CAddInstanceDlg::OnInitDialog () 
{
    int iMaxWidth=0;
    int iWidth;

    CDialog::OnInitDialog ();

    // run over all instances to determine width of column
    CStringArray    astrInstancePaths;
    m_pInstances->getAllStringsFromSubstring("", astrInstancePaths);

    int iNumPaths = astrInstancePaths.GetSize();
    for(int iPath = 0; iPath < iNumPaths; ++iPath)
    {
        iWidth = m_InstanceList.GetStringWidth(astrInstancePaths[iPath]);
        if (iWidth > iMaxWidth)
        {
            iMaxWidth = iWidth;
        }
    }

    // create new column
    m_InstanceList.InsertColumn (0, _T (""), LVCFMT_LEFT, iMaxWidth + 20);

    // fill instance list
    for(iPath = 0; iPath < iNumPaths; ++iPath)
    {
        m_InstanceList.InsertItem(iPath, astrInstancePaths[iPath]);
    }
    if(iNumPaths > 0)
    {
        m_InstanceList.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
    }

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CGenEdFrame::OnRefreshAll() 
{
//    CMainFrame *pMainFrame;
//
//    pMainFrame = (CMainFrame *)AfxGetMainWnd ();
//    assert (pMainFrame != NULL);
//    if (pMainFrame == NULL)
//    {
//        return;
//    }
//
//    pMainFrame->UpdateAllTabFileViews ();
}


void CGenEdFrame::OnPreviousTab ()
{
    int iIndex;

    if (!m_bTabbed)
        return;

    m_pGenEdTabWnd->GetActiveTab(iIndex);

    if (iIndex != 0)
    {
        m_pGenEdTabWnd->ActivateTab(iIndex - 1);
    }
    else
    {
        m_pGenEdTabWnd->ActivateTab(m_pGenEdTabWnd->GetTabCount() - 1);
    }
}


void CGenEdFrame::OnNextTab ()
{
    int iIndex;

    if (!m_bTabbed)
        return;

    m_pGenEdTabWnd->GetActiveTab(iIndex);

    if (iIndex < m_pGenEdTabWnd->GetTabCount() - 1)
    {
        m_pGenEdTabWnd->ActivateTab (iIndex + 1);
    }
    else
    {
        m_pGenEdTabWnd->ActivateTab (0);
    }
}


void CGenEdFrame::OnNextServer ()
{
    CGenEdDoc *pDoc;
    CGenEdCntrItem *pActiveServer;
    CArray<CGenEdView *, CGenEdView *> apServerViews;
    int iCurrentIndex;
    int iNewIndex;
    CGenEdCntrItem *pNewServer;

    pDoc = GetCurrentEditorDoc ();
    ASSERT (pDoc != NULL);
    if (pDoc == NULL)
        return;

    pActiveServer = pDoc->GetActiveServer ();
    if (pActiveServer == NULL)
    {
        SwitchToAnyServer ();
        return;
    }

    if (!GetServerIndex (pActiveServer, iCurrentIndex))
        return;

    GetServerViews (apServerViews);

    iNewIndex = iCurrentIndex == (apServerViews.GetSize () - 1) ? 0 : iCurrentIndex + 1;

    while (iNewIndex != iCurrentIndex)
    {
        CGenEdView *pView;

        pView = apServerViews[iNewIndex];
        ASSERT (pView != NULL);
        if (pView == NULL)
            continue;

        if (pView->GetServer () != NULL)
            break;

        iNewIndex = iNewIndex == (apServerViews.GetSize () - 1) ? 0 : iNewIndex + 1;
    }

    if (iNewIndex == iCurrentIndex)
        return;

    pNewServer = apServerViews[iNewIndex]->GetServer ();
    
//    ActivateServer();

    SetFocusToServer();
}


void CGenEdFrame::OnPreviousServer ()
{
    CGenEdDoc *pDoc;
    CGenEdCntrItem *pActiveServer;
    CArray<CGenEdView *, CGenEdView *> apServerViews;
    int iCurrentIndex;
    int iNewIndex;
    CGenEdCntrItem *pNewServer;

    pDoc = GetCurrentEditorDoc ();
    ASSERT (pDoc != NULL);
    if (pDoc == NULL)
        return;

    pActiveServer = pDoc->GetActiveServer ();
    if (pActiveServer == NULL)
    {
        SwitchToAnyServer ();
        return;
    }

    if (!GetServerIndex (pActiveServer, iCurrentIndex))
        return;

    GetServerViews (apServerViews);

    iNewIndex = iCurrentIndex == 0 ? apServerViews.GetSize () - 1 : iCurrentIndex - 1;

    while (iNewIndex != iCurrentIndex)
    {
        CGenEdView *pView;

        pView = apServerViews[iNewIndex];
        ASSERT (pView != NULL);
        if (pView == NULL)
            continue;

        if (pView->GetServer () != NULL)
            break;

        iNewIndex = iNewIndex == 0 ? apServerViews.GetSize () - 1 : iNewIndex - 1;
    }

    if (iNewIndex == iCurrentIndex)
        return;

    pNewServer = apServerViews[iNewIndex]->GetServer ();
    
//    ActivateServer ();

    SetFocusToServer ();
}


void CGenEdFrame::SwitchToAnyServer ()
{
    CArray<CGenEdView *, CGenEdView *> apServerViews;
    int iIndex;
    CGenEdView *pView;
    CGenEdCntrItem *pServer;

    GetServerViews (apServerViews);

    for (iIndex = 0; iIndex < apServerViews.GetSize (); ++iIndex)
    {
        pView = apServerViews[iIndex];
        ASSERT (pView != NULL);
        if (pView == NULL)
            continue;

        pServer = pView->GetServer ();
        ASSERT (pServer != NULL);
        if (pServer == NULL)
            continue;

        break;
    }

    if (iIndex == apServerViews.GetSize ())
        return;

//    ActivateServer ();

    SetFocusToServer ();
}


bool CGenEdFrame::GetServerIndex (CGenEdCntrItem *pServer, int &iIndex)
{
    CArray<CGenEdView *, CGenEdView *> apServerViews;

    GetServerViews(apServerViews);

    for (iIndex = 0; iIndex < apServerViews.GetSize (); ++iIndex)
    {
        CGenEdView *pView;

        pView = apServerViews[iIndex];
        ASSERT (pView != NULL);
        if (pView == NULL)
            continue;

        if (pServer == pView->GetServer ())
        {
            iIndex = iIndex;
            return (true);
        }
    }

    return (false);
}


void CGenEdFrame::CheckAssignmentChange ()
{
	CGenEdDoc *pDoc;
    CString strInstance;
    CString strOldMachineName;
    CString strOldMachineAddress;
    CString strNewMachineName;
    CString strNewMachineAddress;
    CTargetOnlBase *pTarget;

    if (m_iSelectedTab == 0)
    {
        return;
    }

    pDoc = GetCurrentEditorDoc ();
    assert (pDoc != NULL);
    if (pDoc == NULL)
    {
        return;
    }

    pDoc->GetInstance (strInstance, strOldMachineName, strOldMachineAddress);

    pTarget = GetMachineFromInstance (m_arrInst[m_iSelectedTab - 1]);
    ASSERT (pTarget != NULL);
    if (pTarget == NULL)
    {
        return;
    }

    strNewMachineName = pTarget->GetId ();
    strNewMachineAddress = pTarget->GetControlAddress ();

    if (strNewMachineName.CompareNoCase (strOldMachineName) != 0)
    {
        pDoc->SetInstance (strInstance, strNewMachineName, strNewMachineAddress);
    }
}


CGenEdView *CGenEdFrame::FindView()
{
//    CGenEdCntrItem*  pServerTmp = NULL;
    CWnd*       pWndTmp;
    CGenEdView* pView = NULL;

    if(m_pGenEdTabWnd)
    {
        if(m_pGenEdTabWnd->GetActiveTab(pWndTmp))
        {
            pView = dynamic_cast<CGenEdView*> (pWndTmp);
        }
    }
    else
    {
        CDocument *pDoc;
        POSITION pPosition;

        pDoc = GetActiveDocument ();
//        assert (pDoc != NULL);
        if (pDoc == NULL)
        {
            return (false);
        }

        pPosition = pDoc->GetFirstViewPosition();
//        assert (pPosition != NULL);
        if (pPosition == NULL)
        {
            return (false);
        }

        pView = dynamic_cast<CGenEdView *> (pDoc->GetNextView (pPosition));
    }
//    ASSERT(pView);
    return(pView);
}


void CGenEdFrame::BeginLayout ()
{
    CGenEdDoc *pDoc;

    pDoc = dynamic_cast<CGenEdDoc *> (GetActiveDocument ());
//    assert (pDoc != NULL);
    if (pDoc != NULL)
    {
        pDoc->SetDelayedPositioning (TRUE);
    }
}


void CGenEdFrame::EndLayout ()
{
    PostMessage (RECALCULATE_LAYOUT);
}


void CGenEdFrame::OnRecalculateLayout (WPARAM, LPARAM)
{
    CGenEdDoc *pDoc;

    RecalcLayout ();

    pDoc = dynamic_cast<CGenEdDoc *> (GetActiveDocument ());
//    assert (pDoc != NULL);
    if (pDoc != NULL)
    {
        pDoc->SetDelayedPositioning (FALSE);
    }

    RecalcLayout ();
}


void CGenEdFrame::OnSysCommand (UINT nID, LONG lParam)
{
    SECMDIChildWnd::OnSysCommand (nID, lParam);

    // There were heavy problems for another child frame to obtain the focus
    // when switching via Ctrl+Tab from out of an editor window (the other
    // frame became active but the editor frame kept the focus). I don't know
    // what's wrong and thus I just posted a message to force the focus to
    // the frame being active after switch.

    if ((nID & 0xfff0) == SC_NEXTWINDOW || (nID & 0xfff0) == SC_PREVWINDOW)
    {
        PostMessage (SET_FOCUS_TO_ACTIVE_CHILD_FRAME);
    }
}


LRESULT CGenEdFrame::OnSetFocusToActiveChildFrame (WPARAM, LPARAM)
{
    CMDIFrameWnd *pMainFrame;
    CMDIChildWnd *pChildFrame;

    pMainFrame = dynamic_cast<CMDIFrameWnd *> (GetParentFrame ());
    if (pMainFrame == NULL)
        return (0);

    pChildFrame = pMainFrame->MDIGetActive ();
    if (pChildFrame == NULL)
        return (0);

    pChildFrame->SetFocus ();
    return (0);
}


void CGenEdFrame::LoadWindowPosition(const CString& crstrFilePath)
{
    WINDOWPLACEMENT wndPlc;

    // get settings
    CCEMAINApp* pApp = (CCEMAINApp*)AfxGetApp();
    CSettings*  pSettings = pApp->GetSettings();
    ASSERT(pSettings);
    CDocumentSettings* pDocSettings = pSettings->m_containerSettings.FindDocument(crstrFilePath, FALSE);
    if(!pDocSettings)
    {
        return;
    }
    // get window placement
    if(pDocSettings->GetWindowPlacement(wndPlc))
    {
        SetWindowPlacement(&wndPlc);
        pDocSettings->ResetWindowPlacement();   // to get data only at the first time
    }
}


CDocumentSettings* CGenEdFrame::GetDocumentSettings()
{
    CDocumentSettings* pDocSettings = NULL;
    CGenEdDoc*  pDoc = GetCurrentEditorDoc();
    if(pDoc)
    {
        pDocSettings = pDoc->GetDocumentSettings();
    }
    return pDocSettings;
}


CString CGenEdFrame::GetToolTipTextForTab(int iTab)
{
    CString strText;
    if(iTab == 0)
    {
        // show file name incl path
        CGenEdDoc* pDoc = GetCurrentEditorDoc();
        if(pDoc)
        {
            strText = pDoc->GetPathName();
        }
    }
    else
    {
        int iNumTabs = m_arrInst.GetSize();
        if(iTab <= iNumTabs)
        {
            return m_arrInst[iTab-1];
        }
        else
        {
            if(m_bHaveAddTab)
            {
                strText.LoadString(IDS_ADD_TAB_TOOLTIP);
            }
        }
    }
    return strText;
}


bool CGenEdFrame::IsTabbed()
{
    return m_bTabbed;
}


void CGenEdFrame::OnDestroy() 
{
    // save instance information
    SaveInstanceInfo();

	SECMDIChildWnd::OnDestroy();
}


int CGenEdFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    CKADInfo*   pKADInfo = ((CCEMAINApp*)AfxGetApp())->GetKADInfoObject();
    if(pKADInfo)
    {
        UINT    nStrLen = _MAX_PATH;
        CString strImageBase;
        HRESULT hr = UTIL_GetInstallPath(strImageBase.GetBuffer(_MAX_PATH), &nStrLen);
        strImageBase.ReleaseBuffer();
        strImageBase += CE_IMAGE_PATH;

        CString strDesktopIcon = strImageBase + _T("\\") + pKADInfo->GetDesktopIcon();
        CString strAppTitle = pKADInfo->GetApplicationTitle();

        if(hr == S_OK && !strDesktopIcon.IsEmpty())
        {
            // set icon
            HICON   h_Icon;
            h_Icon = (HICON)::LoadImage(0, strDesktopIcon, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
            if(h_Icon)
            {
                SetIcon(h_Icon, FALSE);
            }
        }
    }

	if (SECMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}


int CGenEdFrame::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
    SetFocusToServer();
    CMDIChildWnd::OnMDIActivate(TRUE, this, NULL);
    return SECMDIChildWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}


void CGenEdFrame::OnSetFocus(CWnd* pOldWnd) 
{
	SECMDIChildWnd::OnSetFocus(pOldWnd);
    SetFocusToServer();
}

