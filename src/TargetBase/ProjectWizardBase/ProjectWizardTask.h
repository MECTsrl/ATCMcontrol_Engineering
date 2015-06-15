/* $Header: /4CReleased/V2.20.00/TargetBase/PROJECTWIZARDBASE/ProjectWizardTask.h 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: ProjectWizardTask.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/PROJECTWIZARDBASE/ProjectWizardTask.h $
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

#ifndef _PROJECTWIZARDTASK_H_
#define _PROJECTWIZARDTASK_H_

#include "ProjectWizardDefs.h"


class CProjectWizardTask
{
public:
    CProjectWizardTask ();

    CString GetName ();
    void SetName (CString strName);

    CString GetProgram ();
    void SetProgram (CString strProgram);

    TIecLanguage GetLanguage ();
    void SetLanguage (TIecLanguage language);

    CString GetIecLanguageString ();

private:
    CString      m_strName;
    CString      m_strProgram;
    TIecLanguage m_Language;
};

#endif

/* ---------------------------------------------------------------------------- */
