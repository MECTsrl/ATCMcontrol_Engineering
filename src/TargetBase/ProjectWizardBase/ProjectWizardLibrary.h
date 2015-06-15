/* $Header: /4CReleased/V2.20.00/TargetBase/PROJECTWIZARDBASE/ProjectWizardLibrary.h 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: ProjectWizardLibrary.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/PROJECTWIZARDBASE/ProjectWizardLibrary.h $
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

#ifndef _PROJECTWIZARDLIBRARY_H_
#define _PROJECTWIZARDLIBRARY_H_

#include "ProjectWizardDefs.h"


class CProjectWizardLibrary
{
public:
    CProjectWizardLibrary ();

    CString GetLibraryName ();
    void SetLibraryName (CString strLibraryName);

    BOOL GetSelectFlag ();
    void SetSelectFlag (BOOL bSelectFlag);

    TSubSystems GetSubSystemDefault ();

protected:
    CString     m_strLibraryName;
    BOOL        m_bSelectFlag;
    TSubSystems m_subSystemDefault;
};

#endif

/* ---------------------------------------------------------------------------- */
