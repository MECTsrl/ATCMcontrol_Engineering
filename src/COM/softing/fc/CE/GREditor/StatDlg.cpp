

#include "stdafx.h"
#include "greditor.h"
#include "StatDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatDlg dialog


CStatDlg::CStatDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStatDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStatDlg)
	//}}AFX_DATA_INIT
}


void CStatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatDlg)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_GR_STAT_ALTDIV,   data.m_iSFC_AltDiv);
	DDX_Text(pDX, IDC_GR_STAT_CMT,      data.m_iFBD_Cmt   );
	DDX_Text(pDX, IDC_GR_STAT_CONN,     data.m_iFBD_Conn  );
	DDX_Text(pDX, IDC_GR_STAT_CONT,     data.m_iFBD_Cont  );
	DDX_Text(pDX, IDC_GR_STAT_FB,       data.m_iFBD_FB    );
	DDX_Text(pDX, IDC_GR_STAT_FCT,      data.m_iFBD_Fct   );
	DDX_Text(pDX, IDC_GR_STAT_FIXLINE,  data.m_iLINE_Fix  );
	DDX_Text(pDX, IDC_GR_STAT_JUMP,     data.m_iFBD_Jump  );
	DDX_Text(pDX, IDC_GR_STAT_LABEL,    data.m_iFBD_Label );
	DDX_Text(pDX, IDC_GR_STAT_PRO,      data.m_iFBD_Pro   );
	DDX_Text(pDX, IDC_GR_STAT_RET,      data.m_iFBD_Ret   );
	DDX_Text(pDX, IDC_GR_STAT_RUBBLINE, data.m_iLINE_Rubb );
	DDX_Text(pDX, IDC_GR_STAT_SIMDIV,   data.m_iSFC_SimDiv);
	DDX_Text(pDX, IDC_GR_STAT_STEP,     data.m_iSFC_Step  );
	DDX_Text(pDX, IDC_GR_STAT_SUMFBD,   data.m_iSUM_FBD   );
	DDX_Text(pDX, IDC_GR_STAT_SUMLINES, data.m_iSUM_LINE  );
	DDX_Text(pDX, IDC_GR_STAT_SUMSFC,   data.m_iSUM_SFC   );
	DDX_Text(pDX, IDC_GR_STAT_TOTAL,    data.m_iSUM_Total );
	DDX_Text(pDX, IDC_GR_STAT_TRA,      data.m_iSFC_Tra   );
	DDX_Text(pDX, IDC_GR_STAT_VARFB,    data.m_iFBD_VarFb );
	DDX_Text(pDX, IDC_GR_STAT_VARRD,    data.m_iFBD_VarRd );
	DDX_Text(pDX, IDC_GR_STAT_VARWR,    data.m_iFBD_VarWr );
	DDX_Text(pDX, IDC_GR_STAT_ACTBLK,   data.m_iSFC_ActBlk);
	DDX_Text(pDX, IDC_GR_STAT_ACTION,   data.m_iSFC_Action);
}


BEGIN_MESSAGE_MAP(CStatDlg, CDialog)
	//{{AFX_MSG_MAP(CStatDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatDlg message handlers

BOOL CStatDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
