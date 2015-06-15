
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*




// MsgWnd.cpp : implementation file
//

#include "stdafx.h"
#include "MsgDoc.h"
#include "MsgWnd.h"
#include "regex.h"
#include "msgregex.h"
#include "MsgView.h"
#include "msgrc.h"
#include "cemain.h"
#include "MsgFrame.h"
#include "ImplMsgSink.h"
#include "CECompMan.h"
#include "CEKadMan.h"
#include "utilif.h"

#include "TargetMsgView.h"
#include "targetset.h"
#include "download.h"
#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CMsgWnd

IMPLEMENT_DYNCREATE(CMsgWnd, CSizedDialogBar)



//------------------------------------------------------------------*
/**
 * Function name			CMsgWnd::CMsgWnd
 * Description			
 * @return			
 * @exception			
 * @see			
*/
CMsgWnd::CMsgWnd()
: CSizedDialogBar(),
  m_StateChangeCB (this, OnStateChange),
  m_SessionCB (this, OnSessionCB),
  m_ConnectStateCB (this, OnConnectStateCB),
  m_ExclusiveSessionCB (this, OnExclusiveSessionCB)
{
    m_ImageList.Create (IDB_MSG_INDICATORS_NEW, INDICATOR_BITMAP_WIDTH, 0, INDICATOR_TRANSPARENT_COLOR);
    m_pMsgFrame = new CMsgFrame;
    ASSERT(m_pMsgFrame);
    m_pRegex = new CMsgRegex;
    ASSERT(m_pRegex);
    m_pMsgSink = new CImplMsgSink;
    ASSERT(m_pMsgSink != NULL);
    
    CESMRegisterCallback (&m_StateChangeCB);
}



//------------------------------------------------------------------*
/**
 * Function name			CMsgWnd::~CMsgWnd
 * Description			
 * @return			
 * @exception			
 * @see			
*/
CMsgWnd::~CMsgWnd() 
{
    if(m_pRegex) 
    {
        delete m_pRegex;
    }
    if(m_pMsgSink != NULL) 
    {
        delete(m_pMsgSink);
    }
    
    m_SourceNameCookieMap.RemoveAll();

    CESMUnregisterCallback (&m_StateChangeCB);
    return;
}

BEGIN_MESSAGE_MAP(CMsgWnd, CSizedDialogBar)
    //{{AFX_MSG_MAP(CMsgWnd)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_HELPINFO()
    ON_MESSAGE (MSGTHRD_NOTIFY, OnMsgThrdNotify)
    ON_MESSAGE (MSGTHRD_READY, OnMsgThrdReady)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMsgWnd message handlers


//------------------------------------------------------------------*
/**
 * Function name	CMsgWnd::OnCreate
 * Description			
 * @param			LPCREATESTRUCT lpCreateStruct
 * @return			int 
 * @exception			
 * @see			    SECDialogBar, CMsgFrame
*/
int CMsgWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    RECT rect;
    HINSTANCE hInstance;

    if (SECDialogBar::OnCreate(lpCreateStruct) == -1)
        return -1;
	
    hInstance = AfxFindResourceHandle (MAKEINTRESOURCE (IDR_MSGACCEL), RT_ACCELERATOR);
    m_hAccelerators = ::LoadAccelerators (hInstance, MAKEINTRESOURCE (IDR_MSGACCEL));

    rect.left = MSGWND_FRAME;
    rect.top = MSGWND_FRAME;
    rect.right = lpCreateStruct->cx-3*MSGWND_FRAME;
    rect.bottom = lpCreateStruct->cy-3*MSGWND_FRAME;
    m_pMsgFrame->Create(NULL, "MsgFrame", WS_OVERLAPPED | WS_CHILD | WS_VISIBLE, rect, this, 0, WS_EX_TOOLWINDOW);

    //start messaging of the system viewer tab and the find in files.
    StartMessagingViewer(MSG_SystemMsgViewer);
    StartMessagingViewer(MSG_FindInFilesMsgViewer);
    return 0;
}


//------------------------------------------------------------------*
/**
 * Function name	CMsgWnd::OnSize
 * Description			
 * @param			UINT nType
 * @param			int cx
 * @param			int cy
 * @return			void 
 * @exception			
 * @see			
*/
void CMsgWnd::OnSize(UINT nType, int cx, int cy) 
{
    SECDialogBar::OnSize(nType, cx, cy);

    // TODO: Add your message handler code here

    ::SetWindowPos(m_pMsgFrame->GetSafeHwnd(), NULL,
        MSGWND_FRAME, MSGWND_FRAME, cx - 2*MSGWND_FRAME, cy - 2*MSGWND_FRAME, SWP_NOZORDER);
}


//------------------------------------------------------------------*
/**
 * Function name	CMsgWnd::RegisterMsgSource
 * Description			
 * @param			LPUNKNOWN lpUnkMsgSource
 * @param			DWORD* lpdwCookie
 * @return			HRESULT 
 * @exception			
 * @see			
*/
HRESULT CMsgWnd::RegisterMsgSource(LPUNKNOWN lpUnkMsgSource, DWORD* lpdwCookie) 
{
    ASSERT(m_pMsgSink != NULL);
    return(m_pMsgSink->RegisterMsgSource(lpUnkMsgSource, lpdwCookie));
}


//------------------------------------------------------------------*
/**
 * Function name	CMsgWnd::UnregisterMsgSource
 * Description			
 * @param			LPUNKNOWN lpUnkMsgSource
 * @param			DWORD dwCookie
 * @return			HRESULT 
 * @exception			
 * @see			
*/

HRESULT CMsgWnd::UnregisterMsgSource(LPUNKNOWN lpUnkMsgSource, DWORD dwCookie) 
{
    ASSERT(m_pMsgSink != NULL);
    return(m_pMsgSink->UnregisterMsgSource(lpUnkMsgSource, dwCookie));
}



//------------------------------------------------------------------*
/**
 * Function name	CMsgWnd::GetMsgFrame
 * Description			
 * @return			CMsgFrame * 
 * @exception			
 * @see			
*/
CMsgFrame * CMsgWnd::GetMsgFrame()
{
    return m_pMsgFrame;
}

//------------------------------------------------------------------*
/**
 * Function name	CMsgWnd::OnHelpInfo
 * Description			
 * @param			HELPINFO* pHelpInfo
 * @return			BOOL 
 * @exception			
 * @see			
*/
BOOL CMsgWnd::OnHelpInfo(HELPINFO* pHelpInfo) 
{
    LPARAM lParam;
    lParam = MSG_RESOURCEBASE + HID_BASE_RESOURCE;
    CWinApp* pApp = AfxGetApp();
    if (pApp != NULL)
    {
        pApp->WinHelp(lParam);
        return TRUE;
    }

    return FALSE;	
}

//------------------------------------------------------------------*
/**
 * Function name	CMsgWnd::PreTranslateMessage
 * Description			
 * @param			MSG *pMsg
 * @return			BOOL 
 * @exception			
 * @see			
*/
BOOL CMsgWnd::PreTranslateMessage(MSG *pMsg)
{
    
    // Pump key messages through our local accelerator table first.

    if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
    {
        if (m_hAccelerators != NULL && ::TranslateAccelerator (m_hWnd, m_hAccelerators, pMsg))
        {
            return (TRUE);
        }
    }

    return (SECDialogBar::PreTranslateMessage(pMsg));
}

//------------------------------------------------------------------*
/**
 * Function name	CMsgWnd::ShowCompileWindow
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CMsgWnd::ShowCompileWindow()
{
    ASSERT(m_pMsgFrame);
    m_pMsgFrame->m_tabWnd.ActivateTab(0);
}



void CMsgWnd::ShowDownloadWindow()
{
    ASSERT(m_pMsgFrame);
    m_pMsgFrame->m_tabWnd.ActivateTab(1);
}


void CMsgWnd::ShowFileFindWindow()
{
    ASSERT(m_pMsgFrame);
    int iCount = m_pMsgFrame->m_tabWnd.GetTabCount();
    if (iCount >= 2)
    {
        m_pMsgFrame->m_tabWnd.ActivateTab(iCount - 2);
    }
}


void CMsgWnd::ShowSystemWindow()
{
    ASSERT(m_pMsgFrame);
    int iCount = m_pMsgFrame->m_tabWnd.GetTabCount();
    if (iCount>=1)
    {
        m_pMsgFrame->m_tabWnd.ActivateTab(iCount-1);
    }
}


void CMsgWnd::ClearCompileWindow(bool bShow /*=false*/)
{
    ASSERT(m_pMsgFrame);
    CWnd * pDLWnd;
    LPCTSTR lpszLabel;
    BOOL    bSelected;
    void*   pExtra;

    if(bShow)
    {
        ShowCompileWindow();
    }
    if(m_pMsgFrame->m_tabWnd.GetTabInfo(0, lpszLabel, bSelected, pDLWnd, pExtra))
    {
        ((CMsgView *)pDLWnd)->Clear(0);
    }
}

void CMsgWnd::ClearDownloadWindow(bool bShow /*=false*/)
{
    ASSERT(m_pMsgFrame);
    CWnd * pDLWnd;
    LPCTSTR lpszLabel;
    BOOL    bSelected;
    void*   pExtra;

    if(bShow)
    {
        ShowDownloadWindow();
    }
    if(m_pMsgFrame->m_tabWnd.GetTabInfo(1, lpszLabel, bSelected, pDLWnd, pExtra))
    {
        ((CMsgView *)pDLWnd)->Clear(1);
    }
}

void CMsgWnd::ClearFileFindWindow(bool bShow /*=false*/)
{
    ASSERT(m_pMsgFrame);
    CWnd * pDLWnd;
    LPCTSTR lpszLabel;
    BOOL    bSelected;
    void*   pExtra;

    if (bShow)
    {
        ShowFileFindWindow();
    }

    int iCount = m_pMsgFrame->m_tabWnd.GetTabCount();
    if (iCount >= 2)
    {
        if(m_pMsgFrame->m_tabWnd.GetTabInfo(iCount-2, lpszLabel, bSelected, pDLWnd, pExtra))
        {
            ((CMsgView *)pDLWnd)->Clear(iCount-2);
        }
    }
}

