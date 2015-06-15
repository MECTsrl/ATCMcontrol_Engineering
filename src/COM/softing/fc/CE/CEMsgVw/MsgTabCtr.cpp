
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*



// MsgTabCtr.cpp: implementation of the CMsgTabCtr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MsgTabCtr.h"
#include "MsgTab.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// Constants
const int nIconWidth = 16;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC (CMsgTabCtr, SECTabControl)


BEGIN_MESSAGE_MAP(CMsgTabCtr, SECTabControl)
    //{{AFX_MSG_MAP(CMsgTabCtr)
    ON_WM_CONTEXTMENU()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


//------------------------------------------------------------------*
/**
 * Function name			CMsgTabCtr::CMsgTabCtr
 * Description			
 * @return			
 * @exception			
 * @see			
*/
CMsgTabCtr::CMsgTabCtr()
: m_pContextMenuOwner (NULL)
{

}


//------------------------------------------------------------------*
/**
 * Function name			CMsgTabCtr::~CMsgTabCtr
 * Description			
 * @return			
 * @exception			
 * @see			
*/
CMsgTabCtr::~CMsgTabCtr()
{

}

//------------------------------------------------------------------*
/**
 * Set owner of context menu that is responsible for popping up a context
 * menu on right mouse click inside the tab control. The owner is sent
 * a WM_TAB_CONTEXTMENU message on right mouse button click and then
 * may popup its context menu.
 *
 * @param           pOwner: window receiving context menu popup notifications
 * @return          -
 * @exception       -
*/
void CMsgTabCtr::SetContextMenuOwner (CWnd *pOwner)
{
    m_pContextMenuOwner = pOwner;
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
void CMsgTabCtr::Initialize (DWORD dwStyle)
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
void CMsgTabCtr::DeleteTab (int iIndex)
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
 * Function name	CMsgTabCtr::SetTabIcon
 * Description			
 * @param			int nIndex
 * @param			HICON hIcon
 * @return			void 
 * @exception			
 * @see			
*/
void CMsgTabCtr::SetTabIcon(int nIndex, HICON hIcon)
{
    int i;
	CMsgTab* pTab = (CMsgTab*)GetTabPtr(nIndex);
	pTab->m_hIcon = hIcon;

    // SIS: this solves repainting problems of tabs
    // (don't know why)
	// Invalidate the tabs to the right
	for (i=nIndex; i<GetTabCount(); i++)
		InvalidateTab(i);

    RecalcLayout();

	// Invalidate the tabs to the right
	for (i=nIndex; i<GetTabCount(); i++)
		InvalidateTab(i);
}


//------------------------------------------------------------------*
/**
 * Function name			CMsgTabCtr::SetTabColor
 * Description			
 * @param			int nIndex
 * @param			COLORREF Color
 * @return			void 
 * @exception			
 * @see			
*/
void CMsgTabCtr::SetTabColor(int nIndex, COLORREF Color)
{
    int i;
	CMsgTab* pTab = (CMsgTab*)GetTabPtr(nIndex);
	pTab->m_Color = Color;

    // SIS: this solves repainting problems of tabs
    // (don't know why)
	// Invalidate the tabs to the right
	for (i=nIndex; i<GetTabCount(); i++)
		InvalidateTab(i);

    RecalcLayout();

	// Invalidate the tabs to the right
	for (i=nIndex; i<GetTabCount(); i++)
		InvalidateTab(i);
}


//------------------------------------------------------------------*
/**
 * Function name	CMsgTabCtr::CreateNewTab
 * Description			
 * @return			SECTab* 
 * @exception			
 * @see			
*/
SECTab* CMsgTabCtr::CreateNewTab() const
{
	return new CMsgTab();	
}


//------------------------------------------------------------------*
/**
 * Function name	CMsgTabCtr::DrawTab
 * Description			
 * @param			CDC *pDC
 * @param			int nTab
 * @param			CRgn &tabRgn
 * @return			void 
 * @exception			
 * @see			
*/
void CMsgTabCtr::DrawTab(CDC *pDC, int nTab, CRgn &tabRgn)
{
    CMsgTab* pTab = (CMsgTab*)GetTabPtr(nTab);
    CPen *pOldPen;
    CPoint pts[4];
    CPoint tab_offset;
    CRect rectClient;
    RECT rectTab;
    CSize text_size;
    UINT oldTextAlign;
 	CRgn fillRgn;
	CPoint refPt;
    ASSERT(nTab >= 0 && nTab < GetTabCount());
	
    // Fill tab	using currently selected brush
    CRect tabsRect;
    int tab;
    
    GetTabsRect(tabsRect);
    for (tab = 0, tab_offset.x = tabsRect.left + m_nScrollOffset; tab < nTab;
	 tab_offset.x += (GetTab(tab).m_nWidth + m_nTabSpacing), tab++);

    // Initialize pts in the tab polygon
    pts[0].x = tab_offset.x;
    pts[0].y = tabsRect.top;
    pts[1].x = pts[0].x + m_nTabFold;
    pts[1].y = tabsRect.bottom;
    pts[2].x = pts[1].x + GetTab(nTab).m_nWidth - m_nTabFold*2 - 1;
    pts[2].y = pts[1].y;
    pts[3].x = pts[2].x + m_nTabFold - 1;
    pts[3].y = pts[0].y;

    fillRgn.CreatePolygonRgn(pts, 4, ALTERNATE);
	pDC->PaintRgn(&fillRgn);
	fillRgn.DeleteObject();

    // Compute size of label (in pixels)
    text_size = pDC->GetTextExtent(GetTab(nTab).m_csLabel,
		_tcslen(GetTab(nTab).m_csLabel));
    
    // Compute tab points
    // (Unfortunately, we can't access the points comprising the tabRgn
    //  so we have to recompute them)
    tabRgn.GetRgnBox(&rectTab);
    
    pts[0].x = rectTab.left;
    pts[0].y = rectTab.top;
    pts[1].x = pts[0].x + m_nTabFold;
    pts[1].y = rectTab.bottom - 1;   
    pts[2].x = pts[1].x + GetTab(nTab).m_nWidth - m_nTabFold*2;
    pts[2].y = pts[1].y;
    pts[3].x = pts[2].x + m_nTabFold;
    pts[3].y = pts[0].y;
    refPt.x = pts[1].x + (pts[2].x - pts[1].x - text_size.cx)/2;
    refPt.y = pts[1].y + (pts[0].y - pts[1].y + text_size.cy)/2;
    // Draw tab lines
    pDC->MoveTo(pts[0].x, pts[0].y);
    pDC->LineTo(pts[1].x, pts[1].y);
    pOldPen = pDC->SelectObject(&m_pens[SECTAB_ACTIVE]);
    pDC->LineTo(pts[2].x-2, pts[2].y);
    pDC->LineTo(pts[3].x-2, pts[3].y);
    pDC->SelectObject(pOldPen);
    pDC->MoveTo(pts[2].x-1, pts[2].y);
    pDC->LineTo(pts[3].x-1, pts[3].y);
	// Draw icon
	// center bitmap vertically
#ifdef WIN32

        COLORREF crOldCol = pDC->SetTextColor((COLORREF)pTab->m_Color);
        CString strText = GetTab(nTab).m_csLabel;
    
        if (pTab->m_hIcon)
        {
            // Draw icon
	        // center bitmap vertically
			::DrawIconEx(pDC->GetSafeHdc(), pts[0].x+(nIconWidth/2), pts[0].y,
				pTab->m_hIcon, nIconWidth, nIconWidth, 0, 0, DI_NORMAL);
            // Draw tab text
            oldTextAlign = pDC->SetTextAlign( TA_LEFT | TA_BOTTOM | TA_NOUPDATECP );

            pDC->TextOut(refPt.x+(nIconWidth/2), refPt.y, strText,
    		        		_tcslen(strText));
        }
        else
        {
        // Draw tab text
            oldTextAlign = pDC->SetTextAlign( TA_LEFT | TA_BOTTOM | TA_NOUPDATECP );

            pDC->TextOut(refPt.x, refPt.y, strText,
    	        			_tcslen(strText));
        }
        pDC->SetTextColor( crOldCol );
#endif
}


//------------------------------------------------------------------*
/**
 * Function name	CMsgTabCtr::RecalcLayout
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CMsgTabCtr::RecalcLayout()
{
	CMsgTab* pTab;
	int nTab;

	for (nTab = 0; nTab < GetTabCount(); nTab++)
    {
		pTab = (CMsgTab*)GetTabPtr(nTab);
        if(pTab->m_hIcon)
        {
            pTab->m_nWidth = GetLabelWidth(nTab) + m_nLabelPadding + nIconWidth;
        }
        else
        {
		    pTab->m_nWidth = GetLabelWidth(nTab) + m_nLabelPadding;
        }
	}
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
void CMsgTabCtr::OnContextMenu (CWnd *pWnd, CPoint point)
{
    MsgTabHitInfo hitInfo;


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

    m_pContextMenuOwner->SendMessage (WM_TAB_CONTEXTMENU, hitInfo.m_iTabIndex, (LPARAM)&hitInfo);
}


int CMsgTabCtr::GetTabIndexFromScreenPosition(const CPoint& rtPoint)
{
    int iTabReturn = -1;
    CPoint  tPoint(rtPoint);

    // run over all tabs and test if mouse pos is inside tab region
    int iTabCount = GetTabCount();
    for(int iTab = 0; iTab < iTabCount; ++iTab)
    {
        if(TabHit(iTab, tPoint))
        {
            iTabReturn = iTab;
            break;
        }
    }
    return iTabReturn;
}


CString CMsgTabCtr::GetTabToolTipText(int iTab /*=-1*/)
{
    CString strToolTipText;
    // get mouse position
    POINT   curPos;
    if(::GetCursorPos(&curPos))
    {
        CPoint  tCursorPos(curPos);
        CString strToolTipText;
        if(iTab == -1)
        {
            iTab = GetTabIndexFromScreenPosition(tCursorPos);
        }
        if(iTab >= 0)
        {
            strToolTipText.Format("Tab%d", iTab);
        }
    }
    return strToolTipText;
}


