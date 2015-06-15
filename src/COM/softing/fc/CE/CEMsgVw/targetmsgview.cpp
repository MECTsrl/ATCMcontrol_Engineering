// ProbeFormView.cpp : implementation file
//
//----  Local Defines:   -------------------------------------------*
#include "stdafx.h"
#include "TargetMsgView.h"
#include "resource.h"
#include "MsgWnd.h"
#include "cfacility.h"
#include "cecompman.h"
#include "cemain.h"
#include "CESysDef.h"
#include "assert.h"
#include "TargetSet.h"
#include "CEProjInfo.h"
#include "XMLHelper/xmlnode.h"
#include "CESys.h"

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*


class CTargetOnlBase;

/////////////////////////////////////////////////////////////////////////////
// local defines

// maximum number of lines in a MessageViewer
#define MSGWND_MAXLINES     1000
// number of lines to delete if an overflow is detected
#define MSGWND_DELLINES     100

// maximum text length (limitations of Windows 95/98
#define MSGWND_MAXTEXTLEN   0xffff

/////////////////////////////////////////////////////////////////////////////
// CTargetMsgView

IMPLEMENT_DYNCREATE(CTargetMsgView, CFormView)

CTargetMsgView::CTargetMsgView()
: CFormView(CTargetMsgView::IDD),
  m_SessionState (not_connected),
  m_ExclusiveSessionState (not_connected),
  m_eConnectState(eTOCS_not_connected),
  m_bResourceCleared (false),
  m_iResourceState (0),
  m_bSubscribedForState (false),
  m_ResourceStateCB (this, OnResourceStateCB),
  m_pStateSubscription (NULL),
  m_pClearedSubscription (NULL),
  m_bInitialized(FALSE)
{
	//{{AFX_DATA_INIT(CTargetMsgView)
	//}}AFX_DATA_INIT
    m_phbr = new CBrush();
    ASSERT(m_phbr);
    m_strTargetAddr = _T("No target");
    EnableToolTips(TRUE);
}

CTargetMsgView::~CTargetMsgView()
{
    if(m_phbr)
    {
        delete m_phbr;
    }
}

void CTargetMsgView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTargetMsgView)
	DDX_Control(pDX, IDC_EDITMESSAGE, m_Edit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTargetMsgView, CFormView)
	//{{AFX_MSG_MAP(CTargetMsgView)
	ON_WM_CREATE()
    ON_WM_SIZE()
	ON_WM_CTLCOLOR_REFLECT()
	ON_COMMAND(IDC_CONNECTTARGET, OnConnect)
	ON_COMMAND(IDC_OPERATE_ACK, OnAcknowledge)
	ON_COMMAND(IDC_OPERATE_ALLTASKSSTART, OnAlltasksstart)
	ON_COMMAND(IDC_OPERATE_ALLTASKSSTOP, OnAlltasksstop)
	ON_COMMAND(IDC_OPERATE_WARMSTART, OnResWarmstart)
	ON_COMMAND(IDC_OPERATE_COLDSTART, OnResColdstart)
	ON_COMMAND(IDC_OPERATE_RESCLEAR, OnResclear)
	ON_COMMAND(IDC_OPERATE_EXCLUSIVECONNECT, OnConnectExclusive)
	ON_COMMAND(IDC_OPERATE_EXCLUSIVEDISCONNECT, OnDisconnectExclusive)
	ON_COMMAND(IDC_OPERATE_RESSTART, OnResStart)
	ON_COMMAND(IDC_OPERATE_RESSTOP, OnResStop)
	ON_COMMAND(IDC_DISCONNECTTARGET, OnDisconnect)
	ON_UPDATE_COMMAND_UI(IDC_CONNECTTARGET, OnUpdateConnect)
	ON_UPDATE_COMMAND_UI(IDC_DISCONNECTTARGET, OnUpdateDisconnect)
	ON_UPDATE_COMMAND_UI(IDC_OPERATE_RESSTART, OnUpdateResStart)
	ON_UPDATE_COMMAND_UI(IDC_OPERATE_RESSTOP, OnUpdateResStop)
	ON_UPDATE_COMMAND_UI(IDC_OPERATE_ALLTASKSSTART, OnUpdateAllTasksStart)
	ON_UPDATE_COMMAND_UI(IDC_OPERATE_ALLTASKSSTOP, OnUpdateAllTasksStop)
	ON_UPDATE_COMMAND_UI(IDC_OPERATE_EXCLUSIVECONNECT, OnUpdateExclusiveConnect)
	ON_UPDATE_COMMAND_UI(IDC_OPERATE_EXCLUSIVEDISCONNECT, OnUpdateExclusiveDisconnect)
	ON_UPDATE_COMMAND_UI(IDC_OPERATE_RESCLEAR, OnUpdateResClear)
	ON_WM_PAINT()
    ON_WM_CONTEXTMENU()
	ON_COMMAND(IDM_MSG_COPY, OnEditCopy)
	ON_COMMAND(IDM_MSG_CLEAR, OnEditClear)
    ON_COMMAND(IDM_MSG_CONTEXT_MENU, OnContextMenuCommand)
    ON_UPDATE_COMMAND_UI(IDC_OPERATE_ACK, OnUpdateAck)
	ON_UPDATE_COMMAND_UI(IDC_OPERATE_WARMSTART, OnUpdateWarmstart)
	ON_UPDATE_COMMAND_UI(IDC_OPERATE_COLDSTART, OnUpdateColdstart)
    ON_NOTIFY_EX (TTN_NEEDTEXT, 0, OnToolTipNfy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTargetMsgView diagnostics

#ifdef _DEBUG
void CTargetMsgView::AssertValid() const
{
	CFormView::AssertValid();
}

void CTargetMsgView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTargetMsgView message handlers

/////////////////////////////////////////////////////
//Nur zum testen
static const int IDI_SYMBOL_CROSSED        = 0;
static const int IDI_SYMBOL_ESTABLISHED    = 1;
static const int IDI_SYMBOL_EXCLUSIVE      = 2;
static const int IDI_SYMBOL_INTERRUPTED    = 3;
static const int IDI_SYMBOL_WAITING        = 4;
static const int IDI_SYMBOL_CONNECT        = 5;
static const int IDI_SYMBOL_DISCONNECT     = 6;
static const int IDI_SYMBOL_RUNRES         = 7;
static const int IDI_SYMBOL_STOPRES        = 8;
static const int IDI_SYMBOL_RUNALLTASKS    = 9;
static const int IDI_SYMBOL_STOPALLTASKS   = 10;
static const int IDI_SYMBOL_WARMSTART      = 11;
static const int IDI_SYMBOL_COLDSTART      = 12;
static const int IDI_SYMBOL_CLEARRES       = 13;
static const int IDI_SYMBOL_ACKNOWLEDGE    = 14;
static const int IDI_SYMBOL_RES            = 15;
static const int IDI_SYMBOL_RUN            = 16;
static const int IDI_SYMBOL_PAUSE          = 17;
static const int IDI_SYMBOL_EXCLUSIVECONNECT     = 18;
static const int IDI_SYMBOL_EXCLUSIVEDISCONNECT  = 19;
static const int IDI_SYMBOL_RUN_CLEARED    = 20;
static const int IDI_SYMBOL_SERVICESTOPPED = 21;
static const int IDI_SYMBOL_PAUSE_CLEARED  = 22;

static const int IDI_SYMBOL_CONN_CONN     = 0;
static const int IDI_SYMBOL_CONN_NOTCONN  = 1;
static const int IDI_SYMBOL_CONN_WAITING  = 2;
static const int IDI_SYMBOL_CONN_COMM     = 3;
static const int IDI_SYMBOL_CONN_ERROR    = 4;
static const int IDI_SYMBOL_CONN_OK       = 5;

static const int CONNECTION_GROUP_LEFT     = 5;
static const int CONTROL_SS_GROUP_LEFT     = 146;
static const int CONTROL_INIT_GROUP_LEFT   = 290;
static const int ACKNOWLEDGE_GROUP_LEFT    = 386;

static const int SESSION_SYMBOL_LEFT   = CONNECTION_GROUP_LEFT;
static const int SESSION_SYMBOL_TOP          = 2;
static const int SESSION_SYMBOL_WIDTH        = 20;
static const int SESSION_SYMBOL_CONN_WIDTH   = 25;
static const int SESSION_SYMBOL_HEIGHT       = 15;

static const int RESOURCE_SYMBOL_LEFT   = CONTROL_SS_GROUP_LEFT;
static const int RESOURCE_SYMBOL_TOP    = SESSION_SYMBOL_TOP;
static const int RESOURCE_SYMBOL_WIDTH  = 20;
static const int RESOURCE_SYMBOL_HEIGHT = 15;

struct MsgTargetDialogItemInfo
{
    UINT m_uiId;
    bool m_bRelative;
    int  m_iLeft;
};


//////////////////////////////////////////////////////

int CTargetMsgView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
    m_ImageList.Create (IDB_MSG_STATE, 20, 0, RGB (255, 0, 255));
    m_ConnImageList.Create (IDB_MSG_CONNSTATE, 25, 0, RGB (255, 0, 255));

    m_bResourceCleared = false;
    m_iResourceState = 0;

	return 0;
}


