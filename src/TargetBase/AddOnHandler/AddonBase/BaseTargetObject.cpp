
/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"

#include "BaseTargetObject.h"
#include "BaseTargetData.h"
#include "basedefs.h"
#include "CeSysDef.h"

#include "CEProjInfo_i.c"
#include "CEAddonAction_i.c"
#include "CEBackparse_i.c"
#include "CEContainer_i.c"
#include "CEMsg_i.c"
#include "CEAddonActionEvents_i.c"
#include "cecompmanev_i.c"
#include "ExeDelegator_i.c"
#include "utilif.h"
#include "fc_tools\fc_fileIO.h"

#include "DlgProjectInfoUpload.h"

#include "Toolbox.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------*
/**
 * constructor.
 *
 * @param           crstrKADFileName: file where the addon parameter are described (based to engineering\bin directory)
 * @return          -
 * @exception       -
 * @see             -
*/
CBaseTargetObject::CBaseTargetObject(const CString& crstrKADFileName)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    m_strKADFileName = crstrKADFileName;

}

CBaseTargetObject::~CBaseTargetObject()
{

}

//------------------------------------------------------------------*
/**
 * create target data.
 *
 *  overwrite this function to create your own target data object
 *  derived from CBaseTargetData.
 *
 * @param           pICEProjInfo: project info interface pointer
 * @return          pointer to new CBaseTargetData object.
 * @exception       memory exception
 * @see             -
*/
CBaseTargetData* CBaseTargetObject::CreateTargetData(ICEProjInfo* pICEProjInfo)
{
    CBaseTargetData*    ptTargetData = new CBaseTargetData( this, 
                                                            pICEProjInfo,
                                                            m_strKADFileName);
    ptTargetData->Init();
    return ptTargetData;
}


// ICEAddonAction
//------------------------------------------------------------------*
/**
 * doAction.
 *
 *  perform engineering action.
 *
 * @param           sMethodName: name of method to be performed
 * @param           sSourceFile: source file name
 * @param           sId: id path of target
 * @param           pICEProjInfo: project info interface pointer
 * @param           sAdditionalInfo: additional info string
 * @return          S_OK: ok
 *                  E_FAIL: error occurred
 *                  ...
 * @exception       -
 * @see             -
*/
STDMETHODIMP CBaseTargetObject::doAction(BSTR sMethodName, 
                                         BSTR sSourceFile, 
                                         BSTR sId, 
                                         ICEProjInfo * pICEProjectInfo, 
                                         BSTR sAdditionalInfo)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    HRESULT             hr;
    CBaseTargetData*    ptTargetData = NULL;

    try
    {
        CXMLDocument    tXMLDocument;

        ptTargetData = CreateTargetData( pICEProjectInfo);

        CString strMethodName(sMethodName);
        CString strSourceFile(sSourceFile);
        CString strSourceId(sId);
        CString strAddInfo(sAdditionalInfo);

        // call outer handler
        BOOL bHandled;
        hr = ActionDispatcher(ptTargetData, 
                              strMethodName, 
                              strSourceFile, 
                              strSourceId, 
                              strAddInfo,
                              bHandled);

        if (bHandled)
        {           
            delete ptTargetData;
            return hr;
        }

        if (strMethodName.Compare(DISP_NEW)==0)
        {
            hr = ptTargetData->OpNew(strSourceFile, strSourceId, strAddInfo);
            delete ptTargetData;
            return hr;
        }
        else if (strMethodName.Compare(DISP_ASSIGNMENT)==0)
        {
            hr = ptTargetData->OpAssignment(strSourceFile, strSourceId, strAddInfo);
            delete ptTargetData;
            return hr;
        }
        else if (strMethodName.Compare(DISP_RENAME)==0)
        {
            hr = ptTargetData->OpRename(strSourceFile, strSourceId, strAddInfo);
            delete ptTargetData;
            return hr;
        }
        else if (strMethodName.Compare(DISP_DELETE)==0)
        {
            hr = ptTargetData->OpDelete(strSourceFile, strSourceId, strAddInfo);
            delete ptTargetData;
            return hr;
        }
        else if (strMethodName.Compare(DISP_PROPERTIES)==0)
        {
            hr = ptTargetData->OpProperties(strSourceFile, strSourceId, strAddInfo);
            delete ptTargetData;
            return hr;
        }
        else if (strMethodName.Compare(DISP_SETTINGS)==0)
        {
            hr = ptTargetData->OpSettings(strSourceFile, strSourceId, strAddInfo);
            delete ptTargetData;
            return hr;
        }
        else if (strMethodName.Compare(DISP_PROJINFO_UPLOAD)==0)
        {
            hr = DoProjectInfoUpload(strSourceId);
            delete ptTargetData;
            return hr;
        }
        else if (strMethodName.Compare(DISP_PRE_DLSTEP)==0)
        {
            hr = ptTargetData->OpCustomDownload(strSourceFile, strSourceId, strAddInfo);
            delete ptTargetData;
            return hr;
        }
    }
    catch(CFileException* e)
    {
        HRESULT hr;
        hr = HRESULT_FROM_WIN32(e->m_lOsError);
        e->Delete();
        delete ptTargetData;
        return hr;
    }
    catch(CArchiveException* e)
    {
        HRESULT hr;
        hr = E_FAIL;
        e->Delete();
        delete ptTargetData;
        return hr;
    }
    catch(CMemoryException* e)
    {
        e->Delete();
        return E_OUTOFMEMORY;
    }

	return E_FAIL;
}

