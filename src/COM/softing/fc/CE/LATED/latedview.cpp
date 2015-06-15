//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "LATED.h"

#include "lateddoc.h"
#include "latedview.h"
#include "lcconfig.h"
#include "stdedattr.h"
#include "lcdatatype.h"
#include "lclink.h"
#include "ipframe.h"
#include "resource.h"
#include "toolbox.h"
//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLATEDView

IMPLEMENT_DYNCREATE(CLATEDView, CTreeView)

BEGIN_MESSAGE_MAP(CLATEDView, CTreeView)
	//{{AFX_MSG_MAP(CLATEDView)
	ON_WM_CREATE()
	ON_WM_MOUSEACTIVATE()
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRClick)
	ON_COMMAND(IDM_ADD_DATA_TYPE, OnAddDataType)
	ON_COMMAND(IDM_DELETE, OnDelete)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, OnEndlabeledit)
	ON_NOTIFY_REFLECT(TVN_BEGINLABELEDIT, OnBeginlabeledit)
	ON_COMMAND(IDM_RENAME, OnRename)
	ON_COMMAND(IDM_PROP, OnProperty)
	ON_COMMAND(IDM_LINKGROUP_ADDLINK, OnAddlink)
	ON_UPDATE_COMMAND_UI(IDM_DELETE, OnUpdateDelete)
	ON_COMMAND(IDM_INS_DEST_LINK, OnAddDestLink)
	ON_COMMAND(IDM_ASSIGN_DATATYPE, OnAssignDatatype)
	ON_UPDATE_COMMAND_UI(IDM_PROP, OnUpdateProp)
	ON_UPDATE_COMMAND_UI(IDM_ASSIGN_DATATYPE, OnUpdateAssignDatatype)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_NOTIFY_REFLECT(TVN_GETDISPINFO, OnGetDispInfo)
    ON_NOTIFY_REFLECT(TVN_GETINFOTIP, OnToolTipNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLATEDView construction/destruction

CLATEDView::CLATEDView(): CTreeView() , m_ctlImage() ,
    m_strLinkGroupLabel(), m_strDataGroupLabel() ,
    m_strLinkGroupTT(), m_strDataGroupTT()
{
	//{{AFX_DATA_INIT(CLATEDView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Add construction code here
    m_hDataGroup = NULL;
    m_hLatAccel = ::LoadAccelerators(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_LATACCEL));
}

CLATEDView::~CLATEDView()
{
}

void CLATEDView::GetPosition (CStdEditorPosition & pos)
{
    long lX,lY;
    
    lX = 0;
    lY = 0; //can be the control within the page that has focus.
    pos = CStdEditorPosition(EDP_STLINE,lX,lY);
}

void CLATEDView::GotoPosition(CStdEditorPosition & pos)
{
    long lX,lY;

    lX = pos.GetX();
    lY = pos.GetY(); //position to the control within the page to activate
}


void CLATEDView::SetModified()
{
    CLATEDDoc * pDoc = NULL;

    pDoc = GetDocument();
    if(!pDoc) {
        return;
    }

    pDoc->SetModifiedFlag(TRUE);
}

BOOL CLATEDView::PreCreateWindow(CREATESTRUCT& cs)
{
	// Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return CTreeView::PreCreateWindow(cs);
}

void CLATEDView::OnInitialUpdate()
{
    m_strLinkGroupLabel.LoadString(IDS_LATED_LINKGROUP_LABEL);
    m_strDataGroupLabel.LoadString(IDS_LATED_DATAGROUP_LABEL);
    m_strLinkGroupTT.LoadString(IDS_LATED_LINKGROUP_TT);
    m_strDataGroupTT.LoadString(IDS_LATED_DATAGROUP_TT);
    EnableToolTips(TRUE);   // enable tool tips for view
}

void CLATEDView::OnDataType(int id, CLCDataType *pDataType)
{
    ASSERT(pDataType);

    switch(id) {
        case AddDataType:
        case TraverseDatatypes:
        {
            DWORD dwData = (DWORD) pDataType;
            HTREEITEM hItem = InsertItem(dwData,true,m_hDataGroup);
            ASSERT(hItem);
            pDataType->SetTreeItem(hItem);
            size_t size = pDataType->GetDataLen();

            if(id == AddDataType && hItem) {
                Expand(hItem);
            }

        }
        break;
        case DelDataType:
        {
            HTREEITEM hItem = pDataType->GetTreeItem();
            if(!hItem) {
                break;
            }
            
            CTreeCtrl& rTree = GetTreeCtrl();
            rTree.DeleteItem(hItem);

            pDataType->SetTreeItem(NULL);
        }
        break;
        default:
        break;
    }
}

