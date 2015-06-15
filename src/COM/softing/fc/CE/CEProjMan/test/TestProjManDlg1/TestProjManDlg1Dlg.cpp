// TestProjManDlg1Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestProjManDlg1.h"
#include "TestProjManDlg1Dlg.h"

#include "CEProjInfo_i.c"
#include "CEProjMan_i.c"
#include "CEContainer_i.c"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestProjManDlg1Dlg dialog

CTestProjManDlg1Dlg::CTestProjManDlg1Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestProjManDlg1Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestProjManDlg1Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_pProjEventHandler = NULL;
    m_dwProjEventHandlerCookie = NULL;
    m_pMsgEventHandler = NULL;
    m_dwMsgEventHandlerCookie = NULL;
    m_pCompManEventHandler = NULL;
    m_dwCompManEventHandlerCookie = NULL;
    m_pContainer = NULL;
}

CTestProjManDlg1Dlg::~CTestProjManDlg1Dlg()
{
    if (pProjMan)
    {
        pProjMan->CloseProject();
    }
    if (m_pContainer!=NULL)
    {
        m_pContainer->Release();
    }
    
}

void CTestProjManDlg1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestProjManDlg1Dlg)
	DDX_Control(pDX, IDC_EDIT_Test_2, m_edit_test_2);
	DDX_Control(pDX, IDC_EDIT_Test_ClassType, m_edit_test_ClassType);
	DDX_Control(pDX, IDC_EDIT_TEST_ERROR, m_edit_testError);
	DDX_Control(pDX, IDC_EDIT_TestOut, m_edit_testOut);
	DDX_Control(pDX, IDC_EDIT_cont_Error, m_edit_cont_Error);
	DDX_Control(pDX, IDC_EDIT_Cont_OldLink, m_edit_cont_OldLink);
	DDX_Control(pDX, IDC_EDIT_Cont_NewLink, m_edit_cont_NewLink);
	DDX_Control(pDX, IDC_EDIT_Cont_Location, m_edit_cont_Location);
	DDX_Control(pDX, IDC_EDIT_Cont_FileName, m_edit_cont_FileName);
	DDX_Control(pDX, IDC_EDIT_cont_ParentLocation, m_edit_cont_ParentLocation);
	DDX_Control(pDX, IDC_EDIT_cont_ParentFile, m_edit_cont_ParentFile);
	DDX_Control(pDX, IDC_EDIT_CP_OUTPUT, m_editCPOutput);
	DDX_Control(pDX, IDC_EDIT_Value, m_editValue);
	DDX_Control(pDX, IDC_EDIT_Section, m_editSection);
	DDX_Control(pDX, IDC_EDIT_Key, m_editKey);
	DDX_Control(pDX, IDC_EDIT_IMSG, m_editIMsg);
	DDX_Control(pDX, IDC_EDITMsg, m_editMsg);
	DDX_Control(pDX, IDC_CHANGE, m_staticChange);
	DDX_Control(pDX, IDC_EDITTREEs, m_editTrees);
	DDX_Control(pDX, IDC_EDITPROJNAME, m_editProjName);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestProjManDlg1Dlg, CDialog)
	//{{AFX_MSG_MAP(CTestProjManDlg1Dlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnOpenProj)
	ON_BN_CLICKED(IDC_BUTTON2, OnCloseProj)
	ON_BN_CLICKED(IDC_BUTTON3, OnShowClass)
	ON_BN_CLICKED(IDC_BUTTON4, OnShowTarget)
	ON_BN_CLICKED(IDC_BUTTON5, OnReparse)
	ON_BN_CLICKED(IDC_BUTTON6, OnSave)
	ON_BN_CLICKED(IDC_BUTTON7, OnClassTree)
	ON_BN_CLICKED(IDC_BUTTON8, OnTargetTree)
	ON_BN_CLICKED(IDC_BUTTON9, OnCPInsert)
	ON_BN_CLICKED(IDC_BUTTON10, OnCPDeleteKey)
	ON_BN_CLICKED(IDC_BUTTON11, OnCPDeleteKeyVal)
	ON_BN_CLICKED(IDC_BUTTON12, OnCPGetVal)
	ON_BN_CLICKED(IDC_BUTTON13, OnCPGetAllVal)
	ON_BN_CLICKED(IDC_BUTTON14, OnContGetContainer)
	ON_BN_CLICKED(IDC_BUTTON16, OnContCreate)
	ON_BN_CLICKED(IDC_BUTTON17, OnContRename)
	ON_BN_CLICKED(IDC_BUTTON15, OnContDelete)
	ON_BN_CLICKED(IDC_BUTTON18, OnContGetContainerFull)
	ON_BN_CLICKED(IDC_BUTTON19, OnTestGetLibPath)
	ON_BN_CLICKED(IDC_BUTTON20, OnTestInstPath)
	ON_BN_CLICKED(IDC_BUTTON21, OnTestGetAllClassTypes)
	ON_BN_CLICKED(IDC_BUTTON22, OnTestGetAllTargetTypes)
	ON_BN_CLICKED(IDC_BUTTON23, OnTestTextForClassType)
	ON_BN_CLICKED(IDC_BUTTON25, OnTestFileChangeHint)
	ON_BN_CLICKED(IDC_BUTTON24, OnTestFileParseNow)
	ON_BN_CLICKED(IDC_BUTTON26, OnTestGetXMLNodePtr)
	ON_BN_CLICKED(IDC_BUTTON27, OnTestGetNameProp)
	ON_BN_CLICKED(IDC_BUTTON28, OnTestGetAllClasses)
    ON_BN_CLICKED(IDC_BUTTON31, OnTestGetAllClassesIdPaths)
	ON_BN_CLICKED(IDC_BUTTON29, OnTestGetAllClassesOfType)
	ON_BN_CLICKED(IDC_BUTTON32, OnTestGetAllClassesOfTypeIdPaths)
	ON_BN_CLICKED(IDC_BUTTON30, OnTestGetAllTargets)
    ON_BN_CLICKED(IDC_BUTTON37, OnTestGetAllTargetsIdPaths)
	ON_BN_CLICKED(IDC_BUTTON33, OnTestGetAllFilesOfCat)
	ON_BN_CLICKED(IDC_BUTTON34, OnTestGetFileCat)
	ON_BN_CLICKED(IDC_BUTTON35, OnTestAddFVFile)
	ON_BN_CLICKED(IDC_BUTTON36, OnTestRemoveFVFile)
	ON_BN_CLICKED(IDC_BUTTON38, OnTestGetFileFromIdPath)
	ON_BN_CLICKED(IDC_BUTTON39, OnTestGetIdPathFromFile)
	ON_BN_CLICKED(IDC_BUTTON40, OnTestGetTypeOfTarget)
	ON_BN_CLICKED(IDC_BUTTON41, OnGetAddressStringForTarget)
	ON_BN_CLICKED(IDC_BUTTON42, OnGetUserAddrStringForTarget)
	ON_BN_CLICKED(IDC_BUTTON43, OnGetNameForAddr)
	ON_BN_CLICKED(IDC_BUTTON44, OnGetResourceForTarget)
	ON_BN_CLICKED(IDC_BUTTON45, OnGetFreeChannelNrForPC)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestProjManDlg1Dlg message handlers

