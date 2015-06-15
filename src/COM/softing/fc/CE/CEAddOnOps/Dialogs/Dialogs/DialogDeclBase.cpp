
#include "stdafx.h"
#include "dialogs.h"
#include "DialogDeclBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//------------------------------------------------------------------*
/**
 * Function name    CDialogDeclBase::CDialogDeclBase
 * Description      constructor for class CDialogDeclBase
 * @param           CObjPropDeclBase &CObjPropDeclBase
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogDeclBase::CDialogDeclBase(CObjPropDeclBase &CObjPropDeclBase) :
    m_ObjPropDeclBase(CObjPropDeclBase)
{

}

//------------------------------------------------------------------*
/**
 * Function name    CDialogDeclBase::~CDialogDeclBase
 * Description      destructor for class CDialogDeclBase
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogDeclBase::~CDialogDeclBase()
{

}

//------------------------------------------------------------------*
/**
 * Function name    CDialogDeclBase::SetMembers
 * Description      sets the st variables
 * @param           SAFEARRAY * pMembers
 * @return          -
 * @exception       -
 * @see             -
*/
void CDialogDeclBase::SetMembers(SAFEARRAY * pMembers)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    if (m_ObjPropDeclBase.m_pMembers != NULL)
    {
        ::SafeArrayDestroy(m_ObjPropDeclBase.m_pMembers);
        m_ObjPropDeclBase.m_pMembers = NULL;
    }
    m_ObjPropDeclBase.m_pMembers = pMembers;
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogDeclBase::GetMembers
 * Description      returns the st variables
 * @return          SAFEARRAY*
 * @exception       -
 * @see             -
*/
SAFEARRAY * CDialogDeclBase::GetMembers()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
	SAFEARRAY* tmpMembers = m_ObjPropDeclBase.m_pMembers;
    m_ObjPropDeclBase.m_pMembers = NULL;
    return tmpMembers;
}