void CLATEDView::OnMember(int id, CLCDataType *pDataType, CLCMember *pMember)
{
    //father is the datatype
    ASSERT(pDataType);
    ASSERT(pMember);

    switch(id) {
        case AddDataType:
        case TraverseDatatypes:
        case TraverseMembers:
        {
            HTREEITEM hData = pDataType->GetTreeItem();
            ASSERT(hData);
            if(!hData) {
                return;
            }

            DWORD dwData = (DWORD) pMember;
            HTREEITEM hItem = InsertItem(dwData,false,hData);

            ASSERT(hItem);
            pMember->SetTreeItem(hItem);
        }
        break;

        case RemoveMembers:
        case DelDataType:
        {
            HTREEITEM hItem = pMember->GetTreeItem();
            if(!hItem) {
                break;
            }
            
            CTreeCtrl& rTree = GetTreeCtrl();
            rTree.DeleteItem(hItem);
            pMember->SetTreeItem(NULL);
        }
        break;
        default:
        break;
    }
}

void CLATEDView::OnLink(int id, CLCLink *pSrc)
{
    ASSERT(pSrc);

    switch(id) {
        case Add_Link:
        case TraverseLinks:
        {
            HTREEITEM hRoot,hSrc;
            DWORD dwData;

            CTreeCtrl& rTree = GetTreeCtrl();
            hRoot = rTree.GetRootItem();
            ASSERT(hRoot);
            if(!hRoot) {
                return;
            }

            //inserts sourc link
            dwData = (DWORD) pSrc;
            hSrc = InsertItem(dwData,true,hRoot);
            ASSERT(hSrc);
            pSrc->SetTreeItem(hSrc);

            if(id == Add_Link && hSrc) {
                Expand(hSrc);
            }
        }
        break;
        case Del_Link:
        {
            HTREEITEM hItem = pSrc->GetTreeItem();
            if(!hItem) {
                break;
            }
            
            CTreeCtrl& rTree = GetTreeCtrl();
            rTree.DeleteItem(hItem);
            pSrc->SetTreeItem(NULL);
        }
        break;
        default:
        break;
    }
}

void CLATEDView::OnDestLink(int id, CLCLink *pLink, CLCDestLink *pDest)
{
    ASSERT(pLink);
    ASSERT(pDest);

    switch(id) {
        case Add_Link:
        case TraverseLinks:
        {
            HTREEITEM hParent,hItem;
            DWORD dwData;

            hParent = pLink->GetTreeItem();
            ASSERT(hParent);

            //inserts sourc link
            dwData = (DWORD) pDest;
            hItem = InsertItem(dwData,false,hParent);
            ASSERT(hItem);
            pDest->SetTreeItem(hItem);

            if(id == Add_Link && hItem) {
                Expand(hItem);
            }

        }
        break;
        case Del_Link:
        {
            HTREEITEM hItem = pDest->GetTreeItem();
            if(!hItem) {
                break;
            }
            
            CTreeCtrl& rTree = GetTreeCtrl();
            rTree.DeleteItem(hItem);
            pDest->SetTreeItem(NULL);
        }
        break;
        default:
        break;
    }
}

/////////////////////////////////////////////////////////////////////////////
// CLATEDView printing

BOOL CLATEDView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLATEDView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLATEDView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CLATEDView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CLATEDView diagnostics

#ifdef _DEBUG
void CLATEDView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CLATEDView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CLATEDDoc* CLATEDView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLATEDDoc)));
	return (CLATEDDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLATEDView message handlers

int CLATEDView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    lpCreateStruct->style |=  TVS_HASLINES  | TVS_INFOTIP | TVS_LINESATROOT
                          | TVS_HASBUTTONS | TVS_EDITLABELS  | TVS_SHOWSELALWAYS;

	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
    // Create the Image List
    VERIFY(m_ctlImage.Create(IDB_NAMESPACE_TREE, 20 , 0, RGB(255,0,255)));
    m_ctlImage.SetBkColor(GetSysColor(COLOR_WINDOW));

   	/// Attach image list to Tree
    CTreeCtrl& rTree = GetTreeCtrl();
    rTree.SetImageList(&m_ctlImage, TVSIL_NORMAL);
   
	return 0;
}