BOOL CTestProjManDlg1Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	CoCreateInstance(CLSID_CEProjectManager,
                  NULL,
                  CLSCTX_INPROC_SERVER,
                  IID_ICEProjectManager,
                  (void**)&pProjMan);

    AdviseProjEventHandler();
    AdviseMsgEventHandler();
    AdviseCompManEventHandler();

    m_editProjName.SetWindowText("d:\\4ControlV2\\projects\\test\\test.4cp");
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestProjManDlg1Dlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestProjManDlg1Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTestProjManDlg1Dlg::OnOpenProj() 
{
    HRESULT hr;
	CString strProjNameWithPath;
    m_editProjName.GetWindowText(strProjNameWithPath);

    CComBSTR sFileName = strProjNameWithPath;

    hr = pProjMan->OpenProject(sFileName);	
}

void CTestProjManDlg1Dlg::OnCloseProj() 
{
	// TODO: Add your control notification handler code here
	 HRESULT hr;
	hr = pProjMan->CloseProject();
}

void CTestProjManDlg1Dlg::OnShowClass() 
{
	// TODO: Add your control notification handler code here
	HRESULT hr;
	CComBSTR strRet;
    hr = pProjMan->GetClassViewXML(&strRet);
	if (FAILED(hr))
	{
	    m_editTrees.SetWindowText("error: getting class tree");
		return;
	}

    CString strXML = (CString)strRet;

    m_editTrees.SetWindowText(strXML);
}

void CTestProjManDlg1Dlg::OnShowTarget() 
{
	// TODO: Add your control notification handler code here
	HRESULT hr;
	CComBSTR strRet;
    hr = pProjMan->GetTargetViewXML(&strRet);
	if (FAILED(hr))
	{
	    m_editTrees.SetWindowText("error: getting target tree");
		return;
	}

    CString strXML = (CString)strRet;

    m_editTrees.SetWindowText(strXML);
}

void CTestProjManDlg1Dlg::OnReparse() 
{
	// TODO: Add your control notification handler code here
    HRESULT hr;
	hr = pProjMan->ReparseCheck();	
}

void CTestProjManDlg1Dlg::OnSave() 
{
	// TODO: Add your control notification handler code here
	HRESULT hr;
	CComBSTR strRet;
    hr =  pProjMan->SaveViewData();
	if (FAILED(hr))
    {
		return;
	}
    ChangeChangeState(FALSE);
}

void CTestProjManDlg1Dlg::ChangeChangeState(BOOL state)
{
    if (state)
    {
        m_staticChange.SetWindowText("CHANGED!");
    }
    else
    {
        m_staticChange.SetWindowText("--------");
    }

}

void CTestProjManDlg1Dlg::AddMsgString(const CString& str)
{
    m_editMsg.FmtLines(TRUE);
    CString oldStr;
    m_editMsg.GetWindowText(oldStr);
    CString newStr = oldStr + "\r\n" + str;
    m_editMsg.SetWindowText(newStr);
    m_editMsg.LineScroll(m_editMsg.GetLineCount()-3);
}


void CTestProjManDlg1Dlg::AddMsgViewString(const CString& str)
{
    m_editIMsg.FmtLines(TRUE);
    CString oldStr;
    m_editIMsg.GetWindowText(oldStr);
    CString newStr = oldStr + "\r\n" + str;
    m_editIMsg.SetWindowText(newStr);
    m_editIMsg.LineScroll(m_editIMsg.GetLineCount()-3);
}


void CTestProjManDlg1Dlg::AddTestOutString(const CString& str)
{
    m_edit_testOut.FmtLines(TRUE);
    CString oldStr;
    m_edit_testOut.GetWindowText(oldStr);
    CString newStr = oldStr + "\r\n" + str;
    m_edit_testOut.SetWindowText(newStr);
    m_edit_testOut.LineScroll(m_edit_testOut.GetLineCount()-3);
}


HRESULT CTestProjManDlg1Dlg::AdviseProjEventHandler()
{
    HRESULT hr;

    ASSERT (pProjMan != NULL);
    if (pProjMan==NULL)
    {
        return (E_FAIL);
    };

    ASSERT(m_pProjEventHandler == NULL);
    hr = CComObject<CProjManEventsImpl>::CreateInstance (&m_pProjEventHandler);
    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        return (hr);
    };
    ASSERT (m_pProjEventHandler!= NULL);
    if (m_pProjEventHandler == NULL)
    {
        return (E_UNEXPECTED);
    };
    m_pProjEventHandler->AddRef ();

    hr = AtlAdvise (pProjMan, m_pProjEventHandler->GetUnknown (), 
                    IID__ICEProjectManagerEvents, &m_dwProjEventHandlerCookie);
    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        return (hr);
    };
    ASSERT (m_dwProjEventHandlerCookie != NULL);
    if (m_dwProjEventHandlerCookie == NULL)
    {
        m_pProjEventHandler->Release ();
        m_pProjEventHandler = NULL;
        return (E_UNEXPECTED);
    };

    m_pProjEventHandler->SetDlgWnd(this);

    return (S_OK);
}


void CTestProjManDlg1Dlg::UnadviseProjEventHandler()
{
    HRESULT hr;

    ASSERT (m_dwProjEventHandlerCookie != NULL);
    if (m_dwProjEventHandlerCookie == NULL)
    {
        return;
    };

    ASSERT (pProjMan != NULL);
    if (pProjMan == NULL)
    {
        return;
    };

    hr = AtlUnadvise (pProjMan, 
                      IID__ICEProjectManagerEvents, 
                      m_dwProjEventHandlerCookie);

    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        ;
    };

    if (m_pProjEventHandler != NULL)
    {
        m_pProjEventHandler->SetDlgWnd(NULL);
        m_pProjEventHandler->Release();
        m_pProjEventHandler = NULL;
    }
}





HRESULT CTestProjManDlg1Dlg::AdviseMsgEventHandler()
{
    HRESULT hr;

    ASSERT (pProjMan != NULL);
    if (pProjMan==NULL)
    {
        return (E_FAIL);
    };

    ASSERT(m_pMsgEventHandler == NULL);
    hr = CComObject<CMsgEventsImpl>::CreateInstance (&m_pMsgEventHandler);
    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        return (hr);
    };
    ASSERT (m_pMsgEventHandler!= NULL);
    if (m_pMsgEventHandler == NULL)
    {
        return (E_UNEXPECTED);
    };
    m_pMsgEventHandler->AddRef ();

    hr = AtlAdvise (pProjMan, m_pMsgEventHandler->GetUnknown (), 
                    IID__ICEMessageEvent, &m_dwMsgEventHandlerCookie);
    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        return (hr);
    };
    ASSERT (m_dwMsgEventHandlerCookie != NULL);
    if (m_dwMsgEventHandlerCookie == NULL)
    {
        m_pMsgEventHandler->Release ();
        m_pMsgEventHandler = NULL;
        return (E_UNEXPECTED);
    };

    m_pMsgEventHandler->SetDlgWnd(this);

    return (S_OK);
}


