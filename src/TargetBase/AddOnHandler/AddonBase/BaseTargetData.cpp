/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"
#include "BaseTargetData.h"
#include "AddonBaseResource.h"
#include "BaseTargetObject.h"

#include "TargetPropertySheet.h"
#include "TargetSettingsSheet.h"
#include "AssignmentDialog.h"

#include "basedefs.h"
#include "CeSysDef.h"
#include "utilif.h"

#include "fc_todebug\fc_assert.h"
#include "fc_tools\fc_fileIO.h"
#include "fc_tools\fc_cstring.h"

#define  FC_LIB_USE_CRT 1
#include "fc_tools\fc_libmem.h"

#include "vmSharedDef.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------*
/**
 * CBaseTargetData constructor.
 *
 * @param           pTargetObject: pointer to target object
 * @param           pICEProjInfo: project info interface pointer
 * @param           crstrKADFileName: file name of the adaptation kad
 * @return          -
 * @exception       -
 * @see             -
*/
CBaseTargetData::CBaseTargetData(CBaseTargetObject* pTargetObject,
                                 ICEProjInfo* pICEProjInfo,
                                 const CString& crstrKADFileName)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    m_uiActFirmware = 0;

    ASSERT(pICEProjInfo);
    m_pICEProjInfo = pICEProjInfo;
    m_pICEProjInfo->AddRef();

    ASSERT(pTargetObject);
    m_pTargetObject = pTargetObject;

    m_pAddonPar = 0;
    m_strKADFileName = crstrKADFileName;
}


//------------------------------------------------------------------*
/**
 * init.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CBaseTargetData::Init()
{
    m_pAddonPar = CreateAddonParameter();
    ASSERT(m_pAddonPar);
    
    // could the addon be adapted via kad in CBaseAddonParameter, else
    // stop here, addon handler can not work
    if(!m_pAddonPar->LoadKADFile(m_strKADFileName))
    {
        return;
    }

    // init firmware and communication parameter
    CFirmwareData* pFirmwareData = GetActFirmwareData();
    if(pFirmwareData->m_tCommChannelArr.GetSize())
    {
        m_strCommChannel = pFirmwareData->m_tCommChannelArr[m_uiActFirmware].m_strId;
    }

    // some more initializations
    TCHAR szUserName[MAXUSERNAMELEN] = _T("");
    DWORD Size = sizeof(szUserName) / sizeof(szUserName[0]);
    ::GetUserName(szUserName, &Size);
    m_strCreateUser = szUserName;
    m_strModifyUser = szUserName;
}



CBaseTargetData::~CBaseTargetData()
{
    m_pICEProjInfo->Release();
    if(m_pAddonPar)
    {
        delete m_pAddonPar;
        m_pAddonPar = NULL;
    }
}

//------------------------------------------------------------------*
/**
 * create addon parameter.
 *
 * @return          addon parameter object
 * @exception       -
 * @see             -
*/
CBaseAddonParameter* CBaseTargetData::CreateAddonParameter()
{
    return new CBaseAddonParameter;
}


//------------------------------------------------------------------*
/**
 * operation new.
 *
 * @param           crstrSourceFile: parent source file name
 * @param           crstrSourceId: id of parent
 * @param           crstrAddInfo: additional info
 * @return          S_OK: ok
 *                  E_FAIL: error occurred
 * @exception       -
 * @see             -
*/
HRESULT CBaseTargetData::OpNew(const CString& crstrSourceFile, 
                               const CString& crstrSourceId, 
                               const CString& crstrAddInfo)
{
    HRESULT hr = S_OK;
    CString strTitle;
    strTitle.LoadString(IDS_PROP_TITLE_NEW);
    strTitle += _T(" ") + m_pAddonPar->m_strDescription;

    CTargetPropertySheet propSheet(this, TRUE, strTitle);
    propSheet.AddPages();
    propSheet.InitPages();

    m_strParentId = crstrSourceId;

    if (propSheet.DoModal() == IDOK)
    {
        CString strFileName = m_strName + _T(".") + m_pAddonPar->m_strFileExtension;
        CString strFilePath = GetFileAbsolute(strFileName);

        // test if file already exists
        CFileStatus fileStatus;
        if (CFile::GetStatus(strFilePath, fileStatus))
        {
            // file already exists -> ask user
            CString errorMsg;
            errorMsg.FormatMessage(IDS_ERROR_FILE_EXISTS, strFilePath);
            if(::AfxMessageBox(errorMsg) != ID_OK)
            {
                return S_FALSE;
            }
        }

        CXMLDocument doc;
        hr = Save(strFileName, doc);

        if(hr != S_OK)
        {
            CString errorMsg;
            errorMsg.FormatMessage(IDS_ERROR_SAVING_FILE, 
                                   strFilePath);
            ::AfxMessageBox(errorMsg, MB_OK|MB_ICONERROR);
            return hr;
        }

        if(!AddToProject(strFileName, crstrSourceFile, crstrSourceId))
        {
            CString errorMsg;
            errorMsg.FormatMessage(IDS_ERROR_ADD_TO_PROJECT, 
                                   strFilePath);
            ::AfxMessageBox(errorMsg, MB_OK|MB_ICONERROR);
            return E_FAIL;
        }
    }

    return hr;
}



//------------------------------------------------------------------*
/**
 * operation rename.
 *
 * @param           crstrSourceFile: parent source file name
 * @param           crstrSourceId: id of parent
 * @param           crstrAddInfo: additional info
 * @return          S_OK: ok
 *                  E_FAIL: error occurred
 * @exception       -
 * @see             
*/
HRESULT CBaseTargetData::OpRename(const CString& crstrSourceFile, 
                                  const CString& crstrSourceId, 
                                  const CString& crstrAddInfo)
{
    HRESULT hr;

    if (GetFileName(crstrSourceFile).CompareNoCase(crstrAddInfo)==0)
    {
        return S_OK;
    }
    
    // get some necessary information
    CComBSTR sSourceFile(crstrSourceFile);
    CComBSTR sSourceId(crstrSourceId);
    CComPtr<ICEContainer> pContainer;
    CComBSTR sParentSource;
    CComBSTR sParentId;

    hr = m_pICEProjInfo->getContainerIF(sSourceFile, 
                                        sSourceId,  
                                        &pContainer,
                                        &sParentSource, 
                                        &sParentId);

    if (hr!=S_OK)
    {
        CString errorMsg;
        errorMsg.FormatMessage(IDS_ERR_GET_DESC);
        ::AfxMessageBox(errorMsg, MB_OK|MB_ICONERROR);
        return E_FAIL;
    }

    CString strParentFile(sParentSource);
    CString strParentId(sParentId);

    // check new name
    if (!CheckNewName(crstrAddInfo, strParentId))
    {
        return E_FAIL;
    }

    CComBSTR    sTargetId(m_strName);
    hr = m_pTargetObject->Fire_OnEnsureTargetDisconnected(sTargetId, TRUE);
    if(hr != S_OK)
    {
        return S_FALSE;
    }

    CString strNewFileName = crstrAddInfo + _T(".") + m_pAddonPar->m_strFileExtension;

    // rename in container
    CComBSTR sOldLinkName(GetFileRelative(crstrSourceFile));
    CComBSTR sNewLinkName(strNewFileName);
    hr = pContainer->renameLink(sParentSource, sParentId, sOldLinkName, sNewLinkName);
    if (hr!=S_OK)
    {
        CString errorMsg;
        errorMsg.FormatMessage(IDS_ERR_RENAME_LINK, strNewFileName);
        ::AfxMessageBox(errorMsg, MB_OK|MB_ICONERROR);
        return E_FAIL;
    }

    // rename file
    try
    {
        CFile::Rename(GetFileAbsolute(crstrSourceFile), GetFileAbsolute(strNewFileName));
    }
    catch(CFileException* e)
    {
        HRESULT hr;
        hr = HRESULT_FROM_WIN32(e->m_lOsError);
        e->Delete();
    
        // recover old link in container
        hr = pContainer->renameLink(sParentSource, sParentId, sNewLinkName, sOldLinkName);

        CString errorMsg;
        errorMsg.FormatMessage(IDS_ERR_RENAME_FILE, GetFileAbsolute(crstrSourceFile));
        ::AfxMessageBox(errorMsg, MB_OK|MB_ICONERROR);
        return E_FAIL;
    }

    m_pICEProjInfo->fileChangedHint(sParentSource);

    return S_OK;
}



