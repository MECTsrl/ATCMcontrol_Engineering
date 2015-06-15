
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*


#include "stdafx.h"
#include "CEMAIN.h"

#include "CEMAINDoc.h"
//#include "CntrItem.h"
#include "CEMAINView.h"
#include "dragitem.h"
#include "watchwnd.h"
//#include "coolmenu.h"
//#include "mainfrm.h"
#include "utilif.h"
//#include "cehint.h"
#include "cesysdef.h"
#include "CECompMan.h"
#include "CEProjMan.h"
#include "Navigator.h"
#include "MainFrm.h"
#include "cemain.h"
#include "KADHelper/KADMenu.h"
#include "DragDropHelper\PouVarObject.h"
#include "fc_tools/fc_string.h"

#define TAKE_TIME_DEFINE
#include "TakeTime.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCEMAINView

IMPLEMENT_DYNCREATE(CCEMAINView, CListView)

BEGIN_MESSAGE_MAP(CCEMAINView, CListView)
	//{{AFX_MSG_MAP(CCEMAINView)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_NOTIFY_REFLECT(LVN_BEGINLABELEDIT, OnBeginlabeledit)
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnEndlabeledit)
	ON_WM_KEYDOWN()
    ON_NOTIFY_REFLECT(NM_RETURN, OnReturnPressed)
    ON_WM_CONTEXTMENU()
	ON_WM_DESTROY()
    ON_COMMAND(ID_PLV_DELETE, OnDelete)
	ON_WM_DROPFILES()
	ON_NOTIFY_REFLECT(LVN_BEGINDRAG, OnBegindrag)
	ON_NOTIFY_REFLECT(LVN_BEGINRDRAG, OnBeginrdrag)
	//}}AFX_MSG_MAP
    ON_COMMAND(ID_NAV_SHOW_NODE, OnShowNode)
    ON_COMMAND_RANGE(ID_NAV_CONTEXT_BASE, ID_NAV_CONTEXT_END, OnContextMenuCommand)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCEMAINView construction/destruction

CCEMAINView::CCEMAINView() :
  m_pPrjDoc(NULL),
  m_pProjEventHandler(NULL),
  m_dwProjEventHandlerCookie(0),
  m_iColumns(0),
  m_iItemCount(0),
  m_lItemId(0),
  m_iSortCol(-1),
  m_bSortInverse(FALSE)
//  m_pSelection(NULL)
{
    m_uiLibCBFormat = RegisterClipboardFormat(CB_FORMAT_4CLIB);
    m_uiCBXMLFormat = RegisterClipboardFormat(CB_FORMAT_4CXML);
    m_uiCBXMLNodeFormat = ::RegisterClipboardFormat(CB_FORMAT_4CXMLNODE);
}


CCEMAINView::~CCEMAINView()
{
}


//------------------------------------------------------------------*
/**
 * adjust column width
 *
*/
void CCEMAINView::AdjustColumnWidths()
{
	CListCtrlEx& tListCtrl = (CListCtrlEx&) GetListCtrl();

	tListCtrl.SetColumnWidth(-1, LVSCW_AUTOSIZE);
	return;
}



//------------------------------------------------------------------*
/**
 * display column headings.
 *
 *  load ids from resource and show them
 *  vertical or horizontal mode
 *
 * @param           [in] iStringId: id of string resource containing the headings
*/
void CCEMAINView::DisplayColumnHeadings(UINT iStringId)
{
	CString strHeadings;
	strHeadings.LoadString(iStringId);

	CListCtrlEx& tListCtrl = (CListCtrlEx&) GetListCtrl();

	int iPos;
	int iCount = 0;

	EraseList();

	while((iPos = strHeadings.Find(_T(","))) != -1)
    {
		CString strItem;
		strItem = strHeadings.Left(iPos);
		tListCtrl.AddColumn(strItem,iCount++);
		strItem = strHeadings.Mid(iPos + 1);
		strHeadings = strItem;
	}

    tListCtrl.AddColumn(strHeadings,iCount);
	m_iColumns = iCount;
	return;
}

//------------------------------------------------------------------*
/**
 * erase list
 *
 *  call EraseListData and delete all items
 *
 * @see             EraseListData()
*/
void CCEMAINView::EraseList()
{
	CListCtrlEx& tListCtrl = (CListCtrlEx&) GetListCtrl();
//    EraseListData();

	tListCtrl.DeleteAllItems();
	while(tListCtrl.DeleteColumn(0));

	UpdateWindow();
	return;
}

//------------------------------------------------------------------*
/**
 * erase list data.
 *
 *  run over list and delete CString objects saved in data
 *  used in EraseList()
 *
 * @see             EraseList()
*/
void CCEMAINView::EraseListData()
{
    CListCtrlEx& tListCtrl = (CListCtrlEx&)GetListCtrl();

    LVITEM  tItem;

    int iItemCount = tListCtrl.GetItemCount();

    for(int iItem = 0; iItem < iItemCount; ++iItem)
    {
        tItem.iItem = iItem;
        tItem.iSubItem = 0;
        tItem.mask = LVIF_PARAM;
        if(tListCtrl.GetItem(&tItem))
        {
            if(tItem.lParam)
            {
                CString*    strTmp = (CString*)tItem.lParam;
                delete strTmp;
            }
        }
    }
}

//------------------------------------------------------------------*
/**
 * build list.
 *
 *  - erase list
 *  - get XML-DOM documents of class and target view
 *  - fill list starting from root nodes of class and target view
 *    using AddNodeToList()
 *
 * @return          successful?
 * @see             AddNodeToList(), AddNode()
*/
BOOL CCEMAINView::BuildList()
{
//    if(!m_pKADInfo)
//    {
//        TRACE("CCEMAINView::BuildList: no KAD info object available\n");
//        return FALSE;
//    }

    // build list can take a long time -> wait cursor and status text
    CWaitCursor tCursor;

    CMainFrame* pMainFrame = (CMainFrame*)::AfxGetMainWnd();
    if(pMainFrame)
    {
        CString strStatusText;
        strStatusText.LoadString(IDS_BUILD_PLV);
        pMainFrame->UpdateStatusBar(strStatusText);
    }

    // now fill project list view
    CXMLDocument                    tClassViewDoc;
    CXMLDocument                    tTargetViewDoc;
    if(!GetXMLDocuments(tClassViewDoc, tTargetViewDoc))
    {
        return FALSE;
    }

    GetListCtrl().SetRedraw(FALSE);
    EraseList();

    // get root nodes from documents
    CXMLNode    tRootClass;
    CXMLNode    tRootTarget;


    if(tClassViewDoc.GetRootNode(tRootClass) && tTargetViewDoc.GetRootNode(tRootTarget))
    {
        TAKE_TIME_INIT;
        TAKE_TIME_START;

        // now add root nodes to list
	    CListCtrlEx& tListCtrl = (CListCtrlEx&) GetListCtrl();
    
	    DisplayColumnHeadings(IDS_COL_PRJLIST);
        AddNodeToList(tRootClass, tListCtrl);
        AddNodeToList(tRootTarget, tListCtrl);

        TAKE_TIME_END(_T("PLV AddNodes"));
    }
    else
    {
        TRACE("CCEMAINView::BuildList: got no root nodes\n");
    }

    if(pMainFrame)
    {
        CString strStatusText;
        strStatusText.LoadString(IDS_DONE);
        pMainFrame->UpdateStatusBar(strStatusText);
    }

    GetListCtrl().SetRedraw(TRUE);

	return TRUE;
}



//------------------------------------------------------------------*
/**
 * get XML documents of class and target tree from ProjMan.
 *
 * @param           [out] rtClassDoc: XML document of class tree
 * @param           [out] rtTargetDoc: XML document of target tree
 * @return          successful?
*/
BOOL CCEMAINView::GetXMLDocuments(CXMLDocument& rtClassDoc, CXMLDocument& rtTargetDoc)
{
    HRESULT hr;
    CComPtr<IUnknown>               pTmp;
    CComQIPtr<ICEProjectManager>    pProjMan;

    pTmp.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
    pProjMan = pTmp;
    if(!pProjMan)
    {
        TRACE("CCEMAINView::BuildList: ProjectManager not available\n");
        return FALSE;
    }
    pTmp.Release();

    hr = pProjMan->GetClassViewDOM(&pTmp);
    if(hr == S_OK)
    {
        rtClassDoc.SetIXMLDocument(pTmp);
    }
    else
    {
        return FALSE;
    }
    pTmp.Release();

    hr = pProjMan->GetTargetViewDOM(&pTmp);
    if(hr == S_OK)
    {
        rtTargetDoc.SetIXMLDocument(pTmp);
    }
    else
    {
        return FALSE;
    }
    return TRUE;
}

//------------------------------------------------------------------*
/**
 * delete list item
 *
 * @param           [in] iItem: number of item = line number
 * @param           [in] bMarkOnly: if TRUE this item is only marked for delete
 *                          the actual delete has to be done calling UpdateDelete()
 *                          this is useful for deleting many lines
 *
*/
void CCEMAINView::DeleteListItem(int iItem, BOOL bMarkOnly /*=FALSE*/)
{
    CListCtrl&      tListCtrl = GetListCtrl();
    long            lItemId;
    CPLVItemData    tItemData;

    lItemId = tListCtrl.GetItemData(iItem);     // get unique item id

    // now get associated item data
    if(!m_tIdMap.Lookup(lItemId, tItemData))
    {
        ASSERT(0);
    }
    m_tIdMap.RemoveKey(lItemId);                // remove id-path

    // delete id-path from tree
    if(!m_tIdPathTree.deleteData(tItemData.m_strIdPath))
    {
        ASSERT(0);
    }

    if(bMarkOnly)
    {
        tListCtrl.SetItemData(iItem, -1);
    }
    else
    {
        tListCtrl.DeleteItem(iItem);
        --m_iItemCount;
        UpdateItemNumbers();
    }
}


