//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "lated.h"
#include "lcdatatype.h"

//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CObjPropWiz property page

IMPLEMENT_DYNCREATE(CObjPropWiz, CPropertyPage)

CObjPropWiz::CObjPropWiz() : CPropertyPage(CObjPropWiz::IDD)
{
	//{{AFX_DATA_INIT(CObjPropWiz)
	m_strName = _T("");
	//}}AFX_DATA_INIT

    m_pDataType = NULL;
    m_pConfig = NULL;
    m_ImageList.Create(IDB_WIZIMAGELIST, 20, 0, RGB(255,0,255));
    m_ImageList.SetBkColor(::GetSysColor(COLOR_WINDOW));  
}

CObjPropWiz::~CObjPropWiz()
{
}

void CObjPropWiz::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CObjPropWiz)
	DDX_Control(pDX, IDC_LIST1, m_Wizards);
	DDX_Text(pDX, IDC_OBJNAME, m_strName);
	//}}AFX_DATA_MAP
}

void CObjPropWiz::AddWizardName(LPCTSTR WizardName, LPCTSTR Language) 
{
    m_strWizName = WizardName;
    m_strLanguage = Language;
}

BOOL CObjPropWiz::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

    m_Wizards.SetImageList(&m_ImageList, LVSIL_SMALL);

    m_Wizards.InsertItem(0, m_strWizName, 0);
    m_Wizards.SetItemState(0, 
                        LVIS_SELECTED | LVIS_FOCUSED, 
                        LVIS_SELECTED | LVIS_FOCUSED);

    return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_MESSAGE_MAP(CObjPropWiz, CPropertyPage)
	//{{AFX_MSG_MAP(CObjPropWiz)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CObjPropWiz::OnKillActive() 
{
    CString strMessage;

    UpdateData(TRUE);
	m_strName.TrimLeft();
    m_strName.TrimRight();
	UpdateData(FALSE);

    // Check syntax of name.
    HRESULT hr = UTIL_CheckIdentifier(m_strName);
    if (FAILED(hr))
	{
        strMessage.Format(IDS_OBJ_NAME_INVALID, m_strName);
        ::AfxMessageBox(strMessage, MB_ICONWARNING);
        return FALSE;
	}

    ASSERT(m_pConfig);
    if(m_pConfig) {
        //Is there a datatype with name m_strName and not me ?
        CLCDataType* p = m_pConfig->GetDatatype(m_strName);
        if(p && m_pDataType && p != m_pDataType) {
            //a data type with the name already exists.
            strMessage.Format(IDS_DUT_EXISTS, m_strName);
            ::AfxMessageBox(strMessage, MB_ICONWARNING);
            p->Release();
            return FALSE;
        }
    }

    if(m_pDataType) {
        m_pDataType->SetName(m_strName);
    }
	
	return CPropertyPage::OnKillActive();
}

/////////////////////////////////////////////////////////////////////////////
// CObjPropWiz message handlers
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CObjPropGen dialog
IMPLEMENT_DYNCREATE(CObjPropGen, CPropertyPage)

CObjPropGen::CObjPropGen() :  CPropertyPage(CObjPropGen::IDD)
{
	//{{AFX_DATA_INIT(CObjPropGen)
	m_strAlias = _T("");
	m_strName = _T("");
	//}}AFX_DATA_INIT
    m_pDataType = NULL;
    m_bProperty = false;
}

CObjPropGen::CObjPropGen(bool bProperty) :  CPropertyPage(CObjPropGen::IDD)
{
	m_strAlias = _T("");
	m_strName = _T("");
    m_pDataType = NULL;
    m_bProperty = bProperty;
}

void CObjPropGen::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CObjPropGen)
	DDX_Text(pDX, IDC_EDIT_ALIAS, m_strAlias);
	DDX_Text(pDX, IDC_OBJNAME, m_strName);
	//}}AFX_DATA_MAP
}

