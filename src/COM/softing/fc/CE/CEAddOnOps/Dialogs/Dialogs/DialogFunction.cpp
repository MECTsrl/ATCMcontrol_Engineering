
#include "stdafx.h"
#include "dialogs.h"
#include "DialogFunction.h"
#include "extern.h"
#include "HelpString.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const TCHAR * FunctSClasses[] = 
{
    _T("VAR"),
    _T("VAR_INPUT"),
    _T("VAR_OUTPUT"),
    _T("VAR_IN_OUT"),
    _T("VAR CONST"),

    _T("")
};

//------------------------------------------------------------------*
/**
 * Function name    CDialogFunction::CDialogFunction
 * Description      constructor for class CDialogFunction
 * @param           DIALOGTYPE DialogType
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogFunction::CDialogFunction(DIALOGTYPE DialogType) :
    CPropertySheetBase(DialogType,
                       DialogType == CVIEWINSERT ? CHelpString(IDS_INS_FUNC) :
                                                   CHelpString(IDS_PROP_FUNC)),
	CDialogGen(DialogType, this),
	CDialogWiz(DialogType, this, (CDialogGenBase*)this),
	CDialogFunctDecl(DialogType, this, IDS_PROPDECL_COLHEAD_1, FunctSClasses),
	CDialogInfo(DialogType, this),
	CDialogHelp(DialogType, this, (CDialogGenBase*)this, (CDialogInfo*)this)
{
    if (DialogType == CVIEWINSERT)
    {
	    AddPage(&m_ObjPropWiz);
        CString strHelp = _T("");
        // 23.08.06 SIS >>
        // moved ST to beginning
        //ST
        strHelp.LoadString(IDS_POULANGUAGE_ST);
        m_PouLanguages.Add(strHelp);
        strHelp.LoadString(IDS_WIZARDNAME_ST_FUNC);
        m_WizardNames.Add(strHelp);
        //FBD
        strHelp.LoadString(IDS_POULANGUAGE_FBD);
        m_PouLanguages.Add(strHelp);
        strHelp.LoadString(IDS_WIZARDNAME_FBD_FUNC);
        m_WizardNames.Add(strHelp);
        // 23.08.06 SIS <<
        //LD
        strHelp.LoadString(IDS_POULANGUAGE_LD);
        m_PouLanguages.Add(strHelp);
        strHelp.LoadString(IDS_WIZARDNAME_LD_FUNC);
        m_WizardNames.Add(strHelp);
        //IL
        strHelp.LoadString(IDS_POULANGUAGE_IL);
        m_PouLanguages.Add(strHelp);
        strHelp.LoadString(IDS_WIZARDNAME_IL_FUNC);
        m_WizardNames.Add(strHelp);

        if(GetSupportedIECLanguages(&m_IecLanguages) == S_OK) 
        {
            InitializeDialogWiz(m_IecLanguages, m_PouLanguages, m_WizardNames);
        }
    }
	AddPage(&m_ObjPropGen);
	AddPage(&m_ObjPropFunctDecl);
// SIS bug fix d-2019
// allow creation of help file in property dialog
//    if (DialogType == CVIEWINSERT)
//    {
        AddPage(&m_ObjPropHelp);
//    }
	
    AddPage(&m_ObjPropInfo);
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogFunction::~CDialogFunction
 * Description      destructor for class CDialogFunction
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogFunction::~CDialogFunction()
{
}

CREATE_DELETE_IMPL(CDialogFunction);

