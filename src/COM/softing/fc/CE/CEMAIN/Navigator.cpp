
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*


#include "stdafx.h"
#include "CEMAIN.h"
#include "Navigator.h"
#include "cesysdef.h"
//#include "CEProjInfo_i.c"
//#include "CEProjMan_i.c"
#include "CECompMan.h"
#include "TargetSet.h"
#include "utilif.h"
#include "SBLLexTree.h"
#include "KADHelper/KADInfo.h"
#include "Subscription.h"
#include "SubscrElement.h"
#include "ExeDelegator.h"
#include "MainFrm.h"
#include "Settings.h"

#include "assert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CNavigator dialog

//const int con_i_offset = 7;    // extra space for gripper
const int con_i_offset = 0;     // no gripper
const int con_i_dist_tab = 7;
const int con_i_dist_tree = 10;
const int con_i_dist_combo = 13;

IMPLEMENT_DYNCREATE(CNavigator, CSizedDialogBar)

CNavigator::CNavigator(CWnd* pParent /*=NULL*/)
:   m_StateChangeCB(this, OnStateChange),
	m_SessionCB (this, OnSessionCB), 
    m_DebugSessionCB (this, OnDebugSessionCB),
    m_ConnectStateCB (this, OnConnectChange),
    m_hAccelerators(NULL),
    m_pProjEventHandler(NULL)
{
    m_strRegKey = "Navigator";
    LoadSizeData();     // docking height

	//{{AFX_DATA_INIT(CNavigator)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    CCEStateManager *pStateManager = NULL;
    pStateManager = CEGetStateManager();
    ASSERT(pStateManager);
    if (pStateManager != NULL)
    {
        pStateManager->RegisterCallback (&m_StateChangeCB);
    }
    m_bInitialized = FALSE;
    m_bProjectLoaded = FALSE;
    m_IDCheckEvent = 1;
}

CNavigator::~CNavigator()
{
    SaveSizeData();     // docking height
    SaveProfile();
    UnregisterSessionCBs();
    CCEStateManager * pStateManager = CEGetStateManager ();
    if (pStateManager != NULL)
    {
        pStateManager->UnregisterCallback(&m_StateChangeCB);
    }
}

void CNavigator::DoDataExchange(CDataExchange* pDX)
{
	CSizedDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNavigator)
	DDX_Control(pDX, IDC_TAB_NAVIGATOR, m_TabCtrl);
	DDX_Control(pDX, IDC_INSTTREE, m_tInstTreeCtrl);
	DDX_Control(pDX, IDC_HWTREE, m_tHWTreeCtrl);
	DDX_Control(pDX, IDC_FlTREE, m_tFlTreeCtrl);
	DDX_Control(pDX, IDC_FILTER_NAME, m_FltNameCtrl);
	DDX_Control(pDX, IDC_FILTER_FRAME, m_FltFrameCtrl);
	DDX_Control(pDX, IDC_FILTER_CAT, m_FltCatCtrl);
	DDX_Control(pDX, IDC_ClTREE, m_tClTreeCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNavigator, CSizedDialogBar)
	//{{AFX_MSG_MAP(CNavigator)
	ON_WM_TIMER()
	ON_WM_CREATE()
    ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_NAVIGATOR, OnSelchangeTabNavigator)
	ON_CBN_SELENDOK(IDC_FILTER_NAME, OnSelendokFilterName)
	ON_CBN_EDITCHANGE(IDC_FILTER_NAME, OnEditchangeFilterName)
	ON_CBN_SELCHANGE(IDC_FILTER_CAT, OnSelchangeFilterCat)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_FlTREE, OnItemexpandingFlTREE)
	ON_CBN_KILLFOCUS(IDC_FILTER_NAME, OnKillfocusFilterName)
	ON_WM_CONTEXTMENU()
	ON_NOTIFY(TVN_BEGINLABELEDIT, IDC_FlTREE, OnBeginlabeleditFlTREE)
	ON_NOTIFY(TVN_BEGINLABELEDIT, IDC_ClTREE, OnBeginlabeleditClTREE)
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_ClTREE, OnEndlabeleditClTREE)
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_FlTREE, OnEndlabeleditFlTREE)
	ON_NOTIFY(TVN_BEGINLABELEDIT, IDC_HWTREE, OnBeginlabeleditHwtree)
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_HWTREE, OnEndlabeleditHwtree)
	ON_NOTIFY(TVN_BEGINLABELEDIT, IDC_INSTTREE, OnBeginlabeleditInsttree)
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_INSTTREE, OnEndlabeleditInsttree)
	ON_NOTIFY(TVN_BEGINDRAG, IDC_ClTREE, OnBegindragClTREE)
	ON_NOTIFY(TVN_BEGINDRAG, IDC_FlTREE, OnBegindragFlTREE)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_HWTREE, OnItemexpandingHwtree)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_INSTTREE, OnItemexpandingInsttree)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_ClTREE, OnItemexpandingClTREE)
	ON_WM_KEYDOWN()
	ON_NOTIFY(TVN_BEGINDRAG, IDC_HWTREE, OnBegindragHwtree)
	ON_NOTIFY(TVN_BEGINDRAG, IDC_INSTTREE, OnBegindragInsttree)
	ON_NOTIFY(TVN_KEYDOWN, IDC_FlTREE, OnKeydownFlTREE)
	ON_COMMAND(ID_NAV_COPY, OnNavCopy)
	ON_COMMAND(ID_NAV_CUT, OnNavCut)
	ON_COMMAND(ID_NAV_PASTE, OnNavPaste)
	ON_COMMAND(ID_COLLAPSE_ALL, OnCollapseAll)
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAll)
	ON_COMMAND(IDM_NAV_CONTEXT_MENU, OnNavContextMenu)	// SHIFT_F10 14.12.04 SIS
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_DROPFILES()
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
    ON_COMMAND_RANGE(ID_NAV_CONTEXT_BASE, ID_NAV_CONTEXT_END, OnContextMenuTreeCommand)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNavigator message handlers


//------------------------------------------------------------------*
/**
 * save user settings to profile
 *
 * @see             LoadProfile()
*/
void CNavigator::LoadProfile()
{
    m_ActualTreeMode = (NAV_TreeMode)AfxGetApp()->GetProfileInt("Navigator", "ActTree", NAV_CLASSVIEW);
}

//------------------------------------------------------------------*
/**
 * save user settings in profile
 *
 * @see             SaveProfile()
*/
void CNavigator::SaveProfile()
{
    AfxGetApp()->WriteProfileInt("Navigator", "ActTree", m_ActualTreeMode);
}

//------------------------------------------------------------------*
/**
 * initialization
 *
 *  load type data from KAD into kad info object
 *  load profile
 *  init tab
 *  init controls
 *
 * @see             CKADInfo::Load(), LoadProfile(), InitTab(), InitControls()
*/
BOOL CNavigator::Init()
{
    if(m_bInitialized)
        return TRUE;

    BOOL    bReturn = TRUE;
//    CComPtr<IUnknown>   pKadManager;
//    CComPtr<IUnknown>   pTmp;
//
//    pTmp.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
//    m_pProjMan = pTmp;
//    if(!m_pProjMan)
//    {
//        return (FALSE);
//    }

    LoadProfile();

    InitTab();

    bReturn = InitControls();

    // accelerator table
    HINSTANCE hInstance;
    hInstance = AfxFindResourceHandle (MAKEINTRESOURCE (IDR_NAVACCEL), RT_ACCELERATOR);
    m_hAccelerators = ::LoadAccelerators (hInstance, MAKEINTRESOURCE (IDR_NAVACCEL));

    m_bInitialized = TRUE;

    return bReturn;
}


//------------------------------------------------------------------*
/**
 * init controls
 *
 *  set current tab to class view
 *  init tree controls
 *  resize window
 *  show tree
 *
 * @see             CNavTreeCtrl::Init()
*/
BOOL CNavigator::InitControls()
{
    CRect       tClientRect;
    CKADInfo*   pKADInfo;

    UpdateData(FALSE);

    // only preliminary
    m_TabCtrl.SetCurSel(NAV_CLASSVIEW);

    pKADInfo = ((CCEMAINApp*)AfxGetApp())->GetKADInfoObject();

    m_tFlTreeCtrl.Init(pKADInfo);
    m_tClTreeCtrl.Init(pKADInfo);
    m_tHWTreeCtrl.Init(pKADInfo);
    m_tInstTreeCtrl.Init(pKADInfo);

    // now init the Filter controls
    GetWindowRect(&m_WindowRect);
    GetClientRect(&tClientRect);
    ResizeWindow(tClientRect);
    ShowActualTree();

    return TRUE;
}



