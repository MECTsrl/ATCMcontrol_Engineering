//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "LATED.h"
#include "lateddoc.h"
#include "srvritem.h"
#include "latedview.h"
#include "latedif_i.c"
#include "StdEditorIF_i.c"
#include "CEBackparse_i.c"
#include "celatederror.h"
#include "lcconfiggen.h"
#include "cemaindrivers.h"
#include "toolbox.h"
//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLATEDDoc

IMPLEMENT_DYNCREATE(CLATEDDoc, CStdEditorDoc)

BEGIN_MESSAGE_MAP(CLATEDDoc, CStdEditorDoc)
	//{{AFX_MSG_MAP(CLATEDDoc)
	ON_COMMAND(IDM_GEN, OnGenerate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CLATEDDoc, CStdEditorDoc)
	//{{AFX_DISPATCH_MAP(CLATEDDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//      DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_ILATED to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {86046CB4-294D-4EB0-9411-13EB84F3888F}
static const IID IID_ILATED =
{ 0x86046CB4, 0x294D, 0x4EB0, { 0x94, 0x11, 0x13, 0xEB, 0x84, 0xF3, 0x88, 0x8F } };

BEGIN_INTERFACE_MAP(CLATEDDoc, CStdEditorDoc)
	INTERFACE_PART(CLATEDDoc, IID_ILATED, Dispatch)
    INTERFACE_PART(CLATEDDoc, IID_ICEBackparse, BackParse)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLATEDDoc construction/destruction

CLATEDDoc::CLATEDDoc()
{
    m_pConfig = NULL;
    m_bDelayClose = false;
    //see DeleteContents() for the usage/description of m_bNoDelContents
    m_bNoDelContents = false;
	EnableAutomation();
    EnableConnections ();
	AfxOleLockApp();
}

CLATEDDoc::~CLATEDDoc()
{
    DeleteConfig();
	AfxOleUnlockApp();
}

