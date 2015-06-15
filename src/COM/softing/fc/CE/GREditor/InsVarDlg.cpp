

#include "stdafx.h"
#include "GrEditor.h"
#include "InsVarDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInsVarDlg dialog

CInsVarDlg::CInsVarDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInsVarDlg::IDD, pParent)
{
   int  i;

	//{{AFX_DATA_INIT(CInsVarDlg)
	//}}AFX_DATA_INIT
   for( i=0; i<(sizeof(m_Prev)/sizeof(m_Prev[0])); i++)
   {
      m_Prev[i].LoadBitmap( IDB_GR_VAR_PREV01+i );
   }
   m_pNameList = NULL;
   m_pTypeList = NULL;
}


void CInsVarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInsVarDlg)
	DDX_CBString(pDX, IDC_GR_VAR_NAME, m_strName);
	DDX_CBString(pDX, IDC_GR_VAR_TYPE, m_strType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInsVarDlg, CDialog)
	//{{AFX_MSG_MAP(CInsVarDlg)
	ON_CBN_SELCHANGE(IDC_GR_VAR_TYPE, OnSelchangeVarType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInsVarDlg message handlers

BOOL CInsVarDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
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
	if (m_pTypeList != NULL)
	{
	   for (pos = m_pTypeList->GetHeadPosition(); pos != NULL; )
		{
			TypeBox().AddString(m_pNameList->GetNext(pos));
		}
      iLoc = TypeBox().FindStringExact(0, m_strType);
      if( iLoc == CB_ERR ) iLoc = 0;
		TypeBox().SetCurSel( iLoc );
	}
   // display the bitmap
   OnSelchangeVarType();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInsVarDlg::OnSelchangeVarType() 
{
   int  i;

	i = (TypeBox().GetCurSel() % 3);
   ((CStatic*)GetDlgItem(IDC_GR_VAR_PREV))->SetBitmap( m_Prev[i] );
}

void CInsVarDlg::OnOK() 
{
	// TODO: Add extra validation here

	CDialog::OnOK();
}
