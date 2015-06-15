/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlTest/OnlTestDlg.cpp 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: OnlTestDlg.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlTest/OnlTestDlg.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / Online
 *
 * =COMPONENT			OnlTest
 *
 * =CURRENT 	 $Date: 28.02.07 19:06 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"
#include "OnlTest.h"
#include "OnlTestDlg.h"
#include "CommDLL.h"
#include "CommVisu.h"

#include "vmSharedDef.h"

/* ----  Local Defines:   ----------------------------------------------------- */

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/* ----  Global Variables:   -------------------------------------------------- */

/* ----  Local Functions:   --------------------------------------------------- */

/* ----  Implementations:   --------------------------------------------------- */


/* COnlTestDlg
 * ----------------------------------------------------------------------------
 */
COnlTestDlg::COnlTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COnlTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COnlTestDlg)
	m_sConnectParam	= _T("COM1,RS232(COM1:19200,n,8,1),BLOCK(1000),VERSION(21020)");
	m_uBit = 0;
	m_uLength = 0;
	m_uOffset = 0;
	m_uSegment = 0;
	m_bCheckSwap = FALSE;
	m_strPath = _T("");
	m_sFile = _T("");
	m_sGetSet = _T("");
	m_sDBI = _T("");
	m_sCommDLL		= _T("D:\\4C\\Engineering\\bin\\4CWin\\OnlComm.dll");
	m_bPortScan = TRUE;
	m_sTCPAddr = _T("127.0.0.1");
	m_iConnType = 0;
	m_sState = _T("");
	m_sConnectParam	= _T("localhost,TCP(localhost),SIMPLE(25000),VERSION(21020)");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// ------------------------------------------------------------------------

	Init();
}


/* ~COnlTestDlg
 * ----------------------------------------------------------------------------
 */
COnlTestDlg::~COnlTestDlg()
{
	Free();
}


/* DoDataExchange
 * ----------------------------------------------------------------------------
 */
void COnlTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COnlTestDlg)
	DDX_Control(pDX, IDC_CB_DATATYPE, m_cbType);
	DDX_Control(pDX, IDC_CB_PORT, m_cbPort);
	DDX_Control(pDX, IDC_LIST, m_List);
	DDX_Text(pDX, IDC_CONNEC_PARAM, m_sConnectParam);
	DDX_Text(pDX, IDC_BITOFF, m_uBit);
	DDX_Text(pDX, IDC_LENGTH, m_uLength);
	DDX_Text(pDX, IDC_OFFSET, m_uOffset);
	DDX_Text(pDX, IDC_SEGMENT, m_uSegment);
	DDX_Check(pDX, IDC_CHECK_SWAP, m_bCheckSwap);
	DDX_Text(pDX, IDC_EDIT_GETSET, m_sGetSet);
	DDX_Text(pDX, IDC_EDIT_DBI, m_sDBI);
	DDX_Text(pDX, IDC_COMM_DLL, m_sCommDLL);
	DDX_Check(pDX, IDC_CHECK_PORT, m_bPortScan);
	DDX_Text(pDX, IDC_TCPIP_ADDR, m_sTCPAddr);
	DDX_Radio(pDX, IDC_RADIO_ONLCOMM, m_iConnType);
	DDX_Text(pDX, IDC_STATE, m_sState);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COnlTestDlg, CDialog)
	//{{AFX_MSG_MAP(COnlTestDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_LOAD, OnBtnLoad)
	ON_BN_CLICKED(IDC_BTN_UNLOAD, OnBtnUnload)
	ON_BN_CLICKED(IDC_BTN_LOGIN, OnBtnLogin)
	ON_BN_CLICKED(IDC_BTN_LOGOUT, OnBtnLogout)
	ON_BN_CLICKED(IDC_BTN_CONNECT, OnBtnConnect)
	ON_BN_CLICKED(IDC_BTN_DISCONNECT, OnBtnDisconnect)
	ON_BN_CLICKED(IDC_CHECK_SWAP, OnCheckSwap)
	ON_BN_CLICKED(IDC_BTN_STATE, OnBtnState)
	ON_BN_CLICKED(IDC_BTN_GET, OnBtnGetValue)
	ON_BN_CLICKED(IDC_BTN_SET, OnBtnSetValue)
	ON_BN_CLICKED(IDC_BTN_DBI_GETCHILD, OnBtnDbiGetChild)
	ON_BN_CLICKED(IDC_BTN_DBI_GETADDR, OnBtnDbiGetAddr)
	ON_LBN_DBLCLK(IDC_LIST, OnDblclkList)
	ON_LBN_SELCHANGE(IDC_LIST, OnSelchangeList)
	ON_BN_CLICKED(IDC_BTN_DBI_RES, OnBtnDbiRes)
	ON_BN_CLICKED(IDC_BTN_SELCOMMDLL, OnBtnSelCommDLL)
	ON_BN_CLICKED(IDC_RADIO_ONLCOMM, OnRadioOnlComm)
	ON_BN_CLICKED(IDC_RADIO_VISUCOMM, OnRadioVisuComm)
	ON_BN_CLICKED(IDC_CHECK_PORT, OnCheckPort)
	ON_BN_CLICKED(IDC_BTN_DBI_GETPARENT, OnBtnDbiGetParent)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* OnInitDialog
 * ----------------------------------------------------------------------------
 */
BOOL COnlTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	CreateComm();

	EnableControls();

	
	int iIndex;
	
	/* Fill port number / target list box
	 */
	iIndex = m_cbPort.AddString("4C Win");
	m_cbPort.SetItemData(iIndex, 17264);

