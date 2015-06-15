#include "stdafx.h"
#include "WatchWnd.h"
#include "WatchTabWnd.h"
#include "WatchTabCtrl.h"
#include "WatchCtrlParent.h"
#include "WatchRC.h"
#include "CEMAIN.h"
#include "WatchRenameDlg.h"


//----  Local Defines:   -------------------------------------------*
#define LEFT_MARGIN 7
#define RIGHT_MARGIN 7
#define TOP_MARGIN 7
#define BOTTOM_MARGIN 7

//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



IMPLEMENT_DYNCREATE(CWatchWindow, CSizedControlBar)


BEGIN_MESSAGE_MAP(CWatchWindow, CSizedControlBar)
    //{{AFX_MSG_MAP(CWatchWindow)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_ERASEBKGND()
	ON_WM_SETFOCUS()		// SHIFT_F10 15.12.04
    ON_COMMAND(ID_WATCH_ADD_PANE, OnAddPane)
    ON_COMMAND(ID_WATCH_REMOVE_PANE, OnRemovePane)
    ON_COMMAND(ID_WATCH_IMPORT, OnImport)
    ON_COMMAND(ID_WATCH_EXPORT, OnExport)
    ON_COMMAND(ID_WATCH_RENAME_TAB, OnRenameTab)
    ON_WM_HELPINFO()
    //}}AFX_MSG_MAP
    ON_MESSAGE(WM_WATCHTAB_CONTEXTMENU, OnWatchTabContextMenu)
    ON_MESSAGE(WM_WATCHTAB_DBLCLCK, OnWatchTabDblClck)
    ON_MESSAGE(WM_WATCHTAB_RENAME, OnWatchTabRename)
END_MESSAGE_MAP()


