
#include "stdafx.h"
#include "Dialogs.h"
#include "ObjPropGenPc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenPc::CObjPropGenPc
 * Description      constructor for class CObjPropGenPc
 * @param           DIALOGTYPE DialogType
 * @param           CPropertySheetBase *PropertySheet
 * @return          -
 * @exception       -
 * @see             -
*/
CObjPropGenPc::CObjPropGenPc(DIALOGTYPE DialogType, 
                             CPropertySheetBase *PropertySheet) : 
    CObjPropGenBase(DialogType,
                    IDD_OBJPROP_GEN_PC,
                    PropertySheet)
{
	m_Address = _T("");
	m_Assignment = _T("");
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenPc::~CObjPropGenPc
 * Description      destructor for class CObjPropGenPc
 * @return          -
 * @exception       -
 * @see             -
*/
CObjPropGenPc::~CObjPropGenPc()
{
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenPc::SetResources
 * Description      sets the resources for the combobox
 * @param           CStringArray &Resources
 * @return          -
 * @exception       -
 * @see             -
*/
void CObjPropGenPc::SetResources(CStringArray &Resources)
{
    m_Resources.Copy(Resources);
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenPc::DoDataExchange
 * Description      exchanges and validates dialog data
 * @param           CDataExchange* pDX
 * @return          -
 * @exception       -
 * @see             -
*/
void CObjPropGenPc::DoDataExchange(CDataExchange* pDX)
{
	CObjPropGenBase::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ADDRESS, m_Address);
	DDX_CBString(pDX, IDC_ASSIGNMENT, m_Assignment);
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenPc::OnInitDialog
 * Description      message handler for the WM_INITDIALOG message
 * @return          BOOL
 * @exception       -
 * @see             -
*/
BOOL CObjPropGenPc::OnInitDialog() 
{
	CObjPropGenBase::OnInitDialog();

    FillAssignmentComboBox();

    if (m_DialogType == IVIEWPROP)
    {
        GetDlgItem(IDC_ADDRESS)->EnableWindow(FALSE);
        GetDlgItem(IDC_ASSIGNMENT)->EnableWindow(FALSE);
    }
    
    UpdateData(FALSE);
    
    return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenPc::FillAssignmentComboBox
 * Description      fills the combobox for the assignment
 * @return          -
 * @exception       -
 * @see             -
*/
void CObjPropGenPc::FillAssignmentComboBox()
{
    CComboBox * Assignment = (CComboBox *)(this->GetDlgItem(IDC_ASSIGNMENT));
    ASSERT(NULL != Assignment);
    Assignment->ResetContent();

    int Size = m_Resources.GetSize();
    for (int i = 0; i < Size; i++)
	{
        Assignment->AddString(m_Resources.GetAt(i));
    }
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenPc::OnKillActive
 * Description      is called when this property page is no longer the active page
 * @return          BOOL
 * @exception       -
 * @see             -
*/
BOOL CObjPropGenPc::OnKillActive()
{
    UpdateData(TRUE);
   	Trim(m_Address);
   	Trim(m_Assignment);
    UpdateData(FALSE);

    return CObjPropGenBase::OnKillActive();
}