void CTestProjManDlg1Dlg::UnadviseMsgEventHandler()
{
    HRESULT hr;

    ASSERT (m_dwMsgEventHandlerCookie != NULL);
    if (m_dwMsgEventHandlerCookie == NULL)
    {
        return;
    };

    ASSERT (pProjMan != NULL);
    if (pProjMan == NULL)
    {
        return;
    };

    hr = AtlUnadvise (pProjMan, 
                      IID__ICEMessageEvent, 
                      m_dwMsgEventHandlerCookie);

    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        ;
    };

    if (m_pMsgEventHandler != NULL)
    {
        m_pMsgEventHandler->SetDlgWnd(NULL);
        m_pMsgEventHandler->Release();
        m_pMsgEventHandler = NULL;
    }
}






HRESULT CTestProjManDlg1Dlg::AdviseCompManEventHandler()
{
    HRESULT hr;

    ASSERT (pProjMan != NULL);
    if (pProjMan==NULL)
    {
        return (E_FAIL);
    };

    ASSERT(m_pCompManEventHandler == NULL);
    hr = CComObject<CCompManEventsImpl>::CreateInstance (&m_pCompManEventHandler);
    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        return (hr);
    };
    ASSERT (m_pCompManEventHandler!= NULL);
    if (m_pCompManEventHandler == NULL)
    {
        return (E_UNEXPECTED);
    };
    m_pCompManEventHandler->AddRef ();

    hr = AtlAdvise (pProjMan, m_pCompManEventHandler->GetUnknown (), 
                    IID_ICECompManEvents, &m_dwCompManEventHandlerCookie);
    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        return (hr);
    };
    ASSERT (m_dwCompManEventHandlerCookie != NULL);
    if (m_dwCompManEventHandlerCookie == NULL)
    {
        m_pCompManEventHandler->Release ();
        m_pCompManEventHandler = NULL;
        return (E_UNEXPECTED);
    };


    return (S_OK);
}


void CTestProjManDlg1Dlg::UnadviseCompManEventHandler()
{
    HRESULT hr;

    ASSERT (m_dwCompManEventHandlerCookie != NULL);
    if (m_dwCompManEventHandlerCookie == NULL)
    {
        return;
    };

    ASSERT (pProjMan != NULL);
    if (pProjMan == NULL)
    {
        return;
    };

    hr = AtlUnadvise (pProjMan, 
                      IID_ICECompManEvents, 
                      m_dwCompManEventHandlerCookie);

    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        ;
    };

    if (m_pCompManEventHandler != NULL)
    {
        m_pCompManEventHandler->Release();
        m_pCompManEventHandler = NULL;
    }
}








void CTestProjManDlg1Dlg::BuildTree(const CXMLNode& node, int einr, CString& text)
{
    int i;
    for (i=0; i<einr; i++)
    {
        text += _T(" ");
    }
    
    CString nodeText;
    CString nodeId;
    CString nodeSource;
    
    if (!node.GetAttribute(_T("TEXT"), nodeText)
        || !node.GetAttribute(_T("ID"), nodeId))
    {
        text += _T("<UNKNOWN NODE>");
    }
    else
    {
        if (node.GetAttribute(_T("SOURCE"), nodeSource))
        {
            text += nodeText + _T("    (Id: ") + nodeId + _T(")")+
                               _T("    (Source: ") + nodeSource +_T(")");
        }
        else
        {
            text += nodeText + _T("    (Id:") + nodeId + _T(")");
        }
    }
    text += _T("\r\n");

    einr += 4;

    CXMLNodeList nodeList;
    if (node.GetChildNodeList(nodeList))
    {
        CXMLNode childNode;
        while (nodeList.Next(childNode))
        {
            BuildTree(childNode, einr, text);
        }
    }

    einr -= 4;

}


void CTestProjManDlg1Dlg::OnClassTree() 
{
    IUnknown* pUnk;
    CComQIPtr<IXMLDOMDocument, &IID_IXMLDOMDocument> pdom;

	HRESULT hr;
    hr = pProjMan->GetClassViewDOM(&pUnk);
	if (FAILED(hr))
	{
	    m_editTrees.SetWindowText("error: getting target tree");
		return;
	}
    pdom = pUnk;
    pUnk->Release();

    CXMLDocument domDoc;
    if (!domDoc.SetIXMLDocument((IXMLDOMDocument*)pdom))
    {
        return;
    }
    CXMLNode rootNode;
    if (!domDoc.GetRootNode(rootNode))
    {
        return;
    }

    CString strTree;

    BuildTree(rootNode, 0, strTree);

    m_editTrees.SetWindowText(strTree);	
}

void CTestProjManDlg1Dlg::OnTargetTree() 
{
    IUnknown* pUnk;
    CComQIPtr<IXMLDOMDocument, &IID_IXMLDOMDocument> pdom;

	HRESULT hr;
    hr = pProjMan->GetTargetViewDOM(&pUnk);
	if (FAILED(hr))
	{
	    m_editTrees.SetWindowText("error: getting target tree");
		return;
	}
    pdom = pUnk;
    pUnk->Release();

    CXMLDocument domDoc;
    if (!domDoc.SetIXMLDocument((IXMLDOMDocument*)pdom))
    {
        return;
    }
    CXMLNode rootNode;
    if (!domDoc.GetRootNode(rootNode))
    {
        return;
    }

    CString strTree;

    BuildTree(rootNode, 0, strTree);

    m_editTrees.SetWindowText(strTree);		
}




void CTestProjManDlg1Dlg::OnCPInsert() 
{
	CString section;
    CString value;
    CString key;
    m_editSection.GetWindowText(section);
    m_editValue.GetWindowText(value);
    m_editKey.GetWindowText(key);

    CComBSTR ssection = section;
    CComBSTR svalue = value;
    CComBSTR skey = key;


    CComQIPtr<ICEProjInfo, &IID_ICEProjInfo> pPI;
    pPI = pProjMan;

    pPI->CPInsertToSection(ssection, skey, svalue);
	
}

void CTestProjManDlg1Dlg::OnCPDeleteKey() 
{
    CString section;
    CString value;
    CString key;
    m_editSection.GetWindowText(section);
    m_editValue.GetWindowText(value);
    m_editKey.GetWindowText(key);	

    CComBSTR ssection = section;
    CComBSTR svalue = value;
    CComBSTR skey = key;

    CComQIPtr<ICEProjInfo, &IID_ICEProjInfo> pPI;
    pPI = pProjMan;

    pPI->CPDeleteFromSection(ssection, skey);

}