void CMsgWnd::ClearSystemWindow(bool bShow /*=false*/)
{
    ASSERT(m_pMsgFrame);
    CWnd * pDLWnd;
    LPCTSTR lpszLabel;
    BOOL    bSelected;
    void*   pExtra;

    if (bShow)
    {
        ShowSystemWindow();
    }

    int iCount = m_pMsgFrame->m_tabWnd.GetTabCount();
    if (iCount >= 1)
    {
        if(m_pMsgFrame->m_tabWnd.GetTabInfo(iCount-1, lpszLabel, bSelected, pDLWnd, pExtra))
        {
            ((CMsgView *)pDLWnd)->Clear(iCount-1);
        }
    }
}

void CMsgWnd::ClearAllWindows()
{
    ClearCompileWindow();
    ClearDownloadWindow();
    ClearFileFindWindow();
    ClearSystemWindow();
}


void CMsgWnd::ShowTargetWindow(const CString& strTargetName)
{
    ASSERT(m_pMsgFrame);
    
    int index;
    if (FindControlMsgData(strTargetName, index))
    {
        m_pMsgFrame->m_tabWnd.ActivateTab(index);
    }
}


//------------------------------------------------------------------*
/**
 * connect target.
 *
 *  call corresponding function of target view.
 *
 * @param           [in] strTargetName: target name.
 * @param           [in] bActivate: activate target tab?
*/
void CMsgWnd::ConnectTarget(const CString& strTargetName, BOOL bActivate /*=TRUE*/)
{
    ASSERT(m_pMsgFrame);
    
    int             index;
    LPCTSTR         lpszLabel;
    BOOL            bSelected;
    CWnd*           pWnd = NULL;
    void*           pExtra = NULL;
    CTargetMsgView* pTargetView = NULL;

    if (FindControlMsgData(strTargetName, index))
    {
        if(bActivate)
        {
            m_pMsgFrame->m_tabWnd.ActivateTab(index);
        }
        m_pMsgFrame->m_tabWnd.GetTabInfo(index, lpszLabel, bSelected, pWnd, pExtra);
        if(pWnd)
        {
            pTargetView = dynamic_cast<CTargetMsgView*>(pWnd);
            if(pTargetView)
            {
                pTargetView->OnConnect();
            }
        }
    }
}

//------------------------------------------------------------------*
/**
 * disconnect target.
 *
 *  call corresponding function of target view.
 *
 * @param           [in] strTargetName: target name.
 * @param           [in] bActivate: activate target tab?
*/
void CMsgWnd::DisconnectTarget(const CString& strTargetName, BOOL bActivate /*=TRUE*/)
{
    ASSERT(m_pMsgFrame);
    
    int             index;
    LPCTSTR         lpszLabel;
    BOOL            bSelected;
    CWnd*           pWnd = NULL;
    void*           pExtra = NULL;
    CTargetMsgView* pTargetView = NULL;

    if (FindControlMsgData(strTargetName, index))
    {
        if(bActivate)
        {
            m_pMsgFrame->m_tabWnd.ActivateTab(index);
        }
        m_pMsgFrame->m_tabWnd.GetTabInfo(index, lpszLabel, bSelected, pWnd, pExtra);
        if(pWnd)
        {
            pTargetView = dynamic_cast<CTargetMsgView*>(pWnd);
            if(pTargetView)
            {
                pTargetView->OnDisconnect();
            }
        }
    }
}


//------------------------------------------------------------------*
/**
 * connect target exclusively.
 *
 *  call corresponding function of target view.
 *
 * @param           [in] strTargetName: target name.
 * @param           [in] bActivate: activate target tab?
*/
void CMsgWnd::ConnectTargetExclusive(const CString& strTargetName, BOOL bActivate /*=TRUE*/)
{
    ASSERT(m_pMsgFrame);
    
    int             index;
    LPCTSTR         lpszLabel;
    BOOL            bSelected;
    CWnd*           pWnd = NULL;
    void*           pExtra = NULL;
    CTargetMsgView* pTargetView = NULL;

    if (FindControlMsgData(strTargetName, index))
    {
        if(bActivate)
        {
            m_pMsgFrame->m_tabWnd.ActivateTab(index);
        }
        m_pMsgFrame->m_tabWnd.GetTabInfo(index, lpszLabel, bSelected, pWnd, pExtra);
        if(pWnd)
        {
            pTargetView = dynamic_cast<CTargetMsgView*>(pWnd);
            if(pTargetView)
            {
                pTargetView->OnConnectExclusive();
            }
        }
    }
}

//------------------------------------------------------------------*
/**
 * disconnect target exclusively.
 *
 *  call corresponding function of target view.
 *
 * @param           [in] strTargetName: target name.
 * @param           [in] bActivate: activate target tab?
*/
void CMsgWnd::DisconnectTargetExclusive(const CString& strTargetName, BOOL bActivate /*=TRUE*/)
{
    ASSERT(m_pMsgFrame);
    
    int             index;
    LPCTSTR         lpszLabel;
    BOOL            bSelected;
    CWnd*           pWnd = NULL;
    void*           pExtra = NULL;
    CTargetMsgView* pTargetView = NULL;

    if (FindControlMsgData(strTargetName, index))
    {
        if(bActivate)
        {
            m_pMsgFrame->m_tabWnd.ActivateTab(index);
        }
        m_pMsgFrame->m_tabWnd.GetTabInfo(index, lpszLabel, bSelected, pWnd, pExtra);
        if(pWnd)
        {
            pTargetView = dynamic_cast<CTargetMsgView*>(pWnd);
            if(pTargetView)
            {
                pTargetView->OnDisconnectExclusive();
            }
        }
    }
}


//------------------------------------------------------------------*
/**
 * start control (resource).
 *
 *  call corresponding function of target view.
 *
 * @param           [in] strTargetName: target name.
 * @param           [in] bActivate: activate target tab?
*/
void CMsgWnd::StartControl(const CString& strTargetName, BOOL bActivate /*= TRUE*/)
{
    ASSERT(m_pMsgFrame);
    
    int             index;
    LPCTSTR         lpszLabel;
    BOOL            bSelected;
    CWnd*           pWnd = NULL;
    void*           pExtra = NULL;
    CTargetMsgView* pTargetView = NULL;

    if (FindControlMsgData(strTargetName, index))
    {
        if(bActivate)
        {
            m_pMsgFrame->m_tabWnd.ActivateTab(index);
        }
        m_pMsgFrame->m_tabWnd.GetTabInfo(index, lpszLabel, bSelected, pWnd, pExtra);
        if(pWnd)
        {
            pTargetView = dynamic_cast<CTargetMsgView*>(pWnd);
            if(pTargetView)
            {
                pTargetView->OnResStart();
            }
        }
    }
}

//------------------------------------------------------------------*
/**
 * stop control.
 *
 *  call corresponding function of target view.
 *
 * @param           [in] strTargetName: target name.
 * @param           [in] bActivate: activate target tab?
*/
void CMsgWnd::StopControl(const CString& strTargetName, BOOL bActivate /*= TRUE*/)
{
    ASSERT(m_pMsgFrame);
    
    int             index;
    LPCTSTR         lpszLabel;
    BOOL            bSelected;
    CWnd*           pWnd = NULL;
    void*           pExtra = NULL;
    CTargetMsgView* pTargetView = NULL;

    if (FindControlMsgData(strTargetName, index))
    {
        if(bActivate)
        {
            m_pMsgFrame->m_tabWnd.ActivateTab(index);
        }
        m_pMsgFrame->m_tabWnd.GetTabInfo(index, lpszLabel, bSelected, pWnd, pExtra);
        if(pWnd)
        {
            pTargetView = dynamic_cast<CTargetMsgView*>(pWnd);
            if(pTargetView)
            {
                pTargetView->OnResStop();
            }
        }
    }
}

//------------------------------------------------------------------*
/**
 * start all tasks.
 *
 *  call corresponding function of target view.
 *
 * @param           [in] strTargetName: target name.
 * @param           [in] bActivate: activate target tab?
*/
void CMsgWnd::StartAllTasks(const CString& strTargetName, BOOL bActivate /*= TRUE*/)
{
    ASSERT(m_pMsgFrame);
    
    int             index;
    LPCTSTR         lpszLabel;
    BOOL            bSelected;
    CWnd*           pWnd = NULL;
    void*           pExtra = NULL;
    CTargetMsgView* pTargetView = NULL;

    if (FindControlMsgData(strTargetName, index))
    {
        if(bActivate)
        {
            m_pMsgFrame->m_tabWnd.ActivateTab(index);
        }
        m_pMsgFrame->m_tabWnd.GetTabInfo(index, lpszLabel, bSelected, pWnd, pExtra);
        if(pWnd)
        {
            pTargetView = dynamic_cast<CTargetMsgView*>(pWnd);
            if(pTargetView)
            {
                pTargetView->OnAlltasksstart();
            }
        }
    }
}

//------------------------------------------------------------------*
/**
 * stop all tasks.
 *
 *  call corresponding function of target view.
 *
 * @param           [in] strTargetName: target name.
 * @param           [in] bActivate: activate target tab?
*/
void CMsgWnd::StopAllTasks(const CString& strTargetName, BOOL bActivate /*= TRUE*/)
{
    ASSERT(m_pMsgFrame);
    
    int             index;
    LPCTSTR         lpszLabel;
    BOOL            bSelected;
    CWnd*           pWnd = NULL;
    void*           pExtra = NULL;
    CTargetMsgView* pTargetView = NULL;

    if (FindControlMsgData(strTargetName, index))
    {
        if(bActivate)
        {
            m_pMsgFrame->m_tabWnd.ActivateTab(index);
        }
        m_pMsgFrame->m_tabWnd.GetTabInfo(index, lpszLabel, bSelected, pWnd, pExtra);
        if(pWnd)
        {
            pTargetView = dynamic_cast<CTargetMsgView*>(pWnd);
            if(pTargetView)
            {
                pTargetView->OnAlltasksstop();
            }
        }
    }
}

