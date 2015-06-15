
#include "stdafx.h"
#include "dialogs.h"
#include "ModifyDutMember.h"
#include "extern.h"
#include "SyntaxCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
    CString strMsgText;
    CString strCaption;

    UpdateData(TRUE);
    if(m_MemberName.IsEmpty())
	{
        AfxMessageBox(IDS_INVALID_NAME,
            MB_OK | MB_ICONEXCLAMATION);
        return;
	}
    if(m_MemberType.IsEmpty())
    {
        AfxMessageBox(IDS_INVALID_DATATYPE,
            MB_OK | MB_ICONEXCLAMATION);
        return;
    }
    if(m_pParent)
    {
        if(!m_pParent->DoesVarNameExist(m_MemberName, 0))
        return;
    }

    m_MemberInitVal.TrimLeft();
    m_MemberInitVal.TrimRight();
    CString ErrorMessage = _T("");
    if ((m_MemberInitVal.GetLength() != 0) && 
        (CheckTypeOfLiteral(m_MemberType, m_MemberInitVal, ErrorMessage) == FALSE))
    {
        CString strHelp;
        strHelp.LoadString(IDS_INIT_VALUE);
        ::AfxMessageBox(strHelp + ErrorMessage, MB_ICONWARNING);
        return;
    }

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

    const TCHAR* const *Types = GetSupportedBaseTypes();
    FillComboBox(IDC_MEMBER_TYPE, Types);                  // Types

    // Call after the combobox is filled with items, to display the value of m_FbmemberType!
    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

//------------------------------------------------------------------*
/**
 * Function name    CModifyDutMember::FillComboBox
 * Description      fills the combobox
 * @param           int iCBId
 * @param           const char * const pszItems[]
 * @param           int iDefault
 * @return          -
 * @exception       -
 * @see             -
*/
void CModifyDutMember::FillComboBox(int iCBId, const char * const pszItems[], int iDefault)
{
    int iCount = 0;
    int iIndex = 0;
    CComboBox * pCBox = (CComboBox *)(this->GetDlgItem(iCBId));
    ASSERT(NULL != pCBox);
    pCBox->ResetContent();

    while(_tcscmp(pszItems[iCount], _T("")) != 0)
	{
        iIndex = pCBox->AddString(pszItems[iCount]);
        pCBox->SetItemData(iIndex, (DWORD)iCount);
        iCount++;
    }

    if(iDefault >= 0)
    {
        pCBox->SetCurSel(iDefault);
    }
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

