



#include "stdafx.h"
#include "SettingsDialogs.h"
#include "Settings.h"
#include "CECompMan.h"
#include "CEProjInfo.h"
#include "shlobj.h"
#include "CEMain.h"
#include "utilif.h"
#include "MainFrm.h"
#include "PouVarSelCtrlWrapper.h"

/////////////////////////////////////////////////////////////////////////////
// CLibrarySettingsDialog dialog


CLibrarySettingsDialog::CLibrarySettingsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CLibrarySettingsDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLibrarySettingsDialog)
	m_strLibName = _T("");
	m_strLibPath = _T("");
	m_bStandardLibPath = FALSE;
	m_iTargetType = -1;
	//}}AFX_DATA_INIT

    m_validate = TRUE;
    m_strPathValidated = _T("");
    m_strTarget = _T("");

    CCEMAINApp* pApp = dynamic_cast<CCEMAINApp *> (AfxGetApp());
    ASSERT(pApp);

    CSettings* pSettings = pApp->GetSettings();
    if (!pSettings)
    {
        return;
    }

    m_strLibName = pSettings->m_librarySettings.m_strLibName;
    m_strLibPath = pSettings->m_librarySettings.m_strLibPath;
    m_bStandardLibPath = pSettings->m_librarySettings.m_bStandardLibPath;
    m_strTarget = pSettings->m_librarySettings.m_strTargetType;

    // get all installed targets
    GetAllTargets();

    
}


CLibrarySettingsDialog::~CLibrarySettingsDialog()
{
}


BOOL CLibrarySettingsDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

    m_iTargetType = -1;
	
    for (int i=0; i<=m_arrTargetDisplayInfo.GetUpperBound(); i++)
    {
        CTargetDisplayInfo targetdisplay = m_arrTargetDisplayInfo.GetAt(i);
        CString str = targetdisplay.strTargetName;
        m_cbTarget.AddString(str);
        if (targetdisplay.strTargetType.CompareNoCase(m_strTarget)==0)
        {
            m_iTargetType = i;
        }
    }

    if(m_iTargetType >= 0)
    {
        m_cbTarget.SetCurSel(m_iTargetType);
    }
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CLibrarySettingsDialog::GetAllTargets()
{
    m_arrTargetDisplayInfo.RemoveAll();

    CComQIPtr<ICEProjInfo>  pICEProjectInfo;
    CComPtr<IUnknown>       pTmpInfo;
    HRESULT hr;

    pTmpInfo.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
    pICEProjectInfo = pTmpInfo;
    ASSERT(pICEProjectInfo!=NULL);
    if(pICEProjectInfo==NULL)
    { 
        return;
    }

    CComQIPtr<ITypes> pTypes;
    CComPtr<IUnknown> pTmpInfoKad;

    pTmpInfoKad.Attach(CEGetCOM(CE_COMPMAN_KADMAN));
    pTypes = pTmpInfoKad;
    ASSERT(pTypes!=NULL);
    if (pTypes==NULL)
    {
        return;
    }


    SAFEARRAY* psaTargets;
    hr = pICEProjectInfo->getAllTargetTypes(&psaTargets);
    ASSERT(hr==S_OK);
    if (hr != S_OK)
    {
        TRACE("Target not available from project info.\n");
        return;
    }

    BSTR HUGEP *pbstr;
    BSTR bstr;
    unsigned long i;

    // Get a pointer to the elements of the array.
    hr = ::SafeArrayAccessData(psaTargets, (void HUGEP**)&pbstr);
    ASSERT(hr == S_OK);
    if (FAILED(hr))
    {
        return;
    }
    for (i = 0; i < ((psaTargets)->rgsabound[0]).cElements; i++)
    {
        CString str;
        bstr = pbstr[i];
        str = bstr;
        //m_arrTargets.SetAtGrow(i, str);
        CTargetDisplayInfo targetdisplay;
        targetdisplay.strTargetType = str;
        // get display string
        CComPtr<IType> pType;
        CComBSTR sId = str;
        hr = pTypes->GetTypeById(sId, &pType);
        if (hr == S_OK)
        {
            CComBSTR sText;
            hr = pType->GetText(&sText);
            if (hr == S_OK)
            {
                targetdisplay.strTargetName = sText;
            }
        }
        m_arrTargetDisplayInfo.SetAtGrow(i, targetdisplay);
    }

    ::SafeArrayUnaccessData(psaTargets);		
    ::SafeArrayDestroy(psaTargets);
}



