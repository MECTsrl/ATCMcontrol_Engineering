// PropPageFileTransfer.cpp : implementation file
//

#include "stdafx.h"
#include "PMTool.h"
#include "PropPageFileTransfer.h"
#include "FolderDialog.h"
#include "fc_tools\fc_fileIO.h"

#define CONNECT_DELAY_TIMER (1)
#define CONNECT_DELAY_TIME  (1500)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropPageFileTransfer property page

IMPLEMENT_DYNCREATE(CPropPageFileTransfer, CPMPropPage)

CPropPageFileTransfer::CPropPageFileTransfer(CMainPropertySheet* pMainSheet /*=NULL*/) 
  : CPMPropPage(CPropPageFileTransfer::IDD, pMainSheet)
{
	//{{AFX_DATA_INIT(CPropPageFileTransfer)
	m_strAddress = _T("");
	m_strPCPath = _T("");
	//}}AFX_DATA_INIT

    m_hModule   = NULL;
    m_pLayer    = NULL;

    InitializeFromRegistry();
}

CPropPageFileTransfer::~CPropPageFileTransfer()
{
    StopCommunication();

    SaveToRegistry();
}

void CPropPageFileTransfer::DoDataExchange(CDataExchange* pDX)
{
	CPMPropPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageFileTransfer)
	DDX_Control(pDX, IDC_PC_FILE_LIST, m_tPCFileListBox);
	DDX_Control(pDX, IDC_CONTROL_FILE_LIST, m_tControlFileListBox);
	DDX_Text(pDX, IDC_ADDRESS_EDIT, m_strAddress);
	DDX_Text(pDX, IDC_PC_PATH_EDIT, m_strPCPath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropPageFileTransfer, CPMPropPage)
	//{{AFX_MSG_MAP(CPropPageFileTransfer)
	ON_EN_CHANGE(IDC_ADDRESS_EDIT, OnChangeAddressEdit)
	ON_BN_CLICKED(IDC_DOWNLOAD_BTN, OnDownloadBtn)
	ON_BN_CLICKED(IDC_PC_PATH_BTN, OnPcPathBtn)
	ON_EN_CHANGE(IDC_PC_PATH_EDIT, OnChangePcPathEdit)
	ON_BN_CLICKED(IDC_UPLOAD_BTN, OnUploadBtn)
	ON_BN_CLICKED(IDC_REFRESH_PC_BTN, OnRefreshPcBtn)
	ON_BN_CLICKED(IDC_REFRESH_CONTROL_BTN, OnRefreshControlBtn)
	ON_BN_CLICKED(IDC_PC_DELETE_BTN, OnPcDeleteBtn)
	ON_BN_CLICKED(IDC_CONTROL_DELETE_BTN, OnControlDeleteBtn)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CPropPageFileTransfer::InitializeFromRegistry(BOOL bGeneral /*=FALSE*/)
{
    m_strPCPath         = AfxGetApp()->GetProfileString("Settings", "FT_PCPath");
    m_strAddress        = AfxGetApp()->GetProfileString("Settings", "FT_Address");

    if(bGeneral || m_strAddress.IsEmpty())
    {
        m_strAddress = AfxGetApp()->GetProfileString("Settings", "Address");
    }

    // avoid pc path being empty
    if(m_strPCPath.IsEmpty())
    {
        GetCurrentDirectory(_MAX_PATH, m_strPCPath.GetBuffer(_MAX_PATH));
        m_strPCPath.ReleaseBuffer();
    }

    m_bAutoConnect = AfxGetApp()->GetProfileInt("Settings", "FT_AutoConnect", FALSE);
}

void CPropPageFileTransfer::SaveToRegistry(BOOL bGeneral /*=FALSE*/)
{
    AfxGetApp()->WriteProfileString("Settings", "FT_PCPath", m_strPCPath);
    AfxGetApp()->WriteProfileString("Settings", "FT_Address", m_strAddress);

    if(bGeneral)
    {
        if(!m_strAddress.IsEmpty())
        {
            AfxGetApp()->WriteProfileString("Settings", "Address", m_strAddress);
        }
    }

    AfxGetApp()->WriteProfileInt("Settings", "FT_AutoConnect", m_bAutoConnect);
}

/////////////////////////////////////////////////////////////////////////////
// CPropPageFileTransfer message handlers