//------------------------------------------------------------------*
/**
 * coldstart reset.
 *
 *  call corresponding function of target view.
 *
 * @param           [in] strTargetName: target name.
 * @param           [in] bActivate: activate target tab?
*/
void CMsgWnd::ColdStartReset(const CString& strTargetName, BOOL bActivate /*= TRUE*/)
{
    ASSERT(m_pMsgFrame);
    
    int             index;
    LPCTSTR         lpszLabel;
    BOOL            bSelected;
    CWnd*           pWnd = NULL;
    void*           pExtra = NULL;
    CTargetMsgView* pTargetView = NULL;

    if (FindControlMsgData(strTargetName, index))
    {
        if(bActivate)
        {
            m_pMsgFrame->m_tabWnd.ActivateTab(index);
        }
        m_pMsgFrame->m_tabWnd.GetTabInfo(index, lpszLabel, bSelected, pWnd, pExtra);
        if(pWnd)
        {
            pTargetView = dynamic_cast<CTargetMsgView*>(pWnd);
            if(pTargetView)
            {
                pTargetView->OnResColdstart();
            }
        }
    }
}

//------------------------------------------------------------------*
/**
 * warmstart reset.
 *
 *  call corresponding function of target view.
 *
 * @param           [in] strTargetName: target name.
 * @param           [in] bActivate: activate target tab?
*/
void CMsgWnd::WarmStartReset(const CString& strTargetName, BOOL bActivate /*= TRUE*/)
{
    ASSERT(m_pMsgFrame);
    
    int             index;
    LPCTSTR         lpszLabel;
    BOOL            bSelected;
    CWnd*           pWnd = NULL;
    void*           pExtra = NULL;
    CTargetMsgView* pTargetView = NULL;

    if (FindControlMsgData(strTargetName, index))
    {
        if(bActivate)
        {
            m_pMsgFrame->m_tabWnd.ActivateTab(index);
        }
        m_pMsgFrame->m_tabWnd.GetTabInfo(index, lpszLabel, bSelected, pWnd, pExtra);
        if(pWnd)
        {
            pTargetView = dynamic_cast<CTargetMsgView*>(pWnd);
            if(pTargetView)
            {
                pTargetView->OnResWarmstart();
            }
        }
    }
}

//------------------------------------------------------------------*
/**
 * initialize clear.
 *
 *  call corresponding function of target view.
 *
 * @param           [in] strTargetName: target name.
 * @param           [in] bActivate: activate target tab?
*/
void CMsgWnd::InitializeClear(const CString& strTargetName, BOOL bActivate /*= TRUE*/)
{
    ASSERT(m_pMsgFrame);
    
    int             index;
    LPCTSTR         lpszLabel;
    BOOL            bSelected;
    CWnd*           pWnd = NULL;
    void*           pExtra = NULL;
    CTargetMsgView* pTargetView = NULL;

    if (FindControlMsgData(strTargetName, index))
    {
        if(bActivate)
        {
            m_pMsgFrame->m_tabWnd.ActivateTab(index);
        }
        m_pMsgFrame->m_tabWnd.GetTabInfo(index, lpszLabel, bSelected, pWnd, pExtra);
        if(pWnd)
        {
            pTargetView = dynamic_cast<CTargetMsgView*>(pWnd);
            if(pTargetView)
            {
                pTargetView->OnResclear();
            }
        }
    }
}


//------------------------------------------------------------------*
/**
 * Function name	CMsgWnd::StartBuild
 * Description			
 * @param			CString & ProjectName
 * @param			CString & strOption
 * @return			void 
 * @exception			
 * @see			
*/
void CMsgWnd::StartBuild(CString & ProjectName, CString & strOption)
{
    CString str;

    // Switch to compile window
    ShowCompileWindow();

    CCEMAINApp *pApp = (CCEMAINApp *)AfxGetApp();     
    CMsgDoc *pDoc = pApp->GetMsgDoc();
    
    ASSERT (pDoc != NULL);
    if (pDoc != NULL)
    {
        int iIndex;
        m_pMsgFrame->m_tabWnd.GetActiveTab(iIndex); 
        pDoc->Clear(iIndex);
    }

    CWnd* pCompileWnd;
    m_pMsgFrame->m_tabWnd.GetActiveTab(pCompileWnd);
    ((CMsgView *)pCompileWnd)->GetEditCtrl().Clear();

    


    //Getting the Compile Command from the KAD
    CString strName = "KadManager";
    LPUNKNOWN   lpUnkMsgSource = CEGetCOM(strName);
    IGeneral*    pGeneral = NULL;
    HRESULT hr = lpUnkMsgSource->QueryInterface(IID_IGeneral, (void**)&pGeneral);
    CComBSTR sCompilerPath;
    HRESULT hr1 = pGeneral->GetCompilerPath(&sCompilerPath);
    
    // start messaging
    str = sCompilerPath;
    str += " \"";
    str += ProjectName;
    str += "\" ";
    if (!strOption.IsEmpty())
    {
        str += strOption;
        str += " -nologo ";
    }

    StartMessaging(LPCTSTR(str), 0);
    
    lpUnkMsgSource->Release();
    pGeneral->Release();
}


//------------------------------------------------------------------*
/**
 * StartBuildLibrary
 *
 * @param           projectName
 * @param           libPath
 * @param           libName
 * @param           strOption
 * @return          
 * @exception       -
 * @see             
*/
void CMsgWnd::StartBuildLibrary(CString &ProjectName, CString &libPath, CString &libName, 
                                CString &strOption, CString& strTargetExt)
{
    CString str;

    // Switch to compile window
    ShowCompileWindow();

    CCEMAINApp *pApp = (CCEMAINApp *)AfxGetApp();     
    CMsgDoc *pDoc = pApp->GetMsgDoc();
    
    ASSERT (pDoc != NULL);
    if (pDoc != NULL)
    {
        int iIndex;
        m_pMsgFrame->m_tabWnd.GetActiveTab(iIndex); 
        pDoc->Clear(iIndex);
    }

    CWnd* pCompileWnd;
    m_pMsgFrame->m_tabWnd.GetActiveTab(pCompileWnd);
    ((CMsgView *)pCompileWnd)->GetEditCtrl().Clear();

    

    //Getting the Compile Command from the KAD
    CString strName = "KadManager";
    LPUNKNOWN   lpUnkMsgSource = CEGetCOM(strName);
    IGeneral*    pGeneral = NULL;
    HRESULT hr = lpUnkMsgSource->QueryInterface(IID_IGeneral, (void**)&pGeneral);
    CComBSTR sCompilerPath;
    HRESULT hr1 = pGeneral->GetCompilerPath(&sCompilerPath);




    // start messaging
    str = sCompilerPath;
    str += " \"";
    str += ProjectName;
    str += "\" ";
    str += strOption;
    str += " -nologo -ml:";
    str += strTargetExt;
    str += " \"";
    str += libPath;
    if(!libPath.IsEmpty() && libPath.GetAt(libPath.GetLength()-1)!=_T('\\')){
        str += "\\";
    }
    str += libName;
    if(!libName.IsEmpty() && (libName.GetLength()<4 || libName.Right(4).Compare(_T(".4cl"))))
    {
        str += ".4cl";
    }
    str += "\" ";
   
    
    StartMessaging(LPCTSTR(str), 0);

    lpUnkMsgSource->Release();
    pGeneral->Release();
}


//------------------------------------------------------------------*
/**
 * Stops messaging respectively acceptance of messages for a specific
 * message viewer type or tab of the message viewer component. After
 * calling this method messages may be received but are not displayed
 * in the corresponding tab window of the message viewer component.<br>
 * <B>Note:</B> Do not call the method for the target viewers/tabs 
 * associated to a target instance. It's only for non target viewers.
 *
 * @param			MSG_VIEWER_TYPE viewer - type of viewer resp. tab
 *                  of the message viewer component.
 * @see			    StartMessagingViewer()
*/
void CMsgWnd::StopMessagingViewer(MSG_VIEWER_TYPE viewer)
{
    int iIndex;
    CMsgData* p = GetMsgDataByViewer(viewer, iIndex);

    if(iIndex == -1) {
        return;
    }

    StopMessaging(iIndex);
}


//------------------------------------------------------------------*
/**
 * Starts messaging respectively acceptance of messages for a specific
 * message viewer type or tab of the message viewer component. After
 * calling this method messages received will be displayed in the 
 * corresponding tab window of the message viewer component.<br>
 * <B>Note:</B> Do not call the method for the target viewers/tabs 
 * associated to a target instance. It's only for non target viewers.
 *
 * @param			MSG_VIEWER_TYPE viewer - type of viewer resp. tab
 *                  of the message viewer component.
 * @see			    StopMessagingViewer()
*/
void CMsgWnd::StartMessagingViewer(MSG_VIEWER_TYPE viewer)
{
    int iIndex;
    CMsgData* p = GetMsgDataByViewer(viewer, iIndex);

    if(iIndex == -1) {
        return;
    }

    StartMessaging(NULL,iIndex);
}


//------------------------------------------------------------------*
/**
 * Function name	CMsgWnd::StartCompileFile
 * Description			
 * @param			CString & ProjectName
 * @param			CString & strTarget
 * @param			CString & FileName
 * @return			void 
 * @exception			
 * @see			
*/
void CMsgWnd::StartCompileFile(CString & ProjectName, CString & strTarget, CString & FileName) 
{
    CString str;

    // Switch to compile window
    ShowCompileWindow();

    CCEMAINApp *pApp = (CCEMAINApp *)AfxGetApp();     
    CMsgDoc *pDoc = pApp->GetMsgDoc();
    
    ASSERT (pDoc != NULL);
    if (pDoc != NULL)
    {
        int iIndex;
        m_pMsgFrame->m_tabWnd.GetActiveTab(iIndex); 
        pDoc->Clear(iIndex);
    }

    CWnd* pCompileWnd;
    m_pMsgFrame->m_tabWnd.GetActiveTab(pCompileWnd);
    ((CMsgView *)pCompileWnd)->GetEditCtrl().Clear();
    //Getting the Compile Command from the KAD
    CString strName = "KadManager";
    LPUNKNOWN   lpUnkMsgSource = CEGetCOM(strName);
    IGeneral*    pGeneral = NULL;
    HRESULT hr = lpUnkMsgSource->QueryInterface(IID_IGeneral, (void**)&pGeneral);
    CComBSTR sCompilerPath;
    HRESULT hr1 = pGeneral->GetCompilerPath(&sCompilerPath);
    
    // start messaging
    str = sCompilerPath;
    str += " \"";
    str += ProjectName;
    str += "\" ";
    str += " -nologo -c";

	if(!strTarget.IsEmpty())
	{
		str += ':';
		str += strTarget;
	}
    str += " \"";
    str += FileName;
    str += "\"";

    StartMessaging(LPCTSTR(str), 0);

    lpUnkMsgSource->Release();
    pGeneral->Release();
}


