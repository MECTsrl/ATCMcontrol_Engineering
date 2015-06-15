
#include "stdafx.h"
#include "resource.h"
#include "DialogGlobVarFile.h"
#include "HelpString.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//------------------------------------------------------------------*
/**
 * Function name    CDialogGlobVarFile::CDialogGlobVarFile
 * Description      constructor for class CDialogGlobVarFile
 * @param           DIALOGTYPE DialogType
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogGlobVarFile::CDialogGlobVarFile(DIALOGTYPE DialogType) :
    CPropertySheetBase(DialogType,
                       DialogType == CVIEWINSERT ? CHelpString(IDS_INS_GLOB_VAR_FILE):
                                                   CHelpString(IDS_PROP_GLOB_VAR_FILE)),
	CDialogGen(DialogType, this),
	CDialogInfo(DialogType, this),
	CDialogHelp(DialogType, this, (CDialogGenBase*)this, (CDialogInfo*)this)
{
	AddPage(&m_ObjPropGen);

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
 * Function name    CDialogGlobVarFile::~CDialogGlobVarFile
 * Description      constructor for class CDialogGlobVarFile
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogGlobVarFile::~CDialogGlobVarFile()
{
}

CREATE_DELETE_IMPL(CDialogGlobVarFile)
