/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"
#include "AddonBaseResource.h"
#include "BaseAddonParameter.h"
#include "CommDefinitions.h"
#include "utilif.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseAddonParameter::CBaseAddonParameter()
{
    m_uiAnzFirmware = 0;
    m_bInit         = FALSE;
    m_bSupportNCC   = FALSE;
    m_bNccBPSupport = FALSE;
    m_bCustDL       = FALSE;
    m_bCustDLGui    = FALSE;
    m_strCustDLDir    = _T("");
    m_bBigEndian    = FALSE;
    
}



CBaseAddonParameter::~CBaseAddonParameter()
{

}


//------------------------------------------------------------------*
/**
 * load KAD file.
 *
 * @param           crstrKADFileName: KAD file name without path
 * @return          successful?
 * @exception       -
 * @see             ReadProperties(), ReadFirmware()
*/
BOOL CBaseAddonParameter::LoadKADFile(const CString crstrKADFileName)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CXMLDocument doc;

    BOOL bProperties = FALSE;

    CString strPath;
    LPSTR pszPath;

    // Get the setup path
    try
    {
        pszPath = strPath.GetBufferSetLength(MAX_PATH);
    }

    catch (CMemoryException * e)
    {
        e->Delete();
        return FALSE;
    }

    size_t sizeChars = MAX_PATH;
    if (UTIL_GetInstallPath(pszPath, &sizeChars)!=S_OK)
    {
        CString errorMsg;
        errorMsg.FormatMessage(IDS_ERROR_PARAMETER_INIT);
        ::AfxMessageBox(errorMsg, MB_OK|MB_ICONERROR);
        return FALSE;
    }

    strPath.ReleaseBuffer();

    int pos = strPath.ReverseFind('\\');
    if (pos > 0)
    {
        strPath = strPath.Left(pos + 1);
        strPath += _T("Engineering\\bin\\");
        strPath += crstrKADFileName;
    }
    else
    {
        CString errorMsg;
        errorMsg.FormatMessage(IDS_ERROR_PARAMETER_INIT);
        ::AfxMessageBox(errorMsg, MB_OK|MB_ICONERROR);
        return FALSE;
    }

    // load kad
    if (!doc.Load(strPath))
    {
        CString errorMsg;
        errorMsg.FormatMessage(IDS_ERROR_PARAMETER_INIT);
        ::AfxMessageBox(errorMsg, MB_OK|MB_ICONERROR);
        return FALSE;
    }

    CXMLNode rootNode;
    if (!doc.GetRootNode(rootNode))
    {
        CString errorMsg;
        errorMsg.FormatMessage(IDS_ERROR_PARAMETER_INIT);
        ::AfxMessageBox(errorMsg, MB_OK|MB_ICONERROR);
        return FALSE;
    }

    CString strTagName;
    if (!rootNode.GetTagName(strTagName))
    {
        CString errorMsg;
        errorMsg.FormatMessage(IDS_ERROR_PARAMETER_INIT);
        ::AfxMessageBox(errorMsg, MB_OK|MB_ICONERROR);
        return FALSE;
    }

    if (strTagName.Compare(ADDON_BASE_XML_TAG_ADDON)!=0)
    {
        CString errorMsg;
        errorMsg.FormatMessage(IDS_ERROR_PARAMETER_INIT);
        ::AfxMessageBox(errorMsg, MB_OK|MB_ICONERROR);
        return FALSE;
    }

    CXMLNodeList childNodeList;

    if (!rootNode.GetChildNodeList(childNodeList))
    {
        CString errorMsg;
        errorMsg.FormatMessage(IDS_ERROR_PARAMETER_INIT);
        ::AfxMessageBox(errorMsg, MB_OK|MB_ICONERROR);
        return FALSE;
    }

    childNodeList.Reset();
    CXMLNode childNode;

    while (childNodeList.Next(childNode))
    {
        if (!childNode.GetTagName(strTagName))
        {
            CString errorMsg;
            errorMsg.FormatMessage(IDS_ERROR_PARAMETER_INIT);
            ::AfxMessageBox(errorMsg, MB_OK|MB_ICONERROR);
            return FALSE;
        }

        if (strTagName.Compare(ADDON_BASE_XML_TAG_PROPERTIES)==0)
        {
            if (bProperties)
            {
                CString errorMsg;
                errorMsg.FormatMessage(IDS_ERROR_PARAMETER_INIT);
                ::AfxMessageBox(errorMsg, MB_OK|MB_ICONERROR);
                return FALSE;
            }

            if (!ReadProperties(childNode))
            {
                CString errorMsg;
                errorMsg.FormatMessage(IDS_ERROR_PARAMETER_INIT);
                ::AfxMessageBox(errorMsg, MB_OK|MB_ICONERROR);
                return FALSE;
            }
            bProperties = TRUE;
        }
        else if (strTagName.Compare(ADDON_BASE_XML_TAG_FIRMWARE)==0)
        {
            CFirmwareData tFirmware;
            if (!ReadFirmware(childNode, tFirmware))
            {
                CString errorMsg;
                errorMsg.FormatMessage(IDS_ERROR_PARAMETER_INIT);
                ::AfxMessageBox(errorMsg, MB_OK|MB_ICONERROR);
                return FALSE;
            }
            m_arrFirmware.Add(tFirmware);
            m_uiAnzFirmware++;
        }
        else
        {

        }
    }

    if (bProperties && m_uiAnzFirmware>0)
    {
        m_bInit = TRUE;
    }
    else
    {
        CString errorMsg;
        errorMsg.FormatMessage(IDS_ERROR_PARAMETER_INIT);
        ::AfxMessageBox(errorMsg, MB_OK|MB_ICONERROR);
        return FALSE;
    }
    return m_bInit;
}


