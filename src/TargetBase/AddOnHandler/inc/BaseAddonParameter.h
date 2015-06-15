#ifndef _BASEADDONPARAMETER_H_
#define _BASEADDONPARAMETER_H_

// include xml support
#import <msxml.dll> raw_interfaces_only named_guids rename_namespace("MSXML_4C")
using namespace MSXML_4C;

// and xml helper classes
#include "XMLHelper\XMLDocument.h"
#include "XMLHelper\XMLNode.h"
#include "XMLHelper\XMLNodeList.h"
#include "XMLHelper\IdPathHelp.h"
#include "XMLHelper\XMLNodeIter.h"
#include "FirmwareData.h"


// some defines for the adaptation kad
#define ADDON_BASE_XML_TAG_ADDON            _T("ADDON")
#define ADDON_BASE_XML_TAG_PROPERTIES       _T("PROPERTIES")
#define ADDON_BASE_XML_ATTR_FILEEXT         _T("FILEEXT")
#define ADDON_BASE_XML_ATTR_FILEPREFIX      _T("FILEPREFIX")
#define ADDON_BASE_XML_ATTR_KADTYPE         _T("KADTYPE")
#define ADDON_BASE_XML_ATTR_DESCRIPTION     _T("DESCRIPTION")
#define ADDON_BASE_XML_ATTR_SUPPORTNCC      _T("SUPPORTNCC")
#define ADDON_BASE_XML_ATTR_WIZARDNAME      _T("WIZARDNAME")
#define ADDON_BASE_XML_ATTR_WIZARDDESC      _T("WIZARDDESC")
#define ADDON_BASE_XML_TAG_FIRMWARE         _T("FIRMWARE")
#define ADDON_BASE_XML_ATTR_VERSION         _T("VERSION")
#define ADDON_BASE_XML_ATTR_CUSTOMCONNECT   _T("CUSTOMCONNECT")
#define ADDON_BASE_XML_ATTR_DOWNLOAD        _T("DOWNLOAD")
#define ADDON_BASE_XML_TAG_COMMCHANNEL      _T("COMMCHANNEL")
#define ADDON_BASE_XML_ATTR_ID              _T("ID")
#define ADDON_BASE_XML_ATTR_STDCONNPAR      _T("STDCONNPAR")
#define ADDON_BASE_XML_ATTR_PROTOCOL        _T("PROTOCOL")
#define ADDON_BASE_XML_ATTR_PROTOCOLPAR     _T("PROTOCOLPAR")
#define ADDON_BASE_XML_ATTR_CUSTDL          _T("CUSTOM_DOWNLOAD")
#define ADDON_BASE_XML_ATTR_CUSTDL_GUI      _T("CUSTOM_DOWNLOAD_GUI")
#define ADDON_BASE_XML_ATTR_CUSTDL_DIR      _T("CUSTOM_DOWNLOAD_DIR")
#define ADDON_BASE_XML_ATTR_ENDIAN          _T("ENDIAN")
#define ADDON_BASE_XML_VALUE_BIG            _T("BIG")
#define ADDON_BASE_XML_VALUE_LITTLE         _T("LITTLE")

#define STD_FIRMWARE_VERSION                20000


//------------------------------------------------------------------*
/**
 *  class CBaseAddonParameter
 *
 *  - contains target type data from target adaption KAD
 *    - target file extension, KAD type, file prefix ...
 *    - firmware data (incl. communication channel data)
 *  - building and parsing control string
 *
 *  @see    ....
 */
class CBaseAddonParameter  
{
public:
    // creation
	CBaseAddonParameter();
	virtual ~CBaseAddonParameter();

    // reading KAD file
    virtual BOOL LoadKADFile(const CString crstrKADFileName);
    BOOL IsInitialized() {return m_bInit;}

    // firmware data functions
    int GetFirmwareCount();
    CFirmwareData* GetFirmwareData(UINT uiFirmwareIndex);
    
    virtual CString GetCommChannelDescription(const CString& crstrCommChannel);
    virtual CString GetCommChannelFromDescription(const CString& crstrCommChannelDescr);
    int GetAllCommChannels(CStringArray& rastrCommChannels);

    virtual CString GetStandardCommChannel(UINT uiFirmwareIndex);
    virtual CString GetStandardConnectParameter(const CString& crstrCommChannel);
    virtual CString GetStandardProtocol(const CString& crstrCommChannel);
    virtual CString GetStandardProtocolParameter(const CString& crstrCommChannel);

    virtual CString BuildOPCInstName(const CString& crstrCommChannel, const CString& crstrConnectPar);

    // control string functions
    CString BuildControlString( const CString& crstrOPCServer,
                                const CString& crstrCommChannel,
                                const CString& crstrConnectPar,
                                UINT uiFirmwareIndex);
    BOOL ParseControlString(    const CString& crstrControl, 
                                CString& rstrOPCServer, 
                                CString& rstrCommChannel, 
                                CString& rstrConnectPar, 
                                UINT& ruiFirmwareIndex);

    int GetMaxFirmwareVersion(int& riMaxFirmwareIndex);

protected:
    // reading KAD file
    virtual BOOL ReadProperties(CXMLNode& node);
    virtual BOOL ReadFirmware(CXMLNode& node, CFirmwareData& tFirmware);

    // firmware helper function
    UINT GetFirmwareIndex(UINT uiVersion);

private:
    // helper function for GetAllCommChannels()
    void AddCommChannel(CStringArray& rastrCommChannels, const CString& crstrCommChannel);


public:
    CString m_strFileExtension;     // file extension
    CString m_strKADType;           // KAD target type
    CString m_strFilePrefix;        // file prefix for generated file names
    CString m_strDescription;       // target module description for message boxes
    BOOL    m_bSupportNCC;          // supports native code compiler?
    BOOL    m_bNccBPSupport;        // native code compiler supports breakpoints?
    CString m_strWizardName;        // wizard name
    CString m_strWizardDescription; // wizard description 

    BOOL    m_bCustDL;
    BOOL    m_bCustDLGui;
    CString m_strCustDLDir;
    BOOL    m_bBigEndian;
    
protected:
    // firmware data
    CArray<CFirmwareData, CFirmwareData&> m_arrFirmware;
    UINT m_uiAnzFirmware;

    // is class initialized successfully?
    BOOL m_bInit;

};

#endif
