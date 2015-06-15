


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*



#include "stdafx.h"
#include "GenEdTabWnd.h"
#include "GenEdView.h"
#include "GenEdDoc.h"
#include "GenEdCntrItem.h"
#include "GenEdCont.h"
#include "CeSys.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//IMPLEMENT_DYNCREATE(CGenEdTabWnd, CMsgTabWnd)

BEGIN_MESSAGE_MAP(CGenEdTabWnd, CMsgTabWnd)
	//{{AFX_MSG_MAP(CGenEdTabWnd)
	ON_MESSAGE(TCM_TABSEL, OnTabSelect)
	//}}AFX_MSG_MAP
    ON_MESSAGE(WM_TAB_CONTEXTMENU, OnTabContextMenu)
END_MESSAGE_MAP()


/************************************************
   *   Constructor/Destructor 
   *
 * * *
  ***
   *
*/

CGenEdTabWnd::CGenEdTabWnd()
: m_DataTipPopupCB(this, OnDataTipPopup),
  m_DataTipActionQueryCB(this, OnDataTipActionQuery),
  m_iDataTipTab(-1)
{
    m_DataTip.SetPopupCB(&m_DataTipPopupCB);
    m_DataTip.SetActionQueryCB(&m_DataTipActionQueryCB);

    m_pContainer = NULL;
    EnableToolTips();
}

CGenEdTabWnd::~CGenEdTabWnd()
{
    m_DataTip.SetPopupCB(NULL);
    m_DataTip.SetActionQueryCB(NULL);
}


SECTab* CGenEdTabWnd::AddTab
(
    CRuntimeClass* pViewClass,
    LPCTSTR lpszLabel,
    CCreateContext* pContext /*=NULL*/,
    UINT nID /*= -1*/
)
{
    SECTab*             pTabWnd;
    CGenEdView*         pView;
    CGenEdDoc*          pDoc;
    CGenEdCntrItem*     pServer;

    if(!m_pContainer)
    {
        TRACE("AddTab: container not yet set. Return 0\n");
        return 0;
    }

    pTabWnd = SECTabWnd::AddTab(pViewClass, lpszLabel, pContext, nID);
    pView = (CGenEdView*)pTabWnd->m_pClient;
    ASSERT(pView);
    pDoc = pView->GetDocument();
    ASSERT(pDoc);
    pServer = pDoc->GetEditorServer();
    if(pServer)
    {
        pView->ShowServer(pServer);
//        pView->ActivateServer();
    }
    return(pTabWnd);
}

SECTab* CGenEdTabWnd::InsertTab
(
    CRuntimeClass* pViewClass,
    int nIndex,
    LPCTSTR lpszLabel,
    CCreateContext* pContext /*=NULL*/,
    UINT nID /*= -1*/
)
{
    SECTab*             pTabWnd;
    CGenEdView*         pView;
    CGenEdDoc*          pDoc;
    CGenEdCntrItem*     pServer;

    if(!m_pContainer)
    {
        TRACE("InsertTab: container not yet set. Return 0\n");
        return 0;
    }

    pTabWnd = SECTabWnd::InsertTab(pViewClass, nIndex, lpszLabel, pContext, nID);
    pView = (CGenEdView*)pTabWnd->m_pClient;
    ASSERT(pView);
    pDoc = pView->GetDocument();
    ASSERT(pDoc);
    pServer = pDoc->GetEditorServer();
    if(pServer)
    {
        pView->ShowServer(pServer);
//        pView->ActivateServer();
    }
    return(pTabWnd);
}


//------------------------------------------------------------------*
/**
 * set pointer to editor container
 *
 * @param           [in] pContainer: pointer to editor container
 *
*/
void CGenEdTabWnd::SetContainer(CGenEdCont* pContainer)
{
    m_pContainer = pContainer;
}


//CString CGenEdTabWnd::GetToolTipText (UINT uIndex)
//{
//}
//
//
//void CGenEdTabWnd::SetToolTipText (UINT uIndex, const CString &strText)
//{
//}
//
//
//void CGenEdTabWnd::SetImageList (CImageList *pImageList)
//{
//}
//
//
//int CGenEdTabWnd::GetImage (UINT uiIndex)
//{
//}
//
//
//void CGenEdTabWnd::SetImage (UINT uiIndex, int iImage)
//{
//}


/************************************************
   *   Messages
   *
 * * *
  ***
   *
*/
//void CGenEdTabWnd::OnLButtonDown(UINT nFlags, CPoint point)
//{     
//    // prevent the user from dragging the splitter bar
//	return;
//}
//
//BOOL CGenEdTabWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
//{   
//    // Don't allow the cursor to change over splitbar
//	return FALSE;
//}
//
//void CGenEdTabWnd::OnMouseMove(UINT nFlags, CPoint point)
//{       
//    // Don't allow the cursor to change over splitbar
//    CWnd::OnMouseMove(nFlags, point);
//}

