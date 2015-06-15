#include "stdafx.h"
#include "WatchTabWnd.h"
#include "WatchTabCtrl.h"
#include "WatchCtrlParent.h"
#include "WatchCtrlWrapper.h"


//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



IMPLEMENT_DYNCREATE (CWatchTabWindow, SECTabWnd)

BEGIN_MESSAGE_MAP(CWatchTabWindow, SECTabWnd)
	//{{AFX_MSG_MAP(CWatchTabWindow)
	ON_MESSAGE(TCM_TABSEL, OnTabSelect)
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
CWatchTabWindow::CWatchTabWindow ()
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
CWatchTabWindow::~CWatchTabWindow ()
{
}


//------------------------------------------------------------------*
/**
 * Set owner of context menu that is responsible for popping up a context
 * menu on right mouse click inside the tab control. The owner is sent
 * a WM_WATCHTAB_CONTEXTMENU message on right mouse button click and then
 * may popup its context menu. The owner is stored internally and propaga-
 * ted to the tab control if it already exists. Otherwise it is propagated
 * on creation of the tab control.
 *
 * @param           pOwner: window receiving context menu popup notifications
 * @return          -
 * @exception       -
 * @see             CWatchTabControl::SetContextMenuOwner
*/
void CWatchTabWindow::SetContextMenuOwner (CWnd *pOwner)
{
    CWatchTabControl *pTabControl;


    // Remember context menu owner. Might have to pro-
    // pagate later if tab control is not yet created.
    //////////////////////////////////////////////////

    m_pContextMenuOwner = pOwner;


    // If tab control is already created propa-
    // gate context menu owner to tab control.
    ///////////////////////////////////////////

    pTabControl = dynamic_cast<CWatchTabControl *> (m_pTabCtrl);
    if (pTabControl != NULL)
        pTabControl->SetContextMenuOwner (pOwner);
}


//------------------------------------------------------------------*
/**
 * Override for creation of tab control. A tab control derived from
 * Stingrays SECTabControl class is created. The context menu owner
 * is propagated to the tab control.
 *
 * @param           dwStyle: style of tab control
 *                  uiId   : id of tab control
 * @return          TRUE on success, FALSE on failure
 * @exception       -
 * @see             
*/
BOOL CWatchTabWindow::CreateTabCtrl (DWORD dwStyle, UINT uiID)
{
    CWatchTabControl *pTabControl;
    dwStyle;


    // Instantiate derived tab control class. Note that MFC's
    // new operator throws an exception in out of memory case
    // Set tab control into base class attribute.
    /////////////////////////////////////////////////////////

    try
    {
        pTabControl = new CWatchTabControl;
    }
    catch (CMemoryException *e)
    {
        e->Delete ();
        return (FALSE);
    };

    m_pTabCtrl = pTabControl;


    // Create the tab control window. Copied from Stingray source.
    //////////////////////////////////////////////////////////////

    if (!pTabControl->Create (WS_VISIBLE | m_dwTabCtrlStyle, CRect (0, 0, 0, 0), this, uiID))
        return (FALSE);


    // Propagate context menu owner to tab control.
    ///////////////////////////////////////////////

    pTabControl->SetContextMenuOwner (m_pContextMenuOwner);

    return (TRUE);
}


LRESULT CWatchTabWindow::OnTabSelect(WPARAM wParam, LPARAM lParam)
{
	SECTABINFO* pTabInfo;
    pTabInfo = (SECTABINFO*)lParam;

    if(pTabInfo->nIndexPrevTab != -1)
    {
        // deactivate currently active tab
        LPCTSTR                 pszLabel;
        BOOL                    bSelected;
        CWnd*                   pWnd;
        void*                   pExtra;
        CWatchControlParent*    pParent;
        CWatchControlWrapper*   pWrapper;

        if(GetTabInfo(pTabInfo->nIndexPrevTab, pszLabel, bSelected, pWnd, pExtra))
        {
            // Embedded window must be the Active X Control parent window.
            pParent = dynamic_cast<CWatchControlParent *> (pWnd);
            if(pParent)
            {
                // Ask Active X Control parent for Active X Control wrapper
                pWrapper = pParent->GetWrapper();
                if(pWrapper)
                {
                    pWrapper->Deactivate();
                }
            }
        }
        if(GetTabInfo(pTabInfo->nIndexActiveTab, pszLabel, bSelected, pWnd, pExtra))
        {
            // Embedded window must be the Active X Control parent window.
            pParent = dynamic_cast<CWatchControlParent *> (pWnd);
            if(pParent)
            {
                // Ask Active X Control parent for Active X Control wrapper
                pWrapper = pParent->GetWrapper();
                if(pWrapper)
                {
                    pWrapper->ForceResubscribe();
                }
            }
        }
    }

    return(SECTabWnd::OnTabSelect(wParam, lParam));
}