//------------------------------------------------------------------*
/**
 * add node to list
 *
 *  uses AddNode() to actually insert data
 *
 * @param            [in] rtNode: node to be added
 * @param            [in] rtListCtrl: list control
 * @param            [in] bRecursive: recursive call?
 * @see              AddNode()
*/
void CCEMAINView::AddNodeToList(CXMLNode& rtNode, CListCtrlEx& rtListCtrl, BOOL bRecursive /*=TRUE*/)
{

    AddNode(rtNode, rtListCtrl);
    if(bRecursive)
    {
        long            lNumChildren;       // number of children of current node
        long            lChild;             // number of current child in child list
        CXMLNodeList    tNodeList;          // list of child nodes
        CXMLNode        tNodeChildTmp;      // temporary child node
        if(rtNode.GetChildNodeList(tNodeList))
        {
            lNumChildren = tNodeList.GetLength();
            for(lChild = 0; lChild < lNumChildren; ++lChild)
            {
                // get child
                tNodeList.Item(lChild, tNodeChildTmp);
                AddNodeToList(tNodeChildTmp, rtListCtrl, bRecursive);
            }
        }
    }
}


//------------------------------------------------------------------*
/**
 * add node.
 *
 *  called by AddNodeToList()
 *
 * @param            [in] rtNode: node to be added
 * @param            [in] rtListCtrl: list control
 * @see              AddNodeToList()
*/
void CCEMAINView::AddNode(CXMLNode& rtNode, CListCtrlEx& rtListCtrl)
{
    int         iSubItem = 0;
    CString     strType;
    CString     strSource;
    CString     strIdPath;
    CString     strTmp;
    int         iImageId;
    int         iOverlayId;
    CTreeCursor tTreeCursor;
    int*        piItem;
    CCEFileHandler* pFileHandler = ((CCEMAINApp*)AfxGetApp())->GetFileHandler();

//    TAKE_TIME_INIT_HR;
//    TAKE_TIME_START_HR;

    // TYPE
    rtNode.GetAttribute(CE_XMLATTR_TYPE, strType);
    rtNode.GetAttribute(CE_XMLATTR_SOURCE, strSource, TRUE);
//    TAKE_TIME_END_HR(_T("  PLV get attribute"));
  
//    TAKE_TIME_START_HR;
    if(m_pKADInfo && m_pKADInfo->IsProjectListViewType(strType))
    {
//        TAKE_TIME_START_HR;
        rtNode.GetIdPath(strIdPath);
//        TAKE_TIME_END_HR(_T("  PLV get id path"));
        // associate id-path to item id
        piItem = (int*) (m_iItemCount + 1);   // use pointer as integer and save memory allocation
        if(!m_tIdPathTree.insertData(strIdPath, piItem))
        {
            ASSERT(FALSE);
            return;
        }

//        TAKE_TIME_END_HR(_T("  PLV insert data"));
//        TAKE_TIME_START_HR;

        // associate item id to item data
        m_tIdMap.SetAt(m_lItemId, CPLVItemData(strIdPath, strType, strSource));


        // id
        rtNode.GetAttribute(CE_XMLATTR_ID, strTmp);
        // HACK, HACK, HACK: nur um beim Projektknoten das .CLASSVIEW zu entfernen
        strTmp.Replace(_T(".CLASSVIEW"), _T(""));

        m_pKADInfo->GetImageIdsForType(strType, _T(""), iImageId, iOverlayId);
        rtListCtrl.AddItem(m_iItemCount, iSubItem++, strTmp, iImageId);

        // type text
        rtListCtrl.AddItem(m_iItemCount, iSubItem++, m_pKADInfo->GetTypeText(strType));

        // creation date
        rtNode.GetAttribute(CE_XMLATTR_CREATED, strTmp);
        rtListCtrl.AddItem(m_iItemCount, iSubItem++, strTmp);

        // modification date
        rtNode.GetAttribute(CE_XMLATTR_MODIFIED, strTmp);
        rtListCtrl.AddItem(m_iItemCount, iSubItem++, strTmp);

        // url
        rtNode.GetAttribute(CE_XMLATTR_URL, strTmp);
        rtListCtrl.AddItem(m_iItemCount, iSubItem++, strTmp);

        // user text 1
        rtNode.GetAttribute(CE_XMLATTR_USRTEXT1, strTmp);
        rtListCtrl.AddItem(m_iItemCount, iSubItem++, strTmp);

        // user text 2
        rtNode.GetAttribute(CE_XMLATTR_USRTEXT2, strTmp);
        rtListCtrl.AddItem(m_iItemCount, iSubItem++, strTmp);

        // user text 3
        rtNode.GetAttribute(CE_XMLATTR_USRTEXT3, strTmp);
        rtListCtrl.AddItem(m_iItemCount, iSubItem++, strTmp);

        // alias
        rtNode.GetAttribute(CE_XMLATTR_ALIAS, strTmp);
        rtListCtrl.AddItem(m_iItemCount, iSubItem++, strTmp);

        rtListCtrl.SetItemData(m_iItemCount, m_lItemId);

        if(!strSource.IsEmpty())
        {
            CheckSourceState(m_lItemId, strSource, pFileHandler);
        }

        ++m_iItemCount;
        ++m_lItemId;
//        TAKE_TIME_END_HR(_T("  PLV add item"));
    }
//    TAKE_TIME_END_HR(_T("  PLV add node"));
}

//------------------------------------------------------------------*
/**
 * update tree node
 *
 *  get all id paths from m_tIdPathTree that start with the given id-path (i. e. all subnodes)
 *  collect all id-paths of tree node and subnodes
 *  make diff: insert new items, remove deleted items
 *  unchanged id-paths do not mean that data has not changed: update is
 *  also needed for unchanged nodes
 *  
 * @param           [in] crstrIdPath: id-path to node
*/
void CCEMAINView::UpdateTreeNode(const CString& crstrIdPath)
{
    CXMLDocument    tClassDoc;          // XML document of class view
    CXMLDocument    tTargetDoc;         // XML document of target view
    CXMLNode        tNode;              // XML node corresponding to crstrIdPath
    CXMLNode        tNodeTmp;           // XML node used for UpdateSingleNode()
    CXMLNode        tRootNodeClass;     // XML root node of class view
    CXMLNode        tRootNodeTarget;    // XML root node of targe view

    TAKE_TIME_INIT;

    TRACE("CCEMAINView::UpdateTreeNode: %s\n", crstrIdPath);
    if(!GetXMLDocuments(tClassDoc, tTargetDoc))
    {
        TRACE("  Did not get documents: -> return\n");
        ASSERT(0);
        return;
    }

    if(!tClassDoc.GetRootNode(tRootNodeClass) || !tTargetDoc.GetRootNode(tRootNodeTarget))
    {
        TRACE("  Did not get root nodes: -> return\n");
        ASSERT(0);
        return;
    }

    CStringArray    astrIdPathsList;
    m_tIdPathTree.getAllStringsFromSubstring(crstrIdPath, astrIdPathsList);

    if(!GetXMLNodeFromIdPath(crstrIdPath, tRootNodeClass, tRootNodeTarget, tNode))
    {
        TRACE("  Did not get update node: -> return\n");
        ASSERT(0);
        return;
    }

    CSBLLexTree<bool>    tIdPathsNodeTree;

    TAKE_TIME_START;

    CollectIdPaths(tNode, tIdPathsNodeTree);

    TAKE_TIME_END(_T("CollectPaths"));

    // run over all ids of list
    int             iNumPaths = astrIdPathsList.GetSize();
    CString         strIdPath;
    CStringArray    astrNodesToDelete;
    int*            piItem;
    int             iItem;
    CListCtrlEx&    tListCtrl = (CListCtrlEx&)GetListCtrl();

    for(int iPath = 0; iPath < iNumPaths; ++iPath)
    {
        strIdPath = astrIdPathsList[iPath];
        if(!tIdPathsNodeTree.deleteData(strIdPath))
        {
            // this node does not exist any more, delete it from tree and map
            // mark it for deletion
            piItem = m_tIdPathTree.getData(strIdPath);
            iItem = (int)piItem - 1;
            ASSERT(iItem >= 0);
            if(iItem >= 0)
            {
                DeleteListItem(iItem, TRUE);
            }
        }
        else
        {
            if(GetXMLNodeFromIdPath(strIdPath, tRootNodeClass, tRootNodeTarget, tNodeTmp))
            {
                UpdateSingleNode(tNodeTmp, strIdPath);
            }
            else
            {
                ASSERT(0);
            }
        }
    }
    TAKE_TIME_END(_T("UpdateSingle"));

    // now delete all collected items via their id-paths
    UpdateDelete();

    TAKE_TIME_END(_T("UpdateDelete"));

    // all remaining node ids in tIdPathsNodeList are new
    CStringArray    astrNewNodes;
    tIdPathsNodeTree.getAllStringsFromSubstring("", astrNewNodes);
    CXMLNode    tNodeNew;

    iNumPaths = astrNewNodes.GetSize();
    for(iPath = 0; iPath < iNumPaths; ++iPath)
    {
        strIdPath = astrNewNodes[iPath];
        VERIFY(GetXMLNodeFromIdPath(strIdPath, tRootNodeClass, tRootNodeTarget, tNodeNew));
        AddNode(tNodeNew, tListCtrl);
    }
    TAKE_TIME_END(_T("UpdateInsert"));
}


