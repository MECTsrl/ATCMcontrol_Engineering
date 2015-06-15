
#include "stdafx.h"
#include "Dialogs.h"
#include "ObjPropGenGlobVar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenGlobVar::CObjPropGenGlobVar
 * Description      constructor for class CObjPropGenGlobVar
 * @param           DIALOGTYPE DialogType
 * @param           CPropertySheetBase *PropertySheet
 * @return          -
 * @exception       -
 * @see             -
*/
CObjPropGenGlobVar::CObjPropGenGlobVar(DIALOGTYPE DialogType, CPropertySheetBase *PropertySheet) :
    CObjPropGenVarBase(DialogType, PropertySheet, IDD_OBJPROP_GEN_GLOBVAR)
{
	m_Modifier = _T("");
    m_SyntaxErrorMessagePrefix.LoadString(IDS_INIT_VALUE);
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenGlobVar::~CObjPropGenGlobVar
 * Description      destructor for class CObjPropGenGlobVar
 * @return          -
 * @exception       -
 * @see             -
*/
CObjPropGenGlobVar::~CObjPropGenGlobVar()
{
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenGlobVar::DoDataExchange
 * Description      exchanges and validates dialog data
 * @param           CDataExchange* pDX
 * @return          -
 * @exception       -
 * @see             -
*/
void CObjPropGenGlobVar::DoDataExchange(CDataExchange* pDX)
{
	CObjPropGenVarBase::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_COMBO2, m_Modifier);
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenGlobVar::OnInitDialog
 * Description      message handler for the WM_INITDIALOG message
 * @return          BOOL
 * @exception       -
 * @see             -
*/
BOOL CObjPropGenGlobVar::OnInitDialog() 
{
	CObjPropGenVarBase::OnInitDialog();

    if (m_DialogType == IVIEWPROP)
    {
        GetDlgItem(IDC_COMBO2)->EnableWindow(FALSE);
    }

    if ((m_DialogType == CVIEWINSERT) && (m_Modifier == _T("")))
    {
        m_Modifier = _T("VAR_GLOBAL");
    }
    
    UpdateData(FALSE);
    
    return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