//------------------------------------------------------------------*
/**
 * Function name	CMsgWnd::StartMessaging
 * Description			
 * @param			LPCTSTR lpszCmd
 * @param			UINT uiIndex
 * @return			int 
 * @exception			
 * @see			
*/
int CMsgWnd::StartMessaging(LPCTSTR lpszCmd, UINT uiIndex) 
{
    PROCESS_INFORMATION piProcInfo; 
    STARTUPINFO			siStartInfo; 
    BOOL				bResult;
    SECURITY_ATTRIBUTES saInherit = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};

    SECURITY_ATTRIBUTES saAttr; 
    CCEMAINApp *pApp = (CCEMAINApp *)AfxGetApp();
    CMsgDoc *pDoc = pApp->GetMsgDoc();
    CMsgData & MsgData = pDoc->m_arrMsgData[uiIndex];

    if(MsgData.m_MsgViewerType != MSG_CmdLineMsgViewer) 
    {
	// the MsgViewer uses the _ICEMessageEvent interface
	// only set the hWnd to send the text to them
		MsgData.m_hwndClient = GetSafeHwnd();
		return(0);
    }
    if(MsgData.m_pMsgThrd) 
    {
        if(MsgData.m_pMsgThrd->IsThreadRunning()) 
        {
            FC_DEBUGPRINT(_T("CEMSG> Thread still running\n"));
            return 0;
        } 
        else 
        {
            delete MsgData.m_pMsgThrd;
            MsgData.m_pMsgThrd = NULL;
        }
    }

    MsgData.m_pMsgThrd = new(CMsgThrd);
    ASSERT(MsgData.m_pMsgThrd);
    if(!MsgData.m_pMsgThrd) 
    {
        return -1;
    }

    // Set the bInheritHandle flag so pipe handles are inherited. 
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
    saAttr.bInheritHandle = TRUE; 
    saAttr.lpSecurityDescriptor = NULL; 

    //ASSERT(MsgData.m_hChildStdoutRd == NULL);
    //ASSERT(MsgData.m_hChildStdoutWr == NULL);
    //ASSERT(MsgData.m_hChildStdinRd == NULL);
    //ASSERT(MsgData.m_hChildStdinWr == NULL);

    // Create a pipe for the child process's STDOUT. 
    if(!CreatePipe(&MsgData.m_hChildStdoutRd, &MsgData.m_hChildStdoutWr, &saAttr, 0)) 
    {
	    MessageBox("Stdout pipe creation failed\n"); 
        delete MsgData.m_pMsgThrd;
        MsgData.m_pMsgThrd = NULL;
		    return -1;
    }

    // Create a pipe for the child process's STDIN. 
    if(!CreatePipe(&MsgData.m_hChildStdinRd, &MsgData.m_hChildStdinWr, &saAttr, 0)) 
    {
        MessageBox("Stdin pipe creation failed\n"); 
        delete MsgData.m_pMsgThrd;
        MsgData.m_pMsgThrd = NULL;
        return(FALSE);
    }
    
    ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );
    siStartInfo.cb = sizeof(STARTUPINFO); 

    // set a write handle to the pipe to be the STDOUT for the child process
    siStartInfo.hStdInput = MsgData.m_hChildStdinRd;
    siStartInfo.hStdOutput = MsgData.m_hChildStdoutWr; 
    siStartInfo.hStdError = MsgData.m_hChildStdoutWr; 
    siStartInfo.wShowWindow = SW_HIDE;
    siStartInfo.dwFlags = STARTF_USESTDHANDLES|STARTF_USESHOWWINDOW ; 

    // Create the child process. 
    bResult = CreateProcess(NULL, 
        (LPTSTR)lpszCmd,// command line 
        NULL,			// process security attributes 
        NULL,			// primary thread security attributes 
        TRUE,			// handles are inherited 
        0,				// creation flags 
        NULL,			// use parent's environment 
        NULL,			// use parent's current directory 
        &siStartInfo,	// STARTUPINFO pointer 
        &piProcInfo);	// receives PROCESS_INFORMATION 
    CloseHandle(MsgData.m_hChildStdoutWr);
    CloseHandle(MsgData.m_hChildStdinRd);
    MsgData.m_hChildStdoutWr = NULL;
    MsgData.m_hChildStdinRd = NULL;

    if(!bResult) 
    {
        FC_DEBUGPRINT1(_T("CEMSG> Can't create process %s\n"), lpszCmd);
        CloseHandle(MsgData.m_hChildStdoutRd);
        CloseHandle(MsgData.m_hChildStdinWr);
        MsgData.m_hChildStdoutRd = NULL;
        MsgData.m_hChildStdinWr = NULL;
        delete MsgData.m_pMsgThrd;
        MsgData.m_pMsgThrd = NULL;
        return -1;
    }
    else
    {
        CloseHandle(piProcInfo.hThread);
        CloseHandle(piProcInfo.hProcess);

          // set the handle for reading the child stdout!
        MsgData.m_pMsgThrd->SetPipeHandle(MsgData.m_hChildStdoutRd,MsgData.m_hChildStdinWr) ; 
        MsgData.m_pMsgThrd->SetNotificationWnd(GetSafeHwnd());
        MsgData.m_pMsgThrd->SetIndex(uiIndex);
        MsgData.m_pMsgThrd->StartCompNotifyThread();
    }

    return 0;
}


//------------------------------------------------------------------*
/**
 * Function name	CMsgWnd::StopMessaging
 * Description			
 * @param			UINT uiIndex
 * @return			int 
 * @exception			
 * @see			
*/
int CMsgWnd::StopMessaging(UINT uiIndex)
{
    CCEMAINApp *pApp = (CCEMAINApp *)AfxGetApp();
    CMsgDoc *pDoc = pApp->GetMsgDoc();
    CMsgData & MsgData = pDoc->m_arrMsgData[uiIndex];

    if(MsgData.m_MsgViewerType != MSG_CmdLineMsgViewer) {
		// the MsgViewer uses the _ICEMessageEvent interface
		// only reset the hWnd to send the text to them
		MsgData.m_hwndClient = NULL;
		return(0);
    }
    if(!MsgData.m_pMsgThrd) {
        // no connection established
        return 0;
    }

    if(MsgData.m_pMsgThrd->IsThreadRunning()) {

        // Send break to process and finish it
        DWORD dummy;

        FC_DEBUGPRINT1(_T("CEMSG> Kill Msg thread id= 0x%X\n"),
        MsgData.m_pMsgThrd->GetThreadId());
        char cr = '\003';  //BREAK
        WriteFile(MsgData.m_hChildStdinWr, &cr, 1, &dummy, NULL);
        MsgData.m_pMsgThrd->SendKillCompNotifyThread();

        //avoid send messages during cleanup phase
        //MsgData.m_pMsgThrd->Lock();
        FC_DEBUGPRINT(_T("CEMSG> clean up message loop\n"));
        //beyond this point no messages will be sent by the thread
        if(GetSafeHwnd() == MsgData.m_pMsgThrd->GetNotificationWnd()){
            MSG msg;
            // eat up all messages for message window
            // especially pending WM_NOTIFYTHRD messages
            while (PeekMessage(&msg, GetSafeHwnd(), 0, 0, PM_REMOVE)) {}
        }
        FC_DEBUGPRINT(_T("CEMSG> clean up message loop - done\n"));
        //MsgData.m_pMsgThrd->UnLock();
        MsgData.m_pMsgThrd->KillCompNotifyThread();
    }

    delete MsgData.m_pMsgThrd;
    MsgData.m_pMsgThrd = NULL;
    return 0;
}

//------------------------------------------------------------------*
/**
 * OnMsgThrdNotify(): receives the messages sended from OnNewMessage.
 *
 * @param           WPARAM WParam - index of tab resp. message data
 *                  array to add the message to.
 * @param           LPCTSTR szMsgText - original text of message
 * @return          LRESULT - always 0 for message processed.
 * @see             ON_MESSAGE in the MESSAGE_MAP and OnNewMessage()
 *                  in CImplMsgSink
*/
LRESULT CMsgWnd::OnMsgThrdNotify(WPARAM WParam, LPCTSTR szMsgText)
{
    // WParam contains the array index
    CCEMAINApp *pApp = (CCEMAINApp *)AfxGetApp();
    CMsgDoc *pDoc = pApp->GetMsgDoc();
    int index = (int)WParam;
    int iActIndex;
    CWnd* pViewWnd = NULL;
    CMsgView* pView = NULL;
    CTargetMsgView* pTargetMsgView = NULL;

    int iUpperBound = pDoc->m_arrMsgData.GetUpperBound();
    if (index < 0 || index > iUpperBound) {
        //invalid index
        return 0;
    }

    // no alarms for non-control views
    if ((index > 1) && (index < iUpperBound - 1) && CheckMessage(szMsgText))
    { 
        SetAlarm(index);
    }

    ASSERT(pDoc);
    CMsgData & MsgData = pDoc->m_arrMsgData[index];

    // Append text
    if (!MsgData.m_strText.IsEmpty()) {
        MsgData.m_strText += _T("\r\n");
    }

    MsgData.m_strText += szMsgText;
    MsgData.m_iLines++;

    if (MSGWND_MAXLINES <= MsgData.m_iLines ||
        MsgData.m_strText.GetLength() > MSGWND_MAXTEXTLEN) {
        int i;
        // maximum number of lines reached -> shorten text
        for (i=0; i < MSGWND_DELLINES; i++) {
            int pos = MsgData.m_strText.Find(_T("\r\n"));
            if (pos < 0) {
                MsgData.m_strText = _T("");
                MsgData.m_iLines = 0;
                break;
            }

            MsgData.m_strText = MsgData.m_strText.Mid(pos + 2);
            MsgData.m_iLines--;
        }
    }
    
    //obtain the index of the active tab
    m_pMsgFrame->m_tabWnd.GetActiveTab(iActIndex);
    
    // write the text only if the if the active tab is equal to target tab
    if(index == iActIndex) 
    {   
        //index of message and index of active tab are the same
        m_pMsgFrame->m_tabWnd.GetActiveTab(pViewWnd);
        pView = dynamic_cast<CMsgView *> (pViewWnd);
        if(pView) 
        {
            
            pView->AppendText(szMsgText);
            return 0;
        }

        pTargetMsgView = dynamic_cast<CTargetMsgView *> (pViewWnd);
        if(pTargetMsgView)
        {
            pTargetMsgView->AppendText(szMsgText);
            return 0;            
        }

        ASSERT(pTargetMsgView);
    }
    return 0;
}