BOOL CObjPropGen::OnInitDialog()
{
    CString str;

	CPropertyPage::OnInitDialog();

    if(m_pDataType) {
        m_pDataType->GetName(m_strName);
        m_strAlias = m_pDataType->GetAlias();
    }

    if(m_pConfig) {
        HRESULT hr = S_OK;
        CFileStatus status;
        COleDateTime dtCreate,dtModify;
        CComBSTR sType = _T("DT");
        CComBSTR s;
        VARIANT vt;

        CString strPath = m_pConfig->GetPath();

        if(CFile::GetStatus(strPath,status)) {
            dtCreate = status.m_ctime.GetTime();
            dtModify = status.m_mtime.GetTime();

            ::VariantClear(&vt);
            vt.date = dtCreate.m_dt;
            vt.vt = VT_DATE;

            hr = BL_VariantToStringNamed(
                                 &vt,        // in parameter
                                 sType,       // namestring of datatype
                                 NULL,     // may be NULL or "" if not known
                                 BL_USE_IEC_FORMAT, // BL_USE_WINDOWS_FORMAT||BL_USE_IEC_FORMAT
                                 _MAX_PATH,  // number of characters wanted in output string
                                 &s);          // formatted string

            if(SUCCEEDED(hr)) {
                str = s;
                GetDlgItem(IDC_OBJPROP_CREATEDATE)->SetWindowText(str);
            }

            s.Empty();
            ::VariantClear(&vt);
            vt.date = dtModify.m_dt;
            vt.vt = VT_DATE;

            hr = BL_VariantToStringNamed(
                                 &vt,        // in parameter
                                 sType,       // namestring of datatype
                                 NULL,     // may be NULL or "" if not known
                                 BL_USE_IEC_FORMAT, // BL_USE_WINDOWS_FORMAT||BL_USE_IEC_FORMAT
                                 _MAX_PATH,  // number of characters wanted in output string
                                 &s);          // formatted string

            if(SUCCEEDED(hr)) {
                str = s;
                GetDlgItem(IDC_OBJPROP_LASTMODDATE)->SetWindowText(str);
            }

            ::VariantClear(&vt);
            s.Empty();

        }
    }
    
    GetDlgItem(IDC_OBJNAME)->EnableWindow(FALSE);

    GetCurrentUserName(str);

    GetDlgItem(IDC_OBJPROP_CREATEUSER)->SetWindowText(str);
    GetDlgItem(IDC_OBJPROP_LASTMODUSER)->SetWindowText(str);

    UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

}

BOOL CObjPropGen::OnKillActive() 
{
    UpdateData(TRUE);
	m_strAlias.TrimLeft();
    m_strAlias.TrimRight();

    if(m_pDataType) {
        m_pDataType->SetAlias(m_strAlias);
    }
	
	return CPropertyPage::OnKillActive();
}


BEGIN_MESSAGE_MAP(CObjPropGen, CPropertyPage)
	//{{AFX_MSG_MAP(CObjPropGen)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObjPropGen message handlers


// CObjPropInfo property page

IMPLEMENT_DYNCREATE(CObjPropInfo, CPropertyPage)

CObjPropInfo::CObjPropInfo() : CPropertyPage(CObjPropInfo::IDD)
{
	//{{AFX_DATA_INIT(CObjPropInfo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CObjPropInfo::~CObjPropInfo()
{
}

void CObjPropInfo::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CObjPropInfo)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CObjPropInfo, CPropertyPage)
	//{{AFX_MSG_MAP(CObjPropInfo)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObjPropInfo message handlers
/////////////////////////////////////////////////////////////////////////////
// CObjPropDutDecl property page

IMPLEMENT_DYNCREATE(CObjPropDutDecl, CPropertyPage)

CObjPropDutDecl::CObjPropDutDecl() : CPropertyPage(CObjPropDutDecl::IDD)
{
    m_bInit = FALSE;
    m_bProperty = false;
    m_iColumnStrId = -1;
    m_pDataType = NULL;
    m_pConfig = NULL;
    InitColumnHeaders();
}

CObjPropDutDecl::CObjPropDutDecl(bool bProperty,int iColumnStrId) : CPropertyPage(CObjPropDutDecl::IDD)
{
	//{{AFX_DATA_INIT(CObjPropDutDecl)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    m_bInit = FALSE;
    m_bProperty = bProperty;
    m_iColumnStrId = iColumnStrId;
    m_pDataType = NULL;
    m_pConfig = NULL;
    InitColumnHeaders();
}

CObjPropDutDecl::~CObjPropDutDecl()
{
}

