#ifndef __PROJECTWIZARDTASK_H_
#define __PROJECTWIZARDTASK_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "ProjectWizardDefs.h"

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


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


#endif // __PROJECTWIZARDTASK_H_