// OSAI 09/10/2008 Eliminati target altre ditte
//	iIndex = m_cbPort.AddString("4CFC");
//	m_cbPort.SetItemData(iIndex, 17275);

//	iIndex = m_cbPort.AddString("4CVxW");
//	m_cbPort.SetItemData(iIndex, 17265);

//	iIndex = m_cbPort.AddString("DCU01");
//	m_cbPort.SetItemData(iIndex, 17270);

//	iIndex = m_cbPort.AddString("CMZ");
//	m_cbPort.SetItemData(iIndex, 17271);

//	iIndex = m_cbPort.AddString("4CDemo");
//	m_cbPort.SetItemData(iIndex, 17267);

//	iIndex = m_cbPort.AddString("4CSDK");    SDK diventa 4CP20L_TS
//	m_cbPort.SetItemData(iIndex, 17274);

	iIndex = m_cbPort.AddString("4C Series 10");
	m_cbPort.SetItemData(iIndex, 17400);

	iIndex = m_cbPort.AddString("4C XTend");
	m_cbPort.SetItemData(iIndex, 17400);

	iIndex = m_cbPort.AddString("4C SyncMotion");  // ex 4COsaiCE
	m_cbPort.SetItemData(iIndex, 17273);

	iIndex = m_cbPort.AddString("4C P20L_TS");
	m_cbPort.SetItemData(iIndex, 17274);

	iIndex = m_cbPort.AddString("4C Prima_Logic");
	m_cbPort.SetItemData(iIndex, 17277);


	m_cbPort.SetCurSel(m_cbPort.FindString(0, "4C Win"));


	/* Fill data type list box
	 */
	iIndex = m_cbType.AddString("bool");
	m_cbType.SetItemData(iIndex, DBI_DT_BOOL);

	iIndex = m_cbType.AddString("byte");
	m_cbType.SetItemData(iIndex, DBI_DT_BYTE);

	iIndex = m_cbType.AddString("word");
	m_cbType.SetItemData(iIndex, DBI_DT_WORD);

	iIndex = m_cbType.AddString("dword");
	m_cbType.SetItemData(iIndex, DBI_DT_DWORD);

	iIndex = m_cbType.AddString("lword");
	m_cbType.SetItemData(iIndex, DBI_DT_LWORD);

	iIndex = m_cbType.AddString("usint");
	m_cbType.SetItemData(iIndex, DBI_DT_USINT);

	iIndex = m_cbType.AddString("uint");
	m_cbType.SetItemData(iIndex, DBI_DT_UINT);

	iIndex = m_cbType.AddString("udint");
	m_cbType.SetItemData(iIndex, DBI_DT_UDINT);

	iIndex = m_cbType.AddString("ulint");
	m_cbType.SetItemData(iIndex, DBI_DT_ULINT);

	iIndex = m_cbType.AddString("sint");
	m_cbType.SetItemData(iIndex, DBI_DT_SINT);

	iIndex = m_cbType.AddString("int");
	m_cbType.SetItemData(iIndex, DBI_DT_INT);

	iIndex = m_cbType.AddString("dint");
	m_cbType.SetItemData(iIndex, DBI_DT_DINT);

	iIndex = m_cbType.AddString("lint");
	m_cbType.SetItemData(iIndex, DBI_DT_LINT);

	iIndex = m_cbType.AddString("real");
	m_cbType.SetItemData(iIndex, DBI_DT_REAL);

	iIndex = m_cbType.AddString("lreal");
	m_cbType.SetItemData(iIndex, DBI_DT_LREAL);

	iIndex = m_cbType.AddString("string");
	m_cbType.SetItemData(iIndex, DBI_DT_STRING);

	iIndex = m_cbType.AddString("time");
	m_cbType.SetItemData(iIndex, DBI_DT_TIME);


	return TRUE;  // return TRUE  unless you set the focus to a control
}


/* OnPaint
 * ----------------------------------------------------------------------------
 */
void COnlTestDlg::OnPaint() 
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


/* OnQueryDragIcon
 * ----------------------------------------------------------------------------
 */
HCURSOR COnlTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


/* Init
 * ----------------------------------------------------------------------------
 */
void COnlTestDlg::Init()
{
	m_pComm		= NULL;

	m_pDBIVar	= NULL;
	m_pChildren	= NULL;
	m_pAddress	= NULL;

	m_uDBIVar	= 0;
	m_uChildren	= 0;
	m_uAddress	= 0;
}


/* GetTypeStr
 * ----------------------------------------------------------------------------
 */
LPCTSTR COnlTestDlg::GetTypeStr(UINT uType)
{
	static CString sChild;

	switch(uType)
	{
		case DBI_DT_BOOL	: sChild = "bool";			break;
		case DBI_DT_BYTE	: sChild = "byte";			break;
		case DBI_DT_WORD	: sChild = "word";			break;
		case DBI_DT_DWORD	: sChild = "dword";			break;
		case DBI_DT_LWORD	: sChild = "lword";			break;
		case DBI_DT_USINT	: sChild = "usint";			break;
		case DBI_DT_UINT	: sChild = "uint";			break;
		case DBI_DT_UDINT	: sChild = "udint";			break;
		case DBI_DT_ULINT	: sChild = "ulint";			break;
		case DBI_DT_SINT	: sChild = "sint";			break;
		case DBI_DT_INT		: sChild = "int";			break;
		case DBI_DT_DINT	: sChild = "dint";			break;
		case DBI_DT_LINT	: sChild = "lint";			break;
		case DBI_DT_REAL	: sChild = "real";			break;
		case DBI_DT_LREAL	: sChild = "lreal";			break;
		case DBI_DT_STRING	: sChild = "string";		break;
		case DBI_DT_TIME	: sChild = "time";			break;
		default				: sChild = "## INVALID ##";	break;
	}

	return sChild;
}


