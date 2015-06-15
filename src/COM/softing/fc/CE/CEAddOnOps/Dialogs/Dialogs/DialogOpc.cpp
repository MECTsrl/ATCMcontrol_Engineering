#include "stdafx.h"
#include "resource.h"
#include "DialogOpc.h"
#include "HelpString.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//------------------------------------------------------------------*
/**
 * Function name    CDialogOpc::CDialogOpc
 * Description      constructor for class CDialogOpc
 * @param           DIALOGTYPE DialogType
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogOpc::CDialogOpc(DIALOGTYPE DialogType):
    CPropertySheetBase(DialogType,
                       DialogType == CVIEWINSERT ? CHelpString(IDS_INS_OPC_IO):
                                                   CHelpString(IDS_PROP_OPC_IO)),

	CDialogGen(DialogType, this),
	CDialogInfo(DialogType, this),
	CDialogHelp(DialogType, this, (CDialogGenBase*)this, (CDialogInfo*)this)
{
	AddPage(&m_ObjPropGen);

    if (DialogType == CVIEWINSERT)
    {
        AddPage(&m_ObjPropHelp);
    }

	AddPage(&m_ObjPropInfo);
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogOpc::~CDialogOpc
 * Description      destructor for class CDialogOpc
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogOpc::~CDialogOpc()
{
}

CREATE_DELETE_IMPL(CDialogOpc)