LRESULT CMsgWnd::OnMsgThrdReady(WPARAM wParam, LPARAM lParam)
{
    CMainFrame* pMainFrame = NULL;
    pMainFrame = dynamic_cast<CMainFrame* > (AfxGetMainWnd ());
    ASSERT(pMainFrame);
    if (pMainFrame != NULL)
    {
        pMainFrame->NotifyCompileReady();
    }

    return 0;
}



//------------------------------------------------------------------*
/**
 * Function name	CMsgWnd::CheckMessage
 * Description			
 * @param			LPCTSTR szMsgText
 * @return			BOOL 
 * @exception			
 * @see			
*/
BOOL CMsgWnd::CheckMessage(LPCTSTR szMsgText)
{
    int iMsgType;
    BOOL bRet = FALSE;

    if (!m_pRegex->EvaluateMessage(szMsgText))
    {
        iMsgType = m_pRegex->GetMessageType();
        if (CEMSG_TYPE_FATALERROR == iMsgType)
        {
            bRet = TRUE;
        }
    }

    return bRet;
}


//------------------------------------------------------------------*
/**
 * Function name	CMsgWnd::SetAlarm
 * Description			
 * @param			int index
 * @return			void 
 * @exception			
 * @see			
*/
void CMsgWnd::SetAlarm(int index)
{
    CCEMAINApp *pApp = (CCEMAINApp *)AfxGetApp();
    CMsgDoc *pDoc = pApp->GetMsgDoc();

    ASSERT(index >= 0 || index <= pDoc->m_arrMsgData.GetUpperBound());

    CMsgData & MsgData = pDoc->m_arrMsgData[index];
    if (MsgData.m_bAlarm)
    {
        // Alarm already set
        return;
    }
    MsgData.m_bAlarm = TRUE;
    m_pMsgFrame->m_tabWnd.SetTabColor(index, CEMSG_ALARMCOLOR);
}


//------------------------------------------------------------------*
/**
 * Function name			CMsgWnd::GetAlarmState
 * Description			
 * @return			BOOL 
 * @exception			
 * @see			
*/
BOOL CMsgWnd::GetAlarmState()
{
    CCEMAINApp *pApp = (CCEMAINApp *)AfxGetApp();
    CMsgDoc *pDoc = pApp->GetMsgDoc();

    int index;
    m_pMsgFrame->m_tabWnd.GetActiveTab(index);
    CMsgData & MsgData = pDoc->m_arrMsgData[index];

    return MsgData.m_bAlarm;
}

//------------------------------------------------------------------*
/**
 * Function name			CMsgWnd::ResetAlarmInSelectedView
 * Description			
 * @param			void
 * @return			void 
 * @exception			
 * @see			
*/
void CMsgWnd::ResetAlarmInSelectedView(void)
{
    int index;
    m_pMsgFrame->m_tabWnd.GetActiveTab(index);
    ResetAlarm(index);
}


//------------------------------------------------------------------*
/**
 * Function name			CMsgWnd::ResetAlarm
 * Description			
 * @param			int index
 * @return			void 
 * @exception			
 * @see			
*/
void CMsgWnd::ResetAlarm(int index)
{
    CCEMAINApp *pApp = (CCEMAINApp *)AfxGetApp();
    CMsgDoc *pDoc = pApp->GetMsgDoc();

    assert (index >= 0 || index <= pDoc->m_arrMsgData.GetUpperBound());

    CMsgData & MsgData = pDoc->m_arrMsgData[index];
    if (!MsgData.m_bAlarm)
    {
        // Alarm already reset
        return;
    }

    MsgData.m_bAlarm = FALSE;
    m_pMsgFrame->m_tabWnd.SetTabColor(index, CEMSG_STANDARDCOLOR);
}

//------------------------------------------------------------------*
/**
 * Function name    CMsgWnd::OnStateChange
 * Description			
 * @param			CEStateNotify stateNotify
 * @param			unsigned int uiStage
 * @return			void 
 * @exception			
 * @see			
*/
void CMsgWnd::OnStateChange(CEStateNotify stateNotify, unsigned int uiStage)
{
    //Ask if a new component is created
    if (stateNotify == CECreateComponentNotify)
    {
        //Get the name of the component
        CString strName = CESMGetComponentName ();
        DWORD   dwCookie;
        BOOL    bLookup;
        //Look up in the SourceNameCookieMap if the strName, dwCookie allready exists
        bLookup = m_SourceNameCookieMap.Lookup(strName, dwCookie);
        //if not, get the COM component, make an advise and add the couple strName dwCookie 
        //to the map 
        if(!bLookup)
        {
            LPUNKNOWN lpUnkMsgSource = CEGetCOM(strName);
            if(lpUnkMsgSource != NULL)
            {
                //////////////////////
                //Just for test AddTarget, Normaly if stateNotify == CEAddTargetNotify
//                CTargetOnlBase * pTarget;
//                OnAddTarget (pTarget);
                //////////////////////
                
                m_pMsgSink->RegisterMsgSource(lpUnkMsgSource, &dwCookie);
                m_SourceNameCookieMap.SetAt(strName, dwCookie);
                //if the component is the KadManager and I didn't allready
                //did this (because it's allready in the list!!!) than do it
                if(strName == "KadManager")
                {
                    ITargets*    pTargets = NULL;
                    HRESULT hr = lpUnkMsgSource->QueryInterface(IID_ITargets, (void**)&pTargets);
                    if(FAILED(hr))
                    {
                        lpUnkMsgSource->Release();
                        return;
                    }
                    CComPtr<IEnumTargets> EnumTargets;
                    hr = pTargets->GetTargetEnumerator(&EnumTargets);
                    if(hr != S_OK)
                    {
                        pTargets->Release();
                        lpUnkMsgSource->Release();
                        return;
                    }
                    CComPtr<ITarget> rgelt;
                    ULONG n;
                    hr = EnumTargets->Next(1, &rgelt, &n);
                    if(hr != S_OK)
                    {
                        pTargets->Release();
                        lpUnkMsgSource->Release();
                        return;
                    }
                    while ((hr!=S_FALSE) && rgelt)
                    {
                        WORD  FacilityCode;
                        HRESULT hr = rgelt->GetFacilityCode(&FacilityCode);
                        if(hr != S_OK)
                        {
                            rgelt.Release();
                            pTargets->Release();
                            lpUnkMsgSource->Release();
                            return;
                        }
                        m_pMsgSink->AddFacilityCodeToList(FacilityCode);
                        rgelt.Release();
                        hr = EnumTargets->Next(1, &rgelt, &n);
                    }
                    rgelt.Release();
                    pTargets->Release();
                }
                lpUnkMsgSource->Release();
            }
        }
    }
    //Else ask if a new component is being deleted
    else if (stateNotify == CEHandsOffComponentNotify)
    {
        //Get the name of the component
        CString strName = CESMGetComponentName ();
        DWORD   dwCookie;
        BOOL    bLookup;
        //Look up in the map if the couple strName, dwCookie allredy exists
        bLookup = m_SourceNameCookieMap.Lookup(strName, dwCookie);
        //if yes, make an advise and delete the couple from the map.
        if(bLookup)
        {
          LPUNKNOWN lpUnkMsgSource = CEGetCOM(strName);
          m_pMsgSink->UnregisterMsgSource(lpUnkMsgSource, dwCookie);
          lpUnkMsgSource->Release();
        }
    }
    else
    {
        switch(stateNotify) 
        {
        case CEPreCloseProjectNotify:
            {
                StopMessagingViewer(MSG_CmdLineMsgViewer);
                //stop download messaging
                StopMessagingViewer(MSG_DownLoadMsgViewer);
         
                // stop find in files
                CComQIPtr<ICEFindInFiles>  pIFindInFiles;
                CComPtr<IUnknown>       pTmp;
                pTmp.Attach(CEGetCOM(CE_COMPMAN_FIF));
                pIFindInFiles = pTmp;
                ASSERT(!(!pIFindInFiles));
                if(!(!pIFindInFiles))
                {
                    BOOL bActive;
                    pIFindInFiles->IsFindActive(&bActive);
                    if (bActive)
                    {
                        pIFindInFiles->StopFind();
                    }
                }
                ClearAllWindows();
            }
            break;
        case CEPostOpenProjectNotify:
            //start download messaging
            StartMessagingViewer(MSG_DownLoadMsgViewer); 
            break;
        case CEAddTargetNotify:
            OnAddTarget (CESMGetTarget ());
            break;
        case CERemoveTargetNotify:
            OnRemoveTarget (CESMGetTarget ());
            break;
        case CERenameTargetNotify:
            OnRenameTarget (CESMGetTarget (), CESMGetOldTargetName ());
            break;
        case CETargetAddressModifyNotify:
            OnAddressChanged(CESMGetTarget());
            break;
        case CEDownloadInitiatedNotify:
            OnAdviseDownloader(CESMGetTargetName());
            break;
        case CEDownloadCompleteNotify:
            OnUnadviseDownloader(CESMGetTargetName());
            break;
        case CEDownloadAbortedNotify:
            OnUnadviseDownloader(CESMGetTargetName());
            break;
        default:
            break;
        }
    }
}


