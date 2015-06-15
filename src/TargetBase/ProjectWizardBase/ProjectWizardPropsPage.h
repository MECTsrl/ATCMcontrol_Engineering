/* $Header: /4CReleased/V2.20.00/TargetBase/PROJECTWIZARDBASE/ProjectWizardPropsPage.h 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: ProjectWizardPropsPage.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/PROJECTWIZARDBASE/ProjectWizardPropsPage.h $
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

#ifndef _PROJECTWIZARDPROPSPAGE_H_
#define _PROJECTWIZARDPROPSPAGE_H_

#include "ProjectWizardPage.h"
#include "ProjectWizardRes.h"


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

#endif

/* ---------------------------------------------------------------------------- */