//------------------------------------------------------------------*
/**
 * Constructor. Creates tab window object.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             
*/
CWatchWindow::CWatchWindow ()
: m_pTabWindow (NULL),
  m_iContextMenuTab (-1),
  m_iCustomPaneCount (0),
  m_iForcedPaneTab (-1),
  m_hAccelerators(NULL),    // SHIFT_F10 14.12.04 SIS
  m_StateChangeCB(this, OnStateChange)
{
    try
    {
        m_pTabWindow = new CWatchTabWindow;
    }
    catch (CMemoryException *e)
    {
        e->Delete ();
    }

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
 * Destructor. Destroys tab window object.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             
*/
CWatchWindow::~CWatchWindow () 
{
    CCEStateManager *pStateManager = NULL;
    pStateManager = CEGetStateManager();
    ASSERT(pStateManager);
    if (pStateManager != NULL)
    {
        pStateManager->UnregisterCallback (&m_StateChangeCB);
    }
    delete m_pTabWindow;
    DeleteTabInfo();
}


void CWatchWindow::DeleteTabInfo()
{
    POSITION    pos;
    ETabType*   pTabType;
    pos = m_tTabInfo.GetHeadPosition();
    while(pos)
    {
        pTabType = m_tTabInfo.GetNext(pos);
        delete pTabType;
    }
    m_tTabInfo.RemoveAll();
}


//------------------------------------------------------------------*
/**
 * WM_CREATE handler. Creates the tab window as child window. Installs
 * ourselves as context menu owner. Adds one custom pane and activates
 * this pane.
 *
 * @param           lpCreateStruct: creation information
 * @return          0 on success, -1 on failure
 * @exception       -
 * @see             
*/
int CWatchWindow::OnCreate (LPCREATESTRUCT lpCreateStruct) 
{
    if (SECControlBar::OnCreate (lpCreateStruct) == -1)
        return (-1);

    DWORD style = GetExBarStyle();
    style |= CBRS_EX_STDCONTEXTMENU;
    style |= CBRS_EX_ALLOW_MDI_FLOAT;
    SetExBarStyle(style);

    if (m_pTabWindow == NULL)
        return (-1);

    if (!m_pTabWindow->Create (this, WS_CHILD | WS_VISIBLE | TWS_FULLSCROLL | TWS_TABS_ON_BOTTOM | TWS_NOACTIVE_TAB_ENLARGED | TWS_DYNAMIC_ARRANGE_TABS))
        return (-1);

    m_pTabWindow->SetContextMenuOwner (this);

    if(AddCustomPane() == -1)
    {
        return -1;
    }

    ActivateTab(0);
    
    return (0);
}

BOOL CWatchWindow::ActivateTab(int iTab)
{
    // bug fix d-1898: crash if watch ocx not registered
    LPCTSTR     lpszLabel;
    BOOL        bSelected;
    CWnd*       pWnd;
    void*       pExtra = NULL;
    ETabType    eTabType = eAutomatic;

    m_pTabWindow->GetTabInfo(iTab, lpszLabel, bSelected, pWnd, pExtra);
    if(pWnd)
    {
        m_pTabWindow->ActivateTab(iTab);
        return TRUE;
    }
    return FALSE;
}

//------------------------------------------------------------------*
/**
 * WM_SIZE handler. Resizes tab window to fit into our client area.
 *
 * @param           nType: sizing reason
 *                  cx   : new width
 *                  cy   : new height
 * @return          -
 * @exception       -
 * @see             
*/
void CWatchWindow::OnSize (UINT nType, int cx, int cy) 
{
    SECControlBar::OnSize (nType, cx, cy);


    // If we have a tab window shrink our client area geometry by
    // some margin size and reposition/resize the tab window.
    /////////////////////////////////////////////////////////////

    if (m_pTabWindow != NULL && m_pTabWindow->m_hWnd != NULL)
    {
        CRect geometry (0, 0, cx, cy);

        geometry.left += LEFT_MARGIN;
        if (geometry.left > geometry.right)
            geometry.left = geometry.right;

        geometry.right -= RIGHT_MARGIN;
        if (geometry.right < geometry.left)
            geometry.right = geometry.left;

        geometry.top += TOP_MARGIN;
        if (geometry.top > geometry.bottom)
            geometry.top = geometry.bottom;

        geometry.bottom -= BOTTOM_MARGIN;
        if (geometry.bottom < geometry.top)
            geometry.bottom = geometry.top;

        m_pTabWindow->MoveWindow (geometry);
    };
}


//------------------------------------------------------------------*
/**
 * WM_ERASEBKGND handler. Draws background like button background.
 *
 * @param           pDC: DC to draw background into
 * @return          TRUE to tell we actually erased the background.
 * @exception       -
 * @see             
*/
BOOL CWatchWindow::OnEraseBkgnd (CDC *pDC)
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


//------------------------------------------------------------------*
/**
 * ID_WATCH_ADD_PANE handler. Called from popup menu item "Add Pane".
 * Adds another custom pane. Shows this pane.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             
*/
void CWatchWindow::OnAddPane ()
{
    int iCustomPane;


    // Add a custom pane as rightmost custom pane.
    //////////////////////////////////////////////

    iCustomPane = AddCustomPane ();


    // Show this custom pane.
    /////////////////////////

    ShowCustomPane (iCustomPane);
}


//------------------------------------------------------------------*
/**
 * ID_WATCH_REMOVE_PANE handler. Called from popup menu item "Remove Pane".
 * Removes custom pane the mouse cursor pointed at when popping up the
 * context menu.
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CWatchWindow::OnRemovePane ()
{

    // Must have hit a tab since otherwise the menu item should be disabled.
    ////////////////////////////////////////////////////////////////////////

    ASSERT (m_iContextMenuTab != -1);
    if (m_iContextMenuTab == -1)
        return;


    // Remove custom pane of hit tab. m_iContextMenu must designate a custom
    // pane since otherwise the menu item should be disabled. RemoveCustomPane
    // checks if the passed index actually designates a custom pane tab.
    //////////////////////////////////////////////////////////////////////////

    RemoveCustomPane (m_iContextMenuTab);
}


//------------------------------------------------------------------*
/**
 * WM_WATCHTAB_CONTEXTMENU handler. This message is sent by the tab
 * control on right mouse button click.
 *
 * @param           wparam: not used
 *                  lparam: pointer to WatchTabHitInfo
 * @return          0
 * @exception       -
 * @see             
*/
LRESULT CWatchWindow::OnWatchTabContextMenu (WPARAM wparam, LPARAM lparam)
{
    WatchTabHitInfo *pHitInfo;
    CMenu menuBar;
    CMenu *pMenu;
    bool bEnable;


    // LPARAM is a pointer to WatchTabHitInfo.
    //////////////////////////////////////////

    pHitInfo = (WatchTabHitInfo *)lparam;
    if (pHitInfo == NULL)
        return (0);


    // Remember the tab we hit (-1 if none). Menu item command
    // handlers might find this information useful.
    //////////////////////////////////////////////////////////

    m_iContextMenuTab = pHitInfo->m_iTabIndex;


    // Load the context menu and enable/disable menu items according to where the user clicked.
    // "Add Pane" is always enabled.
    // "Remove Pane" is enabled only if the user hit a tab, this tab is a custom pane tab and
    // we have more than one custom pane tab.
    ///////////////////////////////////////////////////////////////////////////////////////////

    if (!menuBar.LoadMenu (IDM_WATCH_TABMENU))
        return (0);

    pMenu = menuBar.GetSubMenu (0);
    if (pMenu == 0)
        return (0);

    pMenu->EnableMenuItem (ID_WATCH_ADD_PANE, MF_BYCOMMAND | MF_ENABLED);
    bEnable = m_iContextMenuTab != -1 &&
              m_iCustomPaneCount > 1 &&
              pHitInfo->m_iTabIndex >= 0 &&
              pHitInfo->m_iTabIndex < m_iCustomPaneCount;
    pMenu->EnableMenuItem (ID_WATCH_REMOVE_PANE, MF_BYCOMMAND | (bEnable ? MF_ENABLED : MF_GRAYED));


    // Show and track popup menu. Command handlers are called from inside here.
    ///////////////////////////////////////////////////////////////////////////

    pMenu->TrackPopupMenu (TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, pHitInfo->m_Point.x, pHitInfo->m_Point.y, this);

    return (0);
}

void CWatchWindow::OnWatchTabDblClck(WPARAM wparam, LPARAM lparam)
{
    OnWatchTabRename(wparam, lparam);
}

void CWatchWindow::OnWatchTabRename(WPARAM wparam, LPARAM lparam)
{
    WatchTabHitInfo *pHitInfo;

    // LPARAM is a pointer to WatchTabHitInfo.
    //////////////////////////////////////////
    pHitInfo = (WatchTabHitInfo *)lparam;
    if (pHitInfo == NULL)
        return;


    // Remember the tab we hit (-1 if none). Menu item command
    // handlers might find this information useful.
    //////////////////////////////////////////////////////////

    m_iContextMenuTab = pHitInfo->m_iTabIndex;

    OnRenameTab();
}

//------------------------------------------------------------------*
/**
 * Store project path and propagate to control wrappers that already exist.
 *
 * @param           strProjectPath: The project path
 * @return          -
 * @exception       -
 * @see             
*/
void CWatchWindow::SetProjectPath (CString strProjectPath)
{
    int i;

    m_strProjectPath = strProjectPath;

    for (i = 0; i < m_iCustomPaneCount; ++i)
    {
        CWatchControlWrapper *pWrapper;

        pWrapper = GetCustomPaneWrapper (i);
        if (pWrapper != NULL)
            pWrapper->SetProjectPath (strProjectPath);
    };
}


//------------------------------------------------------------------*
/**
 * AddCustomPane
 *
 * @param           [in] crstrName: name of pane (default is empty)
 *                       if empty, a default name is created and
 *                       the pane is regarded as automatic pane,
 *                       i.e. the name is always "Watch x" where x is
 *                       the pane index + 1
 * @return          Index of newly added custom pane
 *                  (0..number of custom panes - 1)
 * @exception       -
 * @see             
*/
int CWatchWindow::AddCustomPane(const CString& crstrName /*=_T("")*/)
{
    int iTabIndex;
    int iCustomPane;
    CWatchControlWrapper *pWrapper;
    SECTab* pTab = NULL;
    CString strName(crstrName);
    ETabType    eType = eImportExport;

    if (m_pTabWindow == NULL)
        return (-1);


    // We insert new custom panes to the right of existing custom panes.
    // Thus current custom pane count will be index of new custom pane.
    ////////////////////////////////////////////////////////////////////

    iCustomPane = m_iCustomPaneCount;
    if(strName.IsEmpty())
    {
        strName = BuildCustomPaneCaption (iCustomPane);
        eType = eAutomatic;
    }

    // Add a tab for our custom pane. We have to add the tab index base for custom panes
    // to our custom pane index to get the effective tab index. Note that Stingrays tab
    // window does not allow to use the InsertTab method to append a tab. Instead we have
    // to use AddTab in this case. Set scroll style of new tab.
    /////////////////////////////////////////////////////////////////////////////////////

    iTabIndex = iCustomPane;

    if(!InsertTab(iTabIndex, strName, eType))
    {
        return -1;
    }

    m_pTabWindow->SetScrollStyle (iTabIndex, 0);


    // Obviously have to increase number of custom panes.
    /////////////////////////////////////////////////////

    ++m_iCustomPaneCount;


    // If we have a forced pane and we inserted before the forced
    // pane we have to increase the forced panes tab index.
    /////////////////////////////////////////////////////////////

    if (m_iForcedPaneTab != -1 && m_iForcedPaneTab >= 0)
        ++m_iForcedPaneTab;


    // Propagate project path to the Active X Control wrapper, if already 
    //set (may be not set yet if OnCreate).
    //////////////////////////////////////////////////////////

    pWrapper = GetCustomPaneWrapper (iCustomPane);
    if (pWrapper != NULL && m_strProjectPath.GetLength())
        pWrapper->SetProjectPath (m_strProjectPath);


    return (iCustomPane);
}


//------------------------------------------------------------------*
/**
 * Removes a custom pane.
 *
 * @param           iCustomPane: index of custom pane to remove
 *                               (0..number of custom panes - 1)
 * @return          -
 * @exception       -
 * @see             
*/
void CWatchWindow::RemoveCustomPane (int iCustomPane)
{
    int i;


    // Must be given the index of a custom pane tab.
    // Otherwise our caller did something wrong.
    ////////////////////////////////////////////////

    ASSERT (iCustomPane >= 0 && iCustomPane < m_iCustomPaneCount);
    if (iCustomPane < 0 || iCustomPane >= m_iCustomPaneCount)
        return;


    // Remove tab of custom pane.
    /////////////////////////////

    RemoveTab (iCustomPane);


    // Obviously have to decrease number of custom panes.
    /////////////////////////////////////////////////////

    --m_iCustomPaneCount;


    // Rename all tabs to the right (index has changed -> index depending name changes).
    ////////////////////////////////////////////////////////////////////////////////////

    // SIS: only automatic tabs are renamed
    ETabType eTabType;

    for (i = iCustomPane; i < m_iCustomPaneCount; ++i)
    {
        eTabType = GetTabType(i);
        if(eTabType == eAutomatic)
        {
            m_pTabWindow->RenameTab(i, BuildCustomPaneCaption (i));
        }
    }


    // If we have a forced pane and we removed before the forced
    // pane we have to decrease the forced panes tab index.
    ////////////////////////////////////////////////////////////

    if (m_iForcedPaneTab != -1 && m_iForcedPaneTab >= 0)
        --m_iForcedPaneTab;
}


//------------------------------------------------------------------*
/**
 * get tab type.
 *
 *  get value of extra pointer of tab as tab type.
 *
 * @param           [in] iIndex: index of tab
 * @return          tab type (stored in extra pointer of tab)
 * @exception       -
 * @see             SetTabType()
*/
CWatchWindow::ETabType CWatchWindow::GetTabType(int iIndex)
{
    LPCTSTR     lpszLabel;
    BOOL        bSelected;
    CWnd*       pWnd;
    void*       pExtra = NULL;
    ETabType    eTabType = eAutomatic;

    m_pTabWindow->GetTabInfo(iIndex, lpszLabel, bSelected, pWnd, pExtra);
    ASSERT(pExtra);
    eTabType = *(ETabType*) pExtra;
    return eTabType;
}


//------------------------------------------------------------------*
/**
 * set tab type.
 *
 *  set value of extra pointer of tab as tab type.
 *
 * @param           [in] iIndex: index of tab
 * @param           [in] eTabType: value to be set
 * @return          -
 * @exception       -
 * @see             GetTabType()
*/
void CWatchWindow::SetTabType(int iIndex, ETabType eTabType)
{
    LPCTSTR     lpszLabel;
    BOOL        bSelected;
    CWnd*       pWnd;
    void*       pExtra = NULL;

    m_pTabWindow->GetTabInfo(iIndex, lpszLabel, bSelected, pWnd, pExtra);
    *(ETabType*)pExtra = eTabType;
}

//------------------------------------------------------------------*
/**
 * remove tab.
 *
 *  stingray is not so kind as to remove the associated view.
 *  we do this ourselves.
 *
 * @param           [in] iIndex: index of tab to be removed
 *
*/
void CWatchWindow::RemoveTab(int iIndex)
{
    LPCTSTR     lpszLabel;
    BOOL        bSelected;
    CWnd*       pWnd;
    void*       pExtra;

    // Don't just delete the tab, destroy the associated window too.
    if (m_pTabWindow->GetTabInfo(iIndex, lpszLabel, bSelected, pWnd, pExtra))
    {
        pWnd->ShowWindow(SW_HIDE);
        pWnd->SendMessage(WM_CLOSE);
    }
    m_pTabWindow->RemoveTab(iIndex);
}


//------------------------------------------------------------------*
/**
 * Shows a custom pane
 *
 * @param           iCustomPane: index of custom pane to show
 *                               (0..number of custom panes - 1)
 * @return          -
 * @exception       -
 * @see             
*/
void CWatchWindow::ShowCustomPane (int iCustomPane)
{

    // Must be given the index of a custom pane tab.
    // Otherwise our caller did something wrong.
    ////////////////////////////////////////////////

    ASSERT (iCustomPane >= 0 && iCustomPane < m_iCustomPaneCount);
    if (iCustomPane < 0 || iCustomPane >= m_iCustomPaneCount)
        return;


    // Activate the custom pane's tab.
    //////////////////////////////////

    ActivateTab (iCustomPane);
}


//------------------------------------------------------------------*
/**
 * Loads Active X Control in a custom pane with file content.
 *
 * @param           iCustomPane: index of custom pane to load
 *                               (0..number of custom panes - 1)
 *                               if index is -1, the context index is used
 *                               if this is not valid, a new pane will be added
 *                               and the index is set to this pane.
 *                  strPath    : File path
 *                               if File path is empty, a file selection box appears
 * @return          Error code
 * @exception       -
 * @see             
*/
HRESULT CWatchWindow::LoadCustomPane (int iCustomPane /*=-1*/, CString strPath /*=_T("")*/)
{
    HRESULT hr;
    CWatchControlWrapper *pWrapper;
    BOOL    bAddedPane = FALSE;

    if(iCustomPane < 0 || iCustomPane >= m_iCustomPaneCount)
    {
        iCustomPane = m_iContextMenuTab;
        if(iCustomPane < 0 || iCustomPane >= m_iCustomPaneCount)
        {
            iCustomPane = AddCustomPane();
            ActivateTab(iCustomPane);
            bAddedPane = TRUE;
        }
    }

    if(strPath.IsEmpty())
    {
        strPath = QueryWatchFile(false);
        // if user pressed cancel -> return S_FALSE
        // if pane was added -> remove it
        if(strPath.IsEmpty())
        {
            if(bAddedPane)
            {
                RemoveCustomPane(iCustomPane);
            }
            return S_FALSE;
        }
    }

    // Get Active X Control wrapper of custom pane.
    ///////////////////////////////////////////////

    pWrapper = GetCustomPaneWrapper (iCustomPane);
    if (pWrapper == NULL)
        return (E_UNEXPECTED);


    // Tell wrapper to load file content into Active X Control.
    ///////////////////////////////////////////////////////////

    hr = pWrapper->Load (strPath);
    if(hr == S_OK)
    {
        CString strFileName;
        CString strTabName;
        int     iFound = strPath.ReverseFind(_T('\\'));

        strFileName = strPath.Mid(iFound+1);
        iFound = strFileName.ReverseFind(_T('.'));
        if(iFound > 0)
        {
            strTabName = strFileName.Left(iFound);
        }
//        if(GetTabType(iCustomPane) < eManual)   // eAutomatic or eImportExport
//        {
            m_pTabWindow->RenameTab(iCustomPane, strTabName);
            SetTabType(iCustomPane, eImportExport);
//        }
    }
    return S_OK;
}


//------------------------------------------------------------------*
/**
 * Stores content of Active X Control in a custom pane into a file.
 *
 * @param           iCustomPane: index of custom pane to store
 *                               (0..number of custom panes - 1)
 *                  strPath    : File path
 * @return          Error code
 * @exception       -
 * @see             
*/
HRESULT CWatchWindow::SaveCustomPane (int iCustomPane /*=-1*/, CString strPath /*=_T("")*/)
{
    CWatchControlWrapper *pWrapper;

    if(iCustomPane < 0 || iCustomPane >= m_iCustomPaneCount)
    {
        iCustomPane = m_iContextMenuTab;
        if(iCustomPane < 0 || iCustomPane >= m_iCustomPaneCount)
        {
            return E_FAIL;
        }
    }

    if(strPath.IsEmpty())
    {
        CString strFile;
        if(GetTabType(iCustomPane) != eAutomatic)
        {
            CString strLabel;
            strLabel = GetTabLabel(iCustomPane);
            strFile = strLabel + _T(".4cw");
        }
        strPath = QueryWatchFile(true, strFile);
    }

    // Get Active X Control wrapper of custom pane.
    ///////////////////////////////////////////////

    pWrapper = GetCustomPaneWrapper (iCustomPane);
    if (pWrapper == NULL)
    {
        return (E_UNEXPECTED);
    }


    // Tell wrapper to store Active X Control content into a file.
    //////////////////////////////////////////////////////////////
    HRESULT hr = pWrapper->Store(strPath);
    if(hr == S_OK)
    {
        CString strFileName;
        CString strTabName;
        int     iFound = strPath.ReverseFind(_T('\\'));

        strFileName = strPath.Mid(iFound+1);
        iFound = strFileName.ReverseFind(_T('.'));
        if(iFound > 0)
        {
            strTabName = strFileName.Left(iFound);
        }
//        if(GetTabType(iCustomPane) < eManual)
//        {
            m_pTabWindow->RenameTab(iCustomPane, strTabName);
            SetTabType(iCustomPane, eImportExport);
//        }
    }
    return S_OK;
}


//------------------------------------------------------------------*
/**
 * Resubscribes variables/expressions of Active X Control in a custom pane.
 *
 * @param           iCustomPane: index of custom pane to store
 *                               (0..number of custom panes - 1)
 * @return          Error code
 * @exception       -
 * @see             
*/
HRESULT CWatchWindow::ResubscribeCustomPane (int iCustomPane)
{
    CWatchControlWrapper *pWrapper;


    // Get Active X Control wrapper of custom pane.
    ///////////////////////////////////////////////

    pWrapper = GetCustomPaneWrapper (iCustomPane);
    if (pWrapper == NULL)
        return (E_UNEXPECTED);


    // Tell wrapper to resubscribe variables/expressions of Active X Control.
    /////////////////////////////////////////////////////////////////////////

    return (pWrapper->ForceResubscribe ());
}


//------------------------------------------------------------------*
/**
 * Return Active X Control wrapper of a custom pane.
 *
 * @param           iCustomPane: index of custom pane
 *                  (0..number of custom panes - 1)
 * @return          Active X Control wrapper
 * @exception       -
 * @see             
*/
CWatchControlWrapper *CWatchWindow::GetCustomPaneWrapper (int iCustomPane)
{
    LPCTSTR pszLabel;
    BOOL bSelected;
    CWnd *pWnd;
    void *pExtra;
    CWatchControlParent *pParent;


    // Must be given the index of a custom pane tab.
    // Otherwise our caller did something wrong.
    ////////////////////////////////////////////////

    ASSERT (iCustomPane >= 0 && iCustomPane < m_iCustomPaneCount);
    if (iCustomPane < 0 || iCustomPane >= m_iCustomPaneCount)
        return (NULL);


    // Get info for tab (especially embedded window).
    /////////////////////////////////////////////////

    if (!m_pTabWindow->GetTabInfo (iCustomPane, pszLabel, bSelected, pWnd, pExtra))
        return (NULL);


    // Embedded window must be the Active X Control parent window.
    //////////////////////////////////////////////////////////////

    pParent = dynamic_cast<CWatchControlParent *> (pWnd);
    if (pParent == NULL)
        return (NULL);


    // Ask Active X Control parent for Active X Control wrapper and return it.
    //////////////////////////////////////////////////////////////////////////

    return (pParent->GetWrapper ());
}


//------------------------------------------------------------------*
/**
 * Build default caption for a custom pane. Default caption contains
 * the custom pane index.
 *
 * @param           iCustomPane: Custom pane index
 * @return          Default caption
 * @exception       -
 * @see             
*/
CString CWatchWindow::BuildCustomPaneCaption (int iCustomPane)
{
    CString strText;

    strText.Format (IDS_WATCH_CUSTOM_PANE_CAPTION, iCustomPane + 1);

    return (strText);
}


//------------------------------------------------------------------*
/**
 * Adds the "Forced" pane to the watch.
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CWatchWindow::AddForcedPane ()
{
    CString strText;


    // Nothing to do if we already have the "Forced" pane.
    //////////////////////////////////////////////////////

    if (m_iForcedPaneTab != -1)
        return;


    // Add a tab for our "Forced" pane. It is to the right of all custom pane tabs.
    // Note that Stingrays tab window does not allow to use the InsertTab method to
    // append a tab. Instead we have to use AddTab in this case. Set scroll style of
    // new tab.
    ////////////////////////////////////////////////////////////////////////////////

    strText.LoadString (IDS_WATCH_FORCED_PANE_CAPTION);

    m_iForcedPaneTab = m_iCustomPaneCount;

    AddTab(strText, eForce);

    m_pTabWindow->SetScrollStyle (m_iForcedPaneTab, 0);
}


//------------------------------------------------------------------*
/**
 * add tab.
 *
 * @param           [in] crstrLabel: tab label
 * @param           [in] eType: tab type
 * @return          pointer to new SECTab
 * @exception       -
 * @see             ETabType
*/
SECTab* CWatchWindow::AddTab(const CString& crstrLabel, ETabType eType)
{
    SECTab* pTab = NULL;
    pTab = m_pTabWindow->AddTab(RUNTIME_CLASS (CWatchControlParent), crstrLabel);
    if(pTab)
    {
        if(pTab->m_pClient == NULL)
        {
            m_pTabWindow->RemoveTab(m_pTabWindow->GetTabCount()-1);
            return NULL;
        }
        ETabType*   pTabType = new ETabType;
        *pTabType = eType;
        pTab->m_pExtra = (void*)pTabType;
        m_tTabInfo.AddHead(pTabType);
    }
    return pTab;
}


//------------------------------------------------------------------*
/**
 * insert tab.
 *
 * @param           [in] iIndex: index of new tab
 * @param           [in] crstrLabel: tab label
 * @param           [in] eType: tab type
 * @return          pointer to new SECTab
 * @exception       -
 * @see             ETabType
*/
SECTab* CWatchWindow::InsertTab(int iIndex, const CString& crstrLabel, ETabType eType)
{
    SECTab* pTab = NULL;
    if(iIndex < 0 || iIndex >= m_pTabWindow->GetTabCount())
    {
        return AddTab(crstrLabel, eType);
    }
    pTab = m_pTabWindow->InsertTab(RUNTIME_CLASS (CWatchControlParent), iIndex, crstrLabel);
    if(pTab)
    {
        if(pTab->m_pClient == NULL)
        {
            m_pTabWindow->RemoveTab(iIndex);
            return NULL;
        }
        ETabType*   pTabType = new ETabType;
        *pTabType = eType;
        pTab->m_pExtra = (void*)pTabType;
        m_tTabInfo.AddHead(pTabType);
    }
    return pTab;
}

//------------------------------------------------------------------*
/**
 * Remove the "Forced" pane from the watch.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             
*/
void CWatchWindow::RemoveForcedPane ()
{

    // Nothing to do if we don't have the "Forced" pane.
    ////////////////////////////////////////////////////

    if (m_iForcedPaneTab == -1)
        return;


    // Remove "Forced" pane tab.
    ////////////////////////////
    RemoveTab (m_iForcedPaneTab);

    m_iForcedPaneTab = -1;
}


//------------------------------------------------------------------*
/**
 * Show the "Forced" pane.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             
*/
void CWatchWindow::ShowForcedPane ()
{

    // Should have a "Forced" pane to make it visible.
    //////////////////////////////////////////////////

    ASSERT (m_iForcedPaneTab != -1);
    if (m_iForcedPaneTab == -1)
        return;


    // Activate the "Forced" pane's tab.
    ////////////////////////////////////

    ActivateTab (m_iForcedPaneTab);
}


void CWatchWindow::OnStateChange (CEStateNotify stateNotify, unsigned int uiStage)
{
    CString strName; 

    switch(stateNotify)
    {
    case CEFillSettingsNotify:
        {
            // get settings
            CSettings* pSettings = ((CCEMAINApp*)AfxGetApp())->GetSettings();
            if(pSettings)
            {
                FillSettings(pSettings->m_watchSettings);
            }
        }
        break;
    case CENewSettingsNotify:
        {
            // get settings
            CSettings* pSettings = ((CCEMAINApp*)AfxGetApp())->GetSettings();
            if(pSettings)
            {
                LoadSettings(pSettings->m_watchSettings);
            }
        }
        break;
    case CEDownloadCompleteNotify:
        {   


            for (int i=0; i<m_iCustomPaneCount; i++)
            {
                ResubscribeCustomPane(i);
            }
        }
		break;
	// SHIFT_F10 15.12.04 SIS >>
	case CECreateComponentNotify:
		{
            CString strName = CESMGetComponentName();
            if (strName == CE_COMPMAN_WATCH)
            {
				HINSTANCE hInstance;
				hInstance = AfxFindResourceHandle (MAKEINTRESOURCE (IDR_WATCHACCEL), RT_ACCELERATOR);
                m_hAccelerators = ::LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_WATCHACCEL));
            }
		}
		break;
	// SHIFT_F10 15.12.04 SIS <<
    }
}


