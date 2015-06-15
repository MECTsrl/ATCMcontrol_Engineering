// PropPageRestore.cpp : implementation file
//

#include "stdafx.h"
#include "PMTool.h"
#include "PropPageRestore.h"
#include "MainPropertySheet.h"
#include "FolderDialog.h"
#include "defs.h"

#include "utilif.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropPageRestore property page

IMPLEMENT_DYNCREATE(CPropPageRestore, CPMPropPage)

CPropPageRestore::CPropPageRestore(CMainPropertySheet* pMainSheet /*=NULL*/, const CString& crstrProjectPath /*=_T("")*/)
 : CPMPropPage(CPropPageRestore::IDD, pMainSheet)
{
	//{{AFX_DATA_INIT(CPropPageRestore)
	m_strBackupPath = _T("");
	m_bOPCServerConfig = TRUE;
	m_strProjectPath = crstrProjectPath;
	m_iSource = FILE_MODE;
	m_strAddress = _T("");
	m_strPassword = _T("");
	//}}AFX_DATA_INIT

    InitializeFromRegistry();
}

CPropPageRestore::~CPropPageRestore()
{
    SaveToRegistry();
}

void CPropPageRestore::InitializeFromRegistry(BOOL bGeneral /*=FALSE*/)
{
    m_strBackupPath     = AfxGetApp()->GetProfileString("Settings", "Restore_BackupPath");
    if(m_strProjectPath.IsEmpty())
    {
        m_strProjectPath    = AfxGetApp()->GetProfileString("Settings", "Restore_ProjectPath");
    }
    m_strAddress        = AfxGetApp()->GetProfileString("Settings", "Restore_Address");
    m_bOPCServerConfig  = AfxGetApp()->GetProfileInt("Settings", "Restore_OPCServerConfig", 1);
    m_iSource           = AfxGetApp()->GetProfileInt("Settings", "Restore_Source", 0);

    if(bGeneral || m_strBackupPath.IsEmpty())
    {
        m_strBackupPath = AfxGetApp()->GetProfileString("Settings", "BackupPath");
    }
    if(bGeneral || m_strProjectPath.IsEmpty())
    {
        m_strProjectPath = AfxGetApp()->GetProfileString("Settings", "ProjectPath");
    }
    if(bGeneral || m_strAddress.IsEmpty())
    {
        m_strAddress = AfxGetApp()->GetProfileString("Settings", "Address");
    }
}

void CPropPageRestore::SaveToRegistry(BOOL bGeneral /*=FALSE*/)
{
    AfxGetApp()->WriteProfileString("Settings", "Restore_BackupPath", m_strBackupPath);
    AfxGetApp()->WriteProfileString("Settings", "Restore_ProjectPath", m_strProjectPath);
    AfxGetApp()->WriteProfileString("Settings", "Restore_Address", m_strAddress);
    AfxGetApp()->WriteProfileInt("Settings", "Restore_OPCServerConfig", m_bOPCServerConfig);
    AfxGetApp()->WriteProfileInt("Settings", "Restore_Source", m_iSource);

    if(bGeneral)
    {
        if(!m_strBackupPath.IsEmpty())
        {
            AfxGetApp()->WriteProfileString("Settings", "BackupPath", m_strBackupPath);
        }
        if(!m_strProjectPath.IsEmpty())
        {
            AfxGetApp()->WriteProfileString("Settings", "ProjectPath", m_strProjectPath);
        }
        if(!m_strAddress.IsEmpty())
        {
            AfxGetApp()->WriteProfileString("Settings", "Address", m_strAddress);
        }
    }
}

void CPropPageRestore::DoDataExchange(CDataExchange* pDX)
{
	CPMPropPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageRestore)
	DDX_Text(pDX, IDC_BACKUP_PATH_EDIT, m_strBackupPath);
	DDX_Check(pDX, IDC_OPCSERVER_CONFIG_CHECK, m_bOPCServerConfig);
	DDX_Text(pDX, IDC_PROJECT_PATH_EDIT, m_strProjectPath);
	DDX_Radio(pDX, IDC_FILE_SYSTEM_RADIO, m_iSource);
	DDX_Text(pDX, IDC_ADDRESS_EDIT, m_strAddress);
	DDX_Text(pDX, IDC_PASSWORD_EDIT, m_strPassword);
	DDV_MaxChars(pDX, m_strPassword, 8);
	//}}AFX_DATA_MAP
}

CString CPropPageRestore::GetProjectPath()
{
    return m_strProjectPath;
}

