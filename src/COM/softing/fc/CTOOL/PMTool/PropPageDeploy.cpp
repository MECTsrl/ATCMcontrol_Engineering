// PropPageDeploy.cpp : implementation file
//

#include "stdafx.h"
#include "PMTool.h"
#include "PropPageDeploy.h"
#include "MainPropertySheet.h"
#include "defs.h"
#include "resource.h"

#include "utilif.h"

// d-188 02.02.05 SIS >>
#include <winsvc.h>
// d-188 02.02.05 SIS <<

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropPageDeploy property page

IMPLEMENT_DYNCREATE(CPropPageDeploy, CPMPropPage)

CPropPageDeploy::CPropPageDeploy(CMainPropertySheet* pMainSheet /*=NULL*/, const CString& crstrProjectPath /*=_T("")*/)
 : CPMPropPage(CPropPageDeploy::IDD, pMainSheet)
{
	//{{AFX_DATA_INIT(CPropPageDeploy)
	m_strAddress = _T("");
	m_strProjectPath = crstrProjectPath;
	//}}AFX_DATA_INIT

    InitializeFromRegistry();
}

CPropPageDeploy::~CPropPageDeploy()
{
    SaveToRegistry();
}

void CPropPageDeploy::InitializeFromRegistry(BOOL bGeneral /*=FALSE*/)
{
    if(m_strProjectPath.IsEmpty())
    {
        m_strProjectPath    = AfxGetApp()->GetProfileString("Settings", "Deploy_ProjectPath");
    }
    m_strAddress        = AfxGetApp()->GetProfileString("Settings", "Deploy_Address");
    
    if(bGeneral || m_strProjectPath.IsEmpty())
    {
        m_strProjectPath = AfxGetApp()->GetProfileString("Settings", "ProjectPath");
    }
    if(bGeneral || m_strAddress.IsEmpty())
    {
        m_strAddress = AfxGetApp()->GetProfileString("Settings", "Address");
    }
}

void CPropPageDeploy::SaveToRegistry(BOOL bGeneral /*=FALSE*/)
{
    AfxGetApp()->WriteProfileString("Settings", "Deploy_ProjectPath", m_strProjectPath);
    AfxGetApp()->WriteProfileString("Settings", "Deploy_Address", m_strAddress);

    if(bGeneral)
    {
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


void CPropPageDeploy::DoDataExchange(CDataExchange* pDX)
{
	CPMPropPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageDeploy)
	DDX_Control(pDX, IDC_TARGET_LIST, m_tTargetListCtrl);
	DDX_Text(pDX, IDC_ADDRESS_EDIT, m_strAddress);
	DDX_Text(pDX, IDC_PROJECT_PATH_EDIT, m_strProjectPath);
	//}}AFX_DATA_MAP
}

CString CPropPageDeploy::GetProjectPath()
{
    return m_strProjectPath;
}

BEGIN_MESSAGE_MAP(CPropPageDeploy, CPMPropPage)
	//{{AFX_MSG_MAP(CPropPageDeploy)
	ON_BN_CLICKED(IDC_DEPLOY_BTN, OnDeployBtn)
	ON_BN_CLICKED(IDC_PROJECT_PATH_BTN, OnProjectPathBtn)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TARGET_LIST, OnItemchangedTargetList)
	ON_BN_CLICKED(IDC_REFRESH_LIST_BTN, OnRefreshListBtn)
	ON_BN_CLICKED(IDC_SET_ADDRESS_BTN, OnSetAddressBtn)
	ON_EN_CHANGE(IDC_ADDRESS_EDIT, OnChangeAddressEdit)
	ON_EN_CHANGE(IDC_PROJECT_PATH_EDIT, OnChangeProjectPathEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropPageDeploy message handlers

void CPropPageDeploy::OnDeployBtn() 
{
    m_pMainSheet->SetStatusText(_T("Deploying ..."));

    UpdateData();

    if(!CheckData())
    {
        return;
    }

    if(Deploy())
    {
        m_pMainSheet->SetStatusText(_T("Deploy Done"));
    }

    LoadTargetInfoList(m_strProjectPath, TRUE);
    FillTargetListControl();
}

BOOL CPropPageDeploy::CheckData()
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
    POSITION        pos = m_tTargetInfoList.GetHeadPosition();
    TTargetInfo*    pTargetInfo;
    int             iTargetCount = 0;
    while(pos)
    {
        pTargetInfo = m_tTargetInfoList.GetNext(pos);
        if(!pTargetInfo->strTargetNewAddress.IsEmpty())
        {
            ++iTargetCount;
        }
    }

    if(iTargetCount == 0)
    {
        CString strMessage;
        strMessage.LoadString(IDS_NO_TARGETS);
        ::AfxMessageBox(strMessage);
        //m_pMainSheet->SetStatusText(strMessage);
        CWnd*   pWnd = GetDlgItem(IDC_TARGET_LIST);
        GotoDlgCtrl(pWnd);
        return FALSE;
    }
    return TRUE;
}