void CLATEDView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
    if (lHint == CStdEditorDoc::GetPositionHint) {
        GetPosition (*(CStdEditorPosition *)pHint);
        return;
    }

    if (lHint == CStdEditorDoc::GotoPositionHint) {
        GotoPosition (*(CStdEditorPosition *)pHint);
        return;
    }

    LATED_HINT_TYPE type = (LATED_HINT_TYPE) lHint;

    switch(type) {
    case UPDATE_ALL:
        OnUpdateAll(pHint);
        return;
    case UPDATE_NEW:
        OnUpdateNew();
        return;
    case UPDATE_CLEAR:
        {
            CTreeCtrl& rTree = GetTreeCtrl();
            rTree.SelectItem(NULL);
            rTree.DeleteAllItems();
        }
        return;
    case INVALID:
        break;
    }
    CTreeView::OnUpdate(pSender,lHint,pHint);
}

void CLATEDView::OnDraw(CDC* pDC)
{}

void CLATEDView::DrawInto (CDC *pDC, CSize drawAreaSize)
{
    CRect drawArea;
    
    drawArea.left = 0;
    drawArea.top = 0;
    drawArea.right = drawAreaSize.cx;
    drawArea.bottom = drawAreaSize.cy;

    OnDraw(pDC);
}

void CLATEDView::Expand(HTREEITEM hParent,bool bSelf)
{
    HTREEITEM hItem = NULL;
    CTreeCtrl& rTree = GetTreeCtrl();

    if(hParent == NULL) {
        hParent = rTree.GetRootItem();
    }

    // Show all of the children of hParent expanded.
    if (rTree.ItemHasChildren(hParent))
    {
       hItem = rTree.GetChildItem(hParent);

       while (hItem != NULL)
       {
          rTree.Expand(hItem,TVE_EXPAND);
          hItem = rTree.GetNextSiblingItem(hItem);
       }
    }

    if(bSelf) {
        rTree.Expand(hParent,TVE_EXPAND);
    }

}

void CLATEDView::OnUpdateAll(CObject* pObject)
{
    CTreeCtrl& rTree = GetTreeCtrl();
    rTree.SelectItem(NULL);
    rTree.DeleteAllItems();

    //Root is the link group node - hence it can be retrieved via GetRootItem()
    //so it's not necessary to store the root tree item here.
    HTREEITEM hRoot = InsertItem(1L);
    //store data groups tree item in m_hData
    m_hDataGroup = InsertItem(2L,true,TVI_ROOT,hRoot);

    if(!pObject) {
        return;
    }

    CLCConfig * pConfig = dynamic_cast<CLCConfig*>(pObject);
    if(!pConfig) {
        return;
    }

    pConfig->AddConfigListener(this);
    pConfig->TraverseDatatypes(TraverseDatatypes);
    pConfig->TraverseLinks(TraverseLinks);

    Expand(hRoot);
    Expand(m_hDataGroup);
}

void CLATEDView::OnUpdateNew()
{
    CTreeCtrl& rTree = GetTreeCtrl();
    rTree.SelectItem(NULL);
    rTree.DeleteAllItems();

    //Root is the link group node - hence it can be retrieved via GetRootItem()
    //so it's not necessary to store the root tree item here.
    HTREEITEM hRoot = InsertItem(1L);
    //store data groups tree item in m_hData
    m_hDataGroup = InsertItem(2L,true,TVI_ROOT,hRoot);

    CLCConfig * pConfig = GetConfig();
    if(!pConfig) {
        return;
    }

    pConfig->AddConfigListener(this);
}

CLCBase* CLATEDView::GetNode(HTREEITEM hItem)
{
    CLCBase* pBase = NULL;
    DWORD dwData;
    bool bValid = false;

    CTreeCtrl&  rTree = GetTreeCtrl();
    dwData = rTree.GetItemData(hItem);

    switch(dwData) {
        case 1UL:
            break;
        case 2UL:
            break;
        case 0UL:
            break;
        default:
            pBase = reinterpret_cast<CLCBase*>(dwData);
            bValid = CLCConfig::IsObjectValid(pBase);
            ASSERT(bValid);
            break;
    }
    
    return (bValid == true) ? pBase : NULL;
}