BEGIN_MESSAGE_MAP(CPropPageRestore, CPMPropPage)
	//{{AFX_MSG_MAP(CPropPageRestore)
	ON_BN_CLICKED(IDC_BACKUP_PATH_BTN, OnBackupPathBtn)
	ON_BN_CLICKED(IDC_CONTROL_RADIO, OnControlRadio)
	ON_BN_CLICKED(IDC_FILE_SYSTEM_RADIO, OnFileSystemRadio)
	ON_BN_CLICKED(IDC_PROJECT_PATH_BTN, OnProjectPathBtn)
	ON_BN_CLICKED(IDC_RESTORE_BTN, OnRestoreBtn)
	ON_EN_CHANGE(IDC_ADDRESS_EDIT, OnChangeAddressEdit)
	ON_EN_CHANGE(IDC_BACKUP_PATH_EDIT, OnChangeBackupPathEdit)
	ON_EN_CHANGE(IDC_PROJECT_PATH_EDIT, OnChangeProjectPathEdit)
	ON_BN_CLICKED(IDC_OPCSERVER_CONFIG_CHECK, OnOpcserverConfigCheck)
	ON_EN_MAXTEXT(IDC_PASSWORD_EDIT, OnMaxtextPasswordEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropPageRestore message handlers

void CPropPageRestore::OnBackupPathBtn() 
{
    // not needed here because OnChangeBackupPathEdit() does the job:
    //UpdateData(); 
    
    // file dialog	
    CString strFilter;
    CString strFile;
    
    strFilter.LoadString(IDS_BARFILEFILTER);
    CFileDialog tFileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter, this);

    if(!m_strBackupPath.IsEmpty())
    {
        int iFound = m_strBackupPath.ReverseFind(_T('\\'));
        if(iFound > 0 && iFound < m_strBackupPath.GetLength()-1)
        {
            strFile = m_strBackupPath.Mid(iFound + 1);
        }
        tFileDlg.m_ofn.lpstrInitialDir = m_strBackupPath;
        _tcscpy(tFileDlg.m_ofn.lpstrFile, strFile);
    }

    if(tFileDlg.DoModal() == IDOK)
    {
        m_strBackupPath = tFileDlg.GetPathName();
        // add file extension if missing
        if(m_strBackupPath.Right(4).CompareNoCase(_T(".4cb")) != 0)
        {
            m_strBackupPath += _T(".4cb");
        }
        UpdateData(FALSE);
    }
}

void CPropPageRestore::OnControlRadio() 
{
    UpdateData();

    SetSourceMode();
}

void CPropPageRestore::OnFileSystemRadio() 
{
    UpdateData();

    SetSourceMode();
}

