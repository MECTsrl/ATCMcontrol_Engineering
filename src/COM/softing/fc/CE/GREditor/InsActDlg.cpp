

#include "stdafx.h"
#include "GrEditor.h"
#include "InsActDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInsActDlg dialog

CInsActDlg::CInsActDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInsActDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInsActDlg)
	m_strName = _T("");
	m_bSetDefaultSize = FALSE;
	//}}AFX_DATA_INIT
   m_bSTaction     = FALSE;
   m_bLDaction     = FALSE;
   m_bSTactWarn    = FALSE;
   m_pNameList = NULL;
}

void CInsActDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInsActDlg)
	DDX_Control(pDX, IDC_GR_ACT_STACTION, m_CtrlSTaction);
	DDX_Control(pDX, IDC_GR_ACT_LDACTION, m_CtrlLDaction);
	DDX_Control(pDX, IDC_GR_ACT_FBDACTION, m_CtrlFBDaction);
	DDX_CBString(pDX, IDC_GR_ACT_NAME, m_strName);
	DDX_Check(pDX, IDC_GR_DEFAULT_SIZE, m_bSetDefaultSize);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CInsActDlg, CDialog)
	//{{AFX_MSG_MAP(CInsActDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInsActDlg message handlers

BOOL CInsActDlg::OnInitDialog() 
{
   CString  str;
	POSITION pos;
   int      iLoc;

	CDialog::OnInitDialog();
   // fill the comboboxes ..
	if (m_pNameList != NULL)
	{
	   for (pos = m_pNameList->GetHeadPosition(); pos != NULL; )
		{
			NameBox().AddString(m_pNameList->GetNext(pos));
		}
      // and set the selection to the DDX variable value			
      iLoc = NameBox().FindStringExact(0, m_strName);
      if( iLoc == CB_ERR ) iLoc = 0;
		NameBox().SetCurSel( iLoc );
	}
   if (m_bSTactWarn)
   {
      str.LoadString(IDS_GR_STACTION_WARN);

      SendDlgItemMessage(IDC_GR_ACT_WARN, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)str);
   }
   m_CtrlSTaction.SetCheck( m_bSTaction ? 1 : 0);
   m_CtrlLDaction.SetCheck( m_bLDaction ? 1 : 0);
   m_CtrlFBDaction.SetCheck( (m_bSTaction||m_bLDaction) ? 0 : 1);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInsActDlg::OnOK() 
{
	CDialog::OnOK();
   
   m_bSTaction = (m_CtrlSTaction.GetCheck() != 0);
   m_bLDaction = (m_CtrlLDaction.GetCheck() != 0);
}

/////////////////////////////////////////////////////////////////////////////
// CInsActBlkDlg dialog

#ifdef V120_SFC_NOTIMEQUALIFIER
void AFXAPI DDV_Qualifier(CDataExchange* pDX, CString strQual[QT_LAST], CString const& value)
{
	if (pDX->m_bSaveAndValidate)
	{
      bool bOk;

      if     ( value == strQual[QT_none] ) bOk = true;  // QT_none;
      else if( value == strQual[QT_N   ] ) bOk = true;  // QT_N;
      else if( value == strQual[QT_R   ] ) bOk = true;  // QT_R;
      else if( value == strQual[QT_S   ] ) bOk = true;  // QT_S;
      else if( value == strQual[QT_L   ] ) bOk = false; // QT_L;
      else if( value == strQual[QT_D   ] ) bOk = false; // QT_D;
      else if( value == strQual[QT_P   ] ) bOk = true;  // QT_P;
      else if( value == strQual[QT_SD  ] ) bOk = false; // QT_SD;
      else if( value == strQual[QT_DS  ] ) bOk = false; // QT_DS;
      else if( value == strQual[QT_SL  ] ) bOk = false; // QT_SL;
      else if( value == strQual[QT_P0  ] ) bOk = true;  // QT_P0;
      else if( value == strQual[QT_P1  ] ) bOk = true;  // QT_P1;
      else                                 bOk = false;

      if (!bOk)
      {
         AfxMessageBox(IDS_GR_NO_TIME_QUALIFIER, MB_ICONEXCLAMATION);
		   pDX->Fail();
      }
	}
}
#endif

CInsActBlkDlg::CInsActBlkDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInsActBlkDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInsActBlkDlg)
	m_strName = _T("");
	m_strQual = _T("");
#ifndef V120_SFC_NOTIMEQUALIFIER
	m_strTime = _T("");
#endif
	m_bSetDefaultSize = FALSE;
	//}}AFX_DATA_INIT
   m_bSTaction = FALSE;
   m_bLDaction     = FALSE;
   m_bSTactWarn    = FALSE;
   m_pNameList = NULL;
   m_pQualList = NULL;
}

void CInsActBlkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInsActBlkDlg)
	DDX_CBString(pDX, IDC_GR_ACT_NAME, m_strName);
	DDX_CBString(pDX, IDC_GR_ACT_QUAL, m_strQual);
#ifdef V120_SFC_NOTIMEQUALIFIER
   DDV_Qualifier(pDX, strQual, m_strQual);
#else
   DDX_Text(pDX, IDC_GR_ACT_TIME, m_strTime);
#endif
	DDX_Control(pDX, IDC_GR_ACT_STACTION, m_CtrlSTaction);
	DDX_Control(pDX, IDC_GR_ACT_LDACTION, m_CtrlLDaction);
	DDX_Control(pDX, IDC_GR_ACT_FBDACTION, m_CtrlFBDaction);
	DDX_Check  (pDX, IDC_GR_DEFAULT_SIZE, m_bSetDefaultSize);
   //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CInsActBlkDlg, CDialog)
	//{{AFX_MSG_MAP(CInsActBlkDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInsActBlkDlg message handlers

BOOL CInsActBlkDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
   CString  str;
   POSITION pos;
   int      iLoc;

   // fill the comboboxes ..
	if (m_pNameList != NULL)
	{
	   for (pos = m_pNameList->GetHeadPosition(); pos != NULL; )
		{
			NameBox().AddString(m_pNameList->GetNext(pos));
		}
      // and set the selection to the DDX variable value			
      iLoc = NameBox().FindStringExact(0, m_strName);
      if( iLoc == CB_ERR ) iLoc = 0;
		NameBox().SetCurSel( iLoc );
	}
	if (m_pQualList != NULL)
	{
	   for (pos = m_pQualList->GetHeadPosition(); pos != NULL; )
		{
			QualBox().AddString(m_pQualList->GetNext(pos));
		}
      // and set the selection to the DDX variable value			
      iLoc = QualBox().FindStringExact(0, m_strQual);
      if( iLoc == CB_ERR ) iLoc = 0;
		QualBox().SetCurSel( iLoc );
	}
   if (m_bSTactWarn)
   {
      str.LoadString(IDS_GR_STACTION_WARN);

      SendDlgItemMessage(IDC_GR_ACT_WARN, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)str);
   }
   m_CtrlSTaction.SetCheck( m_bSTaction ? 1 : 0);
   m_CtrlLDaction.SetCheck( m_bLDaction ? 1 : 0);
   m_CtrlFBDaction.SetCheck( (m_bSTaction||m_bLDaction) ? 0 : 1);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInsActBlkDlg::OnOK() 
{
	CDialog::OnOK();

   m_bSTaction = (m_CtrlSTaction.GetCheck() != 0);
   m_bLDaction = (m_CtrlLDaction.GetCheck() != 0);
}
