

#include "stdafx.h"
#include "GrEditor.h"
#include "ZoomDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CZoomDlg dialog


CZoomDlg::CZoomDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CZoomDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CZoomDlg)
   m_iZoom = 0;
	m_fValue = 0.0f;
	//}}AFX_DATA_INIT
}


void CZoomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CZoomDlg)
	DDX_Radio(pDX, IDC_GR_ZOOM_400, m_iZoom);
	DDX_Text(pDX, IDC_GR_ZOOM_VALUE, m_fValue);
	DDV_MinMaxFloat(pDX, m_fValue, 1.e-002f, 10.f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CZoomDlg, CDialog)
	//{{AFX_MSG_MAP(CZoomDlg)
	ON_EN_SETFOCUS(IDC_GR_ZOOM_VALUE, OnSetfocusGrZoomValue)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CZoomDlg::OnSetfocusGrZoomValue() 
{
   ((CButton*)GetDlgItem(IDC_GR_ZOOM_400))->SetCheck(0);
   ((CButton*)GetDlgItem(IDC_GR_ZOOM_200))->SetCheck(0);
   ((CButton*)GetDlgItem(IDC_GR_ZOOM_100))->SetCheck(0);
   ((CButton*)GetDlgItem(IDC_GR_ZOOM_050))->SetCheck(0);
   ((CButton*)GetDlgItem(IDC_GR_ZOOM_CUSTOM))->SetCheck(1);
}