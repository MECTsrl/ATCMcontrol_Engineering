
#include "stdafx.h"
#include "dialogs.h"
#include "DialogFunctDecl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//------------------------------------------------------------------*
/**
 * Function name    CDialogFunctDecl::CDialogFunctDecl
 * Description      constructor for class CDialogFunctDecl
 * @param           DIALOGTYPE DialogType
 * @param           CPropertySheetBase *PropertySheet
 * @param           int iColumnStrId
 * @param           LPCTSTR pszSClass[]
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogFunctDecl::CDialogFunctDecl(DIALOGTYPE DialogType,
                                   CPropertySheetBase *PropertySheet,
                                   int iColumnStrId,
                                   LPCTSTR pszSClass[]) :
    m_ObjPropFunctDecl(DialogType, PropertySheet, iColumnStrId, pszSClass),
    CDialogDeclBase(m_ObjPropFunctDecl)
{

}

//------------------------------------------------------------------*
/**
 * Function name    CDialogFunctDecl::~CDialogFunctDecl
 * Description      destructor for class CDialogFunctDecl
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogFunctDecl::~CDialogFunctDecl()
{

}

//------------------------------------------------------------------*
/**
 * Function name    CDialogFunctDecl::GetReturnType
 * Description      returns the return type of the function
 * @return          CString
 * @exception       -
 * @see             -
*/
CString CDialogFunctDecl::GetReturnType()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
   return m_ObjPropFunctDecl.m_FunctionType;
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogFunctDecl::SetReturnType
 * Description      sets the return type of the function
 * @param           CString strType
 * @return          -
 * @exception       -
 * @see             -
*/
void CDialogFunctDecl::SetReturnType(CString strType)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    m_ObjPropFunctDecl.m_FunctionType = strType;
}