// ICEBackparse
//------------------------------------------------------------------*
/**
 * doBackparse.
 *
 *  generates backparse output for engineering
 *
 * @param           sFileName: source file name
 * @param           pICEProjInfo: project info interface pointer
 * @param           psXMLText: backparse output as xml text
 * @return          HRESULT
 * @exception       -
 * @see             -
*/
STDMETHODIMP CBaseTargetObject::doBackparse(BSTR sFileName, ICEProjInfo* pICEProjInfo, BSTR* psXmlText)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    HRESULT             hr;
    CString             strXmlText;
    CBaseTargetData*    ptTargetData = NULL;
   
    try
    {
        CXMLDocument    tXMLDocument;

        ptTargetData = CreateTargetData( pICEProjInfo);

        CString strFileName(sFileName);
        
        hr = ptTargetData->Load(strFileName, tXMLDocument);
        if(hr != S_OK)
        {
            delete ptTargetData;
            return hr;
        }

        hr = CreateBackparseOutput(ptTargetData, strFileName, strXmlText);
        if (hr==S_OK)
        {
            *psXmlText = strXmlText.AllocSysString();
        }

        delete ptTargetData;

        return hr;
    }
    catch(CFileException* e)
    {
        HRESULT hr;
        hr = HRESULT_FROM_WIN32(e->m_lOsError);
        e->Delete();
        return hr;
    }
    catch(CArchiveException* e)
    {
        HRESULT hr;
        hr = E_FAIL;
        e->Delete();
        return hr;
    }
    catch(CMemoryException* e)
    {
        e->Delete();
        return E_OUTOFMEMORY;
    }

    return E_FAIL;
}


//------------------------------------------------------------------*
/**
 * execute project wizard.
 *
 * @param           sPath: project path
 * @param           sProjectName: name of new project
 * @param           psFileName: not used
 * @return          HRESULT
 * @exception       -
 * @see             -
*/
STDMETHODIMP CBaseTargetObject::ExecuteProjectWizard(BSTR sPath, BSTR sProjectName, BSTR *psFileName)
{
    AFX_MANAGE_STATE (AfxGetStaticModuleState());

    CString strPath(sPath);
    CString strProjectName(sProjectName);

    HRESULT hr = ExecuteProjectWizard(strPath, strProjectName);

    return hr;
}


//------------------------------------------------------------------*
/**
 * get project wizard description.
 *
 * @param           iLanguageId: language id
 * @param           psName: project wizard name
 * @param           psDescription: project wizard description
 * @return          HRESULT
 * @exception       -
 * @see             -
*/
STDMETHODIMP CBaseTargetObject::GetDescription(long iLanguageId, BSTR *psName, BSTR *psDescription)
{
    AFX_MANAGE_STATE (AfxGetStaticModuleState());

    HRESULT hr;
    CString strName;
    CString strDescription;

    hr = GetProjectWizardDescription(iLanguageId, strName, strDescription);
    if(hr != S_OK)
    {
        return hr;
    }

    *psName = strName.AllocSysString();
    *psDescription = strDescription.AllocSysString();

    return hr;
}