//------------------------------------------------------------------*
/**
 * init tab control
 *
 *  create tab image list
 *  set tabs for the four tree views
 *
*/
void CNavigator::InitTab()
{
    UpdateData(FALSE);

    TC_ITEM TabCtrlItem;
    CString strHelp;

    m_ctlImage.Create(IDB_TABIMAGELIST,16,0,RGB(255,0,255));
    m_ctlImage.SetBkColor(GetSysColor(COLOR_WINDOW));
    m_TabCtrl.SetImageList(& m_ctlImage);

    TabCtrlItem.mask = TCIF_TEXT| TCIF_IMAGE;
    strHelp.LoadString(IDS_NAV_FILEVIEW_TITLE);
    TabCtrlItem.pszText = (LPTSTR)LPCTSTR(strHelp);
    TabCtrlItem.iImage = NAV_FILEVIEW;
    m_TabCtrl.InsertItem( NAV_FILEVIEW, &TabCtrlItem );

    strHelp.LoadString(IDS_NAV_CLASSVIEW_TITLE);
    TabCtrlItem.pszText = (LPTSTR)LPCTSTR(strHelp);
    TabCtrlItem.iImage = NAV_CLASSVIEW;
    m_TabCtrl.InsertItem( NAV_CLASSVIEW, &TabCtrlItem );

    strHelp.LoadString(IDS_NAV_TARGETVIEW_TITLE);
    TabCtrlItem.pszText = (LPTSTR)LPCTSTR(strHelp);
    TabCtrlItem.iImage = NAV_HWVIEW;
    m_TabCtrl.InsertItem( NAV_HWVIEW, &TabCtrlItem );

    strHelp.LoadString(IDS_NAV_INSTANCEVIEW_TITLE);
    TabCtrlItem.pszText = (LPTSTR)LPCTSTR(strHelp);
    TabCtrlItem.iImage = NAV_INSTVIEW;
    m_TabCtrl.InsertItem( NAV_INSTVIEW, &TabCtrlItem );
}


//------------------------------------------------------------------*
/**
 * get filter data from active tree control
 *
 * @see             SetFilterData()
*/
void CNavigator::GetFilterData()
{
    int i;
    int iFltCnt;
    int iIndex;
    CStringArray    astrFltCat;
    CStringArray    astrFltText;
    int             iSelCat;
    int             iSelText;
    CNavTreeCtrl*   pActTreeCtrl = GetActiveTreeCtrl();

    if(!pActTreeCtrl)
    {
        return;
    }

    m_FltCatCtrl.SetRedraw(FALSE);
    m_FltCatCtrl.ResetContent();
    m_FltNameCtrl.SetRedraw(FALSE);
    m_FltNameCtrl.ResetContent();

    pActTreeCtrl->GetFilterData(iSelCat, iSelText, astrFltCat, astrFltText);
    iFltCnt = astrFltCat.GetSize();
    for(i = 0; i < iFltCnt; ++i)
    {
        if((iIndex = m_FltCatCtrl.AddString(astrFltCat[i])) >= 0)
        {
            m_FltCatCtrl.SetItemData(iIndex, i);     // later use i to select the right extension
        }
    }         
    iFltCnt = astrFltText.GetSize();
    for(i = 0; i < iFltCnt; ++i)
    {
        if((iIndex = m_FltNameCtrl.AddString(astrFltText[i])) >= 0)
        {
            m_FltNameCtrl.SetItemData(iIndex, i);     // later use i to select the right extension
        }
    }         
    m_FltCatCtrl.SetCurSel(iSelCat);
    m_FltCatCtrl.SetRedraw(TRUE);
    m_FltCatCtrl.Invalidate();

    m_FltNameCtrl.SetCurSel(iSelText);
    m_FltNameCtrl.SetRedraw(TRUE);
    m_FltNameCtrl.Invalidate();
}

//------------------------------------------------------------------*
/**
 * store filter data in active tree control
 *
 * @see             GetFilterData()
*/
void CNavigator::SetFilterData(BOOL bNamesChanged /*= FALSE*/)
{
    int i;
    int iFltCnt;
    CStringArray    astrFltText;
    CString         strText;
    int             iSelCat;
    int             iSelText;
    CNavTreeCtrl*   pActTreeCtrl = GetActiveTreeCtrl();   // active tree control

    if(!pActTreeCtrl)
    {
        return;
    }

    iSelCat = m_FltCatCtrl.GetCurSel();
    iSelText = m_FltNameCtrl.GetCurSel();
    if(bNamesChanged)
    {
        iFltCnt = m_FltNameCtrl.GetCount();
        for(i = 0; i < iFltCnt; ++i)
        {
            m_FltNameCtrl.GetLBText(i, strText);
            astrFltText.Add(strText);
        }
        pActTreeCtrl->SetFilterData(iSelCat, iSelText, &astrFltText);
    }
    else
    {
        pActTreeCtrl->SetFilterData(iSelCat, iSelText);
    }
}


//------------------------------------------------------------------*
/**
 * return pointer to currently active tree control
 *
*/
CNavTreeCtrl* CNavigator::GetActiveTreeCtrl()
{
    return GetTreeCtrl(m_ActualTreeMode);
}


//------------------------------------------------------------------*
/**
 * get tree control.
 *
 * @param           eTreeMode: one of file, class, instance, target or nothing
 * @return          pointer to tree control.
 * @exception       -
 * @see             -
*/
CNavTreeCtrl* CNavigator::GetTreeCtrl(NAV_TreeMode eTreeMode)
{
    CNavTreeCtrl*   pActTreeCtrl = NULL;
    switch(eTreeMode)
    {
    case NAV_FILEVIEW:
        pActTreeCtrl = &m_tFlTreeCtrl;
        break;
    case NAV_CLASSVIEW:
        pActTreeCtrl = &m_tClTreeCtrl;
        break;
    case NAV_INSTVIEW:
        pActTreeCtrl = &m_tInstTreeCtrl;
        break;
    case NAV_HWVIEW:
        pActTreeCtrl = &m_tHWTreeCtrl;
        break;
    }
    return(pActTreeCtrl);
}


//------------------------------------------------------------------*
/**
 * receiving WM_SIZE message
 *
 * @param           nType: not used
 * @param           cx: new width in pixels
 * @param           cy: new height in pixels
*/
void CNavigator::OnSize(UINT nType, int cx, int cy) 
{
    ResizeWindow(CRect(0,0,cx,cy));
}


