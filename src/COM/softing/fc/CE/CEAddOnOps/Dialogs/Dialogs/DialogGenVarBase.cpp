
#include "stdafx.h"
#include "resource.h"
#include "DialogGenVarBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenVarBase::CDialogGenVarBase
 * Description      constructor for class CDialogGenVarBase
 * @param           CObjPropGenVarBase &ObjPropGenVarBase
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogGenVarBase::CDialogGenVarBase(CObjPropGenVarBase &ObjPropGenVarBase) :
    m_ObjPropGenVarBase(ObjPropGenVarBase),
    CDialogGenBase(ObjPropGenVarBase)
{
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenVarBase::~CDialogGenVarBase
 * Description      destructor for class CDialogGenVarBase
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogGenVarBase::~CDialogGenVarBase()
{
}

/*
void CDialogGenVarBase::SetTypes(CStringArray &Types)
{
  	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    m_ObjPropGenVarBase.SetTypes(Types);
}
*/

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenVarBase::GetType
 * Description      returns the type
 * @return          CString
 * @exception       -
 * @see             -
*/
CString CDialogGenVarBase::GetType()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    return m_ObjPropGenVarBase.m_Type;
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenVarBase::SetType
 * Description      sets the type
 * @param           CString Type
 * @exception       -
 * @see             -
*/
void CDialogGenVarBase::SetType(CString Type)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
	m_ObjPropGenVarBase.m_Type = Type;
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenVarBase::GetValue
 * Description      returns the value
 * @return          CString
 * @exception       -
 * @see             -
*/
CString CDialogGenVarBase::GetValue()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
	return m_ObjPropGenVarBase.m_Value;
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenVarBase::SetValue
 * Description      sets the value
 * @param           CString Value
 * @exception       -
 * @see             -
*/
void CDialogGenVarBase::SetValue(CString Value)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
	m_ObjPropGenVarBase.m_Value = Value;
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenVarBase::GetVarComment
 * Description      returns the variable comment
 * @return          CString
 * @exception       -
 * @see             -
*/
CString CDialogGenVarBase::GetVarComment()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
	return m_ObjPropGenVarBase.m_VarComment;
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenVarBase::SetVarComment
 * Description      sets the variable comment
 * @param           CString VarComment
 * @exception       -
 * @see             -
*/
void CDialogGenVarBase::SetVarComment(CString VarComment)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
	m_ObjPropGenVarBase.m_VarComment = VarComment;
}