//------------------------------------------------------------------*
/**
 * delete tree node.
 *
 * @param           [in] crstrIdPath: id path of node to be deleted
 * @return          -
 * @exception       -
 * @see             -
*/
void CCEMAINView::DeleteTreeNode(const CString& crstrIdPath)
{
    TRACE("CCEMAINView::DeleteTreeNode: %s\n", crstrIdPath);
    int*            piItem;
    int             iItem;
    CListCtrlEx&    tListCtrl = (CListCtrlEx&)GetListCtrl();

    TAKE_TIME_INIT;
    TAKE_TIME_START;

    piItem = m_tIdPathTree.getData(crstrIdPath);
    iItem = (int) piItem - 1;
    if(iItem >= 0)
    {
        DeleteListItem(iItem);
    }
    TAKE_TIME_END(_T("DeleteTreeNode"));
}


//------------------------------------------------------------------*
/**
 * update source state.
 *
 * @param           crstrIdPath: id path of source node
 * @return          -
 * @exception       -
 * @see             -
*/
//void CCEMAINView::UpdateSourceState(const CString& crstrIdPath)
//{
//    int*            piItem;
//    int             iItem;
//    long            lItemId;
//    CPLVItemData    tItemData;
//	CListCtrlEx&    tListCtrl = (CListCtrlEx&) GetListCtrl();
//
//    CCEFileHandler* pFileHandler = ((CCEMAINApp*)AfxGetApp())->GetFileHandler();
//
//    piItem = m_tIdPathTree.getData(crstrIdPath);
//    iItem = (int) piItem - 1;
//    if(iItem >= 0)
//    {
//        lItemId = tListCtrl.GetItemData(iItem);     // get unique item id
//
//        // now get associated item data
//        if(!m_tIdMap.Lookup(lItemId, tItemData))
//        {
//            ASSERT(0);
//        }
//        if(!tItemData.m_strSource.IsEmpty())
//        {
//            CheckSourceState(iItem, tItemData.m_strSource, pFileHandler);
//        }
//    }
//}

//------------------------------------------------------------------*
/**
 * update attributes of node.
 *
 * @param           [in] crstrIdPath: id path of node to be updated
 * @return          -
 * @exception       -
 * @see             -
*/
void CCEMAINView::UpdateTreeNodeAttributes(const CString& crstrIdPath)
{
    CXMLDocument    tClassDoc;          // XML document of class view
    CXMLDocument    tTargetDoc;         // XML document of target view
    CXMLNode        tNode;              // XML node corresponding to crstrIdPath
    CXMLNode        tRootNodeClass;     // XML root node of class view
    CXMLNode        tRootNodeTarget;    // XML root node of targe view

    TAKE_TIME_INIT;

    TRACE("CCEMAINView::UpdateTreeNodeAttributes: %s\n", crstrIdPath);
    if(!GetXMLDocuments(tClassDoc, tTargetDoc))
    {
        TRACE("  Did not get documents: -> return\n");
        ASSERT(0);
        return;
    }

    if(!tClassDoc.GetRootNode(tRootNodeClass) || !tTargetDoc.GetRootNode(tRootNodeTarget))
    {
        TRACE("  Did not get root nodes: -> return\n");
        ASSERT(0);
        return;
    }

    if(!GetXMLNodeFromIdPath(crstrIdPath, tRootNodeClass, tRootNodeTarget, tNode))
    {
        TRACE("  Did not get update node: -> return\n");
        ASSERT(0);
        return;
    }

    UpdateSingleNode(tNode, crstrIdPath);

    TAKE_TIME_END(_T("UpdateSingleNode"));
}

//------------------------------------------------------------------*
/**
 * insert tree node.
 *
 * @param           [in] crstrIdPath: id path of node to be inserted
 * @return          -
 * @exception       -
 * @see             -
*/
void CCEMAINView::InsertTreeNode(const CString& crstrIdPath)
{
    CXMLDocument    tClassDoc;          // XML document of class view
    CXMLDocument    tTargetDoc;         // XML document of target view
    CXMLNode        tNode;              // XML node corresponding to crstrIdPath
    CXMLNode        tNodeTmp;           // XML node used for UpdateSingleNode()
    CXMLNode        tRootNodeClass;     // XML root node of class view
    CXMLNode        tRootNodeTarget;    // XML root node of targe view
    CListCtrlEx&    tListCtrl = (CListCtrlEx&)GetListCtrl();

    TAKE_TIME_INIT;

    TRACE("CCEMAINView::InsertTreeNode: %s\n", crstrIdPath);
    if(!GetXMLDocuments(tClassDoc, tTargetDoc))
    {
        TRACE("  Did not get documents: -> return\n");
        ASSERT(0);
        return;
    }

    if(!tClassDoc.GetRootNode(tRootNodeClass) || !tTargetDoc.GetRootNode(tRootNodeTarget))
    {
        TRACE("  Did not get root nodes: -> return\n");
        ASSERT(0);
        return;
    }

    CStringArray    astrIdPathsList;
    m_tIdPathTree.getAllStringsFromSubstring(crstrIdPath, astrIdPathsList);

    if(!GetXMLNodeFromIdPath(crstrIdPath, tRootNodeClass, tRootNodeTarget, tNode))
    {
        TRACE("  Did not get update node: -> return\n");
        ASSERT(0);
        return;
    }

    CSBLLexTree<bool>    tIdPathsNodeTree;

    TAKE_TIME_START;

    CollectIdPaths(tNode, tIdPathsNodeTree);

    TAKE_TIME_END(_T("CollectPaths"));

    // run over all node ids in tIdPathsNodeList and insert them
    CStringArray    astrNewNodes;
    tIdPathsNodeTree.getAllStringsFromSubstring("", astrNewNodes);
    CXMLNode    tNodeNew;

    int iNumPaths = astrNewNodes.GetSize();
    CString strIdPath;
    for(int iPath = 0; iPath < iNumPaths; ++iPath)
    {
        strIdPath = astrNewNodes[iPath];
        VERIFY(GetXMLNodeFromIdPath(strIdPath, tRootNodeClass, tRootNodeTarget, tNodeNew));
        AddNode(tNodeNew, tListCtrl);
    }
    TAKE_TIME_END(_T("UpdateInsert"));
}

//------------------------------------------------------------------*
/**
 * get XML node from id-path.
 *
 * @param           [in] crstrIdPath: id path
 * @param           [in] rtRootNodeClass: root node of class view
 * @param           [in] rtRootNodeTarget: root node of target view
 * @param           [out] rtNode: rtNode
 * @return          node found?
 * @see             
*/
BOOL CCEMAINView::GetXMLNodeFromIdPath(const CString& crstrIdPath, CXMLNode& rtRootNodeClass, CXMLNode& rtRootNodeTarget, CXMLNode& rtNode)
{
    BOOL    bFound = FALSE;
    if(crstrIdPath == _T("/"))
    {
        rtNode = rtRootNodeClass;
        return TRUE;
    }

    if(rtRootNodeClass.GetNodeFromIdPath(crstrIdPath, rtNode))
    {
        bFound = TRUE;
    }
    else
    {
        if(rtRootNodeTarget.GetNodeFromIdPath(crstrIdPath, rtNode))
        {
            bFound = TRUE;
        }
    }
    return bFound;
}

 

//------------------------------------------------------------------*
/**
 * update single node
 *
 *  is called by UpdateTreeNode() to reflect changes in attributes
 *  if no id path is given, it is determined from the XML node
 *
 * @param           [in] rtNode: node to be updated
 * @param           [in] crstrIdPath: id path to node (default is empty)
 * @see             UpdateTreeNode()
*/
void CCEMAINView::UpdateSingleNode(CXMLNode& rtNode, const CString& crstrIdPath /*=""*/)
{
    int         iItem = 0;
    int*        piItem = NULL;
    int         iSubItem = 0;
    CString     strType;
    CString     strTmp;
    CXMLNode    tNode;
    CString     strIdPath = crstrIdPath;
    int         iImageId;
    int         iOverlayId;

    if(strIdPath == "")
    {
        if(!rtNode.GetIdPath(strIdPath))
        {
            return;
        }
    }

    CListCtrlEx& rtListCtrl = (CListCtrlEx&)GetListCtrl();

    // get line number of node
    piItem = m_tIdPathTree.getData(strIdPath);
    iItem = (int) piItem - 1;

    if(iItem < 0)
    {
        return;
    }

    ASSERT(iItem < m_iItemCount);

    if(iItem >= 0 && iItem < m_iItemCount)
    {
        // TYPE
        rtNode.GetAttribute(CE_XMLATTR_TYPE, strType);
        rtNode.GetIdPath(strIdPath);
  
        // id
        rtNode.GetAttribute(CE_XMLATTR_ID, strTmp);
        strTmp.Replace(_T(".CLASSVIEW"), _T(""));
        
        m_pKADInfo->GetImageIdsForType(strType, _T(""), iImageId, iOverlayId);
        rtListCtrl.SetItem(iItem, iSubItem++, strTmp, iImageId);

        // type text
        rtListCtrl.SetItem(iItem, iSubItem++, m_pKADInfo->GetTypeText(strType));

        // creation date
        rtNode.GetAttribute(CE_XMLATTR_CREATED, strTmp);
        rtListCtrl.SetItem(iItem, iSubItem++, strTmp);

        // modification date
        rtNode.GetAttribute(CE_XMLATTR_MODIFIED, strTmp);
        rtListCtrl.SetItem(iItem, iSubItem++, strTmp);

        // url
        rtNode.GetAttribute(CE_XMLATTR_URL, strTmp);
        rtListCtrl.SetItem(iItem, iSubItem++, strTmp);

        // user text 1
        rtNode.GetAttribute(CE_XMLATTR_USRTEXT1, strTmp);
        rtListCtrl.SetItem(iItem, iSubItem++, strTmp);

        // user text 2
        rtNode.GetAttribute(CE_XMLATTR_USRTEXT2, strTmp);
        rtListCtrl.SetItem(iItem, iSubItem++, strTmp);

        // user text 3
        rtNode.GetAttribute(CE_XMLATTR_USRTEXT3, strTmp);
        rtListCtrl.SetItem(iItem, iSubItem++, strTmp);

        // alias
        rtNode.GetAttribute(CE_XMLATTR_ALIAS, strTmp);
        rtListCtrl.SetItem(iItem, iSubItem++, strTmp);
    }
}