void CPropPageFileTransfer::OnChangeAddressEdit() 
{
    UpdateData();
    StopCommunication();
}


void CPropPageFileTransfer::OnDownloadBtn() 
{
    if(!DownloadFiles())
    {
        AfxMessageBox(_T("Errors occurred\nNot all files could be downloaded"));
    }
    else
    {
        m_pMainSheet->SetStatusText(_T("Download Files done."));
    }
}


BOOL CPropPageFileTransfer::DownloadFiles()
{
    if(m_tPCFileListBox.GetSelCount() == 0)
    {
        ::AfxMessageBox(_T("Nothing selected to download"));
        return TRUE;
    }
    if(!InitializeCommunication())
    {
        return FALSE;
    }
    if(!ConnectToTarget(m_pLayer))
    {
        return FALSE;
    }

    CString     strFileName;
    CString     strFilePath;
    CString     strPCDir(m_strPCPath);

    if(strPCDir.GetAt(strPCDir.GetLength()-1) == _T('\\'))
    {
        strPCDir = strPCDir.Left(strPCDir.GetLength() - 1);
    }

    BOOL            bReturn = TRUE;
    BOOL            bOverwrite = FALSE;
    CStringArray    astrDownloaded;

    int iNumFiles = m_tPCFileListBox.GetSelCount();
    CArray<int,int> aiSelectedItems;

    aiSelectedItems.SetSize(iNumFiles);
    m_tPCFileListBox.GetSelItems(iNumFiles, aiSelectedItems.GetData());
    
    m_pMainSheet->InitProgress(0, iNumFiles);

    for(int iFile = 0; iFile < iNumFiles; ++iFile)
    {
        m_tPCFileListBox.GetText(aiSelectedItems[iFile], strFileName);
        strFilePath.Format(_T("%s\\%s"), strPCDir, strFileName);

        if(!DownloadFile(strFilePath, m_pLayer))
        {
            if(iFile < iNumFiles - 1)
            {
                CString strMessage;
                strMessage.Format(_T("Error downloading file %s.\nContinue?"), strFileName);
                if(AfxMessageBox(strMessage, MB_YESNO) == IDNO)
                {
                    m_pMainSheet->SetStatusText(_T("File download aborted."));
                    break;
                }
            }
            bReturn = FALSE;
        }
        else
        {
            astrDownloaded.Add(strFileName);
        }
        m_pMainSheet->SetProgress(iFile+1);
    }

    DoFillControlFileList();
    
    // unselect all files on control side
    m_tControlFileListBox.SetSel(-1, FALSE);

    // try to select all downloaded files on control side
    int iSel;
    iNumFiles = astrDownloaded.GetSize();
    for(iFile = 0; iFile < iNumFiles; ++iFile)
    {
        iSel = m_tControlFileListBox.FindStringExact(0, astrDownloaded[iFile]);
        if(iSel != LB_ERR)
        {
            m_tControlFileListBox.SetSel(iSel);
        }
    }

    m_pMainSheet->StopProgress();
    return bReturn;
}

void CPropPageFileTransfer::OnPcPathBtn() 
{
    // not needed here because OnChangePcPathEdit() does the job:
    //UpdateData();

	CFolderDialog fd(m_strPCPath, "Browse folder");

	int iRet = fd.DoModal();

	switch(iRet)
	{
	case IDOK:
		m_strPCPath = fd.m_sSelPath;
		UpdateData(FALSE);
		break;
		
	default:
		break;
	}

    FillPCFileList();
}

void CPropPageFileTransfer::FillPCFileList()
{
    CWaitCursor tWaitCur;
    m_tPCFileListBox.ResetContent();

    if(m_strPCPath.IsEmpty())
    {
        return;
    }

    HANDLE          hfile;
    WIN32_FIND_DATA findFileData;
    char            szFilePattern[MAX_PATH];
    BOOL            bFound = TRUE;

    strcpy(szFilePattern, m_strPCPath);
    strcat(szFilePattern, "\\*.*");
    hfile = FindFirstFile(szFilePattern, &findFileData);
    
    if (hfile == INVALID_HANDLE_VALUE)
    {
        return;
    }
    else
    {
        CString strFileTmp;

        while(bFound)
        {
            strFileTmp = findFileData.cFileName;
            if(!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                m_tPCFileListBox.AddString(strFileTmp);
            }
            bFound = FindNextFile(hfile, &findFileData);
        }
    }
    FindClose(hfile);
}

