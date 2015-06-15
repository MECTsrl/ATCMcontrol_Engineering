#ifndef __PROJECTWIZARDLIBSPAGE_H_
#define __PROJECTWIZARDLIBSPAGE_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "ProjectWizardPage.h"
#include "ProjectWizardRes.h"

//----  Forward Class Definitions:   -------------------------------*
class CProjectWizardData;

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


class CProjectWizardLibrariesPage : public CProjectWizardPage
{
public:
    enum { IDD = IDD_4CWIZ_LIBRARIES };

    CProjectWizardLibrariesPage (CPropertySheet *pParent, CProjectWizardData *pData, UINT uiCaption);

protected:
    virtual void DoDataExchange (CDataExchange *pDX);

    virtual void Init();
    void SetDefaults();

    void Update ();
    void FillLists ();
    void AdaptButtonStates ();

    void UseSelectedUnusedLibraries ();
    void UnuseSelectedUsedLibraries ();

    virtual BOOL OnSetActive ();
    virtual LRESULT OnWizardBack ();
    virtual void OnAddLibraries ();
    virtual void OnAddAllLibraries ();
    virtual void OnRemoveLibraries ();
    virtual void OnRemoveAllLibraries ();
    virtual void OnItemChanged (NMHDR *pNMHDR, LRESULT *pResult);
    virtual void OnSetFocusUsedLibs (NMHDR *pNMHDR, LRESULT *pResult);
    virtual void OnSetFocusUnusedLibs (NMHDR *pNMHDR, LRESULT *pResult);
    virtual void OnDClkUnusedLibs (NMHDR *pNMHDR, LRESULT *pResult);
    virtual void OnDClkUsedLibs (NMHDR *pNMHDR, LRESULT *pResult);

    bool m_bSetDefaults;

    CImageList m_imageList;
    CListCtrl  m_usedLibs;
    CListCtrl  m_unusedLibs;

    DECLARE_MESSAGE_MAP()
};

#endif // __PROJECTWIZARDLIBSPAGE_H_

