
#include "stdafx.h"
#include "resource.h"
#include "DialogPB.h"
#include "HelpString.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//------------------------------------------------------------------*
/**
 * Function name    CDialogPB::CDialogPB
 * Description      constructor for class CDialogPB
 * @param           DIALOGTYPE DialogType
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogPB::CDialogPB(DIALOGTYPE DialogType):
    CPropertySheetBase(DialogType,
                       DialogType == CVIEWINSERT ? CHelpString(IDS_INS_PB_IO):
                                                   CHelpString(IDS_PROP_PB_IO)),

	CDialogGenPB(DialogType, this),
	CDialogInfo(DialogType, this),
	CDialogHelp(DialogType, this, (CDialogGenBase*)this, (CDialogInfo*)this)
{
	AddPage(&m_ObjPropGenPB);

    if (DialogType == CVIEWINSERT)
    {
        AddPage(&m_ObjPropHelp);
    }

	AddPage(&m_ObjPropInfo);
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogPB::~CDialogPB
 * Description      destructor for class CDialogPB
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogPB::~CDialogPB()
{
}

CREATE_DELETE_IMPL(CDialogPB)
