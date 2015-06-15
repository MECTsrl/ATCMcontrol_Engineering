
//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "cedl.h"
#include "dlext.h"
#include "Downld.h"
#include "dlstat.h"
#include "dlrs.h"
#include "utilif.h"
#include "CEDLError.h"
#include "DLUpdateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//----  Local Defines:   -------------------------------------------*
#define DLG_BITMAP_WIDTH 16
#define IMG_UNCHANGED   0
#define IMG_DELETED     1
#define IMG_DOWNLOAD    2
#define IMG_NEW         3

/////////////////////////////////////////////////////////////////////////////
// CDLUpdateDlg dialog

int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort) {
    CString str1 = (LPCTSTR)lParam1;
    CString str2 = (LPCTSTR)lParam1;
    return(str1.Compare(str2));
}

CDLUpdateDlg::CDLUpdateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDLUpdateDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLUpdateDlg)
	m_strControlName = _T("");
	m_strDomainName = _T("");
	//}}AFX_DATA_INIT
    m_pDLEvents = NULL;
    m_pStation  = NULL;
	m_hSaveCursor = NULL;
	m_hwndCapture = NULL; 
    return;
}


void CDLUpdateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLUpdateDlg)
	DDX_Control(pDX, IDC_UPDATE_PROGRESS, m_DlProgressCtrl);
	DDX_Control(pDX, IDC_UPDATE_DOWNLOAD, m_DownloadBtn);
	DDX_Control(pDX, IDC_UPDATE_LIST, m_DomainListCtrl);
	DDX_Text(pDX, IDC_UPDATE_CONTROLNAME, m_strControlName);
	DDX_Text(pDX, IDC_UPDATE_DOMNAME, m_strDomainName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLUpdateDlg, CDialog)
	//{{AFX_MSG_MAP(CDLUpdateDlg)
	ON_COMMAND(IDM_DOMPOPUP_DEL, OnDompopupDel)
	ON_COMMAND(IDM_DOMPOPUP_DOWN, OnDompopupDown)
	ON_COMMAND(IDM_DOMPOPUP_NOTHING, OnDompopupNothing)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_UPDATE_LIST, OnColumnclickUpdateList)
	ON_BN_CLICKED(IDC_UPDATE_DOWNLOAD, OnUpdateDownload)
	ON_BN_CLICKED(IDC_UPDATE_SELECTALL, OnUpdateSelectall)
	ON_WM_CONTEXTMENU()
	ON_BN_CLICKED(IDOK, OnOK)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLUpdateDlg message handlers