CLCConfig* CLATEDView::GetConfig()
{
    CLATEDDoc * pDoc = NULL;
    CLCConfig* pConfig = NULL;

    pDoc = GetDocument();
    if(!pDoc) {
        return NULL;
    }

    pConfig = pDoc->GetConfig();
    return pConfig;
}


HTREEITEM CLATEDView::InsertItem(DWORD dwData, bool bChildren,
                                 HTREEITEM hParentItem /*= TVI_ROOT*/,
		                         HTREEITEM hInsertAfter /*= TVI_SORT*/)
{
    CTreeCtrl& rTree = GetTreeCtrl();
    HTREEITEM   hItem;

    TV_INSERTSTRUCT tvi;
    tvi.hParent      = hParentItem;
    tvi.hInsertAfter = hInsertAfter;
    ::memset(&tvi.item, 0, sizeof(tvi.item));
    tvi.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM | TVIF_CHILDREN;
    tvi.item.pszText = LPSTR_TEXTCALLBACK;
    tvi.item.iImage  = I_IMAGECALLBACK;
    tvi.item.iSelectedImage = I_IMAGECALLBACK;
    tvi.item.lParam  = (DWORD) dwData;
    tvi.item.cChildren = (bChildren == true) ? 1 : 0;
    hItem = rTree.InsertItem(&tvi);
    ASSERT(hItem != NULL);
    return hItem;
}

void CLATEDView::InvalidateItem(HTREEITEM hItem)
{
    CRect rRect;
    
    if(hItem == NULL) {
        return;
    }

    CTreeCtrl&  rTree = GetTreeCtrl();
    rTree.GetItemRect(hItem,rRect,FALSE);
    rTree.InvalidateRect(rRect);
}

void CLATEDView::OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pDisp = (TV_DISPINFO*)pNMHDR;
    CString str;
    CLCBase * pBase = NULL;
    DWORD dwData;
    
    CTreeCtrl&  rTree = GetTreeCtrl();
    dwData = rTree.GetItemData(pDisp->item.hItem);

    *pResult = 0;

    switch(dwData) {
        case 1UL:
            if (pDisp->item.mask & TVIF_TEXT) {
                _tcsncpy(pDisp->item.pszText, m_strLinkGroupLabel,pDisp->item.cchTextMax);
            }
            if (pDisp->item.mask & TVIF_IMAGE)
            {
                pDisp->item.iImage = LATED_IMAGEID_GROUP;
            }
            // query for selected image:
            if (pDisp->item.mask & TVIF_SELECTEDIMAGE)
            {
                pDisp->item.iSelectedImage = LATED_IMAGEID_GROUP;
            }
            break;
        case 2UL:
            if (pDisp->item.mask & TVIF_TEXT) {
                _tcsncpy(pDisp->item.pszText, m_strDataGroupLabel,pDisp->item.cchTextMax);
            }
            if (pDisp->item.mask & TVIF_IMAGE)
            {
                pDisp->item.iImage = LATED_IMAGEID_DATAGROUP;
            }
            // query for selected image:
            if (pDisp->item.mask & TVIF_SELECTEDIMAGE)
            {
                pDisp->item.iSelectedImage = LATED_IMAGEID_DATAGROUP;
            }
            break;
        case 0UL:
            return;
        default:
            pBase = GetNode(pDisp->item.hItem);
            if(!pBase) {
                break;
            }

            if (pDisp->item.mask & TVIF_TEXT) {
                pBase->GetName(str);
                _tcsncpy(pDisp->item.pszText,str,pDisp->item.cchTextMax);
            }
            if (pDisp->item.mask & TVIF_IMAGE)
            {
                pDisp->item.iImage = pBase->GetImageId();
            }
            // query for selected image:
            if (pDisp->item.mask & TVIF_SELECTEDIMAGE)
            {
                pDisp->item.iSelectedImage = pBase->GetImageId();
            }
            break;
    }

}