//------------------------------------------------------------------*
/**
 * operation delete.
 *
 * @param           crstrSourceFile: parent source file name
 * @param           crstrSourceId: id of parent
 * @param           crstrAddInfo: additional info
 * @return          S_OK: ok
 *                  E_FAIL: error occurred
 * @exception       -
 * @see             
*/
HRESULT CBaseTargetData::OpDelete(const CString& crstrSourceFile, 
                                 const CString& crstrSourceId, 
                                 const CString& crstrAddInfo)
{
    BOOL    bSilent(FALSE);
    if(crstrAddInfo.CompareNoCase(_T("SILENT")) == 0)
    {
        bSilent = TRUE;
    }

    HRESULT hr;

    // first ask user
    CString strMessage;
    CString strId;

    int iFound = crstrSourceId.ReverseFind(_T('/'));
    strId = crstrSourceId.Mid(iFound + 1);

    if(!bSilent)
    {
        strMessage.Format(IDS_DELETE_DIALOG_TEXT, strId);
        if(::AfxMessageBox(strMessage, MB_YESNO) != IDYES)
        {
            return S_FALSE;
        }
    }

    CComBSTR    sTargetId(strId);
    hr = m_pTargetObject->Fire_OnEnsureTargetDisconnected(sTargetId, FALSE);
    if(hr != S_OK)
    {
        return S_FALSE;
    }

    // get some necessary information
    CComBSTR sSourceFile(crstrSourceFile);
    CComBSTR sSourceId(crstrSourceId);
    CComPtr<ICEContainer> pContainer;
    CComBSTR sParentSource;
    CComBSTR sParentId;

    hr = m_pICEProjInfo->getContainerIF(sSourceFile, 
                                        sSourceId,  
                                        &pContainer,
                                        &sParentSource, 
                                        &sParentId);

    if (hr!=S_OK || !pContainer)
    {
        CString errorMsg;
        errorMsg.FormatMessage(IDS_ERR_GET_DESC);
        ::AfxMessageBox(errorMsg, MB_OK|MB_ICONERROR);
        return E_FAIL;
    }

    CString strParentFile(sParentSource);
    CString strParentId(sParentId);

    // delete in container
    CComBSTR sLinkName(crstrSourceFile);
    hr = pContainer->deleteLink(sParentSource, sParentId, sLinkName);
    if (hr!=S_OK)
    {
        CString errorMsg;
        errorMsg.FormatMessage(IDS_ERR_DELETE_LINK, crstrSourceFile);
        ::AfxMessageBox(errorMsg, MB_OK|MB_ICONERROR);
        return E_FAIL;
    }

    CString strFileAbsolute = GetFileAbsolute(crstrSourceFile);

    // remove file
    try
    {
        CFile::Remove(strFileAbsolute);
    }
    catch(CFileException* e)
    {
        HRESULT hr;
        hr = HRESULT_FROM_WIN32(e->m_lOsError);
        e->Delete();
    
        CString errorMsg;
        errorMsg.FormatMessage(IDS_ERR_DELETE_FILE, strFileAbsolute);
        ::AfxMessageBox(errorMsg, MB_OK|MB_ICONERROR);
        return E_FAIL;
    }

    m_pICEProjInfo->fileChangedHint(sParentSource);

    return S_OK;
}



//------------------------------------------------------------------*
/**
 * operation assignment.
 *
 * @param           crstrSourceFile: source file name
 * @param           crstrSourceId: id of parent
 * @param           crstrAddInfo: additional info
 * @return          S_OK: ok
 *                  E_FAIL: error occurred
 * @exception       -
 * @see             GetAllResourcesFromProject(), Fire_OnEnsureTargetDisconnected()
*/
HRESULT CBaseTargetData::OpAssignment(const CString& crstrSourceFile, 
                                      const CString& crstrSourceId, 
                                      const CString& crstrAddInfo)
{
    HRESULT         hr = S_OK;
    CXMLDocument    tXMLDocument;

    hr = Load(crstrSourceFile, tXMLDocument);
    if(hr != S_OK)
    {
        return hr;
    }
 
    CStringArray astrAllResources;

    hr = GetAllResourcesFromProject(astrAllResources);
    if(hr != S_OK)
    {
        return hr;
    }

    CAssignmentDialog tAssignmentDlg(astrAllResources, m_strResource);
    
    if (tAssignmentDlg.DoModal()==ID_OK)
    {
        if (m_strResource.CompareNoCase(tAssignmentDlg.m_strResource) != 0)
        {
            // now ensure that target is disconnected
            HRESULT hr;
            CComBSTR    sTargetId(m_strName);
            hr = m_pTargetObject->Fire_OnEnsureTargetDisconnected(sTargetId, TRUE);
            if(hr != S_OK)
            {
                return hr;
            }

            // set resource
            m_strResource = tAssignmentDlg.m_strResource;

            // save document
            CXMLDocument doc;
            hr = Save(crstrSourceFile, doc);

            // reparse file
            CComBSTR sFileName(crstrSourceFile);
            m_pICEProjInfo->fileChangedHint(sFileName);
        }
    }
    return S_OK;
}



//------------------------------------------------------------------*
/**
 * operation properties.
 *
 * @param           crstrSourceFile: source file name
 * @param           crstrSourceId: id of parent
 * @param           crstrAddInfo: additional info
 * @return          S_OK: ok
 *                  E_FAIL: error occurred
 * @exception       -
 * @see             -
*/
HRESULT CBaseTargetData::OpProperties(const CString& crstrSourceFile, 
                                      const CString& crstrSourceId, 
                                      const CString& crstrAddInfo)
{
    HRESULT         hr = S_OK;
    CString         strTitle;
    CXMLDocument    tXMLDocument;

    hr = Load(crstrSourceFile, tXMLDocument);
    if(hr != S_OK)
    {
        return hr;
    }

    strTitle.LoadString(IDS_PROP_TITLE);
    strTitle += _T(" ") + m_pAddonPar->m_strDescription;
    CTargetPropertySheet propSheet(this, FALSE, strTitle);
    propSheet.AddPages();
    propSheet.InitPages();

    m_strParentId = crstrSourceId;

    if (propSheet.DoModal() == IDOK)
    {
        CXMLDocument doc;
        hr = Save(crstrSourceFile, doc);

        CComBSTR sFileName(crstrSourceFile);
        m_pICEProjInfo->fileChangedHint(sFileName);
    }

    return hr;
}



