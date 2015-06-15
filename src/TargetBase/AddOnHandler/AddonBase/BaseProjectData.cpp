/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"
#include "BaseProjectData.h"
#include "AddonBaseResource.h"
#include "utilif.h"
#include "BaseAddonParameter.h"
#include "basedefs.h"
#include "CommDefinitions.h"

#include "fc_todebug\fc_assert.h"
#include "fc_tools\fc_fileIO.h"


//----  Static Initializations:   ----------------------------------*

CBaseProjectData::CBaseProjectData() : 
  m_pAddonPar(NULL),
  m_iFirmwareIdx(-1)
{
}

//------------------------------------------------------------------*
/**
 * destructor.
 *
 *  delete addon parameter.
 *
 * @param         -
 * @return        -
 * @exception     -
 * @see           -
*/
CBaseProjectData::~CBaseProjectData()
{
    if(m_pAddonPar)
    {
        delete m_pAddonPar;
        m_pAddonPar = NULL;
    }
}


//------------------------------------------------------------------*
/**
 * initialization.
 *
 * @param           crstrPath: base path for new project
 * @param           crstrProjectName: new project name
 * @param           crstrKADFile: target adaption KAD file name without path.
 * @return          S_OK: ok
 *                  E_FAIL: addon parameter could not be read
 *                  error result of CProjectWizardData::Init()
 * @exception       -
 * @see             -
*/
HRESULT CBaseProjectData::Init(const CString& crstrPath, const CString& crstrProjectName, const CString& crstrKADFile)
{
    HRESULT hr;

    // governs default libraries
    SetSubSystems (SUBSYSTEM_OPC | SUBSYSTEM_ANY);

    // read addon parameter
    ASSERT(!m_pAddonPar);   // must not be used until now
    m_pAddonPar = CreateAddonParameter();
    if(!m_pAddonPar)
    {
        return E_FAIL;
    }
    if(!m_pAddonPar->LoadKADFile(crstrKADFile))
    {
        return E_FAIL;
    }

    // init base class
    hr = CProjectWizardData::Init(crstrPath, crstrProjectName);
    if(FAILED(hr))
    {
        return(hr);
    }

    return(S_OK);
}


//------------------------------------------------------------------*
/**
 * create addon parameter.
 *
 * @return          new base addon parameter object
 * @exception       -
 * @see             -
*/
CBaseAddonParameter* CBaseProjectData::CreateAddonParameter()
{
    return new CBaseAddonParameter;
}

//------------------------------------------------------------------*
/**
 * get default comm channel.
 *
 * @param           -
 * @return          default comm channel
 * @exception       -
 * @see             -
*/
CString CBaseProjectData::GetDefaultCommChannel()
{
    return COMM_CHANNEL_TCP;
}


//------------------------------------------------------------------*
/**
 * Overridden to build special target file name <targetname>
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
CString CBaseProjectData::BuildTargetFileName(CProjectWizardResource *pResource)
{
    CString strFileName;

    ASSERT(m_pAddonPar);
    if(!m_pAddonPar)
    {
        return strFileName;
    }

    strFileName.Format(_T("%s.%s"), pResource->GetTarget(), m_pAddonPar->m_strFileExtension);
    return(strFileName);
}


//------------------------------------------------------------------*
/**
 * Overriden to build special target XML description.
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
CString CBaseProjectData::BuildTargetXML(CProjectWizardResource *pResource)
{
    CString strText;
    CString strConnectPar;
    CString strCustDL;

    ASSERT(m_pAddonPar);
    if(!m_pAddonPar)
    {
        return strText;
    }

    // build connect parameter xml string
    strConnectPar = BuildConnectParXMLString(pResource);

    // generate custom download string
    if (m_pAddonPar->m_bCustDL)
    {
        CString strDirRel;
        CString strDirFull;

        strDirRel.Format(_T("%s\\%s"), m_pAddonPar->m_strCustDLDir, pResource->GetTarget());
        strDirFull.Format(_T("%s\\%s"), m_strPath, strDirRel);

        strCustDL.Format(_T("    <CDOWNLOAD DIR=\"%s\"/>\r\n"), strDirRel);

        CFileStatus status;
        if (!strDirFull.IsEmpty() &&
            !CFile::GetStatus(strDirFull, status))
        {
            // create custom download directory if it does not exist
            FC_CreateSubDirA(strDirFull);
        }
    }

    // build xml text
    strText.Format(_T("<%s CONTROL=\"%s\"\r\n  %sRESOURCE=\"%s\">\r\n%s</%s>\r\n"),
                    m_pAddonPar->m_strKADType,
                    pResource->GetControlString(),
                    strConnectPar,
                    pResource->GetResourceName(),
                    strCustDL,
                    m_pAddonPar->m_strKADType);
    return(strText);
}


//------------------------------------------------------------------*
/**
 * build connect parameter xml string.
 *
 * @param           pResource: resource object
 * @return          connect parameter xml string
 * @exception       -
 * @see             -
*/
CString CBaseProjectData::BuildConnectParXMLString(CProjectWizardResource* pResource)
{
    CString strXML;
    CString strCommChannel;
    CString strConnectPar;
    CString strTmp;

    POSITION    pos = pResource->GetConnectParStartPosition();
    while(pos)
    {
        pResource->GetNextConnectPar(pos, strCommChannel, strConnectPar);
        strTmp.Format("%s=\"%s\"\r\n  ", strCommChannel, strConnectPar);
        strXML += strTmp;
    }
    return strXML;
}