void CLATEDView::OnToolTipNotify(NMHDR* pNMHDR, LRESULT* pResult) 
{
    NMTVGETINFOTIP* pInfo = (NMTVGETINFOTIP*) pNMHDR;

    if(!pInfo || !pResult) {
        return;
    }

    *pResult = 0; 

    CTreeCtrl&  rTree = GetTreeCtrl();
    DWORD dwData = rTree.GetItemData(pInfo->hItem);

    switch(dwData) {
        case 1UL:
            _tcsncpy(pInfo->pszText,m_strLinkGroupTT,pInfo->cchTextMax);
            break;
        case 2UL:
            _tcsncpy(pInfo->pszText,m_strDataGroupTT,pInfo->cchTextMax);
            break;
        case 0UL:
            return;
        default:
            {
                CLCBase* pBase = GetNode(pInfo->hItem);
                if(!pBase) {
                    break;
                }

                CString str;
                if(pBase->GetToolTip(str)) {
                    _tcsncpy(pInfo->pszText,str,pInfo->cchTextMax);
                }
            }
            break;
    }

    //Issue with MSDN KB entry Q247382:
    //PRB: ToolTips for Controls in ActiveX Document Server Are Hidden 
    //by the ActiveX Document Container 
    //Workaround described there doesn't work sufficiently.
    //Microsoft states this is by design, so I stop investigation here.
}

void CLATEDView::OnRClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
    CPoint curPoint,client;
    CMenu menu;
    HTREEITEM hItem;
    DWORD dwData;
    bool bMenu = false;
    CTreeCtrl& rTree = GetTreeCtrl();

    *pResult = 0;
    // make sure frame window is active
    GetCursorPos(&curPoint);
    client = curPoint;
    ScreenToClient(&client);
    hItem = rTree.HitTest(client);

    if(!hItem) {
        return;
    }
    // select node if we hit any:
    if (rTree.GetSelectedItem() != hItem) {
        // select as current:
        rTree.SelectItem(hItem);
    }

    dwData = rTree.GetItemData(hItem);

    switch(dwData) {
        case 1UL:
            if(menu.LoadMenu(IDR_POPUP_LINK_GROUP)) {
                bMenu = true;
            }
            break;
        case 2UL:
            if(menu.LoadMenu(IDR_POPUP_DATA_GROUP)) {
                bMenu = true;
            }
            break;
        case 0UL:
            return;
        default:
            {
                CLCBase* pBase = GetNode(hItem);
                if(!pBase) {
                    break;
                }

                UINT uiId = pBase->GetContextMenu();
                if(uiId == 0) {
                    break;
                }

                if(menu.LoadMenu(uiId)) {
                    bMenu = true;
                }
            }            
            break;
    }

    // show popup menu
    if(bMenu) {
        CMenu* pMenu = menu.GetSubMenu(0);
        ASSERT(pMenu);
        if(pMenu) {
            pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,curPoint.x,curPoint.y,this);
        }
    }
}

void CLATEDView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//Add your control notification handler code here
    CLCBase* pBase = NULL;
    CPoint curPoint,client;
    HTREEITEM hItem;

    CTreeCtrl& rTree = GetTreeCtrl();
    *pResult = 0;
    // make sure frame window is active
    GetCursorPos(&curPoint);
    client = curPoint;
    ScreenToClient(&client);
    hItem = rTree.HitTest(client);

    if(!hItem) {
        return;
    }
    // select node if we hit any:
    if (rTree.GetSelectedItem() != hItem) {
        // select as current:
        rTree.SelectItem(hItem);
    }
	
    pBase = GetNode(hItem);
    if(!pBase) {
        return;
    }

    OnProperty();
}


void CLATEDView::OnAddDataType() 
{
    unsigned short unID = 0;
    CLCConfig* pConfig = NULL;
    CLCDataType * pDataType = NULL;

    pConfig = GetConfig();
    if(!pConfig) {
        return;
    }

    unID = pConfig->AllocDataID();
    if(!unID) {
        CString str;
        //Message for allocation failure
        str.Format(IDS_NUM_DATATYPE_EXCEED,LC_LAST_DATA_ID-LC_FIRST_DATA_ID+1);
        ::AfxMessageBox(str,MB_ICONWARNING);
        return;
    }

    pDataType = new CLCDataType(unID);
    if(pDataType == NULL) {
        AfxThrowMemoryException();
    }

    CDialogDataType dialog(pConfig,pDataType,IDS_INS_DATATYPE,
        this, 0, false,IDS_PROPDECL_COLHEAD_2);

	switch(dialog.DoModal()) {
	    case IDOK:
            pConfig->AddDatatype(pDataType);
            SetModified();
            break;
        case IDCANCEL:
        default:
            pDataType->Release();
            break;

    }
}


