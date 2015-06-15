


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "CESys.h"
#include "cemain.h"
#include "resource.h"
#include "panel.h"
#include "PanelVw.h"
#include "panelrc.h" //OH: was not there
//#include "prjdoc.h"
#include "CEFileHandler.h"
#include "fc_tools\fc_string.h"
#include "CEMain.h"
#include "cecompman.h"
#include "MainFrm.h"

#include <WinInet.h>
//----  Static Initializations:   ----------------------------------*


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPanelView

IMPLEMENT_DYNCREATE(CPanelView, CHtmlView)

CPanelView::CPanelView()
{
}

CPanelView::~CPanelView()
{
}


BEGIN_MESSAGE_MAP(CPanelView, CHtmlView)
	//{{AFX_MSG_MAP(CPanelView)
	ON_COMMAND(IDM_GO_BACK, OnGoBack)
	ON_COMMAND(IDM_GO_FORWARD, OnGoForward)
	ON_COMMAND(IDM_GO_SEARCH_THE_WEB, OnGoSearchTheWeb)
	ON_COMMAND(IDM_GO_START_PAGE, OnGoStartPage)
	ON_COMMAND(IDM_GO_STOP, OnViewStop)
	ON_UPDATE_COMMAND_UI(IDM_GO_STOP, OnUpdateViewStop)
	ON_COMMAND(IDM_GO_REFRESH, OnViewRefresh)
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPanelView drawing

void CPanelView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CPanelView diagnostics

#ifdef _DEBUG
void CPanelView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CPanelView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG

CPanelDoc* CPanelView::GetDocument()
{
	assert(m_pDocument->IsKindOf(RUNTIME_CLASS(CPanelDoc)));
	return (CPanelDoc*)m_pDocument;
}
/////////////////////////////////////////////////////////////////////////////
// CPanelView message handlers

void CPanelView::OnInitialUpdate() {
	CHtmlView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
    CPanelDoc *pDoc;
    CString strURL;
    CString strTitle;

	pDoc = GetDocument();
    assert(pDoc != NULL);
    strTitle.LoadString(IDS_CONSOLE_TITLE);
    //pDoc->SetTitle(strTitle);
    strURL = pDoc->GetURL();
    Navigate2(strURL, 0, 0, 0, 0);
    return;
}

void CPanelView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
    CPanelDoc *pDoc;
    CString strURL;
    CString strTitle;

    switch(lHint) {
        case PANELDOC_UPD_DEF:
            pDoc = GetDocument();
            assert(pDoc != NULL);
            strTitle.LoadString(IDS_CONSOLE_TITLE);
            //pDoc->SetTitle(strTitle);
            strURL = pDoc->GetURL();
            Navigate2(strURL, 0, 0, 0, 0);
            break;
        default:
            break;
    }
}


void CPanelView::OnBeforeNavigate2 (LPCTSTR pszURL,
                                    DWORD /*nFlags*/,
                                    LPCTSTR /*pszTargetFrameName*/,
                                    CByteArray &/*postedData*/,
                                    LPCTSTR /*pszHeaders*/,
                                    BOOL *pbCancel)
{
    CString strURL=pszURL;
    CPanelDoc *pDoc;
    int iPos;
    CString strScheme;

    pDoc = GetDocument ();


    // InternetCrackUrl does not behave well if an URL
    // like "4c://e:\..." is used. Thus we crack URLs
    // like "4c:..." manually.

    iPos = strURL.Find (_T (':'));
    if (iPos == -1)
        return;

    strScheme = strURL.Left (iPos);
    if (strScheme.CompareNoCase (CONSOLE_4C_URL_SCHEME) == 0)
    {
        NavigateTo4C (strURL.Mid (iPos + 1));

        *pbCancel = TRUE;
        return;
    };

    if (strScheme.CompareNoCase (CONSOLE_4CFILE_URL_SCHEME) == 0)
    {
        NavigateTo4CFile (strURL.Mid (iPos + 1));

        *pbCancel = TRUE;
        return;
    };

    // Should implement URLs like 4CHTTP:<PC name>/<http path relative to ATCM web address of specified PC>
#if 0
    if (strScheme is some 4chttp)
    {
        CString strMachine;

        strMachine = extract logical pc name from strScheme;

        if (strMachine is localhost or empty)
        {
            NavigateTo4CHttp ("", strURL.Mid (iPos + 1));
        }
        else
        {
            CString strAddress;

            strAddress = get machine address for logical pc name in strMachine;

            NavigateTo4CHttp (strAddress, strURL.Mid (iPos + 1));
        };

        return;
    };
#endif

    if (pDoc != NULL)
    {
        pDoc->SetPathName (strURL);
    };
}