void CPropPageFileTransfer::FillControlFileList()
{
    if(GetSafeHwnd() && m_bAutoConnect)		// SUPFILTRANS 13.06.07 SIS
    {
        KillTimer(CONNECT_DELAY_TIMER);
        SetTimer(CONNECT_DELAY_TIMER, CONNECT_DELAY_TIME, NULL);
    }
}


BOOL CPropPageFileTransfer::DoFillControlFileList()
{
    CWaitCursor tWaitCur;

    m_tControlFileListBox.ResetContent();

    BOOL bReturn = FALSE;
    // load onl comm library
    // TODO:
    // let user specify target type
    // try to load onl comm library in local directory
    if(InitializeCommunication())
    {
        if(ConnectToTarget(m_pLayer))
        {
            CStringArray    astrFiles;
            if(UploadDirectoryContent(astrFiles, m_pLayer))
            {
                int iNumFiles = astrFiles.GetSize();

                for(int iFile = 0; iFile < iNumFiles; ++iFile)
                {
                    m_tControlFileListBox.AddString(astrFiles[iFile]);
                }
                bReturn = TRUE;
                m_pMainSheet->SetStatusText(_T("Upload Directory Content done."));
            }
        }
    }
    else
    {
        StopCommunication();
    }

    m_bAutoConnect = bReturn;
    
    return bReturn;
}


void CPropPageFileTransfer::OnChangePcPathEdit() 
{
    UpdateData();

    FillPCFileList();
}

void CPropPageFileTransfer::OnUploadBtn() 
{
    if(!UploadFiles())
    {
        AfxMessageBox(_T("Errors occurred\nNot all files could be uploaded"));
    }
    else
    {
        m_pMainSheet->SetStatusText(_T("Upload Files done."));
    }
}

BOOL CPropPageFileTransfer::UploadFiles()
{
    if(m_tControlFileListBox.GetSelCount() == 0)
    {
        ::AfxMessageBox(_T("Nothing selected to upload"));
        return TRUE;
    }
    if(!CheckData())
    {
        return FALSE;
    }

    CFileStatus tFileStatus;
    if(!CFile::GetStatus(m_strPCPath, tFileStatus))
    {
        FC_CreateSubDir(m_strPCPath);
    }
    
    if(!InitializeCommunication())
    {
        return FALSE;
    }
    if(!ConnectToTarget(m_pLayer))
    {
        return FALSE;
    }

    CString     strFileName;
    CString     strFilePath;
    CString     strPCDir(m_strPCPath);

    if(strPCDir.GetAt(strPCDir.GetLength()-1) == _T('\\'))
    {
        strPCDir = strPCDir.Right(strPCDir.GetLength() - 1);
    }
    BOOL            bReturn = TRUE;
    BOOL            bOverwrite = FALSE;
    CFileStatus     tStatus;
    CStringArray    astrUploaded;

    int iNumFiles = m_tControlFileListBox.GetSelCount();
    CArray<int,int> aiSelectedItems;

    aiSelectedItems.SetSize(iNumFiles);
    m_tControlFileListBox.GetSelItems(iNumFiles, aiSelectedItems.GetData());

    m_pMainSheet->InitProgress(0, iNumFiles);
    
    for(int iFile = 0; iFile < iNumFiles; ++iFile)
    {
        m_tControlFileListBox.GetText(aiSelectedItems[iFile], strFileName);
        strFilePath.Format(_T("%s\\%s"), strPCDir, strFileName);

        if(CFile::GetStatus(strFilePath, tStatus))
        {
            CString strMessage;
            strMessage.Format(_T("Overwrite existing file %s?"), strFileName);
            int iRet = AfxMessageBox(strMessage, MB_YESNOCANCEL);
            if(iRet == IDCANCEL)
            {
                m_pMainSheet->SetStatusText(_T("File upload aborted."));
                bReturn = FALSE;
                break;
            } 
            else if (iRet == IDNO)
            {
                continue;
            }
        }

        if(!UploadFile(strFilePath, m_pLayer))
        {
            if(iFile < iNumFiles - 1)
            {
                CString strMessage;
                strMessage.Format(_T("Error uploading file %s.\nContinue?"), strFileName);
                if(AfxMessageBox(strMessage, MB_YESNO) == IDNO)
                {
                    m_pMainSheet->SetStatusText(_T("File upload aborted."));
                    break;
                }
            }
            bReturn = FALSE;
        }
        else
        {
            astrUploaded.Add(strFileName);
        }
        m_pMainSheet->SetProgress(iFile+1);
    }

    FillPCFileList();

    // unselect all files on PC side
    m_tPCFileListBox.SetSel(-1, FALSE);

    // try to select all uploaded files on PC side
    int iSel;
    iNumFiles = astrUploaded.GetSize();
    for(iFile = 0; iFile < iNumFiles; ++iFile)
    {
        iSel = m_tPCFileListBox.FindStringExact(0, astrUploaded[iFile]);
        if(iSel != LB_ERR)
        {
            m_tPCFileListBox.SetSel(iSel);
        }
    }

    m_pMainSheet->StopProgress();

    return bReturn;
}

