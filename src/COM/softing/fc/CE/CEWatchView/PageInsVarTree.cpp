

#include "stdafx.h"
#include "CeWatchView.h"
#include "CeWatchList.h"
#include "CeWatchSheetInsVar.h"
#include "CeSymbol.h"

#include "PageInsVarTree.h"

// #2149 27.10.04 SIS >>
#include "ExpandArrayDlg.h"
// #2149 27.10.04 SIS <<

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageInsVarTree property page

IMPLEMENT_DYNCREATE(CPageInsVarTree, CPropertyPage)


CPageInsVarTree::CPageInsVarTree(
	CCeWatchList* pWatchList/*=NULL*/, 
	CCeWatchSheetInsVar* pParent/*=NULL*/, 
	CCeSymbol* pSymbolTable/*=NULL*/) : 
		CPropertyPage(CPageInsVarTree::IDD)
{
	//{{AFX_DATA_INIT(CPageInsVarTree)
	//}}AFX_DATA_INIT

	m_pParent = pParent;
	m_pWatchList = pWatchList;
	m_pSymbolTable = pSymbolTable;
}


CPageInsVarTree::~CPageInsVarTree()
{
}

void CPageInsVarTree::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageInsVarTree)
	DDX_Control(pDX, IDC_TREE_VARNAME, m_VarTreeCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageInsVarTree, CPropertyPage)
	//{{AFX_MSG_MAP(CPageInsVarTree)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_VARNAME, OnSelChanged)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_VARNAME, OnDblClick)
	// #2149 27.10.04 SIS >>
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_TREE_VARNAME, OnItemExpanding)	
	// #2149 27.10.04 SIS <<
	ON_NOTIFY(TVN_KEYDOWN, IDC_TREE_VARNAME, OnKeyDown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageInsVarTree message handlers

BOOL CPageInsVarTree::OnInitDialog() 
{
	CWaitCursor wait;   // display wait cursor

	CPropertyPage::OnInitDialog();
	
	m_ImageList.Create (IDB_OBJECT_TYPES, 20, 0, RGB (255, 0, 255));
	m_ImageList.SetBkColor (GetSysColor(COLOR_WINDOW));
	m_VarTreeCtrl.SetImageList (&m_ImageList, TVSIL_NORMAL);

	InsertItems(NULL, m_pSymbolTable);
	// #2149 27.10.04 SIS >>
	ExpandTree(NULL, TRUE);     // 23.08.06 SIS
	// #2149 27.10.04 SIS <<

    UpdateData (FALSE);
	m_pParent->SetModified(m_VarTreeCtrl.GetSelectedItem() != NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// #2149 27.10.04 SIS
bool CPageInsVarTree::InsertTreeItem(HTREEITEM hParentItem, CCeSymbol* pVar) 
{
	bool bReturn = false;
	if (pVar != NULL)
	{
        if (CCeWatchElement::IsVarKindVisible(pVar->GetType().GetVarKind()))
		{
			CString strItemText = pVar->GetName() + ": " + pVar->GetType().GetName();
			unsigned iIconIndex = CalcIconIndex(pVar);
			HTREEITEM hItem = m_VarTreeCtrl.InsertItem(TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM, strItemText, iIconIndex, iIconIndex, 0, 0, (LPARAM)pVar, hParentItem, NULL);

			TVITEM	tvi;
			memset(&tvi, 0, sizeof(TVITEM));
			tvi.mask = TVIF_CHILDREN;
			tvi.hItem = hItem;
			m_VarTreeCtrl.GetItem(&tvi);
			tvi.cChildren = (pVar->GetChildrenCount() > 0) ? 1 : 0;
			m_VarTreeCtrl.SetItem(&tvi);
			
			bReturn = true;
		}
	}
	return bReturn;
}

bool CPageInsVarTree::InsertItems(HTREEITEM hParentItem, CCeSymbol* pVar) 
{
	if (pVar != NULL)
	{
        if (CCeWatchElement::IsVarKindVisible(pVar->GetType().GetVarKind()))
		{
			CString strItemText = pVar->GetName() + ": " + pVar->GetType().GetName();
			unsigned iIconIndex = CalcIconIndex(pVar);
			HTREEITEM hItem = m_VarTreeCtrl.InsertItem(strItemText, iIconIndex, iIconIndex, hParentItem);
			m_VarTreeCtrl.SetItemData(hItem, (DWORD)pVar);	
			pVar->SetExpandFlag(TRUE);
            
			int iCount = pVar->GetChildrenCount();
			CCeSymbol* pChildVar = NULL;
			for (int ii = 0; ii < iCount; ii++)
			{
				pChildVar = pVar->GetChild(ii);
				ASSERT(pChildVar != NULL);
                if (pChildVar!=NULL)
				{
        			InsertTreeItem(hItem, pChildVar);
				}
				else
                {
                    m_VarTreeCtrl.SetItemImage(hItem, 11, 11);
	                CString strMessage;
	                strMessage.FormatMessage(IDS_ERROR_DBI_NOT_FOUND, pVar->GetFullName());
                    ::AfxMessageBox(strMessage, MB_ICONSTOP);
                    break;
                }
			}
		}
	}
	else
	{
		HTREEITEM hItem = m_VarTreeCtrl.InsertItem("test", 0, 0, hParentItem);
		m_VarTreeCtrl.SetItemData(hItem, (DWORD)NULL);	
	}
	return true; 
}

// #2149 27.10.04 SIS >>
bool CPageInsVarTree::ExpandTree(HTREEITEM hParentItem, BOOL bOnlyFirstChild /*=FALSE*/)    // 23.08.06 SIS
{
	if(hParentItem == NULL)
	{
		hParentItem = m_VarTreeCtrl.GetRootItem();
	}

	CCeSymbol*	pVar;

	pVar = (CCeSymbol*) m_VarTreeCtrl.GetItemData(hParentItem);
	if(pVar && pVar->GetType().GetBaseType() <= CEWATCH_BASETYPE_RESOURCE)
	{
		m_VarTreeCtrl.Expand(hParentItem, TVE_EXPAND);
	}
	else
	{
		return false;
	}

	HTREEITEM	hChild = m_VarTreeCtrl.GetChildItem(hParentItem);

	while(hChild)
	{
		ExpandTree(hChild, bOnlyFirstChild);    // 23.08.06 SIS
		hChild = m_VarTreeCtrl.GetNextSiblingItem(hChild);
        // 23.08.06 SIS >>
        if(bOnlyFirstChild)
        {
            break;
        }
        // 23.08.06 SIS <<
	}
	return true; 
}

void CPageInsVarTree::OnItemExpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	*pResult = ExpandItem(pNMTreeView->itemNew.hItem);	// 14.12.05 SIS
}
// #2149 27.10.04 SIS <<


// 14.12.05 SIS >>
// changed return type to long
// return information about expanding
// values:
//    0: expansion may take place
//    1: do not expand
// this value is passed as pResult in OnItemExpanding() (see above)
LONG CPageInsVarTree::ExpandItem(HTREEITEM hParentItem) 
// 14.12.05 SIS <<
{
	CWaitCursor wait;   // display wait cursor
	CCeSymbol* pVarParent = (CCeSymbol*)m_VarTreeCtrl.GetItemData(hParentItem);
	if(!pVarParent->GetExpandFlag())
	{
		ASSERT(pVarParent != NULL && pVarParent->GetChildrenCount() > 0);
		if (pVarParent != NULL && pVarParent->GetChildrenCount() > 0)
		{
			int iCount = pVarParent->GetChildrenCount();
			int iStart = 0;
			// #2149 26.10.04 SIS >>
			if(iCount > 100 && pVarParent->GetType().IsArray())	// #179 27.01.05 SIS: only for array type
			{
				CExpandArrayDlg tDlg(0, iCount);
				if(tDlg.DoModal() == IDOK)
				{
					iStart = tDlg.m_iOffset;
					iCount = iStart + tDlg.m_iCount;
				}
				else
				{
					// 14.12.05 SIS >>
					return 1;	// cancel -> do not expand
					// 14.12.05 SIS <<
				}
			}
			for (int ii = iStart; ii < iCount; ii++)
			{
				CCeSymbol* pVar = pVarParent->GetChild(ii);
				InsertTreeItem(hParentItem, pVar);
			}
			pVarParent->SetExpandFlag(TRUE);
			// #2149 26.10.04 SIS <<
		}
	}
	m_VarTreeCtrl.EnsureVisible(hParentItem);
	// 14.12.05 SIS >>
	return 0;	// ok -> expand
	// 14.12.05 SIS <<
}


HTREEITEM CPageInsVarTree::GetItem(HTREEITEM hParentItem, const CString& name) 
{
	if (m_VarTreeCtrl.GetCount() == 0)
		return false;

	HTREEITEM hChildItem = m_VarTreeCtrl.GetChildItem(hParentItem);
	while (hChildItem != NULL)
	{
		if (m_VarTreeCtrl.GetItemText(hChildItem) == name)
			return hChildItem;
		hChildItem = m_VarTreeCtrl.GetNextSiblingItem(hChildItem);
	}
	return NULL;
}


void CPageInsVarTree::Add() 
{
	HTREEITEM hItem = m_VarTreeCtrl.GetSelectedItem();
	if (hItem != NULL)
	{
		CWaitCursor wait;   // display wait cursor
		CCeSymbol* pVar = (CCeSymbol*)m_VarTreeCtrl.GetItemData(hItem);
		if (pVar != NULL)
		{
			m_pWatchList->InsertVariable(pVar);
			m_pWatchList->UpdateConnections();	
		}
	}
}

bool CPageInsVarTree::CanAdd() 
{
	return m_VarTreeCtrl.GetSelectedItem() != NULL;
}

void CPageInsVarTree::OnSelChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
//	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	m_pParent->SetModified(CanAdd());
	*pResult = 0;
}

void CPageInsVarTree::OnDblClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	Add();
	*pResult = 1;
}