//------------------------------------------------------------------*
/**
 * execute project wizard.
 *
 *  overwrite this function, if special behaviour needed.
 *
 * @param           crstrPath: path where to create project
 * @param           crstrName: name of project wizard
 * @return          HRESULT
 * @exception       -
 * @see             -
*/
HRESULT CBaseTargetObject::ExecuteProjectWizard(const CString& crstrPath, const CString& crstrName)
{
    HRESULT                     hr;
    CBaseProjectData*           pProjectData = NULL;
    CString                     strTargetBaseName;

    try
    {
        pProjectData = CreateProjectData();
    }
    catch(CMemoryException* pE)
    {
        delete pE;
        return E_FAIL;
    }

    hr = pProjectData->Init(crstrPath, crstrName, m_strKADFileName);
    if(FAILED (hr))
    {
        delete pProjectData;
        return (hr);
    }

    hr = ShowProjectWizardDialog(pProjectData);

    delete pProjectData;
    return (hr);
}


//------------------------------------------------------------------*
/**
 * create project data for wizard.
 *
 * @param           -
 * @return          project data object
 * @exception       -
 * @see             -
*/
CBaseProjectData* CBaseTargetObject::CreateProjectData()
{
    return new CBaseProjectData;
}


//------------------------------------------------------------------*
/**
 * show project wizard dialog.
 *
 *  this function must be overwritten in target object.
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
HRESULT CBaseTargetObject::ShowProjectWizardDialog(CBaseProjectData* pProjectData)
{
    return E_NOTIMPL;
}

//------------------------------------------------------------------*
/**
 * get project wizard description.
 *
 * @param           iLanguageId: language id for description
 * @param           crstrName: name
 * @return          HRESULT
 * @exception       -
 * @see             -
*/
HRESULT CBaseTargetObject::GetProjectWizardDescription(int iLanguageId, CString& rstrName, CString& rstrDescription)
{
    CBaseAddonParameter tAddonPar;

    tAddonPar.LoadKADFile(m_strKADFileName);

    if(!tAddonPar.IsInitialized())
    {
        return E_FAIL;
    }
    rstrName = tAddonPar.m_strWizardName;
    rstrDescription = tAddonPar.m_strWizardDescription;

    return S_OK;
}

//------------------------------------------------------------------*
/**
 * create backparser output.
 *
 *  virtual function to be overwritten by target addons for special needs.
 *
 * @param           ptBaseTargetData: base target data object
 * @param           crstrFileName: target file name
 * @param           rstrXmlText: xml text (output)
 * @return          S_OK: ok
 *                  E_FAIL: error occurred
 *                  ...
 * @exception       -
 * @see             -
*/
HRESULT CBaseTargetObject::CreateBackparseOutput
(
    CBaseTargetData* ptBaseTargetData,
    const CString& crstrFileName, 
    CString& rstrXmlText
)
{
    CXMLDocument    tXMLDoc;
    CXMLNode        tDocNode;
    CXMLNode        tRootNode;

    if(tXMLDoc.GetDocNode(tDocNode))
    {
        if(tXMLDoc.CreateNode(tRootNode, CE_XMLTAG_NODE))
        {
            if(tDocNode.AppendChild(tRootNode))
            {
                SetBackparseRootNodeAttributes(ptBaseTargetData, crstrFileName, tRootNode);
                AddBackparseSubNodes(tRootNode, tXMLDoc);
                tXMLDoc.GetXMLString(rstrXmlText);
                return S_OK;
            }
        }
    }
    return E_FAIL;
}


//------------------------------------------------------------------*
/**
 * set backparse root node attributes.
 *
 *  save attributes like SOURCE, TEXT, ID, TYPE in target root node
 *
 * @param           ptBaseTargetData: base target data object.
 * @param           crstrFileName: target file name incl path.
 * @param           rtRootNode: xml root node
 * @return          -
 * @exception       -
 * @see             CreateBackparseOutput()
*/
void CBaseTargetObject::SetBackparseRootNodeAttributes(CBaseTargetData* ptBaseTargetData, const CString& crstrFileName, CXMLNode& rtRootNode)
{
    CString strFileName;
    // set standard attributes
    ptBaseTargetData->SetBackparseAttributes(rtRootNode);

    // SOURCE
    int iFound = crstrFileName.ReverseFind(_T('\\'));
    strFileName = crstrFileName.Mid(iFound+1);
    rtRootNode.SetAttribute(CE_XMLATTR_SOURCE, strFileName);
}