void CTestProjManDlg1Dlg::OnCPDeleteKeyVal() 
{
	CString section;
    CString value;
    CString key;
    m_editSection.GetWindowText(section);
    m_editValue.GetWindowText(value);
    m_editKey.GetWindowText(key);	

    CComBSTR ssection = section;
    CComBSTR svalue = value;
    CComBSTR skey = key;

    CComQIPtr<ICEProjInfo, &IID_ICEProjInfo> pPI;
    pPI = pProjMan;

    pPI->CPDeleteKeyWithValueFromSection(ssection, skey, svalue);
	
}

void CTestProjManDlg1Dlg::OnCPGetVal() 
{
	CString section;
    CString value;
    CString key;
    m_editSection.GetWindowText(section);
    m_editValue.GetWindowText(value);
    m_editKey.GetWindowText(key);	

    CComBSTR ssection = section;
    CComBSTR svalue = "";
    CComBSTR skey = key;

    CComQIPtr<ICEProjInfo, &IID_ICEProjInfo> pPI;
    pPI = pProjMan;
	
    pPI->CPGetValueForKeyFromSection(ssection, skey, &svalue);
    CString str;
    value = svalue;
    str = key+" = " +value;
    m_editCPOutput.SetWindowText(str);
}

void CTestProjManDlg1Dlg::OnCPGetAllVal() 
{
	CString section;
    CString value;
    CString key;
    m_editSection.GetWindowText(section);
    m_editValue.GetWindowText(value);
    m_editKey.GetWindowText(key);	

    CComBSTR ssection = section;
    CComBSTR svalue = "";
    CComBSTR skey = key;

    CComQIPtr<ICEProjInfo, &IID_ICEProjInfo> pPI;
    pPI = pProjMan;

    SAFEARRAY* parrFileNames;
	
    pPI->CPGetAllValuesForKeyFromSection(ssection, skey, &parrFileNames);
    
    m_editCPOutput.SetWindowText("");
    
    BSTR HUGEP *pbstr;
    BSTR bstr;
    HRESULT hr;
    unsigned long i;

    // Get a pointer to the elements of the array.
    hr = ::SafeArrayAccessData(parrFileNames, (void HUGEP**)&pbstr);
    if (FAILED(hr))
    {
        return;
    }
    for (i = 0; i < (parrFileNames->rgsabound[0]).cElements; i++)
    {
        CString str;
        bstr = pbstr[i];
        str = key + " = " + (CString)bstr;
        m_editCPOutput.FmtLines(TRUE);
        CString oldStr;
        m_editCPOutput.GetWindowText(oldStr);
        CString newStr = oldStr + "\r\n" + str;
        m_editCPOutput.SetWindowText(newStr);
    }

    ::SafeArrayUnaccessData(parrFileNames);
    ::SafeArrayDestroy(parrFileNames);
}









void CTestProjManDlg1Dlg::OnContGetContainerFull() 
{
    m_edit_cont_Error.SetWindowText(" ");

    CString fileName;
    CString location;
    CString oldLink;
    CString newLink;
    m_edit_cont_FileName.GetWindowText(fileName);
    m_edit_cont_Location.GetWindowText(location);
    m_edit_cont_OldLink.GetWindowText(oldLink);	
    m_edit_cont_NewLink.GetWindowText(newLink);

    CComBSTR sFileName	= fileName;
    CComBSTR sLocation  = location;
    CComBSTR sOldLink   = oldLink;
    CComBSTR sNewLink   = newLink;		

    CComBSTR sParentFile;
    CComBSTR sParentLocation;

    if (m_pContainer)
    {
        m_pContainer->Release();
        m_pContainer = NULL;
    }

    CComQIPtr<ICEProjInfo, &IID_ICEProjInfo> pPI;
    pPI = pProjMan;

    IUnknown* pUnk;
    HRESULT hr = pPI->getContainerIF(sFileName, sLocation, &pUnk, 
                                     &sParentFile, &sParentLocation);
    if(hr!=S_OK)
    {
        m_edit_cont_Error.SetWindowText(" !!! error !!! ");
        return;
    }
    hr = pUnk->QueryInterface(IID_ICEContainer, (LPVOID*)&m_pContainer);
    pUnk->Release();
    if(!SUCCEEDED(hr))
    {
        m_edit_cont_Error.SetWindowText(" !!! query error !!! ");
        return;
    }

    CString parentFile = sParentFile;
    CString parentLocation = sParentLocation;

    m_edit_cont_ParentFile.SetWindowText(parentFile);
    m_edit_cont_ParentLocation.SetWindowText(parentLocation);
}


void CTestProjManDlg1Dlg::OnContGetContainer() 
{
    m_edit_cont_Error.SetWindowText(" ");

    CString fileName;
    CString location;
    CString oldLink;
    CString newLink;
    m_edit_cont_FileName.GetWindowText(fileName);
    m_edit_cont_Location.GetWindowText(location);
    m_edit_cont_OldLink.GetWindowText(oldLink);	
    m_edit_cont_NewLink.GetWindowText(newLink);

    CComBSTR sFileName	= fileName;
    CComBSTR sLocation  = location;
    CComBSTR sOldLink   = oldLink;
    CComBSTR sNewLink   = newLink;

    if (m_pContainer)
    {
        m_pContainer->Release();
        m_pContainer = NULL;
    }

    CComQIPtr<ICEProjInfo, &IID_ICEProjInfo> pPI;
    pPI = pProjMan;

    IUnknown* pUnk;
    HRESULT hr = pPI->getContainerIFforFile(sFileName, &pUnk);
    if(hr!=S_OK)
    {
        m_edit_cont_Error.SetWindowText(" !!! error !!! ");
        return;
    }
    hr = pUnk->QueryInterface(IID_ICEContainer, (LPVOID*)&m_pContainer);
    pUnk->Release();
    if(!SUCCEEDED(hr))
    {
        m_edit_cont_Error.SetWindowText(" !!! query error !!! ");
    }
}

void CTestProjManDlg1Dlg::OnContCreate() 
{
    m_edit_cont_Error.SetWindowText(" ");

    CString fileName;
    CString location;
    CString oldLink;
    CString newLink;
    m_edit_cont_FileName.GetWindowText(fileName);
    m_edit_cont_Location.GetWindowText(location);
    m_edit_cont_OldLink.GetWindowText(oldLink);	
    m_edit_cont_NewLink.GetWindowText(newLink);

    CComBSTR sFileName	= fileName;
    CComBSTR sLocation  = location;
    CComBSTR sOldLink   = oldLink;
    CComBSTR sNewLink   = newLink;
    
    if (!m_pContainer)
    {
        m_edit_cont_Error.SetWindowText("no container available");
        return;
    }

    HRESULT hr = m_pContainer->createLink(sFileName, sLocation, sOldLink);
    if (hr!=S_OK)
    {
        m_edit_cont_Error.SetWindowText("error");
    }
}