/* DestroyWindow
 * ----------------------------------------------------------------------------
 */
BOOL COnlTestDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class

	OnBtnUnload();

	return CDialog::DestroyWindow();
}

/* CreateDBIVar
 * ----------------------------------------------------------------------------
 */
void COnlTestDlg::CreateDBIVar()
{
	if (m_pDBIVar != NULL)
	{
		m_pComm->Free(&m_pDBIVar);
	}

	m_uDBIVar = sizeof(XDBIVar) + 1;
	m_pDBIVar = m_pComm->Alloc(m_uDBIVar);
	
	memset(m_pDBIVar, 0x00, m_uDBIVar);

	XVisuVar *pxVisu		= (XVisuVar *)m_pDBIVar;
	pxVisu->xVar.uInst		= 0xFFFFu;
}


/* AddDBIVar
 * ----------------------------------------------------------------------------
 */
void COnlTestDlg::AddDBIVar(BYTE *pVar)
{
	XVisuVar *pxVar = (XVisuVar *)pVar;

	if (m_uDBIVar == sizeof(XDBIVar) + 1 && m_pDBIVar != NULL)
	{
		/* Skip special root element
		 */
		m_uDBIVar = 0;
		m_pComm->Free(&m_pDBIVar);
	}

	UINT uNewLen = m_uDBIVar + sizeof(XDBIVar) + pxVar->xVar.usNameSize + 1;
	BYTE *pTemp = m_pComm->Alloc(uNewLen);

	if (m_uDBIVar != 0)
	{
		memcpy(pTemp, m_pDBIVar, m_uDBIVar);
	}

	memcpy(pTemp + m_uDBIVar, pVar, sizeof(XDBIVar) + pxVar->xVar.usNameSize + 1);

	if (m_uDBIVar != 0 && m_pDBIVar != NULL)
	{
		m_pComm->Free(&m_pDBIVar);
	}

	m_uDBIVar = uNewLen;
	m_pDBIVar = pTemp;
}


/* DelDBIVar
 * ----------------------------------------------------------------------------
 */
void COnlTestDlg::DelDBIVar()
{
	UINT uCount		= 0;
	UINT uLastPos	= -1;
	
	while (uCount < m_uDBIVar)
	{
		XVisuVar *pxVisu = (XVisuVar *)(m_pDBIVar + uCount);

		if (uCount + sizeof(XDBIVar) + pxVisu->xVar.usNameSize + 1 >= m_uDBIVar)
		{
			uLastPos = uCount;
			break;
		}

		uCount += sizeof(XDBIVar) + pxVisu->xVar.usNameSize + 1;
	}

	if (uLastPos == 0)
	{
		m_pComm->Free(&m_pDBIVar);
		m_uDBIVar = 0;

		CreateDBIVar();
	}
	else if (uLastPos != -1)
	{
		BYTE *pTemp = m_pComm->Alloc(uLastPos);

		memcpy(pTemp, m_pDBIVar, uLastPos);

		m_pComm->Free(&m_pDBIVar);

		m_pDBIVar = pTemp;
		m_uDBIVar = uLastPos;
	}
}


/* Free
 * ----------------------------------------------------------------------------
 */
void COnlTestDlg::Free()
{
	FreeListBox();
	
	if (m_pDBIVar != NULL)
	{
		m_pComm->Free(&m_pDBIVar);
	}
	if (m_pChildren != NULL)
	{
		m_pComm->Free(&m_pChildren);
	}
	if (m_pAddress != NULL)
	{
		m_pComm->Free(&m_pAddress);
	}

	if (m_pComm != NULL)
	{
		m_pComm->Logout(this);
		m_pComm->Disconnect(this);
		m_pComm->Unload(this);
		
		delete m_pComm;
		m_pComm = NULL;
	}
}


/* FreeListBox
 * ----------------------------------------------------------------------------
 */
void COnlTestDlg::FreeListBox()
{
	if (m_List.m_hWnd != NULL)
	{
		m_List.ResetContent();
	}

	m_sDBI.Empty();
	m_sDBICurrent.Empty();
}


/* EnableControls
 * ----------------------------------------------------------------------------
 */
