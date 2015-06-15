// TestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Test.h"
#include "TestDlg.h"

#include <comdef.h>

#include "..\ContainerDummy\ContainerDummy_i.c"
#include "..\ContainerDummy\CEContainer_i.c"

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

    m_pICEContainer = NULL;
}

CTestDlg::~CTestDlg()
{
	if(m_pICEContainer != NULL)
	{
		m_pICEContainer->Release();
		m_pICEContainer = NULL;
	}
	CoUninitialize();
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestDlg, CDialog)
	//{{AFX_MSG_MAP(CTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONNECT, OnConnect)
	ON_BN_CLICKED(IDC_DISCONNECT, OnDisconnect)
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

	m_pICEContainer = NULL;

	hRes = CoInitialize(NULL);
	if(FAILED(hRes))
	{
		DispMessage(hRes);
	}
	
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

	if(m_pICEContainer != NULL)
	{
		AfxMessageBox(_T("Already connected ..."));
		return;
	}

	// create the COM object
	hRes = CoCreateInstance(CLSID_ContainerDummyComObj,
							NULL,
							CLSCTX_INPROC_SERVER,
							IID_ICEContainer,
							(LPVOID *) &m_pICEContainer);
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

    Invoke();
}

void CTestDlg::OnDisconnect() 
{
	if(m_pICEContainer != NULL)
	{
		m_pICEContainer->Release();
		m_pICEContainer = NULL;
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

void CTestDlg::Invoke()
{
    HRESULT hRes = 0;
    _bstr_t * psFileName = new _bstr_t(L"FileName.xyz");
    _bstr_t * psLocation = new _bstr_t(L"Location");
    _bstr_t * psLinkName = new _bstr_t(L"LinkName");
    _bstr_t * psOldLinkName = new _bstr_t(L"OldLinkName");

    hRes = m_pICEContainer->createLink(*psFileName, *psLocation, *psLinkName);
    ASSERT(SUCCEEDED(hRes));

    hRes = m_pICEContainer->deleteLink(*psFileName, *psLocation, *psLinkName);
    ASSERT(SUCCEEDED(hRes));

    hRes = m_pICEContainer->renameLink(*psFileName, *psLocation, *psOldLinkName, *psLinkName);
    ASSERT(SUCCEEDED(hRes));

    delete psFileName;
    delete psLocation;
    delete psLinkName;
    delete psOldLinkName;
}
