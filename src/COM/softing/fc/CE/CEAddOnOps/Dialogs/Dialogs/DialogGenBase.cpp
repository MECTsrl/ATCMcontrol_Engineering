
#include "stdafx.h"
#include "resource.h"
#include "DialogGenBase.h"
#include "ObjPropWiz.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenBase::CDialogGenBase
 * Description      constructor for class CDialogGenBase
 * @param           CObjPropGenBase &ObjPropGenBase
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogGenBase::CDialogGenBase(CObjPropGenBase &ObjPropGenBase) :
    m_ObjPropGenBase(ObjPropGenBase)
{
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenBase::~CDialogGenBase
 * Description      destructor for class CDialogGenBase
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogGenBase::~CDialogGenBase()
{
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenBase::SetName
 * Description      sets the name of the st object
 * @param           CString Name
 * @return          -
 * @exception       -
 * @see             -
*/
void CDialogGenBase::SetName(CString Name)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    m_ObjPropGenBase.m_Name = Name;
    m_ObjPropGenBase.m_PropertySheet->ResetApplied();

    if (m_ObjPropGenBase.m_pObjPropWiz != NULL)
    {
        m_ObjPropGenBase.m_pObjPropWiz->m_Name = Name;
    }
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenBase::GetName
 * Description      returns the name of the st object
 * @return          CString
 * @exception       -
 * @see             -
*/
CString CDialogGenBase::GetName()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    return 	m_ObjPropGenBase.m_Name;
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenBase::SetAlias
 * Description      sets the alias for the st object
 * @param           CString Alias
 * @return          -
 * @exception       -
 * @see             -
*/
void CDialogGenBase::SetAlias(CString Alias)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    m_ObjPropGenBase.m_Alias = Alias;
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenBase::GetAlias
 * Description      returns the alias for the st object
 * @return          CString
 * @exception       -
 * @see             -
*/
CString CDialogGenBase::GetAlias()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
	return m_ObjPropGenBase.m_Alias;
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenBase::SetCreateDate
 * Description      sets the creation date of the st object
 * @param           CString CreateDate
 * @return          -
 * @exception       -
 * @see             -
*/
void CDialogGenBase::SetCreateDate(CString CreateDate)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    m_ObjPropGenBase.m_CreateDate = CreateDate;
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenBase::SetCreateUser
 * Description      sets the user who created the st object
 * @param           CString CreateUser
 * @return          -
 * @exception       -
 * @see             -
*/
void CDialogGenBase::SetCreateUser(CString CreateUser)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    m_ObjPropGenBase.m_CreateUser = CreateUser;
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenBase::SetLastModDate
 * Description      sets the modification date of the st object
 * @param           CString LastModDate
 * @return          -
 * @exception       -
 * @see             -
*/
void CDialogGenBase::SetLastModDate(CString LastModDate)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    m_ObjPropGenBase.m_LastModDate = LastModDate;
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenBase::SetLastModUser
 * Description      sets the user who modified of the st object
 * @param           CString LastModUser
 * @return          -
 * @exception       -
 * @see             -
*/
void CDialogGenBase::SetLastModUser(CString LastModUser)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    m_ObjPropGenBase.m_LastModUser = LastModUser;
}


