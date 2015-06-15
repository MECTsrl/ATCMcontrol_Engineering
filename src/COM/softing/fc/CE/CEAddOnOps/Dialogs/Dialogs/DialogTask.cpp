
#include "stdafx.h"
#include "resource.h"
#include "DialogTask.h"
#include "HelpString.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//------------------------------------------------------------------*
/**
 * Function name    CDialogTask::CDialogTask
 * Description      constructor for class CDialogTask
 * @param           DIALOGTYPE DialogType
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogTask::CDialogTask(DIALOGTYPE DialogType):
    CPropertySheetBase(DialogType,
                       DialogType == CVIEWINSERT ? CHelpString(IDS_INS_TASK):
                                                   CHelpString(IDS_PROP_TASK)),

	CDialogGenTask(DialogType, this),
	CDialogInfo(DialogType, this),
	CDialogHelp(DialogType, this, (CDialogGenBase*)this, (CDialogInfo*)this)
{
	AddPage(&m_ObjPropGenTask);

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
 * Function name    CDialogTask::~CDialogTask
 * Description      destructor for class CDialogTask
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogTask::~CDialogTask()
{
}

CREATE_DELETE_IMPL(CDialogTask)

