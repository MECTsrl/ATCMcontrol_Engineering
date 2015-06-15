// PropPageBackup.cpp : implementation file
//

#include "stdafx.h"
#include "PMTool.h"
#include "PropPageBackup.h"
#include "FolderDialog.h"
#include "MainPropertySheet.h"
#include "resource.h"
#include "defs.h"

#include "utilif.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropPageBackup property page

IMPLEMENT_DYNCREATE(CPropPageBackup, CPMPropPage)

CPropPageBackup::CPropPageBackup(CMainPropertySheet* pMainSheet /*=NULL*/, const CString& crstrProjectPath /*=_T("")*/)
 : CPMPropPage(CPropPageBackup::IDD, pMainSheet)
{
	//{{AFX_DATA_INIT(CPropPageBackup)
	m_strBackupPath = _T("");
	m_strProjectPath = crstrProjectPath;
	m_bOPCServerConfig = TRUE;
	m_iDestination = FILE_MODE;
	m_strPassword = _T("");
	//}}AFX_DATA_INIT

    InitializeFromRegistry();
}

CPropPageBackup::~CPropPageBackup()
{
    SaveToRegistry();
}

void CPropPageBackup::InitializeFromRegistry(BOOL bGeneral /*=FALSE*/)
{
    m_strBackupPath     = AfxGetApp()->GetProfileString("Settings", "Backup_BackupPath");
    if(m_strProjectPath.IsEmpty())
    {
        m_strProjectPath    = AfxGetApp()->GetProfileString("Settings", "Backup_ProjectPath");
    }
    m_bOPCServerConfig  = AfxGetApp()->GetProfileInt("Settings", "Backup_OPCServerConfig", 1);
    m_iDestination      = AfxGetApp()->GetProfileInt("Settings", "Backup_Destination", 0);

    // if paths within property page are empty
    // take general paths (of other pages)
    if(bGeneral || m_strBackupPath.IsEmpty())
    {
        m_strBackupPath = AfxGetApp()->GetProfileString("Settings", "BackupPath");
    }
    if(bGeneral || m_strProjectPath.IsEmpty())
    {
        m_strProjectPath = AfxGetApp()->GetProfileString("Settings", "ProjectPath");
    }
}

void CPropPageBackup::SaveToRegistry(BOOL bGeneral /*=FALSE*/)
{
    AfxGetApp()->WriteProfileString("Settings", "Backup_BackupPath", m_strBackupPath);
    AfxGetApp()->WriteProfileString("Settings", "Backup_ProjectPath", m_strProjectPath);
    AfxGetApp()->WriteProfileInt("Settings", "Backup_OPCServerConfig", m_bOPCServerConfig);
    AfxGetApp()->WriteProfileInt("Settings", "Backup_Destination", m_iDestination);

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
    }
}

void CPropPageBackup::DoDataExchange(CDataExchange* pDX)
{
	CPMPropPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageBackup)
	DDX_Control(pDX, IDC_TARGET_COMBO, m_tTargetCombo);
	DDX_Text(pDX, IDC_BACKUP_PATH_EDIT, m_strBackupPath);
	DDX_Text(pDX, IDC_PROJECT_PATH_EDIT, m_strProjectPath);
	DDX_Check(pDX, IDC_OPCSERVER_CONFIG_CHECK, m_bOPCServerConfig);
	DDX_Radio(pDX, IDC_FILE_SYSTEM_RADIO, m_iDestination);
	DDX_Text(pDX, IDC_PASSWORD_EDIT, m_strPassword);
	DDV_MaxChars(pDX, m_strPassword, 8);
	//}}AFX_DATA_MAP
}


CString CPropPageBackup::GetProjectPath()
{
    return m_strProjectPath;
}