//------------------------------------------------------------------*
/**
 * operation settings.
 *
 * @param           crstrSourceFile: source file name
 * @param           crstrSourceId: id of parent
 * @param           crstrAddInfo: additional info
 * @return          S_OK: ok
 * @exception       -
 * @see             -
*/
HRESULT CBaseTargetData::OpSettings(const CString& crstrSourceFile, 
                                    const CString& crstrSourceId, 
                                    const CString& crstrAddInfo)
{
    HRESULT         hr;
    CXMLDocument    tXMLDocument;

    CTargetSettingsSheet tTargetSettings(IDS_SETTINGS_TITLE);
    hr = tTargetSettings.Init(crstrSourceFile, m_pICEProjInfo, this);
    if(hr != S_OK)
    {
        return hr;
    }

    tTargetSettings.DoModal();

    return S_OK;
}




//------------------------------------------------------------------*
/**
 * load.
 *
 * @param           crstrFileName: target file name
 * @param           doc: xml document.
 * @return          S_OK: ok
 *                  E_FAIL: error
 * @exception       -
 * @see             Save()
*/
HRESULT CBaseTargetData::Load(const CString& crstrFileName, CXMLDocument& doc)
{
    BOOL ret;

    m_strName = GetFileName(crstrFileName);

    CFileStatus fstatus;
    if (!CFile::GetStatus(GetFileAbsolute(crstrFileName), fstatus))
    {  
        CString errorMsg;
        errorMsg.FormatMessage(IDS_ERR_FILE_NOT_FOUND, GetFileAbsolute(crstrFileName));
        ::AfxMessageBox(errorMsg, MB_OK|MB_ICONERROR);
        return E_FAIL;
    }
    
    CTime ctime = fstatus.m_ctime;
    CTime mtime = fstatus.m_mtime;

    // get create and modified date
    m_strCreateDate.Format(_T("DT#%04d-%02d-%02d-%02d:%02d:%02d"),
                            ctime.GetYear(),
                            ctime.GetMonth(),
                            ctime.GetDay(),
                            ctime.GetHour(),
                            ctime.GetMinute(),
                            ctime.GetSecond());
    m_strModifyDate.Format(_T("DT#%04d-%02d-%02d-%02d:%02d:%02d"),
                            mtime.GetYear(),
                            mtime.GetMonth(),
                            mtime.GetDay(),
                            mtime.GetHour(),
                            mtime.GetMinute(),
                            mtime.GetSecond());
    
    // parse xml
    if (!doc.Load(GetFileAbsolute(crstrFileName)))
    {
        CString errorMsg;
        errorMsg.FormatMessage(IDS_ERR_SYNTAX_ERROR, GetFileAbsolute(crstrFileName));
        ::AfxMessageBox(errorMsg, MB_OK|MB_ICONERROR);   
        return E_FAIL;
    }

    // starting of the xml tree
    CXMLNode rootNode;
    ret = doc.GetRootNode(rootNode);
    ASSERT(ret);
    if (!ret)
    {
        return E_FAIL;
    }

    // check tag 
    CString strTagName;
    rootNode.GetTagName(strTagName);
    if (strTagName.CompareNoCase(m_pAddonPar->m_strKADType)!=0)
    {
        CString errorMsg;
        errorMsg.FormatMessage(IDS_ERR_SYNTAX_ERROR, GetFileAbsolute(crstrFileName));
        ::AfxMessageBox(errorMsg, MB_OK|MB_ICONERROR);  
        return E_FAIL; 
    }

    // USRTEXT, URL attributes
    if (!rootNode.GetAttribute(CE_XMLATTR_USRTEXT1, m_strText1))
    {
        m_strText1.Empty();
    }
    if (!rootNode.GetAttribute(CE_XMLATTR_USRTEXT2, m_strText2))
    {
        m_strText2.Empty();
    }
    if (!rootNode.GetAttribute(CE_XMLATTR_USRTEXT3, m_strText3))
    {
        m_strText3.Empty();
    }
    if (!rootNode.GetAttribute(CE_XMLATTR_URL, m_strUrl))
    {
        m_strUrl.Empty();
    }
    // ALIAS
    if (!rootNode.GetAttribute(CE_XMLATTR_ALIAS, m_strAlias))
    {
        m_strAlias.Empty();
    }

    // RESOURCE attribute
    rootNode.GetAttribute(TARGET_XMLATTR_RESOURCE, m_strResource);

    // CONTROL attribute
    CString strControl;
    rootNode.GetAttribute(TARGET_XMLATTR_CONTROL, strControl);

	// #d-160 01.02.05 SIS >>
	// bug loading connect parameter
	// set active firmware version number first
	SetActFirmware(strControl);

    // load connect parameters
    LoadConnectParameters(rootNode);

    // parse control string and set corresponding parameters
    SetCtrlAddress(strControl);
	// #d-160 01.02.05 SIS <<

    CXMLNodeList childList;
    if (rootNode.GetChildNodeList(childList))
    {
        CXMLNode child;
        childList.Reset();
        while(childList.Next(child))
        {
            CString strTagName;
            child.GetTagName(strTagName);
            if (strTagName.CompareNoCase(_T("CDOWNLOAD"))==0)
            {
                child.GetAttribute(_T("DIR"), m_strCustomDownloadDir);
                break; 
            }
        }
    }

    return S_OK;
}


//------------------------------------------------------------------*
/**
 * load connect parameters.
 *
 * @param           rtRootNode: xml node of target root
 * @return          -
 * @exception       -
 * @see             -
*/
void CBaseTargetData::LoadConnectParameters(CXMLNode& rtRootNode)
{
    // attribute name = communication channel
    CString             strCommChannel;
    CString             strConnectPar;
    CCommChannelData    tCommChannelData;
    CFirmwareData*      pFirmwareData;

    pFirmwareData = GetActFirmwareData();
    ASSERT(pFirmwareData);

    if(pFirmwareData)
    {
        int iNumCommChannels = pFirmwareData->m_tCommChannelArr.GetSize();
        for(int iCommChannel = 0; iCommChannel < iNumCommChannels; ++iCommChannel)
        {
            tCommChannelData = pFirmwareData->m_tCommChannelArr[iCommChannel];
            strCommChannel = tCommChannelData.m_strId;
            if(rtRootNode.GetAttribute(strCommChannel, strConnectPar))
            {
                m_tConnectParArray.Add(CConnectPar(strCommChannel, strConnectPar));
            }
        }
    }

}