void CObjPropDutDecl::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CObjPropDutDecl)
	DDX_Control(pDX, IDC_MEMBERS, m_FbMembers);
	//}}AFX_DATA_MAP
}

BOOL CObjPropDutDecl::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	return TRUE;  // return TRUE unless you set the focus to a control
             // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CObjPropDutDecl::OnSetActive() 
{
	if(!m_bInit)
	{
		CRect Rect;
		CString strHeadings;
		int iPos;
		int iCount = 0;

		m_FbMembers.GetWindowRect(&Rect);

		// Set column headers (each column header is separated with ',' 
		// in loaded string
		strHeadings.LoadString(m_iColumnStrId);
        // Count number of columns
		while((iPos = strHeadings.Find(_T(","))) != -1) 
		{
            strHeadings = strHeadings.Mid(iPos + 1);
			iCount++;
		}
        m_iColumns = iCount;
        iCount = 0;
        // Insert column header
		strHeadings.LoadString(m_iColumnStrId);
		while((iPos = strHeadings.Find(_T(","))) != -1) 
		{
			CString strItem;
			strItem = strHeadings.Left(iPos);
			m_FbMembers.InsertColumn(iCount, strItem, LVCFMT_LEFT, Rect.Width() / m_iColumns);
			strItem = strHeadings.Mid(iPos + 1);
			strHeadings = strItem;
			iCount++;
		}

        if(m_pDataType) {
            m_pDataType->TraverseMembers(TraverseMembers,this);
        }

		m_bInit = TRUE; //initialized
	}

    if(m_FbMembers.GetItemCount() > 0)
    {
        BOOL bSelected = m_FbMembers.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
    }

    EnableButtons();
	
	return CPropertyPage::OnSetActive();
}

void CObjPropDutDecl::OnOK() 
{
    SetMembers();
	CPropertyPage::OnOK();
}

void CObjPropDutDecl::OnCancel() 
{
    SetMembers();
    CPropertyPage::OnCancel();
}

void CObjPropDutDecl::InitColumnHeaders()
{
    if(m_iColumnStrId == IDS_PROPDECL_COLHEAD_2)
    {
        m_aiCol[COL_NAME]           = 0;
        m_aiCol[COL_ALIAS]          = 1;
        m_aiCol[COL_TYPE]           = 2;
        m_aiCol[COL_INITVAL]        = 3;
        m_aiCol[COL_COMMENT]        = 4;
        m_aiCol[COL_STORAGECLASS]   = -1;    // not used
    }
    else
    {
        m_aiCol[COL_STORAGECLASS]   = 0;
        m_aiCol[COL_NAME]           = 1;
        m_aiCol[COL_ALIAS]          = 2;
        m_aiCol[COL_TYPE]           = 3;
        m_aiCol[COL_INITVAL]        = 4;
        m_aiCol[COL_COMMENT]        = 5;
    }
}

void CObjPropDutDecl::InsertMember(CLCMember* pMember)
{
    if(!pMember) {
        ASSERT(pMember);
        return;
    }

    int iCount = m_FbMembers.GetItemCount();

    LVITEM item;
    ::memset(&item, 0, sizeof(item));
    item.mask = LVIF_TEXT | LVIF_PARAM;
    item.iItem = iCount;
    item.pszText = LPSTR_TEXTCALLBACK;
    item.lParam = (LPARAM) pMember;
    m_FbMembers.InsertItem(&item);
}

void CObjPropDutDecl::OnMember(int id, CLCDataType * pType, CLCMember* pMember)
{
    pMember->AddRef();
    InsertMember(pMember);
}

bool CObjPropDutDecl::OnCheckDuplicate(CString & strName)
{
 	int iItem = -1;
    CString str;
    
    // Get selected item
	POSITION pos = m_FbMembers.GetFirstSelectedItemPosition();
    if(pos) {
    	iItem = m_FbMembers.GetNextSelectedItem(pos);
    }
    
    int nCount = m_FbMembers.GetItemCount();
    
    for (int i=0;i < nCount;i++)
    {
        str = m_FbMembers.GetItemText(i,0);
        if(!str.CompareNoCase(strName)) {
            //identical
            if(i == iItem) { //selected one when modifying
                continue;
            }

            return true;
        }
    }

    return false;
}

