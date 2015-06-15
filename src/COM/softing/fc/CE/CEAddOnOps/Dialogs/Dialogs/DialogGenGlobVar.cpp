
#include "stdafx.h"
#include "resource.h"
#include "DialogGenGlobVar.h"
#include "ObjPropGenGlobVar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenGlobVar::CDialogGenGlobVar
 * Description      constructor for class CDialogGenGlobVar
 * @param           DIALOGTYPE DialogType
 * @param           CPropertySheetBase *PropertySheet
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogGenGlobVar::CDialogGenGlobVar(DIALOGTYPE DialogType, 
                                     CPropertySheetBase *PropertySheet) :
    m_ObjPropGenGlobVar(DialogType, PropertySheet),
    CDialogGenVarBase(m_ObjPropGenGlobVar)
{
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenGlobVar::~CDialogGenGlobVar
 * Description      destructor for class CDialogGenGlobVar
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogGenGlobVar::~CDialogGenGlobVar()
{
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenGlobVar::GetModifier
 * Description      returns the modifier for the global variable
 * @return          CString
 * @exception       -
 * @see             -
*/
CString CDialogGenGlobVar::GetModifier()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
	return m_ObjPropGenGlobVar.m_Modifier;
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenGlobVar::SetModifier
 * Description      sets the modifier for the global variable
 * @param           CString Modifier
 * @return          -
 * @exception       -
 * @see             -
*/
void CDialogGenGlobVar::SetModifier(CString Modifier)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
	m_ObjPropGenGlobVar.m_Modifier = Modifier;
}

