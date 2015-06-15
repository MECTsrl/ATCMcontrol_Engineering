/* $Header: /4CReleased/V2.20.00/TargetBase/PROJECTWIZARDBASE/ProjectWizardResource.h 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: ProjectWizardResource.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/PROJECTWIZARDBASE/ProjectWizardResource.h $
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

#ifndef _PROJECTWIZARDRESOURCE_H_
#define _PROJECTWIZARDRESOURCE_H_

#include "ProjectWizardTask.h"
#include "ProjectWizardDefs.h"

typedef CMap<CString, LPCTSTR, CString, LPCTSTR>    TStrStrMap;

class CProjectWizardResource
{
public:
	CProjectWizardResource();

    void SetResourceName(CString strResource);
    CString GetResourceName();

    void SetTarget(CString strTarget);
    CString GetTarget();

    void SetOPCServer(CString strOPCServer);
    CString GetOPCServer();

    void SetCommChannel(CString strCommChannel);
    CString GetCommChannel();

    void SetConnectPar(const CString& crstrConnectPar, const CString& crstrCommChannel = _T(""));
    CString GetConnectPar(const CString& crstrCommChannel = _T(""));

    POSITION GetConnectParStartPosition();
    void GetNextConnectPar(POSITION& rNextPosition, CString& rstrCommChannel, CString& rstrConnectPar);

    void SetFirmwareVersionIndex(UINT uiFirmwareVersionIndex);
    UINT GetFirmwareVersionIndex();

    void SetControlString(CString strControlString);
    CString GetControlString();

    int GetTaskCount();
    void SetTaskCount(int iCount);
    CProjectWizardTask *GetTask(int i);

protected:
    CString m_strResource;
    CString m_strOPCServer;
    CString m_strCommChannel;
    UINT    m_uiFirmwareVersionIndex;   // index
    CString m_strTarget;
    CString m_strControlString;
    CArray<CProjectWizardTask, CProjectWizardTask &> m_Tasks;

    TStrStrMap  m_tConnectParMap;   // map between communication channel and comm parameter


private:
    CString Trim(CString strString);
};

#endif

/* ---------------------------------------------------------------------------- */
