
#include "stdafx.h"
#include "dialogs.h"
#include "DialogDataType.h"
#include "extern.h"
#include "HelpString.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//------------------------------------------------------------------*
/**
 * Function name    CDialogDataType::CDialogDataType
 * Description      constructor for class CDialogDataType
 * @param           DIALOGTYPE DialogType
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogDataType::CDialogDataType(DIALOGTYPE DialogType) :
    CPropertySheetBase(DialogType,
                       DialogType == CVIEWINSERT ? CHelpString(IDS_INS_DATA_TYPE) :
                                                   CHelpString(IDS_PROP_DATA_TYPE)),
	CDialogGen(DialogType, this),
	CDialogWiz(DialogType, this, (CDialogGenBase*)this),
	CDialogDutDecl(DialogType, this, IDS_PROPDECL_COLHEAD_2),
	CDialogInfo(DialogType, this),
	CDialogHelp(DialogType, this, (CDialogGenBase*)this, (CDialogInfo*)this)
{
    if (DialogType == CVIEWINSERT)
    {
	    AddPage(&m_ObjPropWiz);

        CString strHelp = _T("");
        strHelp.LoadString(IDS_POULANGUAGE_ST);
        m_PouLanguages.Add(strHelp);
        strHelp.LoadString(IDS_WIZARDNAME_ST);
        m_WizardNames.Add(strHelp);

        if(GetSupportedIECLanguages(&m_IecLanguages) == S_OK) {
            InitializeDialogWiz(m_IecLanguages, m_PouLanguages, m_WizardNames);
        }
    }
	AddPage(&m_ObjPropGen);
	AddPage(&m_ObjPropDutDecl);

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
 * Function name    CDialogConst::~CDialogConst
 * Description      destructor for class CDialogConst
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogDataType::~CDialogDataType()
{
}

CREATE_DELETE_IMPL(CDialogDataType)

