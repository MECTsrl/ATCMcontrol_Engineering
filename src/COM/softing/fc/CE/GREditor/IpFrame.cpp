

#include "stdafx.h"
#include "GrEditor.h"
#include "GrEditorDoc.h"
#include "GrEditorView.h"

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

void CInPlaceFrame::ViewToolbarFbd(bool bShow) 
{
   BOOL b = bShow ? TRUE : FALSE;

   ShowControlBar(&m_wndToolBarFBD, b, FALSE);
}

void CInPlaceFrame::ViewToolbarSfc(bool bShow) 
{
   BOOL b = bShow ? TRUE : FALSE;

   ShowControlBar(&m_wndToolBarSFC, b, FALSE);
}

void CInPlaceFrame::ViewToolbarLd(bool bShow) 
{
   BOOL b = bShow ? TRUE : FALSE;

   ShowControlBar(&m_wndToolBarLD, b, FALSE);
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

	// Set owner to this window, so messages are delivered to correct app
	m_wndToolBar.SetOwner(this);
	m_wndToolBarFBD.SetOwner(this);
	m_wndToolBarSFC.SetOwner(this);
	m_wndToolBarLD.SetOwner(this);

	// Create toolbar on client's frame window
	if (!m_wndToolBar.CreateEx(pWndDoc, TBSTYLE_TOOLTIPS | TBSTYLE_FLAT | TBSTYLE_TRANSPARENT) ||
		!m_wndToolBar.LoadToolBar(IDR_GR_TYPE_SRVR_IP))
	{
		TRACE0("Failed to create toolbar\n");
		return FALSE;
	}
	if (!m_wndToolBarFBD.CreateEx(pWndDoc, TBSTYLE_TOOLTIPS | TBSTYLE_FLAT | TBSTYLE_TRANSPARENT) ||
		!m_wndToolBarFBD.LoadToolBar(IDR_GR_TOOLBAR_FBD))
	{
		TRACE0("Failed to create fbd toolbar\n");
		return FALSE;      // fail to create
	}
	if (!m_wndToolBarSFC.CreateEx(pWndDoc, TBSTYLE_TOOLTIPS | TBSTYLE_FLAT | TBSTYLE_TRANSPARENT) ||
		!m_wndToolBarSFC.LoadToolBar(IDR_GR_TOOLBAR_SFC))
	{
		TRACE0("Failed to create sfc toolbar\n");
		return FALSE;      // fail to create
	}
	if (!m_wndToolBarLD.CreateEx(pWndDoc, TBSTYLE_TOOLTIPS | TBSTYLE_FLAT | TBSTYLE_TRANSPARENT) ||
		!m_wndToolBarLD.LoadToolBar(IDR_GR_TOOLBAR_LD))
	{
		TRACE0("Failed to create ld toolbar\n");
		return FALSE;      // fail to create
	}
/*
	// Set owner to this window, so messages are delivered to correct app
	m_wndDlgBar.SetOwner(this);
	// Create dialog bar on client's frame window
	if (!m_wndDlgBar.Create(pWndFrame, IDR_GR_TYPE_SRVR_IP, 
		CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR))
	{
		TRACE0("Failed to create dialogbar\n");
		return FALSE;
	}
*/
	// Set the owner of this window, so messages are delivered to correct app
	m_wndReBar.SetOwner(this);

	// Create a rebar on client's frame window
	if (!m_wndReBar.Create(pWndDoc, RBS_AUTOSIZE | RBS_BANDBORDERS) ||
		!m_wndReBar.AddBar(&m_wndToolBar)    ||
		!m_wndReBar.AddBar(&m_wndToolBarFBD) ||
		!m_wndReBar.AddBar(&m_wndToolBarSFC) || 
      !m_wndReBar.AddBar(&m_wndToolBarLD) )
//		!m_wndReBar.AddBar(&m_wndDlgBar))
	{
		TRACE0("Failed to create rebar\n");
		return FALSE;
	}

	// TODO: Remove this if you don't want tool tips
	m_wndToolBar.SetBarStyle   (m_wndToolBar.GetBarStyle()    | CBRS_FLYBY);
	m_wndToolBarFBD.SetBarStyle(m_wndToolBarFBD.GetBarStyle() |	CBRS_FLYBY);
	m_wndToolBarSFC.SetBarStyle(m_wndToolBarSFC.GetBarStyle() | CBRS_FLYBY);
	m_wndToolBarLD.SetBarStyle (m_wndToolBarLD.GetBarStyle()  | CBRS_FLYBY);
//	m_wndDlgBar.SetBarStyle(m_wndDlgBar.GetBarStyle() | CBRS_FLYBY);

   CGrEditorView * pView = dynamic_cast<CGrEditorView *> (GetActiveView ());
   if (pView != NULL)
   {
      CGrEditorDoc* pDoc = pView->GetDocument();
      ASSERT_VALID(pDoc);

      if (pDoc)
      {
         switch (pDoc->GetCurrLang())
         {
         case EL_FBD:      
            ViewToolbarFbd(true);
            ViewToolbarSfc(false);
            ViewToolbarLd (false);
            break;

         case EL_LD:       
            ViewToolbarFbd(true);
            ViewToolbarSfc(false);
            ViewToolbarLd (true);
            break;

         case EL_ALL:
            ViewToolbarFbd(true);
            ViewToolbarSfc(true);
            ViewToolbarLd (true);
            break;
         }
      }
   }
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
    CGrEditorView *pView;

    if (bSysMenu)
    {
        COleDocIPFrameWnd::OnInitMenuPopup (pPopupMenu, nIndex, bSysMenu);
        return;
    };


    // If first menu item is not "Set Watchpoint" we are not at the "Debug" menu.
    /////////////////////////////////////////////////////////////////////////////

    if (pPopupMenu->GetMenuItemID (0) != ID_GR_SET_WATCHPOINT)
    {
        COleDocIPFrameWnd::OnInitMenuPopup (pPopupMenu, nIndex, bSysMenu);
        return;
    };


    BOOL bFound = FALSE;
    for (uiIndex = 0; uiIndex <= pPopupMenu->GetMenuItemCount (); ++uiIndex)
    {
        if (pPopupMenu->GetMenuItemID (uiIndex) == ID_GR_PERFORM_CYCLE)
        {
            ++uiIndex;
            bFound = TRUE;
            break;
        };
    };

    if(bFound)
    {
        uiCount = pPopupMenu->GetMenuItemCount () - uiIndex;
        while (uiCount-- != 0)
        {
            pPopupMenu->DeleteMenu (uiIndex, MF_BYPOSITION);
        }
    }

    pView = dynamic_cast<CGrEditorView *> (GetActiveView ());
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

