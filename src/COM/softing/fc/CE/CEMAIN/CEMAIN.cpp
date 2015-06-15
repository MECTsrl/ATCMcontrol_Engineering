
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*

// CEMAIN.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "utilif.h"
#include "CESysDef.h"
//state and component manager
#include "cestateman.h"
#include "cecompman.h"
#include "msgdoc.h"
#include "msgview.h"
#include "msgframe.h"

#include "CEMAIN.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "CEMAINDoc.h"
#include "CEMAINView.h"
#include <comdef.h>
#include "msgrc.h"
#include "CEAddonAction.h"
#include "CEDragDrop.h"
#include "GenEdCont.h"
#include "GenEdDocTempl.h"
#include "CEFileHandler.h"
#include "Splash.h"
#include "WatchWnd.h"
#include "CEProjMan.h"
#include "PouVarSelWindow.h"
#include "htmlhelp.h"
#include "fc_tools\fc_fileIO.h"
#include "XMLHelper\XMLNodeList.h"
#include "XMLHelper\XMLDocument.h"
#include "XMLHelper\XMLNode.h"
#include "panel.h"
#include "panelvw.h"
#include "panelrc.h"
#include "KADHelper/KADInfo.h"
#include "Settings.h"
#include "ProjSaveAsDialog.h"
#include "XRefWindow.h"
#include "SetProcAffinity.h"
#include <assert.h>

/**
* include any CLSID *_i.c files here
*/
#include "KadManager_i.c"
#include "CEProjMan_i.c"
#include "CEAddonAction_i.c"
#include "CEDragDrop_i.c"
#include "BrkPtMan.h"
#include "BrkPtMan_i.c"
#include "ExeDelegator.h"
#include "ExeDelegator_i.c"
#include "FindInFiles.h"
#include "FindInFiles_i.c"

#include "printing.h"

#include "traceif.h"

#define FC_LIB_USE_TODEBUG
#include "fc_tools\fc_libmem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static void AppendFilterSuffix(CString& filter, OPENFILENAME& ofn,
	CDocTemplate* pTemplate, CString* pstrDefaultExt);

static int CopyFilesCB(void* pCtx, void* pCtx2, int bIsDir, int bOnError, const TCHAR* pszFile);



/////////////////////////////////////////////////////////////////////////////
// CCEMAINApp

BEGIN_MESSAGE_MAP(CCEMAINApp, CWinApp)
	//{{AFX_MSG_MAP(CCEMAINApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
    ON_COMMAND(ID_HELP_FINDER, OnHelpFinder)
    ON_COMMAND(ID_HELP, CWinApp::OnHelp) 
    ON_COMMAND(ID_CONTEXT_HELP, CWinApp::OnContextHelp) 
	//}}AFX_MSG_MAP
    ON_COMMAND(ID_FILE_NEW, OnFileNew)
    ON_COMMAND(IDM_FILE_NEW, OnFileNew)
    ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
    ON_COMMAND(IDM_FILE_OPEN, OnFileOpen)
    // SIS, 06.02.01: open / new project
	ON_COMMAND(IDM_PROJECT_OPEN, OnProjectOpen)	// 13.12.06 SIS
	ON_COMMAND(IDM_PROJECT_NEW, OnFileNew)
    ON_COMMAND(IDM_PROJECT_CLOSE, OnProjectClose)
    ON_COMMAND(IDM_PROJECT_SAVE, OnProjectSave)
    ON_COMMAND(IDM_PROJECT_SAVE_AS, OnProjectSaveAs)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, OnFilePrintSetup)
    ON_UPDATE_COMMAND_UI(IDM_PROJECT_NEW, OnUpdateIsProjectNotOpen)
    ON_UPDATE_COMMAND_UI(IDM_PROJECT_OPEN, OnUpdateEnableAlways)		// 13.12.06 SIS
    ON_UPDATE_COMMAND_UI(IDM_PROJECT_CLOSE, OnUpdateIsProjectOpen)
    ON_UPDATE_COMMAND_UI(IDM_PROJECT_SAVE, OnUpdateIsProjectOpen)
//    ON_UPDATE_COMMAND_UI(IDM_PROJECT_PRINT, OnUpdateIsProjectOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCEMAINApp construction