void CPanelView::NavigateTo4C (CString strPath)
{
    CCEMAINApp *pApp;
    CCEFileHandler *pFileHandler;
    CGenEdDoc *pDocument;


    // Try to get file handler. If we did not get
    // the file handler (internal error!) we cannot
    // navigate.

    pApp = dynamic_cast<CCEMAINApp *> (AfxGetApp ());
    ASSERT (pApp != NULL);
    if (pApp == NULL)
        return;

    pFileHandler = pApp->GetFileHandler ();
    ASSERT (pFileHandler != NULL);
    if (pFileHandler == NULL)
        return;


    // For strange reasons if an URL like "4c://..." is used
    // the control appends a slash. Remove this slash.

    if (strPath[strPath.GetLength () - 1] == _T ('/'))
    {
        strPath = strPath.Left (strPath.GetLength () - 1);
    }

    // Open document in container
    pFileHandler->ShowFile (strPath, pDocument);
}


void CPanelView::NavigateTo4CFile (CString strPath)
{
    CCEMAINApp *pApp;
    //CPrjDoc *pProject;
    CString strURL;


    // Try to get project document. If we did not
    // get the project document (internal error!)
    // we cannot navigate.

    pApp = dynamic_cast<CCEMAINApp *> (AfxGetApp ());
    ASSERT (pApp != NULL);
    if (pApp == NULL)
        return;

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


    // For strange reasons if an URL like "4cfile://..." is
    // used the control appends a slash. Remove this slash.

    if (strPath[strPath.GetLength () - 1] == _T ('/'))
    {
        strPath = strPath.Left (strPath.GetLength () - 1);
    };


    // If the path is relative make it absolute.

    if (FC_StringIsAbsPath(strPath)==0)
    {
        strPath = prjPath + _T("\\") + strPath;
    }


    // Build the corresponding file URL

    strURL = _T ("file:") + strPath;

    Navigate2 (strURL);
}


void CPanelView::NavigateTo4CHttp (CString strAddress, CString strPath)
{
    CString strURL;


    // For strange reasons if an URL like "4chttp://..." is
    // used the control appends a slash. Remove this slash.

    if (strPath[strPath.GetLength () - 1] == _T ('/'))
    {
        strPath = strPath.Left (strPath.GetLength () - 1);
    };


    if (strAddress.IsEmpty ())
    {
        TCHAR szLocalMachine[100];


        // Get local machine name. If we did not get
        // the local machine name we cannot navigate.

        //OH:
        //if (UTIL_GetLocalMachineName (szLocalMachine, sizeof (szLocalMachine)) != S_OK)
        //    return;

        strAddress = szLocalMachine;
        if (strAddress.IsEmpty ())
            return;
    };

    strURL.Format ("http://%s/ATCM", (LPCTSTR)strAddress);

    if (!strPath.IsEmpty ())
    {
        strURL += _T ("/") + strPath;
    };

    Navigate2 (strURL);
}


// these are all simple one-liners to do simple controlling of the browser
void CPanelView::OnGoBack()
{
    GoBack();
}

void CPanelView::OnGoForward()
{
    GoForward();
}

void CPanelView::OnGoSearchTheWeb()
{
    GoSearch();
}

void CPanelView::OnGoStartPage()
{
    GoHome();
}

void CPanelView::OnViewStop()
{
    Stop();
}

void CPanelView::OnViewRefresh()
{
    Refresh();
}

void CPanelView::OnUpdateViewStop(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(GetBusy() != 0);	
}

void CPanelView::OnSetFocus(CWnd* pOldWnd)
{

    // Update problems if panel views frame is partially obscured
    // by another frame and then activated by clicking into the
    // client area. Forcing repaint of embedded controls container
    // window seems to solve the problem.

    CHtmlView::OnSetFocus (pOldWnd);
    m_wndBrowser.Invalidate ();
    m_wndBrowser.UpdateWindow ();
}


void CPanelView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
    if (bActivate)
    {
        CMainFrame* pMainFrame = NULL;
        pMainFrame = dynamic_cast<CMainFrame* > (AfxGetMainWnd ());
        ASSERT(pMainFrame);
        if (pMainFrame != NULL)
        {
            pMainFrame->SetVarSelectionHint(_T(""));
        }
    }
    
	CHtmlView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}