//------------------------------------------------------------------*
/**
 * set target name.
 *
 *  set target name member of CTargetMsgView.
 *
 * @param           [in] crstrTargetName: target name
 *
*/
void CTargetMsgView::SetTargetName(const CString& crstrTargetName)
{
    m_strTargetName = crstrTargetName;
}



//------------------------------------------------------------------*
/**
 * init target.
 *
 *  set address member, initialize states and
 *  subscribe for state changes.
 *
*/
void CTargetMsgView::InitTarget()
{
    CTargetOnlBase* pTarget = GetTargetOnlObject();
    if (pTarget != NULL)
    {
        m_strTargetAddr = pTarget->GetControlAddress();
        m_SessionState = pTarget->GetSessionState ();
        m_ExclusiveSessionState = pTarget->GetExclusiveSessionState();
        ASSERT (!m_bSubscribedForState);
        if (pTarget->IsConnected() && pTarget->GetSessionState() == RW_established)
        {
            SubscribeForState();
        }
        if (m_hWnd != NULL)
        {
            CRect rect (RESOURCE_SYMBOL_LEFT,
                        RESOURCE_SYMBOL_TOP,
                        RESOURCE_SYMBOL_LEFT + RESOURCE_SYMBOL_WIDTH,
                        RESOURCE_SYMBOL_TOP + RESOURCE_SYMBOL_HEIGHT);

            InvalidateRect (&rect);
            UpdateWindow ();
        }

        UpdateStateText ();
    }
}

CMsgEdit* CTargetMsgView::GetEditCtrl()
{
    if(!m_Edit.m_hWnd)
    {
        m_Edit.SubclassDlgItem (IDC_EDITMESSAGE, this);
        m_Edit.SetReadOnly ();
    }
    return (&m_Edit);
}

void CTargetMsgView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);

    CButton *pButton;
    HICON   hIcon;
    HICON   hIconOld;
    BOOL    bMultitask = TRUE;
    
    CTargetOnlBase* pTarget = GetTargetOnlObject();

    if(pTarget && !pTarget->IsMultitaskEnabled())
    {
        bMultitask = FALSE;
    }

    // connect button
    pButton = (CButton *)GetDlgItem(IDC_CONNECTTARGET);
    pButton->EnableWindow ();
    hIcon = m_ImageList.ExtractIcon(IDI_SYMBOL_CONNECT);
    hIconOld = pButton->GetIcon();
    if(hIconOld)
    {
        DestroyIcon(hIconOld);
    }
    pButton->SetIcon(hIcon);

    // disconnect button
    pButton = (CButton *)GetDlgItem(IDC_DISCONNECTTARGET);
    hIcon = m_ImageList.ExtractIcon(IDI_SYMBOL_DISCONNECT);
    pButton->EnableWindow ();
    hIconOld = pButton->GetIcon();
    if(hIconOld)
    {
        DestroyIcon(hIconOld);
    }
    pButton->SetIcon(hIcon);

    // enable debug session
    pButton = (CButton *)GetDlgItem(IDC_OPERATE_EXCLUSIVECONNECT);
    hIcon = m_ImageList.ExtractIcon(IDI_SYMBOL_EXCLUSIVECONNECT);
    pButton->EnableWindow ();
    hIconOld = pButton->GetIcon();
    if(hIconOld)
    {
        DestroyIcon(hIconOld);
    }
    pButton->SetIcon(hIcon);

    // disable debug session
    pButton = (CButton *)GetDlgItem(IDC_OPERATE_EXCLUSIVEDISCONNECT);
    hIcon = m_ImageList.ExtractIcon(IDI_SYMBOL_EXCLUSIVEDISCONNECT);
    pButton->EnableWindow ();
    hIconOld = pButton->GetIcon();
    if(hIconOld)
    {
        DestroyIcon(hIconOld);
    }
    pButton->SetIcon(hIcon);

    // start button
    pButton = (CButton *)GetDlgItem(IDC_OPERATE_RESSTART);
    hIcon = m_ImageList.ExtractIcon(IDI_SYMBOL_RUNRES);
    pButton->EnableWindow ();
    hIconOld = pButton->GetIcon();
    if(hIconOld)
    {
        DestroyIcon(hIconOld);
    }
    pButton->SetIcon(hIcon);

    // stop button
    pButton = (CButton *)GetDlgItem(IDC_OPERATE_RESSTOP);
    hIcon = m_ImageList.ExtractIcon(IDI_SYMBOL_STOPRES);
    pButton->EnableWindow ();
    hIconOld = pButton->GetIcon();
    if(hIconOld)
    {
        DestroyIcon(hIconOld);
    }
    pButton->SetIcon(hIcon);

    // start all tasks button
    pButton = (CButton *)GetDlgItem(IDC_OPERATE_ALLTASKSSTART);
    hIcon = m_ImageList.ExtractIcon(IDI_SYMBOL_RUNALLTASKS);
    pButton->EnableWindow ();
    hIconOld = pButton->GetIcon();
    if(hIconOld)
    {
        DestroyIcon(hIconOld);
    }
    pButton->SetIcon(hIcon);
    if(!bMultitask)
    {
        CString strText;
        strText.LoadString(IDS_START_TASK);
        pButton->SetWindowText(strText);
    }

    // stop all tasks button
    pButton = (CButton *)GetDlgItem(IDC_OPERATE_ALLTASKSSTOP);
    hIcon = m_ImageList.ExtractIcon(IDI_SYMBOL_STOPALLTASKS);
    pButton->EnableWindow ();
    hIconOld = pButton->GetIcon();
    if(hIconOld)
    {
        DestroyIcon(hIconOld);
    }
    pButton->SetIcon(hIcon);
    if(!bMultitask)
    {
        CString strText;
        strText.LoadString(IDS_STOP_TASK);
        pButton->SetWindowText(strText);
    }

    // warmstart button
    pButton = (CButton *)GetDlgItem(IDC_OPERATE_WARMSTART);
    hIcon = m_ImageList.ExtractIcon(IDI_SYMBOL_WARMSTART);
    pButton->EnableWindow ();
    hIconOld = pButton->GetIcon();
    if(hIconOld)
    {
        DestroyIcon(hIconOld);
    }
    pButton->SetIcon(hIcon);

    // coldstart button
    pButton = (CButton *)GetDlgItem(IDC_OPERATE_COLDSTART);
    hIcon = m_ImageList.ExtractIcon(IDI_SYMBOL_COLDSTART);
    pButton->EnableWindow ();
    hIconOld = pButton->GetIcon();
    if(hIconOld)
    {
        DestroyIcon(hIconOld);
    }
    pButton->SetIcon(hIcon);

    // resource clear button
    pButton = (CButton *)GetDlgItem(IDC_OPERATE_RESCLEAR);
    hIcon = m_ImageList.ExtractIcon(IDI_SYMBOL_CLEARRES);
    pButton->EnableWindow ();
    hIconOld = pButton->GetIcon();
    if(hIconOld)
    {
        DestroyIcon(hIconOld);
    }
    pButton->SetIcon(hIcon);

    // acknowledge button
    pButton = (CButton *)GetDlgItem(IDC_OPERATE_ACK);
    hIcon = m_ImageList.ExtractIcon(IDI_SYMBOL_ACKNOWLEDGE);
    pButton->EnableWindow ();
    hIconOld = pButton->GetIcon();
    if(hIconOld)
    {
        DestroyIcon(hIconOld);
    }
    pButton->SetIcon(hIcon);
    
    if(bActivate) {
        //update the edit view.        
        UpdateEditText(); 
    }

}

