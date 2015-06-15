#ifndef _BASETARGETDATA_H_
#define _BASETARGETDATA_H_

#include "afxtempl.h"
#include "CEProjInfo.h"
#include "BaseAddonParameter.h"

class CBaseTargetObject;

class CConnectPar
{
public:
    CConnectPar() {};

    CConnectPar(const CString& crstrCommChannel, const CString& crstrConnectPar) 
      : m_strCommChannel(crstrCommChannel),
        m_strConnectPar(crstrConnectPar)
    {
    }

    CString m_strCommChannel;
    CString m_strConnectPar;
};


//------------------------------------------------------------------*
/**
 *  class CBaseTargetData
 *
 *  - target data class
 *  - encapsulates all target data and operations (not target type data)
 *  - creates target type data CBaseAddonParameter (CreateAddonParameter())
 *    to read target type data from target adaption KAD
 *  - loading and saving target files
 *  - operations new / delete / rename target, properties dialog, assignment
 *    dialog, settings dialog
 *  - backparsing
 *
 *  @see    CBaseAddonParameter
 */
class CBaseTargetData 
{
public:
	CBaseTargetData(CBaseTargetObject* pTargetObject,
                    ICEProjInfo* pICEProjInfo,
                    const CString& crstrKADFileName);
	virtual ~CBaseTargetData();

    virtual void Init();

    // global operations
    virtual HRESULT OpNew(const CString& crstrSourceFile, 
                          const CString& crstrSourceId, 
                          const CString& crstrAddInfo);

    virtual HRESULT OpRename(const CString& crstrSourceFile, 
                     const CString& crstrSourceId, 
                     const CString& crstrAddInfo);

    virtual HRESULT OpDelete(const CString& crstrSourceFile, 
                     const CString& crstrSourceId, 
                     const CString& crstrAddInfo);
    
    virtual HRESULT OpAssignment(const CString& crstrSourceFile, 
                                 const CString& crstrSourceId, 
                                 const CString& crstrAddInfo);

    virtual HRESULT OpProperties(const CString& crstrSourceFile, 
                                 const CString& crstrSourceId, 
                                 const CString& crstrAddInfo);

    virtual HRESULT OpSettings(const CString& crstrSourceFile, 
                               const CString& crstrSourceId, 
                               const CString& crstrAddInfo);

    virtual HRESULT OpCustomDownload(const CString& crstrSourceFile, 
                                     const CString& crstrSourceId, 
                                     const CString& crstrAddInfo);


    // loading / saving target files
    virtual HRESULT Load(const CString& crstrFileName, CXMLDocument& doc);
    virtual HRESULT Save(const CString& crstrFileName, CXMLDocument& doc);

    // backparsing
    virtual void SetBackparseAttributes(CXMLNode& rtNode);

    // general information
    CString     GetProjectPath();
	// NFTASKHANDLING 01.06.05 SIS >>
	CString		GetProjectName();
	CString		GetProjectFilePath();
	// NFTASKHANDLING 01.06.05 SIS <<
    ICEProjInfo* GetProjInfo() {return m_pICEProjInfo;}

    // communication
    CString GetConnectParameter(const CString& crstrCommChannel);
    void SetConnectParameter(const CString& crstrCommChannel, const CString& crstrConnectPar);

    // helper functions for assignment and properties dialog
    CString GetResource();
    void    SetResource(const CString& crstrResource);
    HRESULT GetAllResourcesFromProject(CStringArray& rastrAllResources);

    CFirmwareData*  GetActFirmwareData();

    CBaseAddonParameter* GetAddonParameter();
    BOOL EnsureTargetDisconnected(const CString& crstrTargetId, BOOL bRebuildHint);

    void OutputDownloadMessage(HRESULT hError, const CString& crstrTarget,
                                               const CString& sMsg);

    virtual int CheckFileNameConventions(const CString& crstrFileName);

    BOOL IsNCCBreakpointSupport();  // ncc with breakpoint support?

protected:
    // create addon parameter (target type specific data)
    virtual CBaseAddonParameter* CreateAddonParameter();

    // helper functions for backparsing
    void SetStandardAttributes(CXMLNode& rtNode);
    virtual CString GetNodeText();

    // helper functions for load and save
    virtual CString GetCtrlAddress();
    virtual CString GetUserCtrlAddress();
    virtual void    SetCtrlAddress(const CString& crstrCtrlAddress);

	// #d-160 01.02.05 SIS >>
	virtual void SetActFirmware(const CString& crstrCtrlAddress);
	// #d-160 01.02.05 SIS <<

    // helper functions for project info and container interfaces
    BOOL AddToProject(const CString& crstrTargetFile, const CString& crstrParentFile, const CString& crstrIdPath);

    HRESULT CreateParentLink(const CString& crstrParentFile,
                             const CString& crstrParentId,
                             const CString& crstrLinkName);
    HRESULT DeleteParentLink(const CString& crstrParentFile,
                             const CString& crstrParentId,
                             const CString& crstrLinkName);

    CString GetFileAbsolute(const CString& crstrFileName);
    CString GetFileRelative(const CString& crstrFileName);
    CString GetFileName(const CString& crstrFileName);

    // this method can be overwritten in derived classes to do additional checks
    // call the base class function to do the basic checks first.
    virtual BOOL CheckNewName(const CString& crstrNewFileName, const CString& crstrParentId);

    // helper function for loading: connect parameters
    virtual void LoadConnectParameters(CXMLNode& rtRootNode);


public:
    // addon data for property pages
    // general page
    CString             m_strName;              // target name (ID)
    CString             m_strCreateDate;        // creation date string
    CString             m_strCreateUser;        // creation user name
    CString             m_strModifyDate;        // modify date string
    CString             m_strModifyUser;        // modify user name
    CString             m_strOPCServer;         // opc server name
    CString             m_strResource;          // resource name

    UINT                m_uiActFirmware;        // which firmware has been selected
    CString             m_strCommChannel;       // communication channel

    // custom download 
    CString             m_strCustomDownloadDir;

    // info page
    CString             m_strText1;             // standard attribute text1
    CString             m_strText2;             // standard attribute text2
    CString             m_strText3;             // standard attribute text3
    CString             m_strAlias;             // standard attribute alias
    CString             m_strUrl;               // standard attribute URL

    // connect parameter data
    CArray<CConnectPar, CConnectPar&>  m_tConnectParArray;  // connect parameter array

    // help data
    CStringArray        m_astrAllResources;     // string array with all resource names
    CString             m_strParentId;          // help to make parentID available to property page

    

protected:
    CBaseTargetObject*      m_pTargetObject;    // target object
    CString                 m_strProjectPath;   // project path
    ICEProjInfo*            m_pICEProjInfo;     // project info interface pointer
    CBaseAddonParameter*    m_pAddonPar;        // base addon parameter
    CString                 m_strKADFileName;   // kad file name
};

#endif