//------------------------------------------------------------------*
/**
 * update delete
 *
 *  remove all items with item data == -1
 *  this value is set by UpdateTreeNode()
 *
 * @see             UpdateTreeNode()
*/
void CCEMAINView::UpdateDelete()
{
    CListCtrl&  tListCtrl = GetListCtrl();
    int iNumItems = tListCtrl.GetItemCount();

    for(int iItem = iNumItems-1; iItem >= 0; --iItem)
    {
        if(tListCtrl.GetItemData(iItem) == -1)
        {
            tListCtrl.DeleteItem(iItem);
            --m_iItemCount;
        }
    }
    UpdateItemNumbers();
}



//------------------------------------------------------------------*
/**
 * collect id-paths from node
 *
 * @param           [in] rtNode: node where collect starts
 * @param           [out] tIdPathsNodeTree: lexical tree containing all id-paths
 * @see             UpdateTreeNode()
*/
void CCEMAINView::CollectIdPaths(CXMLNode& rtNode, CSBLLexTree<bool>& rtIdPathsNodeTree)
{
    CString strType;
    CString strIdPath;
    bool* pbTmp = (bool*) 1;

    if(rtNode.GetIdPath(strIdPath))
    {
        rtNode.GetAttribute(CE_XMLATTR_TYPE, strType);
        if(m_pKADInfo->IsProjectListViewType(strType))
        {
            rtIdPathsNodeTree.insertData(strIdPath, pbTmp);
        }
    }

    long            lNumChildren;       // number of children of current node
    long            lChild;             // number of current child in child list
    CXMLNodeList    tNodeList;          // list of child nodes
    CXMLNode        tNodeChildTmp;      // temporary child node
    if(rtNode.GetChildNodeList(tNodeList))
    {
        lNumChildren = tNodeList.GetLength();
        for(lChild = 0; lChild < lNumChildren; ++lChild)
        {
            // get child
            tNodeList.Item(lChild, tNodeChildTmp);
            CollectIdPaths(tNodeChildTmp, rtIdPathsNodeTree);
        }
    }
}

void CCEMAINView::SortColumn(int iCol)
{
    if(m_iSortCol != iCol)
    {
        m_iSortCol = iCol;
        m_bSortInverse = FALSE;
    }
    else
    {
        m_bSortInverse = TRUE;
    }
    CListCtrlEx& tListCtrl = (CListCtrlEx&)GetListCtrl();
    CSortData   tSortData(&tListCtrl, iCol, &m_tIdPathTree, &m_tIdMap);
    GetListCtrl().SortItems(CompareItems, (LPARAM)(&tSortData));
    UpdateItemNumbers();
}

//------------------------------------------------------------------*
/**
 * update item numbers.
 *
 *  update association of id-path to item number
 *  is required after sorting
 *
 * @see             SortColumn()
 *
*/
void CCEMAINView::UpdateItemNumbers()
{
    // run over all items and change item number in id-path tree
    CListCtrl&      tListCtrl = GetListCtrl();
    int             iNumItems = tListCtrl.GetItemCount();
    CPLVItemData    tItemData;
    int*            piItem;
    long            lItemId;

    for(int iItem = 0; iItem < iNumItems; ++iItem)
    {
        lItemId = tListCtrl.GetItemData(iItem);
        if(m_tIdMap.Lookup(lItemId, tItemData))
        {
            piItem = (int*) (iItem + 1);  // use pointer as integer and save memory allocation
            m_tIdPathTree.insertData(tItemData.m_strIdPath, piItem, true);
        }
        else
        {
            ASSERT(0);
        }
    }
}

int CALLBACK CCEMAINView::CompareItems(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
    // lParamSort contains a pointer to the list view control.
    // The lParam of an item is just its index.
    CSortData* pSortData = (CSortData*) lParamSort;
    CSBLLexTree<int>*   pIdPathTree = pSortData->m_pIdPathTree;
    TItemDataMap*       pIdMap = pSortData->m_pIdMap;
    int*                piItem;
    CString             strItem1;
    CString             strItem2;
    CPLVItemData        tItemData;

    if(!pIdMap->Lookup(lParam1, tItemData))
    {
        return 0;
    }
    piItem = pIdPathTree->getData(tItemData.m_strIdPath);
    ASSERT(piItem >= 0);
    strItem1 = pSortData->m_pListCtrl->GetItemText((int)piItem-1, pSortData->m_iCol);

    if(!pIdMap->Lookup(lParam2, tItemData))
    {
        return 0;
    }
    piItem = pIdPathTree->getData(tItemData.m_strIdPath);
    ASSERT(piItem >= 0);
    strItem2 = pSortData->m_pListCtrl->GetItemText((int)piItem-1, pSortData->m_iCol);
    
    strItem1.MakeLower();
    strItem2.MakeLower();

    return strcmp(strItem1, strItem2);
}



void CCEMAINView::EditLabel(int iItem)
{
    GetListCtrl().EditLabel(iItem);
    return;
}

/////////////////////////////////////////////////////////////////////////////
// CCEMAINView message handlers

void CCEMAINView::OnInitialUpdate()
{
    CFrameWnd *pParent;

    CListView::OnInitialUpdate();

    m_pPrjDoc = (CCEMAINDoc*)GetDocument();
    pParent = GetParentFrame();
    if(pParent)
    {
        pParent->SetWindowText(GetProjectTitle());
    }

	CListCtrlEx& tListCtrl = (CListCtrlEx&) GetListCtrl();

    m_pKADInfo = ((CCEMAINApp*)AfxGetApp())->GetKADInfoObject();
    
    ASSERT(m_pKADInfo);
    if(!m_pKADInfo)
    {
        return;
    }

    m_pKADInfo->CopyImageList(m_tImageList);

	tListCtrl.SetImageList(&m_tImageList, LVSIL_SMALL);

    m_tStateImageList.Create(IDB_STATEIMAGELIST, 16, 0, RGB(255, 0, 255));

    tListCtrl.SetImageList(&m_tStateImageList, LVSIL_STATE);

    AdviseProjEventHandler();
    BuildList();
}

//------------------------------------------------------------------*
/**
 * get project title = project name from project manager.
 *
 * @return          project title
*/
CString CCEMAINView::GetProjectTitle()
{
    CString                 strTitle;
    CComPtr<IUnknown>       pTmp;
    CComQIPtr<ICEProjInfo>  pProjInfo;
    CComBSTR                sTmp;
    
    pTmp.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
    pProjInfo = pTmp;
    if(!pProjInfo)
    {
        TRACE("CCEMAINView::GetProjectTitle: ProjectInfo not available\n");
        return strTitle;
    }
    pProjInfo->getProjectName(&sTmp);
    strTitle = sTmp;
    return strTitle;
}


void CCEMAINView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	CListView::OnUpdate(pSender,lHint,pHint);
	return;
}

int CCEMAINView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	lpCreateStruct->style |= LVS_REPORT | LVS_SHOWSELALWAYS | LVS_EDITLABELS;
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
#if _MFC_VER == 0x0421
    ListView_SetExtendedListViewStyle(GetListCtrl().m_hWnd, LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP | LVS_EX_FULLROWSELECT);
#else
    GetListCtrl().SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP | LVS_EX_FULLROWSELECT);
#endif
   	m_tDropTarget.Register(this);
    DragAcceptFiles();
	return 0;
}


void CCEMAINView::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    
    SortColumn(pNMListView->iSubItem);
	*pResult = 0;
}


void CCEMAINView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) {
	*pResult = 0;

	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;

    ActivateItem (phdn->iItem);
}


void CCEMAINView::ActivateItem (int iItem)
{
    CListCtrlEx& rtListCtrl = (CListCtrlEx&) GetListCtrl();

    if(iItem >= 0)
    {
        CPLVItemData    tItemData;

        if(GetItemData(iItem, tItemData))
        {
            // execute standard operation "open"
            CNavigator* pNavigator = dynamic_cast<CNavigator*>(CEGetDockingTool(CE_COMPMAN_NAV, false));
            ASSERT(pNavigator);
            if(pNavigator)
            {
                pNavigator->OpenNode(tItemData.m_strIdPath);
            }
        }
    }
	return;
}


//void CCEMAINView::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
//{
//	int iItem;
//	LONG lObjId,lClass;
//	FCEOBJECT* pFCEObject;
//	C4CObject Obj;
//	HRESULT hr;
//	
//	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
//	*pResult = 0;
//
//	if(!m_pPrjDoc) 
//		return;
//
//	if(!m_pPrjDoc->m_pObjHdl)
//		return;
//	
//	iItem = pNMListView->iItem;
//	CListCtrl& tListCtrl = GetListCtrl(); 
//
//	lObjId = tListCtrl.GetItemData(iItem);
//	hr = m_pPrjDoc->m_pObjHdl->GetObjById(lObjId,&lClass,&pFCEObject);
//
//	if(GetScode(hr) != S_OK)
//		return;
//
//    //load the object with the data of the stream pFCEObject
//    Obj.Load((LPBYTE)pFCEObject);
//    //free the stream CE_4CCG_GEN_PATH CE_4CCG_CLASS_PATH
//	Obj.FreeObj((LPBYTE)pFCEObject);
//
//    m_pPrjDoc->DragObject(m_uiLibCBFormat,m_uiCBXMLFormat,Obj);
//}