//------------------------------------------------------------------*
/**
 * fill settings.
 *
 * @param           [in] pWatchSettings: pointer to watch settings.
 * @return          -
 * @exception       -
 * @see             -
*/
void CWatchWindow::FillSettings(CWatchSettings& rtWatchSettings)
{
    CWatchControlWrapper *pWrapper;
    CString     strName;
    CString     strXMLString;
    int         iNumPanes = m_pTabWindow->GetTabCount();
    CString     strLabel;
    ETabType    eTabType;

    int iActiveTab;
    if(m_pTabWindow->GetActiveTab(iActiveTab))
    {
        rtWatchSettings.SetActiveTab(iActiveTab);
    }

    // run over all panes
    for(int iPane = 0; iPane < iNumPanes; ++iPane)
    {
        strLabel = GetTabLabel(iPane);
        eTabType = GetTabType(iPane);
        // save name only if no automatic pane
        if(eTabType != eAutomatic)
        {
            strName = strLabel;
        }
		// 15.12.05 SIS >>
		// if automatic pane -> name must be empty
		else
		{
			strName.Empty();
		}
		// 15.12.05 SIS <<
        pWrapper = GetCustomPaneWrapper(iPane);
        pWrapper->SaveToXMLString(strXMLString);
        if(!strXMLString.IsEmpty())
        {
            rtWatchSettings.AddPane(strName, strXMLString);
        }
    }
}


