/* $Header: /4CReleased/V2.20.00/Target/TEMPLATES/AddOnHandler/ProjectWizardDialog.cpp 1     28.02.07 18:54 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: ProjectWizardDialog.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/Target/TEMPLATES/AddOnHandler/ProjectWizardDialog.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Target
 *
 * =COMPONENT			AddOnHandler
 *
 * =CURRENT 	 $Date: 28.02.07 18:54 $
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
#include "ProjectWizardDialog.h"
#include "BaseProjectData.h"

/* ----  Target Specific Includes:    ----------------------------------------- */

/* ----  Local Defines:   ----------------------------------------------------- */

/* ----  Global Variables:   -------------------------------------------------- */

/* ----  Local Functions:   --------------------------------------------------- */

/* ----  Implementations:   --------------------------------------------------- */


BEGIN_MESSAGE_MAP(CProjectWizardDialog, CPropertySheet)
END_MESSAGE_MAP()

/* ---------------------------------------------------------------------------- */
/**
 * CProjectWizardDialog
 *
 */
CProjectWizardDialog::CProjectWizardDialog (CProjectWizardData* pData)
: CPropertySheet (_T (""), AfxGetMainWnd (), 0),
  m_pageResource (this, pData),
  m_pageLibraries (this, pData, IDS_LIBSPAGE_CAPTION),
  m_pageNames (this, pData),
  m_pageProperties (this, pData, IDS_PROPSPAGE_CAPTION)
{
    AddPage (&m_pageResource);
    AddPage (&m_pageLibraries);
    AddPage (&m_pageNames);
    AddPage (&m_pageProperties);

    SetWizardMode ();
}


/* ============================================================================
 * T A R G E T   S P E C I F I C
 * ============================================================================
 *
 */


/* ---------------------------------------------------------------------------- */