//------------------------------------------------------------------*
/**
 * sizing of controls
 *
 *  the height of the filter category combo box and the height of
 *  the tabs of the tab control are fix.
 *  all other sizes are calculated based on these heights
 *  called by OnSize()
 *
 * @param           crt_new_rect: new client rect of window
 * @see             OnSize()
*/
void CNavigator::ResizeWindow(const CRect& crt_new_rect)
{
    CRect rOrigin;              // old client rect of navigator
    CRect rRect;                // helper variable
    CRect rFilterNameRect;      // new client rect of filter name combo
    CRect rFilterFrameRect;     // new client rect of filter frame
    CRect rFilterCatRect;       // new client rect of filter category
    CRect rTreeCtrl;            // new client rect of tree controls
    CRect rTabCtrl;             // new client rect of tab control
    CRect rTab;                 // window rect of tab view in client coordinates of navigator
    CRect rTabClient;           // client area of tab view
    CRect rTree;                // window rect of tree views in client coordinates of navigator
    CRect rCombo;               // window rect of combo boxes in client coordinates of navigator
    //int correction = 0;         // correction, if window height is to small
    CPoint topleft;             // top left point of navigator view
    int   i_tab_height;         // height of tab area in tab view (without client area)
    int   i_combo_height;       // height of filter name combo box

    int cx = crt_new_rect.right;
    int cy = crt_new_rect.bottom;

    // client rect is window rect - top left
    rOrigin = m_WindowRect;
    GetWindowRect(&rRect);
    topleft = rRect.TopLeft();
    rOrigin.OffsetRect(-topleft);

    // get window rect of filter category in client coordinates of navigator
    if(!m_FltCatCtrl.GetSafeHwnd())
        return;
    m_FltCatCtrl.GetWindowRect(&rCombo);
    i_combo_height = rCombo.Height();

    // calculate new coordinates for filter category and resize window
    rFilterCatRect.top = 4*con_i_dist_tab + con_i_offset;
    rFilterCatRect.bottom = rFilterCatRect.top + i_combo_height;
    rFilterCatRect.left = con_i_dist_combo;
    rFilterCatRect.right = cx - con_i_dist_combo;

    ::SetWindowPos(m_FltCatCtrl.GetSafeHwnd(), NULL,
        rFilterCatRect.left, rFilterCatRect.top, rFilterCatRect.Width(), rFilterCatRect.Height(),
        SWP_NOZORDER);

    // calculate new coordinates for filter name and resize window
    rFilterNameRect.top = 5*con_i_dist_tab + i_combo_height + con_i_offset;
    rFilterNameRect.bottom = rFilterNameRect.top + i_combo_height;
    rFilterNameRect.left = con_i_dist_combo;
    rFilterNameRect.right = cx - con_i_dist_combo;

    ::SetWindowPos(m_FltNameCtrl.GetSafeHwnd(), NULL,
        rFilterNameRect.left, rFilterNameRect.top, rFilterNameRect.Width(), rFilterNameRect.Height(),
        SWP_NOZORDER);

    // calculate new coordinates for filter frame and resize window
    rFilterFrameRect.top = con_i_dist_tab + con_i_offset;
    rFilterFrameRect.bottom = rFilterNameRect.bottom + con_i_dist_tab;
    rFilterFrameRect.left = con_i_dist_tab;
    rFilterFrameRect.right = cx - con_i_dist_tab;

    ::SetWindowPos(m_FltFrameCtrl.GetSafeHwnd(), NULL,
        rFilterFrameRect.left, rFilterFrameRect.top, rFilterFrameRect.Width(), rFilterFrameRect.Height(),
        SWP_NOZORDER);

    // get tab height (height of tabs alone)
    if(!m_TabCtrl.GetSafeHwnd())
        return;
    m_TabCtrl.GetWindowRect(&rTab);
    m_TabCtrl.GetClientRect(&rTabClient);
    i_tab_height = rTabClient.Height();
    m_TabCtrl.AdjustRect(FALSE, &rTabClient);
    i_tab_height -= rTabClient.Height();
    rTab.OffsetRect(-topleft);


    // calculate new coordinates for tab control and resize window
    rTabCtrl.top = rFilterFrameRect.bottom + con_i_dist_tab;
    rTabCtrl.bottom = cy - con_i_dist_tab - con_i_dist_tab;
    rTabCtrl.left = con_i_dist_tab;
    rTabCtrl.right = cx - con_i_dist_tab;

    ::SetWindowPos(m_TabCtrl.GetSafeHwnd(), NULL,
        rTabCtrl.left, rTabCtrl.top, rTabCtrl.Width(), rTabCtrl.Height(),
        SWP_NOZORDER);

    // calculate new coordinates for tree controls and resize windows
    rTreeCtrl.top = rTabCtrl.top + i_tab_height + con_i_dist_tree - con_i_dist_tab;
    rTreeCtrl.bottom = cy - con_i_dist_tree - con_i_dist_tab;
    rTreeCtrl.left = con_i_dist_tree;
    rTreeCtrl.right = cx - con_i_dist_tree;

    ::SetWindowPos(m_tClTreeCtrl.GetSafeHwnd(), NULL,
        rTreeCtrl.left, rTreeCtrl.top, rTreeCtrl.Width(), rTreeCtrl.Height(),
        SWP_NOZORDER);
    ::SetWindowPos(m_tFlTreeCtrl.GetSafeHwnd(), NULL,
        rTreeCtrl.left, rTreeCtrl.top, rTreeCtrl.Width(), rTreeCtrl.Height(),
        SWP_NOZORDER);
    ::SetWindowPos(m_tHWTreeCtrl.GetSafeHwnd(), NULL,
        rTreeCtrl.left, rTreeCtrl.top, rTreeCtrl.Width(), rTreeCtrl.Height(),
        SWP_NOZORDER);
    ::SetWindowPos(m_tInstTreeCtrl.GetSafeHwnd(), NULL,
        rTreeCtrl.left, rTreeCtrl.top, rTreeCtrl.Width(), rTreeCtrl.Height(),
        SWP_NOZORDER);

    //ShowActualTree();
    //PopulateTree();
    GetWindowRect(&m_WindowRect);
}

//------------------------------------------------------------------*
/**
 * timer event
 *
 *  look for external changes in project folder
 *
 * @param           nIDEvent: timer event id
*/
void CNavigator::OnTimer(UINT nIDEvent) 
{
	CSizedDialogBar::OnTimer(nIDEvent);

    if(nIDEvent == m_IDCheckEvent)
    {
        // this is just a test:
        //m_pProjMan->ReparseCheck();
        switch(m_ActualTreeMode)
        {
        case NAV_CLASSVIEW:
            break;
        case NAV_FILEVIEW:
            m_tFlTreeCtrl.Check();
            break;
        case NAV_HWVIEW:
            break;
        case NAV_INSTVIEW:
            break;
        }
    }
}