BOOL CDLUpdateDlg::OnInitDialog()
{
    HRESULT hr = S_OK;
	CDialog::OnInitDialog();
	FC_DEBUGPRINT1(_T("CEDL> CDLUpdateDlg::OnInitDialog(): thread id=0x%x\n"),::GetCurrentThreadId());
	
    assert(m_pStation != NULL);
    //set the help id for context sensitive help
    SetWindowContextHelpId(IDD);
    // init the controls
    m_ImageList.Create(IDB_DOMIMAGELIST, DLG_BITMAP_WIDTH, 0, RGB(255, 0, 255));
	m_ImageList.SetBkColor(GetSysColor(COLOR_WINDOW));
	m_DomainListCtrl.SetImageList(&m_ImageList, LVSIL_SMALL);
    m_DomainListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP | LVS_EX_FULLROWSELECT);
    DisplayColumnHeadings(IDS_COL_DOMAINLIST);
    m_DownloadBtn.EnableWindow(FALSE);
    m_strControlName.Format(_T("%s [%s]"), m_pStation->GetConfigName(), m_pStation->GetAddress());
    ClearProgress();
    
    // start the preparation for DL
    assert(m_pDLEvents != NULL);
    m_pDLEvents->SetListClient(this->GetSafeHwnd());

    if (m_pStation->IsRequestPending()) {
		FC_DEBUGPRINT(_T("CEDL> OnInitDialog: Download Pending\n"));
        m_pStation->FireErrorMessage(E_CE_DL_PENDING);
        m_pStation->FireError(E_CE_DL_PENDING);
    	m_strDomainName.LoadString(IDS_CONN_INTR);
        UpdateData(FALSE);
        return TRUE;
    }

	m_strDomainName.LoadString(IDS_CONN);
    UpdateData(FALSE);

	m_hSaveCursor = ::SetCursor(::LoadCursor(NULL, IDC_APPSTARTING));
	m_hwndCapture = GetSafeHwnd();
	if (m_hwndCapture) {
      ::SetCapture(m_hwndCapture);
	}

    m_pStation->SetSignal(BUILD_DL_LIST);
    m_pStation->ResetSignal(DOWN_LD);
    m_pStation->ResetSignal(DOWN_LD_ALL);
    m_pStation->ResetSignal(PRJ_NEW);
    m_pStation->ResetSignal(PRJ_CHANGED);
    m_pStation->SetState(CEDL_STAT_WAIT_CONNECT);
	m_pStation->DoStateMachine();

	if(FAILED(hr)) {
		FC_DEBUGPRINT1(_T("CEDL> OnInitDialog: failed hr= 0x%x\n"),hr);
        m_pStation->FireErrorMessage(hr);
        m_pStation->FireError(hr);
	}
    return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDLUpdateDlg::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
    HRESULT hr = S_OK;

    switch(message) {
        case CEDL_MSG_DLLIST_READY:
			if(m_hSaveCursor) {
				::SetCursor(m_hSaveCursor);
				m_hSaveCursor = NULL;
			}
			
			if (m_hwndCapture) {
				::ReleaseCapture();
				m_hwndCapture = NULL;
			}

            FillListFromDB();
            UpdateState();
            m_DownloadBtn.EnableWindow(TRUE);
            return(TRUE);
        case CEDL_MSG_DOWNLOAD_READY:
            DisplayColumnHeadings(IDS_COL_DOMAINLIST);  // clear the old list
            if (m_pStation->IsRequestPending()) {
                m_pStation->FireErrorMessage(E_CE_DL_PENDING);
                m_pStation->FireError(E_CE_DL_PENDING);
                return TRUE;
            }
            m_pStation->SetSignal(BUILD_DL_LIST);
            m_pStation->ResetSignal(DOWN_LD);
            m_pStation->ResetSignal(DOWN_LD_ALL);
            m_pStation->ResetSignal(PRJ_NEW);
            m_pStation->ResetSignal(PRJ_CHANGED);
            //TODO CHECK
            m_pStation->SetState(CEDL_STAT_WAIT_CONNECT);
            m_pStation->DoStateMachine();
	        if(FAILED(hr)) {
		        FC_DEBUGPRINT1(_T("CEDL> OnWndMsg: failed hr= 0x%x\n"),hr);
                m_pStation->FireErrorMessage(hr);
                m_pStation->FireError(hr);
	        }
            return(TRUE);
        case CEDL_MSG_DOWNLOAD_ERROR:
			if(m_hSaveCursor) {
				::SetCursor(m_hSaveCursor);
				m_hSaveCursor = NULL;
			}
			if (m_hwndCapture) {
				::ReleaseCapture();
				m_hwndCapture = NULL;
			}
            return(TRUE);
    }
    return(CDialog::OnWndMsg(message, wParam, lParam, pResult));
}

