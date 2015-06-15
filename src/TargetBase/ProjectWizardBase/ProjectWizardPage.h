/* $Header: /4CReleased/V2.20.00/TargetBase/PROJECTWIZARDBASE/ProjectWizardPage.h 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: ProjectWizardPage.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/PROJECTWIZARDBASE/ProjectWizardPage.h $
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

#ifndef _PROJECTWIZARDPAGE_H_
#define _PROJECTWIZARDPAGE_H_

class CProjectWizardData;

#define NORMBOOL(b) ((b) ? 1 : 0)


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

#endif

/* ---------------------------------------------------------------------------- */