// on creation of navigator
int CNavigator::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    // activate drop files
    DragAcceptFiles();

	if (CSizedDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

// probably too early
//void CNavigator::OnPaint() 
//{
//	CPaintDC dc(this); // device context for painting
	
//    if(!m_bInitialized)
//    {
//        Init();
//    }
	
	// Do not call CSizedDialogBar::OnPaint() for painting messages
//}


// Load a project
//void CNavigator::LoadProject(LPCTSTR lpszPathName)
//{
//    // only preliminary: hard coded
//    //CComBSTR    bstrPath("C:\\Source\\4ControlV2\\bin\\Projects\\Test");
//    //CComBSTR    bstrProj("Test");
//
//    //HRESULT hr = m_pProjMan->OpenProject(bstrPath, bstrProj);
//
//    // load file view
//    m_TabCtrl.SetCurSel(NAV_CLASSVIEW);
//    m_tFlTreeCtrl.Load(lpszPathName);
//    //m_tClTreeCtrl.Load(lpszPathName);
//    UpdateWindow();
//    //m_IDCheckEvent = SetTimer(m_IDCheckEvent, 5000, 0);
//}



// Save a project
//void CNavigator::SaveProject(LPCTSTR lpszPathName)
//{
//    // preliminary
//    UnadviseProjEventHandler();
//    m_tFlTreeCtrl.Unload();
//    m_tClTreeCtrl.Unload();
//    m_tHWTreeCtrl.Unload();
//    m_tInstTreeCtrl.Unload();
//    //m_pProjMan->CloseProject();
//    m_tFlTreeCtrl.Save();
//}

// change view
void CNavigator::OnSelchangeTabNavigator(NMHDR* pNMHDR, LRESULT* pResult)
{
    // store filters of old view
    SetFilterData(FALSE);
    m_ActualTreeMode = (NAV_TreeMode)m_TabCtrl.GetCurSel();

    ShowActualTree();

    *pResult = 0;
}

// show currently active tree
void CNavigator::ShowActualTree(BOOL bChange /*=FALSE*/)
{
    if(!bChange)
    {
        m_ActualTreeMode = (NAV_TreeMode)m_TabCtrl.GetCurSel();
    }
    m_tClTreeCtrl.ShowWindow(SW_HIDE);
    m_tFlTreeCtrl.ShowWindow(SW_HIDE);
    m_tHWTreeCtrl.ShowWindow(SW_HIDE);
    m_tInstTreeCtrl.ShowWindow(SW_HIDE);
    m_FltNameCtrl.EnableWindow(TRUE); // enable the name combobox
    switch(m_ActualTreeMode)
    {
    case NAV_CLASSVIEW:
        m_tClTreeCtrl.CheckSourceState();
        m_tClTreeCtrl.ShowWindow(SW_SHOW);
        m_tClTreeCtrl.SetWindowPos (&wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        break;
    case NAV_FILEVIEW:
        m_tFlTreeCtrl.CheckSourceState();
        m_tFlTreeCtrl.ShowWindow(SW_SHOW);
        m_tFlTreeCtrl.SetWindowPos (&wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        break;              
    case NAV_HWVIEW:
        m_tHWTreeCtrl.CheckSourceState();
        m_tHWTreeCtrl.ShowWindow(SW_SHOW);
        m_tHWTreeCtrl.SetWindowPos (&wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        break;              
    case NAV_INSTVIEW:
        m_tInstTreeCtrl.CheckSourceState();
        m_FltNameCtrl.EnableWindow(FALSE); // disable the name combobox
        m_tInstTreeCtrl.ShowWindow(SW_SHOW);
        m_tInstTreeCtrl.SetWindowPos (&wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        break;              
    }
    GetFilterData();
    Invalidate();
}

// selection of filter name from list
void CNavigator::OnSelendokFilterName() 
{
    CString str;
    int     iIndex;

    // if not yet present, add filter
    m_FltNameCtrl.GetWindowText(str);
    if(!str.IsEmpty () && m_FltNameCtrl.FindStringExact(0, str) < 0)
    {
        m_FltNameCtrl.AddString(str);
        SetFilterData(TRUE);
    }
    // the selected filter is activated
    if((iIndex = m_FltNameCtrl.GetCurSel()) != LB_ERR)
    {
        m_FltNameCtrl.GetLBText(iIndex, str);
        m_FltNameCtrl.SetWindowText(str);
        SetFilterData(FALSE);
        OnEditchangeFilterName();
    }
}

// filter name changes
void CNavigator::OnEditchangeFilterName() 
{
    // same happens as with changing filter category
    OnSelchangeFilterCat();
}

//------------------------------------------------------------------*
/**
 * receive message that selection of filter category changes
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CNavigator::OnSelchangeFilterCat() 
{
    //CString strFilter1;
    CString strFilter2;
    CNavTreeCtrl*   pActTreeCtrl = GetActiveTreeCtrl();
    int iSelCat = m_FltCatCtrl.GetCurSel();

    //m_FltCatCtrl.GetWindowText(strFilter1);
    m_FltNameCtrl.GetWindowText(strFilter2);

    if(pActTreeCtrl)
    {
        pActTreeCtrl->SetFilters(iSelCat, strFilter2);
    }
}

//------------------------------------------------------------------*
/**
 * insert new filter expression
 *
 * @param           rstrFilter: new wildcard expression for text filter
 *
*/
void CNavigator::InsertNewFilter(const CString& rstrFilter)
{
    int iIndex;

    if (!rstrFilter.IsEmpty ())
    {
        if((iIndex = m_FltNameCtrl.FindStringExact(0, rstrFilter)) >= 0)
        {
            m_FltNameCtrl.SetCurSel(iIndex);
        }
        else
        {
            m_FltNameCtrl.SetCurSel(m_FltNameCtrl.AddString(rstrFilter));
        }
    }
}

//------------------------------------------------------------------*
/**
 * receive message that an item is expanded or collapsed in file tree
 *
*/
void CNavigator::OnItemexpandingFlTREE(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
    BOOL    b_expand;

    if(pNMTreeView->action == TVE_EXPAND)
        b_expand = TRUE;
    else
        b_expand = FALSE;

    // inform file tree control
    if(!m_tFlTreeCtrl.OnItemexpanding(pNMTreeView->itemNew, b_expand))
    {
        *pResult = 1;
        return;
    }
	
	*pResult = 0;
}

//------------------------------------------------------------------*
/**
 * receive message that an item is expanded or collapsed in hardware (target) tree
 *
*/
void CNavigator::OnItemexpandingHwtree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
    BOOL    b_expand;

    if(pNMTreeView->action == TVE_EXPAND)
        b_expand = TRUE;
    else
        b_expand = FALSE;

    // inform file tree control
    if(!m_tHWTreeCtrl.OnItemexpanding(pNMTreeView->itemNew, b_expand))
    {
        *pResult = 1;
        return;
    }
	
	*pResult = 0;
}

//------------------------------------------------------------------*
/**
 * receive message that an item is expanded or collapsed in instance tree
 *
*/
void CNavigator::OnItemexpandingInsttree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
    BOOL    b_expand;

    if(pNMTreeView->action == TVE_EXPAND)
        b_expand = TRUE;
    else
        b_expand = FALSE;

    // inform file tree control
    if(!m_tInstTreeCtrl.OnItemexpanding(pNMTreeView->itemNew, b_expand))
    {
        *pResult = 1;
        return;
    }
	
	*pResult = 0;
}

//------------------------------------------------------------------*
/**
 * receive message that an item is expanded or collapsed in class tree
 *
*/
void CNavigator::OnItemexpandingClTREE(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
    BOOL    b_expand;

    if(pNMTreeView->action == TVE_EXPAND)
        b_expand = TRUE;
    else
        b_expand = FALSE;

    // inform file tree control
    if(!m_tClTreeCtrl.OnItemexpanding(pNMTreeView->itemNew, b_expand))
    {
        *pResult = 1;
        return;
    }
	
	*pResult = 0;
}


//------------------------------------------------------------------*
/**
 * receive message that control for filter category combo box looses focus
 *
*/
void CNavigator::OnKillfocusFilterName() 
{
    CString str;

    // add filter to control if not yet present
    m_FltNameCtrl.GetWindowText(str);
    InsertNewFilter(str);
    SetFilterData(TRUE);
}


void CNavigator::DoContextMenuCommandForNode(const CString& crstrIdPath, UINT nID)
{
    NAV_TreeMode    eTreeMode;
    CNavTreeCtrl*   pNavTreeCtrl = GetTreeForIdPath(crstrIdPath, eTreeMode);
    if(pNavTreeCtrl)
    {
        pNavTreeCtrl->OnCommand(nID);
    }    
}


//------------------------------------------------------------------*
/**
 * context menu commands.
 *
 *  they are just delegated to the active tree control
 *
 * @param           nID: ID of the selected context menu item          
 * @see             CFileTreeCtrl::OnCommand and CNavTreeCtrl::OnCommand()
*/
void CNavigator::OnContextMenuTreeCommand(UINT nID)
{
    switch(m_ActualTreeMode)
    {
    case NAV_CLASSVIEW:
        m_tClTreeCtrl.OnCommand(nID);
        break;
    case NAV_FILEVIEW:
        m_tFlTreeCtrl.OnCommand(nID);
        break;
    case NAV_HWVIEW:
        m_tHWTreeCtrl.OnCommand(nID);
        break;
    case NAV_INSTVIEW:
        m_tInstTreeCtrl.OnCommand(nID);
        break;
    }
}

void CNavigator::OpenNode(const CString& crstrIdPath)
{
    NAV_TreeMode    eTreeMode;
    CNavTreeCtrl*   pNavTreeCtrl = GetTreeForIdPath(crstrIdPath, eTreeMode);
    if(pNavTreeCtrl)
    {
        pNavTreeCtrl->OpenNode(crstrIdPath);
    }
}

BOOL CNavigator::IsKeyMouseActionAllowed(const CString& crstrIdPath, int iKeyMouseEvent)
{
    NAV_TreeMode    eTreeMode;
    CNavTreeCtrl*   pNavTreeCtrl = GetTreeForIdPath(crstrIdPath, eTreeMode);
    if(pNavTreeCtrl)
    {
        return pNavTreeCtrl->IsKeyMouseActionAllowed(crstrIdPath, iKeyMouseEvent);
    }
    return FALSE;
}

void CNavigator::RenameNode(const CString& crstrIdPath, const CString& crstrNewId)
{
    NAV_TreeMode    eTreeMode;
    CNavTreeCtrl*   pNavTreeCtrl = GetTreeForIdPath(crstrIdPath, eTreeMode);
    if(pNavTreeCtrl)
    {
        pNavTreeCtrl->RenameNode(crstrIdPath, crstrNewId);
    }
}


//------------------------------------------------------------------*
/**
 * extend stringray docking context menu
 *
 *  unfortunately pMenu is not filled yet
 *
 * @param           pMenu: menu to be extended
*/
//void CNavigator::OnExtendContextMenu(CMenu* pMenu)
//{
//    switch(m_ActualTreeMode)
//    {
//    case NAV_CLASSVIEW:
//        m_tClTreeCtrl.AddContextMenu(pMenu);
//        break;
//    case NAV_FILEVIEW:
//        m_tFlTreeCtrl.AddContextMenu(pMenu);
//        break;
//    case NAV_HWVIEW:
//        m_tHWTreeCtrl.AddContextMenu(pMenu);
//        break;
//    case NAV_INSTVIEW:
//        m_tInstTreeCtrl.AddContextMenu(pMenu);
//        break;
//    }
//}


//------------------------------------------------------------------*
/**
 * on context menu
 *
 * @param           pWnd: pointer to current window
 * @return          point: mouse position
*/
void CNavigator::OnContextMenu(CWnd* pWnd, CPoint point) 
{
    // only if tree control does not show context menu, call stringray context menu
    CNavTreeCtrl*   pActTreeCtrl = GetActiveTreeCtrl();
    if(!pActTreeCtrl->ShowContextMenu(pWnd, point))
    {
        // this would call OnExtendContextMenu() (not used any more)
        CSizedDialogBar::OnContextMenu(pWnd, point);
    }
}

// SHIFT_F10 14.12.04 SIS >>
void CNavigator::OnNavContextMenu()
{
	CRect rect;
	GetClientRect(rect);
	ClientToScreen(rect);
	CPoint pt = rect.CenterPoint();
	SendMessage(WM_CONTEXTMENU, (WPARAM)m_hWnd, MAKELPARAM(pt.x, pt.y));
}
// SHIFT_F10 14.12.04 SIS <<


//------------------------------------------------------------------*
/**
 *  connection point to project manager is activated
 *
*/
HRESULT CNavigator::AdviseProjEventHandler()
{
    HRESULT hr;
    CComPtr<IUnknown>               pTmp;
    CComQIPtr<ICEProjectManager>    pProjMan;

    pTmp.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
    pProjMan = pTmp;
    if(!pProjMan)
    {
        return (E_FAIL);
    }

    ASSERT(m_pProjEventHandler == NULL);
    hr = CComObject<CProjManEventsImpl>::CreateInstance (&m_pProjEventHandler);
    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        return (hr);
    }
    ASSERT (m_pProjEventHandler!= NULL);
    if (m_pProjEventHandler == NULL)
    {
        return (E_UNEXPECTED);
    }
    m_pProjEventHandler->AddRef ();

    hr = AtlAdvise (pProjMan, m_pProjEventHandler->GetUnknown (), 
                    IID__ICEProjectManagerEvents, &m_dwProjEventHandlerCookie);
    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        return (hr);
    }
    ASSERT (m_dwProjEventHandlerCookie != NULL);
    if (m_dwProjEventHandlerCookie == NULL)
    {
        m_pProjEventHandler->Release ();
        m_pProjEventHandler = NULL;
        return (E_UNEXPECTED);
    }

    hr = AtlAdvise (pProjMan, m_pProjEventHandler->GetUnknown(), 
                    IID__ICEProjManInstanceEvents, &m_dwProjInstEventHandlerCookie);
    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        return (hr);
    }
    ASSERT (m_dwProjInstEventHandlerCookie != NULL);

    m_pProjEventHandler->SetNavigatorPtr(this);

    return (S_OK);
}


