

#include "stdafx.h"
#include "greditor.h"
#include "InsClDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInsClDlg dialog


CInsClDlg::CInsClDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInsClDlg::IDD, pParent)
{
   int  i;

	//{{AFX_DATA_INIT(CInsClDlg)
	m_bSetDefaultSize = FALSE;
	//}}AFX_DATA_INIT
   for( i=0; i<(sizeof(m_Prev)/sizeof(m_Prev[0])); i++)
   {
      m_Prev[i].LoadBitmap( IDB_GR_CL_PREV01+i );
   }
   m_pNameList = NULL;
   m_pTypeList = NULL;
}


void CInsClDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInsClDlg)
	DDX_CBString(pDX, IDC_GR_CL_NAME, m_strName);
	DDX_CBString(pDX, IDC_GR_CL_TYPE, m_strType);
	DDX_Check(pDX, IDC_GR_DEFAULT_SIZE, m_bSetDefaultSize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInsClDlg, CDialog)
	//{{AFX_MSG_MAP(CInsClDlg)
	ON_CBN_SELCHANGE(IDC_GR_CL_TYPE, OnSelchangeClType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInsClDlg message handlers

BOOL CInsClDlg::OnInitDialog() 
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
   OnSelchangeClType();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInsClDlg::OnSelchangeClType() 
{
   int  i;

	i = (TypeBox().GetCurSel() % 6);
   ((CStatic*)GetDlgItem(IDC_GR_CL_PREV))->SetBitmap( m_Prev[i] );
}

void CInsClDlg::OnOK() 
{
	// TODO: Add extra validation here

	CDialog::OnOK();
}
