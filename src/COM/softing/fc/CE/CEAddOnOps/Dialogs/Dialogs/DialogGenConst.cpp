#include "stdafx.h"
#include "resource.h"
#include "DialogGenConst.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenConst::CDialogGenConst
 * Description      constructor for class CDialogGenConst
 * @param           DIALOGTYPE DialogType
 * @param           CPropertySheetBase *PropertySheet
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogGenConst::CDialogGenConst(DIALOGTYPE DialogType, 
                                 CPropertySheetBase *PropertySheet) :
    m_ObjPropGenConst(DialogType, PropertySheet),
    CDialogGenVarBase(m_ObjPropGenConst)
{
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenConst::~CDialogGenConst
 * Description      destructor for class CDialogGenConst
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogGenConst::~CDialogGenConst()
{
}

