//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "lated.h"
#include "lclink.h"
//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CObjPropTarget property page

IMPLEMENT_DYNCREATE(CObjPropTarget, CPropertyPage)

CObjPropTarget::CObjPropTarget() : CPropertyPage(CObjPropTarget::IDD) ,
    m_strInvalid() , m_strInvalidHost()
{
    m_bInit = false;
    m_pParent = NULL;
    m_iColumns = 0;
    m_iSel = -1;
	//{{AFX_DATA_INIT(CObjPropTarget)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CObjPropTarget::~CObjPropTarget()
{
}

void CObjPropTarget::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CObjPropTarget)
	DDX_Control(pDX, IDC_TARGETS, m_TargetList);
	//}}AFX_DATA_MAP
}

void CObjPropTarget::InitColumnHeaders()
{
	CRect Rect;
	CString strHeadings;
	int iPos;
	int iCount = 0;

	m_TargetList.GetWindowRect(&Rect);

	// Set column headers (each column header is separated with ',' 
	// in loaded string
	strHeadings.LoadString(IDS_TARGETS_COLHEAD);
    // Count number of columns
	while((iPos = strHeadings.Find(_T(","))) != -1) 
	{
        strHeadings = strHeadings.Mid(iPos + 1);
		iCount++;
	}
    m_iColumns = iCount;
    iCount = 0;
    // Insert column header
	strHeadings.LoadString(IDS_TARGETS_COLHEAD);
	while((iPos = strHeadings.Find(_T(","))) != -1) 
	{
		CString strItem;
		strItem = strHeadings.Left(iPos);
		m_TargetList.InsertColumn(iCount, strItem, LVCFMT_LEFT, Rect.Width() / m_iColumns);
		strItem = strHeadings.Mid(iPos + 1);
		strHeadings = strItem;
		iCount++;
	}
}

void CObjPropTarget::InsertItem(CTargetInfo & info)
{
    CTargetInfo* pInfo = new CTargetInfo(info);
    if(!pInfo) {
       return;
    }
    
    int iCount = m_TargetList.GetItemCount();

    LVITEM item;
    ::memset(&item, 0, sizeof(item));
    item.mask = LVIF_TEXT | LVIF_PARAM;
    item.iItem = iCount;
    item.pszText = LPSTR_TEXTCALLBACK;
    item.lParam = (LPARAM) pInfo;
    m_TargetList.InsertItem(&item);
}

void CObjPropTarget::InsertPropertyLink()
{
    CTargetInfo info;
    ASSERT(m_pParent);

    if(m_pParent->IsDestLink()) {
        CLCDestLink* pDestLink = m_pParent->GetDestLink();
        ASSERT(pDestLink);
        pDestLink->ToTargetInfo(info);
        InsertItem(info);
    }
    else { // link
        CLCLink* pLink = m_pParent->GetLink();
        ASSERT(pLink);
        pLink->ToTargetInfo(info);
        InsertItem(info);
    }
}

void CObjPropTarget::Cleanup()
{
    DWORD dwData = 0UL;
    int nCount = m_TargetList.GetItemCount();
    CTargetInfo* pInfo;
    
    for (int i=0;i < nCount;i++) {
        dwData = m_TargetList.GetItemData(i);
        if(!dwData) {
            continue;
        }

        pInfo = reinterpret_cast<CTargetInfo*>(dwData);
        if(pInfo) {
            delete pInfo;
            pInfo = NULL;
        }

        m_TargetList.SetItemData(i,0UL);
    }
}

void CObjPropTarget::GetTargets()
{
    HRESULT hr = S_OK;
    CLCConfig* pConfig = NULL;

    if(!m_pParent) {
        return;
    }

    pConfig = m_pParent->GetConfig();
    if(!pConfig) {
        return;
    }
    pConfig->AddConfigListener(this);
    pConfig->TraverseTargetInfo(GetTargetList);
    pConfig->RemoveConfigListener(this);
}

void CObjPropTarget::OnTarget(int id, CTargetInfo & info)
{
    ASSERT(m_pParent);

    if(m_pParent->IsDestLink()) {
        CLCDestLink* pDestLink = m_pParent->GetDestLink();
        ASSERT(pDestLink);

        if(info.m_strName == pDestLink->GetTargetName()) {
            m_iSel = m_TargetList.GetItemCount();
        }
    }
    else { // link
        CLCLink* pLink = m_pParent->GetLink();
        ASSERT(pLink);

        if(info.m_strName == pLink->GetTargetName()) {
            m_iSel = m_TargetList.GetItemCount();
        }
    }
    InsertItem(info);        
}