//------------------------------------------------------------------*
/**
 * Function name	CMsgWnd::AddMessage
 * Description			
 * @param			LPCTSTR pszMachine
 * @param			WORD wScrFacility
 * @param			LPCTSTR pszDescr
 * @return			void 
 * @exception			
 * @see			
*/
void CMsgWnd::AddMessage(LPCTSTR pszMachine, WORD wScrFacility, 
                         LPCTSTR pszDescr) 
{
    MSG_VIEWER_TYPE RequestedViewer;
    CMsgData *pMsgData;
    CString strMachine;
    int iIndex;

    CCEMAINApp *pApp = (CCEMAINApp *)AfxGetApp();     
    CMsgDoc *pDoc = pApp->GetMsgDoc();
    
    if (pDoc == NULL)
        return;

    // first build groups by wScrFacility
    switch(wScrFacility) 
    {
        case E_FACILITY_CONTROL:
        case E_FACILITY_IO:
        case E_FACILITY_SYSCOM:
            RequestedViewer = MSG_ControlMsgViewer;
            break;
        case E_FACILITY_CODEGEN:
            RequestedViewer = MSG_CompileMsgViewer;
            break;
        case E_FACILITY_CEDOWNLOAD:
            RequestedViewer = MSG_DownLoadMsgViewer;
            break;
        case E_FACILITY_CEFINDINFILES:
            RequestedViewer = MSG_FindInFilesMsgViewer;
            break;
        case E_FACILITY_CESYSTEM:
        case E_FACILITY_CEBPMAN:
        case E_FACILITY_CEGR:
        case E_FACILITY_CEST:
        case E_FACILITY_CEEXE:
            RequestedViewer = MSG_SystemMsgViewer;
            break;
		case E_FACILITY_CEXREF:
            RequestedViewer = MSG_SystemMsgViewer;
            break;

        default:
            RequestedViewer = MSG_SystemMsgViewer;
            break;
    }

    strMachine = pszMachine;

    pMsgData = pDoc->FindMsgData(RequestedViewer, &strMachine, &iIndex);
    if (pMsgData == NULL)
    {
        FC_DEBUGPRINT(_T ("CEMSG> Target not found\n"));
        return;
    }

    ::SendMessage (GetSafeHwnd (), MSGTHRD_NOTIFY, iIndex, (LPARAM)pszDescr);
}


//------------------------------------------------------------------*
/**
 * Function name	CMsgWnd::OnAddTarget
 * Description			
 * @param			CTargetOnlBase *pTarget
 * @return			void 
 * @exception			
 * @see			
*/
void CMsgWnd::OnAddTarget(CTargetOnlBase *pTarget) 
{
    CMsgData MsgData;
    int uiIndex;
    int iIndex;
   
    try 
    {
        CCEMAINApp *pApp = (CCEMAINApp *)AfxGetApp();     
        CMsgDoc *pDoc = pApp->GetMsgDoc();

        if(!pDoc) 
        {
            throw E_POINTER;
        }
        
        if (pTarget == NULL) 
        {
            ASSERT (pTarget != NULL);
            throw E_POINTER;            
        }
        
        ASSERT (m_pMsgFrame != NULL);
        if (m_pMsgFrame == NULL)
        {
            throw E_POINTER;
        }
        
        if (FindControlMsgData(pTarget,iIndex) != NULL)
        {
            AfxMessageBox (IDS_CANNOT_CREATE_MESSAGE_WINDOW, MB_ICONEXCLAMATION);
            return;
        }

        // insert alphabetically
        uiIndex = (UINT)pDoc->GetTargetInsertPosition(pTarget->GetId());
        
        // add MsgData entry
        MsgData.m_pTarget = pTarget;
        MsgData.m_MsgViewerType = MSG_ControlMsgViewer;
        MsgData.m_hwndClient = NULL;

        pDoc->m_arrMsgData.InsertAt(uiIndex, MsgData);
        
        //add a Tab with a Dialog and an Icon with the tab text
        m_pMsgFrame->InsertTargetTab(uiIndex, pTarget->GetId());

        pDoc->RegisterSessionCB(uiIndex, &m_SessionCB);
        pDoc->RegisterExclusiveSessionCB(uiIndex, &m_ExclusiveSessionCB);
        pDoc->RegisterConnectStateCB(uiIndex, &m_ConnectStateCB);
    }
    catch(HRESULT) 
    {
        return;
    }
}


//------------------------------------------------------------------*
/**
 * Function name	CMsgWnd::OpenMessageSession
 * Description		Opens a message session with 4CSC for a specific
 *                  Target object. The target control will be identified by the address
 *                  attribute of the Target object. The pointer to the message session
 *                  will be stored in a MsgData object. The sink object for the
 *                  messages CImplMsgSink will be advised to the message session
 *                  object via a call to RegisterMsgSource.	
 * @param			CTargetOnlBase* pTarget
 * @param			CMsgData & MsgData
 * @return			HRESULT 
 * @exception			
 * @see			
*/
HRESULT CMsgWnd::OpenMessageSession(CTargetOnlBase* pTarget, CMsgData & MsgData)
{
    HRESULT hr;
    CComPtr<IFCSessions> pIFCSessions;
    CComPtr<IUnknown> pUnknown;
    CComBSTR sMachine;

    // try to get the IFCMessageSession interface
    hr = CoCreateInstance (CLSID_FCSessions, NULL, CLSCTX_ALL, IID_IFCSessions, (void**)&pIFCSessions);
    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        CString strMessage;

        strMessage.Format (IDS_NO_COMPONENT, _T ("Sessions"));
        UTIL_FatalError(strMessage, hr);
        return (hr);
    }


    // OpenMessageSession should have machine address and configuration parameter.
    // Currently both parameters have to be the machine address.
    sMachine = pTarget->GetControlAddress ();
    hr = pIFCSessions->OpenMessageSession(sMachine, &pUnknown);
    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        CString strMessage;

        strMessage.Format(IDS_NO_COMPONENT, _T ("Message Session"));
        UTIL_FatalError (strMessage, hr);
        return (hr);
    }
    hr = pUnknown->QueryInterface(IID_IFCConnect, (LPVOID *)&MsgData.m_pIFCMessageSession);
    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        CString strMessage;

        strMessage.Format(IDS_NO_COMPONENT, _T ("Message Session"));
        UTIL_FatalError (strMessage, hr);
        return (hr);
    }

    hr = m_pMsgSink->RegisterMsgSource(MsgData.m_pIFCMessageSession, &MsgData.m_dwCookie);
    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        CString strMessage;

        strMessage.Format (IDS_NO_COMPONENT, _T ("Message Session"));
        UTIL_FatalError (strMessage, hr);
        return (hr);
    }

    return (S_OK);
}


//------------------------------------------------------------------*
/**
 * Function name	CMsgWnd::CloseMessageSession
 * Description		If an message session is opened the sink object will be
 *                  unregistered and the message session object referenced by 
 *                  m_pIFCMessageSession will be released	
 * @param			CMsgData & MsgData
 * @return			void 
 * @exception			
 * @see			
*/
void CMsgWnd::CloseMessageSession(CMsgData & MsgData)
{
    HRESULT hr;

    if(MsgData.m_pIFCMessageSession) {
        hr = m_pMsgSink->UnregisterMsgSource(MsgData.m_pIFCMessageSession, MsgData.m_dwCookie);
        MsgData.m_pIFCMessageSession = NULL;
        if(FAILED(hr)) {
            throw hr;
        }
    }
}


//------------------------------------------------------------------*
/**
 * Function name	CMsgWnd::OnRemoveTarget
 * Description			
 * @param			CTargetOnlBase *pTarget
 * @return			void 
 * @exception			
 * @see			
*/
void CMsgWnd::OnRemoveTarget(CTargetOnlBase *pTarget)
{
    CMsgData *pMsgData;
    CCEMAINApp *pApp = NULL;
    CMsgDoc *pDoc = NULL;
    int iIndex;

    try
    {
        //get document first for removal
        pApp = (CCEMAINApp *)AfxGetApp(); 
        ASSERT(pApp);
        if(!pApp) {
            FC_DEBUGPRINT (_T ("CEMSG> Main application object not found\n"));
            return;
        }
        pDoc = pApp->GetMsgDoc();
        ASSERT(pDoc);
        if(!pDoc) {
            FC_DEBUGPRINT (_T ("CEMSG> Message document not found\n"));
            return;
        }

        pMsgData = FindControlMsgData(pTarget,iIndex);
        if (pMsgData == NULL) {
            return;
        }

        DisconnectMessageSession(pTarget,pMsgData,iIndex);
        CloseMessageSession (*pMsgData);

        // first destroy view
		LPCTSTR lpszLabel;
        BOOL    bSelected;
        CWnd*   pWnd;
        void*   pExtra;

        if(m_pMsgFrame->m_tabWnd.GetTabInfo(iIndex, lpszLabel, bSelected, pWnd, pExtra))
        {   
            //OH: this is a hack to fix the bug with the destroying of the content
            //of the view. here just activate the tab before.
            if(bSelected)
            {
                m_pMsgFrame->m_tabWnd.ActivateTab(iIndex -1);
            }
            pWnd->ShowWindow(SW_HIDE);
			pWnd->SendMessage(WM_CLOSE);
		}
        // then remove tab
        m_pMsgFrame->m_tabWnd.RemoveTab(iIndex);
        
        // Do not use MsgDoc methods because they lookup Target
        // in Target set. But in CERemoveTargetNotify notification
        // the Target is already removed from the Target set.
        pTarget->UnregisterSessionCB (&m_SessionCB);
        pTarget->UnregisterConnectStateCB (&m_ConnectStateCB);

        CTargetOnl* pTarg = dynamic_cast<CTargetOnl*> (pTarget);
        if(pTarg)
        {
            pTarg->UnregisterExclusiveSessionCB (&m_ExclusiveSessionCB);
        }
        pDoc->m_arrMsgData.RemoveAt (iIndex);
    }
    catch (HRESULT)
    {
        return;
    }
}


