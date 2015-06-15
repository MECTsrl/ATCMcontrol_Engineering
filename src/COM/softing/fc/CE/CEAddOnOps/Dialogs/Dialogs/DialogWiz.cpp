
// DialogWiz.cpp: implementation of the CDialogWiz class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "DialogWiz.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//------------------------------------------------------------------*
/**
 * Function name    CDialogWiz::CDialogWiz
 * Description      constructor for class CDialogWiz
 * @param           DIALOGTYPE DialogType
 * @param           CPropertySheetBase *PropertySheet
 * @param           CDialogGenBase *pDialogGenBase
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogWiz::CDialogWiz(DIALOGTYPE DialogType, 
                       CPropertySheetBase *PropertySheet,
                       CDialogGenBase *pDialogGenBase) :
    m_ObjPropWiz(DialogType, 
                 PropertySheet,
                 &(pDialogGenBase->m_ObjPropGenBase))
{
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogWiz::~CDialogWiz
 * Description      destructor for class CDialogWiz
 * @param           CStringArray IecLanguages
 * @param           CStringArray PouLanguages
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogWiz::~CDialogWiz()
{
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogWiz::~CDialogWiz
 * Description      initializes the dialog with the wizard names 
 *                  for the intersection of the languages
 * @param           CStringArray& IecLanguages 
 * @param           CStringArray& PouLanguages 
 * @param           CStringArray& WizardNames 
 * @return          -
 * @exception       -
 * @see             -
*/
void CDialogWiz::InitializeDialogWiz(CStringArray& IecLanguages, 
                                     CStringArray& PouLanguages, 
                                     CStringArray& WizardNames)
{
    //ASSERT(PouLanguages.GetUpperBound() == WizardNames.GetUpperBound());

    for (int j = 0; j <= PouLanguages.GetUpperBound(); j++)
    {
        for (int k = 0; k <= IecLanguages.GetUpperBound(); k++)
        {
            if (IecLanguages.GetAt(k).CompareNoCase(PouLanguages.GetAt(j)) == 0)
            {
                AddWizardName(WizardNames.GetAt(j), PouLanguages.GetAt(j));
                break;
            }
        }
    }

}

//------------------------------------------------------------------*
/**
 * Function name    CDialogWiz::AddWizardName
 * Description      adds a wizard name and the corresponding iec language
 *                  to the lists of wizard names
 * @param           LPCTSTR lpszWizardName
 * @param           LPCTSTR lpszLanguage
 * @return          -
 * @exception       -
 * @see             -
*/
void CDialogWiz::AddWizardName(LPCTSTR lpszWizardName, LPCTSTR lpszLanguage)
{
    m_ObjPropWiz.AddWizardName(lpszWizardName, lpszLanguage);
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogWiz::GetIndexOfSelectedWizardName
 * Description      returns the index of the selected wizard name
 *                  obsolete, use CDialogWiz::GetLanguageOfSelectedWizard instead
 * @return          int
 * @exception       -
 * @see             -
*/
int CDialogWiz::GetIndexOfSelectedWizardName()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    return m_ObjPropWiz.m_IndexOfSelectedWizardName;
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogWiz::GetLanguageOfSelectedWizard
 * Description      returns the iec language of the selected wizard 
 * @return          CString
 * @exception       -
 * @see             -
*/
CString CDialogWiz::GetLanguageOfSelectedWizard()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    return m_ObjPropWiz.GetLanguageOfSelectedWizard();
}
