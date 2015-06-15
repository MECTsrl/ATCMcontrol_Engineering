#ifndef __PROJECTWIZARDDIALOG_H_
#define __PROJECTWIZARDDIALOG_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "WizardPageResource.h"
#include "ProjectWizardLibsPage.h"
#include "WizardPageNames.h"
#include "ProjectWizardPropsPage.h"

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


class CProjectWizardDialog : public CPropertySheet
{
public:
    CProjectWizardDialog (CProjectWizardData *pData);

private:
    CWizardPageResource          m_pageResource;   // page 1
    CProjectWizardLibrariesPage  m_pageLibraries;  // page 2
    CWizardPageNames             m_pageNames;      // page 3
    CProjectWizardPropertiesPage m_pageProperties; // page 4

    DECLARE_MESSAGE_MAP()
};

#endif // __PROJECTWIZARDDIALOG_H_

