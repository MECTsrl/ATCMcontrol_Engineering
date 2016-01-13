
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "NavTreeControl.h"
#include "SBLStack.h"
#include "KADHelper/KADInfo.h"
#include "XMLHelper/IdPathHelp.h"
#include "CESysDef.h"
#include "ComHelper.h"
#include "CECompman.h"
#include "CEAddonAction.h"
#include "DragItem.h"
#include "fc_tools/fc_string.h"
#include "resource.h"
#include "MsgWnd.h"
#include "TargetSet.h"
#include "download.h"
#include "cemain.h"
#include "cefilehandler.h"
#include "fc_tools/fc_string.h"
#include "MainFrm.h"
#include "DragDropHelper\PouVarObject.h"
#include "CEDragDrop.h"
#include "Settings.h"
#include "traceif.h"

#include "geneddoc.h"

#include <atlbase.h>

#define ON_DEMAND_NODE (-90)
#define ON_DEMAND_CHILD (-91)


const int con_iScrollTimer = 5;

//----  Static Initializations:   ----------------------------------*

// construction / destruction
CNavTreeCtrl::CNavTreeCtrl() :
  m_bInitialized(FALSE),
  m_bFilter1(FALSE),
//  m_pRegMatch1(NULL),
  m_bFilter2(FALSE),
//  m_pRegMatch2(NULL),
//  m_iGroups1(0),
//  m_iGroups2(0),
  m_bUseCase(FALSE),
  m_pKADInfo(NULL),
  m_pXMLDoc(NULL),
  m_iSelCat(-1),
  m_iSelText(-1),
  m_pCatFilter(NULL),
  m_pDragImageList(NULL),
  m_bDragging(FALSE),
  m_uiCBXMLNodeFormat(0),
  m_uiCBXMLFormat(0),
  m_iScrollCounter(0),
  m_tBgColFilter(RGB(240, 240, 240)),
  m_pNavigator(0)
{
}

CNavTreeCtrl::~CNavTreeCtrl()
{
    m_tDropTarget.Revoke();
    UnloadXML();
    ResetFilters();
    if(m_pDragImageList)
    {
        delete m_pDragImageList;
        m_pDragImageList = NULL;
    }
}


//------------------------------------------------------------------*
/**
 * get view index.
 *
 *  base class returns -1
 *
 * @param           -
 * @return          view index.
 * @exception       -
 * @see             -
*/
int CNavTreeCtrl::GetViewIndex()
{
    return(-1);
}

BEGIN_MESSAGE_MAP(CNavTreeCtrl, CTreeCtrlEx)
	//{{AFX_MSG_MAP(CNavTreeCtrl)
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()		// SHIFT_F10 15.12.04 SIS
	ON_WM_DROPFILES()
	//}}AFX_MSG_MAP
    ON_COMMAND_RANGE(ID_NAV_CONTEXT_BASE, ID_NAV_CONTEXT_END, OnContextMenuCommand)
END_MESSAGE_MAP()

//------------------------------------------------------------------*
/**
 * initialization.
 *
 *   set KAD-info, set tree control style, set image list
 *   initialize tree with "No Project Loaded"
 *
 * @param           pKADInfo: pointer to KAD information
*/
void CNavTreeCtrl::Init(CKADInfo* pKADInfo)
{
    m_pKADInfo = pKADInfo;
    m_pCatFilter = m_pKADInfo->GetFilterInfo(GetViewIndex());
    if(!m_bInitialized)
    {
        ModifyStyle(0, TVS_EDITLABELS | TVS_SHOWSELALWAYS);
        // attach navigator image list
        m_pKADInfo->CopyImageList(m_tImageList);
        m_tImageList.SetBkColor(CLR_NONE);
        SetImageList(&m_tImageList, TVSIL_NORMAL);
        m_tStateImageList.Create(IDB_STATEIMAGELIST, 16, 0, RGB(255, 0, 255));
        SetImageList(&m_tStateImageList, TVSIL_STATE);
        InitFilters();
    }
    BuildTree();
    m_tDropTarget.SetTreeCtrl(this);
    m_tDropTarget.Register(this);
    m_uiCBXMLNodeFormat = ::RegisterClipboardFormat(CB_FORMAT_4CXMLNODE);
    m_uiCBXMLFormat = ::RegisterClipboardFormat(CB_FORMAT_4CXML);
    m_bInitialized = TRUE;
    m_pNavigator = dynamic_cast<CNavigator*>(CEGetDockingTool(CE_COMPMAN_NAV, true));
    ASSERT(m_pNavigator);
}



//------------------------------------------------------------------*
/**
 * set project name and path.
 *
*/
BOOL CNavTreeCtrl::Load()
{
    CComBSTR            sTmp;
    CComPtr<IUnknown>   pTmp;

    pTmp.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));

    m_pProjMan = pTmp;
    m_pProjInfo = m_pProjMan;

    ASSERT(m_pProjMan);
    ASSERT(m_pProjInfo);

    m_pProjInfo->getProjectName(&sTmp);
    m_strProjectName = sTmp;
    sTmp.Empty();
    m_pProjInfo->getProjectPath(&sTmp);
    m_strProjectPath = sTmp;

    return TRUE;
}


//------------------------------------------------------------------*
/**
 * build complete tree.
 *
 *  after tree is builded use Update.. commands and Invalidate() to
 *  reflect changes in XML document.
 *  do not call the method with new XML document unless old is unloaded
 *
 * @param           pXMLDoc: pointer to XML document or NULL if document
 *                                is already loaded, default is NULL
 * @return          successful?
*/
BOOL CNavTreeCtrl::BuildTree()
{
    CString strType;
    CString strText;
    CXMLNode    tNodeRoot;
    int         iImageId;
    int         iOverlayId;

    ClearTree(FALSE);

    if(m_pXMLDoc)
    {
        if(!m_pXMLDoc->GetRootNode(tNodeRoot))
        {
            return FALSE;
        }
        tNodeRoot.GetAttribute(CE_XMLATTR_TEXT, strText);

        VERIFY(GetImageIds(&tNodeRoot, iImageId, iOverlayId));
        m_tCursorRoot = AddItemTail(strText, GetRootItem(), tNodeRoot, iImageId);
        if(iOverlayId > 0)
        {
            SetItemState(m_tCursorRoot, INDEXTOOVERLAYMASK(iOverlayId), TVIS_OVERLAYMASK);
        }
        // run over all children and start recursive fill node method (no filter)
        if(IsFilterActive())
        {
            SetBkColor(m_tBgColFilter);
            FilterNodes(&tNodeRoot);
        }
        else
        {
            SetBkColor(-1);
        }
        FillChildNodes(m_tCursorRoot);
        // call recursive expanding of nodes using DOM for root
        ExpandNodeFromDom(m_tCursorRoot);
        // expand root
        ExpandTreeItem(m_tCursorRoot, TVE_EXPAND);
        // check source state recursively
        CheckNodeSourceState(m_tCursorRoot);
    }
    else
    {
        m_tCursorRoot = GetRootItem().AddTail("No Project Loaded", IID_PROJECT);
        m_tCursorRoot.SetData(0);
        m_tCursorRoot.Expand();
    }
    m_tCursorRoot.Select();
    Invalidate();
    return(TRUE);
}


//------------------------------------------------------------------*
/**
 * public check source state function.
 *   calls protected method for visible items.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             CheckNodeSourceState()
*/
void CNavTreeCtrl::CheckSourceState()
{
    //
    CTreeCursor tTreeCursor;
    tTreeCursor = GetFirstVisibleItem();
    while(HTREEITEM(tTreeCursor))
    {
        CheckNodeSourceState(tTreeCursor, FALSE);
        tTreeCursor = GetNextVisibleItem(tTreeCursor);
    }
}

//------------------------------------------------------------------*
/**
 * check source state of node.
 *
 * @param           rtTreeCursor: cursor to tree item
 * @param           bRecursive: recursive (default is TRUE)
 * @return          -
 * @exception       -
 * @see             -
*/
void CNavTreeCtrl::CheckNodeSourceState(CTreeCursor& rtTreeCursor, BOOL bRecursive /*=TRUE*/)
{
    BOOL    bExpanded = FALSE;

    CCEFileHandler* pFileHandler = ((CCEMAINApp*)AfxGetApp())->GetFileHandler();
    if(!pFileHandler)
    {
        return;
    }

    CString     strSource;
    CXMLNode*   pNode = (CXMLNode*)rtTreeCursor.GetData();
    if(pNode)
    {
//        if(rtTreeCursor.GetState(TVIS_EXPANDED) & TVIS_EXPANDED)
//        {
//            bExpanded = TRUE;
//        }
        bExpanded = IsXMLNodeExpanded(pNode);
        if(pNode->GetAttribute(CE_XMLATTR_SOURCE, strSource))
        {
            EFileStatus eStatus = pFileHandler->GetFileStatus(strSource);
            SetItemState(rtTreeCursor, INDEXTOSTATEIMAGEMASK(eStatus), TVIS_STATEIMAGEMASK);
        }
    }

    if(bRecursive && bExpanded)
    {
        CTreeCursor tTreeCursorChild = rtTreeCursor.GetChild();
        if(HTREEITEM(tTreeCursorChild))
        {
            do
            {
                CheckNodeSourceState(tTreeCursorChild, bRecursive);
                tTreeCursorChild = tTreeCursorChild.GetNextSibl();
            } while(HTREEITEM(tTreeCursorChild));
        }
    }
}


//------------------------------------------------------------------*
/**
 * get image and overlay ids.
 *
 *  if no overlay image:
 *  image id = base id for type + state idx
 *
 *  else
 *  image id = base id for type
 *
 * @param           pNode: pointer to XML node
 * @param           riImageId: image id
 * @param           riOverlay: overlay image id
 * @return          successful?
 * @see             CKADInfo::GetImageIdsForType()
*/
BOOL CNavTreeCtrl::GetImageIds(CXMLNode* pNode, int& riImageId, int& riOverlay)
{
    CString strType;
    CString strState;
    int     iImageId = 0;
    BOOL    bSuccessful;

    ASSERT(pNode);
    if(pNode)
    {
        pNode->GetAttribute(CE_XMLATTR_TYPE, strType);

        pNode->GetAttribute(CE_XMLATTR_STATE, strState);
        bSuccessful = m_pKADInfo->GetImageIdsForType(strType, strState, riImageId, riOverlay);

        return(bSuccessful);
    }
    return FALSE;
}


//------------------------------------------------------------------*
/**
 * set XML pointers
 *
 *  if m_pXMLDoc is set once it may not be changed by subsequent calls ->
 *  if XML pointer is already set, return FALSE
 *
 * @param           pXMLDoc: pointer to XML document
 * @return          successful?
*/
BOOL CNavTreeCtrl::InitializeDOM(CXMLDocument* pXMLDoc /*=NULL*/)
{
    // if document already loaded -> return FALSE
    if(pXMLDoc)
    {
        if(m_pXMLDoc)
        {
            return FALSE;
        }
        m_pXMLDoc = pXMLDoc;
    }

    return TRUE;
}


//------------------------------------------------------------------*
/**
 * unload project data.
 *
 *  clear tree, unload XML data and build tree again to get "No Project Loaded"
 *
 * @see             ClearTree(), UnloadXML(), BuildTree()
*/
void CNavTreeCtrl::Unload()
{
    ClearTree();
    //UnloadXML();
    BuildTree();

    m_pProjMan.Release();
    m_pProjInfo.Release();
}


void CNavTreeCtrl::ClearStates()
{
    CXMLNode    tRootNode;

    if(m_pXMLDoc)
    {
        if(m_pXMLDoc->GetRootNode(tRootNode))
        {
            ClearStates(tRootNode);
        }
    }
}

//------------------------------------------------------------------*
/**
 * clear states.
 *  
 *  remove all state attributes.
*/
void CNavTreeCtrl::ClearStates(CXMLNode& rtNode)
{
    CXMLNodeList    tNodeList;
    long            lNumChildren;
    long            lChild;
    CXMLNode        tNodeChildTmp;

    // remove Attribute STATE
    rtNode.RemoveAttribute(CE_XMLATTR_STATE);
    // remove Attribute STATETEXT
    rtNode.RemoveAttribute(CE_XMLATTR_STATETEXT);

    // run recursively over all children and call ClearStates
    if(rtNode.GetChildNodeList(tNodeList))
    {
        lNumChildren = tNodeList.GetLength();
        for(lChild = 0; lChild < lNumChildren; ++lChild)
        {
            // get child
            tNodeList.Item(lChild, tNodeChildTmp);
            ClearStates(tNodeChildTmp);
        }
    }
}

//------------------------------------------------------------------*
/**
 * release XML data.
 *
 *  reset XML node and document
 *
 * @see             Unload()
*/
void CNavTreeCtrl::UnloadXML()
{
    if(m_pXMLDoc)
    {
        delete m_pXMLDoc;
        m_pXMLDoc = NULL;
    }
}


