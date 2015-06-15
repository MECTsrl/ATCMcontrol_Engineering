#ifndef _BASEPROJECTDATA_H_
#define _BASEPROJECTDATA_H_

class CBaseAddonParameter;

#include "ProjectWizardData.h"

//------------------------------------------------------------------*
/**
 *  class CBaseProjectData
 *  
 *  - base class for project wizard
 *  - maximum resource count
 *  - default communication channel
 *  - build target file (XML)
 *  - derived from CProjectWizardData (ProjectWizardBase.lib)
 *
 *  @see    CProjectWizardData
 */
class CBaseProjectData : public CProjectWizardData
{
public:

    CBaseProjectData();
    ~CBaseProjectData();

    // initialization
    virtual HRESULT Init(const CString& crstrPath, const CString& crstrProjectName, const CString& crstrKADFile);
    virtual int GetMaxResourceCount();

    // communication channel
    virtual CString GetCommChannelDescription(const CString& crstrCommChannel);
    virtual CString GetDefaultCommChannel();

    // target type
    CString GetTargetType();

protected:
    // create addon parameter
    virtual CBaseAddonParameter* CreateAddonParameter();

    // creating target file
    virtual CString BuildTargetFileName (CProjectWizardResource *pResource);
    virtual CString BuildTargetXML (CProjectWizardResource *pResource);
    virtual CString BuildConnectParXMLString(CProjectWizardResource* pResource);

public:
    CBaseAddonParameter*    m_pAddonPar;        // parameter from target adaption KAD
    int                     m_iFirmwareIdx;     // current firmware index
};

#endif