BOOL CPropPageFileTransfer::OnKillActive() 
{
    UpdateData();

    //StopCommunication();
	
    SaveToRegistry(TRUE);

	return CPMPropPage::OnKillActive();
}

BOOL CPropPageFileTransfer::OnSetActive() 
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

    FillPCFileList();

	EnablePage(m_pMainSheet->GetSupportFileTransfer());		// SUPFILTRANS 13.06.07 SIS

    // if not startup, fill control file list always
    if(!m_pMainSheet->IsStartup())
    {
        FillControlFileList();
    }
    else
    {
        // else only if auto connect
        if(m_bAutoConnect)
        {
            FillControlFileList();
        }
    }

	return CPMPropPage::OnSetActive();
}

BOOL CPropPageFileTransfer::OnInitDialog() 
{
	CPMPropPage::OnInitDialog();

    SetConnectStatus(FALSE);                 // to initialize connect status

    CPMPropPage::OnCommSettingsChanged();    // to update connect info

    EnablePage(m_pMainSheet->GetSupportFileTransfer());		// SUPFILTRANS 24.05.07 SIS

    if(m_bAutoConnect)
    {
        FillControlFileList();
    }
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPropPageFileTransfer::InitializeCommunication()
{
    if(m_strAddress.IsEmpty())
    {
        m_pMainSheet->SetStatusText(_T("Target address not yet specified."));
        return FALSE;
    }

    if(m_pLayer != NULL)
    {
        return TRUE;
    }

    CString strTargetType = m_pMainSheet->GetTargetType();
    CString strTargetControl = m_pMainSheet->GetControlString(m_strAddress);

    if(m_hModule == NULL)
    {
        m_hModule = LoadOnlCommLibraryForTarget(strTargetType);
    }

    //HMODULE m_hModule = LoadLocalOnlCommLibrary();
    if(m_hModule == NULL)
    {
        // use win target if not found locally
        m_hModule = LoadOnlCommLibraryForTarget(_T("4CWin"));
    }
    if (m_hModule == NULL)
    {
        m_pMainSheet->SetStatusText(_T("Could not load online dll"));
        return FALSE;
    }
    // connect to target
    LPCONSTRUCT4CLAYER lpConstruct4CLayer = (LPCONSTRUCT4CLAYER)GetProcAddress(m_hModule, "Construct4CLayer");
    if (lpConstruct4CLayer == NULL)
    {
        FreeLibrary(m_hModule);
        m_hModule = NULL;
        m_pMainSheet->SetStatusText(_T("Could not get proc address for Construct4CLayer"));
        return FALSE;
    }
    m_pLayer = lpConstruct4CLayer(strTargetControl);
    if(m_pLayer == NULL)
    {
        return FALSE;
    }
    return TRUE;
}


void CPropPageFileTransfer::StopCommunication()
{
    // remember whether I was connected to control
    // if so reconnect on start up
    m_bAutoConnect = FALSE;
    if(m_pLayer && !m_pLayer->IsConnected())    // TODO inverse IsConnected()
    {
        m_bAutoConnect = TRUE;
    }

    if(m_pLayer)
    {
        DisconnectFromTarget(m_pLayer);
        delete m_pLayer;
        m_pLayer = NULL;
    }
    if(m_hModule)
    {
        FreeLibrary(m_hModule);
        m_hModule = NULL;
    }
}

void CPropPageFileTransfer::OnRefreshPcBtn() 
{
    FillPCFileList();
    m_pMainSheet->SetStatusText(_T("Refresh done."));
}

void CPropPageFileTransfer::OnRefreshControlBtn() 
{
    if(m_strAddress.IsEmpty())
    {
        ::AfxMessageBox(IDS_ERR_NO_ADDRESS);
        CWnd*    pWnd = GetDlgItem(IDC_ADDRESS_EDIT);
        GotoDlgCtrl(pWnd);
        return;
    }
    StopCommunication();
    if(DoFillControlFileList())
    {
        m_pMainSheet->SetStatusText(_T("Refresh done."));
    }
}


BOOL CPropPageFileTransfer::CheckData()
{
    if(m_strPCPath.IsEmpty())
    {
        ::AfxMessageBox(_T("PC directory empty.\nPlease select a directory"));
        CWnd*   pWnd = GetDlgItem(IDC_PC_PATH_EDIT);
        GotoDlgCtrl(pWnd);
        return FALSE;
    }
    if(m_strAddress.IsEmpty())
    {
        ::AfxMessageBox(_T("Address field is empty.\nPlease specify control address."));
        CWnd*    pWnd = GetDlgItem(IDC_ADDRESS_EDIT);
        GotoDlgCtrl(pWnd);
        return FALSE;
    }
    return TRUE;
}


BOOL CPropPageFileTransfer::PreTranslateMessage(MSG* pMsg) 
{
    if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_DELETE)
    {
    	//TRACE("Message %d, %d, %d\n", pMsg->message, pMsg->wParam, pMsg->lParam);
        CWnd*   pWnd = GetFocus();
        if(pWnd->GetSafeHwnd() == m_tPCFileListBox.GetSafeHwnd())
        {
            OnPcDeleteBtn();
        }
        else if(pWnd->GetSafeHwnd() == m_tControlFileListBox.GetSafeHwnd())
        {
            OnControlDeleteBtn();
        }
        
    }
	return CPMPropPage::PreTranslateMessage(pMsg);
}