//------------------------------------------------------------------*
/**
 * save.
 *
 * @param           crstrFileName: target file name
 * @param           doc: xml document.
 * @return          S_OK: ok
 *                  E_FAIL: error
 * @exception       -
 * @see             Load()
*/
HRESULT CBaseTargetData::Save(const CString& crstrFileName, CXMLDocument& doc)
{
    BOOL ret;
    
    CXMLNode rootNode;

    ret = doc.CreateNode(rootNode, m_pAddonPar->m_strFilePrefix);
    ASSERT(ret);
    if (!ret)
    {
        CString errorMsg;
        errorMsg.FormatMessage(IDS_ERR_SAVE_TARGET, GetFileAbsolute(crstrFileName));
        ::AfxMessageBox(errorMsg, MB_OK|MB_ICONERROR);
        return E_FAIL;
    }

    // USRTEXT, URL attributes
    if (!m_strText1.IsEmpty())
    {
        rootNode.SetAttribute(CE_XMLATTR_USRTEXT1, m_strText1);
    }
    if (!m_strText2.IsEmpty())
    {
        rootNode.SetAttribute(CE_XMLATTR_USRTEXT2, m_strText2);
    }
    if (!m_strText3.IsEmpty())
    {
        rootNode.SetAttribute(CE_XMLATTR_USRTEXT3, m_strText3);
    }
    if (!m_strUrl.IsEmpty())
    {
        rootNode.SetAttribute(CE_XMLATTR_URL, m_strUrl);
    }
    // ALIAS
    if (!m_strAlias.IsEmpty())
    {
        rootNode.SetAttribute(CE_XMLATTR_ALIAS, m_strAlias);
    }

    CXMLNode docNode;
    doc.GetDocNode(docNode);
    docNode.AppendChild(rootNode);

    // CONTROL attribute
    CString strControl = GetCtrlAddress();
    rootNode.SetAttribute(TARGET_XMLATTR_CONTROL, strControl);

    // RESOURCE attribute
    rootNode.SetAttribute(TARGET_XMLATTR_RESOURCE, m_strResource);

    // save connect parameters
    int iNumConnPar = m_tConnectParArray.GetSize();
    CConnectPar tConnectPar;
    for(int iConnPar = 0; iConnPar < iNumConnPar; ++iConnPar)
    {
        rootNode.SetAttribute(m_tConnectParArray[iConnPar].m_strCommChannel, m_tConnectParArray[iConnPar].m_strConnectPar);
    }

    if (m_pAddonPar->m_bCustDL)
    {
        CXMLNode child;
        if (doc.CreateNode(child, _T("CDOWNLOAD")))
        {
            child.SetAttribute(_T("DIR"), m_strCustomDownloadDir);
            rootNode.AppendChild(child);
        }
    }

    ret = doc.Save(GetFileAbsolute(crstrFileName));
    ASSERT(ret);
    if (!ret)
    {
        CString errorMsg;
        errorMsg.FormatMessage(IDS_ERR_SAVE_TARGET, GetFileAbsolute(crstrFileName));
        ::AfxMessageBox(errorMsg, MB_OK|MB_ICONERROR);
        return E_FAIL;
    }

    return S_OK;
}



//------------------------------------------------------------------*
/**
 * set backparse attributes.
 *
 *  base version saves standard attributes in xml node.
 *
 * @param           rtNode: xml target node
 * @return          -
 * @exception       -
 * @see             -
*/
void CBaseTargetData::SetBackparseAttributes(CXMLNode& rtNode)
{
    CString strTmp;

    SetStandardAttributes(rtNode);

    // TEXT attribute
    rtNode.SetAttribute(CE_XMLATTR_TEXT, GetNodeText());
    // ID attribute
    rtNode.SetAttribute(CE_XMLATTR_ID, m_strName);
    // TYPE attribute
    rtNode.SetAttribute(CE_XMLATTR_TYPE, m_pAddonPar->m_strKADType);

    // ADDR attribute
    strTmp = GetCtrlAddress();
    if(!strTmp.IsEmpty())
    {
        strTmp = ADDR_START + strTmp + ADDR_END;
        rtNode.SetAttribute(CE_XMLATTR_ADDR, strTmp);
    }

    // USER ADDR
    strTmp = GetUserCtrlAddress();
    if(!strTmp.IsEmpty())
    {
        strTmp = ADDR_START + strTmp + ADDR_END;
        rtNode.SetAttribute(CE_XMLATTR_USERADDR, strTmp);
    }

    // RESOURCE
    if(!m_strResource.IsEmpty())
    {
        rtNode.SetAttribute(CE_XMLATTR_RESOURCE, m_strResource);
    }

    // DOWNLOADER attribute
    CString strDownloadGuid;
    CFirmwareData*  pFirmwareData = GetActFirmwareData();
    ASSERT(pFirmwareData);
    if(pFirmwareData)
    {
        strDownloadGuid = _T("{") + pFirmwareData->m_strDownloadGuid + _T("}");
    }
    rtNode.SetAttribute(CE_XMLATTR_DOWNLOAD, strDownloadGuid);
}


//------------------------------------------------------------------*
/**
 * set standard attributes.
 *
 * @param           rtNode: xml node
 * @return          -
 * @exception       -
 * @see             -
*/
void CBaseTargetData::SetStandardAttributes(CXMLNode& rtNode)
{
    // USERTEXT
    if (!m_strText1.IsEmpty())
    {
        rtNode.SetAttribute(CE_XMLATTR_USRTEXT1, m_strText1);
    }
    if (!m_strText2.IsEmpty())
    {
        rtNode.SetAttribute(CE_XMLATTR_USRTEXT2, m_strText2);
    }
    if (!m_strText3.IsEmpty())
    {
        rtNode.SetAttribute(CE_XMLATTR_USRTEXT3, m_strText3);
    }
    // URL
    if (!m_strUrl.IsEmpty())
    {
        rtNode.SetAttribute(CE_XMLATTR_URL, m_strUrl);
    }
    // ALIAS
    if (!m_strAlias.IsEmpty())
    {
        rtNode.SetAttribute(CE_XMLATTR_ALIAS, m_strAlias);
    }
    // CREATE/MODIFIED
    if (!m_strCreateDate.IsEmpty())
    {
        rtNode.SetAttribute(CE_XMLATTR_CREATED, m_strCreateDate);
    }
    if (!m_strCreateUser.IsEmpty())
    {
        rtNode.SetAttribute(CE_XMLATTR_USRCREATED, m_strCreateUser);
    }
    if (!m_strModifyDate.IsEmpty())
    {
        rtNode.SetAttribute(CE_XMLATTR_MODIFIED, m_strModifyDate);
    }
    if (!m_strModifyUser.IsEmpty())
    {
        rtNode.SetAttribute(CE_XMLATTR_USRMODIFIED, m_strModifyUser);
    }
}



//------------------------------------------------------------------*
/**
 * get node text.
 *
 *  get node text for backparsing.
 *
 * @param           ptBaseTargetData: base target data object
 * @return          node text.
 * @exception       -
 * @see             SetBackparseAttributes()
*/
CString CBaseTargetData::GetNodeText()
{
    CString strText(m_strName);
    CString strCtrlUserAddr(GetUserCtrlAddress());

    if(!m_strResource.IsEmpty())
    {
        strText += _T(" = ") + m_strResource;
    }
    if(!strCtrlUserAddr.IsEmpty())
    {
        strText += _T(" ");
        strText += ADDR_START + strCtrlUserAddr + ADDR_END;
    }
    return strText;
}



//------------------------------------------------------------------*
/**
 * GetProjectPath returns the path of the open project.
 *
 * @param           
 * @return          project path
 * @exception       -
 * @see             
*/
CString CBaseTargetData::GetProjectPath()
{
    HRESULT hr;

	ASSERT(m_pICEProjInfo);
    if (m_strProjectPath.IsEmpty())
    {
        if (m_pICEProjInfo)
        {  
            CComBSTR sProjPath;
            hr = m_pICEProjInfo->getProjectPath(&sProjPath);
            if (hr == S_OK)
            {
                m_strProjectPath = sProjPath;
            }
        }
    }

    return m_strProjectPath;
}

// NFTASKHANDLING 01.06.05 SIS >>
//------------------------------------------------------------------*
/**
 * GetProjectName returns the name of the open project.
 *
 * @param           
 * @return          project name
 * @exception       -
 * @see             
*/
CString CBaseTargetData::GetProjectName()
{
    HRESULT hr;
	CString	strProjectName;

	ASSERT(m_pICEProjInfo);
    if (m_pICEProjInfo)
    {  
        CComBSTR sProjName;
        hr = m_pICEProjInfo->getProjectName(&sProjName);
        if (hr == S_OK)
        {
            strProjectName = sProjName;
        }
    }

    return strProjectName;
}