//------------------------------------------------------------------*
/**
 * load settings.
 *
 * @param           [in] pWatchSettings: pointer to watch settings
 * @return          -
 * @exception       -
 * @see             -
*/
void CWatchWindow::LoadSettings(CWatchSettings& rtWatchSettings)
{
    HRESULT hr;
    CWatchControlWrapper *pWrapper;
    CString strName;
    CString strXMLString;
    int     iTmp;

    //run over all panes of watch settings
    int iNumPanes = rtWatchSettings.GetPaneCount();

    rtWatchSettings.GetPaneInfo(0, strName, strXMLString);
    pWrapper = GetCustomPaneWrapper(0);
    hr = pWrapper->LoadFromXMLString(strXMLString);
    if(hr != S_OK)
    {
        return;
    }
    if(!strName.IsEmpty())
    {
        SetTabType(0, eImportExport);
        SetTabLabel(0, strName);
    }

    for(int iPane = 1; iPane < iNumPanes; ++iPane)
    {
        rtWatchSettings.GetPaneInfo(iPane, strName, strXMLString);
        iTmp = AddCustomPane(strName);
        pWrapper = GetCustomPaneWrapper(iPane);
        hr = pWrapper->LoadFromXMLString(strXMLString);
        if(hr != S_OK)
        {
            break;
        }
    }
    int iActiveTab = rtWatchSettings.GetActiveTab();
    if(iActiveTab >= 0 && iActiveTab < m_iCustomPaneCount)
    {
        ActivateTab(iActiveTab);
    }

    // now delete all panes
    rtWatchSettings.Reset();
}