void CLATEDView::OnProperty() 
{
    HTREEITEM hItem;
    CLCBase* pBase = NULL;
    CLCDataType * pType = NULL;
    CLCLink* pLink = NULL;
    CLCDestLink* pDestLink = NULL;
    CLCMember* pMember = NULL;

    CTreeCtrl& rTree = GetTreeCtrl();

    hItem = rTree.GetSelectedItem();
    if(hItem == NULL) {
        return;
    }

    pBase = GetNode(hItem);
    if(!pBase) {
        return;
    }

    pType = dynamic_cast<CLCDataType*>(pBase);
    if(pType) {
        //delete a datatype;
        OnProperty(pType);
        InvalidateItem(hItem);
        return;
    }

    pLink = dynamic_cast<CLCLink*>(pBase);
    if(pLink) {
        OnProperty(pLink);
        InvalidateItem(hItem);
        return;
    }

    pDestLink = dynamic_cast<CLCDestLink*>(pBase);
    if(pDestLink) {
        HTREEITEM hParent = rTree.GetParentItem(hItem);
        if(hParent == NULL) {
            return;
        }

        pBase = GetNode(hParent);
        if(!pBase) {
            return;
        }
        
        pLink = dynamic_cast<CLCLink*>(pBase);
        if(pLink) {
            OnProperty(pLink,pDestLink);
            InvalidateItem(hItem);
        }

        return;
    }

    pMember = dynamic_cast<CLCMember*>(pBase);
    if(pMember) {
        HTREEITEM hParent = rTree.GetParentItem(hItem);
        if(hParent == NULL) {
            return;
        }

        pBase = GetNode(hParent);
        if(!pBase) {
            return;
        }

        pType = dynamic_cast<CLCDataType*>(pBase);
        if(pType) {
            //delete a datatype;
            OnProperty(pType);
            InvalidateItem(hParent);
            return;
        }
    }
}

void CLATEDView::OnUpdateProp(CCmdUI* pCmdUI) 
{
    HTREEITEM hItem;
    CLCBase* pBase = NULL;
    CLCMember* pMember = NULL;

    CTreeCtrl& rTree = GetTreeCtrl();

    hItem = rTree.GetSelectedItem();
    if(hItem == NULL) {
        pCmdUI->Enable(FALSE);
        return;
    }

    pBase = GetNode(hItem);
    if(!pBase) {
        pCmdUI->Enable(FALSE);
        return;
    }

    //everything has properties except for the members of datatypes.
    pMember = dynamic_cast<CLCMember*>(pBase);
    if(pMember) {
        pCmdUI->Enable(FALSE);
        return;
    }
    pCmdUI->Enable(TRUE);
}

void CLATEDView::OnProperty(CLCDataType *pDataType)
{
    ASSERT(pDataType);
    CLCConfig* pConfig = NULL;
    CLCDataType* pCopy = NULL;

    ASSERT(pDataType);

    pConfig = GetConfig();
    if(!pConfig) {
        return;
    }

    //make a copy
    pCopy = new CLCDataType();
    if(pCopy == NULL) {
        AfxThrowMemoryException();
    }

    //copy the values.
    *pCopy = *pDataType;


    //now work on copy with property dialog
    CDialogDataType dialog(pConfig,pCopy,IDS_MOD_DATATYPE,
        this, 0, true ,IDS_PROPDECL_COLHEAD_2);

	switch(dialog.DoModal()) {
	    case IDOK:
            //tell listeners to remove all members.
            pConfig->TraverseDatatype(RemoveMembers,pDataType);
            //copy data back.
            *pDataType = *pCopy;
            pConfig->TraverseDatatype(TraverseMembers,pDataType);
            pCopy->Release();
            SetModified();
            break;
        case IDCANCEL:
            //release the copy
            pCopy->Release();
        default:
            break;

    }
}

void CLATEDView::OnProperty(CLCLink* pLink)
{
    CLCConfig* pConfig = NULL;

    pConfig = GetConfig();
    if(!pConfig) {
        return;
    }

    {   //do this in a block due to hourglass
        CHourglass glass;
        pConfig->ActualizeLinks();
    }
    CDialogTarget dialog(pConfig,pLink,NULL,true,IDS_PROP_LINK,this,0);
	switch(dialog.DoModal()) {
	    case IDOK:
            SetModified();
            break;
        case IDCANCEL:
        default:
            break;
    }
}

