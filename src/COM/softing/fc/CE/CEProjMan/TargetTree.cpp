


#include "stdafx.h"
#include "resource.h"
#include "TargetTree.h"

#include "CEProjMan.h"
#include "CEProjectManager.h"
#include "CEKadMan.h"

#include "SBLLexTree.h"

#include "BuildNr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



//------------------------------------------------------------------*
/**
 * Contructor
 *
 * @param           pProjMan:  a pointer to the ProjectManager class
 *                             to get additional information of the project
 * @return          
 * @exception       -
 * @see             
*/
CTargetTree::CTargetTree(CCEProjectManager* pProjMan) : CGenericTree(pProjMan)
{
    m_strFileName = CE_FILE_TARGETVIEW;
    m_bTargetTreeChanged = TRUE;
    m_bMsgMissingFiles = TRUE;
}


//------------------------------------------------------------------*
/**
 * destructor
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
CTargetTree::~CTargetTree()
{
    POSITION pos;
    pos = m_listTargetInfo.GetHeadPosition();

    while(pos)
    {
        TTargetInfo* pTargetInfo = m_listTargetInfo.GetNext(pos);
        delete pTargetInfo;
    }
}


//------------------------------------------------------------------*
/**
 * InitTree
 * initialize the class tree for the first time: get all necessary 
 * information from the kad and construct root and classes group nodes
 *
 * @return          TRUE: on success
 *                  FALSE: else
 * @exception       -
 * @see             
*/
BOOL CTargetTree::InitTree()
{
    GetStaticInfos();

    if (LoadViewData())
    {
        IXMLDOMDocument *pXMLDoc;
        m_domDocument.GetIXMLDocument(&pXMLDoc);
        m_pProjectManager->Fire_InitTargetTree((IUnknown*)pXMLDoc);
        if (pXMLDoc)
        {
            pXMLDoc->Release();
        }
        m_bInit = FALSE; 
        CheckSourceFilesToReparse();
        Reparse();

        return TRUE;
    }

    TSourceFileInfo* pProjFileInfo;

    // create project node
    CXMLNode docRoot;
    CXMLNode projectNode;

    PROJ_MAN_ASSERT(m_pProjectManager);
    PROJ_MAN_ASSERT(m_pProjectManager->GetKadManager());

    CString strProjNodeId;
    strProjNodeId = m_pProjectManager->GetProjectName()+_T(".")+ID_PATH_TARGETVIEW;
    CString strVersion;
    strVersion.Format(_T("%i"), PRODUCT_BUILD);
    if (  (!m_domDocument.GetDocNode(docRoot)) 
        ||(!m_domDocument.CreateNode(projectNode, CE_XMLTAG_NODE))
        ||(!docRoot.AppendChild(projectNode))
        ||(!projectNode.SetAttribute(CE_XMLATTR_TEXT, m_pProjectManager->GetProjectName()))
        ||(!projectNode.SetAttribute(CE_XMLATTR_ID, strProjNodeId))
        ||(!projectNode.SetAttribute(CE_XMLATTR_SOURCE, m_pProjectManager->GetProjectFileName())) 
        ||(!projectNode.SetAttribute(CE_XMLATTR_PARSERSOURCE, m_pProjectManager->GetProjectFileName())) 
        ||(!projectNode.SetAttribute(PRJMAN_VERSION, strVersion))    )
    {
        return FALSE;
    }

    Reparse4CPAttributes(&projectNode);


    CComQIPtr<ITargets, &IID_ITargets> kadTargets;
    CComBSTR sNodeType;
    
    kadTargets = m_pProjectManager->GetKadManager();
    if (!kadTargets)
    {
        return FALSE;
    }

    kadTargets->GetTargetsTypeString(&sNodeType);

    if (!projectNode.SetAttribute(CE_XMLATTR_TYPE, (CString)sNodeType))
    {
        return FALSE;
    }

    // add project file to source file list
    CString strProjFileName = m_pProjectManager->GetProjectFileName();
    pProjFileInfo = CreateSourceFileInfo(strProjFileName,
                                         projectNode, _T(""));
    pProjFileInfo->strIdPath = CE_XML_IDPATH_SEP + strProjNodeId;
    SetSourceTime(pProjFileInfo, m_pProjectManager->GetProjectFileNameWithPath());
    SourceFileMap_SetAt(strProjFileName, pProjFileInfo, m_mapSourceFiles);
//    m_pProjectManager->Fire_OnFileAdded((CComBSTR)strProjFileName, FV_FILE_SOURCE);

    ReparseProjFile();
    Reparse(FALSE);

    IXMLDOMDocument *pXMLDoc;
    m_domDocument.GetIXMLDocument(&pXMLDoc);
    m_pProjectManager->Fire_InitTargetTree((IUnknown*)pXMLDoc);
    pXMLDoc->Release();
    m_bInit = FALSE;

    return TRUE;
}


