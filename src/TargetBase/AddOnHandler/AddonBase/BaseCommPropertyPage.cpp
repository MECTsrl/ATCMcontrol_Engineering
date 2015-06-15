
/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"
#include "BaseCommPropertyPage.h"

//----  Static Initializations:   ----------------------------------*


CBaseCommPropertyPage::CBaseCommPropertyPage(UINT nIDTemplate, UINT nIDCaption /*=0*/)
  : CBasePropertyPage(nIDTemplate, nIDCaption),
    m_bEnablePage(TRUE)
{
}



//------------------------------------------------------------------*
/**
 * enable/disable page.
 *
 * @param            bEnable: enable page?
 * @return           -
 * @exception        -
 * @see              -
*/
void CBaseCommPropertyPage::EnablePage(BOOL bEnable /*= TRUE*/)
{
    m_bEnablePage = bEnable;
}


//------------------------------------------------------------------*
/**
 * on set active
 *
 * @param           -
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CBaseCommPropertyPage::OnSetActive()
{
    EnableControls();
    return CBasePropertyPage::OnSetActive();
}


//------------------------------------------------------------------*
/**
 * enable/disable controls.
 *
 *  edit controls are also set read only when disabled
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CBaseCommPropertyPage::EnableControls()
{
    CWnd*       pWnd = GetWindow(GW_CHILD);
    CEdit*      pEdit;
    while(pWnd)
    {
        pEdit = dynamic_cast<CEdit*> (pWnd);
        if(pEdit)
        {
            pEdit->SetReadOnly(m_bEnablePage);
        }
        pWnd->EnableWindow(m_bEnablePage);
        pWnd = pWnd->GetWindow(GW_HWNDNEXT);
    }
}

/* ---------------------------------------------------------------------------- */