void CTestProjManDlg1Dlg::OnContRename() 
{
    m_edit_cont_Error.SetWindowText(" ");

    CString fileName;
    CString location;
    CString oldLink;
    CString newLink;
    m_edit_cont_FileName.GetWindowText(fileName);
    m_edit_cont_Location.GetWindowText(location);
    m_edit_cont_OldLink.GetWindowText(oldLink);	
    m_edit_cont_NewLink.GetWindowText(newLink);

    CComBSTR sFileName	= fileName;
    CComBSTR sLocation  = location;
    CComBSTR sOldLink   = oldLink;
    CComBSTR sNewLink   = newLink;	
    
    if (!m_pContainer)
    {
        m_edit_cont_Error.SetWindowText("no container available");
        return;
    }

    HRESULT hr = m_pContainer->renameLink(sFileName, sLocation, sOldLink, sNewLink);
    if (hr!=S_OK)
    {
        m_edit_cont_Error.SetWindowText("error");
    }
}

void CTestProjManDlg1Dlg::OnContDelete() 
{
    m_edit_cont_Error.SetWindowText(" ");

    CString fileName;
    CString location;
    CString oldLink;
    CString newLink;
    m_edit_cont_FileName.GetWindowText(fileName);
    m_edit_cont_Location.GetWindowText(location);
    m_edit_cont_OldLink.GetWindowText(oldLink);	
    m_edit_cont_NewLink.GetWindowText(newLink);

    CComBSTR sFileName	= fileName;
    CComBSTR sLocation  = location;
    CComBSTR sOldLink   = oldLink;
    CComBSTR sNewLink   = newLink;	

    if (!m_pContainer)
    {
        m_edit_cont_Error.SetWindowText("no container available");
        return;
    }

    HRESULT hr = m_pContainer->deleteLink(sFileName, sLocation, sOldLink);
    if (hr!=S_OK)
    {
        m_edit_cont_Error.SetWindowText("error");
    }
}



void CTestProjManDlg1Dlg::OnTestGetLibPath() 
{
    m_edit_testError.SetWindowText(" ");

    CComBSTR sLibPath;

    CComQIPtr<ICEProjInfo, &IID_ICEProjInfo> pPI;
    pPI = pProjMan;

    HRESULT hr = pPI->getLibraryPath(&sLibPath);
    if(hr!=S_OK)
    {
        m_edit_testError.SetWindowText(" !!! error !!! ");
        return;
    }

    CString strLibPath = sLibPath;
    AddTestOutString("Get Lib Path:");
    AddTestOutString("      " + strLibPath);
}

void CTestProjManDlg1Dlg::OnTestInstPath() 
{
	m_edit_testError.SetWindowText(" ");

    CComBSTR sInstPath;

    CComQIPtr<ICEProjInfo, &IID_ICEProjInfo> pPI;
    pPI = pProjMan;

    HRESULT hr = pPI->getInstallationPath(&sInstPath);
    if(hr!=S_OK)
    {
        m_edit_testError.SetWindowText(" !!! error !!! ");
        return;
    }

    CString strInstPath = sInstPath;
    AddTestOutString("Get Inst Path:");
    AddTestOutString("      " + strInstPath);
}




void CTestProjManDlg1Dlg::OnTestGetAllClassTypes() 
{
  	m_edit_testError.SetWindowText(" ");

    SAFEARRAY* parrClassTypes;
    
    CComQIPtr<ICEProjInfo, &IID_ICEProjInfo> pPI;
    pPI = pProjMan;

    HRESULT hr = pPI->getAllClassTypes(&parrClassTypes);
    if(hr!=S_OK)
    {
        m_edit_testError.SetWindowText(" !!! error !!! ");
        return;
    }   

    AddTestOutString("Get All Class Types:");
    BSTR HUGEP *pbstr;
    BSTR bstr;
    unsigned long i;

    // Get a pointer to the elements of the array.
    hr = ::SafeArrayAccessData(parrClassTypes, (void HUGEP**)&pbstr);
    if (FAILED(hr))
    {
        return;
    }
    for (i = 0; i < ((parrClassTypes)->rgsabound[0]).cElements; i++)
    {
        CString str;
        bstr = pbstr[i];
        str = "    " + (CString)bstr;
        AddTestOutString(str);
    }

    ::SafeArrayUnaccessData(parrClassTypes);
    ::SafeArrayDestroy(parrClassTypes);
}



void CTestProjManDlg1Dlg::OnTestGetAllTargetTypes() 
{
  	m_edit_testError.SetWindowText(" ");

    SAFEARRAY* parrTargetTypes;
    
    CComQIPtr<ICEProjInfo, &IID_ICEProjInfo> pPI;
    pPI = pProjMan;

    HRESULT hr = pPI->getAllTargetTypes(&parrTargetTypes);
    if(hr!=S_OK)
    {
        m_edit_testError.SetWindowText(" !!! error !!! ");
        return;
    }   

    AddTestOutString("Get All Target Types:");
    BSTR HUGEP *pbstr;
    BSTR bstr;
    unsigned long i;

    // Get a pointer to the elements of the array.
    hr = ::SafeArrayAccessData(parrTargetTypes, (void HUGEP**)&pbstr);
    if (FAILED(hr))
    {
        return;
    }
    for (i = 0; i < ((parrTargetTypes)->rgsabound[0]).cElements; i++)
    {
        CString str;
        bstr = pbstr[i];
        str = "    " + (CString)bstr;
        AddTestOutString(str);
    }

    ::SafeArrayUnaccessData(parrTargetTypes);	
    ::SafeArrayDestroy(parrTargetTypes);
}

void CTestProjManDlg1Dlg::OnTestTextForClassType() 
{
  	m_edit_testError.SetWindowText(" ");

    CString strClassType;
    m_edit_test_ClassType.GetWindowText(strClassType);

    CComQIPtr<ICEProjInfo, &IID_ICEProjInfo> pPI;
    pPI = pProjMan;

    CComBSTR sCT = strClassType;
    CComBSTR sText;

    HRESULT hr = pPI->getTextForClassType(sCT, &sText);
    if(hr!=S_OK)
    {
        m_edit_testError.SetWindowText(" !!! error !!! ");
        return;
    }   

    AddTestOutString("Get Text For Class Types:");
    CString strOut = sText;
    strOut = "    " + strClassType + " = \"" +  strOut + "\"";
    AddTestOutString(strOut);


}

void CTestProjManDlg1Dlg::OnTestFileChangeHint() 
{
    m_edit_testError.SetWindowText(" ");

    CString strFileName;
    m_edit_test_ClassType.GetWindowText(strFileName);

    CComQIPtr<ICEProjInfo, &IID_ICEProjInfo> pPI;
    pPI = pProjMan;

    CComBSTR sFN = strFileName;

    HRESULT hr = pPI->fileChangedHint(sFN);
    if(hr!=S_OK)
    {
        m_edit_testError.SetWindowText(" !!! error !!! ");
        return;
    }   
}

