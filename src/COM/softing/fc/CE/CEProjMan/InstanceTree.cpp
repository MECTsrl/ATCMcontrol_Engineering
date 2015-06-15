

#include "stdafx.h"
#include "InstanceTree.h"

#include "CEProjMan.h"
#include "CEProjectManager.h"
#include "CEKadMan.h"
#include "fc_tools\fc_fileIO.h"

#include "BuildNr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*

const CString con_strInstanceRootType = _T("PROJECT_INSTANCE");


//------------------------------------------------------------------*
/**
 * Constructor
 *
 * @param           pProjMan:  a pointer to the ProjectManager instance
 *                             to get additional information of the project
 * @return          
*/
CInstanceTree::CInstanceTree(CCEProjectManager *pProjMan) : CGenericTree(pProjMan)
{
    m_strFileName = CE_FILE_INSTANCEVIEW;
    m_bMsgMissingFiles = FALSE;
    m_bDeleteSem = FALSE;
}


//------------------------------------------------------------------*
/**
 * destructor
 *
*/
CInstanceTree::~CInstanceTree()
{
    POSITION pos;
    pos = m_mapInstanceTargets.GetStartPosition();
    while(pos)
    {
        TInstanceTargetInfo* pInfo;
        CString strId;
        m_mapInstanceTargets.GetNextAssoc(pos, strId, pInfo);
        delete pInfo;
    }

    m_mapInstanceTargets.RemoveAll();
}


//------------------------------------------------------------------*
/**
 * InitTree
 * initialize the instance tree for the first time: get all necessary 
 * information from the kad and construct root and classes group nodes
 *
 * @return          TRUE: on success
 *                  FALSE: else
 * @exception       -
 * @see             
*/
BOOL CInstanceTree::InitTree()
{
    GetStaticInfos();

    // create project node
    CXMLNode docRoot;
    CXMLNode projectNode;

    PROJ_MAN_ASSERT(m_pProjectManager);

    CString strProjNodeId;
    strProjNodeId = m_pProjectManager->GetProjectName()+_T(".")+ID_PATH_INSTANCEVIEW;
    CString strVersion;
    strVersion.Format(_T("%i"), PRODUCT_BUILD);
    if (  (!m_domDocument.GetDocNode(docRoot))
        ||(!m_domDocument.CreateNode(projectNode, CE_XMLTAG_NODE))
        ||(!docRoot.AppendChild(projectNode))
        ||(!projectNode.SetAttribute(CE_XMLATTR_TEXT, m_pProjectManager->GetProjectName()))
        ||(!projectNode.SetAttribute(CE_XMLATTR_ID, strProjNodeId))
        ||(!projectNode.SetAttribute(CE_XMLATTR_PARSERSOURCE, m_pProjectManager->GetProjectFileName()))
        ||(!projectNode.SetAttribute(CE_XMLATTR_TYPE, m_strInstancesType))
        ||(!projectNode.SetAttribute(PRJMAN_VERSION, strVersion))    )
    {
        return FALSE;
    }

    Reparse4CPAttributes(&projectNode);

    // add project file to source file list
    TSourceFileInfo* pProjFileInfo;
    CString strProjFileName = m_pProjectManager->GetProjectFileName();
    pProjFileInfo = CreateSourceFileInfo(strProjFileName,
                                         projectNode,  _T(""));
    PROJ_MAN_ASSERT(pProjFileInfo);
    pProjFileInfo->strIdPath = CE_XML_IDPATH_SEP + strProjNodeId;
    SetSourceTime(pProjFileInfo, m_pProjectManager->GetProjectFileNameWithPath());
    SourceFileMap_SetAt(strProjFileName, pProjFileInfo, m_mapSourceFiles);

    // update targets
    UpdateTargets();

    Reparse(FALSE);

    ExpandFromViewData();

    IXMLDOMDocument *pXMLDoc;
    m_domDocument.GetIXMLDocument(&pXMLDoc);
    m_pProjectManager->Fire_InitInstanceTree((IUnknown*)pXMLDoc);
    pXMLDoc->Release();

    m_bInit = FALSE;

    POSITION pos = m_mapInstanceTargets.GetStartPosition();
    while(pos)
    {
        CString strId;
        TInstanceTargetInfo* pInfo;
        m_mapInstanceTargets.GetNextAssoc(pos, strId, pInfo);
        if (pInfo)
        {
            CComBSTR sId = pInfo->strTargetId;
            CComBSTR sIdPath = pInfo->strIdPath;
            m_pProjectManager->Fire_TargetInstanceAdded(sId, sIdPath);
        }
    }

    return TRUE;
}