//------------------------------------------------------------------*
/**
 * delete all children of a tree item
 *
 *  used in UpdateNode()
 *
 * @param           rtTreeCursor: parent tree item whose children are to be deleted
 * @return          successful?
 * @see             UpdateNode()
*/
BOOL CNavTreeCtrl::DeleteTreeNodeChildren(CTreeCursor& rtTreeCursor)
{
    CTreeCursor tTreeCursor;
    CTreeCursor tNextSibling;

    // run over all children from rtTreeCursor
    tTreeCursor = rtTreeCursor.GetChild();
    if(HTREEITEM(tTreeCursor))
    {
        do
        {
            tNextSibling = tTreeCursor.GetNextSibl();
            DeleteTreeItem(tTreeCursor);
            tTreeCursor = tNextSibling;
        } while(HTREEITEM(tTreeCursor));
    }
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * update node.
 *
 *  gets tree item and corresponding XML node and
 *  calls protected method UpdateTreeNode()
 *
 * @param           crstrIdPath: ID-path to node
 * @return          successful?
 * @see             UpdateTreeNode()
*/
BOOL CNavTreeCtrl::UpdateNode(const CString& crstrIdPath)
{
    // this may take some time: show wait cursor.
    CWaitCursor tWaitCursor;

    // find tree item
    CTreeCursor     tCursor;
    CTreeCursor     tCursorParent;
    CXMLNode        tNode;
    CXMLNode        tNodeRoot;

    TRACE("Update Node: %s\n", crstrIdPath);
    ASSERT(m_pXMLDoc);
    if(!m_pXMLDoc)
    {
        return FALSE;
    }

    if(!m_pXMLDoc->GetRootNode(tNodeRoot))
    {
        return FALSE;
    }

    if(GetTreeCursorFromNodeIdPath(crstrIdPath, tCursor))
    {
        if(tNodeRoot.GetNodeFromIdPath(crstrIdPath, tNode))
        {
            // ensure association between tree item and XML node
            UpdateItem(tCursor, tNode);

            // update tree from XML node
            UpdateTreeNode(tCursor, tNode);
        }
    }
    return(TRUE);
}


//------------------------------------------------------------------*
/**
 * update attributes.
 *
 * @param           crstrIdPath: id path to node.
 * @return          -
 * @exception       -
 * @see             -
*/
BOOL CNavTreeCtrl::UpdateAttributes(const CString& crstrIdPath)
{
    CTreeCursor tTreeCursor;
    if(GetTreeCursorFromNodeIdPath(crstrIdPath, tTreeCursor))
    {
        UpdateItem(tTreeCursor);
        return TRUE;
    }
    return FALSE;
}


//------------------------------------------------------------------*
/**
 * update delete.
 *
 *  update message from project manager to delete tree item.
 *  XML node is already deleted.
 *
 * @param           crstrIdPath: id path to node
 * @return          successful?
 * @exception       -
 * @see             UpdateInsert()
*/
BOOL CNavTreeCtrl::UpdateDelete(const CString& crstrIdPath)
{
    CTreeCursor tTreeCursor;
    if(GetTreeCursorFromNodeIdPath(crstrIdPath, tTreeCursor))
    {
        DeleteTreeItem(tTreeCursor);
        return TRUE;
    }
    return FALSE;
}


//------------------------------------------------------------------*
/**
 * update insert.
 *
 *  update message from project manager to insert tree item.
 *
 * @param           crstrIdPath: id path to node.
 * @return          successful?
 * @exception       -
 * @see             UpdateDelete()
*/
BOOL CNavTreeCtrl::UpdateInsert(const CString& crstrIdPath)
{
    CTreeCursor tParent;
    CXMLNode    tNodeNew;
    CString     strIdPathParent(crstrIdPath);
    CString     strId;

    // get id path of parent by stripping id
    if(!StripIdFromIdPath(strIdPathParent, strId, TRUE))
    {
        return FALSE;
    }

    // if parent item not found -> return false
    if(!GetTreeCursorFromNodeIdPath(strIdPathParent, tParent))
    {
        return FALSE;
    }

    if(HTREEITEM(tParent))
    {
        if(FindTreeNode(crstrIdPath, tNodeNew))
        {
            CXMLNode tNodeParent;
            if(tNodeNew.GetParentNode(tNodeParent))
            {
                if(IsFilterActive())
                {
                    FilterNodes(&tNodeParent);
                }
                // insert new nodes recursiveley
                UpdateTreeNodeInsertRun(tParent, tNodeParent);
            }
        }
        else
        {
            return FALSE;
        }
    }
    // ensure that root node is expanded
    ExpandTreeItem(m_tCursorRoot);
    return TRUE;
}

//------------------------------------------------------------------*
/**
 * update source state.
 *
 * @param           crstrIdpath: id path to source node.
 * @return          successful?
 * @exception       -
 * @see             -
*/
//BOOL CNavTreeCtrl::UpdateSourceState(const CString& crstrIdPath)
//{
//    CTreeCursor tTreeCursor;
//    if(GetTreeCursorFromNodeIdPath(crstrIdPath, tTreeCursor))
//    {
//        CheckNodeSourceState(tTreeCursor, FALSE);
//        return TRUE;
//    }
//    return FALSE;
//}

//------------------------------------------------------------------*
/**
 * update tree node.
 *
 *  - filter nodes (if necessary)
 *  - delete all tree items not existing any more (UpdateTreeNodeDeleteRun())
 *  - insert new tree items (UpdateTreeNodeInsertRun())
 *
 *  By performing update in delete and insert run it was possible to avoid
 *  deletion of all tree items and building up the tree anew.
 *
 * @param           rtTreeCursor: cursor to tree item
 * @param           rtNode: corresponding xml node
 * @return          -
 * @exception       -
 * @see             -
*/
void CNavTreeCtrl::UpdateTreeNode(CTreeCursor& rtTreeCursor, CXMLNode& rtNode)
{
#ifdef _DEBUG
    CString strCursorId = rtTreeCursor.GetText();
    CString strNodeId;
    rtNode.GetAttribute(CE_XMLATTR_TEXT, strNodeId);
    TRACE("UpdateTreeNode: Cursor: %s, Node: %s\n", strCursorId, strNodeId);
#endif

    if(IsFilterActive())
    {
        FilterNodes(&rtNode);
    }

    // remove old items
    UpdateTreeNodeDeleteRun(rtTreeCursor, rtNode);

    // insert new items and set insert path
    UpdateTreeNodeInsertRun(rtTreeCursor, rtNode);
}

//------------------------------------------------------------------*
/**
 * update tree node first run.
 *
 *  run over all tree items and delete those that do not correspond
 *  to XML-nodes.
 *
 *  precondition: if filter is set, FilterNodes() has to be called before
 *
 * @param           rtTreeCursor: cursor to tree item
 * @param           rtNode: corresponding xml node
 * @return          -
 * @exception       -
 * @see             -
*/
void CNavTreeCtrl::UpdateTreeNodeDeleteRun(CTreeCursor& rtTreeCursor, CXMLNode& rtNode)
{
    // first run:
    // run over all children of rtTreeCursor
    CTreeCursor     tChildCursor;
    CTreeCursor     tNextCursor;
    CXMLNode        tChild;
    CXMLNode        tStartNode;

    CTreeCursor     tStartCursor;
    CXMLNodeList    tChildList;
    CXMLNode        tChildNode;
    long            lNumChildren;
    long            lChild;
    long            lStartChild = 0;
    BOOL            bFound;


    if(!rtNode.GetChildNodeList(tChildList))
    {
        DeleteTreeNodeChildren(rtTreeCursor);
        return;
    }

    tChildCursor = rtTreeCursor.GetChild();
    while(HTREEITEM(tChildCursor))
    {
        tNextCursor = tChildCursor.GetNextSibl();

        // now run over xml nodes and find corresponding
        bFound = FALSE;
        lNumChildren = tChildList.GetLength();
        for(lChild = lStartChild; lChild < lNumChildren; ++lChild)
        {
            // get child
            tChildList.Item(lChild, tChildNode);
            if(IsNodeToBeInserted(tChildNode))
            {
                if(IsCorresponding(tChildCursor, tChildNode))
                {
                    bFound = TRUE;
                    lStartChild = lChild + 1;
                    break;
                }
            }
        }

        if(bFound)
        {
            UpdateItem(tChildCursor, tChildNode);
            UpdateTreeNodeDeleteRun(tChildCursor, tChildNode);
        }
        else
        {
            DeleteTreeItem(tChildCursor);
        }

        tChildCursor = tNextCursor;
    }
}



//------------------------------------------------------------------*
/**
 * update tree node second run.
 *
 *  run over all tree items and XML-nodes and insert new items for
 *  nodes that do not correspond to a tree item.
 *
 *  precondition: if filter is set, FilterNodes() has to be called before
 *
 * @param           rtTreeCursor: cursor to tree item
 * @param           rtNode: corresponding xml node
 * @return          -
 * @exception       -
 * @see             -
*/
void CNavTreeCtrl::UpdateTreeNodeInsertRun(CTreeCursor& rtTreeCursor, CXMLNode& rtNode)
{
    // second run:
    // run over all children of pNode
    CTreeCursor     tChildCursor;
    CTreeCursor     tNextCursor;
    CXMLNode        tChild;
    CXMLNode        tStartNode;

    CTreeCursor     tStartCursor;
    CXMLNodeList    tChildList;
    CXMLNode        tChildNode;
    long            lNumChildren;
    long            lChild;
    long            lStartChild = 0;
    BOOL            bFound;

    if(!rtNode.GetChildNodeList(tChildList))
    {
        return;
    }

    tChildCursor = rtTreeCursor.GetChild();

    lNumChildren = tChildList.GetLength();
    for(lChild = 0; lChild < lNumChildren; ++lChild)
    {
        // get child
        tChildList.Item(lChild, tChildNode);
        if(IsNodeToBeInserted(tChildNode))
        {
            // now run over xml nodes and find corresponding
            bFound = FALSE;
            if(HTREEITEM(tStartCursor))
            {
                tChildCursor = tStartCursor.GetNextSibl();
            }
            while(HTREEITEM(tChildCursor))
            {
                tNextCursor = tChildCursor.GetNextSibl();
                if(IsCorresponding(tChildCursor, tChildNode))
                {
                    tStartCursor = tChildCursor;
                    bFound = TRUE;
                    break;
                }
                tChildCursor = tNextCursor;
            }
            if(!bFound)
            {
                CTreeCursor tNewCursor;
                tNewCursor = InsertNewItemAfter(rtTreeCursor, tStartCursor, tChildNode);
                FillChildNodes(tNewCursor);
                ExpandNodeFromDom(tNewCursor);
                tStartCursor = tNewCursor;
            }
            else
            {
                UpdateTreeNodeInsertRun(tChildCursor, tChildNode);
            }
        }
    }
}


//------------------------------------------------------------------*
/**
 * is node to be inserted?
 *
 * @param           rtNode: XML node to test
 * @return          is node visible and has to be inserted in form of a tree item?
 * @exception       -
 * @see             -
*/
BOOL CNavTreeCtrl::IsNodeToBeInserted(CXMLNode& rtNode)
{
    BOOL    bVisible;
    BOOL    bFilterPassed;
    CString strType;
    CString strVis;
    CString strFilterPassed;

    rtNode.GetAttribute(CE_XMLATTR_TYPE, strType);
    rtNode.GetAttribute(CE_XMLATTR_VISIBILITY, strVis);
    bFilterPassed = rtNode.GetAttribute(CE_XMLATTR_FILTER_VIS, strFilterPassed);
    if(strVis != CE_XMLATTR_FALSE_VAL && m_pKADInfo->IsTypeVisible(strType))
    {
        bVisible = TRUE;
    }
    else
    {
        bVisible = FALSE;
    }

    if(bVisible && !(IsFilterActive() && !bFilterPassed))
    {
        return TRUE;
    }
    return FALSE;
}

//------------------------------------------------------------------*
/**
 * update item.
 *
 *  exchange node and call UpdateItem(1)
 *  throws memory exception, if new node cannot be created
 *
 * @param           rtTreeCursor: cursor to tree item
 * @param           rtNode: xml node
 * @return          -
 * @exception       -
 * @see             UpdateItem(1)
*/
void CNavTreeCtrl::UpdateItem(CTreeCursor& rtTreeCursor, CXMLNode& rtNode)
{
    if(!HTREEITEM(rtTreeCursor))
    {
        return;
    }

    CXMLNode*   pNode;
    CXMLNode*   pNodeOld;

    pNode = new CXMLNode;
    ASSERT(pNode);
    if(!pNode)
    {
        ::AfxThrowMemoryException();
    }
    *pNode = rtNode;

    pNodeOld = (CXMLNode*)rtTreeCursor.GetData();
    if(pNodeOld)
    {
        delete pNodeOld;
    }

    rtTreeCursor.SetData((DWORD)pNode);

    UpdateItem(rtTreeCursor);
}



//------------------------------------------------------------------*
/**
 * update item.
 *
 * @param           rtTreeCursor: cursor to tree item to be updated
 * @return          -
 * @exception       -
 * @see             UpdateItem(2)
*/
void CNavTreeCtrl::UpdateItem(CTreeCursor& rtTreeCursor)
{
    if(!HTREEITEM(rtTreeCursor))
    {
        return;
    }

    int iImageId, iOverlayId;
    CXMLNode*   pNode;
    CString     strNodeText;
    CString     strCursorText(rtTreeCursor.GetText());

    pNode = (CXMLNode*)rtTreeCursor.GetData();

    ASSERT(pNode);
    if(pNode)
    {
        pNode->GetAttribute(CE_XMLATTR_TEXT, strNodeText);
        CString strStateText;
        if(pNode->GetAttribute(CE_XMLATTR_STATETEXT, strStateText))
        {
            strNodeText += _T(": ");
            strNodeText += strStateText;
        }
        if(strNodeText.Compare(strCursorText) != 0)
        {
            rtTreeCursor.SetText(strNodeText);
        }

        // add new tree control item for file
        GetImageIds(pNode, iImageId, iOverlayId);
        if(iOverlayId >= 0)
        {
            SetItemState(rtTreeCursor, INDEXTOOVERLAYMASK(iOverlayId), TVIS_OVERLAYMASK);
        }
        rtTreeCursor.SetImage(iImageId, iImageId);
    }
}


//------------------------------------------------------------------*
/**
 * insert new item after.
 *
 * @param           rtCursorParent: cursor to parent item
 * @param           rtCursorChildBefore: cursor to child after which the new item
 *                                            will be inserted
 * @param           rtNodeToInsert: xml node to insert
 * @return          cursor to new item.
 * @exception       -
 * @see             -
*/
CTreeCursor CNavTreeCtrl::InsertNewItemAfter(CTreeCursor& rtCursorParent, CTreeCursor& rtCursorChildBefore, CXMLNode& rtNodeToInsert)
{
    CString     strText;
    CTreeCursor tCursorNew;

    rtNodeToInsert.GetAttribute(CE_XMLATTR_TEXT, strText);

#ifdef _DEBUG
    CString strTextBefore;
    CString strTextParent;
    strTextParent = rtCursorParent.GetText();
    if(HTREEITEM(rtCursorChildBefore))
    {
        strTextBefore = rtCursorChildBefore.GetText();
    }
    //TRACE("InsertNewItemAfter: %s (Parent = %s, Before = %s)\n", strText, strTextParent, strTextBefore);
#endif

    if(HTREEITEM(rtCursorChildBefore))
    {
        tCursorNew = InsertItemAfter(strText, rtCursorParent, rtCursorChildBefore, rtNodeToInsert);
    }
    else
    {
        tCursorNew = InsertItemAfter(strText, rtCursorParent, TVI_FIRST, rtNodeToInsert);
    }
    rtNodeToInsert.GetIdPath(m_strInsertIdPath);
    UpdateItem(tCursorNew, rtNodeToInsert);
    return tCursorNew;
}

//------------------------------------------------------------------*
/**
 * insert item after.
 *
 *  base class calls InsertAfter() of parent tree item and sets pointer
 *  to xml node.
 *
 * @param           crstrText: text for new item
 * @param           rtCursorParent: cursor to parent item
 * @param           hItemAfter: tree item after which the new item is inserted
 * @param           rtNode: corresponding xml node
 * @param           iImageIdx: image index of new item
 * @return          cursor to new tree item
 * @exception       -
 * @see             -
*/
CTreeCursor CNavTreeCtrl::InsertItemAfter(const CString& crstrText, CTreeCursor& rtCursorParent, HTREEITEM hItemAfter, CXMLNode& rtNode, int iImageIdx /*=-1*/)
{
    CTreeCursor tTreeCursor;
    CXMLNode*   pNode = new CXMLNode;
    if(!pNode)
    {
        ::AfxThrowMemoryException();
    }
    *pNode = rtNode;
    tTreeCursor = rtCursorParent.InsertAfter(crstrText, hItemAfter, iImageIdx);
    tTreeCursor.SetData((DWORD)pNode);
    pNode->GetIdPath(m_strInsertIdPath);
    return tTreeCursor;
}

//------------------------------------------------------------------*
/**
 * add item tail.
 *
 *  base class calls AddTail() of parent tree item and sets pointer
 *  to xml node
 *
 * @param           crstrText: text for new item
 * @param           rtCursorParent: cursor to parent item
 * @param           rtNode: corresponding xml node
 * @param           iImageIdx: image index of new item
 * @return          cursor to new tree item
 * @exception       -
 * @see             -
*/
CTreeCursor CNavTreeCtrl::AddItemTail(const CString& crstrText, CTreeCursor& rtCursorParent, CXMLNode& rtNode, int iImageIdx /*=-1*/)
{
    CTreeCursor tTreeCursor;
    CXMLNode*   pNode = new CXMLNode;
    if(!pNode)
    {
        ::AfxThrowMemoryException();
    }
    *pNode = rtNode;
    tTreeCursor = rtCursorParent.AddTail(crstrText, iImageIdx);
    tTreeCursor.SetData((DWORD)pNode);
    pNode->GetIdPath(m_strInsertIdPath);
    return tTreeCursor;
}

//------------------------------------------------------------------*
/**
 * add item head.
 *
 *  base class calls AddHead() of parent tree item and sets pointer to
 *  xml node.
 *
 * @param           crstrText: text for new item
 * @param           rtCursorParent: cursor to parent item
 * @param           rtNode: corresponding xml node
 * @param           iImageIdx: image index of new item
 * @return          cursor to new tree item
 * @exception       -
 * @see             -
*/
CTreeCursor CNavTreeCtrl::AddItemHead(const CString& crstrText, CTreeCursor& rtCursorParent, CXMLNode& rtNode, int iImageIdx /*=-1*/)
{
    CTreeCursor tTreeCursor;
    CXMLNode*   pNode = new CXMLNode;
    if(!pNode)
    {
        ::AfxThrowMemoryException();
    }
    *pNode = rtNode;
    tTreeCursor = rtCursorParent.AddHead(crstrText, iImageIdx);
    tTreeCursor.SetData((DWORD)pNode);
    pNode->GetIdPath(m_strInsertIdPath);
    return tTreeCursor;
}


//------------------------------------------------------------------*
/**
 * is corresponding.
 *
 *  is tree item and node corresponding?
 *  I assume this is the case, if id and type are the same.
 *
 * @param           rtTreeCursor: cursor to tree item
 * @param           rtNode: xml node
 * @return          is corresponding?
 * @exception       -
 * @see             -
*/
BOOL CNavTreeCtrl::IsCorresponding(CTreeCursor& rtTreeCursor, CXMLNode& rtNode)
{
    if(!HTREEITEM(rtTreeCursor))
    {
        return FALSE;
    }

    CXMLNode*   pCursorNode = (CXMLNode*) rtTreeCursor.GetData();
    CString     strCursorId;
    CString     strCursorType;
    CString     strNodeId;
    CString     strNodeType;
    BOOL        bReturn = FALSE;

    ASSERT(pCursorNode);
    if(pCursorNode)
    {
        pCursorNode->GetAttribute(CE_XMLATTR_ID, strCursorId);
        pCursorNode->GetAttribute(CE_XMLATTR_TYPE, strCursorType);

        rtNode.GetAttribute(CE_XMLATTR_ID, strNodeId);
        rtNode.GetAttribute(CE_XMLATTR_TYPE, strNodeType);

        //TRACE("Node - Cursor:  %s %s - %s %s\n", strNodeId, strNodeType, strCursorId, strCursorType);

        if(strCursorId.CompareNoCase(strNodeId) == 0)
        {
            if(strCursorType.Compare(strNodeType) == 0)
            {
                bReturn = TRUE;
            }
        }
    }
    return bReturn;
}

// 
//------------------------------------------------------------------*
/**
 * invalidate tree.
 *
 *  call this function after updating all nodes to see changes
*/
void CNavTreeCtrl::InvalidateTree()
{
    Invalidate();
}

//------------------------------------------------------------------*
/**
 * add tree context menu (virtual).
 *
 *  adds tree context menu for the selected tree item
 *  context menu is built up from KAD-type data
 *  overwrite this method to change this behaviour
 *  uses AddContextMenuForNode()
 *
 * @param           pMenu: pointer to context menu
 * @return          successful?
 * @see             CNavigator::OnExtendContextMenu(), AddContextMenuItem()
*/
BOOL CNavTreeCtrl::AddContextMenu(CMenu* pMenu)
{
    CXMLNode*   pNode;
    CPoint      CurrPoint;
    CPoint	    OrigPoint;
    CString     strTmp;
    CTreeCursor tContextCursor;
    CString     strType;

	tContextCursor = GetSelectedItem();
	if(!HTREEITEM(tContextCursor))
	{
		return FALSE;
	}

    pNode = (CXMLNode*)tContextCursor.GetData();
    if(!pNode)
    {
        return FALSE;
    }
    pNode->GetIdPath(m_strContextIdPath);
    return (AddContextMenuForNode(pMenu, pNode));
}




//pszCmdString = "Stop My FB1|PC1.resource1.myFB1.myCmd1|34"
static bool SplitUserOnlCmdString(
    const TCHAR*  pszCmdString, 
    CString*      pstrMenueTxt, 
    CString*      pstrTrgName,
    CString*      pstrVarPath,
    CString*      pstrIECType,
    CString*      pstrValue
)
{
    TCHAR   buffer[2*MAX_PATH];
    TCHAR*  pszParts[5];
    TCHAR*  pszVarPart;
    TCHAR*  pszTrgPart = _T("");

    if(!FC_StringCopyToBuffer(buffer, FC_ARRAY_LEN(buffer), pszCmdString))
        return false;
    if(FC_StringSplitBuf(buffer,'|', pszParts, 5, 1) != 4)
        return false;

    //insert 0 at first '.' of var full path, if no '.' threrse is no target
    //name given TODO this could be the trigger to use as a default the id-path
    //can safe possibly much string stuff in the XML IF.
    for(pszVarPart = pszParts[1]; *pszVarPart; pszVarPart++)
    {
        if(*pszVarPart=='.')
        {
            pszTrgPart = pszParts[1];
            *pszVarPart++ = 0;
            break;
        }
    }

    //caller gets desired info:
    if(pstrMenueTxt)
        *pstrMenueTxt = pszParts[0];
    if(pstrIECType)
        *pstrIECType  = pszParts[2];
    if(pstrValue)
        *pstrValue    = pszParts[3];
    if(pstrTrgName)
        *pstrTrgName  = pszTrgPart;
    if(pstrVarPath)
        *pstrVarPath  = pszVarPart;

    return true;
}

//------------------------------------------------------------------*
/**
 * add tree context menu for node.
 *
 *  adds tree context menu for a node
 *  context menu is built up from KAD-type data
 *  overwrite this method to change this behaviour
 *  uses AddContextMenuItem()
 *
 * @param           pMenu: pointer to context menu
 * @return          successful?
 * @see             CNavigator::OnExtendContextMenu(), AddContextMenuItem()
*/
BOOL CNavTreeCtrl::AddContextMenuForNode(CMenu* pMenu, CXMLNode* pNode)
{
    CKADMenu*   pTypeMenu;
    CString     strType;
    BOOL        bSourceReadOnly = FALSE;
    CString     strSource;
    CCEFileHandler* pFileHandler = ((CCEMAINApp*)AfxGetApp())->GetFileHandler();
    ASSERT(pFileHandler);

    pNode->GetAttribute(CE_XMLATTR_TYPE, strType);
    pNode->GetAttribute(CE_XMLATTR_SOURCE, strSource, TRUE);

    if(pFileHandler)
    {
        CString strSourcePathName = pFileHandler->GetFilePathName(strSource);
        CFileStatus tFileStatus;
        if(CFile::GetStatus(strSourcePathName, tFileStatus))
        {
            if(tFileStatus.m_attribute & CFile::readOnly)
            {
                bSourceReadOnly = TRUE;
            }
        }
    }

    pTypeMenu = m_pKADInfo->GetContextMenuInfoForType(strType);
    if(!pTypeMenu)
        return FALSE;

    // add menu
    long            l_num_items = pTypeMenu->m_tItems.GetSize();
    CKADMenuItem*   pItem;

    for(long lItem = 0; lItem < l_num_items; ++lItem)
    {
        pItem = pTypeMenu->m_tItems.GetAt(lItem);
        AddContextMenuItem(pMenu, pItem, pNode, lItem, bSourceReadOnly);
    }

    //user commands 0..N (should only be used in instance tree):
    CString strUserX;
    CString strDescX;
    for(int nCmd=0; ;nCmd++)
    {
        TCHAR buffer[64];
        strUserX  = _T("USER_ONLCMD");
        strUserX += _itot(nCmd, buffer, 10);
        if(!pNode->GetAttribute(strUserX, strDescX))
            break;
        
        if(nCmd==0)
            pMenu->AppendMenu(MF_SEPARATOR);

        //strDescX = Stop My FB1|PC1.resource1.myFB1.myCmd1|34
        CString strMenueTxt;
        CString strTrgName;
        if(SplitUserOnlCmdString(strDescX, &strMenueTxt, &strTrgName, NULL, NULL, NULL))
        {
            DWORD dwStyle = MF_STRING;
            //gray if not connected:
            CTargetOnlBase* pTarget = GetTargetOnlObject(strTrgName);
            if(!pTarget || !pTarget->IsTargetConnected())
                dwStyle |= MF_GRAYED;
            pMenu->AppendMenu(dwStyle, ID_NAV_CONTEXT_BASE + lItem, strMenueTxt);
            lItem++;
        }
        //else error handling sems tobe a problem in 4CE, isn't there a simple function to but
        //a plain text into the system tab, trace is not sufficient, assert even wrong!
    }


    return TRUE;
}



//------------------------------------------------------------------*
/**
 * add menu item.
 *
 * @param           pMenu: pointer to context menu
 * @param           pItem: pointer to context menu item data from KAD
 * @param           pNode: pointer to currently selected XML node
 * @param           lItem: number of menu item in context menu
 * @param           bSourceReadOnly: source file is read only
 *
 * @see             AddContextMenu()
*/
void CNavTreeCtrl::AddContextMenuItem(CMenu* pMenu, CKADMenuItem* pItem, CXMLNode* pNode, long lItem, BOOL bSourceReadOnly)
{
    BOOL    bProjectReadOnly = IsProjectReadOnly();
    if(!pItem->m_strShowExpr.IsEmpty())
    {
        if(pItem->m_strShowExpr.Find(_T("source!=readonly")) >= 0 && bSourceReadOnly)
        {
            return;
        }
        if(pItem->m_strShowExpr.Find(_T("source=readonly")) >= 0 && !bSourceReadOnly)
        {
            return;
        }
        if(pItem->m_strShowExpr.Find(_T("project!=readonly")) >= 0 && bProjectReadOnly)
        {
            return;
        }
        if(pItem->m_strShowExpr.Find(_T("project=readonly")) >= 0 && !bProjectReadOnly)
        {
            return;
        }
        BOOL bConStateCon = FALSE;
        BOOL bConStateConEx = FALSE;
        BOOL bConStateNotCon = FALSE;
        BOOL bConStateNotConEx = FALSE;
        if (pItem->m_strShowExpr.Find(_T("constate==connected")) >=0 )
        {
            bConStateCon = TRUE;
        }
        if (pItem->m_strShowExpr.Find(_T("constate==connectedex")) >=0 )
        {
            bConStateConEx = TRUE;
        }
        if (pItem->m_strShowExpr.Find(_T("constate!=connected")) >=0 )
        {
            bConStateNotCon = TRUE;
        }
        if (pItem->m_strShowExpr.Find(_T("constate!=connectedex")) >=0 )
        {
            bConStateNotConEx = TRUE;
        }
        if (bConStateCon || bConStateConEx || bConStateNotCon || bConStateNotConEx)
        {
            CTargetSet*     pTargetSet = CEGetTargetSet("Project Targets", false);
            CTargetOnlBase* pTarget = NULL;
            CString strIdPath;
            CString strTargetId;
            if(!pTargetSet)
            {
                return;
            }
            pNode->GetIdPath(strIdPath);
            int i = strIdPath.ReverseFind(_T('/'));
            strTargetId = strIdPath;
            if (i>0)
            {
                strTargetId = strTargetId.Mid(i+1);
            }
            pTarget = pTargetSet->GetTarget(strTargetId);
            E_TargetOnlConnectState onlState = pTarget->GetConnectState();

            if (bConStateCon && onlState<eTOCS_connected)
            {
                return;
            }
            if (bConStateConEx && onlState<eTOCS_connected_ex)
            {
                return;
            }
            if (bConStateNotCon && onlState>=eTOCS_connected)
            {
                return;
            }
            if (bConStateNotConEx && onlState>=eTOCS_connected_ex)
            {
                return;
            }
        }
    }

    DWORD dwStyle = MF_STRING;
    BOOL    bEnabled = pItem->m_bEnabled;

    if(!pItem->m_strEnableExpr.IsEmpty())
    {
        if(pItem->m_strEnableExpr.Find(_T("source!=readonly")) >= 0 && bSourceReadOnly)
        {
            bEnabled = FALSE;
        }
        if(pItem->m_strEnableExpr.Find(_T("source=readonly")) >= 0 && !bSourceReadOnly)
        {
            bEnabled = FALSE;
        }
        if(pItem->m_strEnableExpr.Find(_T("project!=readonly")) >= 0 && bProjectReadOnly)
        {
            bEnabled = FALSE;
        }
        if(pItem->m_strEnableExpr.Find(_T("project=readonly")) >= 0 && !bProjectReadOnly)
        {
            bEnabled = FALSE;
        }


        BOOL bConStateCon = FALSE;
        BOOL bConStateConEx = FALSE;
        BOOL bConStateNotCon = FALSE;
        BOOL bConStateNotConEx = FALSE;
        if (pItem->m_strEnableExpr.Find(_T("constate==connected")) >=0 )
        {
            bConStateCon = TRUE;
        }
        if (pItem->m_strEnableExpr.Find(_T("constate==connectedex")) >=0 )
        {
            bConStateConEx = TRUE;
        }
        if (pItem->m_strEnableExpr.Find(_T("constate!=connected")) >=0 )
        {
            bConStateNotCon = TRUE;
        }
        if (pItem->m_strEnableExpr.Find(_T("constate!=connectedex")) >=0 )
        {
            bConStateNotConEx = TRUE;
        }
        if (bConStateCon || bConStateConEx || bConStateNotCon || bConStateNotConEx)
        {
            CTargetSet*     pTargetSet = CEGetTargetSet("Project Targets", false);
            CTargetOnlBase* pTarget = NULL;
            CString strIdPath;
            CString strTargetId;
            if(!pTargetSet)
            {
                bEnabled = FALSE;
            }
            pNode->GetIdPath(strIdPath);
            int i = strIdPath.ReverseFind(_T('/'));
            strTargetId = strIdPath;
            if (i>0)
            {
                strTargetId = strTargetId.Mid(i+1);
            }
            pTarget = pTargetSet->GetTarget(strTargetId);
            E_TargetOnlConnectState onlState = pTarget->GetConnectState();

            if (bConStateCon && onlState<eTOCS_connected)
            {
                bEnabled = FALSE;
            }
            if (bConStateConEx && onlState<eTOCS_connected_ex)
            {
                bEnabled = FALSE;
            }
            if (bConStateNotCon && onlState>=eTOCS_connected)
            {
                bEnabled = FALSE;
            }
            if (bConStateNotConEx && onlState>=eTOCS_connected_ex)
            {
                bEnabled = FALSE;
            }
        }
    }

    if(!bEnabled)
    {
        dwStyle |= MF_GRAYED;
    }

    long lMenuId = ID_NAV_CONTEXT_BASE + lItem;
    if(pItem->m_bSeparator)
        pMenu->AppendMenu(MF_SEPARATOR);
    else
    {
        // if kernel standard operation
        if(pItem->m_iKSO > -1 && !IsKSOPossible(pItem->m_iKSO, pNode))
        {
            dwStyle |= MF_GRAYED;
        }
        pMenu->AppendMenu(dwStyle, lMenuId, pItem->m_strText);
    }
}

//------------------------------------------------------------------*
/**
 * is kernel standard operation possible?.
 *
 *  each kernel standard operation is dependent on a certain attribute
 *  if this attribute is existing, the operation can be performed.
 *
 * @param           iKSO: number of kernel standard operation (see kso.h)
 * @param           pNode: pointer to currently selected XML node
 * @return          successful?
 *
 * @see             AddContextMenuItem()
*/
BOOL CNavTreeCtrl::IsKSOPossible(int iKSO, CXMLNode* pNode)
{
    CString strAttribValue;

    // ask for corresponding attribute
    switch(iKSO)
    {
    case KSO_OPEN:              // connected to attribute SOURCE
        pNode->GetAttribute(CE_XMLATTR_SOURCE, strAttribValue);
        break;
    case KSO_OPEN_USER_HELP:    // connected to attribute URL
        //pNode->GetAttribute(_T("UserHelpURL"), strAttribValue);
        pNode->GetAttribute(CE_XMLATTR_USER_HELP, strAttribValue);
        break;
    case KSO_OPEN_URL:
        //pNode->GetAttribute(_T("UserURL"), strAttribValue);
        pNode->GetAttribute(CE_XMLATTR_URL, strAttribValue);
        break;
    case KSO_GOTO:              // connected to attribute LOCATION
        pNode->GetAttribute(CE_XMLATTR_LOCATION, strAttribValue);
        if(strAttribValue.IsEmpty())
        {
            pNode->GetAttribute(CE_XMLATTR_SOURCE, strAttribValue, TRUE);
        }
        break;
    case KSO_GOTO_DEFINITION:   // connected to attribute DATA_TYPE
        pNode->GetAttribute(CE_XMLATTR_DATA_TYPE, strAttribValue);
        break;
    case KSO_DELETE:            // connected to attribute SOURCE
        pNode->GetAttribute(CE_XMLATTR_SOURCE, strAttribValue);
        break;

    case KSO_CONNECT:
        pNode->GetAttribute(CE_XMLATTR_ADDR, strAttribValue);
        {
            // if already connected, return FALSE
            CString strId;
            CTargetOnlBase* pTarget = NULL;

            pNode->GetAttribute(CE_XMLATTR_ID, strId);
            pTarget = GetTargetOnlObject(strId);
            if(pTarget && !pTarget->IsConnected() && !strAttribValue.IsEmpty())
            {
                return TRUE;
            }
        }
        return FALSE;
    case KSO_DISCONNECT:      
        pNode->GetAttribute(CE_XMLATTR_ADDR, strAttribValue);
        {
            // if not connected, return FALSE
            CString strId;
            CTargetOnlBase* pTarget = NULL;

            pNode->GetAttribute(CE_XMLATTR_ID, strId);
            pTarget = GetTargetOnlObject(strId);
            if(pTarget && pTarget->IsConnected())
            {
                return TRUE;
            }
        }
        return FALSE;
    case KSO_CONNECT_EX:
        pNode->GetAttribute(CE_XMLATTR_ADDR, strAttribValue);
        {
            // if already exclusive connected, return FALSE
            CString strId;
            CTargetOnlBase* pTarget = NULL;

            pNode->GetAttribute(CE_XMLATTR_ID, strId);
            pTarget = GetTargetOnlObject(strId);
            if(pTarget && !pTarget->IsConnectedExclusive() && !strAttribValue.IsEmpty())
            {
                return TRUE;
            }
        }
        return FALSE;
    case KSO_DISCONNECT_EX:
        pNode->GetAttribute(CE_XMLATTR_ADDR, strAttribValue);
        {
            // if not connected exclusive, return FALSE
            CString strId;
            CTargetOnlBase* pTarget = NULL;

            pNode->GetAttribute(CE_XMLATTR_ID, strId);
            pTarget = GetTargetOnlObject(strId);
            if(pTarget && pTarget->IsConnectedExclusive())
            {
                return TRUE;
            }
        }
        return FALSE;
    case KSO_DOWNLOAD_FULL:
    case KSO_DOWNLOAD_CHANGES:
    case KSO_DOWNLOAD_SELECTION:
        pNode->GetAttribute(CE_XMLATTR_ADDR, strAttribValue);
        break;  
    case KSO_DOWNLOAD_FULL_ALL:
    case KSO_DOWNLOAD_CHANGES_ALL:
    case KSO_DOWNLOAD_FULL_SELECT:
    case KSO_DOWNLOAD_CHANGES_SELECT:
        strAttribValue=_T("TRUE");
        break;

    case KSO_BUILD_ALL:
    case KSO_BUILD_SELECT:
        strAttribValue=_T("TRUE");
        break;
    case KSO_BUILD_TARGET:
        pNode->GetAttribute(CE_XMLATTR_SOURCE, strAttribValue);
        break;
    case KSO_COMPILE_FILE:
        break;
    case KSO_BUILD_LIBRARY:
        strAttribValue = _T("TRUE");
        break;


    case KSO_START_CONTROL:
        {
            // if target is connected and control is not running, return TRUE else FALSE
            CString     strTarget;
            CTargetOnlBase* pTarget = NULL;
            CXMLNode    tTargetNode;

            // get target node and id
            if(pNode->GetParentNode(tTargetNode))
            {
                tTargetNode.GetAttribute(CE_XMLATTR_ID, strTarget);
            }
            pTarget = GetTargetOnlObject(strTarget);

            if(pTarget)
            {
                BOOL    bConnected = pTarget->IsConnected();
                BOOL    bResourceNotStarted = !pTarget->IsResourceStarted();
                BOOL    bServiceIsRunning = !pTarget->IsServiceStopped();

                if(bConnected && bResourceNotStarted && bServiceIsRunning)
                {
                    return TRUE;
                }
            }
        }
        return FALSE;
    case KSO_STOP_CONTROL:
        {
            // if target is connected and control is not running, return TRUE else FALSE
            CString     strTarget;
            CTargetOnlBase* pTarget = NULL;
            CXMLNode    tTargetNode;

            // get target node and id
            if(pNode->GetParentNode(tTargetNode))
            {
                tTargetNode.GetAttribute(CE_XMLATTR_ID, strTarget);
            }
            pTarget = GetTargetOnlObject(strTarget);

            if(pTarget)
            {
                BOOL    bConnected = pTarget->IsConnected();
                BOOL    bResourceIsRunning = pTarget->IsResourceStarted();
                BOOL    bServiceIsRunning = !pTarget->IsServiceStopped();

                if(bConnected && bResourceIsRunning && bServiceIsRunning)
                {
                    return TRUE;
                }
            }
        }
        return FALSE;
    case KSO_START_ALL_TASKS: 
    case KSO_STOP_ALL_TASKS:
        {
            // if target is connected and control is not running, return TRUE else FALSE
            CString     strTarget;
            CTargetOnlBase* pTarget = NULL;
            CXMLNode    tTargetNode;

            // get target node and id
            if(pNode->GetParentNode(tTargetNode))
            {
                tTargetNode.GetAttribute(CE_XMLATTR_ID, strTarget);
            }
            pTarget = GetTargetOnlObject(strTarget);

            if(pTarget)
            {
                BOOL    bConnected = pTarget->IsConnected();
                BOOL    bResourceIsRunning = pTarget->IsResourceStarted();
                BOOL    bServiceIsRunning = !pTarget->IsServiceStopped();
                int     iNumTasksNotRunning = 0;
                int     iNumTasksRunning = 0;

                GetTaskRunInfo(pNode, iNumTasksRunning, iNumTasksNotRunning);

                if(iKSO == KSO_START_ALL_TASKS)
                {
                    if(bConnected && bResourceIsRunning && bServiceIsRunning && iNumTasksNotRunning)
                    {
                        return TRUE;
                    }
                }
                else
                {
                    if(bConnected && bResourceIsRunning && bServiceIsRunning && iNumTasksRunning)
                    {
                        return TRUE;
                    }
                }
            }
        }
        return FALSE;
    case KSO_COLDSTART_RESET:
        {
            // if target is connected, return TRUE else FALSE
            CString     strTarget;
            CTargetOnlBase* pTarget = NULL;
            CXMLNode    tTargetNode;

            // get target node and id
            if(pNode->GetParentNode(tTargetNode))
            {
                tTargetNode.GetAttribute(CE_XMLATTR_ID, strTarget);
            }
            pTarget = GetTargetOnlObject(strTarget);
            if(pTarget && pTarget->IsConnected())
            {
                return TRUE;
            }
        }
        return FALSE;
    case KSO_WARMSTART_RESET:
        {
            // if target is connected, return TRUE else FALSE
            CString     strTarget;
            CTargetOnlBase* pTarget = NULL;
            CXMLNode    tTargetNode;

            // get target node and id
            if(pNode->GetParentNode(tTargetNode))
            {
                tTargetNode.GetAttribute(CE_XMLATTR_ID, strTarget);
            }
            pTarget = GetTargetOnlObject(strTarget);
            if(pTarget && pTarget->IsWarmstartEnabled() && pTarget->IsConnected())
            {
                return TRUE;
            }
        }
        return FALSE;
    case KSO_INITIALIZE_CLEAR:
        {
            // if target is connected, return TRUE else FALSE
            CString     strTarget;
            CTargetOnlBase* pTarget = NULL;
            CXMLNode    tTargetNode;

            // get target node and id
            if(pNode->GetParentNode(tTargetNode))
            {
                tTargetNode.GetAttribute(CE_XMLATTR_ID, strTarget);
            }
            pTarget = GetTargetOnlObject(strTarget);
            if(pTarget && pTarget->IsInitClearEnabled() && pTarget->IsConnectedExclusive())
            {
                return TRUE;
            }
        }
        return FALSE;
    case KSO_START_TASK:
    case KSO_STOP_TASK:
        {
            // if target is connected, return TRUE else FALSE
            CString     strTarget;
            CString     strState;
            CTargetOnlBase* pTarget = NULL;
            CXMLNode    tResourceNode;
            CXMLNode    tTargetNode;
            int         iState = 0;

            // get state
            if(pNode->GetAttribute(CE_XMLATTR_STATE, strState))
            {
                iState = atoi(strState);
            }

            // get target node and id
            if(pNode->GetParentNode(tResourceNode))
            {
                if(tResourceNode.GetParentNode(tTargetNode))
                {
                    tTargetNode.GetAttribute(CE_XMLATTR_ID, strTarget);
                }
            }
            pTarget = GetTargetOnlObject(strTarget);
            if(iKSO == KSO_START_TASK)
            {
                if(pTarget && pTarget->IsConnected() && pTarget->IsResourceStarted() && (iState==eStopped || iState==eError))
                {
                    return TRUE;
                }
            }
            else
            {
                if(pTarget && pTarget->IsConnected() && pTarget->IsResourceStarted() && iState==eRunning)
                {
                    return TRUE;
                }
            }
        }
        return FALSE;

    case KSO_SETTINGS_RESET_DOCKING:
    case KSO_SETTINGS_LIBRARY:
    case KSO_SETTINGS_ENGINEERING:
    case KSO_SETTINGS_PASSWORD:
        strAttribValue = _T("TRUE");
        break;
    case KSO_DOWNLOAD_FLASH:
    case KSO_UPDATE_FLASH:
    case KSO_CLEAR_FLASH:
        pNode->GetAttribute(CE_XMLATTR_ADDR, strAttribValue);
        break;  

    }

    if(strAttribValue.IsEmpty())
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}


//------------------------------------------------------------------*
/**
 * get task run info.
 *
 *  run over all tasks and count running and not running ones
 *
 * @param           pNode: resource instance node (this is not checked)
 * @param           riNumTasksRunning: number of running tasks
 * @param           riNumTasksNotRunning: number of not running tasks
 * @return          total number of tasks
*/
int CNavTreeCtrl::GetTaskRunInfo(CXMLNode* pNode, int& riNumTasksRunning, int& riNumTasksNotRunning)
{
    int iNumTasks = 0;
    riNumTasksRunning = 0;
    riNumTasksNotRunning = 0;

    CXMLNodeList    tNodeList;
    long            lNumChildren;
    long            lChild;
    CXMLNode        tTaskNode;
    CString         strType;
    CString         strState;
    int             iState;

    // get all children from pNode
    if(pNode->GetChildNodeList(tNodeList))
    {
        lNumChildren = tNodeList.GetLength();
        for(lChild = 0; lChild < lNumChildren; ++lChild)
        {
            // get child
            tNodeList.Item(lChild, tTaskNode);
            tTaskNode.GetAttribute(CE_XMLATTR_TYPE, strType);
            if(strType == _T("TASK_INST"))
            {
                tTaskNode.GetAttribute(CE_XMLATTR_STATE, strState);
                iState = atoi(strState);
                if(iState == eRunning)
                {
                    ++riNumTasksRunning;
                }
                else
                {
                    ++riNumTasksNotRunning;
                }
                ++iNumTasks;
            }
        }
    }
    return(iNumTasks);
}



//------------------------------------------------------------------*
/**
 * get target online object.
 *
 *  helper function
 *
 * @param           crstrTargetName: name (id) of target.
 * @return          pointer to target online object
*/
CTargetOnlBase* CNavTreeCtrl::GetTargetOnlObject(const CString& crstrTargetName)
{
    CTargetSet*     pTargetSet = CEGetTargetSet("Project Targets", false);
    CTargetOnlBase* pTarget = NULL;
    if(!pTargetSet)
    {
        return pTarget;
    }
    pTarget = pTargetSet->GetTarget(crstrTargetName);
    return pTarget;
}

//------------------------------------------------------------------*
/**
 * open source file
 *
 * @param          rstrFile: file name without path
 * @return          
 * @exception       -
 * @see             
*/
CDocument* CNavTreeCtrl::OpenSourceFile(const CString& crstrFile, const CString& crstrLocation /*=""*/)
{
    CGenEdDoc*  pGenEdDoc;

    CString strFileName = crstrFile;
    
    ASSERT(m_pProjInfo);
    if (m_pProjInfo)
    {
        CComBSTR sIn(crstrFile);
        CComBSTR sOut;
        HRESULT hr = m_pProjInfo->ReplaceMacros(sIn, &sOut);
        if (hr==S_OK)
        {
            strFileName = sOut;
        }
    }
       
    CCEFileHandler* pFileHandler = ((CCEMAINApp*)AfxGetApp())->GetFileHandler();
    if(pFileHandler)
    {
        pFileHandler->ShowFileAt(strFileName, crstrLocation, pGenEdDoc);
        return pGenEdDoc;
    }
    return NULL;
}


//------------------------------------------------------------------*
/**
 * open source file in st editor
 *
 * @param          rstrFile: file name without path
 * @return          
 * @exception       -
 * @see             
*/
CDocument* CNavTreeCtrl::OpenSourceFileAsText(const CString& crstrFile)
{
    CGenEdDoc*  pGenEdDoc;

    CString strFileName = crstrFile;
    
    ASSERT(m_pProjInfo);
    if (m_pProjInfo)
    {
        CComBSTR sIn(crstrFile);
        CComBSTR sOut;
        HRESULT hr = m_pProjInfo->ReplaceMacros(sIn, &sOut);
        if (hr==S_OK)
        {
            strFileName = sOut;
        }
    }
       
    CCEFileHandler* pFileHandler = ((CCEMAINApp*)AfxGetApp())->GetFileHandler();
    if(pFileHandler)
    {
        pFileHandler->ShowFile(strFileName, pGenEdDoc, false, true);
        return pGenEdDoc;
    }
    return NULL;
}




//------------------------------------------------------------------*
/**
 * delete source file.
 *
 * @param           pNode: pointer to node to be unlinked
 * @return          successful?
 * @exception       -
 * @see             
*/
BOOL CNavTreeCtrl::UnlinkNode(CXMLNode* pNode)
{
    CString strSource;
    CString strIdPath;
    pNode->GetIdPath(strIdPath);
    pNode->GetAttribute(CE_XMLATTR_SOURCE, strSource, TRUE);

    // get project info interface
    CComQIPtr<ICEProjInfo>    pICEProjectInfo;
    pICEProjectInfo = m_pProjMan;

    if(!pICEProjectInfo)
    {
        TRACE("No project info available.\n");
        return FALSE;
    }

    // get container interface
    HRESULT hr;
    CComBSTR    sSourceFile(strSource);
    CComBSTR    sIdPath(strIdPath);
    CComBSTR    sSourceParent;
    CComBSTR    sIdPathParent;
    CComPtr<ICEContainer>   pICEContainer;

    hr = pICEProjectInfo->getContainerIF(sSourceFile, sIdPath, &pICEContainer, &sSourceParent, &sIdPathParent);
    if(hr != S_OK && !pICEContainer)
    {
        ::AfxMessageBox(IDS_CANNOT_DELETE_ERRNODE);
        return FALSE;
    }

    // remove link
    hr = pICEContainer->deleteLink(sSourceParent, sIdPath, sSourceFile);
    if(hr != S_OK)
    {
        return FALSE;
    }
    CCEFileHandler* pFileHandler = ((CCEMAINApp*)AfxGetApp())->GetFileHandler();
    ASSERT(pFileHandler);
    if(!pFileHandler)
    {
        return FALSE;
    }

    // if file exists, ask user whether to delete the source file
    CFileStatus tFileStatus;
    CString strSourcePath = pFileHandler->GetFilePathName(strSource);
    if(CFile::GetStatus(strSourcePath, tFileStatus))
    {
        CString strMessage;
        strMessage.Format(IDS_ASK_DELETE_FILE_ERRNODE, strSourcePath);
        if(::AfxMessageBox(strMessage, MB_YESNO | MB_DEFBUTTON2) == IDYES)
        {
            RemoveFile(strSourcePath);
        }
    }
    pICEProjectInfo->fileParseNow(NULL);
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * remove file.
 *
 * @param           crstrFilePathName: file name incl path
 * @return          successful?
 * @exception       -
 * @see             
*/
BOOL CNavTreeCtrl::RemoveFile(const CString& crstrFilePathName)
{
    try
    {
        CFile::Remove(crstrFilePathName);
    }
    catch(CFileException* pFileException)
    {
        pFileException->ReportError();
        pFileException->Delete();
        return FALSE;
    }
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * OpenUrl
 *
 * @param           strSource: the url to be opened
 * @exception       -
 * @see             
*/
void CNavTreeCtrl::OpenUrl(const CString& strSource)
{
    CMainFrame* pMainFrame = NULL;
    pMainFrame = dynamic_cast<CMainFrame* > (AfxGetMainWnd ());
    ASSERT(pMainFrame);
    if (pMainFrame != NULL)
    {
        pMainFrame->OpenURL(strSource);
    }
}



//------------------------------------------------------------------*
/**
 * set filters.
 *
 *  called by navigator.
 *
 * @param           iSelCat: number of category filter in combo box
 * @param           rstrFilter2: wildcard expression for text filter 2
 *                                    text filter 1 is used for category filter
*/
void CNavTreeCtrl::SetFilters(int iSelCat, const CString& rstrFilter2)
{
    BOOL        bFilterChanged = FALSE;
//    int         iRegError;
    CString     cs_reg_ex;
    CString     strCatTextFilterExpr = GetCategoryFilterExpr(iSelCat);
    CString     strFilter2 = rstrFilter2;

    if(!m_bUseCase)
    {
        strCatTextFilterExpr.MakeLower();
        strFilter2.MakeLower();
    }

    if(m_iSelCat != iSelCat)
    {
        m_iSelCat = iSelCat;
        bFilterChanged = TRUE;
    }
    // text filter 1 is not in use at the moment
    // only if filter changed
    if(m_strFilter1 != strCatTextFilterExpr)
    {
        bFilterChanged = TRUE;

        // reset filter data
//        if(m_pRegMatch1)
//        {
//            delete [] m_pRegMatch1;
//            regfree(&m_tRegex1);
//            m_iGroups1 = 0;
//        }

        // if filter empty or filter is "*" -> no filtering
        m_strFilter1 = strCatTextFilterExpr;
        if(m_strFilter1 == "" || m_strFilter1 == "*")
            m_bFilter1 = FALSE;
        else
            m_bFilter1 = TRUE;

        // compile regular expression
//        cs_reg_ex = GetRegularExpression(m_strFilter1);
//        iRegError = regcomp(&m_tRegex1, cs_reg_ex, REG_EXTENDED);
//        if(iRegError)
//        {
//            TRACE("Error in SetFilters: regcomp failed\n");
//            m_bFilter1 = FALSE;
//            return;    // do not filter
//        }
//        m_iGroups1 = m_tRegex1.re_nsub + 1;
//        m_pRegMatch1 = new regmatch_t[m_iGroups1];
    }

    // only if filter changed
    if(m_strFilter2 != strFilter2)
    {
        bFilterChanged = TRUE;

        // reset filter data
//        if(m_pRegMatch2)
//        {
//            delete [] m_pRegMatch2;
//            regfree(&m_tRegex2);
//            m_iGroups2 = 0;
//        }

        // if filter empty or filter is "*" -> no filtering
        m_strFilter2 = strFilter2;
        if(m_strFilter2 == "" || m_strFilter2 == "*")
            m_bFilter2 = FALSE;
        else
            m_bFilter2 = TRUE;

        // compile regular expression
//        cs_reg_ex = GetRegularExpression(m_strFilter2);
//        iRegError = regcomp(&m_tRegex2, cs_reg_ex, REG_EXTENDED);
//        if(iRegError)
//        {
//            TRACE("Error in SetFilters: regcomp failed\n");
//            m_bFilter2 = FALSE;
//            return;    // do not filter
//        }
//        m_iGroups2 = m_tRegex2.re_nsub + 1;
//        m_pRegMatch2 = new regmatch_t[m_iGroups2];

    }
    // if filter changed, build up tree control
    if(bFilterChanged)
    {
        BuildTree();
    }
}

//------------------------------------------------------------------*
/**
 * get filter expression (text filter) for certain category
 *
 * @param           iSelCat: number of category in combo box
 * @return          filter expression
 * @see             SetFilters()
*/
CString CNavTreeCtrl::GetCategoryFilterExpr(int iSelCat)
{ 
    return(m_pCatFilter->GetFilterExpr(iSelCat));
}



//------------------------------------------------------------------*
/**
 * expand / collapse tree item
 *
 *  synchronize expand state of tree item and xml node.
 *
 * @param           rtItem: structure with tree item
 * @param           bToExpand: item to expand?
 * @return          TRUE: item is expanded / collapsed
 *                  FALSE: item is not expanded / collapsed
*/
BOOL CNavTreeCtrl::OnItemexpanding(TVITEM& rtItem, BOOL bToExpand)
{
    // prevent root node to be collapsed
    if(rtItem.hItem == HTREEITEM(m_tCursorRoot) && !bToExpand)
    {
        return FALSE;
    }

    // do the same for associated DOM node
    CTreeCursor tTreeCursor(rtItem.hItem, this);
    CXMLNode* pNode = (CXMLNode*)tTreeCursor.GetData();
    if(pNode)
    {
        if(bToExpand)
        {
            if(ExpandXMLNode(pNode, TRUE))
            {
                UpdateTreeItemState(tTreeCursor, pNode);
            }
            CheckNodeSourceState(tTreeCursor);
        }
        else
        {
            if(ExpandXMLNode(pNode, FALSE))
            {
                UpdateTreeItemState(tTreeCursor, pNode);
            }
        }
    }
    return TRUE;
}



//------------------------------------------------------------------*
/**
 * on command.
 *
 *  execute context menu commands.
 *
 *  - determine context menu item number and call OnCommand(2)
 *  - expand / collapse all commands
 *
 *  overwrite this function in derived classes to get other behaviour
 *
 * @param           nID: id of context menu command
 * @see             OnCommand(2)
*/
void CNavTreeCtrl::OnCommand(UINT nID)
{
    long    lItem = nID - ID_NAV_CONTEXT_BASE;

    ASSERT(lItem >= 0);

    if(m_strContextIdPath.IsEmpty())
    {
        return;
    }

    // check expand all / collapse all
    if(nID == ID_EXPAND_ALL || lItem == KSO_EXPAND_ALL)
    {
        CTreeCursor tContextCursor;
        if(GetTreeCursorFromNodeIdPath(m_strContextIdPath, tContextCursor))
        {
            ExpandTreeItem(tContextCursor, TVE_EXPAND, TRUE);
        }
        return;
    }
    if(nID == ID_COLLAPSE_ALL || lItem == KSO_COLLAPSE_ALL)
    {
        CTreeCursor tContextCursor;
        if(GetTreeCursorFromNodeIdPath(m_strContextIdPath, tContextCursor))
        {
            ExpandTreeItem(tContextCursor, TVE_COLLAPSE, TRUE);
        }
        return;
    }

    CXMLNode tNode;
    if(!FindTreeNode(m_strContextIdPath, tNode))
    {
        return;
    }

    OnCommand(lItem, tNode);
}


//------------------------------------------------------------------*
/**
 * on command.
 *
 *  context menu commands
 *  execute rename commands
 *  uses DoContextAction()
 *
 * @param           lItem: item number within context menu
 * @param           rtNode: context node
 * @return          -
 * @see             OnCommand(1), DoContextAction()
*/
void CNavTreeCtrl::OnCommand(long lItem, CXMLNode& rtNode)
{
    CKADMenu*   pTypeMenu;

    CString     strType;

    rtNode.GetAttribute(CE_XMLATTR_TYPE, strType);

    pTypeMenu = m_pKADInfo->GetContextMenuInfoForType(strType);
    if(!pTypeMenu)
    {
        return;
    }

    int iMenuItemNo = pTypeMenu->m_aiKeyMouseArray[KEY_MOUSE_RENAME];
    if(iMenuItemNo == lItem)
    {
        CTreeCursor tContextCursor;
        CString strId;

        if(GetTreeCursorFromNodeIdPath(m_strContextIdPath, tContextCursor))
        {
            rtNode.GetAttribute(CE_XMLATTR_ID, strId);

            CEdit* edit = EditLabel(tContextCursor);
            return;
        }
    }
   
    DoContextAction(lItem);
}



//------------------------------------------------------------------*
/**
 * do context action.
 *
 *  execute context action.
 *  
 * @param           lItem: no of context item
 * @see             OnCommand(2)
*/
void CNavTreeCtrl::DoContextAction(long lItem)
{
    CKADMenu*   pTypeMenu;
    CString     strType;

    CXMLNode    tNode;
    if(!FindTreeNode(m_strContextIdPath, tNode))
    {
        return;
    }

    // now is a good time to save all editors
    SaveAllFiles();

    tNode.GetAttribute(CE_XMLATTR_TYPE, strType);

    pTypeMenu = m_pKADInfo->GetContextMenuInfoForType(strType);
    if(!pTypeMenu)
        return;

    long  l_numKAD_items = pTypeMenu->m_tItems.GetSize();
    if(lItem<l_numKAD_items)
    {
        CKADMenuItem*   pItem;
        pItem = pTypeMenu->m_tItems.GetAt(lItem);
        if(pItem->m_iKSO > -1)
        {
            DoKernelStandardOperation(pItem->m_iKSO, &tNode);
        }
        else
        {
            if(pItem->m_strExe != "")
            {
                DoExeOperation(tNode, pItem);
            }
            else
            {
                if(pItem->m_pHandler)
                {
                    DoAddOnAction(pItem);
                }
            }
        }
    }
    else
    {
        CString strUserX;
        CString strDescX;
        TCHAR buffer[64];
        strUserX  = _T("USER_ONLCMD");
        strUserX += _itot(lItem - l_numKAD_items , buffer, 10);

        if(tNode.GetAttribute(strUserX, strDescX))
        {
            CString    strTrgName;
            CString    strVarPath;
            CString    strIECType;
            CString    strValue;

            if(SplitUserOnlCmdString(strDescX, NULL, &strTrgName, &strVarPath, &strIECType, &strValue))
            {
                CTargetOnlBase* pTarget = GetTargetOnlObject(strTrgName);
                if(pTarget)
                {
                    CSubscription* pSubsc = NULL;
                    pTarget->SubscribeVariable(strVarPath, NULL, pSubsc);

                    if(strValue[0]=='\'') //HACK: all the string related handling is MISSING !
                    {                      //There is no simple stupid take away helper function for messi things.
                        strValue.TrimLeft(_T("'"));
                        strValue.TrimRight(_T("'"));
                    }
                    if(pSubsc)
                    {
                        VARIANT variVal; VariantInit(&variVal);
                        BSTR    bstrType = strIECType.AllocSysString();
                        BSTR    bstrVal  = strValue.AllocSysString();
                        
                        BL_StringToVariantNamed(bstrVal, bstrType, NULL, BL_USE_IEC_FORMAT, &variVal);
                        pSubsc->Write(variVal);
                        pTarget->UnsubscribeVariable(pSubsc, NULL);

                        SysFreeString(bstrVal);
                        SysFreeString(bstrType);
                        VariantClear(&variVal);
                    }
                }
            }
        }
        else
        {
            ASSERT(!"DoContextAction: XML attribiute USER_ONLCMD<N> not found"); 
        }
    }
//    ShowNode(m_strContextIdPath);
    m_strContextIdPath.Empty();        
}


BOOL CNavTreeCtrl::ExpandAttributeMacros(CXMLNode& rtNode, CString& rstrCommand)
{
    CString strWork(rstrCommand);
    CString strReplaced;
    CString strAttribName;
    CString strAttribValue;
    BOOL    bReturn = TRUE;

    int     iFound = strWork.Find(_T("$attr("));

    while(iFound >= 0)
    {
        strReplaced += strWork.Left(iFound);
        strWork = strWork.Mid(iFound);
        if(ExtractNamedMacro(_T("attr"), strWork, strAttribName))
        {
            rtNode.GetAttribute(strAttribName, strAttribValue);
            strReplaced += strAttribValue;
        }
        else
        {
            bReturn = FALSE;
            break;
        }
        iFound = strWork.Find(_T("$attr("));
    }
    strReplaced += strWork.Mid(iFound + 1);
    rstrCommand = strReplaced;
    return bReturn;
}



//------------------------------------------------------------------*
/**
 * extract named macro.
 *
 * @param           crstrName: macro name
 * @param           rstrRest: not yet parsed part of macro string (in/out)
 * @param           rstrMacroText: macro text within braces (out)
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CNavTreeCtrl::ExtractNamedMacro(const CString& crstrName, CString& rstrRest, CString& rstrMacroText)
{
    CString strTmpName;
    strTmpName.Format("$%s(", crstrName);        // temp name = name + "("
    int iTmpNameLen = strTmpName.GetLength();
    CString strLeft = rstrRest.Left(iTmpNameLen);

    if(strLeft.CompareNoCase(strTmpName)==0)
    {
        int iBraceRight = 0;
        int iNumBraceOpen = 1;
        int iLen = rstrRest.GetLength();

        for(int iChar = iTmpNameLen; iChar < iLen; ++iChar)
        {
            if(rstrRest[iChar] == _T('('))
            {
                ++iNumBraceOpen;
            }
            else if (rstrRest[iChar] == _T(')'))
            {
                --iNumBraceOpen;
                if(iNumBraceOpen == 0)
                {
                    iBraceRight = iChar;                        
                }
                break;
            }
        }

        if (iBraceRight>0)
        {
            rstrMacroText = rstrRest.Mid(iTmpNameLen, iBraceRight-iTmpNameLen);
            rstrRest = rstrRest.Mid(iBraceRight+1);
            return TRUE;
        }
    }
    return FALSE;
}

//------------------------------------------------------------------*
/**
 * open node.
 *
 *  perform kernel standard operation "open" for node.
 *
 * @param           crstrIdPath: id path of node to open
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CNavTreeCtrl::OpenNode(const CString& crstrIdPath)
{
    CXMLNode    tNode;
    if(FindTreeNode(crstrIdPath, tNode))
    {
        DoKernelStandardOperation(KSO_OPEN, &tNode);
        return TRUE;
    }
    return FALSE;
}

//------------------------------------------------------------------*
/**
 * is rename allowed?.
 *
 * @param           crstrIdPath: node id path
 * @param           iKeyMouseEvent: key mouse event
 * @return          is rename allowed?
 * @exception       -
 * @see             IsKeyMouseActionAllowed() node version
*/
BOOL CNavTreeCtrl::IsKeyMouseActionAllowed(const CString& crstrIdPath, int iKeyMouseEvent)
{
    CXMLNode    tNode;
    ASSERT(iKeyMouseEvent >= 0 && iKeyMouseEvent < NUM_KEY_MOUSE_EVENTS);
    if(FindTreeNode(crstrIdPath, tNode))
    {
        return IsKeyMouseActionAllowed(tNode, iKeyMouseEvent);
    }
    return FALSE;
}



//------------------------------------------------------------------*
/**
 * is rename allowed?.
 *
 * @param           rtNode: xml node
 * @param           iKeyMouseEvent: key mouse event
 * @return          is rename allowed?
 * @exception       -
 * @see             IsKeyMouseActionAllowed() id path version
*/
BOOL CNavTreeCtrl::IsKeyMouseActionAllowed(CXMLNode& rtNode, int iKeyMouseEvent)
{
    if(iKeyMouseEvent < 0 || iKeyMouseEvent >= NUM_KEY_MOUSE_EVENTS)
    {
        return FALSE;
    }

    CString     strType;
    CString     strId;
    CKADMenu*   pTypeMenu = NULL;
    BOOL        bSourceReadOnly = FALSE;
    BOOL        bProjectReadOnly = IsProjectReadOnly();
    BOOL        bProjectOpen = !m_strProjectName.IsEmpty();

    rtNode.GetAttribute(CE_XMLATTR_TYPE, strType);
    rtNode.GetAttribute(CE_XMLATTR_ID, strId);

    pTypeMenu = m_pKADInfo->GetContextMenuInfoForType(strType);
    if(pTypeMenu)
    {
        int iMenuItemNo = pTypeMenu->m_aiKeyMouseArray[iKeyMouseEvent];

        if(iMenuItemNo >= 0)
        {
            CKADMenuItem* pItem = pTypeMenu->m_tItems[iMenuItemNo];
            if(pItem->m_strShowExpr.IsEmpty() && pItem->m_strEnableExpr.IsEmpty())
            {
                return TRUE;
            }

            CCEFileHandler* pFileHandler = ((CCEMAINApp*)AfxGetApp())->GetFileHandler();
            CString         strSource;
            rtNode.GetAttribute(CE_XMLATTR_SOURCE, strSource, TRUE);
            if(pFileHandler)
            {
                bSourceReadOnly = (pFileHandler->GetFileStatus(strSource) == eFSReadOnly);
                {
                    if(!EvaluateExpression(pItem->m_strShowExpr, bProjectReadOnly, bProjectOpen, bSourceReadOnly))
                    {
                        return FALSE;
                    }
                    if(!EvaluateExpression(pItem->m_strEnableExpr, bProjectReadOnly, bProjectOpen, bSourceReadOnly))
                    {
                        return FALSE;
                    }
                }
                return TRUE;
            }
        }
    }
    return FALSE;
}


//------------------------------------------------------------------*
/**
 * simple evaluate expression.
 *
 *  project read only?
 *  project open?
 *  source read only?
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
BOOL CNavTreeCtrl::EvaluateExpression(const CString& crstrExpression, BOOL bProjectReadOnly, BOOL bProjectOpen, BOOL bSourceReadOnly)
{
    if(crstrExpression.Find(_T("source!=readonly")) >= 0 && bSourceReadOnly)
    {
        return FALSE;
    }
    if(crstrExpression.Find(_T("source=readonly")) >= 0 && !bSourceReadOnly)
    {
        return FALSE;
    }
    if(crstrExpression.Find(_T("project!=readonly")) >= 0 && bProjectReadOnly)
    {
        return FALSE;
    }
    if(crstrExpression.Find(_T("project=readonly")) >= 0 && !bProjectReadOnly)
    {
        return FALSE;
    }
    if(crstrExpression.Find(_T("project!=open")) >= 0 && bProjectOpen)
    {
        return FALSE;
    }
    if(crstrExpression.Find(_T("project=open")) >= 0 && !bProjectOpen)
    {
        return FALSE;
    }
    return TRUE;
}

//------------------------------------------------------------------*
/**
 * rename node.
 *
 * @param           crstrIdPath: node id path
 * @param           crstrNewId: new id
 * @return          
 * @exception       -
 * @see             
*/
void CNavTreeCtrl::RenameNode(const CString& crstrIdPath, const CString& crstrNewId)
{
    CXMLNode    tNode;
    CString     strType;
    CString     strId;
    CKADMenu*   pTypeMenu = NULL;

    if(crstrNewId.IsEmpty())
    {
        return;
    }

    if(FindTreeNode(crstrIdPath, tNode))
    {
        tNode.GetIdPath(m_strContextIdPath);
        tNode.GetAttribute(CE_XMLATTR_TYPE, strType);
        tNode.GetAttribute(CE_XMLATTR_ID, strId);

        pTypeMenu = m_pKADInfo->GetContextMenuInfoForType(strType);
        if(pTypeMenu)
        {
            int iMenuItemNo = pTypeMenu->m_aiKeyMouseArray[KEY_MOUSE_RENAME];

            if(iMenuItemNo >= 0)
            {
                m_strRenameId = crstrNewId;
                if(!m_strRenameId.IsEmpty())
                {
                    DoContextAction(iMenuItemNo);
                }
            }
        }
    }
}


//------------------------------------------------------------------*
/**
 * execute kernel standard operation
 *
 * @param           iKSO: number of kernel standard operation
 * @param           rtTreeCursor: cursor to current tree item
 * @return          operation performed?
*/
BOOL CNavTreeCtrl::DoKernelStandardOperation(int iKSO, CXMLNode* pNode)
{
    CString     strSource;
    BOOL        bReturn = TRUE;     // operation found?
    if(!pNode)
    {
        return FALSE;
    }

    // is there only one target defined?
    CTargetSet* pTargetSet = CEGetTargetSet("Project Targets", false);
    if(!pTargetSet)
    {
        return FALSE;
    }

    int iTargetCount = -1;
    CString strTargetId;
    
    iTargetCount = pTargetSet->GetTargetCount();
    
    if (iTargetCount==1)
    {
        POSITION pos = pTargetSet->GetFirstTargetPosition();
        ASSERT(pos);
        CTargetOnlBase* pOneTarget = pTargetSet->GetNextTarget(pos);
        ASSERT(pOneTarget);
        strTargetId = pOneTarget->GetId();
    }

    CMainFrame* pMainFrame = NULL;
    pMainFrame = dynamic_cast<CMainFrame* > (AfxGetMainWnd ());
    ASSERT(pMainFrame);
    if (pMainFrame == NULL)
    {
        return FALSE;
    }
       


    switch(iKSO)
    {
    case KSO_OPEN:
        TRACE("kernel standard operation \"Open\"\n");
        if(pNode->GetAttribute(CE_XMLATTR_OPENSOURCE, strSource))
        {
            OpenSourceFile(strSource);
        }
        else if(pNode->GetAttribute(CE_XMLATTR_SOURCE, strSource))
        {
            OpenSourceFile(strSource);
        }
        else
        {
            bReturn = FALSE;
        }
        break;
    case KSO_OPEN_USER_HELP:
        TRACE("kernel standard operation \"OpenUserHelp\"\n");
        //if (pNode->GetAttribute(_T("UserHelpURL"), strSource))
        if (pNode->GetAttribute(CE_XMLATTR_USER_HELP, strSource))
        {
            OpenUrl(strSource);
        }
        break;
    case KSO_OPEN_URL:
        TRACE("kernel standard operation \"OpenUrl\"\n");
        //if (pNode->GetAttribute(_T("UserURL"), strSource))
        if (pNode->GetAttribute(CE_XMLATTR_URL, strSource))
        {
            OpenUrl(strSource);
        }
        break;
    case KSO_GOTO:
        TRACE("kernel standard operation \"Goto\"\n");
        {
            CString strSource;
            CString strLocation;

            pNode->GetAttribute(CE_XMLATTR_SOURCE, strSource, TRUE);
            pNode->GetAttribute(CE_XMLATTR_LOCATION, strLocation);

            OpenSourceFile(strSource, strLocation);
        }
        break;
    case KSO_GOTO_DEFINITION:
        TRACE("kernel standard operation \"GotoDefinition\"\n");
        {
            // get data type
            CString strDataType;
            pNode->GetAttribute(CE_XMLATTR_DATA_TYPE, strDataType);
            if(!strDataType.IsEmpty())
            {
                CPouVarSelWindow *pPouVarSel = NULL;
                pPouVarSel = pMainFrame->GetPouVarSel();
                if (pPouVarSel != NULL)
                {
                    CPouVarSelCtrlWrapper* pPouVarSelWrapper = NULL;
                    pPouVarSelWrapper = pPouVarSel->GetWrapper();
                    if (pPouVarSelWrapper != NULL)
                    {
                        CString strFileName;
                        pPouVarSelWrapper->LookupHeader(strDataType, strFileName);

                        if (!strFileName.IsEmpty())
                        {
                            OpenSourceFile(strFileName);
                        }
                    }
                }
            }
        }
        break;
    case KSO_DELETE:
        TRACE("kernel standard operation \"Delete\"\n");
        {
            UnlinkNode(pNode);
        }
        break;
    
    case KSO_CONNECT:
        TRACE("kernel standard operation \"Connect\"\n");
        {
            CString         strId;
            CMsgWnd*        pMsgWnd = (dynamic_cast<CMsgWnd *> (CEGetDockingTool ("Message View", true)));
            if(pMsgWnd)
            {
                pNode->GetAttribute(CE_XMLATTR_ID, strId);
                pMsgWnd->ConnectTarget(strId);
            }
        }
        break;
    case KSO_DISCONNECT:
        TRACE("kernel standard operation \"Disconnect\"\n");
        {
            CString         strId;
            CMsgWnd*        pMsgWnd = (dynamic_cast<CMsgWnd *> (CEGetDockingTool ("Message View", true)));
            if(pMsgWnd)
            {
                pNode->GetAttribute(CE_XMLATTR_ID, strId);
                pMsgWnd->DisconnectTarget(strId);
            }
        }
        break;
    case KSO_CONNECT_EX:
        TRACE("kernel standard operation \"ConnectEx\"\n");
        {
            CString         strId;
            CMsgWnd*        pMsgWnd = (dynamic_cast<CMsgWnd *> (CEGetDockingTool ("Message View", true)));
            if(pMsgWnd)
            {
                pNode->GetAttribute(CE_XMLATTR_ID, strId);
                pMsgWnd->ConnectTargetExclusive(strId);
            }
        }
        break;
    case KSO_DISCONNECT_EX:
        TRACE("kernel standard operation \"DisconnectEx\"\n");
        {
            CString         strId;
            CMsgWnd*        pMsgWnd = (dynamic_cast<CMsgWnd *> (CEGetDockingTool ("Message View", true)));
            if(pMsgWnd)
            {
                pNode->GetAttribute(CE_XMLATTR_ID, strId);
                pMsgWnd->DisconnectTargetExclusive(strId);
            }
        }
        break;


    case KSO_DOWNLOAD_FULL:
        TRACE("kernel standard operation \"DownloadFull\"\n");
        {
            CString strId;
            CString strConnectInfo;

            if (pNode->GetAttribute(CE_XMLATTR_ID, strId)
                && pNode->GetAttribute(CE_XMLATTR_ADDR, strConnectInfo))
            {
                pMainFrame->DownloadTarget(eTargetOne, CEDL_DOWNLD_ALL,
                                           strId, strConnectInfo);
            }
        }
        break;
    case KSO_DOWNLOAD_CHANGES:
        TRACE("kernel standard operation \"DownloadChanges\"\n");
        {
            CString strId;
            CString strConnectInfo;

            if (pNode->GetAttribute(CE_XMLATTR_ID, strId)
                && pNode->GetAttribute(CE_XMLATTR_ADDR, strConnectInfo))
            {
                pMainFrame->DownloadTarget(eTargetOne, CEDL_DOWNLD_CHANGES,
                                           strId, strConnectInfo);
            }
        }
        break;
    case KSO_DOWNLOAD_SELECTION:
        TRACE("kernel standard operation \"DownloadSelection\"\n");
        {
            CString strId;
            CString strConnectInfo;

            if (pNode->GetAttribute(CE_XMLATTR_ID, strId)
                && pNode->GetAttribute(CE_XMLATTR_ADDR, strConnectInfo))
            {               
                pMainFrame->DownloadTarget(eTargetOne, CEDL_DOWNLD_SELECT,
                                           strId, strConnectInfo);
            }
        }
        break;
    case KSO_DOWNLOAD_FULL_ALL:
        TRACE("kernel standard operation \"DownloadFullAll\"\n");
        pMainFrame->DownloadTarget(eTargetAll, CEDL_DOWNLD_ALL);
        break;
    case KSO_DOWNLOAD_CHANGES_ALL:
        TRACE("kernel standard operation \"DownloadChangesAll\"\n");
        pMainFrame->DownloadTarget(eTargetAll, CEDL_DOWNLD_CHANGES);
        break;
    case KSO_DOWNLOAD_FULL_SELECT:
        TRACE("kernel standard operation \"DownloadFullSelect\"\n");
        if (iTargetCount==1)
        {
            pMainFrame->DownloadTarget(eTargetOne, CEDL_DOWNLD_ALL, strTargetId);
        }
        else
        {
            pMainFrame->DownloadTarget(eTargetSelect, CEDL_DOWNLD_ALL);
        }
        break;
    case KSO_DOWNLOAD_CHANGES_SELECT:
        TRACE("kernel standard operation \"DownloadChangesSelect\"\n");
        if (iTargetCount==1)
        {
            pMainFrame->DownloadTarget(eTargetOne, CEDL_DOWNLD_CHANGES, strTargetId);
        }
        else
        {
            pMainFrame->DownloadTarget(eTargetSelect, CEDL_DOWNLD_CHANGES);
        }
        break;
 

    case KSO_BUILD_ALL:
        TRACE("kernel standard operation \"BuildAll\"\n");
        pMainFrame->Build(eTargetAll);
        break;
    case KSO_BUILD_SELECT:
        TRACE("kernel standard operation \"BuildSelect\"\n");
        if (iTargetCount==1)
        {
            pMainFrame->Build(eTargetOne, strTargetId);
        }
        else
        {
            pMainFrame->Build(eTargetSelect);
        }
        break;
    case KSO_BUILD_TARGET:
        TRACE("kernel standard operation \"BuildTarget\"\n");
        {
            CString strId;  
            if (pNode->GetAttribute(CE_XMLATTR_ID, strId))
            {
                pMainFrame->Build(eTargetOne, strId);
                bReturn = TRUE;
            }
        }
        break;
    case KSO_COMPILE_FILE:
        TRACE("kernel standard operation \"CompileFile\"\n");
        ASSERT(!"KSO: not implemented as context menu kso!");
        break;
    case KSO_BUILD_LIBRARY:
        TRACE("kernel standard operation \"BuildLibrary\"\n");
        pMainFrame->BuildLibrary();
        break;
       

    case KSO_START_CONTROL:
        TRACE("kernel standard operation \"StartControl\"\n");
        {
            CString         strId;
            CXMLNode        tTargetNode;

            CMsgWnd*        pMsgWnd = (dynamic_cast<CMsgWnd *> (CEGetDockingTool ("Message View", true)));
            if(pMsgWnd)
            {
                if(pNode->GetParentNode(tTargetNode))
                {
                    tTargetNode.GetAttribute(CE_XMLATTR_ID, strId);
                    pMsgWnd->StartControl(strId);
                }
            }
        }
        break;
    case KSO_STOP_CONTROL:
        TRACE("kernel standard operation \"StopControl\"\n");
        {
            CString         strId;
            CXMLNode        tTargetNode;

            CMsgWnd*        pMsgWnd = (dynamic_cast<CMsgWnd *> (CEGetDockingTool ("Message View", true)));
            if(pMsgWnd)
            {
                if(pNode->GetParentNode(tTargetNode))
                {
                    tTargetNode.GetAttribute(CE_XMLATTR_ID, strId);
                    pMsgWnd->StopControl(strId);
                }
            }
        }
        break;
    case KSO_START_ALL_TASKS:
        TRACE("kernel standard operation \"StartAllTasks\"\n");
        {
            CString         strId;
            CXMLNode        tTargetNode;

            CMsgWnd*        pMsgWnd = (dynamic_cast<CMsgWnd *> (CEGetDockingTool ("Message View", true)));
            if(pMsgWnd)
            {
                if(pNode->GetParentNode(tTargetNode))
                {
                    tTargetNode.GetAttribute(CE_XMLATTR_ID, strId);
                    pMsgWnd->StartAllTasks(strId);
                }
            }
        }
        break;
    case KSO_STOP_ALL_TASKS:
        TRACE("kernel standard operation \"StopAllTasks\"\n");
        {
            CString         strId;
            CXMLNode        tTargetNode;

            CMsgWnd*        pMsgWnd = (dynamic_cast<CMsgWnd *> (CEGetDockingTool ("Message View", true)));
            if(pMsgWnd)
            {
                if(pNode->GetParentNode(tTargetNode))
                {
                    tTargetNode.GetAttribute(CE_XMLATTR_ID, strId);
                    pMsgWnd->StopAllTasks(strId);
                }
            }
        }
        break;
    case KSO_COLDSTART_RESET:
        TRACE("kernel standard operation \"ColdstartReset\"\n");
        {
            CString         strId;
            CMsgWnd*        pMsgWnd = (dynamic_cast<CMsgWnd *> (CEGetDockingTool ("Message View", true)));
            if(pMsgWnd)
            {
                pNode->GetAttribute(CE_XMLATTR_ID, strId);
                pMsgWnd->ColdStartReset(strId);
            }
        }
        break;
    case KSO_WARMSTART_RESET:
        TRACE("kernel standard operation \"WarmstartReset\"\n");
        {
            CString         strId;
            CMsgWnd*        pMsgWnd = (dynamic_cast<CMsgWnd *> (CEGetDockingTool ("Message View", true)));
            if(pMsgWnd)
            {
                pNode->GetAttribute(CE_XMLATTR_ID, strId);
                pMsgWnd->WarmStartReset(strId);
            }
        }
        break;
    case KSO_INITIALIZE_CLEAR:
        TRACE("kernel standard operation \"InitializeClear\"\n");
        {
            CString         strId;
            CMsgWnd*        pMsgWnd = (dynamic_cast<CMsgWnd *> (CEGetDockingTool ("Message View", true)));
            if(pMsgWnd)
            {
                pNode->GetAttribute(CE_XMLATTR_ID, strId);
                pMsgWnd->InitializeClear(strId);
            }
        }
        break;
    case KSO_START_TASK:
        TRACE("kernel standard operation \"StartTask\"\n");
        {
            // first get target name
            CString     strTarget;
            CString     strId;

            CXMLNode    tResourceNode;
            CXMLNode    tTargetNode;

            pNode->GetAttribute(CE_XMLATTR_ID, strId);

            if(pNode->GetParentNode(tResourceNode))
            {
                if(tResourceNode.GetParentNode(tTargetNode))
                {
                    tTargetNode.GetAttribute(CE_XMLATTR_ID, strTarget);
                }
            }

            CTargetOnlBase* pTarget = GetTargetOnlObject(strTarget);
            if(pTarget)
            {
                pTarget->StartTask(strId);
            }
        }
        break;
    case KSO_STOP_TASK:
        TRACE("kernel standard operation \"StopTask\"\n");
        {
            // first get target name
            CString     strTarget;
            CString     strId;

            CXMLNode    tResourceNode;
            CXMLNode    tTargetNode;

            pNode->GetAttribute(CE_XMLATTR_ID, strId);

            if(pNode->GetParentNode(tResourceNode))
            {
                if(tResourceNode.GetParentNode(tTargetNode))
                {
                    tTargetNode.GetAttribute(CE_XMLATTR_ID, strTarget);
                }
            }

            CTargetOnlBase* pTarget = GetTargetOnlObject(strTarget);
            if(pTarget)
            {
                pTarget->StopTask(strId);
            }
        }
        break;

    case KSO_SETTINGS_RESET_DOCKING:
        TRACE("kernel standard operation \"SettingsResetDocking\"\n");
        pMainFrame->DoSettingsResetDocking();
        break;
    case KSO_SETTINGS_LIBRARY:
        TRACE("kernel standard operation \"SettingsLibrary\"\n");
        pMainFrame->DoSettingsLibrary();
        break;
    case KSO_SETTINGS_ENGINEERING:
        TRACE("kernel standard operation \"SettingsEngineering\"\n");
        pMainFrame->DoSettingsEngineering();
        break;
    case KSO_SETTINGS_PASSWORD:
        TRACE("kernel standard operation \"SettingsPassword\"\n");
        pMainFrame->DoSettingsPassword();
        break;
    case KSO_DOWNLOAD_FLASH:
        TRACE("kernel standard operation \"DownloadFlash\"\n");
        {
            CString strId;
            CString strConnectInfo;

            if (pNode->GetAttribute(CE_XMLATTR_ID, strId)
                && pNode->GetAttribute(CE_XMLATTR_ADDR, strConnectInfo))
            {
                pMainFrame->DownloadTarget(eTargetOne, CEDL_DOWNLD_ALL,
                                           strId, strConnectInfo , DOWNLD_FLASH); 
            }
        }
        break;
    case KSO_UPDATE_FLASH:
        TRACE("kernel standard operation \"UpdateFlash\"\n");
        {
            CString strId;
            CString strConnectInfo;

            if (pNode->GetAttribute(CE_XMLATTR_ID, strId)
                && pNode->GetAttribute(CE_XMLATTR_ADDR, strConnectInfo))
            {
                pMainFrame->DownloadTarget(eTargetOne, CEDL_DOWNLD_ALL, strId, strConnectInfo, DOWNLD_FLASH_ONLY); 
            }
        }
        break;

    case KSO_CLEAR_FLASH:
        TRACE("kernel standard operation \"ClearFlash\"\n");
        {
            CString         strId;
            pNode->GetAttribute(CE_XMLATTR_ID, strId);

            CTargetSet*     pTargetSet = CEGetTargetSet("Project Targets", false);
            ASSERT(pTargetSet);
            if(pTargetSet)
            {
                CTargetOnlBase* pTarget = pTargetSet->GetTarget(strId);	// SIS bug fix #136
                if(pTarget)
                {
					// 15.12.05 SIS >>
					// change tab if necessary
		            CMsgWnd* pMsgWnd = (dynamic_cast<CMsgWnd *> (CEGetDockingTool ("Message View", true)));
					pMsgWnd->ShowTargetWindow(pTarget->GetId());
					// 15.12.05 SIS <<
                    HRESULT hr = pTarget->ClearFlash();
                }
            }
        }
        break;

    default:
        bReturn = FALSE;
        break;

    }
    return bReturn;
}


//------------------------------------------------------------------*
/**
 * do exe operation.
 *
 *  perform KAD exe operation using CMainFrame::DoExeOperation()
 *
 * @param           rtNode: current xml node
 * @param           pItem: KAD menu item
 * @return          -
 * @exception       -
 * @see             CMainFrame::DoExeOperation()
*/
void CNavTreeCtrl::DoExeOperation(CXMLNode& rtNode, CKADMenuItem* pItem)
{
    CString strExeCommand = pItem->m_strExe;
    ExpandAttributeMacros(rtNode, strExeCommand);

    TRACE("exe operation: %s %s (%s)\n", pItem->m_strText, strExeCommand, pItem->m_strCommand);
    
    CMainFrame* pMainFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
    pMainFrame->DoExeOperation(pItem, strExeCommand);
}

//------------------------------------------------------------------*
/**
 * do add on action.
 *
 * @param            pItem: menu item pointer
 * @param            bSilent: silent mode?
 *                        not in combination with rename since additional info
 *                        is used for both.
 * @return           
 * @exception        -
 * @see              
*/
void CNavTreeCtrl::DoAddOnAction(CKADMenuItem* pItem, BOOL bSilent /*=FALSE*/)
{
    CXMLNode*       pNode;          // current node
    CString         strSource;      // source file
    CString         strId;          // ID
    CTreeCursor     tContextCursor;

    if(!GetTreeCursorFromNodeIdPath(m_strContextIdPath, tContextCursor))
    {
        return;
    }

    pNode = (CXMLNode*)tContextCursor.GetData();
    ASSERT(pNode);
    if(!pNode)
    {
        return;
    }

    pNode->GetAttribute(CE_XMLATTR_SOURCE, strSource, TRUE);
    //pNode->GetAttribute(CE_XMLATTR_ID, strId);
    pNode->GetIdPath(strId);

    TRACE("addon action %s: %s %s %s\n", pItem->m_strText, pItem->m_strCommand, strSource, strId);

    // if no handler is given, return
    if(!pItem->m_pHandler)
    {
        TRACE("handler is null!\n");
        return;
    }

    // get addon action interface
    HRESULT                     hr;
    CGuid                       tGuid(*(pItem->m_pHandler));
    CComPtr<IUnknown>           pTmp;
    CComQIPtr<ICEAddonAction>   pAddOnIF;
    CString                     strGuid = tGuid;

    pTmp.Attach(CEGetCOMByCLSID(strGuid, true));

    pAddOnIF = pTmp;
    if(!pAddOnIF)
    {
        return;
    }

    // call do action
    CString     strAdditionalInfo = m_strRenameId;
    if(bSilent)     // not to be used in combination with rename!
    {
        strAdditionalInfo = _T("SILENT");
    }
    CComBSTR    sMethodName(pItem->m_strCommand);
    CComBSTR    sSourceFile(strSource);
    CComBSTR    sId(strId);
    CComBSTR    sAdditionalInfo(strAdditionalInfo);
    ASSERT(m_pProjInfo);
    if(!m_pProjInfo)
    {
        return;
    }

    m_pNavigator->EnableFileCheck(FALSE);

    hr = pAddOnIF->doAction(sMethodName, sSourceFile, sId, m_pProjInfo, sAdditionalInfo);

    if(!bSilent)
    {
        if(hr == S_OK)
        {
            m_pProjInfo->fileParseNow(NULL);
        }
        else if(hr != S_FALSE)
        {
            ::MessageBeep(MB_ICONASTERISK);
        }
    }
    m_pNavigator->EnableFileCheck(TRUE);
}



//------------------------------------------------------------------*
/**
 * do drag drop action.
 *
 * @param           rtDragCursor: cursor to item where drag started
 * @param           rtDropCursor: cursor to item where drop occurred
 * @param           dropEffect: drop effect for drag operation (link or copy)
 * @param           rtDragItem: dragged data
 * @return          successful?
 * @exception       -
 * @see             
*/
BOOL CNavTreeCtrl::DoDragDropAction
(
    CTreeCursor& rtDragCursor,
    CTreeCursor& rtDropCursor,
    DROPEFFECT dropEffect,
    CXMLNodeDragItem& rtDragItem
)
{
    CXMLNode*       pDragNode;
    CXMLNode*       pDropNode;
    CString         strSource;      // source file
    CString         strId;          // ID
    CString         strType;        // Type

    if(!HTREEITEM(rtDragCursor) || !HTREEITEM(rtDropCursor))
    {
        return FALSE;
    }

    pDragNode = (CXMLNode*)rtDragCursor.GetData();
    pDropNode = (CXMLNode*)rtDropCursor.GetData();
    ASSERT(pDragNode && pDropNode);
    if(!pDragNode || !pDropNode)
    {
        return FALSE;
    }

    GUID    tGUIDTmp;

    if(!GetDropHandler(pDragNode, pDropNode, tGUIDTmp))
    {
        TRACE("CNavTreeCtrl::DoDragDropAction: Drop Handler not found\n");
        return FALSE;
    }

    pDropNode->GetAttribute(CE_XMLATTR_SOURCE, strSource, TRUE);
    pDropNode->GetAttribute(CE_XMLATTR_TYPE, strType);
    pDropNode->GetIdPath(strId);

    TRACE("drag drop action: OnDrop %s %s\n", strSource, strId);

    // get addon action interface
    CGuid                           tGuid(tGUIDTmp);
    CComPtr<IUnknown>               pTmp;
    CComQIPtr<ICEDragDrop>          pDragDropIF;
    CString                         strGuid = tGuid;

    pTmp.Attach(CEGetCOMByCLSID(strGuid, true));

    pDragDropIF = pTmp;
    if(!pDragDropIF)
    {
        return FALSE;
    }

    // call drag drop
    CString strDragSource = rtDragItem.m_strProjPath + _T("\\") + rtDragItem.m_strSource;
    IUnknown*       pIDragNode;
    CXMLDocument    tXMLDocTmp;
    tXMLDocTmp.LoadXMLString(rtDragItem.m_strXMLString);
    CXMLNode        tNodeTmp;
    if(!tXMLDocTmp.GetRootNode(tNodeTmp))
    {
        return FALSE;
    }

    if(!tNodeTmp.GetIXMLNodeUnknown(&pIDragNode))
    {
        return FALSE;
    }

    CComBSTR    sDropSourceFile(strSource);
    CComBSTR    sDropId(strId);
    CComBSTR    sDropType(strType);

    CComQIPtr<ICEProjInfo>    pICEProjectInfo;
    pICEProjectInfo = m_pProjMan;

    if(!pICEProjectInfo)
    {
        TRACE("No project info available.\n");
        return FALSE;
    }
    pDragDropIF->OnDrop(pIDragNode,
                        sDropSourceFile,
                        sDropId,
                        sDropType,
                        dropEffect,
                        pICEProjectInfo);

    pIDragNode->Release();
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * save all files.
 *
 *  get editor container and call SaveAll()
 *
 * @param            bAskBefore: ask before saving?
 *
*/
void CNavTreeCtrl::SaveAllFiles(bool bAskBefore /*=false*/)
{
    CCEMAINApp* pApp = (CCEMAINApp*) AfxGetApp();
    CGenEdCont* pContainer = pApp->GetContainer();
    if(pContainer)
    {
        pContainer->SaveAll(bAskBefore);
    }
}

//------------------------------------------------------------------*
/**
 * clear tree control.
 *
 *  clear tree control complete (destruction) or just for building 
 *  the tree anew
 *  called by Unload()
 *
 * @param            bUnloadXML: is XML to be unloaded?
 * @see              Unload()
*/
void CNavTreeCtrl::ClearTree(BOOL bUnloadXML /*=TRUE*/)
{
    // if window is already destroyed, return
    if(!GetSafeHwnd() || !HTREEITEM(m_tCursorRoot))
        return;

    DeleteTreeNodeData(m_tCursorRoot);
    DeleteAllItems();
    m_tCursorRoot = CTreeCursor();
//    m_pNodeRoot = NULL;

    // if XML is to be unloaded, all XML data is deleted
    if(bUnloadXML)
    {
        UnloadXML();
    }
    // else initialize DOM again with old document
    else
    {
        InitializeDOM();
    }
}


//------------------------------------------------------------------*
/**
 * build up tree structure from DOM recursively.
 *
 *  fills child items with data from DOM nodes
 *  and calls function for newly created tree items recursively.
 *  used by BuildTree()
 *
 * @param           rtTreeCursor: cursor to tree item
 * @return          successful?
 * @see             BuildTree()
*/
BOOL CNavTreeCtrl::FillChildNodes(CTreeCursor& rtTreeCursor)
{
    if(!HTREEITEM(rtTreeCursor))
    {
        return FALSE;
    }

    CXMLNode*       pNode;              // current xml-node
    CXMLNodeList    tNodeList;          // list of children of current node
    CXMLNode        tNodeChildTmp;      // temporary child node
    CTreeCursor     tTreeCursorChild;   // tree cursor for new node
    long            lNumChildren;       // number of children of current node
    long            lChild;             // number of current child in child list
    CString         strType;            // KAD-type of node
    CString         strText;            // node text of current node
    CString         strVis;             // visibility of node
    CString         strFilterPassed;    // filter passed
    BOOL            bFilterPassed;      // filter passed?
    BOOL            bFilter = IsFilterActive();
    BOOL            bVisible;
    int             iImageId;
    int             iOverlayId;

    pNode = (CXMLNode*)rtTreeCursor.GetData();
    ASSERT(pNode);
    if(!pNode)
    {
        return FALSE;
    }

    // get all children from pNode
    if(pNode->GetChildNodeList(tNodeList))
    {
        lNumChildren = tNodeList.GetLength();
        for(lChild = 0; lChild < lNumChildren; ++lChild)
        {
            // get child
            tNodeList.Item(lChild, tNodeChildTmp);
            // get node type
            tNodeChildTmp.GetAttribute(CE_XMLATTR_TYPE, strType);
            tNodeChildTmp.GetAttribute(CE_XMLATTR_TEXT, strText);
            tNodeChildTmp.GetAttribute(CE_XMLATTR_VISIBILITY, strVis);
            bFilterPassed = tNodeChildTmp.GetAttribute(CE_XMLATTR_FILTER_VIS, strFilterPassed);
            if(strVis != CE_XMLATTR_FALSE_VAL && m_pKADInfo->IsTypeVisible(strType))
            {
                bVisible = TRUE;
            }
            else
            {
                bVisible = FALSE;
            }

            if(bVisible && !(bFilter && !bFilterPassed))
            {
                // add new tree control item for file
                GetImageIds(&tNodeChildTmp, iImageId, iOverlayId);
                tTreeCursorChild = AddItemTail(strText, rtTreeCursor, tNodeChildTmp, iImageId);
                if(iOverlayId >= 0)
                {
                    SetItemState(tTreeCursorChild, INDEXTOOVERLAYMASK(iOverlayId), TVIS_OVERLAYMASK);
                }
                //m_tNodeItemMap.SetAt(pNodeChild, tTreeCursor);
                FillChildNodes(tTreeCursorChild);
            }
        }
    }
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * delete tree node and all children.
 *
 *  uses DeleteTreeNodeData()
 *
 * @param           rtTreeCursor: cursor to tree item
 * @return          void
 * @see             DeleteTreeNodeData()
*/
void CNavTreeCtrl::DeleteTreeItem(CTreeCursor& rtTreeCursor)
{
#ifdef _DEBUG
    CString strText(rtTreeCursor.GetText());
    TRACE("DeleteTreeItem: %s\n", strText);
#endif

    DeleteTreeNodeData(rtTreeCursor);
    // dont remove root item
    if(HTREEITEM(m_tCursorRoot) == HTREEITEM(rtTreeCursor))
    {
        return;
    }
    rtTreeCursor.Delete();
}

//------------------------------------------------------------------*
/**
 * delete tree node data recursively.
 *
 *  before a tree item can be deleted, the stored node pointers have to
 *  be deleted.
 *  delete node data for current tree item and start method for all children
 *
 * @param           rtTreeCursor: cursor to starting item
 * @return          successful?
 * @see             DeleteTreeItem()
*/
BOOL CNavTreeCtrl::DeleteTreeNodeData(CTreeCursor& rtTreeCursor)
{
    CXMLNode*           pNode;          // current node
    CTreeCursor         tTreeCursor;    // tree cursor for new node

    if(!HTREEITEM(rtTreeCursor))
    {
        return TRUE;
    }

    // delete data of node
    pNode = (CXMLNode*)rtTreeCursor.GetData();
    if(!pNode)
    {
        return TRUE;
    }

    delete pNode;
    rtTreeCursor.SetData((DWORD)NULL);

    //m_tNodeItemMap.RemoveKey(pNode);

    // run over all children from rtTreeCursor
    tTreeCursor = rtTreeCursor.GetChild();
    if(HTREEITEM(tTreeCursor))
    {
        do
        {
            DeleteTreeNodeData(tTreeCursor);
            tTreeCursor = tTreeCursor.GetNextSibl();
        } while(HTREEITEM(tTreeCursor));
    }

    return TRUE;
}

// 

//------------------------------------------------------------------*
/**
 * filter nodes that are passing filter recursively.
 *
 *  if the text of the node or of child nodes is passing filters, the node
 *  is marked to be shown
 *  this method is used by BuildTree() and UpdateNode()
 *
 * @param           pNode: pointer to starting node
 * @return          is this node or any child passing filter? <-> is this node to be shown?
 * @see             BuildTree(), UpdateNode()
*/
BOOL CNavTreeCtrl::FilterNodes(CXMLNode* pNode)
{
    CXMLNodeList        tNodeList;      // list of children of current node
    CXMLNode            tNodeChild;     // current child node
    long                lNumChildren;   // number of children of current node
    long                lChild;         // number of current child in child list
    CString             strText;        // node text of current node
    CString             strType;        // node type of current node
    BOOL                bSetVisible = FALSE;    // is visible after filtering?

    ASSERT(pNode);

    pNode->GetAttribute(CE_XMLATTR_TEXT, strText);
    pNode->GetAttribute(CE_XMLATTR_TYPE, strType);
    if(IsPassingFilters(strText, strType))
    {
        bSetVisible = TRUE;
    }

    // run over children
    if(pNode->GetChildNodeList(tNodeList))
    {
        lNumChildren = tNodeList.GetLength();
        for(lChild = 0; lChild < lNumChildren; ++lChild)
        {
            // get child
            tNodeList.Item(lChild, tNodeChild);
            if(FilterNodes(&tNodeChild))
            {
                bSetVisible = TRUE;
            }
        }
    }
    if(bSetVisible)
    {
        pNode->SetAttribute(CE_XMLATTR_FILTER_VIS, CE_XMLATTR_TRUE_VAL);
    }
    else
    {
        pNode->RemoveAttribute(CE_XMLATTR_FILTER_VIS);
    }
    return(bSetVisible);
}



//------------------------------------------------------------------*
/**
 * expand, collapse or toggle tree item.
 *
 * @param           rtTreeCursor: reference to tree item
 * @param           nCode: code what to happen (TVE_EXPAND, TVE_COLLAPSE or TVE_TOGGLE)
*/
void CNavTreeCtrl::ExpandTreeItem(CTreeCursor& rtTreeCursor, UINT nCode /*=TVE_EXPAND*/, BOOL bRecursive /*=FALSE*/)
{
    if(!HTREEITEM(rtTreeCursor))
    {
        return;
    }

    CXMLNode*   pNode = (CXMLNode*)rtTreeCursor.GetData();
    BOOL        bExpanded = FALSE;
    CString     strExpanded;
    BOOL        bUpdateState = FALSE;

    ASSERT(pNode);
    if(!pNode)
    {
        return;
    }
    bExpanded = IsXMLNodeExpanded(pNode);
    switch(nCode)
    {
    case TVE_EXPAND:
        bUpdateState = ExpandXMLNode(pNode, TRUE);
        break;
    case TVE_COLLAPSE:
        bUpdateState = ExpandXMLNode(pNode, FALSE);
        break;
    case TVE_TOGGLE:
        if(bExpanded)
        {
            bUpdateState = ExpandXMLNode(pNode, FALSE);
        }
        else
        {
            bUpdateState = ExpandXMLNode(pNode, TRUE);
        }
        break;
    }
    rtTreeCursor.Expand(nCode);
    if(bRecursive)
    {
        CTreeCursor tTreeCursorChild = rtTreeCursor.GetChild();
        if(HTREEITEM(tTreeCursorChild))
        {
            do
            {
                ExpandTreeItem(tTreeCursorChild, nCode, bRecursive);
                tTreeCursorChild = tTreeCursorChild.GetNextSibl();
            } while(HTREEITEM(tTreeCursorChild));
        }
    }
    if(bUpdateState)
    {
        UpdateTreeItemState(rtTreeCursor, NULL, bRecursive);
    }
}

//------------------------------------------------------------------*
/**
 * update tree item status.
 *
 *  recursive call possible.
 *
 * @param            rtTreeCursor: cursor to tree item
 * @param            pNode: xml node
 * @return           bRecursive: recursive call? default FALSE
 * @exception        -
 * @see              -
*/
void CNavTreeCtrl::UpdateTreeItemState(CTreeCursor& rtTreeCursor, CXMLNode* pNode /*=NULL*/, BOOL bRecursive /*=FALSE*/)
{
    if(!HTREEITEM(rtTreeCursor))
    {
        return;
    }

    int iImageId;
    int iOverlayId;

    if(!pNode)
    {
        pNode = (CXMLNode*)rtTreeCursor.GetData();
        ASSERT(pNode);
        if(!pNode)
        {
            return;
        }
    }
    GetImageIds(pNode, iImageId, iOverlayId);
    rtTreeCursor.SetImage(iImageId, iImageId);

    CString strStateText;
    CString strNodeText;
    pNode->GetAttribute(CE_XMLATTR_TEXT, strNodeText);
    if(pNode->GetAttribute(CE_XMLATTR_STATETEXT, strStateText))
    {
        strNodeText += _T(": ");
        strNodeText += strStateText;
    }
    if(strNodeText.Compare(rtTreeCursor.GetText()) != 0)
        rtTreeCursor.SetText(strNodeText);
    

    if(iOverlayId >= 0)
    {
        SetItemState(rtTreeCursor, INDEXTOOVERLAYMASK(iOverlayId), TVIS_OVERLAYMASK);
    }
    if(bRecursive)
    {
        CTreeCursor tTreeCursorChild = rtTreeCursor.GetChild();
        CXMLNode* pNode;
        if(HTREEITEM(tTreeCursorChild))
        {
            do
            {
                pNode = (CXMLNode*)tTreeCursorChild.GetData();
                ASSERT(pNode);
                if(pNode)
                {
                    // if child is expanded call ExpandNodeFromDom for this node in turn
                    UpdateTreeItemState(tTreeCursorChild, pNode, bRecursive);
                    tTreeCursorChild = tTreeCursorChild.GetNextSibl();
                }
            } while(HTREEITEM(tTreeCursorChild));
        }
    }
}


//------------------------------------------------------------------*
/**
 * expand XML node.
 *
 *  base class returns FALSE -> need not update state
 *
 * @param            pNode: pointer to XML node
 * @param            bExpand: is node to be expanded?
 *                                  default TRUE
 * @param            bRecursive: recursive expanding? default FALSE
 * @return           need to update state?
*/
BOOL CNavTreeCtrl::ExpandXMLNode(CXMLNode* pNode, BOOL bExpand /*=TRUE*/, BOOL bRecursive /*=FALSE*/)
{
    ASSERT(pNode);
    if(pNode)
    {
        if(bExpand)
        {
            pNode->SetAttribute(CE_XMLATTR_EXPANDED, CE_XMLATTR_TRUE_VAL);
        }
        else
        {
            pNode->SetAttribute(CE_XMLATTR_EXPANDED, CE_XMLATTR_FALSE_VAL);
        }
        if(bRecursive)
        {
            // run over all children and expand them
            long            lNumChildren;       // number of children of current node
            long            lChild;             // number of current child in child list
            CXMLNodeList    tNodeList;          // list of child nodes
            CXMLNode        tNodeChildTmp;      // temporary child node
            if(pNode->GetChildNodeList(tNodeList))
            {
                lNumChildren = tNodeList.GetLength();
                for(lChild = 0; lChild < lNumChildren; ++lChild)
                {
                    // get child
                    tNodeList.Item(lChild, tNodeChildTmp);
                    ExpandXMLNode(&tNodeChildTmp, bExpand, bRecursive);
                }
            }
        }
    }
    return FALSE;
}

// 
//------------------------------------------------------------------*
/**
 * expand tree structure from XML document.
 *
 *  is used for building up the last expand state of the tree
 *  if an item is expanded the function will be called for every child.
 *
 * @param           rtTreeCursor: starting item
 * @return          successful?
*/
BOOL CNavTreeCtrl::ExpandNodeFromDom(CTreeCursor& rtTreeCursor)
{
    if(!HTREEITEM(rtTreeCursor))
    {
        return FALSE;
    }

    // pointer of DOM-Element is stored in tree control item -> get it
    CXMLNode*       pNode = (CXMLNode*)rtTreeCursor.GetData();
    CTreeCursor     tTreeCursorChild;   // tree cursor to child

    ASSERT(pNode);
    if(!pNode)
    {
        return FALSE;
    }

    // restore expand state from DOM:
    if(IsXMLNodeExpanded(pNode))
    {
        rtTreeCursor.Expand(TVE_EXPAND);
    }
    else
    {
        rtTreeCursor.Expand(TVE_COLLAPSE);
    }

    // run over all children from rtTreeCursor
    tTreeCursorChild = rtTreeCursor.GetChild();
    if(HTREEITEM(tTreeCursorChild))
    {
        do
        {
            // if child is expanded call ExpandNodeFromDom for this node in turn
            ExpandNodeFromDom(tTreeCursorChild);
            tTreeCursorChild = tTreeCursorChild.GetNextSibl();
        } while(HTREEITEM(tTreeCursorChild));
    }
    return TRUE;
}


// 
//------------------------------------------------------------------*
/**
 * is XML-node expanded?.
 *
 * @param           pNode: pointer to XML node
 * @return          is XML-node expanded?
*/
BOOL CNavTreeCtrl::IsXMLNodeExpanded(CXMLNode* pNode)
{
    CString strExpanded;
    if(pNode && pNode->GetAttribute(CE_XMLATTR_EXPANDED, strExpanded))
    {
        if(strExpanded == CE_XMLATTR_TRUE_VAL)
        {
            return TRUE;
        }
    }
    return FALSE;
}


//------------------------------------------------------------------*
/**
 * translate wildcard expression into regular expression
 *
 *  used in method SetFilters()
 *
 * @param           rstrFilter: wildcard expression
 * @return          regular expression
 * @see             SetFilters()
*/
//CString CNavTreeCtrl::GetRegularExpression(const CString& rstrFilter)
//{
//    CString cs_reg_ex = "^" + rstrFilter;
//    if(!m_bUseCase)
//        cs_reg_ex.MakeLower();
//    cs_reg_ex.Replace("\\", "\\\\");
//    cs_reg_ex.Replace(".", "\\.");
//    cs_reg_ex.Replace("*", ".*");
//    cs_reg_ex.Replace("?", ".");
//    cs_reg_ex.Replace("[", "\\[");
//    cs_reg_ex.Replace("]", "\\]");
//    cs_reg_ex += "$";
//    return(cs_reg_ex);
//}

//------------------------------------------------------------------*
/**
 * is file name passing filters?.
 *
 * @param           strText: text to be filtered
 * @return          is file name passing filters?
*/
BOOL CNavTreeCtrl::IsPassingFilters(CString strText, CString strType)
{
//    int         iRegError;

    // if no filter applies return TRUE
    if(!IsFilterActive())
        return TRUE;

    // first try category filter
    if(m_iSelCat > 0)
    {
        if(!m_pCatFilter->IsPassingTypeFilter(strType, m_iSelCat))
            return FALSE;
    }

    // if no cases to be considered make lower
    if(!m_bUseCase)
    {
        strText.MakeLower();
    }

    // text filter 1 is not in use at the moment
    // try filter 1
    if(m_bFilter1)
    {
//        iRegError = regexec(&m_tRegex1, strText, m_iGroups1, m_pRegMatch1, 0);

//        if(iRegError != 0 && iRegError != REG_NOMATCH)
//        {
//            TRACE("Error in IsPassingFilters: regexec failed\n");
//            return TRUE;    // in case of error do not filter
//        }
        if(!FC_StringMatchFilePattern(strText, m_strFilter1))
        {
            return FALSE;
        }
    }

    // if not passed return FALSE
//    if(iRegError == REG_NOMATCH)
//        return FALSE;

    // try filter 2
    if(m_bFilter2)
    {
//        iRegError = regexec(&m_tRegex2, strText, m_iGroups2, m_pRegMatch2, 0);
//
//        if(iRegError != 0 && iRegError != REG_NOMATCH)
//        {
//            TRACE("Error in IsPassingFilters: regexec failed\n");
//            return TRUE;    // in case of error do not filter
//        }
        if(!FC_StringMatchFilePattern(strText, m_strFilter2))
        {
            return FALSE;
        }
    }

    // if not passed return FALSE
//    if(iRegError == REG_NOMATCH)
//        return FALSE;

    return TRUE;
}

//------------------------------------------------------------------*
/**
 * reset filters.
 *
 *  delete filter data and reset members
*/
void CNavTreeCtrl::ResetFilters()
{
//    if(m_pRegMatch1)
//    {
//        delete [] m_pRegMatch1;
//        regfree(&m_tRegex1);
//        m_pRegMatch1 = 0;
//    }
//    if(m_pRegMatch2)
//    {
//        delete [] m_pRegMatch2;
//        regfree(&m_tRegex2);
//        m_pRegMatch2 = 0;
//    }
    m_strFilter1 = "";
    m_bFilter1 = FALSE;
    m_strFilter2 = "";
    m_bFilter2 = FALSE;
}

//------------------------------------------------------------------*
/**
 * get tree item from node-id-path
 *
 *  strip id from path and find child until path is empty or child cannot be found
 *  uses FindChildFromId()
 *
 * @param           rcsIdPath: node-id-path
 * @param           rtTreeCursor: tree cursor to item if found
 * @return          item found?
 * @see             FindChildFromId()
*/
BOOL CNavTreeCtrl::GetTreeCursorFromNodeIdPath(const CString& rcsIdPath, CTreeCursor& rtTreeCursor)
{
    CString         cs_id;                  // current id from id-path
    BOOL            bFound;                 // tree cursor for next child found?
    CTreeCursor     tTreeCursorChild;       // tree cursor to found child
    CString         csIdPath = rcsIdPath;   // id-path for using in StripIdFromIdPath

    ASSERT(HTREEITEM(m_tCursorRoot));

    // if path is empty -> return root cursor
    if(!StripIdFromIdPath(csIdPath, cs_id))
    {
        rtTreeCursor = m_tCursorRoot;
        return(TRUE);
    }
    // if path started with / (cs_id is empty) -> rtTreeCursor is root cursor
    if(cs_id == "")
    {
        rtTreeCursor = m_tCursorRoot;
        // if no id left -> return root cursor
        if(!StripIdFromIdPath(csIdPath, cs_id))
            return(TRUE);
        // strip project name
        if(!StripIdFromIdPath(csIdPath, cs_id))
            return(TRUE);
    }
    else
        ASSERT(HTREEITEM(rtTreeCursor));    // start from rtTreeCursor -> HTREEITEM must exist

    // find all ids in path
    do
    {
        bFound = FindChildFromId(rtTreeCursor, tTreeCursorChild, cs_id);
        if(!bFound)
            break;
        rtTreeCursor = tTreeCursorChild;
    } while(StripIdFromIdPath(csIdPath, cs_id));

    return(bFound);
}

//------------------------------------------------------------------*
/**
 * find child with given id
 *
 *  used by GetTreeCursorFromNodeIdPath()
 *
 * @param           rtTreeCursor: tree cursor to parent item
 * @param           rtChild: tree cursor to child item if found
 * @return          child found?
 * @see             GetTreeCursorFromNodeIdPath()
*/
BOOL CNavTreeCtrl::FindChildFromId(CTreeCursor& rtTreeCursor, CTreeCursor& rtChild, const CString& rcs_id)
{
    BOOL        b_found = FALSE;
    CXMLNode*   pNode;
    CString     cs_id;

    // run over all children from rtTreeCursor
    rtChild = rtTreeCursor.GetChild();
    if(HTREEITEM(rtChild))
    {
        do
        {
            pNode = (CXMLNode*)rtChild.GetData();
            ASSERT(pNode);
            if(pNode)
            {
                pNode->GetAttribute(CE_XMLATTR_ID, cs_id);
                if(cs_id.CompareNoCase(rcs_id) == 0)
                {
                    b_found = TRUE;
                    break;
                }
            }
            rtChild = rtChild.GetNextSibl();
        } while(HTREEITEM(rtChild));
    }
    return(b_found);
}


//------------------------------------------------------------------*
/**
 * start editing label.
 *
 * @param           description see MSDN
 * @return          -
 * @exception       -
 * @see             -
*/
void CNavTreeCtrl::OnBeginlabeledit(TV_DISPINFO* pDispInfo, LRESULT* pResult)
{
    CXMLNode*   pNode;
    CKADMenu*   pTypeMenu;
    int         iMenuItemNo;
    CString     strType;
    CString     strId;
    CString     strSource;

    *pResult = 1;   // generally prevent rename

    // editing is only allowed, if standard operation rename is available
    CTreeCursor tContextCursor = GetSelectedItem();

    pNode = (CXMLNode*)tContextCursor.GetData();
    if(!pNode)
    {
        return;
    }

    pNode->GetIdPath(m_strContextIdPath);
    pNode->GetAttribute(CE_XMLATTR_TYPE, strType);
    pNode->GetAttribute(CE_XMLATTR_ID, strId);

    if(!IsKeyMouseActionAllowed(*pNode, KEY_MOUSE_RENAME))
    {
        return;
    }

    pTypeMenu = m_pKADInfo->GetContextMenuInfoForType(strType);
    if(!pTypeMenu)
        return;

    iMenuItemNo = pTypeMenu->m_aiKeyMouseArray[KEY_MOUSE_RENAME];

    if(iMenuItemNo >= 0)
    {
        *pResult = 0;   // allow rename
        
        CEdit* edit;    // set rename text (== id of node)
        edit = GetEditControl();
        if (edit != NULL)
        {
            edit->SetWindowText(strId);
        }
    }
}

//------------------------------------------------------------------*
/**
 * end editing label.
 *
 *  this function is called, when the user ended with label editing
 *  if the key mouse array of the corresponding type has an entry
 *  the corresponding context menu item will be executed by calling
 *  DoContextAction()
 *  this behaviour may be overwritten by derived classes (e. g. file view)
 *
 * @param           pNMHDR: context information
 * @param           pResult: return value
 * @see             DoContextAction, OnBeginlabeledit()
*/
void CNavTreeCtrl::OnEndlabeledit(TV_DISPINFO* pDispInfo, LRESULT* pResult)
{
    CKADMenu*   pTypeMenu;
    int         iMenuItemNo;
    CString     strType;
    CXMLNode    tNode;
    FindTreeNode(m_strContextIdPath, tNode);
    tNode.GetAttribute(CE_XMLATTR_TYPE, strType);

    pTypeMenu = m_pKADInfo->GetContextMenuInfoForType(strType);
    if(!pTypeMenu)
        return;

    iMenuItemNo = pTypeMenu->m_aiKeyMouseArray[KEY_MOUSE_RENAME];

    if(iMenuItemNo >= 0)
    {
        if (pDispInfo->item.pszText)
        {
            m_strRenameId = pDispInfo->item.pszText;
            DoContextAction(iMenuItemNo);
        }
        else
        {
            // renaming cancled -> reset node text
            CEdit* edit;
            edit = GetEditControl();
            if (edit != NULL)
            {
                edit->SetWindowText(m_strRenameText);
            }
        }
    }
    m_strRenameId = "";
    m_strRenameText = "";
}


//------------------------------------------------------------------*
/**
 * store filter data for view
 *
 * @param           iSelCat: index of currently selected category filter
 * @param           iSelText: index of currently selected text filter
 * @param           pastrFilterText: pointer to array of text filter expressions 
 *                                        or NULL if not changed; default is NULL
 * @see             GetFilterData(), AddTextFilter()
*/
void CNavTreeCtrl::SetFilterData(int iSelCat, int iSelText, CStringArray* pastrFilterText /*=NULL*/)
{
    m_iSelCat = iSelCat;
    m_iSelText = iSelText;
    if(pastrFilterText)
    {
        m_astrFilterText.Copy(*pastrFilterText);
    }
}

//------------------------------------------------------------------*
/**
 * retrieve filter data for view
 *
 * @param           astrFilterCat: array of category filter names
 * @param           astrFilterText: array of text filter expressions
 * @param           iSelCat: index of currently selected category filter
 * @param           iSelText: index of currently selected text filter
 * @see             SetFilterData(), AddTextFilter()
*/
void CNavTreeCtrl::GetFilterData(int& riSelCat, int& riSelText, CStringArray& astrFilterCat, CStringArray& astrFilterText)
{
    astrFilterCat.RemoveAll();
    if(m_pCatFilter)
    {
        m_pCatFilter->GetFilterTexts(astrFilterCat);
    }
    else
    {
        astrFilterCat.Add("All");
    }
    astrFilterText.Copy(m_astrFilterText);
    riSelCat = m_iSelCat;
    riSelText = m_iSelText;
}

//------------------------------------------------------------------*
/**
 * initialize filters
 *
 * @see             Init()
*/
void CNavTreeCtrl::InitFilters()
{
    m_iSelCat = 0;
    m_astrFilterText.Add("*");
    m_iSelText = 0;
    //TODO: m_pKADInfo is to be used
}

//------------------------------------------------------------------*
/**
 * is filter active?
 *
 * @return          is filter active?
*/
BOOL CNavTreeCtrl::IsFilterActive()
{
    return(m_bFilter1 || m_bFilter2 || (m_iSelCat > 0));
}


//------------------------------------------------------------------*
/**
 * handle key down message
 *
 * @param           see WM_KEYDOWN message
 *
*/
void CNavTreeCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    // general key strokes:
    // escape: cancel cut operation
    if(nChar == VK_ESCAPE)
    {
        if(HTREEITEM(m_tDragCursor))
        {
            m_tDragCursor.SetState(0, TVIS_CUT);
        }
    }
    // key strokes referring to the selected tree item:
    CXMLNode*   pNode;
    CString     strType;
    CTreeCursor tContextCursor;

    tContextCursor = GetSelectedItem();
    if(HTREEITEM(tContextCursor))
    {
        pNode = (CXMLNode*)tContextCursor.GetData();
        if(pNode)
        {
            pNode->GetIdPath(m_strContextIdPath);
            pNode->GetAttribute(CE_XMLATTR_TYPE, strType);
            // get context menu information
            CKADMenu*   pTypeMenu;
            pTypeMenu = m_pKADInfo->GetContextMenuInfoForType(strType);
            if(pTypeMenu)
            {
                int iMenuItemNo = -1;
                int iCode = -1;

                // get context menu item for this key event
                switch(nChar)
                {
                case VK_RETURN:
                    iCode = KEY_MOUSE_OPEN;
                    break;
                case VK_DELETE:
                    iCode = KEY_MOUSE_DELETE;
                    break;
                case VK_INSERT:
                    iCode = KEY_MOUSE_INSERT;
                    break;
                }

                // if key mouse event is defined
                if(iCode > -1)
                {
                    iMenuItemNo = pTypeMenu->m_aiKeyMouseArray[iCode];
                    if(iMenuItemNo >= 0)
                    {
                        // perform associated context action and return
                        DoContextAction(iMenuItemNo);
                        return;
                    }
                    else
                    {
                        ::MessageBeep(MB_ICONEXCLAMATION);
                    }
                }
            }
        }
    }
	CTreeCtrlEx::OnKeyDown(nChar, nRepCnt, nFlags);
}


//------------------------------------------------------------------*
/**
 * handle begin drag message
 *
 * @param           see TVN_BEGINDRAG message
 *
*/
void CNavTreeCtrl::OnBegindrag(NM_TREEVIEW* pNMTreeView, LRESULT* pResult)
{
    CPoint		ptAction = pNMTreeView->ptDrag;
	CTreeCursor tTreeCursor(pNMTreeView->itemNew.hItem, this);
	m_tDragCursor = tTreeCursor;
    ASSERT(HTREEITEM(m_tDragCursor));

    CXMLNode*   pNode = (CXMLNode*)m_tDragCursor.GetData();

    if(pNode)
    {

        TRACE(_T("Begin Drag\n"));

        SetClipboardData(pNode, TRUE);
    }
	
	*pResult = 0;
}


//------------------------------------------------------------------*
/**
 * is drag of node allowed?
 *
 * @param           pNode: pointer to XML node to be dragged
 * @return          drag allowed?
 * @see             OnBegindrag()
*/
BOOL CNavTreeCtrl::IsDragAllowed(CXMLNode* pNode)
{
    if(!pNode)
    {
        return FALSE;
    }

    // drag is allowed if type is drag type
    CString             strType;
    CKADDragDropMap*    pKADDragDropMap = m_pKADInfo->GetDragDropMap();

    if(!pKADDragDropMap)
    {
        return FALSE;
    }

    pNode->GetAttribute(CE_XMLATTR_TYPE, strType);

    if(pKADDragDropMap->IsDragType(strType))
    {
        return TRUE;
    }
    return FALSE;
}


//------------------------------------------------------------------*
/**
 * is drop allowed.
 *
 * @param           pDragNode: drag node
 * @param           pDropNode: drop node
 * @return          is drop allowed?
*/
BOOL CNavTreeCtrl::IsDropAllowed(CXMLNode* pDragNode, CXMLNode* pDropNode)
{
    if(!pDragNode || !pDropNode)
    {
        return FALSE;
    }

    CString             strDragType;
    CString             strDropType;

    CKADDragDropMap*    pKADDragDropMap = m_pKADInfo->GetDragDropMap();

    if(!pKADDragDropMap)
    {
        return FALSE;
    }

    pDragNode->GetAttribute(CE_XMLATTR_TYPE, strDragType);
    pDropNode->GetAttribute(CE_XMLATTR_TYPE, strDropType);

    if(pKADDragDropMap->IsDropType(strDragType, strDropType))
    {
        CCEFileHandler* pFileHandler = ((CCEMAINApp*)AfxGetApp())->GetFileHandler();
        CString strSource;
        pDropNode->GetAttribute(CE_XMLATTR_SOURCE, strSource, TRUE);
        if(pFileHandler)
        {
            if(pFileHandler->GetFileStatus(strSource) == eFSNormal)
            {
                return TRUE;
            }
        }
    }
    return FALSE;
}

//------------------------------------------------------------------*
/**
 * get drop handler.
 *
 * @param           pDragNode: drag node
 * @param           pDropNode: drop node
 * @param           rtGUID: drop handler
 * @return          is drop allowed?
*/
BOOL CNavTreeCtrl::GetDropHandler(CXMLNode* pDragNode, CXMLNode* pDropNode, GUID& rtGUID)
{
    if(!pDragNode || !pDropNode)
    {
        return FALSE;
    }

    CString             strDragType;
    CString             strDropType;
    CString             strEnableExpression;
    BOOL                bSourceReadOnly = FALSE;
    BOOL                bProjectReadOnly = IsProjectReadOnly();
    CString             strDropSource;

    CKADDragDropMap*    pKADDragDropMap = m_pKADInfo->GetDragDropMap();

    if(!pKADDragDropMap)
    {
        return FALSE;
    }

    pDragNode->GetAttribute(CE_XMLATTR_TYPE, strDragType);
    pDropNode->GetAttribute(CE_XMLATTR_TYPE, strDropType);
    pDropNode->GetAttribute(CE_XMLATTR_SOURCE, strDropSource, TRUE);

    CCEFileHandler* pFileHandler = ((CCEMAINApp*)AfxGetApp())->GetFileHandler();
    if(pFileHandler)
    {
        CString strSourcePathName = pFileHandler->GetFilePathName(strDropSource);
        CFileStatus tFileStatus;
        if(CFile::GetStatus(strSourcePathName, tFileStatus))
        {
            if(tFileStatus.m_attribute & CFile::readOnly)
            {
                bSourceReadOnly = TRUE;
            }
        }
    }

    if(pKADDragDropMap->GetDropInfo(strDragType, strDropType, rtGUID, strEnableExpression))
    {
        if(!strEnableExpression.IsEmpty())
        {
            if(strEnableExpression.Find(_T("project=readonly")) >= 0 && !bProjectReadOnly)
            {
                return FALSE;
            }
            if(strEnableExpression.Find(_T("project=!readonly")) >= 0 && bProjectReadOnly)
            {
                return FALSE;
            }
            if(strEnableExpression.Find(_T("source=readonly")) >= 0 && !bSourceReadOnly)
            {
                return FALSE;
            }
            if(strEnableExpression.Find(_T("source=!readonly")) >= 0 && bSourceReadOnly)
            {
                return FALSE;
            }
        }
        return TRUE;
    }
    return FALSE;
}

//------------------------------------------------------------------*
/**
 * set clipboard data
 *
 * @param           pNode: pointer to node
 * @param           bDragDrop: start drag drop operation
 * @return          data copied?
*/
BOOL CNavTreeCtrl::SetClipboardData(CXMLNode* pNode, BOOL bDragDrop /*=FALSE*/)
{
    BOOL    bDataCached = FALSE;
    HRESULT hr;

    COleDataSource* pDataSource = new COleDataSource;
    if(!pDataSource)
    {
        ::AfxThrowMemoryException();
    }

    // cache xml format for editor
    hr = SetEditorClipboardData(pNode, pDataSource);
    if(!SUCCEEDED(hr))
    {
        delete pDataSource;
        return FALSE;
    }
    if(hr == S_OK)
    {
        bDataCached = TRUE;
    }

    // cache xml node format for navigator
    hr = SetInternalClipboardData(pNode, pDataSource);
    if(!SUCCEEDED(hr))
    {
        delete pDataSource;
        return FALSE;
    }
    if(hr == S_OK)
    {
        bDataCached = TRUE;
    }

    // if no data was cached -> return false
    if(!bDataCached)
    {
        delete pDataSource;
        return FALSE;
    }

    // copy cached data to clipboard
    if(bDragDrop)
    {
        pDataSource->DoDragDrop();
		pDataSource->InternalRelease();
    }
    else
    {
        pDataSource->SetClipboard();
    }

    return TRUE;
}


//------------------------------------------------------------------*
/**
 * set internal clipboard data.
 *
 * @param           pNode: pointer to node
 * @param           pDataSource: pointer to data source object
 * @return          S_OK:       data was copied successfully
 *                  S_FALSE:    no data was copied
 *                  E_FAIL:     error occurred
 * @see             SetClipboardData() 
*/
HRESULT CNavTreeCtrl::SetInternalClipboardData(CXMLNode* pNode, COleDataSource* pDataSource)
{
    if(!pNode)
    {
        return E_FAIL;
    }

    if(!IsDragAllowed(pNode))
    {
        return S_FALSE;
    }
    
    CString             strText;
    CString             strId;
    CString             strType;
    CString             strSource;
    CString             strXMLString;
    CXMLNodeDragItem    tDragItem;

    pNode->GetAttribute(CE_XMLATTR_TEXT, strText);
    pNode->GetAttribute(CE_XMLATTR_ID, strId);
    pNode->GetAttribute(CE_XMLATTR_TYPE, strType);
    pNode->GetAttribute(CE_XMLATTR_SOURCE, strSource);
    pNode->GetXMLString(strXMLString);

    tDragItem.SetData(strText, strId, strType, strSource, m_strProjectPath, strXMLString);
    tDragItem.SetCBFormatId(m_uiCBXMLNodeFormat);
    if(!tDragItem.CopyToClipboard(pDataSource))
    {
        return E_FAIL;
    }
    return S_OK;
}

//------------------------------------------------------------------*
/**
 * set clipboard data for editor.
 *
 * @param           pNode: pointer to node
 * @param           pDataSource: pointer to data source object
 * @return          S_OK:       data was copied successfully
 *                  S_FALSE:    no data was copied
 *                  E_FAIL:     error occurred
*/
HRESULT CNavTreeCtrl::SetEditorClipboardData(CXMLNode* pNode, COleDataSource* pDataSource)
{
    if(!pNode)
    {
        return E_FAIL;
    }
	try
	{
        CPouObject   tPouObj;

        if(!tPouObj.InitFromXMLNode(pNode))
        {
            return S_FALSE;
        }
        CString strPath = tPouObj.m_strName;
        int itemType = tPouObj.GetDBObjectType();
        CString strXML = tPouObj.GetDragAndDropString();
		
		//
		//construct the drag item
		//
        CComBSTR sXML = strXML;
		CXMLDragItem Item(strPath,itemType,sXML);
    
		CSharedFile GlobFile;
     
		CArchive ar(&GlobFile, CArchive::store);
		Item.Serialize(ar);
		ar.Close();
		pDataSource->CacheGlobalData(m_uiCBXMLFormat, GlobFile.Detach());
	}
	catch(CMemoryException *e)
	{
		//out of memory
		//TODO status output
		e->Delete();
        return E_FAIL;
	}
	catch(CFileException* e)
	{
		//file op failed
		//TODO status output
		e->Delete();
        return E_FAIL;
	}
	catch(CArchiveException* e)
	{
		//archive op failed
		//TODO status output
		e->Delete();
        return E_FAIL;
	}
	catch(HRESULT hrRes)
	{
		//hr exception
		//TODO status output
		HRESULT hr = hrRes;
        return E_FAIL;
	}
    return S_OK;
}



//------------------------------------------------------------------*
/**
 * on drop.
 *
*/
BOOL CNavTreeCtrl::OnDrop( CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
    BOOL bReturn = FALSE;

	if(pDataObject->IsDataAvailable(m_uiCBXMLNodeFormat))
    {
        CXMLNodeDragItem    tDragItem;

        tDragItem.SetCBFormatId(m_uiCBXMLNodeFormat);
        if(tDragItem.LoadFromClipboard(pDataObject))
        {

            CTreeCursor tDropCursor = GetNewInsertItem(point);
            if(HTREEITEM(tDropCursor) && HTREEITEM(m_tDragCursor))
            {
                if(!DoDragDropAction(m_tDragCursor, tDropCursor, dropEffect, tDragItem))
                {
                    ::MessageBeep(MB_ICONEXCLAMATION);
                }
                bReturn = TRUE;
            }
            // in this case the drag started from outside the navigator
            else
            {
//                CXMLDocument    tXMLDocTmp;
//                tXMLDocTmp.LoadXMLString(tDragItem.m_strXMLString);
//                CXMLNode        tNodeTmp;
//                InsertNodeFromExtern(tDropCursor, &tNodeTmp, tDragItem.m_strProjPath);
            }
        }
    }
    m_tDragCursor = CTreeCursor();    // set item drag to NULL
    OnDragLeave(this);
    return bReturn;
}


//------------------------------------------------------------------*
/**
 * on drag enter.
 *
 * @see             MSDN
*/
DROPEFFECT CNavTreeCtrl::OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
    // bring mainframe to front
    CMainFrame* pMainFrame = (CMainFrame*)::AfxGetMainWnd();
    if(pMainFrame)
    {
        pMainFrame->SetForegroundWindow();
    }

    if(!m_pDragImageList)
    {
        if(HTREEITEM(m_tDragCursor))
        {
            m_pDragImageList = CreateDragImage(m_tDragCursor);
    	    m_pDragImageList->DragShowNolock(TRUE);
            m_pDragImageList->BeginDrag(0, CPoint(0,0));
        }
    }
    return(OnDragOver(pWnd, pDataObject, dwKeyState, point));
}

//------------------------------------------------------------------*
/**
 * on drag leave.
 *
 * @see             MSDN
*/
void CNavTreeCtrl::OnDragLeave(CWnd* pWnd)
{
    SelectDropTarget(NULL);
    if(m_pDragImageList)
    {
	    m_pDragImageList->DragLeave(this);
	    m_pDragImageList->EndDrag();
	    delete m_pDragImageList;
	    m_pDragImageList = NULL;
        Invalidate();
    }
}

//------------------------------------------------------------------*
/**
 * on drag over.
 *
 * @see             MSDN
*/
DROPEFFECT CNavTreeCtrl::OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
    CXMLNode*   pDragNode = NULL;
    CXMLNode*   pDropNode;

    if(HTREEITEM(m_tDragCursor))
    {
        pDragNode = (CXMLNode*)m_tDragCursor.GetData();
        if(!pDragNode)
        {
            return DROPEFFECT_NONE;
        }
    }
    // drops from outside not allowed in base class
    else
    {
        return DROPEFFECT_NONE;
    }

	if(pDataObject && pDataObject->IsDataAvailable(m_uiCBXMLNodeFormat))
    {
	    CTreeCursor			tItem;

        if(m_pDragImageList)
        {
		    m_pDragImageList->DragMove(point);
            tItem = GetNewInsertItem(point);
            //tItem = HitTest(point);
		    if(HTREEITEM(tItem))
		    {
			    m_pDragImageList->DragLeave(this);
			    SelectDropTarget(tItem);
			    m_pDragImageList->DragEnter(this, point);
                pDropNode = (CXMLNode*)tItem.GetData();
                ASSERT(pDropNode);

                if(IsDropAllowed(pDragNode, pDropNode))
                {
                    return GetDropEffect(tItem, dwKeyState);
                }
		    }
        }
        else
        {
            //tItem = GetNewInsertItem(point);
            tItem = HitTest(point);
		    if(HTREEITEM(tItem))
		    {
			    SelectDropTarget(tItem);
                return GetDropEffect(tItem, dwKeyState);
		    }
        }
    }
    return DROPEFFECT_NONE;
}

//------------------------------------------------------------------*
/**
 * on drag enter.
 *
 *   is not called by now (see CNavTreeDropTarget)
 *
 * @see             MSDN, CNavTreeDropTarget.
*/
DROPEFFECT CNavTreeCtrl::OnDragScroll(CWnd* pWnd, DWORD dwKeyState, CPoint point)
{
    if(m_iScrollCounter == con_iScrollTimer)
    {
        m_iScrollCounter = 0;
        if(m_pDragImageList)
        {
	        m_pDragImageList->DragLeave(this);
        }
        // check, if point is outside 80% region
        CRect   tClientRect;
        CRect   tNoScrollRect;
        GetClientRect(&tClientRect);
        double  dfBorderFakt = 0.1;
        int     iBorderHeight = (int)(dfBorderFakt * tClientRect.Height());
        int     iBorderWidth = (int)(dfBorderFakt * tClientRect.Width());
        CTreeCursor tCursorSelected = HitTest(point);
        CTreeCursor tCursorNext;

        tNoScrollRect.top = tClientRect.top + iBorderHeight;
        tNoScrollRect.bottom = tClientRect.bottom - iBorderHeight;
        tNoScrollRect.left = tClientRect.left + iBorderWidth;
        tNoScrollRect.right = tClientRect.right - iBorderWidth;

        if(point.x < tNoScrollRect.left  || point.x > tNoScrollRect.right)
        {
            if(HTREEITEM(tCursorSelected))
            {
                EnsureVisible(tCursorSelected);
            }
        }

        if(point.y < tNoScrollRect.top)
        {
            tCursorSelected = GetFirstVisibleItem();
            if(HTREEITEM(tCursorSelected))
            {
                tCursorNext = tCursorSelected.GetPrevVisible();
                if(HTREEITEM(tCursorNext))
                {
                    EnsureVisible(tCursorNext);
                }
            }
        }

        if(point.y > tNoScrollRect.bottom)
        {
            tCursorSelected = GetFirstVisibleItem();
            if(HTREEITEM(tCursorSelected))
            {
                tCursorNext = tCursorSelected.GetNextVisible();
                if(HTREEITEM(tCursorNext))
                {
                    SelectSetFirstVisible(tCursorNext);
                }
            }
        }
    }

    ++m_iScrollCounter;
    return(DROPEFFECT_NONE);
}

//------------------------------------------------------------------*
/**
 * on drop ex.
 *
 *   is not called by now (see CNavTreeDropTarget)
 *
 * @see             MSDN, CNavTreeDropTarget.
*/
DROPEFFECT CNavTreeCtrl::OnDropEx(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, DROPEFFECT dropEffectList, CPoint point)
{
    return(DROPEFFECT_NONE);
}

//------------------------------------------------------------------*
/**
 * get drop effect
 *
 * @param           rtTreeCursor: cursor to current drop item
 * @param           dwKeyState: key state value
 * @return          DROPEFFECT for drag operation
 *
*/
DROPEFFECT CNavTreeCtrl::GetDropEffect(CTreeCursor& rtTreeCursor, DWORD dwKeyState)
{
    if(!HTREEITEM(m_tDragCursor))
    {
        return DROPEFFECT_COPY;
    }
    if(dwKeyState & MK_CONTROL)
    {
        return DROPEFFECT_COPY;
    }
    if(dwKeyState & MK_SHIFT)
    {
        return DROPEFFECT_MOVE;
    }
    if(dwKeyState & MK_ALT)
    {
        return DROPEFFECT_SCROLL;
    }
    return DROPEFFECT_LINK;
}

//------------------------------------------------------------------*
/**
 * pre translate message.
 *
 *   to get Return/Escape keys when renaming tree items.
 *
 * @see             MSDN, CNavTreeDropTarget.
*/
BOOL CNavTreeCtrl::PreTranslateMessage(MSG* pMsg)
{
    if( pMsg->message == WM_KEYDOWN )
    {
        // If it is a KEYDOWN message from an edit control of a systree control
        // check for return or escape keys

        if  (pMsg->wParam == VK_RETURN ||
             pMsg->wParam == VK_ESCAPE)
        {
            ::TranslateMessage(pMsg);
            ::DispatchMessage(pMsg);
            return TRUE;  // DO NOT process further
        }
    }

    return CTreeCtrlEx::PreTranslateMessage(pMsg);
}

//------------------------------------------------------------------*
/**
 * on left button double click.
 *
 *  call operation for key mouse event "open"
 *
 * @see             MSDN
*/
void CNavTreeCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
    CString strType;
    UINT    uiHitFlags;
    CTreeCursor tContextCursor = HitTest(point, &uiHitFlags);

    if(!(uiHitFlags & TVHT_ONITEM))
	{
        CTreeCtrlEx::OnLButtonDblClk(nFlags, point);
        return;
    }

    if(!HTREEITEM(tContextCursor))
    {
        return;
    }

//    SelectItem(tContextCursor);
    
    CXMLNode*   pNode = (CXMLNode*)tContextCursor.GetData();
    ASSERT(pNode);
    if(!pNode)
    {
        return;
    }
    pNode->GetIdPath(m_strContextIdPath);

    if(!IsKeyMouseActionAllowed(m_strContextIdPath, KEY_MOUSE_OPEN))
    {
        ::MessageBeep(MB_ICONASTERISK);
        return;
    }

    pNode->GetAttribute(CE_XMLATTR_TYPE, strType);

    // get context menu information
    CKADMenu*   pTypeMenu;
    pTypeMenu = m_pKADInfo->GetContextMenuInfoForType(strType);
    if(pTypeMenu)
    {
        // if key mouse event open is defined
        int iMenuItemNo;
        iMenuItemNo = pTypeMenu->m_aiKeyMouseArray[KEY_MOUSE_OPEN];
        if(iMenuItemNo >= 0)
        {
            // perform associated context action and return
            DoContextAction(iMenuItemNo);
            return;
        }
    }
    // else do kernel standard operation open
    if(!DoKernelStandardOperation(KSO_OPEN, pNode))
    {
        CTreeCtrlEx::OnLButtonDblClk(nFlags, point);
    }
}


// SHIFT_F10 15.12.04 SIS >>
void CNavTreeCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
    CString strType;
    UINT    uiHitFlags;
    CTreeCursor tContextCursor = HitTest(point, &uiHitFlags);

    if(HTREEITEM(tContextCursor))
    {
	    SelectItem(tContextCursor);
    }
}
// SHIFT_F10 15.12.04 SIS <<

