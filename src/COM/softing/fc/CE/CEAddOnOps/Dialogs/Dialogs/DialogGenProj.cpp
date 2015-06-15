
#include "stdafx.h"
#include "resource.h"
#include "DialogGenProj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenProj::CDialogGenProj
 * Description      constructor for class CDialogGenProj
 * @param           DIALOGTYPE DialogType
 * @param           CPropertySheetBase *PropertySheet
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogGenProj::CDialogGenProj(DIALOGTYPE DialogType, 
                                 CPropertySheetBase *PropertySheet) :
    m_ObjPropGenProj(DialogType, PropertySheet),
    CDialogGenBase(m_ObjPropGenProj)
{
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenProj::~CDialogGenProj
 * Description      destructor for class CDialogGenProj
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogGenProj::~CDialogGenProj()
{
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenProj::GetType
 * Description      returns the location of the project
 * @return          CString
 * @exception       -
 * @see             -
*/
CString CDialogGenProj::GetLocation()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
	return m_ObjPropGenProj.m_Location;
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenProj::SetType
 * Description      sets the location of the project
 * @param           CString Location
 * @return          -
 * @exception       -
 * @see             -
*/
void CDialogGenProj::SetLocation(CString Location)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
	m_ObjPropGenProj.m_Location = Location;
}