void CCEMAINView::OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
    CListCtrl& tListCtrl = GetListCtrl(); 
    LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

    *pResult = TRUE;

    // get id path of currently selected node
    int             iItem;
    CPLVItemData    tItemData;
    CString         strId;
    int             iFound;

    iItem = pDispInfo->item.iItem;
    if(!GetItemData(iItem, tItemData))
    {
        return;
    }

    iFound = tItemData.m_strIdPath.ReverseFind(_T('/'));
    if(iFound >= 0)
    {
        strId = tItemData.m_strIdPath.Mid(iFound + 1);
    }

    CNavigator* pNavigator = dynamic_cast<CNavigator*>(CEGetDockingTool(CE_COMPMAN_NAV, false));
    ASSERT(pNavigator);
    if(pNavigator)
    {
        if(pNavigator->IsKeyMouseActionAllowed(tItemData.m_strIdPath, KEY_MOUSE_RENAME))
        {
            *pResult = 0;   // allow rename
            CEdit* edit;    // set rename text (== id of node)
            edit = tListCtrl.GetEditControl();
            if (edit != NULL)
            {
                edit->SetWindowText(strId);
            }
        }
    }
    return;
}

void CCEMAINView::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult)
{
    CListCtrl& tListCtrl = GetListCtrl(); 
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

    // get id path of currently selected node
    int             iItem;
    CPLVItemData    tItemData;
    CString         strId;

    iItem = pDispInfo->item.iItem;
    if(!GetItemData(iItem, tItemData))
    {
        return;
    }

    // rename object
    CNavigator* pNavigator = dynamic_cast<CNavigator*>(CEGetDockingTool(CE_COMPMAN_NAV, false));
    ASSERT(pNavigator);
    if(pNavigator)
    {
        pNavigator->RenameNode(tItemData.m_strIdPath, pDispInfo->item.pszText);
    }
    *pResult = 0;
}

//CString CCEMAINView::GetPOUNameFromLabel(int iItem, int iSubItem)
//{
//    CString strText;
//    int iPos;
//
//    strText = GetListCtrl().GetItemText (iItem,iSubItem);
//
//    iPos = strText.Find (_T ('['));
//    if (iPos != -1)
//    {
//        strText = strText.Left (iPos);
//    };
//
//    strText.TrimLeft ();
//    strText.TrimRight ();
//
//    return (strText);
//}


//LONG CCEMAINView::OnParseProject( UINT wParam, LONG lParam)
//{
//    if(lParam != NULL) {
//        CString* pstrFileName = (CString*)lParam;
//        if(m_pPrjDoc) {
//            m_pPrjDoc->ParseProject(*pstrFileName);
//        }
//        delete pstrFileName;
//    } else {
//        if(m_pPrjDoc) {
//            m_pPrjDoc->ParseProject(NULL);
//        }
//    }    
//    return 1L;
//}

void CCEMAINView::OnDelete()
{
    DeleteSelectedItems();
}

void CCEMAINView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    switch(nChar)
    {
    case VK_DELETE:
        DeleteSelectedItems();
        break;
    case VK_INSERT:
        break;
    }
    CListView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CCEMAINView::DeleteSelectedItems()
{
    CStringArray    astrIdPath;

    if(GetSelectedNodeIdPaths(astrIdPath))
    {
        DeleteNodes(astrIdPath);
    }
}

int CCEMAINView::GetSelectedNodeIdPaths(CStringArray& rastrIdPath)
{
    int             iItem;
    CListCtrl&      tListCtrl = GetListCtrl(); 
    CPLVItemData    tItemData;
    long            lItemId;

    assert(&tListCtrl != NULL);
    POSITION pos = tListCtrl.GetFirstSelectedItemPosition();
    if (pos == NULL)
    {
       TRACE("No items selected!\n");
    }
    else
    {
        while (pos)
        {
            iItem = tListCtrl.GetNextSelectedItem(pos);
            lItemId = tListCtrl.GetItemData(iItem);     // get unique item id

            // now get associated item data
            if(m_tIdMap.Lookup(lItemId, tItemData))
            {
                rastrIdPath.Add(tItemData.m_strIdPath);                
            }
            else
            {
                ASSERT(0);
            }
        }
    }
    return(rastrIdPath.GetSize());
}

void CCEMAINView::DeleteNodes(CStringArray& rastrIdPath)
{
    // delegate this to the navigator
    CNavigator* pNavigator = dynamic_cast<CNavigator*>(CEGetDockingTool(CE_COMPMAN_NAV, false));
    ASSERT(pNavigator);
    if(pNavigator)
    {
        if(rastrIdPath.GetSize() > 1)
        {
            CString strMessage;
            strMessage.LoadString(IDS_REALLY_DELETE_SEL_FILES);
            if(::AfxMessageBox(strMessage, MB_YESNO) == IDYES)
            {
                pNavigator->DeleteNodesByIdPath(rastrIdPath);
            }
        }
        else
        {
            pNavigator->DeleteNodesByIdPath(rastrIdPath);
        }
    }
}


void CCEMAINView::OnReturnPressed (NMHDR *pNMHDR, LRESULT *pResult)
{
    CListCtrl &listControl=GetListCtrl ();
    POSITION pPosition;
    int iItem;

    pPosition = listControl.GetFirstSelectedItemPosition ();
    if (pPosition == NULL)
        return;

    iItem = listControl.GetNextSelectedItem (pPosition);

    ActivateItem (iItem);
}



int CCEMAINView::GetFirstSelectedItem()
{
    int iFound = -1;

    CListCtrl& tListCtrl = GetListCtrl(); 
    assert(&tListCtrl != NULL);

#if _MFC_VER == 0x0421
    int iMaxItems = tListCtrl.GetItemCount();
    if(iMaxItems == LB_ERR) return;

	for(iItem = iStart; iItem < iMaxItems; iItem++)
    {
	    LV_ITEM	Item;

        Item.iItem = iItem;
		Item.iSubItem = 0;
        Item.mask = LVIF_STATE; 
		if(tListCtrl.GetItemState(iItem, LVIS_SELECTED) == LVIS_SELECTED)
        {
            iFound = iItem;
            break;
        }
    }
#else
    POSITION pos = tListCtrl.GetFirstSelectedItemPosition();
    if(pos != NULL)
    {
        iFound = tListCtrl.GetNextSelectedItem(pos);
    }
#endif
    return iFound;
}

//------------------------------------------------------------------*
/**
 *  connection point to project manager is activated
 *
*/
HRESULT CCEMAINView::AdviseProjEventHandler()
{
    HRESULT hr;
    CComPtr<IUnknown>               pTmp;
    CComQIPtr<ICEProjectManager>    pProjMan;

    pTmp.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
    pProjMan = pTmp;
    if(!pProjMan)
    {
        return (E_FAIL);
    }

    ASSERT (pProjMan != NULL);
    if (pProjMan==NULL)
    {
        return (E_FAIL);
    }

    ASSERT(m_pProjEventHandler == NULL);
    hr = CComObject<CProjManListEvents>::CreateInstance (&m_pProjEventHandler);
    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        return (hr);
    }
    ASSERT (m_pProjEventHandler!= NULL);
    if (m_pProjEventHandler == NULL)
    {
        return (E_UNEXPECTED);
    }
    m_pProjEventHandler->AddRef ();

    hr = AtlAdvise (pProjMan, m_pProjEventHandler->GetUnknown (), 
                    IID__ICEProjectManagerEvents, &m_dwProjEventHandlerCookie);
    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        return (hr);
    }
    ASSERT (m_dwProjEventHandlerCookie != NULL);
    if (m_dwProjEventHandlerCookie == NULL)
    {
        m_pProjEventHandler->Release ();
        m_pProjEventHandler = NULL;
        return (E_UNEXPECTED);
    }

    m_pProjEventHandler->SetListViewPtr(this);

    return (S_OK);
}


//------------------------------------------------------------------*
/**
 *  connection point to project manager is deactivated
 *
*/
void CCEMAINView::UnadviseProjEventHandler()
{
    HRESULT hr;

    CComPtr<IUnknown>               pTmp;
    CComQIPtr<ICEProjectManager>    pProjMan;

    pTmp.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
    pProjMan = pTmp;
    if(!pProjMan)
    {
        return;
    }

    if (m_dwProjEventHandlerCookie == 0)
    {
        return;
    }

    ASSERT (pProjMan != NULL);
    if (pProjMan == NULL)
    {
        return;
    }

    hr = AtlUnadvise (pProjMan, 
                      IID__ICEProjectManagerEvents, 
                      m_dwProjEventHandlerCookie);

    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        ;
    }

    if (m_pProjEventHandler != NULL)
    {
        m_pProjEventHandler->SetListViewPtr(NULL);
        m_pProjEventHandler->Release();
        m_pProjEventHandler = NULL;
    }
}


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////// old stuff generated by the app wizard  //////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//BEGIN_MESSAGE_MAP(CCEMAINView, CView)
//	//{{AFX_MSG_MAP(CCEMAINView)
//		// NOTE - the ClassWizard will add and remove mapping macros here.
//		//    DO NOT EDIT what you see in these blocks of generated code!
//	ON_WM_DESTROY()
//	ON_WM_SETFOCUS()
//	ON_WM_SIZE()
//	ON_COMMAND(ID_OLE_INSERT_NEW, OnInsertObject)
//	ON_COMMAND(ID_CANCEL_EDIT_CNTR, OnCancelEditCntr)
//	//}}AFX_MSG_MAP
//	// Standard printing commands
//	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
//	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
//	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
//END_MESSAGE_MAP()


//BOOL CCEMAINView::PreCreateWindow(CREATESTRUCT& cs)
//{
//	// TODO: Modify the Window class or styles here by modifying
//	//  the CREATESTRUCT cs
//
//	return CView::PreCreateWindow(cs);
//}

/////////////////////////////////////////////////////////////////////////////
// CCEMAINView drawing

//void CCEMAINView::OnDraw(CDC* pDC)
//{
//	CCEMAINDoc* pDoc = GetDocument();
//	ASSERT_VALID(pDoc);
//	// TODO: add draw code for native data here
//}

