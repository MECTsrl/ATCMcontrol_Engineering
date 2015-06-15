


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*



#include "stdafx.h"
#include "XRefWindow.h"


#include "XRef_i.c"

#include "CEMAIN.h"
#include "GenEdCont.h"
#include "GenEdDoc.h"
#include "MainFrm.h"

#include "CEFileHandler.h"


IMPLEMENT_DYNCREATE(CXRefCtrlWrapper, CWnd)


/////////////////////////////////////////////////////////////////////////////
// CXRefCtrlWrapper operations

void CXRefCtrlWrapper::LinkXRef()
{
    BOOL bDisabled = FALSE;
    CCEMAINApp* pApp = dynamic_cast<CCEMAINApp *> (AfxGetApp());
    ASSERT(pApp);

    CSettings* pSettings = pApp->GetSettings();
    if (pSettings)
    {
        bDisabled = pSettings->m_engineeringSettings.m_bXRefDeactivated;
    }

    if (!bDisabled)
    {
	    InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
    }
}

void CXRefCtrlWrapper::ChangeRefPosition(long bForward)
{
    BOOL bDisabled = FALSE;
    CCEMAINApp* pApp = dynamic_cast<CCEMAINApp *> (AfxGetApp());
    ASSERT(pApp);

    CSettings* pSettings = pApp->GetSettings();
    if (pSettings)
    {
        bDisabled = pSettings->m_engineeringSettings.m_bXRefDeactivated;
    }

    if (!bDisabled)
    {
	    static BYTE parms[] =
		    VTS_I4;
	    InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		             bForward);
    }
}

void CXRefCtrlWrapper::SetActiveQuery(LPCTSTR sVariableName)
{
    BOOL bDisabled = FALSE;
    CCEMAINApp* pApp = dynamic_cast<CCEMAINApp *> (AfxGetApp());
    ASSERT(pApp);

    CSettings* pSettings = pApp->GetSettings();
    if (pSettings)
    {
        bDisabled = pSettings->m_engineeringSettings.m_bXRefDeactivated;
    }

    if (!bDisabled)
    {
	    static BYTE parms[] =
		    VTS_BSTR;
	    InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		     sVariableName);
    }
}

void CXRefCtrlWrapper::InitXRef()
{
	InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


// OSAI feature print XRef 24.03.04 SIS >>
BOOL CXRefCtrlWrapper::GetPrintString(CString& rstrContent, int iLineLength, int iMode /*=-1*/)	// XREFPRINT 21.07.05 SIS
{
	SCODE result = S_FALSE;
    BOOL bDisabled = FALSE;
    CCEMAINApp* pApp = dynamic_cast<CCEMAINApp *> (AfxGetApp());
    ASSERT(pApp);

    CSettings* pSettings = pApp->GetSettings();
    if (pSettings)
    {
        bDisabled = pSettings->m_engineeringSettings.m_bXRefDeactivated;
    }

    if (!bDisabled)
    {
		// XREFPRINT 21.07.05 SIS >>
	    static BYTE parms[] =
		    VTS_PBSTR VTS_I4 VTS_I4;
		CComBSTR	sContent;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &sContent, iLineLength, iMode);
		// XREFPRINT 21.07.05 SIS <<
		rstrContent = sContent;
		return TRUE;
	}
	return FALSE;
}
// OSAI feature print XRef 24.03.04 SIS <<







//----  Local Defines:   -------------------------------------------*
#define XREF_LEFT_MARGIN 7
#define XREF_RIGHT_MARGIN 7
#define XREF_TOP_MARGIN 7
//#define XREF_TOP_MARGIN 17
#define XREF_BOTTOM_MARGIN 7


IMPLEMENT_DYNCREATE(CXRefWindow, CSizedControlBar )


