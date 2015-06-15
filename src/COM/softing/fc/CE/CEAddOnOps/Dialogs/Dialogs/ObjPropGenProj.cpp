
#include "stdafx.h"
#include "resource.h"
#include "ObjPropGenProj.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenProj::CObjPropGenProj
 * Description      Constructor for class CObjPropGenProj
 * @param           DIALOGTYPE DialogType
 * @param           CPropertySheetBase *PropertySheet
 * @return          -
 * @exception       -
 * @see             -
*/
CObjPropGenProj::CObjPropGenProj(DIALOGTYPE DialogType, CPropertySheetBase *PropertySheet) : 
    CObjPropGenBase(DialogType,
                    IDD_OBJPROP_GEN_PROJ,
                    PropertySheet)
{
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenProj::~CObjPropGenProj
 * Description      destructor for class CObjPropGenProj
 * @return          -
 * @exception       -
 * @see             -
*/
CObjPropGenProj::~CObjPropGenProj()
{
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenProj::DoDataExchange
 * Description      exchanges and validates dialog data
 * @param           CDataExchange* pDX
 * @return          -
 * @exception       -
 * @see             -
*/
void CObjPropGenProj::DoDataExchange(CDataExchange* pDX)
{
	CObjPropGenBase::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LOCATION, m_Location);
}
