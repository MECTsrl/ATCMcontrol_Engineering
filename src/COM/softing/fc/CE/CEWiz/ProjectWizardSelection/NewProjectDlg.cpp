#include "stdafx.h"

#include "ProjectWizardSelectionApp.h"
#include "CEProjWiz.h"
#include "NewProjectDlg.h"
#include "WizardList.h"
#include "extern.h"
#include "error.h"
#include "utilif.h"

BEGIN_MESSAGE_MAP(CNewProjectDlg, CDialog)
	ON_BN_CLICKED(IDC_BROWSE_PATH, OnBrowseDirectory)
	ON_EN_CHANGE(IDC_PROJECT_NAME, OnChangedProjectName)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_WIZARD_LIST, OnChangedWizardList)
	ON_BN_CLICKED(IDOK, OnOk)
END_MESSAGE_MAP()

CNewProjectDlg::CNewProjectDlg(CWnd *pParent) :
    CDialog(CNewProjectDlg::IDD, pParent)
// creates the dialog where new projects can be created
{
    HRESULT hr;

    m_sProjectPath = _T("");
    m_bWizardSelected = FALSE;
    //Using GetInstallPath + Projects to get the ProjectPath
    TCHAR pszInstPath[1000];   
    size_t sizeChars = 1000;
    hr = UTIL_GetInstallPath(pszInstPath, &sizeChars);
    m_sProjectPath = CString(pszInstPath) + _T("Projects");
    //m_sProjectPath.ReleaseBuffer();
    if (hr != S_OK) 
    {
        m_sProjectPath = _T("");
    }
    m_hrDialogState = E_FAIL;
}

BOOL CNewProjectDlg::OnInitDialog() 
// initializes the dialog, fills the project wizard list
// and sets the default path
{
    HWND hwnd;
    CRect rect;
    CString str;
    LV_ITEM item;
    LV_COLUMN column;
    int iPosition;
    CWizardListEntry *pWizardListEntry;

    CDialog::OnInitDialog();
    m_imageList.Create(IDB_WIZARD_IMAGE_LIST,20,0,RGB(255,0,255));
    m_imageList.SetBkColor(::GetSysColor(COLOR_WINDOW));
    m_wizardListCtrl.SetImageList(&m_imageList,LVSIL_SMALL);
	(GetDlgItem(IDOK))->EnableWindow(FALSE);
    hwnd = m_wizardListCtrl.GetSafeHwnd();
    if(hwnd != NULL) {
        m_wizardListCtrl.GetClientRect(rect);
        column.mask = LVCF_FMT;
        column.fmt = LVCFMT_LEFT;
        m_wizardListCtrl.InsertColumn(0,&column);
        m_wizardListCtrl.SetColumnWidth(0,rect.Width());
        m_wizardList.Init();
        iPosition = 0;
        pWizardListEntry = m_wizardList.GetFirst();
        while (pWizardListEntry != NULL) {
            m_wizardListCtrl.InsertItem(iPosition,
                pWizardListEntry->GetName(),0);
            pWizardListEntry = m_wizardList.GetNext();
            iPosition++;
        }
        if(m_wizardListCtrl.GetItemCount()) {
            item.mask = LVIF_STATE;
            item.iItem= 0;
            item.iSubItem = 0;
            item.state = LVIS_SELECTED|LVIS_FOCUSED;
            item.stateMask = LVIS_SELECTED|LVIS_FOCUSED;
            m_wizardListCtrl.SetItem(&item);
        }
        UpdateData(FALSE);
    }
    return TRUE;
}

void CNewProjectDlg::DoDataExchange(CDataExchange *pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WIZARD_LIST, m_wizardListCtrl);
	DDX_Text(pDX, IDC_PROJECT_PATH, m_sProjectPath);
	DDX_Text(pDX, IDC_PROJECT_NAME, m_sProjectName);
}

BOOL CNewProjectDlg::DirectoryExists()
// checks whether the specified directory exists
{
    CFileStatus fileStatus;

    return CFile::GetStatus(m_sProjectPath,fileStatus); 
}

