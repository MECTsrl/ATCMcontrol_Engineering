
#ifndef __PROJECTWIZARDPAGE_H_
#define __PROJECTWIZARDPAGE_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*
class CProjectWizardData;

//----  Defines:    ------------------------------------------------*
#define NORMBOOL(b) ((b) ? 1 : 0)

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


class CProjectWizardPage : public CPropertyPage
{
public:
    CProjectWizardPage (UINT uiTemplate, CPropertySheet *pParent, CProjectWizardData *pData, UINT uiCaption=0);

    virtual BOOL OnSetActive ();

protected:
    virtual void Init ();

    CPropertySheet     *m_pParent;
    CProjectWizardData *m_pData;

private:
    bool m_bInitialized;
};

#endif // __PROJECTWIZARDPAGE_H_