CCEMAINApp::CCEMAINApp()
:   m_pFileHandler(NULL),
    m_pContainer(NULL),
    m_pKADInfo(NULL),
    m_pSettings(NULL),
    m_pMsgTemplate(NULL),
    m_pStateManager(NULL),
    m_pComponentManager(NULL),
    m_pPrjTemplate(NULL),
    m_pPanelTemplate(NULL),
    m_bLaunch(FALSE),
    //m_strWndClassName(_T("CEMainWndClass")),   // V1.3
    m_strWndClassName(_T("CEMainWindowClass")),  // V2.0
    m_bShuttingDown (false),
	m_bOpeningProject(FALSE)	// 09.11.04 SIS
{
    // Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCEMAINApp object

CCEMAINApp theApp;

/////////////////////////////////////////////////////////////////////////////
// The one and only _Module for ATL
CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
END_OBJECT_MAP()

//------------------------------------------------------------------*
/**
 * CCEMAINApp initialization.
 * NOTE: Do not call CoInitialize directly, AfxOleInit will handle
 * gracefully for you.
 *
 * @return          TRUE - Initialization successful
 *                  FALSE - Initialization failed
 *
 * @see             ExitInstance()
*/
BOOL CCEMAINApp::InitInstance()
{
    FC_SetProcAffinity(2); //EF 23.01.07 HACK: avoid any problems with dual core CPUs

    try {
	    // Initialize COM
        _Module.Init(ObjectMap, m_hInstance);

        InitializeCOM();

        InitTrace(m_hInstance, _T("SOFTWARE\\SOFTING\\4ControlV2\\"), _T("4CEng"));

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	    Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	    Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

        // SIS, 22.01.01: own registry key for new version
	    SetRegistryKey(_T("Softing\\4CONTROLV2"));

	    LoadStdProfileSettings(6);  // Load standard INI file options (including MRU)


        m_pStateManager = new CCEStateManager;
        if (m_pStateManager == NULL) {
            AfxThrowMemoryException();
        }

        m_pComponentManager = new CCEComponentManager;
        if (m_pComponentManager == NULL) {
            AfxThrowMemoryException();
	    }

        m_pStateManager->TriggerSystemStartup ();
        if(!RegisterSystemComponents())
        {
            AfxMessageBox(IDS_LOAD_KAD_FAILED);
            return FALSE;
        }

        // create the generic container manager instance
        HRESULT hr = S_OK;
        m_pContainer = new CGenEdCont;
        assert (m_pContainer != NULL);
        if (m_pContainer == NULL)
        {
            hr = E_OUTOFMEMORY;
        }
        if (FAILED (hr))
        {
            CString strMessage;

            strMessage.Format (IDS_NO_PART, _T ("Container"));
            UTIL_FatalError (strMessage, hr);
            return (FALSE);
        }

        m_pFileHandler = new CCEFileHandler;
        assert (m_pFileHandler != NULL);
        if (m_pFileHandler == NULL)
        {
            hr = E_OUTOFMEMORY;
        }
        else
        {
	        hr = m_pFileHandler->Init(m_pContainer);
	        assert (SUCCEEDED (hr));
            if (FAILED (hr))
            {
                delete m_pFileHandler;
                m_pFileHandler = NULL;
            }
        }
        if (FAILED (hr))
        {
            CString strMessage;

            strMessage.Format (IDS_NO_PART, _T ("FileHandler"));
            UTIL_FatalError (strMessage, hr);
            return (FALSE);
        }

	    CMultiDocTemplate* pDocTemplate;

	    pDocTemplate = m_pContainer->GetDocTemplate(false);
	    AddDocTemplate (pDocTemplate);

	    pDocTemplate = m_pContainer->GetDocTemplate(true);
	    AddDocTemplate (pDocTemplate);


	    // Register the application's document templates.  Document templates
	    //  serve as the connection between documents, frame windows and views.
	    m_pPrjTemplate = new CMultiDocTemplate(
		    IDR_CEMAINTYPE,
		    RUNTIME_CLASS(CCEMAINDoc),
		    RUNTIME_CLASS(CCEMAINFrame), // custom MDI child frame
		    RUNTIME_CLASS(CCEMAINView));
	    AddDocTemplate(m_pPrjTemplate);

        m_pPanelTemplate = new CMultiDocTemplate(
		    IDR_PANELVIEW,
		    RUNTIME_CLASS(CPanelDoc),
		    RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		    RUNTIME_CLASS(CPanelView));
	    AddDocTemplate(m_pPanelTemplate);

        m_pMsgTemplate = new CMultiDocTemplate(
            IDR_CEMAINTYPE,
            RUNTIME_CLASS(CMsgDoc),
            RUNTIME_CLASS(CMsgFrame), // custom MDI Msg frame
            RUNTIME_CLASS(CMsgView));
        AddDocTemplate(m_pMsgTemplate);

       
        m_pComponentManager->AddTargetSet("Project Targets");

	    // create main MDI Frame window
	    CMainFrame* pMainFrame = new CMainFrame;
        if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
        {
		    _com_raise_error(E_FAIL);
        }
	    m_pMainWnd = pMainFrame;

        m_pComponentManager->SetDefaultParent(pMainFrame);
        RegisterFrameComponents();

        // SIS, 05.03.01: Initialize CECONT
        hr = m_pContainer->Init();
        assert (SUCCEEDED (hr));
        if (FAILED (hr))
	    {
            delete m_pContainer;
            m_pContainer = NULL;
            return FALSE;
        }

        // SIS, 20.02.01: register window class and set app icon
        // RegisterWindowClass needs application icon
        if(!RegisterWindowClass())
        {
            return FALSE;
        }

        // SIS, 06.02.01: prevents creating new project on start up
        m_bLaunch = TRUE;

	    // Parse command line for standard shell commands, DDE, file open
	    CCommandLineInfo cmdInfo;
	    ParseCommandLine(cmdInfo);

	    // The main window has been initialized, so show and update it.
	    pMainFrame->ShowWindow(m_nCmdShow);
	    pMainFrame->UpdateWindow();
        RegisterProjectComponents();

        // SIS: update main resources (menu and toolbars) from KAD
        pMainFrame->SendMessage(ID_UPDATE_MAIN_MENU);
        pMainFrame->CreateKADToolbars();
        pMainFrame->AddExploreToolbar();
        pMainFrame->AddHelpToolbar();

#ifndef _DEBUG
        // The following line was added by the Splash Screen component.
		CSplashWnd::EnableSplashScreen(TRUE);
        IUnknown*   pKadManager = NULL;
        hr = m_pComponentManager->GetCOM(CE_COMPMAN_KADMAN, pKadManager, FALSE);
        if(hr == S_OK)
        {
            CComQIPtr<IGeneral, &IID_IGeneral> pGeneral(pKadManager);
            if(pGeneral)
            {
                BSTR sSplashScreenFileName;
                HRESULT hr = pGeneral->GetSplashScreenFilename(&sSplashScreenFileName);
                if(hr == S_OK)
                {
            	    CSplashWnd::ShowSplashScreen(CString(sSplashScreenFileName), pMainFrame);
                    ::SysFreeString(sSplashScreenFileName);
                }
            }
            pKadManager->Release();
        }
#endif
	    // Dispatch commands specified on the command line
	    if (!ProcessShellCommand(cmdInfo))
        {
	    	return FALSE;
        }

    }
    catch(CMemoryException* e) {
        e->Delete();
        //TODO System message but with static string - could not allocate anything
        //anymore
        return FALSE;
    }
    catch(_com_error err) {
        return FALSE;
    }

	return TRUE;
}


//------------------------------------------------------------------*
/**
 * load KAD info.
 *
 *  get KAD manager, create new CKADInfo object and call Load() method of
 *  KAD info object.
*/
BOOL CCEMAINApp::LoadKADInfo()
{
    CComPtr<IUnknown>   pKadManager;

    pKadManager.Attach(CEGetCOM(CE_COMPMAN_KADMAN));
    if(!pKadManager)
    {
        return FALSE;
    }

    ASSERT(!m_pKADInfo);
    m_pKADInfo = new CKADInfo;

    if(!m_pKADInfo->Load(pKadManager))
    {
        return FALSE;
    }
    return TRUE;
}



//------------------------------------------------------------------*
/**
 * Exits the CCEMAINApp application.  Triggers shutdown with the
 * state manager. Deletes state and component manager.
 * NOTE: Do not call CoUnInitialize directly, CWinAppExitInstance will 
 * handle this gracefully for you. Uncorrect behaviour will result
 * if CoUnInitialize is called directly here.
 *
 * @return          of CWinApp::ExitInstance
 *
 * @see             InitInstance(), CWinApp::ExitInstance.
*/
int CCEMAINApp::ExitInstance() 
{

    if(m_pKADInfo)
    {
        delete m_pKADInfo;
        m_pKADInfo = NULL;
    }

	if (m_pFileHandler != NULL)
	{
		m_pFileHandler->Cleanup();
		delete m_pFileHandler;
        m_pFileHandler = NULL;
	}

    if(m_pContainer)
    {
		m_pContainer->Cleanup();
        delete m_pContainer;
        m_pContainer = NULL;
    }

    if (m_pStateManager != NULL)
    {
        m_pStateManager->TriggerSystemShutdown ();
    }

    // delete component manager and state manager

    if (m_pComponentManager != NULL)
    {
        delete m_pComponentManager;
        m_pComponentManager = NULL;
    }

    if (m_pStateManager != NULL)
    {
        delete m_pStateManager;
        m_pStateManager = NULL;
    }

    if(m_pSettings)
    {
        delete m_pSettings;
        m_pSettings = NULL;
    }

    TrcTerminate();

    _Module.Term();

//    IFCSessions *pSessions = NULL;
//    if (SUCCEEDED( CoCreateInstance(CLSID_FCSessions, NULL, CLSCTX_ALL, IID_IFCSessions, (void**)&pSessions) ))
//    {
//        pSessions->Shutdown_Gracefully();
//        pSessions->Release();
//    }
    
	return CWinApp::ExitInstance();
}


BOOL CCEMAINApp::PreTranslateMessage(MSG* pMsg)
{
	// The following lines were added by the Splash Screen component.
	if (CSplashWnd::PreTranslateAppMessage(pMsg))
		return TRUE;

	return CWinApp::PreTranslateMessage(pMsg);
}


// SIS, 20.02.01: in analogy to V1.3
//------------------------------------------------------------------*
/**
 * register the unique window class so others can find it.
 *
*/
BOOL CCEMAINApp::RegisterWindowClass()
{
	WNDCLASS wndcls;    
    memset((void*)&wndcls, 0, sizeof(WNDCLASS));
	wndcls.style = CS_DBLCLKS;
	wndcls.lpfnWndProc = AfxWndProc;
	wndcls.hInstance = AfxGetInstanceHandle();
	//wndcls.hIcon = (HICON)LoadImage(0, strMainIcon, IMAGE_ICON, 0, 0, LR_LOADFROMFILE /*| LR_CREATEDIBSECTION*/);
	wndcls.hIcon = LoadIcon(IDR_MAINFRAME);
	wndcls.hCursor = LoadStandardCursor(IDC_ARROW);
	wndcls.hbrBackground = NULL;
	wndcls.lpszMenuName = NULL;
	wndcls.lpszClassName = m_strWndClassName;
	// Register name, exit if it fails
	if(!AfxRegisterClass(&wndcls))
    {
//		AfxMessageBox(IDS_REGISTERCLASS_FAILED, MB_ICONSTOP | MB_OK );
		return FALSE;
	}
    return TRUE;
}

//------------------------------------------------------------------*
/**
 * Registers all components with life time system with the component
 * manager. InitInstance calls RegisterSystemComponents after the system
 * startup has been notified to the state manager.
 *
 * @exception       CMemoryException if out of memory situation.<br>
 * @exception       _com_error exception for any other error.                 
 * @see             InitInstance.
*/

BOOL CCEMAINApp::RegisterSystemComponents()
{
    HRESULT hr = S_OK;
    hr = m_pComponentManager->AddCOM(CE_COMPMAN_KADMAN,
                                CCEComponentManager::LifetimeSystem,
                                CLSID_KadManagerComObj);

    if(FAILED(hr)) {
        if(hr == E_OUTOFMEMORY) {
            AfxThrowMemoryException();
        }
        _com_raise_error(hr);
    }

    if(!LoadKADInfo())
    {
        return FALSE;
    }
    return TRUE;
}

//------------------------------------------------------------------*
/**
 * Registers all components with life time frame with the component
 * manager. InitInstance calls RegisterFrameComponents after the main
 * frame has been created. The order in which components are created
 * is important here:<br>
 * 1.) - Create message viewer first. Other components created subsequently
 *       are then able to issue messages to a view or tab of the
 *       message viewer.<br>
 * 2.) - Create KAD and Projectmanager second, cause Navigator needs them.<br>
 * 3.) - Create Navigator finally, hence Navigator is able to gain access to
 *       KAD- and Projectmanager.<br>
 *
 * @exception       CMemoryException if out of memory situation.<br>
 * @exception       _com_error exception for any other error.                 
 * @see             InitInstance.
*/
void CCEMAINApp::RegisterFrameComponents()
{
    HRESULT hr = S_OK;


    hr = m_pComponentManager->AddDialogDockingTool (CE_COMPMAN_MSGVIEW,
                                               CCEComponentManager::LifetimeFrame,
                                               RUNTIME_CLASS (CMsgWnd),
                                               CG_ID_VIEW_MESSAGE,
                                               WS_VISIBLE| CBRS_BOTTOM | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS,
                                               IDD_MSG_DLGBAR,
                                               CBRS_EX_ALLOW_MDI_FLOAT | CBRS_EX_STDCONTEXTMENU | CBRS_EX_BORDERSPACE);
    
    if(FAILED(hr)) {
        if(hr == E_OUTOFMEMORY) {
            AfxThrowMemoryException();
        }
        _com_raise_error(hr);
    }

    hr = m_pComponentManager->AddCOM(CE_COMPMAN_PRJMAN,
                                CCEComponentManager::LifetimeFrame,
                                CLSID_CEProjectManager);

    if(FAILED(hr)) {
        if(hr == E_OUTOFMEMORY) {
            AfxThrowMemoryException();
        }
        _com_raise_error(hr);
    }

    hr = m_pComponentManager->AddCOM(CE_COMPMAN_EXE,
                                CCEComponentManager::LifetimeFrame,
                                CLSID_CEExecute);

    if(FAILED(hr)) {
        if(hr == E_OUTOFMEMORY) {
            AfxThrowMemoryException();
        }
        _com_raise_error(hr);
    }

    hr = m_pComponentManager->AddCOM(CE_COMPMAN_FIF,
                                CCEComponentManager::LifetimeFrame,
                                CLSID_CEFindInFiles);

    if(FAILED(hr)) {
        if(hr == E_OUTOFMEMORY) {
            AfxThrowMemoryException();
        }
        _com_raise_error(hr);
    }

    hr = m_pComponentManager->AddDialogDockingTool (CE_COMPMAN_NAV,
                                               CCEComponentManager::LifetimeFrame,
                                               RUNTIME_CLASS (CNavigator),
                                               CG_ID_VIEW_NAVIGATOR,
                                               WS_VISIBLE| CBRS_LEFT | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS,
                                               IDD_NAVIGATOR,
                                               CBRS_EX_ALLOW_MDI_FLOAT | CBRS_EX_STDCONTEXTMENU | CBRS_EX_BORDERSPACE);

    if(FAILED(hr)) {
        if(hr == E_OUTOFMEMORY) {
            AfxThrowMemoryException();
        }
        _com_raise_error(hr);
    }
}

void CCEMAINApp::RegisterProjectComponents()
{
    HRESULT hr = S_OK;

    CString strTitle;

    strTitle.LoadString(IDS_VIEW_WATCH);
    m_pComponentManager->AddSimpleDockingTool (CE_COMPMAN_WATCH,
                                               CCEComponentManager::LifetimeProject,
                                               RUNTIME_CLASS (CWatchWindow),
                                               CG_ID_VIEW_WATCH,
                                               WS_VISIBLE | CBRS_TOP | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS,
                                               strTitle, //_T ("Watch Window"),
                                               CBRS_EX_ALLOW_MDI_FLOAT | CBRS_EX_STDCONTEXTMENU | CBRS_EX_BORDERSPACE);



    strTitle.LoadString(IDS_VIEW_POUVARSEL);
    m_pComponentManager->AddSimpleDockingTool (CE_COMPMAN_POUVARSEL,
                                               CCEComponentManager::LifetimeProject,
                                               RUNTIME_CLASS (CPouVarSelWindow),
                                               CG_ID_VIEW_POUVARSEL,
                                               WS_VISIBLE | CBRS_LEFT | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS,
                                               strTitle, //_T ("POU/Variable Selection"),
                                               CBRS_EX_ALLOW_MDI_FLOAT | CBRS_EX_STDCONTEXTMENU | CBRS_EX_BORDERSPACE);


    m_pComponentManager->AddCOM (CE_COMPMAN_BREAKPTMAN,
                                 CCEComponentManager::LifetimeProject,
                                 CLSID_BreakPointMan);

    strTitle.LoadString(IDS_VIEW_BREAKPOINT);
    m_pComponentManager->AddSimpleDockingTool (CE_COMPMAN_BPVIEW,
                                               CCEComponentManager::LifetimeProject,
                                               RUNTIME_CLASS (CBPViewWindow),
                                               CG_ID_VIEW_BPVIEW,
                                               WS_VISIBLE | CBRS_TOP | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS,
                                               strTitle, //_T ("BreakPoint Window"),
                                               CBRS_EX_ALLOW_MDI_FLOAT | CBRS_EX_STDCONTEXTMENU | CBRS_EX_BORDERSPACE);

    strTitle.LoadString(IDS_VIEW_XREF);
    m_pComponentManager->AddSimpleDockingTool (CE_COMPMAN_XREF,
                                               CCEComponentManager::LifetimeProject,
                                               RUNTIME_CLASS (CXRefWindow),
                                               CG_ID_VIEW_XREF,
                                               WS_VISIBLE | CBRS_LEFT | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS,
                                               strTitle, //_T ("XRef Tool"),
                                               CBRS_EX_ALLOW_MDI_FLOAT | CBRS_EX_STDCONTEXTMENU | CBRS_EX_BORDERSPACE);



    hr = m_pComponentManager->AddFromKAD();

    if(FAILED(hr)) {
        if(hr == E_OUTOFMEMORY) {
            AfxThrowMemoryException();
        }
        _com_raise_error(hr);
    }
}



//------------------------------------------------------------------*
/**
 * Initializes COM, sets COM security to NULL DACL and modifies the
 * message filter to show no Not-Responding dialog and no Busy-Dialog
 * @exception		_com_error	
 * @see			
*/
void CCEMAINApp::InitializeCOM() throw()
{
    HRESULT hr = S_OK;
    COleMessageFilter * pFilter = NULL;

	if (!AfxOleInit()) //calls ::CoInitialize
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
        _com_raise_error(E_FAIL);
	}

    AfxEnableControlContainer();

     // This provides a NULL DACL which will allow access to everyone.
    hr = ::CoInitializeSecurity(NULL, -1, NULL, NULL,
                                RPC_C_AUTHN_LEVEL_NONE, 
                                RPC_C_IMP_LEVEL_IMPERSONATE, 
                                NULL, EOAC_NONE, NULL);
    assert(SUCCEEDED(hr));
    if(FAILED(hr)) {
        _com_raise_error(hr);
    }
    
	pFilter = AfxOleGetMessageFilter();
    assert(pFilter);
    if(!pFilter) {
        _com_raise_error(E_FAIL);
    }

	pFilter->EnableNotRespondingDialog(FALSE);
	pFilter->EnableBusyDialog(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CCEMAINApp commands

// SIS, 06.02.01: copy of OnFileNew() from V1.3
void CCEMAINApp::OnFileNew() 
{
    if(m_bLaunch)
    {
        m_bLaunch = FALSE;
        return;
    }
    else 
    {
        if(!m_bPrjOpen)
        {
            m_pPrjTemplate->OpenDocumentFile(NULL);
        }
        else
        {
            CMainFrame* pMainFrame = dynamic_cast<CMainFrame *> (m_pMainWnd);
            if(pMainFrame)
            {
                pMainFrame->OpenNewDocument();
            }
        }
    }
}

// SIS, 06.02.01: copy of OnFileOpen() from V1.3
void CCEMAINApp::OnFileOpen()
{
	// 13.12.06 SIS >>
    if(m_bPrjOpen)
    {
		CMainFrame* pMainFrame = dynamic_cast<CMainFrame *> (m_pMainWnd);
		if(pMainFrame)
		{
			pMainFrame->OpenDocument();
		}
	}
    else
    {
		OnProjectOpen();
    }
	// 13.12.06 SIS <<
}

// 13.12.06 SIS >>
void CCEMAINApp::OnProjectOpen()
{
	CFileDialog dlgFile(TRUE);
    CString strFileName;
    CString strDir;
	CString strTitle;
	CString strFilter;
	CString strDefault;

	VERIFY(strTitle.LoadString(AFX_IDS_OPENFILE));
    ASSERT(m_pPrjTemplate);

    AppendFilterSuffix(strFilter, dlgFile.m_ofn, m_pPrjTemplate, &strDefault);
	// append the "*.*" all files filter
	CString allFilter;
	VERIFY(allFilter.LoadString(IDS_ALLFILTER));
	strFilter += allFilter;
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.*");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;

	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = strTitle;

    CString strInitialDir = GetInitialBrowseProjectPath();
    dlgFile.m_ofn.lpstrInitialDir = strInitialDir;

	dlgFile.m_ofn.lpstrFile = strFileName.GetBuffer(_MAX_PATH);
	dlgFile.m_ofn.Flags |= (OFN_HIDEREADONLY | OFN_FILEMUSTEXIST);
	int nResult = dlgFile.DoModal();
	strFileName.ReleaseBuffer();

    if(nResult == IDOK)
    {
        OpenDocumentFile(strFileName);        
    }
}
// 13.12.06 SIS <<


//------------------------------------------------------------------*
/**
 * get initial browse project path.
 *
 *  try to get last recently used file and strip path.
 *  if not found use standard project path.
 *
 * @return          initial browse project path
*/
CString CCEMAINApp::GetInitialBrowseProjectPath()
{
    // try to get last project file name
    CString strInitialDir;

    strInitialDir = GetProfileString(REG_SECTION_ENGINEERING, REG_KEY_LAST_PROJECT);
    if(!strInitialDir.IsEmpty())
    {
        // now strip path
        int iFound = strInitialDir.ReverseFind(_T('\\'));
        if(iFound >= 0)
        {
            strInitialDir = strInitialDir.Left(iFound);
        }
    }
    else
    {
        strInitialDir = GetStandardProjectPath();
    }
    return strInitialDir;
}


//------------------------------------------------------------------*
/**
 * get standard project path.
 *
 * @return          standard project path
*/
CString CCEMAINApp::GetStandardProjectPath()
{
    CString strStdProjPath;
    UINT    nStrLen = _MAX_PATH;
    HRESULT hr = UTIL_GetStdProjectPath(strStdProjPath.GetBuffer(_MAX_PATH), nStrLen);
    strStdProjPath.ReleaseBuffer();
    return strStdProjPath;
}


CDocument *CCEMAINApp::OpenDocumentFile (LPCTSTR lpszFileName)
{

	// File drops come in here. We allow any file drop if a
	// project is open. However we allow project file drops
	// only if no project is open.

    CGenEdDoc*  pDoc;

// SIS: check, if file is 4cp
    // 25.08.06 SIS >>
    // added last recently used projects
    // if project is open, it must be closed first
    CString strFileName = lpszFileName;
    BOOL    bIsProjectFile = (strFileName.Right(3).CompareNoCase(CE_PROJ_FILE_EXT) == 0);
	if(m_bPrjOpen)
	{
        if(bIsProjectFile)
        {
            // close project now
            OnProjectClose();
        }
        else
        {
            HRESULT     hr;

            if (m_pFileHandler == NULL)
            {
		        return (CWinApp::OpenDocumentFile (lpszFileName));
            }

            hr = m_pFileHandler->ShowFile(lpszFileName, pDoc);
            if (FAILED (hr))
            {
                return (NULL);
            }
            return(pDoc);
        }
	}
    // 25.08.06 SIS <<
    if(bIsProjectFile)
    {
		m_bOpeningProject = TRUE;	// 09.11.04 SIS

        CDocument* pDocRet = m_pPrjTemplate->OpenDocumentFile(lpszFileName);
        if(pDocRet)
        {
            LoadSettings(lpszFileName);
            // d-1805 23.03.04 SIS >>
            // Watch is loading as last and claims focus on tab control
            // accelerator keys do not work, if no editor is opened
            // set focus to navigator here to avoid this problem
            CEGetDockingTool("Navigator", true)->SetFocus();
            // d-1805 23.03.04 SIS <<
        }
		m_bOpeningProject = FALSE;	// 09.11.04 SIS
   	    return (pDocRet);
    }
    return 0;
}


void CCEMAINApp::LoadSettings(const CString& crstrFilePath)
{
    CString strPath;
    int iFound = crstrFilePath.ReverseFind(_T('\\'));
    if(iFound >= 0)
    {
        strPath = crstrFilePath.Left(iFound);
    }

    if (m_pSettings)
    {
        delete m_pSettings;
    }
    m_pSettings = new CSettings;
    ASSERT(m_pSettings);

    if (!strPath.IsEmpty())
    {
        m_pSettings->Load(strPath);
    }

    CCEStateManager* pStateManager = CEGetStateManager();
    if (pStateManager != NULL)
    {
        pStateManager->TriggerNewSettings();
    }
}


// SIS, 15.03.01: close project in app
// maybe this is not the right place
//------------------------------------------------------------------*
/**
 * close project
 *
*/
void CCEMAINApp::OnProjectClose()
{
    CCEMAINDoc* pDoc = GetProjectDoc();
    if (pDoc)
    {
        // terminate a running find in files session
//        TerminateFindInFilesThread();

//	    CloseDownloader();
	    if(m_pContainer)
	    {
		    if(!m_pContainer->SaveAll())
            {
                CString strMessage;
                strMessage.LoadString(IDS_WARN_SAVE_INCOMPLETE);
                if(::AfxMessageBox(strMessage, MB_YESNO) == IDYES)
                {
                    pDoc->OnCloseDocument();
                    return;
                }
                else
                {
                    m_bShuttingDown = false;
                    return;
                }
            }
	    }

//        assert (m_pPanelTemplate);
//        if (m_pPanelTemplate)
//        {
//            m_pPanelTemplate->SaveAllModified();
//        }

        // shut it down
        pDoc->OnCloseDocument();
    }
}

void CCEMAINApp::OnProjectSave()
{
    CCEMAINDoc* pDoc = GetProjectDoc();
    if (pDoc)
    {
	    if(m_pContainer)
	    {
		    m_pContainer->SaveAll(false);
	    }

        assert (m_pPanelTemplate);
        if (m_pPanelTemplate)
        {
            m_pPanelTemplate->SaveAllModified();
        }
    }
}

void CCEMAINApp::OnProjectSaveAs()
{
    CCEMAINApp* pApp = (CCEMAINApp*) AfxGetApp();
    ASSERT(pApp);
    CGenEdCont* pContainer = pApp->GetContainer();
    if(pContainer)
    {
        pContainer->SaveAll(FALSE);
    }

    //Get the ProjectInfo pointer, get old project name and path
    CString strOldProjPath;
    CString strOldProjName;
    CString strProjectDirectory;

    CComQIPtr<ICEProjInfo>    pProjInfo;
    CComPtr<IUnknown>         pTmp;
    pTmp.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
    pProjInfo = pTmp;
    
    HRESULT hRes = 0;
    CComBSTR sProjName;
    hRes = pProjInfo->getProjectName(&sProjName);
    ASSERT(hRes==S_OK);
    strOldProjName = sProjName;

    CComBSTR sProjPath;
    hRes =  pProjInfo->getProjectPath(&sProjPath);
    strOldProjPath = sProjPath;

    CComBSTR sInstDir;
    hRes = pProjInfo->getInstallationPath(&sInstDir);
    strProjectDirectory = sInstDir;
    strProjectDirectory += _T("Projects");//CE_STDPROJECT_PATH; 

    CProjectSaveAsDlg dlgProjSave(strProjectDirectory, strOldProjName);

    if (dlgProjSave.DoModal() == IDCANCEL)
    {
        return;
    }


    ////////////////////////////////
    // start save as process

    CString strNewProjectPath = dlgProjSave.GetProjectPath();;
    CString strNewProjectName = dlgProjSave.GetProjectName();;
    
    // prepare project path and name
    //strNewProjectPath = strNewProj;
    //int iNamePos = strNewProj.ReverseFind(_T('\\'));
    //ASSERT(iNamePos>0);
    //strNewProjectName = strNewProj.Mid(iNamePos+1);

   
    // test if the new project directory does not exist
    int iDesDirExist = FC_ExistSubDirA(strNewProjectPath);
    if(iDesDirExist == 1)
    {
        CString strMsg;
        strMsg.FormatMessage(IDS_PROJ_SAVE_AS_FOLDER_EXISTS, strNewProjectPath);
        AfxMessageBox(strMsg, MB_OK);
        return;
    }

    // create new directory
    int iDirCreated = FC_CreateSubDirA(strNewProjectPath);
    if (iDirCreated == 0)
    {
        CString strMsg;
        strMsg.FormatMessage(IDS_PROJ_SAVE_AS_CREATE_FOLDER_FAILED, strNewProjectPath);
        AfxMessageBox(strMsg, MB_OK);
        return;
    }

    // close old project
    OnProjectClose();
     
    // copy files and all subdirs (except from "gen") using callback function CopyFilesCB
    FC_FileEnumFiles(strOldProjPath, FC_FILE_ENUM_SUBDIRS|FC_FILE_ENUM_FULLPATH, CopyFilesCB, 
                        (void*)&strOldProjPath, (void*)&strNewProjectPath);
    
    // rename new 4cp-file
    CString strOld4CPFileName;
    CString strNew4CPFileName;

    strOld4CPFileName = strNewProjectPath + _T("\\") + strOldProjName + _T(".4cp");
    strNew4CPFileName = strNewProjectPath + _T("\\") + strNewProjectName + _T(".4cp");
    try
    {
        CFile::Rename(strOld4CPFileName, strNew4CPFileName);
    }
    catch (CFileException* e)
    {
        HRESULT hr;
        hr = HRESULT_FROM_WIN32(e->m_lOsError);
        e->Delete();

        CString strMsg;
        strMsg.FormatMessage(IDS_PROJ_SAVE_AS_RENAME_4CP);
        AfxMessageBox(strMsg, MB_OK);
        OpenDocumentFile(strOld4CPFileName);
        return;
    }

    // rename new 4clc-file if one exists
    CString strOld4CLCFileName;
    CString strNew4CLCFileName;

    strOld4CLCFileName = strNewProjectPath + _T("\\") + strOldProjName + _T(".4clc");
    strNew4CLCFileName = strNewProjectPath + _T("\\") + strNewProjectName + _T(".4clc");
    try
    {
        CFile::Rename(strOld4CLCFileName, strNew4CLCFileName);
    }
    catch (CFileException* e)
    {
        // don't react to an error, the file msut not exist in older projects.
        HRESULT hr;
        hr = HRESULT_FROM_WIN32(e->m_lOsError);
        e->Delete();
    }

    // check if new configuration file has to be renamed
    CString strNewConFileName = strNewProjectPath + _T("\\") + _T("Configuration1") + _T(".con");
    CString strOldConFileName = strNewProjectPath+_T("\\")+strOldProjName+_T(".con");
    BOOL bRenameConNecessary = FALSE;

    CFileStatus conFileStatus;
    if (CFile::GetStatus(strOldConFileName, conFileStatus))
    {
        bRenameConNecessary = TRUE;
    }

    // rename configuration file
    BOOL bConFileRenamed = TRUE;
    if (bRenameConNecessary)
    {
        try
        {
            CFile::Rename(strOldConFileName, 
                          strNewConFileName);
        }
        catch (CFileException* e)
        {
            HRESULT hr;
            hr = HRESULT_FROM_WIN32(e->m_lOsError);
            e->Delete();

            CString strMsg;
            strMsg.FormatMessage(IDS_PROJECT_SAVE_AS_RENAME_CON_FILE);
            AfxMessageBox(strMsg, MB_OK);
            bConFileRenamed = FALSE;
        }
    }

    // rename configuration
    if (bRenameConNecessary && bConFileRenamed)
    {
        try
        {
            CFile configFile;
            //Can the file be open
            if (configFile.Open(strNewConFileName, 
                                 CFile::modeReadWrite | CFile::shareDenyRead))
            {
                DWORD   dwLength = configFile.GetLength();
                DWORD   dwRead;

                CString strConfig;
                dwRead = configFile.Read(strConfig.GetBuffer(dwLength), dwLength);
                strConfig.ReleaseBuffer();

                ASSERT(dwRead == dwLength);

                //replace the text
                CString strConHelp1 = _T(" ") + strOldProjName;
                CString strConHelp2 = _T(" Configuration1");
                int n = strConfig.Replace(strConHelp1, strConHelp2);
                ASSERT(n != 0);
                //write the file
                configFile.SeekToBegin();
                dwLength = strConfig.GetLength();
                configFile.Write(strConfig, dwLength);
                configFile.SetLength(dwLength);
                configFile.Close();
            }
        }
        catch (CFileException* e)
        {
            HRESULT hr;
            hr = HRESULT_FROM_WIN32(e->m_lOsError);
            e->Delete();

            CString strMsg;
            strMsg.FormatMessage(IDS_PROJECT_SAVE_AS_RENAME_CON_FILE);
            AfxMessageBox(strMsg, MB_OK);
        }
    }

    // rename link in 4cp file
    if (bRenameConNecessary && bConFileRenamed)
    {
        try
        {
            CString strProjectFileName = strNewProjectPath + _T("\\") + strNewProjectName + _T(".4cp");
            CFile projFile;
            if (projFile.Open(strProjectFileName, CFile::modeReadWrite | CFile::shareDenyRead))
            {
                CArchive projArchLoad(&projFile, CArchive::load);
                CArchive projArchStore(&projFile, CArchive::store);
    
                int lineNr = 0;
                bool bFoundConfig = false;

                CString strLine;
                CString strHelp;
                CString strLineNum;
                while (projArchLoad.ReadString(strLine))
                {
                    lineNr++;
                    strHelp = strLine;
                    strHelp.TrimLeft();
                    strHelp.TrimRight();

                    if (strHelp.IsEmpty())
                    {
                        projArchStore.WriteString(strLine);
                        projArchStore.WriteString(_T("\r\n"));
                        continue;
                    }

                    if (strHelp[0]==_T(';')) // a comment line
                    {
                        projArchStore.WriteString(strLine);
                        projArchStore.WriteString(_T("\r\n"));
                        continue;
                    }

                    if (strHelp.CompareNoCase(_T("[Classes.Configurations]"))==0) // configuration section
                    {
                        bFoundConfig = true;
                    }
                    if(!bFoundConfig)
                    {
                        projArchStore.WriteString(strLine);
                        projArchStore.WriteString(_T("\r\n"));
                        continue;
                    }
                    else    // should be the key value line for the config
                    {
                        CString strKey;
                        CString strValue;
                        int iPosition = strHelp.Find(_T('='));
                        if (iPosition<0)
                        {
                            projArchStore.WriteString(strLine);
                            projArchStore.WriteString(_T("\r\n"));
                            continue;
                        }
                        strKey = strHelp.Left(iPosition);
                        strValue = strHelp.Mid(iPosition+1);
                        strValue.TrimLeft();
                        strValue.TrimRight();
                        if (strValue.CompareNoCase(strOldProjName+_T(".con"))==0)
                        {
                            strLine = strKey + _T("=") + _T("Configuration1.con");
                            bFoundConfig = false;
                        }
                        projArchStore.WriteString(strLine);
                        projArchStore.WriteString(_T("\r\n"));
                    }

                }
                projArchStore.WriteString(_T("\r\n"));
                projArchLoad.Close();
                projFile.SeekToBegin();
                projArchStore.Close();
                projFile.Close();
            }
            else
            {
                CString strMsg;
                strMsg.FormatMessage(IDS_PROJECT_SAVE_AS_RENAME_CON_FILE);
                AfxMessageBox(strMsg, MB_OK);
            }
        }
        catch (CFileException* e)
        {
            HRESULT hr;
            hr = HRESULT_FROM_WIN32(e->m_lOsError);
            e->Delete();

            CString strMsg;
            strMsg.FormatMessage(IDS_PROJECT_SAVE_AS_RENAME_CON_FILE);
            AfxMessageBox(strMsg, MB_OK);        
        }
    }

    // renaming in fileview.xml
    // no longer necessary, the navigator will update the fileview.xml itself,
    // the two renamed files will disappear and the new files will be sorted under source files.

    // open new project
    CString strNewOpen4CPFile = strNewProjectPath + _T("\\") + strNewProjectName + _T(".4cp");
    OpenDocumentFile(strNewOpen4CPFile);
}


void CCEMAINApp::AddToRecentFileList( LPCTSTR lpszPathName )
{
    CString strPath;
    CString strFileName, strExt;
    CFile File;

    strPath = lpszPathName;
//    if(!strExt.LoadString(IDS_PRJEXT))
//        TO_ASSERT("CEMAIN> Load string from resource failed");
    strExt = CE_PROJ_FILE_EXT;

    File.SetFilePath(strPath);
    strFileName = File.GetFileName();
    
    if(strFileName.Right(3) == CE_PROJ_FILE_EXT)
    {
        CWinApp::AddToRecentFileList(lpszPathName);
    }
}


// SIS, 15.03.01: copied from V1.3
CCEMAINDoc* CCEMAINApp::GetProjectDoc()
{
    CMultiDocTemplate *pTemplate;
    CDocument *pDocument;
    POSITION pos;

    assert(m_pPrjTemplate);
    pTemplate = m_pPrjTemplate;

    //get first document position of the document template
    pos = pTemplate->GetFirstDocPosition();
    //no document created 
    if(NULL == pos) 
        return NULL;

    //get the first document
    pDocument = pTemplate->GetNextDoc(pos);
    if(!pDocument)
        return NULL;

    if(!pDocument->IsKindOf(RUNTIME_CLASS(CCEMAINDoc))) {
        assert(pDocument->IsKindOf(RUNTIME_CLASS(CCEMAINDoc)));
        return NULL;
    }
    return (CCEMAINDoc*)pDocument;
}



CMsgDoc * CCEMAINApp::GetMsgDoc()
{
    CMultiDocTemplate *pTemplate;
    CDocument *pDocument;
    POSITION pos;

    ASSERT(m_pMsgTemplate);
    pTemplate = m_pMsgTemplate;

    //get first document position of the document template
    pos = pTemplate->GetFirstDocPosition();
    //no document created 
    if(NULL == pos) 
        return NULL;

    //get the first document
    pDocument = pTemplate->GetNextDoc(pos);
    if(!pDocument)
        return NULL;

    if(!pDocument->IsKindOf(RUNTIME_CLASS(CMsgDoc))) {
        ASSERT(pDocument->IsKindOf(RUNTIME_CLASS(CMsgDoc)));
        return NULL;
    }
    return (CMsgDoc *)pDocument;
}

void CCEMAINApp::OnHelpFinder()
{
	// TODO: Add your command handler code here
    ShowHelp(0, HELP_FINDER);
    return;
}

void CCEMAINApp::WinHelp(DWORD dwData, UINT nCmd)
{
	// TODO: Add your specialized code here and/or call the base class
    ShowHelp(dwData, nCmd);
    return;
}

void CCEMAINApp::ShowHelp(DWORD dwData, UINT nCmd)
{
    ASSERT(m_pMainWnd->m_hWnd != NULL);
    CString strManualPath;
    UINT    nStrLen = _MAX_PATH;

    HRESULT hr = UTIL_GetInstallPath(strManualPath.GetBuffer(_MAX_PATH), &nStrLen);
    strManualPath.ReleaseBuffer();
    if(SUCCEEDED(hr)) {
        strManualPath += CE_MANUAL_FILENAME;
        switch(nCmd) {
            case HELP_CONTEXT:
                if(dwData >= 0x30000) {
                    dwData -= 0x30000;
                } else if(dwData >= 0x20000) {
                    dwData -= 0x20000;
                } else if(dwData >= 0x10000) {
                    dwData -= 0x10000;
                }
                TRACE(_T("HtmlHelp ID: %i\n"), dwData);
#ifdef _DEBUG
                {
                    CString strMessage;
                    strMessage.Format("ShowHelp: ID = %d\n", dwData);
                    ::AfxMessageBox(strMessage);
                }
#endif
                HtmlHelp(0, strManualPath, HH_HELP_CONTEXT, dwData);
                break;
            case HELP_INDEX:
                break;
            case HELP_FINDER:
                HtmlHelp(0, strManualPath, HH_DISPLAY_TOPIC, NULL);
                break;
        }
    }
    return;
}


static void AppendFilterSuffix(CString& filter, OPENFILENAME& ofn,
	CDocTemplate* pTemplate, CString* pstrDefaultExt)
{
	ASSERT_VALID(pTemplate);
	ASSERT_KINDOF(CDocTemplate, pTemplate);

	CString strFilterExt, strFilterName;
	if (pTemplate->GetDocString(strFilterExt, CDocTemplate::filterExt) &&
	 !strFilterExt.IsEmpty() &&
	 pTemplate->GetDocString(strFilterName, CDocTemplate::filterName) &&
	 !strFilterName.IsEmpty())
	{
		// a file based document template - add to filter list
		ASSERT(strFilterExt[0] == '.');
		if (pstrDefaultExt != NULL)
		{
			// set the default extension
			*pstrDefaultExt = ((LPCTSTR)strFilterExt) + 1;  // skip the '.'
			ofn.lpstrDefExt = (LPTSTR)(LPCTSTR)(*pstrDefaultExt);
			ofn.nFilterIndex = ofn.nMaxCustFilter + 1;  // 1 based number
		}

		// add to filter
		filter += strFilterName;
		ASSERT(!filter.IsEmpty());  // must have a file type name
		filter += (TCHAR)'\0';  // next string please
		filter += (TCHAR)'*';
		filter += strFilterExt;
		filter += (TCHAR)'\0';  // next string please
		ofn.nMaxCustFilter++;
	}
}

//------------------------------------------------------------------*
/**
 * CopyFilesCB
 *
 *  copy files callback function for FC_FileEnumFiles in copy project as
 *
 * @param           pCtx: context pointer 1: contains source directory
 * @param           pCtx2: context pointer 2: contains destination directory
 * @param           bIsDir: is directory?
 * @param           bOnError: error occurred?
 * @param           pszFile: file name including full path (this depends on flags)
 * @return           1: ok, proceed with subdir
 *                   0: ok, stop with subdir
 *                  <0: stop 
 * @exception       -
 * @see             
*/
static int CopyFilesCB(void* pCtx, void* pCtx2, int bIsDir, int bOnError, const TCHAR* pszFile)
{
    ASSERT(pCtx);
    ASSERT(pCtx2);

    CString strSrcDir = *(CString*)pCtx;
    CString strDestDir = *(CString*)pCtx2;
    CString strOldFilePathName(pszFile);
    CString strNewFilePathName;
    CString strFileName;

    if(strOldFilePathName.Compare(_T("..")) == 0 || strOldFilePathName.Compare(_T(".")) == 0)
    {
        return 0;
    }

    CString strRelPath = strOldFilePathName.Mid(strSrcDir.GetLength()+1);
    strNewFilePathName.Format(_T("%s\\%s"), strDestDir, strRelPath);

    int iFound = strOldFilePathName.ReverseFind(_T('\\'));
    ASSERT(iFound >= 0);

    strFileName = strOldFilePathName.Mid(iFound+1);
    if(strFileName.GetLength() > 1)
    {
        if(strFileName.Left(2) == _T("~$"))
            return 0;
    }

    if(bIsDir)
    {
        if(strFileName.CompareNoCase(_T("gen")) == 0)
        {
            return 0;
        }
        return CreateDirectory(strNewFilePathName, NULL);
    }
    return CopyFile(strOldFilePathName, strNewFilePathName, FALSE);
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CCEMAINApp::OnAppAbout()
{
    BOOL bCustomAbout = FALSE;
    
    if(m_pComponentManager != NULL) {
        IUnknown*   pKadManager = NULL;
        HRESULT hr = m_pComponentManager->GetCOM(CE_COMPMAN_KADMAN, pKadManager, FALSE);
        if(hr == S_OK) {
            CComQIPtr<IGeneral, &IID_IGeneral> pGeneral(pKadManager);
            if(pGeneral) {
                GUID guidHandlerId;
                hr = pGeneral->GetAboutBoxHandler(&guidHandlerId);
                if(hr == S_OK) {
                    CComPtr<IUnknown> AboutBox = NULL;
                    hr = AboutBox.CoCreateInstance(guidHandlerId);
                    if(SUCCEEDED(hr)) {
                        CComQIPtr<ICEAddonAction, &IID_ICEAddonAction> pAbout(AboutBox);
                        if(pAbout) {
                            bCustomAbout = TRUE;
                            pAbout->doAction(CComBSTR("showAboutBox"),  //   [in, string] BSTR         sMethodName, 
                                             CComBSTR(""),              //   [in, string] BSTR         sSourceFile,
                                             CComBSTR(""),              //   [in, string] BSTR         sId,
                                             NULL,                      //   [in]         IUnknown*    pICEProjectInfo,   
                                             CComBSTR(""));             //   [in, string] BSTR         sAdditionalInfo)
                        }
                        AboutBox.Release();
                    }
                }
            }
            pKadManager->Release();
        }
    }
    if(!bCustomAbout) {
        CAboutDlg aboutDlg;
	    aboutDlg.DoModal();
    }
}

//------------------------------------------------------------------*
/**
 * get pointer to editor container
 *
 * @return          pointer to editor container
*/
CGenEdCont* CCEMAINApp::GetContainer ()
{
	return (m_pContainer);
}


//------------------------------------------------------------------*
/**
 * get pointer to file handler
 *
 * @return          pointer to file handler
*/
CCEFileHandler* CCEMAINApp::GetFileHandler()
{
	return (m_pFileHandler);
}

//------------------------------------------------------------------*
/**
 * get pointer to KAD info object.
 *
 * @return          pointer to KAD info object
*/
CKADInfo* CCEMAINApp::GetKADInfoObject()
{
	return(m_pKADInfo);
}

CSettings* CCEMAINApp::GetSettings()
{
    if(!m_pSettings)
    {
        m_pSettings = new CSettings;
        m_pSettings->Init();
    }
    return m_pSettings;
}

void CCEMAINApp::DeleteSettings()
{
    if(m_pSettings)
    {
        delete m_pSettings;
        m_pSettings = NULL;
    }
}




void CCEMAINApp::OnFilePrintSetup() 
{
    // first run the print setup
    CWinApp::OnFilePrintSetup();

    // now try to set print infos to the editor container
    SetPrintInfo();
    return;
}


void CCEMAINApp::SetPrintInfo() 
{
    CString strPrinterName = _T("");
    long lWidth = 0L;
    long lHeight = 0L;
    CGenEdCont* pContainer = GetContainer();
    if(pContainer != NULL) 
    {
        CMainFrame* pMainFrame = dynamic_cast<CMainFrame *> (m_pMainWnd);
        if(pMainFrame->GetPrintInfo(strPrinterName, &lWidth, &lHeight)) 
        {
            pContainer->SetPrintInfo(strPrinterName, lWidth, lHeight);
        }
    }
    return;
}



void CCEMAINApp::OnUpdateIsProjectNotOpen(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(!m_bPrjOpen);
}

void CCEMAINApp::OnUpdateIsProjectOpen(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(m_bPrjOpen);
}

// 13.12.06 SIS >>
void CCEMAINApp::OnUpdateEnableAlways(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}
// 13.12.06 SIS <<

// SIS, 15.03.01: copied from V1.3
void CCEMAINApp::OnAppExit() 
{
    assert (m_pContainer != NULL);
    if (m_pContainer != NULL)
    {
        if (!m_pContainer->SaveAll (true))
        {
            return;
        }

        m_pContainer->ClearModified ();
    }

    CWinApp::OnAppExit ();
}




//------------------------------------------------------------------*
/**
 * OnIdle
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
BOOL CCEMAINApp::OnIdle(LONG lCount) 
{
    BOOL bMore = FALSE;
    HRESULT hr;
    CMainFrame *pMainFrame;

    bMore = CWinApp::OnIdle (lCount);

//    if(lCount > 0)
//    {
//        return FALSE;
//    }

    if (m_pContainer != NULL && m_pContainer->OnIdle ())
    {
        bMore = TRUE;
    }

    if (m_bPrjOpen)
    {
        // call project manager
        IUnknown*   pUnk = NULL;
        CComQIPtr<ICEProjectManager, &IID_ICEProjectManager> pProjMan;
    
        pUnk = CEGetCOM(CE_COMPMAN_PRJMAN);
        pProjMan = pUnk;
        pUnk->Release();

        if (!(!pProjMan))
        {
            hr = pProjMan->ReparseCheck();
            if (hr==S_FALSE)
            {
                bMore = TRUE;
            }
        }
        
        pMainFrame = dynamic_cast<CMainFrame *> (m_pMainWnd);
        if(pMainFrame)
        {
            if(pMainFrame->OnIdle(lCount))
            {
                bMore = TRUE;
            }
        }

        CCEMAINView*    pPLVView = GetPLView();
        if(pPLVView)
        {
            pPLVView->OnIdle(lCount);
        }
    }

#ifdef CEMAIN_CRT_MEMORY_DEBUG
    _ASSERTE( _CrtCheckMemory( ) );
#endif

    return (bMore);	
}


CCEMAINView* CCEMAINApp::GetPLView()
{
    CCEMAINView*    pView = NULL;
    CCEMAINDoc*     pDoc = NULL;

    POSITION posdoc = m_pPrjTemplate->GetFirstDocPosition();
    pDoc = dynamic_cast<CCEMAINDoc*>(m_pPrjTemplate->GetNextDoc(posdoc));
    if(pDoc)
    {
        POSITION posview = pDoc->GetFirstViewPosition();
        pView = dynamic_cast<CCEMAINView*>(pDoc->GetNextView(posview));
    }

    return pView;
}

void CCEMAINApp::SetShuttingDown (bool bShuttingDown)
{
    m_bShuttingDown = bShuttingDown;
}


bool CCEMAINApp::IsShuttingDown ()
{
    return (m_bShuttingDown);
}


BOOL CCEMAINApp::OnDDECommand (LPTSTR lpszCommand)
{

    // Do not handle DDE requests during shutdown.

    if (m_bShuttingDown)
    {
        AfxMessageBox (IDS_NO_DDE_DURING_SHUTDOWN);
        return (TRUE);
    }

    return (CWinApp::OnDDECommand (lpszCommand));
}


void CCEMAINApp::CloseAllPanelViews()
{
    if(m_pPanelTemplate)
    {
        m_pPanelTemplate->CloseAllDocuments(TRUE);
    }
}

// avoid pou var selection processing opening document on opening project
BOOL CCEMAINApp::IsOpeningProject()
{
	return m_bOpeningProject;
}



/////////////////////////////////////////////////////////////////////////////
// CCEMAINApp message handlers


