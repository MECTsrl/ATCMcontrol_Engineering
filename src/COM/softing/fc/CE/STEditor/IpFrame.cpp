
// IpFrame.cpp : implementation of the CInPlaceFrame class
//

#include "stdafx.h"
#include "STEditor.h"
#include "STEditorIF.h"

#include "IpFrame.h"
#include "STEditorView.h"

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
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_HELP_FINDER, COleDocIPFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_HELP, COleDocIPFrameWnd::OnHelp)
	ON_COMMAND(ID_DEFAULT_HELP, COleDocIPFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_CONTEXT_HELP, COleDocIPFrameWnd::OnContextHelp)
	ON_WM_INITMENUPOPUP()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInPlaceFrame construction/destruction

CInPlaceFrame::CInPlaceFrame()
{
}

CInPlaceFrame::~CInPlaceFrame()
{
}


LPOLEINPLACEUIWINDOW CInPlaceFrame::GetOleDocFrame ()
{
	return (m_lpDocFrame);
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
	if (pWndDoc == NULL)
	{
		pWndDoc = pWndFrame;
	};

#if _MFC_VER == 0x0421
#else
	// Set owner to this window, so messages are delivered to correct app
	m_wndReBar.SetOwner (this);

	// Create rebar on client's frame window
	if (!m_wndReBar.Create (pWndDoc, RBS_AUTOSIZE | RBS_BANDBORDERS))
	{
		TRACE0 ("Failed to create rebar\n");
		return (FALSE);
	};
#endif
	// Set owner to this window, so messages are delivered to correct app
	m_wndToolBar.SetOwner(this);

	// Create toolbar on client's frame window
#if _MFC_VER == 0x0421
	if (!m_wndToolBar.Create(pWndDoc) ||
		!m_wndToolBar.LoadToolBar (IDR_STTYPE_SRVR_IP))
#else
	if (!m_wndToolBar.CreateEx (pWndDoc, TBSTYLE_TOOLTIPS | TBSTYLE_FLAT | TBSTYLE_TRANSPARENT) ||
		!m_wndToolBar.LoadToolBar (IDR_STTYPE_SRVR_IP))
#endif
	{
		TRACE0 ("Failed to create toolbar\n");
		return (FALSE);
	};

#if _MFC_VER == 0x0421
#else
	m_wndToolBar.GetToolBarCtrl ().SetExtendedStyle (TBSTYLE_EX_DRAWDDARROWS);
#endif
	m_wndToolBar.SetBarStyle (m_wndToolBar.GetBarStyle () | CBRS_FLYBY);

#if _MFC_VER == 0x0421
#else
	m_wndReBar.AddBar(&m_wndToolBar);
#endif
	return TRUE;
}

BOOL CInPlaceFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return COleDocIPFrameWnd::PreCreateWindow(cs);
}


void CInPlaceFrame::SetStatusText (const CString &strText)
{
	USES_CONVERSION;

	m_lpFrame->SetStatusText (T2OLE (strText));
}

void CInPlaceFrame::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
    UINT uiIndex;
    UINT uiCount;
    CSTEditorView *pView;

    if (bSysMenu)
    {
        COleDocIPFrameWnd::OnInitMenuPopup (pPopupMenu, nIndex, bSysMenu);
        return;
    };


    // If first menu item is not "Set Watchpoint" we are not at the "Debug" menu.
    /////////////////////////////////////////////////////////////////////////////

    if (pPopupMenu->GetMenuItemID (0) != IDM_SET_WATCHPOINT)
    {
        COleDocIPFrameWnd::OnInitMenuPopup (pPopupMenu, nIndex, bSysMenu);
        return;
    };


    for (uiIndex = 0; uiIndex <= pPopupMenu->GetMenuItemCount (); ++uiIndex)
    {
        if (pPopupMenu->GetMenuItemID (uiIndex) == IDM_PERFORM_CYCLE)
        {
            ++uiIndex;
            break;
        };
    };

    uiCount = pPopupMenu->GetMenuItemCount () - uiIndex;
    while (uiCount-- != 0)
    {
        pPopupMenu->DeleteMenu (uiIndex, MF_BYPOSITION);
    };

    pView = dynamic_cast<CSTEditorView *> (GetActiveView ());
    if (pView == NULL)
    {
        COleDocIPFrameWnd::OnInitMenuPopup (pPopupMenu, nIndex, bSysMenu);
        return;
    };

    pView->ExtendMenuBySFCItems (pPopupMenu);

    COleDocIPFrameWnd::OnInitMenuPopup (pPopupMenu, nIndex, bSysMenu);
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