LRESULT CGenEdTabWnd::OnTabSelect(WPARAM wParam, LPARAM lParam)
{
	SECTABINFO* pTabInfo;
    pTabInfo = (SECTABINFO*)lParam;
    CGenEdView* pViewActive = (CGenEdView*)pTabInfo->pActiveClient;
    CGenEdView* pViewPrevious = (CGenEdView*)pTabInfo->pPrevClient;

    CGenEdCntrItem* pServerActive = pViewActive->GetServer();
    CGenEdCntrItem* pServerPrevious = NULL;

    if(pViewPrevious)
    {
        pViewPrevious->DeactivateServer();
        pServerPrevious = pViewPrevious->GetServer();
    }
//    if(pServerActive == NULL && pServerPrevious != NULL)
//    {
//        pViewActive->ShowServer(pServerPrevious);
//    }
    pViewActive->ActivateServer();

    return(SECTabWnd::OnTabSelect(wParam, lParam));
}


void CGenEdTabWnd::OnContextMenu( CWnd* pWnd, CPoint pos )
{
    // send message to frame window
	CFrameWnd *pFrame;
//	CPoint point;

	pFrame = GetParentFrame();

//	point = pos;
//	ScreenToClient (&point);

	pFrame->SendMessage(WM_TAB_CONTEXTMENU, 0, (LPARAM)(LPPOINT)&pos);
}


void CGenEdTabWnd::OnTabContextMenu(WPARAM tab, LPARAM pos)
{
    // send message to frame window
	CFrameWnd *pFrame;
	pFrame = GetParentFrame();
	pFrame->SendMessage(WM_TAB_CONTEXTMENU, tab, pos);
}



CString CGenEdTabWnd::GetToolTipTextForTab(int iTab)
{
    CString         strText;
    CGenEdFrame*    pFrame = (CGenEdFrame*)GetParent();

    if(pFrame)
    {
        return pFrame->GetToolTipTextForTab(iTab);
    }
    return strText;
}

HRESULT CGenEdTabWnd::EnableTips(CWnd *pTabWindow)
{
    HRESULT hr;

    ASSERT(m_pTabCtrl);
    hr = m_DataTip.Create(AfxGetMainWnd(), TTS_ALWAYSTIP | TTS_NOPREFIX, m_pTabCtrl);
    if(FAILED(hr))
        return(hr);

    hr = m_DataTip.Activate();
    if(FAILED(hr))
    {
        m_DataTip.Destroy();
        return(hr);
    };

    return(S_OK);
}


void CGenEdTabWnd::DisableTips()
{
    m_DataTip.Hide();
    m_DataTip.Deactivate();
    m_DataTip.Destroy();
}


void CGenEdTabWnd::ActivateTips()
{
    if(!m_DataTip.IsCreated())
        return;

    m_DataTip.Activate();
}


void CGenEdTabWnd::DeactivateTips()
{
    if(!m_DataTip.IsCreated())
        return;

    m_DataTip.Deactivate();
}


void CGenEdTabWnd::RelayMessage(MSG *pMessage)
{
    m_DataTip.RelayMessage(pMessage);
}


void CGenEdTabWnd::OnDataTipPopup(bool &bAllow)
{
    bAllow = true;
}


void CGenEdTabWnd::OnDataTipActionQuery(CPoint point, FCDataTipAction &action)
{
    int iHitTab;

    iHitTab = TabHitTest(point);

    if(iHitTab == -1)
    {
        action = FCHideDataTip;
        return;
    }

    if(m_DataTip.IsSleeping() || iHitTab != m_iDataTipTab)
    {
        CString strText = GetToolTipTextForTab(iHitTab);

        m_DataTip.SetText(strText);

        m_iDataTipTab = iHitTab;

        action = FCShowDataTip;
        return;
    }

    action = FCMoveDataTip;
}


int CGenEdTabWnd::TabHitTest(CPoint point)
{
    // Return index of tab under point. Return -1 if none is hit.
    int iTab = -1;
    CMsgTabCtr* pTabCtrl = dynamic_cast<CMsgTabCtr*> (m_pTabCtrl);

    if(pTabCtrl)
    {
        iTab = pTabCtrl->GetTabIndexFromScreenPosition(point);
    }
    return iTab;
}

BOOL CGenEdTabWnd::PreTranslateMessage(MSG* pMsg) 
{
	RelayMessage(pMsg);
	return CMsgTabWnd::PreTranslateMessage(pMsg);
}