//
//void CCEMAINView::OnInitialUpdate()
//{
//	CView::OnInitialUpdate();
//
//
//	// TODO: remove this code when final selection model code is written
//	m_pSelection = NULL;    // initialize selection
//
//	//Active documents should always be activated
//	COleDocument* pDoc = (COleDocument*) GetDocument();
//	if (pDoc != NULL)
//	{
//		// activate the first one
//		POSITION posItem = pDoc->GetStartPosition();
//		if (posItem != NULL)
//		{
//			CDocItem* pItem = pDoc->GetNextItem(posItem);
//
//			// only if it's an Active document
//			COleDocObjectItem *pDocObjectItem =
//				DYNAMIC_DOWNCAST(COleDocObjectItem, pItem);
//
//			if (pDocObjectItem != NULL)
//			{
//				pDocObjectItem->DoVerb(OLEIVERB_SHOW, this);
//			}
//		}
//	}
//}

/////////////////////////////////////////////////////////////////////////////
// CCEMAINView printing

//BOOL CCEMAINView::OnPreparePrinting(CPrintInfo* pInfo)
//{
//	if (!CView::DoPreparePrinting(pInfo))
//		return FALSE;
//	
//	if (!COleDocObjectItem::OnPreparePrinting(this, pInfo))
//		return FALSE;
//
//	return TRUE;
//}

//void CCEMAINView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
//{
//	// TODO: add extra initialization before printing
//}
//
//void CCEMAINView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
//{
//	// TODO: add cleanup after printing
//}

//void CCEMAINView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
//{
//	// TODO: add customized printing code here
//	if(pInfo->m_bDocObject)
//		COleDocObjectItem::OnPrint(this, pInfo, TRUE);
//	else
//		CView::OnPrint(pDC, pInfo);
//}

//void CCEMAINView::OnDestroy()
//{
//	// Deactivate the item on destruction; this is important
//	// when a splitter view is being used.
//   CView::OnDestroy();
//   COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
//   if (pActiveItem != NULL && pActiveItem->GetActiveView() == this)
//   {
//      pActiveItem->Deactivate();
//      ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
//   }
//}


/////////////////////////////////////////////////////////////////////////////
// OLE Client support and commands

//BOOL CCEMAINView::IsSelected(const CObject* pDocItem) const
//{
//	// The implementation below is adequate if your selection consists of
//	//  only CCEMAINCntrItem objects.  To handle different selection
//	//  mechanisms, the implementation here should be replaced.
//
//	// TODO: implement this function that tests for a selected OLE client item
//
//	return pDocItem == m_pSelection;
//}

//void CCEMAINView::OnInsertObject()
//{
//	// Invoke the standard Insert Object dialog box to obtain information
//	//  for new CCEMAINCntrItem object.
//	COleInsertDialog dlg;
//	if (dlg.DoModal(COleInsertDialog::DocObjectsOnly) != IDOK)
//		return;
//
//	BeginWaitCursor();
//
//	CCEMAINCntrItem* pItem = NULL;
//	TRY
//	{
//		// Create new item connected to this document.
//		CCEMAINDoc* pDoc = GetDocument();
//		ASSERT_VALID(pDoc);
//		pItem = new CCEMAINCntrItem(pDoc);
//		ASSERT_VALID(pItem);
//
//		// Initialize the item from the dialog data.
//		if (!dlg.CreateItem(pItem))
//			AfxThrowMemoryException();  // any exception will do
//		ASSERT_VALID(pItem);
//		
//		pItem->DoVerb(OLEIVERB_SHOW, this);
//
//		ASSERT_VALID(pItem);
//
//		// As an arbitrary user interface design, this sets the selection
//		//  to the last item inserted.
//
//		// TODO: reimplement selection as appropriate for your application
//
//		m_pSelection = pItem;   // set selection to last inserted item
//		pDoc->UpdateAllViews(NULL);
//	}
//	CATCH(CException, e)
//	{
//		if (pItem != NULL)
//		{
//			ASSERT_VALID(pItem);
//			pItem->Delete();
//		}
//		AfxMessageBox(IDP_FAILED_TO_CREATE);
//	}
//	END_CATCH
//
//	EndWaitCursor();
//}

// The following command handler provides the standard keyboard
//  user interface to cancel an in-place editing session.  Here,
//  the container (not the server) causes the deactivation.
//void CCEMAINView::OnCancelEditCntr()
//{
//	// Close any in-place active item on this view.
//	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
//	if (pActiveItem != NULL)
//	{
//		pActiveItem->Close();
//	}
//	ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
//}

// Special handling of OnSetFocus and OnSize are required for a container
//  when an object is being edited in-place.
//void CCEMAINView::OnSetFocus(CWnd* pOldWnd)
//{
//	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
//	if (pActiveItem != NULL &&
//		pActiveItem->GetItemState() == COleClientItem::activeUIState)
//	{
//		// need to set focus to this item if it is in the same view
//		CWnd* pWnd = pActiveItem->GetInPlaceWindow();
//		if (pWnd != NULL)
//		{
//			pWnd->SetFocus();   // don't call the base class
//			return;
//		}
//	}
//
//	CView::OnSetFocus(pOldWnd);
//}

//void CCEMAINView::OnSize(UINT nType, int cx, int cy)
//{
//	CView::OnSize(nType, cx, cy);
//	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
//	if (pActiveItem != NULL)
//		pActiveItem->SetItemRects();
//}

/////////////////////////////////////////////////////////////////////////////
// CCEMAINView diagnostics

#ifdef _DEBUG
void CCEMAINView::AssertValid() const
{
	CView::AssertValid();
}

void CCEMAINView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCEMAINDoc* CCEMAINView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCEMAINDoc)));
	return (CCEMAINDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCEMAINView message handlers

void CCEMAINView::OnDestroy() 
{
    UnadviseProjEventHandler();
	CListView::OnDestroy();
}

//------------------------------------------------------------------*
/**
 * on context menu
 *
 * @param           [in] pWnd: pointer to current window
 * @return          [in] point: mouse position
*/
void CCEMAINView::OnContextMenu(CWnd* pWnd, CPoint point)
{
    CPLVItemData    tItemData;
	CListCtrlEx&    tListCtrl = (CListCtrlEx&) GetListCtrl();
    int             iItem;
    int             iSelCount = tListCtrl.GetSelectedCount();

    // if multi selection -> only delete is allowed
    if(iSelCount > 1)
    {
        CMenu   tMenu;
        if(tMenu.CreatePopupMenu())
        {
            CString strItem;
            strItem.LoadString(IDS_PLV_MENU_ITEM_DELETE);
            if(tMenu.AppendMenu(MF_STRING, ID_PLV_DELETE, strItem))
            {
                tMenu.TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, point.x, point.y, this);
                return;
            }
        }
    }

    iItem = GetFirstSelectedItem();
    if(!GetItemData(iItem, tItemData))
    {
        return;
    }

    CNavigator* pNavigator = dynamic_cast<CNavigator*>(CEGetDockingTool(CE_COMPMAN_NAV, false));
    ASSERT(pNavigator);
    if(pNavigator)
    {
        CMenu       tMenu;
        if(tMenu.CreatePopupMenu())
        {
            m_strContextIdPath = tItemData.m_strIdPath;
            pNavigator->AddContextMenuForNode(&tMenu, tItemData.m_strIdPath);
            // add "goto navigator"
            CString strItem;
            strItem.LoadString(IDS_PLV_MENU_GOTO_NAVIGATOR);
            tMenu.AppendMenu(MF_SEPARATOR);
            tMenu.AppendMenu(MF_STRING, ID_NAV_SHOW_NODE, strItem);
            tMenu.TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, point.x, point.y, this);
        }
    }
}





void CCEMAINView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
    if (bActivate)
    {
        CMainFrame* pMainFrame = NULL;
        pMainFrame = dynamic_cast<CMainFrame* > (AfxGetMainWnd ());
        ASSERT(pMainFrame);
        if (pMainFrame != NULL)
        {
            pMainFrame->SetVarSelectionHint(_T(""));
        }
    }
    
	CListView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

DROPEFFECT CCEMAINView::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
    // bring mainframe to front
    CMainFrame* pMainFrame = (CMainFrame*)::AfxGetMainWnd();
    if(pMainFrame)
    {
        pMainFrame->SetForegroundWindow();
    }

	return CListView::OnDragEnter(pDataObject, dwKeyState, point);
}

void CCEMAINView::OnDragLeave() 
{
	CListView::OnDragLeave();
}

DROPEFFECT CCEMAINView::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
	if(pDataObject)
    {
        BOOL    bDataAvailable = FALSE;
        if(pDataObject->IsDataAvailable(m_uiCBXMLNodeFormat))
        {
            CXMLNodeDragItem    tDragItem;
            CString             strProjPath = GetProjectPath();

            tDragItem.SetCBFormatId(m_uiCBXMLNodeFormat);
            if(tDragItem.LoadFromClipboard(pDataObject))
            {
                if(strProjPath.CompareNoCase(tDragItem.m_strProjPath) != 0)
                {
                    bDataAvailable = TRUE;
                }
            }
        }
//        if(pDataObject->IsDataAvailable(m_uiCBXMLFormat))
//        {
//            bDataAvailable = TRUE;
//        }
        if(bDataAvailable)
        {
            return GetDropEffect(dwKeyState);
        }
    }
    return DROPEFFECT_NONE;
}


DROPEFFECT CCEMAINView::GetDropEffect(DWORD dwKeyState)
{
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


BOOL CCEMAINView::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point) 
{
    BOOL bReturn = FALSE;


    if(pDataObject->IsDataAvailable(m_uiCBXMLNodeFormat))
    {
        CXMLNodeDragItem    tDragItem;

        tDragItem.SetCBFormatId(m_uiCBXMLNodeFormat);
        if(tDragItem.LoadFromClipboard(pDataObject))
        {
            return DropInXMLNodeFormat(tDragItem, dropEffect);
        }
    }
	return CListView::OnDrop(pDataObject, dropEffect, point);
}


