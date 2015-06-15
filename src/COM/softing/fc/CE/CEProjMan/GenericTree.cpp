



#include "stdafx.h"
#include "resource.h"
#include "GenericTree.h"

#include "CEProjMan.h"
#include "CEProjectManager.h"
#include "CEBackparse.h"

#include "fc_tools\fc_fileIO.h"

#include "BuildNr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//------------------------------------------------------------------*
/**
 * constructor
 *
 * @param           pProjMan:  a pointer to the ProjectManager class
 *                             to get additional information of the project
 * @exception       -
 * @see             
*/
CGenericTree::CGenericTree(CCEProjectManager* pProjMan)
{
    m_pProjectManager = pProjMan;
    m_bProjFileToReparse = FALSE;
    m_bFileViewChanged = FALSE;
    m_bDeleteSem = TRUE;
    m_posNextPosToCheck = NULL;
    CFileHelper::pProjMan = pProjMan;
    m_bInit = TRUE;
}


//------------------------------------------------------------------*
/**
 * destructor
 *
 * @exception       -
 * @see             
*/
CGenericTree::~CGenericTree()
{   
    POSITION pos;
    pos = m_mapSourceFiles.GetStartPosition();
    while(pos!=NULL)
    {
        TSourceFileInfo *finfo=NULL;
        CString strKey;
        m_mapSourceFiles.GetNextAssoc(pos, strKey, finfo);
        PROJ_MAN_ASSERT(finfo);
        delete finfo;
        m_mapSourceFiles.RemoveKey(strKey);
    }
    m_mapSourceFiles.RemoveAll();   
}


//------------------------------------------------------------------*
/**
 * CreateSourceFileInfo creates and fills a new TSourceFileInfo
 *
 * @param           [in] strFileName: the name of the source file
 * @param           [in] xmlNode:   the class that holds the pointer 
 *                                  to the xml node
 * @param           [in] strXMLString: the xml representation that the 
 *                                     backparser returned for that source file
 * @param           [in] strParentFileName: the file name of the parent file
 *                                          where this file has been linked
 * @return          a pointer to the newly created source file info
 * @exception       -
 * @see             
*/
TSourceFileInfo* CGenericTree::CreateSourceFileInfo(const CString& strFileName,
                                                    const CXMLNode& xmlNode,
                                                    const CString& strParentFileName)
{
    TSourceFileInfo* pFileInfo = new TSourceFileInfo();
    PROJ_MAN_ASSERT(pFileInfo);

    pFileInfo->strFileName       = strFileName;
    pFileInfo->strParentFileName = strParentFileName;
    pFileInfo->xmlNode           = xmlNode;
    pFileInfo->bFileMissing      = FALSE;
    pFileInfo->strIdPath         = _T("");
    pFileInfo->parseSourceTime   = 0;

    return pFileInfo;
}


//------------------------------------------------------------------*
/**
 * SetSourceTime sets the modified time info in a source file info struct
 *
 * @param           [in] pFileInfo: this is the pointer to the file
 *                                  info for the source file
 * @param           [in] strFile: the name of the source file
 * @return          
 * @exception       -
 * @see             
*/
BOOL CGenericTree::SetSourceTime(TSourceFileInfo* pFileInfo, const CString& strFile)
{
    PROJ_MAN_ASSERT(pFileInfo);
    CFileStatus fileStatus;
    CString strFullFileName = CFileHelper::GetFileAbsoluteReplaced(strFile);
    if (CFile::GetStatus(strFullFileName, fileStatus))
    {
        pFileInfo->parseSourceTime = fileStatus.m_mtime;
    }
    else
    {
        pFileInfo->parseSourceTime = 0;
    }

    SetModTimeInfo(pFileInfo->xmlNode, pFileInfo->parseSourceTime);
    
    
    return TRUE;
}



//------------------------------------------------------------------*
/**
 * RemoveSourceFileAndChilds removes a source file from the source file map
 * it also finds all children of that source file and removes them
 * too, these files are no longer needed if the parent file has been
 * removed
 *
 * @param           [in] strFileName: the name of the file to remove
 * @return          TRUE : on success
 *                  FALSE: error
 * @exception       -
 * @see             
*/
BOOL CGenericTree::RemoveSourceFileAndChilds(const CString& strFileName)
{
    TSourceFileInfo* fInfo = NULL;
 
    SourceFileMap_Lookup(strFileName, fInfo, m_mapSourceFiles);

    if (!fInfo)
    {
        return FALSE;
    }

    // remove all children and then that file itself
    POSITION pos = (fInfo->strlChildFileNames).GetHeadPosition();
    CString childFileName;
    while (pos!=NULL)
    {
        childFileName = (fInfo->strlChildFileNames).GetAt(pos);
        RemoveSourceFileAndChilds(childFileName);
        (fInfo->strlChildFileNames).GetNext(pos);
    }      

    delete fInfo;
   
    SourceFileMap_RemoveKey(strFileName, m_mapSourceFiles);
    ATLTRACE("Removing source file: %s\n", strFileName);
    m_pProjectManager->Fire_OnFileRemoved((CComBSTR)strFileName, FV_FILE_SOURCE);

    return TRUE;
}




//------------------------------------------------------------------*
/**
 * Replace the node with an error node, because file has not been found,
 * or backparser returned non xml
 *
 * @param           pFileInfo: pointer to the source file infromation
 * @return          
 * @exception       -
 * @see             
*/
BOOL CGenericTree::BuildErrorNode(TSourceFileInfo* pFileInfo)
{
    // remove all child nodes    
    POSITION pos = (pFileInfo->strlChildFileNames).GetHeadPosition();
    CString childFileName;
    while (pos!=NULL)
    {
        childFileName = (pFileInfo->strlChildFileNames).GetAt(pos);
        RemoveSourceFileAndChilds(childFileName);
        
        (pFileInfo->strlChildFileNames).GetNext(pos);
    }   

    (pFileInfo->strlChildFileNames).RemoveAll();

    CString strOldIdPath = pFileInfo->strIdPath;

    CXMLNode node;
    m_domDocument.CreateNode(node, CE_XMLTAG_NODE);
    node.SetAttribute(CE_XMLATTR_TEXT, pFileInfo->strFileName);
    node.SetAttribute(CE_XMLATTR_ID, pFileInfo->strFileName);
    node.SetAttribute(CE_XMLATTR_PARSERSOURCE, pFileInfo->strFileName);
    node.SetAttribute(CE_XMLATTR_SOURCE, pFileInfo->strFileName);
    node.SetAttribute(CE_XMLATTR_TYPE, CE_XMLTYPE_ERRORNODE);

    CXMLNode parentNode;
    (pFileInfo->xmlNode).GetParentNode(parentNode);
       
    parentNode.ReplaceChild(node, pFileInfo->xmlNode);
    pFileInfo->xmlNode = node;
    node.GetIdPath(pFileInfo->strIdPath);

    //CString strParentNode;
    //parentNode.GetIdPath(strParentNode);
    //AddChangedNode(strParentNode);
    if (!m_bInit)
    {
        if (!strOldIdPath.IsEmpty())
        {
            FireUpdateTree(strOldIdPath, eUpdateDelete);
        }
        FireUpdateTree(pFileInfo->strIdPath, eUpdateInsert);
    }

    SetSourceTime(pFileInfo, pFileInfo->strFileName);
    
    return TRUE;  
}




