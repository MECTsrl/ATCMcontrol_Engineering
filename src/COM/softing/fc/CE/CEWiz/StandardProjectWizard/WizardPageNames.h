
#ifndef __WIZARDPAGENAMES_H_
#define __WIZARDPAGENAMES_H_

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


class CWizardPageNames : public CProjectWizardPage 
{
public:
    CWizardPageNames(CPropertySheet *pParent, CProjectWizardData *pData);

private:
    int m_iActualResource;
    CString	m_sResource;
    CString	m_sProgram;
    int m_iLanguage;
    CString	m_sTask;
    CString	m_sTarget;
    CString	m_sAddress;
    CListCtrl m_resources;
    CImageList m_imageList;
    enum {IDD = IDD_4CWIZ_NAMES};
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual void Init();
    void ReadResourceData();
    void WriteResourceData();
    virtual BOOL OnSetActive();
    virtual LRESULT OnWizardNext();
    virtual LRESULT OnWizardBack();
    void OnResourceChanged(NMHDR* pNMHDR, LRESULT* pResult);

    DECLARE_MESSAGE_MAP()
};

#endif // __WIZARDPAGENAMES_H_