void COnlTestDlg::EnableControls()
{
	if (m_pComm == NULL)
	{
		return;
	}

	#define EnW(ctl,b)	GetDlgItem((ctl))->EnableWindow((b))

	BOOL bOnlComm	= m_pComm->GetType() == OCT_ONLCOMM_DLL;
	BOOL bLoaded	= m_pComm->IsLoaded();
	BOOL bConn		= m_pComm->IsConnected();
	BOOL bLogIn		= m_pComm->IsLogedIn();
	
	EnW(IDC_COMM_DLL,		  bOnlComm	&& ! bLoaded);
	EnW(IDC_BTN_SELCOMMDLL,	  bOnlComm	&& ! bLoaded);
	EnW(IDC_BTN_LOAD,		  bOnlComm	&& ! bLoaded);
	EnW(IDC_BTN_UNLOAD,		  bOnlComm	&&   bLoaded);
	EnW(IDC_CONNEC_PARAM,	  bOnlComm	&& ! bLoaded);
	EnW(IDC_CHECK_SWAP,		  bOnlComm	&& ! bLoaded);

	EnW(IDC_TCPIP_ADDR,		! bOnlComm	&& ! bConn);
	EnW(IDC_CHECK_PORT, 	! bOnlComm	&& ! bConn);
	EnW(IDC_CB_PORT, 		! bOnlComm	&& ! bConn	&&	! m_bPortScan);


    EnW(IDC_BTN_CONNECT,	  bLoaded	&& ! bConn);
    EnW(IDC_BTN_DISCONNECT,	  bLoaded	&&   bConn);

    EnW(IDC_BTN_LOGIN,		  bLoaded	&&   bConn	&&	! bLogIn);
    EnW(IDC_BTN_LOGOUT,		  bLoaded	&&   bConn	&&	  bLogIn);

	EnW(IDC_BTN_STATE,		  bLogIn);

	EnW(IDC_BTN_DBI_GETCHILD, bLogIn);
	EnW(IDC_BTN_DBI_GETPARENT,bLogIn);
	EnW(IDC_BTN_DBI_GETADDR,  bLogIn);
	EnW(IDC_BTN_DBI_RES,	  bLogIn);

	EnW(IDC_BTN_GET,		  bLogIn);
	EnW(IDC_BTN_SET,		  bLogIn);
	EnW(IDC_EDIT_GETSET,	  bLogIn);

	EnW(IDC_SEGMENT,		  bLogIn);
	EnW(IDC_OFFSET,			  bLogIn);
	EnW(IDC_BITOFF,			  bLogIn);
	EnW(IDC_BITOFF,			  bLogIn);
	EnW(IDC_CB_DATATYPE,	  bLogIn);
	EnW(IDC_LENGTH,			  bLogIn);

	EnW(IDC_LIST,			  bLogIn);
}

/* CreateComm
 * ----------------------------------------------------------------------------
 */
void COnlTestDlg::CreateComm()
{
	Free();

	switch (m_iConnType)
	{
		case  OCT_ONLCOMM_DLL:
			m_pComm = new CCommDLL;
			break;
		case OCT_VISUCOMM_LIB:
			m_pComm = new CCommVisu;
			break;
		default:
			m_pComm = NULL;
			break;
	}

	if (m_pComm == NULL)
	{
		return;
	}

	CreateDBIVar();
	m_sDBI.Empty();

	UpdateData(FALSE);
}


/* ShowChildren
 * ----------------------------------------------------------------------------
 */
void COnlTestDlg::ShowChildren()
{
	/* Clear list box
	 */
	m_List.ResetContent();
	
	/* Display result
	 */
	XVisuVar	*pxVisu;
	CString		sChild;

	UINT uCount = 0;

	while (uCount < m_uChildren)
	{
		pxVisu = (XVisuVar *)(m_pChildren + uCount);

		/* Get the variables name
		 */
		sChild  = pxVisu->szName;

		/* Get the data type
		 */
		switch(pxVisu->xVar.usType & 0xE0u)
		{
			case DBI_DTM_SIMPLE:
			{
				sChild += "  [";

				switch(pxVisu->xVar.usType & ~0xE0u)
				{
					case DBI_DT_BOOL	: sChild += "bool";			break;
					case DBI_DT_BYTE	: sChild += "byte";			break;
					case DBI_DT_WORD	: sChild += "word";			break;
					case DBI_DT_DWORD	: sChild += "dword";		break;
					case DBI_DT_LWORD	: sChild += "lword";		break;
					case DBI_DT_USINT	: sChild += "usint";		break;
					case DBI_DT_UINT	: sChild += "uint";			break;
					case DBI_DT_UDINT	: sChild += "udint";		break;
					case DBI_DT_ULINT	: sChild += "ulint";		break;
					case DBI_DT_SINT	: sChild += "sint";			break;
					case DBI_DT_INT		: sChild += "int";			break;
					case DBI_DT_DINT	: sChild += "dint";			break;
					case DBI_DT_LINT	: sChild += "lint";			break;
					case DBI_DT_REAL	: sChild += "real";			break;
					case DBI_DT_LREAL	: sChild += "lreal";		break;
					case DBI_DT_STRING	: 
					{
						CString sFormat;
						sFormat.Format("string(%d)", pxVisu->xVar.uLen - 2);
						sChild += sFormat;		
						break;
					}
					case DBI_DT_TIME	: sChild += "time";			break;
					default				: sChild += "## INVALID ##";break;
				}

				sChild += "]";
			}
			break;

			case DBI_DTM_OBJECT:
				sChild += "  <<object>>";
				break;

			case DBI_DTM_ARRAY:
			{
				CString sFormat;

				sFormat.Format("  <<array [%d]>>", m_pComm->Swap16(pxVisu->xVar.uLen));
				sChild += sFormat;
			}
			break;
			
			default:
				sChild += "## INVALID ##";
				break;
		}

		/* Add to list box
		 */
		int iIndex = m_List.AddString(sChild);

		m_List.SetItemData(iIndex, uCount);

		uCount += sizeof(XDBIVar) + pxVisu->xVar.usNameSize + 1;
	
	} // while (uCount < m_uChildren)

	m_List.SetCurSel(0);
	OnSelchangeList();
}


/* OnRadioOnlComm
 * ----------------------------------------------------------------------------
 */
void COnlTestDlg::OnRadioOnlComm() 
{
	UpdateData(TRUE);

	CreateComm();

	EnableControls();	
}