//------------------------------------------------------------------*
/**
 *  connection point to project manager is deactivated
 *
*/
void CNavigator::UnadviseProjEventHandler()
{
    HRESULT hr;

    CComPtr<IUnknown>               pTmp;
    CComQIPtr<ICEProjectManager>    pProjMan;

    pTmp.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
    pProjMan = pTmp;
    if(!pProjMan)
    {
        return;
    }

    ASSERT (m_dwProjEventHandlerCookie != NULL);
    if (m_dwProjEventHandlerCookie == NULL)
    {
        return;
    }

    ASSERT (pProjMan != NULL);
    if (pProjMan == NULL)
    {
        return;
    }

    hr = AtlUnadvise (pProjMan, 
                      IID__ICEProjectManagerEvents, 
                      m_dwProjEventHandlerCookie);

    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        ;
    }

    hr = AtlUnadvise (pProjMan, 
                      IID__ICEProjManInstanceEvents, 
                      m_dwProjInstEventHandlerCookie);

    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        ;
    }

    if (m_pProjEventHandler != NULL)
    {
        m_pProjEventHandler->SetNavigatorPtr(NULL);
        m_pProjEventHandler->Release();
        m_pProjEventHandler = NULL;
    }
}

//------------------------------------------------------------------*
/**
 * Initilize tree control
 *
 *  initialize tree control and set all necessary XML pointers
 *
 * @param           eNavTree: tree to be initialized
 * @param           pXMLDoc: pointer to XML document
 * @see             BuildTree()
*/
void CNavigator::InitTree(NAV_TreeMode eNavTree, CXMLDocument* pXMLDoc)
{
    switch(eNavTree)
    {
    case NAV_CLASSVIEW:
        m_tClTreeCtrl.InitializeDOM(pXMLDoc);
        break;
    case NAV_HWVIEW:
        m_tHWTreeCtrl.InitializeDOM(pXMLDoc);
        break;
    case NAV_INSTVIEW:
        m_tInstTreeCtrl.InitializeDOM(pXMLDoc);
        break;
    }
}



//------------------------------------------------------------------*
/**
 * Build tree from XML document.
 *
 *  after tree is builded use UpdateTreeNode() and InvalidateTree() to
 *  reflect changes in XML document
 *
 * @param           eNavTree: tree to be builded
 * @param           pXMLDoc: pointer to XML document for build
 * @see             UpdateTreeNode() and InvalidateTree()
*/
void CNavigator::BuildTree(NAV_TreeMode eNavTree)
{
    switch(eNavTree)
    {
    case NAV_CLASSVIEW:
        m_tClTreeCtrl.BuildTree();
        break;
    case NAV_HWVIEW:
        m_tHWTreeCtrl.BuildTree();
        break;
    case NAV_INSTVIEW:
        m_tInstTreeCtrl.BuildTree();
        break;
    }
}

//------------------------------------------------------------------*
/**
 * Update node of specified tree.
 *
 *  calls UpdateNode command of specified tree
 *  after calling UpdateTreeNode for all nodes call InvalidateTree to
 *  make changes visible
 *
 * @param           eNavTree: tree to update
 * @param           crstrIdPath: id path to tree node that should be updated
 * @see             InvalidateTree(), CNavTreeCtrl::UpdateNode()
*/
void CNavigator::UpdateTreeNode(NAV_TreeMode eNavTree, const CString& crstrIdPath)
{
    switch(eNavTree)
    {
    case NAV_CLASSVIEW:
        m_tClTreeCtrl.UpdateNode(crstrIdPath);
        break;
    case NAV_HWVIEW:
        m_tHWTreeCtrl.UpdateNode(crstrIdPath);
        break;
    case NAV_INSTVIEW:
        m_tInstTreeCtrl.UpdateNode(crstrIdPath);
        break;
    }
}


//------------------------------------------------------------------*
/**
 * Update node attributes of specified tree.
 *
 *  calls UpdateAttributes command of specified tree
 *  after calling UpdateTreeNode for all nodes call InvalidateTree to
 *  make changes visible
 *
 * @param           eNavTree: tree to update
 * @param           crstrIdPath: id path to tree node that should be updated
 * @see             InvalidateTree(), CNavTreeCtrl::UpdateAttributes()
*/
void CNavigator::UpdateAttributes(NAV_TreeMode eNavTree, const CString& crstrIdPath)
{
    switch(eNavTree)
    {
    case NAV_CLASSVIEW:
        m_tClTreeCtrl.UpdateAttributes(crstrIdPath);
        break;
    case NAV_HWVIEW:
        m_tHWTreeCtrl.UpdateAttributes(crstrIdPath);
        break;
    case NAV_INSTVIEW:
        m_tInstTreeCtrl.UpdateAttributes(crstrIdPath);
        break;
    }
}


//------------------------------------------------------------------*
/**
 * Update source state of specified tree.
 *
 *  calls UpdateSourceState command of specified tree
 *  after calling UpdateTreeNode for all nodes call InvalidateTree to
 *  make changes visible
 *
 * @param           eNavTree: tree to update
 * @param           crstrIdPath: id path to tree node that should be updated
 * @see             InvalidateTree(), CNavTreeCtrl::UpdateAttributes()
*/
//void CNavigator::UpdateSourceState(NAV_TreeMode eNavTree, const CString& crstrIdPath)
//{
//    switch(eNavTree)
//    {
//    case NAV_CLASSVIEW:
//        m_tClTreeCtrl.UpdateSourceState(crstrIdPath);
//        break;
//    case NAV_HWVIEW:
//        m_tHWTreeCtrl.UpdateSourceState(crstrIdPath);
//        break;
//    case NAV_INSTVIEW:
//        m_tInstTreeCtrl.UpdateSourceState(crstrIdPath);
//        break;
//    }
//    // must be called additionally
//    m_tFlTreeCtrl.UpdateSourceState(crstrIdPath);
//}