BOOL CPageInsVarTree::OnApply() 
{
	if (m_pParent->GetActivePage() == this)
		Add();	

	return CPropertyPage::OnApply();
}

BOOL CPageInsVarTree::OnSetActive() 
{
	m_pParent->SetModified(CanAdd());	
	return CPropertyPage::OnSetActive();
}


unsigned CPageInsVarTree::CalcIconIndex(CCeSymbol* pVar)
{
	unsigned iBaseType = pVar->GetType().GetBaseType();
	switch (iBaseType)
	{
	case CEWATCH_BASETYPE_PROJECT        :
	case CEWATCH_BASETYPE_CONFIGURATION  :
	case CEWATCH_BASETYPE_RESOURCE       :
	case CEWATCH_BASETYPE_TASK           :
	case CEWATCH_BASETYPE_PROGRAM        :
	case CEWATCH_BASETYPE_STEP        :
	case CEWATCH_BASETYPE_ACTION        :
	case CEWATCH_BASETYPE_TRANSITION        :
	case CEWATCH_BASETYPE_FUNCTIONBLOCK  :
		return iBaseType;
		break;

	case CEWATCH_BASETYPE_ARRAY          :
	case CEWATCH_BASETYPE_STRUCTURE      :
	case CEWATCH_BASETYPE_SIMPLE         :
		if (pVar->GetParent()->GetType().IsArray())
			return 10;
		else
			return 9;
		break;

	default:
		return 11;
		break;
	}
}



void CPageInsVarTree::OnKeyDown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	TV_KEYDOWN* pTVKeyDown = (TV_KEYDOWN*)pNMHDR;
	HTREEITEM hItem = m_VarTreeCtrl.GetSelectedItem();
	if (hItem != NULL)
	{
		char c = (char)MapVirtualKey(pTVKeyDown->wVKey, 2);
		if (c == '+')
			*pResult = m_VarTreeCtrl.Expand(hItem, TVE_EXPAND) ? 1 : 0;
		else if (c == '-')
			*pResult = m_VarTreeCtrl.Expand(hItem, TVE_COLLAPSE) ? 1 : 0;
	}
}