//------------------------------------------------------------------*
/**
 * read properties.
 *
 * @param           node: xml root node.
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CBaseAddonParameter::ReadProperties(CXMLNode& node)
{
    if (!node.GetAttribute(ADDON_BASE_XML_ATTR_FILEEXT, m_strFileExtension))
    {
        return FALSE;
    }

    if (!node.GetAttribute(ADDON_BASE_XML_ATTR_FILEPREFIX, m_strFilePrefix))
    {
        return FALSE;
    }

    if (!node.GetAttribute(ADDON_BASE_XML_ATTR_KADTYPE, m_strKADType))
    {
        return FALSE;
    }

    CString strSupportNCC;
    if (!node.GetAttribute(ADDON_BASE_XML_ATTR_SUPPORTNCC, strSupportNCC))
    {
        return FALSE;
    }
    else
    {
        if (strSupportNCC.Compare("1")==0)
        {
            m_bSupportNCC = TRUE;
        }
        else if(strSupportNCC.Compare("2")==0)
        {
            m_bSupportNCC = TRUE;
            m_bNccBPSupport = TRUE;
        }
    }
    
    if (!node.GetAttribute(ADDON_BASE_XML_ATTR_WIZARDNAME, m_strWizardName))
    {
        return FALSE;
    }

    node.GetAttribute(ADDON_BASE_XML_ATTR_WIZARDDESC, m_strWizardDescription);

    node.GetAttribute(ADDON_BASE_XML_ATTR_DESCRIPTION, m_strDescription);

    m_bCustDL    = FALSE;
    m_bCustDLGui = FALSE;
    m_strCustDLDir = _T("");
    m_bBigEndian = FALSE;
    
    CString strTemp;
    if (node.GetAttribute(ADDON_BASE_XML_ATTR_CUSTDL, strTemp))
    {
        if (strTemp.Compare(_T("1"))==0)
        {
            m_bCustDL = TRUE;

            if (node.GetAttribute(ADDON_BASE_XML_ATTR_CUSTDL_GUI, strTemp))
            {
                if (strTemp.Compare(_T("1"))==0)
                {
                    m_bCustDLGui = TRUE;
                }
            }

            node.GetAttribute(ADDON_BASE_XML_ATTR_CUSTDL_DIR, m_strCustDLDir);
        }
    }

    if (node.GetAttribute(ADDON_BASE_XML_ATTR_ENDIAN, strTemp))
    {
        if (strTemp.CompareNoCase(ADDON_BASE_XML_VALUE_BIG)==0)
        {
            m_bBigEndian = TRUE;
        }
    }

    return TRUE;
}



//------------------------------------------------------------------*
/**
 * read firmware.
 *
 * @param           tFirmware: firmware data object
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CBaseAddonParameter::ReadFirmware(CXMLNode& node, CFirmwareData& tFirmware)
{
    CString strTmp;
    if (!node.GetAttribute(ADDON_BASE_XML_ATTR_VERSION, strTmp))
    {
        return FALSE;
    }
    tFirmware.m_uiVersion = atoi(strTmp);
    if (!node.GetAttribute(ADDON_BASE_XML_ATTR_DESCRIPTION, tFirmware.m_strVersionDescription))
    {
        return FALSE;
    }
    if (!node.GetAttribute(ADDON_BASE_XML_ATTR_CUSTOMCONNECT, tFirmware.m_strCustomConnectGuid))
    {
        return FALSE;
    }
    if (!node.GetAttribute(ADDON_BASE_XML_ATTR_DOWNLOAD, tFirmware.m_strDownloadGuid))
    {
        return FALSE;
    }

    // run over all children and get comm channel data
    CXMLNodeList childNodeList;

    if (!node.GetChildNodeList(childNodeList))
    {
        CString errorMsg;
        errorMsg.FormatMessage(IDS_ERROR_PARAMETER_INIT);
        ::AfxMessageBox(errorMsg, MB_OK|MB_ICONERROR);
        return FALSE;
    }

    CXMLNode    childNode;
    CString     strTagName;

    while (childNodeList.Next(childNode))
    {
        if (!childNode.GetTagName(strTagName))
        {
            CString errorMsg;
            errorMsg.FormatMessage(IDS_ERROR_PARAMETER_INIT);
            ::AfxMessageBox(errorMsg, MB_OK|MB_ICONERROR);
            return FALSE;
        }

        if(strTagName.Compare(ADDON_BASE_XML_TAG_COMMCHANNEL)==0)
        {
            CCommChannelData    tCommChannelData;

            if(!childNode.GetAttribute(ADDON_BASE_XML_ATTR_ID, tCommChannelData.m_strId))
            {
                return FALSE;
            }
//            if(childNode.GetAttribute(ADDON_BASE_XML_ATTR_DESCRIPTION, tCommChannelData.m_strDescription))
//            {
//                return FALSE;
//            }
            if(!childNode.GetAttribute(ADDON_BASE_XML_ATTR_PROTOCOL, tCommChannelData.m_strProtocol))
            {
                tCommChannelData.m_strProtocol = GetStandardProtocol(tCommChannelData.m_strId);
            }

            if(!childNode.GetAttribute(ADDON_BASE_XML_ATTR_PROTOCOLPAR, tCommChannelData.m_strProtocolPar))
            {
                tCommChannelData.m_strProtocolPar = GetStandardProtocolParameter(tCommChannelData.m_strId);
            }

            if(!childNode.GetAttribute(ADDON_BASE_XML_ATTR_STDCONNPAR, tCommChannelData.m_strStdConnPar))
            {
                tCommChannelData.m_strStdConnPar = GetStandardConnectParameter(tCommChannelData.m_strId);
            }

            tFirmware.m_tCommChannelArr.Add(tCommChannelData);
        }
    }
    return TRUE;
}



//------------------------------------------------------------------*
/**
 * build control string.
 *
 *  control string is built up like this:
 *  "<OPCServer>,<CustomConnectGuid>,<CommChannel>(<ConnectParameters>),VERSION(<FirmwareVersion>)"
 *      where:
 *      <OPCServer>: OPC-Server address
 *      <CustomConnectGuid>: GUID of custom connect interface
 *      <CommChannel>: communication channel "TCP" or "RS232" ...
 *      <ConnectParameters>: connect parameter string depending on communication channel
 *      <FirmwareVersion>: internal firmware version number
 *  
 * @param           crstrOPCServer: OPC-Server address
 * @param           crstrCommChannel: communication channel string
 * @param           crstrConnectPar: connect parameter string
 * @param           uiFirmwareIndex: index of firmware data in firmware array
 * @return          control string.
 * @exception       -
 * @see             ParseControlString()
*/
CString CBaseAddonParameter::BuildControlString
(
    const CString&  crstrOPCServer,
    const CString&  crstrCommChannel,
    const CString&  crstrConnectPar,
    UINT            uiFirmwareIndex
)
{
    HRESULT hr;
    CString strControl;
    CString strConnectString;
    CString strCustomConnectGuid;
    CString strFirmwareVersion;
    CString strProtocol;
    CString strProtocolPar;
    CString strOPCInstName;
    
    if(uiFirmwareIndex != UINT(-1))
    {
        ASSERT(uiFirmwareIndex < (UINT)m_arrFirmware.GetSize());
        if(uiFirmwareIndex < (UINT)m_arrFirmware.GetSize())
        {
            CFirmwareData&   tFirmwareData = m_arrFirmware[uiFirmwareIndex];
            CCommChannelData   tCommChannelData;
            if(tFirmwareData.GetCommChannelDataFromId(crstrCommChannel, tCommChannelData))
            {
                strCustomConnectGuid = tFirmwareData.m_strCustomConnectGuid;
                strFirmwareVersion.Format("%d", tFirmwareData.m_uiVersion);
                strProtocol = tCommChannelData.m_strProtocol;
                strProtocolPar = tCommChannelData.m_strProtocolPar;
            }
        }
    }

    strOPCInstName = BuildOPCInstName(crstrCommChannel, crstrConnectPar);

    hr = UTIL_BuildControlString(crstrOPCServer, strCustomConnectGuid, strOPCInstName, crstrCommChannel, crstrConnectPar, strProtocol, strProtocolPar, strFirmwareVersion, strControl.GetBuffer(UTIL_MAX_CONTROL_STRING_LEN));
    strControl.ReleaseBuffer();

    return strControl;
}