void CTargetMsgView::OnSize(UINT nType, int cx, int cy) 
{
    //CFormView::OnSize(nType, cx, cy);

    CButton * pButton;
    CMsgEdit * pEdit = NULL;
    int buttonRowLeft;
    static const int buttonYPos = SESSION_SYMBOL_TOP - 2;
    static const int buttonWidth = SESSION_SYMBOL_WIDTH + 2;
    static const int buttonHeight = SESSION_SYMBOL_HEIGHT + 4;

    if(!GetSafeHwnd())
        return;

    // size buttons
    buttonRowLeft = CONNECTION_GROUP_LEFT + SESSION_SYMBOL_CONN_WIDTH + 2;
    pButton = (CButton *)GetDlgItem(IDC_CONNECTTARGET);
    if(!pButton)
    {
        return;
    }

    pButton->MoveWindow(buttonRowLeft, buttonYPos, buttonWidth, buttonHeight);
    pButton = (CButton *)GetDlgItem(IDC_OPERATE_EXCLUSIVECONNECT);
    pButton->MoveWindow(buttonRowLeft + buttonWidth, buttonYPos, buttonWidth, buttonHeight);
    pButton = (CButton *)GetDlgItem(IDC_OPERATE_EXCLUSIVEDISCONNECT);
    pButton->MoveWindow(buttonRowLeft + 2 * buttonWidth, buttonYPos, buttonWidth, buttonHeight);
    pButton = (CButton *)GetDlgItem(IDC_DISCONNECTTARGET);
    pButton->MoveWindow(buttonRowLeft + 3 * buttonWidth, buttonYPos, buttonWidth, buttonHeight);

    buttonRowLeft = CONTROL_SS_GROUP_LEFT + SESSION_SYMBOL_WIDTH + 2;
    pButton = (CButton *)GetDlgItem(IDC_OPERATE_RESSTART);
    pButton->MoveWindow(buttonRowLeft, buttonYPos, buttonWidth, buttonHeight);
    pButton = (CButton *)GetDlgItem(IDC_OPERATE_RESSTOP);
    pButton->MoveWindow(buttonRowLeft + buttonWidth, buttonYPos, buttonWidth, buttonHeight);
    pButton = (CButton *)GetDlgItem(IDC_OPERATE_ALLTASKSSTART);
    pButton->MoveWindow(buttonRowLeft + 2 * (buttonWidth) + 3, buttonYPos, buttonWidth, buttonHeight);
    pButton = (CButton *)GetDlgItem(IDC_OPERATE_ALLTASKSSTOP);
    pButton->MoveWindow(buttonRowLeft + 3 * (buttonWidth) + 3, buttonYPos, buttonWidth, buttonHeight);

    buttonRowLeft = CONTROL_INIT_GROUP_LEFT;
    pButton = (CButton *)GetDlgItem(IDC_OPERATE_WARMSTART);
    pButton->MoveWindow(buttonRowLeft, buttonYPos, buttonWidth, buttonHeight);
    pButton = (CButton *)GetDlgItem(IDC_OPERATE_COLDSTART);
    pButton->MoveWindow(buttonRowLeft + buttonWidth, buttonYPos, buttonWidth, buttonHeight);
    pButton = (CButton *)GetDlgItem(IDC_OPERATE_RESCLEAR);
    pButton->MoveWindow(buttonRowLeft + 2 * (buttonWidth), buttonYPos, buttonWidth, buttonHeight);

    buttonRowLeft = ACKNOWLEDGE_GROUP_LEFT;
    pButton = (CButton *)GetDlgItem(IDC_OPERATE_ACK);
    pButton->MoveWindow(buttonRowLeft, buttonYPos, buttonWidth, buttonHeight);
    
    pEdit =  GetEditCtrl();
    if(!pEdit) 
    {
        return;
    }
    else 
    {
        if(!m_bInitialized)
        {
            CFont *pFont;
            //Set fixed font
            pFont = CFont::FromHandle ((HFONT)GetStockObject (ANSI_FIXED_FONT));
            pEdit->SetFont (pFont);
       
            // set text limit to maximum of Windows 95/98
            //pEdit->SetLimitText(MSGWND_MAXTEXTLEN);
            m_bInitialized = TRUE;
        }
    }

    OnSizeDialogItems (0, 0, cx, cy);
}

void CTargetMsgView::OnSizeDialogItems (int iLeft, int iTop, int iWidth, int iHeight)
{
    int iItemLeft;
    CMsgEdit* pEdit;
    CWnd *pDialogItem;
    static MsgTargetDialogItemInfo itemInfos[12] =
    {
        { IDC_CONNECTTARGET            , false, CONNECTION_GROUP_LEFT + SESSION_SYMBOL_CONN_WIDTH + 2 },
        { IDC_OPERATE_EXCLUSIVECONNECT   , true , 0                                                     },
        { IDC_OPERATE_EXCLUSIVEDISCONNECT, true , 0                                                     },
        { IDC_DISCONNECTTARGET         , true , 0                                                     },
        { IDC_OPERATE_RESSTART     , false, CONTROL_SS_GROUP_LEFT + SESSION_SYMBOL_WIDTH + 2      },
        { IDC_OPERATE_RESSTOP      , true , 0                                                     },
        { IDC_OPERATE_ALLTASKSSTART, true , 3                                                     },
        { IDC_OPERATE_ALLTASKSSTOP , true , 0                                                     },
        { IDC_OPERATE_WARMSTART    , false, CONTROL_INIT_GROUP_LEFT                               },
        { IDC_OPERATE_COLDSTART    , true , 0                                                     },
        { IDC_OPERATE_RESCLEAR     , true , 0                                                     },
        { IDC_OPERATE_ACK          , false, ACKNOWLEDGE_GROUP_LEFT                                }
    };
    
    pEdit = GetEditCtrl();
    if(!pEdit) {
        return;
    }

    if (::IsWindow (pEdit->m_hWnd))
    {
        pEdit->MoveWindow (iLeft, iTop + SESSION_SYMBOL_HEIGHT + 6, iWidth, iHeight - (SESSION_SYMBOL_HEIGHT + 6));
    }

    iItemLeft = SizeItems (12, itemInfos);

    pDialogItem = GetDlgItem (IDC_STATE_TEXT);
    if (pDialogItem != NULL)
    {
        iItemLeft += 20;
        pDialogItem->MoveWindow (iItemLeft, iTop, iWidth - iItemLeft, SESSION_SYMBOL_HEIGHT + 6);
        iItemLeft = iWidth;
    }
}


int CTargetMsgView::SizeItems (unsigned int uiCount, MsgTargetDialogItemInfo *pItemInfos)
{
    UINT uiIndex;
    int iItemLeft=0;
    static const int iItemTop=SESSION_SYMBOL_TOP-2;
    static const int iItemWidth=SESSION_SYMBOL_WIDTH+2;
    static const int iItemHeight=SESSION_SYMBOL_HEIGHT+4;

    ASSERT (pItemInfos != NULL);
    if (pItemInfos == NULL)
        return (iItemLeft);

    for (uiIndex = 0; uiIndex < uiCount; ++uiIndex)
    {
        CWnd *pDialogItem;

        if (pItemInfos[uiIndex].m_bRelative)
        {
            iItemLeft += pItemInfos[uiIndex].m_iLeft;
        }
        else
        {
            iItemLeft = pItemInfos[uiIndex].m_iLeft;
        }

        pDialogItem = GetDlgItem (pItemInfos[uiIndex].m_uiId);
        if (pDialogItem != NULL)
        {
            pDialogItem->MoveWindow (iItemLeft, iItemTop, iItemWidth, iItemHeight);
        }

        iItemLeft += iItemWidth;
    }

    return (iItemLeft);
}


void CTargetMsgView::OnPaint() 
{
	CPaintDC dc (this);

	CFormView::OnPaint ();

//	DrawSessionState (&dc);
    DrawConnectState (&dc);
	DrawResourceState (&dc);
}


