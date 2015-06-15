// FileStatusDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FileStatusDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileStatusDlg dialog


CFileStatusDlg::CFileStatusDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileStatusDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileStatusDlg)
	m_strCreated = _T("");
	m_strLastModified = _T("");
	m_strName = _T("");
	m_strPath = _T("");
	m_bReadOnly = FALSE;
	m_strSize = _T("");
	//}}AFX_DATA_INIT
}


void CFileStatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileStatusDlg)
	DDX_Text(pDX, IDC_FILE_PROP_CREATED, m_strCreated);
	DDX_Text(pDX, IDC_FILE_PROP_LAST_MOD, m_strLastModified);
	DDX_Text(pDX, IDC_FILE_PROP_NAME, m_strName);
	DDX_Text(pDX, IDC_FILE_PROP_PATH, m_strPath);
	DDX_Check(pDX, IDC_FILE_PROP_READ_ONLY, m_bReadOnly);
	DDX_Text(pDX, IDC_FILE_PROP_SIZE, m_strSize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileStatusDlg, CDialog)
	//{{AFX_MSG_MAP(CFileStatusDlg)
	ON_BN_CLICKED(IDC_FILE_PROP_READ_ONLY, OnFilePropReadOnly)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileStatusDlg message handlers


//------------------------------------------------------------------*
/**
 * on init dialog
 *
 *  get file informations
 *
*/
BOOL CFileStatusDlg::OnInitDialog() 
{
    CFileStatus tStatus;
    CString strFile = m_strPath + "\\" + m_strName;

    if(!CFile::GetStatus(strFile, tStatus))
    {
        return FALSE;
    }

    m_strCreated = tStatus.m_ctime.Format(IDS_DATE_TIME_FORMAT);
    m_strLastModified = tStatus.m_mtime.Format(IDS_DATE_TIME_FORMAT);

    if(tStatus.m_size < 1024)
    {
        m_strSize.Format("%d Bytes", tStatus.m_size);
    }
    else
    {
        double  dfSize;

        dfSize = (double)tStatus.m_size / 1024;
        if(dfSize < 1024.0)
        {
            m_strSize.Format("%4.1f kB", dfSize);
        }
        else
        {
            dfSize /= 1024.0;
            m_strSize.Format("%4.1f MB", dfSize);
        }
    }

    m_bReadOnly = (tStatus.m_attribute & CFile::readOnly) > 0 ? TRUE : FALSE;
    if(m_strPath == "")
    {
        CString strPath = tStatus.m_szFullName;
        int     iLeft = strPath.GetLength() - m_strName.GetLength() - 1;
        m_strPath = strPath.Left(iLeft);
    }
	
	CDialog::OnInitDialog();

#ifndef _DEBUG
    CWnd*   pWnd = GetDlgItem(IDC_FILE_PROP_READ_ONLY);
    if(pWnd)
    {
        pWnd->EnableWindow(FALSE);
    }
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


//------------------------------------------------------------------*
/**
 * show properties dialog
 *
 * @param           [in] strFileName: file name
 * @param           [in] strPath: path to file
*/
void CFileStatusDlg::ShowProperties(const CString& strFileName, const CString& strPath /*=""*/)
{
    m_strName = strFileName;
    m_strPath = strPath;
    DoModal();
}

//------------------------------------------------------------------*
/**
 * change read only flag
 *
*/
void CFileStatusDlg::OnFilePropReadOnly() 
{
    TRACE("Read Only: %d\n", m_bReadOnly);
    CFileStatus tStatus;
    CString     strFile = m_strPath + "\\" + m_strName;

    if(CFile::GetStatus(strFile, tStatus))
    {
        tStatus.m_attribute ^= CFile::readOnly;
        CFile::SetStatus(strFile, tStatus);
    }
}
