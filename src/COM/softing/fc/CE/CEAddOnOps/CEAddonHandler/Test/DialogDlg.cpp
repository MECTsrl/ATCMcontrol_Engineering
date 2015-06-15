// DialogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Dialog.h"
#include "DialogDlg.h"

#include "..\CEAddonHandler\Methods.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogDlg dialog

CDialogDlg::CDialogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogDlg)
	m_strSourceFile = _T("");
	m_strId = _T("");
	m_strAdditionalInfo = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDialogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogDlg)
	DDX_Control(pDX, IDC_DISPID, m_CtrlDispId);
	DDX_Control(pDX, IDC_DISCONNECT, m_Disconnect);
	DDX_Control(pDX, IDC_CONNECT, m_Connect);
	DDX_Text(pDX, IDC_ADDITIONALINFO, m_strAdditionalInfo);
	DDX_Text(pDX, IDC_ID, m_strId);
	DDX_Text(pDX, IDC_SOURCEFILE, m_strSourceFile);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDialogDlg, CDialog)
	//{{AFX_MSG_MAP(CDialogDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONNECT, OnConnect)
	ON_BN_CLICKED(IDC_DISCONNECT, OnDisconnect)
	ON_BN_CLICKED(IDC_INVOKE, OnInvoke)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogDlg message handlers

BOOL CDialogDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

    // Fill combobox
    int i = 0;
    while(DISP_IDS[i] != _T(""))
    {
        m_CtrlDispId.AddString(DISP_IDS[i++]);
    }
    m_CtrlDispId.SetCurSel(0);
	
    m_strSourceFile = _T("SourceFile");
    m_strId = _T("Id");
    m_strAdditionalInfo = _T("AdditionalInfo");

    UpdateData(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDialogDlg::OnPaint() 
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
HCURSOR CDialogDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CDialogDlg::OnConnect() 
{
	if(test.connect())
	{
		// Disable connect-button
		this->GetDlgItem(IDC_CONNECT)->EnableWindow(FALSE);
		// Enable disconnect-button
		this->GetDlgItem(IDC_DISCONNECT)->EnableWindow(TRUE);
	}
}

void CDialogDlg::OnDisconnect() 
{
	if(test.disconnect())
	{
		// Enable connect-button
		this->GetDlgItem(IDC_CONNECT)->EnableWindow(TRUE);
		// Disable disconnect-button
		this->GetDlgItem(IDC_DISCONNECT)->EnableWindow(FALSE);
	}
}

void CDialogDlg::OnInvoke() 
{
    UpdateData(TRUE);

	test.invoke(DISP_IDS[m_CtrlDispId.GetCurSel()],
        m_strSourceFile,
        m_strId,
        m_strAdditionalInfo);
}