BOOL CPropPageFileTransfer::DeletePCFiles()
{
    BOOL bReturn = TRUE;
    int iNumFiles = m_tPCFileListBox.GetSelCount();
    CArray<int,int> aiSelectedItems;
    CString     strPCDir(m_strPCPath);
    CString     strFileName;
    CString     strFilePath;

    if(strPCDir.GetAt(strPCDir.GetLength()-1) == _T('\\'))
    {
        strPCDir = strPCDir.Right(strPCDir.GetLength() - 1);
    }

    aiSelectedItems.SetSize(iNumFiles);
    m_tPCFileListBox.GetSelItems(iNumFiles, aiSelectedItems.GetData());

    m_pMainSheet->InitProgress(0, iNumFiles);

    for(int iFile = 0; iFile < iNumFiles; ++iFile)
    {
        m_tPCFileListBox.GetText(aiSelectedItems[iFile], strFileName);
        strFilePath.Format(_T("%s\\%s"), strPCDir, strFileName);

        if(!DeleteFile(strFilePath))
        {
            bReturn = FALSE;
        }
        m_pMainSheet->SetProgress(iFile+1);
    }

    m_pMainSheet->StopProgress();

    FillPCFileList();

    return bReturn;
}


BOOL CPropPageFileTransfer::DeleteControlFiles()
{
    if(m_tControlFileListBox.GetSelCount() == 0)
    {
        ::AfxMessageBox(_T("Nothing selected to delete"));
        return TRUE;
    }

    if(!InitializeCommunication())
    {
        return FALSE;
    }

    if(!ConnectToTarget(m_pLayer))
    {
        return FALSE;
    }

    CString     strFileName;
    CString     strFilePath;
    CString     strPCDir(m_strPCPath);

    if(strPCDir.GetAt(strPCDir.GetLength()-1) == _T('\\'))
    {
        strPCDir = strPCDir.Right(strPCDir.GetLength() - 1);
    }
    BOOL            bReturn = TRUE;
    BOOL            bOverwrite = FALSE;
    CFileStatus     tStatus;

    int iNumFiles = m_tControlFileListBox.GetSelCount();
    CArray<int,int> aiSelectedItems;

    aiSelectedItems.SetSize(iNumFiles);
    m_tControlFileListBox.GetSelItems(iNumFiles, aiSelectedItems.GetData());

    m_pMainSheet->InitProgress(0, iNumFiles);
    
    for(int iFile = 0; iFile < iNumFiles; ++iFile)
    {
        m_tControlFileListBox.GetText(aiSelectedItems[iFile], strFileName);

        if(!DeleteControlFile(strFileName, m_pLayer))
        {
            bReturn = FALSE;
        }
        m_pMainSheet->SetProgress(iFile+1);
    }

    DoFillControlFileList();

    m_pMainSheet->StopProgress();

    return bReturn;
}