void CWatchWindow::OnImport()
{
    LoadCustomPane();
}

void CWatchWindow::OnExport()
{
    SaveCustomPane();
}


CString CWatchWindow::QueryWatchFile(bool bSave, const CString& crstrInitFile /*=_T("")*/)
{
    CString strFilterName1;
    CString strFilterExt1;
    CString strFilterName2;
    CString strFilterExt2;
    CString strFilter;
    CString strTitle;

    strTitle.LoadString(bSave ? IDS_STORE_WATCH_TITLE : IDS_LOAD_WATCH_TITLE);
    strFilterName1.LoadString(IDS_WATCH_FILE_FILTERNAME1);
    strFilterExt1.LoadString(IDS_WATCH_FILE_FILTEREXT1);
    strFilterName2.LoadString(IDS_WATCH_FILE_FILTERNAME2);
    strFilterExt2.LoadString(IDS_WATCH_FILE_FILTEREXT2);

    strFilter.Format (_T ("%s|*.%s|%s|*.%s||"), strFilterName1, strFilterExt1, strFilterName2, strFilterExt2);
    for (int ii = 0; ii < strFilter.GetLength (); ++ii)
    {
        if (strFilter[ii] == _T ('|'))
        {
            strFilter.SetAt (ii, _T ('\0'));
        }
    }

    CFileDialog dialog (!bSave, strFilterExt1);
    dialog.m_ofn.lpstrTitle = strTitle;
    dialog.m_ofn.lpstrFilter = strFilter;
    _tcscpy(dialog.m_ofn.lpstrFile, crstrInitFile);

    if (!m_strProjectPath.IsEmpty ())
        dialog.m_ofn.lpstrInitialDir = m_strProjectPath;

    if (dialog.DoModal () != IDOK)
    {
        return "";
    }
    return dialog.m_ofn.lpstrFile;
}


