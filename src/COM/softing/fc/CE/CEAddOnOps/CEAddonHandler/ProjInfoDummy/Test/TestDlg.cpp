// TestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Test.h"
#include "TestDlg.h"

#include "..\ProjInfoDummy\ProjInfoDummy_i.c"
#include "..\ProjInfoDummy\CEProjInfo_i.c"
#include "..\ProjInfoDummy\CEContainer_i.c"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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

/////////////////////////////////////////////////////////////////////////////
// CTestDlg dialog

CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_pICEProjInfo = NULL;
    m_pIUnkCEContainer = NULL;
}

CTestDlg::~CTestDlg()
{
	if(m_pICEProjInfo != NULL)
	{
		m_pICEProjInfo->Release();
		m_pICEProjInfo = NULL;
	}
	CoUninitialize();
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestDlg)
	DDX_Control(pDX, IDC_METHOD, m_CtrlMethod);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestDlg, CDialog)
	//{{AFX_MSG_MAP(CTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONNECT, OnConnect)
	ON_BN_CLICKED(IDC_DISCONNECT, OnDisconnect)
	ON_BN_CLICKED(IDC_INVOKE, OnInvoke)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestDlg message handlers

BOOL CTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	HRESULT hRes = 0;

	m_pICEProjInfo = NULL;

	hRes = CoInitialize(NULL);
	if(FAILED(hRes))
	{
		DispMessage(hRes);
	}
	
    m_CtrlMethod.SetCurSel(0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestDlg::OnPaint() 
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
HCURSOR CTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTestDlg::OnConnect() 
{
	HRESULT hRes = 0;

	if(m_pICEProjInfo != NULL)
	{
		AfxMessageBox(_T("Already connected ..."));
		return;
	}

	// create the COM object
	hRes = CoCreateInstance(CLSID_ProjInfoDummyComObj,
							NULL,
							CLSCTX_INPROC_SERVER,
							IID_ICEProjInfo,
							(LPVOID *) &m_pICEProjInfo);
	if(FAILED(hRes))
	{
		DispMessage(hRes);
		return;
	}
	else
	{
		// Disable connect-button
		this->GetDlgItem(IDC_CONNECT)->EnableWindow(FALSE);
		// Enable disconnect-button
		this->GetDlgItem(IDC_DISCONNECT)->EnableWindow(TRUE);
	}
}

void CTestDlg::OnDisconnect() 
{
	if(m_pICEProjInfo != NULL)
	{
		m_pICEProjInfo->Release();
		m_pICEProjInfo = NULL;
		// Enable connect-button
		this->GetDlgItem(IDC_CONNECT)->EnableWindow(TRUE);
		// Disable disconnect-button
		this->GetDlgItem(IDC_DISCONNECT)->EnableWindow(FALSE);
	}
}

void CTestDlg::DispMessage(DWORD dwErrCode)
{
	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dwErrCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
	);
	CString strMessage;
	strMessage.Format("Fehler:\n%s\nHRESULT: %d", lpMsgBuf, dwErrCode);
	AfxMessageBox((LPCTSTR)strMessage);
}

void CTestDlg::OnInvoke() 
{
	HRESULT hRes = 0;

	if(m_pICEProjInfo == NULL)
	{
		AfxMessageBox(_T("Not connected ..."));
		return;
	}

    int iSel = m_CtrlMethod.GetCurSel();
	
    switch(iSel)
    {
    case 0:
        InVoke_fileChangedHint();
        break;
    case 1:
        InVoke_getContainerIF();
        break;
    case 2:
        InVoke_getContainerIFforFile();
        break;
    default:
        ;
    }
}

void CTestDlg::InVoke_fileChangedHint()
{
	bstr_t * psSourceFile = new bstr_t(L"SourceFile");

	HRESULT hRes = m_pICEProjInfo->fileChangedHint(*psSourceFile);
	if(FAILED(hRes))
	{
		DispMessage(hRes);
		return;
	}

    delete psSourceFile;
}

void CTestDlg::InVoke_getContainerIF()
{
	bstr_t * psSourceFile = new bstr_t(L"SourceFile");
	bstr_t * psId = new bstr_t(L"Id");
	BSTR psParentSource = L"ParentSource";
	BSTR psParentId = L"ParentId";

	HRESULT hRes = m_pICEProjInfo->getContainerIF(*psSourceFile,
                                               *psId,
                                               &m_pIUnkCEContainer,
                                               &psParentSource,
                                               &psParentId);

    CString strMsg = _T("call: ICEProjInfo::getContainerIF\n");
    strMsg += _T("\npsSourceFile:\t");
    strMsg += *psSourceFile;
    strMsg += _T("\npsId:\t\t");
    strMsg += *psId;
    strMsg += _T("\npsParentSource:\t");
    strMsg += psParentSource;
    strMsg += _T("\npsParentId:\t");
    strMsg += psParentId;
    AfxMessageBox(strMsg);

	if(FAILED(hRes))
	{
		DispMessage(hRes);
		return;
	}

    InvokeContainerIF();
    m_pIUnkCEContainer->Release();
    m_pIUnkCEContainer = NULL;

    delete psSourceFile;
    delete psId;
}

void CTestDlg::InVoke_getContainerIFforFile()
{
	bstr_t * psSourceFile = new bstr_t(L"SourceFile");

	HRESULT hRes = m_pICEProjInfo->getContainerIFforFile(*psSourceFile, &m_pIUnkCEContainer);
	if(FAILED(hRes))
	{
		DispMessage(hRes);
		return;
	}

    InvokeContainerIF();
    m_pIUnkCEContainer->Release();
    m_pIUnkCEContainer = NULL;

    delete psSourceFile;
}

void CTestDlg::InvokeContainerIF()
{
    if(m_pIUnkCEContainer)
    {
        HRESULT hRes = 0;
        ICEContainer * pICEContainer = NULL;

        hRes= m_pIUnkCEContainer->QueryInterface(IID_ICEContainer, (LPVOID *) &pICEContainer);
        ASSERT(SUCCEEDED(hRes));

        _bstr_t * psFileName = new _bstr_t(L"FileName.xyz");
        _bstr_t * psLocation = new _bstr_t(L"Location");
        _bstr_t * psLinkName = new _bstr_t(L"LinkName");
        _bstr_t * psOldLinkName = new _bstr_t(L"OldLinkName");

        hRes = pICEContainer->createLink(*psFileName, *psLocation, *psLinkName);
        ASSERT(SUCCEEDED(hRes));

        hRes = pICEContainer->deleteLink(*psFileName, *psLocation, *psLinkName);
        ASSERT(SUCCEEDED(hRes));

        hRes = pICEContainer->renameLink(*psFileName, *psLocation, *psOldLinkName, *psLinkName);
        ASSERT(SUCCEEDED(hRes));

        if(pICEContainer != NULL)
        {
            pICEContainer->Release();
            pICEContainer = NULL;
        }

        delete psFileName;
        delete psLocation;
        delete psLinkName;
        delete psOldLinkName;
    }
}