//------------------------------------------------------------------*
/**
 * parse control string.
 *  
 * @param           crstrControl: control string to be parsed
 * @param           rstrOPCServer: OPC-Server address
 * @param           ruiCommChannel: communication channel
 * @param           rstrConnectPar: connect parameter string
 * @param           ruiFirmwareIndex: index of firmware data in firmware array
 * @return          successful?
 * @exception       -
 * @see             BuildControlString()
*/
BOOL CBaseAddonParameter::ParseControlString
(
    const CString&  crstrControl, 
    CString&        rstrOPCServer, 
    CString&        rstrCommChannel, 
    CString&        rstrConnectPar, 
    UINT&           ruiFirmwareIndex
)
{

    HRESULT hr;
    CString strFirmwareVersion;
    UINT    uiFirmwareVersion;

    hr = UTIL_ParseControlString(crstrControl, 
            rstrOPCServer.GetBuffer(UTIL_MAX_OPC_SERVER_LEN), 
            NULL, // custom connect guid not needed
            NULL, // OPC server instance name not needed
            rstrCommChannel.GetBuffer(UTIL_MAX_COMM_CHANNEL_LEN), 
            rstrConnectPar.GetBuffer(UTIL_MAX_CONNECT_PAR_LEN), 
            NULL, // protocol not needed
            NULL, // protocol parameter not needed
            strFirmwareVersion.GetBuffer(UTIL_MAX_FIRMWARE_VERSION_LEN));

    rstrOPCServer.ReleaseBuffer();
    rstrCommChannel.ReleaseBuffer();
    rstrConnectPar.ReleaseBuffer();
    strFirmwareVersion.ReleaseBuffer();

    if(strFirmwareVersion.GetLength() == 5)
    {
        uiFirmwareVersion = atoi(strFirmwareVersion);
    }
    else
    {
        uiFirmwareVersion = STD_FIRMWARE_VERSION;
    }

    ruiFirmwareIndex = GetFirmwareIndex(uiFirmwareVersion);

    if(hr == S_OK)
    {
        return TRUE;
    }
    return FALSE;
}



