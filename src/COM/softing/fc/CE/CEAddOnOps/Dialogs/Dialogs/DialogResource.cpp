
#include "stdafx.h"
#include "resource.h"
#include "DialogResource.h"
#include "HelpString.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//------------------------------------------------------------------*
/**
 * Function name    CDialogResource::CDialogResource
 * Description      constructor for class CDialogResource
 * @param           DIALOGTYPE DialogType
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogResource::CDialogResource(DIALOGTYPE DialogType):
    CPropertySheetBase(DialogType,
                       DialogType == CVIEWINSERT ? CHelpString(IDS_INS_RESOURCE):
                                                   CHelpString(IDS_PROP_RESOURCE)),
	CDialogGenResource(DialogType, this),
	CDialogInfo(DialogType, this),
	CDialogHelp(DialogType, this, (CDialogGenBase*)this, (CDialogInfo*)this)
{
	AddPage(&m_ObjPropGenResource);

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
 * Function name    CDialogResource::~CDialogResource
 * Description      constructor for class CDialogResource
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogResource::~CDialogResource()
{
}

CREATE_DELETE_IMPL(CDialogResource)

