#include "stdafx.h"
#include "ProjectWizardTask.h"


//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



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