// d-188 02.02.05 SIS >>
BOOL CPropPageDeploy::IsWindowsNT(void)
{
    OSVERSIONINFO VI;

    VI.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&VI);

    return VI.dwPlatformId == VER_PLATFORM_WIN32_NT;
}


/* IsAdmin
 * ----------------------------------------------------------------------------
 */
BOOL CPropPageDeploy::IsAdmin(void)
{
  SC_HANDLE hSC;

  if (IsWindowsNT())
  {
      hSC = OpenSCManager(NULL, NULL, GENERIC_READ | GENERIC_WRITE | GENERIC_EXECUTE);
      if (!hSC)
      {
        return FALSE;
      }

      CloseServiceHandle(hSC);
  }

  return TRUE;
}
// d-188 02.02.05 SIS <<


BOOL CPropPageDeploy::Deploy()
{
    BOOL    bReturn = FALSE;

	// d-188 02.02.05 SIS >>
	if(!IsAdmin())
	{
		AfxMessageBox(IDS_NO_ADMINRIGHTS);
		return FALSE;
	}
	// d-188 02.02.05 SIS <<

    // get installation path: install path + Engineering\bin
    CString strCmdLine;
    CString strInstallPath;
    size_t sizeInstDir = MAX_PATH;
    if(S_OK != UTIL_GetInstallPath(strInstallPath.GetBuffer(MAX_PATH), &sizeInstDir))
    {
        return bReturn;
    }
    strInstallPath.ReleaseBuffer();
    strInstallPath += _T("Engineering\\bin");
    
    // build command line
    strCmdLine = _T("\"") + strInstallPath + _T("\\4cdeploy.exe\" -s ") + _T("\"") + m_strProjectPath + _T("\"") + 
                 _T(" -patch ");

    POSITION        pos = m_tTargetInfoList.GetHeadPosition();
    CString         strTargetString;
    CString         strTmp;
    TTargetInfo*    pTargetInfo;
    while(pos)
    {
        pTargetInfo = m_tTargetInfoList.GetNext(pos);
        if(!pTargetInfo->strTargetNewAddress.IsEmpty())
        {
			CString strAddress;
			strAddress.Format(_T("%s(%s)"), m_pMainSheet->GetCommChannel(), pTargetInfo->strTargetNewAddress);
            strTmp.Format(_T("%s:%s"), (LPCTSTR)pTargetInfo->strTargetName, (LPCTSTR)strAddress);
            if(!strTargetString.IsEmpty())
            {
                strTargetString += _T(" ");
            }
            strTargetString += strTmp;
        }
    }

    if(strTargetString.IsEmpty())
    {
        m_pMainSheet->SetStatusText(_T("No target to deploy"));
        return bReturn;
    }

    strCmdLine += strTargetString;

    // execute command
    TRACE("%s", strCmdLine);
    if(ExecuteCommand(strCmdLine) == ST_OK)
    {
        bReturn = TRUE;
    }
    return bReturn;
}

void CPropPageDeploy::OnProjectPathBtn() 
{
    CString strProjectPath(m_strProjectPath);
    if(strProjectPath.IsEmpty())
    {
        strProjectPath = GetStandardProjectPath();
    }

	CFolderDialog fd(strProjectPath, "Browse for Project folder");

	int iRet = fd.DoModal();
    CString strProjFileTmp;

	switch(iRet)
	{
	case IDOK:
		m_strProjectPath = fd.m_sSelPath;
        switch(FindProjectFile(m_strProjectPath, strProjFileTmp))
        {
        case ST_OK:
            LoadProjectTargetInfo(m_strProjectPath);
            break;
        case ST_ERROR:
            {
                CString strMessage;
                strMessage.LoadString(IDS_NOPROJECTFILE);
                ::AfxMessageBox(strMessage, MB_OK | MB_ICONEXCLAMATION);
            }
            break;
        case ST_MULTIPLE_PROJECT_FILES:
            {
                CString strMessage;
                strMessage.LoadString(IDS_WARN_MULTIPLE_PROJECTFILES);
                ::AfxMessageBox(strMessage, MB_OK | MB_ICONASTERISK);
            }
            break;
        }
		UpdateData(FALSE);
		break;
		
	default:
		break;
	}
    LoadTargetInfoList(m_strProjectPath, TRUE);
    FillTargetListControl();
}