/* OnRadioVisuComm
 * ----------------------------------------------------------------------------
 */
void COnlTestDlg::OnRadioVisuComm() 
{
	UpdateData(TRUE);

	CreateComm();
	
	EnableControls();
}


/* OnBtnSelCommDLL
 * ----------------------------------------------------------------------------
 */
void COnlTestDlg::OnBtnSelCommDLL() 
{
	UpdateData(TRUE);

	CString sFile = "";

	CFileDialog fd(TRUE, NULL, sFile, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY, "4C Communication DLL (OnlComm.dll)|OnlComm.dll|", this);

	int iRet = fd.DoModal();

	switch(iRet)
	{
	case IDOK:
		m_sCommDLL = fd.GetPathName();
		UpdateData(FALSE);
		break;
		
	default:
		break;
	}	
}


/* OnCheckSwap
 * ----------------------------------------------------------------------------
 */
void COnlTestDlg::OnCheckSwap() 
{
    UpdateData(TRUE);

}


/* OnCheckPort
 * ----------------------------------------------------------------------------
 */
void COnlTestDlg::OnCheckPort() 
{
	UpdateData(TRUE);

	EnableControls();
}


/* OnBtnLoad
 * ----------------------------------------------------------------------------
 */
void COnlTestDlg::OnBtnLoad() 
{
	BeginWaitCursor();
    UpdateData(TRUE);

	m_pComm->Load(this, m_sCommDLL, m_sConnectParam);

	if (m_pComm->GetType() == OCT_ONLCOMM_DLL)
	{
		((CCommDLL *)m_pComm)->SetSwap(m_bCheckSwap);
	}

	m_sState = m_pComm->IsLoaded() ? "Communication module loaded" : "";
	UpdateData(FALSE);

	EnableControls();
	EndWaitCursor();
}


/* OnBtnUnload
 * ----------------------------------------------------------------------------
 */
void COnlTestDlg::OnBtnUnload() 
{
	BeginWaitCursor();
	UpdateData(TRUE);

	m_pComm->Unload(this);

	m_sState = m_pComm->IsLoaded() ? "Communication module loaded" : "";

	FreeListBox();

	UpdateData(FALSE);

	EnableControls();
	EndWaitCursor();
}


/* OnBtnConnect
 * ----------------------------------------------------------------------------
 */
void COnlTestDlg::OnBtnConnect() 
{
	BeginWaitCursor();
	UpdateData(TRUE);
	
	CString sPara;

	switch (m_pComm->GetType())
	{
	case OCT_ONLCOMM_DLL:
		sPara.Empty();
		break;

	case OCT_VISUCOMM_LIB:
		sPara = m_sTCPAddr;

		if (m_bPortScan == FALSE)
		{			
			CString sDummy;
			sDummy.Format("%d", m_cbPort.GetItemData(m_cbPort.GetCurSel()));

			sPara += ';';
			sPara += sDummy;
		}
		break;
	}

	m_pComm->Connect(this, sPara);

	m_sState = m_pComm->IsConnected() ? "Connected to control" : "Connect failed";
	UpdateData(FALSE);

	EnableControls();
	EndWaitCursor();
}


/* OnBtnDisconnect
 * ----------------------------------------------------------------------------
 */
void COnlTestDlg::OnBtnDisconnect() 
{
	BeginWaitCursor();
	UpdateData(TRUE);

	m_pComm->Disconnect(this);

	m_sState = m_pComm->IsLoaded() ? "Communication module loaded" : "";
	FreeListBox();
	
	UpdateData(FALSE);

	EnableControls();
	EndWaitCursor();
}


/* OnBtnLogin
 * ----------------------------------------------------------------------------
 */
void COnlTestDlg::OnBtnLogin() 
{
	BeginWaitCursor();
	UpdateData(TRUE);

	m_pComm->Login(this);

	m_sState = m_pComm->IsLogedIn() ? "Logged into control" : "Login failed";
	UpdateData(FALSE);

	EnableControls();	 
	EndWaitCursor();
}


/* OnBtnLogout
 * ----------------------------------------------------------------------------
 */
void COnlTestDlg::OnBtnLogout() 
{
	BeginWaitCursor();
	UpdateData(TRUE);

	m_pComm->Logout(this);

	m_sState = m_pComm->IsConnected() ? "Connected to control" : "";

	FreeListBox();
	UpdateData(FALSE);

	EnableControls();
	EndWaitCursor();
}


/* OnBtnDbiGetChild
 * ----------------------------------------------------------------------------
 */
void COnlTestDlg::OnBtnDbiGetChild() 
{
	UpdateData(TRUE);

	CString sNew;

	/* Prepare identifier list
	 */
	int iSel = m_List.GetCurSel();
	if (iSel == LB_ERR)
	{
		m_sDBICurrent.Empty();
		CreateDBIVar();
	}
	else
	{
		UINT uOffs = m_List.GetItemData(iSel);
		if (uOffs > m_uChildren || m_pChildren == NULL)
		{
			return;
		}

		AddDBIVar(m_pChildren + uOffs);
		
		XVisuVar *pxVisu = (XVisuVar *)(m_pChildren + uOffs);

		sNew = pxVisu->szName;
	}

	/* Get the children
	 */
	USHORT uRes = m_pComm->GetChildren(this, m_pDBIVar, m_uDBIVar, &m_pChildren, &m_uChildren);
	if (uRes != OK)
	{
		DelDBIVar();

		if (uRes == ERR_LOGIN)
		{
			OnBtnLogout();
		}
		return;
	}

	/* Update internal string
	 */
	if (! m_sDBICurrent.IsEmpty())
	{
		m_sDBICurrent += ".";
	}
	m_sDBICurrent += sNew;

	ShowChildren();
	
	UpdateData(FALSE);
}


