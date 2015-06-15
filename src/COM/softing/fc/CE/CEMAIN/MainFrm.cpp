
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*

// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include <assert.h>
#include "cecompman.h"
#include "CEMAIN.h"
#include "MainFrm.h"
#include "CESysDef.h"
#include "panel.h"
#include "panelrc.h"
#include "panelvw.h"
#include "fc_tools\fc_string.h"
#include "CEAddonAction.h"
#include "KADHelper\KADToolbar.h"
#include "KADHelper\KADFileExtInfo.h"
#include "utilif.h"
#include "TargetSet.h"
#include "MultiSelDlg.h"
#include "SettingsDialogs.h"
#include "GenEdView.h"
#include "GenEdCont.h"
#include "ExeDelegator.h"
#include "FindFileDlg.h"
#include "ComHelper.h"
#include "CEMAINDoc.h"
#include "PrintSettingsDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CCEDockBar, SECDockBar)


//------------------------------------------------------------------*
/**
 * Overridable of SECDockBar. SECDockBar's version invalidates itself
 * and later in its WM_PAINT handler invalidates all children contained
 * in the m_arrInvalidBars array. Unfortunately this array is not filled
 * correctly when resizing dock bar rows. Thus we put all control bars
 * into the array and let the WM_PAINT handler invalidate them. Makes
 * all control bars flicker but what can we do.
 *
 * How to reproduce? Remove everything but the base class call in here.
 * Stack two control bars (e.g. watch and message view) at the main frames
 * bottom. Drag the divider between the MDI client area and the upper con-
 * trol bar towards the main frames top. When releasing the mouse button
 * the lower control bar is not updated correctly.
 *
 * What's the problem? Hard to explain. I'll try. After having dragged the
 * divider first the upper control bar is resized to become taller. The upper
 * control bar now partly overlaps the lower control bar. Then the lower control
 * bar is moved downwards to its new position which reveals the overlapped part
 * of the lower control bar. For optimization reasons the SWP_NOREDRAW flag is
 * used for moving/sizing which prevents control bars from being updated immedi-
 * ately. The idea behind is to collect all control bars that require invalida-
 * tion due to use of the SWP_NOREDRAW flag in an array (m_arrInvalidBars) and
 * invalidate them together. And here we go: Stingray adds control bars to this
 * array only if they are resized but not if they are just moved. I suspect they
 * just did not take into account that moving the lower control bar reveals part
 * of this control bar which is not updated automatically (SWP_NOREDRAW).
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             
*/
void CCEDockBar::ProcessDelayedInvalidates ()
{
    int i;

    if (m_arrInvalidBars.GetSize () != 0)
    {
        m_arrInvalidBars.RemoveAll ();

        for (i = 0; i < m_arrBars.GetSize (); ++i)
        {
            SECControlBar *pControlBar;

            pControlBar = (SECControlBar *)GetDockedControlBar (i);

            if (pControlBar != NULL)
                m_arrInvalidBars.Add (pControlBar);
        }
    }

    SECDockBar::ProcessDelayedInvalidates ();
}


/////////////////////////////////////////////////////////////////////////////
// CMainFrame
IMPLEMENT_DYNAMIC(CMainFrame, SECMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, SECMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_DROPFILES()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_UPDATE_COMMAND_UI(ID_TOGGLE_NAVIGATOR, OnUpdateToggleNavigator)
	ON_COMMAND(ID_TOGGLE_NAVIGATOR, OnToggleNavigator)
	ON_COMMAND(ID_TOGGLE_MSG_VIEW, OnToggleMsgView)
	ON_UPDATE_COMMAND_UI(ID_TOGGLE_MSG_VIEW, OnUpdateToggleMsgView)
	ON_COMMAND(ID_TOGGLE_POU_SELECTION, OnTogglePouSelection)
	ON_UPDATE_COMMAND_UI(ID_TOGGLE_POU_SELECTION, OnUpdateTogglePouSelection)
	ON_COMMAND(ID_TOGGLE_XREF, OnToggleXRef)
  	ON_UPDATE_COMMAND_UI(ID_TOGGLE_XREF, OnUpdateToggleXRef)
	ON_COMMAND(ID_TOGGLE_WATCH_VIEW, OnToggleWatchView)
	ON_UPDATE_COMMAND_UI(ID_TOGGLE_WATCH_VIEW, OnUpdateToggleWatchView)
    ON_COMMAND(ID_TOGGLE_BREAKPT_VIEW, OnToggleBPView)
	ON_UPDATE_COMMAND_UI(ID_TOGGLE_BREAKPT_VIEW, OnUpdateToggleBPView)
	ON_WM_CLOSE()
    ON_NOTIFY_EX( TTN_GETDISPINFO, 0, OnGetDispInfo)
    ON_UPDATE_COMMAND_UI(ID_INDICATOR_EDITPOS, OnUpdateEditPos)
    ON_COMMAND(IDM_PROJECT_PRINT, OnPrintProject)
    ON_COMMAND(IDM_PRJFILE_PRINT, OnPrintFile)
	ON_UPDATE_COMMAND_UI(IDM_PRJFILE_PRINT, OnUpdatePrintFile)
    ON_COMMAND(IDM_XREF_PRINT, OnPrintXRef)
	ON_UPDATE_COMMAND_UI(IDM_XREF_PRINT, OnUpdatePrintXRef)
    ON_COMMAND(ID_FINDINFILES, OnFindInFiles)
    ON_UPDATE_COMMAND_UI(ID_FINDINFILES, OnUpdateFindInFiles)
	ON_COMMAND(ID_VIEW_FULLSCREEN, OnViewFullscreen)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FULLSCREEN, OnUpdateViewFullscreen)
    ON_COMMAND(ID_BREAK, OnBreak)
    ON_COMMAND(ID_CLOSE_ALL_WINDOWS, OnCloseAllWindows)
    ON_UPDATE_COMMAND_UI(ID_CLOSE_ALL_WINDOWS, OnUpdateCloseAllWindows)
    ON_COMMAND(ID_SOFT_HOMEPAGE, OnSoftHomepage)
	ON_COMMAND(ID_NEXT_ERROR, OnNextError)
	ON_COMMAND(ID_PREVIOUS_ERROR, OnPreviousError)
	//}}AFX_MSG_MAP
	// Global help commands
	ON_COMMAND(ID_HELP_FINDER, SECMDIFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_HELP, SECMDIFrameWnd::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, SECMDIFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, SECMDIFrameWnd::OnHelpFinder)
    ON_MESSAGE(ID_UPDATE_MAIN_MENU, OnUpdateMainMenu)
    ON_COMMAND_RANGE(ID_MAIN_MENU_BASE, ID_MAIN_MENU_END, OnMainMenuCommand)
    ON_UPDATE_COMMAND_UI_RANGE(ID_MAIN_MENU_BASE, ID_MAIN_MENU_END, OnUpdateMainMenuCommand)
	ON_COMMAND(IDM_CONTINUE_TASK, OnContinueTask)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
    ID_INDICATOR_MODE,
    ID_INDICATOR_EDITPOS,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


//static UINT BASED_CODE mainButtons[] =
//{
//    ID_FILE_NEW,
//    ID_FILE_OPEN,
//    ID_FILE_SAVE,
//	ID_SEPARATOR,
//    ID_FILE_PRINT,
//	ID_SEPARATOR,
//    ID_APP_ABOUT,
//    ID_CONTEXT_HELP
//};

//static UINT BASED_CODE viewButtons[] =
//{
//    ID_VIEW_FULLSCREEN,
//	ID_SEPARATOR,
//    ID_TOGGLE_NAVIGATOR,
//    ID_TOGGLE_MSG_VIEW,
//	ID_SEPARATOR,
//    ID_TOGGLE_POU_SELECTION,
//    ID_TOGGLE_WATCH_VIEW,
//    ID_TOGGLE_BREAKPT_VIEW,
////    ID_TOGGLE_LIB
//};

static UINT toolbars[] =     // static toolbars for cool menu
{
//    IDR_ADDTOOLBAR,
    IDR_IETOOLBAR,
    IDR_MAINFRAME,
//    IDR_MSGTOOLBAR,
    IDR_VIEWTOOLBAR,
//	IDR_HELPTOOLBAR
};




/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction
//------------------------------------------------------------------*
/**
 * construct CMainFrame object - main window of application.
 * Initializes Toolbar manager and registers callback method
 * OnStateChange with the state manager to receive state
 * callbacks.
 *
 * @see             CCEStateManager::RegisterCallback
*/
CMainFrame::CMainFrame()
: m_StateChangeCB (this, OnStateChange),
  m_pFileInfo(NULL),
  m_pContainer(NULL),
  m_bCheckDownloadGUIDs (false),
  m_bDoNotAddToCheckList(false)
{
    m_pDockBarClass = RUNTIME_CLASS (CCEDockBar);

	//m_pControlBarManager = new SECToolBarManager(this);
//	m_pMenuBar           = new SECMDIMenuBar;	// or SECMenuBar for SDI
//	// Use the cool bitmap menu support
//	EnableBmpMenus();

    m_bShowNavigator            = TRUE;
    m_bShowMsgView              = TRUE;
    m_bShowPouVarSel            = FALSE;
    m_bShowBPView               = FALSE;
    m_bShowWatch                = FALSE;
    m_bShowXRef                 = FALSE;
    m_iToolbarCount             = 0;
    m_bProjectLoaded            = FALSE;
    m_isRestoringDockingTools   = FALSE;
    m_hKADAccel                 = NULL;
    m_bKADAccelLoaded           = FALSE;    // accelerator from KAD are not yet loaded
    m_bViewFullScreen           = FALSE;
    m_uiVisFlags                = 0;
    m_bMaximized                = FALSE;    // only used for full screen mode
    m_bProjectReadOnly          = FALSE;
    m_bHandleForcedLogouts      = FALSE;


    CCEStateManager *pStateManager = NULL;
    pStateManager = CEGetStateManager ();
    assert(pStateManager);
    if (pStateManager != NULL)
    {
        pStateManager->RegisterCallback (&m_StateChangeCB);
    }
}

//------------------------------------------------------------------*
/**
 * Dtor of CMainFrame object - main window of application.
 * deletes allocated objects and unregisters callback method
 * OnStateChange from the state manager to receive state
 * callbacks.
 *
 * @see             CCEStateManager::UnregisterCallback
*/
CMainFrame::~CMainFrame()
{
    CCEStateManager * pStateManager = CEGetStateManager ();
    if (pStateManager != NULL)
    {
        pStateManager->UnregisterCallback (&m_StateChangeCB);
    }

    if(m_pFileInfo)
    {
        delete m_pFileInfo;
        m_pFileInfo = NULL;
    }

//	if(m_pControlBarManager)
//    {
//		delete m_pControlBarManager;
//        m_pControlBarManager = NULL;
//    }

//	if(m_pMenuBar)
//    {
//		delete m_pMenuBar;
//        m_pMenuBar = NULL;
//    }

	//if(m_hNewMenu)
	//	::DestroyMenu(m_hNewMenu);
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    CCEMAINApp* pApp = (CCEMAINApp *)AfxGetApp();
    int s, t, b, r, l;
    // only restore if there is a previously saved position
    if ( -1 != (s = pApp->GetProfileInt(REG_SECTION_FRAME, REG_KEY_FRAME_SHOW,     -1)) &&
         -1 != (t = pApp->GetProfileInt(REG_SECTION_FRAME, REG_KEY_FRAME_TOP,      -1)) &&
         -1 != (l = pApp->GetProfileInt(REG_SECTION_FRAME, REG_KEY_FRAME_LEFT,     -1)) &&
         -1 != (b = pApp->GetProfileInt(REG_SECTION_FRAME, REG_KEY_FRAME_BOTTOM,   -1)) &&
         -1 != (r = pApp->GetProfileInt(REG_SECTION_FRAME, REG_KEY_FRAME_RIGHT,    -1))) 
    {
        // restore the window's status
        pApp->m_nCmdShow = s;
        // restore the window's width and height
        cs.cx = r - l;
        cs.cy = b - t;
        // the following correction is needed when the taskbar is
        // at the left or top and it is not "auto-hidden"
//        RECT workArea;
//        SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);
//        l += workArea.left;
//        t += workArea.top;
        // make sure the window is not completely out of sight
        int max_x = GetSystemMetrics(SM_CXSCREEN) -
                        GetSystemMetrics(SM_CXICON);
        int max_y = GetSystemMetrics(SM_CYSCREEN) -
                        GetSystemMetrics(SM_CYICON);
        cs.x = min(l, max_x);
        cs.y = min(t, max_y);
    }

//    cs.lpszClass = pApp->GetWndClassName();
	if( !SECMDIFrameWnd::PreCreateWindow(cs) )
    {
		return FALSE;
    }
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	SECMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	SECMDIFrameWnd::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// Public operations

/////////////////////////////////////////////////////////////////////////////
// CMainFrame private operations
//------------------------------------------------------------------*
/**
 * Returns navigator object from the component manager.
 * The navigator object is stored with the id "Navigator" in the
 * component manager. The navigator is a docking tool.
 *
 * @return          reference to the CNavigator object or
 *                  NULL if no CNavigator object exists yet.
 *
 * @see             CEGetDockingTool
*/
CNavigator *CMainFrame::GetNavigator (bool bMustLive)
{
    return (dynamic_cast<CNavigator *> (CEGetDockingTool ("Navigator", bMustLive)));
}

CMsgWnd *CMainFrame::GetMsgView(bool bMustLive)
{
    return (dynamic_cast<CMsgWnd *> (CEGetDockingTool ("Message View", bMustLive)));
}

CWatchWindow *CMainFrame::GetWatch (bool bMustLive)
{
    return (dynamic_cast<CWatchWindow *> (CEGetDockingTool (CE_COMPMAN_WATCH, bMustLive)));
}

CPouVarSelWindow *CMainFrame::GetPouVarSel (bool bMustLive)
{
    return (dynamic_cast<CPouVarSelWindow *> (CEGetDockingTool (CE_COMPMAN_POUVARSEL, bMustLive)));
}

CBPViewWindow *CMainFrame::GetBPView (bool bMustLive)
{
    return (dynamic_cast<CBPViewWindow *> (CEGetDockingTool (CE_COMPMAN_BPVIEW, bMustLive)));
}

CXRefWindow *CMainFrame::GetXRef (bool bMustLive)
{
    return (dynamic_cast<CXRefWindow *> (CEGetDockingTool (CE_COMPMAN_XREF, bMustLive)));
}

//------------------------------------------------------------------*
/**
 * Initializes the navigator object after creation by the component
 * manager. Returns silently if the navigator has not been created.
 *
*/
void CMainFrame::InitNavigator()
{
    CNavigator *pNavigator;
    CRect rect;

    pNavigator = GetNavigator ();
    if (pNavigator == NULL)
        return;

	pNavigator->EnableDocking(CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT);
	DockControlBarEx(pNavigator, AFX_IDW_DOCKBAR_LEFT, 0, 0, (float)1.0, pNavigator->GetDockingHeight());
}

void CMainFrame::InitMsgView()
{
    CMsgWnd *pMsgWnd;
    CRect rect;

    pMsgWnd = GetMsgView ();
    if (pMsgWnd == NULL)
        return;

	pMsgWnd->EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);
	DockControlBarEx(pMsgWnd, AFX_IDW_DOCKBAR_BOTTOM, 0,0);
}

void CMainFrame::InitWatch()
{
    CWatchWindow*            pWatch;
    CRect                    rect;

    pWatch = GetWatch();
    if (pWatch == NULL)
        return;

    pWatch->EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);
	DockControlBarEx(pWatch, AFX_IDW_DOCKBAR_BOTTOM, 0, 0, (float)1.0, 0);
    ShowControlBar(pWatch, m_bShowWatch, FALSE);
   
    // Uncomment next line to have a "Forced" pane.
    //pWatch->AddForcedPane ();
}

void CMainFrame::InitBPView()
{
    CBPViewWindow*  pBPView;
    CRect           rect;

    pBPView = GetBPView();
    if (pBPView == NULL)
        return;

    pBPView->EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);
    DockControlBarEx(pBPView, AFX_IDW_DOCKBAR_BOTTOM, 0, 0, (float)1.0, 0);
    m_bShowBPView = FALSE;
    ShowControlBar(pBPView, m_bShowBPView, FALSE);
}

void CMainFrame::InitPouVarSel()
{
    CPouVarSelWindow *pVarSelWin;
    CRect rect;

    pVarSelWin = GetPouVarSel ();
    if (pVarSelWin == NULL)
        return;

	pVarSelWin->EnableDocking(CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT);
	DockControlBarEx(pVarSelWin, AFX_IDW_DOCKBAR_LEFT, 1, 1);
    m_bShowPouVarSel = FALSE;
    ShowControlBar(pVarSelWin, m_bShowPouVarSel, FALSE);
}

void CMainFrame::InitXRef()
{
    CXRefWindow *pXRefWin;
    CRect rect;

    pXRefWin = GetXRef();
    if (pXRefWin == NULL)
        return;

	pXRefWin->EnableDocking(CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT);
	DockControlBarEx(pXRefWin, AFX_IDW_DOCKBAR_LEFT, 1, 1);
    m_bShowXRef = FALSE;
    ShowControlBar(pXRefWin, m_bShowXRef, FALSE);
}

void CMainFrame::InitFindInFiles()
{
    CComPtr<IUnknown>           pTmp;
    CComPtr<ICEFindInFiles>     pIFindInFiles;

    if(!GetFindInFiles(&pIFindInFiles))
    {
        return;
    }

    // now add file extensions from KAD:
    CStringArray    astrTmp;
    if(!m_pFileInfo)
    {
        if(!LoadFileInfo())
        {
            return;
        }
    }
    m_pFileInfo->GetAllExtensions(astrTmp);

    HRESULT             hr;
    long                lNumExt = astrTmp.GetSize();
    long                lExt;
    CString             strExt;
    CGuid               tGuid;
    CKADFileExtInfo*    pExtInfo;

    // set filter categories
    //m_pFileInfo->GetAllTexts(astrTmp);
    for(lExt = 0; lExt < lNumExt; ++lExt)
    {
        strExt = astrTmp[lExt];
        pExtInfo = m_pFileInfo->GetFileExtInfo(strExt);
        ASSERT(pExtInfo);
        if(pExtInfo && pExtInfo->m_pEditorGUID && pExtInfo->m_bFindInFiles)
        {
            tGuid = *pExtInfo->m_pEditorGUID;
            CComBSTR    sEditGUID;
            CComBSTR    sExtension(strExt);
            hr = tGuid.ToBSTR(&sEditGUID);
            if(hr == S_OK)
            {
                pIFindInFiles->AddFileType(sExtension, sEditGUID);
            }
        }
    }
}

//------------------------------------------------------------------*
/**
 * This callback method has been registered with the state manager.
 * The method will be called upon frame creation and destruction
 * and for components which have the lifetime = LifetimeFrame and
 * are created or destroyed.
 *
 * @see             CCEStateManager
*/
void CMainFrame::OnStateChange (CEStateNotify stateNotify, unsigned int uiStage)
{
    CString strName; 

    switch(stateNotify)
    {
    case CECreateComponentNotify:
        {
            strName = CESMGetComponentName ();
            if (strName == _T ("Navigator")) 
            {
                InitNavigator ();
            }
            if (strName == _T ("Message View"))
            {
                InitMsgView ();
            }
            if (strName == CE_COMPMAN_WATCH)
            {
                InitWatch ();
            }
            if (strName == CE_COMPMAN_POUVARSEL)
            {
                InitPouVarSel();
            }
            if (strName == CE_COMPMAN_BPVIEW)
            {
                InitBPView();
            }
            if (strName == CE_COMPMAN_XREF)
            {
                InitXRef();
            }
            if (strName.Compare(CE_COMPMAN_FIF) == 0)
            {
                InitFindInFiles();
            }
        }
        break;
    case CEHandsOffComponentNotify:
        {
            CString strName = CESMGetComponentName ();
            if (CEIsDockingTool(strName))
            {
                /*  This is a HACK to avoid a crash within the 
                    stingray tools. There is a concurrency situation with the 
                    destruction of a docking tool based on SECControlBar and
                    OnIdleUpdateCmdUI of main frame caused by bugs in the stingray
                    administration of SECDockBar (not SECControlBar).
                */
                CRect rScreen;
                SECControlBar* pControl = CEGetDockingTool (strName);
                assert(pControl);

                if(pControl->GetSafeHwnd() != NULL)
                {
                    pControl->GetWindowRect(&rScreen);
                    ShowControlBar(pControl, FALSE, FALSE);
                    FloatControlBar(pControl,rScreen.TopLeft());
                }
            }
        }
        break;
    case CEPostOpenProjectNotify:
        m_bProjectLoaded = TRUE;
        {
            CComBSTR                sPrjPath;
            CComBSTR                sPrjName;
            CComPtr<IUnknown>       pTmp;
            CComQIPtr<ICEProjInfo>  pProjInfo;
            CWatchWindow*           pWatch;
            CRect                   rect;
            CString                 prjPath;
            HRESULT                 hr;
            CString                 strProjName;
            CFileStatus             tFileStatus;

            pWatch = GetWatch();
            ASSERT(pWatch);
            if (pWatch)
            {
                pTmp.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
                pProjInfo = pTmp;
                if(!pProjInfo)
                {
                    ASSERT (!"get project info IF failed");
                    return;
                }
                hr = pProjInfo->getProjectPath(&sPrjPath);
                assert(hr == S_OK);
                hr = pProjInfo->getProjectName(&sPrjName);
                prjPath = sPrjPath;
                pWatch->SetProjectPath(prjPath);
//                pWatch->Load();
                // determine whether project is read only
                m_strProjFilePathName.Format("%s\\%s.4cp", prjPath, (CString)sPrjName);
                if(CFile::GetStatus(m_strProjFilePathName, tFileStatus))
                {
                    if(tFileStatus.m_attribute & CFile::readOnly)
                    {
                        m_bProjectReadOnly = TRUE;
                    }
                }
            }
        }
        break;
    case CEPreCloseProjectNotify:
        {
            DoSettingsResetDocking();
//            CWatchWindow*            pWatch;
//            pWatch = GetWatch(false);
//            if(pWatch)
//            {
//                pWatch->Store();
//            }
        }
        m_bProjectLoaded = FALSE;
        break;
    case CEPostCloseProjectNotify:
        // reset window cascading
        if(m_hWndMDIClient)
        {
            ::SendMessage(m_hWndMDIClient, WM_MDICASCADE, NULL, NULL);
        }
        break;
    case CEFillSettingsNotify:
        SaveSettings();
        break;
    case CENewSettingsNotify:
        LoadSettings();
        break;
    case CETargetDebugConnectNotify:
        {
            CCEStateManager *pStateManager;

            pStateManager = CEGetStateManager ();
            if (pStateManager != NULL)
            {
                CTargetOnl* pTargetOnl;
                CEConnectAction connectAction;

                pTargetOnl = dynamic_cast<CTargetOnl*> (pStateManager->GetTarget());
                if(pTargetOnl)
                {

                    connectAction = pStateManager->GetConnectAction ();
                    if (connectAction == CEPostConnect)
                    {
                        if (!m_bDoNotAddToCheckList && m_DownloadGUIDCheckPCs.Find(pTargetOnl) == NULL)
                        {
                            m_DownloadGUIDCheckPCs.AddTail (pTargetOnl);
                        }
                    }
                    else if (connectAction == CEPreDisconnect)
                    {
                        POSITION pPosition;

                        pPosition = m_DownloadGUIDCheckPCs.Find (pTargetOnl);
                        if (pPosition != NULL)
                        {
                            m_DownloadGUIDCheckPCs.RemoveAt (pPosition);
                        }
                    }
                }
            }
        }
       break;
    case CEDownloadInfoNotify:
        m_bCheckDownloadGUIDs = true;
        break;
    case CEDownloadCompleteNotify:
        {
            CCEStateManager*    pStateMan = CEGetStateManager();
            CTargetSet*         pTargetSet = CEGetTargetSet("Project Targets", false);
            CString             strTargetName;
            CTargetOnlBase*     pTarget;

            ASSERT(pStateMan);
            ASSERT(pTargetSet);

            strTargetName = pStateMan->GetTargetName();
            pTarget = pTargetSet->GetTarget(strTargetName);

            DoPostDownloadStep(pTarget);
        }
        break;
    case CERemoveTargetNotify:
        UpdateAllTabFileViews();
        break;
    case CETargetLoginNotify:
        OnTargetLoginNotify();
        break;

    }
}


