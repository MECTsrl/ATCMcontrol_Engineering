


// PouVarSelWindow.cpp: implementation of the CPouVarSelWindow class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PouVarSelWindow.h"
#include "PouVarSelection_i.c"

#include "CEMAIN.h"
#include "GenEdCont.h"
#include "GenEdDoc.h"
#include "MainFrm.h"


//----  Local Defines:   -------------------------------------------*
#define POU_VAR_SEL_LEFT_MARGIN 7
#define POU_VAR_SEL_RIGHT_MARGIN 7
#define POU_VAR_SEL_TOP_MARGIN 7
//#define POU_VAR_SEL_TOP_MARGIN 17
#define POU_VAR_SEL_BOTTOM_MARGIN 7


IMPLEMENT_DYNCREATE(CPouVarSelWindow, CSizedControlBar )


BEGIN_MESSAGE_MAP(CPouVarSelWindow, CSizedControlBar )
    //{{AFX_MSG_MAP(CPouVarSelWindow)
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
CPouVarSelWindow::CPouVarSelWindow () :   
    m_StateChangeCB(this, OnStateChange)
{
    CCEStateManager *pStateManager = NULL;
    pStateManager = CEGetStateManager();
    ASSERT(pStateManager);
    if (pStateManager != NULL)
    {
        pStateManager->RegisterCallback (&m_StateChangeCB);
    }

    m_pPouVarSelEventHandler = NULL;
    m_dwPouVarSelEventHandlerCookie = NULL;
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
CPouVarSelWindow::~CPouVarSelWindow ()
{
    
}



void CPouVarSelWindow::OnStateChange (CEStateNotify stateNotify, unsigned int uiStage)
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
        InitPouVarSelection();
        break;
    case CEPreCloseProjectNotify:
        UnadvisePouVarSelEventHandler();
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
CPouVarSelCtrlWrapper* CPouVarSelWindow::GetWrapper()
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
int CPouVarSelWindow::OnCreate (LPCREATESTRUCT lpCreateStruct)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());

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

    geometry.left = POU_VAR_SEL_LEFT_MARGIN;
    geometry.top = POU_VAR_SEL_TOP_MARGIN;
    geometry.right = lpCreateStruct->cx - POU_VAR_SEL_RIGHT_MARGIN;
    geometry.bottom = lpCreateStruct->cy - POU_VAR_SEL_BOTTOM_MARGIN;

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

    AdvisePouVarSelEventHandler();  
    
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

void CPouVarSelWindow::InitPouVarSelection()
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	pMainFrame->UpdateStatusBar(_T("Initializing POU / Variable Selection ..."));
    m_ControlWrapper.InitPouVarSelection();
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
void CPouVarSelWindow::OnSize (UINT nType, int cx, int cy)
{
    CRect geometry;

    SECControlBar::OnSize (nType, cx, cy);

    // Resize Active X Control wrapper to fit into our client area.
    // CWnd's MoveWindow method honours MFC's Active X Control wrapper
    // implementation. See file header for why this is important.
    //////////////////////////////////////////////////////////////////
    geometry.left = POU_VAR_SEL_LEFT_MARGIN;
    geometry.top = POU_VAR_SEL_TOP_MARGIN;
    geometry.right = cx - POU_VAR_SEL_RIGHT_MARGIN;
    geometry.bottom = cy - POU_VAR_SEL_BOTTOM_MARGIN;

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
void CPouVarSelWindow::OnSetFocus (CWnd *pOldWnd)
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
BOOL CPouVarSelWindow::OnEraseBkgnd (CDC *pDC)
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





HRESULT CPouVarSelWindow::AdvisePouVarSelEventHandler()
{
    HRESULT hr;

    IUnknown* pUnk = m_ControlWrapper.GetControlUnknown();
    ASSERT(pUnk);
    if (!pUnk)
    {
        return E_FAIL;
    }
        
    ASSERT(m_pPouVarSelEventHandler == NULL);
    hr = CComObject<CPouVarSelEventSink>::CreateInstance(&m_pPouVarSelEventHandler);
    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        return (hr);
    }
    ASSERT (m_pPouVarSelEventHandler!= NULL);
    if (m_pPouVarSelEventHandler == NULL)
    {
        return (E_UNEXPECTED);
    }
    m_pPouVarSelEventHandler->AddRef ();

    hr = AtlAdvise (pUnk, m_pPouVarSelEventHandler->GetUnknown (), 
                    IID__IPouVarSelCtrlEvents, &m_dwPouVarSelEventHandlerCookie);
    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        return (hr);
    }
    ASSERT (m_dwPouVarSelEventHandlerCookie != NULL);
    if (m_dwPouVarSelEventHandlerCookie == NULL)
    {
        m_pPouVarSelEventHandler->Release ();
        m_pPouVarSelEventHandler = NULL;
        return (E_UNEXPECTED);
    }

    return (S_OK);
}


void CPouVarSelWindow::UnadvisePouVarSelEventHandler()
{
    HRESULT hr;

    IUnknown* pUnk = m_ControlWrapper.GetControlUnknown();
    ASSERT(pUnk);
    if (!pUnk)
    {
        return;
    }

    ASSERT (m_dwPouVarSelEventHandlerCookie != NULL);
    if (m_dwPouVarSelEventHandlerCookie == NULL)
    {
        return;
    }

    ASSERT (pUnk != NULL);
    if (pUnk == NULL)
    {
        return;
    }

    hr = AtlUnadvise (pUnk, 
                      IID__IPouVarSelCtrlEvents, 
                      m_dwPouVarSelEventHandlerCookie);

    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        ;
    }

    if (m_pPouVarSelEventHandler != NULL)
    {
        m_pPouVarSelEventHandler->Release();
        m_pPouVarSelEventHandler = NULL;
    }
}