void CNewProjectDlg::EnsureDirectoryExists()
// checks whether the specified directory exists
// if it does not exist change to the current directory
{
    if (! DirectoryExists()) {
        GetCurrentDirectory(MAX_PATH,m_sProjectPath.GetBuffer(MAX_PATH));
        m_sProjectPath.ReleaseBuffer();
    }
}

int CALLBACK BrowsePathCallbackProc(HWND hwnd,UINT uMsg,LPARAM lp, LPARAM pData) 
{
    switch(uMsg) 
    {
        case BFFM_INITIALIZED: 
        {
            LPTSTR pszDir = (LPTSTR)pData;
            SendMessage(hwnd,BFFM_SETSELECTION,TRUE,(LPARAM)pszDir);
            break;
        }
        default:
            break;
    }

    return 0;
}

void CNewProjectDlg::OnBrowseDirectory()
// open a dialog to browse for the directory where the project will
// be created
{
    HRESULT hr;
    OLECHAR oleInitialPath[MAX_PATH];
    TCHAR sNewPath[MAX_PATH];
    ULONG chEaten;
    LPMALLOC pMalloc;
    ULONG dwAttributes;
    BROWSEINFO browseInfo;
    LPSHELLFOLDER pDesktopFolder;
    LPITEMIDLIST pItemList;
    TCHAR           szInitialPath[MAX_PATH];

    UpdateData(TRUE);
    EnsureDirectoryExists();
    hr = SHGetDesktopFolder(&pDesktopFolder);
    if (SUCCEEDED(hr)) {
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, m_sProjectPath, -1,
                        oleInitialPath, MAX_PATH);
        lstrcpy(szInitialPath, m_sProjectPath);
        hr = pDesktopFolder->ParseDisplayName(NULL,NULL,oleInitialPath,
            &chEaten,&pItemList,&dwAttributes);
        if (SUCCEEDED(hr)) {
            hr = SHGetMalloc(&pMalloc);
            if (SUCCEEDED(hr)) {
                ZeroMemory(&browseInfo,sizeof(browseInfo));
                browseInfo.hwndOwner = NULL;
                browseInfo.pidlRoot = NULL;
                browseInfo.pszDisplayName = NULL;
                browseInfo.lpszTitle = "";
                browseInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_VALIDATE;
                browseInfo.lpfn = BrowsePathCallbackProc;
                browseInfo.lParam = (LPARAM)szInitialPath;

                EnableWindow(FALSE);
                pItemList = SHBrowseForFolder(&browseInfo);
                EnableWindow(TRUE);
                SetFocus();
                if (pItemList != NULL) {
                    SHGetPathFromIDList(pItemList,sNewPath);
                    pMalloc->Free(pItemList);
                    pMalloc->Release();
                    m_sProjectPath = sNewPath;
                    UpdateData(FALSE);
                }
            }
        }
        pDesktopFolder->Release();
    }
    if (FAILED(hr)) {
        UTIL_DisplaySysError(hr,NULL,NULL);
    }
}



void CNewProjectDlg::OnChangedProjectName()
// enables / disables the OK button depending on whether 
// the project name is valid and a wizard was selected
{
    BOOL bEnabled;
    
    if (m_bWizardSelected) {
        bEnabled = IsProjectNameValid();
    } else {
        bEnabled = FALSE;
    }
	GetDlgItem(IDOK)->EnableWindow(bEnabled);
}

void CNewProjectDlg::OnChangedWizardList(NMHDR *pNMHDR, LRESULT *pResult)
// sets the flag indication whether or not a wizard is selected
// from the list
{
	NM_LISTVIEW *pListView;
    
    pListView = (NM_LISTVIEW *) pNMHDR;
	if (pListView->uNewState&LVIS_SELECTED) {
		m_bWizardSelected = TRUE;
        m_iSelectedWizardItem = pListView->iItem;
    } else {
		m_bWizardSelected = FALSE;
	}
	*pResult = 0;
    //OH: bug fix
    OnChangedProjectName();
}

