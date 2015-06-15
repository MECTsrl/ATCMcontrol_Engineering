/* $Header: /4CReleased/V2.20.00/TargetBase/PROJECTWIZARDBASE/ProjectWizardPage.cpp 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: ProjectWizardPage.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/PROJECTWIZARDBASE/ProjectWizardPage.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase
 *
 * =COMPONENT			ProjectWizardBase
 *
 * =CURRENT 	 $Date: 28.02.07 19:06 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"
#include "ProjectWizardPage.h"

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

/* ---------------------------------------------------------------------------- */