//------------------------------------------------------------------*
/**
 * CheckSourceFilesToReparse checks the modified date of the 
 * source files and add all changed files to the list with files to reparse
 *
 * @return          TRUE: changed files have been found
 *                  FALSE: no files have changed
 * @exception       -
 * @see             
*/
BOOL CGenericTree::CheckSourceFilesToReparse()
{
    POSITION pos;
    pos = m_mapSourceFiles.GetStartPosition();
    while(pos!=NULL)
    {
        TSourceFileInfo *finfo=NULL;
        CString strKey;
        m_mapSourceFiles.GetNextAssoc(pos, strKey, finfo);
        PROJ_MAN_ASSERT(finfo);
        
        CTime parseSourceTime;
        CTime actSourceTime;
        parseSourceTime = finfo->parseSourceTime;
        CFileStatus fileStatus;
        if (!CFile::GetStatus(
                CFileHelper::GetFileAbsoluteReplaced(finfo->strFileName), // m_pProjectManager->GetProjectPath() + _T("\\") + finfo->strFileName, 
                              fileStatus))
        {
            if (!StringList_Find(finfo->strFileName, m_listFilesToReparse) && 
                (finfo->bFileMissing==FALSE))
            {
                if ((finfo->strFileName).CompareNoCase(m_pProjectManager->GetProjectFileName())==0)
                {
                    m_bProjFileToReparse = TRUE;
                    ATLTRACE("Project File changed\n");
                }
                else
                {
                    AddSourceFileToReparse(finfo->strFileName);
                    ATLTRACE("Source file changed: %s\n", finfo->strFileName);
                }
            }
            continue;
        }
        actSourceTime = fileStatus.m_mtime;
        
        if (actSourceTime!=parseSourceTime)
        {
            if (!StringList_Find(finfo->strFileName, m_listFilesToReparse))
            {
                if ((finfo->strFileName).CompareNoCase(m_pProjectManager->GetProjectFileName())==0)
                {
                    m_bProjFileToReparse = TRUE;
                    ATLTRACE("Project File changed\n");
                }
                else
                {
                    AddSourceFileToReparse(finfo->strFileName);
                    ATLTRACE("Source file changed: %s\n", finfo->strFileName);
                }
            }
        }
    }

    return !(m_listFilesToReparse.IsEmpty());
}



//------------------------------------------------------------------*
/**
 * CheckSourceFilesToReparseIncrementally checks the modified date of the 
 * source files and add all changed files to the list with files to reparse
 * only checks one file a time
 *
 * @return          TRUE: one round done
 *                  FALSE: wants more calls, not everything checked
 * @exception       -
 * @see             
*/
BOOL CGenericTree::CheckSourceFilesToReparseIncrementally()
{
    BOOL ret = FALSE;

    // collect all source files
    if (m_posNextPosToCheck==NULL)
    {
        m_listFilesToCheck.RemoveAll();
        POSITION pos;
        pos = m_mapSourceFiles.GetStartPosition();
        while(pos!=NULL)
        {
            TSourceFileInfo *finfo=NULL;
            CString strKey;
            m_mapSourceFiles.GetNextAssoc(pos, strKey, finfo);
            m_listFilesToCheck.AddTail(strKey);
        }

        m_posNextPosToCheck = m_listFilesToCheck.GetHeadPosition();

    }

    if (m_posNextPosToCheck==NULL)
    {
        return TRUE;
    }

    CString strFileToCheck;
    strFileToCheck = m_listFilesToCheck.GetNext(m_posNextPosToCheck);

    if (m_posNextPosToCheck==NULL)
    {
        ret = TRUE;
    }
    else 
    {
        ret = FALSE;
    }

    TSourceFileInfo *finfo=NULL;
        
    if (!SourceFileMap_Lookup(strFileToCheck, finfo, m_mapSourceFiles))
    {
        return ret;
    }
    
    CTime parseSourceTime;
    CTime actSourceTime;
    parseSourceTime = finfo->parseSourceTime;
    CFileStatus fileStatus;
    if (!CFile::GetStatus(
                CFileHelper::GetFileAbsoluteReplaced(finfo->strFileName), // m_pProjectManager->GetProjectPath() + _T("\\") + finfo->strFileName, 
                          fileStatus))
    {
        if (!StringList_Find(finfo->strFileName, m_listFilesToReparse) && 
            (finfo->bFileMissing==FALSE))
        {
            if ((finfo->strFileName).CompareNoCase(m_pProjectManager->GetProjectFileName())==0)
            {
                m_bProjFileToReparse = TRUE;
                ATLTRACE("Project File changed\n");
            }
            else
            {
                AddSourceFileToReparse(finfo->strFileName);
                ATLTRACE("Source file changed: %s\n", finfo->strFileName);
            }
        }
        return ret;
    }

    actSourceTime = fileStatus.m_mtime;
    
    if (actSourceTime!=parseSourceTime)
    {
        if (!StringList_Find(finfo->strFileName, m_listFilesToReparse))
        {
            if ((finfo->strFileName).CompareNoCase(m_pProjectManager->GetProjectFileName())==0)
            {
                m_bProjFileToReparse = TRUE;
                ATLTRACE("Project File changed\n");
            }
            else
            {
                AddSourceFileToReparse(finfo->strFileName);
                ATLTRACE("Source file changed: %s\n", finfo->strFileName);
            }
        }
    }
   
    return ret;
}