void CLibrarySettingsDialog::DoDataExchange(CDataExchange* pDX)
{
    CFileStatus FileStatus;
    CString     strHelp;

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLibrarySettingsDialog)
	DDX_Control(pDX, IDC_LIB_TARGET, m_cbTarget);
	DDX_Text(pDX, IDC_LIB_NAME, m_strLibName);
	DDX_Text(pDX, IDC_LIB_PATH, m_strLibPath);
	DDX_Check(pDX, IDC_LIB_STANDARD_PATH, m_bStandardLibPath);
	DDX_CBIndex(pDX, IDC_LIB_TARGET, m_iTargetType);
	//}}AFX_DATA_MAP

    if (m_bStandardLibPath)
    {
        GetDlgItem(IDC_LIB_PATH)->EnableWindow(FALSE);
        GetDlgItem(IDC_NEW_BT_LIBRARY_PATH)->EnableWindow(FALSE);
    }
    else
    {
        GetDlgItem(IDC_LIB_PATH)->EnableWindow(TRUE);
        GetDlgItem(IDC_NEW_BT_LIBRARY_PATH)->EnableWindow(TRUE);
    }
    

    if (pDX->m_bSaveAndValidate && m_validate){
        if (!m_bStandardLibPath)
        {
            if(//m_strLibPath.Compare(m_pLibraryOptions->m_strLibPath)!=0 &&
                m_strLibPath.Compare(m_strPathValidated)!=0 &&
               !CFile::GetStatus(m_strLibPath,FileStatus)) {
                strHelp.Format(IDS_LIB_PATH_NOT_FOUND, m_strLibPath);
                if (AfxMessageBox(strHelp, MB_YESNO) != IDYES)
                {
                    pDX->Fail();
                }
                else
                {
                    m_strPathValidated = m_strLibPath;
                }
            }
        }
    }
}


BEGIN_MESSAGE_MAP(CLibrarySettingsDialog, CDialog)
	//{{AFX_MSG_MAP(CLibrarySettingsDialog)
	ON_BN_CLICKED(IDC_NEW_BT_LIBRARY_PATH, OnNewBtLibraryPath)
	ON_BN_CLICKED(IDC_LIB_STANDARD_PATH, OnLibStandardPath)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLibrarySettingsDialog message handlers


void CLibrarySettingsDialog::OnOK() 
{
    UpdateData(TRUE);

    CCEMAINApp* pApp = dynamic_cast<CCEMAINApp *> (AfxGetApp());
    ASSERT(pApp);

    CSettings* pSettings = pApp->GetSettings();
    if (!pSettings)
    {
        return;
    }

    m_strLibName.TrimLeft();
    m_strLibName.TrimRight();
    m_strLibPath.TrimLeft();
    m_strLibPath.TrimRight();

    pSettings->m_librarySettings.m_strLibName = m_strLibName;
    pSettings->m_librarySettings.m_strLibPath = m_strLibPath;
    pSettings->m_librarySettings.m_bStandardLibPath = m_bStandardLibPath;
    
    int i = m_cbTarget.GetCurSel();
    ASSERT(i<=m_arrTargetDisplayInfo.GetUpperBound());
    if (i>=0)
    {
        CTargetDisplayInfo targetdisplay;
        targetdisplay = m_arrTargetDisplayInfo.GetAt(i);
        m_strTarget = targetdisplay.strTargetType;
        pSettings->m_librarySettings.m_strTargetType = m_strTarget;	
    }

	CDialog::OnOK();
}




int CALLBACK BrowseLibraryPathCallbackProc(HWND hwnd,UINT uMsg,LPARAM lp, LPARAM pData) 
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

BOOL GetCurrentPath(CString & StrPath)
{
    LPTSTR lpBuffer = new TCHAR[_MAX_PATH];
    if(!lpBuffer) {
        //TODO assertion or message
        return FALSE;
    }

    GetCurrentDirectory(_MAX_PATH-1,lpBuffer);
    StrPath = lpBuffer;

    delete [] lpBuffer;
    return TRUE;
}