void CLATEDView::OnProperty(CLCLink* pLink, CLCDestLink* pDestLink)
{
    CLCConfig* pConfig = NULL;

    pConfig = GetConfig();
    if(!pConfig) {
        return;
    }

    {   //do this in a block due to hourglass
        CHourglass glass;
        pConfig->ActualizeLinks();
    }
    CDialogTarget dialog(pConfig,pLink,pDestLink,true,IDS_PROP_DESTLINK,this,0);
	switch(dialog.DoModal()) {
	    case IDOK:
            SetModified();
            break;
        case IDCANCEL:
        default:
            break;
    }
}

void CLATEDView::OnDelete() 
{
    HTREEITEM hItem;
    CLCBase* pBase = NULL;
    CLCDataType * pType = NULL;
    CLCLink* pLink = NULL;
    CLCDestLink* pDestLink = NULL;
    CLCConfig* pConfig = NULL;

    CTreeCtrl& rTree = GetTreeCtrl();

    hItem = rTree.GetSelectedItem();
    if(hItem == NULL) 
    {
        return;
    }

    pBase = GetNode(hItem);
    if(!pBase) {
        return;
    }

    pConfig = GetConfig();
    if(!pConfig) {
        return;
    }

    pType = dynamic_cast<CLCDataType*>(pBase);
    if(pType) {
        //delete a datatype;
        pConfig->DeleteDatatype(pType);
        SetModified();
        return;
    }

    pLink = dynamic_cast<CLCLink*>(pBase);
    if(pLink) {
        pConfig->DeleteLink(pLink);
        SetModified();
        return;
    }

    pDestLink = dynamic_cast<CLCDestLink*>(pBase);
    if(pDestLink) {
        HTREEITEM hParent = rTree.GetParentItem(hItem);
        if(hParent == NULL) {
            return;
        }

        pBase = GetNode(hParent);
        if(!pBase) {
            return;
        }
        
        pLink = dynamic_cast<CLCLink*>(pBase);
        if(pLink) {
            pConfig->DeleteDestLink(pLink,pDestLink);
            SetModified();
        }

    }
    return;
}

void CLATEDView::OnUpdateDelete(CCmdUI* pCmdUI) 
{
    HTREEITEM hItem;
    CLCBase* pBase = NULL;

    CTreeCtrl& rTree = GetTreeCtrl();

    hItem = rTree.GetSelectedItem();
    if(hItem == NULL) {
        pCmdUI->Enable(FALSE);
        return;
    }

    pBase = GetNode(hItem);
    if(!pBase) {
        pCmdUI->Enable(FALSE);
    }
    else {
        pCmdUI->Enable(TRUE);
    }
}


void CLATEDView::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
    HTREEITEM hItem;
    CLCConfig* pConfig = NULL;
    CLCBase* pBase = NULL;
    CLCDataType* pType = NULL;

	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
    ASSERT(pTVDispInfo);
	*pResult = FALSE;

    if(!pTVDispInfo->item.pszText) {
        return;
    }

    hItem = pTVDispInfo->item.hItem;
    if(!hItem) {
        return;
    }

    pConfig = GetConfig();
    if(!pConfig) {
        return;
    }

    pBase = GetNode(hItem);
    if(!pBase) {
        return;
    }


    pType = dynamic_cast<CLCDataType*>(pBase);
    if(pType) {
        //rename a datatype;
        CString str = pTVDispInfo->item.pszText;
        CLCDataType* p = pConfig->GetDatatype(str);
        if(p && p != pType) { //another datatype has same name
            FC_RELEASE_PTR(p);    
            return;
        }

        FC_RELEASE_PTR(p);
        pConfig->RenameDatatype(str,pType);
        SetModified();
        *pResult = TRUE;
        return;
    }
}

void CLATEDView::OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
    ASSERT(pTVDispInfo);
    
    *pResult = TRUE;

    HTREEITEM hItem = pTVDispInfo->item.hItem;
    if(!hItem) {
        return;
    }

    CLCBase* pBase = GetNode(hItem);
    if(!pBase) {
        return;
    }

    if(pBase->CanLabelEdit()) {
        *pResult = FALSE;
    }
	
}

void CLATEDView::OnRename() 
{
    CTreeCtrl& rTree = GetTreeCtrl();

    HTREEITEM hItem = rTree.GetSelectedItem();
    if(hItem == NULL) {
        return;
    }

	CEdit* pEdit = rTree.EditLabel(hItem);
    TRACE("Edit hwnd: %x\n", pEdit->GetSafeHwnd());
}