//------------------------------------------------------------------*
/**
 * on erase background.
 *
 *  update background color depending on filter settings.
 *
 * @see             MSDN
*/
BOOL CNavTreeCtrl::OnEraseBkgnd(CDC* pDC) 
{
	ASSERT_VALID(pDC);

    if(IsFilterActive())
    {
        CBrush backBrush(m_tBgColFilter);
        CBrush* pOldBrush = pDC->SelectObject(&backBrush);
        CRect rect;
        pDC->GetClipBox(&rect);     // Erase the area needed

        pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
        pDC->SelectObject(pOldBrush);
        return TRUE;
    }
	return CTreeCtrlEx::OnEraseBkgnd(pDC);
}


//------------------------------------------------------------------*
/**
 * cut command (STRG + X)
 *
 *  is called from navigator
 *
 * @see             CNavigator::OnNavCut()
*/
void CNavTreeCtrl::OnCut()
{
    CXMLNode*   pNode;
    CString     strType;

    CEdit*  pEdit = GetEditControl();
    if(pEdit)
    {
        pEdit->Cut();
        return;
    }

    m_tDragCursor = GetSelectedItem();
    if(HTREEITEM(m_tDragCursor))
    {
        pNode = (CXMLNode*)m_tDragCursor.GetData();
        if(pNode)
        {
            pNode->GetAttribute(CE_XMLATTR_TYPE, strType);
            // get context menu information
            CKADMenu*   pTypeMenu;
            pTypeMenu = m_pKADInfo->GetContextMenuInfoForType(strType);
            if(pTypeMenu)
            {
                int iMenuItemNo = pTypeMenu->m_aiKeyMouseArray[KEY_MOUSE_CUT];
                if(iMenuItemNo >= 0)
                {
                    m_eContextAction = eCut;
                    DoContextAction(iMenuItemNo);
                    if(!m_pDragImageList)
                    {
                        m_pDragImageList = CreateDragImage(m_tDragCursor);
                    }
  	                m_pDragImageList->DragShowNolock(TRUE);
//                    m_pDragImageList->BeginDrag(0, CPoint(0,0));
                }
            }
        }
    }
}