//------------------------------------------------------------------*
/**
 * Add a source file to the list of source files to reparse, so that
 * it will be reparsed the next time a reparsing operation will be done.
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
BOOL CGenericTree::AddSourceFileToReparse(const CString& strFileName)
{
    DeleteProjectBuildSem(strFileName);
    
    if (strFileName.CompareNoCase(m_pProjectManager->GetProjectFileName())==0)
    {
        m_bProjFileToReparse = TRUE;
        return TRUE;
    }
    if (!StringList_Find(strFileName, m_listFilesToReparse))
    {
        CString strFN = strFileName;
        m_listFilesToReparse.AddTail(strFN);
        return TRUE;
    }
    return FALSE;
}


//------------------------------------------------------------------*
/**
 * Reparse reparses all files in the reparse file list.
 * 
 * @param           updateInfo: (TRUE): if FALSE the navigator will not
 *                              be informed that some nodes have changed,
 *                              that is necessary only for the time of the 
 *                              first creation of the tree, the navigator 
 *                              does not have a tree at that time at all.
 * @return          
 * @exception       -
 * @see             
*/
BOOL CGenericTree::Reparse(BOOL updateInfo /*=TRUE*/)
{
    CString fileName;
    TSourceFileInfo* pFileInfo;
    POSITION pos1, pos2;

    //m_listChangedNodes.RemoveAll();

    if (m_bProjFileToReparse)
    {
        ATLTRACE("Reparsing project file\n");
        DeleteProjectBuildSem(m_pProjectManager->GetProjectFileName());
        ReparseProjFile();
        m_bProjFileToReparse = FALSE;
    }

    if (!m_listFilesToReparse.IsEmpty())
    {
        pos1 = m_listFilesToReparse.GetHeadPosition();
        while (pos1!=NULL)
        {
            pos2 = pos1;
            fileName = m_listFilesToReparse.GetAt(pos2);
            if (SourceFileMap_Lookup(fileName, pFileInfo, m_mapSourceFiles))
            {
                ReparseFile(pFileInfo);
            }

            m_listFilesToReparse.GetNext(pos1);
            m_listFilesToReparse.RemoveAt(pos2);
        }
        m_listFilesToReparse.RemoveAll();
    }

    //if (updateInfo && (m_listChangedNodes.GetCount()>0))
    //{
    //    FireUpdateTree();
    //}
    if (updateInfo)
    {
        if (m_bFileViewChanged==TRUE)
        {
//            m_pProjectManager->Fire_InvalidateFileView();
            m_bFileViewChanged = FALSE;
        }

    }

    return TRUE;
}


//------------------------------------------------------------------*
/**
 * ReparseFile reparsed one source file, do all necessary changes in the dom
 *
 * @param           [in] pFileInfo: the info containing the source file name
 * @return          TRUE: on success
 *                  FALSE: error
 * @exception       -
 * @see             
*/
BOOL CGenericTree::ReparseFile(TSourceFileInfo *pFileInfo, BOOL bAddToParent /* = TRUE */)
{
    HRESULT hr;
    CComBSTR sBackXML;
    CString fileName;
    CString fileExt;
    CList<CString, CString&> newChildList;
    BOOL bNodeIdChanged = TRUE;

    ICEBackparse* pBackparser = NULL;

    PROJ_MAN_ASSERT(pFileInfo);
    ATLTRACE("Parsing File %s\n", pFileInfo->strFileName);

    DeleteProjectBuildSem(pFileInfo->strFileName);

    // get full file name and the file extension
    fileName = CFileHelper::GetFileAbsoluteReplaced(pFileInfo->strFileName);
    fileExt  = CFileHelper::GetFileExt(pFileInfo->strFileName);
    
    PROJ_MAN_ASSERT(!fileName.IsEmpty());
    PROJ_MAN_ASSERT(!fileExt.IsEmpty());

    CFileStatus fStatus;
    if (!CFile::GetStatus(fileName, fStatus))
    {
        if (m_bMsgMissingFiles)
        {
            m_pProjectManager->OutputErrorMessage(E_CEPROJMAN_SOURCE_NOT_FOUND,
                                              fileName);
        }
        BuildErrorNode(pFileInfo);
        pFileInfo->bFileMissing = TRUE;
        return FALSE;
    }
    pFileInfo->bFileMissing = FALSE;

    // get the backparser
    pBackparser = m_pProjectManager->GetBackparserForFileExt(fileExt);

    if (!pBackparser)
    {
        m_pProjectManager->OutputErrorMessage(E_CEPROJMAN_BACKPARSER_CREATE,
                                              fileExt);
        BuildErrorNode(pFileInfo);
        return FALSE;
    }
    
    CComBSTR sFileName(fileName);

    IUnknown* pUnk = m_pProjectManager->GetUnknown();
    CComQIPtr<ICEProjInfo, &IID_ICEProjInfo> pProjInfo;
    pProjInfo = pUnk;
    PROJ_MAN_ASSERT(!(!pProjInfo));
    hr = pBackparser->doBackparse(sFileName, pProjInfo, &sBackXML);
    pBackparser->Release();         
    if (!SUCCEEDED(hr) || ((CString)sBackXML).IsEmpty())
    {
        m_pProjectManager->OutputErrorMessage(E_CEPROJMAN_BACKPARSER_CALL_FAILED,
                                              fileName);
        BuildErrorNode(pFileInfo);
        return FALSE;
    }

    // temporary dom document for parsing the returned string
    CXMLDocument tempDoc;
    CXMLNode newNode;

    if ((!tempDoc.LoadXMLString((CString)sBackXML))
        ||(!tempDoc.GetRootNode(newNode)))
    {
        m_pProjectManager->OutputErrorMessage(E_CEPROJMAN_BACKPARSER_CALL_FAILED,
                                              fileName);
        BuildErrorNode(pFileInfo);
        return FALSE;
    }

    // set parser source attribute 
    newNode.SetAttribute(CE_XMLATTR_PARSERSOURCE, pFileInfo->strFileName);
    // get expanded attribute from old node and reset it on new node
    CString strExpanded;
    if ((pFileInfo->xmlNode).GetAttribute(CE_XMLATTR_EXPANDED, strExpanded))
    {
        newNode.SetAttribute(CE_XMLATTR_EXPANDED, strExpanded);
    }

    newNode.SetAttribute(CE_XMLATTR_PARSERSOURCE, pFileInfo->strFileName);
    
    // check if there are linked source files in the backparser return
    CheckForLinkedFiles(newNode, pFileInfo->strFileName, newChildList);

    CString strOldId;
    CString strOldIdPath;

    if (bAddToParent)
    {
        CXMLNode parentNode;
        if (!(pFileInfo->xmlNode).GetParentNode(parentNode))
        {
            return FALSE;
        }

        ParentCheck(parentNode, newNode);

        strOldIdPath = pFileInfo->strIdPath;
  
        CString strNewId;
        (pFileInfo->xmlNode).GetAttribute(CE_XMLATTR_ID, strOldId);
        newNode.GetAttribute(CE_XMLATTR_ID, strNewId);
        CString strOldType;
        CString strNewType;
        (pFileInfo->xmlNode).GetAttribute(CE_XMLATTR_TYPE, strOldType);
        newNode.GetAttribute(CE_XMLATTR_TYPE, strNewType);
        if ((strOldId.Compare(strNewId)==0) && (strOldType.Compare(strNewType)==0))
        {
            bNodeIdChanged = FALSE;
        }

        if (!parentNode.ReplaceChild(newNode, pFileInfo->xmlNode))
        {
            return FALSE;
        }
    }

    // find removed files: compare old and new child file lists of the node: 
    // all files that are in the old one but no longer in the new one can be deleted 
    // with all their children
    POSITION pos = (pFileInfo->strlChildFileNames).GetHeadPosition();
    CString childFileName;
    while (pos!=NULL)
    {
        childFileName = (pFileInfo->strlChildFileNames).GetAt(pos);
        if (!StringList_Find(childFileName, newChildList))
        {
            RemoveSourceFileAndChilds(childFileName);
        }

        (pFileInfo->strlChildFileNames).GetNext(pos);
    }   
   
    (pFileInfo->strlChildFileNames).RemoveAll();
    (pFileInfo->strlChildFileNames).AddTail(&newChildList);

    pFileInfo->xmlNode = newNode;
    (pFileInfo->xmlNode).GetIdPath(pFileInfo->strIdPath);
    SetSourceTime(pFileInfo, fileName);

    if (bNodeIdChanged)
    {
        if (!strOldIdPath.IsEmpty())
        {
            FireUpdateTree(strOldIdPath, eUpdateDelete);
        }
        FireUpdateTree(pFileInfo->strIdPath, eUpdateInsert);
    }
    else
    {
        FireUpdateTree(pFileInfo->strIdPath, eUpdateAll);
    }

    /*if (bAddToParent)
    {
        CString strChangedNode;
        newNode.GetIdPath(strChangedNode);
        if (bNodeIdChanged)
        {
            CString strParentNode;
            GetParentIdPath(strChangedNode, strParentNode);
            AddChangedNode(strParentNode);
        }
        else
        {
            AddChangedNode(strChangedNode);
        }
    }*/

    return TRUE;
}


