
#ifndef __PROJECTWIZARDPROPSPAGE_H_
#define __PROJECTWIZARDPROPSPAGE_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "ProjectWizardPage.h"
#include "ProjectWizardRes.h"

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


class CProjectWizardPropertiesPage : public CProjectWizardPage
{
public:
    enum { IDD = IDD_4CWIZ_PROPERTIES };

    CProjectWizardPropertiesPage (CPropertySheet *pParent, CProjectWizardData *pData, UINT uiCaption);

private:
    virtual void DoDataExchange (CDataExchange *pDX);

    virtual BOOL OnSetActive();
    virtual BOOL OnWizardFinish();
    virtual void OnBrowseUrl ();

    CString	m_strAlias;
    CString	m_strUserText1;
    CString	m_strUserText2;
    CString	m_strUserText3;
    CString	m_strURL;

    DECLARE_MESSAGE_MAP()
};

#endif // __PROJECTWIZARDPROPSPAGE_H_

