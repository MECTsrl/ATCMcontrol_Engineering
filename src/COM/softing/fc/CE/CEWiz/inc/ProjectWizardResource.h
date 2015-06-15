
#ifndef __PROJECTWIZARDRESOURCE_H_
#define __PROJECTWIZARDRESOURCE_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "ProjectWizardTask.h"
#include "ProjectWizardDefs.h"

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


class CProjectWizardResource
{
public:
	CProjectWizardResource ();

    void SetResource (CString sResource);
    CString GetResource ();

    void SetTarget (CString sTarget);
    CString GetTarget ();

    void SetAddress (CString sAddress);
    CString GetAddress ();

    int GetTaskCount ();
    void SetTaskCount (int iCount);
    CProjectWizardTask *GetTask (int i);

protected:
    CString m_sResource;
    CString m_sAddress;
    CString m_sTarget;
    CArray<CProjectWizardTask, CProjectWizardTask &> m_Tasks;

private:
    CString Trim(CString sString);
};

#endif // __PROJECTWIZARDRESOURCE_H_