//------------------------------------------------------------------*
/**
 * CheckForLinkedFiles lookd at all subnodes of a node, 
 * if there are nodes with a LINK tag,
 * these nodes reference another source file, that has already been 
 * reparsed and has to be inserted there or is a new source file and
 * has to be added to the source file map.
 *
 * @param           [in] node: the xml node to start with 
 *                              (parent, look at all childs)
 * @param           [in] strFileName: the name of the source file
 *                                    for that node
 * @param           [in,out] strlChildList: the list with all childs of
 *                                  that node, has to be build up
 * @return          TRUE: on success
 *                  FALSE: error
 * @exception       -
 * @see             
*/
BOOL CGenericTree::CheckForLinkedFiles(CXMLNode& node, CString& strFileName, CList<CString, CString&>& strlChildList)
{
    CXMLNodeList childNodeList;
    CXMLNode childNode;

    if (!node.GetChildNodeList(childNodeList))
    {
        return FALSE;
    }

    if (childNodeList.GetLength()<=0)
    {
        return TRUE;
    }

    childNodeList.Reset();
    while(childNodeList.Next(childNode))
    {
         CString strTagName;
         if (!childNode.GetTagName(strTagName))
         {
             continue;
         }

         if (strTagName.Compare(CE_XMLTAG_NODE)==0)
         {
             CheckForLinkedFiles(childNode, strFileName, strlChildList); 
             continue;
         }

         if (strTagName.Compare(CE_XMLTAG_LINK)!=0)
         {
             continue;
         }

         // LINK tag found
         CString strLinkedFileName;
         if (!childNode.GetAttribute(CE_XMLATTR_PARSERSOURCE, strLinkedFileName))
         {
             if (!childNode.GetAttribute(CE_XMLATTR_SOURCE, strLinkedFileName))
             {
                 continue;
             }
         }

         strlChildList.AddTail(strLinkedFileName);

         // does file already exist in source file list?
         TSourceFileInfo* pLinkedFileInfo;
         if (SourceFileMap_Lookup(strLinkedFileName, pLinkedFileInfo, m_mapSourceFiles))
         {
            PROJ_MAN_ASSERT(pLinkedFileInfo);
            CXMLNode parentNode;
            if (!childNode.GetParentNode(parentNode))
            {
                continue;
            }
            parentNode.ReplaceChild(pLinkedFileInfo->xmlNode, childNode);
         }
         else
         {
             pLinkedFileInfo = CreateSourceFileInfo(strLinkedFileName, childNode, strFileName);
             PROJ_MAN_ASSERT(pLinkedFileInfo);
             SourceFileMap_SetAt(strLinkedFileName, pLinkedFileInfo, m_mapSourceFiles);
             if(!m_bInit)
             {
                m_pProjectManager->Fire_OnFileAdded((CComBSTR)strLinkedFileName, FV_FILE_SOURCE);
             }
             AddSourceFileToReparse(strLinkedFileName);
             m_bFileViewChanged = TRUE;
             ATLTRACE("New source file: %s\n", strLinkedFileName);
         }
    }
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * GetTreeXML returns the xml tree representation as string
 *
 * @param           
 * @return          the xml string representation
 * @exception       -
 * @see             
*/
CString CGenericTree::GetTreeXML()
{
    CString xmlString;

    m_domDocument.GetXMLString(xmlString);

    return xmlString;
}


//------------------------------------------------------------------*
/**
 * GetIXMLDoc returns the IXMLDOMDocument interface pointer to the
 * dom tree.
 *
 * @param           
 * @return          the xml string representation
 * @exception       -
 * @see             
*/
BOOL CGenericTree::GetIXMLDoc(IUnknown** ppDoc)
{
    return (m_domDocument.GetIXMLDocumentUnknown(ppDoc));
}



//------------------------------------------------------------------*
/**
 * SaveViewData saves the xml tree in a file. The member m_strFileName
 * has to be set by the subclass.
 *
 * @param           
 * @return          TRUE: on success
 *                  FALSE: error
 * @exception       -
 * @see             
*/
BOOL CGenericTree::SaveViewData()
{
    CString fileName;
    fileName = m_pProjectManager->GetProjectPath() + _T("\\") +
        CE_PROJECT_VIEW_PATH + _T("\\");
    if (!FC_ExistSubDir(fileName))
    {
        if (!FC_CreateSubDir(fileName))
        {
            return FALSE;
        }
    }
    fileName += m_strFileName;  
    m_domDocument.Save(fileName);

    return TRUE;
}


//------------------------------------------------------------------*
/**
 * LoadViewData loads the xml tree from a file. The member m_strFileName
 * has to be set by the subclass.
 *
 * @param           
 * @return          TRUE: on success
 *                  FALSE: error
 * @exception       -
 * @see             
*/
BOOL CGenericTree::LoadViewData()
{
    CString fileName;
    fileName = m_pProjectManager->GetProjectPath() + _T("\\") +
        CE_PROJECT_VIEW_PATH + _T("\\") + m_strFileName;

    if (!FC_Exist(fileName))
    {
        return FALSE;
    }

    if (!m_domDocument.Load(fileName))
    {
        return FALSE;
    }

    // walk through the xml tree and fill source file info structs
    CXMLNode projectNode;
    m_domDocument.GetRootNode(projectNode);

    CString strExpVersion;
    strExpVersion.Format(_T("%i"), PRODUCT_BUILD);
    CString strFoundVersion;
    projectNode.GetAttribute(PRJMAN_VERSION, strFoundVersion);
    CString strProjectFileName;

    if (strFoundVersion.Compare(strExpVersion)!=0)
    {
        ATLTRACE("ProjMan: wrong version of %s: reparsing!\n", fileName);
        CXMLNode docRoot;
        if (m_domDocument.GetDocNode(docRoot)) 
        {
            docRoot.RemoveChild(projectNode);
        }
        return FALSE;
    }

    if(!projectNode.GetAttribute(CE_XMLATTR_PARSERSOURCE, strProjectFileName))
    {
        ATLTRACE("ProjMan: no referenced project file: reparsing!\n");
        CXMLNode docRoot;
        if (m_domDocument.GetDocNode(docRoot)) 
        {
            docRoot.RemoveChild(projectNode);
        }
        return FALSE;
    }

    // check, if project file exists
    if(strProjectFileName.Right(4).CompareNoCase(_T(".4cp")) == 0)
    {
        CFileStatus tStatus;
        CString strProjectFilePath = CFileHelper::GetFileAbsolute(strProjectFileName);
        if(!CFile::GetStatus(strProjectFilePath, tStatus))
        {
            ATLTRACE("ProjMan: referenced project file not existing %s: reparsing!\n", strProjectFileName);
            CXMLNode docRoot;
            if (m_domDocument.GetDocNode(docRoot)) 
            {
                docRoot.RemoveChild(projectNode);
            }
            return FALSE;
        }
    }

    if (!UpdateFileInfo(projectNode, _T("")))
    {
        // clean up source file list if something went wrong
        POSITION pos;
        pos = m_mapSourceFiles.GetStartPosition();
        while(pos!=NULL)
        {
            TSourceFileInfo *finfo=NULL;
            CString strKey;
            m_mapSourceFiles.GetNextAssoc(pos, strKey, finfo);
            PROJ_MAN_ASSERT(finfo);
            delete finfo;
            m_mapSourceFiles.RemoveKey(strKey);
        }
        m_mapSourceFiles.RemoveAll();
        return FALSE;
    }

    // do a check if some source files have been changed
    CheckSourceFilesToReparse();

    return TRUE;
}


//------------------------------------------------------------------*
/**
 * UpdateFileInfo is a helper function for LoadViewData: 
 * it initializes a new info struct for a source file and checks all children
 *
 * @param           [in] node   : the xml node for which the source file info 
 *                                 struct should be created
 * @param           [in] strParentFile: the name of the parent file, this is needed
 *                                     to fill the source file info
 * @return          TRUE: on success
 *                  FALSE: error
 * @exception       -
 * @see             
*/
BOOL CGenericTree::UpdateFileInfo(const CXMLNode& node, const CString& strParentFile)
{
    CString fileName;
    CTime parseTime;

    TSourceFileInfo* pFileInfo;
    if (!node.GetAttribute(CE_XMLATTR_PARSERSOURCE, fileName))
    {
        return FALSE;
    }

    pFileInfo = CreateSourceFileInfo(fileName, node, strParentFile);
    PROJ_MAN_ASSERT(pFileInfo);
    GetModTimeInfo(node, parseTime);
    pFileInfo->parseSourceTime = parseTime;
    (pFileInfo->xmlNode).GetIdPath(pFileInfo->strIdPath);

    CString strNodeType;
    if ((pFileInfo->xmlNode).GetAttribute(CE_XMLATTR_TYPE, strNodeType))
    {
        if (strNodeType.Compare(CE_XMLTYPE_ERRORNODE)==0)
        {
            AddSourceFileToReparse(fileName);
        }
    }

    SourceFileMap_SetAt(fileName, pFileInfo, m_mapSourceFiles);
    if(!m_bInit)
    {
        m_pProjectManager->Fire_OnFileAdded((CComBSTR)fileName, FV_FILE_SOURCE);
    }

    // check child nodes
    if (!UpdateChildFileInfo(node, fileName, pFileInfo->strlChildFileNames))
    {
        return FALSE;
    }
    
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * UpdateChildFileInfo is a helper function for UpdateFileInfo, it checks
 * for all childs of a node, if the child node has a SOURCE attribute 
 * and the source file info struct has to be filled, and recursively checks
 * the children of the node. UpdateChildFileInfo collects and returns 
 * all children source files.
 *
 * @param           [in] node   : the xml node for which the source file info 
 *                                struct should be created
 * @param           [in] strParentFile: the name of the parent file, this is needed
 *                                     to fill the source file info
 * @param           [out] childList: the list with all child source files
 * @return          TRUE: on success
 *                  FALSE: error
 * @return          
 * @exception       -
 * @see             
*/
BOOL CGenericTree::UpdateChildFileInfo(const CXMLNode& node, 
                                       const CString& strParentFile,
                                       CList<CString, CString&>& childList)
{
    CXMLNodeList childNodes;
    CXMLNode child;

    node.GetChildNodeList(childNodes);

    if (childNodes.GetLength()<=0)
    {
        return TRUE;
    }

    childNodes.Reset();

    while(childNodes.Next(child))
    {
        CString childFileName;
        if (child.GetAttribute(CE_XMLATTR_PARSERSOURCE, childFileName))
        {
            childList.AddTail(childFileName);
            UpdateFileInfo(child, strParentFile);
        }
        else
        {
            if (!UpdateChildFileInfo(child, strParentFile, childList))
            {
                return FALSE;
            }
        }
    }
    return TRUE;
}



//------------------------------------------------------------------*
/**
 * SetModTimeInfo stores the modification time of a source file
 * as attribute in the xml representation of the node.
 *
 * @param           [in,out] node: the node for which the time should be set
 * @param           [in] time    : the modification time
 * @return          TRUE: on success
 *                  FALSE: error
 * @exception       -
 * @see             
*/
BOOL CGenericTree::SetModTimeInfo(CXMLNode& node, const CTime& time)
{
    CString strTime;
    strTime.Format(_T("%.4i:%.2i:%.2i:%.2i:%.2i:%.2i"), 
                    time.GetYear(),
                    time.GetMonth(),
                    time.GetDay(),
                    time.GetHour(),
                    time.GetMinute(),
                    time.GetSecond());
    return node.SetAttribute(CE_XMLATTR_MODTIME, strTime);
}


//------------------------------------------------------------------*
/**
 * GetModTimeInfo reloads the modification time of a source file
 * from an attribute in the xml representation of the node.
 *
 * @param           [in] node    : the node of which the time should be set
 * @param           [out] time   : the modification time
 * @return          TRUE: on success
 *                  FALSE: error
 * @exception       -
 * @see             
*/
BOOL CGenericTree::GetModTimeInfo(const CXMLNode& node, CTime& time)
{
    CString strTime;
    if (!node.GetAttribute(CE_XMLATTR_MODTIME, strTime))
    {
        return FALSE;
    }

    int year,month,day,hour,min,sec;

    if (strTime.GetLength()!=19)
    {
        return FALSE;
    }

    CString s;

    s = strTime.Mid(0,4);
    year = _ttoi(s);

    s = strTime.Mid(5,2);
    month = _ttoi(s);

    s = strTime.Mid(8,2);
    day = _ttoi(s);

    s = strTime.Mid(11,2);
    hour = _ttoi(s);

    s = strTime.Mid(14,2);
    min = _ttoi(s);

    s = strTime.Mid(17,2);
    sec = _ttoi(s);

    if (month<1 || month>12 || day<1 || day>31 || 
        hour<0 || hour>24 || min<0 || min >60 || sec<0 || sec>60)
    {
        return FALSE;
    }

    CTime constrTime(year, month, day, hour, min, sec);
    time = constrTime;

    return TRUE;
}


//------------------------------------------------------------------*
/**
 * FillChangedNodesSafeArray is a helper function to create a 
 * SAFEARRAY from the string list m_listChangedNodes
 *
 * @param           pparrChagedNodes: a pointer to a SAFEARRAY pointer
 * @return          TRUE: on success
 *                  FALSE: error
 * @exception       -
 * @see             
*/
/*BOOL CGenericTree::FillChangedNodesSafeArray(SAFEARRAY** pparrChangedNodes)
{
    HRESULT hr;
    SAFEARRAYBOUND Bound;
    Bound.lLbound = 0;
    Bound.cElements = m_listChangedNodes.GetCount();

    *pparrChangedNodes = ::SafeArrayCreate(VT_BSTR, 1, &Bound);
    if (NULL == (*pparrChangedNodes)) 
    {
        return FALSE;    
    }

    long lIndex = 0;
    POSITION pos = m_listChangedNodes.GetHeadPosition();
    CString str;

    while(pos!=NULL)
    {
        str = m_listChangedNodes.GetNext(pos);
        CComBSTR sStr(str);
        hr = ::SafeArrayPutElement(*pparrChangedNodes, &lIndex, 
                                    (void*)sStr);
        if (FAILED(hr))
        {
            ::SafeArrayDestroy(*pparrChangedNodes);
            return FALSE;
        }
        lIndex++;
    }

    return TRUE;

}
*/

//------------------------------------------------------------------*
/**
 * AddChangedNode add an id path of a node to the changed nodes list.
 * It checks if the new path is a subpath of another path already in 
 * the list, or if there are subpathes of that path in the list.
 *
 * @param           strChangedNode: the id path to add
 * @return          
 * @exception       -
 * @see             
*/
/*void CGenericTree::AddChangedNode(CString& strChangedNode)
{
    POSITION pos = m_listChangedNodes.GetHeadPosition();
    POSITION thispos;
    CString str;

    while(pos!=NULL)
    {
        thispos = pos;
        str = m_listChangedNodes.GetNext(pos);
        if ((str.GetLength() > strChangedNode.GetLength())
            &&(str.Left(strChangedNode.GetLength()).Compare(strChangedNode)==0)
            &&(str.GetAt(strChangedNode.GetLength())==_T('/')) )
        {
            m_listChangedNodes.RemoveAt(thispos);
        }
        else if ((str.GetLength() < strChangedNode.GetLength())
                 &&(strChangedNode.Left(str.GetLength()).Compare(str)==0)
                 &&(strChangedNode.GetAt(str.GetLength())==_T('/')) )
        {
            return;
        }
        else if ((str.GetLength() == strChangedNode.GetLength())
                 &&(strChangedNode.Compare(str)==0))
        {
            return;
        }
    }
    
    m_listChangedNodes.AddTail(strChangedNode);
}
*/

//------------------------------------------------------------------*
/**
 * GetParentSourceAndId returns the parent file of a source file 
 * belonging to this tree and the id path for this source file.
 *
 * @param           strSourceFile: the source file name for which the
 *                                 parent should be searched
 * @param           strParentSourceFile: the file name of the parent file
 * @param           strParentId: the id path of the parent node
 * @return          TRUE: parent file found
 *                  FALSE: parent file not found
 * @exception       -
 * @see             
*/
BOOL CGenericTree::GetParentSourceAndId(const CString& strSourceFile, 
                                        CString& strParentSourceFile,
                                        CString& strParentId)
{
    TSourceFileInfo* pSourceFileInfo;
    if (!SourceFileMap_Lookup(strSourceFile, pSourceFileInfo, m_mapSourceFiles))
    {
        return FALSE;
    }
    PROJ_MAN_ASSERT(pSourceFileInfo);

    strParentSourceFile = pSourceFileInfo->strParentFileName;
    TSourceFileInfo* pParentFileInfo;
    if (!SourceFileMap_Lookup(strParentSourceFile, pParentFileInfo, m_mapSourceFiles))
    {
        return FALSE;
    }

    PROJ_MAN_ASSERT(pParentFileInfo);
    strParentId = pParentFileInfo->strIdPath;
    return TRUE;
}



//------------------------------------------------------------------*
/**
 * IsTreeSouceFile checks if a source file belongs to the source files
 * that have been used to build up this tree.
 *
 * @param           strFileName: the name of the source file
 * @return          TRUE: the file belongs to the tree source files
 *                  FALSE: the file does not belong to the tree files
 * @exception       -
 * @see             
*/
BOOL CGenericTree::IsTreeSourceFile(const CString& strFileName)
{
    TSourceFileInfo* pFileInfo;
    if (!SourceFileMap_Lookup(strFileName, pFileInfo, m_mapSourceFiles))
    {
        return FALSE;
    }
    return TRUE;
}



//------------------------------------------------------------------*
/**
 * Get the list of all source files used to build up this tree.
 *
 * @param           listSourceFiles: this list will be filled
 * @return          TRUE: ok
 *                  FALSE: error
 * @exception       -
 * @see             
*/
BOOL CGenericTree::GetAllTreeSourceFiles(CList<CString, CString&>& listSourceFiles)
{
    POSITION pos = m_mapSourceFiles.GetStartPosition();
    while(pos)
    {
        CString strKey;
        TSourceFileInfo* pFileInfo;
        m_mapSourceFiles.GetNextAssoc(pos, strKey, pFileInfo);
        listSourceFiles.AddTail(pFileInfo->strFileName);
    }
    return TRUE;
}



//------------------------------------------------------------------*
/**
 * GetFileNameFromIdPath finds the corresponding file name for a node
 * in the tree given by an id path, if the node has no SOURCE attribute 
 * the tree is searched up until one parent has a SOURCE attribute
 *
 * @param           strIdPath: the id path of the node
 * @param           strFileName: the corresponding file name
 * @return          TRUE: file name found
 *                  FASLE: error
 * @exception       -
 * @see             
*/
BOOL CGenericTree::GetFileNameFromIdPath(const CString& strIdPath, CString& strFileName)
{

    CXMLNode rootNode;
    if (!m_domDocument.GetRootNode(rootNode))
    {
        return FALSE;
    }

    CXMLNode node;
    if (!rootNode.GetNodeFromIdPath(strIdPath, node))
    {
        return FALSE;
    }

    BOOL bHasSource = node.GetAttribute(CE_XMLATTR_SOURCE, strFileName);
    while (!bHasSource)
    {
        if (!node.GetParentNode(node))
        {
            return FALSE;
        }
        bHasSource = node.GetAttribute(CE_XMLATTR_SOURCE, strFileName);
    }
   
    return TRUE;
}



//------------------------------------------------------------------*
/**
 * GetIdPathFromFileName finds the corresponding id path for a node
 * in the tree given by a file name.
 *
 * @param           strFileName: the file name
 * @param           strIdPath: the corresponding id path of the node
 * @return          TRUE: file found
 *                  FASLE: error
 * @exception       -
 * @see             
*/BOOL CGenericTree::GetIdPathFromFileName(const CString& strFileName, CString& strIdPath)
{
    TSourceFileInfo* pFileInfo;

    if (!SourceFileMap_Lookup(strFileName, pFileInfo, m_mapSourceFiles))
    {
        return FALSE;
    }
    PROJ_MAN_ASSERT(pFileInfo);

    strIdPath = pFileInfo->strIdPath;
    return TRUE;
}



//------------------------------------------------------------------*
/**
 * GetAllClassesOfTypeUnderNode returns the id paths of all nodes of
 * a given type starting with a given node in the tree.
 *
 * @param           strStartNode: the node to start the search with
 * @param           strType: the type of the nodes searched for
 * @param           parrNodes: the found nodes
 * @return          TRUE: ok
 *                  FALSE: error
 * @exception       -
 * @see             
*/
BOOL CGenericTree::GetAllClassesOfTypeUnderNode(const CString strStartNode, const CString strType, 
                                  SAFEARRAY** parrNodes)
{
    CList<CString, CString&> nameList;
    // look at all nodes to find nodes of type strType
    CXMLNode rootNode;
    if (!m_domDocument.GetRootNode(rootNode))
    {
        return FALSE;
    }

    CXMLNode startNode;
    if (!rootNode.GetNodeFromIdPath(strStartNode, startNode))
    {
        return FALSE;
    }

    CXMLNodeIter nodeIter(startNode);
    CXMLNode node;
    while(nodeIter.GetNext(node))
    {
        CString strFoundType;
        if (node.GetAttribute(CE_XMLATTR_TYPE, strFoundType))
        {
            if (strFoundType.Compare(strType)==0)
            {
                CString strIdPath;
                if (node.GetIdPath(strIdPath))
                {
                    nameList.AddTail(strIdPath);
                } 
            }
        }
    }

    POSITION pos;
    HRESULT hr;
    SAFEARRAYBOUND Bound;
    Bound.lLbound = 0;
    Bound.cElements = nameList.GetCount();

    *parrNodes = ::SafeArrayCreate(VT_BSTR, 1, &Bound);
    if (NULL == (*parrNodes)) 
    {
        return FALSE;    
    }

    long lIndex = 0;
    pos = nameList.GetHeadPosition();
    CString str;

    while(pos!=NULL)
    {
        CString strNode = nameList.GetNext(pos);
        CComBSTR sNode(strNode);
        hr = ::SafeArrayPutElement(*parrNodes, &lIndex, 
                                    (void*)sNode);
        if (FAILED(hr))
        {
            ::SafeArrayDestroy(*parrNodes);
            return FALSE;
        }
        lIndex++;
    }

    return TRUE;
}


//------------------------------------------------------------------*
/**
 * GetXMLNodePtr returns a xml node interface ptr for a node in the tree
 * addons and backparser can store additional information in these nodes
 * to do some source generation, so they can ask for the node, 
 * but this node should not be manipulated by the operations
 *
 * @param           strIdPath: the id path for the node
 * @param           ppXMLPtr:  the pointer to the IXMLDOMNode
 * @return          TRUE: if the interface pointer could be obtained
 *                  FALSE: else
 * @exception       -
 * @see             
*/
BOOL CGenericTree::GetXMLNodePtr(const CString& strIdPath, IUnknown** ppXMLPtr)
{
    *ppXMLPtr = NULL;

    CXMLNode rootNode;
    CXMLNode searchNode;
    
    if (!m_domDocument.GetRootNode(rootNode))
    {
        return FALSE;
    }

    if (!rootNode.GetNodeFromIdPath(strIdPath, searchNode))
    {
        return FALSE;
    }
        
    if (!searchNode.GetIXMLNodeUnknown(ppXMLPtr))
    {
        return FALSE;
    }

    return TRUE;

}



//------------------------------------------------------------------*
/**
 * Reparse4CPAttributes reads all attributes like UserTxt, URL from
 * 4cp file and adds them to the backparsed node
 *
 * @param           pProjectNode: a pointer to the project node
 *                                where the attributes should be added.
 * @return          
 * @exception       -
 * @see             
*/
void CGenericTree::Reparse4CPAttributes(CXMLNode* pProjectNode)
{
    CString strUserTxt1;
    CString strUserTxt2;
    CString strUserTxt3;
    CString strUrl;
    CString strAlias;
    CString strCreated;
    CString strModified;
    CString strUsrCreated;
    CString strUsrModified;
    
    // read attributes from 4cp file
    CSection* pAttribSec = NULL;
    if (!(m_pProjectManager->GetProjectFileHandler())->FindSection(CE_CP_ATTRIBUTES, &pAttribSec))
    {
        return;
    }

    ASSERT(pAttribSec);

    CKeyAndValue* pKey = NULL;
    pAttribSec->GetFirstKeyAndValue(&pKey);

    while(pKey)
    {
        CString strKey = pKey->GetKey();
        CString strValue = pKey->GetValue();
        strValue.TrimLeft(_T("\t \""));
        strValue.TrimRight(_T("\t \""));
        if (strKey.CompareNoCase(CE_CP_USERTEXT1)==0)
        {
            strUserTxt1 = strValue;
        }
        else if (strKey.CompareNoCase(CE_CP_USERTEXT2)==0)
        {
            strUserTxt2 = strValue;
        }
        else if (strKey.CompareNoCase(CE_CP_USERTEXT3)==0)
        {
            strUserTxt3 = strValue;
        }
        else if (strKey.CompareNoCase(CE_CP_URL)==0)
        {
            strUrl = strValue;
        }
        else if (strKey.CompareNoCase(CE_CP_ALIAS)==0)
        {
            strAlias = strValue;
        }

        if (!pAttribSec->GetNextKeyAndValue(&pKey))
        {
            pKey = NULL;
        }
    }
        
    // read attributes from file system
    CFileStatus fstatus;
    if (CFile::GetStatus(m_pProjectManager->GetProjectFileNameWithPath(), 
                        fstatus))
    {
        CTime ctime = fstatus.m_ctime;
        CTime mtime = fstatus.m_mtime;

        // get create and modified date
        strCreated.Format(_T("DT#%04d-%02d-%02d-%02d:%02d:%02d"),
                                ctime.GetYear(),
                                ctime.GetMonth(),
                                ctime.GetDay(),
                                ctime.GetHour(),
                                ctime.GetMinute(),
                                ctime.GetSecond());
        strModified.Format(_T("DT#%04d-%02d-%02d-%02d:%02d:%02d"),
                                mtime.GetYear(),
                                mtime.GetMonth(),
                                mtime.GetDay(),
                                mtime.GetHour(),
                                mtime.GetMinute(),
                                mtime.GetSecond());
    }

    // write attributes to xml node
    if (!strUserTxt1.IsEmpty())
    {
        pProjectNode->SetAttribute(CE_XMLATTR_USRTEXT1, strUserTxt1);
    }
    else
    {
        pProjectNode->RemoveAttribute(CE_XMLATTR_USRTEXT1);
    }
    if (!strUserTxt2.IsEmpty())
    {
        pProjectNode->SetAttribute(CE_XMLATTR_USRTEXT2, strUserTxt2);
    }
    else
    {
        pProjectNode->RemoveAttribute(CE_XMLATTR_USRTEXT2);
    }
    if (!strUserTxt3.IsEmpty())
    {
        pProjectNode->SetAttribute(CE_XMLATTR_USRTEXT3, strUserTxt3);
    }
    else
    {
        pProjectNode->RemoveAttribute(CE_XMLATTR_USRTEXT3);
    }
    if (!strUrl.IsEmpty())
    {
        pProjectNode->SetAttribute(CE_XMLATTR_URL, strUrl);
    }
    else
    {
        pProjectNode->RemoveAttribute(CE_XMLATTR_URL);
    }
    if (!strAlias.IsEmpty())
    {
        pProjectNode->SetAttribute(CE_XMLATTR_ALIAS, strAlias);
    }
    else
    {
        pProjectNode->RemoveAttribute(CE_XMLATTR_ALIAS);
    }
    if (!strCreated.IsEmpty())
    {
        pProjectNode->SetAttribute(CE_XMLATTR_CREATED, strCreated);
    }
    else
    {
        pProjectNode->RemoveAttribute(CE_XMLATTR_CREATED);
    }
    if (!strModified.IsEmpty())
    {
        pProjectNode->SetAttribute(CE_XMLATTR_MODIFIED, strModified);
    }
    else
    {
        pProjectNode->RemoveAttribute(CE_XMLATTR_MODIFIED);
    }
    if (!strUsrCreated.IsEmpty())
    {
        pProjectNode->SetAttribute(CE_XMLATTR_USRCREATED, strUsrCreated);
    }
    else
    {
        pProjectNode->RemoveAttribute(CE_XMLATTR_USRCREATED);
    }
    if (!strUsrModified.IsEmpty())
    {
        pProjectNode->SetAttribute(CE_XMLATTR_USRMODIFIED, strUsrModified);
    }
    else
    {
        pProjectNode->RemoveAttribute(CE_XMLATTR_USRMODIFIED);
    }


    CString strIdPathProjNode;
    VERIFY(pProjectNode->GetIdPath(strIdPathProjNode));
    FireUpdateTree(strIdPathProjNode, eUpdateAttributes);
}



//------------------------------------------------------------------*
/**
 * StringList_Find
 * Helper function for string lists. Search case insensitive for 
 * strings in the list
 *
 * @param           strSearch: the string to search
 * @param           list: the string list
 * @return          TRUE: if string has been found
 *                  FALSE: else
 * @exception       -
 * @see             
*/
BOOL CGenericTree::StringList_Find(CString strSearch, CList<CString, CString&>& list)
{
    POSITION pos = list.GetHeadPosition();
    while(pos)
    {
        CString strComp = list.GetNext(pos);
        if (strSearch.CompareNoCase(strComp)==0)
        {
            return TRUE;
        }
    }
    return FALSE;
}



//------------------------------------------------------------------*
/**
 * SourceFileMap_SetAt
 * Helper function for the source file map: the file keys should be 
 * case insensitive, so enter and search for the files in lower case
 * The original file name will be stored separately.
 *
 * @param           strId: The id in the path
 * @param           pSourceInfo: the info to enter in the map
 * @param           map: the source file map itself
 * @return          
 * @exception       -
 * @see             
*/
void CGenericTree::SourceFileMap_SetAt(CString strId, TSourceFileInfo* pSourceInfo,
                                       CMap<CString, LPCTSTR, TSourceFileInfo*, TSourceFileInfo*>& map)
{
    strId.MakeLower();
    map.SetAt(strId, pSourceInfo);
}



//------------------------------------------------------------------*
/**
 * SourceFileMap_Lookup
 * Helper function for the source file map: the file keys should be 
 * case insensitive, so enter and search for the files in lower case
 * The original file name will be stored separately.
 *
 * @param           strId: The id in the path
 * @param           pSourceInfo: the info found in the map
 * @param           map: the source file map itself
 * @return          TRUE: if value has been found
 *                  FALSE: else
 * @exception       -
 * @see             
*/
BOOL CGenericTree::SourceFileMap_Lookup(CString strId, TSourceFileInfo*& pSourceInfo,
                                        CMap<CString, LPCTSTR, TSourceFileInfo*, TSourceFileInfo*>& map)
{
    strId.MakeLower();
    return map.Lookup(strId, pSourceInfo);
}


//------------------------------------------------------------------*
/**
 * SourceFileMap_RemoveKey
 * Helper function for the source file map: the file keys should be 
 * case insensitive, so enter and search for the files in lower case
 * The original file name will be stored separately.
 *
 * @param           strId: The id in the path
 * @param           map: the source file map itself
 * @return          TRUE: if the id has been found and deleted
 *                  FALSE: else
 * @exception       -
 * @see             
*/
BOOL CGenericTree::SourceFileMap_RemoveKey(CString strId,
                                           CMap<CString, LPCTSTR, TSourceFileInfo*, TSourceFileInfo*>& map)
{
    strId.MakeLower();
    return map.RemoveKey(strId);
}



void CGenericTree::DeleteProjectBuildSem(const CString& strFileName)
{
    // delete project compiled semaphore file
    if (m_bDeleteSem)
    {
        // check file date
        CString strChangedFileName = CFileHelper::GetFileAbsolute(strFileName);
        CString strSemFile = CFileHelper::GetFileAbsolute(CE_GEN_PATH _T("\\project.sem"));

        CFileStatus fsChangedFile;
        CFileStatus fsSemFile;

        if (CFile::GetStatus(strChangedFileName, fsChangedFile) &&
            CFile::GetStatus(strSemFile, fsSemFile) &&
            (fsSemFile.m_mtime < fsChangedFile.m_mtime))
        {
            DeleteFile(m_pProjectManager->GetProjectPath() +_T("\\")  CE_GEN_PATH   _T("\\project.sem"));
        }
    }
}



void CGenericTree::ParentCheck(const CXMLNode& parentNode, const CXMLNode& newNode)
{
    return;
}