void CTestProjManDlg1Dlg::OnTestFileParseNow() 
{
    m_edit_testError.SetWindowText(" ");

    CString strFileName;
    m_edit_test_ClassType.GetWindowText(strFileName);

    CComQIPtr<ICEProjInfo, &IID_ICEProjInfo> pPI;
    pPI = pProjMan;

    CComBSTR sFN = strFileName;

    HRESULT hr = pPI->fileParseNow(sFN);
    if(hr!=S_OK)
    {
        m_edit_testError.SetWindowText(" !!! error !!! ");
        return;
    }   
}

void CTestProjManDlg1Dlg::OnTestGetXMLNodePtr() 
{
    IUnknown* pUnk;
    m_edit_testError.SetWindowText(" ");

    CString strIdPath;
    m_edit_test_ClassType.GetWindowText(strIdPath);

    CComQIPtr<ICEProjInfo, &IID_ICEProjInfo> pPI;
    pPI = pProjMan;

    CComBSTR sIP = strIdPath;

    HRESULT hr = pPI->getXMLNodePtr(sIP, &pUnk);
    if(hr!=S_OK)
    {
        m_edit_testError.SetWindowText(" !!! error !!! ");
        return;
    }   

    CXMLNode node;
    node.SetIXMLNode(pUnk);
    pUnk->Release();

    CString str;
    CString outString = "   ";
    node.GetTagName(str);
    outString += str;
    node.GetAttribute(_T("ID"), str);
    outString += "  ID: " + str;
    node.GetAttribute(_T("TEXT"), str);
    outString += "  TEXT: " + str;

    AddTestOutString("Get XMLNode Ptr:");
    AddTestOutString(outString);
        

}

void CTestProjManDlg1Dlg::OnTestGetNameProp() 
{
	m_edit_testError.SetWindowText(" ");

    CString strNameBase;
    m_edit_test_ClassType.GetWindowText(strNameBase);

    CComQIPtr<ICEProjInfo, &IID_ICEProjInfo> pPI;
    pPI = pProjMan;

    CComBSTR sNB = strNameBase;
    CComBSTR sProp;

    HRESULT hr = pPI->getGlobalNameProposal(sNB, &sProp);
    if(hr!=S_OK)
    {
        m_edit_testError.SetWindowText(" !!! error !!! ");
        return;
    }   

    AddTestOutString("Get Global Name Proposal:");
    CString strOut = sProp;
    strOut = "    " + strNameBase + " = \"" +  strOut + "\"";
    AddTestOutString(strOut);

}

void CTestProjManDlg1Dlg::OnTestGetAllClasses() 
{
	m_edit_testError.SetWindowText(" ");

    SAFEARRAY* parrClasses;
    
    CComQIPtr<ICEProjInfo, &IID_ICEProjInfo> pPI;
    pPI = pProjMan;

    HRESULT hr = pPI->getAllClasses(&parrClasses);
    if(hr!=S_OK)
    {
        m_edit_testError.SetWindowText(" !!! error !!! ");
        return;
    }   

    AddTestOutString("Get All Classes:");
    BSTR HUGEP *pbstr;
    BSTR bstr;
    unsigned long i;

    // Get a pointer to the elements of the array.
    hr = ::SafeArrayAccessData(parrClasses, (void HUGEP**)&pbstr);
    if (FAILED(hr))
    {
        return;
    }
    for (i = 0; i < ((parrClasses)->rgsabound[0]).cElements; i++)
    {
        CString str;
        bstr = pbstr[i];
        str = "    " + (CString)bstr;
        AddTestOutString(str);
    }

    ::SafeArrayUnaccessData(parrClasses);		
    ::SafeArrayDestroy(parrClasses);
}



void CTestProjManDlg1Dlg::OnTestGetAllClassesIdPaths() 
{
	m_edit_testError.SetWindowText(" ");

    SAFEARRAY* parrClasses;
    
    CComQIPtr<ICEProjInfo, &IID_ICEProjInfo> pPI;
    pPI = pProjMan;

    HRESULT hr = pPI->getAllClassesAsIdPaths(&parrClasses);
    if(hr!=S_OK)
    {
        m_edit_testError.SetWindowText(" !!! error !!! ");
        return;
    }   

    AddTestOutString("Get All Classes:");
    BSTR HUGEP *pbstr;
    BSTR bstr;
    unsigned long i;

    // Get a pointer to the elements of the array.
    hr = ::SafeArrayAccessData(parrClasses, (void HUGEP**)&pbstr);
    if (FAILED(hr))
    {
        return;
    }
    for (i = 0; i < ((parrClasses)->rgsabound[0]).cElements; i++)
    {
        CString str;
        bstr = pbstr[i];
        str = "    " + (CString)bstr;
        AddTestOutString(str);
    }

    ::SafeArrayUnaccessData(parrClasses);		
    ::SafeArrayDestroy(parrClasses);
}




void CTestProjManDlg1Dlg::OnTestGetAllClassesOfType() 
{
	m_edit_testError.SetWindowText(" ");

    SAFEARRAY* parrClasses;
    
    CString strClassType;
    m_edit_test_ClassType.GetWindowText(strClassType);


    CComQIPtr<ICEProjInfo, &IID_ICEProjInfo> pPI;
    pPI = pProjMan;

    CComBSTR sClassType(strClassType);

    HRESULT hr = pPI->getClassesOfType(sClassType, &parrClasses);
    if(hr!=S_OK)
    {
        m_edit_testError.SetWindowText(" !!! error !!! ");
        return;
    }   

    AddTestOutString("Get All Classes of Type ("+strClassType+"):");
    BSTR HUGEP *pbstr;
    BSTR bstr;
    unsigned long i;

    // Get a pointer to the elements of the array.
    hr = ::SafeArrayAccessData(parrClasses, (void HUGEP**)&pbstr);
    if (FAILED(hr))
    {
        return;
    }
    for (i = 0; i < ((parrClasses)->rgsabound[0]).cElements; i++)
    {
        CString str;
        bstr = pbstr[i];
        str = "    " + (CString)bstr;
        AddTestOutString(str);
    }

    ::SafeArrayUnaccessData(parrClasses);
    ::SafeArrayDestroy(parrClasses);
}



void CTestProjManDlg1Dlg::OnTestGetAllClassesOfTypeIdPaths() 
{
	m_edit_testError.SetWindowText(" ");

    SAFEARRAY* parrClasses;
    
    CString strClassType;
    m_edit_test_ClassType.GetWindowText(strClassType);


    CComQIPtr<ICEProjInfo, &IID_ICEProjInfo> pPI;
    pPI = pProjMan;

    CComBSTR sClassType(strClassType);

    HRESULT hr = pPI->getClassesOfTypeAsIdPaths(sClassType, &parrClasses);
    if(hr!=S_OK)
    {
        m_edit_testError.SetWindowText(" !!! error !!! ");
        return;
    }   

    AddTestOutString("Get All Classes of Type ("+strClassType+"):");
    BSTR HUGEP *pbstr;
    BSTR bstr;
    unsigned long i;

    // Get a pointer to the elements of the array.
    hr = ::SafeArrayAccessData(parrClasses, (void HUGEP**)&pbstr);
    if (FAILED(hr))
    {
        return;
    }
    for (i = 0; i < ((parrClasses)->rgsabound[0]).cElements; i++)
    {
        CString str;
        bstr = pbstr[i];
        str = "    " + (CString)bstr;
        AddTestOutString(str);
    }

    ::SafeArrayUnaccessData(parrClasses);
    ::SafeArrayDestroy(parrClasses);
}