void CLibrarySettingsDialog::OnNewBtLibraryPath() 
{
    LPMALLOC pMalloc;
    BROWSEINFO bi;
    LPITEMIDLIST pidl;
    LPSHELLFOLDER pDesktopFolder;
    TCHAR         szPath[MAX_PATH];
    OLECHAR       olePath[MAX_PATH];
    ULONG         chEaten;
    ULONG         dwAttributes;
    HRESULT       hr;
    CFileStatus   FileStatus;

    m_validate = FALSE;
    UpdateData(TRUE);
    m_validate = TRUE;
    lstrcpy(szPath, m_strLibPath);

    /*  IMPORTANT NOTE: Do not forget that the path may not be existent,
        SHBrowseForFolder reacts very vexing to this fact. So assure that the
        path is valid and exists.
    */

    
    if(!CFile::GetStatus(m_strLibPath,FileStatus)) {
        CString strPath;    
        GetCurrentPath(strPath);
        lstrcpy(szPath,strPath);
    }
    

    //
    // Get a pointer to the Desktop's IShellFolder interface.
    //
    hr = SHGetDesktopFolder(&pDesktopFolder);

    if (FAILED(hr)) {
        UTIL_DisplaySysError(hr, NULL, NULL);
        return;
    }

    //
    // IShellFolder::ParseDisplayName requires the file name be in
    // Unicode.
    //
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szPath, -1,
                        olePath, MAX_PATH);

    //
    // Convert the path to an ITEMIDLIST.
    //
    hr = pDesktopFolder->ParseDisplayName(NULL,
                                          NULL,
                                          olePath,
                                          &chEaten,
                                          &pidl,
                                          &dwAttributes);

    //release the desktop folder object
    pDesktopFolder->Release();

    if (FAILED(hr)){
        UTIL_DisplaySysError(hr, NULL, NULL);
        return;
    }

    //
    // pidl now contains a pointer to an ITEMIDLIST for .\readme.txt.
    // This ITEMIDLIST needs to be freed using the IMalloc allocator
    // returned from SHGetMalloc().
    //
    hr = SHGetMalloc(&pMalloc);

    if(FAILED(hr)) {
        UTIL_DisplaySysError(hr, NULL, NULL);
        return;
    }

    ZeroMemory(&bi,sizeof(bi));
    bi.hwndOwner = NULL;
    bi.pidlRoot = NULL;
    bi.pszDisplayName = NULL;
    bi.lpszTitle = "";
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_VALIDATE;
    bi.lpfn = BrowseLibraryPathCallbackProc;
    bi.lParam = (LPARAM)szPath;
    EnableWindow(FALSE);  // disable dialog
    pidl = SHBrowseForFolder(&bi);
    EnableWindow(TRUE);   // enable dialog
    SetFocus();           // and set back focus to dialog
    if (pidl) {
        SHGetPathFromIDList(pidl, szPath);
        pMalloc->Free(pidl);
        pMalloc->Release();
        m_strLibPath = szPath;
        UpdateData(FALSE);
    }	
}



void CLibrarySettingsDialog::OnLibStandardPath() 
{
    if (IsDlgButtonChecked(IDC_LIB_STANDARD_PATH))
    {
        GetDlgItem(IDC_LIB_PATH)->EnableWindow(FALSE);
        GetDlgItem(IDC_NEW_BT_LIBRARY_PATH)->EnableWindow(FALSE);
        
        // get standard library path
        CString strStdLibPath;

        CComQIPtr<ICEProjInfo>  pICEProjectInfo;
        CComPtr<IUnknown>       pTmpInfo;
        HRESULT hr;

        pTmpInfo.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
        pICEProjectInfo = pTmpInfo;
        ASSERT(pICEProjectInfo!=NULL);
        if(pICEProjectInfo!=NULL)
        { 
            CComBSTR sLibPath;
            hr = pICEProjectInfo->getLibraryPath(&sLibPath);
            ASSERT(hr==S_OK);
            if (hr == S_OK)
            {
                strStdLibPath = sLibPath;
            }
        }

        GetDlgItem(IDC_LIB_PATH)->SetWindowText(strStdLibPath);
    }
    else
    {
        GetDlgItem(IDC_LIB_PATH)->EnableWindow(TRUE);
        GetDlgItem(IDC_NEW_BT_LIBRARY_PATH)->EnableWindow(TRUE);
    }	
}







/////////////////////////////////////////////////////////////////////////////
// CPasswordSettingsDialog dialog


CPasswordSettingsDialog::CPasswordSettingsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPasswordSettingsDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPasswordSettingsDialog)
	m_strFullPwd = _T("");
	m_strReadPwd = _T("");
	//}}AFX_DATA_INIT

    InitValues();
}


void CPasswordSettingsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPasswordSettingsDialog)
	DDX_Text(pDX, IDC_SET_PWD_FULL, m_strFullPwd);
	DDX_Text(pDX, IDC_SET_PWD_READ, m_strReadPwd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPasswordSettingsDialog, CDialog)
	//{{AFX_MSG_MAP(CPasswordSettingsDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPasswordSettingsDialog message handlers