//------------------------------------------------------------------*
/**
 * GetProjectPath returns the path of the open project file.
 *
 * @param           
 * @return          full path to project file
 * @exception       -
 * @see             
*/
CString CBaseTargetData::GetProjectFilePath()
{
	ASSERT(m_pICEProjInfo);

	CString strProjectFilePath;
	CString	strProjectPath = GetProjectPath();
	CString	strProjectName = GetProjectName();

	strProjectFilePath.Format(_T("%s\\%s.4cp"), (LPCTSTR)strProjectPath, (LPCTSTR)strProjectName);

	return strProjectFilePath;
}
// NFTASKHANDLING 01.06.05 SIS <<


//------------------------------------------------------------------*
/**
 * get resource.
 *
 * @param           -
 * @return          resource name.
 * @exception       -
 * @see             SetResource()
*/
CString CBaseTargetData::GetResource()
{
    return m_strResource;
}



//------------------------------------------------------------------*
/**
 * set resource.
 *
 * @param           crstrResource: resource name
 * @return          -
 * @exception       -
 * @see             GetResource()
*/
void CBaseTargetData::SetResource(const CString& crstrResource)
{
    m_strResource = crstrResource;
}



//------------------------------------------------------------------*
/**
 * get all resources from project.
 *
 *  get all resource names from project info.
 *
 * @param           rastrAllResources: string array containing all resource names (output)
 * @return          HRESULT
 * @exception       -
 * @see             -
*/
HRESULT CBaseTargetData::GetAllResourcesFromProject(CStringArray& rastrAllResources)
{
    // is project manager interface available?
    if (!m_pICEProjInfo)
    {
        CString errorMsg;
        errorMsg.FormatMessage(IDS_ERROR_NOPROJMAN);
        ::AfxMessageBox(errorMsg, MB_OK|MB_ICONERROR);
        return E_FAIL;
    }

    SAFEARRAY* parrResources;
    CString strClassType = TYPE_RESOURCE;
    CComBSTR sClassType(strClassType);

    // get id paths of all resources
    HRESULT hr = m_pICEProjInfo->getClassesOfTypeAsIdPaths(sClassType, &parrResources);
    if(hr!=S_OK)
    {
        CString errorMsg;
        errorMsg.FormatMessage(IDS_ERROR_FAIL_INITRESOURCE);
        ::AfxMessageBox(errorMsg, MB_OK|MB_ICONERROR);
        return E_FAIL;
    }   

    BSTR HUGEP *pbstr;
    BSTR bstr;
    unsigned long i;

    // Get a pointer to the elements of the array.
    hr = ::SafeArrayAccessData(parrResources, (void HUGEP**)&pbstr);
    if (FAILED(hr))
    {
        CString errorMsg;
        errorMsg.FormatMessage(IDS_ERROR_FAIL_INITRESOURCE);
        ::AfxMessageBox(errorMsg, MB_OK|MB_ICONERROR);
        return E_FAIL;    
    }
    for (i = 0; i < ((parrResources)->rgsabound[0]).cElements; i++)
    {
        // returned have been id paths: 
        // the last part of the id paths is the object name
        CString str;
        bstr = pbstr[i];
        str = (CString)bstr;
        int i = str.ReverseFind(_T('/'));
        ASSERT(i>0);
        str = str.Right(str.GetLength()-i-1);
        rastrAllResources.Add(str);
    }

    ::SafeArrayUnaccessData(parrResources);
    ::SafeArrayDestroy(parrResources);

    return S_OK;
}




//------------------------------------------------------------------*
/**
 * get control address.
 *
 *  uses CBaseAddonParameter::BuildControlString()
 *
 * @param           -
 * @return          address string.
 * @exception       -
 * @see             CBaseAddonParameter::BuildControlString(), SetCtrlAddress()
*/
CString CBaseTargetData::GetCtrlAddress()
{
    CString strControl;
    CString strConnectPar;

    strConnectPar = GetConnectParameter(m_strCommChannel);
    strControl = m_pAddonPar->BuildControlString(m_strOPCServer, m_strCommChannel, strConnectPar, m_uiActFirmware);

    return strControl;
}



//------------------------------------------------------------------*
/**
 * set control address.
 *
 *  uses CBaseAddonParameter::ParseControlString().
 *
 * @param           crstrCtrlAddress: control address.
 * @return          -
 * @exception       -
 * @see             CBaseAddonParameter::ParseControlString(), GetCtrlAddress()
*/
void CBaseTargetData::SetCtrlAddress(const CString& crstrCtrlAddress)
{
    CString strConnectPar;

    m_pAddonPar->ParseControlString(crstrCtrlAddress, m_strOPCServer, m_strCommChannel, strConnectPar, m_uiActFirmware);

    if(m_strCommChannel.IsEmpty())
    {
        m_strCommChannel = m_pAddonPar->GetStandardCommChannel(m_uiActFirmware);
    }
    if(strConnectPar.IsEmpty())
    {
        strConnectPar = m_pAddonPar->GetStandardConnectParameter(m_strCommChannel);
    }

    SetConnectParameter(m_strCommChannel, strConnectPar);
}

// #d-160 01.02.05 SIS >>
//------------------------------------------------------------------*
/**
 * set active firmware version number.
 *
 *  uses CBaseAddonParameter::ParseControlString().
 *
 * @param           crstrCtrlAddress: control address.
 * @return          -
 * @exception       -
 * @see             CBaseAddonParameter::ParseControlString()
*/
void CBaseTargetData::SetActFirmware(const CString& crstrCtrlAddress)
{
    CString strConnectPar;
	CString	strOPCServer;
	CString	strCommChannel;

    m_pAddonPar->ParseControlString(crstrCtrlAddress, strOPCServer, strCommChannel, strConnectPar, m_uiActFirmware);
}
// #d-160 01.02.05 SIS <<



//------------------------------------------------------------------*
/**
 * get connect parameter.
 *
 * @param           crstrCommChannel: communication channel
 * @return          connect parameter or standard connect parameter if not found
 * @exception       -
 * @see             GetStandardConnectParameter()
*/
CString CBaseTargetData::GetConnectParameter(const CString& crstrCommChannel)
{
    int iNumConnPar = m_tConnectParArray.GetSize();
    for(int iConnPar = 0; iConnPar < iNumConnPar; ++iConnPar)
    {
        if(m_tConnectParArray[iConnPar].m_strCommChannel.CompareNoCase(crstrCommChannel) == 0)
        {
            return m_tConnectParArray[iConnPar].m_strConnectPar;
        }
    }
    CFirmwareData*  pFirmwareData = GetActFirmwareData();
    if(pFirmwareData)
    {
        CCommChannelData    tCommChannelData;
        if(pFirmwareData->GetCommChannelDataFromId(crstrCommChannel, tCommChannelData))
        {
            return tCommChannelData.m_strStdConnPar;
        }
    }
    return _T("");
}