void CObjPropDutDecl::SetMembers()
{
    int nCount;
    DWORD dwData;
    CLCMember* pMember = NULL;

    if(!m_pDataType) {
        return;
    }

    m_pDataType->RemoveMembers();

    nCount = m_FbMembers.GetItemCount();
    
    for (int i=0;i < nCount;i++) {
        dwData = m_FbMembers.GetItemData(i);
        if(!dwData) {
            continue;
        }

        pMember = reinterpret_cast<CLCMember*>(dwData);
        
        m_FbMembers.SetItemData(i,0UL);
        m_pDataType->AddMember(pMember);
    }
}

BEGIN_MESSAGE_MAP(CObjPropDutDecl, CPropertyPage)
	//{{AFX_MSG_MAP(CObjPropDutDecl)
	ON_BN_CLICKED(IDC_ADDMEMBER, OnAddmember)
	ON_NOTIFY(NM_CLICK, IDC_MEMBERS, OnClickMembers)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_MEMBERS, OnGetdispinfoMembers)
	ON_BN_CLICKED(IDC_MODIFYMEMBER, OnModifymember)
	ON_BN_CLICKED(IDC_DELETEMEMBER, OnDeletemember)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObjPropDutDecl message handlers
void CObjPropDutDecl::OnAddmember() 
{

	CString	strName;
	CString	strAlias;
	CString	strComment;
	CString	strType;
	CString	strInitVal;
    int iCount = -1;

    // Add your control notification handler code here
    CModifyDutMember aDutMember;
    aDutMember.SetParent(this);

	switch(aDutMember.DoModal())
	{
	case IDOK:
        {
		    aDutMember.GetMemberDecl(strName, strAlias, strComment, strType, strInitVal);

            CLCMember* pMember = new CLCMember();
            if(!pMember) {
                AfxThrowMemoryException();
            }

            pMember->SetProperty(CLCMember::all,strName,strType,strAlias,strComment,strInitVal);
            InsertMember(pMember);

        }
		break;
	case IDCANCEL:
		break;
	default:
        break;
	}

}

void CObjPropDutDecl::OnClickMembers(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// Add your control notification handler code here
    EnableButtons();
	*pResult = 0;
}


void CObjPropDutDecl::EnableButtons()
{
    int iSelCount = m_FbMembers.GetSelectedCount();
    int iItemCount = m_FbMembers.GetItemCount();

    // Enable'Modify' and 'Delete' button
    GetDlgItem(IDC_MODIFYMEMBER)->EnableWindow(iSelCount == 1 && iItemCount > 0);
    GetDlgItem(IDC_DELETEMEMBER)->EnableWindow(iSelCount == 1 && iItemCount > 0);
    GetDlgItem(IDC_ADDMEMBER)->EnableWindow(TRUE);

    // Get selected item
    POSITION pos = m_FbMembers.GetFirstSelectedItemPosition();
    if(!pos)
    {
        return;
    }
    int iItem = m_FbMembers.GetNextSelectedItem(pos);
}

void CObjPropDutDecl::OnGetdispinfoMembers(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDisp = (LV_DISPINFO*)pNMHDR;
    *pResult = 0;

    if(pDisp->item.mask & LVIF_TEXT) {

        CLCMember* pMember = reinterpret_cast<CLCMember*>(pDisp->item.lParam);
        if(!pMember) {
            return;
        }

        CString strName,strType,strAlias,strComment,strInitVal;

        pMember->GetProperty(CLCMember::all,strName,strType,strAlias,strComment,strInitVal);


        if(pDisp->item.iSubItem == m_aiCol[COL_NAME]) {
            _tcsncpy(pDisp->item.pszText, strName,pDisp->item.cchTextMax);
            return;
        }

        if(pDisp->item.iSubItem == m_aiCol[COL_TYPE]) {
            _tcsncpy(pDisp->item.pszText, strType,pDisp->item.cchTextMax);
            return;
        }

        if(pDisp->item.iSubItem == m_aiCol[COL_ALIAS]) {
            _tcsncpy(pDisp->item.pszText, strAlias,pDisp->item.cchTextMax);
            return;
        }

        if(pDisp->item.iSubItem == m_aiCol[COL_COMMENT]) {
            _tcsncpy(pDisp->item.pszText, strComment,pDisp->item.cchTextMax);
            return;
        }

        if(pDisp->item.iSubItem == m_aiCol[COL_INITVAL]) {
            _tcsncpy(pDisp->item.pszText, strInitVal,pDisp->item.cchTextMax);
            return;
        }
   }
}


