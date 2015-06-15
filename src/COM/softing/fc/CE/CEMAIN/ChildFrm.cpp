
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*

// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "CEMAIN.h"
#include "MainFrm.h"
#include "utilif.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, SECMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, SECMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	if( !SECMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

void CChildFrame::OnUpdateFrameMenu(BOOL bActivate, CWnd* pActivateWnd,
	HMENU hMenuAlt)
{
    SECMDIChildWnd::OnUpdateFrameMenu(bActivate, pActivateWnd, hMenuAlt);

    // inform mainframe to update main resources
    CMainFrame* pMainFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
    if(pMainFrame)
    {
        pMainFrame->SendMessage(ID_UPDATE_MAIN_MENU);
    }
}


HACCEL CChildFrame::GetDefaultAccelerator()
{
	HACCEL hAccelTable = m_hAccelTable;
    CMainFrame* pMainFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
    if(pMainFrame)
    {
        hAccelTable = pMainFrame->m_hAccelTable;
    }
	return hAccelTable;
}


/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	SECMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	SECMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers



// CCEMAINFrame

IMPLEMENT_DYNCREATE(CCEMAINFrame, CChildFrame)

CCEMAINFrame::CCEMAINFrame()
{
}

CCEMAINFrame::~CCEMAINFrame()
{
}


BEGIN_MESSAGE_MAP(CCEMAINFrame, CChildFrame)
	//{{AFX_MSG_MAP(CCEMAINFrame)
		ON_WM_SYSCOMMAND()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCEMAINFrame message handlers

BOOL CCEMAINFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style &= ~FWS_ADDTOTITLE;
	return CChildFrame::PreCreateWindow(cs);
}


//------------------------------------------------------------------*
/**
 * load window placement.
 *
 *  if window placement could be found in settings, it will be set,
 *  i. e. the size and position of the project list view is restored
 *
 * @param           [in] pSettings: pointer to settings
 * @return          -
 * @exception       -
 * @see             -
*/
void CCEMAINFrame::LoadWindowPlacement(CSettings* pSettings)
{
    WINDOWPLACEMENT wndPlacement;

    ASSERT(pSettings);
    if(pSettings)
    {
        if(pSettings->m_projectListViewSettings.m_tWindowPlacement.GetWindowPlacement(wndPlacement))
        {
            SetWindowPlacement(&wndPlacement);
        }
    }
}


//------------------------------------------------------------------*
/**
 * save window placement to settings.
 *
 * @param           [in] pSettings: pointer to settings
 * @return          -
 * @exception       -
 * @see             LoadWindowPlacement()
*/
void CCEMAINFrame::SaveWindowPlacement(CSettings* pSettings)
{
    WINDOWPLACEMENT wndPlacement;
    GetWindowPlacement(&wndPlacement);

    ASSERT(pSettings);
    if(pSettings)
    {
        pSettings->m_projectListViewSettings.m_tWindowPlacement.SetWindowPlacement(wndPlacement);
    }
}

void CCEMAINFrame::OnSysCommand (UINT nID, LONG lParam)
{
	if ((nID & 0xfff0) == SC_CLOSE)
	{
		if (AfxMessageBox (IDS_REALLY_CLOSE_PROJECT, MB_OKCANCEL) == IDCANCEL)
		{
			return;
		};
	};

	CChildFrame::OnSysCommand (nID, lParam);
}

BOOL CCEMAINFrame::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
	{
        // get KAD accelerators from main frame
        CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
        HACCEL  hAccel = pMainFrame->GetAccelerators();
		// translate accelerators
        HWND    hWnd = pMainFrame->GetSafeHwnd();
        if (hAccel && hWnd && ::TranslateAccelerator(hWnd, hAccel, pMsg))
		{
			return TRUE;
		}
    }
	
	return CChildFrame::PreTranslateMessage(pMsg);
}

int CCEMAINFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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

	if (CChildFrame::OnCreate(lpCreateStruct) == -1)
		return -1;
	
    // read windowplacement from settings
//    LoadWindowPlacement();
	return 0;
}

void CCEMAINFrame::OnDestroy() 
{
    //SaveWindowPlacement();
	CChildFrame::OnDestroy();
}