void CWatchWindow::OnRenameTab()
{
    CWatchRenameDlg tDlg;

    if(m_iContextMenuTab < 0 || m_iContextMenuTab >= m_iCustomPaneCount)
    {
        return;
    }

    tDlg.m_strName = GetTabLabel(m_iContextMenuTab);
    if(tDlg.DoModal() == IDOK)
    {
        SetTabLabel(m_iContextMenuTab, tDlg.m_strName, TRUE);
    }
}

//------------------------------------------------------------------*
/**
 * get tab label.
 *
 * @param           [in] iIndex: index of pane
 * @return          tab label
 * @exception       -
 * @see             -
*/
CString CWatchWindow::GetTabLabel(int iIndex)
{
    LPCTSTR     lpszLabel;
    BOOL        bSelected;
    CWnd*       pWnd;
    void*       pExtra = NULL;

    m_pTabWindow->GetTabInfo(iIndex, lpszLabel, bSelected, pWnd, pExtra);
    return lpszLabel;
}

//------------------------------------------------------------------*
/**
 * set tab label.
 *
 * @param            [in] iIndex: index of pane
 * @param            [in] crstrLabel: new label
 * @param            [in] bUser: user action -> tab type changes to manual
 *                               it is not renamed on import
 * @return           -
 * @exception        -
 * @see              -
*/
void CWatchWindow::SetTabLabel(int iIndex, const CString& crstrLabel, BOOL bUser /*=FALSE*/)
{
    m_pTabWindow->RenameTab(iIndex, crstrLabel);
    if(bUser)
    {
        SetTabType(iIndex, eManual);
    }
    m_pTabWindow->Invalidate();
}



