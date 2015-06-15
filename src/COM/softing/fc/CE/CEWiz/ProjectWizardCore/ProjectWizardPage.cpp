#include "stdafx.h"
#include "ProjectWizardPage.h"


//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CProjectWizardPage::CProjectWizardPage (UINT uiTemplate, CPropertySheet *pParent, CProjectWizardData *pData, UINT uiCaption)
: CPropertyPage (uiTemplate, uiCaption),
  m_pParent (pParent),
  m_pData (pData),
  m_bInitialized (false)
{
}


//------------------------------------------------------------------*
/**
 * Called when the page gets active. Calls Init on first activation.
 * Subclasses may override this but should always call the base class.
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
BOOL CProjectWizardPage::OnSetActive ()
{
    if (!m_bInitialized)
    {
        Init ();
        m_bInitialized = true;
    }

    return (CPropertyPage::OnSetActive ());
}


//------------------------------------------------------------------*
/**
 * Called on first activation of the page. Subclasses may override this.
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CProjectWizardPage::Init ()
{
    SetWindowText (m_strCaption);
}


