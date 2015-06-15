
#include "stdafx.h"
#include "resource.h"
#include "ObjPropGenConst.h"
#include "GlobConst.h"
#include "SyntaxCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenConst::CObjPropGenConst
 * Description      constructor for class CObjPropGenConst
 * @param           DIALOGTYPE DialogType
 * @param           CPropertySheetBase *PropertySheet
 * @return          -
 * @exception       -
 * @see             -
*/
CObjPropGenConst::CObjPropGenConst(DIALOGTYPE DialogType, CPropertySheetBase *PropertySheet) : 
    CObjPropGenVarBase(DialogType, 
                       PropertySheet,
//                       CObjPropGenConst::IDD)
                       IDD_OBJPROP_GEN_CONST)
{
    m_SyntaxErrorMessagePrefix.LoadString(IDS_CONST_VALUE);
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenConst::~CObjPropGenConst
 * Description      destructor for class CObjPropGenConst
 * @return          -
 * @exception       -
 * @see             -
*/
CObjPropGenConst::~CObjPropGenConst()
{
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenConst::OnKillActive
 * Description      is called when this property page is no longer the active page
 * @return          BOOL
 * @exception       -
 * @see             -
*/
BOOL CObjPropGenConst::OnKillActive()
{
    BOOL    bReturn = CObjPropGenVarBase::OnKillActive();
    if(!bReturn)
    {
        return bReturn;
    }

    // value must be set
    if(m_Value.IsEmpty())
    {
        CString strErrorMessage;
        CString strMessage;
        strMessage.LoadString(IDS_ERR_EMPTY_VALUE);
        strErrorMessage.Format("%s: %s", m_SyntaxErrorMessagePrefix, strMessage);
        ::AfxMessageBox(strErrorMessage, MB_ICONWARNING);

        CWnd*   pWnd = GetDlgItem(IDC_VALUE);
        if(pWnd)
        {
            GotoDlgCtrl(pWnd);
        }

        return FALSE;
    }
    return TRUE;
}