//------------------------------------------------------------------*
/**
 * ReparseProjFile
 * the project file has changed and needs to be reparsed. Get all
 * necessary changes for the class view
 *
 * @return          TRUE: on success
 *                  FALSE: else
 * @exception       -
 * @see             
*/
BOOL CTargetTree::ReparseProjFile()
{
    BOOL bTargetTreeChanged = FALSE;

    CList<CString, CString&> newSourceFileList;

    CXMLNode projectNode;
    (m_pProjectManager->GetProjectFileHandler())->ReparseProjectFile();

    if (!m_domDocument.GetRootNode(projectNode))
    {
        return FALSE;
    }

    Reparse4CPAttributes(&projectNode);

    CInstanceTree* pInstTree = NULL;
    pInstTree = m_pProjectManager->GetInstanceTree();
    if (pInstTree)
    {
        pInstTree->ReparseProjFile();
    }

    projectNode.RemoveAllChilds();

    CSBLLexTree<bool>   tFileNames;
    bool*               pbTmp;
    CString             strFileName;

    tFileNames.setModusOwner();

    CSection* pTargetSection = NULL;
    BOOL ret  = (m_pProjectManager->GetProjectFileHandler())->FindSection(CE_CP_TARGETS, &pTargetSection);
    if (ret)
    {
        CKeyAndValue* pTargetFile = NULL;
        ret = pTargetSection->GetFirstKeyAndValue(&pTargetFile);
        while (ret)
        {
            if (pTargetFile->GetKey().CompareNoCase(CE_CP_FILE)!=0)
            {
                ret = pTargetSection->GetNextKeyAndValue(&pTargetFile);
                continue;
            }

            strFileName = pTargetFile->GetValue();

            strFileName = pTargetFile->GetValue();
            pbTmp = new bool;
            tFileNames.insertData(strFileName, pbTmp);

            ret = pTargetSection->GetNextKeyAndValue(&pTargetFile);
        }
    }
    // now run over lex tree of file names an insert them to source file list
    CStringArray    astrFileNames;
    tFileNames.getAllStringsFromSubstring("", astrFileNames);
    long    lNumFiles = astrFileNames.GetSize();
    for(long lFile = 0; lFile < lNumFiles; ++lFile)
    {
        strFileName = astrFileNames[lFile];

        newSourceFileList.AddTail(strFileName);

        TSourceFileInfo* pFileInfo;

        if (SourceFileMap_Lookup(strFileName, pFileInfo, m_mapSourceFiles))
        {
            projectNode.AppendChild(pFileInfo->xmlNode);                        
        }
        else
        {
            CXMLNode fileNode;

            if (m_domDocument.CreateNode(fileNode, CE_XMLTAG_LINK) && 
                projectNode.AppendChild(fileNode))
            {
                fileNode.SetAttribute(CE_XMLATTR_PARSERSOURCE, strFileName);

                pFileInfo = CreateSourceFileInfo(strFileName, fileNode, 
                                                 m_pProjectManager->GetProjectFileName());
                SourceFileMap_SetAt(strFileName, pFileInfo, m_mapSourceFiles);
                if(!m_bInit)
                {
                    m_pProjectManager->Fire_OnFileAdded((CComBSTR)strFileName, FV_FILE_SOURCE);
                }
                //ReparseFile(pFileInfo, FALSE);
                // reset id
                //(pFileInfo->xmlNode).GetIdPath(pFileInfo->strIdPath);
                AddSourceFileToReparse(strFileName);
                m_bFileViewChanged = TRUE;
                ATLTRACE("Add source file (reparse cp file target view): %s\n", strFileName);
                bTargetTreeChanged = TRUE;      
            }
        }
    }

    // change project file info parse date
    TSourceFileInfo* pPFInfo;
    SourceFileMap_Lookup(m_pProjectManager->GetProjectFileName(), pPFInfo, m_mapSourceFiles);

    POSITION pos = (pPFInfo->strlChildFileNames).GetHeadPosition();
    CString childFileName;
    while (pos!=NULL)
    {
        childFileName = (pPFInfo->strlChildFileNames).GetAt(pos);
        if (!StringList_Find(childFileName, newSourceFileList))
        {
            TSourceFileInfo* pRemoveFileInfo;
            SourceFileMap_Lookup(childFileName, pRemoveFileInfo, m_mapSourceFiles);
            CString strRemoveId = pRemoveFileInfo->strIdPath;
            RemoveSourceFileAndChilds(childFileName);
            FireUpdateTree(strRemoveId, eUpdateDelete);
            bTargetTreeChanged = TRUE;
            m_bFileViewChanged = TRUE;
        }

        (pPFInfo->strlChildFileNames).GetNext(pos);
    }   


    (pPFInfo->strlChildFileNames).RemoveAll();
    (pPFInfo->strlChildFileNames).AddTail(&newSourceFileList);


    PROJ_MAN_ASSERT(pPFInfo);
    ATLTRACE("Setting project file time\n");
    SetSourceTime(pPFInfo, m_pProjectManager->GetProjectFileNameWithPath());
    
    // in target view there are no subfolder, alsways mark project node as unchanged
    // and all children could have changed
    //if (bTargetTreeChanged)
    //{
    //    CString strChangedNode;
    //    projectNode.GetIdPath(strChangedNode);
    //    AddChangedNode(strChangedNode);
    //}

    return TRUE;
}



//------------------------------------------------------------------*
/**
 * FireUpdateTree informs all client that some nodes in the target
 * tree have changed
 *
 * @exception       -
 * @see             
*/
void CTargetTree::FireUpdateTree(const CString& strChangedNode, enum E_UPDATE_MODE eMode)
{
    if (!m_bInit)
    {
        CComBSTR sChangedNode = strChangedNode;
        m_pProjectManager->Fire_UpdateTargetTree(sChangedNode, eMode);
    }

    m_bTargetTreeChanged = TRUE;  // tell instance view that something has changed
}