//------------------------------------------------------------------*
/**
 * add backparse sub nodes.
 *
 *  virtual function to insert sub nodes to backparse xml output.
 *  base version does nothing.
 *
 * @param           rtRootNode: xml root node
 * @param           rtXMLDoc: xml document object
 * @return          successful?
 * @exception       -
 * @see             CreateBackparseOutput()
*/
BOOL CBaseTargetObject::AddBackparseSubNodes(CXMLNode& rtRootNode, CXMLDocument& rtXMLDoc)
{
    return TRUE;
}



//------------------------------------------------------------------*
/**
 * action dispatcher.
 *
 *  actions that are not yet handled by the base implementation
 *  may be added here.
 *
 * @param           ptBaseTargetData: base target data object
 * @param           crstrMethodName: name of method to be performed
 * @param           crstrSourceFile: source file name
 * @param           crstrSourceId: id path of target
 * @param           crstrAddInfo: additional info string
 * @return          S_OK: ok
 *                  E_FAIL: error occurred
 *                  ...
 * @exception       -
 * @see             -
*/
HRESULT CBaseTargetObject::ActionDispatcher(CBaseTargetData* ptBaseTargetData,
                                            const CString& crstrMethodName,
                                            const CString& crstrSourceFile,
                                            const CString& crstrSourceId,
                                            const CString& crstrAddInfo,
                                            BOOL& bHandled)
{
    bHandled = FALSE;
    return S_OK;
}


HRESULT CBaseTargetObject::DoProjectInfoUpload(const CString strSourceId)
{
    HRESULT hr = S_OK;

    CDlgProjectInfoUpload dlg;
    
    CString strId = strSourceId;
    int i = strId.ReverseFind(_T('/'));
    if (i>0)
    {
        strId = strId.Mid(i+1);
    }

    CComBSTR sId(strId);
    CComBSTR sProjVers;
    CComBSTR sProjName;
    CComBSTR sModified;
    CComBSTR sCreated;
    CComBSTR sOwner;
    CComBSTR sComment1;
    CComBSTR sComment2;
    CComBSTR sComment3;
    
    Fire_ShowTargetMsgTab(sId);

    hr = Fire_ProjectInfoUpload(sId,
                               &sProjVers,
                               &sProjName,
                               &sModified,
                               &sCreated,
                               &sOwner,
                               &sComment1,
                               &sComment2,
                               &sComment3 );

    if (hr == S_OK)
    {
        CString strProjVers(sProjVers);
        CString strProjName(sProjName);
        CString strModified(sModified);
        CString strCreated(sCreated);
        CString strOwner(sOwner);
        CString strComment1(sComment1);
        CString strComment2(sComment2);
        CString strComment3(sComment3);

        dlg.m_strVersion = strProjVers;
        dlg.m_strName = strProjName;
        dlg.m_strModified = strModified;
        dlg.m_strCreated = strCreated;
        dlg.m_strOwner = strOwner;
        dlg.m_strText1 = strComment1;
        dlg.m_strText2 = strComment2;
        dlg.m_strText3 = strComment3;

        dlg.DoModal();
    }
    else
    {
        CString str;
        str.FormatMessage(IDS_PROJ_INFO_FAILED, hr);
        AfxMessageBox(str);
    }

    return hr;
}



HRESULT CBaseTargetObject::GetExeDelegator(CComPtr<ICEExecute>& pExeDelegator)
{
    // get exe delegator
    IUnknown*       pUnk;
    HRESULT         hr;
    CComBSTR        sExeDelegatorId(CE_COMPMAN_EXE);

    hr = Fire_OnComponentById(sExeDelegatorId, &pUnk);
    if(hr != S_OK)
    {
        return hr;
    }

    hr = pUnk->QueryInterface(IID_ICEExecute, (void**)&pExeDelegator);
    pUnk->Release();
    return hr;
}


CString CBaseTargetObject::GetInstallPath()
{
    HRESULT hr;
    CString strInstallPath;
    UINT   uiLen = _MAX_PATH;
    hr = UTIL_GetInstallPath(strInstallPath.GetBuffer(_MAX_PATH), &uiLen);
    strInstallPath.ReleaseBuffer();
    return strInstallPath;
}

/* ---------------------------------------------------------------------------- */
