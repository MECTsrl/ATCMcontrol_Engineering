
#include "stdafx.h"
#include "WatchTabCtrl.h"
#include "WatchRC.h"


//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



IMPLEMENT_DYNAMIC (CWatchTabControl, SECTabControl)


BEGIN_MESSAGE_MAP(CWatchTabControl, SECTabControl)
    //{{AFX_MSG_MAP(CWatchTabControl)
    ON_WM_CONTEXTMENU()
	ON_WM_LBUTTONDBLCLK()
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
CWatchTabControl::CWatchTabControl ()
: m_pContextMenuOwner (NULL)
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
CWatchTabControl::~CWatchTabControl ()
{
}


//------------------------------------------------------------------*
/**
 * Override for initialization of tab control. Set a more readable
 * font. Height of tab control is considered to create a suitable
 * font.
 *
 * @param           dwStyle: tab style
 * @return          -
 * @exception       -
 * @see             SECTabControl::Initialize
*/
void CWatchTabControl::Initialize (DWORD dwStyle)
{
    CFont font;
    LOGFONT logFont;
    CFont adaptedFont;

    SECTabControl::Initialize (dwStyle);


    // Create a font object for the default GUI font. Get the fonts
    // properties. Modify font height to fit into tab control height.
    // Create a new font object according to modified font properties.
    // Tell base class about adapted font.
    //////////////////////////////////////////////////////////////////

    if (!font.CreateStockObject (DEFAULT_GUI_FONT))
        return;

    if (font.GetLogFont (&logFont) == 0)
        return;

    logFont.lfHeight = m_nHeight - 2;

    if (!adaptedFont.CreateFontIndirect (&logFont))
        return;

    SetFontTab (&adaptedFont, SECTAB_ACTIVE, FALSE);
    SetFontTab (&adaptedFont, SECTAB_SELECTED, FALSE);
    SetFontTab (&adaptedFont, SECTAB_UNSELECTED, FALSE);
}


//------------------------------------------------------------------*
/**
 * Override for removing tabs. Bugfix.
 *
 * @param           iIndex: Index of tab to be removed
 * @return          -
 * @exception       -
 * @see             SECTabControl::DeleteTab
*/
void CWatchTabControl::DeleteTab (int iIndex)
{
    int i;


    // Bug in stingray implementation. Tabs to the right of the one to
    // be removed are invalidated after removing from the internal data
    // structure. Should do it before. Otherwise a region as wide as the
    // removed tab that needs to be invalidated remains valid.
    ////////////////////////////////////////////////////////////////////

    for (i = iIndex; i < GetTabCount (); ++i)
        InvalidateTab (i);


    SECTabControl::DeleteTab (iIndex);
}


//------------------------------------------------------------------*
/**
 * Set owner of context menu that is responsible for popping up a context
 * menu on right mouse click inside the tab control. The owner is sent
 * a WM_WATCHTAB_CONTEXTMENU message on right mouse button click and then
 * may popup its context menu.
 *
 * @param           pOwner: window receiving context menu popup notifications
 * @return          -
 * @exception       -
 * @see             CWatchTabWindow::SetContextMenuOwner
*/
void CWatchTabControl::SetContextMenuOwner (CWnd *pOwner)
{
    m_pContextMenuOwner = pOwner;
}


//------------------------------------------------------------------*
/**
 * WM_CONTEXTMENU handler. Determines which tab was hit (if any) and
 * sends a notification to our context menu owner.
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CWatchTabControl::OnContextMenu (CWnd *pWnd, CPoint point)
{
    WatchTabHitInfo hitInfo;


    // Anybody to receive context menu popup notifications?
    // Nothing to do if none.
    //////////////////////////////////////////////////////

    if (m_pContextMenuOwner == NULL)
        return;


    // Put together hit info.
    /////////////////////////

    hitInfo.m_Point = point;

    ScreenToClient (&point);

    for (hitInfo.m_iTabIndex = 0; hitInfo.m_iTabIndex < GetTabCount (); ++hitInfo.m_iTabIndex)
        if (TabHit (hitInfo.m_iTabIndex, point))
            break;
    if (hitInfo.m_iTabIndex == GetTabCount ())
        hitInfo.m_iTabIndex = -1;


    // Send hit info to our context menu owner.
    ///////////////////////////////////////////

    m_pContextMenuOwner->SendMessage (WM_WATCHTAB_CONTEXTMENU, 0, (LPARAM)&hitInfo);
}

void CWatchTabControl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
    WatchTabHitInfo hitInfo;

    // create hit info
    hitInfo.m_Point = point;

    for (hitInfo.m_iTabIndex = 0; hitInfo.m_iTabIndex < GetTabCount (); ++hitInfo.m_iTabIndex)
    {
        if (TabHit (hitInfo.m_iTabIndex, point))
        {
            break;
        }
    }
    if (hitInfo.m_iTabIndex == GetTabCount ())
    {
        hitInfo.m_iTabIndex = -1;
    }

    // send message to context menu owner
    if(m_pContextMenuOwner)
    {
        m_pContextMenuOwner->SendMessage (WM_WATCHTAB_DBLCLCK, 0, (LPARAM)&hitInfo);
    }

	SECTabControl::OnLButtonDblClk(nFlags, point);
}