//void CTargetMsgView::DrawSessionState (CPaintDC *pDC)
//{
//    int main = IDI_SYMBOL_CONN_NOTCONN;
//    int overlayError = -1;
//    int overlayWait = -1;
//    int overlayComm = -1;
//
//
//    if (waiting_for_connect == m_SessionState || waiting_for_exclusive == m_ExclusiveSessionState || 
//        waiting_for_connect == m_SessionState || waiting_for_exclusive == m_ExclusiveSessionState)
//    {
//        overlayWait = IDI_SYMBOL_CONN_WAITING;
//    }
//
//    if (RW_established == m_SessionState || exclusive_established == m_SessionState)
//    {
//        main = IDI_SYMBOL_CONN_CONN;
//        overlayError = IDI_SYMBOL_CONN_OK;
//    }
//
//    if (interrupted != m_SessionState &&
//        (RW_established == m_ExclusiveSessionState || exclusive_established == m_ExclusiveSessionState))
//    {
//        main = IDI_SYMBOL_CONN_CONN;
//        if(m_iResourceState >= 0)
//        {
//            overlayComm = IDI_SYMBOL_CONN_COMM;
//        }
//        else
//        {
//            overlayComm = IDI_SYMBOL_CONN_ERROR;
//        }
//    }
//
//
//    if (interrupted == m_SessionState || interrupted == m_ExclusiveSessionState)
//    {
//        main = IDI_SYMBOL_CONN_CONN;
//        overlayWait = -1;
//        overlayError = IDI_SYMBOL_CONN_ERROR;
//    }
//
//    if (-1 != main)
//    {
//	    m_ConnImageList.Draw (pDC,
//	                      main,
//	                      CPoint (SESSION_SYMBOL_LEFT, SESSION_SYMBOL_TOP),
//	                      ILD_TRANSPARENT);
//    }
//
//    if (-1 != overlayError)
//    {
//        m_ConnImageList.Draw (pDC,
//                              overlayError,
//                              CPoint (SESSION_SYMBOL_LEFT, SESSION_SYMBOL_TOP),
//                              ILD_TRANSPARENT);
//    };
//
//    if (-1 != overlayWait)
//    {
//        m_ConnImageList.Draw (pDC,
//                              overlayWait,
//                              CPoint (SESSION_SYMBOL_LEFT, SESSION_SYMBOL_TOP),
//                              ILD_TRANSPARENT);
//    };
//
//    if (-1 != overlayComm)
//    {
//        m_ConnImageList.Draw (pDC,
//                              overlayComm,
//                              CPoint (SESSION_SYMBOL_LEFT, SESSION_SYMBOL_TOP),
//                              ILD_TRANSPARENT);
//    };
//}


void CTargetMsgView::DrawConnectState(CPaintDC *pDC)
{

    int                     iImageId;

    GetConnectImage(m_eConnectState, iImageId);

    m_ConnImageList.Draw (pDC,
                          iImageId,
                          CPoint (SESSION_SYMBOL_LEFT, SESSION_SYMBOL_TOP),
                          ILD_TRANSPARENT);
}


void CTargetMsgView::GetConnectImage(E_TargetOnlConnectState eState, int& iImage)
{
    iImage = -eTOCS_exception + (int)eState;
}


void CTargetMsgView::DrawResourceState (CPaintDC *pDC)
{
    int overlay;

    m_ImageList.Draw (pDC,
                      IDI_SYMBOL_RES,
                      CPoint (RESOURCE_SYMBOL_LEFT, RESOURCE_SYMBOL_TOP),
                      ILD_TRANSPARENT);

    switch (m_iResourceState)
    {
    case -2:
        overlay = IDI_SYMBOL_SERVICESTOPPED;
        break;

    case -1:
        overlay = IDI_SYMBOL_INTERRUPTED;
        break;

    case 1:
        overlay = m_bResourceCleared ? IDI_SYMBOL_PAUSE_CLEARED : IDI_SYMBOL_PAUSE;
        break;

    case 2:
        overlay = m_bResourceCleared ? IDI_SYMBOL_RUN_CLEARED : IDI_SYMBOL_RUN;
        break;

    default:
        overlay = -1;
        break;
		};

    if (overlay != -1)
    {
        m_ImageList.Draw (pDC,
                          overlay,
                          CPoint (RESOURCE_SYMBOL_LEFT, RESOURCE_SYMBOL_TOP),
                          ILD_TRANSPARENT);
    }
}


void CTargetMsgView::UpdateStateText()
{
    CTargetOnlBase* pTarget = GetTargetOnlObject();
    CString strDiagnosis;

    if(pTarget)
    {
        strDiagnosis = pTarget->GetDiagnosisText();
    }

    SetStateText(strDiagnosis);
}


//////////////////////////////////////////////////////////////////////////////
//void CTargetMsgView::UpdateStateText()
//{
//    CString strConnectionText;
//    CString strDebugText;
//    CString strResourceText;
//    CString strClearedText;
//    CString strText;
//    CString strUserConnectInfo;
//    CWnd *pDialogItem;
//
//    if(m_SessionState != interrupted && m_SessionState != RW_established)
//    {
//        strConnectionText.LoadString (IDS_STATETEXT_NOT_CONNECTED);
//    }
//    else
//    {
//        if(m_SessionState == interrupted || m_ExclusiveSessionState == interrupted)
//        {
//            strConnectionText.LoadString (IDS_STATETEXT_INTERRUPTED);
//        }
//        else
//        {
//            strConnectionText.LoadString (IDS_STATETEXT_CONNECTED);
//        }
//
//        if(m_ExclusiveSessionState == interrupted || m_ExclusiveSessionState == exclusive_established)
//        {
//            strDebugText.LoadString (IDS_STATETEXT_DEBUG_ENABLED);
//        }
//    }
//
//    if(m_SessionState == RW_established)
//    {
//        switch(m_iResourceState)
//        {
//        case -2: strResourceText.LoadString (IDS_STATETEXT_SERVICE_STOPPED);  break;
//        case -1: strResourceText.LoadString (IDS_STATETEXT_RESOURCE_ERROR);   break;
//        case 1 : strResourceText.LoadString (IDS_STATETEXT_RESOURCE_PAUSED);  break;
//        case 2 : strResourceText.LoadString (IDS_STATETEXT_RESOURCE_RUNNING); break;
//        }
//
//        if (m_iResourceState == 1 || m_iResourceState == 2)
//        {
//            if (m_bResourceCleared)
//            {
//                strClearedText.LoadString (IDS_STATETEXT_RESOURCE_CLEARED);
//            }
//        }
//    }
//
////    strText = m_strTargetAddr + ": "+ strConnectionText;
//
//    GetUserConnectInfo(strUserConnectInfo);
//    strText = strUserConnectInfo + ": "+ strConnectionText;
//
//    if (!strDebugText.IsEmpty ())
//    {
//        strText += _T (" (") + strDebugText + _T (")");
//    }
//
//    if (!strResourceText.IsEmpty ())
//    {
//        strText += _T (" / ") + strResourceText;
//
//        if (!strClearedText.IsEmpty ())
//        {
//            strText += _T (" (") + strClearedText + _T (")");
//        }
//    }
//
//    pDialogItem = GetDlgItem(IDC_STATE_TEXT);
//    pDialogItem->SetWindowText (strText);
//}


void CTargetMsgView::SetStateText(const CString& crstrStateText)
{
    CWnd *pDialogItem;

    pDialogItem = GetDlgItem(IDC_STATE_TEXT);
    pDialogItem->SetWindowText(crstrStateText);
}


////------------------------------------------------------------------*
///**
// * get user connect info.
// *
// *  get attribute USER_CONNECT_INFO from target node in instance tree
// *
// * @param           
// * @return          
// * @exception       -
// * @see             
//*/
//BOOL CTargetMsgView::GetUserConnectInfo(CString& rstrUserConnectInfo)
//{
//    CXMLNode                    tNode;
//    CComPtr<IUnknown>           pTmp;
//    CComQIPtr<ICEProjInfo>      pProjMan;
//    HRESULT                     hr;
//    CTargetOnlBase*             pTarget = NULL;
//    CString                     strIdPath;
//
//    pTmp.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
//    pProjMan = pTmp;
//    if(!pProjMan)
//    {
//        return FALSE;
//    }
//
//    // now get target online object to retrieve node path
//    pTarget = GetTargetOnlObject();
//    if(!pTarget)
//    {
//        return FALSE;
//    }
//    strIdPath = pTarget->GetIdPath();
//
//    CComPtr<IUnknown>           pTmpNode;
//    CComBSTR                    sId(strIdPath);
//
//    hr = pProjMan->getXMLNodePtr(sId, &pTmpNode);
//    if(hr != S_OK)
//    {
//        return FALSE;
//    }
//
//    if(tNode.SetIXMLNode(pTmpNode))
//    {
//        if(tNode.GetAttribute(CE_XMLATTR_USERADDR, rstrUserConnectInfo))
//        {
//            return TRUE;
//        }
//    }
//    return FALSE;
//}