void CPasswordSettingsDialog::InitValues() 
{
    CComQIPtr<ICEProjInfo>  pICEProjectInfo;
    CComPtr<IUnknown>       pTmpInfo;
    HRESULT hr;

    pTmpInfo.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
    pICEProjectInfo = pTmpInfo;
    ASSERT(pICEProjectInfo!=NULL);
    if(pICEProjectInfo!=NULL)
    { 
        CComBSTR sSection = _T("Attributes");
        CComBSTR sKeyRead = _T("password.read");
        CComBSTR sKeyFull = _T("password.write");
        CComBSTR sValueRead;
        CComBSTR sValueFull;
        hr = pICEProjectInfo->CPGetValueForKeyFromSection(sSection,
                                                          sKeyRead,
                                                          &sValueRead);
        if (hr==S_OK)
        {
            m_strReadPwd = sValueRead;
        }
        hr = pICEProjectInfo->CPGetValueForKeyFromSection(sSection,
                                                          sKeyFull,
                                                          &sValueFull);
        if (hr==S_OK)
        {
            m_strFullPwd = sValueFull;
        }

    }

	return; 
}


void CPasswordSettingsDialog::OnOK() 
{
    UpdateData(TRUE);

    CComQIPtr<ICEProjInfo>  pICEProjectInfo;
    CComPtr<IUnknown>       pTmpInfo;
    HRESULT hr;

    if (!m_strReadPwd.IsEmpty() && m_strFullPwd.IsEmpty())
    {
        CString str;
        str.LoadString(IDS_SET_RWPWD_NO_RWPWD);
        ::AfxMessageBox(str, MB_OK|MB_ICONEXCLAMATION);
        return;
    }

    pTmpInfo.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
    pICEProjectInfo = pTmpInfo;
    ASSERT(pICEProjectInfo!=NULL);
    if(pICEProjectInfo!=NULL)
    { 
        CComBSTR sSection = _T("Attributes");
        CComBSTR sKeyRead = _T("password.read");
        CComBSTR sKeyFull = _T("password.write");
        CComBSTR sValueRead = m_strReadPwd;
        CComBSTR sValueFull = m_strFullPwd;

        hr = pICEProjectInfo->CPDeleteFromSection(sSection, sKeyRead);
        hr = pICEProjectInfo->CPDeleteFromSection(sSection, sKeyFull);

        hr = pICEProjectInfo->CPInsertToSection(sSection, sKeyRead, sValueRead);
        hr = pICEProjectInfo->CPInsertToSection(sSection, sKeyFull, sValueFull);
    }
    
   	CDialog::OnOK();
}




/////////////////////////////////////////////////////////////////////////////
// CEngineeringSettingsDialog dialog


CEngineeringSettingsDialog::CEngineeringSettingsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CEngineeringSettingsDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEngineeringSettingsDialog)
	m_bXRefDisabled = FALSE;
	m_bConsoleInfo = FALSE;
	//}}AFX_DATA_INIT

    InitValues();
}


void CEngineeringSettingsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEngineeringSettingsDialog)
	DDX_Check(pDX, IDC_SET_ENG_XREF, m_bXRefDisabled);
	DDX_Check(pDX, IDC_SET_ENG_CONSOLE_INF, m_bConsoleInfo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEngineeringSettingsDialog, CDialog)
	//{{AFX_MSG_MAP(CEngineeringSettingsDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEngineeringSettingsDialog message handlers



void CEngineeringSettingsDialog::InitValues() 
{
    CCEMAINApp* pApp = dynamic_cast<CCEMAINApp *> (AfxGetApp());
    ASSERT(pApp);

    CSettings* pSettings = pApp->GetSettings();
    if (pSettings)
    {
        m_bXRefDisabled = pSettings->m_engineeringSettings.m_bXRefDeactivated;
		m_bConsoleInfo = pSettings->m_engineeringSettings.m_bConsoleInfDeactivated;
    }

	return;
}



void CEngineeringSettingsDialog::OnOK() 
{
    UpdateData(TRUE);

    CCEMAINApp* pApp = dynamic_cast<CCEMAINApp *> (AfxGetApp());
    ASSERT(pApp);

    CSettings* pSettings = pApp->GetSettings();
    if (pSettings)
    {
        pSettings->m_engineeringSettings.m_bXRefDeactivated = m_bXRefDisabled;
		pSettings->m_engineeringSettings.m_bConsoleInfDeactivated = m_bConsoleInfo ;
        pSettings->m_engineeringSettings.SaveSettings();
    }
    
	CDialog::OnOK();
}