//------------------------------------------------------------------*
/**
 * get firmware index from firmware version number.
 *
 * @param           uiVersion: firmware version number
 * @return          index of firmware structure
 *                  if not found return highest index (newest version)
 * @exception       -
 * @see             -
*/
UINT CBaseAddonParameter::GetFirmwareIndex(UINT uiVersion)
{
    // run over all firmware structures and find the right one
    UINT     uiNumFirmware = m_arrFirmware.GetSize();
    for(UINT uiFirmware = 0; uiFirmware < uiNumFirmware; ++uiFirmware)
    {
        if(uiVersion == m_arrFirmware[uiFirmware].m_uiVersion)
        {
            return uiFirmware;
        }
    }
    return 0;
}


//------------------------------------------------------------------*
/**
 * get number of firmware objects in array.
 *
 * @param           -
 * @return          number of firmware objects in array
 * @exception       -
 * @see             -
*/
int CBaseAddonParameter::GetFirmwareCount()
{
    return m_arrFirmware.GetSize();
}

//------------------------------------------------------------------*
/**
 * get firmware data.
 *
 * @param           uiFirmwareIndex: index of firmware object in array
 * @return          pointer to firmware data
 * @exception       -
 * @see             -
*/
CFirmwareData* CBaseAddonParameter::GetFirmwareData(UINT uiFirmwareIndex)
{
    int iNumFirmware = m_arrFirmware.GetSize();
    if(uiFirmwareIndex < (UINT) iNumFirmware)
    {
        return &(m_arrFirmware[uiFirmwareIndex]);
    }
    return NULL;
}


