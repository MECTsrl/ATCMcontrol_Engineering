
#include "stdafx.h"
#include "dialogs.h"
#include "DialogProgram.h"
#include "extern.h"
#include "HelpString.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const TCHAR * PrgSClasses[] = 
{
    _T("VAR"),
    _T("VAR RETAIN"),
    _T("VAR NONRETAIN"),
    _T("VAR CONST"),
    _T("VAR_EXTERNAL"),

    _T("")
};

//------------------------------------------------------------------*
/**
 * Function name    CDialogProgram::CDialogProgram
 * Description      constructor for class CDialogProgram
 * @param           DIALOGTYPE DialogType
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogProgram::CDialogProgram(DIALOGTYPE DialogType) :
    CPropertySheetBase(DialogType,
                       DialogType == CVIEWINSERT ? CHelpString(IDS_INS_PROG):
                                                   CHelpString(IDS_PROP_PROG)),
	CDialogGen(DialogType, this),
	CDialogWiz(DialogType, this, (CDialogGenBase*)this),
	CDialogFbDecl(DialogType, this, IDS_PROPDECL_COLHEAD_1, PrgSClasses),
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
        strHelp.LoadString(IDS_WIZARDNAME_ST_PROG);
        m_WizardNames.Add(strHelp);
        //FBD
        strHelp.LoadString(IDS_POULANGUAGE_FBD);
        m_PouLanguages.Add(strHelp);
        strHelp.LoadString(IDS_WIZARDNAME_FBD_PROG);
        m_WizardNames.Add(strHelp);
        //SFC
        strHelp.LoadString(IDS_POULANGUAGE_SFC);
        m_PouLanguages.Add(strHelp);
        strHelp.LoadString(IDS_WIZARDNAME_SFC_PROG);
        m_WizardNames.Add(strHelp);
        // 23.08.06 SIS <<
        //LD
        strHelp.LoadString(IDS_POULANGUAGE_LD);
        m_PouLanguages.Add(strHelp);
        strHelp.LoadString(IDS_WIZARDNAME_LD_PROG);
        m_WizardNames.Add(strHelp);
        //IL
        strHelp.LoadString(IDS_POULANGUAGE_IL);
        m_PouLanguages.Add(strHelp);
        strHelp.LoadString(IDS_WIZARDNAME_IL_PROG);
        m_WizardNames.Add(strHelp);

        if(GetSupportedIECLanguages(&m_IecLanguages) == S_OK) 
        {
            InitializeDialogWiz(m_IecLanguages, m_PouLanguages, m_WizardNames);
        }
    }
	AddPage(&m_ObjPropGen);
	AddPage(&m_ObjPropFbDecl);

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
 * Function name    CDialogProgram::~CDialogProgram
 * Description      destructor for class CDialogProgram
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogProgram::~CDialogProgram()
{
}

CREATE_DELETE_IMPL(CDialogProgram)

