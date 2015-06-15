/* $Header: /4CReleased/V2.20.00/TargetBase/PROJECTWIZARDBASE/ProjectWizardLibrary.cpp 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: ProjectWizardLibrary.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/PROJECTWIZARDBASE/ProjectWizardLibrary.cpp $
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
#include "ProjectWizardLibrary.h"


///////////////////////////////////////////////////////////////////////////

CProjectWizardLibrary::CProjectWizardLibrary()
// creates a new entry for the library table
{
	m_bSelectFlag = FALSE;
	m_subSystemDefault = SUBSYSTEM_NONE;
}

void CProjectWizardLibrary::SetLibraryName(CString strLibraryName)
// sets the name of the library and checks for which subsystems
// the corresponding library is to be used by default
{
	m_strLibraryName = strLibraryName;
	if (_tcsicmp(_T("4C_PROFIBUS_IO.") FILE_EXT_LIBRARY,strLibraryName) == 0) {
		m_subSystemDefault = SUBSYSTEM_PROFIBUS;
	} else if (_tcsicmp(_T("4C_IEC1131_3.") FILE_EXT_LIBRARY,strLibraryName) == 0) {
		m_subSystemDefault = SUBSYSTEM_ANY;
	} else if (_tcsicmp(_T("4C_System2.") FILE_EXT_LIBRARY,strLibraryName) == 0) {
		m_subSystemDefault = SUBSYSTEM_ANY;
	} else if (_tcsicmp(_T("4C_Communication.") FILE_EXT_LIBRARY,strLibraryName) == 0) {
		m_subSystemDefault = SUBSYSTEM_SERIAL_COMMUNICATION;
    } else if (_tcsicmp(_T("4C_Utility.") FILE_EXT_LIBRARY,strLibraryName) == 0) {
        m_subSystemDefault = SUBSYSTEM_SERIAL_COMMUNICATION;
    } else if (_tcsicmp(_T("4CFC_System.") FILE_EXT_LIBRARY,strLibraryName) == 0) {
        m_subSystemDefault = SUBSYSTEM_FC;
    } else if (_tcsicmp(_T("4CBC_System.") FILE_EXT_LIBRARY,strLibraryName) == 0) {
        m_subSystemDefault = SUBSYSTEM_BC;
    } else if (_tcsicmp(_T("4C_BACnet.") FILE_EXT_LIBRARY,strLibraryName) == 0) {
        m_subSystemDefault = SUBSYSTEM_BC;
    } else if (_tcsicmp(_T("4CDC_System.") FILE_EXT_LIBRARY,strLibraryName) == 0) {
        m_subSystemDefault = SUBSYSTEM_DC;
    }


}

CString CProjectWizardLibrary::GetLibraryName()
// returns the name of the library
{
	return m_strLibraryName;
}

void CProjectWizardLibrary::SetSelectFlag(BOOL bSelectFlag)
// sets the flag indicating whether the library is selected
{
    m_bSelectFlag = (bSelectFlag ? TRUE : FALSE);
}

BOOL CProjectWizardLibrary::GetSelectFlag()
// returns the flag indicating whether the library is selected
{
	return m_bSelectFlag;
}

TSubSystems CProjectWizardLibrary::GetSubSystemDefault()
// returns the subsystems for which this library is chosen by default
{
	return m_subSystemDefault;
}

/* ---------------------------------------------------------------------------- */