BEGIN_MESSAGE_MAP(CPropPageBackup, CPMPropPage)
	//{{AFX_MSG_MAP(CPropPageBackup)
	ON_BN_CLICKED(IDC_BACKUP_BTN, OnBackupBtn)
	ON_BN_CLICKED(IDC_BACKUP_PATH_BTN, OnBackupPathBtn)
	ON_BN_CLICKED(IDC_CONTROL_RADIO, OnControlRadio)
	ON_BN_CLICKED(IDC_FILE_SYSTEM_RADIO, OnFileSystemRadio)
	ON_BN_CLICKED(IDC_PROJECT_PATH_BTN, OnProjectPathBtn)
	ON_EN_CHANGE(IDC_ADDRESS_EDIT, OnChangeAddressEdit)
	ON_EN_CHANGE(IDC_BACKUP_PATH_EDIT, OnChangeBackupPathEdit)
	ON_EN_CHANGE(IDC_PROJECT_PATH_EDIT, OnChangeProjectPathEdit)
	ON_BN_CLICKED(IDC_OPCSERVER_CONFIG_CHECK, OnOpcserverConfigCheck)
	ON_EN_CHANGE(IDC_PASSWORD_EDIT, OnChangePasswordEdit)
	ON_EN_MAXTEXT(IDC_PASSWORD_EDIT, OnMaxtextPasswordEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropPageBackup message handlers

void CPropPageBackup::OnBackupBtn() 
{
    m_pMainSheet->SetStatusText(_T("Backup Project ..."));

    UpdateData();

    if(!CheckData())
    {
        return;
    }
    
    if(BackupProject())
    {
        m_pMainSheet->SetStatusText(_T("Backup Done"));
        m_strPassword.Empty();
        CWnd*   pWnd = GetDlgItem(IDC_PASSWORD_EDIT);
        if(pWnd && pWnd->GetSafeHwnd())
        {
            pWnd->SetWindowText(_T(""));
        }
    }
}

BOOL CPropPageBackup::CheckData()
{
    if(m_strProjectPath.IsEmpty())
    {
        CString strMessage;
        strMessage.LoadString(IDS_ERR_NO_PROJECT_PATH);
        ::AfxMessageBox(strMessage);
        //m_pMainSheet->SetStatusText(strMessage);
        CWnd*   pWnd = GetDlgItem(IDC_PROJECT_PATH_EDIT);
        GotoDlgCtrl(pWnd);
        return FALSE;
    }
    else
    {
        CFileStatus tStatus;
        if(!CFile::GetStatus(m_strProjectPath, tStatus))
        {
            ::AfxMessageBox(_T("Specified project does not exist."));
            CWnd*   pWnd = GetDlgItem(IDC_PROJECT_PATH_EDIT);
            GotoDlgCtrl(pWnd);
            return FALSE;
        }
    }
    switch(m_iDestination)
    {
    case FILE_MODE:
        if(m_strBackupPath.IsEmpty())
        {
            CString strMessage;
            strMessage.LoadString(IDS_ERR_NO_BACKUP_PATH);
            ::AfxMessageBox(strMessage);
            //m_pMainSheet->SetStatusText(strMessage);
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
            if(CFile::GetStatus(m_strBackupPath, tStatus))
            {
                CString strMessage(_T("Backup File already exists.\nOverwrite?"));
                if(::AfxMessageBox(strMessage, MB_YESNO) == IDYES)
                {
                    DeleteFile(m_strBackupPath);
                    break;
                }
                else
                {
                    CWnd*   pWnd = GetDlgItem(IDC_PROJECT_PATH_EDIT);
                    GotoDlgCtrl(pWnd);
                }
                return FALSE;
            }
        }
        break;
    case TARGET_MODE:
        if(m_tTargetCombo.GetCurSel() == CB_ERR)
        {
            CString strMessage;
            strMessage.LoadString(IDS_ERR_NO_TARGET);
            ::AfxMessageBox(strMessage);
            //m_pMainSheet->SetStatusText(strMessage);
            CWnd*   pWnd = GetDlgItem(IDC_TARGET_COMBO);
            GotoDlgCtrl(pWnd);
            return FALSE;
        }
        break;
    }
    return TRUE;
}

BOOL CPropPageBackup::BackupProject()
{
    CString strCmdLine;
    CString strInstallPath;
    CString strBackupPath(m_strBackupPath);
    size_t sizeInstDir = MAX_PATH;
    if(S_OK != UTIL_GetInstallPath(strInstallPath.GetBuffer(MAX_PATH), &sizeInstDir))
    {
        return FALSE;
    }
    strInstallPath.ReleaseBuffer();
    strInstallPath += _T("Engineering\\bin");

    // if destination is target, use temporary backup file
    if(m_iDestination == TARGET_MODE)
    {
        CString strTempPath;
        CString strFileName;
        VERIFY(::GetTempPath(_MAX_PATH, strTempPath.GetBuffer(_MAX_PATH)));
        strTempPath.ReleaseBuffer();
        VERIFY(::GetTempFileName(strTempPath, "bar", 1, strFileName.GetBuffer(_MAX_PATH)));
        strFileName.ReleaseBuffer();

        strBackupPath = strFileName;
        ::DeleteFile(strBackupPath);
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

    if(m_bOPCServerConfig)
    {
        strParam.Format(_T(" -project \"%s\" -backup \"%s\" -file -all%s"), m_strProjectPath, strBackupPath, strPassword);
    }
    else
    {
        strParam.Format(_T(" -project \"%s\" -backup \"%s\" -file%s"), m_strProjectPath, strBackupPath, strPassword);
    }
    strCmdLine = strExe + strParam;

    // execute command
    TRACE("%s", strCmdLine);
    int iRet = ExecuteCommand(strCmdLine);
    if(iRet != ST_OK)
    {
        CString strMessage = _T("Backup failed");
        CString strReason;
        
        switch(iRet)
        {
            case 503:
                strReason = _T("Invalid archive format.");
                break;
            case 506:
                strReason = _T("Project directory not found.");
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

    // download project on target
    if(m_iDestination == TARGET_MODE)
    {
        int iSelTarget = m_tTargetCombo.GetCurSel();
        if(iSelTarget == CB_ERR)
        {
            m_pMainSheet->SetStatusText(_T("Error: No Target"));
            return FALSE;
        }
        TTargetInfo*    pTargetInfo = (TTargetInfo*)m_tTargetCombo.GetItemData(iSelTarget);
        if(!pTargetInfo)
        {
            m_pMainSheet->SetStatusText(_T("Error: No Target"));
            return FALSE;
        }
        m_pMainSheet->SetStatusText(_T(""));
        // download backup file to target
        if(DownloadProject(strBackupPath, pTargetInfo))
        {
            m_pMainSheet->SetStatusText(_T("Download Done"));
        }
        else
        {
            AfxMessageBox(_T("Could not download project"), MB_ICONEXCLAMATION);
            return FALSE;
        }
        ::DeleteFile(strBackupPath);
    }

    return TRUE;
}

void CPropPageBackup::OnBackupPathBtn() 
{
    // not needed here because OnChangeBackupPathEdit() does the job:
    //UpdateData();

    // file dialog	
    CString strFilter;
    CString strFile;

    strFilter.LoadString(IDS_BARFILEFILTER);
    CFileDialog tFileDlg(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter, this);

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
        // if file is existing, file save dialog asks user whether to overwrite it
        // if you choose no, you can only leave dialog with cancel
        // i.e. user has chosen yes here and we can remove the file now
        // backup tool will not work, if file is already existing.
        ::DeleteFile(m_strBackupPath);
    }
}

void CPropPageBackup::OnControlRadio() 
{
    UpdateData();

    SetDestinationMode();
}

void CPropPageBackup::OnFileSystemRadio() 
{
    UpdateData();

    SetDestinationMode();
}

//------------------------------------------------------------------*
/**
 * SetDestinationMode
 *
 *  enables / disables project path edit / button and address edit
 *  according to destination mode (m_iDestination)
 *  When called from within radio button handlers, UpdateData must
 *  be called before to ensure that m_iDestination has been set correctly.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CPropPageBackup::SetDestinationMode()
{
    BOOL    bFileSystem = (m_iDestination == FILE_MODE);
    
    // enable/disable project path edit, button, address edit according
    // to destination
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
    pWnd = GetDlgItem(IDC_TARGET_COMBO);
    if(pWnd)
    {
        pWnd->EnableWindow(!bFileSystem);
    }
}

void CPropPageBackup::OnProjectPathBtn() 
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
		break;
		
	default:
		break;
	}

    CString strCurTarget = GetCurrentTargetString();
    TryLoadTargetInfo(m_strProjectPath);
    FillTargetCombo(strCurTarget);
}

BOOL CPropPageBackup::OnInitDialog() 
{
	CPMPropPage::OnInitDialog();
	
    SetDestinationMode();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropPageBackup::OnChangeAddressEdit() 
{
    UpdateData();
}

void CPropPageBackup::OnChangeBackupPathEdit() 
{
    UpdateData();
}

void CPropPageBackup::OnChangeProjectPathEdit() 
{
    UpdateData();

    CString strCurTarget = GetCurrentTargetString();
    TryLoadTargetInfo(m_strProjectPath);
    FillTargetCombo(strCurTarget);
}


//------------------------------------------------------------------*
/**
 * FillTargetCombo
 *
 * @param           crstrOldTargetString: old target string
 * @return          -
 * @exception       -
 * @see             GetCurrentTargetString()
*/
void CPropPageBackup::FillTargetCombo(const CString& crstrOldTargetString)
{
    m_tTargetCombo.ResetContent();

    POSITION        pos = m_tTargetInfoList.GetHeadPosition();
    int             iItem;
    CString         strTargetString;
    TTargetInfo*    pTargetInfo;
    while(pos)
    {
        pTargetInfo = m_tTargetInfoList.GetNext(pos);
        if(pTargetInfo->strTargetType.CompareNoCase(_T("4cpc")) != 0)
        {
            strTargetString.Format(_T("%s (%s)"), pTargetInfo->strTargetName, pTargetInfo->strTargetAddress);
            iItem = m_tTargetCombo.AddString(strTargetString);
            m_tTargetCombo.SetItemData(iItem, (DWORD)pTargetInfo);
        }
    }

    int iCurSel = m_tTargetCombo.FindStringExact(0, crstrOldTargetString);
    if(iCurSel == CB_ERR)
    {
        iCurSel = m_tTargetCombo.FindString(0, crstrOldTargetString);
        if(iCurSel == CB_ERR)
        {
            iCurSel = 0;
        }
    }
    if(m_tTargetCombo.GetCount() > 0)
    {
        m_tTargetCombo.SetCurSel(iCurSel);
    }
}


//------------------------------------------------------------------*
/**
 * GetCurrentTargetString
 *
 *  use this function as input for FillTargetCombo
 *  to reselect current target after filling.
 *  caution:
 *  call this function before loading target information
 *  otherwise the function may crash because target
 *  info objects have been deleted.
 *
 * @param           -
 * @return          current target string
 * @exception       -
 * @see             -
*/
CString CPropPageBackup::GetCurrentTargetString()
{
    CString strCurTargetString;

    int             iCurSel = m_tTargetCombo.GetCurSel();
    TTargetInfo*    pTargetInfo;
    if(iCurSel != CB_ERR)
    {
        pTargetInfo = (TTargetInfo*)m_tTargetCombo.GetItemData(iCurSel);
        strCurTargetString.Format(_T("%s (%s)"), pTargetInfo->strTargetName, pTargetInfo->strTargetAddress);
    }
    return strCurTargetString;
}


void CPropPageBackup::OnOpcserverConfigCheck() 
{
    UpdateData();
}

BOOL CPropPageBackup::OnKillActive() 
{
    UpdateData();

    SaveToRegistry(TRUE);
    
	return CPMPropPage::OnKillActive();
}

BOOL CPropPageBackup::OnSetActive() 
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

    CString strCurTarget = GetCurrentTargetString();
    TryLoadTargetInfo(m_strProjectPath);
    FillTargetCombo(strCurTarget);

	return CPMPropPage::OnSetActive();
}


void CPropPageBackup::OnChangePasswordEdit() 
{
    UpdateData();
}

void CPropPageBackup::OnMaxtextPasswordEdit() 
{
    m_pMainSheet->SetStatusText(_T("Encryption Password may not exeed 8 characters"));
}