//------------------------------------------------------------------*
/**
 * Function name	CMsgWnd::OnRenameTarget
 * Description			
 * @param			CTargetOnlBase *pTarget
 * @param			const CString &strOldTargetName
 * @return			void 
 * @exception			
 * @see			
*/
void CMsgWnd::OnRenameTarget(CTargetOnlBase *pTarget, const CString &strOldTargetName)
{
    CMsgData *pMsgData;
    int iIndex;
    CWnd * pDLWnd;
    LPCTSTR lpszLabel;
    BOOL    bSelected;
    void*   pExtra;

    try
    {
        ASSERT (pTarget != NULL);
        if (pTarget == NULL)
            throw E_POINTER;

        pMsgData = FindControlMsgData (pTarget,iIndex);
        if (pMsgData == NULL) {
            return;
        }
        //Clear the tab and than rename it
        if(m_pMsgFrame->m_tabWnd.GetTabInfo(iIndex, lpszLabel, bSelected, pDLWnd, pExtra))
        {
            ((CMsgView *)pDLWnd)->Clear(iIndex);
        }

        m_pMsgFrame->m_tabWnd.RenameTab(iIndex, pTarget->GetId());
    }
    catch (HRESULT)
    {
        return;
    }
}

//------------------------------------------------------------------*
/**
 * Function name	CMsgWnd::OnAddressChanged
 * Description		Closes the current session and reopens new session
 *                  with a new address. The function will be called by the state manager
 *                  of 4CE if the address of a Target object has changed.
 * @param			CTargetOnlBase *pTarget
 * @return			void 
 * @exception			
 * @see			
*/
void CMsgWnd::OnAddressChanged(CTargetOnlBase *pTarget)
{
    HRESULT hr;
    CMsgData *pMsgData;
    int iIndex;


    try
    {
        ASSERT (pTarget != NULL);
        if (pTarget == NULL)
            throw E_POINTER;

        pMsgData = FindControlMsgData (pTarget,iIndex);
        if (pMsgData == NULL) {
            return;
        }

        CloseMessageSession (*pMsgData);
        hr = OpenMessageSession (pTarget, *pMsgData);
        if (FAILED (hr))
            throw hr;

        pMsgData = FindControlMsgData (pTarget,iIndex);
        if (pMsgData == NULL) {
            return;
        }
        
        int ActiveTab;
        m_pMsgFrame->m_tabWnd.GetActiveTab(ActiveTab);

        if (iIndex == ActiveTab)
        {
            CWnd *pViewWnd;
            m_pMsgFrame->m_tabWnd.GetActiveTab(pViewWnd);
            CTargetMsgView* pView = dynamic_cast<CTargetMsgView *> (pViewWnd);
            ASSERT(pView);
            if(pView) {
                pView->OnTargetAddChangeNotify(pTarget->GetControlAddress());
            }
        }
    }
    catch (HRESULT)
    {
        return;
    }
    catch (CMemoryException *e)
    {
        e->Delete ();
        return;
    }
}


void CMsgWnd::OnSessionCB(CTargetOnlBase *pTarget, CSC_SESSION_STATE sessionState, HRESULT hrError)
{
    CMsgData *pMsgData = NULL;
    CTargetOnl* pTarg = NULL;
    int iIndex;
//    int iImage;

    if (FAILED (hrError))
    {
        TRACE("OnSessionCB: Error detected, hr = %x\n", hrError);

// TODO:  CString strMessage;
// 
//        if (GetErrorDescription (hrError, strMessage))
//        {
//            CString strSysMessage;
//            CString strTargetName;
//
//            strTargetName = pTarget->GetId ();
//
//            strSysMessage.Format (_T ("4CE: %s: %s"), strTargetName, strMessage);
//            AddMessage(strTargetName, E_FACILITY_CESYSTEM, strSysMessage);
//        }
    }

    pMsgData = FindControlMsgData(pTarget,iIndex);
    if (pMsgData == NULL) {
        return;
    }

    // SU: Moved connection of the message session to this callback received when an
    // normal variable session has been established. 
    // Within that callback OnSessionCB of the message window the message session
    // will be connected and disconnected. Since exclusive sessions always establish
    // a normal variable session prior to establishing an exclusive session the
    // connection of the message session will solely be done if the variable session
    // has been established.
    //
    // Do not try to connect the message session unless the variable session
    // has not succeeded in connecting. Otherwise the following scenario appears.
    //
    // 1) Both variable session and message session connect requests are queued
    //    to the CSC_Online worker thread.
    // 2) First the variable session connect request blocks in an RPC call and
    //    returns after some timeout (few minutes).
    // 3) CSC_Online continues with trying to connect the message session which
    //    also blocks in an RPC call.
    // 4) The user changes the Targets address and requests connect.
    // 5) CSC_Online is not able to handle the connect request while it is blocked
    //    in the bad Targets message session connect request (step 3)
    // 6) If the user was quick enough in renaming the Target and requesting connect
    //    the progress bar may appear a long time (until the blocked call returns).
    switch(sessionState) {
        case RW_established:
            ConnectMessageSession(pTarget,pMsgData,iIndex);
            break;
        case not_connected:
            DisconnectMessageSession(pTarget,pMsgData,iIndex);
            CloseMessageSession (*pMsgData);
            break;
    }

    int iCurrIndex = -1;
    m_pMsgFrame->m_tabWnd.GetActiveTab(iCurrIndex);

    if (iIndex == iCurrIndex)
    {
        CWnd *pViewWnd;
        CTargetMsgView* pView;

        m_pMsgFrame->m_tabWnd.GetActiveTab(pViewWnd);
        pView = dynamic_cast<CTargetMsgView *> (pViewWnd);
        ASSERT(pView);
        if(pView)
        {
           pView->OnSessionNotify(sessionState);
        }
    }
}


void CMsgWnd::OnExclusiveSessionCB (CTargetOnl *pTarget, CSC_SESSION_STATE sessionState, HRESULT hrError)
{
    CMsgData *pMsgData;
    int iIndex;
    int iImage;

    if (FAILED (hrError))
    {
        TRACE("OnExclusiveSessionCB: Error detected, hr = %x\n", hrError);
// TODO:
//        CString strMessage;
//        if (GetErrorDescription (hrError, strMessage))
//        {
//            CString strTargetName;
//            CString strSysMessage;
//
//            strTargetName = pTarget->GetId ();
//
//            strSysMessage.Format (_T ("4CE: %s: %s (HRESULT: 0x%08x)"), strTargetName, strMessage, hrError);
//            AddMessage (strTargetName, E_FACILITY_CESYSTEM, strSysMessage);
//        }
    }

    pMsgData = FindControlMsgData(pTarget,iIndex);
    if (pMsgData == NULL) {
        return;
    }

    GetConnectImage(pTarget->GetConnectState(), iImage);
    m_pMsgFrame->SetImage(iIndex, iImage);

    int iActiveTab;
    m_pMsgFrame->m_tabWnd.GetActiveTab(iActiveTab);

    if (iIndex == iActiveTab)
    {
        CWnd*           pViewWnd;
        CTargetMsgView* pView;

        m_pMsgFrame->m_tabWnd.GetActiveTab(pViewWnd);
        if(pViewWnd)
        {
            pView = dynamic_cast<CTargetMsgView*> (pViewWnd);
            ASSERT (pView);
            if (pView)
            {
                pView->OnExclusiveSessionNotify (sessionState);
            }
        }
    }
}


void CMsgWnd::OnConnectStateCB (CTargetOnlBase *pTarget)
{
    UpdateTargetConnectState(pTarget);
}


int CMsgWnd::ConnectMessageSession(CTargetOnlBase* pTarget,CMsgData *pMsgData,int iIndex)
{
    HRESULT hr = S_OK;
    BOOL bConn = FALSE;

    TRACE("Connect Target\n");
    ASSERT(pTarget);
    ASSERT(pMsgData);

    // If opening the message session failed during creation
    // of the message view retry it now. If still failing to
    // open the message session return with error.

    if (pMsgData->m_pIFCMessageSession == NULL)
    {
        (void)OpenMessageSession (pMsgData->m_pTarget, *pMsgData);

        if (pMsgData->m_pIFCMessageSession == NULL)
            return (-1);
    };

    //check first if it's already connected
    hr = pMsgData->m_pIFCMessageSession->get_IsConnected(&bConn);
    if(FAILED(hr)) {
        return (-1);
    }

    if(bConn) {//already connected
        return 0;
    }

    // Start messaging for control.
    ///////////////////////////////

    StartMessaging (NULL, iIndex);


    // Connect the message session.
    ///////////////////////////////
    CComBSTR sUserName = pTarget->GetUserName();
    CComBSTR sPassword = pTarget->GetPassword();

    hr = pMsgData->m_pIFCMessageSession->Connect(sUserName,sPassword);
    if (FAILED (hr))
        return (-1);

    return (0);
}

int CMsgWnd::DisconnectMessageSession(CTargetOnlBase* pTarget,CMsgData *pMsgData, int iIndex)
{
    HRESULT hr = S_OK;
    BOOL bConn = FALSE;
    
//    TRACE("Disconnect Target\n");
    ASSERT(pTarget);
    ASSERT(pMsgData);
    
    // Message session not open? Don't care because the monitoring
    // connection may be connected and disconnecting calls this method.
    ///////////////////////////////////////////////////////////////////

    if (pMsgData->m_pIFCMessageSession == NULL)
        return (0);

    //check first if it's already disconnected
    hr = pMsgData->m_pIFCMessageSession->get_IsConnected(&bConn);
    if(FAILED(hr)) {
        return (-1);
    }

    if(!bConn) {//already disconnected
        return 0;
    }


    // Stop messaging for control.
    //////////////////////////////

    StopMessaging (iIndex);


    // Disconnect the message session.
    //////////////////////////////////

    hr = pMsgData->m_pIFCMessageSession->Disconnect ();
    if (FAILED (hr))
    {
        TRACE("Disconnect failed\n");
        return (-1);
    }

    return (0);
}