void CTestProjManDlg1Dlg::OnTestGetAllTargets() 
{
	m_edit_testError.SetWindowText(" ");

    SAFEARRAY* parrTargets;
    
    CComQIPtr<ICEProjInfo, &IID_ICEProjInfo> pPI;
    pPI = pProjMan;

    HRESULT hr = pPI->getAllTargets(&parrTargets);
    if(hr!=S_OK)
    {
        m_edit_testError.SetWindowText(" !!! error !!! ");
        return;
    }   

    AddTestOutString("Get All Targets:");
    BSTR HUGEP *pbstr;
    BSTR bstr;
    unsigned long i;

    // Get a pointer to the elements of the array.
    hr = ::SafeArrayAccessData(parrTargets, (void HUGEP**)&pbstr);
    if (FAILED(hr))
    {
        return;
    }
    for (i = 0; i < ((parrTargets)->rgsabound[0]).cElements; i++)
    {
        CString str;
        bstr = pbstr[i];
        str = "    " + (CString)bstr;
        AddTestOutString(str);
    }

    ::SafeArrayUnaccessData(parrTargets);		
    ::SafeArrayDestroy(parrTargets);
}



void CTestProjManDlg1Dlg::OnTestGetAllTargetsIdPaths() 
{
	m_edit_testError.SetWindowText(" ");

    SAFEARRAY* parrTargets;
    
    CComQIPtr<ICEProjInfo, &IID_ICEProjInfo> pPI;
    pPI = pProjMan;

    HRESULT hr = pPI->getAllTargetsAsIdPaths(&parrTargets);
    if(hr!=S_OK)
    {
        m_edit_testError.SetWindowText(" !!! error !!! ");
        return;
    }   

    AddTestOutString("Get All Targets:");
    BSTR HUGEP *pbstr;
    BSTR bstr;
    unsigned long i;

    // Get a pointer to the elements of the array.
    hr = ::SafeArrayAccessData(parrTargets, (void HUGEP**)&pbstr);
    if (FAILED(hr))
    {
        return;
    }
    for (i = 0; i < ((parrTargets)->rgsabound[0]).cElements; i++)
    {
        CString str;
        bstr = pbstr[i];
        str = "    " + (CString)bstr;
        AddTestOutString(str);
    }

    ::SafeArrayUnaccessData(parrTargets);		
    ::SafeArrayDestroy(parrTargets);
}



void CTestProjManDlg1Dlg::OnTestGetAllFilesOfCat() 
{
	enum E_FV_FileCategory eFC;
    CString strFileType;
    m_edit_test_2.GetWindowText(strFileType);

    if (strFileType.CompareNoCase(_T("source"))==0)
    {
        eFC = FV_FILE_SOURCE;
    }
    else if (strFileType.CompareNoCase(_T("help"))==0)
    {
        eFC = FV_FILE_HELP;
    }
    else
    {
        eFC = FV_FILE_UNKNOWN;
    }
		
    m_edit_testError.SetWindowText(" ");

    SAFEARRAY* parrFiles;
    
    //CComQIPtr<ICEProjectManager, &IID_IICEProjectManager> pPM;
    //pPM = pProjMan;

    HRESULT hr = pProjMan->GetAllFilesOfCategory(eFC ,&parrFiles);
    if(hr!=S_OK)
    {
        m_edit_testError.SetWindowText(" !!! error !!! ");
        return;
    }   

    AddTestOutString("Get Files of Category: " + strFileType);
    BSTR HUGEP *pbstr;
    BSTR bstr;
    unsigned long i;

    // Get a pointer to the elements of the array.
    hr = ::SafeArrayAccessData(parrFiles, (void HUGEP**)&pbstr);
    if (FAILED(hr))
    {
        return;
    }
    for (i = 0; i < ((parrFiles)->rgsabound[0]).cElements; i++)
    {
        CString str;
        bstr = pbstr[i];
        str = "    " + (CString)bstr;
        AddTestOutString(str);
    }

    ::SafeArrayUnaccessData(parrFiles);	
    ::SafeArrayDestroy(parrFiles);
}

void CTestProjManDlg1Dlg::OnTestGetFileCat() 
{
    m_edit_testError.SetWindowText(" ");
    CString strFileName;
    m_edit_test_ClassType.GetWindowText(strFileName);
 
    enum E_FV_FileCategory eFC;

    CComBSTR sFileName(strFileName);
    HRESULT hr = pProjMan->GetFileCategory(sFileName, &eFC);
    if(hr!=S_OK)
    {
        m_edit_testError.SetWindowText(" !!! error !!! ");
        return;
    }   

    AddTestOutString("Get Category of File: " + strFileName);
    if (eFC==FV_FILE_SOURCE)
    {
        AddTestOutString("    Source");
    }
    else if (eFC==FV_FILE_HELP)
    {
        AddTestOutString("    Help");
    }
    else 
    {
        AddTestOutString("    Unknown");
    }
	
}

void CTestProjManDlg1Dlg::OnTestAddFVFile() 
{
    m_edit_testError.SetWindowText(" ");
    
    CString strFileName;
    CString strFileCat;

    m_edit_test_ClassType.GetWindowText(strFileName);
    m_edit_test_2.GetWindowText(strFileCat);

    enum E_FV_FileCategory eFC;

    if (strFileCat.CompareNoCase(_T("source"))==0)
    {
        eFC = FV_FILE_SOURCE;
    }
    else if (strFileCat.CompareNoCase(_T("help"))==0)
    {
        eFC = FV_FILE_HELP;
    }
    else
    {
        eFC = FV_FILE_UNKNOWN;
    }
    
    CComBSTR sFileName(strFileName);
    HRESULT hr = pProjMan->AddFile(sFileName, eFC);
    if(hr!=S_OK)
    {
        m_edit_testError.SetWindowText(" !!! error !!! ");
        return;
    }   
 	
}