//------------------------------------------------------------------*
/**
 * Delete node in specified tree.
 *
 *  calls DeleteNode command of specified tree
 *  after calling DeleteNode call InvalidateTree to
 *  make changes visible
 *
 * @param           eNavTree: tree to delete
 * @param           crstrIdPath: id path to tree node that should be deleted
 * @see             InvalidateTree(), CNavTreeCtrl::UpdateDelete()
*/
void CNavigator::UpdateDelete(NAV_TreeMode eNavTree, const CString& crstrIdPath)
{
    switch(eNavTree)
    {
    case NAV_CLASSVIEW:
        m_tClTreeCtrl.UpdateDelete(crstrIdPath);
        break;
    case NAV_HWVIEW:
        m_tHWTreeCtrl.UpdateDelete(crstrIdPath);
        break;
    case NAV_INSTVIEW:
        m_tInstTreeCtrl.UpdateDelete(crstrIdPath);
        break;
    }
}
//------------------------------------------------------------------*
/**
 * Insert node of specified tree.
 *
 *  calls UpdateInsert command of specified tree
 *  after calling UpdateInsert for all nodes call InvalidateTree to
 *  make changes visible
 *
 * @param           eNavTree: tree to update
 * @param           crstrIdPath: id path to tree node that should be inserted
 * @see             InvalidateTree(), CNavTreeCtrl::UpdateInsert()
*/
void CNavigator::UpdateInsert(NAV_TreeMode eNavTree, const CString& crstrIdPath)
{
    switch(eNavTree)
    {
    case NAV_CLASSVIEW:
        m_tClTreeCtrl.UpdateInsert(crstrIdPath);
        break;
    case NAV_HWVIEW:
        m_tHWTreeCtrl.UpdateInsert(crstrIdPath);
        break;
    case NAV_INSTVIEW:
        m_tInstTreeCtrl.UpdateInsert(crstrIdPath);
        break;
    }
}

//------------------------------------------------------------------*
/**
 * Invalidates specified tree.
 *
 *  used in context of updating tree
 *
 * @param           eNavTree: tree to invalidate
 * @see             UpdateTreeNode()
*/
void CNavigator::InvalidateTree(NAV_TreeMode eNavTree)
{
    switch(eNavTree)
    {
    case NAV_CLASSVIEW:
        m_tClTreeCtrl.InvalidateTree();
        break;
    case NAV_HWVIEW:
        m_tHWTreeCtrl.InvalidateTree();
        break;
    case NAV_INSTVIEW:
        m_tInstTreeCtrl.InvalidateTree();
        break;
    }
}

//------------------------------------------------------------------*
/**
 * This callback method has been registered with the state manager.
 *
 * @see             CCEStateManager
*/
void CNavigator::OnStateChange (CEStateNotify stateNotify, unsigned int uiStage)
{
    switch(stateNotify)
    {
    case CEHandsOffComponentNotify:
        {
            CString strName = CESMGetComponentName ();
            if (strName == CE_COMPMAN_EXE)
            {
                // get exe delegator and close all open sessions
                CComPtr<IUnknown>       pTmp;
                CComQIPtr<ICEExecute>   pExeDelegator;

                pTmp.Attach(CEGetCOM(CE_COMPMAN_EXE));

                pExeDelegator = pTmp;
                if(pExeDelegator)
                {
                    pExeDelegator->CloseAllExeSessions(FALSE);
                }
            }
        }
        break;
    case CECreateComponentNotify:
        {
            CString strName = CESMGetComponentName ();
            if (strName == CE_COMPMAN_NAV)
            {
                Init();
            }
        }
        break;
    case CEPreOpenProjectNotify:
        AdviseProjEventHandler();
        break;
    case CEPostOpenProjectNotify:
        m_tFlTreeCtrl.Load();
        m_tClTreeCtrl.Load();
        m_tHWTreeCtrl.Load();
        m_tInstTreeCtrl.Load();
        m_bProjectLoaded = TRUE;
//        m_IDCheckEvent = SetTimer(m_IDCheckEvent, 2000, 0);
        break;
    case CEPreCloseProjectNotify:
//        KillTimer(m_IDCheckEvent);
        UnadviseProjEventHandler();
        m_tFlTreeCtrl.ClearStates();
        m_tHWTreeCtrl.ClearStates();
        m_tInstTreeCtrl.ClearStates();
        m_bProjectLoaded = FALSE;
        break;
    case CEPostCloseProjectNotify:
        // reset tree controls
        m_tFlTreeCtrl.Unload();
        m_tClTreeCtrl.Unload();
        m_tHWTreeCtrl.Unload();
        m_tInstTreeCtrl.Unload();
        break;
    case CEAddTargetNotify:
        {
            CTargetOnlBase* pTargetBase;
//            CTargetOnl*     pTarget;

            pTargetBase = CESMGetTarget();
            ASSERT (pTargetBase != NULL);
            if (pTargetBase != NULL)
            {
                pTargetBase->RegisterConnectStateCB(&m_ConnectStateCB);
                pTargetBase->RegisterSessionCB(&m_SessionCB);
//                pTarget = dynamic_cast<CTargetOnl*> (pTargetBase);
//                if(pTarget)
//                {
//                    pTarget->RegisterExclusiveSessionCB(&m_DebugSessionCB);
//                }
            }

            // When adding a Target first the project is parsed and then the
            // the Target is added to the Target set. Thus we have to repopulate
            // the hardware tree as soon as the Target is added to the Target set
            // to get the machine address of the Target for displaying it in
            // the hardware view. Fine tuning would be to just change the
            // single Target tree item.

//            if (m_ActualTreeMode == NAV_HWVIEW)
//            {
//                m_HWModified = true;
//                m_tHWTreeCtrl->UpdateStates(pTargetBase);
//            }
        }
        break;
    case CERemoveTargetNotify:
        {
            CTargetOnlBase* pTargetBase;
//            CTargetOnl*     pTarget;

            pTargetBase = CESMGetTarget();
            ASSERT (pTargetBase != NULL);
            if (pTargetBase != NULL)
            {
                pTargetBase->UnregisterConnectStateCB(&m_ConnectStateCB);
                pTargetBase->UnregisterSessionCB(&m_SessionCB);
//                pTarget = dynamic_cast<CTargetOnl*> (pTargetBase);
//                if(pTarget)
//                {
//                    pTarget->UnregisterExclusiveSessionCB(&m_DebugSessionCB);
//                }
            }
        }
        break;
    }
}


//------------------------------------------------------------------*
/**
 * Is navigator visible?.
 *
 * @return          visible?
*/
BOOL CNavigator::IsVisible()
{
    return(IsWindowVisible());
}


//------------------------------------------------------------------*
/**
 * invalidate file view 
 *
 *  update root node of file view
 *
*/
void CNavigator::InvalidateFileView()
{
    m_tFlTreeCtrl.UpdateNode("");
}

void CNavigator::OnFileAdded(const CString& crstrFilePath, enum E_FV_FileCategory eFileCat)
{
    m_tFlTreeCtrl.OnFileAdded(crstrFilePath, eFileCat);
}

void CNavigator::OnFileRemoved(const CString& crstrFilePath, enum E_FV_FileCategory eFileCat)
{
    m_tFlTreeCtrl.OnFileRemoved(crstrFilePath, eFileCat);
}



//void CNavigator::OnDblclkFlTREE (NMHDR *pNMHDR, LRESULT *pResult)
//{
//    m_tFlTreeCtrl.OnDblclk(pNMHDR, pResult);
//}


//void CNavigator::OnDblclkClTREE(NMHDR* pNMHDR, LRESULT* pResult)
//{
//    m_tClTreeCtrl.OnDblclk(pNMHDR, pResult);
//}


//void CNavigator::OnDblclkHwtree(NMHDR* pNMHDR, LRESULT* pResult)
//{
//    m_tHWTreeCtrl.OnDblclk(pNMHDR, pResult);
//}