//------------------------------------------------------------------*
/**
 * expand from view data.
 *
 *  expand dom from old instanceview.xml data
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CInstanceTree::ExpandFromViewData()
{
    CXMLDocument    tOldDoc;

    CString strFileName;

    strFileName = m_pProjectManager->GetProjectPath() + _T("\\") +
        CE_PROJECT_VIEW_PATH + _T("\\") + m_strFileName;

    if (!FC_Exist(strFileName))
    {
        return;
    }

    if (!tOldDoc.Load(strFileName))
    {
        return;
    }
    
    CXMLNode    tNodeRootNew;
    CXMLNode    tNodeRootOld;

    if(!m_domDocument.GetRootNode(tNodeRootNew) || !tOldDoc.GetRootNode(tNodeRootOld))
    {
        return;
    }

    ExpandParallel(tNodeRootNew, tNodeRootOld);
}


//------------------------------------------------------------------*
/**
 * expand parallel.
 *
 *  copies expand attribute from old node to new node and call function
 *  recursively for children.
 *  stops, if number of children is not equal.
 *
 * @param           rtNodeNew: new node
 * @param           rtNodeNew: old node
 * @return          -
 * @exception       -
 * @see             -
*/
void CInstanceTree::ExpandParallel(CXMLNode& rtNodeNew, CXMLNode& rtNodeOld)
{
    CXMLNode        tNodeChildNew;
    CXMLNode        tNodeChildOld;
    CXMLNodeList    tChildListNew;
    CXMLNodeList    tChildListOld;

    long            lNumChildrenNew;
    long            lNumChildrenOld;
    long            lChild;

    CString         strExpanded;

    if(rtNodeOld.GetAttribute(CE_XMLATTR_EXPANDED, strExpanded))
    {
        rtNodeNew.SetAttribute(CE_XMLATTR_EXPANDED, strExpanded);
    }

    if(!rtNodeNew.GetChildNodeList(tChildListNew) || !rtNodeOld.GetChildNodeList(tChildListOld))
    {
        return;
    }

    lNumChildrenNew = tChildListNew.GetLength();
    lNumChildrenOld = tChildListOld.GetLength();

    if(lNumChildrenNew != lNumChildrenOld)
    {
        return;
    }

    for(lChild = 0; lChild < lNumChildrenNew; ++lChild)
    {
        if(tChildListNew.Item(lChild, tNodeChildNew) && tChildListOld.Item(lChild, tNodeChildOld))
        {
            ExpandParallel(tNodeChildNew, tNodeChildOld);
        }
    }
}


//------------------------------------------------------------------*
/**
 * Replace the node with an error node, because file has not been found,
 * or backparser returned non xml
 *
 *  overwrites base class method: instead of file name the node text
 *  is "Not yet compiled!"
 *
 * @param           pFileInfo: pointer to the source file information
 * @return          successful?
*/
BOOL CInstanceTree::BuildErrorNode(TSourceFileInfo* pFileInfo)
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

    CXMLNode    node;
    CString     strText;

    strText.LoadString(IDS_NOT_COMPILED);
    m_domDocument.CreateNode(node, CE_XMLTAG_NODE);

    node.SetAttribute(CE_XMLATTR_TEXT, strText);
    node.SetAttribute(CE_XMLATTR_TYPE, CE_XMLTYPE_ERRORNODE);
    CString strParserSource;
    strParserSource = pFileInfo->strFileName;
    node.SetAttribute(CE_XMLATTR_PARSERSOURCE, strParserSource);

    CXMLNode parentNode;
    (pFileInfo->xmlNode).GetParentNode(parentNode);
    
    node.SetAttribute(CE_XMLATTR_ID, pFileInfo->strFileName);

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

    CString strFullFileName;
    strFullFileName = m_pProjectManager->GetProjectPath() + 
                      _T("\\") + pFileInfo->strFileName;
    SetSourceTime(pFileInfo, strFullFileName);
    
    return TRUE;  
}



