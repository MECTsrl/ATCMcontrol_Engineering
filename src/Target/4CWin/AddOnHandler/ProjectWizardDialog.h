/* $Header: /4CReleased/V2.20.00/Target/4CWin/ADDONHANDLER/ProjectWizardDialog.h 1     28.02.07 19:09 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: ProjectWizardDialog.h $
 *						 $Logfile: /4CReleased/V2.20.00/Target/4CWin/ADDONHANDLER/ProjectWizardDialog.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Target
 *
 * =COMPONENT			AddOnHandler
 *
 * =CURRENT 	 $Date: 28.02.07 19:09 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

#ifndef _PROJECTWIZARDDIALOG_H_
#define _PROJECTWIZARDDIALOG_H_

#include "ProjectWizardLibsPage.h"
#include "WizardPageNames.h"
#include "ProjectWizardPropsPage.h"
#include "WizardPageResource.h"

/* ---------------------------------------------------------------------------- */
/**
 * class CProjectWizardDialog
 *
 */

class CProjectWizardDialog : public CPropertySheet
{
public:
    CProjectWizardDialog(CProjectWizardData* pData);

private:
    CWizardPageResource          m_pageResource;   // page 1
    CProjectWizardLibrariesPage  m_pageLibraries;  // page 2
    CWizardPageNames             m_pageNames;      // page 3
    CProjectWizardPropertiesPage m_pageProperties; // page 4

    DECLARE_MESSAGE_MAP()


/* ============================================================================
 * T A R G E T   S P E C I F I C
 * ============================================================================
 *
 */

};

#endif

/* ---------------------------------------------------------------------------- */
