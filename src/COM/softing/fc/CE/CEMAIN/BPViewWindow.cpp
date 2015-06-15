

#include "stdafx.h"
#include "BPViewWindow.h"


#include "stdafx.h"
#include "CEMAIN.h"
#include "CeSysdef.h"

//----  Local Defines:   -------------------------------------------*
#define BPVIEW_LEFT_MARGIN 7
#define BPVIEW_RIGHT_MARGIN 7
#define BPVIEW_TOP_MARGIN 7
#define BPVIEW_BOTTOM_MARGIN 7


IMPLEMENT_DYNCREATE(CBPViewWindow, CSizedControlBar )


BEGIN_MESSAGE_MAP(CBPViewWindow, CSizedControlBar )
    //{{AFX_MSG_MAP(CBPViewWindow)
    ON_WM_CREATE()
    ON_WM_SIZE()
	ON_WM_SETFOCUS()
    ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//------------------------------------------------------------------*
/**
 * Constructor
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             
*/
CBPViewWindow::CBPViewWindow () :   
    m_StateChangeCB(this, OnStateChange)
{
    m_bAttached = false;
    CCEStateManager *pStateManager = NULL;
    pStateManager = CEGetStateManager();
    ASSERT(pStateManager);
    if (pStateManager != NULL)
    {
        pStateManager->RegisterCallback (&m_StateChangeCB);
    }

}


//------------------------------------------------------------------*
/**
 * Destructor
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             
*/
CBPViewWindow::~CBPViewWindow ()
{

}



void CBPViewWindow::AttachToBreakpointManager ()
{
    CCEStateManager *pStateManager;
    CCEComponentManager *pComponentManager;
    IUnknown* pUnknown = NULL;

    if (m_bAttached)
    {
        return;
    };

    pStateManager = CEGetStateManager ();
    if (pStateManager == NULL)
    {
        return;
    };

    pComponentManager = CEGetComponentManager ();
    if (pComponentManager == NULL)
    {
        return;
    };

    pUnknown = CEGetCOM (CE_COMPMAN_BREAKPTMAN, false);
    if (pUnknown == NULL)
    {
        return;
    };

    m_ControlWrapper.AttachToBPMan (pUnknown);
    pUnknown->Release();

    m_bAttached = true;
}


void CBPViewWindow::DetachFromBreakpointManager ()
{
    if (!m_bAttached)
    {
        return;
    };

    m_ControlWrapper.DetachFromBPMan ();
    m_bAttached = false;
}


void CBPViewWindow::OnStateChange (CEStateNotify stateNotify, unsigned int uiStage)
{
    CCEStateManager *pStateManager = NULL;

    if (stateNotify == CEPostOpenProjectNotify)
    {
        AttachToBreakpointManager ();
    }
    else if (stateNotify == CEPreCloseProjectNotify)
    {
        pStateManager = CEGetStateManager();
        ASSERT(pStateManager);
        if (pStateManager != NULL)
        {
            pStateManager->UnregisterCallback(&m_StateChangeCB);
        }
        
        DetachFromBreakpointManager ();
    }
    else if (stateNotify == CEComponentAccessibleNotify)
    {
        if (CESMGetComponentName () == _T ("CE_COMPMAN_BREAKPTMAN"))
        {
            AttachToBreakpointManager ();
        };
    }
    else if (stateNotify == CEHandsOffComponentNotify)
    {
        if (CESMGetComponentName () == _T ("CE_COMPMAN_BREAKPTMAN"))
        {
            DetachFromBreakpointManager ();
        };
    };
}






//------------------------------------------------------------------*
/**
 * Returns the Active X Control wrapper.
 *
 * @param           -
 * @return          The Active X Control wrapper
 * @exception       -
 * @see             
*/
CBPViewWrapper* CBPViewWindow::GetWrapper()
{
    return (&m_ControlWrapper);
}


//------------------------------------------------------------------*
/**
 * WM_CREATE handler. Creates the CEWatchView Active X Control wrapper
 * window as child window.
 *
 * @param           lpCreateStruct: creation information
 * @return          0 on success, -1 on failure
 * @exception       -
 * @see             
*/
int CBPViewWindow::OnCreate (LPCREATESTRUCT lpCreateStruct)
{
    CRect geometry;

    if (SECControlBar::OnCreate (lpCreateStruct) == -1)
        return (-1);

    DWORD style = GetExBarStyle();
    style |= CBRS_EX_STDCONTEXTMENU;
    style |= CBRS_EX_ALLOW_MDI_FLOAT;
    SetExBarStyle(style);


    // Create Active X Control wrapper. Initial size is our own
    // initial size. Automatically creates the Active X Control.
    ////////////////////////////////////////////////////////////

    geometry.left = BPVIEW_LEFT_MARGIN;
    geometry.top = BPVIEW_TOP_MARGIN;
    geometry.right = lpCreateStruct->cx - BPVIEW_RIGHT_MARGIN;
    geometry.bottom = lpCreateStruct->cy - BPVIEW_BOTTOM_MARGIN;

    if (geometry.right<geometry.left)
    {
        geometry.right = geometry.left;
    }
    if (geometry.bottom<geometry.top)
    {
        geometry.bottom = geometry.top;
    }

    if (!m_ControlWrapper.Create (NULL, WS_CHILD | WS_VISIBLE, geometry, this, 0))
        return (-1);


    return (0);
}