//------------------------------------------------------------------*
/**
 * load file info.
 *
 * @param           -
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CMainFrame::LoadFileInfo()
{
    CComPtr<IUnknown>   pKadManager;

    pKadManager.Attach(CEGetCOM(CE_COMPMAN_KADMAN));
    if(!pKadManager)
    {
        return FALSE;
    }
    m_pFileInfo = new CKADFileInfo;
    if(!m_pFileInfo)
    {
        ::AfxThrowMemoryException();
    }
    if(!m_pFileInfo->Load(pKadManager))
    {
        delete m_pFileInfo;
        m_pFileInfo = NULL;
        return FALSE;
    }
    m_tDocuManager.Init(m_pFileInfo);
    return TRUE;
}



void CMainFrame::SaveSettings()
{
    StoreDockingTools();
    // store window placement of project list view
    CCEMAINApp* pApp = dynamic_cast<CCEMAINApp *> (AfxGetApp());
    ASSERT(pApp);

    CSettings* pSettings = pApp->GetSettings();
    if (!pSettings)
    {
        return;
    }
    CCEMAINDoc* pDoc = pApp->GetProjectDoc();
    if(pDoc)
    {
        pDoc->SaveProjectListViewSettings(pSettings);
    }

    // and of open editor files
    CGenEdCont* pContainer = pApp->GetContainer();
    if(pContainer)
    {
        pContainer->SaveSettings(pSettings->m_containerSettings);
    }
}

void CMainFrame::LoadSettings()
{
    RestoreDockingTools();
    // restore window placement of project list view
    CCEMAINApp* pApp = dynamic_cast<CCEMAINApp *> (AfxGetApp());
    ASSERT(pApp);

    CSettings* pSettings = pApp->GetSettings();
    if (!pSettings)
    {
        return;
    }
    CCEMAINDoc* pDoc = pApp->GetProjectDoc();
    if(pDoc)
    {
        pDoc->LoadProjectListViewSettings(pSettings);
    }

    // and of open editor files
    CGenEdCont* pContainer = pApp->GetContainer();
    if(pContainer)
    {
        pContainer->LoadSettings(pSettings->m_containerSettings);
    }
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
//------------------------------------------------------------------*
/**
 * Message handler OnCreate will be called upon creation of the 
 * main frame window represented by the CMainFrame object. Avoid
 * creating components or windows here. Rather register the components
 * with the component manager that the manager can create the components
 * for you. Then add additional initialization code to the OnStateChange
 * callback method. The callback method will be called by the component
 * manager upon creation of your component.
 *
 * @param           lpCreateStruct - creation structure
 * @return          -1 - error , 0 - no error
 * 
 * @see             CCEStateManager, CCEComponentManager, OnStateChange()
*/
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CCEStateManager* pStateManager = NULL;
    //SECToolBarManager* pToolBarMgr = (SECToolBarManager *)m_pControlBarManager;

    CKADInfo*   pKADInfo = ((CCEMAINApp*)AfxGetApp())->GetKADInfoObject();
    if(pKADInfo)
    {
        UINT    nStrLen = _MAX_PATH;
        CString strImageBase;
        HRESULT hr = UTIL_GetInstallPath(strImageBase.GetBuffer(_MAX_PATH), &nStrLen);
        strImageBase.ReleaseBuffer();
        strImageBase += CE_IMAGE_PATH;

        CString strDesktopIcon = strImageBase + _T("\\") + pKADInfo->GetDesktopIcon();
        CString strAppTitle = pKADInfo->GetApplicationTitle();

        if(hr == S_OK && !strDesktopIcon.IsEmpty())
        {
            // set icon
            HICON   h_Icon;
            h_Icon = (HICON)::LoadImage(0, strDesktopIcon, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
            if(h_Icon)
            {
                SetIcon(h_Icon, FALSE);
            }
        }

        if(!strAppTitle.IsEmpty())
        {
            m_strTitle = strAppTitle;
            ::AfxSetWindowText(m_hWnd, m_strTitle);
        }
    }

    // activate drop files
    DragAcceptFiles();

   	m_tDropTarget.Register(this);
    m_tDropTarget.SetMainFrame(this);
    
    if (SECMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

//    InitToolbars();

    // Load toolbars for cool menu
    m_MenuMgr.Install(this);
//    m_MenuMgr.m_bAutoAccel = FALSE;
//    m_MenuMgr.LoadToolbars(m_pauiToolbars, sizeof (m_pauiToolbars) / sizeof (m_pauiToolbars[0]));
    m_MenuMgr.LoadToolbars(toolbars, sizeof (toolbars) / sizeof (toolbars[0]));

//    CreateKADToolbars();
	
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

//////////////////// Stingray ///////////////////////////////////////////////////////
//    pToolBarMgr->SetMenuInfo(6, IDR_MAINFRAME, IDR_CEMAINTYPE, IDR_CEMAINTYPE_CNTR_IP, IDR_GENEDTYPE, IDR_GENEDTYPE_CNTR_IP, IDR_PANELVIEW);
//	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
//		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
//		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
//	{
//		TRACE0("Failed to create toolbar\n");
//		return -1;      // fail to create
//	}
//////////////////// End Stingray ///////////////////////////////////////////////////////

    if (!m_wndReBar.Create(this, /*RBS_AUTOSIZE | */ RBS_BANDBORDERS))
    {
        TRACE0("Failed to create rebar\n");
        return -1;      // fail to create
    }

	// Have to clear after Create because Create always sets
	// this one if id is AFX_IDW_REBAR (as is by default).
	m_wndReBar.SetBarStyle (m_wndReBar.GetBarStyle () & ~CBRS_HIDE_INPLACE);
	if (!m_wndToolBar.CreateEx(this,TBSTYLE_FLAT |TBSTYLE_TRANSPARENT|TBSTYLE_TOOLTIPS) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
    m_wndToolBar.SetBarStyle (m_wndToolBar.GetBarStyle () & ~CBRS_HIDE_INPLACE | CBRS_FLYBY);
	m_wndReBar.AddBar(&m_wndToolBar);

	if (!m_wndViewToolBar.CreateEx(this,TBSTYLE_FLAT |TBSTYLE_TRANSPARENT|TBSTYLE_TOOLTIPS) ||
		!m_wndViewToolBar.LoadToolBar(IDR_VIEWTOOLBAR))
	{
		TRACE0("Failed to create view toolbar\n");
		return -1;      // fail to create
	}

    // Have to clear after Create because Create always sets
	// this one if id is AFX_IDW_TOOLBAR (as is by default).
	m_wndViewToolBar.SetBarStyle (m_wndViewToolBar.GetBarStyle () & ~CBRS_HIDE_INPLACE | CBRS_FLYBY);
	m_wndReBar.AddBar(&m_wndViewToolBar);


	EnableDocking(CBRS_ALIGN_ANY);

// Stingray tool bars
	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
//	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
//	DockControlBar(&m_wndToolBar);
//
//    pToolBarMgr->LoadToolBarResource(MAKEINTRESOURCE(IDR_MAINFRAME), 
//               MAKEINTRESOURCE(IDR_MAINFRAME));
//
//    pToolBarMgr->DefineDefaultToolBar(AFX_IDW_TOOLBAR, _T("Main"), NUMELEMENTS(mainButtons), mainButtons,
//        CBRS_ALIGN_ANY, AFX_IDW_DOCKBAR_TOP);
//
//    pToolBarMgr->DefineDefaultToolBar(AFX_IDW_TOOLBAR+5, _T("View"), NUMELEMENTS(viewButtons), viewButtons,
//        CBRS_ALIGN_ANY, AFX_IDW_DOCKBAR_TOP, AFX_IDW_TOOLBAR);
//
//    pToolBarMgr->SetDefaultDockState();
//
//    pToolBarMgr->EnableCoolLook();

//    m_pMenuBar->EnableDocking(CBRS_ALIGN_ANY);
//    DockControlBar(m_pMenuBar);

    // Clear text of edit position pane
    SetEditPositionText(0, 0, FALSE);

    pStateManager = CEGetStateManager ();
    if (pStateManager != NULL)
    {
        pStateManager->TriggerCreateFrame ();
    }

	return 0;
}


//------------------------------------------------------------------*
/**
 * create runtime toolbars from KAD.
 *
 *  call CreateKADToolbar() for each toolbar in KAD.
 *
 * @see             CreateKADToolbar()
*/
void CMainFrame::CreateKADToolbars()
{
    // get KAD main menu from application
    CKADInfo*           pKADInfo = ((CCEMAINApp*)AfxGetApp())->GetKADInfoObject();
    if(!pKADInfo)
    {
        return;
    }

    CKADToolbarInfo*    pKADToolbarInfo = pKADInfo->GetToolbarInfo();
    if(!pKADToolbarInfo)
    {
        return;
    }

    // run over all toolbars
    CKADToolbar*    pKADToolbar = pKADToolbarInfo->GetFirstToolbar();
    while(pKADToolbar)
    {
        CreateKADToolbar(pKADToolbar);
        pKADToolbar = pKADToolbarInfo->GetNextToolbar();
    }
}


//------------------------------------------------------------------*
/**
 * create toolbar from KAD description.
 *
 * @param           [in] pKADToolbar: pointer to KAD description of toolbar
 * @return          
 *
*/
void CMainFrame::CreateKADToolbar(CKADToolbar* pKADToolbar)
{
    CString strBitmapFile;
    CString strImageBase;
    UINT    uiID;
    UINT    nStrLen = _MAX_PATH;

    HRESULT hr = UTIL_GetInstallPath(strImageBase.GetBuffer(_MAX_PATH), &nStrLen);
    strImageBase.ReleaseBuffer();
    strImageBase += CE_IMAGE_PATH;

    CToolbarData*   pToolbarData = new CToolbarData;

    pToolbarData->SetToolbarId(ID_MAIN_TOOLBAR_BASE + m_iToolbarCount);
    pToolbarData->SetBackgroundColor(RGB(192,192,192));

    CKADToolbarItem*    pKADToolbarItem = pKADToolbar->GetFirstItem();
    while(pKADToolbarItem)
    {
        if(pKADToolbarItem->m_bSeparator)
        {
            pToolbarData->AddToolbarButton(ID_SEPARATOR, _T(""));
            pKADToolbarItem = pKADToolbar->GetNextItem();
            continue;
        }
        strBitmapFile = strImageBase + _T("\\") + pKADToolbarItem->m_strBitmapFile;
        uiID = ID_MAIN_MENU_BASE + pKADToolbarItem->m_iMainMenuItemNo;
        pToolbarData->AddToolbarButton(uiID, strBitmapFile);
        pKADToolbarItem = pKADToolbar->GetNextItem();
    }

    m_MenuMgr.AddRuntimeToolbar(pToolbarData);
    pKADToolbar->m_strId;

    if(!LoadKADToolBar(m_wndKADToolBar[m_iToolbarCount], pToolbarData))
	{
		TRACE0("Failed to create toolbar\n");
		return;      // fail to create
	}

    m_wndReBar.AddBar(&m_wndKADToolBar[m_iToolbarCount]);
    ++m_iToolbarCount;
}

//------------------------------------------------------------------*
/**
 * load KAD toolbar.
 *
 *  set toolbar button info.
 *
 * @param           [in] rtToolbar: MFC CToolBar object
 * @param           [in] pToolbarData: pointer to toolbar data object
 * @return          successful?
 *
*/
BOOL CMainFrame::LoadKADToolBar(CToolBar& rtToolbar, CToolbarData* pToolbarData)
{
    ASSERT(pToolbarData);
    HBITMAP hToolbarBitmap;

    int iNumButtons = pToolbarData->GetNumButtons();
    if(iNumButtons == 0)
    {
        return FALSE;
    }

    if(!rtToolbar.CreateEx(this, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT | TBSTYLE_TOOLTIPS))
    {
        return FALSE;
    }
    // set style info
    rtToolbar.SetBarStyle(rtToolbar.GetBarStyle () & ~CBRS_HIDE_INPLACE | CBRS_FLYBY);

    hToolbarBitmap = BuildToolbarBitmap(pToolbarData);
    if(!hToolbarBitmap)
    {
        return FALSE;
    }

    rtToolbar.SetBitmap(hToolbarBitmap);
    rtToolbar.SetSizes(CSize(TOOLBAR_BUTTON_WIDTH + 7, TOOLBAR_BUTTON_HEIGHT + 6), CSize(TOOLBAR_BUTTON_WIDTH, TOOLBAR_BUTTON_HEIGHT));

    rtToolbar.SetButtons(pToolbarData->m_ItemIdArray.GetData(), iNumButtons);
    return TRUE;
}

//------------------------------------------------------------------*
/**
 * build toolbar bitmap.
 *
 * @param           [in] pToolbarData: pointer to toolbar data
 * @return          handle to toolbar bitmap
 * @see             
*/
HBITMAP CMainFrame::BuildToolbarBitmap(CToolbarData* pToolbarData)
{
    CClientDC   tClientDC(this);    // draw context for client area of Mainframe
    CDC         tToolbarDC;         // draw context for toolbar bitmap
    CDC         tButtonDC;          // draw context for button bitmap
    CBitmap     tToolbarBitmap;     // toolbar bitmap
    CBitmap     tButtonBitmap;      // button bitmap
    CBitmap*    pOldBitmap;         // tmp var for select object
    CBitmap*    pOldBitmap2;        // tmp var for select object
    int         iNumButtons;        // number of buttons
    int         iWidth;             // width of toolbar bitmap
    int         iHeight;            // height of toolbar bitmap
    int         iXOffset = 0;       // pixel offset of button in toolbar
    CString     strBitmapFile;      // bitmap file name

    // determine width and height for toolbar bitmap
    iNumButtons = pToolbarData->GetNumButtons();
    iWidth = iNumButtons * TOOLBAR_BUTTON_WIDTH;
    iHeight = TOOLBAR_BUTTON_HEIGHT;

    tToolbarDC.CreateCompatibleDC(&tClientDC);
    tButtonDC.CreateCompatibleDC(&tClientDC);

    tToolbarBitmap.CreateCompatibleBitmap(&tClientDC, iWidth, iHeight);

    pOldBitmap = tToolbarDC.SelectObject(&tToolbarBitmap);

    // now run over all buttons and draw button bitmaps into the toolbar bitmap
    for(int iButton = 0; iButton < iNumButtons; ++iButton)
    {
        // load button bitmap
        strBitmapFile = pToolbarData->m_astrBitmapFileName[iButton];
        if(!strBitmapFile.IsEmpty())
        {
            HBITMAP hbmpButton = (HBITMAP)::LoadImage(0, strBitmapFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_LOADMAP3DCOLORS);
            if(hbmpButton)
            {
                // now draw button into toolbar bitmap
                tButtonBitmap.Attach(hbmpButton);
                pOldBitmap2 = tButtonDC.SelectObject(&tButtonBitmap);
                tToolbarDC.BitBlt(iXOffset, 0, TOOLBAR_BUTTON_WIDTH, TOOLBAR_BUTTON_HEIGHT, &tButtonDC, 0, 0, SRCCOPY);
                iXOffset += TOOLBAR_BUTTON_WIDTH;
                tButtonDC.SelectObject(pOldBitmap2);
                tButtonBitmap.Detach();
                ::DeleteObject(hbmpButton);
            }
        }
    }
    return((HBITMAP)tToolbarBitmap.Detach());
}


void CMainFrame::AddExploreToolbar()
{
	if (!m_wndIEToolBar.CreateEx(this,TBSTYLE_FLAT |TBSTYLE_TRANSPARENT|TBSTYLE_TOOLTIPS) ||
		!m_wndIEToolBar.LoadToolBar(IDR_IETOOLBAR))
	{
		TRACE0("Failed to create view toolbar\n");
		return;      // fail to create
	}

    // Have to clear after Create because Create always sets
	// this one if id is AFX_IDW_TOOLBAR (as is by default).
	m_wndIEToolBar.SetBarStyle (m_wndIEToolBar.GetBarStyle () & ~CBRS_HIDE_INPLACE | CBRS_FLYBY);
	m_wndReBar.AddBar(&m_wndIEToolBar);
}

void CMainFrame::AddHelpToolbar()
{
	if (!m_wndHelpToolBar.CreateEx(this,TBSTYLE_FLAT |TBSTYLE_TRANSPARENT|TBSTYLE_TOOLTIPS) ||
		!m_wndHelpToolBar.LoadToolBar(IDR_HELPTOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return;      // fail to create
	}
    m_wndHelpToolBar.SetBarStyle (m_wndHelpToolBar.GetBarStyle () & ~CBRS_HIDE_INPLACE | CBRS_FLYBY);
	m_wndReBar.AddBar(&m_wndHelpToolBar);
}

//------------------------------------------------------------------*
/**
 * Message handler OnDestroy will be called upon destruction of the 
 * main frame window represented by CMainFrame. Triggers the state
 * manager to inform the state manager about destruction of the 
 * frame. Calls super class OnDestroy method.
 *
 * @see  CCEStateManager::TriggerDestroyFrame
*/
void CMainFrame::OnDestroy() 
{
    CCEStateManager *pStateManager;

    pStateManager = CEGetStateManager ();
    if (pStateManager != NULL)
    {
        pStateManager->TriggerDestroyFrame ();
    }

    CCEMAINApp* pApp = dynamic_cast<CCEMAINApp *> (AfxGetApp());

    ASSERT (pApp != NULL);

    if (pApp != NULL)
    {
  	    // Get the window position	WINDOWPLACEMENT wp;
        WINDOWPLACEMENT wp;

        wp.length = sizeof(WINDOWPLACEMENT);
        GetWindowPlacement( &wp );
        pApp->WriteProfileInt( REG_SECTION_FRAME, REG_KEY_FRAME_SHOW, wp.showCmd);
        pApp->WriteProfileInt( REG_SECTION_FRAME, REG_KEY_FRAME_LEFT, wp.rcNormalPosition.left);
        pApp->WriteProfileInt( REG_SECTION_FRAME, REG_KEY_FRAME_RIGHT, wp.rcNormalPosition.right);
        pApp->WriteProfileInt( REG_SECTION_FRAME, REG_KEY_FRAME_TOP,  wp.rcNormalPosition.top);
        pApp->WriteProfileInt( REG_SECTION_FRAME, REG_KEY_FRAME_BOTTOM, wp.rcNormalPosition.bottom);	
    }

	SECMDIFrameWnd::OnDestroy();
}


void CMainFrame::OnClose ()
{
    CCEMAINApp* pApp = dynamic_cast<CCEMAINApp *> (AfxGetApp());
        
    if(!DisconnectAllTargets())
    {
        return;
    }

    ASSERT (pApp != NULL);

    if (pApp != NULL)
    {
  	    // Get the window position	WINDOWPLACEMENT wp;
        WINDOWPLACEMENT wp;

        wp.length = sizeof(WINDOWPLACEMENT);
        GetWindowPlacement( &wp );
        pApp->WriteProfileInt( REG_SECTION_FRAME, REG_KEY_FRAME_SHOW, wp.showCmd);
        pApp->WriteProfileInt( REG_SECTION_FRAME, REG_KEY_FRAME_LEFT, wp.rcNormalPosition.left);
        pApp->WriteProfileInt( REG_SECTION_FRAME, REG_KEY_FRAME_RIGHT, wp.rcNormalPosition.right);
        pApp->WriteProfileInt( REG_SECTION_FRAME, REG_KEY_FRAME_TOP,  wp.rcNormalPosition.top);
        pApp->WriteProfileInt( REG_SECTION_FRAME, REG_KEY_FRAME_BOTTOM, wp.rcNormalPosition.bottom);	
    }

    if (pApp != NULL)
    {
        pApp->SetShuttingDown(true);
    }

    pApp->OnProjectClose();

    // if user cancels, shutting down is set to false
    if(pApp->IsShuttingDown())
    {
    	SECMDIFrameWnd::OnClose();
    }

    // If shutdown does not complete (e.g. due to user cancelling shutdown
    // in a dialog box) the application has to be reenabled to e.g. accept
    // DDE request. Decision whether shutdown completed is based upon whether
    // the applications main window attribute has been cleared by the appli-
    // cation.

    if (pApp != NULL && pApp->m_pMainWnd != NULL)
    {
        pApp->SetShuttingDown(false);
    }
}

BOOL CMainFrame::DisconnectAllTargets()
{
    if(!m_bProjectLoaded)
    {
        return TRUE;
    }

    CTargetSet *pProjectTargets;

    pProjectTargets = CEGetTargetSet ("Project Targets");
    ASSERT (pProjectTargets != NULL);
    if (pProjectTargets == NULL)
    {
        return TRUE;
    }

    return(pProjectTargets->DisconnectAllTargets());
}




//------------------------------------------------------------------*
/**
 * Toggles the visibility of the navigator.
 *
 * @see             GetNavigator()
*/
void CMainFrame::OnToggleNavigator() 
{
    CNavigator * pNavigator = GetNavigator();
    assert(pNavigator);

    if(pNavigator)
    {
        StoreDockingTools();
        BOOL    bNavVisible = pNavigator->IsVisible();
        m_bShowNavigator = !bNavVisible;
        ShowControlBar(pNavigator, m_bShowNavigator, FALSE);

        if (m_bShowNavigator)
        {
            ::SetFocus(pNavigator->m_hWnd);
        }
    }
}

//------------------------------------------------------------------*
/**
 * Sets check if navigator is visible.
 *
 * @see             OnToggleNavigator()
*/
void CMainFrame::OnUpdateToggleNavigator(CCmdUI* pCmdUI) 
{
    CNavigator * pNavigator = GetNavigator(false);

    if(pNavigator)
    {
        pCmdUI->Enable(TRUE);
        pCmdUI->SetCheck(pNavigator->IsVisible());
    }
    else
    {
        pCmdUI->Enable(TRUE);
    }
}

//------------------------------------------------------------------*
/**
 * Sets check if message view is visible.
 *
 * @see             OnToggleMsgView()
*/
void CMainFrame::OnUpdateToggleMsgView(CCmdUI* pCmdUI) 
{
	CMsgWnd * pMsgWnd = GetMsgView(false);

    if(pMsgWnd)
    {
        pCmdUI->Enable(TRUE);
        pCmdUI->SetCheck(pMsgWnd->IsVisible());
    }
    else
    {
        pCmdUI->Enable(FALSE);
    }
}



void CMainFrame::OnToggleMsgView() 
{
	CMsgWnd * pMsgWnd = GetMsgView();
    assert(pMsgWnd);
	if(pMsgWnd)
    {
        StoreDockingTools();
        BOOL    bVisible = pMsgWnd->IsVisible();
        m_bShowMsgView = !bVisible;
        ShowControlBar(pMsgWnd, m_bShowMsgView, FALSE);

        if (m_bShowMsgView)
        {
            ::SetFocus(pMsgWnd->m_hWnd);
        }
    }
}



void CMainFrame::OnTogglePouSelection() 
{
	CWaitCursor tCur;	// SIS: may take a long time
	CPouVarSelWindow* pPouVarWnd = GetPouVarSel();
    ASSERT(pPouVarWnd);
    if(m_bViewFullScreen)
    {
        m_uiVisFlags |= F_DOCKING_POU;
    }
	if(pPouVarWnd)
    {
        StoreDockingTools();
        BOOL    bVisible = pPouVarWnd->IsVisible();
        m_bShowPouVarSel = !bVisible;

		// 10.11.04 SIS >>
		// trigger pou var selection to load current 
		if(m_bShowPouVarSel)
		{
			CString		strFileName;
			CGenEdCont* pCont = GetContainer();
			CGenEdDoc*  pDoc = pCont->GetActiveDocument();
			if(pDoc)
			{
				CString strPathName = pDoc->GetPathName();
				int		iFound = strPathName.ReverseFind(_T('\\'));
				strFileName = strPathName.Mid(iFound + 1);
			}
			SetVarSelectionHint(strFileName);
		}
		// 10.11.04 SIS <<

        ShowControlBar(pPouVarWnd, m_bShowPouVarSel, FALSE);

        if (m_bShowPouVarSel)
        {
            ::SetFocus(pPouVarWnd->m_hWnd);
        }
    }
	
}

void CMainFrame::OnUpdateTogglePouSelection(CCmdUI* pCmdUI) 
{
	CPouVarSelWindow* pPouVarWnd = GetPouVarSel(false);

    if(pPouVarWnd)
    {
        pCmdUI->Enable(TRUE);
        pCmdUI->SetCheck(pPouVarWnd->IsVisible());
        m_bShowPouVarSel = pPouVarWnd->IsVisible();
    }
    else
    {
        pCmdUI->Enable(FALSE);
    }
}



void CMainFrame::ShowPouVarSelection()
{
    CPouVarSelWindow* pPouVarWnd = GetPouVarSel();
    ASSERT(pPouVarWnd);
	if(pPouVarWnd)
    {
        StoreDockingTools();
        m_bShowPouVarSel = TRUE;
        ShowControlBar(pPouVarWnd, m_bShowPouVarSel, FALSE);

        if (m_bShowPouVarSel)
        {
            ::SetFocus(pPouVarWnd->m_hWnd);
        }
    }
}


void CMainFrame::OnToggleXRef() 
{
	CXRefWindow* pXRefWnd = GetXRef();
    ASSERT(pXRefWnd);
	if(pXRefWnd)
    {
        StoreDockingTools();
        BOOL    bVisible = pXRefWnd->IsVisible();
        m_bShowXRef = !bVisible;
        ShowControlBar(pXRefWnd, m_bShowXRef, FALSE);

        if (m_bShowXRef)
        {
            ::SetFocus(pXRefWnd->m_hWnd);
        }
    }
	
}

void CMainFrame::OnUpdateToggleXRef(CCmdUI* pCmdUI) 
{
	CXRefWindow* pXRefWnd = GetXRef(false);

    if(pXRefWnd)
    {
        pCmdUI->Enable(TRUE);
        pCmdUI->SetCheck(pXRefWnd->IsVisible());
        m_bShowXRef = pXRefWnd->IsVisible();
    }
    else
    {
        pCmdUI->Enable(FALSE);
    }
}



void CMainFrame::ShowXRef()
{
    CXRefWindow* pXRefWnd = GetXRef();
    ASSERT(pXRefWnd);
	if(pXRefWnd)
    {
        StoreDockingTools();
        m_bShowXRef = TRUE;
        ShowControlBar(pXRefWnd, m_bShowXRef, FALSE);

        if (m_bShowXRef)
        {
            ::SetFocus(pXRefWnd->m_hWnd);
        }
    }
}




void CMainFrame::OnToggleWatchView() 
{
	CWatchWindow* pWatch = GetWatch();
    ASSERT(pWatch);
	if(pWatch)
    {
        StoreDockingTools();
        BOOL    bVisible = pWatch->IsVisible();
        m_bShowWatch = !bVisible;
        ShowControlBar(pWatch, m_bShowWatch, FALSE);

        if (m_bShowWatch)
        {
            ::SetFocus(pWatch->m_hWnd);
        }
    }
	
}

void CMainFrame::OnUpdateToggleWatchView(CCmdUI* pCmdUI) 
{
	CWatchWindow* pWatch = GetWatch(false);

    if(pWatch)
    {
        pCmdUI->Enable(TRUE);
        pCmdUI->SetCheck(pWatch->IsVisible());
        m_bShowWatch = pWatch->IsVisible();
    }	
    else
    {
        pCmdUI->Enable(FALSE);
    }
}


void CMainFrame::OnToggleBPView() 
{
	CBPViewWindow* pBPView = GetBPView();
    ASSERT(pBPView);
	if(pBPView)
    {
        StoreDockingTools();
        BOOL    bVisible = pBPView->IsVisible();
        m_bShowBPView = !bVisible;
        ShowControlBar(pBPView, m_bShowBPView, FALSE);

        if (m_bShowBPView)
        {
            ::SetFocus(pBPView->m_hWnd);
        }
    }
	
}

void CMainFrame::OnUpdateToggleBPView(CCmdUI* pCmdUI) 
{
	CBPViewWindow* pBPView = GetBPView(false);
    
	if(pBPView)
    {
        pCmdUI->Enable(TRUE);
        pCmdUI->SetCheck(pBPView->IsVisible());
        m_bShowBPView = pBPView->IsVisible();
    }	
    else
    {
        pCmdUI->Enable(FALSE);
    }
}

void CMainFrame::OpenDocument() 
{
    CString strTitle;
    CString strFilter;
    CString strAllFilter;
    int iPos;

    // get filter for all files
    m_pFileInfo->GetFileFilterForAllFiles(strFilter);
    VERIFY(strAllFilter.LoadString(IDS_ALLFILTER));
	strFilter += strAllFilter;
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.*");
	strFilter += (TCHAR)'\0';   // last string

    strTitle.LoadString (IDS_FILE_OPEN_TITLE);


	for (iPos = 0; iPos < strFilter.GetLength (); ++iPos)
	{
		if (strFilter[iPos] == _T ('|'))
		{
			strFilter.SetAt (iPos, _T ('\0'));
		}
	}

    CFileDialog dialog (TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, NULL, this);
	dialog.m_ofn.lpstrTitle = strTitle;
	dialog.m_ofn.lpstrFilter = strFilter;

    if (dialog.DoModal () == IDOK)
    {
		ActivateFileView (dialog.GetPathName ());
    }
}


void CMainFrame::OpenNewDocument() 
{
	CGenEdCont *pContainer;

	pContainer = GetContainer ();
	if (pContainer == NULL)
	{
		return;
	}

	pContainer->CreateUntypedDocument();
}




void CMainFrame::Build(enum ETargetCommand cmd, const CString& strTarget)
{
    // first save all open files
    SaveAllFiles();

    HANDLE hMutex = CreateMutex(NULL, FALSE, _4CCG_MUTEX);
    if (NULL != hMutex)
    {
        if (WAIT_OBJECT_0 != WaitForSingleObject(hMutex, 0))
        {
            // Compiler is running
            AfxMessageBox(IDS_4CCG_RUNNING, MB_OK | MB_ICONEXCLAMATION);
            CloseHandle(hMutex);
            return;
        }
    }

	UpdateStatusBar(_T("Compiling..."));	// s-2069 22.03.05 SIS

    // get necessary information from project manager
    CList<CString, CString&> listTargets;
    CString strProjectName;
    CString strProjectPath;

    CComQIPtr<ICEProjInfo>  pICEProjectInfo;
    CComPtr<IUnknown>       pTmpInfo;
    HRESULT hr;

    pTmpInfo.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
    pICEProjectInfo = pTmpInfo;
    ASSERT(!(!pICEProjectInfo));
    if(!pICEProjectInfo)
    {
        TRACE("No project info available.\n");
        CloseHandle(hMutex);
		UpdateStatusBar(_T("Stop."));	// s-2069 22.03.05 SIS
        return;
    }

    CComBSTR sProjName;
    hr = pICEProjectInfo->getProjectName(&sProjName);
    ASSERT(hr==S_OK);
    if (hr != S_OK)
    {
        TRACE("Project name not available.\n");
        CloseHandle(hMutex);
		UpdateStatusBar(_T("Stop."));	// s-2069 22.03.05 SIS
        return;
    }
    strProjectName = sProjName;

    CComBSTR sProjPath;
    hr = pICEProjectInfo->getProjectPath(&sProjPath);
    ASSERT(hr==S_OK);
    if (hr != S_OK)
    {
        TRACE("Project name not available.\n");
        CloseHandle(hMutex);
		UpdateStatusBar(_T("Stop."));	// s-2069 22.03.05 SIS
        return;
    }
    strProjectPath = sProjPath;

    SAFEARRAY* psaTargets;
    hr = pICEProjectInfo->getAllTargetsAsIdPaths(&psaTargets);
    ASSERT(hr==S_OK);
    if (hr != S_OK)
    {
        TRACE("Target not available from project info.\n");
        CloseHandle(hMutex);
		UpdateStatusBar(_T("Stop."));	// s-2069 22.03.05 SIS
        return;
    }

    BSTR HUGEP *pbstr;
    BSTR bstr;
    unsigned long i;

    // Get a pointer to the elements of the array.
    hr = ::SafeArrayAccessData(psaTargets, (void HUGEP**)&pbstr);
    ASSERT(hr == S_OK);
    if (FAILED(hr))
    {
        CloseHandle(hMutex);
		UpdateStatusBar(_T("Stop."));	// s-2069 22.03.05 SIS
        return;
    }
    for (i = 0; i < ((psaTargets)->rgsabound[0]).cElements; i++)
    {
        CString str;
        bstr = pbstr[i];
        str = bstr;
        listTargets.AddTail(str);
    }

    ::SafeArrayUnaccessData(psaTargets);		
    ::SafeArrayDestroy(psaTargets);

   
    // fill target list option string for compiler    
    CString  strOption;    

    if (cmd==eTargetOne)
    {
        // find file name for target id
        CString strId = strTarget;
        // find id path in list
        POSITION pos = NULL;
        pos = listTargets.GetHeadPosition();
        while (pos)
        {
            CString strIdPath = listTargets.GetNext(pos);
            if (strIdPath.GetLength()>=strId.GetLength())
            {
                int i = strIdPath.ReverseFind(CE_XML_IDPATH_SEP_CHAR);
                CString strIdPathIdPart;
                if (i>0 && strIdPath.GetLength()>i)
                {
                    strIdPathIdPart = strIdPath.Mid(i+1);
                }
                else 
                {
                    strIdPathIdPart = strIdPath;
                }
                if (strId.Compare(strIdPathIdPart)==0)
                {
                    CString strFileName;
                    CComBSTR sIdPath = strIdPath;
                    CComBSTR sFileName;
                    hr = pICEProjectInfo->getFileNameFromIdPath(sIdPath, &sFileName);
                    ASSERT(hr==S_OK);
                    if (hr == S_OK)
                    {
                        strFileName = sFileName;
                        strOption += _T(" ") + strFileName;
                        pos = NULL;
                    }
                }
            }
        }
        if (strOption.IsEmpty())
        {
            strOption = strTarget;
        }
    }
    else if (cmd==eTargetSelect) // select targets
    {
        CTargetOnlBase* pTarget = NULL;
        CTargetSet* pTargetSet = CEGetTargetSet("Project Targets", false);
        if(!pTargetSet)
        {
            CloseHandle(hMutex);
            return;
        }
       
        CStringArray        astrTargetsSel;
        if(SelectTargets(astrTargetsSel, eAll, pTargetSet))
        { 
            int iNumTargets = astrTargetsSel.GetSize();
            for(int iTarg = 0; iTarg < iNumTargets; ++iTarg)
            {
                pTarget = pTargetSet->GetTarget(astrTargetsSel[iTarg]);
                if(pTarget)
                {
                    CString strId;
                    strId = pTarget->GetId();
                    // find id path in list
                    POSITION pos = NULL;
                    pos = listTargets.GetHeadPosition();
                    while (pos)
                    {
                        CString strIdPath = listTargets.GetNext(pos);
                        if (strIdPath.GetLength()>=strId.GetLength())
                        {
                            int i = strIdPath.ReverseFind(CE_XML_IDPATH_SEP_CHAR);
                            CString strIdPathIdPart;
                            if (i>0 && strIdPath.GetLength()>i)
                            {
                                strIdPathIdPart = strIdPath.Mid(i+1);
                            }
                            else 
                            {
                                strIdPathIdPart = strIdPath;
                            }
                            if (strId.Compare(strIdPathIdPart)==0)
                            {
                                CString strFileName;
                                CComBSTR sIdPath = strIdPath;
                                CComBSTR sFileName;
                                hr = pICEProjectInfo->getFileNameFromIdPath(sIdPath, &sFileName); ;
                                ASSERT(hr==S_OK);
                                if (hr == S_OK)
                                {
                                    strFileName = sFileName;
                                    strOption += _T(" ") + strFileName;
                                    pos = NULL;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else    // eTargetAll  ( add all targets )
    {
        // for all targets: pass no target at all, then the compiler does
        //  a rebuild all and can delete the gen folder and really rebuild all.
        strOption = _T(" ");
    }

    if (strOption.IsEmpty())
    {
        CloseHandle(hMutex);
        return;
    }

    // call compiler
    CMsgWnd* pMsgWnd = (dynamic_cast<CMsgWnd *> (CEGetDockingTool ("Message View", true)));
    if (pMsgWnd==NULL)
    {
        CloseHandle(hMutex);
        return;
    }

    CString  strProjectFileName = strProjectPath + _T("\\") + strProjectName 
                              + _T(".") + CE_PROJ_FILE_EXT;
    
    pMsgWnd->StartBuild(strProjectFileName, strOption);
    CloseHandle(hMutex); 
}

//------------------------------------------------------------------*
/**
 * get project info interface.
 *
 * @param           [in/out] pICEProjInfo: project info interface pointer
 * @return          successful?
*/
BOOL CMainFrame::GetProjectInfo(ICEProjInfo** ppICEProjInfo)
{
    CComQIPtr<ICEProjInfo>  pICEProjectInfo;
    CComPtr<IUnknown>       pTmpInfo;

    pTmpInfo.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
    pICEProjectInfo = pTmpInfo;
    ASSERT(!(!pICEProjectInfo));
    if(!pICEProjectInfo)
    {
        return FALSE;
    }
    *ppICEProjInfo = pICEProjectInfo.Detach();
    return TRUE;
}

//------------------------------------------------------------------*
/**
 * get find in files interface.
 *
 * @param           [in/out] pIFindInFiles: find in files interface pointer
 * @return          successful?
*/
BOOL CMainFrame::GetFindInFiles(ICEFindInFiles** ppIFindInFiles)
{
    CComQIPtr<ICEFindInFiles>  pIFindInFiles;
    CComPtr<IUnknown>       pTmp;

    pTmp.Attach(CEGetCOM(CE_COMPMAN_FIF));
    pIFindInFiles = pTmp;
    ASSERT(!(!pIFindInFiles));
    if(!pIFindInFiles)
    {
        return FALSE;
    }
    *ppIFindInFiles = pIFindInFiles.Detach();
    return TRUE;
}


void CMainFrame::BuildLibrary()
{
    // first save all open files
    SaveAllFiles();

    HANDLE hMutex = CreateMutex(NULL, FALSE, _4CCG_MUTEX);
    if (NULL != hMutex)
    {
        if (WAIT_OBJECT_0 != WaitForSingleObject(hMutex, 0))
        {
            // Compiler is running
            AfxMessageBox(IDS_4CCG_RUNNING, MB_OK | MB_ICONEXCLAMATION);
            CloseHandle(hMutex);
            return;
        }
    }

    CString strProjectName;
    CString strProjectPath;
    CString strLibName;
    CString strLibPath;
    CString strOption;
    CString strTargetExt;

    // get necessary information from project manager
    CComQIPtr<ICEProjInfo>  pICEProjectInfo;
    CComPtr<IUnknown>       pTmpInfo;
    HRESULT hr;

    pTmpInfo.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
    pICEProjectInfo = pTmpInfo;
    ASSERT(!(!pICEProjectInfo));
    if(!pICEProjectInfo)
    {
        TRACE("No project info available.\n");
        CloseHandle(hMutex);
        return;
    }

    CComBSTR sProjName;
    hr = pICEProjectInfo->getProjectName(&sProjName);
    ASSERT(hr==S_OK);
    if (hr != S_OK)
    {
        TRACE("Project name not available.\n");
        CloseHandle(hMutex);
        return;
    }
    strProjectName = sProjName;

    CComBSTR sProjPath;
    hr = pICEProjectInfo->getProjectPath(&sProjPath);
    ASSERT(hr==S_OK);
    if (hr != S_OK)
    {
        TRACE("Project name not available.\n");
        CloseHandle(hMutex);
        return;
    }
    strProjectPath = sProjPath;


    // get infos from settings
    CCEMAINApp* pApp = dynamic_cast<CCEMAINApp *> (AfxGetApp());
    ASSERT(pApp);

    CSettings* pSettings = pApp->GetSettings();
    ASSERT(pSettings);
    if (!pSettings)
    {
        return;
    }

    strLibName = pSettings->m_librarySettings.m_strLibName;
    strLibPath = pSettings->m_librarySettings.m_strLibPath;
    if (pSettings->m_librarySettings.m_bStandardLibPath)
    {
        CComBSTR sLibPath;
        hr = pICEProjectInfo->getLibraryPath(&sLibPath);
        if (hr == S_OK)
        {
            strLibPath = sLibPath;
        }
    }

    CString strTargetType = pSettings->m_librarySettings.m_strTargetType;

    if(strTargetType.IsEmpty())
    {
        ::AfxMessageBox(IDS_ERR_NO_TARGET_TYPE, MB_ICONEXCLAMATION);
        return;
    }

    CComBSTR sTargetType = strTargetType;
    CComBSTR sExt;
    hr = pICEProjectInfo->getExtForTargetType(sTargetType, &sExt);
    if (hr!=S_OK)
    {
        // todo error box
        ASSERT(hr==S_OK);
    }
    strTargetExt = sExt;

    // call compiler
    CMsgWnd* pMsgWnd = (dynamic_cast<CMsgWnd *> (CEGetDockingTool ("Message View", true)));
    if (pMsgWnd==NULL)
    {
        CloseHandle(hMutex);
        return;
    }

    CString  strProjectFileName = strProjectPath + _T("\\") + strProjectName 
                              + _T(".") + CE_PROJ_FILE_EXT;
    
    pMsgWnd->StartBuildLibrary(strProjectFileName, strLibPath, strLibName, strOption, strTargetExt);
    CloseHandle(hMutex); 
}



//local helper class to get all info for a compiler call
//TODO: use this class also in the CMainFrame::Build method, 
//the code in this helper class was mainly copied from CMainFrame::Build
//to be recycled in the CMainFrame::CompileFile method, Reason: the CompileFile
//does now also allow the user to select a target.
class CCallCompilerHelper
{
public:	
	CCallCompilerHelper(ICEProjInfo* pICEProjInfo)
		: m_pICEProjInfo (pICEProjInfo)
	{}

	~CCallCompilerHelper(){}

	ICEProjInfo*	m_pICEProjInfo;

	//these members are set on a sucessfull call to CollectInfo():
	CString			m_strProjectName;
    CString			m_strProjectPath;
	CString			m_strProjectFileAndPath;

    CStringArray	m_targetIdPaths;  //for all targets in target view
    CStringArray	m_targetNames;    //for all targets in target view
    CStringArray	m_targetFiles;    //for all targets in target view
    CStringArray	m_targetExt;      //for all targets in target view
    CStringArray	m_targetVersions; //for all targets in target view
	


	BOOL CollectInfo() //return FALSE if any error
	{
		HRESULT		hr;

		CComBSTR	sProjName;
		hr = m_pICEProjInfo->getProjectName(&sProjName);
		ASSERT(hr==S_OK);
		if (hr != S_OK)
		{
			TRACE("Project name not available.\n");
			return FALSE;
		}
		m_strProjectName = sProjName;

		CComBSTR sProjPath;
		hr = m_pICEProjInfo->getProjectPath(&sProjPath);
		ASSERT(hr==S_OK);
		if (hr != S_OK)
		{
			TRACE("Project name not available.\n");
			return FALSE;
		}
		m_strProjectPath = sProjPath;

		m_strProjectFileAndPath = 
			m_strProjectPath + _T("\\") + m_strProjectName + _T(".")CE_PROJ_FILE_EXT;


		SAFEARRAY* psaTargets;
		hr = m_pICEProjInfo->getAllTargetsAsIdPaths(&psaTargets);
		ASSERT(hr==S_OK);
		if (hr != S_OK)
		{
			TRACE("Target not available from project info.\n");
			return FALSE;
		}

		BSTR HUGEP *pbstr;
		BSTR bstr;
		unsigned long i;

		// Get a pointer to the elements of the array.
		hr = ::SafeArrayAccessData(psaTargets, (void HUGEP**)&pbstr);
		ASSERT(hr == S_OK);
		if (FAILED(hr))
			return FALSE;

		for (i = 0; i < ((psaTargets)->rgsabound[0]).cElements; i++)
		{
			CString str;
			bstr = pbstr[i];
			str = bstr;
			m_targetIdPaths.Add(str);

			CString  strExt;
			CComBSTR sTrgType, sTrgExt;
			hr = m_pICEProjInfo->getTypeOfTarget(bstr, &sTrgType);
			ASSERT(hr==S_OK);
			if(hr == S_OK)
			{
				hr = m_pICEProjInfo->getExtForTargetType(sTrgType, &sTrgExt);
				ASSERT(hr==S_OK);
				if(hr==S_OK)
					strExt = sTrgExt;
			}
			m_targetExt.Add(strExt);



			//get the target name part:
            int iLen = str.ReverseFind(CE_XML_IDPATH_SEP_CHAR);
            CString strTrgName;
            if (iLen>0 && str.GetLength()>iLen)
                strTrgName = str.Mid(iLen+1);
            else 
                strTrgName = str;

			m_targetNames.Add(strTrgName);


			CComBSTR sTrgName(strTrgName), sAddr;

			hr = m_pICEProjInfo->getAddressStringForTarget(sTrgName, &sAddr);
			ASSERT(hr==S_OK);
			if(hr==S_OK)
			{
				str = sAddr;
				TCHAR szFirmwareVersion[UTIL_MAX_FIRMWARE_VERSION_LEN+1];

				szFirmwareVersion[0] = 0;
				if(S_OK == UTIL_ParseControlString(
					str,
					NULL, // pszOPCServer 
					NULL, // pszCustomConnectGuid 
					NULL, // pszOPCInstName
					NULL, // pszCommChannel 
					NULL, // pszConnectPar
					NULL, // pszProtocol
					NULL, // pszProtocolPar
					szFirmwareVersion)
				  )
				{
					m_targetVersions.Add(szFirmwareVersion);
				}
				else
				{
					ASSERT(!"UTIL_ParseControlString failed");
					m_targetVersions.Add(_T(""));
				}
			}
			else
			{
				m_targetVersions.Add(_T(""));
			}

			CComBSTR sFile;
            hr = m_pICEProjInfo->getFileNameFromIdPath(bstr, &sFile);
            ASSERT(hr==S_OK);
            if (hr == S_OK)
			{
				str = sFile;
				m_targetFiles.Add(str);
			}
			else
			{
				m_targetFiles.Add(_T(""));
			}
		}

		::SafeArrayUnaccessData(psaTargets);		
		::SafeArrayDestroy(psaTargets);
		return TRUE;
	}



	//return: TRUE: user pressed OK, FALSE: user pressed CANCEL or any internal error
    BOOL SelectCheckFileTargetTypeAndVersion(
		CString* pStrTargetTypeAndVersion//return for example "4cwin:21050"
		)
	{
		int nTargets = m_targetIdPaths.GetSize();

		if(nTargets == 0)
		{
			//special case: no target in target view. In this case
			//use the build lib target <== TODO: is this a good idea (very intransparent for the user!)? 
			//CCEMAINApp* pApp = dynamic_cast<CCEMAINApp *> (AfxGetApp());
			//CSettings* pSettings = pApp ? pApp->GetSettings() : NULL;
			//ASSERT(pSettings);
			//if(pSettings)
			//	strCheckTarget = pSettings->m_librarySettings.m_strTargetType;

			//use empty target type name: the compiler will perform a IEC syntax check only
			//or if only one target installed this one is used:
			*pStrTargetTypeAndVersion = _T("");
			return TRUE;
		}

		CStringArray	astrTargetTypeAndVersion;
		CString         strHelp;

		//build a list of different targettype:version 
		for(int i=0; i<nTargets; i++)
		{
			strHelp  = m_targetExt.GetAt(i);
			if(!m_targetVersions.GetAt(i).IsEmpty())
			{
				strHelp += ':';
				strHelp += m_targetVersions.GetAt(i);
			}

			for(int j=astrTargetTypeAndVersion.GetSize()-1; j>=0; j--)
			{
				if(!astrTargetTypeAndVersion.GetAt(j).CompareNoCase(strHelp))
					break;
			}

			if(j<0) //no such targettype:version in list: add this one
				astrTargetTypeAndVersion.Add(strHelp);
		}

		if(astrTargetTypeAndVersion.GetSize()==1)
		{
			//don't need the dialog:
			*pStrTargetTypeAndVersion  = astrTargetTypeAndVersion.GetAt(0);
			return TRUE;
		}

		//use dialog:
		CMultiSelDlg    tTargetSel(NULL);
		strHelp.LoadString(IDS_TARGETTYPE_SELECTION);
		tTargetSel.Initialize(strHelp, astrTargetTypeAndVersion, TRUE);
		
		if(tTargetSel.DoModal() != IDOK)
			return FALSE;

		tTargetSel.GetSelection(astrTargetTypeAndVersion);

		ASSERT(astrTargetTypeAndVersion.GetSize()==1);
		if(astrTargetTypeAndVersion.GetSize()!=1)
			return FALSE;

		*pStrTargetTypeAndVersion = astrTargetTypeAndVersion.GetAt(0);

		return TRUE;
    }
};




void CMainFrame::CompileFile(CString strFileName)
{
    // first save all open files
    SaveAllFiles();

	
	ICEProjInfo* pICEProjInfo;
	GetProjectInfo(&pICEProjInfo);
	ASSERT(pICEProjInfo);
	if(!pICEProjInfo)
	{
		TRACE("No project info available.\n");
		return;
	}

    CString				strTargetTypeAndVersion;
	CCallCompilerHelper comhlp(pICEProjInfo);

    HANDLE hMutex = CreateMutex(NULL, FALSE, _4CCG_MUTEX);
    if (NULL != hMutex)
    {
        if (WAIT_OBJECT_0 != WaitForSingleObject(hMutex, 0))
        {
            // Compiler is running
            AfxMessageBox(IDS_4CCG_RUNNING, MB_OK | MB_ICONEXCLAMATION);
            CloseHandle(hMutex);
            return;
        }
    }

    // get necessary information from project manager
	if(!comhlp.CollectInfo())
	{
		CloseHandle(hMutex);
		return; //error: assert & trace already done inside function
	}

	if(!comhlp.SelectCheckFileTargetTypeAndVersion(&strTargetTypeAndVersion))
	{
		CloseHandle(hMutex);
		return; //user canceled (or error assert & trace already done inside function)
	}

    // call compiler
    CMsgWnd* pMsgWnd = (dynamic_cast<CMsgWnd *> (CEGetDockingTool ("Message View", true)));
	ASSERT(pMsgWnd);
    if (pMsgWnd)
		pMsgWnd->StartCompileFile(comhlp.m_strProjectFileAndPath, strTargetTypeAndVersion, strFileName);

    CloseHandle(hMutex); 
}


void CMainFrame::DownloadTarget(enum ETargetCommand cmd, CEDL_DOWNLD_TYPE dlType, 
                                const CString& strTarget, const CString& strConnectInfo, 
                                CEDL_DOWNLD_OPTIONS eOptions /*=DOWNLD_NO_OPTIONS*/)
{

    // check for running compiler (owns mutex!) 
    HANDLE hMutex = CreateMutex(0, FALSE, _4CCG_MUTEX);
    if (NULL != hMutex)
    {
        BOOL bDoReturn = FALSE;
        if (WAIT_OBJECT_0 != WaitForSingleObject(hMutex, 0))
        {
            // Compiler is running
            bDoReturn = TRUE;
            AfxMessageBox(IDS_4CCG_RUNNING, MB_OK | MB_ICONEXCLAMATION);
        }

        CloseHandle(hMutex);
        if (bDoReturn)
        {
            return;
        }
    }

    CTargetSet* pTargetSet = CEGetTargetSet("Project Targets", false);
    if(!pTargetSet)
    {
        return;
    }

    // collect target id to download
    CStringArray arrTargetIds;

    if (cmd==eTargetOne)
    {
        arrTargetIds.Add(strTarget);
    }
    else if (cmd==eTargetSelect)
    {
        BOOL bOk = FALSE;
        switch(dlType)
        {
        case CEDL_DOWNLD_ALL:
            bOk = SelectTargets(arrTargetIds, eAll, pTargetSet);
            break;
        case CEDL_DOWNLD_CHANGES:
            bOk = SelectTargets(arrTargetIds, eDownloadChanges, pTargetSet);
            break;
        }
        if(!bOk)
        {
            return;
        }
    }
    else  // eTargetAll
    {
        pTargetSet->GetAllTargetIds(arrTargetIds);      
    }

    CMsgWnd* pMsgWnd = (dynamic_cast<CMsgWnd *> (CEGetDockingTool ("Message View", true)));
   
    if (pMsgWnd)
    {
        if (arrTargetIds.GetUpperBound() == 0)
        {
            pMsgWnd->ShowTargetWindow(arrTargetIds[0]);   
        }
        else
        {
            pMsgWnd->ShowDownloadWindow();   
        }
    }
    

    CTargetOnlBase* pTargetBase;
    CTargetOnl* pTarget;
    CDownload* pDL;
    CString strTargetId;
    HRESULT hr = S_OK;
    CString strMessage;
    BOOL    bLast;
    ELoopAction eAction;
    BOOL bCompiled;
    BOOL bDownloaded;

    int iNumTargets = arrTargetIds.GetSize();
    for(int iTarg = 0; iTarg < iNumTargets; ++iTarg)
    {
        strTargetId = arrTargetIds[iTarg];
        bCompiled = FALSE;
        bDownloaded = FALSE;

        GetTargetCompileDownloadState(strTargetId, bCompiled, bDownloaded);
        if(!bCompiled)
        {
            CString strMessage;
            if(iTarg == iNumTargets - 1)
            {
                if(eOptions == DOWNLD_FLASH_ONLY)
                {
                    strMessage.Format(IDS_NOT_COMPILED_WARNING, strTargetId);
                    if(::AfxMessageBox(strMessage, MB_YESNO) == IDNO)
                    {
                        return;
                    }
                }
                else
                {
                    strMessage.Format(IDS_TARGET_NOT_COMPILED, strTargetId);
                    ::AfxMessageBox(strMessage);
                }
                return;
            }
            else
            {
                if(eOptions == DOWNLD_FLASH_ONLY)
                {
                    strMessage.Format(IDS_NOT_COMPILED_WARNING, strTargetId);
                    if(::AfxMessageBox(strMessage, MB_YESNO) == IDNO)
                    {
                        return;
                    }
                }
                else
                {
                    strMessage.Format(IDS_TARGET_NOT_COMPILED2, strTargetId);
                    if(::AfxMessageBox(strMessage, MB_YESNO) == IDNO)
                    {
                        return;
                    }
                }
                continue;
            }
        }


        pTargetBase = pTargetSet->GetTarget(strTargetId);
        if(pTargetBase)
        {
            pTarget = dynamic_cast<CTargetOnl*> (pTargetBase);
            if(pTarget)
            {
                // check if control address is up to date
                // and disconnect if not
                if (!strConnectInfo.IsEmpty())
                {
                    pTarget->SetControlAddress(strConnectInfo);
                }
                bLast = (iTarg == iNumTargets - 1);    
                eAction = CheckDownloadConnection(pTarget, pMsgWnd, bLast);
                if(eAction == eLABreak)
                {
                    break;
                }
                else if(eAction == eLAContinue)
                {
                    continue;
                }
                if(dlType == CEDL_DOWNLD_CHANGES || dlType == CEDL_DOWNLD_SELECT)
                {
                    if(!CheckTaskState(pTarget))
                    {
                        break;
                    }
                }

                if(pTarget->IsTargetConnectedExclusive() ||
                   (!pTarget->GetWaitForTargetConnection() && pTarget->IsConnectedExclusive()) )
                {
                    // now get downloader for target
                    pDL = pTargetSet->GetDownloader(strTargetId);
                    if(!pDL) 
                    {
                        assert(pDL);
                        continue;
                    }

                    hr = pDL->Create(dlType);

                    // downloader could not be created
                    if(FAILED(hr))
                    {
                        ShowDownloadTab();
                        pDL->Release();
                        if(bLast)
                        {
                            strMessage.LoadString(IDS_DL_ERROR);
                            ::AfxMessageBox(strMessage);
                            break;
                        }
                        else
                        {
                            strMessage.LoadString(IDS_DL_ERROR_CONTINUE);
                            if(::AfxMessageBox(strMessage, MB_YESNO) == IDYES)
                            {
                                continue;
                            }
                            else
                            {
                                break;
                            }
                        }
                    }

                    // try to do pre download step
                    // if error occurs -> continue;
                    hr = DoPreDownloadStep(pTarget);
                    if(hr == E_FAIL)
                    {
                        ShowDownloadTab();
                        pDL->Release();
                        continue;
                    }

                    // downloader is created -> now download
                    hr = pDL->Download(_T(""), eOptions);
                    if(hr != S_OK)
                    {
                        ShowDownloadTab();
                        if(iTarg == iNumTargets - 1)
                        {
                            strMessage.LoadString(IDS_DL_ERROR);
                            ::AfxMessageBox(strMessage);
                            break;
                        }
                        else
                        {
                            strMessage.LoadString(IDS_DL_ERROR_CONTINUE);
                            if(::AfxMessageBox(strMessage, MB_YESNO) == IDYES)
                            {
                                pDL->Release();
                                continue;
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                    pDL->Release();
                }
            }
            else
            {
                // target only of type CTargetOnlBase
                // no download connection possible
                strMessage.Format(IDS_NO_DOWNLOAD_POSSIBLE, strTargetId);
                ::AfxMessageBox(strMessage);
            }
        }
    }
}


//------------------------------------------------------------------*
/**
 * get target compile and download state.
 *
 * @param           crstrTargetId: target id (name)
 * @param           rbCompiled: target is already compiled? (out)
 * @param           rbDownloaded: target is already downloaded? (out)
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CMainFrame::GetTargetCompileDownloadState(const CString& crstrTargetId, BOOL& rbCompiled, BOOL& rbDownloaded)
{
    HRESULT     hr;
    int         iCompiled;
    int         iDownloaded;
    CString     strSemaphoreFile;
    CString     strProjPath;
    CComBSTR    sProjPath;

    rbCompiled = rbDownloaded = FALSE;

    CComPtr<ICEProjInfo> pICEProjInfo; 
    GetProjectInfo(&pICEProjInfo);

    if(!pICEProjInfo)
    {
        return FALSE;
    }

    hr = pICEProjInfo->getProjectPath(&sProjPath);
    if(hr != S_OK)
    {
        return FALSE;
    }
    strProjPath = sProjPath;
    strSemaphoreFile.Format("%s\\gen\\project.sem", strProjPath);

    iCompiled = GetPrivateProfileInt("compiled", crstrTargetId, 0, strSemaphoreFile);
    if(iCompiled != 0)
    {
        rbCompiled = TRUE;
    }
    
    iDownloaded = GetPrivateProfileInt("downloaded", crstrTargetId, 0, strSemaphoreFile);
    if(iDownloaded != 0)
    {
        rbDownloaded = TRUE;
    }

    return TRUE;
}


//------------------------------------------------------------------*
/**
 * do pre download step.
 *
 * @param           [in] pTarget: pointer to target online object
 * @return          S_OK, S_FALSE or E_FAIL
 * @exception       -
 * @see             
*/
HRESULT CMainFrame::DoPreDownloadStep(CTargetOnlBase* pTarget)
{
    // get addon action interface
    HRESULT                     hr;
    CGuid                       tGuid;

    CKADTargetData* pTargetData = pTarget->GetKADTargetData();
    if(!pTargetData->GetPreDLStepGuid(tGuid))
    {
        return S_FALSE;
    }

    CComQIPtr<ICEProjInfo>  pICEProjectInfo;
    CComPtr<IUnknown>       pTmpInfo;

    pTmpInfo.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
    pICEProjectInfo = pTmpInfo;
    
    if(!pICEProjectInfo)
    {
        TRACE("No project info available.\n");
        return E_FAIL;
    }

    CComPtr<IUnknown>           pTmp;
    CComQIPtr<ICEAddonAction>   pAddOnIF;
    CString                     strGuid = tGuid;

    CComBSTR    sIdPath(pTarget->GetIdPath());
    CComBSTR    sSourceFile;

    hr = pICEProjectInfo->getFileNameFromIdPath(sIdPath, &sSourceFile);
    ASSERT(hr==S_OK);
    if(hr != S_OK)
    {
        return E_FAIL;
    }

    pTmp.Attach(CEGetCOMByCLSID(strGuid, true));

    pAddOnIF = pTmp;
    if(!pAddOnIF)
    {
        return S_FALSE;
    }

    // call do action
    CString     strAdditionalInfo;
    CComBSTR    sMethodName(CE_PREDL_STEP_DISP_ID);
    CComBSTR    sId(pTarget->GetId());
    CComBSTR    sAdditionalInfo(strAdditionalInfo);

    hr = pAddOnIF->doAction(sMethodName, sSourceFile, sId, pICEProjectInfo, sAdditionalInfo);
    return hr;
}


//------------------------------------------------------------------*
/**
 * do post download step.
 *
 * @param           [in] pTarget: pointer to target online object
 * @return          S_OK, S_FALSE or E_FAIL
 * @exception       -
 * @see             
*/
HRESULT CMainFrame::DoPostDownloadStep(CTargetOnlBase* pTarget)
{
    // get addon action interface
    HRESULT                     hr;
    CGuid                       tGuid;

    CKADTargetData* pTargetData = pTarget->GetKADTargetData();
    if(!pTargetData->GetPostDLStepGuid(tGuid))
    {
        return S_FALSE;
    }

    CComQIPtr<ICEProjInfo>  pICEProjectInfo;
    CComPtr<IUnknown>       pTmpInfo;

    pTmpInfo.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
    pICEProjectInfo = pTmpInfo;
    
    if(!pICEProjectInfo)
    {
        TRACE("No project info available.\n");
        return E_FAIL;
    }

    CComPtr<IUnknown>           pTmp;
    CComQIPtr<ICEAddonAction>   pAddOnIF;
    CString                     strGuid = tGuid;

    CComBSTR    sIdPath(pTarget->GetIdPath());
    CComBSTR    sSourceFile;

    hr = pICEProjectInfo->getFileNameFromIdPath(sIdPath, &sSourceFile);
    ASSERT(hr==S_OK);
    if(hr != S_OK)
    {
        return E_FAIL;
    }

    pTmp.Attach(CEGetCOMByCLSID(strGuid, true));

    pAddOnIF = pTmp;
    if(!pAddOnIF)
    {
        return S_FALSE;
    }

    // call do action
    CString     strAdditionalInfo;
    CComBSTR    sMethodName(CE_POSTDL_STEP_DISP_ID);
    CComBSTR    sId(pTarget->GetId());
    CComBSTR    sAdditionalInfo(strAdditionalInfo);

    hr = pAddOnIF->doAction(sMethodName, sSourceFile, sId, pICEProjectInfo, sAdditionalInfo);
    return hr;
}

//------------------------------------------------------------------*
/**
 * check download connection.
 *
 *  also shows message boxes for user information and decisions.
 *
 * @param           [in] pTarget: pointer to target online object
 * @param           [in] pMsgWnd: pointer to message window
 * @param           [in] bLast: last target in loop?
 * @return          loop action: eLAOk, eLABreak or eLAContinue
 * @exception       -
 * @see             -
*/
ELoopAction CMainFrame::CheckDownloadConnection(CTargetOnl* pTarget, CMsgWnd* pMsgWnd, BOOL bLast /*=TRUE*/)
{
    HRESULT hrTmp;
    CString strMessage;
    CString strTargetId = pTarget->GetId();

    // if connected, check if connected address and address is the same
    if(pTarget->IsConnected() && pTarget->GetControlAddress().CompareNoCase(pTarget->GetConnectedControlAddress()) != 0)
    {
        strMessage.Format(IDS_DL_DISCONN_OLD, strTargetId);
        if(::AfxMessageBox(strMessage, MB_YESNO) == IDNO)
        {
            return eLABreak;
        }
        hrTmp = S_OK;
        if(pTarget->IsConnectedExclusive())
        {
            hrTmp = pTarget->DisconnectExclusive();
        }
        if(hrTmp == S_OK)
        {
            hrTmp = pTarget->Disconnect();
        }
        if(hrTmp != S_OK)
        {
            if(bLast)
            {
                strMessage.Format(IDS_DL_ERR_DISCONN, strTargetId);
                ::AfxMessageBox(strTargetId);
                return eLABreak;
            }
            else
            {
                strMessage.Format(IDS_DL_ERR_DISCONN_CONTINUE, strTargetId);
                if(::AfxMessageBox(strMessage, MB_YESNO) == IDYES)
                {
                    return eLAContinue;
                }
                else
                {
                    return eLABreak;
                }
            }
        }
    }

    // check debug connection
    if(!pTarget->IsConnectedExclusive())
    {
        strMessage.Format(IDS_ENABLEDEBUG, strTargetId, pTarget->GetUserControlAddress());
        if(::AfxMessageBox(strMessage, MB_YESNO) == IDNO)
        {
            return eLABreak;
        }
        m_bDoNotAddToCheckList = true;
        hrTmp = EstablishDebugConnection(pTarget, pMsgWnd);
        m_bDoNotAddToCheckList = false;
        if(hrTmp != S_OK)
        {
            if(bLast)
            {
                strMessage.Format(IDS_DL_ERR_NO_CONN, strTargetId);
                ::AfxMessageBox(strMessage);
                return eLABreak;
            }
            else
            {
                strMessage.Format(IDS_DL_ERR_NO_CONN_CONTINUE, strTargetId);
                if(::AfxMessageBox(strMessage, MB_YESNO) == IDYES)
                {
                    return eLAContinue;
                }
                else
                {
                    return eLABreak;
                }
            }
        }
    }
    return eLAOk;
}


//------------------------------------------------------------------*
/**
 * check task state.
 *
 * @param           [in] pTarget: pointer to target online object
 * @return          continue with download?
 * @exception       -
 * @see             -
*/
BOOL CMainFrame::CheckTaskState(CTargetOnl* pTarget)
{
    ASSERT(pTarget);
    if(pTarget && pTarget->IsTaskInBreakpoint())
    {
        if(::AfxMessageBox(IDS_WARN_DL_DURING_BPHOLD, MB_YESNO) == IDNO)
        {
            return FALSE;
        }
    }
    return TRUE;
}


void CMainFrame::ShowDownloadTab()
{
    CMsgWnd*        pMsgWnd = (dynamic_cast<CMsgWnd *> (CEGetDockingTool ("Message View", true)));
    if(pMsgWnd)
    {
        pMsgWnd->ShowDownloadWindow();
    }
}


// helper function for establishing an exclusive debug connection
HRESULT CMainFrame::EstablishDebugConnection(CTargetOnl* pTarget, CMsgWnd *pMessageWindow)
{
    CString strMessage;

    ASSERT(pTarget != NULL);
    if(pTarget == NULL)
        return(E_FAIL);

    ASSERT(pMessageWindow != NULL);
    if(!pMessageWindow)
    {
        return E_FAIL;
    }

    if(pTarget->IsTargetConnectedExclusive() ||
       (pTarget->GetWaitForTargetConnection()==FALSE && pTarget->IsConnectedExclusive()) )
    {
        return S_OK;
    }
    else
    {
        pMessageWindow->ConnectTargetExclusive(pTarget->GetId());
        if(pTarget->IsTargetConnectedExclusive() ||
           (pTarget->GetWaitForTargetConnection()==FALSE && pTarget->IsConnectedExclusive()) )
        {
            return S_OK;
        }
    }
    return E_FAIL;
}


void CMainFrame::DoSettingsResetDocking()
{
    CCEMAINApp* pApp = dynamic_cast<CCEMAINApp *> (AfxGetApp());
    ASSERT(pApp);

    CSettings* pSettings = pApp->GetSettings();
    if (!pSettings)
    {
        return;
    }

    pSettings->ResetDocking();
    RestoreDockingTools();
}


  
void CMainFrame::DoSettingsLibrary()
{
    CLibrarySettingsDialog dlgLibSettings;

    dlgLibSettings.DoModal();
}


void CMainFrame::DoSettingsEngineering()
{
    CEngineeringSettingsDialog dlgEngSettings;

    dlgEngSettings.DoModal();
}


void CMainFrame::DoSettingsPassword()
{
    CPasswordSettingsDialog dlgPwdSettings;

    dlgPwdSettings.DoModal();
}






void CMainFrame::NotifyCompileReady()
{
	CWaitCursor tWaitCur;									// s-2069 22.03.05 SIS

    // notify project manager to update instance view
    CComQIPtr<ICEProjectManager>   pICEProjectMan;
    CComPtr<IUnknown>       pTmpInfo;

    pTmpInfo.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
    pICEProjectMan = pTmpInfo;
    ASSERT(!(!pICEProjectMan));
    if(!pICEProjectMan)
    {
        TRACE("No project manager available.\n");
    }
    else
    {
        pICEProjectMan->CompileReady();
    }

	UpdateStatusBar(_T("Compile finished."));				// s-2069 22.03.05 SIS

    // notify xref tool that compilation has been ready
    CXRefWindow* pXRef = NULL;
    pXRef = GetXRef();
    if (!pXRef)
    {
        return;
    }
    CXRefCtrlWrapper* pXRefWrapper = NULL;
    pXRefWrapper = pXRef->GetWrapper();
    if (pXRefWrapper)
    {
        UpdateStatusBar(_T("Linking Cross Reference..."));	// s-2069 22.03.05 SIS
        pXRefWrapper->LinkXRef();
    }
    UpdateStatusBar(_T("Updating views..."));				// s-2069 22.03.05 SIS
    UpdateAllTabFileViews();
    UpdateStatusBar(_T("Done."));							// s-2069 22.03.05 SIS
}




//------------------------------------------------------------------*
/**
 * OpenURL
 *
 * @param           strUrl: the url with special syntax:
 *                          
 * @return          
 * @exception       -
 * @see             
*/
void CMainFrame::OpenURL(const CString& strUrl)
{
    CCEMAINApp *pApp = (CCEMAINApp *)AfxGetApp();
    ASSERT(pApp);
    if (pApp == NULL)
    {
        return;
    }
   
    int     iPos;
    CString strScheme;

    iPos = strUrl.Find(_T(':'));
    if (iPos != -1)
    {
        strScheme = strUrl.Left(iPos);
    }

    // ATCM:
    if (strScheme.CompareNoCase(CONSOLE_4C_URL_SCHEME) == 0)
    {
        CString strPath = strUrl.Mid(iPos+1);
        CCEFileHandler* pFileHandler;
        CGenEdDoc* pDocument;

        pFileHandler = pApp->m_pFileHandler;
        ASSERT(pFileHandler!=NULL);
        if (!pFileHandler)
        {
            return;
        }

        pFileHandler->ShowFile(strPath, pDocument);
        return;
    }

    // ATCMFile:
    if (strScheme.CompareNoCase(CONSOLE_4CFILE_URL_SCHEME) == 0)
    {
        CString strFullUrl;
        CString strPath = strUrl.Mid(iPos+1);

        CComBSTR                 sPrjPath;
        CComPtr<IUnknown>        pTmp;
        CComQIPtr<ICEProjInfo>   pProjInfo;
        CString                  prjPath;
        HRESULT                  hr;

        pTmp.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
        pProjInfo = pTmp;
        if(!pProjInfo)
        {
            ASSERT (!"get project info IF failed");
            return;
        }
        hr = pProjInfo->getProjectPath(&sPrjPath);
        ASSERT(hr == S_OK);
        prjPath = sPrjPath;
        
        if (FC_StringIsAbsPath(strPath)==0)
        {
            strPath = prjPath + _T("\\") + strPath;
        }

        strFullUrl = _T("file:") + strPath;
        CreatePanelView(strFullUrl);
        return;
    }

    // else just open panel:
    if (!strUrl.IsEmpty())
    {
        CreatePanelView(strUrl);
    }
}




BOOL CMainFrame::CreatePanelView(const CString& strName) 
{
    CCEMAINApp *pApp = (CCEMAINApp *)AfxGetApp();
    ASSERT(pApp);

    if(pApp != NULL) {
        if(pApp->m_pPanelTemplate != NULL) {
            return(pApp->m_pPanelTemplate->OpenDocumentFile(strName) != NULL);
        }
    }
    return(FALSE);
}

void CMainFrame::OnUpdateFrameMenu(HMENU hMenuAlt)
{
    
    SECMDIFrameWnd::OnUpdateFrameMenu(hMenuAlt);
    OnUpdateMainMenu(0, 0);
}

//------------------------------------------------------------------*
/**
 * update main resources.
 *
 * @param           [in] lParam, wParam: not used
 *
*/
void CMainFrame::OnUpdateMainMenu(LPARAM lParam, WPARAM wParam)
{
    CMenu*  pMainMenu = GetMenu();
    if(!pMainMenu)
    {
        return;
    }

    // get KAD main menu from application
    CKADInfo*       pKADInfo = ((CCEMAINApp*)AfxGetApp())->GetKADInfoObject();
    CKADMainMenu*   pKADMainMenu = pKADInfo->GetMainMenu();
    if(!pKADMainMenu)
    {
        return;
    }

    CKADMenu*   pKADSubMenu;
    CMenu*      pSubMenu = NULL;
    int         iInsertPos;
    int         iKADMenu;
    int         iKADMenuCount;
    CString     strMenuId;
    int         iDummyPos;
    TAccelList  tAccelList;
    int         iSubMenuIdx;

    // first run over all adapt menus
    iKADMenuCount = pKADMainMenu->GetAdaptMenuCount();
    for(iKADMenu = 0; iKADMenu < iKADMenuCount; ++iKADMenu)
    {
        pKADSubMenu = pKADMainMenu->GetAdaptMenu(iKADMenu);
        strMenuId = pKADSubMenu->m_strID;

        iSubMenuIdx = FindSubmenuByName(pMainMenu, strMenuId, pSubMenu);
        if(pSubMenu)
        {
            // find insert position
            iInsertPos = FindMenuItemByName(pSubMenu, _T("..."));
            if(iInsertPos >= 0)
            {
                iDummyPos = AddMainSubMenu(pSubMenu, pKADSubMenu, pKADMainMenu, tAccelList, iInsertPos);
                pSubMenu->RemoveMenu(iDummyPos, MF_BYPOSITION);
                // if no menu items were added or insert position was last pos
                // remove preceeding separator
                if((iDummyPos == iInsertPos && iDummyPos > 0) || (UINT)iDummyPos == pSubMenu->GetMenuItemCount())
                {
                    MENUITEMINFO    tMenuItemInfo;
                    ::memset(&tMenuItemInfo, 0, sizeof(tMenuItemInfo));
                    tMenuItemInfo.cbSize = sizeof(tMenuItemInfo);
                    tMenuItemInfo.fMask = MIIM_TYPE;
                    if(pSubMenu->GetMenuItemInfo(iDummyPos - 1, &tMenuItemInfo, TRUE) &&
                        tMenuItemInfo.fType & MFT_SEPARATOR)
                    {
                        pSubMenu->RemoveMenu(iDummyPos - 1, MF_BYPOSITION);
                    }
                }
            }
            // special case of help submenu:
            else
            {
                iInsertPos = FindMenuItemByName(pSubMenu, _T("ATCMControl Engineering Help"));
                if(iInsertPos >= 0)
                {
                    CMenu*  pSubHelpMenu = pSubMenu->GetSubMenu(iInsertPos);

                    iInsertPos = FindMenuItemByName(pSubHelpMenu, _T("..."));
                    if(iInsertPos >= 0)
                    {
                        iDummyPos = AddMainSubMenu(pSubHelpMenu, pKADSubMenu, pKADMainMenu, tAccelList, iInsertPos);
                        pSubHelpMenu->RemoveMenu(iDummyPos, MF_BYPOSITION);
                    }
                }
            }
            if(pSubMenu->GetMenuItemCount() == 0)
            {
                pMainMenu->RemoveMenu(iSubMenuIdx, MF_BYPOSITION);
            }
        }
    }

    // now add all additional menus
    int iSubMenuInsertPos;
    iSubMenuInsertPos = FindMenuItemByName(pMainMenu, _T("..."));
    if(iSubMenuInsertPos < 0)
    {
        DrawMenuBar();
        return;
    }

    iKADMenuCount = pKADMainMenu->GetAddMenuCount();
    for(iKADMenu = 0; iKADMenu < iKADMenuCount; ++iKADMenu)
    {
        pKADSubMenu = pKADMainMenu->GetAddMenu(iKADMenu);
        ASSERT(pKADSubMenu);
        if(pKADSubMenu && pKADSubMenu->GetItemCount() > 0)
        {
            CMenu tMenu;
            if(tMenu.CreatePopupMenu())
            {
                AddMainSubMenu(&tMenu, pKADSubMenu, pKADMainMenu, tAccelList);
                strMenuId = pKADSubMenu->m_strTitle;
                // insert menu
                if(pMainMenu->InsertMenu(iSubMenuInsertPos, MF_BYPOSITION | MF_POPUP, (UINT)tMenu.GetSafeHmenu(), strMenuId))
                {
                    ++iSubMenuInsertPos;
                    // detach local menu handle, for it is attached to main menu now
                    tMenu.Detach();
                }
            }
        }
    }
    // now delete insert position dummy
    pMainMenu->RemoveMenu(iSubMenuInsertPos, MF_BYPOSITION);

    if(!m_bKADAccelLoaded)
    {
        AppendKADAccelerators(tAccelList);
        //CreateKADAcceleratorTable(tAccelList);
        m_bKADAccelLoaded = TRUE;
    }
}


//------------------------------------------------------------------*
/**
 * append accelerators
 *
 * @param           [in] rtAccelList: list of accelerator structures
 * @return          -
 * @exception       -
 * @see             
*/
void CMainFrame::AppendKADAccelerators(TAccelList& rtAccelList)
{
    // append accelerators from KAD
    // get number of accelerators
    // version 1: get all accelerators and merge with KAD
    int iNumAccel = 0;
    int iNumNewAccel = rtAccelList.GetCount();
    LPACCEL paAccel = NULL;
    ACCEL   tAccel;

    iNumAccel = ::CopyAcceleratorTable(m_hAccelTable, NULL, iNumAccel);

    if(iNumAccel > 0 && iNumNewAccel > 0)
    {
        int iCopiedAccel;

        paAccel = new ACCEL[iNumAccel + iNumNewAccel];

        iCopiedAccel = ::CopyAcceleratorTable(m_hAccelTable, paAccel, iNumAccel);
        if(iCopiedAccel == iNumAccel)
        {
            // now add KAD accelerators from list
            POSITION    pos = rtAccelList.GetHeadPosition();
            while(pos)
            {
                tAccel = rtAccelList.GetNext(pos);
                paAccel[iNumAccel++] = tAccel;
            }
        }
        // now delete old accelerator table
        BOOL bRet = ::DestroyAcceleratorTable(m_hAccelTable);
        ASSERT(bRet);
        // and create new one
        m_hAccelTable = ::CreateAcceleratorTable(paAccel, iNumAccel);
        delete [] paAccel;
    }
}

//------------------------------------------------------------------*
/**
 * create KAD accelerator table.
 *
 * @param           [in] rtAccelList: list of accelerator structures
 * @return          
 * @exception       -
 * @see             
*/
void CMainFrame::CreateKADAcceleratorTable(TAccelList& rtAccelList)
{
    int iNumNewAccel = rtAccelList.GetCount();
    LPACCEL paAccel = NULL;
    ACCEL   tAccel;

    if(iNumNewAccel > 0)
    {
        paAccel = new ACCEL[iNumNewAccel];
        // now add KAD accelerators from list
        int         iNumAccel = 0;
        POSITION    pos = rtAccelList.GetHeadPosition();
        while(pos)
        {
            tAccel = rtAccelList.GetNext(pos);
            paAccel[iNumAccel++] = tAccel;
        }
        m_hKADAccel = ::CreateAcceleratorTable(paAccel, iNumNewAccel);
        ASSERT(m_hKADAccel);
        delete [] paAccel;
    }
}

//------------------------------------------------------------------*
/**
 * find submenu by name.
 *
 * @param           [in] pMainMenu: pointer to main menu
 * @param           [in] crstrName: name of submenu (& sign is not needed)
 * @param           [in/out] pSubMenu: pointer to found menu
 * @return          index of found submenu or -1 if not found
*/
int CMainFrame::FindSubmenuByName(CMenu* pMainMenu, const CString& crstrName, CMenu*& pSubMenu)
{
    pSubMenu = NULL;

    CString strText;
    CString strName = crstrName;
    strName.Replace("&", "");
    if(pMainMenu)
    {
        int iSubMenuCount = pMainMenu->GetMenuItemCount();
        for(int iSubMenu = 0; iSubMenu < iSubMenuCount; ++iSubMenu)
        {
            pMainMenu->GetMenuString(iSubMenu, strText, MF_BYPOSITION);
            strText.Replace("&", "");
            if(strText.CompareNoCase(strName) == 0)
            {
                pSubMenu = pMainMenu->GetSubMenu(iSubMenu);
                return iSubMenu;
            }
        }
    }
    return -1;
}

//------------------------------------------------------------------*
/**
 * find menu item by name.
 *
 * @param           [in] pSubMenu: sub menu where to search
 * @param           [in] crstrName: name of menu item
 * @return          index of menu item
 *
*/
int CMainFrame::FindMenuItemByName(CMenu* pSubMenu, const CString& crstrName)
{
    CString strText;
    int     iFound;

    if(pSubMenu)
    {
        int iMenuItemCount = pSubMenu->GetMenuItemCount();
        for(int iMenuItem = 0; iMenuItem < iMenuItemCount; ++iMenuItem)
        {
            pSubMenu->GetMenuString(iMenuItem, strText, MF_BYPOSITION);
            
            // remove hot key info
            iFound = strText.Find(_T('\t'));
            if(iFound >= 0)
            {
                strText = strText.Left(iFound);
            }
            // remove ampersand
            strText.Replace("&", "");

            if(strText.CompareNoCase(crstrName) == 0)
            {
                return iMenuItem;
            }
        }
    }
    return(-1);
}


//------------------------------------------------------------------*
/**
 * add main submenu.
 *
 *  menu items are inserted before certain index position.
 *
 * @param           [in] pSubMenu: MFC menu object
 * @param           [in] pKADSubMenu: KAD sub menu object
 * @param           [in] pKADMainMenu: KAD main menu object
 * @param           [in] rtAccelList: list of accelerator structures
 * @param           [in] iStartPos: starting position to insert (default 0)
 * @return          index of next insert position = index of last inserted menu item + 1
 *
*/
int CMainFrame::AddMainSubMenu(CMenu* pSubMenu, CKADMenu* pKADSubMenu, CKADMainMenu* pKADMainMenu, TAccelList& rtAccelList, int iStartPos /*=0*/)
{
    int iMenuItemCount = pKADSubMenu->GetItemCount();
    
    // add menu
    int                 iNumItems = pKADSubMenu->m_tItems.GetSize();
    int                 iInsertPos = iStartPos;
    CKADMenuItem*       pItem;

    for(int iItem = 0; iItem < iNumItems; ++iItem)
    {
        pItem = pKADSubMenu->m_tItems.GetAt(iItem);
        if(InsertMenuItem(pSubMenu, pItem, pKADMainMenu, rtAccelList, iInsertPos))
        {
            ++iInsertPos;
        }
    }
    return iInsertPos;
}

// insert menu item 
//------------------------------------------------------------------*
/**
 * insert menu item.
 *
 * @param           [in] pMenu: MFC menu object
 * @param           [in] pItem: KAD menu item object
 * @param           [in] pKADMainMenu: KAD main menu item
 * @param           [in] iInsertPos: insert position (index of new item)
 * @return          successful?
 *
*/
BOOL CMainFrame::InsertMenuItem(CMenu* pMenu, CKADMenuItem* pItem, CKADMainMenu* pKADMainMenu, TAccelList& rtAccelList, int iInsertPos)
{
    DWORD   dwStyle = MF_STRING | MF_BYPOSITION;
    
    if(!pItem->m_bEnabled)
    {
        dwStyle |= MF_GRAYED;
    }

    if(pItem->m_bSeparator)
    {
        // suppress separator at first position
        if(iInsertPos == 0)
        {
            return FALSE;
        }

        int iTmp = pMenu->GetMenuItemCount();
        // suppress separator after separator
        MENUITEMINFO    tMenuItemInfo;
        ::memset(&tMenuItemInfo, 0, sizeof(tMenuItemInfo));
        tMenuItemInfo.cbSize = sizeof(tMenuItemInfo);
        tMenuItemInfo.fMask = MIIM_TYPE;

        if(pMenu->GetMenuItemInfo(iInsertPos - 1, &tMenuItemInfo, TRUE))
        {
            if(tMenuItemInfo.fType & MFT_SEPARATOR)
            {
                return FALSE;
            }
        }
        else
        {
            DWORD dwErr = ::GetLastError();
            return FALSE;
        }
        pMenu->InsertMenu(iInsertPos, MF_SEPARATOR | MF_BYPOSITION);
        return TRUE;
    }
    // evaluate show expression
    if(!pItem->m_strShowExpr.IsEmpty())
    {
        if(pItem->m_strShowExpr.Find(_T("project=open")) >= 0 && !m_bProjectLoaded)
        {
            return FALSE;
        }
        if(pItem->m_strShowExpr.Find(_T("project!=open")) >= 0 && m_bProjectLoaded)
        {
            return FALSE;
        }
        if(pItem->m_strShowExpr.Find(_T("project=readonly")) >= 0 && !m_bProjectReadOnly)
        {
            return FALSE;
        }
        if(pItem->m_strShowExpr.Find(_T("project!=readonly")) >= 0 && m_bProjectReadOnly)
        {
            return FALSE;
        }
    }

    BOOL    bSuccessful = FALSE;
    long    lItemId;

    ASSERT(pItem->m_iItemNo >= 0);
    lItemId = ID_MAIN_MENU_BASE + pItem->m_iItemNo;
    if(pMenu->InsertMenu(iInsertPos, dwStyle, lItemId, pItem->m_strText))
    {
        bSuccessful = TRUE;
    }
    if(pItem->m_bAccel)
    {
        ACCEL   tAccel;
        tAccel.cmd = (WORD)lItemId;
        tAccel.fVirt = (BYTE)pItem->m_wAccelVirt;
        tAccel.key = pItem->m_wAccelKey;
        rtAccelList.AddTail(tAccel);
    }
    return(bSuccessful);
}


//------------------------------------------------------------------*
/**
 * on main menu command.
 *
 * @param           [in] nID: menu item id
 *
*/
void CMainFrame::OnMainMenuCommand(UINT nID)
{
    // get KAD main menu from application
    CKADInfo*       pKADInfo = ((CCEMAINApp*)AfxGetApp())->GetKADInfoObject();
    // get menu item from ID
    int     iItemId = nID - ID_MAIN_MENU_BASE;
    CKADMenuItem*   pItem = pKADInfo->GetMenuItemFromId(iItemId);
    if(pItem)
    {
        TRACE("Main menu command: %s\n", pItem->m_strText);
        if(pItem->m_iKSO > -1)
        {
            DoKernelStandardOperation(pItem->m_iKSO);
        }
        else
        {
            if(pItem->m_strExe != "")
            {
                SaveAllFiles();
                DoExeOperation(pItem);
            }
            else
            {
                if(pItem->m_pHandler)
                {
                    SaveAllFiles();
                    DoAddOnAction(pItem);
                }
            }
        }
    }
}


//------------------------------------------------------------------*
/**
 * on update main menu command.
 *
 * @param           [in] pCmdUI: see MSDN ON_UPDATE_COMMAND
 * @see             
*/
void CMainFrame::OnUpdateMainMenuCommand(CCmdUI* pCmdUI) 
{
    // get KAD main menu from application
    CKADInfo*       pKADInfo = ((CCEMAINApp*)AfxGetApp())->GetKADInfoObject();
    // get menu item from ID
    int     iItemId = pCmdUI->m_nID - ID_MAIN_MENU_BASE;
    CKADMenuItem*   pItem = pKADInfo->GetMenuItemFromId(iItemId);

    if(pItem)
    {
        //TRACE("Main menu command: %s\n", pItem->m_strText);
        if(pItem->m_iKSO > -1)
        {
            if(!IsKernelStandardOperationPossible(pItem->m_iKSO))
            {
                pCmdUI->Enable(FALSE);
                return;
            }
        }
        if(!pItem->m_bEnabled)
        {
            pCmdUI->Enable(FALSE);
            return;
        }
        if(!pItem->m_strEnableExpr.IsEmpty())
        {
            if(pItem->m_strEnableExpr.Find(_T("project=open")) >= 0 && !m_bProjectLoaded)
            {
                pCmdUI->Enable(FALSE);
                return;
            }
            if(pItem->m_strEnableExpr.Find(_T("project!=open")) >= 0 && m_bProjectLoaded)
            {
                pCmdUI->Enable(FALSE);
                return;
            }
            if(pItem->m_strEnableExpr.Find(_T("project=readonly")) >= 0 && !m_bProjectReadOnly)
            {
                pCmdUI->Enable(FALSE);
                return;
            }
            if(pItem->m_strEnableExpr.Find(_T("project!=readonly")) >= 0 && m_bProjectReadOnly)
            {
                pCmdUI->Enable(FALSE);
                return;
            }
        }
    }
    else
    {
        pCmdUI->Enable(FALSE);
        return;
    }

    pCmdUI->Enable(TRUE);
}


//------------------------------------------------------------------*
/**
 * is kernel standard operation possible?.
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
BOOL CMainFrame::IsKernelStandardOperationPossible(int iKSO)
{
    BOOL    bReturn = FALSE;

    if(!m_bProjectLoaded)
    {
        return bReturn;
    }

    CTargetSet* pTargetSet = CEGetTargetSet("Project Targets", false);
    if(!pTargetSet)
    {
        return bReturn;
    }

    switch(iKSO)
    {

    case KSO_OPEN:
    case KSO_OPEN_USER_HELP:
    case KSO_OPEN_URL:
    case KSO_GOTO:
    case KSO_GOTO_DEFINITION:
    case KSO_DELETE:
    case KSO_OPEN_CONSOLE:
    case KSO_OPEN_INSTANCE:
        break;

        
    case KSO_CONNECT:
        if(pTargetSet->GetTargetCountDisconnected() > 0)
        {
            bReturn = TRUE;
        }
        break;
    case KSO_DISCONNECT:
        if(pTargetSet->GetTargetCountConnected() > 0)
        {
            bReturn = TRUE;
        }
        break;
    case KSO_CONNECT_EX:
        if(pTargetSet->GetTargetCountDisconnectedEx() > 0)
        {
            bReturn = TRUE;
        }
        break;
    case KSO_DISCONNECT_EX:
        if(pTargetSet->GetTargetCountConnectedEx() > 0)
        {
            bReturn = TRUE;
        }
        break;

    case KSO_DOWNLOAD_FULL:
    case KSO_DOWNLOAD_FULL_ALL:
    case KSO_DOWNLOAD_FULL_SELECT:
        // successful compile should be checked!
        if(pTargetSet->GetTargetCount() > 0)
        {
            bReturn = TRUE;
        }
        break;
    case KSO_DOWNLOAD_CHANGES:
    case KSO_DOWNLOAD_CHANGES_ALL:
    case KSO_DOWNLOAD_CHANGES_SELECT:
        if(pTargetSet->GetTargetCountDLChanges() > 0)
        {
            bReturn = TRUE;
        }
        break;
    case KSO_DOWNLOAD_SELECTION:
        break;

    case KSO_BUILD_ALL:
    case KSO_BUILD_SELECT:
    case KSO_BUILD_TARGET:
        bReturn = TRUE;
        break;
    case KSO_COMPILE_FILE:
        // is editor active?
        {
            CGenEdCont* pContainer = GetContainer();
            if(pContainer->GetActiveDocument())
            {
                bReturn = TRUE;
            }
        }
        break;
    case KSO_BUILD_LIBRARY:
        bReturn = TRUE;
        break;

    case KSO_START_CONTROL:
    case KSO_STOP_CONTROL:
    case KSO_START_ALL_TASKS: 
    case KSO_STOP_ALL_TASKS:
    case KSO_COLDSTART_RESET:
        if(pTargetSet->GetTargetCountConnected() > 0)
        {
            bReturn = TRUE;
        }
        break;
    case KSO_WARMSTART_RESET:
        if(pTargetSet->GetTargetCountWarmstartReady() > 0)
        {
            bReturn = TRUE;
        }
        break;
    case KSO_INITIALIZE_CLEAR:
        if(pTargetSet->GetTargetCountInitClearReady() > 0)
        {
            bReturn = TRUE;
        }
        break;
    case KSO_START_TASK:
    case KSO_STOP_TASK:
        break;
    case KSO_SETTINGS_RESET_DOCKING:
    case KSO_SETTINGS_LIBRARY:
    case KSO_SETTINGS_ENGINEERING:
    case KSO_SETTINGS_PASSWORD:
    case KSO_SETTINGS_PRINT:
        bReturn = TRUE;
        break;
    }
    return bReturn;
}

//------------------------------------------------------------------*
/**
 * execute kernel standard operation
 *
 * @param           [in] iKSO: number of kernel standard operation
 * @return          operation performed?
*/
void CMainFrame::DoKernelStandardOperation(int iKSO)
{
    CTargetSet* pTargetSet = CEGetTargetSet("Project Targets", false);
    if(!pTargetSet)
    {
        return;
    }

    int iTargetCount;
    CTargetOnlBase* pTarget = NULL;

    iTargetCount = pTargetSet->GetTargetCount();
    CString strTargetId;
    if (iTargetCount==1)
    {
        POSITION pos = pTargetSet->GetFirstTargetPosition();
        ASSERT(pos);
        CTargetOnlBase* pOneTarget = pTargetSet->GetNextTarget(pos);
        ASSERT(pOneTarget);
        strTargetId = pOneTarget->GetId();
    }


    TRACE("main menu kernel stancard operation: %d\n", iKSO);
    switch(iKSO)
    {
    case KSO_OPEN:
        TRACE("MainFrm: kernel standard operation \"Open\"\n");
        ASSERT(!"KSO: not implemented as main menu kso!");
        break;
    case KSO_OPEN_USER_HELP:
        TRACE("MainFrm: kernel standard operation \"OpenUserHelp\"\n");
        ASSERT(!"KSO: not implemented as main menu kso!");
        break;
    case KSO_OPEN_URL:
        TRACE("MainFrm: kernel standard operation \"OpenUrl\"\n");
        ASSERT(!"KSO: not implemented as main menu kso!");
        break;
    case KSO_GOTO:
        TRACE("MainFrm: kernel standard operation \"Goto\"\n");
        ASSERT(!"KSO: not implemented as main menu kso!");
        break;
    case KSO_GOTO_DEFINITION:
        TRACE("MainFrm: kernel standard operation \"GotoDefinition\"\n");
        ASSERT(!"KSO: not implemented as main menu kso!");
        break;
    case KSO_DELETE:
        TRACE("MainFrm: kernel standard operation \"Delete\"\n");
        ASSERT(!"KSO: not implemented as main menu kso!");
        break;
    case KSO_OPEN_CONSOLE:
        TRACE("MainFrm: kernel standard operation \"OpenConsole\"\n");
        ASSERT(!"KSO: not implemented as main menu kso!");
        break;
    case KSO_OPEN_INSTANCE:
        TRACE("MainFrm: kernel standard operation \"OpenInstance\"\n");
        ASSERT(!"KSO: not implemented as main menu kso!");
        break;



    case KSO_CONNECT:
        TRACE("MainFrm: kernel standard operation \"Connect\"\n");
        {
            CMsgWnd*        pMsgWnd = (dynamic_cast<CMsgWnd *> (CEGetDockingTool ("Message View", true)));
            if(pMsgWnd)
            {
                if (iTargetCount>1)
                {
                    CStringArray        astrTargetsSel;
                    if(SelectTargets(astrTargetsSel, eDisconnected, pTargetSet))
                    {
                        int iNumTargets = astrTargetsSel.GetSize();
                        for(int iTarg = 0; iTarg < iNumTargets; ++iTarg)
                        {
                            pMsgWnd->ConnectTarget(astrTargetsSel[iTarg]);
                        }
                    }
                }
                else
                {
                    pMsgWnd->ConnectTarget(strTargetId);
                }
            }
        }
        break;
    case KSO_DISCONNECT:
        TRACE("MainFrm: kernel standard operation \"Disconnect\"\n");
        {
            CMsgWnd*        pMsgWnd = (dynamic_cast<CMsgWnd *> (CEGetDockingTool ("Message View", true)));
            if(pMsgWnd)
            {
                if (iTargetCount>1)
                {
                    CStringArray        astrTargetsSel;
                    if(SelectTargets(astrTargetsSel, eConnected, pTargetSet))
                    {
                        int iNumTargets = astrTargetsSel.GetSize();
                        for(int iTarg = 0; iTarg < iNumTargets; ++iTarg)
                        {
                            pMsgWnd->DisconnectTarget(astrTargetsSel[iTarg]);
                        }
                    }
                }
                else
                {
                    pMsgWnd->DisconnectTarget(strTargetId);
                }
            }
        }
        break;
    case KSO_CONNECT_EX:
        TRACE("MainFrm: kernel standard operation \"ConnectEx\"\n");
        {
            CMsgWnd*        pMsgWnd = (dynamic_cast<CMsgWnd *> (CEGetDockingTool ("Message View", true)));
            if(pMsgWnd)
            {
                if (iTargetCount>1)
                {
                    CStringArray        astrTargetsSel;
                    if(SelectTargets(astrTargetsSel, eDisconnectedEx, pTargetSet))
                    {
                        int iNumTargets = astrTargetsSel.GetSize();
                        for(int iTarg = 0; iTarg < iNumTargets; ++iTarg)
                        {
                            pMsgWnd->ConnectTargetExclusive(astrTargetsSel[iTarg]);
                        }
                    }
                }
                else
                {
                    pMsgWnd->ConnectTargetExclusive(strTargetId);
                }
            }
        }
        break;
    case KSO_DISCONNECT_EX:
        TRACE("MainFrm: kernel standard operation \"DisconnectEx\"\n");
        {
            CMsgWnd*        pMsgWnd = (dynamic_cast<CMsgWnd *> (CEGetDockingTool ("Message View", true)));
            if(pMsgWnd)
            {
                if (iTargetCount>1)
                {
                    CStringArray        astrTargetsSel;
                    if(SelectTargets(astrTargetsSel, eConnectedEx, pTargetSet))
                    {
                        int iNumTargets = astrTargetsSel.GetSize();
                        for(int iTarg = 0; iTarg < iNumTargets; ++iTarg)
                        {
                            pMsgWnd->DisconnectTargetExclusive(astrTargetsSel[iTarg]);
                        }
                    }
                }
                else
                {
                    pMsgWnd->DisconnectTargetExclusive(strTargetId);
                }
            }
        }
        break;


    case KSO_DOWNLOAD_FULL:
        TRACE("MainFrm: kernel standard operation \"DownloadFull\"\n");
        ASSERT(!"KSO: not implemented as main menu kso!");
        break;
    case KSO_DOWNLOAD_CHANGES:
        TRACE("MainFrm: kernel standard operation \"DownloadChanges\"\n");
        ASSERT(!"KSO: not implemented as main menu kso!");
        break;
    case KSO_DOWNLOAD_SELECTION:
        TRACE("MainFrm: kernel standard operation \"DownloadSelection\"\n");
        ASSERT(!"KSO: not implemented as main menu kso!");
        break;
    case KSO_DOWNLOAD_FULL_ALL:
        TRACE("MainFrm: kernel standard operation \"DownloadFullAll\"\n");
        DownloadTarget(eTargetAll, CEDL_DOWNLD_ALL);
        break;
    case KSO_DOWNLOAD_CHANGES_ALL:
        TRACE("MainFrm: kernel standard operation \"DownloadChangesAll\"\n");
        DownloadTarget(eTargetAll, CEDL_DOWNLD_CHANGES);
        break;
    case KSO_DOWNLOAD_FULL_SELECT:
        TRACE("MainFrm: kernel standard operation \"DownloadFullSelect\"\n");
        if (iTargetCount==1)
        {
            DownloadTarget(eTargetOne, CEDL_DOWNLD_ALL, strTargetId);
        }
        else
        {
            DownloadTarget(eTargetSelect, CEDL_DOWNLD_ALL);
        }
        break;
    case KSO_DOWNLOAD_CHANGES_SELECT:
        TRACE("MainFrm: kernel standard operation \"DownloadChangesSelect\"\n");
        if (iTargetCount==1)
        {
            DownloadTarget(eTargetOne, CEDL_DOWNLD_CHANGES, strTargetId);
        }
        else
        {
            DownloadTarget(eTargetSelect, CEDL_DOWNLD_CHANGES);
        }
        break;



    case KSO_BUILD_ALL:
        TRACE("MainFrm: kernel standard operation \"BuildAll\"\n");
        Build(eTargetAll);
        break;
    case KSO_BUILD_SELECT:
        TRACE("MainFrm: kernel standard operation \"BuildSelect\"\n");
        if (iTargetCount==1)
        {
            Build(eTargetOne, strTargetId);
        }
        else
        {
            Build(eTargetSelect);
        }
        break;
    case KSO_BUILD_TARGET:
        TRACE("MainFrm: kernel standard operation \"BuildTarget\"\n");
        ASSERT(!"KSO: not implemented as main menu kso!");
        break;
    case KSO_COMPILE_FILE:
        TRACE("MainFrm: kernel standard operation \"CompileFile\"\n");
        {
            CGenEdCont* pContainer = GetContainer();
            CGenEdDoc *pDoc = pContainer->GetActiveDocument();
            if(pDoc)
            {
                CString strFileName = pDoc->GetDocumentFile();
                CompileFile(strFileName);
            }
        }
        break;
    case KSO_BUILD_LIBRARY:
        TRACE("MainFrm: kernel standard operation \"BuildLibrary\"\n");
        BuildLibrary();
        break;


    case KSO_START_CONTROL:
        TRACE("MainFrm: kernel standard operation \"StartControl\"\n");
//        iTargetCount = pTargetSet->GetTargetCountConnected();
//        if(iTargetCount == 1)
//        {
//            POSITION pos = pTargetSet->GetFirstTargetPosition();
//            pTarget = pTargetSet->GetNextTarget(pos);
//        }
//        if(pTarget)
//        {
//            StartResource(pTarget);
//        }
//        else
        {
            CStringArray        astrTargetsSel;
            if(SelectTargets(astrTargetsSel, eConnected, pTargetSet))
            {
                int iNumTargets = astrTargetsSel.GetSize();
                for(int iTarg = 0; iTarg < iNumTargets; ++iTarg)
                {
                    pTarget = pTargetSet->GetTarget(astrTargetsSel[iTarg]);
                    if(pTarget)
                    {
                        StartResource(pTarget);
                    }
                }
            }
        }
        break;
    case KSO_STOP_CONTROL:
        TRACE("MainFrm: kernel standard operation \"StopControl\"\n");
//        iTargetCount = pTargetSet->GetTargetCountConnected();
//        if(iTargetCount == 1)
//        {
//            POSITION pos = pTargetSet->GetFirstTargetPosition();
//            pTarget = pTargetSet->GetNextTarget(pos);
//        }
//        if(pTarget)
//        {
//            StopResource(pTarget);
//        }
//        else
        {
            CStringArray        astrTargetsSel;
            if(SelectTargets(astrTargetsSel, eConnected, pTargetSet))
            {
                int iNumTargets = astrTargetsSel.GetSize();
                for(int iTarg = 0; iTarg < iNumTargets; ++iTarg)
                {
                    pTarget = pTargetSet->GetTarget(astrTargetsSel[iTarg]);
                    if(pTarget)
                    {
                        StopResource(pTarget);
                    }
                }
            }
        }
        break;
    case KSO_START_ALL_TASKS: 
        TRACE("MainFrm: kernel standard operation \"StartAllTasks\"\n");
//        iTargetCount = pTargetSet->GetTargetCountConnected();
//        if(iTargetCount == 1)
//        {
//            POSITION pos = pTargetSet->GetFirstTargetPosition();
//            pTarget = pTargetSet->GetNextTarget(pos);
//        }
//        if(pTarget)
//        {
//            StartAllTasks(pTarget);
//        }
//        else
        {
            CStringArray        astrTargetsSel;
            if(SelectTargets(astrTargetsSel, eConnected, pTargetSet))
            {
                int iNumTargets = astrTargetsSel.GetSize();
                for(int iTarg = 0; iTarg < iNumTargets; ++iTarg)
                {
                    pTarget = pTargetSet->GetTarget(astrTargetsSel[iTarg]);
                    if(pTarget)
                    {
                        StartAllTasks(pTarget);
                    }
                }
            }
        }
        break;
    case KSO_STOP_ALL_TASKS:
        TRACE("MainFrm: kernel standard operation \"StopAllTasks\"\n");
//        iTargetCount = pTargetSet->GetTargetCountConnected();
//        if(iTargetCount == 1)
//        {
//            POSITION pos = pTargetSet->GetFirstTargetPosition();
//            pTarget = pTargetSet->GetNextTarget(pos);
//        }
//        if(pTarget)
//        {
//            StopAllTasks(pTarget);
//        }
//        else
        {
            CStringArray        astrTargetsSel;
            if(SelectTargets(astrTargetsSel, eConnected, pTargetSet))
            {
                int iNumTargets = astrTargetsSel.GetSize();
                for(int iTarg = 0; iTarg < iNumTargets; ++iTarg)
                {
                    pTarget = pTargetSet->GetTarget(astrTargetsSel[iTarg]);
                    if(pTarget)
                    {
                        StopAllTasks(pTarget);
                    }
                }
            }
        }
        break;
    case KSO_COLDSTART_RESET:
        TRACE("MainFrm: kernel standard operation \"ColdstartReset\"\n");
//        iTargetCount = pTargetSet->GetTargetCountConnected();
//        if(iTargetCount == 1)
//        {
//            POSITION pos = pTargetSet->GetFirstTargetPosition();
//            pTarget = pTargetSet->GetNextTarget(pos);
//        }
//        if(pTarget)
//        {
//            ColdStartResource(pTarget);
//        }
//        else
        {
            CStringArray        astrTargetsSel;
            if(SelectTargets(astrTargetsSel, eConnected, pTargetSet))
            {
                int iNumTargets = astrTargetsSel.GetSize();
                for(int iTarg = 0; iTarg < iNumTargets; ++iTarg)
                {
                    pTarget = pTargetSet->GetTarget(astrTargetsSel[iTarg]);
                    if(pTarget)
                    {
                        ColdStartResource(pTarget);
                    }
                }
            }
        }
        break;
    case KSO_WARMSTART_RESET:
        TRACE("MainFrm: kernel standard operation \"WarmstartReset\"\n");
//        iTargetCount = pTargetSet->GetTargetCountDisconnectedEx();
//        if(iTargetCount == 1)
//        {
//            POSITION pos = pTargetSet->GetFirstTargetPosition();
//            pTarget = pTargetSet->GetNextTarget(pos);
//        }
//        if(pTarget)
//        {
//            WarmStartResource(pTarget);
//        }
//        else
        {
            CStringArray        astrTargetsSel;
            if(SelectTargets(astrTargetsSel, eConnected, pTargetSet))
            {
                int iNumTargets = astrTargetsSel.GetSize();
                for(int iTarg = 0; iTarg < iNumTargets; ++iTarg)
                {
                    pTarget = pTargetSet->GetTarget(astrTargetsSel[iTarg]);
                    if(pTarget)
                    {
                        WarmStartResource(pTarget);
                    }
                }
            }
        }
        break;
    case KSO_INITIALIZE_CLEAR:
        TRACE("MainFrm: kernel standard operation \"InitializeClear\"\n");
//        iTargetCount = pTargetSet->GetTargetCountConnectedEx();
//        if(iTargetCount == 1)
//        {
//            POSITION pos = pTargetSet->GetFirstTargetPosition();
//            pTarget = pTargetSet->GetNextTarget(pos);
//        }
//        if(pTarget)
//        {
//            ClearResource(pTarget);
//        }
//        else
        {
            CStringArray        astrTargetsSel;
            if(SelectTargets(astrTargetsSel, eConnectedEx, pTargetSet))
            {
                int iNumTargets = astrTargetsSel.GetSize();
                for(int iTarg = 0; iTarg < iNumTargets; ++iTarg)
                {
                    pTarget = pTargetSet->GetTarget(astrTargetsSel[iTarg]);
                    if(pTarget)
                    {
                        ClearResource(pTarget);
                    }
                }
            }
        }
        break;
    case KSO_START_TASK:
        TRACE("MainFrm: kernel standard operation \"StartTask\"\n");
        ASSERT(!"KSO: not implemented as main menu kso!");
        break;
    case KSO_STOP_TASK:
        TRACE("MainFrm: kernel standard operation \"StopTask\"\n");
        ASSERT(!"KSO: not implemented as main menu kso!");
        break;


    case KSO_SETTINGS_RESET_DOCKING:
        TRACE("MainFrm: kernel standard operation \"SettingsResetDocking\"\n");
        DoSettingsResetDocking();
        break;
    case KSO_SETTINGS_LIBRARY:
        TRACE("MainFrm: kernel standard operation \"SettingsLibrary\"\n");
        DoSettingsLibrary();        
        break;
    case KSO_SETTINGS_ENGINEERING:
        TRACE("MainFrm: kernel standard operation \"SettingsEngineering\"\n");
        DoSettingsEngineering();        
        break;
    case KSO_SETTINGS_PASSWORD:
        TRACE("MainFrm: kernel standard operation \"SettingsPassword\"\n");
        DoSettingsPassword();        
        break;
    case KSO_SETTINGS_PRINT:
        TRACE("MainFrm: kernel standard operation \"SettingsPrint\"\n");
        DoSettingsPrint();
        break;

    }
}


//------------------------------------------------------------------*
/**
 * start resource.
 *
 *  if target is not connected this is done now.
 *
 * @param           [in] pTarget: target onl object
 * @return          successful?
*/
BOOL CMainFrame::StartResource(CTargetOnlBase* pTarget)
{
    HRESULT hr;
    if(!pTarget->IsTargetConnected())
    {
        pTarget->Connect();
    }
    hr = pTarget->StartResource();
    if(hr == S_OK)
    {
        return TRUE;
    }
    return FALSE;
}

//------------------------------------------------------------------*
/**
 * stop resource.
 *
 *  if target is not connected this is done now.
 *
 * @param           [in] pTarget: target onl object
 * @return          successful?
*/
BOOL CMainFrame::StopResource(CTargetOnlBase* pTarget)
{
    HRESULT hr;
    if(!pTarget->IsTargetConnected())
    {
        pTarget->Connect();
    }
    hr = pTarget->StopResource();
    if(hr == S_OK)
    {
        return TRUE;
    }
    return FALSE;
}

//------------------------------------------------------------------*
/**
 * start all tasks.
 *
 *  if target is not connected this is done now.
 *
 * @param           [in] pTarget: target onl object
 * @return          successful?
*/
BOOL CMainFrame::StartAllTasks(CTargetOnlBase* pTarget)
{
    HRESULT hr;
    if(!pTarget->IsTargetConnected())
    {
        pTarget->Connect();
    }
    hr = pTarget->StartAllTasks();
    if(hr == S_OK)
    {
        return TRUE;
    }
    return FALSE;
}

//------------------------------------------------------------------*
/**
 * stop all tasks.
 *
 *  if target is not connected this is done now.
 *
 * @param           [in] pTarget: target onl object
 * @return          successful?
*/
BOOL CMainFrame::StopAllTasks(CTargetOnlBase* pTarget)
{
    HRESULT hr;
    if(!pTarget->IsTargetConnected())
    {
        pTarget->Connect();
    }
    hr = pTarget->StopAllTasks();
    if(hr == S_OK)
    {
        return TRUE;
    }
    return FALSE;
}

//------------------------------------------------------------------*
/**
 * coldstart resource.
 *
 *  if target is not connected this is done now.
 *
 * @param           [in] pTarget: target onl object
 * @return          successful?
*/
BOOL CMainFrame::ColdStartResource(CTargetOnlBase* pTarget)
{
    HRESULT hr;
    if(!pTarget->IsTargetConnected())
    {
        pTarget->Connect();
    }
    hr = pTarget->ColdStartResource();
    if(hr == S_OK)
    {
        return TRUE;
    }
    return FALSE;
}

//------------------------------------------------------------------*
/**
 * warmstart resource.
 *
 *  if target is not connected this is done now.
 *
 * @param           [in] pTarget: target onl object
 * @return          successful?
*/
BOOL CMainFrame::WarmStartResource(CTargetOnlBase* pTarget)
{
    HRESULT hr;
    if(!pTarget->IsTargetConnected())
    {
        pTarget->Connect();
    }
    hr = pTarget->WarmStartResource();
    if(hr == S_OK)
    {
        return TRUE;
    }
    return FALSE;
}

//------------------------------------------------------------------*
/**
 * clear resource.
 *
 *  if target is not connected this is done now.
 *
 * @param           [in] pTarget: target onl object
 * @return          successful?
*/
BOOL CMainFrame::ClearResource(CTargetOnlBase* pTarget)
{
    HRESULT hr;
    CTargetOnl* pTarg = dynamic_cast<CTargetOnl*> (pTarget);
    if(!pTarg)
    {
        return FALSE;
    }

    CMsgWnd* pMsgWnd = (dynamic_cast<CMsgWnd *> (CEGetDockingTool ("Message View", true)));

    if(!pTarg->IsTargetConnectedExclusive())
    {
        m_bDoNotAddToCheckList = true;
        EstablishDebugConnection(pTarg, pMsgWnd);
        m_bDoNotAddToCheckList = false;
    }
    hr = pTarg->ClearResource(pTarget->GetId());
    if(hr == S_OK)
    {
        return TRUE;
    }
    return FALSE;
}


//------------------------------------------------------------------*
/**
 * select targets.
 *
 *  shows target selection dialog and allows the user to select one or
 *  more targets.
 *
 * @param            [out] rastrTargetsSel: selected target ids (names)
 * @return           user pressed OK?
*/
BOOL CMainFrame::SelectTargets(CStringArray& rastrTargetsSel, ETargetSelectMode eMode, CTargetSet* pTargetSet /*=NULL*/)
{
    if(!pTargetSet)
    {
        pTargetSet = CEGetTargetSet("Project Targets", false);
        if(!pTargetSet)
        {
            return FALSE;
        }
    }

    //
    CStringArray    astrTargets;
    CMultiSelDlg    tTargetSel(this);
    CString         strTitle;
    strTitle.LoadString(IDS_TARGET_SELECTION);

    switch(eMode)
    {
    case eAll:
        pTargetSet->GetAllTargetIds(astrTargets);
        break;
    case eConnected:
        pTargetSet->GetTargetIdsConnected(astrTargets);
        break;
    case eDisconnected:
        pTargetSet->GetTargetIdsDisconnected(astrTargets);
        break;
    case eConnectedEx:
        pTargetSet->GetTargetIdsConnectedEx(astrTargets);
        break;
    case eDisconnectedEx:
        pTargetSet->GetTargetIdsDisconnectedEx(astrTargets);
        break;
    case eDownloadChanges:
        pTargetSet->GetTargetIdsDLChanges(astrTargets);
        break;
    }

    tTargetSel.Initialize(strTitle, astrTargets);
    if(tTargetSel.DoModal() == IDOK)
    {
        tTargetSel.GetSelection(rastrTargetsSel);
        return TRUE;
    }
    return FALSE;
}


// do exe operation

//------------------------------------------------------------------*
/**
 * do exe operation.
 *
 * @param           pItem: KAD menu item
 * @param           crstrExeCommand: optional execute command overrides pItem->m_strExe
 * @return          -
 * @exception       -
 * @see             -
*/
void CMainFrame::DoExeOperation(CKADMenuItem* pItem, const CString& crstrExeCommand /*=_T("")*/)
{
    TRACE("main menu exe operation: %s %s (%s)\n", pItem->m_strText, pItem->m_strExe, pItem->m_strCommand);

    CComPtr<IUnknown>       pTmp;
    CComQIPtr<ICEExecute>   pExeDelegator;
    CComQIPtr<ICEProjInfo>  pProjInfo;

    pTmp.Attach(CEGetCOM(CE_COMPMAN_EXE));

    pExeDelegator = pTmp;

    pTmp.Release();
    pTmp.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));

    pProjInfo = pTmp;

    if(pExeDelegator && pProjInfo)
    {
        DWORD   dwSessionId;
        HRESULT hr;
        int     iNumCommands;

        CStringArray    astrCommands;
        CString strCommand;
        if(crstrExeCommand.IsEmpty())
        {
            strCommand = pItem->m_strExe;
        }
        else
        {
            strCommand = crstrExeCommand;
        }

        // substitute macros: first let project manager do some work
        CComBSTR    sCommandIn(strCommand);
        CComBSTR    sCommandOut;
        hr = pProjInfo->ReplaceMacros(sCommandIn, &sCommandOut);
        if(hr == S_OK)
        {
            strCommand = sCommandOut;
        }
        else
        {
            ReplacePrjMacrosEmpty(strCommand);
        }

        // now do rest of macro substitution
        SubstituteFileMacros(strCommand);
        iNumCommands = GetCommandStrings(strCommand, astrCommands);

        if(iNumCommands > 1)
        {
            // open exe session with auto close attribute
            hr = pExeDelegator->OpenExeSession(&dwSessionId, pProjInfo, TRUE);
            if(hr != S_OK)
            {
                CString strMessage;
                strMessage.LoadString(IDS_ERR_EXECUTE);
                AfxMessageBox(strMessage);
            }
            for(int iCommand = 0; iCommand < iNumCommands; ++iCommand)
            {
                CComBSTR    sCommand(astrCommands[iCommand]);
                hr = pExeDelegator->WriteExeCommand(dwSessionId, sCommand, 0);
                ASSERT(hr == S_OK);
                if(hr != S_OK)
                {
                    break;
                }
            }
            if(hr != S_OK)
            {
                // close session and kill process
                hr = pExeDelegator->CloseExeSession(dwSessionId, TRUE);
            }
            // last command is empty -> auto close will close session
            CComBSTR    sEmpty;
            hr = pExeDelegator->WriteExeCommand(dwSessionId, sEmpty, 0);
        }
        else
        {
            DWORD       dwReturn;
            CComBSTR    sCommand(strCommand);
            hr = pExeDelegator->ExecuteCommand(sCommand, pProjInfo, 0, FALSE, TRUE, &dwReturn);
        }
    }
}


int CMainFrame::GetCommandStrings(const CString& crstrCommandSeq, CStringArray& rastrCommands)
{
    int iStart = 0;
    int iStop;

    ASSERT(!crstrCommandSeq.IsEmpty());

    CString strCommand;
    CString strCommandSeq = crstrCommandSeq;

    rastrCommands.RemoveAll();

    iStop = strCommandSeq.Find(_T(';'));
    while(iStop != -1)
    {
        strCommand = strCommandSeq.Mid(iStart, iStop-iStart);
        rastrCommands.Add(strCommand);
        iStart = iStop + 1;
        iStop = strCommandSeq.Find(_T(';'), iStart);
    }
    strCommand = strCommandSeq.Mid(iStart);
    if(!strCommand.IsEmpty())
    {
        rastrCommands.Add(strCommand);
    }
    return(rastrCommands.GetSize());
}



//------------------------------------------------------------------*
/**
 * substitute standard parameters.
 *
 *  substitute $() parameters in exe commands.
 *
 * @param           [in/out] rstrCommand: exe command line
*/
void CMainFrame::SubstituteFileMacros(CString& rstrCommand)
{
//$(FileDir)
    CString strFileNamePath = GetEditorFileNamePath();
    CString strFilePath = GetPathFromNamePath(strFileNamePath);
//    rstrCommand.Replace(CE_STDARG_FILEDIR, strFilePath);
    rstrCommand.Replace(_T("$(FileDir)"), strFilePath);
//$(FileName)
    CString strFileName = GetFileNameFromNamePath(strFileNamePath);
//    rstrCommand.Replace(CE_STDARG_FILENAME, strFileName);
    rstrCommand.Replace(_T("$(FileName)"), strFileName);
//$(FileBaseName)
    CString strFileBaseName = GetFileBaseName(strFileName);
//    rstrCommand.Replace(CE_STDARG_FILEBASENAME, strFileBaseName);
    rstrCommand.Replace(_T("$(FileBaseName)"), strFileBaseName);
//$(FileExt)
    CString strFileExt = GetFileExtension(strFileName);
//    rstrCommand.Replace(CE_STDARG_FILEEXT, strFileExt);
    rstrCommand.Replace(_T("$(FileExt)"), strFileExt);
}


//------------------------------------------------------------------*
/**
 * remove all macros that were not substituted by project manager
 *
 *  this is needed when a tool is called with no project open
 *
 * @param           [in/out] rstrCommand: exe command line
*/
void CMainFrame::ReplacePrjMacrosEmpty(CString& rstrCommand)
{
    rstrCommand.Replace(_T("$(PrjDir)"), _T(""));
    rstrCommand.Replace(_T("$(PrjName)"), _T(""));
    rstrCommand.TrimRight();
}


//------------------------------------------------------------------*
/**
 * get editor file name path.
 *
 *  get file name incl path of currently active document.
 *
 * @return          file name incl file path
*/
CString CMainFrame::GetEditorFileNamePath()
{
    CString strPathName;
    CGenEdCont* pContainer = GetContainer();
    ASSERT(pContainer);
    if(pContainer)
    {
        CGenEdDoc*  pGenEdDoc = pContainer->GetActiveDocument();
        if(pGenEdDoc)
        {
            strPathName = pGenEdDoc->GetPathName();
        }
    }
    return strPathName;
}


//------------------------------------------------------------------*
/**
 * get path from name path.
 *
 *  get file path from file name incl path.
 *
 * @return          file path
*/
CString CMainFrame::GetPathFromNamePath(const CString& crstrFileNamePath)
{
    CString strFilePath = crstrFileNamePath;
    int iFound = crstrFileNamePath.ReverseFind(_T('\\'));
    if(iFound != -1)
    {
        strFilePath = crstrFileNamePath.Left(iFound);
    }
    return strFilePath;
}


//------------------------------------------------------------------*
/**
 * get file name from file path.
 *
 * @param           [in] crstrFileNamePath: file name incl path
 * @return          file name
*/
CString CMainFrame::GetFileNameFromNamePath(const CString& crstrFileNamePath)
{
    CString strFileName = crstrFileNamePath;
    int iFound = crstrFileNamePath.ReverseFind(_T('\\'));
    if(iFound != -1)
    {
        strFileName = crstrFileNamePath.Mid(iFound+1);
    }
    return strFileName;
}

//------------------------------------------------------------------*
/**
 * get file base name from file name.
 *
 * @param           [in] crstrFileName: file name
 * @return          file extension
*/
CString CMainFrame::GetFileBaseName(const CString& crstrFileName)
{
    CString strFileBaseName = crstrFileName;
    int iFound = crstrFileName.ReverseFind(_T('.'));
    if(iFound != -1)
    {
        strFileBaseName = crstrFileName.Left(iFound);
    }
    return strFileBaseName;
}

//------------------------------------------------------------------*
/**
 * get file extension from file name.
 *
 * @param           [in] crstrFileName: file name
 * @return          file extension
*/
CString CMainFrame::GetFileExtension(const CString& crstrFileName)
{
    CString strFileExt;
    int iFound = crstrFileName.ReverseFind(_T('.'));
    if(iFound != -1)
    {
        strFileExt = crstrFileName.Mid(iFound+1);
    }
    return strFileExt;
}

// execute add on action
void CMainFrame::DoAddOnAction(CKADMenuItem* pItem, BOOL bSilent /*=FALSE*/)
{
    HRESULT hr;

    TRACE("main menu addon action %s: %s\n", pItem->m_strText, pItem->m_strCommand);
    // if no handler is given, return
    if(!pItem->m_pHandler)
    {
        TRACE("handler is null!\n");
        return;
    }

    // get addon action interface
    CGuid                       tGuid(*(pItem->m_pHandler));
    CComPtr<IUnknown>           pTmp;
    CComQIPtr<ICEAddonAction>   pAddOnIF;
    CString                     strGuid = tGuid;

    pTmp.Attach(CEGetCOMByCLSID(strGuid, true));

    pAddOnIF = pTmp;
    if(!pAddOnIF)
    {
        return;
    }

    // call do action
    CString     strAdditionalInfo;
    if(bSilent)
    {
        strAdditionalInfo = _T("SILENT");
    }
    CComBSTR    sMethodName = pItem->m_strCommand;
    CComBSTR    sSourceFile;
    CComBSTR    sId;
    CComBSTR    sAdditionalInfo(strAdditionalInfo);

    CComQIPtr<ICEProjInfo>  pICEProjectInfo;
    CComPtr<IUnknown>       pTmpInfo;

    pTmpInfo.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
    pICEProjectInfo = pTmpInfo;
    
    if(!pICEProjectInfo)
    {
        TRACE("No project info available.\n");
        return;
    }

    hr = pICEProjectInfo->getProjectName(&sSourceFile);
    if (hr == S_OK)
    {
        sSourceFile += _T(".");
        sSourceFile += CE_PROJ_FILE_EXT;
    }

    hr = pAddOnIF->doAction(sMethodName, sSourceFile, sId, pICEProjectInfo, sAdditionalInfo);

    if(!bSilent && hr == S_OK)
    {
        pICEProjectInfo->fileParseNow(NULL);
    }
}

//------------------------------------------------------------------*
/**
 * save all files.
 *
 *  get editor container and call SaveAll()
 *
 * @param            [in] bAskBefore: ask before saving?
 *
*/
void CMainFrame::SaveAllFiles(bool bAskBefore /*=false*/)
{
    CGenEdCont* pContainer = GetContainer();
    if(pContainer)
    {
        pContainer->SaveAll(bAskBefore);
    }
    // now inform project manager
    CComPtr<ICEProjInfo> pICEProjInfo; 
    GetProjectInfo(&pICEProjInfo);

    if(pICEProjInfo)
    {
        pICEProjInfo->fileParseNow(NULL);
    }
}


//------------------------------------------------------------------*
/**
 * on get disp info.
 *
 *  command handler for the TTN_DISPINFO message.
 *  this message is sent from controls to get tool tip text.
 *
 * @param           [in] id: not used
 * @param           [in/out] pTTTStruct: tool tip text structure
 * @param           [in/out] pResult: not used
 * @see             GetToolTipTextForMainMenuItem()
*/
void CMainFrame::OnGetDispInfo(UINT id, NMHDR* pTTTStruct, LRESULT* pResult )
{
    CString strToolTip;
    TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pTTTStruct;

    if(GetToolTipTextForMainMenuItem(pTTTStruct->idFrom, strToolTip))
    {
        _tcscpy(pTTT->lpszText, strToolTip);
    }
}

//------------------------------------------------------------------*
/**
 * get tool tip text for main menu item.
 *
 * @param           [in] uiID: main menu ID
 * @param           [out] rstrToolTip: tool tip text to be shown
 * @return          successful?
 * @see             
*/
BOOL CMainFrame::GetToolTipTextForMainMenuItem(UINT uiID, CString& rstrToolTip) const
{
    CKADInfo*           pKADInfo = ((CCEMAINApp*)AfxGetApp())->GetKADInfoObject();
    if(!pKADInfo)
    {
        return FALSE;
    }
    CKADMenuItem* pMenuItem = pKADInfo->GetMenuItemFromId(uiID - ID_MAIN_MENU_BASE);
    if(pMenuItem)
    {
        rstrToolTip = pMenuItem->m_strToolTipText;
        return TRUE;
    }
    else
    {
        rstrToolTip.Empty();
    }
    return FALSE;
}


//------------------------------------------------------------------*
/**
 * get status text for main menu item.
 *
 * @param           [in] uiID: main menu ID
 * @param           [out] rstrStatus: status text to be shown
 * @return          successful?
 * @see             
*/
BOOL CMainFrame::GetStatusTextForMainMenuItem(UINT uiID, CString& rstrStatus) const
{
    CKADInfo*           pKADInfo = ((CCEMAINApp*)AfxGetApp())->GetKADInfoObject();
    if(!pKADInfo)
    {
        return FALSE;
    }
    CKADMenuItem* pMenuItem = pKADInfo->GetMenuItemFromId(uiID - ID_MAIN_MENU_BASE);
    if(pMenuItem)
    {
        rstrStatus = pMenuItem->m_strStatusText;
        return TRUE;
    }
    else
    {
        rstrStatus.Empty();
    }
    return FALSE;
}

//------------------------------------------------------------------*
/**
 * get message string.
 *
 *  overwritten version of MFC method to get status bar text for KAD
 *  menu entries.
 *
 * @param           [in] nID: main menu ID
 * @param           [out] rMessage: message string to show in status bar
 * @see             GetStatusTextForMainMenuItem()
*/
void CMainFrame::GetMessageString(UINT nID, CString& rMessage) const
{
    if(nID >= ID_MAIN_MENU_BASE && nID < ID_MAIN_MENU_END)
    {
        if(GetStatusTextForMainMenuItem(nID, rMessage))
        {
            return;
        }
    }
    SECMDIFrameWnd::GetMessageString(nID, rMessage);
}



void CMainFrame::StoreDockingTool(SECControlBar* pDockingTool, DockToolSettings* pSettings, BOOL bVisible)
{
    pSettings->m_bFloating = pDockingTool->IsFloating () == TRUE;
    pSettings->m_bVisible = (pDockingTool->GetStyle () & WS_VISIBLE) != 0;

    if (pSettings->m_bVisible)
    {
        if (pSettings->m_bFloating)
        { 
            pDockingTool->GetParentFrame()->GetWindowRect(&(pSettings->m_geometry));
            CSize szFloat = pDockingTool->m_szFloat;
            pSettings->m_geometry.right = pSettings->m_geometry.left + szFloat.cx;
            pSettings->m_geometry.bottom = pSettings->m_geometry.top + szFloat.cy;  
            pSettings->m_bMDIChild = pDockingTool->IsMDIChild();
        }
        else
        { 
            pDockingTool->GetBarSizePos(pSettings->m_iRow, 
                                        pSettings->m_iColumn,
                                        pSettings->m_uiMRUDockId,
                                        pSettings->m_fPctWidth,
                                        pSettings->m_iHeight);   
        }
    }

    pSettings->m_bAllowDocking = ((pDockingTool->m_dwDockStyle & CBRS_ALIGN_ANY) != 0);
    DWORD dwMRUDock = pDockingTool->m_dwMRUDockingState;

    SECControlBarInfo barInfo;
    pDockingTool->GetBarInfo(&barInfo);
}



void CMainFrame::RestoreDockingTool(SECControlBar* pDockingTool, DockToolSettings* pSettings, BOOL* pbVisible)
{
    CRect geometry;

    geometry = pSettings->m_geometry;
    
    CRect screenGeometry;

    screenGeometry.left = 0;
    screenGeometry.top = 0;
    screenGeometry.right = GetSystemMetrics (SM_CXFULLSCREEN);
    screenGeometry.bottom = GetSystemMetrics (SM_CYFULLSCREEN);

    // if position is outside the visible screen area
    // set it to position 0,0
    if (geometry.left < 0 || geometry.right > screenGeometry.right)
    {
        geometry.left = 0;
    }

    if (geometry.top < 0 || geometry.bottom > screenGeometry.bottom)
    {
        geometry.top = 0;
    }

    if(!pDockingTool)
    {
        return;
    }

    ShowControlBar(pDockingTool, FALSE, FALSE);

    SECDockPos posDock (pSettings->m_iColumn, pSettings->m_iRow);
    SECDockSize sizeDock (pSettings->m_fPctWidth, pSettings->m_iHeight);

    pDockingTool->m_szFloat = geometry.Size();

    DockControlBarEx(pDockingTool, posDock, pSettings->m_uiMRUDockId, sizeDock);   

    if (pSettings->m_bFloating)
    {
        if (pSettings->m_bMDIChild)
        {
            FloatControlBarInMDIChild(pDockingTool, geometry.TopLeft());
        }
        else
        {
            FloatControlBar (pDockingTool, geometry.TopLeft());
        }
    }

    if (!pSettings->m_bAllowDocking )
    {
        pDockingTool->m_dwMRUDockingState = pSettings->m_dwMRUDockingState;
        pDockingTool->EnableDocking(0);
    }


    ShowControlBar (pDockingTool, pSettings->m_bVisible, FALSE);

    *pbVisible = pSettings->m_bVisible;
    RecalcLayout (FALSE);
}


void CMainFrame::StoreDockingTools()
{
    if (!m_bProjectLoaded)
    {
        return;
    }

    if (m_isRestoringDockingTools)
    {
        return;
    }

    CCEMAINApp* pApp = dynamic_cast<CCEMAINApp *> (AfxGetApp());
    ASSERT(pApp);

    CSettings* pSettings = pApp->GetSettings();
    if (!pSettings)
    {
        return;
    }

    SECControlBar* pControlBar;
    DockToolSettings* pDockSettings;
    BOOL bVisible;

    pSettings->m_dockToolsPosSettings.m_bFullScreen = m_bViewFullScreen;
    pSettings->m_dockToolsPosSettings.m_uiVisFlags = m_uiVisFlags;

    pControlBar = GetNavigator(false);
    if(pControlBar)
    {
        pDockSettings = &(pSettings->m_dockToolsPosSettings.m_NavigatorSettings);
        bVisible = m_bShowNavigator;
        StoreDockingTool(pControlBar, pDockSettings, bVisible);
    }

    pControlBar = GetWatch(false);
    if(pControlBar)
    {
        pDockSettings = &(pSettings->m_dockToolsPosSettings.m_WatchSettings);
        bVisible = m_bShowWatch;
        StoreDockingTool(pControlBar, pDockSettings, bVisible);
    }

    pControlBar = GetBPView(false);
    if(pControlBar)
    {
        pDockSettings = &(pSettings->m_dockToolsPosSettings.m_BPointSettings);
        bVisible = m_bShowBPView;
        StoreDockingTool(pControlBar, pDockSettings, bVisible);
    }

    pControlBar = GetMsgView(false);
    if(pControlBar)
    {
        pDockSettings = &(pSettings->m_dockToolsPosSettings.m_MessageSettings);
        bVisible = m_bShowMsgView;
        StoreDockingTool(pControlBar, pDockSettings, bVisible);
    }

    pControlBar = GetPouVarSel(false);
    if(pControlBar)
    {
        pDockSettings = &(pSettings->m_dockToolsPosSettings.m_PouVarSelSettings);
        bVisible = m_bShowPouVarSel;
        StoreDockingTool(pControlBar, pDockSettings, bVisible);
    }

    pControlBar = GetXRef(false);
    if(pControlBar)
    {
        pDockSettings = &(pSettings->m_dockToolsPosSettings.m_XRefSettings);
        bVisible = m_bShowXRef;
        StoreDockingTool(pControlBar, pDockSettings, bVisible);
    }
}



void CMainFrame::RestoreDockingTools()
{
    CCEMAINApp* pApp = dynamic_cast<CCEMAINApp *> (AfxGetApp());
    ASSERT(pApp);

    CSettings* pSettings = pApp->GetSettings();
    if (!pSettings)
    {
        return;
    }

    m_isRestoringDockingTools = TRUE;

    SECControlBar* pControlBar;
    DockToolSettings* pDockSettings;
    BOOL bVisible;

    m_bViewFullScreen = pSettings->m_dockToolsPosSettings.m_bFullScreen;
    m_uiVisFlags = pSettings->m_dockToolsPosSettings.m_uiVisFlags;

    pControlBar = GetNavigator();
    pDockSettings = &(pSettings->m_dockToolsPosSettings.m_NavigatorSettings);
    RestoreDockingTool(pControlBar, pDockSettings, &bVisible);
    m_bShowNavigator = bVisible;

    pControlBar = GetWatch();
    pDockSettings = &(pSettings->m_dockToolsPosSettings.m_WatchSettings);
    RestoreDockingTool(pControlBar, pDockSettings, &bVisible);
    m_bShowWatch = bVisible;

    pControlBar = GetBPView();
    pDockSettings = &(pSettings->m_dockToolsPosSettings.m_BPointSettings);
    RestoreDockingTool(pControlBar, pDockSettings, &bVisible);
    m_bShowBPView = bVisible;

    pControlBar = GetMsgView();
    pDockSettings = &(pSettings->m_dockToolsPosSettings.m_MessageSettings);
    RestoreDockingTool(pControlBar, pDockSettings, &bVisible);
    m_bShowMsgView = bVisible;

    pControlBar = GetPouVarSel();
    pDockSettings = &(pSettings->m_dockToolsPosSettings.m_PouVarSelSettings);
    RestoreDockingTool(pControlBar, pDockSettings, &bVisible);
    m_bShowPouVarSel = bVisible;

    pControlBar = GetXRef();
    pDockSettings = &(pSettings->m_dockToolsPosSettings.m_XRefSettings);
    RestoreDockingTool(pControlBar, pDockSettings, &bVisible);
    m_bShowXRef = bVisible;


    m_isRestoringDockingTools = FALSE;

}

void CMainFrame::SetEditPositionText(LONG lLine, LONG lColumn, BOOL bShow)
{
    CString strPosition;

    if (bShow)
    {
        strPosition.Format(IDS_EDITPOS, lLine, lColumn);
    }
    else
    {
        strPosition = _T("");
    }

    m_wndStatusBar.SetPaneText(1, strPosition);

}

void CMainFrame::UpdateStatusBar(LPCTSTR pszText)
{
	m_wndStatusBar.SetWindowText(pszText);
}



void CMainFrame::OnUpdateEditPos (CCmdUI *pCmdUI)
{
	CGenEdCont *pContainer;

	// Get container subsystem from application. The container
	// subsystem may not exist. If the container subsystem exists
	// get its active document. There may not exist an active
	// container document. If there is an active container docu-
	// ment get the position text and write it into the CCmdUI.

	pContainer = GetContainer();
	if (pContainer != NULL)
	{
		CGenEdDoc *pDoc;

		pDoc = pContainer->GetActiveDocument ();
		if (pDoc != NULL)
		{
			pCmdUI->SetText (pDoc->GetPositionText ());
			return;
		}
	}


	// There is no further source for position information,
	// so display an string.

	pCmdUI->SetText (_T (""));
}



// *************************************************************************
// Printing
//


void CMainFrame::OnPrintFile()
{
    if(m_tDocuManager.PrintActiveDocument() == E_FAIL)
    {
        ::AfxMessageBox(IDS_PRINT_FAILED);
    }
	UpdateAllTabFileViews();
}

// OSAI feature print XRef 24.03.04 SIS >>
void CMainFrame::OnPrintXRef()
{
    if(m_tDocuManager.PrintXRef() == E_FAIL)
    {
        ::AfxMessageBox(IDS_PRINT_FAILED);
    }
	UpdateAllTabFileViews();
}
// OSAI feature print XRef 24.03.04 SIS <<


void CMainFrame::OnUpdatePrintFile(CCmdUI* pCmdUI) 
{
    CFrameWnd*   pMainWnd = (CFrameWnd*)AfxGetMainWnd();
    if(!pMainWnd)
    {
        pCmdUI->Enable(FALSE);
        return;
    }
    CFrameWnd*  pFrameWnd = (CFrameWnd*) pMainWnd->GetActiveFrame();
    if(!pFrameWnd)
    {
        pCmdUI->Enable(FALSE);
        return;
    }

    CView* pView = dynamic_cast<CGenEdView*> (pFrameWnd->GetActiveView());
    if(pView)
    {
        CGenEdDoc*  pDoc = (CGenEdDoc*)pView->GetDocument();
        CString     strFilePath = pDoc->GetPathName();
        if(m_tDocuManager.IsFilePrintable(strFilePath))
        {
            pCmdUI->Enable(TRUE);
            return;
        }
    }
    pCmdUI->Enable(FALSE);
}


// OSAI feature print XRef 24.03.04 SIS >>
void CMainFrame::OnUpdatePrintXRef(CCmdUI* pCmdUI) 
{
    BOOL            bEnable(FALSE);
    CCEMAINApp*     pApp = (CCEMAINApp*)AfxGetApp();
    CSettings* pSettings = pApp->GetSettings();
    if (pSettings)
    {
        bEnable = !pSettings->m_engineeringSettings.m_bXRefDeactivated;
    }
    pCmdUI->Enable(bEnable);
}
// OSAI feature print XRef 24.03.04 SIS <<



void CMainFrame::SetVarSelectionHint(const CString& strFileName)
{
    // MDI child window changed
    // inform var selection
    CPouVarSelWindow* pPouVarSel = NULL;
    pPouVarSel = GetPouVarSel(FALSE);
    if (!pPouVarSel)
    {
        return;
    }
    CPouVarSelCtrlWrapper* pPouVarSelWrapper = NULL;
    pPouVarSelWrapper = pPouVarSel->GetWrapper();
    if (pPouVarSelWrapper)
    {
        pPouVarSelWrapper->SetVarSelectionHint(strFileName, _T(""), _T(""));
    }
}

void CMainFrame::OnFindInFiles() 
{
     FindInFiles();
}

void CMainFrame::OnUpdateFindInFiles(CCmdUI* pCmdUI) 
{
    UpdateFindInFiles(pCmdUI);
}


void CMainFrame::TerminateFindInFilesThread()
{
}

void CMainFrame::FindInFiles()
{
    HRESULT hr;

    CComPtr<ICEFindInFiles> pIFindInFiles;

    if(!GetFindInFiles(&pIFindInFiles))
    {
        return;
    }

    // FIF Dialog
    CFindfileDlg tDlg;
    CGenEdDoc*  pDoc = NULL;
    CGenEdCont* pContainer = GetContainer();
    ASSERT(pContainer);
    if(!pContainer)
    {
        return;
    }

    pDoc = pContainer->GetActiveDocument();
    if(pDoc)
    {
        pDoc->GetSelectedText(tDlg.m_strFindText);
    }

    // Save all open file views
    SaveAllFiles(true);

    if (tDlg.DoModal() != IDOK)
    {
        return;
    }

    CMsgWnd*    pMessageWindow = GetMsgView();

    if (pMessageWindow != NULL)
    {
        ShowControlBar(pMessageWindow, TRUE, FALSE);
        pMessageWindow->ClearFileFindWindow (true);
    }

    CComBSTR    sExpression(tDlg.m_strFindText);
    CComBSTR    sProjPath;

    CComPtr<ICEProjInfo> pICEProjInfo; 
    GetProjectInfo(&pICEProjInfo);

    // set find in files toolbar button state
    if(!pICEProjInfo)
    {
        return;
    }

    hr = pICEProjInfo->getProjectPath(&sProjPath);
    if(hr != S_OK)
    {
        return;
    }

    // start find (this will stop the current thread, if running)
    pIFindInFiles->StartFind(sExpression, sProjPath, tDlg.m_uiOption);
}

void CMainFrame::UpdateFindInFiles(CCmdUI* pCmdUI)
{
	CComPtr<ICEProjInfo> pICEProjInfo;
    GetProjectInfo(&pICEProjInfo);
    CComPtr<ICEFindInFiles> pIFindInFiles;
    GetFindInFiles(&pIFindInFiles);

    // set find in files toolbar button state
    if (pICEProjInfo)
    {
        CComBSTR    sProjName;
        HRESULT hr = pICEProjInfo->getProjectName(&sProjName);
        if(hr == S_OK)
        {
            pCmdUI->Enable(TRUE);
        }
        else
        {
            pCmdUI->Enable(FALSE);
        }

        BOOL  bFindInFilesActive = FALSE;
        if(pIFindInFiles)
        {
            pIFindInFiles->IsFindActive(&bFindInFilesActive);
        }
        pCmdUI->SetCheck(bFindInFilesActive);
    }
}


CGenEdCont *CMainFrame::GetContainer()
{
    if(m_pContainer)
    {
        return m_pContainer;
    }

	CCEMAINApp *pApp;

	pApp = dynamic_cast<CCEMAINApp *> (AfxGetApp ());
	assert (pApp != NULL);
	if (pApp == NULL)
	{
		return (NULL);
	}

	m_pContainer = pApp->GetContainer ();
	assert (m_pContainer != NULL);

	return (m_pContainer);
}


CGenEdDoc *CMainFrame::ActivateContainerFileView (const CString &strName, LPCTSTR pszPosition)
{
    HRESULT hr;
    CCEFileHandler *pFileHandler;
    CGenEdDoc *pDoc;

    pFileHandler = GetFileHandler ();
    if (pFileHandler == NULL)
    {
        return (NULL);
    }

    hr = pFileHandler->ShowFile(strName, pDoc);
    if (FAILED (hr))
    {
        return (NULL);
    }
    //ASSERT (pDoc != NULL);  // Null can be returned now 
                    // ==> the file could not be opend in ATCM, 
                    // if possible an extern editor has been opened
    if (pDoc == NULL)
        return (NULL);

    if (pszPosition != NULL)
    {
        CGenEdCont *pContainer;

        pContainer = GetContainer ();
        if (pContainer != NULL)
        {
            pContainer->ShowLocation (pDoc, pszPosition);
        }
    }

    return (pDoc);
}


BOOL CMainFrame::ActivateFileView (const CString &strName, LPCTSTR pszPosition)
{
	CGenEdDoc *pDoc;

    // special operation for project settings file

//    if (!strName.CompareNoCase(CE_PSET_FILE))
//    {
//        CPrjDoc * pPrjDoc = GetProjectDocument();
//        pPrjDoc->ShowOptions();
//        return TRUE;
//    }

    pDoc = ActivateContainerFileView (strName, pszPosition);
    if (pDoc == NULL)
    {
        return (FALSE);
    }

    return (TRUE);
}


CCEFileHandler *CMainFrame::GetFileHandler ()
{
	CCEMAINApp *pApp;
	CCEFileHandler *pFileHandler;

	pApp = dynamic_cast<CCEMAINApp *> (AfxGetApp());
	assert (pApp != NULL);
	if (pApp == NULL)
	{
		return (NULL);
	}

	pFileHandler = pApp->GetFileHandler ();
	assert (pFileHandler != NULL);

	return (pFileHandler);
}


BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
	{
		// translate accelerators
		if (m_hKADAccel != NULL &&
			::TranslateAccelerator(m_hWnd, m_hKADAccel, pMsg))
		{
			return TRUE;
		}
    }
	
	return SECMDIFrameWnd::PreTranslateMessage(pMsg);
}




void CMainFrame::OnViewFullscreen() 
{
    ToggleViewFullScreen();	
}

void CMainFrame::OnUpdateViewFullscreen(CCmdUI* pCmdUI) 
{
    pCmdUI->SetCheck(m_bViewFullScreen);
}

void CMainFrame::ToggleViewFullScreen()
{
    if(m_bViewFullScreen)
    {
        UINT uiVisFlags = GetDockingToolsVisibilityFlags();
        m_bViewFullScreen = FALSE;
        uiVisFlags |= m_uiVisFlags;
        ShowDockingTools(uiVisFlags);
        MaximizeMDIWindows(m_bMaximized);
    }
    else
    {
        m_uiVisFlags = GetDockingToolsVisibilityFlags();
        ShowDockingTools(0);
        m_bViewFullScreen = TRUE;
        m_bMaximized = MaximizeMDIWindows(TRUE);
    }
}

//------------------------------------------------------------------*
/**
 * get docking tools visibility flags.
 *
 *  used for full screen to store visible docking tools
 *
 * @param           -
 * @return          visibility flags
 * @exception       -
 * @see             ToggleViewFullScreen()
*/
UINT CMainFrame::GetDockingToolsVisibilityFlags()
{
    UINT    uiVisFlags = 0;
    if(m_bShowNavigator)
    {
        uiVisFlags |= F_DOCKING_NAV;
    }
    if(m_bShowMsgView)
    {
        uiVisFlags |= F_DOCKING_MESSAGE;
    }
    if(m_bShowPouVarSel)
    {
        uiVisFlags |= F_DOCKING_POU;
    }
    if(m_bShowWatch)
    {
        uiVisFlags |= F_DOCKING_WATCH;
    }
    if(m_bShowBPView)
    {
        uiVisFlags |= F_DOCKING_BRKPT;
    }
    if(m_bShowXRef)
    {
        uiVisFlags |= F_DOCKING_XREF;
    }
    return uiVisFlags;
}


//------------------------------------------------------------------*
/**
 * show docking tools.
 *
 * @param           [in] uiVisFlags: docking tools visibility flags
 * @return          -
 * @see             ToggleViewFullScreen()
*/
void CMainFrame::ShowDockingTools(UINT uiVisFlags)
{
    BOOL    bVisible;
    bVisible = ((uiVisFlags & F_DOCKING_NAV) == F_DOCKING_NAV);
    if(m_bShowNavigator != bVisible)
    {
        OnToggleNavigator();
    }
    bVisible = ((uiVisFlags & F_DOCKING_MESSAGE) == F_DOCKING_MESSAGE);
    if(m_bShowMsgView != bVisible)
    {
        OnToggleMsgView();
    }
    bVisible = ((uiVisFlags & F_DOCKING_POU) == F_DOCKING_POU);
    if(m_bShowPouVarSel != bVisible)
    {
        OnTogglePouSelection();
    }
    bVisible = ((uiVisFlags & F_DOCKING_WATCH) == F_DOCKING_WATCH);
    if(m_bShowWatch != bVisible)
    {
        OnToggleWatchView();
    }
    bVisible = ((uiVisFlags & F_DOCKING_BRKPT) == F_DOCKING_BRKPT);
    if(m_bShowBPView != bVisible)
    {
        OnToggleBPView();
    }
    bVisible = ((uiVisFlags & F_DOCKING_XREF) == F_DOCKING_XREF);
    if(m_bShowXRef != bVisible)
    {
        OnToggleXRef();
    }
}

void CMainFrame::OnBreak()
{
    // stop find in files and exe delegator processes
    CComPtr<ICEFindInFiles> pIFindInFiles;
    GetFindInFiles(&pIFindInFiles);
    if(pIFindInFiles)
    {
        pIFindInFiles->StopFind();
    }

//    CComPtr<IUnknown>       pTmp;
//    CComQIPtr<ICEExecute>   pExeDelegator;
//
//    pTmp.Attach(CEGetCOM(CE_COMPMAN_EXE));
//
//    pExeDelegator = pTmp;
//    if(pExeDelegator)
//    {
//        pExeDelegator->CloseSession(m_dwLastExeSession, FALSE);
//    }
}


void CMainFrame::OnCloseAllWindows()
{
	CGenEdCont *pContainer;

	pContainer = GetContainer ();
	if(pContainer)
	{
        if(pContainer->SaveAll())
        {
            pContainer->CloseAll();
        }
	}
}


void CMainFrame::OnUpdateCloseAllWindows(CCmdUI* pCmdUI) 
{
    BOOL    bEnable(FALSE);
	CGenEdCont *pContainer;

	pContainer = GetContainer ();
	if(pContainer && pContainer->HaveOpenDocuments())
	{
        bEnable = TRUE;
	}

    pCmdUI->Enable(bEnable);
}


BOOL CMainFrame::UpdateAllTabFileViews ()
{
	CGenEdCont *pContainer;

	pContainer = GetContainer ();
	if (pContainer == NULL)
	{
		return (TRUE);
	}

	pContainer->UpdateTabs ();
	return (TRUE);
}


HACCEL CMainFrame::GetAccelerators()
{
    return m_hAccelTable;
}

void CMainFrame::OnDropFiles(HDROP hDropInfo)
{
    CNavigator* pNavigator = dynamic_cast<CNavigator*>(CEGetDockingTool(CE_COMPMAN_NAV, false));
    ASSERT(pNavigator);
    if(pNavigator)
    {
        pNavigator->OnDropFiles(hDropInfo);
    }
}

void CMainFrame::OnUpdateFrameTitle (BOOL bAddToTitle)
{
    if ((GetStyle() & FWS_ADDTOTITLE) == 0)
    {
		return;     // leave it alone!
    }

    CGenEdCont *pContainer;

	pContainer = GetContainer ();
	if (pContainer == NULL)
	{
		return;
	}

    CMDIChildWnd* pActiveChild = NULL;
    DWORD dwStyle;
    pActiveChild = MDIGetActive();
    if(pActiveChild)
    {
        dwStyle = pActiveChild->GetStyle();
    }

    CGenEdDoc* pDocument =  (CGenEdDoc*) pContainer->GetActiveDocument();

    //TCHAR szText[256+_MAX_PATH];
    CString strText;

    if (bAddToTitle && pActiveChild != NULL && !(dwStyle & WS_MAXIMIZE))
    {    
        if (pDocument == NULL)
        {
            CComBSTR    sProjPath;
            CComBSTR    sProjName;
            CString     strHelp =_T("");
            CString     strHelp1 =_T("");
            CComPtr<ICEProjInfo> pICEProjInfo; 
            GetProjectInfo(&pICEProjInfo);
            // set find in files toolbar button state
            if(!pICEProjInfo)
            {
                return;
            }
            HRESULT hr = pICEProjInfo->getProjectPath(&sProjPath);
            if(hr != S_OK)
            {
                return;
            }
            hr = pICEProjInfo->getProjectName(&sProjName);
            if(hr != S_OK)
            {
                return;
            }

            strHelp = sProjPath;
            strHelp1 = sProjName;
            if(m_bProjectReadOnly)
            {
                CString strReadOnly;
                strReadOnly.LoadString(IDS_READ_ONLY);
                strText.Format("%s - %s\\%s.4cp (%s)",m_strTitle, strHelp, strHelp1, strReadOnly);
            }
            else
            {
                strText.Format("%s - %s\\%s.4cp",m_strTitle, strHelp, strHelp1);
            }
        }
        else
        {//ask if the document is modified
            BOOL bMod = pDocument->GetModified();
            if(bMod)
            {
                strText = m_strTitle + _T(" - ") + pDocument->GetPathName() + _T(" *");
            }
            else
            {
                strText = m_strTitle + _T(" - ") + pDocument->GetPathName();
            }
        }
        
        //if (m_nWindow > 0)
        //{
        //    wsprintf(szText + lstrlen(szText), _T(":%d"), m_nWindow);
        //}
        
        // set title if changed, but don't remove completely
        AfxSetWindowText(m_hWnd, strText);
    }
    else
    {
        AfxSetWindowText(m_hWnd, m_strTitle);
    }
}


void CMainFrame::OnPrintProject()
{
    if(m_tDocuManager.PrintProject() == E_FAIL)
    {
        ::AfxMessageBox(IDS_PRINT_FAILED);
    }
}




// SIS: 28.09.01: adapted from V1.3
BOOL CMainFrame::OnIdle(LONG lCount)
{
    if (m_bCheckDownloadGUIDs)
    {
        CheckDownloadGUIDs();
        return (FALSE);
    }

    if(m_bShowNavigator)
    {
        CNavigator * pNavigator = GetNavigator();
        if(pNavigator)
        {
            pNavigator->OnIdle(lCount);
        }
    }

    CheckProjectReadOnly();

    if (m_bHandleForcedLogouts)
    {
        HandleForcedLogouts ();
        return (FALSE);
    }

    return (FALSE);
}



//------------------------------------------------------------------*
/**
 * check project read only.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CMainFrame::CheckProjectReadOnly()
{
    if(!m_bProjectLoaded)
    {
        return;
    }

    CFileStatus tFileStatus;
    if(CFile::GetStatus(m_strProjFilePathName, tFileStatus))
    {
        if(tFileStatus.m_attribute & CFile::readOnly)
        {
            m_bProjectReadOnly = TRUE;
        }
        else
        {
            m_bProjectReadOnly = FALSE;
        }
    }
}


// SIS: 11.12.01: adapted from V1.3
void CMainFrame::OnSoftHomepage() 
{
    CString strHome;
    strHome.LoadString(IDS_SOFING_HOMEADDR);
    //CreatePanelView(strHome);
    ShellExecute(NULL, "open", strHome, NULL, NULL, SW_SHOWNORMAL);
}

//------------------------------------------------------------------*
/**
 * check download guids.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CMainFrame::CheckDownloadGUIDs ()
{
    POSITION pPosition;
    TTargOnlList checkNow;

    m_bCheckDownloadGUIDs = false;

    pPosition = m_DownloadGUIDCheckPCs.GetHeadPosition ();
    while (pPosition != NULL)
    {
        CTargetOnl* pTargetOnl;
        POSITION pRemovePosition=NULL;

        pTargetOnl = m_DownloadGUIDCheckPCs.GetAt(pPosition);
        ASSERT (pTargetOnl != NULL);

        if (pTargetOnl)
        {
            checkNow.AddTail(pTargetOnl);
            pRemovePosition = pPosition;
        }

        m_DownloadGUIDCheckPCs.GetNext(pPosition);

        if(pRemovePosition != NULL)
        {
            m_DownloadGUIDCheckPCs.RemoveAt(pRemovePosition);
        }
    }

    pPosition = checkNow.GetHeadPosition();
    while(pPosition != NULL)
    {
        HRESULT hr;
        CTargetOnl*             pTargetOnl;
        CComPtr<IBreakPoint>    pBreakPoint;
        CString                 strResource;
        GUIDMatchCode           guidMatchCode;
        CString                 strMessage;
        bool                    bSuccess;
        CString                 strRemoteProjectName;
        CString                 strRemoteDownloadDate;
        CString                 strRemoteDownloadHost;

        pTargetOnl = checkNow.GetNext(pPosition);

        guidMatchCode = CheckDownloadGUID(pTargetOnl);
        switch(guidMatchCode)
        {
        case GUIDMatchGood:
            break;

        case GUIDMatchUnknown:
            strMessage.Format(IDS_DEBUG_UNKNOWN_DLSTATE, (LPCTSTR)pTargetOnl->GetId(), (LPCTSTR)pTargetOnl->GetControlAddress());
            AfxMessageBox(strMessage);
            break;

        case GUIDMatchNotBuilt:
            strMessage.Format(IDS_DEBUG_WITHOUT_BUILD, (LPCTSTR)pTargetOnl->GetId(), (LPCTSTR)pTargetOnl->GetControlAddress());
            AfxMessageBox(strMessage);
            break;

        case GUIDMatchNoDownload:
            strMessage.Format(IDS_DEBUG_WITHOUT_PROJECT, (LPCTSTR)pTargetOnl->GetId(), (LPCTSTR)pTargetOnl->GetControlAddress());
            AfxMessageBox(strMessage);
            break;

        case GUIDMatchNotDownloaded:
            strMessage.Format(IDS_DEBUG_WITHOUT_DOWNLOAD, (LPCTSTR)pTargetOnl->GetId(), (LPCTSTR)pTargetOnl->GetControlAddress());
            AfxMessageBox(strMessage);
            break;

        case GUIDMatchDifferentDownload:
            bSuccess = pTargetOnl->GetProjectName(strRemoteProjectName);
            ASSERT(bSuccess);
            if(!bSuccess)
            {
                strRemoteProjectName = "unavailable";
            }

            bSuccess = pTargetOnl->GetDownloadDate(strRemoteDownloadDate);
            ASSERT(bSuccess);
            if(!bSuccess)
            {
                strRemoteDownloadDate = "unavailable";
            }

            bSuccess = pTargetOnl->GetDownloadHost(strRemoteDownloadHost);
            ASSERT(bSuccess);
            if(!bSuccess)
            {
                strRemoteDownloadHost = "unavailable";
            }

            strMessage.Format(IDS_DEBUG_ON_DIFFERENT_PROJECT,
                              (LPCTSTR)pTargetOnl->GetId(), (LPCTSTR)pTargetOnl->GetControlAddress(),
                              (LPCTSTR)strRemoteProjectName, (LPCTSTR)strRemoteDownloadDate, (LPCTSTR)strRemoteDownloadHost);

            AfxMessageBox(strMessage);
            break;
        }


        strResource = pTargetOnl->GetAssignedResource();
        IUnknown *pUnknown;

        pUnknown = CEGetCOM(CE_COMPMAN_BREAKPTMAN, false);
        if(pUnknown != NULL)
        {
            hr = pUnknown->QueryInterface(IID_IBreakPoint, (LPVOID *)&pBreakPoint);
            ASSERT(SUCCEEDED(hr));
            //do not forget to release the reference returned by component manager.
            pUnknown->Release(); 

            if(SUCCEEDED(hr))
            {
                ASSERT(pBreakPoint != NULL);
                if(pBreakPoint != NULL)
                {
                    CComBSTR    sTargetId(pTargetOnl->GetId());
                    switch(guidMatchCode)
                    {
                    case GUIDMatchUnknown:
                    case GUIDMatchDifferentDownload:
                        pBreakPoint->DeactivateAllConfigBPs(sTargetId);
                        break;

                    case GUIDMatchGood:
                        pBreakPoint->Synchronize(sTargetId);
                    }
                }
            }
        }
    }
}


CMainFrame::GUIDMatchCode CMainFrame::CheckDownloadGUID(CTargetOnl *pTargetOnl)
{
    GUIDMatchCode   eCodeRet = GUIDMatchUnknown;
    HRESULT     hr;
    bool        bSuccess;
    CString     strRemoteDownloadGUID;
    CString     strAddress;
    CString     strLocalDownloadGUID;
    CString     strLocalProjName;
    CDownload*  pDownload = NULL;
    bool        bDownloaderCreated(false);
    CTargetSet* pTargetSet = CEGetTargetSet(_T("Project Targets"), false);
    BOOL        bCompiled;
    BOOL        bDownloaded;

    GetTargetCompileDownloadState(pTargetOnl->GetId(), bCompiled, bDownloaded);
    if(!bCompiled)
    {
        return(GUIDMatchNotBuilt);
    }
    if(!bDownloaded)
    {
        return(GUIDMatchNotDownloaded);
    }

    if(!pTargetSet)
    {
        return(GUIDMatchUnknown);
    }

    pDownload = pTargetSet->GetDownloader(pTargetOnl->GetId());
    if(!pDownload)
    {
        return(GUIDMatchNoDownload);
    }

    bSuccess = pTargetOnl->GetDownloadGUID(strRemoteDownloadGUID);
    ASSERT(bSuccess);
    if(!bSuccess)
    {
        return(GUIDMatchUnknown);
    }

    hr = pDownload->Create(CEDL_DOWNLD_SELECT);
    if(hr == S_OK)
    {
        bDownloaderCreated = TRUE;
    }

    // downloader is created now
    if(strRemoteDownloadGUID.IsEmpty() ||
        strRemoteDownloadGUID.CompareNoCase("{00000000-0000-0000-0000-000000000000}") == 0)
    {
        eCodeRet = GUIDMatchNoDownload;
    }
    else
    {
        strAddress = pTargetOnl->GetControlAddress();
        ASSERT(!strAddress.IsEmpty());
        if(!strAddress.IsEmpty())
        {
            if(bDownloaderCreated)
            {
                hr = pDownload->GetTargetDLVersion(strLocalDownloadGUID, strLocalProjName);
            }
            if(strLocalDownloadGUID.CompareNoCase(strRemoteDownloadGUID) != 0)
            {
                eCodeRet = GUIDMatchDifferentDownload;
            }
            else
            {
                eCodeRet = GUIDMatchGood;
            }
        }
    }
    pDownload->Release();
    return eCodeRet;
}

DROPEFFECT CMainFrame::OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
    // bring window to front
    SetForegroundWindow();
    return DROPEFFECT_NONE;
}


//------------------------------------------------------------------*
/**
 * maximize windows.
 *
 * @param           bMaximize: TRUE: maximize, FALSE: show normal
 * @return          old state: maximized?
 * @exception       -
 * @see             -
*/
BOOL CMainFrame::MaximizeMDIWindows(BOOL bMaximize /*=TRUE*/)
{
    BOOL    bMaximized;

	CMDIChildWnd* pMDIChild	= MDIGetActive(&bMaximized);
    if(!pMDIChild)
    {
        return FALSE;
    }

    if(bMaximize)
    {
        if(!bMaximized)
        {
            pMDIChild->ShowWindow(SW_SHOWMAXIMIZED);
        }
    }
    else
    {
        if(bMaximized)
        {
            pMDIChild->ShowWindow(SW_SHOWNORMAL);
        }
    }
    return bMaximized;
}

//------------------------------------------------------------------*
/**
 * get MDI maximize state.
 *
 * @param           -
 * @return          is active MDI window maximized?
 * @exception       -
 * @see             -
*/
BOOL CMainFrame::GetMDIMaximizeState()
{
    BOOL    bMaximized;

	MDIGetActive(&bMaximized);

    return bMaximized;
}


//------------------------------------------------------------------*
/**
 * on continue task.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CMainFrame::OnContinueTask()
{
    // if project not loaded -> return
    if(!m_bProjectLoaded)
    {
        return;
    }

    // check, whether any target is connected exclusively
    CTargetSet* pTargetSet = CEGetTargetSet("Project Targets", false);
    if(!pTargetSet)
    {
        return;
    }
    if(pTargetSet->GetTargetCountConnectedEx() == 0)
    {
        return;
    }

    // get active instance name
    CString strInstanceName;
    CGenEdDoc*  pDoc = NULL;

    CGenEdCont* pGenEdCont = CEGetContainer();
    if(!pGenEdCont)
    {
        return;
    }
    pDoc = pGenEdCont->GetActiveDocument();
    if(pDoc)
    {
        CString strMachineName;
        CString strMachineAddress;
        CTargetOnlBase* pTarget = NULL;

        pDoc->GetInstance(strInstanceName, strMachineName, strMachineAddress);
        pTarget = pTargetSet->GetTarget(strMachineName);
        if(!pTarget)
        {
            return;
        }
        if(!pTarget->IsConnectedExclusive())
        {
            return;
        }
    }

    // get breakpoint manager
    HRESULT hr;
    IUnknown *pUnknown;
    CComPtr<IBreakPoint>    pBreakPoint;

    pUnknown = CEGetCOM(CE_COMPMAN_BREAKPTMAN, false);
    if(pUnknown != NULL)
    {
        hr = pUnknown->QueryInterface(IID_IBreakPoint, (LPVOID *)&pBreakPoint);
        ASSERT(SUCCEEDED(hr));
        //do not forget to release the reference returned by component manager.
        pUnknown->Release(); 

        if(SUCCEEDED(hr))
        {
            CComBSTR    sInstanceName(strInstanceName);
            hr = pBreakPoint->ContinueTask(sInstanceName, 0);
        }
    }
}


void CMainFrame::DoSettingsPrint() 
{
    // get print settings
    CCEMAINApp* pApp = (CCEMAINApp*) AfxGetApp();
    CSettings*  pSettings = pApp->GetSettings();
    if(!pSettings)
    {
        return;
    }

    CPrintSettingsDlg   tPrintSettingsDlg(&pSettings->m_printSettings, this);
    tPrintSettingsDlg.DoModal();
}


CKADFileInfo* CMainFrame::GetKADFileInfo()
{
    return m_pFileInfo;
}


BOOL CMainFrame::GetPrintInfo(CString &strPrinterName, long* plWidth, long* plHeight)
{
    return m_tDocuManager.GetPrintInfo(strPrinterName, plWidth, plHeight);
}


BOOL CMainFrame::IsProjectReadOnly()
{
    return m_bProjectReadOnly;
}

void CMainFrame::OnNextError() 
{
    CMsgWnd*    pMsgWnd = GetMsgView();
    if(pMsgWnd)
    {
        BOOL    bVisible = pMsgWnd->IsVisible();
        if(!bVisible)
        {
            ShowControlBar(pMsgWnd, TRUE, FALSE);
            ::SetFocus(pMsgWnd->m_hWnd);
        }
        pMsgWnd->ShowNextError();
    }
}

void CMainFrame::OnPreviousError() 
{
    CMsgWnd*    pMsgWnd = GetMsgView();
    if(pMsgWnd)
    {
        BOOL    bVisible = pMsgWnd->IsVisible();
        if(!bVisible)
        {
            ShowControlBar(pMsgWnd, TRUE, FALSE);
            ::SetFocus(pMsgWnd->m_hWnd);
        }
        pMsgWnd->ShowPreviousError();
    }
}


void CMainFrame::OnTargetLoginNotify ()
{
    CCEStateManager *pStateManager;
    CTargetOnlBase *pTarget;
    CString strAddress;
    CString strUser;
    CString strHost;
    int iIndex;

    pStateManager = CEGetStateManager ();
    if (pStateManager == NULL)
        return;

    pTarget = pStateManager->GetTarget ();
    ASSERT (pTarget != NULL);
    if (pTarget == NULL)
        return;

    strAddress = pTarget->GetControlAddress ();

    strUser = pStateManager->GetTargetLoginUser ();
    strHost = pStateManager->GetTargetLoginHost ();

    if (!pStateManager->IsTargetLogin ())
    {

        // If we are being logged out and the session state
        // is not exclusive_established any more it must have
        // been ourselves that performed the logout.

        if (pTarget->GetConnectState () != eTOCS_connected_ex)
            return;


        // Check if the forced logout is already recorded.

        for (iIndex = 0; iIndex < m_ForcedLogouts.GetSize (); ++iIndex)
        {
            if (strAddress.CompareNoCase(m_ForcedLogouts[iIndex].m_strMachine) == 0)
                break;
        }
    

        // If the forced logout is not yet recorded add a new record.
   
        if (iIndex == m_ForcedLogouts.GetSize ())
        {
            ForcedLogout forcedLogout;

            forcedLogout.m_strMachine = strAddress;
            forcedLogout.m_strPreviousUser = strUser;
            forcedLogout.m_strPreviousHost = strHost;

            m_ForcedLogouts.Add(forcedLogout);
        }


        m_bHandleForcedLogouts = TRUE;

        return;
    }


    // Lookup the corresponding forced logout entry.
    // If there is no such entry do not continue.

    for (iIndex = 0; iIndex < m_ForcedLogouts.GetSize (); ++iIndex)
    {
        if (strAddress.CompareNoCase (m_ForcedLogouts[iIndex].m_strMachine) == 0)
            break;
    }
    if (iIndex == m_ForcedLogouts.GetSize ())
        return;


    // If the login came from ourselves remove
    // the corresponding forced logout entry.

    if (strHost.CompareNoCase (strAddress) == 0)
    {
        if (iIndex < m_ForcedLogouts.GetSize ())
        {
            m_ForcedLogouts.RemoveAt (iIndex);
        }

        return;
    }


    // Wanted to update the forced logout record with
    // info about who logged out ourselves. Problem
    // is that the notification comes too late (after
    // the posted message arrives) and the forced
    // logout record is already handled and removed
    // by OnHandleForcedLogouts. Maybe defer handling
    // of forced logout records until the notification
    // arrives. But who knows if the notification ever
    // arrives. Need a timeout then. Defer this until
    // there is time to do it.

    m_ForcedLogouts[iIndex].m_strNewUser = strUser;
    m_ForcedLogouts[iIndex].m_strNewHost = strHost;

}


void CMainFrame::HandleForcedLogouts ()
{
    while (m_ForcedLogouts.GetSize () != 0)
    {
        ForcedLogout forcedLogout;
        CTargetSet *pProjectTargets;
        CTargetOnlBase *pLogoutTarget=NULL;
        POSITION pPosition;

        forcedLogout = m_ForcedLogouts[0];
        m_ForcedLogouts.RemoveAt (0);

        pProjectTargets = CEGetTargetSet (_T("Project Targets"));
        ASSERT (pProjectTargets != NULL);
        if (pProjectTargets == NULL)
            continue;

        pPosition = pProjectTargets->GetFirstTargetPosition ();
        while (pPosition != NULL)
        {
            CTargetOnlBase *pTarget=NULL;

            pTarget = pProjectTargets->GetNextTarget (pPosition);
            ASSERT (pTarget != NULL);
            if (pTarget == NULL)
                continue;

            if (forcedLogout.m_strMachine.CompareNoCase (pTarget->GetControlAddress ()) == 0)
            {
                pLogoutTarget = pTarget;
                break;
            }
        }

        if (pLogoutTarget != NULL)
        {
            CString strMessage;

            strMessage.Format (IDS_FORCED_DEBUG_DISCONNECT, pLogoutTarget->GetId (), pLogoutTarget->GetControlAddress ());
            AfxMessageBox (strMessage);

            pLogoutTarget->DisconnectExclusive ();
        };
    };


    // As long as program execution did not leave the loop
    // above forced logout records may be added to the array
    // and these records are handled in the loop. As soon as
    // program execution leaves the loop newly added forced
    // logout records require another handling cycle in idle
    // loop.    
    m_bHandleForcedLogouts = FALSE;
}


// XREFPRINT 20.07.05 SIS >>
CCEDocuManager*	CMainFrame::GetDocuManager()
{
	return &m_tDocuManager;
}
// XREFPRINT 20.07.05 SIS <<