//------------------------------------------------------------------*
/**
 * get communication channel description.
 *
 * @param           crstrCommChannel: communication channel
 * @return          communication channel description (as used in combo boxes)
 * @exception       -
 * @see             -
*/
CString CBaseAddonParameter::GetCommChannelDescription(const CString& crstrCommChannel)
{
    CString strCommChannelDesc(crstrCommChannel);
    if(crstrCommChannel.CompareNoCase(COMM_CHANNEL_TCP) == 0)
    {
        strCommChannelDesc.LoadString(IDS_COMM_CHANNEL_DESC_TCP);
    }
    else if(crstrCommChannel.CompareNoCase(COMM_CHANNEL_RS232) == 0)
    {
        strCommChannelDesc.LoadString(IDS_COMM_CHANNEL_DESC_RS232);
    }
	// NFTASKHANDLING 19.05.05 SIS >>
    else if(crstrCommChannel.CompareNoCase(COMM_CHANNEL_NETBUI) == 0)
    {
        strCommChannelDesc.LoadString(IDS_COMM_CHANNEL_DESC_NETBUI);
    }
	// NFTASKHANDLING 19.05.05 SIS <<
	
    return strCommChannelDesc;
}

//------------------------------------------------------------------*
/**
 * get communication channel from description.
 *
 *  overwrite this function if extension regarding comm channels is required
 *
 * @param           crstrCommChannelDesc: communication channel description
 * @return          communication channel
 * @exception       -
 * @see             -
*/
CString CBaseAddonParameter::GetCommChannelFromDescription(const CString& crstrCommChannelDesc)
{
    CString strCommChannelDescTmp;
    strCommChannelDescTmp.LoadString(IDS_COMM_CHANNEL_DESC_TCP);
    if(crstrCommChannelDesc.CompareNoCase(strCommChannelDescTmp) == 0)
    {
        return COMM_CHANNEL_TCP;
    }
    strCommChannelDescTmp.LoadString(IDS_COMM_CHANNEL_DESC_RS232);
    if(crstrCommChannelDesc.CompareNoCase(strCommChannelDescTmp) == 0)
    {
        return COMM_CHANNEL_RS232;
    }
	// NFTASKHANDLING 19.05.05 SIS >>	
	strCommChannelDescTmp.LoadString(IDS_COMM_CHANNEL_DESC_NETBUI);
    if(crstrCommChannelDesc.CompareNoCase(strCommChannelDescTmp) == 0)
    {
        return COMM_CHANNEL_NETBUI;
    }
	// NFTASKHANDLING 19.05.05 SIS <<
    return _T("");
}