BEGIN_MESSAGE_MAP(CXRefWindow, CSizedControlBar )
    //{{AFX_MSG_MAP(CXRefWindow)
    ON_WM_CREATE()
    ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_SHOWWINDOW()
    ON_WM_ERASEBKGND()
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
CXRefWindow::CXRefWindow () :   
    m_StateChangeCB(this, OnStateChange)
{
    CCEStateManager *pStateManager = NULL;
    pStateManager = CEGetStateManager();
    ASSERT(pStateManager);
    if (pStateManager != NULL)
    {
        pStateManager->RegisterCallback (&m_StateChangeCB);
    }

    m_pXRefEventHandler = NULL;
    m_dwXRefEventHandlerCookie = NULL;
    m_dwMsgEventHandlerCookie = NULL;
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
CXRefWindow::~CXRefWindow ()
{

}



void CXRefWindow::OnStateChange (CEStateNotify stateNotify, unsigned int uiStage)
{
    CCEStateManager *pStateManager = NULL;

    switch(stateNotify)
    {
    case CECreateComponentNotify:
        /*{
            CString strName = CESMGetComponentName ();
            if (strName == _T ("dfgsdfg"))
            {
                Init();
            }
        }*/
        break;
    case CEPreOpenProjectNotify:
        //AdviseProjEventHandler();
        break;
    case CEPostOpenProjectNotify:
        InitXRef();
        break;
    case CEPreCloseProjectNotify:
        UnadviseXRefEventHandler();

        // unregister as msg source
        if (m_dwMsgEventHandlerCookie>0)
        {
            CMainFrame* pMainFrame = NULL;
            pMainFrame = dynamic_cast<CMainFrame* > (AfxGetMainWnd ());
            if (pMainFrame != NULL)
            {
                CMsgWnd* pMsgWnd = NULL;
                pMsgWnd = pMainFrame->GetMsgView();
                if (pMsgWnd!=NULL)
                {   
                    pMsgWnd->UnregisterMsgSource(m_ControlWrapper.GetControlUnknown(), m_dwMsgEventHandlerCookie);
                }
            }
        }

        pStateManager = CEGetStateManager();
        ASSERT(pStateManager);
        if (pStateManager != NULL)
        {
            pStateManager->UnregisterCallback(&m_StateChangeCB);
        }
        break;
    case CEPostCloseProjectNotify:
       
        break;
    }
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
CXRefCtrlWrapper* CXRefWindow::GetWrapper()
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
int CXRefWindow::OnCreate (LPCREATESTRUCT lpCreateStruct)
{
    CRect geometry;

    if (SECControlBar::OnCreate (lpCreateStruct) == -1)
        return (-1);

    DWORD style = GetExBarStyle();
    style |= CBRS_EX_STDCONTEXTMENU;
    style |= CBRS_EX_ALLOW_MDI_FLOAT;
    //style |= CBRS_EX_COOL; // for cool docking tools with gripper ... like in Visual Studio.
    SetExBarStyle(style);

    // Create Active X Control wrapper. Initial size is our own
    // initial size. Automatically creates the Active X Control.
    ////////////////////////////////////////////////////////////

    geometry.left = XREF_LEFT_MARGIN;
    geometry.top = XREF_TOP_MARGIN;
    geometry.right = lpCreateStruct->cx - XREF_RIGHT_MARGIN;
    geometry.bottom = lpCreateStruct->cy - XREF_BOTTOM_MARGIN;

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

    AdviseXRefEventHandler();    

    // register as msg source

    CMainFrame* pMainFrame = NULL;
    pMainFrame = dynamic_cast<CMainFrame* > (AfxGetMainWnd ());
    if (pMainFrame != NULL)
    {
        CMsgWnd* pMsgWnd = NULL;
        pMsgWnd = pMainFrame->GetMsgView();
        if (pMsgWnd!=NULL)
        {   
            pMsgWnd->RegisterMsgSource(m_ControlWrapper.GetControlUnknown(), &m_dwMsgEventHandlerCookie);
        }
    }


    return (0);
}

void CXRefWindow::InitXRef()
{
    // TODO if XRef wants an init send it here
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	pMainFrame->UpdateStatusBar(_T("Initializing Cross Reference ..."));
    m_ControlWrapper.InitXRef();
}


// OSAI feature print XRef 24.03.04 SIS >>
BOOL CXRefWindow::PrintFile(CString& rstrPathFile, int iLineLength, int iMode /*=-1*/)	// XREFPRINT 21.07.05 SIS
{
	CString	strContent;
    if(!m_ControlWrapper.GetPrintString(strContent, iLineLength, iMode))	// XREFPRINT 21.07.05 SIS
	{
		return FALSE;
	}

    CStdioFile  tFile;
    CString     strFileName;
    CString     strTmpPath;
    VERIFY(GetTempPath(_MAX_PATH, strTmpPath.GetBuffer(_MAX_PATH)) != 0);
    strTmpPath.ReleaseBuffer();

    rstrPathFile.Format("%sXRef.txt", strTmpPath);
    ::DeleteFile(rstrPathFile);
    try
    {
        if(!tFile.Open(rstrPathFile, CFile::modeCreate | CFile::modeWrite))
        {
            tFile.Close();
            return FALSE;
        }
    }
    catch(CFileException* pFileException)
    {
        pFileException->Delete();
        return FALSE;
    }

    // print to file
	tFile.WriteString(strContent);

    tFile.Close();
    return TRUE;
}
// OSAI feature print XRef 24.03.04 SIS <<

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
void CXRefWindow::OnSize (UINT nType, int cx, int cy)
{
    CRect geometry;

    SECControlBar::OnSize (nType, cx, cy);

    // Resize Active X Control wrapper to fit into our client area.
    // CWnd's MoveWindow method honours MFC's Active X Control wrapper
    // implementation. See file header for why this is important.
    //////////////////////////////////////////////////////////////////
    geometry.left = XREF_LEFT_MARGIN;
    geometry.top = XREF_TOP_MARGIN;
    geometry.right = cx - XREF_RIGHT_MARGIN;
    geometry.bottom = cy - XREF_BOTTOM_MARGIN;

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
void CXRefWindow::OnSetFocus (CWnd *pOldWnd)
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
/*void CPouVarSelWindow::OnShowWindow (BOOL bShow, UINT nStatus)
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
BOOL CXRefWindow::OnEraseBkgnd (CDC *pDC)
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





HRESULT CXRefWindow::AdviseXRefEventHandler()
{
    HRESULT hr;

    IUnknown* pUnk = m_ControlWrapper.GetControlUnknown();
    ASSERT(pUnk);
    if (!pUnk)
    {
        return E_FAIL;
    }
        
    ASSERT(m_pXRefEventHandler == NULL);
    hr = CComObject<CXRefEventSink>::CreateInstance(&m_pXRefEventHandler);
    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        return (hr);
    }
    ASSERT (m_pXRefEventHandler!= NULL);
    if (m_pXRefEventHandler == NULL)
    {
        return (E_UNEXPECTED);
    }
    m_pXRefEventHandler->AddRef ();

    hr = AtlAdvise (pUnk, m_pXRefEventHandler->GetUnknown (), 
                    IID__IXRefCtrlEvents, &m_dwXRefEventHandlerCookie);
    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        return (hr);
    }
    ASSERT (m_dwXRefEventHandlerCookie != NULL);
    if (m_dwXRefEventHandlerCookie == NULL)
    {
        m_pXRefEventHandler->Release ();
        m_pXRefEventHandler = NULL;
        return (E_UNEXPECTED);
    }

    return (S_OK);
}


void CXRefWindow::UnadviseXRefEventHandler()
{
    HRESULT hr;

    IUnknown* pUnk = m_ControlWrapper.GetControlUnknown();
    ASSERT(pUnk);
    if (!pUnk)
    {
        return;
    }

    ASSERT (m_dwXRefEventHandlerCookie != NULL);
    if (m_dwXRefEventHandlerCookie == NULL)
    {
        return;
    }

    ASSERT (pUnk != NULL);
    if (pUnk == NULL)
    {
        return;
    }

    hr = AtlUnadvise (pUnk, 
                      IID__IXRefCtrlEvents, 
                      m_dwXRefEventHandlerCookie);

    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        ;
    }

    if (m_pXRefEventHandler != NULL)
    {
        m_pXRefEventHandler->Release();
        m_pXRefEventHandler = NULL;
    }
}




STDMETHODIMP CXRefEventSink::GetProjectManager(IUnknown** ppUnk)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());

    IUnknown* pUnk;
    *ppUnk = NULL;

    pUnk = CEGetCOM(CE_COMPMAN_PRJMAN);
    if(!pUnk)
    {
        return (E_FAIL);
    }
    *ppUnk = pUnk;
 
    return (S_OK);
}


STDMETHODIMP CXRefEventSink::ShowInEditor(BSTR sEdiPosInfo)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());

    CString strSourceFile;
    CString strEdiPos;

    CString strLocation = sEdiPosInfo;

    // extract source file and edipos from location info
    int i = strLocation.Find(_T('('));
    if (i<=0)
    {
        return E_FAIL;
    }

    strSourceFile = strLocation.Left(i);
    strSourceFile.TrimLeft();
    strSourceFile.TrimRight();

    strEdiPos = strLocation.Mid(i);
    strEdiPos.TrimLeft();
    strEdiPos.TrimRight();
    strEdiPos.TrimLeft(_T("()"));
    strEdiPos.TrimRight(_T("()"));
  
    CCEMAINApp* pApp = dynamic_cast<CCEMAINApp*>(AfxGetApp());
    ASSERT(pApp);
    if (pApp == NULL)
    {
        return E_FAIL;
    }

    CCEFileHandler *pFileHandler;
    CGenEdDoc *pDoc;

    pFileHandler = pApp->GetFileHandler();
    if (pFileHandler == NULL)
    {
        return E_FAIL;
    }

    HRESULT hr;
    hr = pFileHandler->ShowFile(strSourceFile, pDoc);
    if (FAILED (hr) || (pDoc==NULL))
    {
        // Null can be returned now 
        // ==> the file could not be opend in 4C, 
        // if possible an extern editor has been opened
        return E_FAIL;
    }
    
    CGenEdCont *pContainer;
    pContainer = pApp->GetContainer();
    if (pContainer != NULL)
    {
        pContainer->ShowLocation(pDoc, strEdiPos);
    }
    
    return S_OK;
}

// XREFPRINT 21.07.05 SIS >>
STDMETHODIMP CXRefEventSink::OnPrint()
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());
	HRESULT hr = S_OK;

    CMainFrame* pMainFrame = NULL;
    pMainFrame = dynamic_cast<CMainFrame* > (AfxGetMainWnd ());
    if (pMainFrame == NULL)
    {
		return E_FAIL;
	}

	CCEDocuManager*	pDocuMan = pMainFrame->GetDocuManager();
	hr = pDocuMan->PrintXRef(1);
  
    return hr;
}
// XREFPRINT 21.07.05 SIS <<





//////////////////////////////////////////////////////////////////////