//void CNavigator::OnDblclkInsttree(NMHDR* pNMHDR, LRESULT* pResult)
//{
//    m_tInstTreeCtrl.OnDblclk(pNMHDR, pResult);
//}

void CNavigator::OnBeginlabeleditFlTREE(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;

    m_tFlTreeCtrl.OnBeginlabeledit(pTVDispInfo, pResult);
}

void CNavigator::OnEndlabeleditFlTREE(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;

    m_tFlTreeCtrl.OnEndlabeledit(pTVDispInfo, pResult);
}

void CNavigator::OnBeginlabeleditClTREE(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;

    m_tClTreeCtrl.OnBeginlabeledit(pTVDispInfo, pResult);
}

void CNavigator::OnEndlabeleditClTREE(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;

    m_tClTreeCtrl.OnEndlabeledit(pTVDispInfo, pResult);
}


void CNavigator::OnBeginlabeleditHwtree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;

    m_tHWTreeCtrl.OnBeginlabeledit(pTVDispInfo, pResult);
}

void CNavigator::OnEndlabeleditHwtree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	
    m_tHWTreeCtrl.OnEndlabeledit(pTVDispInfo, pResult);
}

void CNavigator::OnBeginlabeleditInsttree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;

    m_tInstTreeCtrl.OnBeginlabeledit(pTVDispInfo, pResult);
}

void CNavigator::OnEndlabeleditInsttree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;

    m_tInstTreeCtrl.OnEndlabeledit(pTVDispInfo, pResult);
}

void CNavigator::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    CNavTreeCtrl*   pActTreeCtrl = GetActiveTreeCtrl();
//    if(pActTreeCtrl)
//    {
//        pActTreeCtrl->OnKeyDown(nChar, nRepCnt, nFlags);
//    }
	CSizedDialogBar::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CNavigator::OnBegindragClTREE(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	
    m_tClTreeCtrl.OnBegindrag(pNMTreeView, pResult);
}

void CNavigator::OnBegindragFlTREE(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

    m_tFlTreeCtrl.OnBegindrag(pNMTreeView, pResult);
}


void CNavigator::OnBegindragHwtree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

    m_tHWTreeCtrl.OnBegindrag(pNMTreeView, pResult);
}

void CNavigator::OnBegindragInsttree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

    m_tInstTreeCtrl.OnBegindrag(pNMTreeView, pResult);
}