/* OnBtnDbiGetParent
 * ----------------------------------------------------------------------------
 */
void COnlTestDlg::OnBtnDbiGetParent() 
{
	UpdateData(TRUE);

	BYTE *pTemp = NULL;
	UINT uTemp	= 0;

	/* Create backup of current DBI identifier
	 */
	if (m_uDBIVar > sizeof(XDBIVar) + 1)
	{
		uTemp = m_uDBIVar;
		pTemp = m_pComm->Alloc(uTemp);

		memcpy(pTemp, m_pDBIVar, uTemp);
	}

	DelDBIVar();

	/* Get the children
	 */
	USHORT uRes = m_pComm->GetChildren(this, m_pDBIVar, m_uDBIVar, &m_pChildren, &m_uChildren);
	if (uRes != OK)
	{
		/* Error, restore DBI identifier
		 */
		if (m_uDBIVar != 0 && m_pDBIVar != NULL)
		{
			m_pComm->Free(&m_pDBIVar);
		}

		m_pDBIVar = pTemp;
		m_uDBIVar = uTemp;

		if (uRes == ERR_LOGIN)
		{
			OnBtnLogout();
		}
		
		return;
	}
	
	if (pTemp != NULL)
	{
		m_pComm->Free(&pTemp);
		uTemp = 0;
	}

	int iPos = m_sDBICurrent.ReverseFind('.');

	CString sCut;

	if (iPos == -1)
	{
		m_sDBICurrent.Empty();
	}
	else
	{
		sCut = m_sDBICurrent.Right(m_sDBICurrent.GetLength() - iPos - 1);
		m_sDBICurrent = m_sDBICurrent.Left(iPos);
	}

	ShowChildren();

	/* Select last children
	 */
	if (! sCut.IsEmpty())
	{
		int iIndex	= 0;
		UINT uCount = 0;

		while (uCount < m_uChildren)
		{
			if (uCount + sizeof(XDBIVar) > m_uChildren)
			{
				break;
			}

			XVisuVar *pxVisu = (XVisuVar *)(m_pChildren + uCount);

			if (sCut == pxVisu->szName)
			{
				m_List.SetCurSel(iIndex);
				break;
			}
			
			iIndex++;
			uCount += sizeof(XDBIVar) + pxVisu->xVar.usNameSize + 1;
		}
	}

	UpdateData(FALSE);
}


/* OnDblclkList
 * ----------------------------------------------------------------------------
 */
void COnlTestDlg::OnDblclkList() 
{

	OnBtnDbiGetChild();
}


/* OnSelchangeList
 * ----------------------------------------------------------------------------
 */
void COnlTestDlg::OnSelchangeList() 
{
	UpdateData(TRUE);

	int iSel = m_List.GetCurSel();
	if (iSel == LB_ERR)
	{
		return;
	}

	UINT uOffs = m_List.GetItemData(iSel);
	if (uOffs > m_uChildren || m_pChildren == NULL)
	{
		return;
	}

	XVisuVar *pxVisu = (XVisuVar *)(m_pChildren + uOffs);

	m_sDBI =  m_sDBICurrent;

	if (! m_sDBI.IsEmpty())
	{
		m_sDBI += ".";
	}

	m_sDBI += pxVisu->szName;

	if ((pxVisu->xVar.usType & DBI_DTM_SIMPLE) != 0)
	{
		m_uBit		= pxVisu->xVar.usBit;
		m_uOffset	= m_pComm->Swap32(pxVisu->xVar.ulOffset);
		m_uLength	= m_pComm->Swap16(pxVisu->xVar.uLen);
		m_uSegment	= m_pComm->Swap16(pxVisu->xVar.uInst);

		m_cbType.SetCurSel(m_cbType.FindString(0, GetTypeStr(pxVisu->xVar.usType & ~0xE0)));
	}
	else
	{
		m_uBit		= 0;
		m_uOffset	= 0;
		m_uLength	= 0;
		m_uSegment	= 0;

		m_cbType.SetCurSel(LB_ERR);
	}

	UpdateData(FALSE);
}


/* OnBtnDbiRes
 * ----------------------------------------------------------------------------
 */
void COnlTestDlg::OnBtnDbiRes() 
{
	UpdateData(TRUE);

	m_sDBI.Empty();
	m_sDBICurrent.Empty();

	m_List.ResetContent();

	CreateDBIVar();

	UpdateData(FALSE);
	
	OnBtnDbiGetChild();
}


/* OnBtnDbiGetAddr
 * ----------------------------------------------------------------------------
 */
