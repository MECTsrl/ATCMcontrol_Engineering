

#include "stdafx.h"
#include "FcHtmlEd.h"

#include "IpFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInPlaceFrame

IMPLEMENT_DYNCREATE(CInPlaceFrame, COleDocIPFrameWnd)

BEGIN_MESSAGE_MAP(CInPlaceFrame, COleDocIPFrameWnd)
	//{{AFX_MSG_MAP(CInPlaceFrame)
	ON_WM_CREATE()
	ON_COMMAND_EX(ID_VIEW_FORMATBAR, OnToggleFcToolBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FORMATBAR, OnUpdateFcToolBarMenu)
	//}}AFX_MSG_MAP
	// Global help commands
	ON_COMMAND(ID_HELP_FINDER, COleDocIPFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_HELP, COleDocIPFrameWnd::OnHelp)
	ON_COMMAND(ID_DEFAULT_HELP, COleDocIPFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_CONTEXT_HELP, COleDocIPFrameWnd::OnContextHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInPlaceFrame construction/destruction

CInPlaceFrame::CInPlaceFrame()
{
}

CInPlaceFrame::~CInPlaceFrame()
{
}

int CInPlaceFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COleDocIPFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// CResizeBar implements in-place resizing.
	if (!m_wndResizeBar.Create(this))
	{
		TRACE0("Failed to create resize bar\n");
		return -1;      // fail to create
	}

	// By default, it is a good idea to register a drop-target that does
	//  nothing with your frame window.  This prevents drops from
	//  "falling through" to a container that supports drag-drop.
	m_dropTarget.Register(this);

	return 0;
}

// OnCreateControlBars is called by the framework to create control bars on the
//  container application's windows.  pWndFrame is the top level frame window of
//  the container and is always non-NULL.  pWndDoc is the doc level frame window
//  and will be NULL when the container is an SDI application.  A server
//  application can place MFC control bars on either window.
BOOL CInPlaceFrame::OnCreateControlBars(CFrameWnd* pWndFrame, CFrameWnd* pWndDoc)
{
	// Remove this if you use pWndDoc
//	UNREFERENCED_PARAMETER(pWndDoc);
	if (pWndDoc)
		pWndFrame = pWndDoc;
	
	// Set owner to this window, so messages are delivered to correct app
	m_wndToolBar.SetOwner(this);

	// Create toolbar on client's frame window
	if (!m_wndToolBar.CreateEx(pWndFrame) ||
		!m_wndToolBar.LoadToolBar(IDR_SRVR_IP))
	{
		TRACE0("Failed to create toolbar\n");
		return FALSE;
	}

	// Set owner to this window, so messages are delivered to correct app
	m_wndFontBar.SetOwner(this);

	// Create dialog bar on client's frame window
	if (!m_wndFontBar.Create(pWndFrame, IDD_FONT_BAR, 
		CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR))
	{
		TRACE0("Failed to create dialogbar\n");
		return FALSE;
	}

	// Set owner to this window, so messages are delivered to correct app
	m_wndFormatBar.SetOwner(this);

	// Create toolbar on client's frame window
	if (!m_wndFormatBar.CreateEx(pWndFrame) ||
		!m_wndFormatBar.LoadToolBar(IDR_FORMAT))
	{
		TRACE0("Failed to create toolbar\n");
		return FALSE;
	}

	// Set the owner of this window, so messages are delivered to correct app
	m_wndReBar.SetOwner(this);

	// Create a rebar on client's frame window
	if (!m_wndReBar.Create(pWndFrame) ||
		!m_wndReBar.AddBar(&m_wndToolBar) ||
		!m_wndReBar.AddBar(&m_wndFormatBar) ||
		!m_wndReBar.AddBar(&m_wndFontBar))
	{
		TRACE0("Failed to create rebar\n");
		return FALSE;
	}

	// TODO: Remove this if you don't want tool tips
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndFormatBar.SetBarStyle(m_wndFormatBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndFontBar.SetBarStyle(m_wndFontBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndFontBar.Init();
	m_wndFontBar.SetDlgCtrlID(IDD_FONT_BAR);

	return TRUE;
}

BOOL CInPlaceFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return COleDocIPFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CInPlaceFrame diagnostics

#ifdef _DEBUG
void CInPlaceFrame::AssertValid() const
{
	COleDocIPFrameWnd::AssertValid();
}

void CInPlaceFrame::Dump(CDumpContext& dc) const
{
	COleDocIPFrameWnd::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CInPlaceFrame commands

BOOL CInPlaceFrame::OnToggleFcToolBar(UINT nID) 
{
	CControlBar* pBar = GetTopLevelFrame()->GetControlBar(IDR_FORMAT);
	if (pBar != NULL)
	{
		ShowControlBar(pBar, (pBar->GetStyle() & WS_VISIBLE) == 0, FALSE);
		return TRUE;
	}
	return FALSE;
}

void CInPlaceFrame::OnUpdateFcToolBarMenu(CCmdUI* pCmdUI) 
{
	CControlBar* pBar = GetTopLevelFrame()->GetControlBar(IDR_FORMAT);
	if (pBar != NULL)
	{
		pCmdUI->SetCheck((pBar->GetStyle() & WS_VISIBLE) != 0);
		return;
	}
	pCmdUI->ContinueRouting();
}

