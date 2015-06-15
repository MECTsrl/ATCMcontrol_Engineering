/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"
#include "BasePropertyPage.h"

//----  Static Initializations:   ----------------------------------*


CBasePropertyPage::CBasePropertyPage(UINT nIDTemplate, UINT nIDCaption /*= 0*/) :
  CPropertyPage(nIDTemplate, nIDCaption)
{
}


//------------------------------------------------------------------*
/**
 * update page data.
 *
 * @param         -
 * @return        -
 * @exception     -
 * @see           -
*/
void CBasePropertyPage::UpdatePageData()
{
    if(GetSafeHwnd())
    {
        UpdateData();
    }
}

//------------------------------------------------------------------*
/**
 * validate.
 *
 *  base class version just returns TRUE.
 *
 * @param         -
 * @return        data OK?    
 * @exception     -
 * @see               -
*/
BOOL CBasePropertyPage::Validate()
{
    return TRUE;
}

/* ---------------------------------------------------------------------------- */