void CTargetMsgView::OnTargetAddChangeNotify(LPCTSTR pszNewTargetAddr)
{
    ASSERT(pszNewTargetAddr); 
    if(pszNewTargetAddr)
    {
        m_strTargetAddr = (TCHAR*) pszNewTargetAddr;
        UpdateStateText();
    }
}

void CTargetMsgView::OnConnect() 
{
    CTargetOnlBase *pTarget;
    CMsgWnd *pMessageWindow;
    CString strMessage = "OnConnectTest";
    
    pTarget = GetTargetOnlObject();
    ASSERT (pTarget != NULL);
    if (pTarget == NULL)
        return;

    if (pTarget->IsTargetConnected())
        return;

    pMessageWindow = GetMsgView();
    ASSERT (pMessageWindow != NULL);
    if (pMessageWindow == NULL)
    {
        return;
    }

    if(pTarget && pTarget->ShowConnectDialog (strMessage, FALSE) == S_FALSE)
    {
        if (!strMessage.IsEmpty ())
        {
            pMessageWindow->AddMessage (_T (""), E_FACILITY_CESYSTEM, strMessage);
        }
        return;
    }

    // SU: Moved connection of the message session to callback received when an
    // normal variable session has been established. 
    // Within that callback OnSessionCB of the message window the message session
    // will be connected and disconnected. Since exclusive sessions always establish
    // a normal variable session prior to establishing an exclusive session the
    // connection of the message session will solely be done if the variable session
    // has been established.
}

void CTargetMsgView::OnUpdateConnect(CCmdUI* pCmdUI) 
{
    BOOL bEnable = FALSE;
    CTargetOnlBase * pTarget = GetTargetOnlObject();
    if (pTarget)
    {
        bEnable = !pTarget->IsConnected();
    }

    pCmdUI->Enable(bEnable);
}


void CTargetMsgView::OnAcknowledge() 
{
    CMsgWnd *pMessageWindow;

    pMessageWindow = GetMsgView();
    if (pMessageWindow != NULL)
    {
        pMessageWindow->ResetAlarmInSelectedView();
    }
	
}

void CTargetMsgView::OnUpdateAck(CCmdUI* pCmdUI) 
{
    BOOL bEnable = FALSE;
    CMsgWnd *pMessageWindow;

    pMessageWindow = GetMsgView();
    if (pMessageWindow != NULL)
    {
        bEnable = pMessageWindow->GetAlarmState ();
    }

    pCmdUI->Enable(bEnable);
}

void CTargetMsgView::OnAlltasksstart() 
{
    CTargetOnlBase* pTarget = GetTargetOnlObject();
    if (pTarget)
    {
        pTarget->StartAllTasks();
    }
}

void CTargetMsgView::OnUpdateAllTasksStart (CCmdUI *pCmdUI) 
{
    CTargetOnlBase *pTarget;


    // Get TargetOnl object. If TargetOnl object does not exist it
    // is an internal error. Be as friendly as possible in
    // this case (nobody should suffer from our errors).

    pTarget = GetTargetOnlObject ();
    ASSERT (pTarget != NULL);
    if (pTarget == NULL)
    {
        pCmdUI->Enable (TRUE);
        return;
    }

    // Enable starting all tasks if we are connected and the
    // resource is not stopped and the service is not stopped.
    BOOL    bConnected = pTarget->IsTargetConnected();
    BOOL    bResourceIsRunning = pTarget->IsResourceStarted();
    BOOL    bServiceIsRunning = !pTarget->IsServiceStopped();

    pCmdUI->Enable(bConnected && bResourceIsRunning && bServiceIsRunning);
}


void CTargetMsgView::OnAlltasksstop() 
{
    CTargetOnlBase* pTarget = GetTargetOnlObject();
    if (pTarget)
    {
        pTarget->StopAllTasks();
    }
}

void CTargetMsgView::OnUpdateAllTasksStop (CCmdUI *pCmdUI) 
{
    CTargetOnlBase *pTarget;


    // Get TargetOnl object. If TargetOnl object does not exist it
    // is an internal error. Be as friendly as possible in
    // this case (nobody should suffer from our errors).

    pTarget = GetTargetOnlObject ();
    ASSERT (pTarget != NULL);
    if (pTarget == NULL)
    {
        pCmdUI->Enable (TRUE);
        return;
    }


    // Enable stopping all tasks if we are connected and the
    // resource is not stopped and the service is not stopped.
    BOOL    bConnected = pTarget->IsTargetConnected();
    BOOL    bResourceIsRunning = pTarget->IsResourceStarted();
    BOOL    bServiceIsRunning = !pTarget->IsServiceStopped();

    pCmdUI->Enable(bConnected && bResourceIsRunning && bServiceIsRunning);
}



void CTargetMsgView::OnResWarmstart() 
{
    CTargetOnlBase* pTarget = GetTargetOnlObject();
    if (pTarget)
    {
        pTarget->WarmStartResource();
    }
}

void CTargetMsgView::OnResColdstart() 
{
    CTargetOnlBase* pTarget = GetTargetOnlObject();
    if (pTarget)
    {
        pTarget->ColdStartResource();
    }
	
}

void CTargetMsgView::OnResclear() 
{
    CTargetOnlBase * pTarget = GetTargetOnlObject();
    if (pTarget)
    {
        CTargetOnl* pTarg = dynamic_cast<CTargetOnl*> (pTarget);
        if(pTarg)
        {
            pTarg->ClearResource(pTarget->GetId());
        }
    }
}

void CTargetMsgView::OnUpdateResClear(CCmdUI* pCmdUI) 
{
    BOOL bEnable = FALSE;
    CTargetOnlBase* pTargetBase = GetTargetOnlObject();
    CTargetOnl*     pTarget = dynamic_cast<CTargetOnl*> (pTargetBase);

    if (pTarget)
    {
        if(!pTarget->IsInitClearEnabled())
        {
            bEnable = FALSE;
        }
        else
        {
            bEnable = (pTarget->IsTargetConnectedExclusive() && pTarget->IsTargetConnected());
        }
    }

    pCmdUI->Enable(bEnable);
}


void CTargetMsgView::OnConnectExclusive() 
{
    CTargetOnlBase* pTargetBase;
    CTargetOnl*     pTarget = NULL;
    CMsgWnd *pMessageWindow;
    BOOL bConnected;
    CString strMessage;
    
    pTargetBase = GetTargetOnlObject ();
    ASSERT (pTargetBase != NULL);
    if (pTargetBase == NULL)
        return;

    pTarget = dynamic_cast<CTargetOnl*> (pTargetBase);

    pMessageWindow = GetMsgView();
    ASSERT (pMessageWindow != NULL);
    if (pMessageWindow == NULL)
        return;

    bConnected = pTarget->IsTargetConnected ();

    if (bConnected && pTarget->IsTargetConnectedExclusive())
        return;

    if (pTarget->ShowConnectDialog (strMessage, TRUE) == S_FALSE)
    {
        if (!strMessage.IsEmpty ())
        {
            pMessageWindow->AddMessage (_T (""), E_FACILITY_CESYSTEM, strMessage);
        }

        return;
    }

    // SU: Moved connection of the message session to callback received when an
    // normal variable session has been established. 
    // Within that callback OnSessionCB of the message window the message session
    // will be connected and disconnected. Since exclusive sessions always establish
    // a normal variable session prior to establishing an exclusive session the
    // connection of the message session will solely be done if the variable session
    // has been established.
}

void CTargetMsgView::OnUpdateExclusiveConnect(CCmdUI* pCmdUI) 
{
    BOOL bEnable = FALSE;
    CTargetOnlBase * pTargetBase = GetTargetOnlObject();
    CTargetOnl*     pTarget = dynamic_cast<CTargetOnl*> (pTargetBase);

    if (pTarget)
    {
        if(!pTarget->IsDebugEnabled())
        {
            bEnable = FALSE;
        }
        else
        {
            bEnable = !pTarget->IsConnectedExclusive();
        }
    }

    pCmdUI->Enable(bEnable);
}


void CTargetMsgView::OnDisconnectExclusive() 
{
    CTargetOnlBase* pTargetBase = GetTargetOnlObject();

    if (pTargetBase)
    {
        CTargetOnl* pTarget = dynamic_cast<CTargetOnl*> (pTargetBase);
        if(pTarget)
        {
            pTarget->DisconnectExclusive();
        }
    }
}

HBRUSH CTargetMsgView::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	pDC->SetBkColor(0x00ffffff);
	return (HBRUSH) m_phbr->GetSafeHandle();
}