void CPropPageDeploy::OnItemchangedTargetList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	*pResult = 0;
}

void CPropPageDeploy::OnRefreshListBtn() 
{
    LoadTargetInfoList(m_strProjectPath, TRUE);
    FillTargetListControl();
}

void CPropPageDeploy::OnSetAddressBtn() 
{
    int iCurSel = m_tTargetListCtrl.GetNextItem(-1, LVNI_SELECTED);
    if(iCurSel < 0)
    {
        if(m_tTargetListCtrl.GetItemCount() == 1)
        {
            iCurSel = 0;
        }
    }
    if(iCurSel >= 0)
    {
        TTargetInfo* pTargetInfo = (TTargetInfo*)m_tTargetListCtrl.GetItemData(iCurSel);
        pTargetInfo->strTargetNewAddress = m_strAddress;
        m_tTargetListCtrl.SetItemText(iCurSel, COL_ADDRESS_NEW, m_strAddress);
        if(iCurSel < m_tTargetListCtrl.GetItemCount()-1)
        {
            m_tTargetListCtrl.SetItemState(iCurSel, 0, LVIS_SELECTED);
            m_tTargetListCtrl.SetItemState(iCurSel+1, LVIS_SELECTED, LVIS_SELECTED);
        }
    }
    else
    {
        ::AfxMessageBox(_T("No target selected"));
    }
}

BOOL CPropPageDeploy::OnInitDialog() 
{
	CPMPropPage::OnInitDialog();
    CString     strColName;

    CRect   tRect;
    m_tTargetListCtrl.GetClientRect(&tRect);
	LV_COLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lvc.fmt = LVCFMT_LEFT;
	lvc.pszText = "Name";
	lvc.cx = tRect.Width()/3;
    m_tTargetListCtrl.InsertColumn(COL_TARGET_NAME, &lvc);

	lvc.pszText = "Address";
    m_tTargetListCtrl.InsertColumn(COL_ADDRESS_OLD, &lvc);

	lvc.pszText = "New Address";
    m_tTargetListCtrl.InsertColumn(COL_ADDRESS_NEW, &lvc);

    if(!m_strProjectPath.IsEmpty())
    {
        TryLoadTargetInfo(m_strProjectPath);
        FillTargetListControl();
    }

    CPMPropPage::OnCommSettingsChanged();    // to update connect info

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CPropPageDeploy::FillTargetListControl()
{
    m_tTargetListCtrl.DeleteAllItems();

    POSITION    pos = m_tTargetInfoList.GetHeadPosition();
    TTargetInfo*    pTargetInfo;
    int             iItem = 0;
    int             iItemNew;   // new index of item in sorted list
    while(pos)
    {
        pTargetInfo = m_tTargetInfoList.GetNext(pos);
        iItemNew = m_tTargetListCtrl.InsertItem(iItem, pTargetInfo->strTargetName);

        ASSERT(iItemNew != -1);
        
        if(iItemNew != -1)
        {
            m_tTargetListCtrl.SetItemText(iItemNew, COL_ADDRESS_OLD, pTargetInfo->strTargetAddress);
            m_tTargetListCtrl.SetItemText(iItemNew, COL_ADDRESS_NEW, pTargetInfo->strTargetNewAddress);
            m_tTargetListCtrl.SetItemData(iItemNew, (DWORD)pTargetInfo);
        }
        ++iItem;
    }
    if(iItem > 0)
    {
        m_tTargetListCtrl.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
    }
}

void CPropPageDeploy::OnChangeAddressEdit() 
{
    UpdateData();
}

void CPropPageDeploy::OnChangeProjectPathEdit() 
{
    UpdateData();
    
    TryLoadTargetInfo(m_strProjectPath);
    FillTargetListControl();
}

BOOL CPropPageDeploy::OnKillActive() 
{
    UpdateData();

    SaveToRegistry(TRUE);

	return CPMPropPage::OnKillActive();
}

BOOL CPropPageDeploy::OnSetActive() 
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

    LoadTargetInfoList(m_strProjectPath, TRUE);
    FillTargetListControl();

	return CPMPropPage::OnSetActive();
}