BOOL CNewProjectDlg::IsProjectNameValid()
// checks for illegal characters within the project name
{
    BOOL bResult;
    CString sIllegalChars;
    
    UpdateData();
    bResult = FALSE;
    if (! m_sProjectName.IsEmpty()) {
        sIllegalChars.LoadString(IDS_PROJECT_NAME_INVALID_CHARS);
        if (! m_sProjectName.IsEmpty()) {
            bResult = (m_sProjectName.FindOneOf(sIllegalChars) == -1);
        }
    }
    return bResult;
}


void CNewProjectDlg::ExecuteWizard(CLSID classId)
// executes the project wizard
{
    CComPtr<ICEProjectWizard> pProjectWizard;
    CComBSTR sFileName;

    m_hrDialogState = CoCreateInstance (classId, NULL, CLSCTX_INPROC_SERVER, IID_ICEProjectWizard, (LPVOID *)&pProjectWizard);
    if (FAILED (m_hrDialogState))
    {
        ErrorMessage (EC_FAILED_TO_EXECUTE_WIZARD);
        return;
    };

    // Execute the project wizard.
    // Returns S_OK if project is successfully created.
    // Returns S_FALSE if user cancelled project creation
    // Returns a failure code if project could not be created
    //-------------------------------------------------------

    m_hrDialogState = pProjectWizard->ExecuteProjectWizard (CComBSTR (m_sProjectPath), CComBSTR (m_sProjectName), &sFileName);

    m_strFileName = m_sProjectPath + _T ("\\") + m_sProjectName + _T ("\\") + m_sProjectName + _T (".4cp");


    // Show error message if project could not be created
    // but not if user cancelled project creation.
    //---------------------------------------------------

    if (FAILED (m_hrDialogState))
    {
        // check for ERROR_CANCELLED
        if (HRESULT_CODE(m_hrDialogState)==ERROR_CANCELLED)
        {
            return;
        }
        ErrorMessage (EC_FAILED_TO_EXECUTE_WIZARD);
    }
}

void CNewProjectDlg::OnOk() 
// searches the class ID of the chosen wizard and executes the wizard
{
    UpdateData(TRUE);

    // Check, if identifier is IEC compatible
    HRESULT hr = UTIL_CheckIdentifier(m_sProjectName);
    if(hr != S_OK)
    {
        CString strMessage;
        switch(hr)
        {
        case E_UTIL_ID_SYNTAX:
            strMessage.LoadString(IDS_ERR_SYNTAX_INCORRECT);
            break;
        case E_UTIL_ID_IS_KEYWORD:
            strMessage.LoadString(IDS_ERR_IS_KEYWORD);
            break;
        case E_UTIL_ID_TOO_LONG:
            strMessage.Format(IDS_ERR_TOO_LONG, UTIL_MAX_ID_LENGTH);
            break;
        }
        ::AfxMessageBox(strMessage, MB_OK|MB_ICONERROR);
        return;
    }

    CString strNewProjDir = m_sProjectPath + _T ("\\") + m_sProjectName;
    m_strFileName = strNewProjDir + _T ("\\") + m_sProjectName + _T (".4cp");

    //Check if the given directory already exists
    CFileStatus tStatus;
    if(CFile::GetStatus(strNewProjDir, tStatus))
    {
		// directory does already exist
        CString errorMsg = _T("Directory \"") + strNewProjDir + _T("\" already exists");
        ::AfxMessageBox(errorMsg, MB_OK|MB_ICONWARNING);

        return;
    }

    CString sSelectedWizard;
    CWizardListEntry *pWizardListEntry;

    sSelectedWizard = m_wizardListCtrl.GetItemText(m_iSelectedWizardItem,0);
    pWizardListEntry = m_wizardList.GetEntry(sSelectedWizard);
    ASSERT(pWizardListEntry != NULL);
    ExecuteWizard(pWizardListEntry->GetClassId());
    CDialog::OnOK();
}
