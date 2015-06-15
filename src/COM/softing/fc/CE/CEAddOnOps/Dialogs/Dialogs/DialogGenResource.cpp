
#include "stdafx.h"
#include "resource.h"
#include "DialogGenResource.h"
#include "ObjPropGenResource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenResource::CDialogGenResource
 * Description      constructor for class CDialogGenResource
 * @param           DIALOGTYPE DialogType
 * @param           CPropertySheetBase *PropertySheet
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogGenResource::CDialogGenResource(DIALOGTYPE DialogType, 
                                       CPropertySheetBase *PropertySheet) :
    m_ObjPropGenResource(DialogType, PropertySheet),
    CDialogGenBase(m_ObjPropGenResource)
{
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenResource::~CDialogGenResource
 * Description      destructor for class CDialogGenResource
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogGenResource::~CDialogGenResource()
{
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenResource::GetType
 * Description      returns the type of the resource
 * @return          CString
 * @exception       -
 * @see             -
*/
//CString CDialogGenResource::GetType()
//{
//    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
//	return m_ObjPropGenResource.m_Type;
//}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenResource::SetType
 * Description      sets the type of the resource
 * @param           CString Type
 * @return          -
 * @exception       -
 * @see             -
*/
//void CDialogGenResource::SetType(CString Type)
//{
//    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
//	m_ObjPropGenResource.m_Type = Type;
//}

