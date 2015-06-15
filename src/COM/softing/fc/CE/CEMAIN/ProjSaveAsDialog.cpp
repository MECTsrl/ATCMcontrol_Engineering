


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*


// ProjSaveAsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "cemain.h"
#include "ProjSaveAsDialog.h"
#include "fc_tools\fc_fileIO.h"
#include "DirectoryDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CProjectSaveAsDlg dialog


CProjectSaveAsDlg::CProjectSaveAsDlg(const CString& strProjectDir,
                                     const CString& strOldProjectName,
                                     CWnd* pParent /*=NULL*/)
	: CDialog(CProjectSaveAsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProjectSaveAsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    m_strProjectDir = strProjectDir;
    m_strOldProjectName = strOldProjectName;
}


void CProjectSaveAsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProjectSaveAsDlg)
	DDX_Control(pDX, IDC_PROJ_SAVE_AS_NAME, m_editName);
	DDX_Control(pDX, IDC_PROJ_SAVE_AS_LOCATION, m_editLocation);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProjectSaveAsDlg, CDialog)
	//{{AFX_MSG_MAP(CProjectSaveAsDlg)
	ON_BN_CLICKED(IDC_PROJ_SAVE_AS_BROWSE, OnProjSaveAsBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProjectSaveAsDlg message handlers


BOOL CProjectSaveAsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

    m_editLocation.SetWindowText(m_strProjectDir);
    m_strOldProjectName = _T("Copy_of_") + m_strOldProjectName;
    m_editName.SetWindowText(m_strOldProjectName);
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}


void CProjectSaveAsDlg::OnProjSaveAsBrowse() 
{
	CDirectoryDlg dirDlg;

    CString strDir;
    m_editLocation.GetWindowText(strDir);
    dirDlg.SetInitialDir(strDir);

    if (dirDlg.DoModal()==IDOK)
    {
        strDir =  dirDlg.GetDir();
        m_editLocation.SetWindowText(strDir);
    }
}



void CProjectSaveAsDlg::OnOK()
{
    CString strDir;

    m_editLocation.GetWindowText(m_strProjectPath);
    m_editName.GetWindowText(m_strProjectName);

    strDir = m_strProjectPath+_T("\\")+m_strProjectName;

    // test if the new project directory does not exist
    int iDesDirExist = FC_ExistSubDirA(strDir);
    if(iDesDirExist == 1)
    {
        CString strMsg;
        strMsg.FormatMessage(IDS_PROJ_SAVE_AS_FOLDER_EXISTS, strDir);
        AfxMessageBox(strMsg, MB_OK);
        return;
    }

    m_strProjectPath = strDir;

    CDialog::OnOK();
}