BOOL CLATEDDoc::OnNewDocument()
{
    HRESULT hr = S_OK;
    ICEProjectManager* pPM = NULL;

    //Base class function calls DeleteContents()
    //this in turn calls DeleteConfig() to delete any persisting configuration
    //Hence a call to DeleteConfig() is not necessary here.
    if (!CXMLDocument<CStdEditorDoc>::OnNewDocument()) {
		return FALSE;
    }
    
    m_pConfig = new CLCConfig(GetPathName());
    if(!m_pConfig) {
        return FALSE;
    }

    GetProjectManager(&pPM);
    m_pConfig->SetPM(pPM);
    m_pConfig->GetIECTypes();

    LATED_HINT_TYPE type = UPDATE_NEW;
    UpdateAllViews(NULL,type);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CLATEDDoc server implementation

COleServerItem* CLATEDDoc::OnGetEmbeddedItem()
{
	// OnGetEmbeddedItem is called by the framework to get the COleServerItem
	//  that is associated with the document.  It is only called when necessary.

	CLATEDSrvrItem* pItem = new CLATEDSrvrItem(this);
	ASSERT_VALID(pItem);
	return pItem;
}

/////////////////////////////////////////////////////////////////////////////
// CLATEDDoc Active Document server implementation

CDocObjectServer *CLATEDDoc::GetDocObjectServer(LPOLEDOCUMENTSITE pDocSite)
{
	return new CDocObjectServer(this, pDocSite);
}


/////////////////////////////////////////////////////////////////////////////
// CLATEDDoc diagnostics

#ifdef _DEBUG
void CLATEDDoc::AssertValid() const
{
	COleServerDoc::AssertValid();
}

void CLATEDDoc::Dump(CDumpContext& dc) const
{
	COleServerDoc::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CLATEDDoc public operations 

void CLATEDDoc::OnSerializeDocument (CArchive &ar)
{}

void CLATEDDoc::DeleteContents()
{
    DeleteConfig();
    if( !m_bNoDelContents ) { 
        /*  only if false call DeleteContents() of base class

            There is a handshake between the COleServerDoc/COleServerItem and the 
            COleClientItem residing within the container. The COleClientItem implements
            IAdviseSink and advises itself to the COleServerItem to 
            receive notifications e.g. about closing operation of the server
            object. 
            The base class method of CStdEditorDoc calls COleServerDoc::DeleteContents()
            which in turn destroys all COleServerItem objects attached and therefore 
            'forgets' all advised COleClientItem objects.

            Hence COleServerDoc::DeleteContents() shall not be called to often
            especially not if a document is loaded via LoadDocument() because
            it releases client item objects (COleClientItem) that have already
            been advised. The impact is that no notifications (especially when
            the document itself is closed) will be propagated to the client item
            object and this leads to unpredicted results.
        */
        CStdEditorDoc::DeleteContents();
    }
}

void CLATEDDoc::DeleteConfig()
{
    if(m_pConfig) {
        m_pConfig->Release();
        m_pConfig = NULL;
    }
}

void CLATEDDoc::OnUpdateReadOnly()
{
    CStdEditorDoc::OnUpdateReadOnly();
}

HRESULT CLATEDDoc::doBackparse(BSTR sFileName,ICEProjInfo* pICEProjInfo, BSTR* psXmlText)
{
    HRESULT hr = S_OK;

    FC_PCHECK_HR(sFileName,E_POINTER);
    FC_PCHECK_HR(pICEProjInfo,E_POINTER);
    FC_PCHECK_HR(psXmlText,E_POINTER);

    try {
        CString FileName = sFileName;
        CString Result;

        *psXmlText = NULL;
        
        int pos1 = FileName.ReverseFind(_T('\\'));
        int pos2 = FileName.ReverseFind(_T('.'));
        CString title = FileName.Mid(pos1+1, pos2-pos1-1);
        CString name = FileName.Mid(pos1+1);

        BSTR encoded;
        UTIL_XMLEncodeString(CComBSTR(title), &encoded, XML_ENCODE_DEFAULT);
        title = encoded;
        SysFreeString(encoded);
        UTIL_XMLEncodeString(CComBSTR(name), &encoded, XML_ENCODE_DEFAULT);
        name = encoded;
        SysFreeString(encoded);

        Result = LATED_BACKPARSE_NODE;
        Result += LATED_QUOTE;
        Result += title;
        Result += LATED_QUOTE;
        Result += LATED_BACKPARSE_ID; 
        Result += LATED_QUOTE;
        Result += title;
        Result += LATED_QUOTE;
        Result += LATED_BACKPARSE_TYPE;
        Result += LATED_QUOTE;
        Result += LATED_TYPE;
        Result += LATED_QUOTE;
        Result += LATED_BACKPARSE_SOURCE;
        Result += LATED_QUOTE + name + LATED_QUOTE;
        Result += LATED_BACKPARSE_ENDTAG;
        *psXmlText = Result.AllocSysString();
    }
    catch(CMemoryException* e) {
        e->Delete();
        hr = E_OUTOFMEMORY;
    }
    catch(HRESULT hrRes) {
        hr = hrRes;
    }
    return hr;
}

/////////////////////////////////////////////////////////////////////////////
// interface ICEBackparse

STDMETHODIMP_(ULONG) CLATEDDoc::XBackParse::AddRef ()
{
	METHOD_PROLOGUE_EX (CLATEDDoc, BackParse)
	return (pThis->ExternalAddRef ());
}

STDMETHODIMP_(ULONG) CLATEDDoc::XBackParse::Release ()
{
	METHOD_PROLOGUE_EX (CLATEDDoc, BackParse)
	return (pThis->ExternalRelease ());
}

STDMETHODIMP CLATEDDoc::XBackParse::QueryInterface(REFIID iid, LPVOID *ppvObj)
{
	METHOD_PROLOGUE_EX (CLATEDDoc, BackParse)
	return ((HRESULT)pThis->ExternalQueryInterface (&iid, ppvObj));
}

STDMETHODIMP CLATEDDoc::XBackParse::doBackparse(/*[in, string]*/BSTR sFileName, 
    /*[in]*/ ICEProjInfo* pICEProjInfo,/*[out, string]*/BSTR* psXmlText)
{
	METHOD_PROLOGUE_EX (CLATEDDoc, BackParse)
    return pThis->doBackparse(sFileName,pICEProjInfo,psXmlText);
}

/////////////////////////////////////////////////////////////////////////////
// CLATEDDoc commands


BOOL CLATEDDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
    HRESULT hr = S_OK;
    ICEProjectManager* pPM = NULL;
    CString str;
    LATED_HINT_TYPE type;
    CHourglass hourglass;

    LPTSTR psz = str.GetBuffer(_MAX_PATH);
    UTIL_FormatMessage(AfxGetResourceHandle(),psz,_MAX_PATH,
        S_CELATED_LOAD,lpszPathName);
    str.ReleaseBuffer();

    PrintMessage(S_CELATED_LOAD,str);

    type = UPDATE_CLEAR;
    UpdateAllViews(NULL,type,NULL);

    if (!CXMLDocument<CStdEditorDoc>::OnOpenDocument(lpszPathName)) {
		return FALSE;
    }
	
    GetProjectManager(&pPM);
    m_pConfig->SetPM(pPM);
    m_pConfig->GetIECTypes();
    m_pConfig->ActualizeLinks();

    type = UPDATE_ALL;
    UpdateAllViews(NULL,type,m_pConfig);
	return TRUE;
}

BOOL CLATEDDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
    if(!CXMLDocument<CStdEditorDoc>::OnSaveDocument(lpszPathName)) {
        return FALSE;
    }

    CStdEditorDoc::OnSaveDocument(lpszPathName);

    return TRUE;
}

