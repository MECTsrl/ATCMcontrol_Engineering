
#include "stdafx.h"
#include "resource.h"
#include "DialogGenPc.h"
#include "ObjPropGenPc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenPc::CDialogGenPc
 * Description      constructor for class CDialogGenPc
 * @param           DIALOGTYPE DialogType
 * @param           CPropertySheetBase *PropertySheet
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogGenPc::CDialogGenPc(DIALOGTYPE DialogType, 
                           CPropertySheetBase *PropertySheet) :
    m_ObjPropGenPc(DialogType, PropertySheet),
    CDialogGenBase(m_ObjPropGenPc)
{
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenPc::~CDialogGenPc
 * Description      destructor for class CDialogGenPc
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogGenPc::~CDialogGenPc()
{
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenPc::GetAddress
 * Description      returns the address of the pc
 * @return          CStrig
 * @exception       -
 * @see             -
*/
CString CDialogGenPc::GetAddress()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
	return m_ObjPropGenPc.m_Address;
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenPc::SetAddress
 * Description      sets the address of the pc
 * @param           CString Address
 * @return          -
 * @exception       -
 * @see             -
*/
void CDialogGenPc::SetAddress(CString Address)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
	m_ObjPropGenPc.m_Address = Address;
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenPc::SetResources
 * Description      sets the resources for the pc
 * @param           CStringArray &Resources
 * @return          -
 * @exception       -
 * @see             -
*/
void CDialogGenPc::SetResources(CStringArray &Resources)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
	m_ObjPropGenPc.SetResources(Resources);
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenPc::GetAssignment
 * Description      returns the assignment for the pc
 * @return          CString
 * @exception       -
 * @see             -
*/
CString CDialogGenPc::GetAssignment()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
	return m_ObjPropGenPc.m_Assignment;
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenPc::SetAssignment
 * Description      sets the assignment for the pc
 * @param           CString Assignment
 * @return          -
 * @exception       -
 * @see             -
*/
void CDialogGenPc::SetAssignment(CString Assignment)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
	m_ObjPropGenPc.m_Assignment = Assignment;
}

