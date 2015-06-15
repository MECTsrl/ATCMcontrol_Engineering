#ifndef __WIZARDPAGERESOURCE_H_
#define __WIZARDPAGERESOURCE_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "ProjectWizardPage.h"
#include "resource.h"

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


class CWizardPageResource : public CProjectWizardPage
{
public:
    CWizardPageResource (CPropertySheet *pParent, CProjectWizardData *pData);

protected:
    void PopulateTables (int iCount);

private:
    enum {IDD = IDD_4CWIZ_RESOURCES};
    BOOL m_bProfibus;
    BOOL m_bSerialCommunication;
    virtual void Init();
    virtual BOOL OnSetActive();
//    virtual LRESULT OnWizardBack();
    virtual void DoDataExchange(CDataExchange* pDX);
    DECLARE_MESSAGE_MAP()
};

#endif // __WIZARDPAGERESOURCE_H_