void CObjPropDutDecl::OnModifymember() 
{
	CString	strName;
	CString	strAlias;
	CString	strComment;
	CString	strType;
	CString	strInitVal;
    DWORD dwData;
    POSITION pos = NULL;
    int iItem = -1;

	// Get selected item
	pos = m_FbMembers.GetFirstSelectedItemPosition();
    if(!pos)
    {
        return;
    }
	
    iItem = m_FbMembers.GetNextSelectedItem(pos);
    
    if(iItem == -1)
    {
        return;
    }

    dwData = m_FbMembers.GetItemData(iItem);
    if(!dwData) {
        return;
    }

    CLCMember* pMember = reinterpret_cast<CLCMember*>(dwData);

    pMember->GetProperty(CLCMember::all,strName,strType,strAlias,strComment,strInitVal);

    CModifyDutMember aDutMember(strName, strAlias, strComment, strType, strInitVal);
    aDutMember.SetParent(this);

    switch(aDutMember.DoModal())
    {
        case IDOK:
            aDutMember.GetMemberDecl(strName, strAlias, strComment, strType, strInitVal);
            pMember->SetProperty(CLCMember::all,strName,strType,strAlias,strComment,strInitVal);
            break;
        case IDCANCEL:
            break;
        default:
            break;
    }
}

void CObjPropDutDecl::OnDeletemember() 
{
    POSITION pos;
    int iItem = -1;
    DWORD dwData = 0UL;

	pos = m_FbMembers.GetFirstSelectedItemPosition();
    if(!pos)
    {
        return;
    }
	
    iItem = m_FbMembers.GetNextSelectedItem(pos);
    
    if(iItem == -1)
    {
        return;
    }

    dwData = m_FbMembers.GetItemData(iItem);
    if(!dwData) {
        return;
    }

    CLCMember* pMember = reinterpret_cast<CLCMember*>(dwData);
    m_FbMembers.SetItemData(iItem,0UL);
    m_FbMembers.DeleteItem(iItem);
    pMember->Release();

    EnableButtons();
}

//------------------------------------------------------------------*
/**
 * Function name    CModifyDutMember::CModifyDutMember
 * Description      constructor for class CModifyDutMember
 * @return          -
 * @exception       -
 * @see             -
*/
CModifyDutMember::CModifyDutMember() : CDialog(CModifyDutMember::IDD)
{
    //{{AFX_DATA_INIT(CModifyDutMember)
    m_MemberAlias = _T("");
    m_MemberComment = _T("");
    m_MemberInitVal = _T("");
    m_MemberName = _T("");
    m_MemberType = _T("");
    //}}AFX_DATA_INIT
    m_pParent = NULL;
    SetCaption(TRUE);
}

//------------------------------------------------------------------*
/**
 * Function name    CModifyDutMember::CModifyDutMember
 * Description      constructor for class CModifyDutMember
 * @param           CString strName
 * @param           CString strAlias
 * @param           CString strComment
 * @param           CString strType
 * @param           CString strInitVal
 * @return          -
 * @exception       -
 * @see             -
*/
CModifyDutMember::CModifyDutMember(CString strName,
                                   CString strAlias,
                                   CString strComment,
                                   CString strType,
                                   CString strInitVal)
    : CDialog(CModifyDutMember::IDD)
{
    m_MemberInitVal = strInitVal;
    m_MemberName = strName;
    m_MemberAlias = strAlias;
    m_MemberComment = strComment;
    m_MemberType = strType;
    SetCaption(FALSE);
}

//------------------------------------------------------------------*
/**
 * Function name    CModifyDutMember::~CModifyDutMember
 * Description      destructor for class CModifyDutMember
 * @return          -
 * @exception       -
 * @see             -
*/
CModifyDutMember::~CModifyDutMember()
{
}

