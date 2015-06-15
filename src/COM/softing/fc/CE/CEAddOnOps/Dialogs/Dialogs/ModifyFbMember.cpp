
#include "stdafx.h"
#include "resource.h"
#include "ModifyFbMember.h"
#include "extern.h"
#include "SyntaxCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModifyFbMember

//------------------------------------------------------------------*
/**
 * Function name    CModifyFbMember::CModifyFbMember
 * Description      constructor for class CModifyFbMember
 * @return          -
 * @exception       -
 * @see             -
*/
CModifyFbMember::CModifyFbMember() : CDialog(CModifyFbMember::IDD)
{
	//{{AFX_DATA_INIT(CModifyFbMember)
	m_FbMemberName = _T("");
	m_FbMemberAlias = _T("");
	m_FbMemberComment = _T("");
	m_FbMemberType = _T("");
	m_FbMemberInitVal = _T("");
	m_FbMemberSclass = 0;
	//}}AFX_DATA_INIT
    m_pszSClasses = NULL;
    SetCaption(TRUE);
}

//------------------------------------------------------------------*
/**
 * Function name    CModifyFbMember::CModifyFbMember
 * Description      constructor for class CModifyFbMember
 * @param           LPCTSTR pszSClasses[]
 * @return          -
 * @exception       -
 * @see             -
*/
CModifyFbMember::CModifyFbMember(LPCTSTR pszSClasses[]) : CDialog(CModifyFbMember::IDD)
{
	m_FbMemberName = _T("");
	m_FbMemberAlias = _T("");
	m_FbMemberComment = _T("");
	m_FbMemberType = _T("");
	m_FbMemberInitVal = _T("");
	m_FbMemberSclass = 0;

    m_pszSClasses = pszSClasses;
    SetCaption(TRUE);
}

//------------------------------------------------------------------*
/**
 * Function name    CModifyFbMember::CModifyFbMember
 * Description      constructor for class CModifyFbMember
 * @param           LPCTSTR pszSClasses[]
 * @param           CString strName
 * @param           CString strAlias
 * @param           CString strComment
 * @param           CString strType
 * @param           CString strInitVal
 * @param           CString strSclass
 * @return          -
 * @exception       -
 * @see             -
*/
CModifyFbMember::CModifyFbMember(LPCTSTR pszSClasses[],
                                 CString strName,
                                 CString strAlias,
                                 CString strComment,
                                 CString strType,
                                 CString strInitVal,
                                 CString strSclass)
	 : CDialog(CModifyFbMember::IDD)
{
    m_pszSClasses = pszSClasses;
    m_pParent = NULL;

    int iCount = 0;
	m_FbMemberInitVal = strInitVal;
	m_FbMemberName = strName;
	m_FbMemberAlias = strAlias;
	m_FbMemberComment = strComment;
	m_FbMemberType = strType;
    while(_tcscmp(m_pszSClasses[iCount], _T("")) != 0 && m_pszSClasses[iCount] != strSclass)
    {
        iCount++;
    }
   	m_FbMemberSclass = iCount;
    SetCaption(FALSE);
}

//------------------------------------------------------------------*
/**
 * Function name    CModifyFbMember::~CModifyFbMember
 * Description      destructor for class CModifyFbMember
 * @return          -
 * @exception       -
 * @see             -
*/
CModifyFbMember::~CModifyFbMember()
{
}

//------------------------------------------------------------------*
/**
 * Function name    CModifyFbMember::SetCaption
 * Description      sets the caption
 * @param           BOOL add
 * @return          -
 * @exception       -
 * @see             -
*/
void CModifyFbMember::SetCaption(BOOL add)
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
 * Function name    CModifyFbMember::DoDataExchange
 * Description      exchanges and validates dialog data
 * @param           CDataExchange* pDX
 * @return          -
 * @exception       -
 * @see             -
*/
void CModifyFbMember::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModifyFbMember)
	DDX_Control(pDX, IDOK, m_Ok);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Text(pDX, IDC_FBMEMBER_INITVAL, m_FbMemberInitVal);
	DDX_Text(pDX, IDC_FBMEMBER_NAME, m_FbMemberName);
	DDX_CBString(pDX, IDC_FBMEMBER_TYPE, m_FbMemberType);
	DDX_CBIndex(pDX, IDC_FBMEMBER_SCLASS, m_FbMemberSclass);
	DDX_Text(pDX, IDC_FBMEMBER_ALIAS, m_FbMemberAlias);
	DDX_Text(pDX, IDC_FBMEMBER_COMMENT, m_FbMemberComment);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModifyFbMember, CDialog)
	//{{AFX_MSG_MAP(CModifyFbMember)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModifyFbMember message handlers

