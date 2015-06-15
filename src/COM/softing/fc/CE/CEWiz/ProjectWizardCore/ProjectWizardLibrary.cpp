
#include "stdafx.h"
#include "ProjectWizardLibrary.h"


//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



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
	} else if (_tcsicmp(_T("4C_System.") FILE_EXT_LIBRARY,strLibraryName) == 0) {
		m_subSystemDefault = SUBSYSTEM_ANY;
	} else if (_tcsicmp(_T("4C_Communication.") FILE_EXT_LIBRARY,strLibraryName) == 0) {
		m_subSystemDefault = SUBSYSTEM_SERIAL_COMMUNICATION;
    } else if (_tcsicmp(_T("4C_Utility.") FILE_EXT_LIBRARY,strLibraryName) == 0) {
        m_subSystemDefault = SUBSYSTEM_SERIAL_COMMUNICATION;
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
