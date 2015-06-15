
#include "stdafx.h"
#include "resource.h"
#include "DialogHelp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//------------------------------------------------------------------*
/**
 * Function name    CDialogHelp::CDialogHelp
 * Description      constructor for class CDialogHelp
 * @param           DIALOGTYPE DialogType
 * @param           CPropertySheetBase *PropertySheet
 * @param           CDialogGenBase *pDialogGenBase
 * @param           DCDialogInfo *pDialogInfo
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogHelp::CDialogHelp(DIALOGTYPE DialogType, 
                         CPropertySheetBase *PropertySheet,
                         CDialogGenBase *pDialogGenBase, 
                         CDialogInfo *pDialogInfo) :
    m_ObjPropHelp(DialogType, 
                  PropertySheet,
                  &(pDialogGenBase->m_ObjPropGenBase), 
                  &(pDialogInfo->m_ObjPropInfo))
{
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogHelp::~CDialogHelp
 * Description      destructor for class CDialogHelp
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogHelp::~CDialogHelp()
{
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogHelp::GetPathOfSelectedTemplate
 * Description      returns the path of the selected template for 
 *                  the user help
 * @return          CString
 * @exception       -
 * @see             -
*/
CString CDialogHelp::GetPathOfSelectedTemplate()
{
    if(m_ObjPropHelp.m_GenerateHelp)
    {
        return m_ObjPropHelp.m_PathOfSelectedTemplate;
    }
    else
    {
        CString strHelp = _T("");
        return strHelp;
    }
}
