
#include "stdafx.h"
#include "resource.h"
#include "ObjPropGenVarBase.h"
#include "GlobConst.h"
#include "SyntaxCheck.h"
#include "extern.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenVarBase::CObjPropGenVarBase
 * Description      constructor for class CObjPropGenVarBase
 * @param           DIALOGTYPE DialogType
 * @param           CPropertySheetBase *PropertySheet
 * @param           UINT nIDTemplate
 * @return          -
 * @exception       -
 * @see             -
*/
CObjPropGenVarBase::CObjPropGenVarBase(DIALOGTYPE DialogType, 
                                       CPropertySheetBase *PropertySheet,
                                       UINT nIDTemplate) : 
    CObjPropGenBase(DialogType, nIDTemplate, PropertySheet)
{
  	m_Type = _T("");
	m_Value = _T("");
	m_VarComment = _T("");
    m_SyntaxErrorMessagePrefix.LoadString(IDS_VALUE);
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenVarBase::~CObjPropGenVarBase
 * Description      destructor for class CObjPropGenVarBase
 * @return          -
 * @exception       -
 * @see             -
*/
CObjPropGenVarBase::~CObjPropGenVarBase()
{
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenVarBase::OnInitDialog
 * Description      message handler for the WM_INITDIALOG message
 * @return          BOOL
 * @exception       -
 * @see             -
*/
BOOL CObjPropGenVarBase::OnInitDialog() 
{
	CObjPropGenBase::OnInitDialog();

    FillTypesComboBox();

    if (m_DialogType == IVIEWPROP)
    {
        GetDlgItem(IDC_COMBO1)->EnableWindow(FALSE);
        GetDlgItem(IDC_INITIAL)->EnableWindow(FALSE);
    }

    if ((m_DialogType == CVIEWINSERT) && (m_Type == _T("")))
    {
        m_Type = _T("BOOL");
    }
    
    UpdateData(FALSE);
    
    return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenVarBase::DoDataExchange
 * Description      exchanges and validates dialog data
 * @param           CDataExchange* pDX
 * @return          -
 * @exception       -
 * @see             -
*/
void CObjPropGenVarBase::DoDataExchange(CDataExchange* pDX)
{
	CObjPropGenBase::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_COMBO1, m_Type);
	DDX_Text(pDX, IDC_VALUE, m_Value);
	DDX_Text(pDX, IDC_COMMENT, m_VarComment);
}


//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenVarBase::FillTypesComboBox
 * Description      fills the types combobox
 * @return          -
 * @exception       -
 * @see             -
*/
void CObjPropGenVarBase::FillTypesComboBox()
{
    CComboBox * pCBox = (CComboBox *)(this->GetDlgItem(IDC_COMBO1));
    ASSERT(NULL != pCBox);
    pCBox->ResetContent();

    const TCHAR* const *Types = GetSupportedBaseTypes();
    int iIndex = 0;
    int iCount = 0;
    while(_tcscmp(Types[iCount], _T("")) != 0)
	{
        iIndex = pCBox->AddString(Types[iCount]);
        pCBox->SetItemData(iIndex, (DWORD)iCount);
        iCount++;
    }
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenVarBase::OnKillActive
 * Description      is called when this property page is no longer the active page
 * @return          BOOL
 * @exception       -
 * @see             -
*/
BOOL CObjPropGenVarBase::OnKillActive()
{
    UpdateData(TRUE);

    if (m_Type != _T("WSTRING"))
    {
        Trim(m_Value);
        UpdateData(FALSE);
    }

    CString ErrorMessage;
    m_Value.TrimLeft();
    m_Value.TrimRight();
    if ((m_Value.GetLength() != 0) && (CheckTypeOfLiteral(m_Type, m_Value, ErrorMessage) == FALSE))
    {
        ::AfxMessageBox(m_SyntaxErrorMessagePrefix + _T(": ") + ErrorMessage, MB_ICONWARNING);
        return FALSE;
    }

    return CObjPropGenBase::OnKillActive();
}