//------------------------------------------------------------------*
/**
 * copy command (STRG + C)
 *
 *  is called from navigator
 *
 * @see             CNavigator::OnNavCopy()
*/
void CNavTreeCtrl::OnCopy()
{
    CXMLNode*   pNode;
    CString     strType;

    CEdit*  pEdit = GetEditControl();
    if(pEdit)
    {
        pEdit->Copy();
        return;
    }

    m_tDragCursor = GetSelectedItem();
    if(HTREEITEM(m_tDragCursor))
    {
        pNode = (CXMLNode*)m_tDragCursor.GetData();
        if(pNode)
        {
            pNode->GetAttribute(CE_XMLATTR_TYPE, strType);
            // get context menu information
            CKADMenu*   pTypeMenu;
            pTypeMenu = m_pKADInfo->GetContextMenuInfoForType(strType);
            if(pTypeMenu)
            {
                int iMenuItemNo = pTypeMenu->m_aiKeyMouseArray[KEY_MOUSE_COPY];
                if(iMenuItemNo >= 0)
                {
                    m_eContextAction = eCopy;
                    DoContextAction(iMenuItemNo);
                }
            }
        }
    }
}

//------------------------------------------------------------------*
/**
 * paste command (STRG + V)
 *
 *  is called from navigator
 *
 * @see             CNavigator::OnNavPaste()
*/
void CNavTreeCtrl::OnPaste()
{
    CXMLNode*   pNode;
    CString     strType;

    CEdit*  pEdit = GetEditControl();
    if(pEdit)
    {
        pEdit->Paste();
        return;
    }

    CTreeCursor tContextCursor = GetSelectedItem();
    if(HTREEITEM(tContextCursor))
    {
        pNode = (CXMLNode*)tContextCursor.GetData();
        if(pNode)
        {
            pNode->GetIdPath(m_strContextIdPath);
            pNode->GetAttribute(CE_XMLATTR_TYPE, strType);
            // get context menu information
            CKADMenu*   pTypeMenu;
            pTypeMenu = m_pKADInfo->GetContextMenuInfoForType(strType);
            if(pTypeMenu)
            {
                int iMenuItemNo = pTypeMenu->m_aiKeyMouseArray[KEY_MOUSE_PASTE];
                int iMenuItemDelete = pTypeMenu->m_aiKeyMouseArray[KEY_MOUSE_DELETE];

                if(iMenuItemNo >= 0 && iMenuItemDelete >= 0)
                {
                    DoContextAction(iMenuItemNo);
                    if(m_eContextAction == eCut)
                    {
                        // now delete context node
                        DoContextAction(iMenuItemDelete);
                    }
                }
            }
        }
    }
}