//------------------------------------------------------------------*
/**
 * get standard communication channel.
 *
 *  returns first communication channel in firmware data.
 *
 * @param           uiFirmwareIndex: index of firmware data in array
 * @return          standard communication channel.
 * @exception       -
 * @see             -
*/
CString CBaseAddonParameter::GetStandardCommChannel(UINT uiFirmwareIndex)
{
    CString strCommChannel;
    CFirmwareData*  pFirmwareData = GetFirmwareData(uiFirmwareIndex);
    if(pFirmwareData)
    {
        if(pFirmwareData->m_tCommChannelArr.GetSize() > 0)
        {
            strCommChannel = pFirmwareData->m_tCommChannelArr[0].m_strId;
        }
    }
    return strCommChannel;
}

//------------------------------------------------------------------*
/**
 * get standard connect parameter for comm channel.
 *
 * @param           crstrCommChannel: communication channel
 * @return          standard connect parameter
 * @exception       -
 * @see             -
*/
CString CBaseAddonParameter::GetStandardConnectParameter(const CString& crstrCommChannel)
{
    CString strStdConnPar;
    if(crstrCommChannel.CompareNoCase(COMM_CHANNEL_TCP) == 0)
    {
        // get local machine
//        UTIL_GetLocalMachineName(strStdConnPar.GetBuffer(100), 100);
//        strStdConnPar.ReleaseBuffer();
        strStdConnPar = _T("localhost");
    }
    else if(crstrCommChannel.CompareNoCase(COMM_CHANNEL_RS232) == 0)
    {
        strStdConnPar = STD_RS232_PAR;
    }
	// NFTASKHANDLING 19.05.05 SIS >>
    else if(crstrCommChannel.CompareNoCase(COMM_CHANNEL_NETBUI) == 0)
    {
        return _T("NETBUI");
    }
	// NFTASKHANDLING 19.05.05 SIS <<
    return strStdConnPar;
}


//------------------------------------------------------------------*
/**
 * get standard protocol.
 *
 * @param           crstrCommChannel: communication channel
 * @return          standard protocol
 * @exception       -
 * @see             -
*/
CString CBaseAddonParameter::GetStandardProtocol(const CString& crstrCommChannel)
{
	// NFTASKHANDLING 19.05.05 SIS >>
    if(crstrCommChannel.CompareNoCase(COMM_CHANNEL_NETBUI) == 0)
    {
        return PROTOCOL_SIMPLE;
    }
	// NFTASKHANDLING 19.05.05 SIS <<
    return STD_PROTOCOL;
}


//------------------------------------------------------------------*
/**
 * get standard protocol parameter.
 *
 * @param           crstrCommChannel: communication channel
 * @return          standard protocol parameter
 * @exception       -
 * @see             -
*/
CString CBaseAddonParameter::GetStandardProtocolParameter(const CString& crstrCommChannel)
{
	// NFTASKHANDLING 19.05.05 SIS >>
    if(crstrCommChannel.CompareNoCase(COMM_CHANNEL_NETBUI) == 0)
    {
        return STD_NETBUI_PAR;
    }
	// NFTASKHANDLING 19.05.05 SIS <<
    return STD_PROTOCOL_PARAMETER;
}


