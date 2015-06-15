
#include "stdafx.h"
#include "resource.h"
#include "ObjPropGenButtonBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenButtonBase::CObjPropGenButtonBase
 * Description      constructor for class CObjPropGenButtonBase
 * @return          -
 * @exception       -
 * @see             -
*/
CObjPropGenButtonBase::CObjPropGenButtonBase()
{
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenButtonBase::~CObjPropGenButtonBase
 * Description      destructor for class CObjPropGenButtonBase
 * @return          -
 * @exception       -
 * @see             -
*/
CObjPropGenButtonBase::~CObjPropGenButtonBase()
{
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenButtonBase::DoDataExchange
 * Description      exchanges and validates dialog data
 * @param           CDataExchange* pDX
 * @return          -
 * @exception       -
 * @see             -
*/
void CObjPropGenButtonBase::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_NEWVERSIONID, m_NewVersionID);
}