//------------------------------------------------------------------*
/**
 * Determines if the IDPath contains to a node in the instance tree
 * this function checks only the beginning of the path, if it starts
 * with 'projectname'.CLASSVIEW it is a instance tree path,
 * this function does not check if the node for the full path
 * does exist.
 *
 * @param     strIdPath : the id path of the node      
 * @return    TRUE : it is a instance tree path
 *            FALSE: it is no instance tree path
 * @exception       -
 * @see             
*/
BOOL CInstanceTree::IsViewPath(const CString& strIdPath)
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

    CString strExpId = m_pProjectManager->GetProjectName()+_T(".")+ID_PATH_INSTANCEVIEW;
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
 * FireUpdateTree informs all client that some nodes in the instance
 * tree have changed
 *
 * @exception       -
 * @see             
*/
void CInstanceTree::FireUpdateTree(const CString& strChangedNode, enum E_UPDATE_MODE eMode)
{
    if (!m_bInit)
    {
        CComBSTR sChangedNode = strChangedNode;
        m_pProjectManager->Fire_UpdateInstanceTree(sChangedNode, eMode);
    }

}



//------------------------------------------------------------------*
/**
 * ReparseProjFile
 * the project file has changed and needs to be reparsed. Get all
 * necessary changes for the instance view
 * instance view gets target info directly from target tree
 * parsing of project file is not necessary
 *
 * @return          TRUE: on success
 *                  FALSE: else
 * @exception       -
 * @see             
*/
BOOL CInstanceTree::ReparseProjFile()
{
    TSourceFileInfo* pPFInfo;
    SourceFileMap_Lookup(m_pProjectManager->GetProjectFileName(), pPFInfo, m_mapSourceFiles);
    ASSERT(pPFInfo);
    if (pPFInfo)
    {
        SetSourceTime(pPFInfo, m_pProjectManager->GetProjectFileNameWithPath());
    }

    // reparse project file to update attributes in the xml tree
    CXMLNode projectNode;
    if (!m_domDocument.GetRootNode(projectNode))
    {
        return FALSE;
    }

    Reparse4CPAttributes(&projectNode);

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
BOOL CInstanceTree::GetStaticInfos()
{
    m_strInstancesType = con_strInstanceRootType;
    return TRUE;
}




BOOL CInstanceTree::UpdateTargets()
{
    // only do update if something has changed in the target view
    if (!(m_pProjectManager->GetTargetTree())->m_bTargetTreeChanged)
    {
        return TRUE;
    }

    BOOL ret;
    CXMLDocument    tTargetDoc;
    TStringList listNewTargets;
    CXMLNode nodeTargetRoot;
    CXMLNode nodeInstanceRoot;

    // update lists
    TStringList listUpdateChanged;
    TStringList listUpdateNew;
    TStringList listUpdateDeleted;

    (m_pProjectManager->GetTargetTree())->m_bTargetTreeChanged = FALSE;

    // get instance tree root node
    if (!m_domDocument.GetRootNode(nodeInstanceRoot))
    {
        return FALSE;
    }

    // get root node of target view
    IUnknown*   pTmp = NULL;
    HRESULT hr = m_pProjectManager->GetTargetViewDOM(&pTmp);
    if(hr != S_OK)
    {
        return FALSE;
    }
    tTargetDoc.SetIXMLDocument(pTmp);
    pTmp->Release();
    // add target nodes as children
    if(!tTargetDoc.GetRootNode(nodeTargetRoot))
    {
        return FALSE;
    }

    // get new targets from target view
    if (!GetTargetsFromTargetTree(listNewTargets))
    {
        return FALSE;
    }

    // empty xml tree
    nodeInstanceRoot.RemoveAllChilds();

    // mark all nodes not used:
    POSITION posH = m_mapInstanceTargets.GetStartPosition();
    while(posH)
    {
        CString strId;
        TInstanceTargetInfo* pInfo;
        m_mapInstanceTargets.GetNextAssoc(posH, strId, pInfo);
        if (pInfo)
        {
            pInfo->bUsed = FALSE;
        }
    }
    


    // look at all new targets, hang targets into the tree:
    // test if target is a new target or the attributes have changed or it is the same
    // in the last two cases, take the old node and only change attributes if necessary
    POSITION posNewTargets = listNewTargets.GetHeadPosition();
    while(posNewTargets)
    {
        CXMLNode nodeTarget;
        CXMLNode nodeInstance;
        CXMLNode nodeLink;

        // attributes of the target node
        CString     strTText;                // node text
        CString     strTId;                  // node id
        CString     strTType;                // node type (KAD type)
        CString     strTAddress;             // connect info
        CString     strTUserAddress;         // user connect info
        CString     strTConsoleAddress;      // console address
        CString     strTAssignedResource;    // assigned resource
        CString     strTSource;              // source file for add on action
        
        CString     strParseSource;         // parser source help var
        CString     strParseSourceNew;      // parser source
        CString     strInstType;             // type of the instance node

        CString     strIdPathTarg;          // id path of target node in target tree
        CString     strIdPathInst;          // id path of target instance in instance tree
        CString     strTargetId;
        CString     strWaitForTarget;
        
        strIdPathTarg = listNewTargets.GetNext(posNewTargets);

        strTargetId = strIdPathTarg;
        int iFind = strTargetId.ReverseFind(CE_XML_IDPATH_SEP_CHAR);
        ASSERT(iFind>0);
        if (iFind>0)
        {
            strTargetId = strTargetId.Mid(iFind+1);
        }

        // get target node
        ret = nodeTargetRoot.GetNodeFromIdPath(strIdPathTarg, nodeTarget);
        ASSERT(ret);
        if (!ret)
        {
            continue;
        }

        // get attributes of the target node
        nodeTarget.GetAttribute(CE_XMLATTR_TEXT, strTText);
        nodeTarget.GetAttribute(CE_XMLATTR_ID, strTId);
        nodeTarget.GetAttribute(CE_XMLATTR_TYPE, strTType);
        nodeTarget.GetAttribute(CE_XMLATTR_ADDR, strTAddress);
        nodeTarget.GetAttribute(CE_XMLATTR_USERADDR, strTUserAddress);
        nodeTarget.GetAttribute(CE_XMLATTR_CONSOLE_ADDR, strTConsoleAddress);
        nodeTarget.GetAttribute(CE_XMLATTR_RESOURCE, strTAssignedResource);
        nodeTarget.GetAttribute(CE_XMLATTR_SOURCE, strTSource);
        nodeTarget.GetAttribute(CE_XMLATTR_WAIT_TARGET_CONN , strWaitForTarget);

        strInstType = strTType + CE_XML_INST_ADD;
        strParseSource = strTId + _T(".") + CE_INSTANCE_EXT;
        strParseSourceNew = CString(CE_GEN_PATH) + _T("\\") + strTId + _T("\\") + strParseSource;

        // does node already exist in instance tree:
        TInstanceTargetInfo* pInstTargetInfo = NULL;
        CString strLowerId = strTargetId;
        strLowerId.MakeLower();
        if (m_mapInstanceTargets.Lookup(strLowerId, pInstTargetInfo))
        {
            // *** existing node ***
            ASSERT(pInstTargetInfo);
            pInstTargetInfo->bUsed = TRUE;
            nodeInstance = pInstTargetInfo->xmlNode;
            ret = nodeInstanceRoot.AppendChild(nodeInstance, TRUE);
            ASSERT(ret);

            // check if attributes have changed
            // get instance node attributes
            CString strInstText;
            CString strInstId;
            //CString strInstType;
            CString strInstAddress;
            CString strInstUserAddress;
            CString strInstConsoleAddress;
            CString strInstAssignedResource;
            CString strInstSource;

            nodeInstance.GetAttribute(CE_XMLATTR_TEXT, strInstText);
            nodeInstance.GetAttribute(CE_XMLATTR_ID, strInstId);
            //nodeInstance.GetAttribute(CE_XMLATTR_TYPE, strInstType); // type can't change
            nodeInstance.GetAttribute(CE_XMLATTR_ADDR, strInstAddress);
            nodeInstance.GetAttribute(CE_XMLATTR_USERADDR, strInstUserAddress);
            nodeInstance.GetAttribute(CE_XMLATTR_CONSOLE_ADDR, strInstConsoleAddress);
            nodeInstance.GetAttribute(CE_XMLATTR_RESOURCE, strInstAssignedResource);
            nodeInstance.GetAttribute(CE_XMLATTR_SOURCE, strInstSource);
            
            if ((strInstText.CompareNoCase(strTText)!=0)
                || (strInstId.CompareNoCase(strTId)!=0)
                //|| (strInstType.CompareNoCase(strType)!=0)
                || (strInstAddress.CompareNoCase(strTAddress)!=0)
                || (strInstConsoleAddress.CompareNoCase(strTConsoleAddress)!=0)
                || (strInstAssignedResource.CompareNoCase(strTAssignedResource)!=0)
                || (strInstSource.CompareNoCase(strTSource) != 0))
            {
                // node has changed -> update nodes
                nodeInstance.SetAttribute(CE_XMLATTR_TEXT, strTText);
                nodeInstance.SetAttribute(CE_XMLATTR_ID, strTId);
                //nodeInstance.SetAttribute(CE_XMLATTR_TYPE, strType);
                nodeInstance.SetAttribute(CE_XMLATTR_ADDR, strTAddress);
                nodeInstance.SetAttribute(CE_XMLATTR_USERADDR, strTUserAddress);
                nodeInstance.SetAttribute(CE_XMLATTR_CONSOLE_ADDR, strTConsoleAddress);
                nodeInstance.SetAttribute(CE_XMLATTR_RESOURCE, strTAssignedResource);
                nodeInstance.SetAttribute(CE_XMLATTR_SOURCE, strTSource);

                listUpdateChanged.AddTail(pInstTargetInfo->strIdPath);
            }
           
        }
        else // *** new node ***
        {
            if(!m_domDocument.CreateNode(nodeInstance, CE_XMLTAG_NODE))
            {
                continue;
            }

            // set attributes of target instance node
            nodeInstance.SetAttribute(CE_XMLATTR_TEXT, strTText);
            nodeInstance.SetAttribute(CE_XMLATTR_ID, strTId);
            nodeInstance.SetAttribute(CE_XMLATTR_ADDR, strTAddress);
            nodeInstance.SetAttribute(CE_XMLATTR_USERADDR, strTUserAddress);
            nodeInstance.SetAttribute(CE_XMLATTR_CONSOLE_ADDR, strTConsoleAddress);
            nodeInstance.SetAttribute(CE_XMLATTR_RESOURCE, strTAssignedResource);
            nodeInstance.SetAttribute(CE_XMLATTR_TARGET_TYPE, strTType);
            nodeInstance.SetAttribute(CE_XMLATTR_SOURCE, strTSource);

            nodeInstance.SetAttribute(CE_XMLATTR_TYPE, strInstType);
            if (!strWaitForTarget.IsEmpty())
            {
                nodeInstance.SetAttribute(CE_XMLATTR_WAIT_TARGET_CONN, strWaitForTarget);
            }

            // create new instance info struct
            ASSERT(pInstTargetInfo==NULL);
            pInstTargetInfo = new TInstanceTargetInfo;
            ASSERT(pInstTargetInfo);
            if (!pInstTargetInfo)
            {
                continue;
            }

            ret = nodeInstanceRoot.AppendChild(nodeInstance, TRUE);
            ASSERT(ret);

            nodeInstance.GetIdPath(strIdPathInst);
            pInstTargetInfo->strIdPath = strIdPathInst;
            pInstTargetInfo->strTargetId = strTId;
            pInstTargetInfo->xmlNode = nodeInstance;
            pInstTargetInfo->bUsed = TRUE;

            strLowerId = strTId;
            strLowerId.MakeLower();

            m_mapInstanceTargets.SetAt(strLowerId, pInstTargetInfo);

            // add to update list (new node)
            listUpdateNew.AddTail(strIdPathInst);

           

            // now create target link node: the link that will be replaced by compiler outpu
            if((!m_domDocument.CreateNode(nodeLink, CE_XMLTAG_LINK))
                ||(!nodeInstance.AppendChild(nodeLink)) )
            {
                continue;
            }

            nodeLink.SetAttribute(CE_XMLATTR_PARSERSOURCE, strParseSourceNew);
            nodeLink.SetAttribute(CE_XMLATTR_ID, strTId);

            TSourceFileInfo* pFileInfo;
            pFileInfo = CreateSourceFileInfo(strParseSourceNew, nodeLink, "");
            SourceFileMap_SetAt(strParseSourceNew, pFileInfo, m_mapSourceFiles);

            AddSourceFileToReparse(strParseSourceNew);
        }
    }

   
    

    // find deleted nodes:
    posH = m_mapInstanceTargets.GetStartPosition();
    while(posH)
    {
        CString strId;
        TInstanceTargetInfo* pInfo;
        m_mapInstanceTargets.GetNextAssoc(posH, strId, pInfo);
        if (pInfo)
        {
            if (pInfo->bUsed == FALSE)
            {
                listUpdateDeleted.AddTail(pInfo->strIdPath);
                delete pInfo;
                m_mapInstanceTargets.RemoveKey(strId);
            }
        }
    }

    if (m_bInit)
    {
        return TRUE;
    }

    // send update messages
    POSITION lPos;
    lPos = listUpdateDeleted.GetHeadPosition();
    while(lPos)
    {
        CString strIdPath = listUpdateDeleted.GetNext(lPos);
        if (!strIdPath.IsEmpty())
        {
            FireUpdateTree(strIdPath, eUpdateDelete);
            CString strId = strIdPath;
            int iFind = strId.ReverseFind(CE_XML_IDPATH_SEP_CHAR);
            ASSERT(iFind>0);
            if (iFind>0)
            {
                strId = strId.Mid(iFind+1);
            }
            CComBSTR sId = strId;
            m_pProjectManager->Fire_TargetInstanceRemoved(sId);
        }
    }
    lPos = listUpdateChanged.GetHeadPosition();
    while(lPos)
    {
        CString strIdPath = listUpdateChanged.GetNext(lPos);
        if (!strIdPath.IsEmpty())
        {
            FireUpdateTree(strIdPath, eUpdateAttributes);
            CString strId = strIdPath;
            int iFind = strId.ReverseFind(CE_XML_IDPATH_SEP_CHAR);
            ASSERT(iFind>0);
            if (iFind>0)
            {
                strId = strId.Mid(iFind+1);
            }
            CComBSTR sId = strId;
            CComBSTR sIdPath = strIdPath;
            m_pProjectManager->Fire_TargetInstanceAdded(sId, sIdPath);
        }
    }
    lPos = listUpdateNew.GetHeadPosition();
    while(lPos)
    {
        CString strIdPath = listUpdateNew.GetNext(lPos);
        if (!strIdPath.IsEmpty())
        {
            FireUpdateTree(strIdPath, eUpdateInsert);
            CString strId = strIdPath;
            int iFind = strId.ReverseFind(CE_XML_IDPATH_SEP_CHAR);
            ASSERT(iFind>0);
            if (iFind>0)
            {
                strId = strId.Mid(iFind+1);
            }
            CComBSTR sId = strId;
            CComBSTR sIdPath = strIdPath;
            m_pProjectManager->Fire_TargetInstanceAdded(sId, sIdPath);
        }
    }

    Reparse(FALSE);
    
    return TRUE;
}



//------------------------------------------------------------------*
/**
 * get target id paths for all targets from target tree.
 *
 * @param           [in] rtIdMap: id map with id -> id path data
 * @see             
*/
BOOL CInstanceTree::GetTargetsFromTargetTree(TStringList& listTargets)
{
    // get all targets as id paths
    SAFEARRAY* parrTargets;
    m_pProjectManager->getAllTargetsAsIdPaths(&parrTargets);

    BSTR HUGEP *pbstr;
    BSTR bstr;
    HRESULT hr;
    unsigned long i;

    // Get a pointer to the elements of the array.
    hr = ::SafeArrayAccessData(parrTargets, (void HUGEP**)&pbstr);
    if(FAILED(hr))
    {
        return FALSE;
    }

    CString strIdPath;
    for (i = 0; i < (parrTargets->rgsabound[0]).cElements; i++)
    {
        bstr = pbstr[i];
        strIdPath = (CString)bstr;
        listTargets.AddTail(strIdPath);
    }

    ::SafeArrayUnaccessData(parrTargets);		
    ::SafeArrayDestroy(parrTargets);
 
    return TRUE;
}