//------------------------------------------------------------------*
/**
 * get maximum resource count.
 *
 * @param           -
 * @return          maximum resource count in wizard dialog
 * @exception       -
 * @see             -
*/
int CBaseProjectData::GetMaxResourceCount()
{
    int iResourceCount = DEFAULT_MAX_RESOURCE_COUNT;
    return iResourceCount;
}


//------------------------------------------------------------------*
/**
 * get target type.
 *
 * @param           -
 * @return          target type
 * @exception       -
 * @see             -
*/
CString CBaseProjectData::GetTargetType()
{
    if(m_pAddonPar)
    {
        return m_pAddonPar->m_strKADType;
    }
    return _T("");
}


//------------------------------------------------------------------*
/**
 * get communication channel description.
 *
 * @param           crstrCommChannel: communication channel
 * @return          -
 * @exception       -
 * @see             -
*/
CString CBaseProjectData::GetCommChannelDescription(const CString& crstrCommChannel)
{
    if(m_pAddonPar)
    {
        return m_pAddonPar->GetCommChannelDescription(crstrCommChannel);
    }
    return _T("");
}


//------------------------------------------------------------------*
/**
 * get resource address.
 *
 * @param           iResource: index of resource
 * @param           rstrOPCServer: OPC-Server address
 * @param           rstrCommChannel: communication channel
 * @param           rstrIPAddress: IP-Address string
 * @param           rstrSerialPar: serial parameter string
 * @param           ruiFirmwareVersion: firmware version index
 * @return          successful?
 * @exception       -
 * @see             SetResourceAddress()
*/
//BOOL CBaseProjectData::GetResourceAddress(int iResource, CString &rstrOPCServer, CString& rstrCommChannel, CString& rstrIPAddress, CString& rstrSerialPar, UINT& ruiFirmwareVersion)
//{
//    if(!m_pAddonPar)
//    {
//        return FALSE;
//    }
//    CProjectWizardResource* pResource = GetResource(iResource);
//    if(!pResource)
//    {
//        return FALSE;
//    }
//
//    rstrOPCServer = pResource->GetOPCServer();
//    rstrCommChannel = pResource->GetCommChannel();
//    rstrIPAddress = pResource->GetIPAddress();
//    rstrSerialPar = pResource->GetSerialPar();
//    ruiFirmwareVersion = pResource->GetFirmwareVersion();
//
//    return TRUE;
//}

//------------------------------------------------------------------*
/**
 * set resource address.
 *
 * @param           iResource: index of resource
 * @param           crstrOPCServer: OPC-Server address
 * @param           crstrCommChannel: communication channel
 * @param           crstrConnectPar: IP-Address string
 * @param           crstrSerialPar: serial parameter string
 * @param           uiFirmwareVersion: firmware version index
 * @return          successful?
 * @exception       -
 * @see             GetResourceAddress()
*/
//BOOL CBaseProjectData::SetResourceAddress(int iResource, const CString& crstrOPCServer, const CString& crstrCommChannel, const CString& crstrConnectPar, UINT uiFirmwareVersion)
//{
//    if(!m_pAddonPar)
//    {
//        return FALSE;
//    }
//    CProjectWizardResource* pResource = GetResource(iResource);
//    if(!pResource)
//    {
//        return FALSE;
//    }
//    pResource->SetOPCServer(crstrOPCServer);
//    pResource->SetCommChannel(crstrCommChannel);
//    pResource->SetFirmwareVersion(uiFirmwareVersion);
//
//    if(!crstrConnectPar.IsEmpty())
//    {
//        pResource->SetConnectPar(crstrConnectPar, crstrCommChannel);
//    }
//    else
//    {
//        CFirmwareData*  pFirmwareData = m_pAddonPar->GetFirmwareData(uiFirmwareVersion);
//        if(pFirmwareData)
//        {
//            CCommChannelData    tCommChannelData;
//            pFirmwareData->GetCommChannelDataFromId(crstrCommChannel, tCommChannelData);
//            pResource->SetConnectPar(tCommChannelData.m_strStdConnectPar, crstrCommChannel);
//        }
//    }
//
//    // build control string and save it in pResource
//    CString strConnectPar = pResource->GetConnectPar();
//    CString strControlString;
//
//    strControlString = m_pAddonPar->BuildControlString(crstrOPCServer, crstrCommChannel, strConnectPar, uiFirmwareVersion);
//    pResource->SetControlString(strControlString);
//
//    return TRUE;
//}

/* ---------------------------------------------------------------------------- */