//------------------------------------------------------------------*
/**
 * set connect parameter.
 *
 * @param           crstrCommChannel: communication channel
 * @param           crstrConnectPar: connect parameter
 * @return          -
 * @exception       -
 * @see             -
*/
void CBaseTargetData::SetConnectParameter(const CString& crstrCommChannel, const CString& crstrConnectPar)
{
    int iNumConnPar = m_tConnectParArray.GetSize();
    for(int iConnPar = 0; iConnPar < iNumConnPar; ++iConnPar)
    {
        if(m_tConnectParArray[iConnPar].m_strCommChannel.CompareNoCase(crstrCommChannel) == 0)
        {
            m_tConnectParArray[iConnPar].m_strConnectPar = crstrConnectPar;
            return;
        }
    }
    m_tConnectParArray.Add(CConnectPar(crstrCommChannel, crstrConnectPar));
}




//------------------------------------------------------------------*
/**
 * get control address in readable from.
 *
 * @param           -
 * @return          user control address.
 * @exception       -
 * @see             -
*/
CString CBaseTargetData::GetUserCtrlAddress()
{
    return m_strOPCServer;
}



//------------------------------------------------------------------*
/**
 * add to project.
 *
 * @param           crstrTargetFile: target file to be added to project
 * @param           crstrParentFile: parent file name
 * @param           crstrIdPath: id path where to add
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CBaseTargetData::AddToProject(const CString& crstrTargetFile, const CString& crstrParentFile, const CString& crstrIdPath)
{
    // add file to container
    HRESULT hr;
    CComBSTR sProjFile(crstrParentFile);
    CComPtr<ICEContainer> pCont;
    hr = m_pICEProjInfo->getContainerIFforFile(sProjFile, &pCont);
    if (hr != S_OK)
    {
        CString errorMsg;
        errorMsg.FormatMessage(IDS_ERROR_FAIL_GETCONT, crstrParentFile);
        ::AfxMessageBox(errorMsg, MB_OK|MB_ICONERROR);
        return FALSE; // TODO error handling
    }

    CComBSTR sLinkName(crstrTargetFile);
    CComBSTR sId(crstrIdPath);
    hr = pCont->createLink(sProjFile, sId, sLinkName);
    if (hr != S_OK)
    {
        CString errorMsg;
        errorMsg.FormatMessage(IDS_ERROR_FAIL_CREATEINCONT);
        ::AfxMessageBox(errorMsg, MB_OK|MB_ICONERROR);
        return FALSE; // TODO error handling
    }

    m_pICEProjInfo->fileChangedHint(sProjFile);

    return TRUE;
}



//------------------------------------------------------------------*
/**
 * CreateParentLink creates the link in the source file of the parent node
 *
 * @param           crstrParentFile: the file name of the parent node
 * @param           crstrParentId: the id of the parent node
 * @param           crstrLinkName: the name of the link to generate
 * @return          
 * @exception       -
 * @see             
*/
HRESULT CBaseTargetData::CreateParentLink(const CString& crstrParentFile,
                                          const CString& crstrParentId,
                                          const CString& crstrLinkName)
{
    HRESULT hr;
    CComBSTR sParentFile(crstrParentFile);
    CComBSTR sParentId(crstrParentId);

    CComPtr<ICEContainer> pContainer;

    hr = m_pICEProjInfo->getContainerIFforFile(sParentFile, &pContainer);
    if (hr!=S_OK)
    {
        CString errorMsg;
        errorMsg.FormatMessage(IDS_ERR_GET_DESC);
        ::AfxMessageBox(errorMsg, MB_OK|MB_ICONERROR);
        return E_FAIL;
    }


    CComBSTR sLinkName(crstrLinkName);
    hr = pContainer->createLink(sParentFile, sParentId, sLinkName);
    if (hr!=S_OK)
    {
        CString errorMsg;
        errorMsg.FormatMessage(IDS_ERR_CREATE_LINK);
        ::AfxMessageBox(errorMsg, MB_OK|MB_ICONERROR);
        return E_FAIL;
    }

    return S_OK;
}



//------------------------------------------------------------------*
/**
 * DeleteParentLink deletes a link in the source file of the parent node.
 *
 * @param           crstrParentFile: the file name of the parent node
 * @param           crstrParentId: the id of the parent node
 * @param           crstrLinkName: the name of the link to generate
 * @return          
 * @exception       -
 * @see             
*/
HRESULT CBaseTargetData::DeleteParentLink(const CString& crstrParentFile,
                                          const CString& crstrParentId,
                                          const CString& crstrLinkName)
{
    HRESULT hr;
    CComBSTR sParentFile(crstrParentFile);
    CComBSTR sParentId(crstrParentId);

    CComPtr<ICEContainer> pContainer;

    hr = m_pICEProjInfo->getContainerIFforFile(sParentFile, &pContainer);
    if (hr!=S_OK)
    {
        CString errorMsg;
        errorMsg.FormatMessage(IDS_ERR_GET_DESC);
        ::AfxMessageBox(errorMsg, MB_OK|MB_ICONERROR);
        return E_FAIL;
    }


    CComBSTR sLinkName(crstrLinkName);
    hr = pContainer->deleteLink(sParentFile, sParentId, sLinkName);
    if (hr!=S_OK)
    {
        CString errorMsg;
        errorMsg.FormatMessage(IDS_ERR_DELETE_LINK, crstrParentFile);
        ::AfxMessageBox(errorMsg, MB_OK|MB_ICONERROR);
        return E_FAIL;
    }

    return S_OK;
}



//------------------------------------------------------------------*
/**
 * GetFileAbsolute returns the file with absolute path. Macros will be
 * expanded, project path added if necessary.
 *
 * @param           crstrFileName: file name
 * @return          file name with absolute path
 * @exception       -
 * @see             
*/
CString CBaseTargetData::GetFileAbsolute(const CString& crstrFileName)
{
    HRESULT hr;
    CString strRet;
    CComBSTR sFileName(crstrFileName);
    CComBSTR sAbsFileName;
    
    hr = m_pICEProjInfo->GetFileAbsolute(sFileName, &sAbsFileName);
    if (hr == S_OK)
    {
        strRet = sAbsFileName;
    }
    else
    {
        strRet = crstrFileName;
    }

    return strRet;
}



//------------------------------------------------------------------*
/**
 * GetFileRelative returns the file with relative path. The project path
 * will be stripped if possible.
 *
 * @param           crstrFileName: file name with absolute path
 * @return          relative file name
 * @exception       -
 * @see             
*/
CString CBaseTargetData::GetFileRelative(const CString& crstrFileName)
{
    HRESULT hr;
    CString strRet;
    CComBSTR sFileName(crstrFileName);
    CComBSTR sRelFileName;
    
    hr = m_pICEProjInfo->GetFileRelative(sFileName, &sRelFileName);
    if (hr == S_OK)
    {
        strRet = sRelFileName;
    }
    else
    {
        strRet = crstrFileName;
    }

    return strRet;
}



//------------------------------------------------------------------*
/**
 * GetFileName returns the name of a file. Path and file extension will
 * be stripped.
 *
 * @param           crstrFileName: file name with path
 * @return          name of the file
 * @exception       -
 * @see             
*/
CString CBaseTargetData::GetFileName(const CString& crstrFileName)
{
    CString strRet = crstrFileName;
    int i = strRet.ReverseFind(_T('.'));
    if (i>0)
    {
        strRet = strRet.Left(i);
    }
    
    i = strRet.ReverseFind(_T('\\'));
    if (i>0 && strRet.GetLength()>i+1)
    {
        strRet = strRet.Mid(i+1);
    }

    return strRet;
}