void CTargetMsgView::OnUpdateExclusiveDisconnect(CCmdUI* pCmdUI) 
{
    BOOL bEnable = FALSE;
    CTargetOnlBase* pTargetBase = GetTargetOnlObject();
    CTargetOnl*     pTarget = dynamic_cast<CTargetOnl*> (pTargetBase);
    if (pTarget)
    {
        bEnable = pTarget->IsConnectedExclusive();
    }

    pCmdUI->Enable(bEnable);
}

void CTargetMsgView::OnResStart() 
{
    CTargetOnlBase * pTarget = GetTargetOnlObject();
    if (pTarget)
    {
        pTarget->StartResource();
    }
}

void CTargetMsgView::OnUpdateResStart (CCmdUI *pCmdUI) 
{
    CTargetOnlBase *pTarget;


    // Get TargetOnl object. If TargetOnl object does not exist it
    // is an internal error. Be as friendly as possible in
    // this case (nobody should suffer from our errors).

    pTarget = GetTargetOnlObject ();
    ASSERT (pTarget != NULL);
    if (pTarget == NULL)
    {
        pCmdUI->Enable (TRUE);
        return;
    }

    // Enable starting the resource if we are connected
    // and the resource is not already started and the
    // service is not stopped.

    BOOL    bConnected = pTarget->IsTargetConnected();
    BOOL    bResourceNotStarted = !pTarget->IsResourceStarted();
    BOOL    bServiceIsRunning = !pTarget->IsServiceStopped();

    pCmdUI->Enable(bConnected && bResourceNotStarted && bServiceIsRunning);
}

void CTargetMsgView::OnResStop() 
{
    CTargetOnlBase * pTarget = GetTargetOnlObject();
    if (pTarget)
    {
        pTarget->StopResource();
    }
}

void CTargetMsgView::OnUpdateResStop (CCmdUI *pCmdUI) 
{
    CTargetOnlBase *pTarget;


    // Get TargetOnl object. If TargetOnl object does not exist it
    // is an internal error. Be as friendly as possible in
    // this case (nobody should suffer from our errors).

    pTarget = GetTargetOnlObject ();
    ASSERT (pTarget != NULL);
    if (pTarget == NULL)
    {
        pCmdUI->Enable (TRUE);
        return;
    }


    // Enable stopping the resource if we are connected
    // and the resource is not already stopped and the
    // service is not stopped.
    BOOL    bConnected = pTarget->IsTargetConnected();
    BOOL    bResourceNotStopped = !pTarget->IsResourceStopped();
    BOOL    bServiceIsRunning = !pTarget->IsServiceStopped();

    pCmdUI->Enable(bConnected && bResourceNotStopped && bServiceIsRunning);
}

void CTargetMsgView::OnDisconnect() 
{
    CTargetOnlBase* pTargetBase = GetTargetOnlObject();
    CTargetOnl*     pTarget = NULL;

    if(pTargetBase)
    {
        pTarget = dynamic_cast<CTargetOnl*> (pTargetBase);
    }

    UnsubscribeForState();

    if (pTarget) 
    {
        pTarget->DisconnectExclusive();

        if (S_OK == pTarget->Disconnect()) 
        {
            CMsgWnd *pMessageWindow;
        
            pMessageWindow = GetMsgView();
            // SU: Moved dis connection of the message session to callback received when an
            // normal variable session has been established. 
            // Within that callback OnSessionCB of the message window the message session
            // will be connected and disconnected.
        }
    }
}

void CTargetMsgView::OnUpdateDisconnect(CCmdUI* pCmdUI) 
{
    BOOL bEnable = FALSE;
    CTargetOnlBase * pTarget = GetTargetOnlObject();
    if (pTarget)
    {
        bEnable = pTarget->IsConnected();
    }

    pCmdUI->Enable(bEnable);
}


void CTargetMsgView::OnUpdateColdstart(CCmdUI* pCmdUI) 
{
    BOOL bEnable = FALSE;
    CTargetOnlBase * pTarget = GetTargetOnlObject();
    if (pTarget)
    {
        bEnable = pTarget->IsTargetConnected();
    }

    pCmdUI->Enable(bEnable);
}


void CTargetMsgView::OnUpdateWarmstart(CCmdUI* pCmdUI) 
{
    BOOL bEnable = FALSE;
    CTargetOnlBase * pTarget = GetTargetOnlObject();
    if (pTarget)
    {
        if(!pTarget->IsWarmstartEnabled())
        {
            bEnable = FALSE;
        }
        else
        {
            bEnable = pTarget->IsTargetConnected();
        }
    }
    pCmdUI->Enable(bEnable);
}


CMsgWnd *CTargetMsgView::GetMsgView(bool bMustLive)
{
    return (dynamic_cast<CMsgWnd *> (CEGetDockingTool ("Message View", bMustLive)));
}

void CTargetMsgView::OnSessionNotify (CSC_SESSION_STATE sessionState)
{
    CTargetOnlBase *pTarget;

    SetSessionState(sessionState);
    pTarget = GetTargetOnlObject ();
    assert (pTarget != NULL);
    if (pTarget == NULL)
    {
        return;
    }
 
    if (pTarget->IsTargetConnected () && m_SessionState == RW_established)
    {
        SubscribeForState ();
    }
    else if (m_SessionState == not_connected)
    {
        UnsubscribeForState ();
    }
}

void CTargetMsgView::OnExclusiveSessionNotify (CSC_SESSION_STATE sessionState)
{
    CTargetOnlBase *pTarget;

    pTarget = GetTargetOnlObject ();
    assert (pTarget != NULL);
    if (pTarget == NULL)
        return;

    SetExclusiveSessionState(sessionState);
}

void CTargetMsgView::OnConnectStateNotify(E_TargetOnlConnectState eConnectState)
{
    SetConnectState(eConnectState);
}


CTargetOnlBase* CTargetMsgView::GetTargetOnlObject() 
{
    CTargetSet* pTargetSet = CEGetTargetSet("Project Targets");

    ASSERT(pTargetSet);
    if(pTargetSet)
    {
        return(pTargetSet->GetTarget(m_strTargetName));
    }
    return (NULL);
}


HRESULT CTargetMsgView::SubscribeForState ()
{
    HRESULT hr;
    CMsgWnd *pMessageWindow;
    CMsgFrame *pMessageFrame;
    int iIndex;
    CString strClearedVariable;
    CString strStateVariable;
    short sClearedQuality;
    CComVariant varClearedValue;
    short sStateQuality;
    CComVariant varStateValue;
    long lState=-1;

    if (m_bSubscribedForState)
    {
        return (S_FALSE);
    }

    CCEMAINApp *pApp = (CCEMAINApp *)AfxGetApp();
    CMsgDoc *pDoc = pApp->GetMsgDoc();

    assert (pDoc != NULL);
    if (pDoc == NULL)
    {
        return (E_FAIL);
    }

    pMessageWindow = dynamic_cast<CMsgWnd *> (CEGetDockingTool (CE_COMPMAN_MSGVIEW));
    if (pMessageWindow == NULL)
    {
        return (E_FAIL);
    }

    pMessageFrame = pMessageWindow->GetMsgFrame ();
    if (pMessageFrame == NULL)
        return (E_FAIL);

    pMessageFrame->m_tabWnd.GetActiveTab(iIndex);
    strClearedVariable.Format (_T ("__srt0.%s"), CE_CLEARED_RESOURCE);
    strStateVariable.Format (_T ("__srt0.%s"), CE_STATE_RESOURCE);
	
    hr = pDoc->SubscribeVariable (iIndex, strClearedVariable, &m_ResourceStateCB, m_pClearedSubscription);
    if (FAILED (hr))
    {
        return (hr);
    }

    hr = pDoc->SubscribeVariable (iIndex, strStateVariable, &m_ResourceStateCB, m_pStateSubscription);
    if (FAILED (hr))
    {
        (void)pDoc->UnsubscribeVariable (iIndex, m_pClearedSubscription, &m_ResourceStateCB);
        return (hr);
    }

    m_bSubscribedForState = true;


    // Under certain circumstances (control service not running)
    // we will get no initial notification. For these cases we
    // read synchroneously here.

    hr = pDoc->ReadVariable (iIndex, m_pClearedSubscription, sClearedQuality, varClearedValue);
    if (SUCCEEDED (hr))
    {
        hr = pDoc->ReadVariable (iIndex, m_pStateSubscription, sStateQuality, varStateValue);
    }

    if (FAILED (hr))
    {
        // Variables are subscribed. Don't care if initial read
        // fails because hopefully later there will be notifications.
        return (S_OK);
    }

    if ((sClearedQuality & OPC_QUALITY_MASK) != OPC_QUALITY_BAD)
    {
        bool bCleared=false;

        assert (varClearedValue.vt == VT_BOOL);
        if (varClearedValue.vt == VT_BOOL)
        {
            bCleared = varClearedValue.boolVal != 0;
        }

        SetResourceCleared(bCleared);
    }

    if ((sStateQuality & OPC_QUALITY_MASK) != OPC_QUALITY_BAD)
    {
        assert (varStateValue.vt == VT_I4);
        if (varStateValue.vt == VT_I4)
        {
            lState = varStateValue.lVal;
        }
    }

    SetResourceState (lState);

    return (hr);
}

