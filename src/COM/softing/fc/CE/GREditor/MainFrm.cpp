

/*doc*=======================================================================*/
/*inc*=======================================================================*/

#include "stdafx.h"
#include "GrEditor.h"

#include "MainFrm.h"

/*const*=====================================================================*/
/*macro*=====================================================================*/
/*glovar*====================================================================*/
/*type*======================================================================*/
/*locvar*====================================================================*/

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*proto*=====================================================================*/
/*func*======================================================================*/

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	ON_WM_CREATE()
	ON_COMMAND(ID_GR_VIEW_TOOLBAR_FBD, OnViewToolbarFbd)
	ON_COMMAND(ID_GR_VIEW_TOOLBAR_SFC, OnViewToolbarSfc)
	ON_COMMAND(ID_GR_VIEW_TOOLBAR_LD,  OnViewToolbarLd)
	ON_UPDATE_COMMAND_UI(ID_GR_VIEW_TOOLBAR_SFC, OnUpdateViewToolbarSfc)
	ON_UPDATE_COMMAND_UI(ID_GR_VIEW_TOOLBAR_FBD, OnUpdateViewToolbarFbd)
	ON_UPDATE_COMMAND_UI(ID_GR_VIEW_TOOLBAR_LD, OnUpdateViewToolbarLd)
	//}}AFX_MSG_MAP
	// Global help commands
	ON_COMMAND(ID_HELP_FINDER, CMDIFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_HELP, CMDIFrameWnd::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CMDIFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CMDIFrameWnd::OnHelpFinder)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_TOOLTIPS | TBSTYLE_FLAT | TBSTYLE_TRANSPARENT) ||
		!m_wndToolBar.LoadToolBar(IDR_GR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	if (!m_wndToolBarFBD.CreateEx(this, TBSTYLE_TOOLTIPS | TBSTYLE_FLAT | TBSTYLE_TRANSPARENT) ||
		!m_wndToolBarFBD.LoadToolBar(IDR_GR_TOOLBAR_FBD))
	{
		TRACE0("Failed to create fbd toolbar\n");
		return -1;      // fail to create
	}
	if (!m_wndToolBarSFC.CreateEx(this, TBSTYLE_TOOLTIPS | TBSTYLE_FLAT | TBSTYLE_TRANSPARENT) ||
		!m_wndToolBarSFC.LoadToolBar(IDR_GR_TOOLBAR_SFC))
	{
		TRACE0("Failed to create sfc toolbar\n");
		return -1;      // fail to create
	}
	if (!m_wndToolBarLD.CreateEx(this, TBSTYLE_TOOLTIPS | TBSTYLE_FLAT | TBSTYLE_TRANSPARENT) ||
		!m_wndToolBarLD.LoadToolBar(IDR_GR_TOOLBAR_LD))
	{
		TRACE0("Failed to create ld toolbar\n");
		return -1;      // fail to create
	}
/*
	if (!m_wndDlgBar.Create(this, IDR_GR_MAINFRAME, 
		CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR))
	{
		TRACE0("Failed to create dialogbar\n");
		return -1;		// fail to create
	}
*/
	if (!m_wndReBar.Create(this) ||
		!m_wndReBar.AddBar(&m_wndToolBar) ||
		!m_wndReBar.AddBar(&m_wndToolBarFBD) ||
		!m_wndReBar.AddBar(&m_wndToolBarSFC) ||
		!m_wndReBar.AddBar(&m_wndToolBarLD) )
//		!m_wndReBar.AddBar(&m_wndDlgBar))
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Remove this if you don't want tool tips
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() | CBRS_FLYBY);
	m_wndToolBarFBD.SetBarStyle(m_wndToolBarFBD.GetBarStyle() | CBRS_FLYBY);
	m_wndToolBarSFC.SetBarStyle(m_wndToolBarSFC.GetBarStyle() | CBRS_FLYBY);
	m_wndToolBarLD.SetBarStyle(m_wndToolBarLD.GetBarStyle() | CBRS_FLYBY);
//	m_wndDlgBar.SetBarStyle(m_wndDlgBar.GetBarStyle() | CBRS_FLYBY);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	return CMDIFrameWnd::OnCreateClient(lpcs, pContext);
}

void CMainFrame::OnViewToolbarFbd() 
{
   BOOL b = ((m_wndToolBarFBD.GetStyle() & WS_VISIBLE) != 0);

   ShowControlBar(&m_wndToolBarFBD, !b, FALSE);
}

void CMainFrame::OnViewToolbarSfc() 
{
   BOOL b = ((m_wndToolBarSFC.GetStyle() & WS_VISIBLE) != 0);

   ShowControlBar(&m_wndToolBarSFC, !b, FALSE);
}

void CMainFrame::OnViewToolbarLd() 
{
   BOOL b = ((m_wndToolBarLD.GetStyle() & WS_VISIBLE) != 0);

   ShowControlBar(&m_wndToolBarLD, !b, FALSE);
}

void CMainFrame::OnUpdateViewToolbarFbd(CCmdUI* pCmdUI) 
{
   pCmdUI->SetCheck((m_wndToolBarFBD.GetStyle() & WS_VISIBLE) != 0);	
}

void CMainFrame::OnUpdateViewToolbarSfc(CCmdUI* pCmdUI) 
{
   pCmdUI->SetCheck((m_wndToolBarSFC.GetStyle() & WS_VISIBLE) != 0);	
}

void CMainFrame::OnUpdateViewToolbarLd(CCmdUI* pCmdUI) 
{
   pCmdUI->SetCheck((m_wndToolBarLD.GetStyle() & WS_VISIBLE) != 0);	
}