void CLATEDDoc::OnCloseDocument()
{
    CString str;
    LATED_HINT_TYPE type = UPDATE_CLEAR;

    UpdateAllViews(NULL,type,NULL);

    const CString & rPathName = GetPathName();

    LPTSTR psz = str.GetBuffer(_MAX_PATH);
    UTIL_FormatMessage(AfxGetResourceHandle(),psz,_MAX_PATH,
        S_CELATED_CLOSE,(LPCTSTR)rPathName);
    str.ReleaseBuffer();

    PrintMessage(S_CELATED_CLOSE,str);

    CStdEditorDoc::OnCloseDocument();
}

HRESULT CLATEDDoc::GetProjectManager(ICEProjectManager**ppPM)
{
	HRESULT hr = S_OK;
	CComPtr<IDispatch> pGenEdCont;
	CGenEdContDriver driver;
    IUnknown* pUnk = NULL;
    
    FC_PCHECK_HR(ppPM,E_POINTER);
    *ppPM = NULL;
    
	hr = GetGenEdCont (pGenEdCont);
    if (FAILED (hr)) {
		return (hr);
	}

	ASSERT (pGenEdCont != NULL);
	if (pGenEdCont == NULL)
	{
		return (E_UNEXPECTED);
	};

	driver.AttachDispatch (pGenEdCont, FALSE);

	try{
        
        hr = driver.GetProjectManager(&pUnk);
        
        if(pUnk) {
            pUnk->QueryInterface(__uuidof(ICEProjectManager),(void**)ppPM);
            pUnk->Release();
            pUnk = NULL;
        }
	}
    catch(COleException* e)
	{
        hr = e->m_sc;
	}

	return (hr);
}

bool CLATEDDoc::SaveDocument (const CString &strFileName)
{
   if (!OnSaveDocument((LPCTSTR) strFileName))
   {
      return false;
   }
   return true;
}

bool CLATEDDoc::LoadDocument (const CString &strFileName)
{
    //see DeleteContents() for the usage/description of m_bNoDelContents
    m_bNoDelContents = true;
    if (!OnOpenDocument ((LPCTSTR) strFileName))
    {
        m_bNoDelContents = false;                
        return false;
    }

    m_bNoDelContents = false;
    SetPathName(strFileName,TRUE);
    return true;
}

void CLATEDDoc::SerializeXML(CXMLArchive& ar)
{
    HRESULT hr = S_OK;
    CLCConfig* pConfig = NULL;
    CXMLObject* pObj = NULL;
    CString str;

    try {
        RegisterXMLTypes(ar);

        if(ar.IsStoring()) {
            if(m_pConfig) {
                ar.SetDocType(_T("LATCOMM_4C"));
                m_pConfig->SerializeXML(ar);
            }
            else {
                ar.SetDocType(_T("LATCOMM_4C"));
                return;
            }
        }
        else {
            DeleteConfig();
            pObj = ar.ReadObject();
            pConfig = dynamic_cast<CLCConfig*>(pObj);
            assert(pConfig);
            m_pConfig = pConfig;
            m_pConfig->ResolveDatatypes();
        }
    }
    catch(HRESULT hrRes) {
        hr = HandleParseError(hrRes,ar,str);
        PrintMessage(hr,str);
        
    }
    catch(CMemoryException* e) {
        e->Delete();
        hr = E_OUTOFMEMORY;
    }
}