void CLATEDView::OnAddlink() 
{
    CLCConfig* pConfig = NULL;
    CLCLink* pLink = NULL;

    pConfig = GetConfig();
    if(!pConfig) {
        return;
    }

    {   //do this in a block due to hourglass
        CHourglass glass;

        pConfig->ActualizeLinks();
    
        pLink = new CLCLink();
        if(!pLink) {
            AfxThrowMemoryException();        
        }
    }

    CDialogTarget dialog(pConfig,pLink,NULL,false,IDS_INSERT_LINK, this,0);
	switch(dialog.DoModal()) {
	    case IDOK:
            pConfig->AddLink(pLink);
            SetModified();
            break;
        case IDCANCEL:
        default:
            pLink->Release();
            break;
    }
}

void CLATEDView::OnAddDestLink() 
{
    HTREEITEM hItem;
    CLCBase* pBase = NULL;
    CLCLink* pLink = NULL;
    CLCDestLink* pDest = NULL;
    CLCConfig* pConfig = NULL;

    pConfig = GetConfig();
    if(!pConfig) {
        return;
    }

    CTreeCtrl& rTree = GetTreeCtrl();
    hItem = rTree.GetSelectedItem();
    if(hItem == NULL) {
        return;
    }

    {   //do this in a block due to hourglass
        CHourglass glass;

        pConfig->ActualizeLinks();

        pBase = GetNode(hItem);
        if(!pBase) {
            return;
        }

        pLink = dynamic_cast<CLCLink*>(pBase);
        if(!pLink) {
            return;
        }

        pDest = new CLCDestLink();
        if(!pDest) {
            AfxThrowMemoryException();
        }

    }

    CDialogTarget dialog(pConfig,pLink,pDest,false,IDS_INSERT_DESTLINK, this,0);
	switch(dialog.DoModal()) {
	    case IDOK:
            pConfig->AddDestLink(pLink,pDest);
            SetModified();                                    
            break;
        case IDCANCEL:
        default:
            pDest->Release();
            break;
    }

}

void CLATEDView::OnAssignDatatype() 
{
    HTREEITEM hItem;
    CLCBase* pBase = NULL;
    CLCLink* pLink = NULL;
    CLCConfig* pConfig = NULL;

    pConfig = GetConfig();
    if(!pConfig) {
        return;
    }

    CTreeCtrl& rTree = GetTreeCtrl();
    hItem = rTree.GetSelectedItem();
    if(hItem == NULL) {
        return;
    }

    pBase = GetNode(hItem);
    if(!pBase) {
        return;
    }

    pLink = dynamic_cast<CLCLink*>(pBase);
    if(!pLink) {
        return;
    }

    CDlgAssignType dialog(pConfig,pLink,this);
	switch(dialog.DoModal()) {
	    case IDOK:
            SetModified();
            InvalidateItem(hItem);
            break;
        case IDCANCEL:
        default:
            break;
    }

}

void CLATEDView::OnUpdateAssignDatatype(CCmdUI* pCmdUI) 
{
    HTREEITEM hItem;
    CLCBase* pBase = NULL;
    CLCLink* pLink = NULL;

    CTreeCtrl& rTree = GetTreeCtrl();
    hItem = rTree.GetSelectedItem();
    if(hItem == NULL) {
        pCmdUI->Enable(FALSE);
        return;
    }

    pBase = GetNode(hItem);
    if(!pBase) {
        pCmdUI->Enable(FALSE);
        return;
    }

    pLink = dynamic_cast<CLCLink*>(pBase);
    if(!pLink) {
        pCmdUI->Enable(FALSE);
        return;
    }


    pCmdUI->Enable(TRUE);
}

BOOL CLATEDView::PreTranslateMessage(MSG* pMsg) 
{
    if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
    {
        CTreeCtrl& rTree = GetTreeCtrl();
        CEdit* pEdit = rTree.GetEditControl();

        if (!pEdit && m_hLatAccel != NULL && ::TranslateAccelerator (m_hWnd, m_hLatAccel, pMsg))
        {
            return (TRUE);
        }
    }

	return CTreeView::PreTranslateMessage(pMsg);
}


int CLATEDView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
    CTreeCtrl& rTree = GetTreeCtrl();
    CEdit* pEdit = rTree.GetEditControl();

    if(pEdit)
    {
        return MA_NOACTIVATE;
    }

    return CTreeView::OnMouseActivate(pDesktopWnd, nHitTest, message);
}