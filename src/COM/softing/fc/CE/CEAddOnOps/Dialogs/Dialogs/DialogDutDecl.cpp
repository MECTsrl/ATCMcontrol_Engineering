#include "stdafx.h"
#include "dialogs.h"
#include "DialogDutDecl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//------------------------------------------------------------------*
/**
 * Function name    CDialogDutDecl::CDialogDutDecl
 * Description      constructor for class CDialogDutDecl
 * @param           DIALOGTYPE DialogType
 * @param           CPropertySheetBase *PropertySheet
 * @param           int iColumnStrId
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogDutDecl::CDialogDutDecl(DIALOGTYPE DialogType,
                               CPropertySheetBase *PropertySheet,
                               int iColumnStrId) :
    m_ObjPropDutDecl(DialogType, PropertySheet, iColumnStrId),
    CDialogDeclBase(m_ObjPropDutDecl)
{
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogDutDecl::~CDialogDutDecl
 * Description      destructor for class CDialogDutDecl
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogDutDecl::~CDialogDutDecl()
{
}