void CLATEDDoc::RegisterXMLTypes(CXMLArchive &ar)
{
   ar.AddToClassMap(RUNTIME_CLASS(CLCConfig), LATED_ELEM_LATCOMM);
   ar.AddToClassMap(RUNTIME_CLASS(CLCDataType), LATED_ELEM_LC_DATATYPE);
   ar.AddToClassMap(RUNTIME_CLASS(CLCMember), LATED_ELEM_LC_MEMBER);
   ar.AddToClassMap(RUNTIME_CLASS(CLCLink), LATED_ELEM_LC_LINK);
   ar.AddToClassMap(RUNTIME_CLASS(CLCDestLink),LATED_ELEM_LC_LINK_DEST);
   ar.AddToClassMap(RUNTIME_CLASS(CLCGenObj),LATED_ELEM_LC_GEN);
}

HRESULT CLATEDDoc::HandleParseError(HRESULT hrRes, CXMLArchive& ar, CString & strDesc)
{
    CString str,strMsg;
    CComQIPtr<IXMLError>  pXMLError;
    CComQIPtr<IXMLError2>  pXMLError2;
    IXMLElement2* pElem = NULL;
    IXMLDocument2* pDoc = NULL;
    LPTSTR psz = NULL;
    HRESULT hr = S_OK;
    

    //try with last parse error first
    hr = ar.GetLastParseError(str);
    if(hr == S_OK) {
        strDesc = str;
        return hrRes;
    }

    //now try with current element node
    ar.GetCurrentElement(&pElem);
    //no current element node
    if(!pElem) {
        //try with root node
        ar.GetXMLDocument(&pDoc);
        if(pDoc) {
            pXMLError = pDoc;
            pXMLError2 = pDoc;
        }
    }
    else { //element existing - ask for error interfaces.
        pXMLError = pElem;
        pXMLError2 = pElem;
    }

    if(pXMLError || pXMLError2) { // one of the error interfaces
        XML_ERROR xmle;
        // use the improved error interface if available
        if (pXMLError2)
        {
            BSTR    sReason;
            hr = pXMLError2->get_reason(&sReason);
            strMsg = sReason;
            FC_FREE_BSTR(sReason);

            psz = str.GetBuffer(_MAX_PATH);
            UTIL_FormatMessage(AfxGetResourceHandle(),psz,_MAX_PATH,
                E_CELATED_XML_ERR,(LPCTSTR)strMsg);
            str.ReleaseBuffer();
        }
        else
        {
            CString strFound,strExpected;
            pXMLError->GetErrorInfo(&xmle);
            
            strFound = xmle._pszFound;
            strExpected = xmle._pszExpected;

            psz = str.GetBuffer(_MAX_PATH);
            UTIL_FormatMessage(AfxGetResourceHandle(),psz,_MAX_PATH,
                E_CELATED_XML_ERR2,xmle._nLine,(LPCTSTR)strFound,(LPCTSTR)strExpected);
            str.ReleaseBuffer();

            FC_FREE_BSTR(xmle._pszFound);
            FC_FREE_BSTR(xmle._pszExpected);
            FC_FREE_BSTR(xmle._pchBuf);
        }

    }

    if(str.IsEmpty()) {
        LPTSTR psz = str.GetBuffer(_MAX_PATH);
        UTIL_FormatMessage(AfxGetResourceHandle(),psz,_MAX_PATH,hrRes);
        str.ReleaseBuffer();
        strDesc = str;
    }

    FC_RELEASE_PTR(pElem);
    FC_RELEASE_PTR(pDoc);
    return hrRes;
}

bool CLATEDDoc::QueryDelayClose ()
{
    if (CStdEditorDoc::QueryDelayClose ())
    {
        return (true);
    };

    return (m_bDelayClose);
}


void CLATEDDoc::PrintMessage(HRESULT hr, CString & str)
{
    bool bDelayClose = m_bDelayClose;
    m_bDelayClose = true;
    CStdEditorDoc::PrintMessage(HRESULT_FACILITY(hr),hr,str);   
    m_bDelayClose = bDelayClose;
}

// Only if LATED_SPLITTER defined

#ifdef LATED_SPLITTER