HRESULT CTargetMsgView::UnsubscribeForState ()
{
    HRESULT hr;
    HRESULT hrResult=S_OK;
    CMsgWnd *pMessageWindow;
    CMsgFrame *pMessageFrame;
    int iIndex;

    CCEMAINApp *pApp = (CCEMAINApp *)AfxGetApp();
    CMsgDoc *pDoc = pApp->GetMsgDoc();

    if (!m_bSubscribedForState)
    {
        return (S_FALSE);
    }

    assert (pDoc != NULL);
    if (pDoc == NULL)
    {
        return (E_FAIL);
    }

    pMessageWindow = dynamic_cast<CMsgWnd *> (CEGetDockingTool (CE_COMPMAN_MSGVIEW));
    if (pMessageWindow == NULL)
    {
        return (E_FAIL);
    }

    pMessageFrame = pMessageWindow->GetMsgFrame ();
    if (pMessageFrame == NULL)
    {
        return (E_FAIL);
    }

    pMessageFrame->m_tabWnd.GetActiveTab(iIndex);

    hr = pDoc->UnsubscribeVariable (iIndex, m_pStateSubscription, &m_ResourceStateCB);
    if (hrResult == S_OK && FAILED (hr))
    {
        hrResult = hr;
    }

    hr = pDoc->UnsubscribeVariable (iIndex, m_pClearedSubscription, &m_ResourceStateCB);
    if (hrResult == S_OK && FAILED (hr))
    {
        hrResult = hr;
    }

    m_bSubscribedForState = false;

    SetResourceCleared (false);
    SetResourceState (0);

    return (hrResult);
}


void CTargetMsgView::OnResourceStateCB (CSubscription *pSubscription)
{
    if (pSubscription == m_pClearedSubscription)
    {
        CComVariant varValue;

        if ((pSubscription->GetQuality () & OPC_QUALITY_MASK) != OPC_QUALITY_GOOD)
        {
            return;
        }

        varValue = pSubscription->GetVariantValue ();

        ASSERT (varValue.vt == VT_BOOL);
        if (varValue.vt != VT_BOOL)
        {
            return;
        }

        if(varValue.boolVal != 0)
        {
            SetResourceCleared(true);
        }
        else
        {
            SetResourceCleared(false);
        }
    }
    else if (pSubscription == m_pStateSubscription)
    {
        CComVariant varValue;

        if ((pSubscription->GetQuality () & OPC_QUALITY_MASK) != OPC_QUALITY_GOOD)
        {
            SetResourceState (-1);
            return;
        }

        varValue = pSubscription->GetVariantValue ();

        ASSERT (varValue.vt == VT_I4);
        if (varValue.vt != VT_I4)
        {
            return;
        }

        SetResourceState(varValue.lVal);
    }
}


void CTargetMsgView::SetResourceCleared (bool bCleared)
{
    m_bResourceCleared = bCleared;

    if (m_hWnd != NULL)
    {
        CRect rect (RESOURCE_SYMBOL_LEFT,
                    RESOURCE_SYMBOL_TOP,
                    RESOURCE_SYMBOL_LEFT + RESOURCE_SYMBOL_WIDTH,
                    RESOURCE_SYMBOL_TOP + RESOURCE_SYMBOL_HEIGHT);

        InvalidateRect (&rect);
        UpdateWindow ();
    }

    UpdateStateText ();
}

void CTargetMsgView::SetResourceState (int iState)
{
    m_iResourceState = iState;

    if (m_hWnd != NULL)
    {
        CRect rect (RESOURCE_SYMBOL_LEFT,
                    RESOURCE_SYMBOL_TOP,
                    RESOURCE_SYMBOL_LEFT + RESOURCE_SYMBOL_WIDTH,
                    RESOURCE_SYMBOL_TOP + RESOURCE_SYMBOL_HEIGHT);

        InvalidateRect (&rect);
        UpdateWindow ();
    }

    UpdateStateText ();
}

void CTargetMsgView::SetSessionState (CSC_SESSION_STATE sessionState)
{
    m_SessionState = sessionState;

    if (m_hWnd != NULL)
    {
        CRect rect (SESSION_SYMBOL_LEFT,
                    SESSION_SYMBOL_TOP,
                    SESSION_SYMBOL_LEFT + SESSION_SYMBOL_CONN_WIDTH,
                    SESSION_SYMBOL_TOP + SESSION_SYMBOL_HEIGHT);

        InvalidateRect (&rect);
        UpdateWindow ();
    }

    UpdateStateText ();
}


void CTargetMsgView::SetExclusiveSessionState (CSC_SESSION_STATE sessionState)
{
    m_ExclusiveSessionState = sessionState;

    if (m_hWnd != NULL)
    {
        CRect rect (SESSION_SYMBOL_LEFT,
                    SESSION_SYMBOL_TOP,
                    SESSION_SYMBOL_LEFT + SESSION_SYMBOL_CONN_WIDTH,
                    SESSION_SYMBOL_TOP + SESSION_SYMBOL_HEIGHT);

        InvalidateRect (&rect);
        UpdateWindow ();
    }

    UpdateStateText ();
}

void CTargetMsgView::SetConnectState(E_TargetOnlConnectState eConnectState)
{
    m_eConnectState = eConnectState;

    if (m_hWnd != NULL)
    {
        CRect rect (SESSION_SYMBOL_LEFT,
                    SESSION_SYMBOL_TOP,
                    SESSION_SYMBOL_LEFT + SESSION_SYMBOL_CONN_WIDTH,
                    SESSION_SYMBOL_TOP + SESSION_SYMBOL_HEIGHT);

        InvalidateRect (&rect);
        UpdateWindow ();
    }

    UpdateStateText ();
}


//------------------------------------------------------------------*
/**
 * Function name	CTargetMsgView::AppendText
 * Description		Appends message text to edit control of this
 *                  message view.
 * @param			LPCTSTR pszMsgText
 * @return			int 
 * @see			    CMsgView::AppendText()
*/
void CTargetMsgView::AppendText(LPCTSTR pszMsgText)
{
    CString str(_T(""));
    CString strAppendText;
    int Length;
    int nStart;
    int nEnd;
    BOOL bScroll;
    int iLineCount;
    int iCurrentLine;
    int iMaxTextLen;
    CMsgEdit* pEdit = NULL;
    
    pEdit =  GetEditCtrl();
    iLineCount = pEdit->GetLineCount();
    iCurrentLine = pEdit->LineFromChar(-1);
    iMaxTextLen = pEdit->GetLimitText();

    pEdit->GetWindowText(str);
    Length = str.GetLength();

    bScroll = (iCurrentLine == iLineCount - 1);
    if(iLineCount > MSGWND_MAXLINES || 
       Length + lstrlen(pszMsgText) + 2 > iMaxTextLen) 
    {
        pEdit->SetRedraw(FALSE);
        pEdit->GetSel(nStart, nEnd);
        int nNewFirst = pEdit->LineIndex(MSGWND_DELLINES);
        pEdit->GetWindowText(str);
        str = str.Mid(nNewFirst);
        pEdit->SetWindowText(str);
        nStart = max(0, nStart - nNewFirst);
        nEnd = max(0, nEnd - nNewFirst);
        pEdit->SetRedraw(TRUE);
        pEdit->SetSel(nStart, nEnd);
    }

    pEdit->GetWindowText(str);
    Length = str.GetLength();
    if (Length == 0)
    {
        strAppendText = pszMsgText;
    }
    else
    {
        strAppendText.Format(_T("\r\n%s"), pszMsgText);
    }


    if (!bScroll)
    {
        pEdit->SetRedraw(FALSE);
        pEdit->GetSel(nStart, nEnd);
        Length = str.GetLength();
        pEdit->SetSel(Length, Length);
        pEdit->ReplaceSel(strAppendText);
        pEdit->SetSel(nStart, nEnd);
        pEdit->SetRedraw(TRUE);
    }
    else
    {
        pEdit->SetRedraw(FALSE);
        Length = str.GetLength();
        pEdit->SetSel(Length, Length);
        pEdit->ReplaceSel(strAppendText);
        int iLine = pEdit->LineFromChar(Length + strAppendText.GetLength());
        Length = pEdit->LineIndex(iLine);
        pEdit->SetSel(Length, Length);
        pEdit->SetRedraw(TRUE);
        pEdit->LineScroll(1, 0);
    }
}

