/* $Header: /4CReleased/V2.20.00/TargetBase/PROJECTWIZARDBASE/ProjectWizardLibsPage.h 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: ProjectWizardLibsPage.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/PROJECTWIZARDBASE/ProjectWizardLibsPage.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase
 *
 * =COMPONENT			ProjectWizardBase
 *
 * =CURRENT 	 $Date: 28.02.07 19:06 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

#ifndef _PROJECTWIZARDLIBSPAGE_H_
#define _PROJECTWIZARDLIBSPAGE_H_

#include "ProjectWizardPage.h"
#include "ProjectWizardRes.h"

class CProjectWizardData;

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

#endif

/* ---------------------------------------------------------------------------- */
