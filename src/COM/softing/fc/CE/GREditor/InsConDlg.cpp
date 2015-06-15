

#include "stdafx.h"
#include "GrEditor.h"
#include "InsConDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInsConDlg dialog

CInsConDlg::CInsConDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInsConDlg::IDD, pParent)
{
   int  i;

	//{{AFX_DATA_INIT(CInsConDlg)
	m_strName = _T("");
	m_iConPos = 0;
	//}}AFX_DATA_INIT
   for( i=0; i<(sizeof(m_Prev)/sizeof(m_Prev[0])); i++)
   {
      m_Prev[i].LoadBitmap( IDB_GR_CON_PREV01+i );
   }
   m_pNameList = NULL;
}


void CInsConDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInsConDlg)
	DDX_CBString(pDX, IDC_GR_CON_NAME, m_strName);
	DDX_Radio(pDX, IDC_GR_CON_LEFT, m_iConPos);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInsConDlg, CDialog)
	//{{AFX_MSG_MAP(CInsConDlg)
	ON_BN_CLICKED(IDC_GR_CON_LEFT,  OnConChanged)
	ON_BN_CLICKED(IDC_GR_CON_RIGHT, OnConChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInsConDlg message handlers

BOOL CInsConDlg::OnInitDialog() 
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
   // display the bitmap
   OnConChanged();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInsConDlg::OnConChanged() 
{
   int  i;

   UpdateData(TRUE);
	i = m_iConPos & 1;
   ((CStatic*)GetDlgItem(IDC_GR_CON_PREV))->SetBitmap( m_Prev[i] );
}

void CInsConDlg::OnOK() 
{
	// TODO: Add extra validation here

	CDialog::OnOK();
}