//------------------------------------------------------------------*
/**
 * Updates text in the edit field from the message data associated 
 * with this Tab respectively view.
 * @param       bFocus - true: set focus on edit control, false
 *              do not set focus (default is false).
 * @see			OnActivateView()
*/
void CTargetMsgView::UpdateEditText(bool bFocus)
{
    CMsgWnd* pMsg = NULL;
    CTargetOnlBase* pTarget = NULL;
    CMsgData *pData = NULL;
    CMsgEdit *pEdit = NULL;
    int iIndex;

    pMsg = GetMsgView();
    if(!pMsg) {
        //suppose about to destroy
        return;
    }

    pTarget = GetTargetOnlObject();
    if(!pTarget) {
        //target removed ?
        return;
    }

    pData = pMsg->FindControlMsgData(pTarget,iIndex);
    if(!pData) {
        ASSERT(pData);
        return;
    }

    pEdit = GetEditCtrl();
    if(!pEdit) {
        ASSERT (pEdit != NULL);
        return;
    }

    if(bFocus) {
        pEdit->SetFocus();
    }
    
    if(pEdit->m_hWnd)
    {
        // Save caret position and scroll position
        pData->m_dwEditPos = pEdit->GetSel();
        pData->m_bEndPos = (pEdit->LineFromChar(-1) == pEdit->GetLineCount() - 1);
        int nFirstVisible = pEdit->GetFirstVisibleLine();
        
        pEdit->SetWindowText(pData->m_strText);
        
        if (pData->m_bEndPos) {
            // set position to end of text;
            int iLen = pData->m_strText.GetLength();
            int iLine = pEdit->LineFromChar(iLen);
            iLen = pEdit->LineIndex(iLine);
            pEdit->SetSel(iLen, iLen);
        }
        else {
            // Scroll the edit control so that the first visible line
            // is the first line of text.
            int nowFirstVisible = pEdit->GetFirstVisibleLine();
            pEdit->LineScroll(nFirstVisible-nowFirstVisible, 0);
            pEdit->SetSel(pData->m_dwEditPos);
        }
    }
}

void CTargetMsgView::OnContextMenuCommand ()
{
    CPoint point (0, 0);

    ClientToScreen (&point);
    OnContextMenu (this, point);
}

void CTargetMsgView::OnContextMenu(CWnd *pWnd, CPoint point) 
{
    CFrameWnd *pFrame;
    CMenu menu;
    CMenu *pPopupMenu;
    int nStart;
    int nEnd;
    UINT uiEnable;

    // make sure window is active

    pFrame = GetParentFrame ();
    ASSERT (pFrame != NULL);
    if (pFrame != NULL)
    {
        pFrame->ActivateFrame ();
    };

    if (!menu.LoadMenu (IDR_COMPILEVW_POPUP))
    {
        return;
    }
    pPopupMenu = menu.GetSubMenu (0);
    ASSERT (pPopupMenu != NULL);
    if (pPopupMenu == NULL)
    {
        return;
    }
    GetEditCtrl()->GetSel(nStart, nEnd);
    uiEnable = (nStart == nEnd) ? MF_DISABLED | MF_GRAYED : MF_ENABLED;
    pPopupMenu->EnableMenuItem(IDM_MSG_COPY, uiEnable);
    pPopupMenu->TrackPopupMenu (TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
}


void CTargetMsgView::OnEditCopy () 
{
    GetEditCtrl()->Copy ();
}

void CTargetMsgView::OnEditClear ()
{
    CMsgWnd* pMsg = NULL;
    CMsgData * pData = NULL;
    CTargetOnlBase* pTarget = NULL;
    int iIndex;
    CMsgEdit * pEdit = GetEditCtrl();

    // Delete all of the text.
    pEdit->SetWindowText(_T(""));

    pMsg = GetMsgView();
    if(!pMsg) {
        //suppose about to destroy
        return;
    }

    pTarget = GetTargetOnlObject();
    if(!pTarget) {
        //target removed ?
        return;
    }

    pData = pMsg->FindControlMsgData(pTarget,iIndex);
    if(!pData) {
        ASSERT(pData);
        return;
    }

    pData->m_strText = _T("");
}

BOOL CTargetMsgView::OnToolTipNfy (UINT id, NMHDR *pNMHDR, LRESULT *pResult)
{
    TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;    
    CWnd *pWnd;
    CString strText;

    if (pTTT == NULL)
    {
        return (FALSE);
    };

    if (!(pTTT->uFlags & TTF_IDISHWND))
    {
        return (FALSE);
    };

        
    // idFrom is actually the HWND of the tool
    pWnd = CWnd::FromHandle ((HWND)pNMHDR->idFrom);
    if (pWnd == NULL)
    {
        return (FALSE);
    };

    strText = OnGetToolTipText (pWnd);
    if (strText.IsEmpty ())
    {
        return (FALSE);
    };

    _tcsncpy (pTTT->szText, strText, ARRAY_LEN (pTTT->szText));
    pTTT->hinst = NULL;
    pTTT->lpszText = pTTT->szText;

    return (TRUE);
}

CString CTargetMsgView::OnGetToolTipText (CWnd *pWnd)
{
    CString strText;

    switch (pWnd->GetDlgCtrlID ())
    {
    case IDC_CONNECTTARGET:
    case IDC_OPERATE_EXCLUSIVECONNECT:
    case IDC_OPERATE_EXCLUSIVEDISCONNECT:
    case IDC_DISCONNECTTARGET:
    case IDC_OPERATE_RESSTART:
    case IDC_OPERATE_RESSTOP:
    case IDC_OPERATE_ALLTASKSSTART:
    case IDC_OPERATE_ALLTASKSSTOP:
    case IDC_OPERATE_WARMSTART:
    case IDC_OPERATE_COLDSTART:
    case IDC_OPERATE_RESCLEAR:
    case IDC_OPERATE_ACK:
        pWnd->GetWindowText (strText);
        break;

    default:
        {
            CTargetOnlBase* pTarget = GetTargetOnlObject();
            if(pTarget)
            {
                strText = pTarget->GetDiagnosisText();
            }
        }
        break;
    };

    return (strText);
}



/////////////////////////////////////////////////////////////////////////////
// CMsgEdit

CMsgEdit::CMsgEdit()
{
    m_phbr = new CBrush(0x00ffffff);
    assert(m_phbr);
}

CMsgEdit::~CMsgEdit()
{
    if (m_phbr != NULL)
    {
        delete m_phbr;
    }

}

BEGIN_MESSAGE_MAP(CMsgEdit, CEdit)
	//{{AFX_MSG_MAP(CMsgEdit)
    ON_WM_DESTROY()
	ON_WM_RBUTTONDOWN()
    ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsgEdit message handlers


HBRUSH CMsgEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	pDC->SetBkColor(0x00ffffff);
	return (HBRUSH) m_phbr->GetSafeHandle();
}

void CMsgEdit::OnRButtonDown(UINT nFlags, CPoint point) 
{
    // send WM_CONTEXTMENU message to parent window
    ClientToScreen(&point);
    FORWARD_WM_CONTEXTMENU(GetParent()->m_hWnd, this->m_hWnd, point.x, point.y, ::SendMessage);
}


void CTargetMsgView::OnInitialUpdate() 
{
    CFont *pFont;
    // Set fixed font
    pFont = CFont::FromHandle((HFONT)GetStockObject (ANSI_FIXED_FONT));
    m_Edit.SetFont(pFont);
    m_Edit.SetReadOnly();
    // set text limit to maximum of Windows 95/98
    m_Edit.SetLimitText(MSGWND_MAXTEXTLEN);

    CFormView::OnInitialUpdate();
}
