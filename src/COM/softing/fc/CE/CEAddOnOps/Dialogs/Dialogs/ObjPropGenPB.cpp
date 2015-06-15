
#include "stdafx.h"
#include "resource.h"
#include "ObjPropGenPB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenPB::CObjPropGenPB
 * Description      constructor for class CObjPropGenPB
 * @param           DIALOGTYPE DialogType
 * @param           CPropertySheetBase *PropertySheet
 * @return          -
 * @exception       -
 * @see             -
*/
CObjPropGenPB::CObjPropGenPB(DIALOGTYPE DialogType, 
                             CPropertySheetBase *PropertySheet) : 
    CObjPropGenBase(DialogType, IDD_OBJPROP_GEN_PB, PropertySheet)
{
  	m_ChannelNumber = _T("");
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenPB::~CObjPropGenPB
 * Description      destructor for class CObjPropGenPB
 * @return          -
 * @exception       -
 * @see             -
*/
CObjPropGenPB::~CObjPropGenPB()
{
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenPB::OnInitDialog
 * Description      message handler for the WM_INITDIALOG message
 * @return          BOOL
 * @exception       -
 * @see             -
*/
BOOL CObjPropGenPB::OnInitDialog() 
{
	CObjPropGenBase::OnInitDialog();

    if (m_DialogType == IVIEWPROP)
    {
        GetDlgItem(IDC_COMBO1)->EnableWindow(FALSE);
    }

    if ((m_DialogType == CVIEWINSERT) && (m_ChannelNumber == _T("")))
    {
        m_ChannelNumber = _T("0");
    }
    
    UpdateData(FALSE);
    
    return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenPB::DoDataExchange
 * Description      exchanges and validates dialog data
 * @param           CDataExchange* pDX
 * @return          -
 * @exception       -
 * @see             -
*/
void CObjPropGenPB::DoDataExchange(CDataExchange* pDX)
{
	CObjPropGenBase::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_COMBO1, m_ChannelNumber);
}