BOOL CObjPropTarget::OnSetActive() 
{
    if(!m_bInit) {
        InitColumnHeaders();        
        GetTargets();
        m_bInit = true;   
        m_strInvalid.LoadString(IDS_INVALID_LINK);
        m_strInvalidHost.LoadString(IDS_INVALID_HOST);
    }

    ASSERT(m_pParent);

    if(m_pParent->IsProperty()) {
        if(m_TargetList.GetItemCount() > 0) {
            m_iSel = (m_iSel == -1) ? 0 : m_iSel;
        }
        else {
            InsertPropertyLink();
            m_iSel = 0;
        }

        m_TargetList.SetItemState(m_iSel, LVIS_SELECTED | LVIS_FOCUSED, 
            LVIS_SELECTED | LVIS_FOCUSED);
    }
    else {
        if(m_TargetList.GetItemCount() > 0) {
            m_TargetList.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, 
                LVIS_SELECTED | LVIS_FOCUSED);
        }
    }
	return CPropertyPage::OnSetActive();
}

BOOL CObjPropTarget::OnKillActive() 
{
    int iItem;
    DWORD dwData = 0UL;
    POSITION pos;
    CTargetInfo* pInfo = NULL;
    CLCLink* pLink = NULL;
    CLCDestLink* pDestLink = NULL;

    ASSERT(m_pParent);

    if(m_TargetList.GetItemCount() == 0) {
        ::AfxMessageBox(IDS_NOTARGET, MB_ICONWARNING);
        return FALSE;
    }

    pos = m_TargetList.GetFirstSelectedItemPosition();
    if(!pos) {
        ::AfxMessageBox(IDS_SELTARGET, MB_ICONWARNING);
        return FALSE;
    }
    
    iItem = m_TargetList.GetNextSelectedItem(pos);
    
    //get the data
    dwData = m_TargetList.GetItemData(iItem);
    if(!dwData) {
        return FALSE;
    }

    pInfo = reinterpret_cast<CTargetInfo*>(dwData);
    if(!pInfo) {
        ASSERT(pInfo);
        return FALSE;
    }

    if(m_pParent->IsDestLink()) {
        pDestLink = m_pParent->GetDestLink();
        ASSERT(pDestLink);
        pDestLink->SetConnType(P2P);
        *pDestLink = *pInfo;
    }
    else {
        pLink = m_pParent->GetLink();
        ASSERT(pLink);
        pLink->SetConnType(P2P);
        *pLink = *pInfo; 
    }

    return CPropertyPage::OnKillActive();
}

void CObjPropTarget::OnCancel() 
{
    Cleanup();
	CPropertyPage::OnCancel();
}

void CObjPropTarget::OnOK() 
{
    Cleanup();	
	CPropertyPage::OnOK();
}


void CObjPropTarget::OnDblclkTargets(NMHDR* pNMHDR, LRESULT* pResult) 
{
    if(m_pParent) {
        m_pParent->PressButton(PSBTN_OK);
        m_pParent->EndDialog(IDOK); 
    }
    *pResult = 0;
}


BEGIN_MESSAGE_MAP(CObjPropTarget, CPropertyPage)
	//{{AFX_MSG_MAP(CObjPropTarget)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_TARGETS, OnGetdispinfo)
	ON_NOTIFY(NM_DBLCLK, IDC_TARGETS, OnDblclkTargets)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObjPropTarget message handlers
void CObjPropTarget::OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDisp = (LV_DISPINFO*)pNMHDR;
	// Add your control notification handler code here
	*pResult = 0;

    if(pDisp && pDisp->item.mask & LVIF_TEXT) {
        CString str;
        LPCTSTR psz = NULL;

        CTargetInfo* pInfo = reinterpret_cast<CTargetInfo*>(pDisp->item.lParam);
        if(!pInfo) {
            return;
        }

        switch(pDisp->item.iSubItem) {
        case Col_Name:
            if(!pInfo->m_bValid) {
                str = pInfo->m_strName;
                str += _T(" (");
                str += m_strInvalid;
                str += _T(")");
                psz = str;
            }
            else {
                psz = pInfo->m_strName;
            }
            break;
        case Col_Type:
            psz = pInfo->m_strType;
            break;
        case Col_Resource:
            psz = pInfo->m_strResource;
            break;
        case Col_IP:
            if(!pInfo->m_ulIP || pInfo->m_ulIP == ULONG_MAX) {
                psz = m_strInvalidHost;
                break;
            }
            psz = pInfo->m_strIP;
            break;
        default:
            psz = NULL;
            break;
        }

        if(psz) {
            _tcsncpy(pDisp->item.pszText, psz,pDisp->item.cchTextMax);
        }
    }
}