//------------------------------------------------------------------*
/**
 * Determines if the IDPath contains to a node in the target tree
 * this function checks only the beginning of the path, if it starts
 * with 'projectname'.TARGETVIEW it is a class tree path,
 * this function does not check if the node for the full path
 * does exist.
 *
 * @param     strIdPath : the id path of the node      
 * @return    TRUE : it is a target tree path
 *            FALSE: it is no target tree path
 * @exception       -
 * @see             
*/
BOOL CTargetTree::IsViewPath(const CString& strIdPath)
{
    CString strProjNodePart;
    CString strFullPath = strIdPath;
    
    if (!StripIdFromIdPath(strFullPath, strProjNodePart))
    {
        return FALSE;
    }
    if (!StripIdFromIdPath(strFullPath, strProjNodePart))
    {
        return FALSE;
    }

    CString strExpId = m_pProjectManager->GetProjectName()+_T(".")+ID_PATH_TARGETVIEW;
    if (strProjNodePart.CompareNoCase(strExpId)==0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}




//------------------------------------------------------------------*
/**
 * This function fills the container interface for the project file,
 * if the new file should be inserted in the target view tree.
 *
 * @param           strLocation: the location of the new file in the target view
 * @param           strLinkName: the name of the new file
 * @return          TRUE : if the operation has been successful
 *                  FALSE: else
 * @exception       -
 * @see             
*/
BOOL CTargetTree::CreateLink(const CString& strLocation, 
                             const CString& strLinkName)
{
    PROJ_MAN_ASSERT(m_pProjectManager);
    PROJ_MAN_ASSERT(m_pProjectManager->GetProjectFileHandler());
    m_pProjectManager->GetProjectFileHandler()->ReparseProjectFile();
    CSection* pSection = NULL;
    if (!m_pProjectManager->GetProjectFileHandler()->FindSection(CE_CP_TARGETS, &pSection))
    {
        // create section
        if (!m_pProjectManager->GetProjectFileHandler()->AddSection(CE_CP_TARGETS))
        {
            return FALSE;
        }
        if (!m_pProjectManager->GetProjectFileHandler()->FindSection(CE_CP_TARGETS, &pSection))
        {
            return FALSE;
        }
    }
    PROJ_MAN_ASSERT(pSection);
    // is file=linkName already in this section?
    CKeyAndValue* keyAndValue;
    if (pSection->FindKeyAndValue(CE_CP_FILE, strLinkName, &keyAndValue))
    {
        // don't add it again
        return FALSE;
    }
    if (!pSection->AddKeyAndValue(CE_CP_FILE, strLinkName))
    {
        return FALSE;
    }

    m_pProjectManager->GetProjectFileHandler()->WriteProjectFile();
    m_bProjFileToReparse = TRUE;

    return TRUE;
}


//------------------------------------------------------------------*
/**
 * This function fills the container interface for the project file,
 * if a file the target view tree should be renamed.
 *
 * @param           strLocation: the location of the new file in the target view
 * @param           strOldLinkName: the name of the old file
 * @param           strNewLinkName: the name of the new file
 * @return          TRUE : if the operation has been successful
 *                  FALSE: else
 * @exception       -
 * @see             
*/
BOOL CTargetTree::RenameLink(const CString& strLocation, 
                             const CString& strOldLinkName,
                             const CString& strNewLinkName)
{
    PROJ_MAN_ASSERT(m_pProjectManager);
    PROJ_MAN_ASSERT(m_pProjectManager->GetProjectFileHandler());
    m_pProjectManager->GetProjectFileHandler()->ReparseProjectFile();
    CSection* pSection;
    if (!m_pProjectManager->GetProjectFileHandler()->FindSection(CE_CP_TARGETS, &pSection))
    {
        return FALSE;
    }
    // is file=linkName already in this section?
    CKeyAndValue* pKeyAndValue;
    if (!pSection->FindKeyAndValue(CE_CP_FILE, strOldLinkName, &pKeyAndValue))
    {
        // don't add it again
        return FALSE;
    }
    pKeyAndValue->SetValue(strNewLinkName);

    m_pProjectManager->GetProjectFileHandler()->WriteProjectFile();
    m_bProjFileToReparse = TRUE;

    return TRUE;
}


//------------------------------------------------------------------*
/**
 * This function fills the container interface for the project file,
 * if a file in the target view tree should be deleted.
 *
 * @param           strLocation: the location of the new file in the target view
 * @param           strLinkName: the name of the file to delete
 * @return          TRUE : if the operation has been successful
 *                  FALSE: else
 * @exception       -
 * @see             
*/
BOOL CTargetTree::DeleteLink(const CString& strLocation,
                             const CString& strLinkName)
{
    PROJ_MAN_ASSERT(m_pProjectManager);
    PROJ_MAN_ASSERT(m_pProjectManager->GetProjectFileHandler());
    m_pProjectManager->GetProjectFileHandler()->ReparseProjectFile();
    CSection* pSection;
    if (!m_pProjectManager->GetProjectFileHandler()->FindSection(CE_CP_TARGETS, &pSection))
    {
        return FALSE;
    }
    // is file=linkName already in this section?
    CKeyAndValue* keyAndValue;
    if (!pSection->FindKeyAndValue(CE_CP_FILE, strLinkName, &keyAndValue))
    {
        // don't add it again
        return FALSE;
    }
    if (!pSection->DeleteKeyAndValue(CE_CP_FILE, strLinkName))
    {
        return FALSE;
    }

    m_pProjectManager->GetProjectFileHandler()->WriteProjectFile();
    m_bProjFileToReparse = TRUE;

    return TRUE;
}



//------------------------------------------------------------------*
/**
 * Get all static information for target tre once at the beginning
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
BOOL CTargetTree::GetStaticInfos()
{
    HRESULT hr;
    CComQIPtr<ITargets, &IID_ITargets> kadTargets;
    
    kadTargets = m_pProjectManager->GetKadManager();
    if (!kadTargets)
    {
        return FALSE;
    }

    CComPtr<ITarget> target;
    CComPtr<IEnumTargets> targetEnum;
    
    kadTargets->GetTargetEnumerator(&targetEnum);
    if (!targetEnum)
    {
        return FALSE;
    }

    targetEnum->Reset();

    ULONG n;
    hr = targetEnum->Next(1, &target, &n);
    if (!SUCCEEDED(hr))
    {
        return FALSE;
    }


    m_bSuppRetain = FALSE;

    while (target)
    {
        TTargetInfo* pTargetInfo;
        pTargetInfo = new TTargetInfo;
        
        // get target type
        CComBSTR sAttrTargetText;
        target->GetTypeString(&sAttrTargetText);
        pTargetInfo->strType = sAttrTargetText;

        // get file extension
        CComBSTR sAttrTargetExt;
        target->GetExt(&sAttrTargetExt);
        pTargetInfo->strFileExt = sAttrTargetExt;

        // get iec features 
        CComPtr<IIECFeatures> iecFeatures;
        hr = target->GetIECFeatures(&iecFeatures);
        if (hr == S_OK)
        {
            // supported data types
            CComBSTR sSuppDataTypes;
            hr = iecFeatures->GetDataTypes(&sSuppDataTypes);
            if (hr == S_OK)
            {
                FillTargetDataTypes(pTargetInfo, (CString)sSuppDataTypes);
            }
            else
            {
                // pass empty string: add all available IEC data types
                FillTargetDataTypes(pTargetInfo, _T(""));
            }
            // languages
            CComBSTR sSuppLanguages;
            hr = iecFeatures->GetLanguages(&sSuppLanguages);
            if (hr == S_OK)
            {
                FillTargetLanguages(pTargetInfo, (CString)sSuppLanguages);
            }
            else
            {
                FillTargetLanguages(pTargetInfo, _T(""));
            }
            // RETAIN
            BOOL bSuppRetain;
            hr = iecFeatures->GetRetain(&bSuppRetain);
            if ((hr==S_OK) && (bSuppRetain==TRUE))
            {
                pTargetInfo->bSuppRetain = TRUE;
                m_bSuppRetain = TRUE;
            }
            else
            {
                pTargetInfo->bSuppRetain = FALSE;
            }
        }
        else
        {
            // pass empty string: add all available IEC data types
            FillTargetDataTypes(pTargetInfo, _T(""));
            FillTargetLanguages(pTargetInfo, _T(""));
            pTargetInfo->bSuppRetain = FALSE;
        }

        m_listTargetInfo.AddTail(pTargetInfo);

        target.Release();
        hr = targetEnum->Next(1, &target, &n);
    }

    CreateMaxSupportedDataTypes();
    CreateMaxSupportedLanguages();

    return TRUE;
}


//------------------------------------------------------------------*
/**
 * FillTargetDataTypes
 *
 * @param           pTargetInfo: the target info that should be filled
 * @param           strDataTypes: the string containing 
 *                                the comma separated data type list
 *                                empty string: nothing specified: add all iec data types
 * @exception       -
 * @see             
*/
void CTargetTree::FillTargetDataTypes(TTargetInfo* pTargetInfo, const CString strDataTypes)
{
    if (!strDataTypes.IsEmpty())
    {
        CString strType;
        CString strRest;

        int cpos;

        strRest = strDataTypes;
        cpos = strRest.Find(_T(','), 0);
        while (cpos>=0)
        {
            strType = strRest.Left(cpos);
            strRest = strRest.Mid(cpos+1);
            strType.TrimLeft();
            strType.TrimRight();
            if (!strType.IsEmpty())
            {
                (pTargetInfo->listSuppDataTypes).AddTail(strType);
            }
            cpos = strRest.Find(_T(','), 0);
        }

        strRest.TrimLeft();
        strRest.TrimRight();
        if (!strRest.IsEmpty())
        {
            (pTargetInfo->listSuppDataTypes).AddTail(strRest);
        }
    }
    else    // add all possible iec data types
    {
        (pTargetInfo->listSuppDataTypes).AddTail((CString)_T("BOOL"));
        (pTargetInfo->listSuppDataTypes).AddTail((CString)_T("BYTE"));
        (pTargetInfo->listSuppDataTypes).AddTail((CString)_T("WORD"));
        (pTargetInfo->listSuppDataTypes).AddTail((CString)_T("DWORD"));
        (pTargetInfo->listSuppDataTypes).AddTail((CString)_T("LWORD"));
     
        (pTargetInfo->listSuppDataTypes).AddTail((CString)_T("USINT"));
        (pTargetInfo->listSuppDataTypes).AddTail((CString)_T("SINT"));
        (pTargetInfo->listSuppDataTypes).AddTail((CString)_T("UINT")); 
        (pTargetInfo->listSuppDataTypes).AddTail((CString)_T("INT")); 
        (pTargetInfo->listSuppDataTypes).AddTail((CString)_T("UDINT"));
        (pTargetInfo->listSuppDataTypes).AddTail((CString)_T("DINT"));
        (pTargetInfo->listSuppDataTypes).AddTail((CString)_T("ULINT"));
        (pTargetInfo->listSuppDataTypes).AddTail((CString)_T("LINT"));

        (pTargetInfo->listSuppDataTypes).AddTail((CString)_T("REAL"));
        (pTargetInfo->listSuppDataTypes).AddTail((CString)_T("LREAL"));
       
        (pTargetInfo->listSuppDataTypes).AddTail((CString)_T("STRING"));
        (pTargetInfo->listSuppDataTypes).AddTail((CString)_T("WSTRING"));
       
        (pTargetInfo->listSuppDataTypes).AddTail((CString)_T("DT"));
        (pTargetInfo->listSuppDataTypes).AddTail((CString)_T("TOD"));
        (pTargetInfo->listSuppDataTypes).AddTail((CString)_T("DATE"));
        (pTargetInfo->listSuppDataTypes).AddTail((CString)_T("TIME"));


        (pTargetInfo->listSuppDataTypes).AddTail((CString)_T("QVT_BOOL"));
        (pTargetInfo->listSuppDataTypes).AddTail((CString)_T("QVT_BYTE"));
        (pTargetInfo->listSuppDataTypes).AddTail((CString)_T("QVT_WORD"));
        (pTargetInfo->listSuppDataTypes).AddTail((CString)_T("QVT_DWORD"));
        (pTargetInfo->listSuppDataTypes).AddTail((CString)_T("QVT_LWORD"));
     
        (pTargetInfo->listSuppDataTypes).AddTail((CString)_T("QVT_USINT"));
        (pTargetInfo->listSuppDataTypes).AddTail((CString)_T("QVT_SINT"));
        (pTargetInfo->listSuppDataTypes).AddTail((CString)_T("QVT_UINT")); 
        (pTargetInfo->listSuppDataTypes).AddTail((CString)_T("QVT_INT")); 
        (pTargetInfo->listSuppDataTypes).AddTail((CString)_T("QVT_UDINT"));
        (pTargetInfo->listSuppDataTypes).AddTail((CString)_T("QVT_DINT"));
        (pTargetInfo->listSuppDataTypes).AddTail((CString)_T("QVT_ULINT"));
        (pTargetInfo->listSuppDataTypes).AddTail((CString)_T("QVT_LINT"));

        (pTargetInfo->listSuppDataTypes).AddTail((CString)_T("QVT_REAL"));
        (pTargetInfo->listSuppDataTypes).AddTail((CString)_T("QVT_LREAL"));
       
        (pTargetInfo->listSuppDataTypes).AddTail((CString)_T("QVT_STRING"));
        (pTargetInfo->listSuppDataTypes).AddTail((CString)_T("QVT_WSTRING"));
       
        (pTargetInfo->listSuppDataTypes).AddTail((CString)_T("QVT_DT"));
        (pTargetInfo->listSuppDataTypes).AddTail((CString)_T("QVT_TOD"));
        (pTargetInfo->listSuppDataTypes).AddTail((CString)_T("QVT_DATE"));
        (pTargetInfo->listSuppDataTypes).AddTail((CString)_T("QVT_TIME"));
    }
}


//------------------------------------------------------------------*
/**
 * CreateMaxSupportedTargets constructs the supported data types from
 * the supported data types of the single targets
 *
 * @exception       -
 * @see             
*/
void CTargetTree::CreateMaxSupportedDataTypes()
{
    CList<CString, CString&> listTempDataTypes;
    POSITION posTargets;
    posTargets = m_listTargetInfo.GetHeadPosition();

    while(posTargets)
    {
        TTargetInfo* pTargetInfo = m_listTargetInfo.GetNext(posTargets);
        POSITION posType;
        posType = (pTargetInfo->listSuppDataTypes).GetHeadPosition();
        while (posType)
        {
            CString strType;
            strType = (pTargetInfo->listSuppDataTypes).GetNext(posType);

            // search if data type does already exist in list
            BOOL bFound = FALSE;
            POSITION posData;
            posData = m_listMaxSuppDataTypes.GetHeadPosition();
            while (posData)
            {
                CString strOldType = m_listMaxSuppDataTypes.GetNext(posData);
                if (strOldType.CompareNoCase(strType)==0)
                {
                    bFound = TRUE;
                    break;
                }
            }
            if (!bFound)
            {
                listTempDataTypes.AddTail(strType);
            }
        }
    }


    m_listMaxSuppDataTypes.AddTail((CString)_T("BOOL"));
    m_listMaxSuppDataTypes.AddTail((CString)_T("BYTE"));
    m_listMaxSuppDataTypes.AddTail((CString)_T("WORD"));
    m_listMaxSuppDataTypes.AddTail((CString)_T("DWORD"));
    m_listMaxSuppDataTypes.AddTail((CString)_T("LWORD"));
 
    m_listMaxSuppDataTypes.AddTail((CString)_T("USINT"));
    m_listMaxSuppDataTypes.AddTail((CString)_T("SINT"));
    m_listMaxSuppDataTypes.AddTail((CString)_T("UINT")); 
    m_listMaxSuppDataTypes.AddTail((CString)_T("INT")); 
    m_listMaxSuppDataTypes.AddTail((CString)_T("UDINT"));
    m_listMaxSuppDataTypes.AddTail((CString)_T("DINT"));
    m_listMaxSuppDataTypes.AddTail((CString)_T("ULINT"));
    m_listMaxSuppDataTypes.AddTail((CString)_T("LINT"));

    m_listMaxSuppDataTypes.AddTail((CString)_T("REAL"));
    m_listMaxSuppDataTypes.AddTail((CString)_T("LREAL"));
   
    m_listMaxSuppDataTypes.AddTail((CString)_T("STRING"));
    m_listMaxSuppDataTypes.AddTail((CString)_T("WSTRING"));
   
    m_listMaxSuppDataTypes.AddTail((CString)_T("DT"));
    m_listMaxSuppDataTypes.AddTail((CString)_T("TOD"));
    m_listMaxSuppDataTypes.AddTail((CString)_T("DATE"));
    m_listMaxSuppDataTypes.AddTail((CString)_T("TIME"));


    m_listMaxSuppDataTypes.AddTail((CString)_T("QVT_BOOL"));
    m_listMaxSuppDataTypes.AddTail((CString)_T("QVT_BYTE"));
    m_listMaxSuppDataTypes.AddTail((CString)_T("QVT_WORD"));
    m_listMaxSuppDataTypes.AddTail((CString)_T("QVT_DWORD"));
    m_listMaxSuppDataTypes.AddTail((CString)_T("QVT_LWORD"));
 
    m_listMaxSuppDataTypes.AddTail((CString)_T("QVT_USINT"));
    m_listMaxSuppDataTypes.AddTail((CString)_T("QVT_SINT"));
    m_listMaxSuppDataTypes.AddTail((CString)_T("QVT_UINT")); 
    m_listMaxSuppDataTypes.AddTail((CString)_T("QVT_INT")); 
    m_listMaxSuppDataTypes.AddTail((CString)_T("QVT_UDINT"));
    m_listMaxSuppDataTypes.AddTail((CString)_T("QVT_DINT"));
    m_listMaxSuppDataTypes.AddTail((CString)_T("QVT_ULINT"));
    m_listMaxSuppDataTypes.AddTail((CString)_T("QVT_LINT"));

    m_listMaxSuppDataTypes.AddTail((CString)_T("QVT_REAL"));
    m_listMaxSuppDataTypes.AddTail((CString)_T("QVT_LREAL"));
   
    m_listMaxSuppDataTypes.AddTail((CString)_T("QVT_STRING"));
    m_listMaxSuppDataTypes.AddTail((CString)_T("QVT_WSTRING"));
   
    m_listMaxSuppDataTypes.AddTail((CString)_T("QVT_DT"));
    m_listMaxSuppDataTypes.AddTail((CString)_T("QVT_TOD"));
    m_listMaxSuppDataTypes.AddTail((CString)_T("QVT_DATE"));
    m_listMaxSuppDataTypes.AddTail((CString)_T("QVT_TIME"));

    if (!listTempDataTypes.IsEmpty())
    {
        POSITION posDT = m_listMaxSuppDataTypes.GetHeadPosition();
        while (posDT)
        {
            POSITION posDel = posDT;
            CString strDT = m_listMaxSuppDataTypes.GetNext(posDT);
            if (!StringList_Find(strDT, listTempDataTypes))
            {
                m_listMaxSuppDataTypes.RemoveAt(posDel);
            }
        }
    }

}


//------------------------------------------------------------------*
/**
 * FillTargetLanguages
 *
 * @param           pTargetInfo: the target info that should be filled
 * @param           strLanguages: the string containing 
 *                                the comma separated language list
 *                                empty string: nothing specified: add all iec languages
 * @return          
 * @exception       -
 * @see             
*/
void CTargetTree::FillTargetLanguages(TTargetInfo* pTargetInfo, const CString strLanguages)
{
    if (!strLanguages.IsEmpty())
    {
        CString strLang;
        CString strRest;

        int cpos;

        strRest = strLanguages;
        cpos = strRest.Find(_T(','), 0);
        while (cpos>=0)
        {
            strLang = strRest.Left(cpos);
            strRest = strRest.Mid(cpos+1);
            strLang.TrimLeft();
            strLang.TrimRight();
            if (!strLang.IsEmpty())
            {
                (pTargetInfo->listSuppLanguages).AddTail(strLang);
            }
            cpos = strRest.Find(_T(','), 0);
        }

        strRest.TrimLeft();
        strRest.TrimRight();
        if (!strRest.IsEmpty())
        {
            (pTargetInfo->listSuppLanguages).AddTail(strRest);
        }
    }
    else    // add all possible iec data types
    {
        (pTargetInfo->listSuppLanguages).AddTail((CString)_T("ST"));
        (pTargetInfo->listSuppLanguages).AddTail((CString)_T("FBD"));
        (pTargetInfo->listSuppLanguages).AddTail((CString)_T("LD"));
        (pTargetInfo->listSuppLanguages).AddTail((CString)_T("SFC"));

    }
}


//------------------------------------------------------------------*
/**
 * CreateMaxSupportedLanguages constructs the supported languages from
 * the supported languages of the single targets
 *
 * @see             
*/
void CTargetTree::CreateMaxSupportedLanguages()
{
    POSITION posTargets;
    posTargets = m_listTargetInfo.GetHeadPosition();

    while(posTargets)
    {
        TTargetInfo* pTargetInfo = m_listTargetInfo.GetNext(posTargets);
        POSITION posLang;
        posLang = (pTargetInfo->listSuppLanguages).GetHeadPosition();
        while (posLang)
        {
            CString strLang;
            strLang = (pTargetInfo->listSuppLanguages).GetNext(posLang);

            // search if data type does already exist in list
            BOOL bFound = FALSE;
            POSITION posData;
            posData = m_listMaxSuppLanguages.GetHeadPosition();
            while (posData)
            {
                CString strOldLang = m_listMaxSuppDataTypes.GetNext(posData);
                if (strOldLang.CompareNoCase(strLang)==0)
                {
                    bFound = TRUE;
                    break;
                }
            }
            if (!bFound)
            {
                m_listMaxSuppLanguages.AddTail(strLang);
            }
        }
    }
}


//------------------------------------------------------------------*
/**
 * GetAllTargetTypes returns all target types defined in the kad.
 *
 * @param           pparrTargetType - this string array will be filled 
 *                  with the target names
 * @return          TRUE - on success
 *                  FALSE - error
 * @exception       -
 * @see             
*/
BOOL CTargetTree::GetAllTargetTypes(SAFEARRAY** pparrTargetTypes)
{
    HRESULT hr;
    SAFEARRAYBOUND Bound;
    Bound.lLbound = 0;
    Bound.cElements = m_listTargetInfo.GetCount();

    *pparrTargetTypes = ::SafeArrayCreate(VT_BSTR, 1, &Bound);
    if (NULL == (*pparrTargetTypes)) 
    {
        return FALSE;    
    }

    long lIndex = 0;
    POSITION pos = m_listTargetInfo.GetHeadPosition();
    CString str;

    while(pos!=NULL)
    {
        CString strType = (m_listTargetInfo.GetNext(pos))->strType;
        CComBSTR sType(strType);
        hr = ::SafeArrayPutElement(*pparrTargetTypes, &lIndex, 
                                    (void*)sType);
        if (FAILED(hr))
        {
            ::SafeArrayDestroy(*pparrTargetTypes);
            return FALSE;
        }
        lIndex++;
    }

    return TRUE;  
}



//------------------------------------------------------------------*
/**
 * GetAllTargets returns all targets in the target tree, all nodes of
 * a type belonging to the target type are targets, a target has 
 * special attributes, this function can return the file names
 * of the targets or the id paths.
 *
 * @param           pparrTargets: the string array will be filled 
 *                                with the targets (file names or ids)
 * @param           bIdPaths: if true the array will be filled with id paths
 *                            else with file names
 * @return          
 * @exception       -
 * @see             
*/
BOOL CTargetTree::GetAllTargets(SAFEARRAY** pparrTargets, BOOL bIdPaths)
{
    CList<CString, CString&> targetList;

    POSITION pos;
    pos = m_mapSourceFiles.GetStartPosition();
    while(pos!=NULL)
    {
        TSourceFileInfo *finfo=NULL;
        CString strFileName;
        m_mapSourceFiles.GetNextAssoc(pos, strFileName, finfo);
        PROJ_MAN_ASSERT(finfo);
        CXMLNode node = finfo->xmlNode;
        CString strType;
        if (!node.GetAttribute(CE_XMLATTR_TYPE, strType))
        {
            continue;
        }
        if (FindTargetType(strType))
        {
            if (bIdPaths)
            {
                CString strPath;
                strPath = finfo->strIdPath;
                targetList.AddTail(strPath);
            }
            else
            {
                targetList.AddTail(strFileName);
            }
        }
    }


    HRESULT hr;
    SAFEARRAYBOUND Bound;
    Bound.lLbound = 0;
    Bound.cElements = targetList.GetCount();

    *pparrTargets = ::SafeArrayCreate(VT_BSTR, 1, &Bound);
    if (NULL == (*pparrTargets)) 
    {
        return FALSE;    
    }

    long lIndex = 0;
    pos = targetList.GetHeadPosition();

    while(pos!=NULL)
    {
        CString strTarget = targetList.GetNext(pos);
        CComBSTR sTarget(strTarget);
        hr = ::SafeArrayPutElement(*pparrTargets, &lIndex, 
                                    (void*)sTarget);
        if (FAILED(hr))
        {
            ::SafeArrayDestroy(*pparrTargets);
            return FALSE;
        }
        lIndex++;
    }

    return TRUE;
}



//------------------------------------------------------------------*
/**
 * Try to add a new file to target view.
 *
 * @param           strFileName: the file name
 * @param           bTry: only try but don't do it (default FALSE)
 * @return          TRUE: file has been added (try mode: can be added)
 *                  FALSE: file could not be added (try mode: cannot be added)
 * @exception       -
 * @see             
*/
BOOL CTargetTree::FVAddFile(const CString& strFileName, const CString& strBackparsed, BOOL bTry /*=FALSE*/)
{
    CXMLDocument tempDoc;
    if (!tempDoc.LoadXMLString(strBackparsed))
    {
        return FALSE;
    }
    
    CXMLNode newNode;
    if (!tempDoc.GetRootNode(newNode))
    {
        return FALSE;
    }

    CString strType;

    if (!newNode.GetAttribute(CE_XMLATTR_TYPE, strType))
    {
        return FALSE;
    }
    
    if (!FindTargetType(strType))
    {
        return FALSE;
    }

    CSection* pSection;
    if (!m_pProjectManager->GetProjectFileHandler()->FindSection(CE_CP_TARGETS, &pSection))
    {
        if (!m_pProjectManager->GetProjectFileHandler()->AddSection(CE_CP_TARGETS))
        {
            return FALSE;
        }
        if (!m_pProjectManager->GetProjectFileHandler()->FindSection(CE_CP_TARGETS, &pSection))
        {
            return FALSE;
        }
    }

    CKeyAndValue* pKeyAndValue;
    if (pSection->FindKeyAndValue(CE_CP_FILE, strFileName, &pKeyAndValue))
    {
        return FALSE;
    }

    // if not only try -> do it
    if(!bTry)
    {
        if (!pSection->AddKeyAndValue(CE_CP_FILE, strFileName))
        {
            return FALSE;
        }

        m_pProjectManager->GetProjectFileHandler()->WriteProjectFile();
        m_bProjFileToReparse = TRUE;
    }

    return TRUE;
}

//------------------------------------------------------------------*
/**
 * Try to remove a file from target view.
 *
 * @param           strFileANme: the file name
 * @param           bTry: only try but don't do it (default FALSE)
 * @return          TRUE: file has been removed
 *                  FALSE: file could not be removed
 * @exception       -
 * @see             
*/
BOOL CTargetTree::FVRemoveFile(const CString& strFileName, BOOL bTry /*=FALSE*/)
{
    CSection* pSection = NULL;
    if (!m_pProjectManager->GetProjectFileHandler()->FindSection(CE_CP_TARGETS, &pSection))
    {
        return FALSE;
    }

    // if not only try -> do it
    if(!bTry)
    {
        if (!pSection->DeleteKeyAndValue(CE_CP_FILE, strFileName))
        {
            return FALSE;
        }
        m_pProjectManager->GetProjectFileHandler()->WriteProjectFile();
        m_bProjFileToReparse = TRUE;
    }

    return TRUE;
}




//------------------------------------------------------------------*
/**
 * GetTypeOfTarget returns the type of a target for a given target id path.
 *
 * @param           strTargetId : the id path of the target
 * @param           strType : the type of the target
 * @return          TRUE : on success
 *                  FALSE: error
 * @exception       -
 * @see             
*/
BOOL CTargetTree::GetTypeOfTarget(const CString& strTargetId, CString& strType)
{
    CString strFullTargetId = strTargetId;

    if (strTargetId.Find(CE_XML_IDPATH_SEP_CHAR)==-1)
    {
        strFullTargetId = CE_XML_IDPATH_SEP + m_pProjectManager->GetProjectName()+_T(".") + 
            ID_PATH_TARGETVIEW + CE_XML_IDPATH_SEP + strTargetId;
    }

    CXMLNode rootNode;
    if (!m_domDocument.GetRootNode(rootNode))
    {
        return FALSE;
    }
    CXMLNode node;
    if (!rootNode.GetNodeFromIdPath(strFullTargetId, node))
    {
        return FALSE;
    }
    if (!node.GetAttribute(CE_XMLATTR_TYPE, strType))
    {
        return FALSE;
    }
    if (!FindTargetType(strType))
    {
        return FALSE;
    }
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * GetAddressStringForTarget returns the address of the target 
 * for a given target id path.
 *
 * @param           strTargetId : the id path of the target
 * @param           strAddr : the address string of the target
 * @return          TRUE : on success
 *                  FALSE: error
 * @exception       -
 * @see             
*/
BOOL CTargetTree::GetAddressStringForTarget(const CString& strTargetId, CString& strAddr)
{
    CString strFullTargetId = strTargetId;

    if (strTargetId.Find(CE_XML_IDPATH_SEP_CHAR)==-1)
    {
        strFullTargetId = CE_XML_IDPATH_SEP + m_pProjectManager->GetProjectName()+_T(".") + 
            ID_PATH_TARGETVIEW + CE_XML_IDPATH_SEP + strTargetId;
    }

    CXMLNode rootNode;
    if (!m_domDocument.GetRootNode(rootNode))
    {
        return FALSE;
    }
    CXMLNode node;
    if (!rootNode.GetNodeFromIdPath(strFullTargetId, node))
    {
        return FALSE;
    }
    CString strType;
    if (!node.GetAttribute(CE_XMLATTR_TYPE, strType))
    {
        return FALSE;
    }
    if (!FindTargetType(strType))
    {
        return FALSE;
    }
   
    if (!node.GetAttribute(CE_XMLATTR_ADDR, strAddr))
    {
        return FALSE;
    }
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * GeUserAddressStringForTarget returns the user address string 
 * for a given target id path.
 *
 * @param           strTargetId : the id path of the target
 * @param           strUserAddr : the user address string of the target
 * @return          TRUE : on success
 *                  FALSE: error
 * @exception       -
 * @see             
*/
BOOL CTargetTree::GetUserAddressStringForTarget(const CString& strTargetId, CString& strUserAddr)
{
    CString strFullTargetId = strTargetId;

    if (strTargetId.Find(CE_XML_IDPATH_SEP_CHAR)==-1)
    {
        strFullTargetId = CE_XML_IDPATH_SEP + m_pProjectManager->GetProjectName()+_T(".") + 
            ID_PATH_TARGETVIEW + CE_XML_IDPATH_SEP + strTargetId;
    }

    CXMLNode rootNode;
    if (!m_domDocument.GetRootNode(rootNode))
    {
        return FALSE;
    }
    CXMLNode node;
    if (!rootNode.GetNodeFromIdPath(strFullTargetId, node))
    {
        return FALSE;
    }
    CString strType;
    if (!node.GetAttribute(CE_XMLATTR_TYPE, strType))
    {
        return FALSE;
    }
    if (!FindTargetType(strType))
    {
        return FALSE;
    }

    if (!node.GetAttribute(CE_XMLATTR_USERADDR, strUserAddr))
    {
        return FALSE;
    }
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * GetTargetNameForAddr tries to find a target with the specified
 * address and returns the id path for that target.
 *
 * @param           strAddr : the address of the target
 * @param           strTarget : the id path of the target
 * @return          TRUE : on success
 *                  FALSE: error
 * @exception       -
 * @see             
*/
BOOL CTargetTree::GetTargetNameForAddr(const CString& strAddr, CString& strTarget)
{
    CXMLNode rootNode;
    if (!m_domDocument.GetRootNode(rootNode))
    {
        return FALSE;
    }

    CXMLNodeIter nodeIter(rootNode);
    CXMLNode node;
    while(nodeIter.GetNext(node))
    {
        CString strNodeAddr;
        if (node.GetAttribute(CE_XMLATTR_ADDR, strNodeAddr))
        {
            if (strAddr.CompareNoCase(strNodeAddr)==0)
            {
                node.GetIdPath(strTarget);
                return TRUE;
            }
        }
    }

    return FALSE;
}


//------------------------------------------------------------------*
/**
 * GetResourceNameForTarget returns the name of the resource if any has
 * been assigned at the target.
 *
 * @param           strTargetId : the id path of the target
 * @param           strResName : the name of the resource
 * @return          TRUE : on success
 *                  FALSE: error
 * @exception       -
 * @see             
*/
BOOL CTargetTree::GetResourceNameForTarget(const CString& strTargetId, CString& strResName)
{
    CString strFullTargetId = strTargetId;

    if (strTargetId.Find(CE_XML_IDPATH_SEP_CHAR)==-1)
    {
        strFullTargetId = CE_XML_IDPATH_SEP + m_pProjectManager->GetProjectName()+_T(".") + 
            ID_PATH_TARGETVIEW + CE_XML_IDPATH_SEP + strTargetId;
    }

    CXMLNode rootNode;
    if (!m_domDocument.GetRootNode(rootNode))
    {
        return FALSE;
    }
    CXMLNode node;
    if (!rootNode.GetNodeFromIdPath(strFullTargetId, node))
    {
        return FALSE;
    }
    CString strType;
    if (!node.GetAttribute(CE_XMLATTR_TYPE, strType))
    {
        return FALSE;
    }
    if (!FindTargetType(strType))
    {
        return FALSE;
    }

    if (!node.GetAttribute(CE_XMLATTR_RESOURCE, strResName))
    {
        return FALSE;
    }
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * GetFreeChannelNrForPC returns a free channel nr under a pc node.
 * This function looks at all nodes under the given pc node and
 * if there is a CHANNEL_NR attribute it marks the given channel number as used.
 *
 * @param           strPCId : the id path of the target
 * @param           strChannelNr : a free channel nr
 * @return          TRUE : on success
 *                  FALSE: error
 * @exception       -
 * @see             
*/
BOOL CTargetTree::GetFreeChannelNrForPC(const CString& strPCId, long& lChannelNr)
{
    BOOL channelNr[1000];
    long i;
    for (i=0; i<1000; i++)
    {
        channelNr[i]=FALSE;
    }

    CString strFullTargetId = strPCId;

    if (strPCId.Find(CE_XML_IDPATH_SEP_CHAR)==-1)
    {
        strFullTargetId = CE_XML_IDPATH_SEP + m_pProjectManager->GetProjectName()+_T(".") + 
            ID_PATH_TARGETVIEW + CE_XML_IDPATH_SEP + strPCId;
    }

    CXMLNode rootNode;
    if (!m_domDocument.GetRootNode(rootNode))
    {
        return FALSE;
    }
    CXMLNode targetNode;
    if (!rootNode.GetNodeFromIdPath(strFullTargetId, targetNode))
    {
        return FALSE;
    }

    CXMLNodeIter nodeIter(targetNode);
    CXMLNode node;
    while(nodeIter.GetNext(node))
    {
        CString strChNr;
        if (node.GetAttribute(CE_XMLATTR_CHANNELNR, strChNr))
        {
            int iChNr = -1;
            iChNr = _ttoi(strChNr);
            channelNr[iChNr] = TRUE;
        }
    }

    for (i=0; i<1000; i++)
    {
        if (channelNr[i]==FALSE)
        {
            lChannelNr = i;
            return TRUE;
        }
    }

    return FALSE;
}





//------------------------------------------------------------------*
/**
 * GetSupportedDataTypes returns the supported data types of all targets
 *
 * @param           pparrDataTypes: the pointer to the safearray that will be created
 * @return          
 * @exception       -
 * @see             
*/
BOOL CTargetTree::GetSupportedDataTypes(SAFEARRAY** pparrDataTypes)
{
    HRESULT hr;
    SAFEARRAYBOUND Bound;
    Bound.lLbound = 0;
    Bound.cElements = m_listMaxSuppDataTypes.GetCount();

    *pparrDataTypes = ::SafeArrayCreate(VT_BSTR, 1, &Bound);
    if (NULL == (*pparrDataTypes)) 
    {
        return FALSE;    
    }

    long lIndex = 0;
    POSITION pos = m_listMaxSuppDataTypes.GetHeadPosition();
    CString str;

    while(pos!=NULL)
    {
        CString strType = m_listMaxSuppDataTypes.GetNext(pos);
        CComBSTR sType(strType);
        hr = ::SafeArrayPutElement(*pparrDataTypes, &lIndex, 
                                    (void*)sType);
        if (FAILED(hr))
        {
            ::SafeArrayDestroy(*pparrDataTypes);
            return FALSE;
        }
        lIndex++;
    }

    return TRUE;  
}

    


//------------------------------------------------------------------*
/**
 * GetSupportedDataTypesForTarget returns the supported data types of all targets
 *
 * @param           str´TargetType: the target type to get the information for
 * @param           pparrDataTypes: the pointer to the safearray that will be created
 * @return          
 * @exception       -
 * @see             
*/
BOOL CTargetTree::GetSupportedDataTypesForTarget(const CString& strTargetType, SAFEARRAY** pparrDataTypes)
{
    POSITION pos;
    pos = m_listTargetInfo.GetHeadPosition();

    while(pos)
    {
        TTargetInfo* pTargetInfo;
        pTargetInfo = m_listTargetInfo.GetNext(pos);
        if (strTargetType.Compare(pTargetInfo->strType) == 0)
        {
            // target info found, copy data types into safearray
            HRESULT hr;
            SAFEARRAYBOUND Bound;
            Bound.lLbound = 0;
            Bound.cElements = (pTargetInfo->listSuppDataTypes).GetCount();

            *pparrDataTypes = ::SafeArrayCreate(VT_BSTR, 1, &Bound);
            if (NULL == (*pparrDataTypes)) 
            {
                return FALSE;    
            }

            long lIndex = 0;
            POSITION pos = (pTargetInfo->listSuppDataTypes).GetHeadPosition();
            CString str;

            while(pos!=NULL)
            {
                CString strType = (pTargetInfo->listSuppDataTypes).GetNext(pos);
                CComBSTR sType(strType);
                hr = ::SafeArrayPutElement(*pparrDataTypes, &lIndex, 
                                            (void*)sType);
                if (FAILED(hr))
                {
                    ::SafeArrayDestroy(*pparrDataTypes);
                    return FALSE;
                }
                lIndex++;
            }

            return TRUE;  
        }
    }

    return FALSE;
}


//------------------------------------------------------------------*
/**
 * GetSupportedLanguages returns all supported IEC languages of the installed targets
 *
 * @param           pparrLanguages: the supported languages
 * @return          
 * @exception       -
 * @see             
*/
BOOL CTargetTree::GetSupportedLanguages(SAFEARRAY** pparrLanguages)
{
    HRESULT hr;
    SAFEARRAYBOUND Bound;
    Bound.lLbound = 0;
    Bound.cElements = m_listMaxSuppLanguages.GetCount();

    *pparrLanguages = ::SafeArrayCreate(VT_BSTR, 1, &Bound);
    if (NULL == (*pparrLanguages)) 
    {
        return FALSE;    
    }

    long lIndex = 0;
    POSITION pos = m_listMaxSuppLanguages.GetHeadPosition();
    CString str;

    while(pos!=NULL)
    {
        CString strType = m_listMaxSuppLanguages.GetNext(pos);
        CComBSTR sType(strType);
        hr = ::SafeArrayPutElement(*pparrLanguages, &lIndex, 
                                    (void*)sType);
        if (FAILED(hr))
        {
            ::SafeArrayDestroy(*pparrLanguages);
            return FALSE;
        }
        lIndex++;
    }

    return TRUE;  
}


//------------------------------------------------------------------*
/**
 * GetSupportedLanguagesForTarget returns all supported IEC languages of one target
 *
 * @param           strTargetType: the target type to get the information for
 * @param           pparrLanguages: the supported languages
 * @return          
 * @exception       -
 * @see             
*/
BOOL CTargetTree::GetSupportedLanguagesForTarget(const CString& strTargetType, SAFEARRAY** pparrLanguages)
{
    POSITION pos;
    pos = m_listTargetInfo.GetHeadPosition();

    while(pos)
    {
        TTargetInfo* pTargetInfo;
        pTargetInfo = m_listTargetInfo.GetNext(pos);
        if (strTargetType.Compare(pTargetInfo->strType) == 0)
        {
            // target info found, copy data types into safearray
            HRESULT hr;
            SAFEARRAYBOUND Bound;
            Bound.lLbound = 0;
            Bound.cElements = (pTargetInfo->listSuppLanguages).GetCount();

            *pparrLanguages = ::SafeArrayCreate(VT_BSTR, 1, &Bound);
            if (NULL == (*pparrLanguages)) 
            {
                return FALSE;    
            }

            long lIndex = 0;
            POSITION pos = (pTargetInfo->listSuppLanguages).GetHeadPosition();
            CString str;

            while(pos!=NULL)
            {
                CString strType = (pTargetInfo->listSuppLanguages).GetNext(pos);
                CComBSTR sType(strType);
                hr = ::SafeArrayPutElement(*pparrLanguages, &lIndex, 
                                            (void*)sType);
                if (FAILED(hr))
                {
                    ::SafeArrayDestroy(*pparrLanguages);
                    return FALSE;
                }
                lIndex++;
            }

            return TRUE;  
        }
    }

    return FALSE;
}



//------------------------------------------------------------------*
/**
 * GetSupportRetain returns TRUE if one of the installed targets supports 
 * the retain modifier
 *
 * @return          TRUE: RETAIN is supported
 *                  FALSE: RETAIN is not supported
 * @exception       -
 * @see             
*/
BOOL CTargetTree::GetSupportRetain()
{
    return m_bSuppRetain;
}

//------------------------------------------------------------------*
/**
 * GetSupportRetain returns TRUE if the given target supports 
 * the retain modifier
 *
 * @return          TRUE: RETAIN is supported
 *                  FALSE: RETAIN is not supported
 * @exception       -
 * @see             
*/
BOOL CTargetTree::GetSupportRetainForTarget(const CString& strTargetType)
{
    POSITION pos;
    pos = m_listTargetInfo.GetHeadPosition();

    while(pos)
    {
        TTargetInfo* pTargetInfo;
        pTargetInfo = m_listTargetInfo.GetNext(pos);
        if (strTargetType.Compare(pTargetInfo->strType) == 0)
        {
            return pTargetInfo->bSuppRetain;  
        }
    }

    return FALSE;
}



//------------------------------------------------------------------*
/**
 * GetExtForTarget returns the file extension for a target type
 *
 * @param           strTargetType: the target type
 * @param           strExt: the file extension
 * @return          
 * @exception       -
 * @see             
*/
BOOL CTargetTree::GetExtForTarget(const CString& strTargetType, CString& strExt)
{
    POSITION pos;
    pos = m_listTargetInfo.GetHeadPosition();

    while(pos)
    {
        TTargetInfo* pTargetInfo;
        pTargetInfo = m_listTargetInfo.GetNext(pos);
        if (strTargetType.Compare(pTargetInfo->strType) == 0)
        {
            strExt = pTargetInfo->strFileExt; 
            return TRUE;
        }
    }

    return FALSE;
}




//------------------------------------------------------------------*
/**
 * FindTargetType examines if a given string is a valid target type
 *
 * @param           strType: the target type to search
 * @return          
 * @exception       -
 * @see             
*/
BOOL CTargetTree::FindTargetType(const CString& strType)
{
    POSITION pos;
    pos = m_listTargetInfo.GetHeadPosition();

    while(pos)
    {
        TTargetInfo* pTargetInfo = m_listTargetInfo.GetNext(pos);
        CString strCompType = pTargetInfo->strType;
        if (strType.CompareNoCase(strCompType)==0)
        {
            return TRUE;
        }
    }

    return FALSE;
}


