
#include "stdafx.h"
#include "resource.h"
#include "DialogProj.h"
#include "HelpString.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//------------------------------------------------------------------*
/**
 * Function name    CDialogProj::CDialogProj
 * Description      constructor for class CDialogProj
 * @param           DIALOGTYPE DialogType
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogProj::CDialogProj(DIALOGTYPE DialogType) :
    CPropertySheetBase(DialogType, CHelpString(IDS_PROP_PROJECT)),
	CDialogGenProj(DialogType, this),
	CDialogInfo(DialogType, this)
{
	AddPage(&m_ObjPropGenProj);
    AddPage(&m_ObjPropInfo);
    m_ObjPropInfo.m_bShowUserHelp = FALSE;
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogProj::~CDialogProj
 * Description      destructor for class CDialogProj
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogProj::~CDialogProj()
{
}

CREATE_DELETE_IMPL(CDialogProj)
