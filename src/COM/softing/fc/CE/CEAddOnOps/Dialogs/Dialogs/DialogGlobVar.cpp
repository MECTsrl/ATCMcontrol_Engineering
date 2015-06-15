
#include "stdafx.h"
#include "resource.h"
#include "DialogGlobVar.h"
#include "GlobConst.h"
#include "HelpString.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//------------------------------------------------------------------*
/**
 * Function name    CDialogGlobVar::CDialogGlobVar
 * Description      constructor for class CDialogGlobVar
 * @param           DIALOGTYPE DialogType
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogGlobVar::CDialogGlobVar(DIALOGTYPE DialogType):
    CPropertySheetBase(DialogType,
                       DialogType == CVIEWINSERT ? CHelpString(IDS_INS_GLOB_VAR) :
                                                   CHelpString(IDS_PROP_GLOB_VAR)) ,

	CDialogGenGlobVar(DialogType, this)
{
	AddPage(&m_ObjPropGenGlobVar);
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGlobVar::~CDialogGlobVar
 * Description      destructor for class CDialogGlobVar
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogGlobVar::~CDialogGlobVar()
{
}

CREATE_DELETE_IMPL(CDialogGlobVar)