void COnlTestDlg::OnBtnDbiGetAddr() 
{
    UpdateData(TRUE);

	CString sNew;

	/* Prepare identifier list
	 */
	int iSel = m_List.GetCurSel();
	if (iSel == LB_ERR)
	{
		MessageBox("GetAddress failed, no variable selected.", "OnlCommTest", MB_OK | MB_ICONSTOP);
	}
	else
	{
		UINT uOffs = m_List.GetItemData(iSel);
		if (uOffs > m_uChildren || m_pChildren == NULL)
		{
			return;
		}

		XVisuVar *pxVisu = (XVisuVar *)(m_pChildren + uOffs);

		if ((pxVisu->xVar.usType & DBI_DTM_SIMPLE) == 0)
		{
			MessageBox("Can not get address of complex object.", "OnlCommTest", MB_OK | MB_ICONSTOP);
			return;
		}

		AddDBIVar(m_pChildren + uOffs);
		
		sNew = pxVisu->szName;
	}

	/* Get the address
	 */
	USHORT uRes = m_pComm->GetAddress(this, m_pDBIVar, m_uDBIVar, &m_pAddress, &m_uAddress);
	if (uRes != OK)
	{
		DelDBIVar();

		if (uRes == ERR_LOGIN)
		{
			OnBtnLogout();
		}
		return;
	}

	DelDBIVar();

	XVisuVar xVisu;

	/* Determine address type
	 */
	switch(*m_pAddress & VMM_XV_TYPEMASK)
	{
		case VMM_XV_SMALL:
		{
			XVariableS *pxVar = (XVariableS *)m_pAddress;
			
			xVisu.xVar.uInst	= pxVar->usSegment;
			xVisu.xVar.ulOffset	= pxVar->usOffset;
			xVisu.xVar.uLen		= pxVar->usLen;
			xVisu.xVar.usBit	= (pxVar->usType & VMM_XV_BITMASK);
			break;
		}

		case VMM_XV_MEDIUM:
		{
			XVariableM *pxVar = (XVariableM *)m_pAddress;

			xVisu.xVar.uInst	= pxVar->usSegment;
			xVisu.xVar.ulOffset	= pxVar->uOffset;
			xVisu.xVar.uLen		= pxVar->uLen;
			xVisu.xVar.usBit	= (pxVar->usType & VMM_XV_BITMASK);
			break;
		}

		case VMM_XV_LARGE:
		{
			XVariableL *pxVar = (XVariableL *)m_pAddress;

			xVisu.xVar.uInst	= pxVar->uSegment;
			xVisu.xVar.ulOffset	= pxVar->ulOffset;
			xVisu.xVar.uLen		= pxVar->uLen;
			xVisu.xVar.usBit	= (pxVar->usType & VMM_XV_BITMASK);
			break;
		}
			
		default:
			return;
	}

	/* Show address 
	 */
	CString sFormat;

	sFormat.Format("Address of variable '%s':\n\nInstance:%d\nOffset:%d\nBitOffset:%d\nLength:%d\n", 
		m_sDBI,
		xVisu.xVar.uInst, xVisu.xVar.ulOffset, xVisu.xVar.usBit, xVisu.xVar.uLen);

	MessageBox(sFormat, "OnlCommTest - GetAddress", MB_OK | MB_ICONINFORMATION);

	m_pComm->Free(&m_pAddress);
	m_uAddress = 0;

	UpdateData(FALSE);	
}


/* OnBtnGetValue
 * ----------------------------------------------------------------------------
 */
void COnlTestDlg::OnBtnGetValue() 
{
	UpdateData(TRUE);

	int iSel = m_cbType.GetCurSel();
	if (iSel == CB_ERR)
	{
		MessageBox("GetValue failed, no data type selected.", "OnlCommTest", MB_OK | MB_ICONSTOP);
		return;
	}

	UINT uType	= m_cbType.GetItemData(iSel);
	UINT uDataLen;

	switch(uType)
	{
		case DBI_DT_STRING:
		default:
			uDataLen = m_uLength;
			break;
	}

	IEC_USINT usVType = VMM_XV_LARGE;
	IEC_UINT  uVSize  = sizeof(XVariableL);

	if (m_uOffset <= 0xffu && uDataLen <= 0xffu && m_uSegment <= 0xffu)
	{
		usVType = VMM_XV_SMALL;
		uVSize  = sizeof(XVariableS);
	}
	else if (m_uOffset <= 0xffffu && m_uSegment <= 0xffu)
	{
		usVType = VMM_XV_MEDIUM;
		uVSize  = sizeof(XVariableM);
	}
	else
	{
		usVType = VMM_XV_LARGE;
		uVSize  = sizeof(XVariableL);
	}

	BYTE *pData		= m_pComm->Alloc(uVSize);
	BYTE byCommand	= CMD_GET_VALUE;

	switch(usVType)
	{	
		case VMM_XV_SMALL:
		{
			XVariableS *pxVar	= (XVariableS *)(pData);

			pxVar->usType		= (IEC_USINT)(VMM_XV_SMALL | (m_uBit & VMM_XV_BITMASK));

			pxVar->usOffset		= (IEC_USINT)m_uOffset;
			pxVar->usLen		= (IEC_USINT)uDataLen;
			pxVar->usSegment	= (IEC_USINT)m_uSegment;
			
			break;
		}

		case VMM_XV_MEDIUM:
		{
			XVariableM *pxVar	= (XVariableM *)(pData);

			pxVar->usType		= (IEC_USINT)(VMM_XV_MEDIUM | (m_uBit & VMM_XV_BITMASK));

			pxVar->uOffset		= m_pComm->Swap16((IEC_UINT) m_uOffset);
			pxVar->uLen			= m_pComm->Swap16((IEC_UINT) uDataLen);
			pxVar->usSegment	=				  (IEC_USINT)m_uSegment;

			break;
		}

		case VMM_XV_LARGE:
		{
			XVariableL *pxVar	= (XVariableL *)(pData);

			pxVar->usType		= (IEC_USINT)(VMM_XV_LARGE | (m_uBit & VMM_XV_BITMASK));

			pxVar->ulOffset		= m_pComm->Swap32(m_uOffset);
			pxVar->uLen			= m_pComm->Swap16(uDataLen);
			pxVar->uSegment		= m_pComm->Swap16((IEC_UINT)m_uSegment);

			break;
		}
	}

	BYTE	*pVal	= NULL;
	USHORT  uVal	= 0;

	USHORT uRes = m_pComm->GetValue(this, pData, uVSize, &pVal, &uVal);
	
	if (uRes == OK)
	{
		switch(uType)
		{
			case DBI_DT_BOOL:
				m_sGetSet = *pVal != 0 ? "true" : "false";
				break;
			case DBI_DT_BYTE:
				m_sGetSet.Format("%x", *(BYTE *)pVal);
				break;
			case DBI_DT_WORD:
				m_sGetSet.Format("%x", *(WORD *)pVal);
				break;
			case DBI_DT_DWORD:
				m_sGetSet.Format("%x", *(DWORD *)pVal);
				break;
			case DBI_DT_SINT:
				m_sGetSet.Format("%d", *(CHAR *)pVal);
				break;
			case DBI_DT_INT:
				m_sGetSet.Format("%d", *(SHORT *)pVal);
				break;
			case DBI_DT_DINT:
				m_sGetSet.Format("%d", *(INT *)pVal);
				break;
			case DBI_DT_REAL:
				m_sGetSet.Format("%f", *(FLOAT *)pVal);
				break;
			case DBI_DT_LREAL:
				m_sGetSet.Format("%f", *(DOUBLE *)pVal);
				break;
			case DBI_DT_STRING:
			{
				char sz[300];
				memcpy(sz, pVal + 2, *pVal);
				sz[*pVal] = 0;

				m_sGetSet = sz;
				break;
			}
			case DBI_DT_TIME:
				m_sGetSet.Format("%u", *(DWORD *)pVal);
				break;
			default:
				m_sGetSet.Empty();
				break;
		}
	}
		
	UpdateData(FALSE);

	m_pComm->Free(&pData);
}