void CPropPageFileTransfer::OnPcDeleteBtn() 
{
    if(m_tPCFileListBox.GetSelCount() > 0)
    {
        if(::AfxMessageBox(_T("Really delete selected files on PC?"), MB_YESNO) == IDNO)
        {
            return;
        }
    }
    if(DeletePCFiles())
    {
        m_pMainSheet->SetStatusText(_T("Files deleted on PC."));
    }
    else
    {
        ::AfxMessageBox(_T("Errors occurred.\nNot all files could be deleted"));
    }
}

void CPropPageFileTransfer::OnControlDeleteBtn() 
{
    if(m_tControlFileListBox.GetSelCount() > 0)
    {
        if(::AfxMessageBox(_T("Really delete selected files on control?"), MB_YESNO) == IDNO)
        {
            return;
        }
    }
    if(DeleteControlFiles())
    {
        m_pMainSheet->SetStatusText(_T("Files deleted on control."));
    }
    else
    {
        ::AfxMessageBox(_T("Errors occurred.\nNot all files could be deleted"));
    }
}

void CPropPageFileTransfer::OnTimer(UINT nIDEvent) 
{
    if(nIDEvent == CONNECT_DELAY_TIMER)
    {
        DoFillControlFileList();
        KillTimer(CONNECT_DELAY_TIMER);
    }
	
	CPMPropPage::OnTimer(nIDEvent);
}

void CPropPageFileTransfer::OnCommSettingsChanged()
{
    CPMPropPage::OnCommSettingsChanged();

    StopCommunication();

	EnablePage(m_pMainSheet->GetSupportFileTransfer());		// SUPFILTRANS 24.05.07 SIS

	// SUPFILTRANS 13.06.07 SIS >>
	if(m_bAutoConnect)
	{
		FillControlFileList();
	}
	// SUPFILTRANS 13.06.07 SIS <<
}

// SUPFILTRANS 24.05.07 SIS >>
void CPropPageFileTransfer::EnablePage(BOOL bEnable)
{
	// if page was not visible yet -> do nothing
	if(GetSafeHwnd() == NULL)
	{
		return;
	}
    CWnd*    pWnd;
	pWnd = GetDlgItem(IDC_PC_PATH_EDIT);
	pWnd->EnableWindow(bEnable);
	pWnd = GetDlgItem(IDC_PC_PATH_BTN);
	pWnd->EnableWindow(bEnable);
	pWnd = GetDlgItem(IDC_PC_DELETE_BTN);
	pWnd->EnableWindow(bEnable);
	pWnd = GetDlgItem(IDC_REFRESH_PC_BTN);
	pWnd->EnableWindow(bEnable);
	pWnd = GetDlgItem(IDC_DOWNLOAD_BTN);
	pWnd->EnableWindow(bEnable);
	pWnd = GetDlgItem(IDC_UPLOAD_BTN);
	pWnd->EnableWindow(bEnable);
	pWnd = GetDlgItem(IDC_ADDRESS_EDIT);
	pWnd->EnableWindow(bEnable);
	pWnd = GetDlgItem(IDC_CONTROL_DELETE_BTN);
	pWnd->EnableWindow(bEnable);
	pWnd = GetDlgItem(IDC_REFRESH_CONTROL_BTN);
	pWnd->EnableWindow(bEnable);
	pWnd = GetDlgItem(IDC_CONTROL_FILE_LIST);
	pWnd->EnableWindow(bEnable);
	pWnd = GetDlgItem(IDC_PC_FILE_LIST);
	pWnd->EnableWindow(bEnable);

	// SUPFILTRANS 13.06.07 SIS >>
	if(!bEnable)
	{
		m_bAutoConnect = FALSE;
		m_tControlFileListBox.ResetContent();
	}
	else
	{
		m_bAutoConnect = TRUE;
	}
	// SUPFILTRANS 13.06.07 SIS <<
}
// SUPFILTRANS 24.05.07 SIS <<