//------------------------------------------------------------------*
/**
 * SetSourceMode
 *
 *  enables / disables project path edit / button and address edit
 *  according to source mode (m_iSource)
 *  When called from within radio button handlers, UpdateData must
 *  be called before to ensure that m_iSource has been set correctly.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CPropPageRestore::SetSourceMode()
{
    BOOL    bFileSystem = (m_iSource == FILE_MODE);
    
    // enable/disable project path edit, button, address edit according
    // to source
    CWnd*   pWnd = GetDlgItem(IDC_BACKUP_PATH_EDIT);
    if(pWnd)
    {
        pWnd->EnableWindow(bFileSystem);
    }
    pWnd = GetDlgItem(IDC_BACKUP_PATH_BTN);
    if(pWnd)
    {
        pWnd->EnableWindow(bFileSystem);
    }
    pWnd = GetDlgItem(IDC_ADDRESS_EDIT);
    if(pWnd)
    {
        pWnd->EnableWindow(!bFileSystem);
    }

    int iShowCmd = bFileSystem ? SW_HIDE : SW_SHOW;
    pWnd = GetDlgItem(IDC_CONNECT_INFO_EDIT);
    if(pWnd)
    {
        pWnd->ShowWindow(iShowCmd);
    }
    pWnd = GetDlgItem(IDC_CONNECT_STATUS_EDIT);
    if(pWnd)
    {
        pWnd->ShowWindow(iShowCmd);
    }
}

void CPropPageRestore::OnProjectPathBtn() 
{
    // not needed here because OnChangeProjectPathEdit() does the job:
    //UpdateData();

    CString strProjectPath(m_strProjectPath);
    if(strProjectPath.IsEmpty())
    {
        strProjectPath = GetStandardProjectPath();
    }

	CFolderDialog fd(strProjectPath, "Browse for Project folder");

	int iRet = fd.DoModal();

	switch(iRet)
	{
	case IDOK:
		m_strProjectPath = fd.m_sSelPath;
		UpdateData(FALSE);
        // folder dialog creates new directory
        // restore tool does not work on existing
        // so remove it here
        // only empty directories will be removed
        ::RemoveDirectory(m_strProjectPath);
		break;
		
	default:
		break;
	}

    TryLoadTargetInfo(m_strProjectPath);
}

void CPropPageRestore::OnRestoreBtn() 
{
    m_pMainSheet->SetStatusText(_T("Restore Project ..."));

    UpdateData();

    if(!CheckData())
    {
        return;
    }

    if(RestoreProject())
    {
        m_pMainSheet->SetStatusText(_T("Restore Done"));
        m_strPassword.Empty();
        CWnd*   pWnd = GetDlgItem(IDC_PASSWORD_EDIT);
        if(pWnd && pWnd->GetSafeHwnd())
        {
            pWnd->SetWindowText(_T(""));
        }
    }
}


BOOL CPropPageRestore::CheckData()
{
    switch(m_iSource)
    {
    case FILE_MODE:
        if(m_strBackupPath.IsEmpty())
        {
            ::AfxMessageBox(IDS_ERR_NO_BACKUP_PATH);
            CWnd*   pWnd = GetDlgItem(IDC_BACKUP_PATH_EDIT);
            GotoDlgCtrl(pWnd);
            return FALSE;
        }
        else
        {
            if(m_strBackupPath.Find(_T('.')) == -1)
            {
                m_strBackupPath += _T(".4cb");
                CWnd*   pWnd = GetDlgItem(IDC_BACKUP_PATH_EDIT);
                pWnd->SetWindowText(m_strBackupPath);
                pWnd->UpdateWindow();
            }
            CFileStatus tStatus;
            if(!CFile::GetStatus(m_strBackupPath, tStatus))
            {
                ::AfxMessageBox(_T("Specified backup file does not exist."));
                CWnd*   pWnd = GetDlgItem(IDC_BACKUP_PATH_EDIT);
                GotoDlgCtrl(pWnd);
                return FALSE;
            }
        }
        break;
    case TARGET_MODE:
        if(m_strAddress.IsEmpty())
        {
            ::AfxMessageBox(IDS_ERR_NO_ADDRESS);
            CWnd*   pWnd = GetDlgItem(IDC_ADDRESS_EDIT);
            GotoDlgCtrl(pWnd);
            return FALSE;
        }
        break;
    }

    if(m_strProjectPath.IsEmpty())
    {
        ::AfxMessageBox(IDS_ERR_NO_PROJECT_PATH);
        CWnd*   pWnd = GetDlgItem(IDC_PROJECT_PATH_EDIT);
        GotoDlgCtrl(pWnd);
        return FALSE;
    }

    CFileStatus tStatus;
    if(CFile::GetStatus(m_strProjectPath, tStatus))
    {
        CString strMessage(_T("Project Path already exists."));
        ::AfxMessageBox(strMessage);
        m_pMainSheet->SetStatusText(strMessage);
        CWnd*   pWnd = GetDlgItem(IDC_PROJECT_PATH_EDIT);
        GotoDlgCtrl(pWnd);
        return FALSE;
    }
    return TRUE;
}


BOOL CPropPageRestore::RestoreProject()
{
    BOOL    bReturn = FALSE;

    // get installation path: install path + Engineering\bin
    CString strCmdLine;
    CString strInstallPath;
    CString strBackupPath(m_strBackupPath);
    size_t sizeInstDir = MAX_PATH;
    if(S_OK != UTIL_GetInstallPath(strInstallPath.GetBuffer(MAX_PATH), &sizeInstDir))
    {
        return bReturn;
    }
    strInstallPath.ReleaseBuffer();
    strInstallPath += _T("Engineering\\bin");
    
    if(m_iSource == TARGET_MODE)
    {
        m_pMainSheet->SetStatusText(_T("Upload from Target ..."));
        // get temporary backup path
        CString strTempPath;
        CString strFileName;
        VERIFY(::GetTempPath(_MAX_PATH, strTempPath.GetBuffer(_MAX_PATH)));
        strTempPath.ReleaseBuffer();
        VERIFY(::GetTempFileName(strTempPath, "bar", 1, strFileName.GetBuffer(_MAX_PATH)));
        strFileName.ReleaseBuffer();

        strBackupPath = strFileName;
        ::DeleteFile(strBackupPath);    // delete temporary backup file

        m_pMainSheet->SetStatusText(_T("Upload from Target ..."));

        // upload backup file from target
        CString strTargetType = m_pMainSheet->GetTargetType();
        CString strTargetControl = m_pMainSheet->GetControlString(m_strAddress);

        if(UploadProject(strBackupPath, strTargetControl, strTargetType))
        {
            m_pMainSheet->SetStatusText(_T("Upload Done"));
        }
        else
        {
            AfxMessageBox(_T("Could not upload project"), MB_ICONEXCLAMATION);
            return FALSE;
        }
    }
    
    // build command line
    CString strExe;
    CString strParam;
    CString strPassword;
    strExe.Format(_T("\"%s\\4cbar.exe\""), strInstallPath);

    if(!m_strPassword.IsEmpty())
    {
        strPassword.Format(_T(" -encrypt \"%s\""), m_strPassword);
    }

    strExe.Format(_T("\"%s\\4cbar.exe\""), strInstallPath);
    strParam.Format(_T(" -project \"%s\" -restore \"%s\" -file%s"), m_strProjectPath, strBackupPath, strPassword);

    strCmdLine = strExe + strParam;

    // execute command
    m_pMainSheet->SetStatusText(_T("Restore from File ..."));

    TRACE("%s", strCmdLine);
    int iRet = ExecuteCommand(strCmdLine);

    if(iRet != ST_OK)
    {
        CString strMessage = _T("Restore failed");
        CString strReason;
        
        switch(iRet)
        {
            case 502:
                strReason = _T("Project directory already exists.");
                break;
            case 503:
                strReason = _T("Invalid archive format.");
                break;
            case 507:
                strReason = _T("Wrong encryption password.");
                break;
            case 600:
                strReason = _T("Wrong format or encryption password missing.");
                break;
        }
        if(!strReason.IsEmpty())
        {
            strMessage += _T(":\n") + strReason;
        }
        else
        {
            strReason = strMessage;
        }
        AfxMessageBox(strMessage);
        m_pMainSheet->SetStatusText(strReason);
        return FALSE;
    }

    if(!LoadTargetInfoList(m_strProjectPath, TRUE))
    {
        m_pMainSheet->SetStatusText(_T("Could not read target info. OPC-Configuration not restored"));
        if(m_iSource == TARGET_MODE)
        {
            ::DeleteFile(strBackupPath);    // delete temporary backup file
        }
        return FALSE;
    }

    if(iRet == ST_OK)
    {
        if(m_bOPCServerConfig)
        {
            TTargetInfo*    pTargetInfo;
            CString         strErrMessage;
            CString         strErrTargets;
            BOOL            bError;

            POSITION    pos = m_tTargetInfoList.GetHeadPosition();
            while(pos)
            {
                bError = FALSE;
                pTargetInfo = m_tTargetInfoList.GetNext(pos);
                if(CopyOPCServerConfig(pTargetInfo, m_strProjectPath) != ST_OK)
                {
                    bError = TRUE;
                }
                if(PatchOPCServerRegistry(pTargetInfo, m_strProjectPath) != ST_OK)
                {
                    bError = TRUE;
                }
                if(bError)
                {
                    if(!strErrTargets.IsEmpty())
                    {
                        strErrTargets += _T(", ");
                    }
                    strErrTargets += pTargetInfo->strTargetName;
                }
            }
            if(strErrTargets.IsEmpty())
            {
                bReturn = TRUE;
            }
            else
            {
                strErrMessage.Format(IDS_ERR_RESTORE_OPC_CONFIG, strErrTargets);
                m_pMainSheet->SetStatusText(_T("Error: could not restore OPC configuration"));
                ::AfxMessageBox(strErrMessage, MB_ICONEXCLAMATION);
            }
        }
        else
        {
            // nothing to do -> ok
            bReturn = TRUE;
        }
    }

    if(m_iSource == TARGET_MODE)
    {
        ::DeleteFile(strBackupPath);    // delete temporary backup file
    }

    return bReturn;
}

BOOL CPropPageRestore::OnInitDialog() 
{
	CPMPropPage::OnInitDialog();
	
    SetSourceMode();
    
    OnCommSettingsChanged();

    SetConnectStatus(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropPageRestore::OnChangeAddressEdit() 
{
    UpdateData();
}

void CPropPageRestore::OnChangeBackupPathEdit() 
{
    UpdateData();
}

void CPropPageRestore::OnChangeProjectPathEdit() 
{
    UpdateData();

    TryLoadTargetInfo(m_strProjectPath);
}



void CPropPageRestore::OnOpcserverConfigCheck() 
{
    UpdateData();
}

BOOL CPropPageRestore::OnKillActive() 
{
    UpdateData();

    SaveToRegistry(TRUE);
    
	return CPMPropPage::OnKillActive();
}

BOOL CPropPageRestore::OnSetActive() 
{
    if(m_pMainSheet->IsStartup())
    {
        InitializeFromRegistry(FALSE);
    }
    else
    {
        InitializeFromRegistry(TRUE);
    }

    UpdateData(FALSE);

	return CPMPropPage::OnSetActive();
}

void CPropPageRestore::OnMaxtextPasswordEdit() 
{
    m_pMainSheet->SetStatusText(_T("Encryption Password may not exeed 8 characters"));
}