/* OnBtnSetValue
 * ----------------------------------------------------------------------------
 */
void COnlTestDlg::OnBtnSetValue() 
{
	UpdateData(TRUE);
	
	int iSel = m_cbType.GetCurSel();
	if (iSel == CB_ERR)
	{
		MessageBox("SetValue failed, no data type selected.", "OnlCommTest", MB_OK | MB_ICONSTOP);
		return;
	}

	UINT uType	= m_cbType.GetItemData(iSel);
	UINT uDataLen;

	switch(uType)
	{
		case DBI_DT_STRING:
		default:
			uDataLen = m_uLength;
			break;
	}

	USHORT	uData	= sizeof(XVariable) + uDataLen;
	BYTE	*pData	= m_pComm->Alloc(uData);
	BYTE	*pVal	= pData + sizeof(XVariable);

	XValue *pxVal	= (XValue *)pData;
	
	pxVal->VAR.uSegment	= m_pComm->Swap16((USHORT)m_uSegment);
	pxVal->VAR.ulOffset = m_pComm->Swap32(m_uOffset);
	pxVal->VAR.uLen		= m_pComm->Swap16((USHORT)uDataLen);
	pxVal->VAR.byBit	= m_uBit;
	
	switch (uType)
	{
		case DBI_DT_BOOL:
			if (m_sGetSet == "true")
			{
				*pVal = 1;
			}
			else
			{
				*pVal = 0;
			}
			break;
		case DBI_DT_BYTE:
			*pVal = (BYTE)strtoul(m_sGetSet, NULL, 10);
			break;
		case DBI_DT_WORD:
			*(WORD *)pVal = (WORD)strtoul(m_sGetSet, NULL, 10);
			break;
		case DBI_DT_DWORD:
			*(DWORD *)pVal = (DWORD)strtoul(m_sGetSet, NULL, 10);
			break;
		case DBI_DT_SINT:
			*pVal = (CHAR)strtol(m_sGetSet, NULL, 10);
			break;
		case DBI_DT_INT:
			*(SHORT *)pVal = (SHORT)strtol(m_sGetSet, NULL, 10);
			break;
		case DBI_DT_DINT:
			*(LONG *)pVal = (LONG)strtol(m_sGetSet, NULL, 10);
			break;
		case DBI_DT_REAL:
			*(FLOAT *)pVal = (FLOAT)atof(m_sGetSet);
			break;
		case DBI_DT_LREAL:
			*(DOUBLE *)pVal = (DOUBLE)atof(m_sGetSet);
			break;
		case DBI_DT_STRING:
		{
			pVal[0] = m_sGetSet.GetLength();
			pVal[1]	= m_sGetSet.GetLength();
			
			memcpy(pVal + 2, (LPCTSTR)m_sGetSet, m_sGetSet.GetLength());
			break;
		}
		case DBI_DT_TIME:
			*(DWORD *)pVal = (DWORD)strtoul(m_sGetSet, NULL, 10);
			break;
		default:
			return;
	}
	
	USHORT uRes = m_pComm->SetValue(this, pData, uData);
	
	m_pComm->Free(&pData);
}


/* OnBtnState
 * ----------------------------------------------------------------------------
 */
void COnlTestDlg::OnBtnState() 
{
	UINT uState = -1;

	USHORT uRes = m_pComm->GetState(this, &uState);

	m_sGetSet.Format("%x", uState);

	UpdateData(FALSE);
}


/* ---------------------------------------------------------------------------- */