BOOL CDLUpdateDlg::FillListFromDB() {
	CDaoDatabase* pDLDB = m_pStation->GetDomainDBPtr();	    // domain database of the downloader
    int nItem = 0;
    CString strCodeDom;
    CString strVisDom;
    CString strDelete;
    CString strNew,strTemp;
    CString strChanged;
    CString strNothing;
    int iImageId;
    
	if(pDLDB != NULL) {
        strCodeDom.LoadString(IDS_DOMTYPE_CODE);
        strVisDom.LoadString(IDS_DOMTYPE_VIS);
        strDelete.LoadString(IDS_DOMACTION_DELETE);
        strNew.LoadString(IDS_DOMACTION_NEW);
        strChanged.LoadString(IDS_DOMACTION_CHANGED);
        strNothing.LoadString(IDS_DOMACTION_NOTHING);
	    try {
            CDomRS DomRS(pDLDB);
			strTemp.Format(_T("SELECT * FROM Domain WHERE ConfId = %d"),m_pStation->GetConfId());

		    DomRS.Open(dbOpenDynaset,strTemp);
		    while(!DomRS.IsEOF()) {
                iImageId = GetImageIdFromFlag((CEDL_DOM_FLAG)DomRS.m_Flags);
                
				CString strDomainName = DomRS.m_DomainName;
				if(SkipDomain(strDomainName)) {
					DomRS.MoveNext();
					continue;
				}

                AddItem(nItem, 0, strDomainName, iImageId);
                m_DomainListCtrl.SetItemData(nItem, DomRS.m_DomainId);
                AddItem(nItem, 1, strCodeDom);

                switch(iImageId) {
                    case IMG_DELETED:
                        AddItem(nItem, 2, strDelete);
                        break;
                    case IMG_NEW:
                        AddItem(nItem, 2, strNew);
                        break;
                    case IMG_DOWNLOAD:
                        AddItem(nItem, 2, strChanged);
                        break;
                    default:
                        AddItem(nItem, 2, strNothing);
                        break;
                }
                nItem++;
                DomRS.MoveNext();
            }
            DomRS.Close();
        }
    	    catch(CDaoException* e) {
		    DisplayDaoException(e);
		    e->Delete();
		    return(FALSE);
	    }
    }
    return(TRUE);
}

BOOL CDLUpdateDlg::SkipDomain(CString & strDomain)
{
	if(!strDomain.CompareNoCase(CEDL_VERSIONDOMAIN)) {
		return TRUE;
	}

    return FALSE;
}


int CDLUpdateDlg::GetImageIdFromFlag(CEDL_DOM_FLAG Flag) {
    if(Flag & DOM_DELETED) return(IMG_DELETED);
    if(Flag & DOM_NEW) return(IMG_NEW);
    if(Flag & DOM_CHANGED) return(IMG_DOWNLOAD);
    return(IMG_UNCHANGED);
}

void CDLUpdateDlg::ChangeAllSelectedItems(CEDL_DO_ACTIONS ActionToDo) {
    CString strSQL;
    int nSelItems = m_DomainListCtrl.GetItemCount();
    if(nSelItems == LB_ERR) return;

    m_DomainListCtrl.SetRedraw(FALSE);
	for(int iItem = 0; iItem < nSelItems; iItem++) {
	    LV_ITEM	Item;

        Item.iItem = iItem;
		Item.iSubItem = 0;
		Item.mask = LVIF_IMAGE;
		if(m_DomainListCtrl.GetItemState(iItem, LVIS_SELECTED) == LVIS_SELECTED) {
            if(m_DomainListCtrl.GetItem(&Item)) {
                DWORD dwDomainId = m_DomainListCtrl.GetItemData(iItem);
                short sNewFlags;
                switch(ActionToDo) {
                    case CEDL_DO_NOTHING:
                        Item.iImage = IMG_UNCHANGED;
                        sNewFlags = DOM_UNCHANGED;
                        break;
                    case CEDL_DO_DOWNLOAD:
                        Item.iImage = IMG_DOWNLOAD;
                        sNewFlags = DOM_CHANGED | DOM_SELECTED;
                        break;
                    case CEDL_DO_DELETE:
                        Item.iImage = IMG_DELETED;
                        sNewFlags = DOM_DELETED;
                        break;
                }
                // now change the database
        	    try {
                    strSQL.Format(_T("UPDATE Domain SET Flags = %d WHERE DomainId = %ld"), sNewFlags, dwDomainId);
                    m_pStation->GetDomainDBPtr()->Execute(strSQL);
                }
    	            catch(CDaoException* e) {
		            DisplayDaoException(e);
		            e->Delete();
		            return;
	            }
				m_DomainListCtrl.SetItem(&Item); 
            }
        }
    }
    m_DomainListCtrl.SetRedraw(TRUE);
    m_DomainListCtrl.Invalidate();
    return;
}

