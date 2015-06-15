
#include "stdafx.h"
#include "resource.h"
#include "DialogProgInst.h"
#include "HelpString.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//------------------------------------------------------------------*
/**
 * Function name    CDialogProgInst::CDialogProgInst
 * Description      constructor for class CDialogProgInst
 * @param           DIALOGTYPE DialogType
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogProgInst::CDialogProgInst(DIALOGTYPE DialogType):
    CPropertySheetBase(DialogType,
                       DialogType == CVIEWINSERT ? CHelpString(IDS_INS_PROG_INST):
                                                   CHelpString(IDS_PROP_PROG_INST)),

	CDialogGenProgInst(DialogType, this),
	CDialogInfo(DialogType, this),
	CDialogHelp(DialogType, this, (CDialogGenBase*)this, (CDialogInfo*)this)
{
	AddPage(&m_ObjPropGenProgInst);

    // no user help for constant files
    //if (DialogType == CVIEWINSERT)
    //{
    //    AddPage(&m_ObjPropHelp);
    //}

	AddPage(&m_ObjPropInfo);
    m_ObjPropInfo.m_bShowUserHelp = FALSE;
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogProgInst::~CDialogProgInst
 * Description      destructor for class CDialogProgInst
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogProgInst::~CDialogProgInst()
{
}

CREATE_DELETE_IMPL(CDialogProgInst)