void CMsgWnd::GetSessionImage(CSC_SESSION_STATE VarSessionState, CSC_SESSION_STATE ExclusiveSessionState, int & iImage)
{
    if (interrupted == ExclusiveSessionState)
    {
        iImage = IDI_COMM_INTERRUPTED;
        return;
    }

    if (interrupted == VarSessionState)
    {
        iImage = IDI_INTERRUPTED;
        return;
    }

    if (exclusive_established == ExclusiveSessionState)
    {
        if (RW_established == VarSessionState)
        {
            iImage = IDI_ALL_ESTABLISHED;
        }
        else
        {
            iImage = IDI_COMM_ESTABLISHED;
        }

        return;
    }

    if (RW_established == VarSessionState)
    {
        iImage = IDI_ESTABLISHED;
        return;
    }

    iImage = -1;
}


void CMsgWnd::GetConnectImage(E_TargetOnlConnectState eState, int& iImage)
{
    if(eState == eTOCS_not_connected)
    {
        iImage = -1;
    }
    else
    {
        iImage = -eTOCS_exception + (int)eState;
    }
}


void CMsgWnd::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
    m_pMsgFrame->OnUpdateCmdUI(pTarget, bDisableIfNoHndler);
	UpdateDialogControls(pTarget, bDisableIfNoHndler);
}




//------------------------------------------------------------------*
/**
 * Advise the message sink object with a created downloader component.
 * Retrieves first the downloader wrapper class CDownload from the 
 * target set. The CDownload object will be identified by the target
 * instance name (strTargetName). Then gets the downloader component
 * interface and advises the message sink object with that interface.
 * After OnAviseDownloader() has been called the message view is ready
 * to receive messages from the downloader.
 * @param		const CString & strTargetName - target instance
 *              name.
 * @see			OnUnadviseDownloader(), class CImplMsgSink
*/
void CMsgWnd::OnAdviseDownloader(const CString & strTargetName)
{
    CDownload* pDL = NULL;
    CComPtr<IUnknown> pUnk;
    DWORD dwCookie = 0UL;
    CString strName;
    
    CTargetSet* pTargetSet = CEGetTargetSet(_T("Project Targets"));
    if(!pTargetSet) {
        return;
    }

    pDL = pTargetSet->GetDownloader(strTargetName);
    if(pDL == NULL) {
        return;
    }
    
    pUnk.Attach(pDL->GetDownloadComponent());
    if(!pUnk) {
        pDL->Release();        
        return;
    }
    
    //use a different mapping cause component does not work here
    strName.Format(_T("Downloader%08ld"),(long)pDL);
    if(m_SourceNameCookieMap.Lookup(strName, dwCookie)) {
        pDL->Release();
        return;
    }
    
    m_pMsgSink->RegisterMsgSource(pUnk, &dwCookie);
    if(dwCookie == 0UL) {
        pDL->Release();
        return;
    }
    m_SourceNameCookieMap.SetAt(strName, dwCookie);
    pDL->Release();
}


//------------------------------------------------------------------*
/**
 * UnAdvises the message sink object with a downloader component.
 * Retrieves first the downloader wrapper class CDownload from the 
 * target set. The CDownload object will be identified by the target
 * instance name (strTargetName). Then gets the downloader component
 * interface and unadvises the message sink object with that interface.
 * After OnUnaviseDownloader() has been called the message view will
 * receive no longer downloader messages from that specific downloader
 * component.
 * @param		const CString & strTargetName - target instance
 *              name.
 * @see			OnAdviseDownloader(), class CImplMsgSink
*/
void CMsgWnd::OnUnadviseDownloader(const CString & strTargetName)
{
    CDownload* pDL = NULL;
    CComPtr<IUnknown> pUnk;
    DWORD dwCookie = 0UL;
    CString strName;

    CTargetSet* pTargetSet = CEGetTargetSet(_T("Project Targets"));
    if(!pTargetSet) {
        return;
    }

    pDL = pTargetSet->GetDownloader(strTargetName);
    if(pDL == NULL) {
        return;
    }

    pUnk.Attach(pDL->GetDownloadComponent());
    if(!pUnk) {
        pDL->Release();
        return;
    }
 
    //use a different mapping cause component does not work here
    strName.Format(_T("Downloader%08ld"),(long)pDL);
    if(!m_SourceNameCookieMap.Lookup(strName, dwCookie)) {
        //not found
        pDL->Release();
        return;
    }

    if(dwCookie == 0UL) {
        pDL->Release();
        return;   
    }

    m_pMsgSink->UnregisterMsgSource(pUnk,dwCookie);
    m_SourceNameCookieMap.RemoveKey(strName);
    pDL->Release();
}

CMsgData * CMsgWnd::FindControlMsgData(CTargetOnlBase *pTarget, int & iIndex)
{
    CMsgData* pData = NULL;
    CCEMAINApp *pApp = NULL;
    CMsgDoc *pDoc = NULL;
    int iX;

    iIndex = -1;

    ASSERT(pTarget); 
    if(!pTarget) {
        FC_DEBUGPRINT (_T ("CEMSG> Target invalid\n"));
        return NULL;
    }

    pApp = (CCEMAINApp *)AfxGetApp(); 
    ASSERT(pApp);
    if(!pApp) {
        FC_DEBUGPRINT (_T ("CEMSG> Main application object not found\n"));
        return NULL;
    }
    pDoc = pApp->GetMsgDoc();
    ASSERT(pDoc);
    if(!pDoc) {
        FC_DEBUGPRINT (_T ("CEMSG> Message document not found\n"));
        return NULL;
    }

    pData = pDoc->FindControlMsgData (pTarget, &iX);
    if (pData == NULL) {
        FC_DEBUGPRINT (_T ("CEMSG> Message data for target not found\n"));
        return NULL;
    }

    iIndex = iX;
    return pData;
}


CMsgData * CMsgWnd::FindControlMsgData(const CString &strTargetName, int &iIndex)
{
    CMsgData* pData = NULL;
    CCEMAINApp *pApp = NULL;
    CMsgDoc *pDoc = NULL;
    int iX;

    iIndex = -1;

    pApp = (CCEMAINApp *)AfxGetApp(); 
    ASSERT(pApp);
    if(!pApp) {
        FC_DEBUGPRINT (_T ("CEMSG> Main application object not found\n"));
        return NULL;
    }
    pDoc = pApp->GetMsgDoc();
    ASSERT(pDoc);
    if(!pDoc) {
        FC_DEBUGPRINT (_T ("CEMSG> Message document not found\n"));
        return NULL;
    }

    pData = pDoc->FindControlMsgData (strTargetName, &iX);
    if (pData == NULL) {
        FC_DEBUGPRINT (_T ("CEMSG> Message data for target not found\n"));
        return NULL;
    }

    iIndex = iX;
    return pData;
}



//------------------------------------------------------------------*
/**
 * Returns the message data associated to a specific message viewer
 * type respectively message viewer tab and the index in the array of
 * the message data.<br>
 * <B>Note:</B> The method shall not be called for a message viewer type
 * MSG_ControlMsgViewer. It's not for the target viewer tabs. Call
 * FindControlMsgData() for specific target instances.
 *
 * @param			MSG_VIEWER_TYPE viewer - type of message viewer.
 * @param			int & iIndex - index in the message data array of
 *                  the message data associated to the specific message
 *                  viewer type.
 * @return			CMsgData* - Pointer to the message data of that type.
 * @see             FindControlMsgData()
*/
CMsgData* CMsgWnd::GetMsgDataByViewer(MSG_VIEWER_TYPE viewer, int & iIndex)
{
    CMsgData* pData = NULL;
    CCEMAINApp *pApp = NULL;
    CMsgDoc *pDoc = NULL;
    int iSize;

    iIndex = -1;

    if(viewer == MSG_ControlMsgViewer) {
        ASSERT(viewer != MSG_ControlMsgViewer);
        return NULL;
    }
    
    pApp = (CCEMAINApp *)AfxGetApp();
    ASSERT(pApp);
    if(!pApp) {
        FC_DEBUGPRINT (_T ("CEMSG> Main application object not found\n"));
        return NULL;
    }

    pDoc = pApp->GetMsgDoc();
    ASSERT(pDoc);
    if(!pDoc) {
        FC_DEBUGPRINT (_T ("CEMSG> Message document not found\n"));
        return NULL;
    }
   
    iSize = pDoc->m_arrMsgData.GetSize();

    for (int i = 0; i < iSize; i++) 
    {
        CMsgData& MsgData = pDoc->m_arrMsgData[i];
        if(MsgData.m_MsgViewerType == viewer) 
        {
            iIndex = i;
            return &(pDoc->m_arrMsgData[i]);
        }
    }
    return NULL;
}


void CMsgWnd::UpdateTargetConnectState(CTargetOnlBase* pTargetOnl)
{
    CMsgData *pMsgData = NULL;
    int iIndex;
    int iImage;

    pMsgData = FindControlMsgData(pTargetOnl,iIndex);
    if (pMsgData == NULL)
    {
        return;
    }

    GetConnectImage(pTargetOnl->GetConnectState(), iImage);
    m_pMsgFrame->SetImage(iIndex, iImage);

    int iCurrIndex = -1;
    m_pMsgFrame->m_tabWnd.GetActiveTab(iCurrIndex);

    if (iIndex == iCurrIndex)
    {
        CWnd *pViewWnd;
        CTargetMsgView* pView;

        m_pMsgFrame->m_tabWnd.GetActiveTab(pViewWnd);
        pView = dynamic_cast<CTargetMsgView *> (pViewWnd);
        ASSERT(pView);
        if(pView)
        {
           pView->SetConnectState(pTargetOnl->GetConnectState());
           pView->SetStateText(pTargetOnl->GetDiagnosisText());
        }
    }
}


void CMsgWnd::ShowNextError()
{
    // Switch to compile window
    ShowCompileWindow();

    CWnd*   pCompileWnd;
    m_pMsgFrame->m_tabWnd.GetActiveTab(pCompileWnd);
    ((CMsgView *)pCompileWnd)->ShowNextError();
}

void CMsgWnd::ShowPreviousError()
{
    // Switch to compile window
    ShowCompileWindow();

    CWnd*   pCompileWnd;
    m_pMsgFrame->m_tabWnd.GetActiveTab(pCompileWnd);
    ((CMsgView *)pCompileWnd)->ShowPreviousError();
}
