
#include "stdafx.h"
#include "resource.h"
#include "DialogConstantFile.h"
#include "HelpString.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//------------------------------------------------------------------*
/**
 * Function name    CDialogConstantFile::CDialogConstantFile
 * Description      constructor for class CDialogConstantFile
 * @param           DIALOGTYPE DialogType
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogConstantFile::CDialogConstantFile(DIALOGTYPE DialogType) :
    CPropertySheetBase(DialogType,
                       DialogType == CVIEWINSERT ? CHelpString(IDS_INS_CONST_FILE):
                                                   CHelpString(IDS_PROP_CONST_FILE)),
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
 * Function name    CDialogConstantFile::~CDialogConstantFile
 * Description      destructor for class CDialogConstantFile
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogConstantFile::~CDialogConstantFile()
{
}

CREATE_DELETE_IMPL(CDialogConstantFile)