//------------------------------------------------------------------*
/**
 * check new name.
 *
 *   two checks: 
 *      1. does a file with the same name does already exist
 *      2. does a node with the same id already exist
 *
 * @param           crstrNewFileName: new file name.
 * @param           crstrParentId: parent id
 * @return          is new name ok?
 * @exception       -
 * @see             -
*/
BOOL CBaseTargetData::CheckNewName(const CString& crstrNewFileName, const CString& crstrParentId)
{
    HRESULT hr;

    CString strFileName = crstrNewFileName + _T(".") + m_pAddonPar->m_strFileExtension;
    CString strFileAbsName = GetFileAbsolute(strFileName);

    // 1.
    CFileStatus status;
    if (CFile::GetStatus(strFileAbsName, status))
    {
        CString errorMsg;
        errorMsg.FormatMessage(IDS_ERROR_FILE_EXISTS, strFileAbsName);
        ::AfxMessageBox(errorMsg, MB_OK|MB_ICONERROR);
        return FALSE;
    }

    // 2.
    IUnknown* pUnk = NULL;
    CComBSTR sId(crstrParentId);

    hr = m_pICEProjInfo->getXMLNodePtr(sId, &pUnk);

    if (hr!=S_OK)
    {
        CString errorMsg;
        errorMsg.FormatMessage(IDS_ERR_GET_DESC);
        ::AfxMessageBox(errorMsg, MB_OK|MB_ICONERROR);
        return FALSE;
    }

    CXMLNode nodeParent;
    if (!nodeParent.SetIXMLNode(pUnk))
    {
        CString errorMsg;
        errorMsg.FormatMessage(IDS_ERR_GET_DESC);
        ::AfxMessageBox(errorMsg, MB_OK|MB_ICONERROR);
        return FALSE;
    }

    CXMLNode sameIdNode;

    if (nodeParent.GetChildWithAttr(CE_XMLATTR_ID, crstrNewFileName, sameIdNode))
    {
        CString errorMsg;
        errorMsg.FormatMessage(IDS_ERR_ID_EXISTS, crstrNewFileName);
        ::AfxMessageBox(errorMsg, MB_OK|MB_ICONERROR);
        return FALSE;
    }

    return TRUE;
}



//------------------------------------------------------------------*
/**
 * get currently selected firmware data.
 *
 * @param           -
 * @return          currently selected firmware data
 * @exception       -
 * @see             -
*/
CFirmwareData* CBaseTargetData::GetActFirmwareData()
{
    return m_pAddonPar->GetFirmwareData(m_uiActFirmware);
}


//------------------------------------------------------------------*
/**
 * get addon parameter.
 *
 * @param           -
 * @return          pointer to addon parameter object
 * @exception       -
 * @see             -
*/
CBaseAddonParameter* CBaseTargetData::GetAddonParameter()
{
    return m_pAddonPar;
}


//------------------------------------------------------------------*
/**
 * ensure target disconnected.
 *
 * @param           crstrTargetId: target id (name)
 * @param           bRebuildHint: give rebuild hint?
 * @return          target is disconnected?
 * @exception       -
 * @see             -
*/
BOOL CBaseTargetData::EnsureTargetDisconnected(const CString& crstrTargetId, BOOL bRebuildHint)
{
    HRESULT     hr;
    CComBSTR    sTargetId(crstrTargetId);
    hr = m_pTargetObject->Fire_OnEnsureTargetDisconnected(sTargetId, bRebuildHint);
    if(hr == S_FALSE)
    {
        return FALSE;
    }
    return TRUE;
}








LPCTSTR GetSystemErrorMsg(HRESULT hError);


static int EnumFileCallBack(
    void*         pCtx, 
    void*         pCtx2, 
    int           nIsDir,  // 0 pszFile is a file name
                           //+1 pszFile is a dir  name
                           //-1 pszFile is ".." to indicate cd ..
    int           bOnError,
    const TCHAR*  pszFile
);

struct SCBEnumFiles
{
    CBaseTargetData* pTarget;
    FILE*           pFile;
    LPCTSTR         pszPath;
    LPCTSTR         pszTargetName;
    BOOL            bError;
};

//------------------------------------------------------------------*
/**
 * operation custom Download
 *
 * @param           crstrSourceFile: source file name
 * @param           crstrSourceId: id of parent
 * @param           crstrAddInfo: additional info
 * @return          S_OK: ok
 * @exception       -
 * @see             -
*/
HRESULT CBaseTargetData::OpCustomDownload(const CString& crstrSourceFile, 
                                          const CString& crstrSourceId, 
                                          const CString& crstrAddInfo)
{
    HRESULT         hr = S_OK;
    CXMLDocument    tXMLDocument;

    hr = Load(crstrSourceFile, tXMLDocument);
    if(hr != S_OK)
    {
        return hr;
    }

    if (!m_pAddonPar->m_bCustDL)
    {
        return S_OK;
    }

    CComBSTR    sProjectPath;
    hr = m_pICEProjInfo->getProjectPath(&sProjectPath);
    if (hr != S_OK)
    {
        CString strMessage;
        strMessage.LoadString(IDS_INTERNAL_ERROR);
        strMessage += _T(": OnPreDLStep1");
        ::AfxMessageBox(strMessage, MB_OK|MB_ICONERROR);
        return hr;
    }
	
	SetCurrentDirectory((CString)sProjectPath);

    CString strDownloadPath = m_strCustomDownloadDir;
    if ( ! strDownloadPath.IsEmpty())
    {
        // write domain "__custdl.bin"
        CString fileName;

        fileName.Format("%s\\gen\\%s\\%s", (CString)sProjectPath, m_strName, FILE_NAME_CUSTOM_BIN);
        FILE* pFile = fopen(fileName, "wb");
        if(pFile == NULL)
        {
            return E_FAIL;
        }

        // printf msg
        CString sMsg;
        sMsg.FormatMessage(IDS_INFORM_GENRATING_DD, fileName);
        OutputDownloadMessage(S_OK, m_strName, sMsg);

        SCBEnumFiles CBEnumF = {this, pFile, strDownloadPath, m_strName, FALSE};
        if ( ! FC_FileEnumFiles(
                    strDownloadPath, 
                    FC_FILE_ENUM_SUBDIRS,   // ##d-1439 removed bit
                    EnumFileCallBack, 
                    &CBEnumF,
                    NULL))
        {
	        fclose(pFile);
            return E_FAIL;
        }

        // close file.
        //##d-1569 deleted write of obsolete termination byte (0)  
        fclose(pFile);

        if (CBEnumF.bError)
        {
            return E_FAIL;
        }
    }
    else
    {
        CString fileName;
        fileName.Format("%s\\gen\\%s\\%s", (CString)sProjectPath, m_strName, FILE_NAME_CUSTOM_BIN);
        FILE* pFile = fopen(fileName, "wb");
        if(pFile)
        {
            char    cTmp = 0;
            fwrite(&cTmp, 1, 1, pFile);
            fclose(pFile);
        }
        else
        {
            hr = E_FAIL;
        }

        //CString sMsg;
        //sMsg.LoadString(IDS_WARNING_NOSPECIFIED_CDD);
        //OutputDownloadMessage(S_OK, m_strName, sMsg);
    }

    
    return hr;
}



//------------------------------------------------------------------*
/**
 * IsNCCBreakpointSupport
 *
 * @param           -
 * @return          ncc with breakpoint support?
 * @exception       -
 * @see             -
*/
BOOL CBaseTargetData::IsNCCBreakpointSupport()
{
    BOOL bBreakPointSupport = FALSE;
    ASSERT(m_pAddonPar);
    if(m_pAddonPar)
    {
        bBreakPointSupport = m_pAddonPar->m_bNccBPSupport;
    }
    return bBreakPointSupport;
}



