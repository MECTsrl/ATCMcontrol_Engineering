#ifndef _WIZARDPAGERESOURCE_H_
#define _WIZARDPAGERESOURCE_H_

#include "ProjectWizardPage.h"
#include "AddonBaseResource.h"


//------------------------------------------------------------------*
/**
 *  class CWizardPageResource
 *
 *  - project new wizard property page for choosing number of resources
 *  - resource data is created directly in DoDataExchange() (PopulateTables())
 *  - derived from CProjectWizardPage (ProjectWizardBase.lib)
 *
 *  @see    CProjectWizardPage
 */
class CWizardPageResource : public CProjectWizardPage
{
public:
    CWizardPageResource (CPropertySheet *pParent, CProjectWizardData *pData);

protected:
    virtual void PopulateTables (int iCount);

private:
    enum {IDD = IDD_4CWIZ_RESOURCES};
    virtual void Init();
    virtual BOOL OnSetActive();
    virtual void DoDataExchange(CDataExchange* pDX);
    DECLARE_MESSAGE_MAP()
};

#endif