//------------------------------------------------------------------*
/**
 * build OPC server instance name.
 *
 *  in case of TCP/IP this is target IP address
 *  in case of RS232 this is COM-Port
 *
 * @param           crstrCommChannel: communication channel
 * @param           crstrConnectPar: connection parameter string
 * @return          OPC server instance name
 * @exception       -
 * @see             -
*/
CString CBaseAddonParameter::BuildOPCInstName(const CString& crstrCommChannel, const CString& crstrConnectPar)
{
    CString strOPCInstName(crstrConnectPar);
	// NFTASKHANDLING 19.05.05 SIS >>
    if(crstrCommChannel.CompareNoCase(COMM_CHANNEL_RS232) == 0)
    {
        int iFound = crstrConnectPar.Find(_T(':'));
        ASSERT(iFound >= 0);
        if(iFound >= 0)
        {
            strOPCInstName = crstrConnectPar.Left(iFound);
        }
    }
    else	// default -> take connect parameter as opc instance name
    {
        strOPCInstName = crstrConnectPar;
    }
	// NFTASKHANDLING 19.05.05 SIS <<
    return strOPCInstName;
}


//------------------------------------------------------------------*
/**
 * get all communication channels.
 *
 *  run through all firmware data objects and add communication channels
 *  preserves order of occurrence
 *
 * @param           rastrCommChannels: string array with communication channels
 * @return          number of comm channels
 * @exception       -
 * @see             -
*/
int CBaseAddonParameter::GetAllCommChannels(CStringArray& rastrCommChannels)
{
    // run over all firmware data objects
    int             iNumFirmware = GetFirmwareCount();
    int             iFirmware;
    int             iNumCommChannels;
    int             iCommChannel;
    CFirmwareData*  pFirmwareData;

    for(iFirmware = 0; iFirmware < iNumFirmware; ++iFirmware)
    {
        pFirmwareData = GetFirmwareData(iFirmware);
        iNumCommChannels = pFirmwareData->m_tCommChannelArr.GetSize();
        for(iCommChannel = 0; iCommChannel < iNumCommChannels; ++iCommChannel)
        {
            AddCommChannel(rastrCommChannels, pFirmwareData->m_tCommChannelArr[iCommChannel].m_strId);
        }
    }
    return rastrCommChannels.GetSize();
}


//------------------------------------------------------------------*
/**
 * add communication channel to string array helper function.
 *
 *  runs through array and looks for communication channel.
 *  if not already present, add it.
 *
 * @param           rastrCommChannels: string array with communictaion channels
 * @param           crstrCommChannel: new communication channel to add.
 * @return          successful?
 * @exception       -
 * @see             -
*/
void CBaseAddonParameter::AddCommChannel(CStringArray& rastrCommChannels, const CString& crstrCommChannel)
{
    BOOL    bFound = FALSE;
    int iNumCommChannels = rastrCommChannels.GetSize();
    for(int iCommChannel = 0; iCommChannel < iNumCommChannels; ++iCommChannel)
    {
        if(crstrCommChannel.CompareNoCase(rastrCommChannels[iCommChannel]) == 0)
        {
            bFound = TRUE;
            break;
        }
    }
    if(!bFound)
    {
        rastrCommChannels.Add(crstrCommChannel);
    }
}


//------------------------------------------------------------------*
/**
 * get maximum firmware version number.
 *
 * @param           riMaxFirmwareIndex: index of firmware data with maximum number
 * @return          maximum firmware version number
 * @exception       -
 * @see             -
*/
int CBaseAddonParameter::GetMaxFirmwareVersion(int& riMaxFirmwareIndex)
{
    // determine firmware index with highest version
    CFirmwareData*  pFirmwareData;
    int             iFirmwareCount = GetFirmwareCount();
    UINT            uiMaxFirmwareVersion = 0;

    riMaxFirmwareIndex = 0;

    ASSERT(iFirmwareCount > 0);
    
    // run over firmware array and add firmware version description
    for(int iFirmware = 0; iFirmware < iFirmwareCount; ++iFirmware)
    {
        pFirmwareData = GetFirmwareData(iFirmware);
        if(pFirmwareData->m_uiVersion > uiMaxFirmwareVersion)
        {
            uiMaxFirmwareVersion = pFirmwareData->m_uiVersion;
            riMaxFirmwareIndex = iFirmware;
        }
    }
    return uiMaxFirmwareVersion;
}

/* ---------------------------------------------------------------------------- */
