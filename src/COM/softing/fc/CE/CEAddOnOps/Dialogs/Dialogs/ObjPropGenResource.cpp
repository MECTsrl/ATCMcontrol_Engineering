
#include "stdafx.h"
#include "Dialogs.h"
#include "ObjPropGenResource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenResource::CObjPropGenResource
 * Description      constructor for class CObjPropGenResource
 * @param           DIALOGTYPE DialogType
 * @param           CPropertySheetBase *PropertySheet
 * @return          -
 * @exception       -
 * @see             -
*/
CObjPropGenResource::CObjPropGenResource(DIALOGTYPE DialogType, 
                                         CPropertySheetBase *PropertySheet) : 
    CObjPropGenBase(DialogType,
                    IDD_OBJPROP_GEN_RESOURCE,
                    PropertySheet)
{
//	m_Type = _T("");
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenResource::~CObjPropGenResource
 * Description      destructor for class CObjPropGenResource
 * @return          -
 * @exception       -
 * @see             -
*/
CObjPropGenResource::~CObjPropGenResource()
{
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenResource::DoDataExchange
 * Description      exchanges and validates dialog data
 * @param           CDataExchange* pDX
 * @return          -
 * @exception       -
 * @see             -
*/
void CObjPropGenResource::DoDataExchange(CDataExchange* pDX)
{
	CObjPropGenBase::DoDataExchange(pDX);
//	DDX_Text(pDX, IDC_Type, m_Type);
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenResource::OnInitDialog
 * Description      message handler for the WM_INITDIALOG message
 * @return          BOOL
 * @exception       -
 * @see             -
*/
BOOL CObjPropGenResource::OnInitDialog() 
{
	CObjPropGenBase::OnInitDialog();

    if (m_DialogType == IVIEWPROP)
    {
        GetDlgItem(IDC_Type)->EnableWindow(FALSE);
    }
    
    UpdateData(FALSE);
    
    return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenConst::OnKillActive
 * Description      is called when this property page is no longer the active page
 * @return          BOOL
 * @exception       -
 * @see             -
*/
BOOL CObjPropGenResource::OnKillActive()
{
//    UpdateData(TRUE);
//   	Trim(m_Type);
//    UpdateData(FALSE);

    return CObjPropGenBase::OnKillActive();
}
