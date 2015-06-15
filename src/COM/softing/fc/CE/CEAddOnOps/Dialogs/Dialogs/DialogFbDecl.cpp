
#include "stdafx.h"
#include "resource.h"
#include "DialogFbDecl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//------------------------------------------------------------------*
/**
 * Function name    CDialogFbDecl::CDialogFbDecl
 * Description      constructor for class CDialogFbDecl
 * @param           DIALOGTYPE DialogType
 * @param           CPropertySheetBase *PropertySheet
 * @param           int iColumnStrId
 * @param           LPCTSTR pszSClass[]
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogFbDecl::CDialogFbDecl(DIALOGTYPE DialogType,
                             CPropertySheetBase *PropertySheet,
                             int iColumnStrId,
                             LPCTSTR pszSClass[]) :
    m_ObjPropFbDecl(DialogType, PropertySheet, iColumnStrId, pszSClass),
    CDialogDeclBase(m_ObjPropFbDecl)
{
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogFbDecl::~CDialogFbDecl
 * Description      destructor for class CDialogFbDecl
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogFbDecl::~CDialogFbDecl()
{
}

