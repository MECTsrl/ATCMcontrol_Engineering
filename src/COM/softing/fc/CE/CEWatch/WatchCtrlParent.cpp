#include "stdafx.h"
#include "WatchCtrlParent.h"


//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



IMPLEMENT_DYNCREATE(CWatchControlParent, CWnd)


BEGIN_MESSAGE_MAP(CWatchControlParent, CWnd)
    //{{AFX_MSG_MAP(CWatchControlParent)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_SHOWWINDOW()
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
CWatchControlParent::CWatchControlParent ()
{
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
CWatchControlParent::~CWatchControlParent ()
{
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
CWatchControlWrapper * CWatchControlParent::GetWrapper ()
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
int CWatchControlParent::OnCreate (LPCREATESTRUCT lpCreateStruct)
{
    CRect geometry;

    if (CWnd::OnCreate (lpCreateStruct) == -1)
        return (-1);


    // Create Active X Control wrapper. Initial size is our own
    // initial size. Automatically creates the Active X Control.
    ////////////////////////////////////////////////////////////

    geometry.left = 0;
    geometry.top = 0;
    geometry.right = lpCreateStruct->cx;
    geometry.bottom = lpCreateStruct->cy;

    if (!m_ControlWrapper.Create (_T ("Watch Active X Control Wrapper"), WS_CHILD | WS_VISIBLE, geometry, this, 0))
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
void CWatchControlParent::OnSize (UINT nType, int cx, int cy)
{
    CWnd::OnSize (nType, cx, cy);


    // Resize Active X Control wrapper to fit into our client area.
    // CWnd's MoveWindow method honours MFC's Active X Control wrapper
    // implementation. See file header for why this is important.
    //////////////////////////////////////////////////////////////////

    if (m_ControlWrapper.m_hWnd != NULL)
        m_ControlWrapper.MoveWindow (0, 0, cx, cy);
}


//------------------------------------------------------------------*
/**
 * WM_PAINT handler. Redraws child Active X Control if we are redrawn.
 * Stingray's SECDockBar calls RedrawWindow to invalidate and redraw
 * control bars. However they do not pass the RDW_ALLCHILDREN flag and
 * as a consequence all children with the WS_CLIPCHILDREN style set are
 * not invalidated and redrawn. Unfortunately our Active X Control does
 * not have the WS_CLIPCHILDREN flag set. The only solution seems to be
 * to invalidate and redraw the Active X Control every time we are re-
 * drawn.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             
*/
void CWatchControlParent::OnPaint ()
{
    CWnd::OnPaint ();

    ::RedrawWindow (m_ControlWrapper, NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);
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
void CWatchControlParent::OnSetFocus (CWnd *pOldWnd)
{
    CWnd::OnSetFocus (pOldWnd);

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
void CWatchControlParent::OnShowWindow (BOOL bShow, UINT nStatus)
{
    CWnd::OnShowWindow (bShow, nStatus);

    if (!bShow && m_ControlWrapper.m_hWnd != NULL)
        m_ControlWrapper.ShowWindow (SW_HIDE);
}


//------------------------------------------------------------------*
/**
 * Window is new'd by tab window using CRuntimeClass's CreateObject
 * method. Easiest way to get rid of ourselves is to self-delete after
 * WM_NCDESTROY, i.e. here.
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CWatchControlParent::PostNcDestroy ()
{
    delete this;
}

BOOL CWatchControlParent::PreTranslateMessage(MSG* pMsg)
{
    
	if(PreTranslateInput(pMsg))
    {
        return TRUE;
    }
    return CWnd::PreTranslateMessage(pMsg);
}
