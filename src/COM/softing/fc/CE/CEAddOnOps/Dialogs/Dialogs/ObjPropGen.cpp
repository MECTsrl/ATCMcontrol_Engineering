
#include "stdafx.h"
#include "resource.h"
#include "ObjPropGen.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGen::CObjPropGen
 * Description      constructor for class CObjPropGen
 * @param           TypeDIALOGTYPE Dialog
 * @param           CPropertySheetBase *PropertySheet
 * @return          -
 * @exception       -
 * @see             -
*/
CObjPropGen::CObjPropGen(DIALOGTYPE DialogType, 
                         CPropertySheetBase *PropertySheet) : 
    CObjPropGenBase(DialogType, IDD_OBJPROP_GEN, PropertySheet)
{
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGen::~CObjPropGen
 * Description      destructor for class CObjPropGen
 * @return          -
 * @exception       -
 * @see             -
*/
CObjPropGen::~CObjPropGen()
{
}