COleIPFrameWnd* CLATEDDoc::CreateInPlaceFrame(CWnd* pParentWnd) 
{
    ASSERT_VALID(this);
    ASSERT_VALID(pParentWnd);
    
    // Get run-time class from the doc template.
    CDocTemplate* pTemplate = GetDocTemplate();
    ASSERT_VALID(pTemplate);
    
    // Use existing view if possible.
    CView* pView = NULL;
    CFrameWnd* pFrame = GetFirstFrame();
    if (pFrame != NULL)
    {
        pView = (CView*)pFrame->GetDescendantWindow(AFX_IDW_PANE_FIRST, TRUE);
        if (pView != NULL)
        {
            ASSERT_KINDOF(CView, pView);
            m_dwOrigStyle = pView->GetStyle();
            m_dwOrigStyleEx = pView->GetExStyle();
        }
    }
    
    // Create the frame from the template, ALWAYS use the CView of the
    // CSplitterWnd.
    COleIPFrameWnd* pFrameWnd = (COleIPFrameWnd*) pTemplate->CreateOleFrame(pParentWnd, this, FALSE);
    if (pFrameWnd == NULL)
        return NULL;
    
    // Connect the splitter window (CSplitterWnd class) to the
    // COleDocIPFrameWnd.
    CWnd* split = pFrame->GetWindow(GW_CHILD);
    VERIFY(pFrame == split->SetParent(pFrameWnd));
    
    // Remember the original parent window for deactivate--uses the
    // CFrameWnd-derived class (that is, CMDIChildWnd for MDI).
    m_pOrigParent = pFrame;
    
    // Set the active view of COleDocIPFrameWnd.
    pFrameWnd->SetActiveView(pView, FALSE);
    pFrameWnd->RecalcLayout();
    pView->ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_DRAWFRAME);

    // force activation of the GR-View
    POSITION pos = GetFirstViewPosition();
    while(pos)
    {
       CView *pView = GetNextView(pos);
       if(dynamic_cast<CLATEDView*>(pView))
       {
          pFrameWnd->SetActiveView(pView);
          pFrameWnd->RecalcLayout();
       }
    }
    
    // Verify the type.
    ASSERT_VALID(pFrameWnd);
    ASSERT_KINDOF(COleIPFrameWnd, pFrameWnd);
    return pFrameWnd;
}


void CLATEDDoc::DestroyInPlaceFrame(COleIPFrameWnd* pFrameWnd) 
{
   
    ASSERT_VALID(this);
    ASSERT_VALID(pFrameWnd);
    
    // connect view to original, if existing view was used
    if (m_pOrigParent != NULL)
    {
        CView* pView = (CView*)pFrameWnd->GetDescendantWindow(
            AFX_IDW_PANE_FIRST, TRUE);
        ASSERT_VALID(pView);
        
        // Leaving the focus on an MDI child or one of its child
        // windows causes Windows to get confused when the child window
        // is destroyed, not to mention the fact that the focus will be
        // out of sync with activation.
        if (::GetFocus() == pView->m_hWnd)
        {
            // Move focus to somewhere safe.
            HWND hWnd = ::GetParent(pFrameWnd->m_hWnd);
            if (hWnd != NULL)
                ::SetFocus(hWnd);
            
            // Check again.
            if (::GetFocus() == pView->m_hWnd)
                SetFocus(NULL); // last ditch effort
        }
        
        // Set parent of the splitter window (CSplitterWnd class) to be
        // the CFrameWnd-derived class (that is, CMDIChildWnd for MDI
        // application).
        ASSERT_KINDOF(CFrameWnd, m_pOrigParent);
        CFrameWnd* frame = (CFrameWnd*) m_pOrigParent;
        CWnd* split = pFrameWnd->GetWindow(GW_CHILD);
        VERIFY(pFrameWnd == split->SetParent(frame));
        
        // Set the active view of CFrameWnd-derived class (that is,
        // CMDIChildWnd for MDI application).
        frame->SetActiveView(pView, FALSE);
        frame->RecalcLayout();
        
        m_pOrigParent = NULL;
        
        // Remove any scrollbars added because of in-place activation.
        if ((m_dwOrigStyle & (WS_HSCROLL|WS_VSCROLL)) == 0 &&
            (pView->GetStyle() & (WS_HSCROLL|WS_VSCROLL)) != 0)
        {
            ::SetScrollRange(pView->m_hWnd, SB_HORZ, 0, 0, TRUE);
            ::SetScrollRange(pView->m_hWnd, SB_VERT, 0, 0, TRUE);
        }
        
        // Restore old 3D style.
        pView->ModifyStyleEx(0, m_dwOrigStyleEx & WS_EX_CLIENTEDGE,
            SWP_DRAWFRAME);
        
        // Force recalc layout on splitter window.
        CSplitterWnd* pSplitter = CView::GetParentSplitter(pView,
            TRUE);
        if (pSplitter != NULL)
            pSplitter->RecalcLayout();
    }
    
    // No active view or document during destroy.
    pFrameWnd->SetActiveView(NULL);
    
    // Destroy in-place frame window.
    pFrameWnd->DestroyWindow();
}
#endif

