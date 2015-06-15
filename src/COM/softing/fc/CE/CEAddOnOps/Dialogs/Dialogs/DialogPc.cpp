
#include "stdafx.h"
#include "resource.h"
#include "DialogPc.h"
#include "HelpString.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//------------------------------------------------------------------*
/**
 * Function name    CDialogPc::CDialogPc
 * Description      constructor for class CDialogPc
 * @param           DIALOGTYPE DialogType
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogPc::CDialogPc(DIALOGTYPE DialogType):
    CPropertySheetBase(DialogType,
                       DialogType == CVIEWINSERT ? CHelpString(IDS_INS_PC):
                                                   CHelpString(IDS_PROP_PC)),
	CDialogGenPc(DialogType, this),
	CDialogInfo(DialogType, this),
	CDialogHelp(DialogType, this, (CDialogGenBase*)this, (CDialogInfo*)this)
{
	AddPage(&m_ObjPropGenPc);

    if (DialogType == CVIEWINSERT)
    {
        AddPage(&m_ObjPropHelp);
    }

	AddPage(&m_ObjPropInfo);
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogPc::~CDialogPc
 * Description      destructor for class CDialogPc
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogPc::~CDialogPc()
{
}

CREATE_DELETE_IMPL(CDialogPc)