//------------------------------------------------------------------*
/**
 * Function name    CModifyDutMember::SetCaption
 * Description      sets the caption
 * @param           BOOL add
 * @return          -
 * @exception       -
 * @see             -
*/
void CModifyDutMember::SetCaption(BOOL add)
{
    if (add == TRUE)
    {
        m_caption.LoadString(IDS_ADD_MEMBER);
    }
    else
    {
        m_caption.LoadString(IDS_MOD_MEMBER);
    }
}

//------------------------------------------------------------------*
/**
 * Function name    CModifyDutMember::DoDataExchange
 * Description      exchanges and validates dialog data
 * @param           CDataExchange* pDX
 * @return          -
 * @exception       -
 * @see             -
*/
void CModifyDutMember::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CModifyDutMember)
    DDX_Text(pDX, IDC_MEMBER_ALIAS, m_MemberAlias);
    DDX_Text(pDX, IDC_MEMBER_COMMENT, m_MemberComment);
    DDX_Text(pDX, IDC_MEMBER_INITVAL, m_MemberInitVal);
    DDX_Text(pDX, IDC_MEMBER_NAME, m_MemberName);
    DDX_CBString(pDX, IDC_MEMBER_TYPE, m_MemberType);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModifyDutMember, CDialog)
    //{{AFX_MSG_MAP(CModifyDutMember)
    ON_BN_CLICKED(IDOK, OnOk)
    ON_BN_CLICKED(IDCANCEL, OnCancel)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModifyDutMember message handlers

//------------------------------------------------------------------*
/**
 * Function name    CModifyDutMember::OnOk
 * Description      message handler for the ok button
 * @return          -
 * @exception       -
 * @see             -
*/
void CModifyDutMember::OnOk() 
{
    DWORD dwModifier = 0;
    size_t size;
    CComBSTR sTypeName;
    CString strMsgText;
    CString str;
    BL_IEC_TYP type;
    bool bSubscript = false;
    HRESULT hr = S_OK;

    UpdateData(TRUE);

    hr = UTIL_CheckIdentifier(m_MemberName);
    if (FAILED(hr))
	{
        strMsgText.Format(IDS_OBJ_NAME_INVALID, m_MemberName);
        ::AfxMessageBox(strMsgText, MB_ICONWARNING);
        return;
	}

    
    //check for subscript.
    bSubscript = CLCConfig::GetSubscript(m_MemberType,str,size);
    if(bSubscript) {
        //only valid with STRING/WSTRING
        sTypeName = str;
        hr = BL_TypenameToDatatype(sTypeName,&type,&dwModifier);
        if(FAILED(hr)) {
            strMsgText.Format(IDS_INVALID_DATATYPE,str);
            ::AfxMessageBox(strMsgText,MB_ICONWARNING);
            return;

        }

        if(type != BL_STRING && type != BL_WSTRING) {
            //no subscription allowed.
            strMsgText.Format(IDS_INVALID_DATATYPE,str);
            ::AfxMessageBox(strMsgText,MB_ICONWARNING);
            return;
        }

    }
    else { //no subscription
        sTypeName = m_MemberType;
        hr = BL_TypenameToDatatype(sTypeName,&type,&dwModifier);
        if(FAILED(hr)) {
            strMsgText.Format(IDS_INVALID_DATATYPE,m_MemberType);
            ::AfxMessageBox(strMsgText,MB_ICONWARNING);
            return;
        }
    }

    if(m_pParent && m_pParent->OnCheckDuplicate(m_MemberName)) {
        //duplicate name
        strMsgText.Format(IDS_MEMBER_EXISTS,m_MemberName);
        ::AfxMessageBox(strMsgText, MB_ICONWARNING);
        return;
    }

    m_MemberInitVal.TrimLeft();
    m_MemberInitVal.TrimRight();

    /*
    if ((m_MemberInitVal.GetLength() != 0) && 
       (CheckTypeOfLiteral(m_MemberType, m_MemberInitVal, ErrorMessage) == FALSE))
    {
        CString strHelp;
        strHelp.LoadString(IDS_INIT_VALUE);
        ::AfxMessageBox(strHelp + ErrorMessage, MB_ICONWARNING);
        return;
    }
    */
    CDialog::OnOK();
}

