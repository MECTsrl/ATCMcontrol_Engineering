
#include "stdafx.h"
#include "resource.h"
#include "DialogGenProgInst.h"
#include "ObjPropGenProgInst.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenProgInst::CDialogGenProgInst
 * Description      constructor for class CDialogGenProgInst
 * @param           DIALOGTYPE DialogType
 * @param           CPropertySheetBase *PropertySheet
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogGenProgInst::CDialogGenProgInst(DIALOGTYPE DialogType, 
                               CPropertySheetBase *PropertySheet) :
    m_ObjPropGenProgInst(DialogType, PropertySheet),
    CDialogGenBase(m_ObjPropGenProgInst)
{
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenProgInst::~CDialogGenProgInst
 * Description      destructor for class CDialogGenProgInst
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogGenProgInst::~CDialogGenProgInst()
{
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenProgInst::GetProgClass
 * Description      returns the programm class
 * @return          CString
 * @exception       -
 * @see             -
*/
CString CDialogGenProgInst::GetProgClass()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
	return m_ObjPropGenProgInst.m_ProgramClass;
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenProgInst::SetProgClass
 * Description      sets the programm class
 * @param           CString ProgClass
 * @return          -
 * @exception       -
 * @see             -
*/
void CDialogGenProgInst::SetProgClass(CString ProgClass)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
	m_ObjPropGenProgInst.m_ProgramClass = ProgClass;
}