//------------------------------------------------------------------*
/**
 * on collapse all.
 *
 *  collapse all items starting from context cursor.
 *  called from navigator.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             OnExpandAll()
*/
void CNavTreeCtrl::OnCollapseAll()
{
    CTreeCursor tContextCursor = GetSelectedItem();
    if(HTREEITEM(tContextCursor))
    {
        ExpandTreeItem(tContextCursor, TVE_COLLAPSE, TRUE);
    }
}

//------------------------------------------------------------------*
/**
 * on expand all.
 *
 *  expand all items starting from context cursor.
 *  called from navigator.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             OnCollapseAll()
*/
void CNavTreeCtrl::OnExpandAll()
{
    CTreeCursor tContextCursor = GetSelectedItem();
    if(HTREEITEM(tContextCursor))
    {
        ExpandTreeItem(tContextCursor, TVE_EXPAND, TRUE);
    }
}

//------------------------------------------------------------------*
/**
 * show context menu.
 *
 *  called from navigator.
 *
 * @param           pWnd: parent window
 * @param           tPoint: current mouse position
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CNavTreeCtrl::ShowContextMenu(CWnd* pWnd, CPoint tPoint)
{
    CMenu   tMenu;

	// d-164 01.02.05 SIS >>
	// only show context menu, if HitTest() returns tree item
	// else docking toolbar context menu will never appear
	CPoint	tPointClient(tPoint);
	ScreenToClient(&tPointClient);
	CTreeCursor	tCursor = HitTest(tPointClient);
	if(!(HTREEITEM)tCursor)
	{
		return FALSE;
	}
	// d-164 01.02.05 SIS <<

    if(tMenu.CreatePopupMenu())
    {
        if(AddContextMenu(&tMenu))
        {
            tMenu.TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, tPoint.x, tPoint.y, this);
            return TRUE;
        }
    }
    return FALSE;
}

//------------------------------------------------------------------*
/**
 * add context menu for node.
 *
 *  used by project list view.
 *
 * @param           pMenu: context menu
 * @param           crstrIdPath: id path to node
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CNavTreeCtrl::AddContextMenuForNode(CMenu* pMenu, const CString& crstrIdPath)
{
    CXMLNode    tNode;
    if(FindTreeNode(crstrIdPath, tNode))
    {
        m_strContextIdPath = crstrIdPath;
        return(AddContextMenuForNode(pMenu, &tNode));
    }
    return FALSE;
}


//------------------------------------------------------------------*
/**
 * on context menu command.
 *
 *  command handler for context menu commands.
 *  call virtual OnCommand()
 *
 * @param           nID: command id.
 * @return          -
 * @exception       -
 * @see             -
*/
void CNavTreeCtrl::OnContextMenuCommand(UINT nID)
{
    OnCommand(nID);
}