void CTestProjManDlg1Dlg::OnTestRemoveFVFile() 
{
    m_edit_testError.SetWindowText(" ");
    
    CString strFileName;
    CString strFileCat;

    m_edit_test_ClassType.GetWindowText(strFileName);
    m_edit_test_2.GetWindowText(strFileCat);

    enum E_FV_FileCategory eFC;

    if (strFileCat.CompareNoCase(_T("source"))==0)
    {
        eFC = FV_FILE_SOURCE;
    }
    else if (strFileCat.CompareNoCase(_T("help"))==0)
    {
        eFC = FV_FILE_HELP;
    }
    else
    {
        eFC = FV_FILE_UNKNOWN;
    }
    
    CComBSTR sFileName(strFileName);
    HRESULT hr = pProjMan->RemoveFile(sFileName, eFC);
    if(hr!=S_OK)
    {
        m_edit_testError.SetWindowText(" !!! error !!! ");
        return;
    }   	
}

void CTestProjManDlg1Dlg::OnTestGetFileFromIdPath() 
{
    m_edit_testError.SetWindowText(" ");
    
    CString strIdPath;

    m_edit_test_ClassType.GetWindowText(strIdPath);
    
    CComBSTR sFileName;
    CComBSTR sIdPath(strIdPath);

    CComQIPtr<ICEProjInfo, &IID_ICEProjInfo> pPI;
    pPI = pProjMan;

    HRESULT hr = pPI->getFileNameFromIdPath(sIdPath, &sFileName);
    if(hr!=S_OK)
    {
        m_edit_testError.SetWindowText(" !!! error !!! ");
        return;
    }   		

    CString strFileName(sFileName);
    AddTestOutString("File Name of Id Path:  " + strIdPath);
    AddTestOutString("      " + strFileName);

}

void CTestProjManDlg1Dlg::OnTestGetIdPathFromFile() 
{
    m_edit_testError.SetWindowText(" ");
    
    CString strFileName;

    m_edit_test_ClassType.GetWindowText(strFileName);
    
    CComBSTR sFileName(strFileName);
    CComBSTR sIdPath;

    CComQIPtr<ICEProjInfo, &IID_ICEProjInfo> pPI;
    pPI = pProjMan;

    HRESULT hr = pPI->getIdPathFromFileName(sFileName, &sIdPath);
    if(hr!=S_OK)
    {
        m_edit_testError.SetWindowText(" !!! error !!! ");
        return;
    }   		

    CString strIdPath(sIdPath);
    AddTestOutString("Id Path of File Name:  " + strFileName);
    AddTestOutString("      " + strIdPath);	
}












void CTestProjManDlg1Dlg::OnTestGetTypeOfTarget() 
{
    m_edit_testError.SetWindowText(" ");
    
    CString strTargetName;

    m_edit_test_ClassType.GetWindowText(strTargetName);
    
    CComBSTR sTargetName(strTargetName);
    CComBSTR sType;

    CComQIPtr<ICEProjInfo, &IID_ICEProjInfo> pPI;
    pPI = pProjMan;

    HRESULT hr = pPI->getTypeOfTarget(sTargetName, &sType);
    if(hr!=S_OK)
    {
        m_edit_testError.SetWindowText(" !!! error !!! ");
        return;
    }   		

    CString strType(sType);
    AddTestOutString("Type opf target:  " + strTargetName);
    AddTestOutString("      " + strType);	
}


void CTestProjManDlg1Dlg::OnGetAddressStringForTarget() 
{
    m_edit_testError.SetWindowText(" ");
    
    CString strTargetName;

    m_edit_test_ClassType.GetWindowText(strTargetName);
    
    CComBSTR sTargetName(strTargetName);
    CComBSTR sAddr;

    CComQIPtr<ICEProjInfo, &IID_ICEProjInfo> pPI;
    pPI = pProjMan;

    HRESULT hr = pPI->getAddressStringForTarget(sTargetName, &sAddr);
    if(hr!=S_OK)
    {
        m_edit_testError.SetWindowText(" !!! error !!! ");
        return;
    }   		

    CString strAddr(sAddr);
    AddTestOutString("Addr of target:  " + strTargetName);
    AddTestOutString("      " + strAddr);		
}

void CTestProjManDlg1Dlg::OnGetUserAddrStringForTarget() 
{
    m_edit_testError.SetWindowText(" ");
    
    CString strTargetName;

    m_edit_test_ClassType.GetWindowText(strTargetName);
    
    CComBSTR sTargetName(strTargetName);
    CComBSTR sAddr;

    CComQIPtr<ICEProjInfo, &IID_ICEProjInfo> pPI;
    pPI = pProjMan;

    HRESULT hr = pPI->getUserAddressStringForTarget(sTargetName, &sAddr);
    if(hr!=S_OK)
    {
        m_edit_testError.SetWindowText(" !!! error !!! ");
        return;
    }   		

    CString strAddr(sAddr);
    AddTestOutString("User Addr of target:  " + strTargetName);
    AddTestOutString("      " + strAddr);
}


void CTestProjManDlg1Dlg::OnGetNameForAddr() 
{
    m_edit_testError.SetWindowText(" ");
    
    CString strAddr;

    m_edit_test_ClassType.GetWindowText(strAddr);
    
    CComBSTR sAddr(strAddr);
    CComBSTR sName;

    CComQIPtr<ICEProjInfo, &IID_ICEProjInfo> pPI;
    pPI = pProjMan;

    HRESULT hr = pPI->getTargetNameForAddr(sAddr, &sName);
    if(hr!=S_OK)
    {
        m_edit_testError.SetWindowText(" !!! error !!! ");
        return;
    }   		

    CString strName(sName);
    AddTestOutString("User target of addr:  " + strAddr);
    AddTestOutString("      " + strName);
}


void CTestProjManDlg1Dlg::OnGetResourceForTarget() 
{
    m_edit_testError.SetWindowText(" ");
    
    CString strTargetName;

    m_edit_test_ClassType.GetWindowText(strTargetName);
    
    CComBSTR sTargetName(strTargetName);
    CComBSTR sResource;

    CComQIPtr<ICEProjInfo, &IID_ICEProjInfo> pPI;
    pPI = pProjMan;

    HRESULT hr = pPI->getResourceNameForTarget(sTargetName, &sResource);
    if(hr!=S_OK)
    {
        m_edit_testError.SetWindowText(" !!! error !!! ");
        return;
    }   		

    CString strResource(sResource);
    AddTestOutString("Resource of target:  " + strTargetName);
    AddTestOutString("      " + strResource);
}


void CTestProjManDlg1Dlg::OnGetFreeChannelNrForPC() 
{
    m_edit_testError.SetWindowText(" ");
    
    CString strTargetName;

    m_edit_test_ClassType.GetWindowText(strTargetName);
    
    CComBSTR sTargetName(strTargetName);
    int iChNr;

    CComQIPtr<ICEProjInfo, &IID_ICEProjInfo> pPI;
    pPI = pProjMan;

    HRESULT hr = pPI->getFreeChannelNrForPC(sTargetName, &iChNr);
    if(hr!=S_OK)
    {
        m_edit_testError.SetWindowText(" !!! error !!! ");
        return;
    }   		

    CString strChNr;
    strChNr.Format("%i",iChNr);
    AddTestOutString("Free Channel Nr of target:  " + strTargetName);
    AddTestOutString("      " + strChNr);
}
