
#include "stdafx.h"
#include "resource.h"
#include "DialogFunctionBlock.h"
#include "extern.h"
#include "HelpString.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const TCHAR * FbSClasses[] = 
{
    _T("VAR"),
    _T("VAR_INPUT"),
    _T("VAR_OUTPUT"),
    _T("VAR_IN_OUT"),
    _T("VAR RETAIN"),
    _T("VAR_INPUT RETAIN"),
    _T("VAR_OUTPUT RETAIN"),
    _T("VAR NONRETAIN"),
    _T("VAR_INPUT NONRETAIN"),
    _T("VAR_OUTPUT NONRETAIN"),
    _T("VAR CONST"),

    _T("")
};

//------------------------------------------------------------------*
/**
 * Function name    CDialogFunctionBlock::CDialogFunctionBlock
 * Description      constructor for class CDialogFunctionBlock
 * @param           DIALOGTYPE DialogType
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogFunctionBlock::CDialogFunctionBlock(DIALOGTYPE DialogType) :
    CPropertySheetBase(DialogType,
                       DialogType == CVIEWINSERT ? CHelpString(IDS_INS_FB) :
                                                   CHelpString(IDS_PROP_FB)),
	CDialogGen(DialogType, this),
	CDialogWiz(DialogType, this, (CDialogGenBase*)this),
	CDialogFbDecl(DialogType, this, IDS_PROPDECL_COLHEAD_1, FbSClasses),
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
        strHelp.LoadString(IDS_WIZARDNAME_ST_FB);
        m_WizardNames.Add(strHelp);
        //FBD
        strHelp.LoadString(IDS_POULANGUAGE_FBD);
        m_PouLanguages.Add(strHelp);
        strHelp.LoadString(IDS_WIZARDNAME_FBD_FB);
        m_WizardNames.Add(strHelp);
        //SFC
        strHelp.LoadString(IDS_POULANGUAGE_SFC);
        m_PouLanguages.Add(strHelp);
        strHelp.LoadString(IDS_WIZARDNAME_SFC_FB);
        m_WizardNames.Add(strHelp);
        // 23.08.06 SIS <<
        //LD
        strHelp.LoadString(IDS_POULANGUAGE_LD);
        m_PouLanguages.Add(strHelp);
        strHelp.LoadString(IDS_WIZARDNAME_LD_FB);
        m_WizardNames.Add(strHelp);
        //IL
        strHelp.LoadString(IDS_POULANGUAGE_IL);
        m_PouLanguages.Add(strHelp);
        strHelp.LoadString(IDS_WIZARDNAME_IL_FB);
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
 * Function name    CDialogFunctionBlock::~CDialogFunctionBlock
 * Description      destructor for class CDialogFunctionBlock
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogFunctionBlock::~CDialogFunctionBlock()
{
}

CREATE_DELETE_IMPL(CDialogFunctionBlock)