//------------------------------------------------------------------*
/**
 * Function name    CModifyFbMember::OnOk
 * Description      message handler for the ok button
 * @return          -
 * @exception       -
 * @see             -
*/
void CModifyFbMember::OnOk() 
{
	CString strMsgText;
	CString strCaption;

	UpdateData(TRUE);
	if(m_FbMemberName.IsEmpty())
	{
		AfxMessageBox(IDS_INVALID_NAME,
			MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	if(m_FbMemberType.IsEmpty())
	{
		AfxMessageBox(IDS_INVALID_DATATYPE,
			MB_OK | MB_ICONEXCLAMATION);
		return;
	}
    if(m_pParent)
    {
        if(!m_pParent->DoesVarNameExist(m_FbMemberName, 1))
        return;
    }

    CString ErrorMessage = _T("");
    m_FbMemberInitVal.TrimLeft();
    m_FbMemberInitVal.TrimRight();
    if ((m_FbMemberInitVal.GetLength() != 0) && 
        (CheckTypeOfLiteral(m_FbMemberType, m_FbMemberInitVal, ErrorMessage) == FALSE))
    {
        CString strHelp;
        strHelp.LoadString(IDS_INIT_VALUE);
        ::AfxMessageBox(strHelp + ErrorMessage, MB_ICONWARNING);
        return;
    }

    // check storage class for BOOL R_EDGE and BOOL F_EDGE
    if(m_FbMemberType.CompareNoCase(_T("BOOL R_EDGE")) == 0 ||
        m_FbMemberType.CompareNoCase(_T("BOOL F_EDGE")) == 0)
    {
        CString strSClass;
        CComboBox*  pCombo = (CComboBox*) GetDlgItem(IDC_FBMEMBER_SCLASS);
        pCombo->GetWindowText(strSClass);
        if(strSClass.Left(9).CompareNoCase(_T("VAR_INPUT")) != 0)
        {
            GotoDlgCtrl(pCombo);
            ::AfxMessageBox(IDS_INVALID_STORAGE_CLASS, MB_OK | MB_ICONEXCLAMATION);
            return;
        }
    }

	CDialog::OnOK();
}

//------------------------------------------------------------------*
/**
 * Function name    CModifyFbMember::OnCancel
 * Description      message handler for the cancel button
 * @return          -
 * @exception       -
 * @see             -
*/
void CModifyFbMember::OnCancel() 
{
	CDialog::OnCancel();
}

//------------------------------------------------------------------*
/**
 * Function name    CModifyFbMember::GetMemberDecl
 * Description      gets the declaration information
 * @param           CString& strName
 * @param           CString& strAlias
 * @param           CString& strComment
 * @param           CString& strType
 * @param           CString& strInitVal
 * @param           CString& strSClass
 * @return          -
 * @exception       -
 * @see             -
*/
void CModifyFbMember::GetMemberDecl(CString& strName,
                                    CString& strAlias,
  									CString& strComment,
									CString& strType,
									CString& strInitVal,
									CString& strSClass)
{
	strName = m_FbMemberName;
	strAlias = m_FbMemberAlias;
	strComment = m_FbMemberComment;
	strType = m_FbMemberType;
	strInitVal = m_FbMemberInitVal;
    if(m_pszSClasses)
    {
	    strSClass = m_pszSClasses[m_FbMemberSclass];
    }

	return;
}

//------------------------------------------------------------------*
/**
 * Function name    CModifyFbMember::OnInitDialog
 * Description      message handler for the WM_INITDIALOG message
 * @return          BOOL
 * @exception       -
 * @see             -
*/
BOOL CModifyFbMember::OnInitDialog() 
{
    CDialog::OnInitDialog();

    SetWindowText(m_caption);

    const TCHAR* const *Types = GetSupportedBaseTypes();
	FillComboBox(IDC_FBMEMBER_TYPE, Types);                  // Types
    if(m_pszSClasses)
    {
        BOOL bIsRetainSupp = IsRetainSupported();
		FillComboBox(IDC_FBMEMBER_SCLASS, m_pszSClasses, 0, bIsRetainSupp); // Storage Classes
    }

    // Call after the combobox is filled with items, to display the value of m_FbmemberType!
    UpdateData(FALSE);

	return TRUE;
}

//------------------------------------------------------------------*
/**
 * Function name    CModifyFbMember::FillComboBox
 * Description      fills the combobox
 * @param           int iCBId
 * @param           const char * const pszItems[]
 * @param           int iDefault
 * @return          -
 * @exception       -
 * @see             -
*/
void CModifyFbMember::FillComboBox(int iCBId, const char * const pszItems[], int iDefault, BOOL bIsRetainSupp)
{
	int iCount = 0;
    int iIndex = 0;
	CComboBox * pCBox = (CComboBox *)(this->GetDlgItem(iCBId));
	ASSERT(NULL != pCBox);
	pCBox->ResetContent();

	while(_tcscmp(pszItems[iCount], _T("")) != 0)
	{
        CString str = pszItems[iCount];
        if (!bIsRetainSupp)
        {
            if (str.GetLength()>((CString)_T("RETAIN")).GetLength())
            {
                CString strRight = str.Right(((CString)_T("RETAIN")).GetLength());
                if (strRight.CompareNoCase(_T("RETAIN"))==0)
                {
                    str.Empty();
                }
            }
        }
        if (!str.IsEmpty())
        {
		    iIndex = pCBox->AddString(pszItems[iCount]);
		    pCBox->SetItemData(iIndex, (DWORD)iCount);
        }
        iCount++;
	}

    if(iDefault >= 0)
	{
        pCBox->SetCurSel(iDefault);
	}
}

//------------------------------------------------------------------*
/**
 * Function name    CModifyFbMember::SetParent
 * Description      sets the member m_pParent
 * @param           CObjPropDutDecl* pParent
 * @return          -
 * @exception       -
 * @see             -
*/
void CModifyFbMember::SetParent(CObjPropDeclBase* pParent)
{
    m_pParent = pParent;
}