void CLATEDDoc::OnGenerate() 
{
    CString strPath = GetPathName();
    ASSERT(!strPath.IsEmpty());
    CHourglass hourglass;

    ASSERT(m_pConfig);
    m_pConfig->ActualizeLinks();

    if(IsModified()) {
        SaveDocument(strPath);
    }
    
    CDialogGen dialog(m_pConfig);
	switch(dialog.DoModal()) {
	    case IDOK:
            SaveDocument(strPath);
            break;
        case IDCANCEL:
            break;
        default:
            break;
    }    
}

/////////////////////////////////////////////////////////////////////////////
// CDialogGen dialog

CDialogGen::CDialogGen(CLCConfig* pConfig,CWnd* pParent /*=NULL*/)
	: CDialog(CDialogGen::IDD, pParent)
{
    ASSERT(pConfig);
    m_pConfig = pConfig;
    m_pConfig->AddRef();
	//{{AFX_DATA_INIT(CDialogGen)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CDialogGen::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogGen)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

CDialogGen::~CDialogGen()
{
    FC_RELEASE_PTR(m_pConfig);
}

BOOL CDialogGen::OnInitDialog() 
{
    WPARAM wParam;
    LPARAM lParam;

    CDialog::OnInitDialog();
	
    ASSERT(m_pConfig);
    m_pConfig->AddMessageListener(this);
    CWnd* pWnd = GetDlgItem(IDC_GEN);

    if(!pWnd) {
        return TRUE;
    }

    lParam = (LPARAM)pWnd->GetSafeHwnd();
    wParam = MAKEWPARAM(IDC_GEN,BN_CLICKED);
    PostMessage(WM_COMMAND,wParam,lParam);
    
    // TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogGen::OnOK()
{
    ASSERT(m_pConfig);
    m_pConfig->RemoveMessageListener(this);
    CDialog::OnOK();
}

void CDialogGen::OnCancel()
{
    ASSERT(m_pConfig);
    m_pConfig->RemoveMessageListener(this);
    CDialog::OnCancel(); 
}

void CDialogGen::OnMessage(HRESULT hr, LPCTSTR pszMessage)
{
    try {
        CListBox * pBox = (CListBox*) GetDlgItem(IDC_GEN_MESSAGES);
        if(pBox) {
            CString str = pszMessage;
            str.TrimRight();
            pBox->AddString(str);
        }
    }
    catch(CMemoryException* e) {
        e->Delete();
    }
}

void CDialogGen::EnableHScroll()
{
    try {
        CString str;
        CSize   sz;
        int     dx=0;

        // The pointer to my list box.
        CListBox * pBox = (CListBox*) GetDlgItem(IDC_GEN_MESSAGES);
        if(!pBox) {
            return;
        }

        // Find the longest string in the list box.
        CDC* pDC = pBox->GetDC();
        if(!pDC) {
            return;
        }

        for (int i=0;i < pBox->GetCount();i++)
        {
           pBox->GetText( i, str );
           sz = pDC->GetTextExtent(str);

           if (sz.cx > dx)
              dx = sz.cx;
        }
        pBox->ReleaseDC(pDC);

        // Set the horizontal extent so every character of all strings 
        // can be scrolled to.
        pBox->SetHorizontalExtent(dx);
    }
    catch(CMemoryException* e) {
        e->Delete();
    }
}

BEGIN_MESSAGE_MAP(CDialogGen, CDialog)
	//{{AFX_MSG_MAP(CDialogGen)
	ON_BN_CLICKED(IDC_GEN, OnGenerate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogGen message handlers
void CDialogGen::OnGenerate() 
{
    CListBox * pBox = (CListBox*) GetDlgItem(IDC_GEN_MESSAGES);
    if(pBox) {
        pBox->ResetContent();
    }
    
    int iErrors = m_pConfig->Check();
    
    if(iErrors > 0) {
        EnableHScroll();
        return;
    }

    CLCGen gen;
    gen.Generate(m_pConfig);
}