//------------------------------------------------------------------*
/**
 * select last insert node.
 *
 *  select last inserted tree item.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CNavTreeCtrl::SelectLastInsertNode()
{
    CTreeCursor tTreeCursor;
    if(!m_strInsertIdPath.IsEmpty())
    {
        if(GetTreeCursorFromNodeIdPath(m_strInsertIdPath, tTreeCursor))
        {
            SelectItem(tTreeCursor);
        }
    }
}

//------------------------------------------------------------------*
/**
 * is tree path?.
 *
 * @param           crstrIdPath
 * @return          is tree path to existing node?
*/
BOOL CNavTreeCtrl::IsTreePath(const CString& crstrIdPath)
{
    BOOL        bReturn = FALSE;
    CXMLNode    tNodeRoot;
    CXMLNode    tNodeTmp;

    if(m_pXMLDoc)
    {
        if(m_pXMLDoc->GetRootNode(tNodeRoot))
        {
            if(tNodeRoot.GetNodeFromIdPath(crstrIdPath, tNodeTmp))
            {
                bReturn = TRUE;
            }
        }
    }
    return bReturn;
}


//------------------------------------------------------------------*
/**
 * find tree node.
 *
 * @param           crstrIdPath
 * @param           rtNode: found node
 * @return          is tree path to existing node?
*/
BOOL CNavTreeCtrl::FindTreeNode(const CString& crstrIdPath, CXMLNode& rtNode)
{
    BOOL        bReturn = FALSE;
    CXMLNode    tNodeRoot;

    if(m_pXMLDoc)
    {
        if(m_pXMLDoc->GetRootNode(tNodeRoot))
        {
            if(tNodeRoot.GetNodeFromIdPath(crstrIdPath, rtNode))
            {
                bReturn = TRUE;
            }
        }
    }
    return bReturn;
}