BOOL CCEMAINView::DropInXMLNodeFormat(CXMLNodeDragItem& rtDragItem, DROPEFFECT dropEffect)
{
    CString strProjPath = GetProjectPath();
    if(strProjPath.CompareNoCase(rtDragItem.m_strProjPath) == 0)
    {
        // do nothing
        ::MessageBeep(MB_ICONEXCLAMATION);
        return FALSE;
    }

    CXMLDocument    tXMLDocTmp;
    tXMLDocTmp.LoadXMLString(rtDragItem.m_strXMLString);
    CXMLNode        tNodeTmp;
    CNavigator* pNavigator = dynamic_cast<CNavigator*>(CEGetDockingTool(CE_COMPMAN_NAV, false));
    ASSERT(pNavigator);
    if(pNavigator)
    {
        if(tXMLDocTmp.GetRootNode(tNodeTmp))
        {
            TRACE("DropInXMLNodeFormat\n");
            CStringArray    astrFiles;
            CollectFiles(tNodeTmp, astrFiles);
            pNavigator->AddSourceFiles(astrFiles, rtDragItem.m_strProjPath);
        }
    }
    return FALSE;
}


//------------------------------------------------------------------*
/**
 * collect files.
 *
 *  get all source file names in subtree
 *
 * @param           [in] rtNode: start node
 * @return          -
 * @exception       -
 * @see             -
*/
void CCEMAINView::CollectFiles(CXMLNode& rtNode, CStringArray& rastrFiles)
{
    CXMLNodeList            tChildList;
    CXMLNode                tNodeChild;
    CString                 strType;
    CString                 strText;
    CString                 strSource;

    rtNode.GetAttribute(CE_XMLATTR_SOURCE, strSource);
    // if absolute path, don't add
    if(!FC_StringIsAbsPath(strSource))
    {
        if(!strSource.IsEmpty())
        {
            rastrFiles.Add(strSource);
        }
    }
    if(rtNode.GetChildNodeList(tChildList))
    {
        long lNumChildren = tChildList.GetLength();
        for(long lChild = 0; lChild < lNumChildren; ++lChild)
        {
            tChildList.Item(lChild, tNodeChild);
            CollectFiles(tNodeChild, rastrFiles);
        }
    }
}


BOOL CCEMAINView::DropInXMLFormat(CXMLDragItem& rtDragItem, DROPEFFECT dropEffect)
{
//    HGLOBAL hGlob = pDataObject->GetGlobalData(m_uiCBXMLFormat);
//
//    if(!hGlob)
//    {
//        return DROPEFFECT_NONE;
//    }
//
//    CSharedFile GlobFile;
//    GlobFile.SetHandle(hGlob, FALSE);
//
//    try 
//    {
//        CArchive ar(&GlobFile, CArchive::load);
//        CXMLDragItem DragItem;
//        DragItem.Serialize(ar);
//        return DropInXMLFormat(rtDragItem, dropEffect);
//    }
//    catch(CException* e) 
//    {
//        e->Delete();
//        return DROPEFFECT_NONE;
//    }
		
    return FALSE;
}

void CCEMAINView::OnDropFiles(HDROP hDropInfo) 
{
    CNavigator* pNavigator = dynamic_cast<CNavigator*>(CEGetDockingTool(CE_COMPMAN_NAV, false));
    ASSERT(pNavigator);
    if(pNavigator)
    {
        pNavigator->OnDropFiles(hDropInfo);
    }
}



void CCEMAINView::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    TRACE(_T("Begin Drag\n"));
    CStringArray    astrSelIdPaths;

    if(GetSelectedIdPaths(astrSelIdPaths))
    {
        if(CopyNodesToClipboard(astrSelIdPaths, TRUE))
        {
            *pResult = 0;
        }
    }
}


//------------------------------------------------------------------*
/**
 * get selected id paths.
 *
 * @param           [in] rastrSelIdPaths: selected id paths
 * @return          -
 * @exception       -
 * @see             -
*/
int CCEMAINView::GetSelectedIdPaths(CStringArray& rastrSelIdPaths)
{
    int             iItem;
    CListCtrl&      tListCtrl = GetListCtrl(); 
    CPLVItemData    tItemData;
    long            lItemId;

    assert(&tListCtrl != NULL);
    POSITION pos = tListCtrl.GetFirstSelectedItemPosition();
    while(pos)
    {
        iItem = tListCtrl.GetNextSelectedItem(pos);
        lItemId = tListCtrl.GetItemData(iItem);     // get unique item id
        // now get associated item data
        if(m_tIdMap.Lookup(lItemId, tItemData))
        {
            rastrSelIdPaths.Add(tItemData.m_strIdPath);
        }
    }
    return rastrSelIdPaths.GetSize();
}