int CBaseTargetData::CheckFileNameConventions(const CString& crstrFileName)
{
    CString strFirst;
    CString strRest;

    strRest = crstrFileName;
    while (!strRest.IsEmpty())
    {
        int i;
        i = strRest.FindOneOf(_T("\\/"));
        if (i<0)
        {
            strFirst = strRest;
            strRest = _T("");
        }
        else
        {
            strFirst = strRest.Left(i);
            if (strRest.GetLength()>i+1)
            {
                strRest = strRest.Mid(i+1);
            }
            else
            {
                strRest = _T("");
            }
        }
        int ip;
        ip = strFirst.Find(_T("."));
        CString strFileName = strFirst;
        CString strExt;
        if (ip>0)
        {
            strFileName = strFirst.Left(ip);
            if (strFirst.GetLength()>ip+1)
            {
                strExt = strFirst.Mid(ip+1);
            }
        }

        if (strFileName.GetLength()>8 || strExt.GetLength()>3)
        {
            CString str;
            str.FormatMessage(IDS_ERROR_8_3_FILENAME, crstrFileName);
            OutputDownloadMessage(E_FAIL, m_strName, str);
            return -1;
        }
       
        
    }

    return 0;

}



void CBaseTargetData::OutputDownloadMessage(HRESULT hError,
                                            const CString& crstrTarget,
                                            const CString& sMsg)
{
    CComBSTR    sTarget(crstrTarget);
    m_pTargetObject->Fire_OnNewMessage(sTarget, E_FACILITY_CEDOWNLOAD, 
                                      hError, (CComBSTR) sMsg);
}


static DWORD translate32(DWORD dw)
{
    DWORD res;

    res =  (dw & 0xff000000) >> 24;
    res |= (dw & 0x00ff0000) >> 8;
    res |= (dw & 0x0000ff00) << 8;
    res |= (dw & 0x000000ff) << 24;

    return res;
}

static short translate16(short val)
{
  short result;
  BYTE *pResult = (BYTE*)&result;
  BYTE *pVal = (BYTE*)&val;
  pResult[0] = pVal[1];
  pResult[1] = pVal[0];
  return result;
}

static int EnumFileCallBack(
    void*         pCtx, 
    void*         pCtx2, 
    int           nIsDir,  // 0 pszFile is a file name
                           //+1 pszFile is a dir  name
                           //-1 pszFile is ".." to indicate cd ..
    int           bOnError,
    const TCHAR*  pszFile
)
{
    CString         sMsg;
    char*           pMem = NULL;
    unsigned long   lFSize = 0xffffffff;
    unsigned long   lFileSize = 0xffffffff;
    CString         strFileName(pszFile);

    SCBEnumFiles* p = (SCBEnumFiles*) pCtx;

    if (bOnError)
    {
        DWORD dwErr = GetLastError();
        HRESULT hr = HRESULT_FROM_WIN32(dwErr);
        if (dwErr == ERROR_PATH_NOT_FOUND)
        {
            sMsg.FormatMessage(IDS_ERROR_NOEXST_DLDIR, strFileName);
            p->pTarget->OutputDownloadMessage(hr, p->pszTargetName, sMsg);
            p->bError = TRUE;
            return -1;
        }
        
        CString sHr;
        sHr.Format(_T("0x%08lx"), hr);
        LPCTSTR pszSysMsg = GetSystemErrorMsg(hr);

        sMsg.FormatMessage(IDS_ERROR_FAILED_CREATE_CDD, (LPCTSTR)sHr, pszSysMsg);
        p->pTarget->OutputDownloadMessage(hr, p->pszTargetName, sMsg);
        p->bError = TRUE;
        return -1;
    }

    CString jot(p->pszPath);
    jot += _T("\\");
    jot += strFileName;

    if (p->pTarget->CheckFileNameConventions(strFileName)!=0)
    {
        p->bError = TRUE;
        return -1;
    }

    if (nIsDir == 0)
    {
        int res = FC_LoadDiskBinFileToMem(jot, (void**) &pMem, &lFSize, NULL);
        if (res == -1)
        {
            // TODO: error Box
            //p->pgctx->m_dm.msg0(CG_E_OUTOFMEM, NULL);
            p->bError = TRUE;
            return -1;
        }
        else if (res != 0)
        { 
            // TODO: error Box
            //CGT_IERR(&p->pgctx->m_dm, _T("invalid file operation"));
            p->bError = TRUE;
            return -1;
        }
    }
    else if (nIsDir == 1)   // directory
    {
        //strFileName += _T("\\");     // append backslash to file name
        // no longer needed: terminating \ for directories, file size has to be -1 now
        lFSize = -1;                     // set size to 0
    }
    else  // .. directory, do nothing
    {
        return 1;
    }

    
    // LITTLE / BIG ENDIAN
    lFileSize = lFSize;
    if (p->pTarget->GetAddonParameter()->m_bBigEndian)
    {
        lFSize = translate32(lFSize);   // transform to big endean
    }


    if (strFileName.GetLength()>100)
    {
        HRESULT hr = E_FAIL;
        sMsg.FormatMessage(IDS_ERR_FILENAME_TOO_LONG, 100, strFileName);
        p->pTarget->OutputDownloadMessage(hr, p->pszTargetName, sMsg);
        p->bError = TRUE;
        return -1;
    }

	if (p->pTarget->GetActFirmwareData()->m_uiVersion < 21000)
	{
		XFile_208 fileInfo;
		fileInfo.ulLen = lFSize;
		strncpy((char*)(fileInfo.szName), strFileName, 100);
		//fwrite(strFileName, sizeof(char), strFileName.GetLength() + 1, p->pFile);
		//fwrite(&lFSize, sizeof(char), sizeof(long), p->pFile);
		fwrite(&fileInfo, sizeof(XFile_208), 1, p->pFile);
	}
	else
	{
		XFileDef xFileDef;

		xFileDef.ulDataLen	= lFSize;
		xFileDef.uNameLen	= p->pTarget->GetAddonParameter()->m_bBigEndian ? translate16(strlen(strFileName)) : strlen(strFileName);
		xFileDef.usCRC		= 0;
		xFileDef.usHash		= 0;

		fwrite(&xFileDef, sizeof(XFileDef), 1, p->pFile);
		fwrite(strFileName, sizeof(char), strlen(strFileName), p->pFile);
	}

	if (nIsDir == 0)
	{
		if(pMem)
		{
			fwrite(pMem, sizeof(char), lFileSize, p->pFile);
			FC_LibFreeMemory(pMem);
		}
	}


    return 1;
}


LPCTSTR GetSystemErrorMsg(HRESULT hError)
{
    static CString strMsg;
    LPTSTR  pBuf = NULL;    
    DWORD   ret;


    ret = FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |	
			FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			hError,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT),   // wlangID,
			(LPTSTR) &pBuf,
			0,
			NULL);
    
    if (ret == 0)
    {
        // have no string resource for error, complain as internal error!!
        pBuf = (LPTSTR) ::LocalLock(::LocalAlloc(NONZEROLPTR, MAX_PATH));
        wsprintf(pBuf, _T(" internal error: no resource for error code 0x%08X\n"), hError);
    }

    strMsg += pBuf;
    ::LocalFree(pBuf);

    return strMsg;
}

/* ---------------------------------------------------------------------------- */