void CNavigator::OnKeydownFlTREE(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_KEYDOWN* pTVKeyDown = (TV_KEYDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}


//------------------------------------------------------------------*
/**
 * get standard docking height
 *
 * @return          docking height in pixels
 * @see             CSizedDialogBar
 *
*/
int CNavigator::GetStandardDockingHeight()
{
    return(300);
}

//------------------------------------------------------------------*
/**
 * on idle method.
 *
 * @param           lCount: idle count
 * @return          need more time?
 * @see             
*/
BOOL CNavigator::OnIdle(LONG lCount)
{
    // allow on idle only all 3 seconds
    BOOL    bReturn = FALSE;
    static  CTime statLastTime = CTime::GetCurrentTime();
    CTime   tTimeTmp = CTime::GetCurrentTime();
    CTimeSpan   tTimeSpan(0,0,0,3);
    if((tTimeTmp - tTimeSpan) < statLastTime)
    {
        return FALSE;
    }

    statLastTime = CTime::GetCurrentTime();

    if(m_bProjectLoaded)
    {
        CNavTreeCtrl*   pTreeCtrl;
        pTreeCtrl = GetActiveTreeCtrl();
        if(pTreeCtrl)
        {
            pTreeCtrl->CheckSourceState();
        }
    }
//    if(m_tInstTreeCtrl.CheckStateVariables(this))
//    {
//        bReturn = TRUE;
//    }
    return bReturn;
}



//------------------------------------------------------------------*
/**
 * pre translate message.
 *
 *  overwritten to get accelerators keys
 *
*/
BOOL CNavigator::PreTranslateMessage(MSG* pMsg) 
{
    if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
    {
        if (m_hAccelerators != NULL && ::TranslateAccelerator (m_hWnd, m_hAccelerators, pMsg))
        {
            return (TRUE);
        }
    }
	return CSizedDialogBar::PreTranslateMessage(pMsg);
}

//------------------------------------------------------------------*
/**
 * show context menu for node
 *
 * @param           pMenu: pointer to menu
 * @param           crstrIdPath: id path to node
 * @return          -
 * @exception       -
 * @see             -
*/
void CNavigator::AddContextMenuForNode(CMenu* pMenu, const CString& crstrIdPath)
{
    NAV_TreeMode    eTreeMode;
    CNavTreeCtrl*   pNavTreeCtrl = GetTreeForIdPath(crstrIdPath, eTreeMode);
    if(pNavTreeCtrl)
    {
        pNavTreeCtrl->AddContextMenuForNode(pMenu, crstrIdPath);
    }
}


//------------------------------------------------------------------*
/**
 * get tree for id path.
 *
 * @param           crstrIdPath: id path to node
 * @param           [in/out] reTreeMode: file, class, instance, target or nothing
 * @return          pointer to tree
*/
CNavTreeCtrl* CNavigator::GetTreeForIdPath(const CString& crstrIdPath, NAV_TreeMode& reTreeMode)
{
    if(m_tClTreeCtrl.IsTreePath(crstrIdPath))
    {
        reTreeMode = NAV_CLASSVIEW;
        return &m_tClTreeCtrl;
    }
    if(m_tHWTreeCtrl.IsTreePath(crstrIdPath))
    {
        reTreeMode = NAV_HWVIEW;
        return &m_tHWTreeCtrl;
    }
    if(m_tInstTreeCtrl.IsTreePath(crstrIdPath))
    {
        reTreeMode = NAV_INSTVIEW;
        return &m_tInstTreeCtrl;
    }
    if(m_tFlTreeCtrl.IsTreePath(crstrIdPath))
    {
        reTreeMode = NAV_FILEVIEW;
        return &m_tFlTreeCtrl;
    }
    reTreeMode = NAV_NOVIEW;
    return NULL;
}


void CNavigator::DeleteNodesByIdPath(CStringArray& rastrIdPath)
{
    // switch off file checking during delete operation
    m_tFlTreeCtrl.EnableFileCheck(FALSE);

    m_tClTreeCtrl.DeleteNodesByIdPath(rastrIdPath);
    m_tHWTreeCtrl.DeleteNodesByIdPath(rastrIdPath);

    Parse();

    m_tFlTreeCtrl.EnableFileCheck(TRUE);
}


void CNavigator::Parse()
{
    // trigger parsing
    CComQIPtr<ICEProjInfo>  pProjInfo;
    CComPtr<IUnknown>       pTmp;

    pTmp.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
    pProjInfo = pTmp;
    if(pProjInfo)
    {
        pProjInfo->fileParseNow(NULL);
    }
}


//------------------------------------------------------------------*
/**
 * accelerator CTRL + C.
 *
*/
void CNavigator::OnNavCopy() 
{
    CNavTreeCtrl*   pActiveTree = GetActiveTreeCtrl();
    if(pActiveTree)
    {
        pActiveTree->OnCopy();
    }
}

//------------------------------------------------------------------*
/**
 * accelerator CTRL + X.
 *
*/
void CNavigator::OnNavCut() 
{
    CNavTreeCtrl*   pActiveTree = GetActiveTreeCtrl();
    if(pActiveTree)
    {
        pActiveTree->OnCut();
    }
}

//------------------------------------------------------------------*
/**
 * accelerator CTRL + V.
 *
*/
void CNavigator::OnNavPaste() 
{
    CNavTreeCtrl*   pActiveTree = GetActiveTreeCtrl();
    if(pActiveTree)
    {
        pActiveTree->OnPaste();
    }
}

//------------------------------------------------------------------*
/**
 * accelerator CTRL + <-.
 *
*/
void CNavigator::OnCollapseAll() 
{
    CNavTreeCtrl*   pActiveTree = GetActiveTreeCtrl();
    if(pActiveTree)
    {
        pActiveTree->OnCollapseAll();
    }
}

//------------------------------------------------------------------*
/**
 * accelerator CTRL + ->.
 *
*/
void CNavigator::OnExpandAll() 
{
    CNavTreeCtrl*   pActiveTree = GetActiveTreeCtrl();
    if(pActiveTree)
    {
        pActiveTree->OnExpandAll();
    }
}



BOOL CNavigator::ShowNode(const CString& crstrIdPath)
{
    // ensure that navigator is visible
    CMainFrame* pMainFrame = (CMainFrame*) ::AfxGetApp()->m_pMainWnd;
    pMainFrame->ShowControlBar(this, TRUE, FALSE);

    CNavTreeCtrl*   pNavTreeCtrl;

    pNavTreeCtrl = GetTreeForIdPath(crstrIdPath, m_ActualTreeMode);
    if(pNavTreeCtrl)
    {
        m_TabCtrl.SetCurSel(m_ActualTreeMode);
        ShowActualTree(TRUE);
        if(pNavTreeCtrl->ShowNode(crstrIdPath))
        {
            return TRUE;
        }
    }
    return FALSE;
}



//------------------------------------------------------------------*
/**
 * copy file to project.
 *
 * @param           crstrFilePathName: file name incl. path
 * @param           eCatTo: file category for new file
 * @return          successful?
*/
BOOL CNavigator::CopyFileToProject(const CString& crstrFilePathName, E_FV_FileCategory eCatTo)
{
    return(m_tFlTreeCtrl.CopyExternalFileToProject(crstrFilePathName, eCatTo));
}

//------------------------------------------------------------------*
/**
 * enable file check.
 *
 * @param            bFileCheck: enable file check?
*/
void CNavigator::EnableFileCheck(BOOL bFileCheck /* = TRUE*/)
{
    m_tFlTreeCtrl.EnableFileCheck(bFileCheck);
}

//------------------------------------------------------------------*
/**
 * check files.
 *
 * @param            bSilent: silently?
*/
void CNavigator::CheckFiles(BOOL bSilent /*=FALSE*/)
{
    m_tFlTreeCtrl.Check(bSilent);
}

//------------------------------------------------------------------*
/**
 * add target instance.
 *
 * @param           crstrId: target instance id
 * @param           crstrIdPath: target instance id path
 *
*/
void CNavigator::AddTargetInstance(const CString& crstrId, const CString& crstrIdPath)
{
    m_tInstTreeCtrl.AddTargetInstance(crstrId, crstrIdPath);
}

//------------------------------------------------------------------*
/**
 * remove target instance.
 *
 * @param           crstrId: target instance id
 *
*/
void CNavigator::RemoveTargetInstance(const CString& crstrId)
{
    // remove target from target set
    m_tInstTreeCtrl.RemoveTargetInstance(crstrId);
}


//------------------------------------------------------------------*
/**
 * unregister session CBs.
 *
*/
void CNavigator::UnregisterSessionCBs()
{
    CTargetSet* pTargetSet;
    POSITION    pPosition;

    pTargetSet = CEGetTargetSet ("Project Targets", false);
    if(!pTargetSet)
    {
        return;
    }

    pPosition = pTargetSet->GetFirstTargetPosition();
    while (pPosition != NULL)
    {
        CTargetOnlBase* pTargetBase;
//        CTargetOnl*     pTarget;

        pTargetBase = pTargetSet->GetNextTarget (pPosition);
        ASSERT (pTargetBase != NULL);
        if(!pTargetBase)
        {
            continue;
        }

        pTargetBase->UnregisterConnectStateCB(&m_ConnectStateCB);
        pTargetBase->UnregisterSessionCB(&m_SessionCB);
//        pTarget = dynamic_cast<CTargetOnl*> (pTargetBase);
//        if(pTarget)
//        {
//            pTarget->UnregisterExclusiveSessionCB (&m_DebugSessionCB);
//        }
    }
}

void CNavigator::OnSessionCB(CTargetOnlBase *pTarget, CSC_SESSION_STATE sessionState, HRESULT hrError)
{
    // inform instance (debug) and HW (target) views
//    m_tHWTreeCtrl.OnSessionStateChange(pTarget, sessionState, hrError);
    m_tInstTreeCtrl.OnSessionStateChange(pTarget, sessionState, hrError);
}

void CNavigator::OnDebugSessionCB(CTargetOnl *pTarget, CSC_SESSION_STATE sessionState, HRESULT hrError)
{
    // inform instance (debug) and HW (target) views
//    m_tHWTreeCtrl.OnDebugSessionStateChange(pTarget, sessionState, hrError);
//    m_tInstTreeCtrl.OnDebugSessionStateChange(pTarget, sessionState, hrError);
}

void CNavigator::OnConnectChange(CTargetOnlBase *pTarget)
{
    // inform instance (debug) and HW (target) views
//    m_tHWTreeCtrl.OnSessionStateChange(pTarget, sessionState, hrError);
    m_tInstTreeCtrl.OnConnectStateChanged(pTarget);
}

void CNavigator::OnDropFiles(HDROP hDropInfo) 
{
    UINT    uiFile;
    UINT    uiNumFiles;
    uiNumFiles = ::DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
    TCHAR*  pcaBuf = new TCHAR[_MAX_PATH+1];
    CString strFile;
    
    // 25.08.06 SIS >>
    // added recently used projects in open project
    // allow dropping project file also if project is open
    if(uiNumFiles == 1)
    {
        ::DragQueryFile(hDropInfo, 0, pcaBuf, _MAX_PATH);
        strFile = pcaBuf;
        // if this is a 4cp file -> load it
        if(strFile.Right(4).CompareNoCase(_T(".4cp")) == 0)
        {
            AfxGetApp()->OpenDocumentFile(strFile);
            return;
        }
    }

    if(m_bProjectLoaded)
    {
        EnableFileCheck(FALSE);
        for(uiFile = 0; uiFile < uiNumFiles; ++uiFile)
        {
            ::DragQueryFile(hDropInfo, uiFile, pcaBuf, _MAX_PATH);
            strFile = pcaBuf;
            TRACE("OnDropFiles: %s\n", strFile);
            CopyFileToProject(strFile, FV_FILE_SOURCE);
        }
        Parse();
        EnableFileCheck(TRUE);
    }
    else
    {
        ::AfxMessageBox(IDS_NO_PROJECT_OPEN);
    }
    // 25.08.06 SIS <<

    delete [] pcaBuf;
    ::DragFinish(hDropInfo);
}



BOOL CNavigator::OnHelpInfo(HELPINFO* pHelpInfo) 
{
    CWinApp *pApp;
    DWORD dwHelpID = HID_BASE_RESOURCE + IDD_NAVIGATOR;

    pApp = AfxGetApp ();
    if (pApp != NULL)
    {
        switch(m_ActualTreeMode)
        {
        case NAV_CLASSVIEW:
            dwHelpID = HID_BASE_RESOURCE + IDC_ClTREE;
            break;
        case NAV_FILEVIEW:
            dwHelpID = HID_BASE_RESOURCE + IDC_FlTREE;
            break;
        case NAV_HWVIEW:
            dwHelpID = HID_BASE_RESOURCE + IDC_HWTREE;
            break;
        case NAV_INSTVIEW:
            dwHelpID = HID_BASE_RESOURCE + IDC_INSTTREE;
            break;
        }

        pApp->WinHelp(dwHelpID);
        return (TRUE);
    };

    return (FALSE);
}


//------------------------------------------------------------------*
/**
 * add source files.
 *
 * @param           rastrFiles: string array with file names
 * @param           crstrForeignProjPath: foreign project path where to be copied from
 * @return          -
 * @exception       -
 * @see             -
*/
void CNavigator::AddSourceFiles(CStringArray& rastrFiles, const CString& crstrForeignProjPath)
{
    m_tFlTreeCtrl.AddSourceFiles(rastrFiles, crstrForeignProjPath);
}


//------------------------------------------------------------------*
/**
 * create tree view text file.
 *
 * @param           eMode: tree mode (which tree)
 * @param           pPrintSettings: print settings
 * @param           crstrFilePathName: file path name of output file
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CNavigator::PrintTreeView(NAV_TreeMode eTree, CPrintSettings* pPrintSettings, CString& rstrFilePathName)
{
    CNavTreeCtrl*   pActTreeCtrl = GetTreeCtrl(eTree);
    if(pActTreeCtrl)
    {
        return pActTreeCtrl->Print(pPrintSettings, rstrFilePathName);
    }
    return FALSE;
}