void CDLUpdateDlg::DisplayColumnHeadings(UINT iStringId)
{
	int iPos,iID;
	int iCount = 0;
    int iIDSDom[] = {IDS_DOMTYPE_CODE,IDS_DOMTYPE_VIS};
    int iIDSTodo[] = {IDS_DOMACTION_CHANGED,IDS_DOMACTION_NEW,IDS_DOMACTION_DELETE,IDS_DOMACTION_NOTHING};

	CString strHeadings;
	strHeadings.LoadString(iStringId);

    m_DomainListCtrl.SetColumnWidth(-1, -3); // LVSCW_AUTOSIZE);
	EraseList();

	while((iPos = strHeadings.Find(_T(","))) != -1) {
		CString strItem;
		strItem = strHeadings.Left(iPos);

        switch(iCount) {
            case 1:
                iID = GetMaxStringId(strItem,iIDSDom,FC_ARRAY_LEN(iIDSDom));
                break;
            default:
                iID = 0;
                break;
        }

		AddColumn(strItem, iCount++,iID);

		strItem = strHeadings.Mid(iPos + 1);
		strHeadings = strItem;
	}

    iID = GetMaxStringId(strHeadings,iIDSTodo,FC_ARRAY_LEN(iIDSTodo));
    AddColumn(strHeadings, iCount,iID);
    return;
}

BOOL CDLUpdateDlg::AddColumn(LPCTSTR strItem, int nItem,int iID, int nSubItem, int nMask, int nFmt) 
{
    CString str;

	LV_COLUMN lvc;
	lvc.mask = nMask;
	lvc.fmt = nFmt;
	lvc.pszText = (LPTSTR) strItem;
    
    if(!iID) {
	    lvc.cx = m_DomainListCtrl.GetStringWidth(strItem) + 15;
    }
    else {
        str.LoadString(iID);
        lvc.cx = m_DomainListCtrl.GetStringWidth(str) + 15;
    }

	if(nMask & LVCF_SUBITEM){
		if(nSubItem != -1)
			lvc.iSubItem = nSubItem;
		else
			lvc.iSubItem = nItem;
	}

	return(m_DomainListCtrl.InsertColumn(nItem,&lvc));
}

void CDLUpdateDlg::EraseList(void) {
	m_DomainListCtrl.DeleteAllItems();
	while(m_DomainListCtrl.DeleteColumn(0));
	UpdateWindow();
	return;
}

BOOL CDLUpdateDlg::AddItem(int nItem,int nSubItem,LPCTSTR strItem,int nImageIndex) {
	LV_ITEM lvItem;
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = nItem;
	lvItem.iSubItem = nSubItem;
	lvItem.pszText = (LPTSTR) strItem;
	if(nImageIndex != -1){
		lvItem.mask |= LVIF_IMAGE;
		lvItem.iImage = nImageIndex;
	}
    if(nSubItem == 0) {
		return(m_DomainListCtrl.InsertItem(&lvItem));
    }
	return(m_DomainListCtrl.SetItem(&lvItem));
}

void CDLUpdateDlg::ClearProgress(void) 
{
    m_strDomainName = _T("");
    m_DlProgressCtrl.SetRange(0, 100);
    m_DlProgressCtrl.SetPos(0);
    UpdateData(FALSE);
    return;
}

void CDLUpdateDlg::OnDompopupDel() 
{
    ChangeAllSelectedItems(CEDL_DO_DELETE);
    return;
}

void CDLUpdateDlg::OnDompopupDown() 
{
    ChangeAllSelectedItems(CEDL_DO_DOWNLOAD);
    return;
}

void CDLUpdateDlg::OnDompopupNothing() 
{
    ChangeAllSelectedItems(CEDL_DO_NOTHING);
    return;
}

void CDLUpdateDlg::OnColumnclickUpdateList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

//pNMListView->iSubItem	
//    m_DomainListCtrl.SortItems(&CompareFunc, NULL);
	*pResult = 0;
}