//------------------------------------------------------------------*
/**
 * Function name    CModifyDutMember::OnCancel
 * Description      message handler for the cancel button
 * @return          -
 * @exception       -
 * @see             -
*/
void CModifyDutMember::OnCancel() 
{
    CDialog::OnCancel();
}

//------------------------------------------------------------------*
/**
 * Function name    CModifyDutMember::GetMemberDecl
 * Description      gets the declaration information
 * @param           CString& strName
 * @param           CString& strAlias
 * @param           CString& strComment
 * @param           CString& strType
 * @param           CString& strInitVal
 * @return          -
 * @exception       -
 * @see             -
*/
void CModifyDutMember::GetMemberDecl(CString& strName,
                                     CString& strAlias,
                                     CString& strComment,
                                     CString& strType,
                                     CString& strInitVal)
{
    strName = m_MemberName;
    strAlias = m_MemberAlias;
    strComment = m_MemberComment;
    strType = m_MemberType;
    strInitVal = m_MemberInitVal;

    return;
}

//------------------------------------------------------------------*
/**
 * Function name    CModifyDutMember::OnInitDialog
 * Description      message handler for the WM_INITDIALOG message
 * @return          BOOL
 * @exception       -
 * @see             -
*/
BOOL CModifyDutMember::OnInitDialog() 
{
    CDialog::OnInitDialog();

    SetWindowText(m_caption);

    if(m_pParent) {
        CLCConfig* pConfig = m_pParent->GetConfig();
        if(pConfig) {
            pConfig->AddConfigListener(this);
            pConfig->TraverseIECTypes(TraverseIECTypes);
            pConfig->RemoveConfigListener(this);
        }

    }

    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CModifyDutMember::OnIECType(int id, BL_IEC_TYP Type, CString & strType)
{
    CComboBox * pCBox = (CComboBox *)(this->GetDlgItem(IDC_MEMBER_TYPE));
    ASSERT(pCBox);
    int iIndex = pCBox->AddString(strType);
}

//------------------------------------------------------------------*
/**
 * Function name    CModifyDutMember::SetParent
 * Description      sets the member m_pParent
 * @param           CObjPropDutDecl* pParent
 * @return          -
 * @exception       -
 * @see             -
*/
void CModifyDutMember::SetParent(CObjPropDutDecl* pParent)
{
    m_pParent = pParent;
}
/////////////////////////////////////////////////////////////////////////////
// CDialogDataType

IMPLEMENT_DYNAMIC(CDialogDataType, CPropertySheet)

CDialogDataType::CDialogDataType(CLCConfig* pConfig, 
                                 CLCDataType* pDataType, 
                                 UINT nIDCaption, 
                                 CWnd* pParentWnd, UINT iSelectPage, 
                                 bool bProperty, int iColumnStrId)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage) 
    , m_DeclPage(bProperty,iColumnStrId) , m_GenPage(bProperty)
{
    m_bProperty = bProperty;
    InitPages(pDataType,pConfig);
}


CDialogDataType::~CDialogDataType()
{
}

void CDialogDataType::InitPages(CLCDataType* pDataType,CLCConfig* pConfig)
{
    CString strLang,strWiz;

    m_psh.dwFlags |= PSH_NOAPPLYNOW;

    if(!m_bProperty) { //insert
        AddPage(&m_WizPage);
        m_WizPage.SetDataType(pDataType);
        m_WizPage.SetConfig(pConfig);

        strLang.LoadString(IDS_POULANGUAGE_ST);
        strWiz.LoadString(IDS_WIZARDNAME_ST);
        m_WizPage.AddWizardName(strWiz,strLang);
    }
    
    AddPage(&m_GenPage);
    AddPage(&m_DeclPage);
    m_DeclPage.SetDataType(pDataType);
    m_DeclPage.SetConfig(pConfig);
    m_GenPage.SetDataType(pDataType);
    m_GenPage.SetConfig(pConfig);
}

BEGIN_MESSAGE_MAP(CDialogDataType, CPropertySheet)
	//{{AFX_MSG_MAP(CDialogDataType)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogDataType message handlers

void GetCurrentUserName(CString & UserName)
{
    TCHAR szUserName[_MAX_PATH] = _T("");
    DWORD Size = sizeof(szUserName) / sizeof(szUserName[0]);
    GetUserName(szUserName, &Size);
    UserName = szUserName;
}

