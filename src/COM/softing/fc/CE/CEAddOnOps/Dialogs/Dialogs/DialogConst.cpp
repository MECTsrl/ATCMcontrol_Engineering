
#include "stdafx.h"
#include "resource.h"
#include "DialogConst.h"
#include "HelpString.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*class CHelpString1 : public CString
{
public:
    CHelpString1(UINT nID) : CString()
    {
        CString::LoadString(nID);
    }

};*/

//------------------------------------------------------------------*
/**
 * Function name    CDialogConst::CDialogConst
 * Description      constructor for class CDialogConst
 * @param           DIALOGTYPE DialogType
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogConst::CDialogConst(DIALOGTYPE DialogType) :
    CPropertySheetBase(DialogType,
                       DialogType == CVIEWINSERT ? CHelpString(IDS_INS_CONST) :
                                                   CHelpString(IDS_INS_CONST)) ,
	
	CDialogGenConst(DialogType, this)
{     
  	AddPage(&m_ObjPropGenConst);
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogConst::~CDialogConst
 * Description      destructor for class CDialogConst
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogConst::~CDialogConst()
{
}

CREATE_DELETE_IMPL(CDialogConst)