//------------------------------------------------------------------*
/**
 * get new insert item.
 *
 *  get item to show in selected mode during a drag and drop operation.
 *
 * @param           rtPoint: mouse position
 * @return          cursor to selected tree item
*/
CTreeCursor CNavTreeCtrl::GetNewInsertItem(CPoint& rtPoint)
{
    UINT		flags;
    CTreeCursor tHitCursor;

    tHitCursor = HitTest(rtPoint, &flags);
    return tHitCursor;
}


//------------------------------------------------------------------*
/**
 * delete nodes by id path.
 *
 *  used by project list view.
 *
 * @param           rastrIdPath: string array with node id paths
 * @return          successful?
 * @exception       -
 * @see             CCEMAINView::DeleteSelectedItems()
*/
BOOL CNavTreeCtrl::DeleteNodesByIdPath(CStringArray& rastrIdPath)
{
    int iNumIdPaths = rastrIdPath.GetSize();
    if(iNumIdPaths <= 0)
    {
        return TRUE;
    }

    CXMLNode    tNodeRoot;
    if(!m_pXMLDoc)
    {
        return FALSE;
    }
    if(!m_pXMLDoc->GetRootNode(tNodeRoot))
    {
        return FALSE;
    }

    // get neighbour item to select after deletion
    CTreeCursor tTreeCursor;
    CTreeCursor tNeighbour;
    if(GetTreeCursorFromNodeIdPath(rastrIdPath[0], tTreeCursor))
    {
        tNeighbour = tTreeCursor.GetPrevSibl();
        if(!HTREEITEM(tNeighbour))
        {
            tNeighbour = tTreeCursor.GetParent();
        }
    }

    // now delete nodes
    if(iNumIdPaths > 1)
    {
        for(int iIdPath = 0; iIdPath < iNumIdPaths; ++iIdPath)
        {
            // find node
            CXMLNode    tNodeTmp;
            m_strContextIdPath = rastrIdPath[iIdPath];
            if(tNodeRoot.GetNodeFromIdPath(m_strContextIdPath, tNodeTmp))
            {
                DoDeleteNodeAction(tNodeTmp, TRUE);            
            }
        }
    }
    else
    {
        CXMLNode    tNodeTmp;
        m_strContextIdPath = rastrIdPath[0];
        if(tNodeRoot.GetNodeFromIdPath(m_strContextIdPath, tNodeTmp))
        {
            DoDeleteNodeAction(tNodeTmp, FALSE);
        }
    }
    ASSERT(m_pProjInfo);
    if(!m_pProjInfo)
    {
        return FALSE;
    }

    m_pProjInfo->fileParseNow(NULL);

    if(HTREEITEM(tNeighbour))
    {
        SelectItem(tNeighbour);
        EnsureVisible(tNeighbour);
    }
    return TRUE;
}



