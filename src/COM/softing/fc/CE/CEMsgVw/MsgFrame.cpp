

// MsgFrame.cpp : implementation file
//

#include "stdafx.h"
#include "msgrc.h"
#include "MsgFrame.h"
#include "msgdoc.h"
#include "MsgView.h"
#include "CEMAIN.h" 
#include "TargetMsgView.h"
#include "MsgTabWnd.h"
#include "Navigator.h"
#include "CECompMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMsgFrame

IMPLEMENT_DYNCREATE(CMsgFrame, CFrameWnd)

CMsgFrame::CMsgFrame()
{
}

CMsgFrame::~CMsgFrame()
{
}


BEGIN_MESSAGE_MAP(CMsgFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMsgFrame)
	ON_WM_DROPFILES()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsgFrame message handlers


//------------------------------------------------------------------*
/**
 * Function name	CMsgFrame::OnCreateClient
 * Description			
 * @param			LPCREATESTRUCT lpcs
 * @param			CCreateContext* pContext
 * @return			BOOL 
 * @exception			
 * @see			
*/
BOOL CMsgFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
    CString Str;
    CString HelpStr;
    CCreateContext Context;
    CMsgDoc * pDoc;
    CMultiDocTemplate * pDocTemplate;
    CMsgData CompileMsgData;
    CMsgData DownloadMsgData;
    CMsgData FileFindMsgData;

    DragAcceptFiles();

    pDocTemplate = ((CCEMAINApp *)AfxGetApp())->m_pMsgTemplate;
    pDoc = (CMsgDoc *)pDocTemplate->CreateNewDocument();
    
    // Compile view
    CompileMsgData.m_MsgViewerType = MSG_CmdLineMsgViewer; 
    CompileMsgData.m_hwndClient = NULL;
    pDoc->m_arrMsgData.Add(CompileMsgData);
    // Download view
    DownloadMsgData.m_MsgViewerType = MSG_DownLoadMsgViewer;
    DownloadMsgData.m_hwndClient = NULL;
    pDoc->m_arrMsgData.Add(DownloadMsgData);
    // Find in Files view
    FileFindMsgData.m_MsgViewerType = MSG_FindInFilesMsgViewer; 
    FileFindMsgData.m_hwndClient = NULL;
    pDoc->m_arrMsgData.Add(FileFindMsgData);
    // System view
    FileFindMsgData.m_MsgViewerType = MSG_SystemMsgViewer; 
    FileFindMsgData.m_hwndClient = NULL;
    pDoc->m_arrMsgData.Add(FileFindMsgData);

    Context.m_pCurrentDoc = pDoc;

    m_tabWnd.Create(this , WS_CHILD | WS_VISIBLE | TWS_FULLSCROLL | TWS_TABS_ON_BOTTOM
                    | TWS_NOACTIVE_TAB_ENLARGED | TWS_DYNAMIC_ARRANGE_TABS );

    // Compile view
    Str.LoadString(IDS_MSG_COMPILE);
    m_tabWnd.AddTab( RUNTIME_CLASS( CMsgView ), Str, &Context);  
    m_tabWnd.SetScrollStyle(0, 0);

    // Download view
    Str.LoadString(IDS_MSG_DOWNLOAD);
    m_tabWnd.AddTab( RUNTIME_CLASS( CMsgView ), Str, &Context );   
    m_tabWnd.SetScrollStyle(1, 0);
    
    // Find in Files view
    Str.LoadString(IDS_MSG_FINDINFILES);
    m_tabWnd.AddTab( RUNTIME_CLASS( CMsgView ), Str, &Context );   
    m_tabWnd.SetScrollStyle(2, 0);

    // System view
    Str.LoadString(IDS_MSG_SYSTEM);
    m_tabWnd.AddTab( RUNTIME_CLASS( CMsgView ), Str, &Context );  
    m_tabWnd.SetScrollStyle(3, 0);

    HelpStr.LoadString(IDS_MSG_CONTROL);

    m_ImageList.Create (IDB_MSG_INDICATORS_NEW, INDICATOR_BITMAP_WIDTH, 0, INDICATOR_TRANSPARENT_COLOR);
//    m_tabWnd.SetImageList(&m_ImageList);

    //Activate tab 1
    m_tabWnd.ActivateTab(1);
    
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * set image for tab.
 *
 * @param           [in] iTabIdx: tab index
 * @param           [in] iImageIdx: image index
*/
void CMsgFrame::SetImage(int iTabIdx, int iImageIdx)
{
    HICON hIcon = NULL;
    if(iImageIdx >= 0)
    {
        hIcon = m_ImageList.ExtractIcon(iImageIdx);
    }
    m_tabWnd.SetTabIcon(iTabIdx, hIcon);
}

void CMsgFrame::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
    CWnd *pViewWnd;

    m_tabWnd.GetActiveTab(pViewWnd);
    if(pViewWnd)
    {
        pViewWnd->UpdateDialogControls(pTarget, bDisableIfNoHndler);
    }
	UpdateDialogControls(pTarget, bDisableIfNoHndler);
}


//------------------------------------------------------------------*
/**
 * insert target tab.
 *
 * @param           [in] iIndex: index where to insert tab
 * @param           [in] crstrTargetName: name of target to be shown in tab
 *
*/
void CMsgFrame::InsertTargetTab(int iIndex, const CString& crstrTargetName)
{
    CTargetMsgView* pView = NULL;

    m_tabWnd.InsertTab(RUNTIME_CLASS(CTargetMsgView), iIndex, crstrTargetName);
    m_tabWnd.SetScrollStyle(iIndex, 0);

    LPCTSTR lpszLabel;
    BOOL    bSelected;
    CWnd*   pWnd;
    void*   pExtra;

    if(m_tabWnd.GetTabInfo(iIndex, lpszLabel, bSelected, pWnd, pExtra))
    {
        pView = dynamic_cast<CTargetMsgView*> (pWnd);
        if(pView)
        {
            pView->SetTargetName(crstrTargetName);
            pView->InitTarget();
        }
    }
}


void CMsgFrame::OnDropFiles(HDROP hDropInfo)
{
    CNavigator* pNavigator = dynamic_cast<CNavigator*>(CEGetDockingTool(CE_COMPMAN_NAV, false));
    ASSERT(pNavigator);
    if(pNavigator)
    {
        pNavigator->OnDropFiles(hDropInfo);
    }
}