STDMETHODIMP CPouVarSelEventSink::GetProjectManager(IUnknown** ppUnk)
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


STDMETHODIMP CPouVarSelEventSink::GetKadManager(IUnknown** ppUnk)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());

    IUnknown* pUnk;
    *ppUnk = NULL;

    pUnk = CEGetCOM(CE_COMPMAN_KADMAN);
    if(!pUnk)
    {
        return (E_FAIL);
    }
    *ppUnk = pUnk;
 
    return (S_OK);
}


STDMETHODIMP CPouVarSelEventSink::InsertFBCall(BSTR sFB)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());

    CString strFB(sFB);

    CGenEdCont* pDocCont = NULL;
    CGenEdDoc* pDoc = NULL;

    pDocCont = ((CCEMAINApp *)AfxGetApp ())->GetContainer();
    if (!pDocCont)
    {
        return E_FAIL;
    }
    pDoc = pDocCont->GetActiveDocument();
    if (!pDoc)
    {
        return E_FAIL;
    }

    pDoc->InsertFBCall(strFB);
    
    return (S_OK);
}


STDMETHODIMP CPouVarSelEventSink::InsertVariable(BSTR sVar)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());

    CString strVar(sVar);

    CGenEdCont* pDocCont = NULL;
    CGenEdDoc* pDoc = NULL;

    pDocCont = ((CCEMAINApp *)AfxGetApp ())->GetContainer();
    if (!pDocCont)
    {
        return E_FAIL;
    }
    pDoc = pDocCont->GetActiveDocument();
    if (!pDoc)
    {
        return E_FAIL;
    }

    pDoc->InsertVariable(strVar);
    
    return (S_OK);
}
	

STDMETHODIMP CPouVarSelEventSink::ShowHelpFile(BSTR sHelpFile)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());

    HRESULT hr = S_OK;
    CMainFrame* pMainFrame = NULL;
    pMainFrame = dynamic_cast<CMainFrame* > (AfxGetMainWnd ());
    if (pMainFrame == NULL)
    {
        return E_FAIL;
    }
   
    CString strHelpFile(sHelpFile);
    pMainFrame->OpenURL(strHelpFile);

    return S_OK;
}

STDMETHODIMP CPouVarSelEventSink::ShowHeaderFile(BSTR sHeaderFile)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());

    CGenEdCont* pDocCont = NULL;

    pDocCont = ((CCEMAINApp *)AfxGetApp ())->GetContainer();
    if (!pDocCont)
    {
        return E_FAIL;
    }

    CString strHeaderFile = sHeaderFile;
    pDocCont->ShowHeaderFile(strHeaderFile);
    
    return S_OK;
}


STDMETHODIMP CPouVarSelEventSink::SaveEditorFile(BSTR sEditorFile)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());

    CGenEdCont* pDocCont = NULL;

    pDocCont = ((CCEMAINApp *)AfxGetApp ())->GetContainer();
    if (!pDocCont)
    {
        return E_FAIL;
    }

    CComBSTR            sTmp;
    CComPtr<IUnknown>   pTmp;
    CComQIPtr<ICEProjInfo> pProjInfo;

    pTmp.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));

    pProjInfo = pTmp;

    pProjInfo->getProjectPath(&sTmp);
    CString strProjectPath = sTmp;

    CString strEditorFile = sEditorFile;

    CGenEdDoc *pDoc = NULL;
    pDoc = pDocCont->FindDocument (strProjectPath + _T("\\") + strEditorFile);
    if (pDoc)
    {
        pDoc->SaveIfModified(FALSE);
    }
    
    return S_OK;
}



STDMETHODIMP CPouVarSelEventSink::ReloadEditorFile(BSTR sEditorFile)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());

    CGenEdCont* pDocCont = NULL;

    pDocCont = ((CCEMAINApp *)AfxGetApp ())->GetContainer();
    if (!pDocCont)
    {
        return E_FAIL;
    }

    CComBSTR            sTmp;
    CComPtr<IUnknown>   pTmp;
    CComQIPtr<ICEProjInfo> pProjInfo;

    pTmp.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));

    pProjInfo = pTmp;

    pProjInfo->getProjectPath(&sTmp);
    CString strProjectPath = sTmp;

    CString strEditorFile = sEditorFile;

    CGenEdDoc *pDoc = NULL;
    pDoc = pDocCont->FindDocument (strProjectPath + _T("\\") + strEditorFile);
    if (pDoc)
    {
        pDoc->ReloadServer();
    }
    
    return S_OK;
}


STDMETHODIMP CPouVarSelEventSink::ShowPouVarSelection()
{
    AFX_MANAGE_STATE (AfxGetAppModuleState());

    HRESULT hr = S_OK;
    CMainFrame* pMainFrame = NULL;
    pMainFrame = dynamic_cast<CMainFrame* > (AfxGetMainWnd ());
    if (pMainFrame == NULL)
    {
        return E_FAIL;
    }
   
    pMainFrame->ShowPouVarSelection();

    return S_OK;
}
