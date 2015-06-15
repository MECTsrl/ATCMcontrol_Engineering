


#include "stdafx.h"
#include "resource.h"
#include "ClassTree.h"

#include "CEProjMan.h"
#include "CEProjectManager.h"
#include "CEKadMan.h"

#include "BuildNr.h"
#include "SBLLexTree.h"

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
CClassTree::CClassTree(CCEProjectManager *pProjMan) : CGenericTree(pProjMan)
{
    m_bMsgMissingFiles = TRUE;
    m_strFileName = CE_FILE_CLASSVIEW;
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
CClassTree::~CClassTree()
{

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
BOOL CClassTree::InitTree()
{
    GetStaticInfos();

    if (LoadViewData())
    {
        IXMLDOMDocument *pXMLDoc;
        m_domDocument.GetIXMLDocument(&pXMLDoc);
        PROJ_MAN_ASSERT(pXMLDoc);
        m_pProjectManager->Fire_InitClassTree((IUnknown*)pXMLDoc);
        if (pXMLDoc)
        {
            pXMLDoc->Release();
        }
        m_bInit = FALSE;
        // fill group xml nodes in class info
        CXMLNode projectNode;
        if (m_domDocument.GetRootNode(projectNode))
        {   
            POSITION pos = m_listClassInfo.GetHeadPosition();
            while (pos!=NULL)
            {
                TClassesInfo& sectionInfo = m_listClassInfo.GetAt(pos);
                CString strSectionName = sectionInfo.strSection;
                CXMLNode groupNode;

                if (projectNode.GetChildWithAttr(CE_XMLATTR_ID, strSectionName, groupNode))
                {
                    sectionInfo.groupNode = groupNode;
                }
                else 
                {
                    sectionInfo.groupNode = projectNode;
                }
                m_listClassInfo.GetNext(pos);
            }
        }

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
    strProjNodeId = m_pProjectManager->GetProjectName()+_T(".")+ID_PATH_CLASSVIEW;
    CString strVersion;
    strVersion.Format(_T("%i"), PRODUCT_BUILD);
    if (  (!m_domDocument.GetDocNode(docRoot))
        ||(!m_domDocument.CreateNode(projectNode, CE_XMLTAG_NODE))
        ||(!docRoot.AppendChild(projectNode))
        ||(!projectNode.SetAttribute(CE_XMLATTR_TEXT, m_pProjectManager->GetProjectName()))
        ||(!projectNode.SetAttribute(CE_XMLATTR_ID, strProjNodeId))
        ||(!projectNode.SetAttribute(CE_XMLATTR_SOURCE, m_pProjectManager->GetProjectFileName()))
        ||(!projectNode.SetAttribute(CE_XMLATTR_PARSERSOURCE, m_pProjectManager->GetProjectFileName()))
        ||(!projectNode.SetAttribute(CE_XMLATTR_TYPE, m_strClassesType))
        ||(!projectNode.SetAttribute(PRJMAN_VERSION, strVersion)) )
    {
        return FALSE;
    }

    Reparse4CPAttributes(&projectNode);

    // add project file to source file list
    CString strProjFileName = m_pProjectManager->GetProjectFileName();
    pProjFileInfo = CreateSourceFileInfo(strProjFileName,
                                         projectNode,  _T(""));
    PROJ_MAN_ASSERT(pProjFileInfo);
    pProjFileInfo->strIdPath = CE_XML_IDPATH_SEP + strProjNodeId;
    SetSourceTime(pProjFileInfo, m_pProjectManager->GetProjectFileNameWithPath());
    SourceFileMap_SetAt(strProjFileName, pProjFileInfo, m_mapSourceFiles);
//    m_pProjectManager->Fire_OnFileAdded((CComBSTR)strProjFileName, FV_FILE_SOURCE);
    
    // add groups of class view
    POSITION pos = m_listClassInfo.GetHeadPosition();
    while (pos!=NULL)
    {
        TClassesInfo& classInfo = m_listClassInfo.GetNext(pos);

        CXMLNode groupNode;

        if (classInfo.bVisible)
        {
            if (  (!m_domDocument.CreateNode(groupNode, CE_XMLTAG_NODE))
                ||(!projectNode.AppendChild(groupNode))  )
            {
                return FALSE;
            }

            groupNode.SetAttribute(CE_XMLATTR_TEXT, classInfo.strText);
            groupNode.SetAttribute(CE_XMLATTR_ID,   classInfo.strSection);
            groupNode.SetAttribute(CE_XMLATTR_TYPE, classInfo.strType);
        }
        else
        {
            groupNode = projectNode;
        }
        classInfo.groupNode = groupNode;   
    }
 
    ReparseProjFile();
    Reparse(FALSE);

    IXMLDOMDocument *pXMLDoc;
    m_domDocument.GetIXMLDocument(&pXMLDoc);
    m_pProjectManager->Fire_InitClassTree((IUnknown*)pXMLDoc);
    m_bInit = FALSE;
    pXMLDoc->Release();

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
BOOL CClassTree::ReparseProjFile()
{
    CList<CString, CString&> newSourceFileList;
    CString strSectionName;
    CString strIdPathProjNode;

    CXMLNode groupNode;
    CXMLNode projectNode;

    (m_pProjectManager->GetProjectFileHandler())->ReparseProjectFile();

    if (!m_domDocument.GetRootNode(projectNode))
    {
        return FALSE;
    }
    
    Reparse4CPAttributes(&projectNode);

    projectNode.RemoveAllChilds();

    // walk over all sections
    POSITION pos = m_listClassInfo.GetHeadPosition();
    while (pos!=NULL)
    {
        TClassesInfo sectionInfo = m_listClassInfo.GetAt(pos);
        strSectionName = sectionInfo.strSection;
        groupNode = sectionInfo.groupNode;

        if (sectionInfo.bVisible)
        {
            projectNode.AppendChild(groupNode);
            groupNode.RemoveAllChilds();
        }
        else
        {
            
        }

        CSBLLexTree<bool>   tFileNames;
        bool*               pbTmp;
        tFileNames.setModusOwner();

        CSection* pGroupSection = NULL;
        CString strFileName;

        BOOL ret = (m_pProjectManager->GetProjectFileHandler())->FindSection((CString)strSectionName, &pGroupSection);
        if (ret)
        {
            CKeyAndValue* pGroupFile = NULL;
            ret = pGroupSection->GetFirstKeyAndValue(&pGroupFile);
            while (ret)
            {
                if (pGroupFile->GetKey().CompareNoCase(CE_CP_FILE)!=0)
                {
                    ret = pGroupSection->GetNextKeyAndValue(&pGroupFile);
                    continue;
                }

                strFileName = pGroupFile->GetValue();
                pbTmp = new bool;
                tFileNames.insertData(strFileName, pbTmp);
                ret = pGroupSection->GetNextKeyAndValue(&pGroupFile);
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
                groupNode.AppendChild(pFileInfo->xmlNode);                        
                // check parents
                CString strTag;
                if (pFileInfo->xmlNode. GetTagName(strTag) && strTag.Compare(CE_XMLTAG_NODE)==0)
                {
                    CString strOldId;
                    CString strNewId;
                    strOldId = pFileInfo->strIdPath;
                    if (pFileInfo->xmlNode.GetIdPath(strNewId) &&
                        strOldId.CompareNoCase(strNewId)!=0)
                    {
                        FireUpdateTree(strOldId, eUpdateDelete);
                        FireUpdateTree(strNewId, eUpdateInsert);
                        pFileInfo->strIdPath = strNewId;
                    }
                }
            }
            else
            {
                CXMLNode fileNode;

                if (m_domDocument.CreateNode(fileNode, CE_XMLTAG_LINK)
                    && groupNode.AppendChild(fileNode))
                {
                    fileNode.SetAttribute(CE_XMLATTR_PARSERSOURCE, strFileName);

                    pFileInfo = CreateSourceFileInfo(strFileName, fileNode,  
                                                     m_pProjectManager->GetProjectFileName());
                    SourceFileMap_SetAt(strFileName, pFileInfo, m_mapSourceFiles);
                    if(!m_bInit)
                    {
                        m_pProjectManager->Fire_OnFileAdded((CComBSTR)strFileName, FV_FILE_SOURCE);
                    }
                    AddSourceFileToReparse(strFileName);
                    m_bFileViewChanged = TRUE;
                    ATLTRACE("Add source file (reparse cp file class view): %s\n", strFileName);
                }
            }
        }

       
        m_listClassInfo.GetNext(pos);
    }

    // check old children of project file and new child list, remove files if no longer used
    TSourceFileInfo* pPFInfo;
    SourceFileMap_Lookup(m_pProjectManager->GetProjectFileName(), pPFInfo, m_mapSourceFiles);

    PROJ_MAN_ASSERT(pPFInfo);

    pos = (pPFInfo->strlChildFileNames).GetHeadPosition();
    CString childFileName;
    while (pos!=NULL)
    {
        childFileName = (pPFInfo->strlChildFileNames).GetAt(pos);
        if (!StringList_Find(childFileName, newSourceFileList))
        {
            // if file has been removed add parent to changed node list
            TSourceFileInfo* pRemoveFileInfo = NULL;
            if (SourceFileMap_Lookup(childFileName, pRemoveFileInfo, m_mapSourceFiles))
            {
                FireUpdateTree(pRemoveFileInfo->strIdPath, eUpdateDelete);
                /*CString strChangedPath;
                if (GetParentIdPath(pRemoveFileInfo->strIdPath, strChangedPath))
                {
                    AddChangedNode(strChangedPath);
                }*/
            }
            // and remove the node with all its children
            RemoveSourceFileAndChilds(childFileName);
            m_bFileViewChanged = TRUE;
        }

        (pPFInfo->strlChildFileNames).GetNext(pos);
    }   

    (pPFInfo->strlChildFileNames).RemoveAll();
    (pPFInfo->strlChildFileNames).AddTail(&newSourceFileList);

    // change project file info parse date
    ATLTRACE("Setting project file time\n");
    SetSourceTime(pPFInfo, m_pProjectManager->GetProjectFileNameWithPath());




    return TRUE;
}










//------------------------------------------------------------------*
/**
 * GetStaticInfo obtains information from the kad that is needed 
 * at different places.
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
BOOL CClassTree::GetStaticInfos()
{
    HRESULT hr;
    CComQIPtr<IClasses, &IID_IClasses> kadClasses;
    
    kadClasses = m_pProjectManager->GetKadManager();
    if (!kadClasses)
    {
        return FALSE;
    }

    CComBSTR sNodeType;
    kadClasses->GetClassesTypeString(&sNodeType);
    m_strClassesType = (CString)sNodeType;

    // add groups of class view
    CComPtr<IClass> classGroup;
    CComPtr<IEnumClasses> classesEnum;
    
    kadClasses->GetClassEnumerator(&classesEnum);
    if (!classesEnum)
    {
        return FALSE;
    }

    classesEnum->Reset();

    ULONG n;
    hr = classesEnum->Next(1, &classGroup, &n);
    if (!SUCCEEDED(hr))
    {
        return FALSE;
    }

    while (classGroup)
    {
        CComBSTR sAttrText;
        CComBSTR sAttrId;
        CComBSTR sAttrType;
        CComBSTR sAttrClassesType;
        BOOL     bVisible;
        IType*   pType;

        classGroup->GetGroupText(&sAttrText);
        classGroup->GetTypeString(&sAttrType);
        classGroup->GetSection(&sAttrId);
        classGroup->GetClassType(&sAttrClassesType);
        hr = classGroup->GetType(&pType);
        if (hr==S_OK)
        {
            hr = pType->GetVisibilityFlag(&bVisible);
            if (hr!=S_OK)
            {
                bVisible = TRUE;
            }
            pType->Release();
        }
                
        TClassesInfo cInfo;
        cInfo.strText = (CString)sAttrText;
        cInfo.strType = (CString)sAttrType;
        cInfo.strSection = (CString)sAttrId;
        cInfo.strClassesType = (CString)sAttrClassesType;
        cInfo.bVisible = bVisible;
        
        m_listClassInfo.AddTail(cInfo);   

        // get next class group
        classGroup.Release();
        hr = classesEnum->Next(1, &classGroup, &n);
    }
    
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * FireUpdateTree informs all client that some nodes in the class
 * tree have changed
 *
 * @exception       -
 * @see             
*/
void CClassTree::FireUpdateTree(const CString& strChangedNode, enum E_UPDATE_MODE eMode)
{
    if (!m_bInit)
    {
        CComBSTR sChangedNode = strChangedNode;
        m_pProjectManager->Fire_UpdateClassTree(sChangedNode, eMode);
    }
}


//------------------------------------------------------------------*
/**
 * Determines if the IDPath contains to a node in the class tree
 * this function checks only the beginning of the path, if it starts
 * with 'projectname'.CLASSVIEW it is a class tree path,
 * this function does not check if the node for the full path
 * does exist.
 *
 * @param     strIdPath : the id path of the node      
 * @return    TRUE : it is a class tree path
 *            FALSE: it is no class tree path
 * @exception       -
 * @see             
*/
BOOL CClassTree::IsViewPath(const CString& strIdPath)
{
    CString strProjNodePart;
    CString strFullPath = strIdPath;
    
    // it should be a root path, starting with / -> strProjNodePart == ""
    if (!StripIdFromIdPath(strFullPath, strProjNodePart))
    {
        return FALSE;
    }
    // now get the project node id
    if (!StripIdFromIdPath(strFullPath, strProjNodePart))
    {
        return FALSE;
    }

    CString strExpId = m_pProjectManager->GetProjectName()+_T(".")+ID_PATH_CLASSVIEW;
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
 * if the new file should be inserted in the class view tree.
 *
 * @param           strLocation: the location of the new file in the class view
 * @param           strLinkName: the name of the new file
 * @return          TRUE : if the operation has been successful
 *                  FALSE: else
 * @exception       -
 * @see             
*/
BOOL CClassTree::CreateLink(const CString& strLocation, 
                            const CString& strLinkName)
{
    CString strGroupId;
    CString strFullPath = strLocation;

    // it should be a root path, starting with / -> strProjNodePart == ""
    if (!StripIdFromIdPath(strFullPath, strGroupId))
    {
        return FALSE;
    }
    // now project node id
    if (!StripIdFromIdPath(strFullPath, strGroupId))
    {
        return FALSE;
    }
    // and then class group id
    if (!StripIdFromIdPath(strFullPath, strGroupId))
    {
        return FALSE;
    }

    // Is it a class tree section?
    CString strClassSection;
    BOOL bFound=FALSE;
    POSITION pos = m_listClassInfo.GetHeadPosition();
    while (pos!=NULL)
    {
        strClassSection = m_listClassInfo.GetNext(pos).strSection;
        if (strClassSection.CompareNoCase(strGroupId)==0)
        {
            bFound = TRUE;
            break;
        }
    }

    if (!bFound)
    {
        return FALSE;
    }

    PROJ_MAN_ASSERT(m_pProjectManager);
    PROJ_MAN_ASSERT(m_pProjectManager->GetProjectFileHandler());
    m_pProjectManager->GetProjectFileHandler()->ReparseProjectFile();
    CSection* pSection=NULL;
    if (!m_pProjectManager->GetProjectFileHandler()->FindSection(strGroupId, &pSection))
    {
        // create section
        if (!m_pProjectManager->GetProjectFileHandler()->AddSection(strGroupId))
        {
            return FALSE;
        }
        if (!m_pProjectManager->GetProjectFileHandler()->FindSection(strGroupId, &pSection))
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
 * if a file the class view tree should be renamed.
 *
 * @param           strLocation: the location of the new file in the class view
 * @param           strOldLinkName: the name of the old file
 * @param           strNewLinkName: the name of the new file
 * @return          TRUE : if the operation has been successful
 *                  FALSE: else
 * @exception       -
 * @see             
*/
BOOL CClassTree::RenameLink(const CString& strLocation, 
                            const CString& strOldLinkName,
                            const CString& strNewLinkName)
{
    CString strGroupId;
    CString strFullPath = strLocation;

    // it should be a root path, starting with / -> strProjNodePart == ""
    if (!StripIdFromIdPath(strFullPath, strGroupId))
    {
        return FALSE;
    }
    // now project node id
    if (!StripIdFromIdPath(strFullPath, strGroupId))
    {
        return FALSE;
    }
    // and then class group id
    if (!StripIdFromIdPath(strFullPath, strGroupId))
    {
        return FALSE;
    }

    // Is it a class tree section?
    CString strClassSection;
    BOOL bFound=FALSE;
    POSITION pos = m_listClassInfo.GetHeadPosition();
    while (pos!=NULL)
    {
        strClassSection = m_listClassInfo.GetNext(pos).strSection;
        if (strClassSection.CompareNoCase(strGroupId)==0)
        {
            bFound = TRUE;
            break;
        }
    }

    if (!bFound)
    {
        return FALSE;
    }

    PROJ_MAN_ASSERT(m_pProjectManager);
    PROJ_MAN_ASSERT(m_pProjectManager->GetProjectFileHandler());
    m_pProjectManager->GetProjectFileHandler()->ReparseProjectFile();
    CSection* pSection;
    if (!m_pProjectManager->GetProjectFileHandler()->FindSection(strGroupId, &pSection))
    {
        return FALSE;
    }
    // is file=oldLinkName  in this section?
    CKeyAndValue* pKeyAndValue;
    if (!pSection->FindKeyAndValue(CE_CP_FILE, strOldLinkName, &pKeyAndValue))
    {
        // can't rename it
        return FALSE;
    }
    pKeyAndValue->SetValue(strNewLinkName);
   
    m_pProjectManager->GetProjectFileHandler()->WriteProjectFile();
    m_bProjFileToReparse = TRUE;
    Reparse();

    return TRUE;
}


//------------------------------------------------------------------*
/**
 * This function fills the container interface for the project file,
 * if a file in the class view tree should be deleted.
 *
 * @param           strLocation: the location of the new file in the class view
 * @param           strLinkName: the name of the file to delete
 * @return          TRUE : if the operation has been successful
 *                  FALSE: else
 * @exception       -
 * @see             
*/
BOOL CClassTree::DeleteLink(const CString& strLocation,
                            const CString& strLinkName)
{
    CString strGroupId;
    CString strFullPath = strLocation;

    // it should be a root path, starting with / -> strProjNodePart == ""
    if (!StripIdFromIdPath(strFullPath, strGroupId))
    {
        return FALSE;
    }
    // now project node id
    if (!StripIdFromIdPath(strFullPath, strGroupId))
    {
        return FALSE;
    }
    // and then class group id
    if (!StripIdFromIdPath(strFullPath, strGroupId))
    {
        return FALSE;
    }

    // Is it a class tree section?
    CString strClassSection;
    BOOL bFound=FALSE;
    POSITION pos = m_listClassInfo.GetHeadPosition();
    while (pos!=NULL)
    {
        strClassSection = m_listClassInfo.GetNext(pos).strSection;
        if (strClassSection.CompareNoCase(strGroupId)==0)
        {
            bFound = TRUE;
            break;
        }
    }

    if (!bFound)
    {
        return FALSE;
    }

    PROJ_MAN_ASSERT(m_pProjectManager);
    PROJ_MAN_ASSERT(m_pProjectManager->GetProjectFileHandler());
    m_pProjectManager->GetProjectFileHandler()->ReparseProjectFile();
    CSection* pSection;
    if (!m_pProjectManager->GetProjectFileHandler()->FindSection(strGroupId, &pSection))
    {
        return FALSE;
    }
    // is file=LinkName  in this section?
    CKeyAndValue* pKeyAndValue;
    if (!pSection->FindKeyAndValue(CE_CP_FILE, strLinkName, &pKeyAndValue))
    {
        // can't remove it
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
 * GetAllClassTypes
 * gets all class types
 *
 * @param           pparrClassType:  Safearray that will be filled
 *                                   with all class types
 * @return          
 * @exception       -
 * @see             
*/
BOOL CClassTree::GetAllClassTypes(SAFEARRAY** pparrClassTypes)
{
    HRESULT hr;
    SAFEARRAYBOUND Bound;
    Bound.lLbound = 0;
    Bound.cElements = m_listClassInfo.GetCount();

    *pparrClassTypes = ::SafeArrayCreate(VT_BSTR, 1, &Bound);
    if (NULL == (*pparrClassTypes)) 
    {
        return FALSE;    
    }

    long lIndex = 0;
    POSITION pos = m_listClassInfo.GetHeadPosition();
    CString str;

    while(pos!=NULL)
    {
        TClassesInfo classInfo;
        classInfo = m_listClassInfo.GetNext(pos);
        CString strType = classInfo.strClassesType;
        CComBSTR sType(strType);
        hr = ::SafeArrayPutElement(*pparrClassTypes, &lIndex, 
                                    (void*)sType);
        if (FAILED(hr))
        {
            ::SafeArrayDestroy(*pparrClassTypes);
            return FALSE;
        }
        lIndex++;
    }

    return TRUE;    
}


//------------------------------------------------------------------*
/**
 * GetTextForClassType
 * get text for a class type (id)
 *
 * @param           strClassType: the class type id
 * @param           strText: the text that cna be displayed for that class type
 * @return          
 * @exception       -
 * @see             
*/
BOOL CClassTree::GetTextForClassType(const CString& strClassType, CString& strText)
{
    POSITION pos = m_listClassInfo.GetHeadPosition();
    CString str;
    while(pos!=NULL)
    {
        TClassesInfo classInfo;
        classInfo = m_listClassInfo.GetNext(pos);
        CString strType = classInfo.strClassesType;
        if (strType==strClassType)
        {
            strText = classInfo.strText;
            return TRUE;
        }
    }
    return FALSE;
}



//------------------------------------------------------------------*
/**
 * Retrieves all class objects in the class view, that means all objects 
 * in the first hirarchy under the class groups, the function can return
 * the id paths or the file names of the classes
 *
 * @param           pparrClasses: the out parameter filled with the file
 *                                names or the id paths
 * @param           bIdPaths: if TRUE: the out parameter is filled with 
 *                            the id paths else with the file names
 * @return          TRUE: on success
 *                  FALSE: on error
 * @exception       -
 * @see             
*/
BOOL CClassTree::GetAllClasses(SAFEARRAY** pparrClasses, BOOL bIdPaths)
{
    CList<CString, CString&> nameList;

    POSITION pos = m_listClassInfo.GetHeadPosition();

    while (pos)
    {
        TClassesInfo classInfo;
        classInfo = m_listClassInfo.GetNext(pos);
        CString strClassSection = classInfo.strSection;
        CSection* pSection;
        if (!m_pProjectManager->GetProjectFileHandler()->FindSection(strClassSection, &pSection))
        {
            continue;
        }
        CString strFileName;
        CKeyAndValue* pKeyAndValue;
        BOOL bFound = pSection->GetFirstKeyAndValue(&pKeyAndValue);
        while (bFound)
        {
            if (pKeyAndValue->GetKey().CompareNoCase(CE_CP_FILE)==0)
            {
                CString strFileName;
                strFileName = pKeyAndValue->GetValue();
                if (!bIdPaths)
                {
                    nameList.AddTail(strFileName);
                }
                else
                {
                    TSourceFileInfo* pSourceInfo;
                    if (SourceFileMap_Lookup(strFileName, pSourceInfo, m_mapSourceFiles))
                    {
                        nameList.AddTail(pSourceInfo->strIdPath);
                    }
                }
            }
            bFound = pSection->GetNextKeyAndValue(&pKeyAndValue);
        }
    }

    HRESULT hr;
    SAFEARRAYBOUND Bound;
    Bound.lLbound = 0;
    Bound.cElements = nameList.GetCount();

    *pparrClasses = ::SafeArrayCreate(VT_BSTR, 1, &Bound);
    if (NULL == (*pparrClasses)) 
    {
        return FALSE;    
    }

    long lIndex = 0;
    pos = nameList.GetHeadPosition();
    CString str;

    while(pos!=NULL)
    {
        CString strClass = nameList.GetNext(pos);
        CComBSTR sClass(strClass);
        hr = ::SafeArrayPutElement(*pparrClasses, &lIndex, 
                                    (void*)sClass);
        if (FAILED(hr))
        {
            ::SafeArrayDestroy(*pparrClasses);
            return FALSE;
        }
        lIndex++;
    }

    return TRUE;
}


//------------------------------------------------------------------*
/**
 * Retrieves all class objects of a special class type.
 *
 * @param           strClass: the name of the classes to get
 * @param           pparrClasses: the out parameter filled with the file
 *                                names or the id paths
 * @param           bIdPaths: if TRUE: the out parameter is filled with 
 *                            the id paths else with the file names
 * @return          TRUE: on success
 *                  FALSE: on error * @exception       -
 * @see             
*/
BOOL CClassTree::GetAllClassesOfType(const CString& strClass, 
                                     SAFEARRAY** pparrClasses, 
                                     BOOL bIdPaths)
{
    CList<CString, CString&> nameList;
    BOOL bIs4CPSection = FALSE;

    POSITION pos = m_listClassInfo.GetHeadPosition();

    while (pos)
    {
        TClassesInfo classInfo;
        classInfo = m_listClassInfo.GetNext(pos);
        if (classInfo.strClassesType.Compare(strClass)==0)
        {
            bIs4CPSection = TRUE;
            CString strClassSection = classInfo.strSection;
            CSection* pSection;
            if (!m_pProjectManager->GetProjectFileHandler()->FindSection(strClassSection, &pSection))
            {
                continue;
            }
            CString strFileName;
            CKeyAndValue* pKeyAndValue;
            BOOL bFound = pSection->GetFirstKeyAndValue(&pKeyAndValue);
            while (bFound)
            {
                if (pKeyAndValue->GetKey().CompareNoCase(CE_CP_FILE)==0)
                {
                    CString strFileName;
                    strFileName = pKeyAndValue->GetValue();
                    if (!bIdPaths)
                    {
                        nameList.AddTail(strFileName);
                    }
                    else
                    {
                        TSourceFileInfo* pSourceInfo;
                        if (SourceFileMap_Lookup(strFileName, pSourceInfo, m_mapSourceFiles))
                        {
                            nameList.AddTail(pSourceInfo->strIdPath);
                        }
                    }
                }
                bFound = pSection->GetNextKeyAndValue(&pKeyAndValue);
            }
            break;
        }
    }

    if (!bIs4CPSection)
    {
        // look at all nodes to find nodes of type strClass
        CXMLNode rootNode;
        if (!m_domDocument.GetRootNode(rootNode))
        {
            return FALSE;
        }

        CXMLNodeIter nodeIter(rootNode);
        CXMLNode node;
        while(nodeIter.GetNext(node))
        {
            CString strType;
            if (node.GetAttribute(CE_XMLATTR_TYPE, strType))
            {
                if (strType.Compare(strClass)==0)
                {
                    if (bIdPaths)
                    {
                        CString strIdPath;
                        if (node.GetIdPath(strIdPath))
                        {
                            nameList.AddTail(strIdPath);
                        }
                    }
                    else
                    {
                        CString strFileName;
              
                        if (node.GetAttribute(CE_XMLATTR_PARSERSOURCE, strFileName))
                        {
                            nameList.AddTail(strFileName);
                        }
                    }
                }
            }
        }
    }

    HRESULT hr;
    SAFEARRAYBOUND Bound;
    Bound.lLbound = 0;
    Bound.cElements = nameList.GetCount();

    *pparrClasses = ::SafeArrayCreate(VT_BSTR, 1, &Bound);
    if (NULL == (*pparrClasses)) 
    {
        return FALSE;    
    }

    long lIndex = 0;
    pos = nameList.GetHeadPosition();
    CString str;

    while(pos!=NULL)
    {
        CString strClass = nameList.GetNext(pos);
        CComBSTR sClass(strClass);
        hr = ::SafeArrayPutElement(*pparrClasses, &lIndex, 
                                    (void*)sClass);
        if (FAILED(hr))
        {
            ::SafeArrayDestroy(*pparrClasses);
            return FALSE;
        }
        lIndex++;
    }

    return TRUE;
}


//------------------------------------------------------------------*
/**
 * Try to add a new file to class view.
 *
 * @param           strFileName: the file name
 * @param           bTry: only try but don't do it (default FALSE)
 * @return          TRUE: file has been added (in try mode: can be added)
 *                  FALSE: file could not be added (in try mode: cannot be added)
 * @exception       -
 * @see             
*/
BOOL CClassTree::FVAddFile(const CString& strFileName, const CString& strBackparsed, BOOL bTry /*=FALSE*/)
{
    // do not remove 4cp and con files
    CString strExt = CFileHelper::GetFileExt(strFileName);
    if(strExt.CompareNoCase(_T("4cp")) == 0 || strExt.CompareNoCase(_T("con")) == 0)
    {
        return FALSE;
    }


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
    CString strId;

    if (!newNode.GetAttribute(CE_XMLATTR_TYPE, strType))
    {
        return FALSE;
    }

    if(!newNode.GetAttribute(CE_XMLATTR_ID, strId))
    {
        return FALSE;
    }

    POSITION pos = m_listClassInfo.GetHeadPosition();
    BOOL bFound = FALSE;
    CString strClassSection;

    while(pos!=NULL)
    {
        TClassesInfo classInfo;
        classInfo = m_listClassInfo.GetNext(pos);
        if (classInfo.strClassesType==strType)
        {
            bFound = TRUE;
            strClassSection = classInfo.strSection;
            break;
        }
    }

    if (!bFound)
    {
        return FALSE;
    }

    if(IsIdExisting(strId, strClassSection))
    {
        return FALSE;
    }

    CSection* pSection;
    if (!m_pProjectManager->GetProjectFileHandler()->FindSection(strClassSection, &pSection))
    {
        if (!m_pProjectManager->GetProjectFileHandler()->AddSection(strClassSection))
        {
            return FALSE;
        }
        if (!m_pProjectManager->GetProjectFileHandler()->FindSection(strClassSection, &pSection))
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
 * Try to remove a file from class view.
 *
 * @param           strFileANme: the file name
 * @param           bTry: only try but don't do it (default FALSE)
 * @return          TRUE: file has been removed (try mode: can be removed)
 *                  FALSE: fiel could not be removed (try mode: cannot be removed)
 * @exception       -
 * @see             
*/
BOOL CClassTree::FVRemoveFile(const CString& strFileName, BOOL bTry /*=FALSE*/)
{
    CString strType;
    TSourceFileInfo* pSourceInfo;
    if (!SourceFileMap_Lookup(strFileName, pSourceInfo, m_mapSourceFiles))
    {
        return FALSE;
    }

    CXMLNode node = pSourceInfo->xmlNode;
    if (!node.GetAttribute(CE_XMLATTR_TYPE, strType))
    {
        return FALSE;
    }

    POSITION pos = m_listClassInfo.GetHeadPosition();
    BOOL bFound = FALSE;
    CString strClassSection;

    while(pos!=NULL)
    {
        TClassesInfo classInfo;
        classInfo = m_listClassInfo.GetNext(pos);
        if (classInfo.strClassesType==strType)
        {
            bFound = TRUE;
            strClassSection = classInfo.strSection;
            break;
        }
    }

    if (!bFound)
    {
        return FALSE;
    }

    CSection* pSection;
    if (!m_pProjectManager->GetProjectFileHandler()->FindSection(strClassSection, &pSection))
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
 * is id existing.
 *
 * @param           crstrId: new id
 * @param           crstrParentId: id of parent node
 * @return          is id existing?
 * @exception       -
 * @see             -
*/
BOOL CClassTree::IsIdExisting(const CString& crstrId, const CString& crstrParentId)
{
    CXMLNode    tRootNode;
    CXMLNode    tNode;
    CString     strIdPath;

    strIdPath.Format("/%s.%s/%s/%s", m_pProjectManager->GetProjectName(), ID_PATH_CLASSVIEW, crstrParentId, crstrId);
    if(!m_domDocument.GetRootNode(tRootNode))
    {
        return FALSE;
    }

    if(tRootNode.GetNodeFromIdPath(strIdPath, tNode))
    {
        return TRUE;
    }
    return FALSE;
}



void CClassTree::ParentCheck(const CXMLNode& parentNode, const CXMLNode& newNode)
{
    CString strIsSectionId;
    CString strIsSection4CP;
    CString strIsNodeId;
    CString strCorrSectionId;
    CString strCorrSection4CP;
    CString strSourceFile;

    if (!newNode.GetAttribute(CE_XMLATTR_TYPE, strIsNodeId))
    {
        return;
    }

    if (!newNode.GetAttribute(CE_XMLATTR_PARSERSOURCE, strSourceFile))
    {
        return;
    }

    if (!parentNode.GetAttribute(CE_XMLATTR_TYPE, strIsSectionId))
    {
        return;
    }

    POSITION pos = m_listClassInfo.GetHeadPosition();

    while (pos)
    {
        TClassesInfo classInfo = m_listClassInfo.GetNext(pos);

        if (strIsNodeId.CompareNoCase(classInfo.strClassesType)==0)
        {
            strCorrSectionId = classInfo.strType;
            strCorrSection4CP = classInfo.strSection;
        }

        if (strIsSectionId.CompareNoCase(classInfo.strType)==0)
        {
            strIsSection4CP = classInfo.strSection;
        }
    }

    if (!strIsSection4CP.IsEmpty() &&
        !strCorrSectionId.IsEmpty() &&
        !strCorrSection4CP.IsEmpty() &&
        strCorrSectionId.CompareNoCase(strIsSectionId)!=0)
    {
        CComBSTR sIsSection(strIsSection4CP);
        CComBSTR sKey(_T("file"));
        CComBSTR sFileName(strSourceFile);
        m_pProjectManager->CPDeleteKeyWithValueFromSection(sIsSection, sKey, sFileName);
        CComBSTR sCorrSection(strCorrSection4CP);
        m_pProjectManager->CPInsertToSection(sCorrSection, sKey, sFileName);
        m_bProjFileToReparse = TRUE;
    }



    return;
}