//------------------------------------------------------------------*
/**
 * do delete node action.
 *
 * @param           rtNode: xml node
 * @param           bSilent: silent mode? default is no
 * @return          successful?
 * @exception       -
 * @see             
*/
BOOL CNavTreeCtrl::DoDeleteNodeAction(CXMLNode& rtNode, BOOL bSilent /*=FALSE*/)
{
    CString strType;
    
    rtNode.GetAttribute(CE_XMLATTR_TYPE, strType);
    
    // get context menu information
    CKADMenu*   pTypeMenu;
    pTypeMenu = m_pKADInfo->GetContextMenuInfoForType(strType);
    if(pTypeMenu)
    {
        int iMenuItemNo = -1;
        // get context menu item for this key event
        iMenuItemNo = pTypeMenu->m_aiKeyMouseArray[KEY_MOUSE_DELETE];
        // if key mouse event open is defined
        if(iMenuItemNo >= 0)
        {
            // perform associated context action and return
            CKADMenuItem*   pMenuItem = pTypeMenu->m_tItems.GetAt(iMenuItemNo);
            DoAddOnAction(pMenuItem, bSilent);
            return TRUE;
        }
    }
    return FALSE;
}


//------------------------------------------------------------------*
/**
 * show node.
 *
 *  used for "goto navigator" in project list view.
 *
 * @param           crstrIdPath: node id path
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CNavTreeCtrl::ShowNode(const CString& crstrIdPath)
{
    BOOL    bFound = TRUE;
    CString strId;
    CString strIdPath(crstrIdPath);
    CTreeCursor tTreeCursor;
    CTreeCursor tTreeCursorChild;

    // if path is empty -> return root cursor
    if(!StripIdFromIdPath(strIdPath, strId))
    {
        m_tCursorRoot.Expand();
        return TRUE;
    }

    // if path started with / (strId is empty) -> rtTreeCursor is root cursor
    if(strId == "")
    {
        tTreeCursor = m_tCursorRoot;
        tTreeCursor.Expand();
        // if no id left -> return root cursor
        if(!StripIdFromIdPath(strIdPath, strId))
        {
            EnsureVisible(m_tCursorRoot);
            return(TRUE);
        }
        // strip project name
        if(!StripIdFromIdPath(strIdPath, strId))
        {
            EnsureVisible(m_tCursorRoot);
            return(TRUE);
        }
    }
    else
    {
        return FALSE;   // relative paths are not allowed here
    }

    // find all ids in path
    do
    {
        bFound = FindChildFromId(tTreeCursor, tTreeCursorChild, strId);
        if(!bFound)
        {
            break;
        }
        tTreeCursor = tTreeCursorChild;
        tTreeCursor.Expand();
    } while(StripIdFromIdPath(strIdPath, strId));
    SelectItem(tTreeCursor);
    EnsureVisible(tTreeCursor);
    return bFound;
}

//------------------------------------------------------------------*
/**
 * on drop files.
 *
 * @param           hDropInfo: handle to drop info
 * @return          -
 * @exception       -
 * @see             MSDN
*/
void CNavTreeCtrl::OnDropFiles(HDROP hDropInfo) 
{
	CTreeCtrlEx::OnDropFiles(hDropInfo);
}


//------------------------------------------------------------------*
/**
 * create text file.
 *
 * @param           pPrintSettings: print settings (for future use)
 * @param           crstrFilePathName: output file path name
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CNavTreeCtrl::Print(CPrintSettings* pPrintSettings, CString& rstrFilePathName)
{
    CStdioFile  tFile;
    CString     strFileName;
    CString     strTmpPath;
    TCHAR    szPath[_MAX_PATH];
    VERIFY(GetTempPath(sizeof(szPath), szPath) != 0);
    strTmpPath = szPath;
    switch(GetViewIndex())
    {
    case 0:     // file view
        strFileName.Format("FileView.txt");
        break;
    case 1:     // class view
        strFileName.Format("ClassView.txt");
        break;
    case 2:     // hardware view
        strFileName.Format("HardwareView.txt");
        break;
    case 3:     // instance view
        strFileName.Format("InstanceView.txt");
        break;
    }
    rstrFilePathName.Format("%s%s", strTmpPath, strFileName);
    ::DeleteFile(rstrFilePathName);
    try
    {
        if(!tFile.Open(rstrFilePathName, CFile::modeCreate | CFile::modeWrite))
        {
            tFile.Close();
            return FALSE;
        }
    }
    catch(CFileException* pFileException)
    {
        pFileException->Delete();
        return FALSE;
    }

    // print to file
    CString strIndentAcc;
    PrintToFile(tFile, m_tCursorRoot, strIndentAcc);

    tFile.Close();
    return TRUE;
}

//------------------------------------------------------------------*
/**
 * print to file.
 *
 * @param           rtFile: output file
 * @param           rtTreeCursor: cursor to tree item
 * @param           rstrIndentAcc: accumulating indentation string
 * @return          -
 * @exception       -
 * @see             CreateTextFile();
*/
void CNavTreeCtrl::PrintToFile(CStdioFile& rtFile, CTreeCursor& rtTreeCursor, const CString &crstrIndentAcc)
{
    CString strText = rtTreeCursor.GetText();
    CString strLine;
    CString strIndentAcc(crstrIndentAcc);
    CString strIndentAccTmp;

    strLine.Format("%s+-%s\n", strIndentAcc, strText);

    rtFile.WriteString(strLine);

    // now call function for children
    CTreeCursor tTreeCursor;
    CTreeCursor tNextTreeCursor;

    tNextTreeCursor = rtTreeCursor.GetNextSibl();
    if(!HTREEITEM(tNextTreeCursor))
    {
        strIndentAccTmp = strIndentAcc + _T("  ");
    }
    else
    {
        strIndentAccTmp = strIndentAcc + _T("| ");
    }

    tTreeCursor = rtTreeCursor.GetChild();
    if(HTREEITEM(tTreeCursor))
    {
        do
        {
            PrintToFile(rtFile, tTreeCursor, strIndentAccTmp);
            tTreeCursor = tTreeCursor.GetNextSibl();
        } while(HTREEITEM(tTreeCursor));
    }
}


//------------------------------------------------------------------*
/**
 * is project read only?
 *
 * @param           -
 * @return          is project read only?
 * @exception       -
 * @see             -
*/
BOOL CNavTreeCtrl::IsProjectReadOnly()
{
    CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
    return pMainFrame->IsProjectReadOnly();
}

