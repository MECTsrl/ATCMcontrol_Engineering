
#include "stdafx.h"
#include "dialogs.h"
#include "ObjPropFunctDecl.h"
#include "ModifyFbMember.h"
#include "extern.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//------------------------------------------------------------------*
/**
 * Function name    CObjPropFunctDecl::CObjPropFunctDecl
 * Description      constructor for class CObjPropFunctDecl
 * @param           DIALOGTYPE DialogType
 * @param           CPropertySheetBase *PropertySheet
 * @param           int iColumnStrId
 * @param           LPCTSTR pszSClass[]
 * @return          -
 * @exception       -
 * @see             -
*/
CObjPropFunctDecl::CObjPropFunctDecl(DIALOGTYPE DialogType,
                                     CPropertySheetBase *PropertySheet,
                                     int iColumnStrId,
                                     LPCTSTR pszSClass[]) :
    CObjPropDeclBase(DialogType, IDD_OBJPROP_FUNCTDECL, PropertySheet, iColumnStrId)
{
    m_FunctionType = _T("");

    m_pszSClasses = pszSClass;
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropFunctDecl::~CObjPropFunctDecl
 * Description      constructor for class CObjPropFunctDecl
 * @return          -
 * @exception       -
 * @see             -
*/
CObjPropFunctDecl::~CObjPropFunctDecl()
{

}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropFunctDecl::DoDataExchange
 * Description      exchanges and validates dialog data
 * @param           CDataExchange* pDX
 * @return          -
 * @exception       -
 * @see             -
*/
void CObjPropFunctDecl::DoDataExchange(CDataExchange* pDX)
{
	CObjPropDeclBase::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FUNCTTYPE, m_FunctionType);
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropFunctDecl::FillComboBox
 * Description      fills the combobox
 * @param           int iCBId
 * @param           const char * const pszItems[]
 * @param           int iDefault
 * @return          -
 * @exception       -
 * @see             -
*/
void CObjPropFunctDecl::FillComboBox(int iCBId, const char * const pszItems[], int iDefault)
{
	int iCount = 0;
    int iIndex = 0;
	CComboBox * pCBox = (CComboBox *)(this->GetDlgItem(iCBId));
	ASSERT(NULL != pCBox);
	pCBox->ResetContent();

	while(_tcscmp(pszItems[iCount], _T("")))
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

/////////////////////////////////////////////////////////////////////////////
// CObjPropFunctDecl message handlers

//------------------------------------------------------------------*
/**
 * Function name    CObjPropFunctDecl::OnInitDialog
 * Description      message handler for the WM_INITDIALOG message
 * @return          BOOL
 * @exception       -
 * @see             -
*/
BOOL CObjPropFunctDecl::OnInitDialog()
{
    CObjPropDeclBase::OnInitDialog();

    if(m_DialogType == IVIEWPROP)
    {
        GetDlgItem(IDC_FUNCTTYPE)->EnableWindow(FALSE);
    }

    const TCHAR* const *Types = GetSupportedBaseTypes();
    if(m_DialogType == CVIEWINSERT)
    {
        FillComboBox(IDC_FUNCTTYPE, Types, 0); // Return types
    }
    else
    {
        FillComboBox(IDC_FUNCTTYPE, Types); // Return types

        // Call after the combobox is filled with items, to display the value of m_FbmemberType!
        UpdateData(FALSE);
    }

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