/////////////////////////////////////////////////////////////////////////////
// CDialogTarget

IMPLEMENT_DYNAMIC(CDialogTarget, CPropertySheet)

CDialogTarget::CDialogTarget(CLCConfig* pConfig, CLCLink* pLink, 
                             CLCDestLink* pDest, bool bProperty,
                             UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
    m_pConfig = pConfig;
    m_pLink = pLink;
    m_pDestLink = pDest;
    m_bProperty = bProperty;
    InitPages();
}

CDialogTarget::~CDialogTarget()
{}

void CDialogTarget::InitPages()
{
    m_psh.dwFlags |= PSH_NOAPPLYNOW;
    m_TargetDecl.SetParent(this);
    AddPage(&m_TargetDecl);
}

BEGIN_MESSAGE_MAP(CDialogTarget, CPropertySheet)
	//{{AFX_MSG_MAP(CDialogTarget)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogTarget message handlers

/////////////////////////////////////////////////////////////////////////////
// CDlgAssignType dialog


CDlgAssignType::CDlgAssignType(CLCConfig* pConfig, CLCLink* pLink, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAssignType::IDD, pParent)
{
    m_pConfig = pConfig;
    m_pLink = pLink;
	//{{AFX_DATA_INIT(CDlgAssignType)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgAssignType::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAssignType)
	DDX_Control(pDX, IDC_DATATYPE_LIST, m_lb);
	//}}AFX_DATA_MAP
}

BOOL CDlgAssignType::OnInitDialog() 
{
	CString str;

    CDialog::OnInitDialog();

    str.LoadString(IDS_NO_ASSIGNMENT);
    m_lb.AddString(str);

    if(m_pConfig) {
        m_pConfig->AddConfigListener(this);
        m_pConfig->TraverseDatatypes(AssignDatatype);
        m_pConfig->RemoveConfigListener(this);
    }

    UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAssignType::OnDataType(int id, CLCDataType *pType)
{
    if(id == AssignDatatype) {
        m_lb.AddString(pType->GetName());
    }
}

bool CDlgAssignType::DoOK()
{
    CString str;
    CString str2;
    int iIndex;

    ASSERT(m_pConfig);
    ASSERT(m_pLink);

    if(!m_pConfig || !m_pLink) {
        return true; //end dialog
    }

    UpdateData(TRUE);
    
    iIndex = m_lb.GetCurSel();
    if(iIndex == LB_ERR) {
        return false; //no selection
    }

    m_lb.GetText(iIndex,str);

    str2.LoadString(IDS_NO_ASSIGNMENT);

    if(str.IsEmpty() || !str.CompareNoCase(str2)) {
        m_pLink->AssignDataType(NULL);
        return true; //end dialog
    }

    if(m_pConfig) {
        CLCDataType * p = m_pConfig->GetDatatype(str);
        if(p && m_pLink) {
            m_pLink->AssignDataType(p);
        }
    }

    return true;

}

BEGIN_MESSAGE_MAP(CDlgAssignType, CDialog)
	//{{AFX_MSG_MAP(CDlgAssignType)
	ON_LBN_DBLCLK(IDC_DATATYPE_LIST, OnDClkDatatypeList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAssignType message handlers
void CDlgAssignType::OnOK() 
{
    bool bEnd = DoOK();
    
    if(bEnd) {
        CDialog::OnOK(); 
    }
    else {
        return;
    }
}

void CDlgAssignType::OnCancel() 
{
	CDialog::OnCancel();
}

void CDlgAssignType::OnDClkDatatypeList() 
{
    bool bEnd = DoOK();

    if(bEnd) {
        EndDialog(IDOK);
    }
    else {
        return;
    }
}