void CDLUpdateDlg::OnUpdateDownload() 
{
    HRESULT hr = S_OK;
    assert(m_pStation != NULL);
    ClearProgress();
    m_DownloadBtn.EnableWindow(FALSE);
    if (m_pStation->IsRequestPending()) {
        m_pStation->FireError(E_CE_DL_PENDING);
        return;
    }
    m_pStation->ResetSignal(DOWN_LD_ALL);
    m_pStation->ResetSignal(BUILD_DL_LIST);
    m_pStation->SetSignal(DOWN_LD);
    m_pStation->ResetSignal(CHANGED);
    m_pStation->FireStartDownload();
    hr = m_pStation->DoStateMachine();
	if(FAILED(hr)) {
        m_pStation->FireError(hr);
	}
    return;
}

void CDLUpdateDlg::OnUpdateAbort() 
{
    assert(m_pStation != NULL);
    m_DownloadBtn.EnableWindow(TRUE);
    m_pStation->Abort();
    return;
}

void CDLUpdateDlg::OnOK() {
    assert(m_pStation != NULL);
	m_pDLEvents->SetListClient(NULL);
    m_pStation->Abort();
    CDialog::OnOK();
}

void CDLUpdateDlg::OnCancel()
{
    assert(m_pStation != NULL);
	m_pDLEvents->SetListClient(NULL);
	m_pStation->Abort();
	CDialog::OnCancel();
}


void CDLUpdateDlg::OnUpdateSelectall() 
{
    int nMaxItems = m_DomainListCtrl.GetItemCount();
    if(nMaxItems == LB_ERR) return;

	for(int iItem = 0; iItem < nMaxItems; iItem++) {
	    LV_ITEM	Item;

        Item.iItem = iItem;
		Item.iSubItem = 0;
        Item.mask = LVIF_STATE; 
        Item.state = LVIS_SELECTED;
        Item.stateMask = (UINT)-1;
        m_DomainListCtrl.SetItem(&Item);
    }
    m_DomainListCtrl.Invalidate();
    ClearProgress();
    return;
}

void CDLUpdateDlg::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here

    CMenu	Menu; 
    CPoint ptItem(0,0),ptHelp(0,0);
	CRect rRect;
	POSITION Pos;
	int iItem;

	Pos = m_DomainListCtrl.GetFirstSelectedItemPosition();
	if(!Pos) {
		return;
	}

	iItem = m_DomainListCtrl.GetNextSelectedItem(Pos);

	m_DomainListCtrl.GetItemRect(iItem,&rRect,LVIR_LABEL);
	ptItem = rRect.TopLeft();

	if(ptItem.x < 0 || ptItem.y < 0) {
		ptItem.x = ptItem.y = 0;
	}

	m_DomainListCtrl.ClientToScreen(&ptItem);


    VERIFY(Menu.LoadMenu(IDR_DOM_POPUP));
    CMenu* pPopup = Menu.GetSubMenu(0);
    assert(pPopup != NULL); 
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, ptItem.x, ptItem.y, this);	

}

void CDLUpdateDlg::UpdateState(void)
{
    CString str;
    int iIDS = -1;
    CStatic *pStatic;

    assert(m_pStation);
    CStationInfo & rInfo = m_pStation->GetStationInfo();

    int iIDSArray[] = {-1,IDS_NOT_CONNECTED,IDS_DIFF_CLEARED,IDS_DIFF_PRJ,IDS_DIFF_UNSYNC,IDS_DIFF_SYNC};

    int state = rInfo.State();
    iIDS = iIDSArray[state];

    if(iIDS != -1) {
        str.LoadString(iIDS);
    }

    pStatic = (CStatic*) GetDlgItem(IDC_UPDATE_STATE);
    assert(pStatic);

    pStatic->SetWindowText(str);
}

BOOL CDLUpdateDlg::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: Add your message handler code here and/or call default
    if (pHelpInfo->iContextType == HELPINFO_WINDOW){
        return ShowHelp(GetSafeHwnd(),pHelpInfo->dwContextId);
    }
    return TRUE;
}