//------------------------------------------------------------------*
/**
 * WM_SIZE handler. Resizes CEWatchView Active X Control wrapper window
 * to fit into our client area.
 *
 * @param           nType: sizing reason
 *                  cx   : new width
 *                  cy   : new height
 * @return          
 * @exception       -
 * @see             
*/
void CBPViewWindow::OnSize (UINT nType, int cx, int cy)
{
    CRect geometry;

    SECControlBar::OnSize (nType, cx, cy);

    // Resize Active X Control wrapper to fit into our client area.
    // CWnd's MoveWindow method honours MFC's Active X Control wrapper
    // implementation. See file header for why this is important.
    //////////////////////////////////////////////////////////////////
    geometry.left = BPVIEW_LEFT_MARGIN;
    geometry.top = BPVIEW_TOP_MARGIN;
    geometry.right = cx - BPVIEW_RIGHT_MARGIN;
    geometry.bottom = cy - BPVIEW_BOTTOM_MARGIN;

    if (geometry.right<geometry.left)
    {
        geometry.right = geometry.left;
    }
    if (geometry.bottom<geometry.top)
    {
        geometry.bottom = geometry.top;
    }

    if (m_ControlWrapper.m_hWnd != NULL)
        m_ControlWrapper.MoveWindow(geometry); //(0, 0, cx, cy);
}


//------------------------------------------------------------------*
/**
 * WM_SETFOCUS handler. Propagates focus to Active X Control wrapper.
 * Beneath setting the focus this makes the wrapper UI activate the
 * Active X Control. But be warned: it does set the focus only if the
 * Active X Control is not yet UI active.
 *
 * @param           pOldWnd: Previous focus window
 * @return          -
 * @exception       -
 * @see             
*/
void CBPViewWindow::OnSetFocus (CWnd *pOldWnd)
{
    SECControlBar::OnSetFocus (pOldWnd);

    if (m_ControlWrapper.m_hWnd != NULL)
        m_ControlWrapper.SetFocus ();
}




//------------------------------------------------------------------*
/**
 * WM_SHOWWINDOW handler. Propagates hiding of parent window to Active
 * X Control wrapper which makes the wrapper UI deactivate the Active X
 * Control. If we do not UI deactivate the control here we are not able
 * to propagate the focus to the Active X Control because the wrapper
 * then believes the Active X Control is still UI active and does not
 * propagate focus. However we must not propagate showing of parent win-
 * dow since this performs UI activation without setting the focus (using
 * verb OLEIVERB_SHOW). What we need is UI activation with setting the
 * focus (using OLEIVERB_UIACTIVATE) which is done by focus propagation.
 * MFC's control container implementation is pure horror!!!
 *
 * @param           bShow  : TRUE on show, FALSE on hide
 *                  nStatus: What caused showing/hiding
 * @return          
 * @exception       -
 * @see             
*/
/*void CBPViewWindow::OnShowWindow (BOOL bShow, UINT nStatus)
{
    CWnd::OnShowWindow (bShow, nStatus);

    if (!bShow && m_ControlWrapper.m_hWnd != NULL)
        m_ControlWrapper.ShowWindow (SW_HIDE);
}*/



//------------------------------------------------------------------*
/**
 * WM_ERASEBKGND handler. Draws background like button background.
 *
 * @param           pDC: DC to draw background into
 * @return          TRUE to tell we actually erased the background.
 * @exception       -
 * @see             
*/
BOOL CBPViewWindow::OnEraseBkgnd (CDC *pDC)
{
    CBrush brush;
    CRect geometry;


    // Create the button face system brush, get our client area
    // geometry and fill it with this brush.
    ///////////////////////////////////////////////////////////

    if (!brush.CreateSysColorBrush (COLOR_BTNFACE))
        return (SECControlBar::OnEraseBkgnd (pDC));

    GetClientRect (&geometry);

    pDC->FillRect (&geometry, &brush);

    return (TRUE);
}




void CBPViewWindow::OnContextMenu(CWnd* pWnd, CPoint point) 
{
    SECControlBar::OnContextMenu (this, point);	
}


