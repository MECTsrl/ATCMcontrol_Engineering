/* $Header: /4CReleased/V2.20.00/TargetBase/PROJECTWIZARDBASE/ProjectWizardTask.cpp 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: ProjectWizardTask.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/PROJECTWIZARDBASE/ProjectWizardTask.cpp $
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
#include "ProjectWizardTask.h"


CProjectWizardTask::CProjectWizardTask ()
: m_Language (IEC_LANGUAGE_ST)
{
}


CString CProjectWizardTask::GetName ()
{
    return (m_strName);
}


void CProjectWizardTask::SetName (CString strName)
{
    m_strName = strName;
}


CString CProjectWizardTask::GetProgram ()
{
    return (m_strProgram);
}


void CProjectWizardTask::SetProgram (CString strProgram)
{
    m_strProgram = strProgram;
}


TIecLanguage CProjectWizardTask::GetLanguage ()
{
    return (m_Language);
}


void CProjectWizardTask::SetLanguage (TIecLanguage language)
{
    m_Language = language;
}


CString CProjectWizardTask::GetIecLanguageString ()
{
    CString strLanguage;

    switch (m_Language)
    {
    case IEC_LANGUAGE_ST : strLanguage = _T ("ST");  break;
    case IEC_LANGUAGE_FBD: strLanguage = _T ("FBD"); break;
    case IEC_LANGUAGE_SFC: strLanguage = _T ("SFC"); break;
    case IEC_LANGUAGE_LD: strLanguage = _T ("LD"); break;
    case IEC_LANGUAGE_IL: strLanguage = _T ("IL"); break;
    }

    return (strLanguage);
}

/* ---------------------------------------------------------------------------- */