BOOL CWatchWindow::OnHelpInfo(HELPINFO* pHelpInfo) 
{
    CWinApp *pApp;
    DWORD dwHelpID = HID_BASE_RESOURCE + WATCH_RESOURCEBASE;

    pApp = AfxGetApp ();
    if (pApp != NULL)
    {
        pApp->WinHelp(dwHelpID);
        return (TRUE);
    };

    return (FALSE);
}

// SHIFT_F10 15.12.04 SIS >>
//------------------------------------------------------------------*
/**
 * pre translate message.
 *
 *  overwritten to get accelerators keys
 *
*/
BOOL CWatchWindow::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
    {
        if (m_hAccelerators != NULL && ::TranslateAccelerator (m_hWnd, m_hAccelerators, pMsg))
        {
            return (TRUE);
        }
    }
	return CSizedControlBar::PreTranslateMessage(pMsg);
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
void CWatchWindow::OnSetFocus (CWnd *pOldWnd)
{
    SECControlBar::OnSetFocus (pOldWnd);

    CWatchControlWrapper *pWrapper;
    int iActiveTab;
    if(m_pTabWindow->GetActiveTab(iActiveTab))
	{
		pWrapper = GetCustomPaneWrapper(iActiveTab);
		if(pWrapper)
		{
			pWrapper->SetFocus();
		}
	}
}
// SHIFT_F10 14.12.04 SIS <<

