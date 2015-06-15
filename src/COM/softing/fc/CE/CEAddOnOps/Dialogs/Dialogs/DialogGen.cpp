
#include "stdafx.h"
#include "resource.h"
#include "DialogGen.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//------------------------------------------------------------------*
/**
 * Function name    CDialogGen::CDialogGen
 * Description      constructor for class CDialogGen
 * @param           DIALOGTYPE DialogType
 * @param           CPropertySheetBase *PropertySheet
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogGen::CDialogGen(DIALOGTYPE DialogType, 
                       CPropertySheetBase *PropertySheet) :
    m_ObjPropGen(DialogType, PropertySheet),
    CDialogGenBase(m_ObjPropGen)
{
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGen::~CDialogGen
 * Description      destructor for class CDialogGen
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogGen::~CDialogGen()
{
}