//------------------------------------------------------------------*
/**
 * copy nodes to clipboard.
 *
 * @param           [in] rastrSelIdPaths: id paths of selected nodes
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CCEMAINView::CopyNodesToClipboard(CStringArray& rastrSelIdPaths, BOOL bDragDrop /*=FALSE*/)
{
    int             iNumNodes = rastrSelIdPaths.GetSize();

    if(iNumNodes == 0)
    {
        return FALSE;
    }

    CXMLNode        tNode;
    CString         strXML;
    CString         strType;
    CString         strIdPath;
    
    COleDataSource* pDataSource = new COleDataSource;
    if(!pDataSource)
    {
        ::AfxThrowMemoryException();
    }

    // set clipboard data for editor
    // (only first node)
    strIdPath = rastrSelIdPaths[0];
    if(GetXMLNodeFromIdPath(strIdPath, tNode))
    {
        HRESULT hr;
        hr = SetEditorClipboardData(tNode, pDataSource);
        if(hr != S_OK)
        {
            TRACE("CCEMAINView: Error in SetEditorClipboardData()\n");
            delete pDataSource;
            return FALSE;
        }
    }

    // set xml clipboard data
    // generate temporary xml with just tmp nodes
    CXMLDocument    tXMLDoc;
    CXMLNode        tNodeDoc;
    CXMLNode        tNodeRootTmp;
    CXMLNode        tNodeTmp;
    CString         strSource;

    if(!tXMLDoc.GetDocNode(tNodeDoc))
    {
        return FALSE;
    }
    if(!tXMLDoc.CreateNode(tNodeRootTmp, CE_XMLTAG_TMPNODE))
    {
        return FALSE;
    }
    if(!tNodeDoc.AppendChild(tNodeRootTmp))
    {
        return FALSE;
    }

    for(int iNode = 0; iNode < iNumNodes; ++iNode)
    {
        strIdPath = rastrSelIdPaths[iNode];
        if(GetXMLNodeFromIdPath(strIdPath, tNode))
        {
            tNode.GetAttribute(CE_XMLATTR_TYPE, strType);
            if(IsDragAllowed(strType))
            {
                if(tXMLDoc.CreateNode(tNodeTmp, CE_XMLTAG_TMPNODE))
                {
                    tNode.GetAttribute(CE_XMLATTR_SOURCE, strSource);
                    tNodeTmp.SetAttribute(CE_XMLATTR_SOURCE, strSource);
                    tNodeRootTmp.AppendChild(tNodeTmp);
                }
            }
        }
    }

    CString             strXMLString;
    tNodeRootTmp.GetXMLString(strXMLString);

    if(!strXMLString.IsEmpty())
    {
        CXMLNodeDragItem    tDragItem;
        CString strProjectPath = GetProjectPath();
        tDragItem.SetData(strSource, strSource, CE_XMLTAG_TMPNODE, strSource, strProjectPath, strXMLString);
        tDragItem.SetCBFormatId(m_uiCBXMLNodeFormat);

        if(tDragItem.CopyToClipboard(pDataSource))
        {
            if(bDragDrop)
            {
                pDataSource->DoDragDrop();
        		pDataSource->InternalRelease();
            }
            else
            {
                pDataSource->SetClipboard();
            }
        }
        else
        {
            ::MessageBeep(MB_ICONEXCLAMATION);
            delete pDataSource;
            return FALSE;
        }
    }
    else
    {
        ::MessageBeep(MB_ICONEXCLAMATION);
        return FALSE;
    }
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * get xml node from id path.
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
BOOL CCEMAINView::GetXMLNodeFromIdPath(const CString& crstrIdPath, CXMLNode& rtNode)
{
    // get project manager to get node pointers
    HRESULT hr;
    CComPtr<IUnknown>       pTmp;
    CComQIPtr<ICEProjInfo>  pProjMan;
    IUnknown*               pXMLNode;

    pTmp.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
    pProjMan = pTmp;
    if(!pProjMan)
    {
        return FALSE;
    }
    CComBSTR    sIdPath = crstrIdPath;
    hr = pProjMan->getXMLNodePtr(sIdPath, &pXMLNode);
    if(hr != S_OK)
    {
        return FALSE;
    }
    if(!rtNode.SetIXMLNode(pXMLNode))
    {
        return FALSE;
    }
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * get project path.
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
CString CCEMAINView::GetProjectPath()
{
    HRESULT hr;
    CComPtr<IUnknown>               pTmp;
    CComQIPtr<ICEProjInfo>          pProjMan;
    CComBSTR                        sProjPath;
    CString                         strProjPath;

    pTmp.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
    pProjMan = pTmp;
    if(pProjMan)
    {
        hr = pProjMan->getProjectPath(&sProjPath);
        if(hr == S_OK)
        {
            strProjPath = sProjPath;
        }
    }
    return strProjPath;
}


//------------------------------------------------------------------*
/**
 * set clipboard data for editor.
 *
 * @param           [in] rtNode: xml node
 * @param           [in] pDataSource: pointer to data source object
 * @return          S_OK:       data was copied successfully
 *                  S_FALSE:    no data was copied (not used)
 *                  E_FAIL:     error occurred
*/
HRESULT CCEMAINView::SetEditorClipboardData(CXMLNode& rtNode, COleDataSource* pDataSource)
{
    CString     strXML;
    int         itemType;
    CString     strPath;
    CPouObject  tPouObj;

    tPouObj.InitFromXMLNode(&rtNode);
    strPath = tPouObj.m_strName;
    itemType = tPouObj.GetDBObjectType();
    strXML = tPouObj.GetDragAndDropString();
    try
    {
		//construct the drag item
        CComBSTR sXML = strXML;
		CXMLDragItem Item(strPath, itemType, sXML);

		CSharedFile GlobFile;

		CArchive ar(&GlobFile, CArchive::store);
		Item.Serialize(ar);
		ar.Close();
		pDataSource->CacheGlobalData(m_uiCBXMLFormat, GlobFile.Detach());
    }
	catch(HRESULT hrRes)
	{
		//hr exception
		//TODO status output
		HRESULT hr = hrRes;
        return E_FAIL;
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
    return S_OK;
}


// old code begin drag for drop files format
// crashes
// 
//void CCEMAINView::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
//{
//	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
//
//    CPLVItemData    tData;
//
//	*pResult = 0;
//    // first get project manager
//    HRESULT hr;
//    CComPtr<IUnknown>               pTmp;
//    CComQIPtr<ICEProjInfo>          pProjMan;
//    CComBSTR                        sProjPath;
//
//    pTmp.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
//    pProjMan = pTmp;
//    if(!pProjMan)
//    {
//        return;
//    }
//
//    hr = pProjMan->getProjectPath(&sProjPath);
//    if(hr != S_OK)
//    {
//        return;
//    }
//
//    CString         strBuffer;
//    CString         strSource;
//    CString         strFileName;
//    CString         strProjectPath = sProjPath;
//    int             iItem;
//    CListCtrl&      tListCtrl = GetListCtrl(); 
//    CPLVItemData    tItemData;
//    long            lItemId;
//    IUnknown*       pXMLNode;
//    CXMLNode        tNode;
//
//    assert(&tListCtrl != NULL);
//    POSITION pos = tListCtrl.GetFirstSelectedItemPosition();
//    if (pos == NULL)
//    {
//       TRACE("No items selected!\n");
//    }
//    else
//    {
//        while (pos)
//        {
//            iItem = tListCtrl.GetNextSelectedItem(pos);
//            lItemId = tListCtrl.GetItemData(iItem);     // get unique item id
//
//            // now get associated item data
//            if(m_tIdMap.Lookup(lItemId, tItemData))
//            {
//                CComBSTR    sIdPath = tItemData.m_strIdPath;
//                hr = pProjMan->getXMLNodePtr(sIdPath, &pXMLNode);
//                if(hr == S_OK)
//                {
//                    tNode.SetIXMLNode(pXMLNode);
//                    pXMLNode->Release();
//                    tNode.GetAttribute(CE_XMLATTR_SOURCE, strSource);
//                    strFileName = strProjectPath + _T("\\") + strSource + _T(";");
//                    strBuffer += strFileName;
//                }
//            }
//        }
//    }
//    int iSize = strBuffer.GetLength() + 1;
//    strBuffer.Replace(_T(';'), _T('\0'));
//    //int iStrlen = strBuffer.GetLength();
//
//    COleDataSource* pDataSource = new COleDataSource;
//
//    HGLOBAL hGlobal = ::GlobalAlloc(0, iSize);
//
//    if(!hGlobal)
//    {
//        return;
//    }
//
//    LPVOID lpBuf = ::GlobalLock(hGlobal);
//	ASSERT(lpBuf != NULL);
//
//    ::memcpy(lpBuf, strBuffer.GetBuffer(iSize), iSize);
//	::GlobalUnlock(hGlobal);
//
//
//    pDataSource->CacheGlobalData(CF_HDROP, hGlobal);
//    pDataSource->DoDragDrop();
//}


void CCEMAINView::OnBeginrdrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
    OnBegindrag(pNMHDR, pResult);
}

//------------------------------------------------------------------*
/**
 * get item data.
 *
 * @param           [in] iItem: number of item in list
 * @param           [out] rtItemData: associated item data
 * @return          item data found?
 * @exception       -
 * @see             
*/
BOOL CCEMAINView::GetItemData(int iItem, CPLVItemData& rtItemData)
{
    CListCtrlEx& tListCtrl = (CListCtrlEx&)GetListCtrl();
    long            lItemId;

    if(iItem < 0)
    {
        return FALSE;
    }

    lItemId = tListCtrl.GetItemData(iItem);     // get unique item id

    // now get associated item data
    if(!m_tIdMap.Lookup(lItemId, rtItemData))
    {
        return FALSE;
    }
    return TRUE;
}

BOOL CCEMAINView::IsDragAllowed(const CString& crstrType)
{
    // get kad info object:
    CCEMAINApp* pApp = (CCEMAINApp*) AfxGetApp();
    CKADInfo*   pKADInfo = pApp->GetKADInfoObject();
    if(!pKADInfo)
    {
        return FALSE;
    }

    // drag is allowed if type is drag type
    CKADDragDropMap*    pKADDragDropMap = pKADInfo->GetDragDropMap();

    if(!pKADDragDropMap)
    {
        return FALSE;
    }

    if(pKADDragDropMap->IsDragType(crstrType))
    {
        return TRUE;
    }
    return FALSE;
}


void CCEMAINView::OnContextMenuCommand(UINT nID)
{
    long    lItem = nID - ID_NAV_CONTEXT_BASE;
    ASSERT(lItem >= 0);

    // get selected item
    CPLVItemData    tItemData;
    int iItem = GetFirstSelectedItem();
    if(!GetItemData(iItem, tItemData))
    {
        return;
    }

    // check for rename
    if(m_pKADInfo)
    {
        CKADMenu*   pTypeMenu = m_pKADInfo->GetContextMenuInfoForType(tItemData.m_strType);
        if(pTypeMenu)
        {
            int iMenuItemNo = pTypeMenu->m_aiKeyMouseArray[KEY_MOUSE_RENAME];
            if(iMenuItemNo == lItem)
            {
                // perform rename
                EditLabel(iItem);
                return;
            }
        }
    }

    // delegate the rest to navigator
    CNavigator* pNavigator = dynamic_cast<CNavigator*>(CEGetDockingTool(CE_COMPMAN_NAV, false));
    if(pNavigator)
    {
        pNavigator->DoContextMenuCommandForNode(tItemData.m_strIdPath, nID);
    }
}


//------------------------------------------------------------------*
/**
 * on idle
 *
 * @param           -
 * @return          another call needed.
 * @exception       -
 * @see             -
*/
BOOL CCEMAINView::OnIdle(LONG lCount)
{
    // allow on idle only all 3 seconds
    static  CTime statLastTime = CTime::GetCurrentTime();
    CTime   tTimeTmp = CTime::GetCurrentTime();
    CTimeSpan   tTimeSpan(0,0,0,3);
    if((tTimeTmp - tTimeSpan) < statLastTime)
    {
        return FALSE;
    }

    statLastTime = CTime::GetCurrentTime();

	CListCtrlEx&      tListCtrl = (CListCtrlEx&) GetListCtrl();

    int iItemCount = GetListCtrl().GetItemCount();
    long            lItemId;
    CPLVItemData    tItemData;
    CCEFileHandler* pFileHandler = ((CCEMAINApp*)AfxGetApp())->GetFileHandler();
    if(!pFileHandler)
    {
        return FALSE;
    }
    int iStart = tListCtrl.GetTopIndex();
    int iStop = iStart + tListCtrl.GetCountPerPage();

    if(iStop > iItemCount)
    {
        iStop = iItemCount;
    }

    for(int iItem = iStart; iItem < iStop; ++iItem)
    {

        lItemId = tListCtrl.GetItemData(iItem);     // get unique item id

        // now get associated item data
        if(!m_tIdMap.Lookup(lItemId, tItemData))
        {
            ASSERT(0);
        }
        if(!tItemData.m_strSource.IsEmpty())
        {
            CheckSourceState(iItem, tItemData.m_strSource, pFileHandler);
        }
    }
    return FALSE;
}


//------------------------------------------------------------------*
/**
 * check source state.
 *
 * @param           iItem: item number
 * @param           crstrSource: source file without path
 * @param           pFileHandler: pointer to file handler (or NULL)
 * @return          -
 * @exception       -
 * @see             -
*/
void CCEMAINView::CheckSourceState(int iItem, const CString& crstrSource, CCEFileHandler* pFileHandler /*=NULL*/)
{
    CFileStatus tFileStatus;
    CListCtrl&  rtListCtrl = GetListCtrl();
    
    if(pFileHandler == NULL)
    {
        pFileHandler = ((CCEMAINApp*)AfxGetApp())->GetFileHandler();
        if(!pFileHandler)
        {
            return;
        }
    }

    EFileStatus eStatus = pFileHandler->GetFileStatus(crstrSource);
    rtListCtrl.SetItemState(iItem, INDEXTOSTATEIMAGEMASK(eStatus+1), TVIS_STATEIMAGEMASK);
}


//------------------------------------------------------------------*
/**
 * on show node.
 *
 *  "goto navigator"
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CCEMAINView::OnShowNode()
{
    CNavigator* pNavigator = dynamic_cast<CNavigator*>(CEGetDockingTool(CE_COMPMAN_NAV, false));
    ASSERT(pNavigator);
    if(pNavigator)
    {
        pNavigator->ShowNode(m_strContextIdPath);
    }
}